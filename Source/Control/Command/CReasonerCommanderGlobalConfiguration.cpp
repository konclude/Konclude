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

#include "CReasonerCommanderGlobalConfiguration.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CReasonerCommanderGlobalConfiguration::CReasonerCommanderGlobalConfiguration(CConfiguration *initialConfiguration) {
				nextUniqueConfigID = 0;
				if (!initialConfiguration) {
					initConf = new CConfigurationBase(CSingletonProvider<CReasonerConfigurationGroup>::getInstance(),getNextUniqueConfigID());
				} else {
					initConf = new CConfigurationBase(initialConfiguration,getNextUniqueConfigID());					
				}
				configContainer.append(initConf);


				lastPersConf = loadGlobalReasonerConfiguration();


				configContainer.append(lastPersConf);
			}


			CReasonerCommanderGlobalConfiguration::~CReasonerCommanderGlobalConfiguration() {
			}


			qint64 CReasonerCommanderGlobalConfiguration::getNextUniqueConfigID() {
				qint64 nextConfID = 0;
				idSyncMutex.lock();
				nextConfID = nextUniqueConfigID++;
				idSyncMutex.unlock();
				return nextConfID;
			}


			CGlobalConfigurationBase *CReasonerCommanderGlobalConfiguration::getCurrentGlobalConfiguration() {
				return lastPersConf;
			}


			CGlobalConfigurationBase *CReasonerCommanderGlobalConfiguration::loadGlobalReasonerConfiguration() {
				if (initConf)  {
					return new CGlobalConfigurationBase(initConf,getNextUniqueConfigID());
				} else {
					return new CGlobalConfigurationBase(CSingletonProvider<CReasonerConfigurationGroup>::getInstance(),getNextUniqueConfigID());
				}
			}

			CReasonerCommanderGlobalConfiguration *CReasonerCommanderGlobalConfiguration::saveGlobalReasonerConfiguration(CGlobalConfigurationBase *configuration) {
				return this;
			}


			bool CReasonerCommanderGlobalConfiguration::installGlobalConfiguration(CGlobalConfigurationBase *configuration) {
				if (configuration->getConfigurationID() > lastPersConf->getConfigurationID()) {
					configContainer.append(configuration);
					lastPersConf = configuration;
				}
				return true;
			}

			CGlobalConfigurationBase *CReasonerCommanderGlobalConfiguration::createNextGlobalConfiguration() {
				CGlobalConfigurationBase *nextConfig = new CGlobalConfigurationBase(lastPersConf,getNextUniqueConfigID());
				return nextConfig;
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
