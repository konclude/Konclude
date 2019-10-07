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

#include "COptimizedComplexConceptOntologyAnsweringItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexConceptOntologyAnsweringItem::COptimizedComplexConceptOntologyAnsweringItem(CConcreteOntology* ontology, CConfigurationBase* configuration) : COntologyAnsweringItem(ontology,configuration) {
				mTestingOntology = nullptr;
				mQueryProcessingLinker = nullptr;
				mItemNotificationLinker = nullptr;
				mBuildingVarItemProcessingLinker = nullptr;
				mCurrentlyAnsweringQueryCount = 0;
				mTopConceptItem = nullptr;
				mNextVariableId = 0;
				mTopRolePropagationExistingIndividualCreationConcept = nullptr;
				mTopRolePropagationExistingIndividual = nullptr;
				mNextComplexConceptItemId = 0;

				mCalcStatCollStrings = new CAnsweringCalculationStatisticsCollectionStrings();
				mDiffStoredStatCollStrings = nullptr;
				mOntologyBuildingAndPreprocessingPrepared = false;

				mRequirementProcessingSchedulingReported = false;

				mNextComputationStepId = 1;

				mNextTestingIndiId = ontology->getABox()->getNextIndividualId(false);
				if (ontology->getOntologyTriplesData() && ontology->getOntologyTriplesData()->getTripleAssertionAccessor()) {
					mNextTestingIndiId = qMax(ontology->getOntologyTriplesData()->getTripleAssertionAccessor()->getIndexedIndividualCount() + 1, mNextTestingIndiId);
				}


				mConfCollectProcessStatistics = CConfigDataReader::readConfigBoolean(configuration, "Konclude.Calculation.Answering.CollectProcessStatistics", false);

			}

			COptimizedComplexConceptOntologyAnsweringItem::~COptimizedComplexConceptOntologyAnsweringItem() {
				delete mDiffStoredStatCollStrings;
				delete mTestingOntology;
			}


			bool COptimizedComplexConceptOntologyAnsweringItem::isOntologyBuildingAndPreprocessingPrepared() {
				return mOntologyBuildingAndPreprocessingPrepared;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::setOntologyBuildingAndPreprocessingPrepared(bool prepared) {
				mOntologyBuildingAndPreprocessingPrepared = prepared;
				return this;
			}


			CConcreteOntology* COptimizedComplexConceptOntologyAnsweringItem::getTestingOntology(bool create) {
				if (!mTestingOntology && create) {
					mTestingOntology = new CConcreteOntology(mOntology, mOntology->getConfiguration());
					mTestingOntology->setOntologyID(mOntology->getOntologyID());
					mTestingOntology->setConsistence(mOntology->getConsistence());
					mTestingOntology->getTBox()->setMinimalNextConceptID(mOntology->getTBox()->getMinimalNextConceptID());
					mTestingOntology->getDataBoxes()->setInitialBuild(false);
					mTestingOntology->getDataBoxes()->setIterationBuild(true);
				}
				return mTestingOntology;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::takeTestingOntology(CConcreteOntology* ontology) {
				if (mTestingOntology) {
					delete mTestingOntology;
				}
				mTestingOntology = ontology;
				return this;
			}


			QHash< TConceptNegPair, COptimizedComplexConceptItem* >* COptimizedComplexConceptOntologyAnsweringItem::getComplexConceptNegationItemHash() {
				return &mConceptNegItemHash;
			}


			COptimizedComplexConceptItem* COptimizedComplexConceptOntologyAnsweringItem::getComplexConceptItem(CConcept* concept, bool negation, bool create, bool* created) {
				COptimizedComplexConceptItem*& conceptItem = mConceptNegItemHash[TConceptNegPair(concept, negation)];
				if (!conceptItem && create) {
					conceptItem = new COptimizedComplexConceptItem();
					conceptItem->initComplexConcept(concept, negation);
					conceptItem->setConceptItemId(getNextComplexConceptItemId());
					if (conceptItem->representsConceptClass()) {
						mConceptClassItemReorderingList.append(conceptItem);
					}
					if (created) {
						*created = true;
					}
				} else {
					if (created) {
						*created = false;
					}
				}
				return conceptItem;
			}




			QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* COptimizedComplexConceptOntologyAnsweringItem::getPropagationFinalizationConceptAbsorptionDataHash() {
				return &mPropagationFinalizationConceptAbsorptionDataHash;
			}

			QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* COptimizedComplexConceptOntologyAnsweringItem::getPropagationFinalizationConceptAbsorptionItemHash() {
				return &mPropagationFinalizationConceptAbsorptionItemHash;
			}

			QList<COptimizedComplexConceptItem*>* COptimizedComplexConceptOntologyAnsweringItem::getConceptClassItemReorderingList() {
				return &mConceptClassItemReorderingList;
			}


			COptimizedComplexConceptItem* COptimizedComplexConceptOntologyAnsweringItem::getTopConceptItem(bool create) {
				if (create && !mTopConceptItem) {
					mTopConceptItem = getComplexConceptItem(mOntology->getTBox()->getTopConcept(), false, true);
				}
				return mTopConceptItem;
			}







			CComputedItemDataNotificationLinker* COptimizedComplexConceptOntologyAnsweringItem::createComputedItemDataNotificationLinker() {
				CComputedItemDataNotificationLinker* itemLinker = mItemNotificationLinker;
				if (itemLinker) {
					mItemNotificationLinker = mItemNotificationLinker->getNext();
				} else {
					itemLinker = new CComputedItemDataNotificationLinker();
				}
				itemLinker->clearNext();
				return itemLinker;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::releaseComputedItemDataNotificationLinker(CComputedItemDataNotificationLinker* itemLinker) {
				mItemNotificationLinker = itemLinker->append(mItemNotificationLinker);
				return this;
			}



			CXLinker<CComplexQueryProcessingData*>* COptimizedComplexConceptOntologyAnsweringItem::createQueryProcessingLinker() {
				CXLinker<CComplexQueryProcessingData*>* queryLinker = mQueryProcessingLinker;
				if (queryLinker) {
					mQueryProcessingLinker = mQueryProcessingLinker->getNext();
				} else {
					queryLinker = new CXLinker<CComplexQueryProcessingData*>();
				}
				queryLinker->clearNext();
				return queryLinker;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::releaseQueryProcessingLinker(CXLinker<CComplexQueryProcessingData*>* queryLinker) {
				mQueryProcessingLinker = queryLinker->append(mQueryProcessingLinker);
				return this;
			}



			CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* COptimizedComplexConceptOntologyAnsweringItem::createBuildingVariableItemProcessingLinker() {
				CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* varBuildItemLinker = mBuildingVarItemProcessingLinker;
				if (varBuildItemLinker) {
					mBuildingVarItemProcessingLinker = mBuildingVarItemProcessingLinker->getNext();
				} else {
					varBuildItemLinker = new CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>();
				}
				varBuildItemLinker->clearNext();
				return varBuildItemLinker;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::releaseBuildingVariableItemProcessingLinker(CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* varBuildItemLinker) {
				mBuildingVarItemProcessingLinker = varBuildItemLinker->append(mBuildingVarItemProcessingLinker);
				return this;
			}





			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::getConceptProcessingStepItem(ComplexConceptStepComputationProcessType type) {
				return &mConceptProcessingStepItems[type];
			}


			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::incCurrentlyAnsweringQueryCount(cint64 incCount) {
				mCurrentlyAnsweringQueryCount += incCount;
				return this;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::decCurrentlyAnsweringQueryCount(cint64 decCount) {
				mCurrentlyAnsweringQueryCount -= decCount;
				return this;
			}


			bool COptimizedComplexConceptOntologyAnsweringItem::hasCurrentlyAnsweringQueries() {
				return mCurrentlyAnsweringQueryCount > 0;
			}

			cint64 COptimizedComplexConceptOntologyAnsweringItem::getCurrentlyAnsweringQueryCount() {
				return mCurrentlyAnsweringQueryCount;
			}

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::getConceptSatisfiabilityProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPESATISFIABILITY);
			}
			
			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::getConceptSuperClassesProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPESUPERCLASSNODES);
			}

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::getConceptSubClassesProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPESUBCLASSNODES);
			}


			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::getConceptSubClassesRealizationProcessingStepItem() {
				return getConceptProcessingStepItem(REALIZATIONTYPESUBCLASS);
			}

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::getConceptEquivalentClassesProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPEEQUIVALENTCLASSNODES);
			}

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::getConceptInstancesProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPEINSTANCES);
			}








			QList<COptimizedComplexVariableCompositionItem*>* COptimizedComplexConceptOntologyAnsweringItem::getVariableCompositionItemProcessingList() {
				return &mVarCompItemProcessingList;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexConceptOntologyAnsweringItem::takeNextProcessingVariableCompositionItem() {
				COptimizedComplexVariableCompositionItem* nextVarItem = nullptr;
				if (!mVarCompItemProcessingList.isEmpty()) {
					nextVarItem = mVarCompItemProcessingList.takeFirst();
				}
				return nextVarItem;
			}


			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::addProcessingVariableCompositionItem(COptimizedComplexVariableCompositionItem* varCompItem) {
				mVarCompItemProcessingList.append(varCompItem);
				return this;
			}




			QList<COptimizedComplexBuildingVariableCompositionsItem*>* COptimizedComplexConceptOntologyAnsweringItem::getVariableBuildingItemProcessingList() {
				return &mVarBuildItemProcessingList;
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexConceptOntologyAnsweringItem::takeNextProcessingVariableBuildingItem() {
				COptimizedComplexBuildingVariableCompositionsItem* nextVarItem = nullptr;
				if (!mVarBuildItemProcessingList.isEmpty()) {
					nextVarItem = mVarBuildItemProcessingList.takeFirst();
				}
				return nextVarItem;
			}


			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::addProcessingVariableBuildingItem(COptimizedComplexBuildingVariableCompositionsItem* varBuildItem) {
				mVarBuildItemProcessingList.append(varBuildItem);
				return this;
			}


			cint64 COptimizedComplexConceptOntologyAnsweringItem::getNextVariableId() {
				return mNextVariableId++;
			}

			QHash<TConceptNegPair, TConceptNegPair>* COptimizedComplexConceptOntologyAnsweringItem::getIndividualTriggerSingleConceptUpgradeHash() {
				return &mIndividualTriggerSingleConceptUpgradeHash;
			}

			QHash< QSet<TConceptNegPair> , TConceptNegPair>* COptimizedComplexConceptOntologyAnsweringItem::getIndividualTriggerMultiConceptUpgradeHash() {
				return &mIndividualTriggerMultiConceptUpgradeHash;
			}

			CConcept* COptimizedComplexConceptOntologyAnsweringItem::getTopRolePropagationExistingIndividualCreationConcept() {
				return mTopRolePropagationExistingIndividualCreationConcept;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::setTopRolePropagationExistingIndividualCreationConcept(CConcept* concept) {
				mTopRolePropagationExistingIndividualCreationConcept = concept;
				return this;
			}

			CIndividual* COptimizedComplexConceptOntologyAnsweringItem::getTopRolePropagationExistingIndividual() {
				return mTopRolePropagationExistingIndividual;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::setTopRolePropagationExistingIndividual(CIndividual* indi) {
				mTopRolePropagationExistingIndividual = indi;
				return this;
			}


			cint64 COptimizedComplexConceptOntologyAnsweringItem::getNextComplexConceptItemId(bool moveNext) {
				cint64 nextId =  mNextComplexConceptItemId;
				if (moveNext) {
					++mNextComplexConceptItemId;
				}
				return nextId;
			}


			CAnsweringStatisticsCollectionStrings* COptimizedComplexConceptOntologyAnsweringItem::getDifferenceStoredAnsweringStatisticsCollectionStrings() {
				return mDiffStoredStatCollStrings;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::setDifferenceStoredAnsweringStatisticsCollectionStrings(CAnsweringStatisticsCollectionStrings* stats) {
				if (mDiffStoredStatCollStrings) {
					delete mDiffStoredStatCollStrings;
				}
				mDiffStoredStatCollStrings = stats;
				return this;
			}


			CAnsweringCalculationStatisticsCollectionStrings* COptimizedComplexConceptOntologyAnsweringItem::getAnsweringStatisticsCollectionStrings() {
				return mCalcStatCollStrings;
			}


			CAnsweringCalculationStatisticsCollection* COptimizedComplexConceptOntologyAnsweringItem::getCalculationStatisticsCollection() {
				CAnsweringCalculationStatisticsCollection* statColl = nullptr;
				if (!mReuseStatCollList.isEmpty()) {
					statColl = mReuseStatCollList.takeFirst();
				}
				if (!statColl && mCalcStatCollStrings) {
					statColl = mCalcStatCollStrings->createCalculationStatisticsCollection();
				}
				return statColl;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::reuseCalculationStatisticsCollection(CAnsweringCalculationStatisticsCollection* statColl) {
				mReuseStatCollList.append(statColl);
				return this;
			}



			bool COptimizedComplexConceptOntologyAnsweringItem::isProcessStatisticsCollectionConfigured() {
				return mConfCollectProcessStatistics;
			}


			CAnsweringHandlingStatistics* COptimizedComplexConceptOntologyAnsweringItem::getAnsweringHandlingStatistics() {
				return &mAnsweringHandlingStatistics;
			}


			CIndividual* COptimizedComplexConceptOntologyAnsweringItem::takeTemporaryTestingIndividual() {
				CIndividual* nextTestingIndi = nullptr;
				if (!mTestingIndiList.isEmpty()) {
					nextTestingIndi = mTestingIndiList.takeFirst();
				}
				if (!nextTestingIndi) {
					nextTestingIndi = new CIndividual();
					nextTestingIndi->initIndividual(mNextTestingIndiId++);
					nextTestingIndi->setTemporaryFakeIndividual(true);
				}
				return nextTestingIndi;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::releaseTemporaryTestingIndividual(CIndividual* indi) {
				mTestingIndiList.append(indi);
				return this;
			}

			cint64 COptimizedComplexConceptOntologyAnsweringItem::getNextComputationStepId(bool next) {
				cint64 compStep = mNextComputationStepId;
				if (next) {
					mNextComputationStepId++;
				}
				return compStep;
			}


			bool COptimizedComplexConceptOntologyAnsweringItem::hasRequirementProcessingSchedulingReported() {
				return mRequirementProcessingSchedulingReported;
			}

			COptimizedComplexConceptOntologyAnsweringItem* COptimizedComplexConceptOntologyAnsweringItem::setRequirementProcessingSchedulingReported(bool reported) {
				mRequirementProcessingSchedulingReported = reported;
				return this;
			}

			QHash<QPair<QString, CDatatype*>, CDataLiteral*>* COptimizedComplexConceptOntologyAnsweringItem::getDataValueDatatypeSingleLiteralHash() {
				return &mDataValueDatatypeSingleLiteralHash;
			}

			CContext* COptimizedComplexConceptOntologyAnsweringItem::getDataValueMemoryManagementContext() {
				return &mDataValueMemoryManagementContext;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
