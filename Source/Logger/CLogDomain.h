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
#ifndef LOGGER_CLOGDOMAIN_H
#define LOGGER_CLOGDOMAIN_H

#include <QLinkedList>
#include <QString>


#include "CLogObserverData.h"
#include "CLogMessage.h"

namespace Konclude {

	namespace Logger {

		/*! \class		CLogDomain
		 *	\version	0.3
		 *	\author		Andreas Steigmiller
		 *	\brief		The abstract base Class for each Log-Entry for the CLogger
		 */
		class CLogDomain {

			public:
				CLogDomain(QString domainName);
				virtual ~CLogDomain();

				void addLogObserver(CLogObserverData *observer);
				void removeLogObserver(CLogObserverData *observer);

				void postLogMessage(CLogMessage *message);

				void setDomain(QString domainName);
				QString getDomain();


			private:
				QLinkedList<CLogObserverData *> obsList;

				QString domain;

		
		};

	}; // end namespace Logger

}; // end namespace Konclude

#endif // LOGGER_CLOGDOMAIN_H
