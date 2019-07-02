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

#ifndef KONCLUDE_CONFIG_CCONFIGDATAREADER_H
#define KONCLUDE_CONFIG_CCONFIGDATAREADER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "ConfigSettings.h"
#include "CConfiguration.h"
#include "CConfigData.h"
#include "CConfigurationProvider.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CConfigDataReader
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConfigDataReader {
			// public methods
			public:
				//! Constructor
				CConfigDataReader();

				//! Destructor
				virtual ~CConfigDataReader();

				static QString readConfigString(CConfiguration *config, const QString &configString, const QString &defaultValue = QString(), bool *errorFlag = 0);
				static qint64 readConfigInteger(CConfiguration *config, const QString &configString, qint64 defaultValue = 0, bool *errorFlag = 0);
				static bool readConfigBoolean(CConfiguration *config, const QString &configString, bool defaultValue = false, bool *errorFlag = 0);
				static QString readConfigString(CConfigurationProvider *configProv, const QString &configString, const QString &defaultValue = QString(), bool *errorFlag = 0);
				static qint64 readConfigInteger(CConfigurationProvider *configProv, const QString &configString, qint64 defaultValue = 0, bool *errorFlag = 0);
				static bool readConfigBoolean(CConfigurationProvider *configProv, const QString &configString, bool defaultValue = false, bool *errorFlag = 0);

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

#endif // KONCLUDE_CONFIG_CCONFIGDATAREADER_H
