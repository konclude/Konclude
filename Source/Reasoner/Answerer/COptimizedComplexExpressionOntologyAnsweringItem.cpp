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

#include "COptimizedComplexExpressionOntologyAnsweringItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexExpressionOntologyAnsweringItem::COptimizedComplexExpressionOntologyAnsweringItem(CConcreteOntology* ontology, CConfigurationBase* configuration) : COntologyAnsweringItem(ontology,configuration) {
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

				mNextQueryExtensionId = 1;


				mConfCollectProcessStatistics = CConfigDataReader::readConfigBoolean(configuration, "Konclude.Calculation.Answering.CollectProcessStatistics", false);

			}

			COptimizedComplexExpressionOntologyAnsweringItem::~COptimizedComplexExpressionOntologyAnsweringItem() {
				delete mDiffStoredStatCollStrings;
				delete mTestingOntology;
			}


			bool COptimizedComplexExpressionOntologyAnsweringItem::isOntologyBuildingAndPreprocessingPrepared() {
				return mOntologyBuildingAndPreprocessingPrepared;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::setOntologyBuildingAndPreprocessingPrepared(bool prepared) {
				mOntologyBuildingAndPreprocessingPrepared = prepared;
				return this;
			}


			CConcreteOntology* COptimizedComplexExpressionOntologyAnsweringItem::getTestingOntology(bool create) {
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

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::takeTestingOntology(CConcreteOntology* ontology) {
				if (mTestingOntology) {
					delete mTestingOntology;
				}
				mTestingOntology = ontology;
				return this;
			}


			QHash< TConceptNegPair, COptimizedComplexConceptItem* >* COptimizedComplexExpressionOntologyAnsweringItem::getComplexConceptNegationItemHash() {
				return &mConceptNegItemHash;
			}


			COptimizedComplexConceptItem* COptimizedComplexExpressionOntologyAnsweringItem::getComplexConceptItem(CConcept* concept, bool negation, bool create, bool* created) {
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




			QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* COptimizedComplexExpressionOntologyAnsweringItem::getPropagationFinalizationConceptAbsorptionDataHash() {
				return &mPropagationFinalizationConceptAbsorptionDataHash;
			}

			QHash<CConcept*, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* COptimizedComplexExpressionOntologyAnsweringItem::getPropagationFinalizationConceptAbsorptionItemHash() {
				return &mPropagationFinalizationConceptAbsorptionItemHash;
			}

			QList<COptimizedComplexConceptItem*>* COptimizedComplexExpressionOntologyAnsweringItem::getConceptClassItemReorderingList() {
				return &mConceptClassItemReorderingList;
			}


			COptimizedComplexConceptItem* COptimizedComplexExpressionOntologyAnsweringItem::getTopConceptItem(bool create) {
				if (create && !mTopConceptItem) {
					mTopConceptItem = getComplexConceptItem(mOntology->getTBox()->getTopConcept(), false, true);
				}
				return mTopConceptItem;
			}







			CComputedItemDataNotificationLinker* COptimizedComplexExpressionOntologyAnsweringItem::createComputedItemDataNotificationLinker() {
				CComputedItemDataNotificationLinker* itemLinker = mItemNotificationLinker;
				if (itemLinker) {
					mItemNotificationLinker = mItemNotificationLinker->getNext();
				} else {
					itemLinker = new CComputedItemDataNotificationLinker();
				}
				itemLinker->setQueryProcessingData(nullptr);
				itemLinker->setVariableCompositionItem(nullptr);
				itemLinker->setBuildingVariableItemProcessingItem(nullptr);
				itemLinker->clearNext();
				return itemLinker;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::releaseComputedItemDataNotificationLinker(CComputedItemDataNotificationLinker* itemLinker) {
				itemLinker->setQueryProcessingData(nullptr);
				itemLinker->setVariableCompositionItem(nullptr);
				itemLinker->setBuildingVariableItemProcessingItem(nullptr);
				mItemNotificationLinker = itemLinker->append(mItemNotificationLinker);
				return this;
			}



			CXLinker<CComplexQueryExpressionProcessingData*>* COptimizedComplexExpressionOntologyAnsweringItem::createQueryProcessingLinker() {
				CXLinker<CComplexQueryExpressionProcessingData*>* queryLinker = mQueryProcessingLinker;
				if (queryLinker) {
					mQueryProcessingLinker = mQueryProcessingLinker->getNext();
				} else {
					queryLinker = new CXLinker<CComplexQueryExpressionProcessingData*>();
				}
				queryLinker->clearNext();
				return queryLinker;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::releaseQueryProcessingLinker(CXLinker<CComplexQueryExpressionProcessingData*>* queryLinker) {
				mQueryProcessingLinker = queryLinker->append(mQueryProcessingLinker);
				return this;
			}



			CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* COptimizedComplexExpressionOntologyAnsweringItem::createBuildingVariableItemProcessingLinker() {
				CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* varBuildItemLinker = mBuildingVarItemProcessingLinker;
				if (varBuildItemLinker) {
					mBuildingVarItemProcessingLinker = mBuildingVarItemProcessingLinker->getNext();
				} else {
					varBuildItemLinker = new CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>();
				}
				varBuildItemLinker->clearNext();
				return varBuildItemLinker;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::releaseBuildingVariableItemProcessingLinker(CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* varBuildItemLinker) {
				mBuildingVarItemProcessingLinker = varBuildItemLinker->append(mBuildingVarItemProcessingLinker);
				return this;
			}





			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::getConceptProcessingStepItem(ComplexConceptStepComputationProcessType type) {
				return &mConceptProcessingStepItems[type];
			}


			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::incCurrentlyAnsweringQueryCount(cint64 incCount) {
				mCurrentlyAnsweringQueryCount += incCount;
				return this;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::decCurrentlyAnsweringQueryCount(cint64 decCount) {
				mCurrentlyAnsweringQueryCount -= decCount;
				return this;
			}


			bool COptimizedComplexExpressionOntologyAnsweringItem::hasCurrentlyAnsweringQueries() {
				return mCurrentlyAnsweringQueryCount > 0;
			}

			cint64 COptimizedComplexExpressionOntologyAnsweringItem::getCurrentlyAnsweringQueryCount() {
				return mCurrentlyAnsweringQueryCount;
			}

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::getConceptSatisfiabilityProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPESATISFIABILITY);
			}
			
			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::getConceptSuperClassesProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPESUPERCLASSNODES);
			}

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::getConceptSubClassesProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPESUBCLASSNODES);
			}


			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::getConceptSubClassesRealizationProcessingStepItem() {
				return getConceptProcessingStepItem(REALIZATIONTYPESUBCLASS);
			}

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::getConceptEquivalentClassesProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPEEQUIVALENTCLASSNODES);
			}

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::getConceptInstancesProcessingStepItem() {
				return getConceptProcessingStepItem(COMPUTATIONTYPEINSTANCES);
			}








			QList<COptimizedComplexVariableCompositionItem*>* COptimizedComplexExpressionOntologyAnsweringItem::getVariableCompositionItemProcessingList() {
				return &mVarCompItemProcessingList;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexExpressionOntologyAnsweringItem::takeNextProcessingVariableCompositionItem() {
				COptimizedComplexVariableCompositionItem* nextVarItem = nullptr;
				if (!mVarCompItemProcessingList.isEmpty()) {
					nextVarItem = mVarCompItemProcessingList.takeFirst();
				}
				return nextVarItem;
			}


			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::addProcessingVariableCompositionItem(COptimizedComplexVariableCompositionItem* varCompItem) {
				mVarCompItemProcessingList.append(varCompItem);
				return this;
			}




			QList<COptimizedComplexBuildingVariableCompositionsItem*>* COptimizedComplexExpressionOntologyAnsweringItem::getVariableBuildingItemProcessingList() {
				return &mVarBuildItemProcessingList;
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexExpressionOntologyAnsweringItem::takeNextProcessingVariableBuildingItem() {
				COptimizedComplexBuildingVariableCompositionsItem* nextVarItem = nullptr;
				if (!mVarBuildItemProcessingList.isEmpty()) {
					nextVarItem = mVarBuildItemProcessingList.takeFirst();
				}
				return nextVarItem;
			}


			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::addProcessingVariableBuildingItem(COptimizedComplexBuildingVariableCompositionsItem* varBuildItem) {
				mVarBuildItemProcessingList.append(varBuildItem);
				return this;
			}


			cint64 COptimizedComplexExpressionOntologyAnsweringItem::getNextVariableId() {
				return mNextVariableId++;
			}

			QHash<TConceptNegPair, TConceptNegPair>* COptimizedComplexExpressionOntologyAnsweringItem::getIndividualTriggerSingleConceptUpgradeHash() {
				return &mIndividualTriggerSingleConceptUpgradeHash;
			}

			QHash< QSet<TConceptNegPair> , TConceptNegPair>* COptimizedComplexExpressionOntologyAnsweringItem::getIndividualTriggerMultiConceptUpgradeHash() {
				return &mIndividualTriggerMultiConceptUpgradeHash;
			}

			CConcept* COptimizedComplexExpressionOntologyAnsweringItem::getTopRolePropagationExistingIndividualCreationConcept() {
				return mTopRolePropagationExistingIndividualCreationConcept;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::setTopRolePropagationExistingIndividualCreationConcept(CConcept* concept) {
				mTopRolePropagationExistingIndividualCreationConcept = concept;
				return this;
			}

			CIndividual* COptimizedComplexExpressionOntologyAnsweringItem::getTopRolePropagationExistingIndividual() {
				return mTopRolePropagationExistingIndividual;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::setTopRolePropagationExistingIndividual(CIndividual* indi) {
				mTopRolePropagationExistingIndividual = indi;
				return this;
			}


			cint64 COptimizedComplexExpressionOntologyAnsweringItem::getNextComplexConceptItemId(bool moveNext) {
				cint64 nextId =  mNextComplexConceptItemId;
				if (moveNext) {
					++mNextComplexConceptItemId;
				}
				return nextId;
			}


			CAnsweringStatisticsCollectionStrings* COptimizedComplexExpressionOntologyAnsweringItem::getDifferenceStoredAnsweringStatisticsCollectionStrings() {
				return mDiffStoredStatCollStrings;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::setDifferenceStoredAnsweringStatisticsCollectionStrings(CAnsweringStatisticsCollectionStrings* stats) {
				if (mDiffStoredStatCollStrings) {
					delete mDiffStoredStatCollStrings;
				}
				mDiffStoredStatCollStrings = stats;
				return this;
			}


			CAnsweringCalculationStatisticsCollectionStrings* COptimizedComplexExpressionOntologyAnsweringItem::getAnsweringStatisticsCollectionStrings() {
				return mCalcStatCollStrings;
			}


			CAnsweringCalculationStatisticsCollection* COptimizedComplexExpressionOntologyAnsweringItem::getCalculationStatisticsCollection() {
				CAnsweringCalculationStatisticsCollection* statColl = nullptr;
				if (!mReuseStatCollList.isEmpty()) {
					statColl = mReuseStatCollList.takeFirst();
				}
				if (!statColl && mCalcStatCollStrings) {
					statColl = mCalcStatCollStrings->createCalculationStatisticsCollection();
				}
				return statColl;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::reuseCalculationStatisticsCollection(CAnsweringCalculationStatisticsCollection* statColl) {
				mReuseStatCollList.append(statColl);
				return this;
			}



			bool COptimizedComplexExpressionOntologyAnsweringItem::isProcessStatisticsCollectionConfigured() {
				return mConfCollectProcessStatistics;
			}


			CAnsweringHandlingStatistics* COptimizedComplexExpressionOntologyAnsweringItem::getAnsweringHandlingStatistics() {
				return &mAnsweringHandlingStatistics;
			}


			CIndividual* COptimizedComplexExpressionOntologyAnsweringItem::takeTemporaryTestingIndividual() {
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

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::releaseTemporaryTestingIndividual(CIndividual* indi) {
				mTestingIndiList.append(indi);
				return this;
			}

			cint64 COptimizedComplexExpressionOntologyAnsweringItem::getNextComputationStepId(bool next) {
				cint64 compStep = mNextComputationStepId;
				if (next) {
					mNextComputationStepId++;
				}
				return compStep;
			}


			bool COptimizedComplexExpressionOntologyAnsweringItem::hasRequirementProcessingSchedulingReported() {
				return mRequirementProcessingSchedulingReported;
			}

			COptimizedComplexExpressionOntologyAnsweringItem* COptimizedComplexExpressionOntologyAnsweringItem::setRequirementProcessingSchedulingReported(bool reported) {
				mRequirementProcessingSchedulingReported = reported;
				return this;
			}

			QHash<QPair<QString, CDatatype*>, CDataLiteral*>* COptimizedComplexExpressionOntologyAnsweringItem::getDataValueDatatypeSingleLiteralHash() {
				return &mDataValueDatatypeSingleLiteralHash;
			}

			CContext* COptimizedComplexExpressionOntologyAnsweringItem::getDataValueMemoryManagementContext() {
				return &mDataValueMemoryManagementContext;
			}

			QHash<cint64, CComplexQueryExpressionProcessingData*>* COptimizedComplexExpressionOntologyAnsweringItem::getQueryExtensionProcessingDataHash() {
				return &mQueryExtensionProcessingDataHash;
			}

			cint64 COptimizedComplexExpressionOntologyAnsweringItem::getNextQueryExtensionId(bool moveNext) {
				cint64 tmpId = mNextQueryExtensionId;
				if (moveNext) {
					mNextQueryExtensionId++;
				}
				return tmpId;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
