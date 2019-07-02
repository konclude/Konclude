/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CQtHttpTransactionManager.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			CQtHttpTransactionManager::CQtHttpTransactionManager(cint64 timeoutInterval) : CIntervalThread("HttpTransactionManagerThread") {
				mQNAM = nullptr;
				startThread();
				mTimeoutInterval = timeoutInterval;
			}

			CQtHttpTransactionManager::~CQtHttpTransactionManager() {
			}


			CHttpRequest* CQtHttpTransactionManager::createRequest(const QString& url) {
				CQtHttpRequest* httpRequest = new CQtHttpRequest(url);
				return httpRequest;
			}



			CHttpRequest* CQtHttpTransactionManager::createRequest(const QString& url, const QByteArray& byteArray) {
				CQtHttpRequest* httpRequest = new CQtHttpRequest(url,new QByteArray(byteArray));
				return httpRequest;
			}


			bool CQtHttpTransactionManager::releaseResponse(CHttpResponse* response, bool releaseAlsoRequest) {
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					CReleaseResponseEvent* responseEvent = new CReleaseResponseEvent(qtResponse,releaseAlsoRequest);
					postEvent(responseEvent);
					return true;
				}
				return false;
			}


			CHttpResponse* CQtHttpTransactionManager::getResponse(CHttpRequest* request) {
				if (request) {
					CQtHttpRequest* qtRequest = dynamic_cast<CQtHttpRequest*>(request);
					return getResponse(qtRequest);
				}
				return nullptr;
			}

			CQtHttpResponse* CQtHttpTransactionManager::getResponse(CQtHttpRequest* request) {
				if (request) {
					CQtHttpResponse* response = new CQtHttpResponse(request);
					postEvent(new CHttpRequstEvent(request,response));
					return response;
				}
				return nullptr;
			}

			void CQtHttpTransactionManager::finishedNetworkReply(QNetworkReply* reply) {
				QNetworkReply::NetworkError error = reply->error();
				if (error) {
					QString errorString = reply->errorString();
					LOG(WARNING,"::Konclude::Network::HTTP::QtHttpTransactionManager",logTr("HTTP-Error: %1").arg(errorString),this);
				}
				postEvent(new CHttpReplyFinishedEvent(reply));
			}

			QString* CQtHttpTransactionManager::getExtractedText(CHttpResponse *response) {
				QString* text = nullptr;
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					text = new QString();
					CBlockingCallbackData blockCallback;
					postEvent(new CExtractResponseTextEvent(qtResponse,text,&blockCallback));
					blockCallback.waitForCallback();
				}
				return text;
			}


			bool CQtHttpTransactionManager::hasFinishedSucecssful(CHttpResponse* response) {
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					QNetworkReply* reply = qtResponse->getQNetworkReply();
					if (reply) {
						//QString errorString = reply->errorString();
						if (reply->isFinished() && reply->error() == QNetworkReply::NoError) {
							return true;
						}
					}
				}
				return false;
			}

			bool CQtHttpTransactionManager::callbackFinishedRequest(CHttpResponse* response, CCallbackData* callbackData) {
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					postEvent(new CInstallRequestFinishedCallbackEvent(qtResponse,callbackData));
					return true;
				}
				return false;
			}

			void CQtHttpTransactionManager::threadStarted() {
				CThread::threadStarted();
				mQNAM = new QNetworkAccessManager();
				connect(mQNAM,SIGNAL(finished(QNetworkReply*)),this,SLOT(finishedNetworkReply(QNetworkReply*)));
				// default is 5 minutes timeout
				startTimerWithInterval(TIMEOUTCHECKTIMER,mTimeoutInterval);
			}

			void CQtHttpTransactionManager::threadStopped() {
				CThread::threadStopped();
				disconnect(mQNAM);
				delete mQNAM;
			}


			bool CQtHttpTransactionManager::processTimer(qint64 timerID) {
				if (timerID == TIMEOUTCHECKTIMER) {
					// timeouts
					foreach (CQtHttpResponse* response, mCriticalTimeoutSet) {
						response->getFinishedCallbackList()->doCallback();
						mTimeoutSet.insert(response);
						mReplyResponseHash.remove(response->getQNetworkReply());
					}
					mCriticalTimeoutSet.clear();
					mCriticalTimeoutSet = mNextTimeoutSet;
					mNextTimeoutSet.clear();
					return true;
				}
				return false;
			}



			bool CQtHttpTransactionManager::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CHttpRequstEvent::EVENTTYPE) {
					CHttpRequstEvent* hre = (CHttpRequstEvent*)event;
					CQtHttpRequest* request = hre->getRequest();
					QByteArray* byteArray = request->getByteArrayData();
					CQtHttpResponse* response = hre->getResponse();

					QNetworkReply* reply = nullptr;
					if (!byteArray) {
						reply = mQNAM->get(*request->getQNetworkRequest());
					} else {
						reply = mQNAM->post(*request->getQNetworkRequest(),*byteArray);
					}
					mReplyResponseHash.insert(reply,response);
					mNextTimeoutSet.insert(response);

					response->setQNetworkReply(reply);
					return true;
				} else if (type == CHttpReplyFinishedEvent::EVENTTYPE) {
					CHttpReplyFinishedEvent* hrfe = (CHttpReplyFinishedEvent*)event;
					QNetworkReply* reply = hrfe->getReply();
					CQtHttpResponse* response = mReplyResponseHash.value(reply);
					bool tryAgain = false;
					if (response) {
						//if (reply->error() == QNetworkReply::ProtocolUnknownError) {
						//	tryAgain = true;
						//}
						if (tryAgain) {
							CQtHttpRequest* request = response->getHttpRequest();
							QByteArray* byteArray = request->getByteArrayData();

							QNetworkReply* reply = nullptr;
							if (!byteArray) {
								reply = mQNAM->get(*request->getQNetworkRequest());
							} else {
								reply = mQNAM->post(*request->getQNetworkRequest(),*byteArray);
							}
							mReplyResponseHash.insert(reply,response);
							response->setQNetworkReply(reply);
						} else {
							response->getFinishedCallbackList()->doCallback();
							mCriticalTimeoutSet.remove(response);
							mNextTimeoutSet.remove(response);
							mReplyResponseHash.remove(reply);
						}
					}
					return true;
				} else if (type == CExtractResponseTextEvent::EVENTTYPE) {
					CExtractResponseTextEvent* erte = (CExtractResponseTextEvent*)event;
					CQtHttpResponse* response = erte->getResponse();
					CExtractResponseTextCallbackEvent* ertce = new CExtractResponseTextCallbackEvent(this,response,erte->getText(),erte->getCallbacks());
					response->getFinishedCallbackList()->addCallbacks(ertce);
					return true;
				} else if (type == CInstallRequestFinishedCallbackEvent::EVENTTYPE) {
					CInstallRequestFinishedCallbackEvent* erte = (CInstallRequestFinishedCallbackEvent*)event;
					CQtHttpResponse* response = erte->getResponse();
					response->getFinishedCallbackList()->addCallbacks(erte->getCallbacks());
					return true;
				} else if (type == CExtractResponseTextCallbackEvent::EVENTTYPE) {
					CExtractResponseTextCallbackEvent* ertce = (CExtractResponseTextCallbackEvent*)event;
					CQtHttpResponse* response = ertce->getResponse();
					QString* testString = ertce->getText();
					if (response && testString && hasFinishedSucecssful(response)) {
						QNetworkReply* reply = response->getQNetworkReply();
						QString respText(reply->readAll());
						*testString = respText;
					}
					CCallbackData* callbackIt = ertce->getCallbacks();
					while (callbackIt) {
						CCallbackData* callback = callbackIt;
						callbackIt = callbackIt->getNext();
						callback->doCallback();
					}
					return true;
				} else if (type == CReleaseResponseEvent::EVENTTYPE) {
					CReleaseResponseEvent* rre = (CReleaseResponseEvent*)event;
					bool releaseAlsoRequest = rre->getReleaseAccordingRequest();
					CQtHttpResponse* response = (CQtHttpResponse*)rre->getResponse();
					response->getQNetworkReply()->deleteLater();
					mReplyResponseHash.remove(response->getQNetworkReply());
					mCriticalTimeoutSet.remove(response);
					mNextTimeoutSet.remove(response);
					if (releaseAlsoRequest) {
						delete response->getHttpRequest();
					}
					delete response;
					return true;
				}
				return false;
			}



		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

