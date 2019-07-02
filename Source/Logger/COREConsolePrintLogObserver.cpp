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

#include "COREConsolePrintLogObserver.h"

namespace Konclude {

	namespace Logger {



		COREConsolePrintLogObserver::COREConsolePrintLogObserver(const QStringList& domainInfoList) {
			mMinLogLevel = 0;
			mMaxLogLevel = 30;
			foreach (QString domainString, domainInfoList) {
				domainInfoSet.insert(domainString);
			}


			CLogger* logger = CLogger::getInstance();
			CCallbackRequestLogMessagesContext* logMessagesContext = logger->getLogMessages(0,-1,30,100,domainInfoList,QLinkedList<void*>());
			foreach (CLogMessage* message, logMessagesContext->getLogMessages()) {
				postLogMessage(message);
			}
			logMessagesContext->releaseMessages();

		}

		COREConsolePrintLogObserver::~COREConsolePrintLogObserver()	{
		}


		void COREConsolePrintLogObserver::postLogMessage(CLogMessage *message) {

			bool printMessage = false;
			double logLevel = message->getLogLevel();
			if (logLevel > mMinLogLevel && logLevel <= mMaxLogLevel) {
				QString domainString = message->getDomain(); 
				printMessage = domainInfoSet.contains(domainString);
			}

			if (printMessage) {
				QString messageString = QString("%1").arg(message->getMessage());
				std::cout << messageString.toUtf8().data() << "\r\n"<<std::flush;
			}

		}

		void COREConsolePrintLogObserver::flushLogging() {
		}


	}; // end namespace Logger

}; // end namespace Konclude
