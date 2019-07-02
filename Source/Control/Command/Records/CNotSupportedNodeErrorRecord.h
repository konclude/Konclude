/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_Control_COMMAND_RECORDS_CNOTSUPPORTEDNODEERRORRECORD_H
#define KONCLUDE_Control_COMMAND_RECORDS_CNOTSUPPORTEDNODEERRORRECORD_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CParseSyntaxErrorRecord.h"

// Other includes
#include "Control/Command/CommandSettings.h"
#include "Control/Command/CCommand.h"
#include "Control/Command/CCommandRecordData.h"



// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Records {

				/*! 
				 *
				 *		\class		CNotSupportedNodeErrorRecord
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CNotSupportedNodeErrorRecord : public CParseSyntaxErrorRecord {
					// public methods
					public:
						//! Constructor
						CNotSupportedNodeErrorRecord(const QString &node, CCommand *command, const QString &recordDomain, CCommandRecordData *nextRecordData = 0);

						//! Destructor
						virtual ~CNotSupportedNodeErrorRecord();

						virtual QString getRecordMessage();

						static void makeRecord(const QString &node, CCommandRecorder *recorder, const QString &domain, CCommand *command);
						static void makeRecord(const QString &node, CCommandRecordRouter *commandRecordRouter);


					// protected methods
					protected:

					// protected variables
					protected:
						QString nodeString;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Records

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_RECORDS_CNOTSUPPORTEDNODEERRORRECORD_H
