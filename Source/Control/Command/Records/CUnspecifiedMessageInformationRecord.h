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

#ifndef KONCLUDE_Control_COMMAND_RECORDS_CUNSPECIFIEDMESSAGEINFORMATIONRECORD_H
#define KONCLUDE_Control_COMMAND_RECORDS_CUNSPECIFIEDMESSAGEINFORMATIONRECORD_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CProcessErrorRecord.h"

// Other includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CCommandRecordData.h"



// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				/*! 
				 *
				 *		\class		CUnspecifiedMessageInformationRecord
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CUnspecifiedMessageInformationRecord : public CCommandRecordData {
					// public methods
					public:
						//! Constructor
						CUnspecifiedMessageInformationRecord(CCommand *command, const QString &informationMessage, const QString &recordDomain, CCommandRecordData *nextRecordData = 0);

						//! Destructor
						virtual ~CUnspecifiedMessageInformationRecord();

						virtual QString getRecordMessage();

						static void makeRecord(CCommandRecorder *recorder, const QString &message, const QString &domain, CCommand *command);
						static void makeRecord(const QString &message, CLogIdentifier *logIdentifier, CCommand *command);

						static void makeRecord(const QString &message, CCommandRecordRouter *commandRecordRouter);

					// protected methods
					protected:

					// protected variables
					protected:
						QString message;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_RECORDS_CUNSPECIFIEDMESSAGEINFORMATIONRECORD_H
