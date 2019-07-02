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

				void addAndPostLogMessage(CLogMessage *message);
				void removeLogMessage(CLogMessage *message);

				void setDomain(QString domainName);
				QString getDomain();


			private:
				QLinkedList<CLogObserverData *> obsList;
				QLinkedList<CLogMessage *> messageList;

				QString domain;

		
		};

	}; // end namespace Logger

}; // end namespace Konclude

#endif // LOGGER_CLOGDOMAIN_H
