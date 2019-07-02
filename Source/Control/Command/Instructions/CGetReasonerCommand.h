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

#ifndef KONCLUDE_Control_COMMAND_CGETREASONERCOMMAND_H
#define KONCLUDE_Control_COMMAND_CGETREASONERCOMMAND_H

// Libraries includes
#include <QString>

// Namespace includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CPreconditionCommand.h"

#include "Reasoner/Kernel/Manager/CReasonerManager.h"

// Other includes
#include "Logger/Record/CContextRecorder.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Kernel::Manager;

	namespace Control {

		namespace Command {

			namespace Instructions {

				/*! 
				 *
				 *		\class		CGetReasonerCommand
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CGetReasonerCommand : public CPreconditionCommand {
					// public methods
					public:
						//! Constructor
						CGetReasonerCommand(CCommand *parentSuperCommand = 0);

						//! Destructor
						virtual ~CGetReasonerCommand();

						virtual qint64 getCommandTag();
						virtual QString getBriefCommandDescription();

						virtual CReasonerManager *getReasonerManager();
						virtual CGetReasonerCommand *setReasonerManager(CReasonerManager *reasonerManager);

					// protected methods
					protected:

					// protected variables
					protected:
						CReasonerManager *reasoner;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CGETREASONERCOMMAND_H
