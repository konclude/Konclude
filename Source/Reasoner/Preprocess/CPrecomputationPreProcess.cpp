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
