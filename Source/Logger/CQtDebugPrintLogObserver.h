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
#ifndef KONCLUDE_LOGGER_CQTDEBUGPRINTLOGOBSERVER_H
#define KONCLUDE_LOGGER_CQTDEBUGPRINTLOGOBSERVER_H


#include <iostream>


#include "CAbstractLogObserver.h"


namespace Konclude {

	namespace Logger {

		/*! 
		 *  \class		CQtDebugPrintLogObserver
		 *	\version	0.3
		 *	\author		Andreas Steigmiller
		 *	\brief		TODO
		 */
		class CQtDebugPrintLogObserver : public CAbstractLogObserver {

			public:
				CQtDebugPrintLogObserver();
				virtual ~CQtDebugPrintLogObserver();

				virtual void postLogMessage(CLogMessage *message);

			private:
		
		};

	}; // end namespace Logger

}; // end namespace Konclude

#endif // KONCLUDE_LOGGER_CQTDEBUGPRINTLOGOBSERVER_H
