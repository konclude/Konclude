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

#ifndef KONCLUDE_CONFIG_CCONFIGURATION_H
#define KONCLUDE_CONFIG_CCONFIGURATION_H

// Libraries includes


// Namespace includes
#include "CConfigurationGroup.h"
#include "CConfigData.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CConfiguration
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConfiguration {
			// public methods
			public:
				//! Constructor
				CConfiguration(CConfigurationGroup *configGroup);
				CConfiguration(CConfiguration *previousConfiguration);

				//! Destructor
				virtual ~CConfiguration();

				virtual bool setConfig(qint64 configIndex, CConfigData *takeData);
				virtual bool setConfig(const QString &configName, CConfigData *takeData);

				virtual CConfigData *createConfig(const QString &configName);
				virtual CConfigData *createAndSetConfig(const QString &configName);

				virtual CConfigData *getLastConfigChange(const QString &configName, bool defaultCompensation = true);
				virtual CConfigData *getLastConfigChange(qint64 configIndex, bool defaultCompensation = true);

				virtual CConfigData *getRelatedConfigChange(const QString &configName, bool defaultCompensation = true);
				virtual CConfigData *getRelatedConfigChange(qint64 configIndex, bool defaultCompensation = true);

				virtual CConfigurationGroup *getConfigurationGroup();
				virtual CConfiguration *extendConfigurationGroup(CConfigurationGroup *referencedConfigGroup);


			// protected methods
			protected:

			// protected variables
			protected:
				CConfigurationGroup *conGroup;
				QVector<CConfigData *> *conData;
				QVector<bool> *hasRefConData;
				QVector<bool> *checkedRefConData;
				QLinkedList<CConfigData *> conDataContainer;

				CConfiguration *prevConf;
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CCONFIGURATION_H
