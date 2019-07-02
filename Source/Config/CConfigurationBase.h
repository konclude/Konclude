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

#ifndef KONCLUDE_CONFIG_CCONFIGURATIONBASE_H
#define KONCLUDE_CONFIG_CCONFIGURATIONBASE_H

// Libraries includes


// Namespace includes
#include "CConfiguration.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CConfigurationBase
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConfigurationBase : public CConfiguration {
			// public methods
			public:
				//! Constructor
				CConfigurationBase(CConfiguration *previousConfiguration, qint64 configurationID);
				CConfigurationBase(CConfigurationBase *previousConfiguration, qint64 configurationID);
				CConfigurationBase(CConfigurationGroup *configGroup, qint64 configurationID);

				//! Destructor
				virtual ~CConfigurationBase();

				virtual CConfigData *getRelatedConfigChange(const QString &configName, bool defaultCompensation = true);
				virtual CConfigData *getRelatedConfigChange(qint64 configIndex, bool defaultCompensation = true);

				virtual bool hasConfigChange(qint64 configIndex, qint64 prevConfID, CConfigData *configData = 0);

				virtual bool isSuccessor(qint64 configID);

				virtual qint64 getConfigurationLevel();
				virtual qint64 getConfigurationID();


				virtual CConfigurationBase *appendConfigurationsToList(QLinkedList<CConfigurationBase *> *configurationList);

				virtual bool isConfigLevelLocked(CConfigData *data);
				virtual bool isConfigLevelLocked(qint64 configIndex, bool defaultCompensation = true);
				virtual bool isConfigLevelLocked(const QString &configName, bool defaultCompensation = true);

			// protected methods
			protected:

			// protected variables
			protected:
				CConfigurationBase *prevConfBase;
				qint64 confID;
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CCONFIGURATIONBASE_H
