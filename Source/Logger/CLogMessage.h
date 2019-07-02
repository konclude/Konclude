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
#ifndef LOGGER_CLOGMESSAGE_H
#define LOGGER_CLOGMESSAGE_H

#include <QString>
#include <QDateTime>
#include <QObject>

#include "LoggerSettings.h"

namespace Konclude {

	namespace Logger {

		/*! \class		CLogMessage
		 *	\version	0.3
		 *	\author		Andreas Steigmiller
		 *	\brief		The abstract base Class for each Log-Entry for the CLogger
		 */
		class CLogMessage {

			public:
				CLogMessage(QString logDomain, QString logMessage, double logLevel, void *logObject);
				~CLogMessage();


				QString getMessage();
				void setMessage(QString logMessage);

				QDateTime getDateTime();
				QString getDateTimeString();
				QString getDateTimeMSecString();
				QString getTimeMSecString();

				double getLogLevel();
				void setLogLevel(double logLevel);

				QString getDomain();
				void setDomain(QString logDomain);

				void setLogObject(void *logObject);
				void *getLogObject();


				void setLogID(qint64 id);
				qint64 getLogID();

				
				void setLockCount(qint64 count);
				void incLockCount();
				void decLockCount();
				qint64 getLockCount();
				


			private:
				QString message;
				QString domain;
				QDateTime datetime;
				double level;
				qint64 logID;
				void *object;
				qint64 lockCount;
		
		};

	}; // end namespace Logger

}; // end namespace Konclude

#endif // LOGGER_CLOGMESSAGE_H
