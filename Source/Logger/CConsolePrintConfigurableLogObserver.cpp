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

#include "CConsolePrintConfigurableLogObserver.h"

namespace Konclude {

	namespace Logger {



		CConsolePrintConfigurableLogObserver::CConsolePrintConfigurableLogObserver(QStringList domainInfoList, double domainFilterLevel, bool printTime, bool printDomain, bool printMessage, bool printError) {
			confTime = printTime;
			confDomain = printDomain;
			confMessage = printMessage;
			confError = printError;
			mDomainFilterLevel = domainFilterLevel;

			foreach (QString domainString, domainInfoList) {
				domainInfoSet.insert(domainString);
			}


			CLogger* logger = CLogger::getInstance();
			CCallbackRequestLogMessagesContext* logMessagesContext = logger->getLogMessages(0,-1,30,100,QStringList(),QLinkedList<void*>());
			foreach (CLogMessage* message, logMessagesContext->getLogMessages()) {
				postLogMessage(message);
			}
			logMessagesContext->releaseMessages();

		}

		CConsolePrintConfigurableLogObserver::~CConsolePrintConfigurableLogObserver()	{
		}


		void CConsolePrintConfigurableLogObserver::postLogMessage(CLogMessage *message) {

			bool printMessage = true;


			double logLevel = message->getLogLevel();
			QString logLevelString;

			QString domainString = message->getDomain(); 

			if (logLevel >= 90) {
				logLevelString = "catastrophic error";
			} else if (logLevel >= 80) {
				logLevelString = "exceptional error";
			} else if (logLevel >= 70) {
				logLevelString = "error";
			} else if (logLevel >= 60) {
				logLevelString = "warning";
			} else if (logLevel >= 30) {
				logLevelString = "info";
			} else {
				printMessage = false;
				logLevelString = "notice";
			}

			bool filterMessage = false;
			if (logLevel < mDomainFilterLevel) {
				filterMessage = true;
			}


			if (filterMessage && logLevel >= 30) {
				printMessage = domainInfoSet.contains(domainString);
			}

			if (printMessage) {
				QString messageString;

				if (confError) {
					if (!messageString.isEmpty()) {
						messageString += QString(" ");
					}
					messageString += QString("{%1}").arg(logLevelString);
				}

				if (confTime) {
					if (!messageString.isEmpty()) {
						messageString += QString(" ");
					}
					messageString += QString(message->getTimeMSecString());
				}


				if (confDomain) {
					if (!messageString.isEmpty()) {
						messageString += QString(" ");
					}
					messageString += QString("[%1]").arg(domainString);
				}
				
				if (confMessage) {
					if (!messageString.isEmpty()) {
						messageString += QString(" >> ");
					}
					messageString += QString("%1").arg(message->getMessage());
				}

				std::cout << messageString.toLocal8Bit().data() << "\r\n"<<std::flush;

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
				QFile loggingFile(QString("./Debugging/logging.txt"));
				if (loggingFile.open(QIODevice::Append)) {
					QByteArray data = messageString.toUtf8();
					loggingFile.write(data);
					loggingFile.write("\r\n");
					loggingFile.close();
				}
#endif

			}
		}


	}; // end namespace Logger

}; // end namespace Konclude
