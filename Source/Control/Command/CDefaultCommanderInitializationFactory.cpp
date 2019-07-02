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

#include "CDefaultCommanderInitializationFactory.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CDefaultCommanderInitializationFactory::CDefaultCommanderInitializationFactory(CReasonerManager *useReasonerManager, CClassificationManager *useClassificationManager, COntologyRevisionManager *useOntologyRevisionManager) {
				useReasonerMan = useReasonerManager;
				useClassificationMan = useClassificationManager;
				useOntRevMan = useOntologyRevisionManager;
			}


			CDefaultCommanderInitializationFactory::~CDefaultCommanderInitializationFactory() {
			}


			CReasonerManager *CDefaultCommanderInitializationFactory::createReasonerManager(CConfigurationProvider *configProvider) {

				CReasonerManager *reasonerManager = 0;
				bool generatedReasoner = false;

				if (useReasonerMan) {
					reasonerManager = useReasonerMan;
					generatedReasoner = true;
				}

				CConfigurationBase *config = 0;
				if (configProvider) {
					config = configProvider->getCurrentConfiguration();
				}

				if (config) {
					CConfigData *confData = 0;

					if (!generatedReasoner) {
						confData = config->getRelatedConfigChange("Konclude.Execution.DefaultReasonerManager");
						if (confData) {
							QString reasonerString = confData->getString();
							if (reasonerString == "ReasonerManager") {
								reasonerManager = new CReasonerManagerThread();
								generatedReasoner = true;
							} else if (reasonerString == "ASAnalyseReasonerManager") {
								reasonerManager = new CAnalyseReasonerManager();
								generatedReasoner = true;
							} else if (reasonerString == "ASExperimentalReasonerManager") {
								reasonerManager = new CExperimentalReasonerManager();
								generatedReasoner = true;
							} 
						}
					}
				}

				if (!generatedReasoner) {
					reasonerManager = new CReasonerManagerThread();
					generatedReasoner = true;
				}
				return reasonerManager;
			}

			CClassificationManager *CDefaultCommanderInitializationFactory::createClassificationManager(CConfigurationProvider *configProvider) {
				CClassificationManager *classificationManager = 0;
				bool generatedClassification = false;

				if (useClassificationMan) {
					classificationManager = useClassificationMan;
					generatedClassification = true;
				}

				CConfigurationBase *config = 0;
				if (configProvider) {
					config = configProvider->getCurrentConfiguration();
				}

				if (!generatedClassification) {
					classificationManager = new CClassificationManager();
					generatedClassification = true;
				}

				return classificationManager;
			}

			COntologyRevisionManager *CDefaultCommanderInitializationFactory::createOntologyRevisionManager(CConfigurationProvider *configProvider) {
				COntologyRevisionManager *ontRevManager = 0;
				bool generatedOntRevMan = false;

				if (useOntRevMan) {
					ontRevManager = useOntRevMan;
					generatedOntRevMan = true;
				}

				CConfigurationBase *config = 0;
				if (configProvider) {
					config = configProvider->getCurrentConfiguration();
				}

				if (!generatedOntRevMan) {
					CConfigurationBasedOntologyRevisionPersistencerFactory* persistencerFactory = new CConfigurationBasedOntologyRevisionPersistencerFactory(config);
					ontRevManager = new CSPOntologyRevisionManager(persistencerFactory->createOntologyRevisionPersistencer());
					delete persistencerFactory;
					generatedOntRevMan = true;
				}

				return ontRevManager;
			}

			CCommanderInitializationFactory *CDefaultCommanderInitializationFactory::initializeReasonerManager(CReasonerManager *reasonerManager, CConfigurationProvider *configProvider) {
				reasonerManager->initializeManager(configProvider);
				return this;
			}

			CCommanderInitializationFactory *CDefaultCommanderInitializationFactory::initializeClassificationManager(CClassificationManager *classificationManager, CConfigurationProvider *configProvider) {
				CReasonerManager *reasonerManager = CConfigManagerReader::readReasonerManagerConfig(configProvider);
				CSubsumptionClassifierFactory *subsumpFactory = new CConfigDependedSubsumptionClassifierFactory(reasonerManager);
				classificationManager->initializeManager(subsumpFactory,configProvider);
				return this;
			}

			CCommanderInitializationFactory *CDefaultCommanderInitializationFactory::initializeOntologyRevisionManager(COntologyRevisionManager *ontologyRevisionManager, CConfigurationProvider *configProvider) {
				ontologyRevisionManager->initializeManager(configProvider);
				return this;
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
