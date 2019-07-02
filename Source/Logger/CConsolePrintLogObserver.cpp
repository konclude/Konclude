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

			std::cout	<< "{" << logLevelString.toLocal8Bit().data() << "} "
						<< message->getDateTimeMSecString().toLocal8Bit().data()
						<< "[" << message->getDomain().toLocal8Bit().data() 
						<< "]>> " << message->getMessage().toLocal8Bit().data() << "\r\n"<<std::flush;
		}


	}; // end namespace Logger

}; // end namespace Konclude
