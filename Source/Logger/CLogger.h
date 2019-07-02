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
#ifndef KONCLUDE_LOGGER_CLOGGER_H
#define KONCLUDE_LOGGER_CLOGGER_H




#include <QThread>
#include <QEvent>
#include <QLinkedList>
#include <QLinkedListIterator>
#include <QFile>
#include <QTimerEvent>
#include <QLinkedListIterator>
#include <QCoreApplication>
#include <QFile>
#include <QMutex>
#include <QDateTime>
#include <QStringList>


#include "Concurrent/CIntervalThread.h"
#include "Concurrent/Callback/CCallbackData.h"
#include "Concurrent/Callback/CBlockingCallbackData.h"


#include "CLogMessage.h"
#include "CLogDomain.h"
#include "CAbstractLogObserver.h"
#include "CCallbackRequestLogMessagesContext.h"
#include "LoggerSettings.h"


#include "Events/CLoggingEvent.h"
#include "Events/CRequestLogMessagesEvent.h"
#include "Events/CReleaseLogMessagesEvent.h"
#include "Events/CConfigureLoggerEvent.h"


using namespace std;



// define which messages are in fact logged
#ifdef LOGGER_DEBUG

#define LOGNOTICELEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(20)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,20,domain,object);
#define LOGINFOLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(30)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,30,domain,object);
#define LOGWARNINGLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(60)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,60,domain,object);
#define LOGERRORLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(70)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,70,domain,object);
#define LOGEXCEPTIONLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(80)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,80,domain,object);
#define LOGCATASTROPHICLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(90)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,90,domain,object);

#else 
 
#define LOGNOTICELEVEL(domain,message,object)
#define LOGINFOLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(30)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,30,domain,object);
#define LOGWARNINGLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(60)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,60,domain,object);
#define LOGERRORLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(70)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,70,domain,object);
#define LOGEXCEPTIONLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(80)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,80,domain,object);
#define LOGCATASTROPHICLEVEL(domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(90)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,90,domain,object);

#endif

#define LOGCUSTOMLEVEL(level,domain,message,object) if (Konclude::Logger::CLogger::getInstance()->hasMinLoggingLevel(level)) Konclude::Logger::CLogger::getInstance()->addLogMessage(message,level,domain,object);

// simplifications (only name mapping to previous defined)
#define LOGINFO(domain,message,object) LOGINFOLEVEL(domain,message,object)
#define LOGINFORMATION(domain,message,object) LOGINFOLEVEL(domain,message,object)
#define LOGNOTICE(domain,message,object) LOGNOTICELEVEL(domain,message,object)
#define LOGNOTE(domain,message,object) LOGNOTICELEVEL(domain,message,object)
#define LOGWARNING(domain,message,object) LOGWARNINGLEVEL(domain,message,object)
#define LOGWARN(domain,message,object) LOGWARNINGLEVEL(domain,message,object)
#define LOGERROR(domain,message,object) LOGERRORLEVEL(domain,message,object)
#define LOGERR(domain,message,object) LOGERRORLEVEL(domain,message,object)
#define LOGEXCEPTION(domain,message,object) LOGEXCEPTIONLEVEL(domain,message,object)
#define LOGEXC(domain,message,object) LOGEXCEPTIONLEVEL(domain,message,object)
#define LOGCATASTROPHIC(domain,message,object) LOGCATASTROPHICLEVEL(domain,message,object)
#define LOGCAT(domain,message,object) LOGCATASTROPHICLEVEL(domain,message,object)

// more simplifications (only name mapping to previous defined)
#define LOGLOGNOTICELEVEL(domain,message,object) LOGNOTICELEVEL(domain,message,object)
#define LOGLOGINFOLEVEL(domain,message,object) LOGINFOLEVEL(domain,message,object)
#define LOGLOGWARNINGLEVEL(domain,message,object) LOGWARNINGLEVEL(domain,message,object)
#define LOGLOGERRORLEVEL(domain,message,object) LOGERRORLEVEL(domain,message,object)
#define LOGLOGEXCEPTIONLEVEL(domain,message,object) LOGEXCEPTIONLEVEL(domain,message,object)
#define LOGLOGCATASTROPHICLEVEL(domain,message,object) LOGCATASTROPHICLEVEL(domain,message,object)
#define LOGLOGINFO(domain,message,object) LOGINFOLEVEL(domain,message,object)
#define LOGLOGINFORMATION(domain,message,object) LOGINFOLEVEL(domain,message,object)
#define LOGLOGNOTICE(domain,message,object) LOGNOTICELEVEL(domain,message,object)
#define LOGLOGNOTE(domain,message,object) LOGNOTICELEVEL(domain,message,object)
#define LOGLOGWARNING(domain,message,object) LOGWARNINGLEVEL(domain,message,object)
#define LOGLOGWARN(domain,message,object) LOGWARNINGLEVEL(domain,message,object)
#define LOGLOGERROR(domain,message,object) LOGERRORLEVEL(domain,message,object)
#define LOGLOGERR(domain,message,object) LOGERRORLEVEL(domain,message,object)
#define LOGLOGEXCEPTION(domain,message,object) LOGEXCEPTIONLEVEL(domain,message,object)
#define LOGLOGEXC(domain,message,object) LOGEXCEPTIONLEVEL(domain,message,object)
#define LOGLOGCATASTROPHIC(domain,message,object) LOGCATASTROPHICLEVEL(domain,message,object)
#define LOGLOGCAT(domain,message,object) LOGCATASTROPHICLEVEL(domain,message,object)

// more simplifications (only name mapping to previous defined)
#define LOG10(domain,message,object) LOGCUSTOMLEVEL(10,domain,message,object)
#define LOG20(domain,message,object) LOGINFOLEVEL(domain,message,object)
#define LOG30(domain,message,object) LOGNOTICELEVEL(domain,message,object)
#define LOG40(domain,message,object) LOGCUSTOMLEVEL(40,domain,message,object)
#define LOG50(domain,message,object) LOGCUSTOMLEVEL(50,domain,message,object)
#define LOG60(domain,message,object) LOGWARNINGLEVEL(domain,message,object)
#define LOG70(domain,message,object) LOGERRORLEVEL(domain,message,object)
#define LOG80(domain,message,object) LOGEXCEPTIONLEVEL(domain,message,object)
#define LOG90(domain,message,object) LOGCATASTROPHICLEVEL(domain,message,object)


/*! 
		Logs a message
		\param LOGTYPE	Identifies the logging level. With the LoggingSettings you are able to compile the program without Logs from specifics levels.
						-	Use NOTICE to writing information logging messages which are probably ignored if compiling not under Debug.
							NOTE: This can be controlled in the LoggerSettings.
						-	Use INFO to log informations which should be always logged. E.g. program outputs.
						-	Use WARNING to log warning messages, what should mean that the program is still able to continue the execution, but something is not how it should.
						-	Use ERROR to log error messages, what should mean that the program can not continue the current operation.
						-	Use EXCEPTION to log exception messages, which is may also called from an exception constructor.
						-	Use CATASTROPHIC to log catastrophic messages, which should mean the program isn't able to continue the execution and a crash is probably unavoidable.
		\param domain	Identifies the log domain, the most global domain is '::'. Separate each domain with another '::'.
		\param message	The literal message which should be logged.
		\param object	Identifies the object where the log message is occurred. This may helps to tracking/monitor a faulty object.
 */
#define LOG(LOGTYPE,domain,message,object) LOG##LOGTYPE(domain,message,object)

//! Log a message with a custom logging level
#define LOGC(level,domain,message,object) LOGCUSTOMLEVEL(level,domain,message,object)


QString logTr(const char *sourceText, const char *comment = 0, int n = -1);


namespace Konclude {

	using namespace Concurrent;
	using namespace Callback;

	namespace Logger {

		using namespace Events;
		

		/*! 
		 *	\class		CLogger
		 *	\version	0.3
		 *	\author		Andreas Steigmiller
		 *	\brief		Logs messages and posts them to connected Log Observers. The Logger is running in an own Thread. Use LOG(...) to add log messages.
		 */
		class CLogger : public CIntervalThread {
				
			public:
				//! returns the Instance of this Singleton-Class
				static CLogger *getInstance();

				void addLogObserver(CAbstractLogObserver *observer, double level = 0, QStringList domain = QStringList());
				void removeObserverFromAllDomains(CAbstractLogObserver *observer);
				void removeObserverFromDomains(CAbstractLogObserver *observer, QStringList domains);

				void addLogMessage(QString message, double level = 0, QString domain = "::", void *object = 0);

				void getLogMessages(CCallbackData *callback, qint64 messageIDBegin = 0, qint64 messageIDEnd = -1, double levelBegin = 0, double levelEnd = 0, QStringList domains = QStringList(), QLinkedList<void *> objects = QLinkedList<void *>()); 
				CCallbackRequestLogMessagesContext *getLogMessages(qint64 messageIDBegin = 0, qint64 messageIDEnd = -1, double levelBegin = 0, double levelEnd = 0, QStringList domains = QStringList(), QLinkedList<void *> objects = QLinkedList<void *>()); 


				void releaseLogMessages(QLinkedList<CLogMessage *> &logMessages);

				static void shutdownLogger();

				void configureLogger(qint64 maxLogMessages, double minLoggingLevel);
				double getMinLoggingLevel();
				bool hasMinLoggingLevel(double level);

			protected:
				virtual void threadStarted();
				virtual void threadStopped();

				virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

				virtual bool processTimer(qint64 timerID);
			
				bool deleteOldLogMessages();

			private:
				CLogger();
				virtual ~CLogger();

				void propagateLogMessageToDomain(QString domain, CLogMessage *message);


				void addObserverToDomains(CLogObserverData *obsData, QStringList domains);


			private:
				//! Singleton-Instance
				static CLogger* mInstance;


				QMutex observerSyncMutex;
				QHash<CAbstractLogObserver*, CLogObserverData*> observerHash;
				QHash<CLogObserverData*, CLogDomain*> observerDomainHash;


				QHash<qint64,CLogMessage *> idMessageHash;


				//! Log-Entry-List
				QLinkedList<CLogMessage *> loglist;
				QHash<QString,CLogDomain *> domainHash;

				//! Filename for saving
				QString saveFileName;

				//! Necessary to not always save the complete list
				QLinkedList<CLogMessage *>::const_iterator listSaveIt;


				qint64 mCurrentLogMessageCount;
				qint64 mMaxLogMessageCount;
				qint64 mMinLoggingLevel;

				qint64 unsavedMessagesCount;
				qint64 savedMessageCount;
				qint64 totalMessageCount;
				qint64 lastInMemoryMessage;

				qint64 nextMessageID;


				static QMutex* mInstanceCreationMutex;
						
				static const qint64 SAVETOFILETIMERID = 1;
				
			
		};

	}; // end namespace Logger

}; // end namespace Konclude

#endif // KONCLUDE_LOGGER_CLOGGER_H
