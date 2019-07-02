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
#include "CLogger.h"


QString logTr(const char *sourceText, const char *comment, int n) {
	return QObject::tr(sourceText,comment,n);
}


namespace Konclude {

	namespace Logger {

		CLogger *CLogger::instance=0;

		QMutex *CLogger::lockCreatingMoreInstances = new QMutex();


		CLogger::CLogger()
				: CIntervalThread("Logger") {

			unsavedMessagesCount = 0;
			savedMessageCount = 0;
			totalMessageCount = 0;
			lastInMemoryMessage = 0;

			nextMessageID = 0;

			startThread();
		}


		CLogger::~CLogger()	{
			stopThread();
		}



		void CLogger::shutdownLogger() {
			lockCreatingMoreInstances->lock();
			instance->stopThread();
			// wait until the thread is stopped
			instance->wait();
			delete instance;
			instance = 0;
			lockCreatingMoreInstances->unlock();
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


		void CLogger::addLogMessage(QString message, double level, QString domain, void *object) {
			//if (level >= 40) {
				postEvent(new CLoggingEvent(new CLogMessage(domain,message,level,object)));
			//}
		}

		void CLogger::addLogObserver(CAbstractLogObserver *observer, double level, QStringList domains) {
			if (domains.count() <= 0) {
				domains<<"::";
			}
			observerSyncMutex.lock();
			CLogObserverData *obsData = new CLogObserverData(observer,level,domains);
			addObserverToDomains(obsData,domains);
			observerHash.insertMulti((qint64)observer,obsData);
			observerSyncMutex.unlock();
			CThread::waitSynchronization();
		}


		void CLogger::removeObserverFromAllDomains(CAbstractLogObserver *observer) {
			observerSyncMutex.lock();
			if (observerHash.contains((qint64)observer)) {
				QList<CLogObserverData *> obsDataList(observerHash.values((qint64)observer));
				foreach (CLogObserverData *obsData, obsDataList) {
					if (observerDomainHash.contains((qint64)obsData)) {
						QList<CLogDomain *> domainList(observerDomainHash.values((qint64)obsData));
						foreach (CLogDomain *domain, domainList) {
							domain->removeLogObserver(obsData);
						}
					}
					delete obsData;
				}
				observerHash.remove((qint64)observer);
			}
			observerSyncMutex.unlock();
		}

		void CLogger::removeObserverFromDomains(CAbstractLogObserver *observer, QStringList remDomains) {
			observerSyncMutex.lock();
			QSet<QString> removeDomainsSet;

			if (observerHash.contains((qint64)observer)) {
				QList<CLogObserverData *> obsDataList(observerHash.values((qint64)observer));
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
					observerDomainHash.insert((qint64)obsData,domain);
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
				nextMessageID++;

				addLogMessageToDomainHash("::",lm);

				QString accDomain;

				foreach (QString endDomain, domainList) {
					accDomain.append("::");
					accDomain.append(endDomain);
					addLogMessageToDomainHash(accDomain,lm);
					addLogMessageToDomainHash(endDomain,lm);
				}
				observerSyncMutex.unlock();

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

				QSet<qint64> messageHash;

				QLinkedList<void *> objectList = rlm->getObjects();

				for (qint64 startID = qMax(beginID,lastInMemoryMessage); startID != endID; ++startID) {

					if (idMessageHash.contains(startID)) {
						CLogMessage *message = idMessageHash.value(startID);

						double logLevel = message->getLogLevel();
						if (logLevel >= beginLevel && logLevel <= endLevel) {

							if (messageDomainHash.contains((qint64)message)) {
								QList<CLogDomain *> domainList(messageDomainHash.values((qint64)message));

								foreach (CLogDomain *domain, domainList) {
									QString strDomain = domain->getDomain();
									if (domainGetList.isEmpty() || domainGetSet.contains(strDomain)) {
										
										if (!messageHash.contains((qint64)message)) {
											messageHash.insert((qint64)message);
											logMessageContext->addLogMessage(message);
											message->incLockCount();
										}

									}
								}


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


		void CLogger::addLogMessageToDomainHash(QString domain, CLogMessage *message) {
			CLogDomain *logDomain = 0;
			if (domainHash.contains(domain)) {
				logDomain = domainHash.value(domain);
			} else {
				logDomain = new CLogDomain(domain);
				domainHash.insert(domain,logDomain);
			}
			if (logDomain) {
				messageDomainHash.insertMulti((qint64)message,logDomain);
				logDomain->addAndPostLogMessage(message);
			}
		}



		bool CLogger::processTimer(qint64 timerID) {
			if (timerID == SAVETOFILETIMERID) {
				return true;
			}
			return false;
		}


		// returns the Instance of the CLogger and creates it when neccessary
		CLogger *CLogger::getInstance() {
			if (instance == 0) {
				lockCreatingMoreInstances->lock();
				if (instance == 0) {
					instance = new CLogger();
				}
				lockCreatingMoreInstances->unlock();
			}
			return instance;
		}



	}; // end namespace Logger

}; // end namespace Konclude