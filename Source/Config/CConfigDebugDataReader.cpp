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

#include "CConfigDebugDataReader.h"


namespace Konclude {

	namespace Config {

		CConfigDebugDataReader::CConfigDebugDataReader() {
		}


		CConfigDebugDataReader::~CConfigDebugDataReader() {
		}




		bool CConfigDebugDataReader::readConfigDebugWritingEnabled(CConfiguration *config, const QString &configString, bool defaultValue, bool *errorFlag) {
			bool generalDebugWritingValue = CConfigDataReader::readConfigBoolean(config, "Konclude.Debugging.WriteDebuggingData", defaultValue, errorFlag);
			bool specificDebugWritingValue = generalDebugWritingValue;
			if (generalDebugWritingValue && configString != "Konclude.Debugging.WriteDebuggingData") {
				specificDebugWritingValue = CConfigDataReader::readConfigBoolean(config, configString, defaultValue, errorFlag);
			}
			return generalDebugWritingValue && specificDebugWritingValue;
		}


		bool CConfigDebugDataReader::readConfigDebugWritingEnabled(CConfigurationProvider *configProv, const QString &configString, bool defaultValue, bool *errorFlag) {
			CConfiguration *config = 0;
			if (configProv) {
				config = configProv->getCurrentConfiguration();
			}
			return readConfigDebugWritingEnabled(config, configString, defaultValue, errorFlag);
		}



		bool CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(CConfiguration *config, const QString &configString, bool defaultValue, bool *errorFlag) {
			bool preprocessingDebugWritingValue = readConfigDebugWritingEnabled(config, "Konclude.Debugging.WritePreprocessingDebuggingData", defaultValue, errorFlag);
			bool specificDebugWritingValue = preprocessingDebugWritingValue;
			if (preprocessingDebugWritingValue && configString != "Konclude.Debugging.WriteDebuggingData" && configString != "Konclude.Debugging.WritePreprocessingDebuggingData") {
				specificDebugWritingValue = CConfigDataReader::readConfigBoolean(config, configString, defaultValue, errorFlag);
			}
			return preprocessingDebugWritingValue && specificDebugWritingValue;
		}


		bool CConfigDebugDataReader::readConfigDebugWritingPreprocessingEnabled(CConfigurationProvider *configProv, const QString &configString, bool defaultValue, bool *errorFlag) {
			CConfiguration *config = 0;
			if (configProv) {
				config = configProv->getCurrentConfiguration();
			}
			return readConfigDebugWritingPreprocessingEnabled(config, configString, defaultValue, errorFlag);
		}





		QString CConfigDebugDataReader::readConfigDebuggingWritingFileString(CConfiguration *config, const QString &configString, const QString &basePathConfigString, const QString& defaultValue, bool *errorFlag) {
			QString baseFileString = CConfigDataReader::readConfigString(config, "Konclude.Debugging.BaseWritingPath");
			if (basePathConfigString != "Konclude.Konclude.Debugging.BaseWritingPath") {
				QString extenstionFileString = CConfigDataReader::readConfigString(config, basePathConfigString);
				baseFileString += extenstionFileString;
			}
			QString fileString = CConfigDataReader::readConfigString(config,configString,defaultValue,errorFlag);
			if (!baseFileString.isEmpty()) {
				fileString = baseFileString + QDir::separator() + fileString;
			}
			return fileString;
		}


		QString CConfigDebugDataReader::readConfigDebuggingWritingFileString(CConfigurationProvider *configProv, const QString &configString, const QString &basePathConfigString, const QString& defaultValue, bool *errorFlag) {
			CConfiguration *config = 0;
			if (configProv) {
				config = configProv->getCurrentConfiguration();
			}
			return readConfigDebuggingWritingFileString(config, configString, basePathConfigString, defaultValue, errorFlag);
		}

	}; // end namespace Config

}; // end namespace Konclude
