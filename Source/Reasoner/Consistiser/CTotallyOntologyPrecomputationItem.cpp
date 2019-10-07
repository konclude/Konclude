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
				mIndividualPrecomputationCreated = false;
				mIndividualPrecomputationChecked = false;
				mIndividualPrecomputationClashed = false;

				COntologyProcessingStepDataVector* ontProStepDataVec = mOntology->getProcessingSteps()->getOntologyProcessingStepDataVector();

				mConsistencePrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::CONSISTENCYPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSCONSISTENCY),this);
				mCyclePrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::CYCLEPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPRECOMPUTECYCLES),this);
				mOccurrenceStatisticsPrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::OCCURRENCESTATISTICSPRECOMPUTATIONSTEP, ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPRECOMPUTEOCCURRENCESTATISTICS), this);

				mSaturationPrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::SATURATIONPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPRECOMPUTESATURATION),this);
				mIndividualPrecomputationStep = new CPrecomputationTestingStep(CPrecomputationTestingStep::INDIVIDUALPRECOMPUTATIONSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSPRECOMPUTEINDIVIDUAL),this);
				mProcessingSteps.append(mCyclePrecomputationStep);
				mProcessingSteps.append(mOccurrenceStatisticsPrecomputationStep);
				mProcessingSteps.append(mConsistencePrecomputationStep);
				mProcessingSteps.append(mSaturationPrecomputationStep);
				mProcessingSteps.append(mIndividualPrecomputationStep);

				mApproxSatCalcJob = nullptr;
				mLastSaturData = nullptr;
				mAllConSatRequired = false;
				mAllConSatOrdered = false;
				mNomDelayedConSatUpRequired = false;
				mNomDelayedConSatUpdated = false;
				mSaturationComputationRunning = false;
				mIndividualComputationRunning = false;

				mAllAssertionIndividual = nullptr;
				mAllAssertionIndividualSaturated = false;
				mAllAssertionIndividualSufficientSaturationChecked = false;
				mAllAssertionIndividualSaturationSufficient = false;

				mIndividualSaturationClashed = false;
				mIndividualSaturationInsufficient = false;
				mIndividualSaturated = false;
				mAllIndividualSaturaturationOrderd = false;

				mIndividualSaturationRunningCount = 0;

				mFreeItemLinker = nullptr;
				mNextSaturationID = 1;
				mIndividualTestRunning = false;
				mIndividualsSaturationCacheSynchronisation = false;
				mTriplesIndexedIndisSaturated = false;

				mHandledTriplesIndiSaturatedId = -1;

				mAllIncompIndiRetrieved = false;
				mCurrentIncompIndiRetrievalLimit = 100000;
				mCurrentIncompIndiComputationLimit = 1000;
				mLastMinRetrievedIncompIndiId = -1;

				mIndividualComputationRunningCount = 0;

				mCurrentIndiCompCoordHash = nullptr;

				mFullCompletionGraphConstruction = false;
				mFullCompletionGraphConstructed = false;
				mForceCompletionGraphConstruction = false;

				mSaturationOccurrenceStatisticsCollectingInitialized = false;
				mSaturationOccurrenceStatisticsCollected = false;

				mFailAfterConsistencyConceptSaturation = CConfigDataReader::readConfigBoolean(config,"Konclude.Debug.FailAfterConsistencyConceptSaturation",false);
				mFailAfterConsistencyChecking = CConfigDataReader::readConfigBoolean(config,"Konclude.Debug.FailAfterConsistencyCheck",false);
				mFailAfterConceptSaturation = CConfigDataReader::readConfigBoolean(config,"Konclude.Debug.FailAfterConceptSaturation",false);
				
				mInitTime.start();
				return this;
			}




			CTotallyOntologyPrecomputationItem::~CTotallyOntologyPrecomputationItem() {
				delete mCalculationConfig;
				delete mConsistencePrecomputationStep;
				delete mCyclePrecomputationStep;
				delete mOccurrenceStatisticsPrecomputationStep;
				delete mSaturationPrecomputationStep;
				delete mIndividualPrecomputationStep;

				if (mAllAssertionIndividual) {
					CConceptAssertionLinker* conAssLinker = mAllAssertionIndividual->getAssertionConceptLinker();
					while (conAssLinker) {
						CConceptAssertionLinker* tmpConAssLinker = conAssLinker;
						conAssLinker = conAssLinker->getNext();
						delete tmpConAssLinker;
					}

					CRoleAssertionLinker* roleAssLinker = mAllAssertionIndividual->getAssertionRoleLinker();
					while (roleAssLinker) {
						CRoleAssertionLinker* tmpRoleAssLinker = roleAssLinker;
						roleAssLinker = roleAssLinker->getNext();
						delete tmpRoleAssLinker;
					}
					delete mAllAssertionIndividual;
				}
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
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSPRECOMPUTEOCCURRENCESTATISTICS) {
						mOccurrenceStatisticsPrecomputationStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
					}
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSPRECOMPUTEINDIVIDUAL) {
						mIndividualPrecomputationStep->addProcessingRequirement(stepProcRequirement);
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
				return mNomDelayedConSatUpRequired;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setNominalDelayedConceptsSaturationUpdateRequired(bool updateRequired) {
				mNomDelayedConSatUpRequired = updateRequired;
				return this;
			}



			QList<CSaturationConceptDataItem*>* CTotallyOntologyPrecomputationItem::getSaturationOrderedItemList() {
				return &mSatOrderedItemList;
			}



			bool CTotallyOntologyPrecomputationItem::hasRemainingRequiredSaturationConcepts() {
				return !mRemainingReqSatConList.isEmpty() || mAllConSatRequired && !mAllConSatOrdered || mNomDelayedConSatUpRequired && !mNomDelayedConSatUpdated;
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




			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setTripleIndexedIndividualSaturated(bool saturated) {
				mTriplesIndexedIndisSaturated = saturated;
				return this;
			}



			bool CTotallyOntologyPrecomputationItem::hasRemainingRequiredSaturationIndividuals() {
				return !mRemainingABoxIndiSatList.isEmpty() || !mTriplesIndexedIndisSaturated;
			}


			bool CTotallyOntologyPrecomputationItem::hasRemainingRequiredABoxSaturationIndividuals() {
				return !mRemainingABoxIndiSatList.isEmpty();
			}

			QList<CIndividual*>* CTotallyOntologyPrecomputationItem::getRemainingRequiredABoxSaturationIndividuals() {
				return &mRemainingABoxIndiSatList;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::addRequiredABoxSaturationIndividual(CIndividual* individual) {
				mRemainingABoxIndiSatList.append(individual);
				return this;
			}



			cint64 CTotallyOntologyPrecomputationItem::getHandledTriplesIndividualSaturatedId() {
				return mHandledTriplesIndiSaturatedId;
			}


			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setHandledTriplesIndividualSaturatedId(cint64 handledId) {
				mHandledTriplesIndiSaturatedId = handledId;
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



			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setSaturationOccurrenceStatisticsCollectingInitialized(bool initialized) {
				mSaturationOccurrenceStatisticsCollectingInitialized = initialized;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasSaturationOccurrenceStatisticsCollectingInitialized() {
				return mSaturationOccurrenceStatisticsCollectingInitialized;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setSaturationOccurrenceStatisticsCollected(bool checked) {
				mSaturationOccurrenceStatisticsCollected = checked;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasSaturationOccurrenceStatisticsCollected() {
				return mSaturationOccurrenceStatisticsCollected;
			}


			bool CTotallyOntologyPrecomputationItem::areOccurrenceStatisticsStepProcessingRequirementSatisfied() {
				return mOccurrenceStatisticsPrecomputationStep->areStepProcessingRequirementSatisfied();
			}

			CPrecomputationTestingStep* CTotallyOntologyPrecomputationItem::getOccurrenceStatisticsPrecomputationStep() {
				return mOccurrenceStatisticsPrecomputationStep;
			}

			bool CTotallyOntologyPrecomputationItem::isOccurrenceStatisticsStepFinished() {
				return mOccurrenceStatisticsPrecomputationStep->isStepFinished();
			}

			bool CTotallyOntologyPrecomputationItem::isOccurrenceStatisticsStepRequired() {
				return mOccurrenceStatisticsPrecomputationStep->hasRequirements();
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

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::incIndividualSaturationRunningCount(cint64 incCount) {
				mIndividualSaturationRunningCount += incCount;
				return this;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::decIndividualSaturationRunningCount(cint64 decCount) {
				mIndividualSaturationRunningCount -= decCount;
				return this;
			}

			cint64 CTotallyOntologyPrecomputationItem::getIndividualSaturationRunningCount() {
				return mIndividualSaturationRunningCount;
			}

			bool CTotallyOntologyPrecomputationItem::hasIndividualSaturationRunning() {
				return mIndividualSaturationRunningCount > 0;
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


			CIndividual* CTotallyOntologyPrecomputationItem::getAllAssertionIndividual() {
				return mAllAssertionIndividual;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setAllAssertionIndividual(CIndividual* individual) {
				mAllAssertionIndividual = individual;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::isAllAssertionIndividualSaturated() {
				return mAllAssertionIndividualSaturated;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setAllAssertionIndividualSaturated(bool saturated) {
				mAllAssertionIndividualSaturated = saturated;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::hasAllAssertionIndividualSufficientSaturationChecked() {
				return mAllAssertionIndividualSufficientSaturationChecked;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setAllAssertionIndividualSufficientSaturationChecked(bool sufficientSaturatedChecked) {
				mAllAssertionIndividualSufficientSaturationChecked = sufficientSaturatedChecked;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::isAllAssertionIndividualSaturationSufficient() {
				return mAllAssertionIndividualSaturationSufficient;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setAllAssertionIndividualSaturationSufficient(bool sufficientSaturated) {
				mAllAssertionIndividualSaturationSufficient = sufficientSaturated;
				return this;
			}


			CSaturationIndividualDataItem* CTotallyOntologyPrecomputationItem::takeFreeSaturationIndividualDataItem(bool create) {
				CSaturationIndividualDataItem* item = mFreeItemLinker;
				if (mFreeItemLinker) {
					mFreeItemLinker = mFreeItemLinker->getNext();
				}
				if (item) {
					item->clearNext();
				}
				if (!item) {
					item = new CSaturationIndividualDataItem();
				}
				return item;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::addFreeSaturationIndividualDataItem(CSaturationIndividualDataItem* item) {
				if (item) {
					for (CSaturationIndividualDataItem* itemIt = item; itemIt; itemIt = itemIt->getNext()) {
						itemIt->initConceptSaturationTestingItem(nullptr,0);
					}
					mFreeItemLinker = item->append(mFreeItemLinker);
				}
				return this;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setSaturationIDIndividualDataItems(cint64 saturationID, CSaturationIndividualDataItem* items) {
				mSatIDIndividualItemsHash.insert(saturationID,items);
				return this;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::releaseSaturationIDIndividualDataItems(cint64 saturationID) {
				CSaturationIndividualDataItem* items = mSatIDIndividualItemsHash.value(saturationID);
				mSatIDIndividualItemsHash.insert(saturationID,nullptr);
				addFreeSaturationIndividualDataItem(items);
				return this;
			}

			cint64 CTotallyOntologyPrecomputationItem::getNextSaturationID(bool moveNext) {
				cint64 nextID = mNextSaturationID;
				if (moveNext) {
					++mNextSaturationID;
				}
				return nextID;
			}



			CSaturationIndividualAnalysationObserver* CTotallyOntologyPrecomputationItem::notifyClashedIndividual() {
				mIndividualSaturationClashed = true;
				return this;
			}

			CSaturationIndividualAnalysationObserver* CTotallyOntologyPrecomputationItem::notifyInsufficientIndividual() {
				mIndividualSaturationInsufficient = true;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::hasClashedSaturationIndividuals() {
				return mIndividualSaturationClashed;
			}

			bool CTotallyOntologyPrecomputationItem::hasInsufficientSaturationIndividuals() {
				return mIndividualSaturationInsufficient;
			}



			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setForceCompletionGraphConstruction(bool forceCompletionGraphConstruction) {
				mForceCompletionGraphConstruction = forceCompletionGraphConstruction;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::isForceCompletionGraphConstruction() {
				return mForceCompletionGraphConstruction;
			}


			bool CTotallyOntologyPrecomputationItem::isFullCompletionGraphConstruction() {
				return mFullCompletionGraphConstruction;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setFullCompletionGraphConstruction(bool fullCompletionGraphConstruction) {
				mFullCompletionGraphConstruction = fullCompletionGraphConstruction;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::isFullCompletionGraphConstructed() {
				return mFullCompletionGraphConstructed;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setFullCompletionGraphConstructed(bool fullCompletionGraphConstructed) {
				mFullCompletionGraphConstructed = fullCompletionGraphConstructed;
				return this;
			}




			bool CTotallyOntologyPrecomputationItem::hasIndividualsSaturated() {
				return mIndividualSaturated;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setIndividualsSaturated(bool saturated) {
				mIndividualSaturated = saturated;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::hasALLIndividualsSaturationOrderd() {
				return mAllIndividualSaturaturationOrderd;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setALLIndividualsSaturationOrderd(bool allSaturationOrderd) {
				mAllIndividualSaturaturationOrderd = allSaturationOrderd;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::hasNominalDelayedConceptsSaturationUpdated() {
				return mNomDelayedConSatUpdated;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setNominalDelayedConceptsSaturationUpdated(bool updated) {
				mNomDelayedConSatUpdated = updated;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::hasIndividualsSaturationCacheSynchronisation() {
				return mIndividualsSaturationCacheSynchronisation;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setIndividualsSaturationCacheSynchronisation(bool synchronized) {
				mIndividualsSaturationCacheSynchronisation = synchronized;
				return this;
			}

			QSet<CIndividualReference>* CTotallyOntologyPrecomputationItem::getIncompletelyHandledIndividualSet() {
				return &mIncompHandledIndiSet;
			}


			CIndividualPrecomputationCoordinationHash* CTotallyOntologyPrecomputationItem::getCurrentIndividualComputationCoordinationHash() {
				return mCurrentIndiCompCoordHash;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setCurrentIndividualComputationCoordinationHash(CIndividualPrecomputationCoordinationHash* coordHash) {
				mCurrentIndiCompCoordHash = coordHash;
				return this;
			}


			QSet<CIndividualReference>* CTotallyOntologyPrecomputationItem::getCurrentIndividualComputationSet() {
				return &mCurrentIndiComputationSet;
			}

			cint64 CTotallyOntologyPrecomputationItem::getCurrentIncompletelyHandledIndividualRetrievalLimit() {
				return mCurrentIncompIndiRetrievalLimit;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setCurrentIncompletelyHandledIndividualRetrievalLimit(cint64 limit) {
				mCurrentIncompIndiRetrievalLimit = limit;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasAllIncompletelyHandledIndividualsRetrieved() {
				return mAllIncompIndiRetrieved;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setAllIncompletelyHandledIndividualsRetrieved(bool allRetrieved) {
				mAllIncompIndiRetrieved = allRetrieved;
				return this;
			}

			cint64 CTotallyOntologyPrecomputationItem::getLastMinimumRetrievedIncompletelyHandledIndividualId() {
				return mLastMinRetrievedIncompIndiId;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setLastMinimumRetrievedIncompletelyHandledIndividualId(cint64 minId) {
				mLastMinRetrievedIncompIndiId = minId;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::areIndividualStepProcessingRequirementSatisfied() {
				return mIndividualPrecomputationStep->areStepProcessingRequirementSatisfied();
			}

			CPrecomputationTestingStep* CTotallyOntologyPrecomputationItem::getIndividualPrecomputationStep() {
				return mIndividualPrecomputationStep;
			}

			bool CTotallyOntologyPrecomputationItem::isIndividualStepRunning() {
				return mIndividualTestRunning;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setIndividualStepRunning(bool satStepRunning) {
				mIndividualTestRunning = satStepRunning;
				return this;
			}



			bool CTotallyOntologyPrecomputationItem::isIndividualComputationRunning() {
				return mIndividualComputationRunningCount > 0;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setIndividualComputationRunning(bool indiCompRunning) {
				mIndividualComputationRunning = indiCompRunning;
				return this;
			}


			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::incIndividualComputationRunningCount(cint64 incCount) {
				mIndividualComputationRunningCount += incCount;
				return this;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::decIndividualComputationRunningCount(cint64 decCount) {
				mIndividualComputationRunningCount -= decCount;
				return this;
			}



			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setAllIndividualPrecomputationCreated(bool initialized) {
				mIndividualPrecomputationCreated = initialized;
				return this;
			}

			bool CTotallyOntologyPrecomputationItem::hasAllIndividualPrecomputationCreated() {
				return mIndividualPrecomputationCreated;
			}


			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setIndividualPrecomputationChecked(bool checked) {
				mIndividualPrecomputationChecked = checked;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::hasIndividualPrecomputationChecked() {
				return mIndividualPrecomputationChecked;
			}


			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setIndividualPrecomputationClashed(bool checked) {
				mIndividualPrecomputationClashed = checked;
				return this;
			}


			bool CTotallyOntologyPrecomputationItem::hasIndividualPrecomputationClashed() {
				return mIndividualPrecomputationClashed;
			}

			bool* CTotallyOntologyPrecomputationItem::getIndividualPrecomputationClashedPointer() {
				return &mIndividualPrecomputationClashed;
			}

			bool CTotallyOntologyPrecomputationItem::isIndividualStepFinished() {
				return mIndividualPrecomputationStep->isStepFinished();
			}


			bool CTotallyOntologyPrecomputationItem::isIndividualStepRequired() {
				return mIndividualPrecomputationStep->hasRequirements();
			}


			QTime* CTotallyOntologyPrecomputationItem::getInitializationTime() {
				return &mInitTime;
			}



			cint64 CTotallyOntologyPrecomputationItem::getCurrentIncompletelyHandledIndividualComputationLimit() {
				return mCurrentIncompIndiComputationLimit;
			}

			CTotallyOntologyPrecomputationItem* CTotallyOntologyPrecomputationItem::setCurrentIncompletelyHandledIndividualComputationLimit(cint64 limit) {
				mCurrentIncompIndiComputationLimit = limit;
				return this;
			}


			cint64 CTotallyOntologyPrecomputationItem::getNextRepresentativeCacheRecomputationId() {
				return mNextRepresentativeCacheRecomputationId++;
			}



		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
