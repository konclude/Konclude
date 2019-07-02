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

#ifndef KONCLUDE_Control_COMMAND_CPRECONDITIONSYNCHRONIZER_H
#define KONCLUDE_Control_COMMAND_CPRECONDITIONSYNCHRONIZER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CCommandDelegater.h"
#include "CPreconditionCommand.h"

// Other includes
#include "Control/Command/Events/CCommandPreconditionChangeEvent.h"

#include "Concurrent/Callback/CCallbackData.h"
#include "Concurrent/CThread.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Concurrent;
	using namespace Callback;

	namespace Control {

		namespace Command {

			using namespace Events;

			/*! 
			 *
			 *		\class		CPreconditionSynchronizer
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPreconditionSynchronizer : public CThread, public CCommandDelegater {
				// public methods
				public:
					//! Constructor
					CPreconditionSynchronizer(CCommandDelegater *commandDelegater);

					//! Destructor
					virtual ~CPreconditionSynchronizer();


					virtual CCommandDelegater *delegateCommand(CCommand *command);



				// protected methods
				protected:
					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

				// protected variables
				protected:
					CCommandDelegater *delegater;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CPRECONDITIONSYNCHRONIZER_H
