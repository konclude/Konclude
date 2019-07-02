/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */
#include "CLogObserverData.h"

namespace Konclude {

	namespace Logger {



		CLogObserverData::CLogObserverData(CAbstractLogObserver *logObserver, double logLevel, QStringList observeDomains) {
			observer = logObserver;
			postOnce = true;
		}

		CLogObserverData::~CLogObserverData() {
		}


		QStringList CLogObserverData::getLogDomains() {
			return domains;
		}

		void CLogObserverData::setLogDomains(QStringList observeDomains) {
			domains = observeDomains;
		}

		void CLogObserverData::addLogDomain(QString domain) {
			domains.append(domain);
		}

		double CLogObserverData::getLogLevel() {
			return level;
		}

		void CLogObserverData::setLogLevel(double logLevel) {
			level = logLevel;
		}


		void CLogObserverData::postLogMessage(CLogMessage *message) {
			if (postOnce) {
				postLogMessageOnce(message);
			} else {
				postLogMessageMulti(message);
			}
		}


		void CLogObserverData::postLogMessageOnce(CLogMessage *message) {
			qint64 messageID = message->getLogID();
			if (!sendedLogSet.contains(messageID)) {
				sendedLogSet.insert(messageID);
				if (observer) {
					observer->postLogMessage(message);
				}
			}
		}


		void CLogObserverData::removeLogDomain(QString domain) {
			domains.removeAll(domain);
		}

		void CLogObserverData::postLogMessageMulti(CLogMessage *message) {
			qint64 messageID = message->getLogID();
			sendedLogSet.insert(messageID);
			if (observer) {
				observer->postLogMessage(message);
			}
		}


	}; // end namespace Logger

}; // end namespace Konclude