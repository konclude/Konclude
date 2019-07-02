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

#include "CDirectBlockedProcessingConsistiser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CDirectBlockedProcessingConsistiser::CDirectBlockedProcessingConsistiser() : mConsAdapter(this) {
				mConsistence = nullptr;
			}


			CDirectBlockedProcessingConsistiser::~CDirectBlockedProcessingConsistiser() {
			}


			bool CDirectBlockedProcessingConsistiser::determineConsistency(CConcreteOntology* onto, CConfiguration *config) {
				QList<CIndividual*> indiList;
				CIndividualVector* indiVec = onto->getABox()->getIndividualVector(false);
				if (indiVec) {
					cint64 indiCount = indiVec->getItemCount();
					for (cint64 idx = 0; idx < indiCount; ++idx) {
						CIndividual* indi = indiVec->getData(idx);
						if (indi) {
							indiList.append(indi);
						}
					}
				}
				CReasonerManager* reasonerManager = CConfigManagerReader::readReasonerManagerConfig(config);
				CSatisfiableCalculationJobGenerator satCalcJobGen(onto);
				CSatisfiableCalculationJob* satCalcJob = nullptr;
				bool simpleConsistency = false;
				if (indiList.isEmpty()) {
					CConcept* topConcept = onto->getTBox()->getTopConcept();
					if (topConcept->getOperandCount() > 0) {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(topConcept);
					} else {
						simpleConsistency = true;
					}
				} else {
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(indiList);
					satCalcJob->setSatisfiableTaskPreyingAdapter(&mConsAdapter);
				}
				mConsistence = new CConsistence();
				if (satCalcJob) {
					CConfigurationBase* configBase = nullptr;
					if (config) {
						configBase = dynamic_cast<CConfigurationBase*>(config);
					}
					CCalculationConfigurationExtension calcConfig(configBase,0);
					satCalcJob->setCalculationConfiguration(&calcConfig);
					CJobSatisfiableResult* result = reasonerManager->reasoningSatisfiableCalcualtionJob(satCalcJob);
					mConsistence->setOntologyConsistent(result->isSatisfiable());
				} else {
					mConsistence->setOntologyConsistent(simpleConsistency);
				}
				onto->setConsistence(mConsistence);

				return this;
			}

			
			CConsistenceObserver* CDirectBlockedProcessingConsistiser::tellConsistenceData(CConsistenceData* consData) {
				mSynMutex.lock();
				if (!mConsistence->hasConsistenceModelData()) {
					mConsistence->setConsistenceModelData(consData);
					consData->overtakeData();
				} else {
					delete consData;
				}
				mSynMutex.unlock();
				return this;
			}


		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
