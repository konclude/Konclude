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
			if (path.contains("/") || path.contains("\\")) {
				int lastSlash = path.lastIndexOf("/");
				int lastBackSlash = path.lastIndexOf("\\");
				int lastSeparator = qMax(lastBackSlash,lastSlash);
				path = path.mid(0,lastSeparator);
				if (!path.isEmpty()) {
					QDir dir;
					dir.mkpath(path);
				}
			}
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

				mOutputFile.write( messageString.toUtf8().data() );
				closeOutputFile();
			}
		}


	}; // end namespace Logger

}; // end namespace Konclude
