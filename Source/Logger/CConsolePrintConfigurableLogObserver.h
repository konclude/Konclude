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

#ifndef KONCLUDE_LOGGER_CCONSOLEPRINTCONFIGURABLELOGOBSERVER_H
#define KONCLUDE_LOGGER_CCONSOLEPRINTCONFIGURABLELOGOBSERVER_H


// Libraries includes
#include <QSet>
#include <QStringList>

// Namespace includes
#include "CAbstractLogObserver.h"
#include "CLogger.h"


// Other includes
#include <iostream>


// Logger includes


namespace Konclude {

	namespace Logger {


		/*! 
		*
		*		\class		CConsolePrintConfigurableLogObserver
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CConsolePrintConfigurableLogObserver : public CAbstractLogObserver {

			public:
				CConsolePrintConfigurableLogObserver(QStringList domainInfoList, double domainFilterLevel = 60., bool printTime = true, bool printDomain = true, bool printMessage = true, bool printError = true);
				virtual ~CConsolePrintConfigurableLogObserver();

				virtual void postLogMessage(CLogMessage *message);

			private:
				QSet<QString> domainInfoSet;
				bool confTime;
				bool confDomain;
				bool confMessage;
				bool confError;
				double mDomainFilterLevel;
		
		};

	}; // end namespace Logger

}; // end namespace Konclude

#endif // KONCLUDE_LOGGER_CCONSOLEPRINTCONFIGURABLELOGOBSERVER_H
