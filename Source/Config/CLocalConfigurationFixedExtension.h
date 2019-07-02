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

#ifndef KONCLUDE_CONFIG_CLOCALCONFIGURATIONFIXEDEXTENSION_H
#define KONCLUDE_CONFIG_CLOCALCONFIGURATIONFIXEDEXTENSION_H

// Libraries includes


// Namespace includes
#include "CConfigurationBase.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Config {

		/*! 
		 *
		 *		\class		CLocalConfigurationFixedExtension
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CLocalConfigurationFixedExtension : public CConfigurationBase {
			// public methods
			public:
				//! Constructor
				CLocalConfigurationFixedExtension(CConfigurationGroup *configGroup, qint64 configID);
				CLocalConfigurationFixedExtension(CConfigurationBase *previousConfiguration, qint64 configID);

				//! Destructor
				virtual ~CLocalConfigurationFixedExtension();


				virtual CLocalConfigurationFixedExtension *addConfiguration(CConfigurationBase *configuration);
				virtual CLocalConfigurationFixedExtension *addCollectedConfigurations(CConfigurationBase *collectFromConfiguration);

				virtual CLocalConfigurationFixedExtension *addConfigurationList(const QLinkedList<CConfigurationBase *> &configList);
				virtual QLinkedList<CConfigurationBase *> getAdditionalConfigurationList();


				virtual CConfigData *getRelatedConfigChange(qint64 configIndex, bool defaultCompensation = true);
				virtual CConfigData *getRelatedConfigChange(const QString &configName, bool defaultCompensation = true);

				virtual qint64 getConfigurationLevel();

				virtual CConfigurationBase *appendConfigurationsToList(QLinkedList<CConfigurationBase *> *configurationList);



			// protected methods
			protected:

			// protected variables
			protected:
				QLinkedList<CConfigurationBase *> remConfigList;
				
			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Config

}; // end namespace Konclude

#endif // KONCLUDE_CONFIG_CLOCALCONFIGURATIONFIXEDEXTENSION_H
