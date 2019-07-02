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

#include "CPrecomputationPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CPrecomputationPreProcess::CPrecomputationPreProcess() {
			}


			CPrecomputationPreProcess::~CPrecomputationPreProcess() {
			}


			CConcreteOntology *CPrecomputationPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				CConfiguration* config = context->getConfiguration();
				if (ontology && config) {
					CReasonerManager* reasonerManager = CConfigManagerReader::readReasonerManagerConfig(config);
					if (reasonerManager) {
						CConfigurationBase* configBase = dynamic_cast<CConfigurationBase*>(config);
						if (configBase) {
							bool precomputed = false;
							CPrecomputationManager* precomputationManager = reasonerManager->getPrecomputationManager();
							CPrecomputator* precomputator = precomputationManager->getPrecomputator(ontology,configBase);
							if (precomputator) {
								precomputed = precomputator->precompute(ontology,configBase,QList<COntologyProcessingRequirement*>());
							}
							
							bool consistent = false;
							CConsistence* consistence = ontology->getConsistence();
							if (consistence) {
								consistent = consistence->isOntologyConsistent();
								QString consString("inconsistent");
								if (consistent) {
									consString = "consistent";
								}
								LOG(INFO,"::Konclude::Reasoner::Preprocess::OntologyConsistencyChecking",logTr("Ontology is %1.").arg(consString),this);
							}
						}
					}
				}
				return ontology;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
