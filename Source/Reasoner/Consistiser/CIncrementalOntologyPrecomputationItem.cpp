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

#include "CIncrementalOntologyPrecomputationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CIncrementalOntologyPrecomputationItem::CIncrementalOntologyPrecomputationItem() {
			}

			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::initTotallyPrecomputationItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				COntologyPrecomputationItem::initPrecomputationItem(ontology,config);
				mConsistenceCheched = false;
				mConsistence = nullptr;
				mPrecomputation = ontology->getPrecomputation();
				mCalculationConfig = new CCalculationConfigurationExtension(config,0);
				mConsistenceCheckCreated = false;
				mIndividualPrecomputationCreated = false;
				mIndividualPrecomputationChecked = false;

				COntologyProcessingStepDataVector* ontProStepDataVec = mOntology->getProcessingSteps()->getOntologyProcessingStepDataVector();

				mConsistencePrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::CONSISTENCYPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSCONSISTENCY),this);
				mCyclePrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::CYCLEPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPRECOMPUTECYCLES),this);
				mSaturationPrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::SATURATIONPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPRECOMPUTESATURATION),this);
				mIndividualPrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::INDIVIDUALPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPRECOMPUTEINDIVIDUAL),this);
				mProcessingSteps.append(mCyclePrecomputationStep);
				mProcessingSteps.append(mConsistencePrecomputationStep);
				mProcessingSteps.append(mSaturationPrecomputationStep);
				mProcessingSteps.append(mIndividualPrecomputationStep);

				mIndividualComputationRunning = false;

				mIndividualTestRunning = false;
				mLastConsistentlyTestedVersion = nullptr;
				mDirectlyChangedIndividualsIdentified = false;
				mInitTime.start();
				return this;
			}


			CIncrementalOntologyPrecomputationItem::~CIncrementalOntologyPrecomputationItem() {
				delete mCalculationConfig;
				delete mConsistencePrecomputationStep;
				delete mCyclePrecomputationStep;
				delete mSaturationPrecomputationStep;
				delete mIndividualPrecomputationStep;
			}






			COntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::addPrecomputationRequirement(COntologyProcessingRequirement* ontoRequirement) {	
				bool supportRequirement = false;
				COntologyProcessingStepRequirement* stepProcRequirement = dynamic_cast<COntologyProcessingStepRequirement*>(ontoRequirement);
				if (stepProcRequirement) {
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSCONSISTENCY) {
						mConsistencePrecomputationStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					} 
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSPRECOMPUTESATURATION) {
						mSaturationPrecomputationStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					}
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSPRECOMPUTECYCLES) {
						mCyclePrecomputationStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					}
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSPRECOMPUTEINDIVIDUAL) {
						mIndividualPrecomputationStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					}
				} 
				COntologyProcessingConceptSaturationRequirement* conSatRequirement = dynamic_cast<COntologyProcessingConceptSaturationRequirement*>(ontoRequirement);
				if (conSatRequirement) {
					mSaturationPrecomputationStep->addProcessingRequirement(conSatRequirement);
					supportRequirement = true;
				}

				if (!supportRequirement) {
					stepProcRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSUNSUPPORTED);
				}
				return this;
			}



			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::setConsistenceCheched(bool consistenceChecked) {
				mConsistenceCheched = consistenceChecked;
				return this;
			}

			bool CIncrementalOntologyPrecomputationItem::hasConsistenceCheched() {
				return mConsistenceCheched;
			}


			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::setConsistenceCheckCreated(bool consistenceCheckCreated) {
				mConsistenceCheckCreated = consistenceCheckCreated;
				return this;
			}

			bool CIncrementalOntologyPrecomputationItem::hasConsistenceCheckCreated() {
				return mConsistenceCheckCreated;
			}


			CConsistenceObserver* CIncrementalOntologyPrecomputationItem::tellConsistenceData(CConsistenceData* consData) {
				if (mConsData.testAndSetOrdered(nullptr,consData)) {
					consData->overtakeData();
				} else {
					delete consData;
				}
				return this;
			}



			CConsistenceData* CIncrementalOntologyPrecomputationItem::getConsistenceData() {
                return mConsData.fetchAndAddRelaxed(0);
			}


			CConsistence* CIncrementalOntologyPrecomputationItem::getConsistence() {
				return mConsistence;
			}


			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::setConsistence(CConsistence* consistence) {
				mConsistence = consistence;
				return this;
			}

			CCalculationConfigurationExtension* CIncrementalOntologyPrecomputationItem::getCalculationConfiguration() {
				return mCalculationConfig;
			}

			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::setPrecomputation(CPrecomputation* precomputation) {
				mPrecomputation = precomputation;
				return this;
			}
			CPrecomputation* CIncrementalOntologyPrecomputationItem::getPrecomputation() {
				return mPrecomputation;
			}



			CPrecomputationTestingStep* CIncrementalOntologyPrecomputationItem::getConsistencePrecomputationStep() {
				return mConsistencePrecomputationStep;
			}

			bool CIncrementalOntologyPrecomputationItem::isConsistenceStepFinished() {
				return mConsistencePrecomputationStep->isStepFinished();
			}

			bool CIncrementalOntologyPrecomputationItem::isConsistenceStepRequired() {
				return mConsistencePrecomputationStep->hasRequirements();
			}

			bool CIncrementalOntologyPrecomputationItem::areConsistenceStepProcessingRequirementSatisfied() {
				return mConsistencePrecomputationStep->areStepProcessingRequirementSatisfied();
			}

			bool CIncrementalOntologyPrecomputationItem::areCycleStepProcessingRequirementSatisfied() {
				return mCyclePrecomputationStep->areStepProcessingRequirementSatisfied();
			}

			CPrecomputationTestingStep* CIncrementalOntologyPrecomputationItem::getCyclePrecomputationStep() {
				return mCyclePrecomputationStep;
			}

			bool CIncrementalOntologyPrecomputationItem::isCycleStepFinished() {
				return mCyclePrecomputationStep->isStepFinished();
			}

			bool CIncrementalOntologyPrecomputationItem::isCycleStepRequired() {
				return mCyclePrecomputationStep->hasRequirements();
			}

			bool CIncrementalOntologyPrecomputationItem::areSaturationStepProcessingRequirementSatisfied() {
				return mSaturationPrecomputationStep->areStepProcessingRequirementSatisfied();
			}

			CPrecomputationTestingStep* CIncrementalOntologyPrecomputationItem::getSaturationPrecomputationStep() {
				return mSaturationPrecomputationStep;
			}



			bool CIncrementalOntologyPrecomputationItem::isSaturationStepFinished() {
				return mSaturationPrecomputationStep->isStepFinished();
			}

			bool CIncrementalOntologyPrecomputationItem::isSaturationStepRequired() {
				return mSaturationPrecomputationStep->hasRequirements();
			}



			bool CIncrementalOntologyPrecomputationItem::areAllStepFinished() {
				for (QList<CPrecomputationTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if (!(*it)->isStepFinished()) {
						return false;
					}
				}
				return true;
			}

			bool CIncrementalOntologyPrecomputationItem::hasRemainingProcessingRequirements() {
				for (QList<CPrecomputationTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if ((*it)->hasRequirements()) {
						return true;
					}
				}
				return false;
			}





			bool CIncrementalOntologyPrecomputationItem::areIndividualStepProcessingRequirementSatisfied() {
				return mIndividualPrecomputationStep->areStepProcessingRequirementSatisfied();
			}

			CPrecomputationTestingStep* CIncrementalOntologyPrecomputationItem::getIndividualPrecomputationStep() {
				return mIndividualPrecomputationStep;
			}

			bool CIncrementalOntologyPrecomputationItem::isIndividualStepRunning() {
				return mIndividualTestRunning;
			}

			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::setIndividualStepRunning(bool satStepRunning) {
				mIndividualTestRunning = satStepRunning;
				return this;
			}



			bool CIncrementalOntologyPrecomputationItem::isIndividualComputationRunning() {
				return mIndividualComputationRunning;
			}

			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::setIndividualComputationRunning(bool indiCompRunning) {
				mIndividualComputationRunning = indiCompRunning;
				return this;
			}





			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::setIndividualPrecomputationCreated(bool initialized) {
				mIndividualPrecomputationCreated = initialized;
				return this;
			}

			bool CIncrementalOntologyPrecomputationItem::hasIndividualPrecomputationCreated() {
				return mIndividualPrecomputationCreated;
			}


			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::setIndividualPrecomputationChecked(bool checked) {
				mIndividualPrecomputationChecked = checked;
				return this;
			}


			bool CIncrementalOntologyPrecomputationItem::hasIndividualPrecomputationChecked() {
				return mIndividualPrecomputationChecked;
			}


			bool CIncrementalOntologyPrecomputationItem::isIndividualStepFinished() {
				return mIndividualPrecomputationStep->isStepFinished();
			}


			bool CIncrementalOntologyPrecomputationItem::isIndividualStepRequired() {
				return mIndividualPrecomputationStep->hasRequirements();
			}



			CConcreteOntology* CIncrementalOntologyPrecomputationItem::getLastConsistentlyTestedVersion() {
				return mLastConsistentlyTestedVersion;
			}

			CIncrementalOntologyPrecomputationItem* CIncrementalOntologyPrecomputationItem::setLastConsistentlyTestedVersion(CConcreteOntology* ontology) {
				mLastConsistentlyTestedVersion = ontology;
				return this;
			}

			QSet<CIndividual*>* CIncrementalOntologyPrecomputationItem::getDirectlyChangedIndividualSet() {
				return &mDirectlyChangedIndividualSet;
			}

			bool CIncrementalOntologyPrecomputationItem::hasDirectlyChangedIndividualsIdentified() {
				return mDirectlyChangedIndividualsIdentified;
			}

			bool CIncrementalOntologyPrecomputationItem::setDirectlyChangedIndividualsIdentified(bool identified) {
				mDirectlyChangedIndividualsIdentified = identified;
				return this;
			}

			cint64 CIncrementalOntologyPrecomputationItem::getIncrementalExpansionID() {
				return mOntology->getOntologyID();
			}

			QTime* CIncrementalOntologyPrecomputationItem::getInitializationTime() {
				return &mInitTime;
			}


		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
