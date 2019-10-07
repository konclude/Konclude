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

#ifndef KONCLUDE_Control_COMMAND_CREASONERCOMMANDERGLOBALCONFIGURATION_H
#define KONCLUDE_Control_COMMAND_CREASONERCOMMANDERGLOBALCONFIGURATION_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CReasonerConfigurationGroup.h"


// Other includes
#include "Config/CConfiguration.h"
#include "Config/CGlobalConfigurationBase.h"
#include "Config/CGlobalConfigurationProvider.h"


#include "Utilities/CSingletonProvider.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;
	using namespace Utilities;

	namespace Control {

		namespace Command {

			/*! 
			 *
			 *		\class		CReasonerCommanderGlobalConfiguration
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerCommanderGlobalConfiguration : public CGlobalConfigurationProvider {
				// public methods
				public:
					//! Constructor
					CReasonerCommanderGlobalConfiguration(CConfiguration *initialConfiguration = 0);

					//! Destructor
					virtual ~CReasonerCommanderGlobalConfiguration();

					virtual CGlobalConfigurationBase *getCurrentGlobalConfiguration();

					virtual qint64 getNextUniqueConfigID();

					virtual bool installGlobalConfiguration(CGlobalConfigurationBase *configuration);
					virtual CGlobalConfigurationBase *createNextGlobalConfiguration();

					

				// protected methods
				protected:
					virtual CGlobalConfigurationBase *loadGlobalReasonerConfiguration();
					virtual CReasonerCommanderGlobalConfiguration *saveGlobalReasonerConfiguration(CGlobalConfigurationBase *configuration);

				// protected variables
				protected:
					CConfigurationBase *initConf;
					CGlobalConfigurationBase *lastPersConf;

					QLinkedList<CConfigurationBase *> configContainer;
					qint64 nextUniqueConfigID;

					QMutex idSyncMutex;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CREASONERCOMMANDERGLOBALCONFIGURATION_H
