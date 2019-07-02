/*
 *
 *		Author:		Andreas Steigmiller
 *		Copyright:	2009 Andreas Steigmiller
 *		Project:	Konclude
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

