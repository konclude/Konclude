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

#ifndef KONCLUDE_Control_COMMAND_CReasonerCommandBuilder_H
#define KONCLUDE_Control_COMMAND_CReasonerCommandBuilder_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CCommandBuilder.h"


// Other includes
#include "Instructions/CSetConfigCommand.h"
#include "Instructions/CGetConfigCommand.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/Record/CContextRecorder.h"


namespace Konclude {

	using namespace Logger;
	using namespace Logger::Record;

	namespace Control {

		namespace Command {

			using namespace Instructions;

			/*! 
			 *
			 *		\class		CReasonerCommandBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerCommandBuilder : public CCommandBuilder {
				// public methods
				public:
					//! Constructor
					CReasonerCommandBuilder();

					//! Destructor
					virtual ~CReasonerCommandBuilder();


					virtual CSetConfigCommand *createSetConfigCommand(const QString &configName, const QString &configValue, CContextRecorder *record = 0);
					virtual CGetConfigCommand *createGetConfigCommand(const QString &configName, CContextRecorder *record = 0);
					
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

#endif // KONCLUDE_Control_COMMAND_CReasonerCommandBuilder_H
