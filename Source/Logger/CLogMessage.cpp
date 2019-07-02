/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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
#include "CLogMessage.h"

namespace Konclude {

	namespace Logger {



		CLogMessage::CLogMessage(QString logDomain, QString logMessage, double logLevel, void *logObject) {
			message = logMessage;
			level = logLevel;
			domain = logDomain;
			datetime = QDateTime::currentDateTime();
			object = logObject;
			lockCount = 0;
		}

		CLogMessage::~CLogMessage()	{

		}


		void CLogMessage::setLockCount(qint64 count) {
			lockCount = count;
		}

		void CLogMessage::incLockCount() {
			lockCount++;
		}

		void CLogMessage::decLockCount() {
			lockCount--;
			if (lockCount < 0) {
				lockCount = 0;
			}
		}

		qint64 CLogMessage::getLockCount() {
			return lockCount;
		}


		QString CLogMessage::getMessage() {
			return message;
		}

		void CLogMessage::setMessage(QString logMessage) {
			message = logMessage;
		}


		QDateTime CLogMessage::getDateTime() {
			return datetime;
		}

		QString CLogMessage::getDateTimeString() {
			return datetime.toString(Qt::SystemLocaleDate);
		}

		QString CLogMessage::getDateTimeMSecString() {

			int msec = datetime.time().msec();
			QString msecString(QString("%1").arg(msec));
			while (msecString.length() < 3) {
				msecString = msecString.insert(0,'0');
			}
			return QString("%1.%2").arg(datetime.toString(Qt::SystemLocaleDate)).arg(msecString);
		}

		QString CLogMessage::getTimeMSecString() {
			return QString("%1").arg(datetime.toString("hh:mm:ss:zzz"));
		}


		double CLogMessage::getLogLevel() {
			return level;
		}

		void CLogMessage::setLogLevel(double logLevel) {
			level = logLevel;
		}

		QString CLogMessage::getDomain() {
			return domain;
		}

		void CLogMessage::setDomain(QString logDomain) {
			domain = logDomain;
		}

		void CLogMessage::setLogObject(void *logObject) {
			object = logObject;
		}

		void *CLogMessage::getLogObject() {
			return object;
		}

		void CLogMessage::setLogID(qint64 id) {
			logID = id;
		}

		qint64 CLogMessage::getLogID() {
			return logID;
		}


	}; // end namespace Logger

}; // end namespace Konclude