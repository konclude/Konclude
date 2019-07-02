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

#include "COptimizedKPSetOntologyConceptRealizingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetOntologyConceptRealizingItem::COptimizedKPSetOntologyConceptRealizingItem() {
			}

			COptimizedKPSetOntologyConceptRealizingItem::~COptimizedKPSetOntologyConceptRealizingItem() {
				delete mCalculationConfig;
				delete mRealizeConceptProcessingStep;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::initRequirementConfigRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				COntologyRealizingItem::initRealizingItem(ontology,config);
				mRealization = nullptr;
				mCalculationConfig = new CCalculationConfigurationExtension(config,0);
				COntologyProcessingStepDataVector* ontProStepDataVec = mOntology->getProcessingSteps()->getOntologyProcessingStepDataVector();
				mRealizeConceptProcessingStep = new CRealizingTestingStep(CRealizingTestingStep::REALIZECONCEPTPROCESSINGSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSCONCEPTREALIZE),this);
				mProcessingSteps.append(mRealizeConceptProcessingStep);
				mRealizeSameIndividualsProcessingStep = new CRealizingTestingStep(CRealizingTestingStep::REALIZESAMEINDIVIDUALSPROCESSINGSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE),this);
				mProcessingSteps.append(mRealizeSameIndividualsProcessingStep);
				mItemsInitialized = false;
				mRealizationSameIndividualsInitialized = false;
				mRealizationConceptsInitialized = false;
				mTestedPossibleInstancesCount = 0;
				mOpenPossibleInstancesCount = 0;
				mTopInstancesItem = nullptr;
				mBottomInstancesItem = nullptr;
				mTestingPossibleSameIndividualCount = 0;
				mTestingPossibleInstanceCount = 0;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::initItemsFromHierarchy() {
				CClassification* classification = mOntology->getClassification();
				if (classification->hasClassConceptClassification()) {
					CClassConceptClassification* classConClass = classification->getClassConceptClassification();
					if (classConClass) {
						CTaxonomy* taxonomy = classConClass->getClassConceptTaxonomy();
						if (taxonomy) {
							QHash<CConcept*,CHierarchyNode*>* conHierNodeHash = taxonomy->getConceptHierarchyNodeHash();
							for (QHash<CConcept*,CHierarchyNode*>::const_iterator it = conHierNodeHash->constBegin(), itEnd = conHierNodeHash->constEnd(); it != itEnd; ++it) {
								CConcept* concept(it.key());
								CHierarchyNode* hierNode(it.value());

								COptimizedKPSetConceptInstancesItem* instancesItem = mHierNodeInstancesItemHash.value(hierNode,nullptr);
								if (!instancesItem) {
									instancesItem = new COptimizedKPSetConceptInstancesItem();
									mInstancesItemContainer.append(instancesItem);
									mHierNodeInstancesItemHash.insert(hierNode,instancesItem);
									instancesItem->initInstancesItem(hierNode);
								}
								mConceptInstancesItemHash.insert(concept,instancesItem);
							}

							for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = mInstancesItemContainer.constBegin(), itEnd = mInstancesItemContainer.constEnd(); it != itEnd; ++it) {
								COptimizedKPSetConceptInstancesItem* item(*it);
								CHierarchyNode* node = item->getHierarchyNode();
								QSet<CHierarchyNode*>* parentNodeSet = node->getParentNodeSet();
								if (parentNodeSet) {
									for (QSet<CHierarchyNode*>::const_iterator parentIt = parentNodeSet->constBegin(), parentItEnd = parentNodeSet->constEnd(); parentIt != parentItEnd; ++parentIt) {
										CHierarchyNode* parentHierNode(*parentIt);
										COptimizedKPSetConceptInstancesItem* parentItem = mHierNodeInstancesItemHash.value(parentHierNode);
										item->addParentItem(parentItem);
										parentItem->addSuccessorItem(item);
									}
								}
							}

							for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = mInstancesItemContainer.constBegin(), itEnd = mInstancesItemContainer.constEnd(); it != itEnd; ++it) {
								COptimizedKPSetConceptInstancesItem* item(*it);
								item->setInitialUnprocessedSuccessorItemCount();
								if (item->getUnprocessedSuccessorItemCount() <= 0) {
									item->setAllSuccessorProcessedFlag(true);
								}
							}

							mTopInstancesItem = mHierNodeInstancesItemHash.value(taxonomy->getTopHierarchyNode());
							mBottomInstancesItem = mHierNodeInstancesItemHash.value(taxonomy->getBottomHierarchyNode());

						}
					}
				}
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasItemsInitialized() {
				return mItemsInitialized;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setItemsInitialized(bool initialized) {
				mItemsInitialized = initialized;
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasRealizationSameIndividualsInitialized() {
				return mRealizationSameIndividualsInitialized;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setRealizationSameIndividualsInitialized(bool initialized) {
				mRealizationSameIndividualsInitialized = initialized;
				return this;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::hasRealizationConceptsInitialized() {
				return mRealizationConceptsInitialized;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setRealizationConceptsInitialized(bool initialized) {
				mRealizationConceptsInitialized = initialized;
				return this;
			}


			COntologyRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement) {	
				bool supportRequirement = false;
				COntologyProcessingStepRequirement* stepProcRequirement = dynamic_cast<COntologyProcessingStepRequirement*>(ontoRequirement);
				if (stepProcRequirement) {
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSCONCEPTREALIZE) {
						mRealizeConceptProcessingStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
						markIntanceItemForConceptRealization(mOntology->getTBox()->getTopConcept());
					} else if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE) {
						mRealizeSameIndividualsProcessingStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
						markInstantiatedItemForSameIndividualsRealization();
					}
				}
				if (!supportRequirement) {
					stepProcRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSUNSUPPORTED);
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addInstanceItemToProcessingPossibleInstances(COptimizedKPSetConceptInstancesItem* instanceItem) {
				if (!instanceItem->hasPossibleInstancesProcessingQueuedFlag()) {
					if (instanceItem->hasPossibleInstances()) {
						mProcessingInstancesItemList.append(instanceItem);
						instanceItem->setPossibleInstancesProcessingQueuedFlag(true);
					}
				}
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addInstantiatedItemToProcessingPossibleSameIndividuals(COptimizedKPSetConceptInstantiatedItem* instantiatedItem) {
				if (!instantiatedItem->hasPossibleSameIndividualsProcessingQueuedFlag()) {
					if (instantiatedItem->hasPossibleSameIndividuals()) {
						mProcessingSameIndividualsItemList.append(instantiatedItem);
						instantiatedItem->setPossibleSameIndividualsProcessingQueuedFlag(true);
					}
				}
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::markIntanceItemForConceptRealization(CConcept* concept) {
				COptimizedKPSetConceptInstancesItem* initialItem = mConceptInstancesItemHash.value(concept);
				QList<COptimizedKPSetConceptInstancesItem*> itemList;
				QSet<COptimizedKPSetConceptInstancesItem*> itemSet;
				itemList.append(initialItem);
				itemSet.insert(initialItem);
				while (!itemList.isEmpty()) {
					COptimizedKPSetConceptInstancesItem* item = itemList.takeFirst();
					if (!item->hasToProcessPossibleInstancesFlag()) {
						item->setToProcessPossibleInstancesFlag(true);
						addInstanceItemToProcessingPossibleInstances(item);
						QList<COptimizedKPSetConceptInstancesItem*>* succItemList = item->getSuccessorItemList();
						for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = succItemList->constBegin(), itEnd = succItemList->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetConceptInstancesItem* succItem(*it);
							if (!itemSet.contains(succItem)) {
								itemSet.insert(succItem);
								itemList.append(succItem);
							}
						}
					}
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::markInstantiatedItemForSameIndividualsRealization() {
				for (QHash<CIndividual*,COptimizedKPSetConceptInstantiatedItem*>::const_iterator it = mIndividualInstantiatedItemHash.constBegin(), itEnd = mIndividualInstantiatedItemHash.constEnd(); it != itEnd; ++it) {
					CIndividual* individual = it.key();
					COptimizedKPSetConceptInstantiatedItem* indiItem = it.value();
					if (!indiItem->hasToProcessPossibleSameIndividualsFlag()) {
						indiItem->setToProcessPossibleSameIndividualsFlag(true);
						addInstantiatedItemToProcessingPossibleSameIndividuals(indiItem);
					}
				}
				return this;
			}

			CCalculationConfigurationExtension* COptimizedKPSetOntologyConceptRealizingItem::getCalculationConfiguration() {
				return mCalculationConfig;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setRealization(CRealization* realization) {
				mRealization = realization;
				return this;
			}

			CRealization* COptimizedKPSetOntologyConceptRealizingItem::getRealization() {
				return mRealization;
			}



			CRealizingTestingStep* COptimizedKPSetOntologyConceptRealizingItem::getRealizeConceptProcessingStep() {
				return mRealizeConceptProcessingStep;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::isRealizeConceptStepFinished() {
				return mRealizeConceptProcessingStep->isStepFinished();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::isRealizeConceptStepRequired() {
				return mRealizeConceptProcessingStep->hasRequirements();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::areRealizeConceptStepProcessingRequirementSatisfied() {
				return mRealizeConceptProcessingStep->areStepProcessingRequirementSatisfied();
			}





			CRealizingTestingStep* COptimizedKPSetOntologyConceptRealizingItem::getRealizeSameIndividualsProcessingStep() {
				return mRealizeSameIndividualsProcessingStep;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::isRealizeSameIndividualsStepFinished() {
				return mRealizeSameIndividualsProcessingStep->isStepFinished();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::isRealizeSameIndividualsStepRequired() {
				return mRealizeSameIndividualsProcessingStep->hasRequirements();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::areRealizeSameIndividualsStepProcessingRequirementSatisfied() {
				return mRealizeSameIndividualsProcessingStep->areStepProcessingRequirementSatisfied();
			}




			bool COptimizedKPSetOntologyConceptRealizingItem::areAllStepFinished() {
				for (QList<CRealizingTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if (!(*it)->isStepFinished()) {
						return false;
					}
				}
				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasRemainingProcessingRequirements() {
				for (QList<CRealizingTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if ((*it)->hasRequirements()) {
						return true;
					}
				}
				return false;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::hasRemainingProcessingInstanceItems() {
				return !mProcessingInstancesItemList.isEmpty();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasRemainingProcessingSameIndividualsItems() {
				return !mProcessingSameIndividualsItemList.isEmpty();
			}
			QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getHierarchyNodeInstancesItemHash() {
				return &mHierNodeInstancesItemHash;
			}

			QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getConceptInstancesItemHash() {
				return &mConceptInstancesItemHash;
			}

			QHash<CIndividual*,COptimizedKPSetConceptInstantiatedItem*>* COptimizedKPSetOntologyConceptRealizingItem::getIndividualInstantiatedItemHash() {
				return &mIndividualInstantiatedItemHash;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetOntologyConceptRealizingItem::getIndividualInstantiatedItem(CIndividual* individual, bool directCreate) {
				COptimizedKPSetConceptInstantiatedItem* instantiatedItem = nullptr;
				if (directCreate) {
					instantiatedItem = new COptimizedKPSetConceptInstantiatedItem();
					instantiatedItem->initInstantiatedItem(individual);
					mInstantiatedItemContainer.append(instantiatedItem);
					mIndividualInstantiatedItemHash.insert(individual,instantiatedItem);
				} else {
					instantiatedItem = mIndividualInstantiatedItemHash.value(individual);
				}
				return instantiatedItem;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::hasIndividualInstantiatedItem(CIndividual* individual) {
				return mIndividualInstantiatedItemHash.contains(individual);
			}


			QList<COptimizedKPSetConceptInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getProcessingPossibleInstancesItemList() {
				return &mProcessingInstancesItemList;
			}

			QList<COptimizedKPSetConceptInstantiatedItem*>* COptimizedKPSetOntologyConceptRealizingItem::getProcessingPossibleSameIndividualsItemList() {
				return &mProcessingSameIndividualsItemList;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestedPossibleInstancesCount() {
				return mTestedPossibleInstancesCount;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getOpenPossibleInstancesCount() {
				return mOpenPossibleInstancesCount;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incTestedPossibleInstancesCount(cint64 incCount) {
				mTestedPossibleInstancesCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incOpenPossibleInstancesCount(cint64 incCount) {
				mTestedPossibleInstancesCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decTestedPossibleInstancesCount(cint64 decCount) {
				mTestedPossibleInstancesCount -= decCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decOpenPossibleInstancesCount(cint64 decCount) {
				mTestedPossibleInstancesCount -= decCount;
				return this;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::visitDirectInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor) {
				COptimizedKPSetConceptInstancesItem* conceptItem = (COptimizedKPSetConceptInstancesItem*)item;
				if (conceptItem) {
					QSet<COptimizedKPSetConceptInstantiatedItem*>* individualSet = conceptItem->getKnownInstancesSet();
					if (individualSet) {
						for (QSet<COptimizedKPSetConceptInstantiatedItem*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetConceptInstantiatedItem* individualItem(*it);
							visitor->visitInstance(individualItem,this);
						}
					}
				}
				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::visitInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor) {
				COptimizedKPSetConceptInstancesItem* conceptItem = (COptimizedKPSetConceptInstancesItem*)item;
				if (conceptItem) {

					QList<COptimizedKPSetConceptInstancesItem*> conceptItemList;
					QSet<COptimizedKPSetConceptInstancesItem*> conceptItemSet;
					conceptItemList.append(conceptItem);
					conceptItemSet.insert(conceptItem);

					while (!conceptItemList.isEmpty()) {
						COptimizedKPSetConceptInstancesItem* procConceptItem = conceptItemList.takeFirst();

						QSet<COptimizedKPSetConceptInstantiatedItem*>* individualSet = procConceptItem->getKnownInstancesSet();
						if (individualSet) {
							for (QSet<COptimizedKPSetConceptInstantiatedItem*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetConceptInstantiatedItem* individualItem(*it);
								visitor->visitInstance(individualItem,this);
							}
						}

						QList<COptimizedKPSetConceptInstancesItem*>* succConceptItemList = procConceptItem->getSuccessorItemList();
						for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = succConceptItemList->constBegin(), itEnd = succConceptItemList->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetConceptInstancesItem* succConceptItem(*it);
							if (!conceptItemSet.contains(succConceptItem)) {
								conceptItemList.append(succConceptItem);
								conceptItemSet.insert(succConceptItem);
							}
						}
					}
				}
				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::visitDirectTypes(CConceptInstanceItem* item, CConceptRealizationInstantiatedVisitor* visitor) {
				COptimizedKPSetConceptInstantiatedItem* individualItem = (COptimizedKPSetConceptInstantiatedItem*)item;
				if (individualItem) {
					COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = individualItem->getKnownPossibleInstancesHash();
					if (knownPossibleInstancesHash) {
						for (COptimizedKPSetConceptInstancesHash::const_iterator it = knownPossibleInstancesHash->constBegin(), itEnd = knownPossibleInstancesHash->constEnd(); it != itEnd; ++it) {
							const COptimizedKPSetConceptInstancesHashData& hashData(it.value());
							COptimizedKPSetConceptInstancesData* instanceItemData = hashData.mInstanceItemData;
							if (instanceItemData->mKnownInstance && instanceItemData->mMostSpecific) {
								COptimizedKPSetConceptInstancesItem* conceptItem(instanceItemData->mInstanceItem);
								visitor->visitType(conceptItem,this);
							}
						}
					}
				}
				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::visitTypes(CConceptInstanceItem* item, CConceptRealizationInstantiatedVisitor* visitor) {
				COptimizedKPSetConceptInstantiatedItem* individualItem = (COptimizedKPSetConceptInstantiatedItem*)item;
				if (individualItem) {
					COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = individualItem->getKnownPossibleInstancesHash();
					if (knownPossibleInstancesHash) {

						QList<COptimizedKPSetConceptInstancesItem*> conceptItemList;
						QSet<COptimizedKPSetConceptInstancesItem*> conceptItemSet;
						for (COptimizedKPSetConceptInstancesHash::const_iterator it = knownPossibleInstancesHash->constBegin(), itEnd = knownPossibleInstancesHash->constEnd(); it != itEnd; ++it) {
							const COptimizedKPSetConceptInstancesHashData& hashData(it.value());
							COptimizedKPSetConceptInstancesData* instanceItemData = hashData.mInstanceItemData;
							if (instanceItemData->mKnownInstance && instanceItemData->mMostSpecific) {
								COptimizedKPSetConceptInstancesItem* conceptItem(instanceItemData->mInstanceItem);
								conceptItemList.append(conceptItem);
								conceptItemSet.insert(conceptItem);
							}
						}

						while (!conceptItemList.isEmpty()) {
							COptimizedKPSetConceptInstancesItem* conceptItem = conceptItemList.takeFirst();
							visitor->visitType(conceptItem,this);

							QList<COptimizedKPSetConceptInstancesItem*>* parentConceptItemList = conceptItem->getParentItemList();
							for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = parentConceptItemList->constBegin(), itEnd = parentConceptItemList->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetConceptInstancesItem* parentConceptItem(*it);
								if (!conceptItemSet.contains(parentConceptItem)) {
									conceptItemList.append(parentConceptItem);
									conceptItemSet.insert(parentConceptItem);
								}
							}
						}

					}
				}
				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::visitIndividuals(CConceptInstanceItem* item, CConceptRealizationIndividualVisitor* visitor) {
				COptimizedKPSetConceptInstantiatedItem* individualItem = (COptimizedKPSetConceptInstantiatedItem*)item;
				QSet<CIndividual*>* individualSet = individualItem->getKnownSameIndividualSet();
				for (QSet<CIndividual*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
					CIndividual* individual(*it);
					visitor->visitIndividual(individual,this);
				}
				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::visitConcepts(CConceptInstantiatedItem* item, CConceptRealizationConceptVisitor* visitor) {
				COptimizedKPSetConceptInstancesItem* conceptItem = (COptimizedKPSetConceptInstancesItem*)item;
				CHierarchyNode* hierNode = conceptItem->getHierarchyNode();
				if (hierNode) {
					QList<CConcept*>* conList = hierNode->getEquivalentConceptList();
					for (QList<CConcept*>::const_iterator it = conList->constBegin(), itEnd = conList->constEnd(); it != itEnd; ++it) {
						CConcept* concept(*it);
						visitor->visitConcept(concept,this);
					}
				}
				return true;
			}

			CConceptInstantiatedItem* COptimizedKPSetOntologyConceptRealizingItem::getInstantiatedItem(CConcept* concept) {
				return mConceptInstancesItemHash.value(concept);
			}

			CConceptInstanceItem* COptimizedKPSetOntologyConceptRealizingItem::getInstanceItem(CIndividual* individual) {
				return mIndividualInstantiatedItemHash.value(individual);
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetOntologyConceptRealizingItem::getTopInstancesItem() {
				return mTopInstancesItem;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetOntologyConceptRealizingItem::getBottomInstancesItem() {
				return mBottomInstancesItem;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestingPossibleSameIndividualCount() {
				return mTestingPossibleSameIndividualCount;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestingPossibleInstanceCount() {
				return mTestingPossibleInstanceCount;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setTestingPossibleSameIndividualCount(cint64 testCount) {
				mTestingPossibleSameIndividualCount = testCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setTestingPossibleInstanceCount(cint64 testCount) {
				mTestingPossibleInstanceCount = testCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incTestingPossibleSameIndividualCount(cint64 incCount) {
				mTestingPossibleSameIndividualCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decTestingPossibleSameIndividualCount(cint64 decCount) {
				mTestingPossibleSameIndividualCount -= decCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incTestingPossibleInstanceCount(cint64 incCount) {
				mTestingPossibleInstanceCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decTestingPossibleInstanceCount(cint64 decCount) {
				mTestingPossibleInstanceCount -= decCount;
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasTestingPossibleSameIndividual() {
				return mTestingPossibleSameIndividualCount > 0;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasTestingPossibleInstances() {
				return mTestingPossibleInstanceCount > 0;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
