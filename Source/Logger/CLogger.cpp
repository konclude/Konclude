/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "CLogger.h"


QString logTr(const char *sourceText, const char *comment, int n) {
	return QObject::tr(sourceText,comment,n);
}


namespace Konclude {

	namespace Logger {

		CLogger *CLogger::mInstance=0;

		QMutex *CLogger::mInstanceCreationMutex = new QMutex();


		CLogger::CLogger()
				: CIntervalThread("Logger") {

			unsavedMessagesCount = 0;
			savedMessageCount = 0;
			totalMessageCount = 0;
			lastInMemoryMessage = 0;

			nextMessageID = 0;

			mMaxLogMessageCount = -1;
			mMinLoggingLevel = 0;
			mCurrentLogMessageCount = 0;

			startThread();
		}


		CLogger::~CLogger()	{
			stopThread();
		}



		void CLogger::shutdownLogger() {
			mInstanceCreationMutex->lock();
			mInstance->stopThread();
			// wait until the thread is stopped
			mInstance->wait();
			delete mInstance;
			mInstance = 0;
			mInstanceCreationMutex->unlock();
		}



		CCallbackRequestLogMessagesContext *CLogger::getLogMessages(qint64 messageIDBegin, qint64 messageIDEnd, double levelBegin, double levelEnd, QStringList domains, QLinkedList<void *> objects) {
			CBlockingCallbackData *blockingCallback = new CBlockingCallbackData();
			getLogMessages(blockingCallback,messageIDBegin,messageIDEnd,levelBegin,levelEnd,domains,objects);
			blockingCallback->waitForCallback();
			CCallbackRequestLogMessagesContext *context = (CCallbackRequestLogMessagesContext *)blockingCallback->takeCallbackDataContext();
			delete blockingCallback;
			return context;
		}


		void CLogger::getLogMessages(CCallbackData *callback, qint64 messageIDBegin, qint64 messageIDEnd, double levelBegin, double levelEnd, QStringList domains, QLinkedList<void *> objects) {
			postEvent(new CRequestLogMessagesEvent(callback,messageIDBegin,messageIDEnd,levelBegin,levelEnd,domains,objects));
		}


		void CLogger::releaseLogMessages(QLinkedList<CLogMessage *> &logMessages) {
			postEvent(new CReleaseLogMessagesEvent(logMessages));
		}


		double CLogger::getMinLoggingLevel() {
			return mMinLoggingLevel;
		}


		bool CLogger::hasMinLoggingLevel(double level) {
			return level >= mMinLoggingLevel;
		}


		void CLogger::configureLogger(qint64 maxLogMessages, double minLoggingLevel) {
			postEvent(new CConfigureLoggerEvent(maxLogMessages,minLoggingLevel));
		}


		void CLogger::addLogMessage(QString message, double level, QString domain, void *object) {
			postEvent(new CLoggingEvent(new CLogMessage(domain,message,level,object)));
		}

		void CLogger::addLogObserver(CAbstractLogObserver *observer, double level, QStringList domains) {
			if (domains.count() <= 0) {
				domains<<"::";
			}
			observerSyncMutex.lock();
			CLogObserverData *obsData = new CLogObserverData(observer,level,domains);
			addObserverToDomains(obsData,domains);
			observerHash.insertMulti(observer,obsData);
			observerSyncMutex.unlock();
			CThread::waitSynchronization();
		}


		void CLogger::removeObserverFromAllDomains(CAbstractLogObserver *observer) {
			observerSyncMutex.lock();
			if (observerHash.contains(observer)) {
				QList<CLogObserverData *> obsDataList(observerHash.values(observer));
				foreach (CLogObserverData *obsData, obsDataList) {
					if (observerDomainHash.contains(obsData)) {
						QList<CLogDomain *> domainList(observerDomainHash.values(obsData));
						foreach (CLogDomain *domain, domainList) {
							domain->removeLogObserver(obsData);
						}
					}
					delete obsData;
				}
				observerHash.remove(observer);
			}
			observerSyncMutex.unlock();
		}

		void CLogger::removeObserverFromDomains(CAbstractLogObserver *observer, QStringList remDomains) {
			observerSyncMutex.lock();
			QSet<QString> removeDomainsSet;

			if (observerHash.contains(observer)) {
				QList<CLogObserverData *> obsDataList(observerHash.values(observer));
				foreach (QString domain,remDomains) {
					removeDomainsSet.insert(domain);
				}
				foreach (CLogObserverData *obsData, obsDataList) {
					QStringList domainList(obsData->getLogDomains());
					foreach (QString domain, domainList) {
						if (removeDomainsSet.contains(domain)) {
							if (domainHash.contains(domain)) {
								CLogDomain *logDomain = domainHash.value(domain);
								logDomain->removeLogObserver(obsData);
							}
							obsData->removeLogDomain(domain);
						}
					}
				}
			}
			observerSyncMutex.unlock();
		}



		void CLogger::addObserverToDomains(CLogObserverData *obsData, QStringList domains) {
			foreach (QString domainString, domains) {
				CLogDomain *domain = 0;
				if (domainHash.contains(domainString)) {
					domain = domainHash.value(domainString);
				} else {
					domain = new CLogDomain(domainString);
					domainHash.insert(domainString,domain);
				}
				if (domain) {
					obsData->addLogDomain(domainString);
					domain->addLogObserver(obsData);
					observerDomainHash.insert(obsData,domain);
				}
			}
		}



		void CLogger::threadStarted() {
			CIntervalThread::threadStarted();
			startTimerWithInterval(SAVETOFILETIMERID,1000);
		}

		void CLogger::threadStopped() {
			CIntervalThread::threadStopped();
			qDeleteAll(loglist);
		}



		bool CLogger::deleteOldLogMessages() {
			bool messagesDeleted = false;
			if (mCurrentLogMessageCount > mMaxLogMessageCount && mMaxLogMessageCount >= 0) {
				while (mCurrentLogMessageCount > mMaxLogMessageCount && !loglist.isEmpty()) {
					CLogMessage* deleteLM = loglist.takeLast();
					idMessageHash.remove(deleteLM->getLogID());
					delete deleteLM;
					messagesDeleted = true;
					--mCurrentLogMessageCount;
				}
			}	
			return messagesDeleted;
		}



		bool CLogger::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
			if (CIntervalThread::processCustomsEvents(type,event)) {
				return true;
			} else if (type == Konclude::Logger::EVENTLOGMESSAGE) {
				SETTASKDESCRIPTION("Process Log Message");
				CLoggingEvent *le = (CLoggingEvent *)event;
				CLogMessage *lm = le->getLogMessage();

				QString domain = lm->getDomain();
				QStringList domainList(domain.split("::",QString::SkipEmptyParts));

				loglist.append(lm);
				unsavedMessagesCount++;
				totalMessageCount++;

				observerSyncMutex.lock();

				lm->setLogID(nextMessageID);
				idMessageHash.insert(nextMessageID,lm);
				++nextMessageID;
				++mCurrentLogMessageCount;

				propagateLogMessageToDomain("::",lm);

				QString accDomain;

				foreach (QString endDomain, domainList) {
					accDomain.append("::");
					accDomain.append(endDomain);
					propagateLogMessageToDomain(accDomain,lm);
					propagateLogMessageToDomain(endDomain,lm);
				}
				observerSyncMutex.unlock();

				deleteOldLogMessages();

				return true;

			} else if (type == Konclude::Logger::EVENTCONFIGURELOGGER) {
				SETTASKDESCRIPTION("Configure Logger");
				CConfigureLoggerEvent* cle = (CConfigureLoggerEvent *)event;

				mMaxLogMessageCount = cle->getMaxLogMessageCount();
				mMinLoggingLevel = cle->getMinLoggingLevel();

				deleteOldLogMessages();

				return true;

			} else if (type == Konclude::Logger::EVENTRELEASELOGMESSAGES) {
				SETTASKDESCRIPTION("Release Log Message");
				CReleaseLogMessagesEvent *rlm = (CReleaseLogMessagesEvent *)event;

				QLinkedList<CLogMessage *> releaseLogMessageList(rlm->getLogMessages());

				foreach (CLogMessage *logMessage, releaseLogMessageList) {
					logMessage->decLockCount();
				}

				return true;

			} else if (type == Konclude::Logger::EVENTREQUESTLOGMESSAGES) {
				SETTASKDESCRIPTION("Create Log Message Collection Response");
				CRequestLogMessagesEvent *rlm = (CRequestLogMessagesEvent *)event;

				CCallbackRequestLogMessagesContext *logMessageContext = new CCallbackRequestLogMessagesContext();

				double beginLevel = rlm->getBeginLevel();
				double endLevel = rlm->getEndLevel();

				qint64 beginID = rlm->getMessageBeginID();
				qint64 endID = rlm->getMessageEndID();
				if (endID == -1) {
					endID = nextMessageID;
				}
				endID = qMax(endID,beginID);

				QStringList domainGetList = rlm->getDomains();
				CCallbackData *callback = rlm->getCallback();

				QSet<QString> domainGetSet;
				foreach (QString domain, domainGetList) {
					domainGetSet.insert(domain);
				}

				QLinkedList<void *> objectList = rlm->getObjects();

				for (qint64 startID = qMax(beginID,lastInMemoryMessage); startID != endID; ++startID) {

					if (idMessageHash.contains(startID)) {
						CLogMessage *message = idMessageHash.value(startID);

						double logLevel = message->getLogLevel();
						if (logLevel >= beginLevel && logLevel <= endLevel) {

							bool messageRequested = false;
							if (domainGetList.isEmpty()) {
								messageRequested = true;
							} else {
								if (domainGetSet.contains("::")) {
									messageRequested = true;
								} else {
									QStringList domainList(message->getDomain().split("::",QString::SkipEmptyParts));
									QString accDomain;
									foreach (const QString& endDomain, domainList) {
										accDomain.append("::");
										accDomain.append(endDomain);
										if (domainGetSet.contains(accDomain) || domainGetSet.contains(endDomain)) {
											messageRequested = true;
											break;
										}
									}

								}
							}


							if (messageRequested) {
								logMessageContext->addLogMessage(message);
								message->incLockCount();
							}
										
						}


					}
					
				}

				callback->setCallbackDataContext(logMessageContext);

				callback->doCallback();
				return true;

			}
			return false;
		}


		void CLogger::propagateLogMessageToDomain(QString domain, CLogMessage *message) {
			CLogDomain *logDomain = domainHash.value(domain);
			if (logDomain) {
				logDomain->postLogMessage(message);
			}
		}



		bool CLogger::processTimer(qint64 timerID) {
			if (timerID == SAVETOFILETIMERID) {
				return true;
			}
			return false;
		}


		// returns the Instance of the CLogger and creates it when necessary
		CLogger *CLogger::getInstance() {
			if (mInstance == nullptr) {
				mInstanceCreationMutex->lock();
				if (mInstance == 0) {
					mInstance = new CLogger();
				}
				mInstanceCreationMutex->unlock();
			}
			return mInstance;
		}



	}; // end namespace Logger

}; // end namespace Konclude