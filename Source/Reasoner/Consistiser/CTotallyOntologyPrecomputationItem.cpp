/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CTotallyOntologyPrecomputationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CTotallyOntologyPrecomputationItem::CTotallyOntologyPrecomputationItem() {
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::initTotallyPrecomputationItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				COntologyPrecomputationItem::initPrecomputationItem(ontology,config);
				mConsistenceCheched = false;
				mConsistence = nullptr;
				mPrecomputation = ontology->getPrecomputation();
				mCalculationConfig = new CCalculationConfigurationExtension(config,0);
				mConsistenceCheckCreated = false;
				mConceptCyclePrecomputationChecked = false;
				mConceptCyclePrecomputationInitialized = false;
				mMinConceptCycleTestSize = 0;
				mConceptSaturationPrecomputationChecked = false;
				mConceptSaturationPrecomputationCreated = false;
				mSaturationTestRunning = false;

				COntologyProcessingStepDataVector* ontProStepDataVec = mOntology->getProcessingSteps()->getOntologyProcessingStepDataVector();

				mConsistencePrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::CONSISTENCYPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSCONSISTENCY),this);
				mCyclePrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::CYCLEPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPRECOMPUTECYCLES),this);
				mSaturationPrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::SATURATIONPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPRECOMPUTESATURATION),this);
				mProcessingSteps.append(mCyclePrecomputationStep);
				mProcessingSteps.append(mConsistencePrecomputationStep);
				mProcessingSteps.append(mSaturationPrecomputationStep);

				mApproxSatCalcJob = nullptr;
				mLastSaturData = nullptr;
				mAllConSatRequired = false;
				mAllConSatOrdered = false;
				mNomDelayedConSatUp = false;
				mSaturationComputationRunning = false;

				mFailAfterConsistencyConceptSaturation = CConfigDataReader::readConfigBoolean(config,"Konclude.Debug.FailAfterConsistencyConceptSaturation",false);
				mFailAfterConsistencyChecking = CConfigDataReader::readConfigBoolean(config,"Konclude.Debug.FailAfterConsistencyCheck",false);
				mFailAfterConceptSaturation = CConfigDataReader::readConfigBoolean(config,"Konclude.Debug.FailAfterConceptSaturation",false);
				return this;
			}


			CTotallyOntologyPrecomputationItem::~CTotallyOntologyPrecomputationItem() {
				delete mCalculationConfig;
				delete mConsistencePrecomputationStep;
				delete mCyclePrecomputationStep;
				delete mSaturationPrecomputationStep;
			}




			bool CTotallyOntologyPrecomputationItem::failAfterConsistencyConceptSaturation() {
				return mFailAfterConsistencyConceptSaturation;
			}

			bool CTotallyOntologyPrecomputationItem::failAfterConsistencyChecking() {
				return mFailAfterConsistencyChecking;
			}

			bool CTotallyOntologyPrecomputationItem::failAfterConceptSaturation() {
				return mFailAfterConceptSaturation;
			}




			COntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::addPrecomputationRequirement(COntologyProcessingRequirement* ontoRequirement) {	
				bool supportRequirement = false;
				COntologyProcessingStepRequirement* stepProcRequirement = dynamic_cast<COntologyProcessingStepRequirement*>(ontoRequirement);
				if (stepProcRequirement) {
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSCONSISTENCY) {
						mConsistencePrecomputationStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					} 
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSPRECOMPUTESATURATION) {
						mSaturationPrecomputationStep->addProcessingRequirement(stepProcRequirement);
						mAllConSatRequired = true;
						supportRequirement = true;
					}
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSPRECOMPUTECYCLES) {
						mCyclePrecomputationStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					}
				} 
				COntologyProcessingConceptSaturationRequirement* conSatRequirement = dynamic_cast<COntologyProcessingConceptSaturationRequirement*>(ontoRequirement);
				if (conSatRequirement) {
					addRequiredSaturationConcept(conSatRequirement->getConcept(),conSatRequirement->getConceptNegation());
					mSaturationPrecomputationStep->addProcessingRequirement(conSatRequirement);
					supportRequirement = true;
				}

				if (!supportRequirement) {
					stepProcRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSUNSUPPORTED);
				}
				return this;
			}



			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setConsistenceCheched(bool consistenceChecked) {
				mConsistenceCheched = consistenceChecked;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasConsistenceCheched() {
				return mConsistenceCheched;
			}


			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setConsistenceCheckCreated(bool consistenceCheckCreated) {
				mConsistenceCheckCreated = consistenceCheckCreated;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasConsistenceCheckCreated() {
				return mConsistenceCheckCreated;
			}


			CConsistenceObserver* CTotallyOntologyPrecomputationItem::tellConsistenceData(CConsistenceData* consData) {
				if (mConsData.testAndSetOrdered(nullptr,consData)) {
					consData->overtakeData();
				} else {
					delete consData;
				}
				return this;
			}



			CSaturationObserver* CTotallyOntologyPrecomputationItem::tellSaturationData(CSaturationData* saturationData) {
				//if (mSaturData.testAndSetOrdered(nullptr,saturationData)) {
				//	saturationData->overtakeData();
				//} else {
				//	delete saturationData;
				//}
				saturationData->overtakeData();
				mSaturDataAddMutex.lock();
				mLastSaturData = saturationData;
				mSaturDataList.append(mLastSaturData);
				mSaturDataAddMutex.unlock();
				return this;
			}

			CSaturationData* CTotallyOntologyPrecomputationItem::getSaturationData() {
				return mLastSaturData;
			}



			CConsistenceData* CTotallyOntologyPrecomputationItem::getConsistenceData() {
                return mConsData.fetchAndAddRelaxed(0);
			}


			CConsistence* CTotallyOntologyPrecomputationItem::getConsistence() {
				return mConsistence;
			}


			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setConsistence(CConsistence* consistence) {
				mConsistence = consistence;
				return this;
			}

			CCalculationConfigurationExtension* CTotallyOntologyPrecomputationItem::getCalculationConfiguration() {
				return mCalculationConfig;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setPrecomputation(CPrecomputation* precomputation) {
				mPrecomputation = precomputation;
				return this;
			}
			CPrecomputation* CTotallyOntologyPrecomputationItem::getPrecomputation() {
				return mPrecomputation;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setConceptCyclePrecomputationInitialized(bool initialized) {
				mConceptCyclePrecomputationInitialized = initialized;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasConceptCyclePrecomputationInitialized() {
				return mConceptCyclePrecomputationInitialized;
			}


			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setConceptCyclePrecomputationChecked(bool checked) {
				mConceptCyclePrecomputationChecked = checked;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasConceptCyclePrecomputationChecked() {
				return mConceptCyclePrecomputationChecked;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setConceptSaturationPrecomputationCreated(bool initialized) {
				mConceptSaturationPrecomputationCreated = initialized;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasConceptSaturationPrecomputationCreated() {
				return mConceptSaturationPrecomputationCreated;
			}


			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setConceptSaturationPrecomputationChecked(bool checked) {
				mConceptSaturationPrecomputationChecked = checked;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasConceptSaturationPrecomputationChecked() {
				return mConceptSaturationPrecomputationChecked;
			}


			bool CTotallyOntologyPrecomputationItem::requiresAllConceptsSaturation() {
				return mAllConSatRequired;
			}

			bool CTotallyOntologyPrecomputationItem::hasAllConceptsSaturationOrdered() {
				return mAllConSatOrdered;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setAllConceptsSaturationOrdered(bool ordered) {
				mAllConSatOrdered = ordered;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::requiresNominalDelayedConceptsSaturationUpdate() {
				return mNomDelayedConSatUp;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setNominalDelayedConceptsSaturationUpdateRequired(bool updateRequired) {
				mNomDelayedConSatUp = updateRequired;
				return this;
			}



			QList<CSaturationConceptDataItem*>* CTotallyOntologyPrecomputationItem::getSaturationOrderedItemList() {
				return &mSatOrderedItemList;
			}



			bool CTotallyOntologyPrecomputationItem::hasRemainingRequiredSaturationConcepts() {
				return !mRemainingReqSatConList.isEmpty() || mAllConSatRequired && !mAllConSatOrdered || mNomDelayedConSatUp;
			}

			QList<TConceptNegPair>* CTotallyOntologyPrecomputationItem::getRemainingRequiredSaturationConceptList() {
				return &mRemainingReqSatConList;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::addRequiredSaturationConcept(CConcept* concept, bool negation) {
				if (!mAllConSatRequired) {
					mRemainingReqSatConList.append(TConceptNegPair(concept,negation));
				}
				return this;
			}




			bool CTotallyOntologyPrecomputationItem::hasRemainingConsistencyRequiredSaturationConcepts() {
				return !mRemainingConsReqSatConList.isEmpty();
			}

			QList<TConceptNegPair>* CTotallyOntologyPrecomputationItem::getRemainingConsistencyRequiredSaturationConceptList() {
				return &mRemainingConsReqSatConList;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::addConsistencyRequiredSaturationConcept(CConcept* concept, bool negation) {
				mRemainingConsReqSatConList.append(TConceptNegPair(concept,negation));
				return this;
			}







			QList<CConceptCycleData*>* CTotallyOntologyPrecomputationItem::getRemainingConceptCyclePrecomputationList() {
				return &mRemainingConceptCyclePrecomputationList;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::addRemainingConceptCyclePrecomputation(CConceptCycleData* conceptCycleData) {
				mRemainingConceptCyclePrecomputationList.append(conceptCycleData);
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasRemainingConceptCyclePrecomputation() {
				return !mRemainingConceptCyclePrecomputationList.isEmpty();
			}

			cint64 CTotallyOntologyPrecomputationItem::getMinConceptCycleTestSize() {
				return mMinConceptCycleTestSize;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setMinConceptCycleTestSize(cint64 minSize) {
				mMinConceptCycleTestSize = minSize;
				return this;
			}


			CPrecomputationTestingStep* CTotallyOntologyPrecomputationItem::getConsistencePrecomputationStep() {
				return mConsistencePrecomputationStep;
			}

			bool CTotallyOntologyPrecomputationItem::isConsistenceStepFinished() {
				return mConsistencePrecomputationStep->isStepFinished();
			}

			bool CTotallyOntologyPrecomputationItem::isConsistenceStepRequired() {
				return mConsistencePrecomputationStep->hasRequirements();
			}

			bool CTotallyOntologyPrecomputationItem::areConsistenceStepProcessingRequirementSatisfied() {
				return mConsistencePrecomputationStep->areStepProcessingRequirementSatisfied();
			}

			bool CTotallyOntologyPrecomputationItem::areCycleStepProcessingRequirementSatisfied() {
				return mCyclePrecomputationStep->areStepProcessingRequirementSatisfied();
			}

			CPrecomputationTestingStep* CTotallyOntologyPrecomputationItem::getCyclePrecomputationStep() {
				return mCyclePrecomputationStep;
			}

			bool CTotallyOntologyPrecomputationItem::isCycleStepFinished() {
				return mCyclePrecomputationStep->isStepFinished();
			}

			bool CTotallyOntologyPrecomputationItem::isCycleStepRequired() {
				return mCyclePrecomputationStep->hasRequirements();
			}

			bool CTotallyOntologyPrecomputationItem::areSaturationStepProcessingRequirementSatisfied() {
				return mSaturationPrecomputationStep->areStepProcessingRequirementSatisfied();
			}

			CPrecomputationTestingStep* CTotallyOntologyPrecomputationItem::getSaturationPrecomputationStep() {
				return mSaturationPrecomputationStep;
			}

			bool CTotallyOntologyPrecomputationItem::isSaturationStepRunning() {
				return mSaturationTestRunning;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setSaturationStepRunning(bool satStepRunning) {
				mSaturationTestRunning = satStepRunning;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::isSaturationComputationRunning() {
				return mSaturationComputationRunning;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setSaturationComputationRunning(bool satCompRunning) {
				mSaturationComputationRunning = satCompRunning;
				return this;
			}



			bool CTotallyOntologyPrecomputationItem::isSaturationStepFinished() {
				return mSaturationPrecomputationStep->isStepFinished();
			}

			bool CTotallyOntologyPrecomputationItem::isSaturationStepRequired() {
				return mSaturationPrecomputationStep->hasRequirements();
			}

			bool CTotallyOntologyPrecomputationItem::areAllStepFinished() {
				for (QList<CPrecomputationTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if (!(*it)->isStepFinished()) {
						return false;
					}
				}
				return true;
			}

			bool CTotallyOntologyPrecomputationItem::hasRemainingProcessingRequirements() {
				for (QList<CPrecomputationTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if ((*it)->hasRequirements()) {
						return true;
					}
				}
				return false;
			}



			CSaturationConceptDataItem* CTotallyOntologyPrecomputationItem::getSaturationConceptDataItem(CConcept* satConcept, bool negated, bool create) {
				CSaturationConceptDataItem* item = nullptr;
				CConceptProcessData* conProcData = (CConceptProcessData*)satConcept->getConceptData();
				CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
				if (conRefLinking) {
					CConceptSaturationReferenceLinkingData* conRefSatLinking = (CConceptSaturationReferenceLinkingData*)conRefLinking;
					item = (CSaturationConceptDataItem*)conRefSatLinking->getConceptSaturationReferenceLinkingData(negated);
				}
				if (!item && create) {
					item = new CSaturationConceptDataItem();
					item->initConceptSaturationTestingItem(satConcept,negated,nullptr);
					mConceptNegationSatItemHash.insert(TConceptNegPair(satConcept,negated),item);
					mSatItemContainer.append(item);
				}
				return item;
			}


			CSaturationConceptDataItem* CTotallyOntologyPrecomputationItem::getSaturationRoleSuccessorConceptDataItem(CRole* role, CConcept* satConcept, bool negated, bool create) {
				CSaturationConceptDataItem* item = nullptr;
				item = mRoleConceptNegationSatItemHash.value(TRoleConceptNegTriple(role,TConceptNegPair(satConcept,negated)));
				if (!item && create) {
					item = new CSaturationConceptDataItem();
					item->initConceptSaturationTestingItem(satConcept,negated,role);
					mRoleConceptNegationSatItemHash.insert(TRoleConceptNegTriple(role,TConceptNegPair(satConcept,negated)),item);
					mSatItemContainer.append(item);
				}
				return item;
			}

			CApproximatedSaturationCalculationJob* CTotallyOntologyPrecomputationItem::getSaturationCalculationJob() {
				return mApproxSatCalcJob;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setSaturationCalculationJob(CApproximatedSaturationCalculationJob* calculationJob) {
				mApproxSatCalcJob = calculationJob;
				return this;
			}

			QSet<CConcept*>* CTotallyOntologyPrecomputationItem::getSaturatedDisjunctionSet() {
				return &mDisjunctionSaturationSet;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::addSaturatedDisjunction(CConcept* disjunctionConcept) {
				mDisjunctionSaturationSet.insert(disjunctionConcept);
				return this;
			}

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
