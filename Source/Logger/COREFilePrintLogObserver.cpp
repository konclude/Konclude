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

#include "COREFilePrintLogObserver.h"

namespace Konclude {

	namespace Logger {



		COREFilePrintLogObserver::COREFilePrintLogObserver(const QString& outputFileString) : mOutputFile(outputFileString) {
			mOutputFileString = outputFileString;
			mOutputFileOpened = false;
			mMinLogLevel = 30;
			mMaxLogLevel = 100;

			CLogger* logger = CLogger::getInstance();
			CCallbackRequestLogMessagesContext* logMessagesContext = logger->getLogMessages(0,-1,30,100,QStringList(),QLinkedList<void*>());
			foreach (CLogMessage* message, logMessagesContext->getLogMessages()) {
				postLogMessage(message);
			}
			logMessagesContext->releaseMessages();

		}

		COREFilePrintLogObserver::~COREFilePrintLogObserver()	{
			if (mOutputFileOpened) {
				mOutputFile.close();
				mOutputFileOpened = false;
			}
		}

		void COREFilePrintLogObserver::closeOutputFile() {
			if (mOutputFileOpened) {
				mOutputFile.close();
				mOutputFileOpened = false;
			}
		}

		void COREFilePrintLogObserver::flushLogging() {
			closeOutputFile();
		}

		void COREFilePrintLogObserver::forcedPathCreated(const QString& filePath) {
			QString path = filePath;
			path = path.mid(0,path.lastIndexOf("/"));
			QDir dir;
			dir.mkpath(path);
		}

		void COREFilePrintLogObserver::postLogMessage(CLogMessage *message) {

			bool printMessage = false;
			double logLevel = message->getLogLevel();
			QString domainString = message->getDomain(); 
			if (logLevel > mMinLogLevel && logLevel <= mMaxLogLevel) {
				printMessage = true;
			}

			if (printMessage) {
				if (!mOutputFileOpened) {
					forcedPathCreated(mOutputFileString);
					mOutputFileOpened = true;
					mOutputFile.open(QIODevice::Append);
				}

				QString messageString;
				QString logLevelString;
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

				messageString += QString("{%1} %2 [%3] >> %4\r\n").arg(logLevelString).arg(message->getTimeMSecString()).arg(domainString).arg(message->getMessage());

				mOutputFile.write( messageString.toLocal8Bit().data() );
				closeOutputFile();
			}
		}


	}; // end namespace Logger

}; // end namespace Konclude
