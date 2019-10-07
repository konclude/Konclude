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

#ifndef KONCLUDE_Control_COMMAND_CCOMMANDRECORDDATA_H
#define KONCLUDE_Control_COMMAND_CCOMMANDRECORDDATA_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CommandSettings.h"
#include "CCommand.h"
#include "CCommandRecorder.h"
#include "CCommandRecordRouter.h"

// Other includes
#include "Logger/Record/CContextRecordData.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Logger::Record;

	namespace Control {

		namespace Command {

			/*! 
			 *
			 *		\class		CCommandRecordData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCommandRecordData : public CContextRecordData {
				// public methods
				public:
					//! Constructor
					CCommandRecordData(CCommand *command, const QString &recordDomain, double recordErrorLevel = 0., CCommandRecordData *nextRecordData = 0);

					//! Destructor
					virtual ~CCommandRecordData();

					virtual CCommand *getCommand();
					virtual CCommandRecordData *setCommand(CCommand *command);

					virtual QString getRecordDomain();
					virtual CCommandRecordData *setRecordDomain(const QString &recordDomain);


					virtual QString getRecordMessage() = 0;

					virtual CContextRecordData *setNextRecordData(CContextRecordData *recordData);
					virtual CCommandRecordData *getNextCommandRecordData();

					
				// protected methods
				protected:

				// protected variables
				protected:
					CCommand *comm;
					QString recDomain;
					CCommandRecordData *prevCommandRecordData;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CCOMMANDRECORDDATA_H
