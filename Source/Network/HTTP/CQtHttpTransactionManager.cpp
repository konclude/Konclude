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

#include "CQtHttpTransactionManager.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			CQtHttpTransactionManager::CQtHttpTransactionManager(cint64 timeoutInterval, cint64 downloadSizeLimit) : CIntervalThread("HttpTransactionManagerThread") {
				mQNAM = nullptr;
				startThread();
				mTimeoutInterval = timeoutInterval;
				mDownloadSizeLimit = downloadSizeLimit;
			}

			CQtHttpTransactionManager::~CQtHttpTransactionManager() {
			}


			CHttpRequest* CQtHttpTransactionManager::createRequest(const QString& url) {
				CQtHttpRequest* httpRequest = new CQtHttpRequest(url);
				return httpRequest;
			}

			//CHttpRequest* CQtHttpTransactionManager::createFileDownloadRequest(const QString& url, QIODevice* openWriteFile) {
			//	CQtHttpDownloadFileRequest* httpRequest = new CQtHttpDownloadFileRequest(url);
			//	return httpRequest;
			//}


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


			bool CQtHttpTransactionManager::abort(CHttpRequest* request, CHttpResponse* response, ABORT_REASON abortReason) {
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					CAbortRequestEvent* abortEvent = new CAbortRequestEvent(request, response, (cint64)abortReason);
					postEvent(abortEvent);
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


			bool CQtHttpTransactionManager::callbackFinishedRequest(CHttpResponse* response, CCallbackData* callbackData) {
				return callbackFinished(response,callbackData);
			}


			bool CQtHttpTransactionManager::callbackFinished(CHttpResponse* response, CCallbackData* callback) {
				if (response) {
					CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
					return callbackFinished(qtResponse,callback);
				}
				return false;
			}

			bool CQtHttpTransactionManager::callbackFinished(CQtHttpResponse* response, CCallbackData* callback) {
				postEvent(new CAddResponseFinishedCallbackEvent(response,callback));
				return true;
			}


			void CQtHttpTransactionManager::finishedNetworkReply(QNetworkReply* reply) {
				QNetworkReply::NetworkError error = reply->error();
				if (error) {
					QString errorString = reply->errorString();
					LOG(WARNING,"::Konclude::Network::HTTP::QtHttpTransactionManager",logTr("HTTP-Error: %1").arg(errorString),this);
				}
				postEvent(new CHttpReplyFinishedEvent(reply));
			}



			QIODevice* CQtHttpTransactionManager::getResponseDataReadDevice(CHttpResponse* response) {
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					return qtResponse->getQNetworkReply();
				}
				return nullptr;
			}

			QString* CQtHttpTransactionManager::getExtractedText(CHttpResponse *response) {
				QString* text = nullptr;
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					QByteArray data;
					CBlockingCallbackData blockCallback;
					postEvent(new CExtractResponseDataEvent(qtResponse,&data,&blockCallback));
					blockCallback.waitForCallback();
					text = new QString(data);
				}
				return text;
			}


			QByteArray* CQtHttpTransactionManager::getResponseData(CHttpResponse* response) {
				QByteArray* data = nullptr;
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					data = new QByteArray();
					CBlockingCallbackData blockCallback;
					postEvent(new CExtractResponseDataEvent(qtResponse,data,&blockCallback));
					blockCallback.waitForCallback();
				}
				return data;
				
			}


			bool CQtHttpTransactionManager::callbackResponseData(CHttpResponse* response, QByteArray* dataArray, CCallbackData* callback) {
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					postEvent(new CExtractResponseDataEvent(qtResponse,dataArray,callback));
					return true;
				}
				return false;
			}

			bool CQtHttpTransactionManager::hasFinishedSucecssfully(CHttpResponse* response) {
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


			bool CQtHttpTransactionManager::hasBeenAborted(CHttpResponse* response, ABORT_REASON* abortReason) {
				CQtHttpResponse* qtResponse = dynamic_cast<CQtHttpResponse*>(response);
				if (qtResponse) {
					if (qtResponse->isAborted()) {
						if (abortReason) {
							*abortReason = (ABORT_REASON)qtResponse->getAbortReason();
						}
						return true;
					}
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
					if (mDownloadSizeLimit >= 0) {
						CQtHttpResponseHandler* handler = new CQtHttpResponseHandler(request, response, mDownloadSizeLimit, this);
						mHandlerHash.insert(response, handler);
						connect(reply, &QNetworkReply::downloadProgress, [=](cint64 received, cint64 total) {
							handler->downloadProgress(received, total);
						});
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
				} else if (type == CExtractResponseDataEvent::EVENTTYPE) {
					CExtractResponseDataEvent* erte = (CExtractResponseDataEvent*)event;
					CQtHttpResponse* response = erte->getResponse();
					CExtractResponseDataCallbackEvent* ertce = new CExtractResponseDataCallbackEvent(this,response,erte->getDataPointer(),erte->getCallbacks());
					response->getFinishedCallbackList()->addCallbacks(ertce);
					return true;
				} else if (type == CAddResponseFinishedCallbackEvent::EVENTTYPE) {
					CAddResponseFinishedCallbackEvent* erte = (CAddResponseFinishedCallbackEvent*)event;
					CQtHttpResponse* response = erte->getResponse();
					response->getFinishedCallbackList()->addCallbacks(erte->getCallbacks());
					return true;
				} else if (type == CExtractResponseDataCallbackEvent::EVENTTYPE) {
					CExtractResponseDataCallbackEvent* ertce = (CExtractResponseDataCallbackEvent*)event;
					CQtHttpResponse* response = ertce->getResponse();
					QByteArray* dataArray = ertce->getDataPointer();
					if (response && dataArray && hasFinishedSucecssfully(response)) {
						QNetworkReply* reply = response->getQNetworkReply();
						*dataArray = reply->readAll();
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

					CQtHttpResponseHandler* handler = mHandlerHash.value(response);
					if (handler) {
						delete handler;
						mHandlerHash.remove(response);
					}

					mReplyResponseHash.remove(response->getQNetworkReply());
					mCriticalTimeoutSet.remove(response);
					mNextTimeoutSet.remove(response);
					if (releaseAlsoRequest) {
						delete response->getHttpRequest();
					}

					delete response;
					return true;
				} else if (type == CAbortRequestEvent::EVENTTYPE) {
					CAbortRequestEvent* are = (CAbortRequestEvent*)event;
					CQtHttpResponse* response = (CQtHttpResponse*)are->getResponse();
					response->setAborted(are->getAbortReason());
					response->getQNetworkReply()->abort();
					QString abortReasonString = "none given";
					ABORT_REASON abortReason = (ABORT_REASON)are->getAbortReason();
					if (abortReason == ABORT_DOWNLOAD_SIZE_LIMIT_REACHED) {
						abortReasonString = QString("Download size limit of %1 bytes reached").arg(mDownloadSizeLimit);
					}
					LOG(WARNING, "::Konclude::Network::HTTP::QtHttpTransactionManager", logTr("HTTP request aborted with reason: %1").arg(abortReasonString), this);
					return true;
				}
				return false;
			}



		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

