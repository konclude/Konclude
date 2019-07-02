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

#ifndef KONCLUDE_Control_COMMAND_CCOMMANDERMANAGER_H
#define KONCLUDE_Control_COMMAND_CCOMMANDERMANAGER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CCommand.h"
#include "CCommandDelegater.h"

// Other includes
#include "Config/CConfiguration.h"

#include "Logger/Record/CContextRecorder.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Logger::Record;
	using namespace Config;

	namespace Control {

		namespace Command {

			/*! 
			 *
			 *		\class		CCommanderManager
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCommanderManager : public CCommandDelegater {
				// public methods
				public:
					//! Constructor
					CCommanderManager();

					//! Destructor
					virtual ~CCommanderManager();


					virtual CCommanderManager *realizeCommand(CCommand *command) = 0;
					virtual CCommandDelegater *delegateCommand(CCommand *command);
					

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CCOMMANDERMANAGER_H
