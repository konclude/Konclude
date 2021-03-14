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
#include "CConsolePrintLogObserver.h"

namespace Konclude {

	namespace Logger {



		CConsolePrintLogObserver::CConsolePrintLogObserver() {
			CLogger* logger = CLogger::getInstance();
			CCallbackRequestLogMessagesContext* logMessagesContext = logger->getLogMessages(0,-1,0,100,QStringList(),QLinkedList<void*>());
			foreach (CLogMessage* message, logMessagesContext->getLogMessages()) {
				postLogMessage(message);
			}
			logMessagesContext->releaseMessages();
		}

		CConsolePrintLogObserver::~CConsolePrintLogObserver()	{
		}


		void CConsolePrintLogObserver::postLogMessage(CLogMessage *message) {

			double logLevel = message->getLogLevel();
			QString logLevelString;

			QString domainString = message->getDomain();

			if (logLevel >= 90) {
				logLevelString = "error";
			} else if (logLevel >= 80) {
				logLevelString = "error";
			} else if (logLevel >= 70) {
				logLevelString = "error";
			} else if (logLevel >= 60) {
				logLevelString = " warn ";
			} else if (logLevel >= 30) {
				logLevelString = " info ";
			} else {
				logLevelString = "notice";
			}
			QString logString = QString("{%1} %2 [%3]>> %4\r\n").arg(logLevelString).arg(message->getDateTimeMSecString()).arg(message->getDomain()).arg(message->getMessage());
			std::cout << logString.toLocal8Bit().data() << std::flush;


#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
			QFile loggingFile(QString("./Debugging/logging.txt"));
			if (loggingFile.open(QIODevice::Append)) {
				QByteArray data = logString.toUtf8();
				loggingFile.write(data);
				loggingFile.write("\r\n");
				loggingFile.close();
			}
#endif
		}


	}; // end namespace Logger

}; // end namespace Konclude
