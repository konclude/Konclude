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

#ifndef KONCLUDE_Control_COMMAND_RECORDS_CCLOSUREPROCESSCOMMANDRECORD_H
#define KONCLUDE_Control_COMMAND_RECORDS_CCLOSUREPROCESSCOMMANDRECORD_H

// Libraries includes
#include <QString>

// Namespace includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CCommandRecordData.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				/*! 
				 *
				 *		\class		CClosureProcessCommandRecord
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CClosureProcessCommandRecord : public CCommandRecordData {
					// public methods
					public:
						//! Constructor
						CClosureProcessCommandRecord(CCommand *command, const QString &recordDomain, CCommandRecordData *nextRecordData = 0, double subCommandsErrorLevel = 0.);

						//! Destructor
						virtual ~CClosureProcessCommandRecord();

						virtual QString getRecordMessage();

						static void makeRecord(CCommandRecorder *recorder, const QString &domain, CCommand *command, double subCommandsErrorLevel = 0.);
						static void makeRecord(CCommandRecordRouter *commandRecordRouter, double subCommandsErrorLevel = 0.);


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_RECORDS_CCLOSUREPROCESSCOMMANDRECORD_H
