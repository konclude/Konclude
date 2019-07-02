/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_Control_COMMAND_CCOMMANDRECORDROUTER_H
#define KONCLUDE_Control_COMMAND_CCOMMANDRECORDROUTER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CommandSettings.h"
#include "CCommandRecordData.h"

// Other includes
#include "Logger/CLogIdentifier.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;

	namespace Control {

		namespace Command {

			/*! 
			 *
			 *		\class		CCommandRecordRouter
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCommandRecordRouter {
				// public methods
				public:
					//! Constructor
					CCommandRecordRouter(CCommand *command, CLogIdentifier *logIdentifier);
					CCommandRecordRouter(CCommandRecordRouter &commandRecordRouter, CLogIdentifier *logIdentifier);

					//! Destructor
					virtual ~CCommandRecordRouter();

					virtual CCommand *getCommand();
					virtual CCommandRecorder *getCommandRecorder();
					virtual QString getLogDomain();
					
				// protected methods
				protected:

				// protected variables
				protected:
					CCommandRecorder *commRecorder;
					CCommand *comm;
					CLogIdentifier *identifier;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CCOMMANDRECORDROUTER_H
