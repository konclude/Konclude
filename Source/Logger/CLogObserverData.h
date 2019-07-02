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
#ifndef LOGGER_CLogObserverData_H
#define LOGGER_CLogObserverData_H

#include <QSet>
#include <QStringList>

#include "CLogMessage.h"
#include "CAbstractLogObserver.h"

namespace Konclude {

	namespace Logger {

		/*! \class		CLogObserverData
		 *	\version	0.3
		 *	\author		Andreas Steigmiller
		 *	\brief		The abstract base Class for each Log-Entry for the CLogger
		 */
		class CLogObserverData {

			public:
				CLogObserverData(CAbstractLogObserver *logObserver, double logLevel = 0, QStringList observeDomains = QStringList());
				virtual ~CLogObserverData();

				virtual void postLogMessage(CLogMessage *message);

				virtual void postLogMessageOnce(CLogMessage *message);
				virtual void postLogMessageMulti(CLogMessage *message);

				QStringList getLogDomains();
				void setLogDomains(QStringList observeDomains);
				void addLogDomain(QString domain);
				void removeLogDomain(QString domain);

				double getLogLevel();
				void setLogLevel(double logLevel);

			private:
				CAbstractLogObserver *observer;
				QSet<qint64> sendedLogSet;

				bool postOnce;
				QStringList domains;
				double level;
		
		};

	}; // end namespace Logger

}; // end namespace Konclude

#endif // LOGGER_CLogObserverData_H
