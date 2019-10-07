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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CCONFIGJNIREADER_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CCONFIGJNIREADER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "JNISettings.h"
#include "CJNIInstanceManagerConfigType.h"
#include "CJNIInstanceManager.h"

// Other includes
#include "Config/CConfigDataReader.h"

#include "Control/Command/CConfigManagerReader.h"

#include "Logger/Record/CContextRecorder.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Logger::Record;
	using namespace Config;

	namespace Control {

		namespace Interface {

			namespace JNI {

				/*! 
				 *
				 *		\class		CConfigJNIReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConfigJNIReader : public CConfigManagerReader {
					// public methods
					public:
						//! Constructor
						CConfigJNIReader();

						//! Destructor
						virtual ~CConfigJNIReader();
						
						static CJNIInstanceManager *readJNIInstanceManagerConfig(CConfiguration *config, bool *errorFlag = 0, CJNIInstanceManager *defaultValue = 0, const QString &configString = "Konclude.JNI.JNIInstanceManager");
						static CJNIInstanceManager *readJNIInstanceManagerConfig(CConfigurationProvider *configProv, bool *errorFlag = 0, CJNIInstanceManager *defaultValue = 0, const QString &configString = "Konclude.JNI.JNIInstanceManager");



					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace JNI

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CCONFIGJNIREADER_H
