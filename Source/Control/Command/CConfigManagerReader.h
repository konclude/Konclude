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

#ifndef KONCLUDE_Control_COMMAND_CCONFIGMANAGERREADER_H
#define KONCLUDE_Control_COMMAND_CCONFIGMANAGERREADER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CCommanderManagerConfigType.h"
#include "CReasonerManagerConfigType.h"
#include "CClassificationManagerConfigType.h"
#include "COntologyRevisionManagerConfigType.h"

// Other includes
#include "Config/CConfigDataReader.h"

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
			 *		\class		CConfigManagerReader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConfigManagerReader : public CConfigDataReader {
				// public methods
				public:
					//! Constructor
					CConfigManagerReader();

					//! Destructor
					virtual ~CConfigManagerReader();
					
					static CCommanderManager *readCommanderManagerConfig(CConfiguration *config, bool *errorFlag = 0, CCommanderManager *defaultValue = 0, const QString &configString = "Konclude.Execution.CommanderManager");
					static CCommanderManager *readCommanderManagerConfig(CConfigurationProvider *configProv, bool *errorFlag = 0, CCommanderManager *defaultValue = 0, const QString &configString = "Konclude.Execution.CommanderManager");

					static CReasonerManager *readReasonerManagerConfig(CConfiguration *config, bool *errorFlag = 0, CReasonerManager *defaultValue = 0, const QString &configString = "Konclude.Execution.ReasonerManager");
					static CReasonerManager *readReasonerManagerConfig(CConfigurationProvider *configProv, bool *errorFlag = 0, CReasonerManager *defaultValue = 0, const QString &configString = "Konclude.Execution.ReasonerManager");

					static CClassificationManager *readClassificationManagerConfig(CConfiguration *config, bool *errorFlag = 0, CClassificationManager *defaultValue = 0, const QString &configString = "Konclude.Execution.ClassificationManager");
					static CClassificationManager *readClassificationManagerConfig(CConfigurationProvider *configProv, bool *errorFlag = 0, CClassificationManager *defaultValue = 0, const QString &configString = "Konclude.Execution.ClassificationManager");

					static COntologyRevisionManager *readOntologyRevisionConfig(CConfiguration *config, bool *errorFlag = 0, COntologyRevisionManager *defaultValue = 0, const QString &configString = "Konclude.Execution.OntologyRevisionManager");
					static COntologyRevisionManager *readOntologyRevisionConfig(CConfigurationProvider *configProv, bool *errorFlag = 0, COntologyRevisionManager *defaultValue = 0, const QString &configString = "Konclude.Execution.OntologyRevisionManager");


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

#endif // KONCLUDE_Control_COMMAND_CCONFIGMANAGERREADER_H
