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

#ifndef KONCLUDE_CONFIG_CCONFIGDEBUGDATAREADER_H
#define KONCLUDE_CONFIG_CCONFIGDEBUGDATAREADER_H

// Libraries includes
#include <QDir>


// Namespace includes
#include "ConfigSettings.h"
#include "CConfigDataReader.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CConfigDebugDataReader
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConfigDebugDataReader : public CConfigDataReader {
			// public methods
			public:
				//! Constructor
				CConfigDebugDataReader();

				//! Destructor
				virtual ~CConfigDebugDataReader();

				
				static QString readConfigDebuggingWritingFileString(CConfiguration *config, const QString &configString, const QString &basePathConfigString = QString("Konclude.Konclude.Debugging.BaseWritingPath"), const QString &defaultValue = QString(), bool *errorFlag = 0);
				static QString readConfigDebuggingWritingFileString(CConfigurationProvider *configProv, const QString &configString, const QString &basePathConfigString = QString("Konclude.Debugging.BaseWritingPath"), const QString &defaultValue = QString(), bool *errorFlag = 0);


				static bool readConfigDebugWritingEnabled(CConfiguration *config, const QString &configString = QString("Konclude.Debugging.WriteDebuggingData"), bool defaultValue = false, bool *errorFlag = 0);
				static bool readConfigDebugWritingEnabled(CConfigurationProvider *configProv, const QString &configString = QString("Konclude.Debugging.WriteDebuggingData"), bool defaultValue = false, bool *errorFlag = 0);

				static bool readConfigDebugWritingPreprocessingEnabled(CConfiguration *config, const QString &configString = QString("Konclude.Debugging.WritePreprocessingDebuggingData"), bool defaultValue = false, bool *errorFlag = 0);
				static bool readConfigDebugWritingPreprocessingEnabled(CConfigurationProvider *configProv, const QString &configString = QString("Konclude.Debugging.WritePreprocessingDebuggingData"), bool defaultValue = false, bool *errorFlag = 0);


			// protected methods
			protected:

			// protected variables
			protected:		
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CCONFIGDEBUGDATAREADER_H
