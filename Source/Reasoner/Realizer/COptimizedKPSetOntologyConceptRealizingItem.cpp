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

#include "COptimizedKPSetOntologyConceptRealizingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetOntologyConceptRealizingItem::COptimizedKPSetOntologyConceptRealizingItem() {
			}

			COptimizedKPSetOntologyConceptRealizingItem::~COptimizedKPSetOntologyConceptRealizingItem() {
				delete mCalculationConfig;
				delete mRealizeConceptProcessingStep;
				delete mRealizeRoleProcessingStep;
				if (mTempRoleRealizationOntology != mOntology) {
					mTempRoleRealizationOntology->setConsistence(nullptr);
					delete mTempRoleRealizationOntology;
				}
				if (mTempSameRealizationOntology != mOntology) {
					mTempSameRealizationOntology->setConsistence(nullptr);
					delete mTempSameRealizationOntology;
				}
				if (mPossAssCollSet) {
					delete mPossAssCollSet;
				}

			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::initRequirementConfigRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				COntologyRealizingItem::initRealizingItem(ontology,config);
				mRealization = nullptr;
				mCalculationConfig = new CCalculationConfigurationExtension(config,0);
				COntologyProcessingStepDataVector* ontProStepDataVec = mOntology->getProcessingSteps()->getOntologyProcessingStepDataVector();
				mRealizeConceptProcessingStep = new CRealizingTestingStep(CRealizingTestingStep::REALIZECONCEPTPROCESSINGSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSCONCEPTREALIZE),this);
				mProcessingSteps.append(mRealizeConceptProcessingStep);
				mRealizeRoleProcessingStep = new CRealizingTestingStep(CRealizingTestingStep::REALIZEROLEPROCESSINGSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSROLEREALIZE),this);
				mProcessingSteps.append(mRealizeRoleProcessingStep);
				mRealizeSameIndividualsProcessingStep = new CRealizingTestingStep(CRealizingTestingStep::REALIZESAMEINDIVIDUALSPROCESSINGSTEP,ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE),this);
				mProcessingSteps.append(mRealizeSameIndividualsProcessingStep);
				mItemsInitialized = false;
				mRealizationSameIndividualsInitialized = false;
				mRealizationConceptsInitialized = false;
				mTestedPossibleConceptInstancesCount = 0;
				mOpenPossibleConceptInstancesCount = 0;
				mTestedPossibleSameIndividualsCount = 0;
				mOpenPossibleSameIndividualsCount = 0;
				mTopConceptInstancesItem = nullptr;
				mBottomConceptInstancesItem = nullptr;
				mTestingPossibleSameIndividualCount = 0;
				mTestingPossibleConceptInstanceCount = 0;
				mTestingPossibleRoleInstanceCount = 0;
				mTestingRoleInstanceCandidateCount = 0;
				mTestedPossibleRoleInstancesCount = 0;
				mOpenPossibleRoleInstancesCount = 0;
				mInitializedRoleInstancesCount = 0;
				mRemainingInitalizingRoleInstancesCount = 0;
				mTopRoleInstancesItem = nullptr;
				mBottomRoleInstancesItem = nullptr;
				mRealizationRolesInitialized = false;
				mTempRoleRealizationOntology = nullptr;
				mTempSameRealizationOntology = nullptr;
				mRoleRealizationInstalled = false;
				mConceptRealizationInstalled = false;
				mSameRealizationInstalled = false;

				mCountingPossibleConceptInstancesTesting = false;
				mCountingPossibleRoleInstancesTesting = false;
				mCountingPossibleSameInstancesTesting = false;

				mExtraConsistencyTestingRequired = false;
				mExtraConsistencyTesting = false;
				mExtraConsistencyTested = false;

				mIndiDepTrackReq = false;
				if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Realization.IndividualDependenceTracking",true)) {
					mIndiDepTrackReq = true;
				}

				mPossAssCollSet = nullptr;
				if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Realization.PossibleAssertionsCollecting",false)) {
					mPossAssCollSet = new CPossibleAssertionsCollectionSet();
				}


				if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Realization.ExtraConsistencyTesting",true)) {
					mExtraConsistencyTestingRequired = true;
				}

				mInitTime.start();

				return this;
			}



			CPossibleAssertionsCollectionSet* COptimizedKPSetOntologyConceptRealizingItem::getPossibleAssertionCollectionSet() {
				return mPossAssCollSet;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::initConceptItemsFromHierarchy() {
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
									mConceptInstancesItemContainer.append(instancesItem);
									mHierNodeInstancesItemHash.insert(hierNode,instancesItem);
									instancesItem->initInstancesItem(hierNode);
								}
								mConceptInstancesItemHash.insert(concept,instancesItem);
							}

							for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = mConceptInstancesItemContainer.constBegin(), itEnd = mConceptInstancesItemContainer.constEnd(); it != itEnd; ++it) {
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

							for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = mConceptInstancesItemContainer.constBegin(), itEnd = mConceptInstancesItemContainer.constEnd(); it != itEnd; ++it) {
								COptimizedKPSetConceptInstancesItem* item(*it);
								item->setInitialUnprocessedSuccessorItemCount();
								if (item->getUnprocessedSuccessorItemCount() <= 0) {
									item->setAllSuccessorProcessedFlag(true);
								}
							}

							mTopConceptInstancesItem = mHierNodeInstancesItemHash.value(taxonomy->getTopHierarchyNode());
							mBottomConceptInstancesItem = mHierNodeInstancesItemHash.value(taxonomy->getBottomHierarchyNode());

						}
					}
				}
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::initRoleCandidateInitializingIndividualItems() {
				for (QList<COptimizedKPSetIndividualItem*>::const_iterator it = mInstantiatedItemContainer.constBegin(), itEnd = mInstantiatedItemContainer.constEnd(); it != itEnd; ++it) {
					COptimizedKPSetIndividualItem* indiItem(*it);
					if (!indiItem->isItemSameIndividualMerged()) {
						addRoleCandidateInitalizingIndividualsItem(indiItem);
					}
				}


				return this;
			}



			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::initComplexRolesStarterCandidates() {
				CRBox* rbox = mOntology->getRBox();
				CRoleVector* roleVector = rbox->getRoleVector(false);
				CBOXSET<CRole*>* activeRoleSet = rbox->getActivePropertyRoleSet(false);
				if (roleVector && activeRoleSet) {
					for (CBOXSET<CRole*>::const_iterator roleIt = activeRoleSet->constBegin(), roleItEnd = activeRoleSet->constEnd(); roleIt != roleItEnd; ++roleIt) {
						CRole* role(*roleIt);
						if (!role->isDataRole() && role->isComplexRole()) {
							for (CXLinker<CRoleChain*>* chainLinkerIt = role->getRoleChainSubSharingLinker(); chainLinkerIt; chainLinkerIt = chainLinkerIt->getNext()) {
								CRoleChain* roleChain = chainLinkerIt->getData();
								CRole* firstRole = roleChain->getRoleChainLinker()->getData();
								CRole* firstInverseRole = nullptr;
								if (roleChain->getInverseRoleChainLinker()) {
									firstInverseRole = roleChain->getInverseRoleChainLinker()->getData();
								}
								for (CSortedNegLinker<CRole*>* indSuperRoleLinker = role->getIndirectSuperRoleList(); indSuperRoleLinker; indSuperRoleLinker = indSuperRoleLinker->getNext()) {
									CRole* superRole = indSuperRoleLinker->getData();
									bool superRoleInversed = indSuperRoleLinker->isNegated();
									COptimizedKPSetRoleInstancesItem* superRoleItem = mRoleInstancesItemHash.value(superRole);
									if (superRoleItem && superRoleItem != mTopRoleInstancesItem) {
										if (superRoleInversed) {
											if (firstInverseRole) {
												superRoleItem->addComplexRoleStarterCandidate(firstInverseRole);
											}
											if (superRoleItem->hasInverseRole()) {
												superRoleItem->addComplexInverseRoleStarterCandidate(firstRole);
											}
										} else {
											superRoleItem->addComplexRoleStarterCandidate(firstRole);
											if (firstInverseRole && superRoleItem->hasInverseRole()) {
												superRoleItem->addComplexInverseRoleStarterCandidate(firstInverseRole);
											}
										}
									}
								}
							}
						}
					}

				}
				return this;
			}



			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::initRoleItemsFromHierarchy() {
				CRBox* rbox = mOntology->getRBox();
				CRoleVector* roleVector = rbox->getRoleVector(false);
				CBOXSET<CRole*>* activeRoleSet = rbox->getActivePropertyRoleSet(false);
				if (roleVector && activeRoleSet) {
					for (CBOXSET<CRole*>::const_iterator roleIt = activeRoleSet->constBegin(), roleItEnd = activeRoleSet->constEnd(); roleIt != roleItEnd; ++roleIt) {
						CRole* role(*roleIt);
						if (!role->isDataRole() && role->hasPropertyName() && !mRedirectedRoleInstancesItemHash.contains(role)) {
							COptimizedKPSetRoleInstancesItem* instancesItem = new COptimizedKPSetRoleInstancesItem();
							mRoleInstancesItemContainer.append(instancesItem);
							mRoleInstancesItemHash.insert(role,instancesItem);
							mRedirectedRoleInstancesItemHash.insert(role,new COptimizedKPSetRoleInstancesRedirectionItem(instancesItem,role,false));
							instancesItem->initInstancesItem(role);
							CRole* inverseRole = nullptr;
							for (CSortedNegLinker<CRole*>* eqInvRoleIt = role->getInverseEquivalentRoleList(); eqInvRoleIt; eqInvRoleIt = eqInvRoleIt->getNext()) {
								CRole* eqInvRole = eqInvRoleIt->getData();
								mRedirectedRoleInstancesItemHash.insert(eqInvRole,new COptimizedKPSetRoleInstancesRedirectionItem(instancesItem,eqInvRole,eqInvRoleIt->isNegated()));
								if (eqInvRoleIt->isNegated()) {
									inverseRole = eqInvRole;
								}
							}
							for (CSortedNegLinker<CRole*>* indSupRoleIt = role->getIndirectSuperRoleList(); indSupRoleIt; indSupRoleIt = indSupRoleIt->getNext()) {
								CRole* superRole = indSupRoleIt->getData();
								bool superRoleInversed = indSupRoleIt->isNegated();
								if (!mRedirectedRoleInstancesItemHash.contains(superRole)) {
									for (CSortedNegLinker<CRole*>* supIndSupRoleIt = superRole->getIndirectSuperRoleList(); supIndSupRoleIt; supIndSupRoleIt = supIndSupRoleIt->getNext()) {
										CRole* superSuperRole = supIndSupRoleIt->getData();
										if (role == superSuperRole) {
											mRedirectedRoleInstancesItemHash.insert(superRole,new COptimizedKPSetRoleInstancesRedirectionItem(instancesItem,superSuperRole,superRoleInversed));
											if (superRoleInversed) {
												inverseRole = superRole;
											}
										}
									}
								}
							}
							if (inverseRole) {
								if (!instancesItem->hasInverseRole()) {
									instancesItem->setInverseRole(inverseRole);
								}
							}

							if (role->isComplexRole()) {
								mComplexRoleInstancesItemContainer.append(instancesItem);
							}
						}
					}

					mTopRoleInstancesItem = mRedirectedRoleInstancesItemHash.value(rbox->getTopObjectRole())->getRedirectedItem();
					mBottomRoleInstancesItem = mRedirectedRoleInstancesItemHash.value(rbox->getBottomObjectRole())->getRedirectedItem();

					for (QList<COptimizedKPSetRoleInstancesItem*>::const_iterator it = mRoleInstancesItemContainer.constBegin(), itEnd = mRoleInstancesItemContainer.constEnd(); it != itEnd; ++it) {
						COptimizedKPSetRoleInstancesItem* roleItem(*it);
						CRole* role = roleItem->getRole();
						for (CSortedNegLinker<CRole*>* eqInvRoleIt = role->getInverseEquivalentRoleList(); eqInvRoleIt; eqInvRoleIt = eqInvRoleIt->getNext()) {
							CRole* eqInvRole = eqInvRoleIt->getData();
							bool inversed = eqInvRoleIt->isNegated();
							for (CSortedNegLinker<CRole*>* eqInvSupRoleIt = eqInvRole->getSuperRoleList(); eqInvSupRoleIt; eqInvSupRoleIt = eqInvSupRoleIt->getNext()) {
								CRole* superRole = eqInvSupRoleIt->getData();
								bool superRoleInversed = eqInvSupRoleIt->isNegated() ^ inversed;
								COptimizedKPSetRoleInstancesRedirectionItem* redRoleRedirectionItem = mRedirectedRoleInstancesItemHash.value(superRole);
								COptimizedKPSetRoleInstancesItem* superRoleRedirectedItem = redRoleRedirectionItem->getRedirectedItem();
								bool superRoleRedirectionInversed = redRoleRedirectionItem->isInversed() ^ superRoleInversed;
								if (superRoleRedirectedItem != roleItem) {
									roleItem->addParentItem(superRoleRedirectedItem,superRoleRedirectionInversed);
									superRoleRedirectedItem->addSuccessorItem(roleItem,superRoleRedirectionInversed);
								}
							}
						}
						if (roleItem->getParentItemList()->isEmpty() && roleItem != mTopRoleInstancesItem) {
							roleItem->addParentItem(mTopRoleInstancesItem,false);
							mTopRoleInstancesItem->addSuccessorItem(roleItem,false);
						}
					}

					for (QList<COptimizedKPSetRoleInstancesItem*>::const_iterator it = mRoleInstancesItemContainer.constBegin(), itEnd = mRoleInstancesItemContainer.constEnd(); it != itEnd; ++it) {
						COptimizedKPSetRoleInstancesItem* item(*it);
						item->setInitialUnprocessedSuccessorItemCount();
						if (item->getUnprocessedSuccessorItemCount() <= 0) {
							item->setAllSuccessorProcessedFlag(true);
						}
						item->setInitialUninitializedSuccessorItemCount();
						if (item->getUninitializedSuccessorItemCount() <= 0) {
							item->setAllSuccessorInitializedFlag(true);
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


			bool COptimizedKPSetOntologyConceptRealizingItem::hasRealizationRolesInitialized() {
				return mRealizationRolesInitialized;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setRealizationRolesInitialized(bool initialized) {
				mRealizationRolesInitialized = initialized;
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setTemporaryRoleRealizationOntology(CConcreteOntology* tmpOntology) {
				mTempRoleRealizationOntology = tmpOntology;
				return this;
			}

			CConcreteOntology* COptimizedKPSetOntologyConceptRealizingItem::getTemporaryRoleRealizationOntology() {
				return mTempRoleRealizationOntology;
			}


			COntologyRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement) {	
				bool supportRequirement = false;
				COntologyProcessingStepRequirement* stepProcRequirement = dynamic_cast<COntologyProcessingStepRequirement*>(ontoRequirement);
				if (stepProcRequirement) {
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSCONCEPTREALIZE) {
						mRealizeConceptProcessingStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
						markIntanceItemForConceptRealization(mOntology->getTBox()->getTopConcept());
					} else if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSROLEREALIZE) {
						mRealizeRoleProcessingStep->addProcessingRequirement(stepProcRequirement);
						supportRequirement = true;
						markIntanceItemForRoleRealization(mOntology->getRBox()->getTopObjectRole());
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


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addRoleInstanceItemToProcessPossibleInstances(COptimizedKPSetRoleInstancesItem* instanceItem) {
				if (!instanceItem->hasPossibleInstancesProcessingQueuedFlag()) {
					if (instanceItem->hasPossibleInstances()) {
						mProcessingRoleInstancesItemList.append(instanceItem);
						instanceItem->setPossibleInstancesProcessingQueuedFlag(true);
					}
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addConceptInstanceItemToProcessPossibleInstances(COptimizedKPSetConceptInstancesItem* instanceItem) {
				if (!instanceItem->hasPossibleInstancesProcessingQueuedFlag()) {
					if (instanceItem->hasPossibleInstances()) {
						mProcessingConceptInstancesItemList.append(instanceItem);
						instanceItem->setPossibleInstancesProcessingQueuedFlag(true);
					}
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addRoleInstanceItemToInitializeCandidates(COptimizedKPSetRoleInstancesItem* instanceItem) {
				if (!instanceItem->hasInitializingQueuedFlag()) {
					if (instanceItem->requiresCandidateInitialization()) {
						mInitializingRoleInstancesItemList.append(instanceItem);
						instanceItem->setInitializingQueuedFlag(true);
					}
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addRoleInstanceIndividualItemToInitializeCandidates(COptimizedKPSetIndividualItem* indiItem) {
				if (!indiItem->hasInitializingRoleCandidatesQueuedFlag()) {
					mInitializingRoleInstancesIndividualItemList.append(indiItem);
					indiItem->setInitializingRoleCandidatesQueuedFlag(true);
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addInstantiatedItemToProcessPossibleSameIndividuals(COptimizedKPSetIndividualItem* instantiatedItem) {
				if (!instantiatedItem->hasPossibleSameIndividualsProcessingQueuedFlag()) {
					if (instantiatedItem->hasPossibleSameIndividuals()) {
						mProcessingSameIndividualsItemList.append(instantiatedItem);
						instantiatedItem->setPossibleSameIndividualsProcessingQueuedFlag(true);
					}
				}
				return this;
			}



			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::markIntanceItemForRoleRealization(CRole* role, bool subroleRealizationRequired) {
				COptimizedKPSetRoleInstancesRedirectionItem* initialRedirectionItem = mRedirectedRoleInstancesItemHash.value(role);
				QList<COptimizedKPSetRoleInstancesItem*> itemList;
				QSet<COptimizedKPSetRoleInstancesItem*> itemSet;
				itemList.append(initialRedirectionItem->getRedirectedItem());
				itemSet.insert(initialRedirectionItem->getRedirectedItem());
				while (!itemList.isEmpty()) {
					COptimizedKPSetRoleInstancesItem* item = itemList.takeFirst();
					if (!item->hasToProcessPossibleInstancesFlag() || !item->hasToInitializeCandidatesFlag()) {
						item->setToProcessPossibleInstancesFlag(true);
						item->setToInitializeCandidatesFlag(true);
						addRoleInstanceItemToInitializeCandidates(item);
						addRoleInstanceItemToProcessPossibleInstances(item);
						if (subroleRealizationRequired) {
							QList<TRoleItemInversionPair>* succItemList = item->getSuccessorItemList();
							for (QList<TRoleItemInversionPair>::const_iterator it = succItemList->constBegin(), itEnd = succItemList->constEnd(); it != itEnd; ++it) {
								TRoleItemInversionPair succItemPair(*it);
								if (!itemSet.contains(succItemPair.first)) {
									itemSet.insert(succItemPair.first);
									itemList.append(succItemPair.first);
								}
							}
						}
					}
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::markIntanceItemForConceptRealization(CConcept* concept, bool subconceptRealizationRequired) {
				COptimizedKPSetConceptInstancesItem* initialItem = mConceptInstancesItemHash.value(concept);
				QList<COptimizedKPSetConceptInstancesItem*> itemList;
				QSet<COptimizedKPSetConceptInstancesItem*> itemSet;
				itemList.append(initialItem);
				itemSet.insert(initialItem);
				while (!itemList.isEmpty()) {
					COptimizedKPSetConceptInstancesItem* item = itemList.takeFirst();
					if (!item->hasToProcessPossibleInstancesFlag()) {
						item->setToProcessPossibleInstancesFlag(true);
						addConceptInstanceItemToProcessPossibleInstances(item);
						if (subconceptRealizationRequired) {
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
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::markInstantiatedItemForSameIndividualsRealization() {
				for (QHash<CIndividual*,COptimizedKPSetIndividualItem*>::const_iterator it = mIndividualInstantiatedItemHash.constBegin(), itEnd = mIndividualInstantiatedItemHash.constEnd(); it != itEnd; ++it) {
					CIndividual* individual = it.key();
					COptimizedKPSetIndividualItem* indiItem = it.value();
					if (!indiItem->hasToProcessPossibleSameIndividualsFlag()) {
						indiItem->setToProcessPossibleSameIndividualsFlag(true);
						addInstantiatedItemToProcessPossibleSameIndividuals(indiItem);
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




			CRealizingTestingStep* COptimizedKPSetOntologyConceptRealizingItem::getRealizeRoleProcessingStep() {
				return mRealizeRoleProcessingStep;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::isRealizeRoleStepFinished() {
				return mRealizeRoleProcessingStep->isStepFinished();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::isRealizeRoleStepRequired() {
				return mRealizeRoleProcessingStep->hasRequirements();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::areRealizeRoleStepProcessingRequirementSatisfied() {
				return mRealizeRoleProcessingStep->areStepProcessingRequirementSatisfied();
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

			bool COptimizedKPSetOntologyConceptRealizingItem::hasRemainingInitializingRoleInstanceItems() {
				return !mInitializingRoleInstancesItemList.isEmpty();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasRemainingInitializingRoleInstanceIndividualItems() {
				return !mInitializingRoleInstancesIndividualItemList.isEmpty();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasRemainingProcessingRoleInstanceItems() {
				return !mProcessingRoleInstancesItemList.isEmpty();
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasRemainingProcessingConceptInstanceItems() {
				return !mProcessingConceptInstancesItemList.isEmpty();
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


			QHash<CRole*,COptimizedKPSetRoleInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getRoleInstancesItemHash() {
				return &mRoleInstancesItemHash;
			}


			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getRoleInstancesItemList() {
				return &mRoleInstancesItemContainer;
			}


			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getComplexRoleInstancesItemList() {
				return &mComplexRoleInstancesItemContainer;
			}


			QHash<CIndividual*,COptimizedKPSetIndividualItem*>* COptimizedKPSetOntologyConceptRealizingItem::getIndividualInstantiatedItemHash() {
				return &mIndividualInstantiatedItemHash;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetOntologyConceptRealizingItem::getIndividualInstantiatedItem(CIndividual* individual, bool directCreate) {
				COptimizedKPSetIndividualItem* instantiatedItem = nullptr;
				if (directCreate) {
					instantiatedItem = new COptimizedKPSetIndividualItem();
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


			QList<COptimizedKPSetConceptInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getProcessingPossibleConceptInstancesItemList() {
				return &mProcessingConceptInstancesItemList;
			}

			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getProcessingPossibleRoleInstancesItemList() {
				return &mProcessingRoleInstancesItemList;
			}

			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getInitializingRoleInstancesItemList() {
				return &mInitializingRoleInstancesItemList;
			}


			QList<COptimizedKPSetIndividualItem*>* COptimizedKPSetOntologyConceptRealizingItem::getInitializingRoleInstancesIndividualItemList() {
				return &mInitializingRoleInstancesIndividualItemList;
			}


			QList<COptimizedKPSetIndividualItem*>* COptimizedKPSetOntologyConceptRealizingItem::getProcessingPossibleSameIndividualsItemList() {
				return &mProcessingSameIndividualsItemList;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestedPossibleConceptInstancesCount() {
				return mTestedPossibleConceptInstancesCount;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getOpenPossibleConceptInstancesCount() {
				return mOpenPossibleConceptInstancesCount;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incTestedPossibleConceptInstancesCount(cint64 incCount) {
				mTestedPossibleConceptInstancesCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incOpenPossibleConceptInstancesCount(cint64 incCount) {
				mOpenPossibleConceptInstancesCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decTestedPossibleConceptInstancesCount(cint64 decCount) {
				mTestedPossibleConceptInstancesCount -= decCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decOpenPossibleConceptInstancesCount(cint64 decCount) {
				mOpenPossibleConceptInstancesCount -= decCount;
				return this;
			}








			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestedPossibleSameIndividualsCount() {
				return mTestedPossibleSameIndividualsCount;
			}


			cint64 COptimizedKPSetOntologyConceptRealizingItem::getOpenPossibleSameIndividualsCount() {
				return mOpenPossibleSameIndividualsCount;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incTestedPossibleSameIndividualsCount(cint64 incCount) {
				mTestedPossibleSameIndividualsCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incOpenPossibleSameIndividualsCount(cint64 incCount) {
				mOpenPossibleSameIndividualsCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decTestedPossibleSameIndividualsCount(cint64 decCount) {
				mTestedPossibleSameIndividualsCount -= decCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decOpenPossibleSameIndividualsCount(cint64 decCount) {
				mOpenPossibleSameIndividualsCount -= decCount;
				return this;
			}






			bool COptimizedKPSetOntologyConceptRealizingItem::visitDirectInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor) {
				COptimizedKPSetConceptInstancesItem* conceptItem = (COptimizedKPSetConceptInstancesItem*)item;
				if (conceptItem) {
					QSet<COptimizedKPSetIndividualItem*>* individualSet = conceptItem->getKnownInstancesSet();
					if (individualSet) {
						for (QSet<COptimizedKPSetIndividualItem*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetIndividualItem* individualItem(*it);
							if (!individualItem->isItemSameIndividualMerged()) {
								visitor->visitInstance(individualItem,this);
							}
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

						QSet<COptimizedKPSetIndividualItem*>* individualSet = procConceptItem->getKnownInstancesSet();
						if (individualSet) {
							for (QSet<COptimizedKPSetIndividualItem*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetIndividualItem* individualItem(*it);
								if (!individualItem->isItemSameIndividualMerged()) {
									visitor->visitInstance(individualItem,this);
								}
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
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)item;
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
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)item;
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
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)item;
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


			CRoleInstantiatedItem* COptimizedKPSetOntologyConceptRealizingItem::getRoleInstantiatedItem(CRole* role) {
				return mRedirectedRoleInstancesItemHash.value(role);
			}

			CRoleInstanceItem* COptimizedKPSetOntologyConceptRealizingItem::getRoleInstanceItem(CIndividual* individual) {
				return mIndividualInstantiatedItemHash.value(individual);
			}



			CSameInstanceItem* COptimizedKPSetOntologyConceptRealizingItem::getSameInstanceItem(CIndividual* individual) {
				return mIndividualInstantiatedItemHash.value(individual);
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::visitSameIndividuals(CSameInstanceItem* item, CSameRealizationIndividualVisitor* visitor) {
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)item;
				QSet<CIndividual*>* individualSet = individualItem->getKnownSameIndividualSet();
				for (QSet<CIndividual*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
					CIndividual* individual(*it);
					visitor->visitIndividual(individual,this);
				}
				return true;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::visitTargetIndividuals(CRoleInstanceItem* individualItem, CRoleInstantiatedItem* roleItem, CRoleRealizationInstanceVisitor* visitor) {
				COptimizedKPSetRoleInstancesRedirectionItem* roleRedirectionItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleItem;
				COptimizedKPSetRoleInstancesItem* roleInstItem = roleRedirectionItem->getRedirectedItem();
				bool roleItemInversed = roleRedirectionItem->isInversed();
				if (roleInstItem == mTopRoleInstancesItem) {
					for (QList<COptimizedKPSetIndividualItem*>::const_iterator it = mInstantiatedItemContainer.constBegin(), itEnd = mInstantiatedItemContainer.constEnd(); it != itEnd; ++it) {
						COptimizedKPSetIndividualItem* neighbourItem(*it);
						if (!neighbourItem->isItemSameIndividualMerged()) {
							visitor->visitRoleInstance(neighbourItem,this);
						}
					}
					return true;
				} else {
					COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)individualItem;
					COptimizedKPSetRoleNeighbourInstancesHash* roleNeighbourHash = indiItem->getKnownPossibleRoleNeighboursInstancesHash();
					const COptimizedKPSetRoleNeighbourInstancesHashData& roleNeighbourHashData = roleNeighbourHash->value(roleInstItem);
					COptimizedKPSetRoleInstancesHash* neighbourHash = nullptr;
					if (roleItemInversed) {
						neighbourHash = roleNeighbourHashData.mRolePredecessorInstancesHash;
					} else {
						neighbourHash = roleNeighbourHashData.mRoleSuccessorInstancesHash;
					}
					if (neighbourHash) {
						for (COptimizedKPSetRoleInstancesHash::const_iterator it = neighbourHash->constBegin(), itEnd = neighbourHash->constEnd(); it != itEnd; ++it) {
							const COptimizedKPSetRoleInstancesHashData& neighbourData = it.value();
							COptimizedKPSetIndividualItem* neighbourItem = it.key();
							if (!neighbourItem->isItemSameIndividualMerged()) {
								COptimizedKPSetRoleInstancesData* roleInstData = neighbourData.mInstanceItemData;
								if (roleInstData && roleInstData->mKnownInstance) {
									visitor->visitRoleInstance(neighbourItem,this);
								}
							}
						}
					}
					return true;
				}
			}




			bool COptimizedKPSetOntologyConceptRealizingItem::visitIndividuals(CRoleInstanceItem* item, CRoleRealizationIndividualVisitor* visitor) {
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)item;
				QSet<CIndividual*>* individualSet = individualItem->getKnownSameIndividualSet();
				for (QSet<CIndividual*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
					CIndividual* individual(*it);
					visitor->visitIndividual(individual,this);
				}
				return true;
			}









			COptimizedKPSetConceptInstancesItem* COptimizedKPSetOntologyConceptRealizingItem::getTopInstancesItem() {
				return mTopConceptInstancesItem;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetOntologyConceptRealizingItem::getBottomInstancesItem() {
				return mBottomConceptInstancesItem;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestingPossibleSameIndividualCount() {
				return mTestingPossibleSameIndividualCount;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestingPossibleConceptInstanceCount() {
				return mTestingPossibleConceptInstanceCount;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setTestingPossibleSameIndividualCount(cint64 testCount) {
				mTestingPossibleSameIndividualCount = testCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setTestingPossibleConceptInstanceCount(cint64 testCount) {
				mTestingPossibleConceptInstanceCount = testCount;
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

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incTestingPossibleConceptInstanceCount(cint64 incCount) {
				mTestingPossibleConceptInstanceCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decTestingPossibleConceptInstanceCount(cint64 decCount) {
				mTestingPossibleConceptInstanceCount -= decCount;
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasTestingPossibleSameIndividual() {
				return mTestingPossibleSameIndividualCount > 0;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasTestingPossibleConceptInstances() {
				return mTestingPossibleConceptInstanceCount > 0;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestingPossibleRoleInstanceCount() {
				return mTestingPossibleRoleInstanceCount;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setTestingPossibleRoleInstanceCount(cint64 testCount) {
				mTestingPossibleRoleInstanceCount = testCount;
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasTestingPossibleRoleInstances() {
				return mTestingPossibleRoleInstanceCount > 0;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incTestingPossibleRoleInstanceCount(cint64 incCount) {
				mTestingPossibleRoleInstanceCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decTestingPossibleRoleInstanceCount(cint64 decCount) {
				mTestingPossibleRoleInstanceCount -= decCount;
				return this;
			}


			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestedPossibleRoleInstancesCount() {
				return mTestedPossibleRoleInstancesCount;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getOpenPossibleRoleInstancesCount() {
				return mOpenPossibleRoleInstancesCount;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getInitializedRoleInstancesCount() {
				return mInitializedRoleInstancesCount;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getRemaningInitalizingRoleInstancesCount() {
				return mRemainingInitalizingRoleInstancesCount;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incTestedPossibleRoleInstancesCount(cint64 incCount) {
				mTestedPossibleRoleInstancesCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incOpenPossibleRoleInstancesCount(cint64 incCount) {
				mOpenPossibleRoleInstancesCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decTestedPossibleRoleInstancesCount(cint64 decCount) {
				mTestedPossibleRoleInstancesCount -= decCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decOpenPossibleRoleInstancesCount(cint64 decCount) {
				mOpenPossibleRoleInstancesCount -= decCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incInitializedRoleInstancesCount(cint64 incCount) {
				mInitializedRoleInstancesCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decInitializedRoleInstancesCount(cint64 decCount) {
				mInitializedRoleInstancesCount -= decCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incRemaningInitalizingInstancesCount(cint64 incCount) {
				mRemainingInitalizingRoleInstancesCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decRemaningInitalizingInstancesCount(cint64 decCount) {
				mRemainingInitalizingRoleInstancesCount -= decCount;
				return this;
			}


			COptimizedKPSetRoleInstancesItem* COptimizedKPSetOntologyConceptRealizingItem::getTopRoleInstancesItem() {
				return mTopRoleInstancesItem;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetOntologyConceptRealizingItem::getBottomRoleInstancesItem() {
				return mBottomRoleInstancesItem;
			}


			QHash<CConcept*,COptimizedKPSetRoleInstancesItem*>* COptimizedKPSetOntologyConceptRealizingItem::getMarkerConceptInstancesItemHash() {
				return &mMarkerConceptInstancesItemHash;
			}



			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addRoleCandidateInitalizingIndividualsItem(COptimizedKPSetIndividualItem* indiItem) {
				mRoleCandidateInitalizingIndividualsItemList.append(indiItem);
				return this;
			}

			QList<COptimizedKPSetIndividualItem*>* COptimizedKPSetOntologyConceptRealizingItem::getRoleCandidateInitalizingIndividualsItemList() {
				return &mRoleCandidateInitalizingIndividualsItemList;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::isConceptRealizationInstalled() {
				return mConceptRealizationInstalled;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setConceptRealizationInstalled(bool installed) {
				mConceptRealizationInstalled = installed;;
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::isRoleRealizationInstalled() {
				return mRoleRealizationInstalled;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setRoleRealizationInstalled(bool installed) {
				mRoleRealizationInstalled = installed;
				return this;
			}



			bool COptimizedKPSetOntologyConceptRealizingItem::isSameRealizationInstalled() {
				return mSameRealizationInstalled;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setSameRealizationInstalled(bool installed) {
				mSameRealizationInstalled = installed;;
				return this;
			}

			cint64 COptimizedKPSetOntologyConceptRealizingItem::getTestingRoleInstanceCandidatesCount() {
				return mTestingRoleInstanceCandidateCount;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setTestingRoleInstanceCandidatesCount(cint64 testCount) {
				mTestingRoleInstanceCandidateCount = testCount;
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::hasTestingRoleInstanceCandidates() {
				return mTestingRoleInstanceCandidateCount > 0;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::incTestingRoleInstanceCandidatesCount(cint64 incCount) {
				mTestingRoleInstanceCandidateCount += incCount;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::decTestingRoleInstanceCandidatesCount(cint64 decCount) {
				mTestingRoleInstanceCandidateCount -= decCount;
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::isCountingPossibleConceptInstancesTesting() {
				return mCountingPossibleConceptInstancesTesting;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::isCountingPossibleRoleInstancesTesting() {
				return mCountingPossibleRoleInstancesTesting;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::isCountingPossibleSameInstancesTesting() {
				return mCountingPossibleSameInstancesTesting;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setCountingPossibleConceptInstancesTesting(bool counting) {
				mCountingPossibleConceptInstancesTesting = counting;
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setCountingPossibleRoleInstancesTesting(bool counting) {
				mCountingPossibleRoleInstancesTesting = counting;
				return this;
			}
			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setCountingPossibleSameInstancesTesting(bool counting) {
				mCountingPossibleSameInstancesTesting = counting;
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setTemporarySameRealizationOntology(CConcreteOntology* tmpOntology) {
				mTempSameRealizationOntology = tmpOntology;
				return this;
			}

			CConcreteOntology* COptimizedKPSetOntologyConceptRealizingItem::getTemporarySameRealizationOntology() {
				return mTempSameRealizationOntology;
			}


			QList<COptimizedKPSetIndividualItem*>* COptimizedKPSetOntologyConceptRealizingItem::getPossibleSameIndividualsItemList() {
				return &mPossibleSameIndiItemList;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::addPossibleSameIndividualsItem(COptimizedKPSetIndividualItem* indiItem) {
				mPossibleSameIndiItemList.append(indiItem);
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::requiresIndividualDependenceTracking() {
				return mIndiDepTrackReq;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setIndividualDependenceTrackingRequired(bool indiDepTrackingRequired) {
				mIndiDepTrackReq = indiDepTrackingRequired;
				return this;
			}


			bool COptimizedKPSetOntologyConceptRealizingItem::isExtraConsistencyTestingStepRequired() {
				return mExtraConsistencyTestingRequired;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setExtraConsistencyTestingStepRequired(bool required) {
				mExtraConsistencyTestingRequired = required;
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::isExtraConsistencyTesting() {
				return mExtraConsistencyTesting;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setExtraConsistencyTesting(bool testing) {
				mExtraConsistencyTesting = testing;
				return this;
			}

			bool COptimizedKPSetOntologyConceptRealizingItem::isExtraConsistencyTested() {
				return mExtraConsistencyTested;
			}

			COptimizedKPSetOntologyConceptRealizingItem* COptimizedKPSetOntologyConceptRealizingItem::setExtraConsistencyTested(bool tested) {
				mExtraConsistencyTested = tested;
				return this;
			}


			QList<COptimizedKPSetIndividualItem*>* COptimizedKPSetOntologyConceptRealizingItem::getInstantiatedItemList() {
				return &mInstantiatedItemContainer;
			}

			QTime* COptimizedKPSetOntologyConceptRealizingItem::getInitializationTime() {
				return &mInitTime;
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
