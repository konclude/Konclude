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

#ifndef KONCLUDE_Control_COMMAND_CCOMMANDEXECUTEDBLOCKER_H
#define KONCLUDE_Control_COMMAND_CCOMMANDEXECUTEDBLOCKER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CCommand.h"

// Other includes

#include "Concurrent/Callback/CBlockingCallbackData.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Concurrent;
	using namespace Callback;

	namespace Control {

		namespace Command {


			/*! 
			 *
			 *		\class		CCommandExecutedBlocker
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCommandExecutedBlocker {
				// public methods
				public:
					//! Constructor
					CCommandExecutedBlocker();

					//! Destructor
					virtual ~CCommandExecutedBlocker();


					static bool waitExecutedCommand(CCommand *blockingCommand);



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

#endif // KONCLUDE_Control_COMMAND_CCOMMANDEXECUTEDBLOCKER_H
