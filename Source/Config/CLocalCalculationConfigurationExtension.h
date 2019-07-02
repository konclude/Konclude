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
/*
 *
 *		Author:		Andreas Steigmiller
 *		Copyright:	2009 Andreas Steigmiller
 *		Project:	UUPR
 *
 */

#ifndef UUPR_CONFIG_CLOCALCONFIGURATIONEXTENSION_H
#define UUPR_CONFIG_CLOCALCONFIGURATIONEXTENSION_H

// Qt includes


// Namespace includes
#include "CConfigurationBase.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace UUPR {

	namespace Config {

		/*! 
		 *
		 *		\class		CLocalConfigurationExtension
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CLocalConfigurationExtension : public CConfigurationBase {
			// public methods
			public:
				//! Constructor
				CLocalConfigurationExtension(CConfigurationBase *previousConfiguration, qint64 configID);

				//! Deconstructor
				virtual ~CLocalConfigurationExtension();


				virtual CLocalConfigurationExtension *addConfiguration(CConfigurationBase *configuration);

				virtual CConfigData *getRelatedConfigChange(const QString &configName, bool defaultCompensatory = true);
				virtual CConfigData *getRelatedConfigChange(qint64 configIndex, bool defaultCompensatory = true);

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

}; // end namespace UUPR

#endif // UUPR_CONFIG_CLOCALCONFIGURATIONEXTENSION_H
