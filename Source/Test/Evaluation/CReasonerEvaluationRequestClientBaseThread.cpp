/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CReasonerEvaluationRequestClientBaseThread.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationRequestClientBaseThread::CReasonerEvaluationRequestClientBaseThread(const QString& threadName, const QString& logIdentifier) : CIntervalThread(threadName),CLogIdentifier(logIdentifier,this) {

				startThread();


			}


			CReasonerEvaluationRequestClientBaseThread::~CReasonerEvaluationRequestClientBaseThread() {
				stopThread();
			}


			bool CReasonerEvaluationRequestClientBaseThread::evaluateReasoner(const QString& initFileString, const QString& testFileString, const QString& addressString, CConfiguration* configuration, CCallbackData* callback) {
				postEvent(new CReasonerEvaluationStartEvent(initFileString,testFileString,addressString,configuration,callback));
				return true;
			}

			CReasonerEvaluationRequestResult* CReasonerEvaluationRequestClientBaseThread::getReasonerEvaluationResult() {
				return mResponse;
			}


			bool CReasonerEvaluationRequestClientBaseThread::processTimer(qint64 timerID) {
				if (timerID == TIMERIDREQUESTTIMEOUT || timerID == TIMERIDTOTALTIMEOUT) {
					++mCurrentOperationNumber;
					closeAllRequestTimeout();
					return true;
				}
				return false;
			}


			void CReasonerEvaluationRequestClientBaseThread::threadStarted() {
				CIntervalThread::threadStarted();
				mCurrentOperationNumber = 0;
				mModifyRequestAbsolutePaths = true;
				mReplaceLoadOntologiesWithTells = false;
				mResolveAppreviatedIRIsForReplacedTells = false;
				mDownloadSizeLimit = -1;
				mResponse = nullptr;
				mTransManager = nullptr;
			}

			void CReasonerEvaluationRequestClientBaseThread::threadStopped() {
				CIntervalThread::threadStopped();
				delete mTransManager;
			}


			CReasonerEvaluationRequestClientBaseThread* CReasonerEvaluationRequestClientBaseThread::readConfig(CConfiguration* config) {
				mRequestTimeout = CConfigDataReader::readConfigInteger(config, "Konclude.Evaluation.RequestTimeout");
				mTotalTimeout = CConfigDataReader::readConfigInteger(config, "Konclude.Evaluation.TestingTimeout");

				mModifyRequestAbsolutePaths = CConfigDataReader::readConfigInteger(config, "Konclude.Evaluation.RequestFileAbsolutePathsModification");
				mReplaceLoadOntologiesWithTells = CConfigDataReader::readConfigInteger(config, "Konclude.Evaluation.RequestReplaceLoadOntologiesWithTell");
				mResolveAppreviatedIRIsForReplacedTells = CConfigDataReader::readConfigInteger(config, "Konclude.Evaluation.ResolveAppreviatedIRIsForReplacedTells");

				mDownloadSizeLimit = CConfigDataReader::readConfigInteger(config, "Konclude.Evaluation.ResponseDownloadSizeLimit");

				if (mTransManager) {
					delete mTransManager;
				}
				mTransManager = new CQtHttpTransactionManager(1000 * 60 * 60 * 24 * 14, mDownloadSizeLimit);

				return this;
			}


			bool CReasonerEvaluationRequestClientBaseThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (CIntervalThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CReasonerEvaluationStartEvent::EVENTTYPE) {
					CReasonerEvaluationStartEvent* rese = (CReasonerEvaluationStartEvent *)event;

					mResponse = new CReasonerEvaluationRequestResult();
					mInitFileString = rese->getInitFileString();
					mTestFileString = rese->getTestFileString();
					mAddressString = rese->getAddressString();
					mConfig = rese->getConfiguration();
					mRequestFinishedCallback = rese->getCallback();
					++mCurrentOperationNumber;

					readConfig(mConfig);

					loadReasonerRequests(mInitFileString,mTestFileString);

					mTotalTiming.start();
					startTimerWithIntervalLimited(TIMERIDTOTALTIMEOUT,mTotalTimeout,1);

					bool sendedNextRequest = false;
					while (!sendedNextRequest) {
						if (hasNextRequest()) {
							sendedNextRequest = sendNextRequest();
						} else {
							finishReasonerRequests();
							break;
						}
					}

					return this;
				
				} else if (type == CReasonerEvaluationNextEvent::EVENTTYPE) {
					CReasonerEvaluationNextEvent* rene = (CReasonerEvaluationNextEvent *)event;

					if (rene->getOperationNumber() == mCurrentOperationNumber) {
						closeNextRequest();

						bool sendedNextRequest = false;
						while (!sendedNextRequest) {
							if (hasNextRequest()) {
								sendedNextRequest = sendNextRequest();
							} else {
								finishReasonerRequests();
								break;
							}
						}
					}

					return this;
				}
				return false;
			}


			bool CReasonerEvaluationRequestClientBaseThread::finishReasonerRequests() {
				cint64 totalTime = mTotalTiming.elapsed();
				cint64 testingTime = mTestingTiming.elapsed();
				mResponse->setResponseTime(testingTime);
				stopTimer(TIMERIDREQUESTTIMEOUT);
				stopTimer(TIMERIDTOTALTIMEOUT);				
				LOG(INFO, getLogDomain(), logTr("Finished reasoner execution."), this);
				if (mRequestFinishedCallback) {
					mRequestFinishedCallback->doCallback();
					mRequestFinishedCallback = nullptr;
				}
				return true;
			}


			bool CReasonerEvaluationRequestClientBaseThread::closeAllRequestTimeout() {
				cint64 elapsedTime = mNextRequestTiming.elapsed();
				mNextReqRes->setResponseTime(elapsedTime);

				LOG(INFO,getLogDomain(),logTr("Request %1 of %2 timed out.").arg(1+mCurrentRequestCount++).arg(mTotalRequestCount),this);

				mNextReqRes->setTimedOut(true);
				mResponse->setResponsedInTime(false);
				mResponse->addResponse(mNextReqRes,mNumberInitRequests-- > 0);

				while (!mRemainingRequestList.isEmpty()) {
					CReasonerEvaluationRequestResponse* nextReqResp = mRemainingRequestList.takeFirst();
					nextReqResp->setResponseError(true);
					mResponse->addResponse(nextReqResp,mNumberInitRequests-- > 0);
				}

				finishReasonerRequests();
				return true;
			}




			bool CReasonerEvaluationRequestClientBaseThread::closeNextRequest() {
				stopTimer(TIMERIDREQUESTTIMEOUT);

				cint64 elapsedTime = mNextRequestTiming.elapsed();
				mNextReqRes->setResponseTime(elapsedTime);

				LOG(INFO, getLogDomain(), logTr("Received response %1 of %2 in %3 ms.").arg(1 + mCurrentRequestCount++).arg(mTotalRequestCount).arg(elapsedTime), this);
				bool responseError = false;


				QIODevice* device = nullptr;
				if (!mTransManager->hasFinishedSucecssfully(mWebResponse)) {
					CHttpTransactionManager::ABORT_REASON abortReason = CHttpTransactionManager::ABORT_NO_REASON;
					if (mTransManager->hasBeenAborted(mWebResponse, &abortReason)) {
						mNextReqRes->setEvaluationError(true);
						QString abortReasonString = "none given";
						if (abortReason == CHttpTransactionManager::ABORT_DOWNLOAD_SIZE_LIMIT_REACHED) {
							abortReasonString = QString("Response size limit of %1 bytes has been reached").arg(mDownloadSizeLimit);
						}
						mNextReqRes->setEvaluationErrorString(QString("Request aborted with reason: %1.").arg(abortReasonString));
						LOG(WARN, getLogDomain(), logTr("Request has been aborted with reason: %1.").arg(abortReasonString), this);
					} else {
						responseError = true;
						LOG(WARN, getLogDomain(), logTr("Response not successfully retrieved, interpreting it as error."), this);
					}
				} else {
					device = mTransManager->getResponseDataReadDevice(mWebResponse);
					if (device) {
						responseError = parseResponse(device);
					} else {
						responseError = true;
					}
				}
				

				mNextReqRes->setResponseError(responseError);
				if (!responseError) {
					mResponse->setSucessfullReasonerCommunication(true);
				}

				mResponse->addResponse(mNextReqRes, mNumberInitRequests-- > 0);
				mNextReqRes = nullptr;

				mTransManager->releaseResponse(mWebResponse);
				return true;
			}


			bool CReasonerEvaluationRequestClientBaseThread::hasNextRequest() {
				return !mRemainingRequestList.isEmpty();
			}
			


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
