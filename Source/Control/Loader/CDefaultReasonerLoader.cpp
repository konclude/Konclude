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

#include "CDefaultReasonerLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CDefaultReasonerLoader::CDefaultReasonerLoader() {
			}



			CDefaultReasonerLoader::~CDefaultReasonerLoader() {
			}


			CLoader *CDefaultReasonerLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {

				reasonerCommander = new CCommanderManagerThread();

				configuration = config;

				CConfigurationGroup *group = configuration->getConfigurationGroup();
				CConfigData *data = configuration->createConfig("Konclude.Execution.CommanderManager");

				CCommanderManagerConfigType *rCConfig = dynamic_cast<CCommanderManagerConfigType *>(data->getConfigType());
				if (rCConfig) {
					rCConfig->setCommanderManager(reasonerCommander);
				}


				group->setConfigDefaultData(group->getConfigIndex("Konclude.Execution.CommanderManager"),data);

				return this;
			}





			CLoader *CDefaultReasonerLoader::load() {

				reasonerCommander->realizeCommand(new CInitializeConfigurationCommand(configuration));

				reasonerCommander->realizeCommand(new CInitializeReasonerCommand(new CDefaultCommanderInitializationFactory()));

				return this;
			}

			CLoader *CDefaultReasonerLoader::exit() {
				delete reasonerCommander;
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
