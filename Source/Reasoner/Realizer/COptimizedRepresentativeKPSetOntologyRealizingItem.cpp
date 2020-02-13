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

#include "COptimizedRepresentativeKPSetOntologyRealizingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetOntologyRealizingItem::COptimizedRepresentativeKPSetOntologyRealizingItem() {
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem::~COptimizedRepresentativeKPSetOntologyRealizingItem() {
				delete mCalculationConfig;
				delete mInitRealizeProcessingStep;
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

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::initRequirementConfigRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader) {
				COntologyRealizingItem::initRealizingItem(ontology,config);
				mBackendAssocCacheReader = backendAssocCacheReader;
				mSameMergedIndisInCache = mBackendAssocCacheReader->hasSameIndividualsMergings();
				mRealization = nullptr;
				mCalculationConfig = new CCalculationConfigurationExtension(config,0);
				COntologyProcessingStepDataVector* ontProStepDataVec = mOntology->getProcessingSteps()->getOntologyProcessingStepDataVector();
				mInitRealizeProcessingStep = new CRealizingTestingStep(CRealizingTestingStep::INITREALIZEPROCESSINGSTEP, ontProStepDataVec->getProcessingStepData(COntologyProcessingStep::OPSINITREALIZE), this);
				mProcessingSteps.append(mInitRealizeProcessingStep);
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
				mPropagatingRoleInstanceCandidateCount = 0;
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
				mAllSameIndividualsProcessing = false;
				mAllRoleInstancesProcessing = false;
				mAllConceptInstancesProcessing = false;

				mProvidedPossibleInstancesMergingIndividualsCount = 0;
				mProvidedPossibleInstancesMergingsCount = 0;
				mAttemptedPossibleInstancesMergingsCount = 0;
				mSucceddedPossibleInstancesMergingsCount = 0;

				mModelMergingsTriedCount = 0;
				mModelMergingsSucessCount = 0;
				mModelMergingsInstanceFoundCount = 0;
				mModelMergingsNonInstanceFoundCount = 0;

				mCountingPossibleConceptInstancesTesting = false;
				mCountingPossibleRoleInstancesTesting = false;
				mCountingPossibleSameInstancesTesting = false;

				mExtraConsistencyTestingRequired = false;
				mExtraConsistencyTesting = false;
				mExtraConsistencyTested = false;

				mPotentiallySameIndividuals = false;
				mIteratorNothingToDoWarning = false;

				mIndiDepTrackReq = false;
				if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Realization.IndividualDependenceTracking",true)) {
					mIndiDepTrackReq = true;
				}

				mPossAssCollSet = nullptr;
				if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Realization.PossibleAssertionsCollecting",false)) {
					mPossAssCollSet = new CPossibleAssertionsCollectionSet();
				}


				if (CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Realization.ExtraConsistencyTesting",false)) {
					mExtraConsistencyTestingRequired = true;
				}

				mInitTime.start();

				return this;
			}



			CPossibleAssertionsCollectionSet* COptimizedRepresentativeKPSetOntologyRealizingItem::getPossibleAssertionCollectionSet() {
				return mPossAssCollSet;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::initConceptItemsFromHierarchy() {
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

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::initRoleCandidateInitializingIndividualItems() {
				for (QList<COptimizedKPSetIndividualItem*>::const_iterator it = mInstantiatedItemContainer.constBegin(), itEnd = mInstantiatedItemContainer.constEnd(); it != itEnd; ++it) {
					COptimizedKPSetIndividualItem* indiItem(*it);
					if (!indiItem->isItemSameIndividualMerged()) {
						addRoleCandidateInitalizingIndividualsItem(indiItem);
					}
				}


				return this;
			}



			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::initComplexRolesStarterCandidates() {
				CRBox* rbox = mOntology->getRBox();
				CRoleVector* roleVector = rbox->getRoleVector(false);
				CBOXSET<CRole*>* activeRoleSet = rbox->getActivePropertyRoleSet(false);

				if (roleVector && activeRoleSet) {
					QHash<CRole*, CRole*> complexRoleStarterCandExpansionHash;
					QSet<QPair<CRole*, CRole*>> complexRoleStarterCandExpansionSet;
					QList<QPair<CRole*, CRole*>> complexRoleStarterCandExpansionList;

					QHash<CRole*, CRole*> complexRoleInverseStarterCandExpansionHash;
					QSet<QPair<CRole*, CRole*>> complexRoleInverseStarterCandExpansionSet;
					QList<QPair<CRole*, CRole*>> complexRoleInverseStarterCandExpansionList;

					for (CBOXSET<CRole*>::const_iterator roleIt = activeRoleSet->constBegin(), roleItEnd = activeRoleSet->constEnd(); roleIt != roleItEnd; ++roleIt) {
						CRole* role(*roleIt);
						if (!role->isDataRole() && role->isComplexRole()) {
							for (CXLinker<CRoleChain*>* chainLinkerIt = role->getRoleChainSuperSharingLinker(); chainLinkerIt; chainLinkerIt = chainLinkerIt->getNext()) {
								CRoleChain* roleChain = chainLinkerIt->getData();
								CRole* firstRole = roleChain->getRoleChainLinker()->getData();

								bool onlyTransitiveComplexRoleCandidates = true;
								for (CXLinker<CRole*>* roleChainLinkerIt = roleChain->getRoleChainLinker(); roleChainLinkerIt && onlyTransitiveComplexRoleCandidates; roleChainLinkerIt = roleChainLinkerIt->getNext()) {
									CRole* chainedRole = roleChainLinkerIt->getData();
									if (chainedRole != role) {
										onlyTransitiveComplexRoleCandidates = false;
									}
								}

								CRole* firstInverseRole = nullptr;
								if (roleChain->getInverseRoleChainLinker()) {
									firstInverseRole = roleChain->getInverseRoleChainLinker()->getData()->getInverseRole();
								}
								CRoleInstantiatedItem* roleItem = getRoleInstantiatedItem(role);
								COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleItem;
								COptimizedKPSetRoleInstancesItem* roleInstItem = redRoleItem->getRedirectedItem();
								bool itemInversed = redRoleItem->isInversed();

								for (CSortedNegLinker<CRole*>* indSuperRoleLinker = role->getIndirectSuperRoleList(); indSuperRoleLinker; indSuperRoleLinker = indSuperRoleLinker->getNext()) {
									CRole* superRole = indSuperRoleLinker->getData();
									bool superRoleInversed = indSuperRoleLinker->isNegated();

									if (!superRoleInversed) {
										if (!complexRoleStarterCandExpansionSet.contains(QPair<CRole*, CRole*>(superRole, firstRole))) {
											complexRoleStarterCandExpansionSet.insert(QPair<CRole*, CRole*>(superRole, firstRole));
											complexRoleStarterCandExpansionHash.insertMulti(superRole, firstRole);
										}
										if (superRole->isComplexRole()) {
											complexRoleStarterCandExpansionList.append(QPair<CRole*, CRole*>(superRole, firstRole));
										}
									} else {
										if (!complexRoleInverseStarterCandExpansionSet.contains(QPair<CRole*, CRole*>(superRole, firstRole))) {
											complexRoleInverseStarterCandExpansionSet.insert(QPair<CRole*, CRole*>(superRole, firstRole));
											complexRoleInverseStarterCandExpansionHash.insertMulti(superRole, firstRole);
										}
										if (superRole->isComplexRole()) {
											complexRoleInverseStarterCandExpansionList.append(QPair<CRole*, CRole*>(superRole, firstRole));
										}
									}
									if (firstInverseRole) {
										if (!superRoleInversed) {
											if (!complexRoleInverseStarterCandExpansionSet.contains(QPair<CRole*, CRole*>(superRole, firstInverseRole))) {
												complexRoleInverseStarterCandExpansionSet.insert(QPair<CRole*, CRole*>(superRole, firstInverseRole));
												complexRoleInverseStarterCandExpansionHash.insertMulti(superRole, firstInverseRole);
											}
											if (superRole->isComplexRole()) {
												complexRoleInverseStarterCandExpansionList.append(QPair<CRole*, CRole*>(superRole, firstInverseRole));
											}
										} else {
											if (!complexRoleStarterCandExpansionSet.contains(QPair<CRole*, CRole*>(superRole, firstInverseRole))) {
												complexRoleStarterCandExpansionSet.insert(QPair<CRole*, CRole*>(superRole, firstInverseRole));
												complexRoleStarterCandExpansionHash.insertMulti(superRole, firstInverseRole);
											}
											if (superRole->isComplexRole()) {
												complexRoleStarterCandExpansionList.append(QPair<CRole*, CRole*>(superRole, firstInverseRole));
											}
										}
									}

									COptimizedKPSetRoleInstancesItem* superRoleItem = mRoleInstancesItemHash.value(superRole);
									if (superRoleItem && superRoleItem != mTopRoleInstancesItem) {
										if (superRoleItem->hasOnlyTransitiveComplexRoleCandidates()) {
											superRoleItem->setOnlyTransitiveComplexRoleCandidates(onlyTransitiveComplexRoleCandidates);
										}
										if (superRoleItem->hasOnlyTransitiveComplexRoleCandidates()) {
											superRoleItem->addOnlyTransitiveComplexSubRoleItem(TRoleItemInversionPair(roleInstItem, superRoleInversed ^ itemInversed));
										}
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


					while (!complexRoleStarterCandExpansionList.isEmpty()) {
						QPair<CRole*, CRole*> roleStarterExpPair = complexRoleStarterCandExpansionList.takeFirst();
						CRole* superRole = roleStarterExpPair.first;

						COptimizedKPSetRoleInstancesItem* superRoleItem = mRoleInstancesItemHash.value(superRole);
						if (superRoleItem && superRoleItem != mTopRoleInstancesItem) {
							CRole* subRole = roleStarterExpPair.second;

							QSet<CRole*> subRolesCandidateSet;
							QList<CRole*> subRolesCandidateList(complexRoleStarterCandExpansionHash.values(subRole));
							for (CRole* role : subRolesCandidateList) {
								subRolesCandidateSet.insert(role);
							}

							while (!subRolesCandidateList.isEmpty()) {
								CRole* role = subRolesCandidateList.takeFirst();
								if (!superRoleItem->hasComplexRoleStarterCandidate(role)) {
									superRoleItem->addComplexRoleStarterCandidate(role);

									QList<CRole*> nextSubRolesCandidateList(complexRoleStarterCandExpansionHash.values(role));
									for (CRole* nextSubRole : nextSubRolesCandidateList) {
										if (!subRolesCandidateSet.contains(role)) {
											subRolesCandidateSet.insert(role);
											subRolesCandidateList.append(nextSubRole);
										}
									}

								}
							}

						}

					}




					while (!complexRoleInverseStarterCandExpansionList.isEmpty()) {
						QPair<CRole*, CRole*> roleStarterExpPair = complexRoleInverseStarterCandExpansionList.takeFirst();
						CRole* superRole = roleStarterExpPair.first;

						COptimizedKPSetRoleInstancesItem* superRoleItem = mRoleInstancesItemHash.value(superRole);
						if (superRoleItem && superRoleItem != mTopRoleInstancesItem) {
							CRole* subRole = roleStarterExpPair.second;

							QSet<CRole*> subRolesCandidateSet;
							QList<CRole*> subRolesCandidateList(complexRoleStarterCandExpansionHash.values(subRole));
							for (CRole* role : subRolesCandidateList) {
								subRolesCandidateSet.insert(role);
							}

							while (!subRolesCandidateList.isEmpty()) {
								CRole* role = subRolesCandidateList.takeFirst();
								if (!superRoleItem->hasComplexInverseRoleStarterCandidate(role)) {
									superRoleItem->addComplexInverseRoleStarterCandidate(role);

									QList<CRole*> nextSubRolesCandidateList(complexRoleStarterCandExpansionHash.values(role));
									for (CRole* nextSubRole : nextSubRolesCandidateList) {
										if (!subRolesCandidateSet.contains(role)) {
											subRolesCandidateSet.insert(role);
											subRolesCandidateList.append(nextSubRole);
										}
									}

								}
							}

						}

					}

				}


				for (COptimizedKPSetRoleInstancesItem* complexRoleItem : mComplexRoleInstancesItemContainer) {
					for (CRole* complexStarterCandidateRole : *complexRoleItem->getComplexRoleStarterCandidateSet()) {
						mComplexStarterCandidateRoleInstancesItemHash.insertMulti(complexStarterCandidateRole, complexRoleItem);
					}
					for (CRole* complexInverseStarterCandidateRole : *complexRoleItem->getComplexInverseRoleStarterCandidateSet()) {
						mInverseComplexStarterCandidateRoleInstancesItemHash.insertMulti(complexInverseStarterCandidateRole, complexRoleItem);
					}
				}


				return this;
			}



			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::initRoleItemsFromHierarchy() {
				CClassification* classification = mOntology->getClassification();
				CPropertyRoleClassification* roleClassification = nullptr; 
				if (classification && classification->hasObjectPropertyRoleClassification()) {
					roleClassification = classification->getObjectPropertyRoleClassification();
				}
				CRolePropertiesHierarchy* roleHierarchy = nullptr;
				if (roleClassification) {
					roleHierarchy = roleClassification->getRolePropertiesHierarchy();
				}
				CRBox* rbox = mOntology->getRBox();
				CRoleVector* roleVector = rbox->getRoleVector(false);
				CBOXSET<CRole*>* activeRoleSet = rbox->getActivePropertyRoleSet(false);
				if (roleVector && activeRoleSet) {
					for (CBOXSET<CRole*>::const_iterator roleIt = activeRoleSet->constBegin(), roleItEnd = activeRoleSet->constEnd(); roleIt != roleItEnd; ++roleIt) {
						CRole* role(*roleIt);

						//if (role && (CIRIName::getRecentIRIName(role->getPropertyNameLinker()) == "http://ontology.dumontierlab.com/isRelatedTo" || CIRIName::getRecentIRIName(role->getPropertyNameLinker()) == "http://ontology.dumontierlab.com/isTagOf")) {
						//	bool debug = true;
						//}

						if (!role->isDataRole() && role->hasPropertyName() && !mRedirectedRoleInstancesItemHash.contains(role)) {
							COptimizedKPSetRoleInstancesItem* instancesItem = new COptimizedKPSetRoleInstancesItem();
							mRoleInstancesItemContainer.append(instancesItem);
							mRoleInstancesItemHash.insert(role,instancesItem);
							COptimizedKPSetRoleInstancesRedirectionItem* roleRedItem = new COptimizedKPSetRoleInstancesRedirectionItem(instancesItem, role, false);
							mRedirectedRoleInstancesItemHash.insert(role, roleRedItem);
							instancesItem->initInstancesItem(role);
							CRole* inverseRole = nullptr;

							if (roleHierarchy) {

								CRolePropertiesHierarchyNode* invRoleHierNode = nullptr;

								CRolePropertiesHierarchyNode* hierNode = roleHierarchy->getHierarchyNode(role);
								instancesItem->setRoleHierarchyNode(hierNode);

								typedef QPair<CRole*, bool> TRoleInversionPair;
								QList<TRoleInversionPair> eqInRoleProcessingList;
								QSet<TRoleInversionPair> eqInRoleProcessingSet;
								eqInRoleProcessingList.append(TRoleInversionPair(role, false));
								eqInRoleProcessingSet.insert(TRoleInversionPair(role, false));

								QList<CRole*>* eqRoleList = hierNode->getEquivalentElementList();
								for (QList<CRole*>::const_iterator it = eqRoleList->constBegin(), itEnd = eqRoleList->constEnd(); it != itEnd; ++it) {
									CRole* eqRole(*it);
									if (role != eqRole) {
										eqInRoleProcessingList.append(TRoleInversionPair(eqRole, false));
										eqInRoleProcessingSet.insert(TRoleInversionPair(eqRole, false));
									}
								}

								while (!eqInRoleProcessingList.isEmpty()) {
									TRoleInversionPair procRoleInvPair = eqInRoleProcessingList.takeFirst();
									CRole* procRole = procRoleInvPair.first;
									bool procRoleInversion = procRoleInvPair.second;

									if (procRole != role) {
										COptimizedKPSetRoleInstancesRedirectionItem* invRedirectItem = roleRedItem;
										if (procRole != role) {
											invRedirectItem = new COptimizedKPSetRoleInstancesRedirectionItem(instancesItem, procRole, procRoleInversion);
											mRedirectedRoleInstancesItemHash.insert(procRole, invRedirectItem);
										}
										if (!invRoleHierNode && procRoleInversion) {
											inverseRole = procRole;
											invRoleHierNode = roleHierarchy->getHierarchyNode(inverseRole);
											instancesItem->setInverseRoleHierarchyNode(invRoleHierNode);
											instancesItem->setInverseRoleRedirectedItem(invRedirectItem);
										}
									} else if (!invRoleHierNode && procRoleInversion) {
										inverseRole = procRole;
										instancesItem->setInverseRoleHierarchyNode(hierNode);
									}


									if (procRole->getInverseRole()) {
										TRoleInversionPair procInverseRoleInvPair(procRole->getInverseRole(), !procRoleInversion);
										if (!eqInRoleProcessingSet.contains(procInverseRoleInvPair)) {
											eqInRoleProcessingSet.insert(procInverseRoleInvPair);
											eqInRoleProcessingList.append(procInverseRoleInvPair);
										}
									}
									for (CSortedNegLinker<CRole*>* eqInvRoleIt = procRole->getInverseEquivalentRoleList(); eqInvRoleIt; eqInvRoleIt = eqInvRoleIt->getNext()) {
										CRole* eqInvRole = eqInvRoleIt->getData();
										TRoleInversionPair procInverseRoleInvPair(eqInvRole, eqInvRoleIt->isNegated() ^ procRoleInversion);
										if (!eqInRoleProcessingSet.contains(procInverseRoleInvPair)) {
											eqInRoleProcessingSet.insert(procInverseRoleInvPair);
											eqInRoleProcessingList.append(procInverseRoleInvPair);
										}
									}
								}


							} else {

								for (CSortedNegLinker<CRole*>* eqInvRoleIt = role->getInverseEquivalentRoleList(); eqInvRoleIt; eqInvRoleIt = eqInvRoleIt->getNext()) {
									CRole* eqInvRole = eqInvRoleIt->getData();
									if (eqInvRole != role) {
										mRedirectedRoleInstancesItemHash.insert(eqInvRole, new COptimizedKPSetRoleInstancesRedirectionItem(instancesItem, eqInvRole, eqInvRoleIt->isNegated()));
									}
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

					CRole* topRole = rbox->getTopObjectRole();
					CRole* bottomRole = rbox->getBottomObjectRole();

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
								if (redRoleRedirectionItem) {
									COptimizedKPSetRoleInstancesItem* superRoleRedirectedItem = redRoleRedirectionItem->getRedirectedItem();
									bool superRoleRedirectionInversed = redRoleRedirectionItem->isInversed() ^ superRoleInversed;
									if (superRoleRedirectedItem != roleItem) {
										roleItem->addParentItem(superRoleRedirectedItem, superRoleRedirectionInversed);
										superRoleRedirectedItem->addSuccessorItem(roleItem, superRoleRedirectionInversed);
									}
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


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasItemsInitialized() {
				return mItemsInitialized;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setItemsInitialized(bool initialized) {
				mItemsInitialized = initialized;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRealizationSameIndividualsInitialized() {
				return mRealizationSameIndividualsInitialized;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setRealizationSameIndividualsInitialized(bool initialized) {
				mRealizationSameIndividualsInitialized = initialized;
				return this;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRealizationConceptsInitialized() {
				return mRealizationConceptsInitialized;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setRealizationConceptsInitialized(bool initialized) {
				mRealizationConceptsInitialized = initialized;
				return this;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRealizationRolesInitialized() {
				return mRealizationRolesInitialized;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setRealizationRolesInitialized(bool initialized) {
				mRealizationRolesInitialized = initialized;
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setTemporaryRoleRealizationOntology(CConcreteOntology* tmpOntology) {
				mTempRoleRealizationOntology = tmpOntology;
				return this;
			}

			CConcreteOntology* COptimizedRepresentativeKPSetOntologyRealizingItem::getTemporaryRoleRealizationOntology() {
				return mTempRoleRealizationOntology;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingItem::ensureIteratorPositionRealization(COntologyRealizingDynamicIteratorRequirmentDataCallback* iteratorReqDataCallback, COntologyRealizingDynamicRequirmentProcessingData* newReqProcDat, CRealizationIndividualInstanceItemReferenceIterator* iterator, const QList<CRealizationIndividualInstanceItemReferenceIterator*>& initializationCheckIteratorList) {

				bool processing = false;
				CRealizationIndividualInstanceItemReference currentPosIndiItemRef = iterator->currentIndividualInstanceItemReference();
				//if (prevCheckingPosIndiItemRef.getIndividualID() != currentPosIndiItemRef.getIndividualID()) {
				//	iterator2->begin();
				//	iterator2->requiresInitialization();
				//}


				for (CRealizationIndividualInstanceItemReferenceIterator* initCheckIterator : initializationCheckIteratorList) {
					COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator* individualRoleFillerIterator = dynamic_cast<COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator*>(initCheckIterator);
					if (individualRoleFillerIterator && individualRoleFillerIterator->requiresInitialization()) {
						if (queueRoleInstanceRealization(newReqProcDat, individualRoleFillerIterator->getQueryingIndividualInstanceItemReference(), currentPosIndiItemRef, individualRoleFillerIterator->getQueryingRoleInstancesItem()->getRole(), individualRoleFillerIterator->getQueryingRoleInstancesItemInversion())) {
							iteratorReqDataCallback->incRoleRealizationCount();
							processing = true;
						}
					}

					COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator* unsortedIndividualRoleFillerIterator = dynamic_cast<COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator*>(initCheckIterator);
					if (unsortedIndividualRoleFillerIterator && unsortedIndividualRoleFillerIterator->requiresInitialization()) {
						if (queueRoleInstanceRealization(newReqProcDat, unsortedIndividualRoleFillerIterator->getQueryingIndividualInstanceItemReference(), currentPosIndiItemRef, unsortedIndividualRoleFillerIterator->getQueryingRoleInstancesItem()->getRole(), unsortedIndividualRoleFillerIterator->getQueryingRoleInstancesItemInversion())) {
							iteratorReqDataCallback->incRoleRealizationCount();
							processing = true;
						}
					}

					COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator* roleNeighbouringIterator = dynamic_cast<COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator*>(initCheckIterator);
					if (roleNeighbouringIterator && roleNeighbouringIterator->requiresInitialization()) {
						QSet<TRoleInstancesItemInversedFlagPair>* roleInstancesItemInversedFlagPairSet = roleNeighbouringIterator->currentRoleInstancesItemInversionPairSet();
						for (TRoleInstancesItemInversedFlagPair roleInstancesItemInversedFlagPair : *roleInstancesItemInversedFlagPairSet) {
							COptimizedKPSetRoleInstancesItem* roleItem = roleInstancesItemInversedFlagPair.first;
							bool roleItemInversion = roleInstancesItemInversedFlagPair.second;
							// TODO: initialize and determine only one certain filler
							if (queueRoleFillerInstanceRealization(newReqProcDat, currentPosIndiItemRef, roleItem->getRole(), roleItemInversion)) {
								iteratorReqDataCallback->incRoleRealizationCount();
								processing = true;
							}
						}
					}

					COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator* conceptLabelIterator = dynamic_cast<COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator*>(initCheckIterator);
					if (conceptLabelIterator && conceptLabelIterator->requiresInitialization()) {
						QSet<TConceptInstancesItemMostSpecificFlagPair>* conceptInstancesItemOnlyMostSpecificFlagPairSet = conceptLabelIterator->currentConceptInstancesItemOnlyMostSpecificFlagPairSet();
						for (TConceptInstancesItemMostSpecificFlagPair conceptInstancesItemOnlyMostSpecificFlagPair : *conceptInstancesItemOnlyMostSpecificFlagPairSet) {
							COptimizedKPSetConceptInstancesItem* conceptItem = conceptInstancesItemOnlyMostSpecificFlagPair.first;
							bool mostSpecificOnly = conceptInstancesItemOnlyMostSpecificFlagPair.second;
							if (queueConceptInstanceRealization(newReqProcDat, currentPosIndiItemRef, conceptItem->getHierarchyNode()->getOneEquivalentConcept(), mostSpecificOnly)) {
								iteratorReqDataCallback->incConceptRealizationCount();
								processing = true;
							}
						}
					}
				}

				return processing;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingItem::handleIteratorRequirement(COntologyRealizingDynamicIteratorRequirmentDataCallback* iteratorReqDataCallback, COntologyRealizingDynamicRequirmentProcessingData* newReqProcDat, COntologyRealizingDynamicRequirmentProcessingData* procData) {
				COntologyProcessingIteratorRealizationRequirement* dynamicIteratorRealReq = iteratorReqDataCallback->getRealizationIteratorRequirement();
				CRealizationIndividualInstanceItemReferenceIterator* iterator = dynamicIteratorRealReq->getRealizationIterator();

				cint64 realReqInstCount = dynamicIteratorRealReq->getRealizationRequiredInstancesCount();

				QList<CRealizationIndividualInstanceItemReferenceIterator*> initializationCheckIteratorList;

				COptimizedRepresentativeKPSetIntersectionCombinationIterator* combinationIterator = dynamic_cast<COptimizedRepresentativeKPSetIntersectionCombinationIterator*>(iterator);
				if (combinationIterator) {
					initializationCheckIteratorList.append(*combinationIterator->getCombinedIteratorList());
				} else {
					initializationCheckIteratorList.append(iterator);
				}

				bool processing = false;
				iterator->begin();
				CRealizationIndividualInstanceItemReference currentPosIndiItemRef = iterator->currentIndividualInstanceItemReference();
				if (iterator->requiresInitialization() && currentPosIndiItemRef.getIndividualID() == -1) {
					for (CRealizationIndividualInstanceItemReferenceIterator* initCheckIterator : initializationCheckIteratorList) {
						COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator* individualRoleFillerIterator = dynamic_cast<COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator*>(initCheckIterator);
						if (individualRoleFillerIterator && individualRoleFillerIterator->requiresInitialization()) {
							// TODO: only do initialization
							if (queueRoleFillerInstanceRealization(newReqProcDat, individualRoleFillerIterator->getQueryingIndividualInstanceItemReference(), individualRoleFillerIterator->getQueryingRoleInstancesItem()->getRole(), individualRoleFillerIterator->getQueryingRoleInstancesItemInversion())) {
								iteratorReqDataCallback->incRoleInitializationCount();
								processing = true;
							}
						}
						COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator* unsortedIndividualRoleFillerIterator = dynamic_cast<COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator*>(initCheckIterator);
						if (unsortedIndividualRoleFillerIterator && unsortedIndividualRoleFillerIterator->requiresInitialization()) {
							// TODO: only do initialization
							if (queueRoleFillerInstanceRealization(newReqProcDat, unsortedIndividualRoleFillerIterator->getQueryingIndividualInstanceItemReference(), unsortedIndividualRoleFillerIterator->getQueryingRoleInstancesItem()->getRole(), unsortedIndividualRoleFillerIterator->getQueryingRoleInstancesItemInversion())) {
								iteratorReqDataCallback->incRoleInitializationCount();
								processing = true;
							}
						}
					}
				}

				if (!processing) {
					//CRealizationIndividualInstanceItemReference prevCheckingPosIndiItemRef = iterator->currentIndividualInstanceItemReference();
					//CRealizationIndividualInstanceItemReferenceIterator* iterator2 = iterator->getCopy();
					if (!iterator->atEnd() && !processing && (realReqInstCount < 0 || iteratorReqDataCallback->getFoundInstancesCount() < realReqInstCount)) {
						if (iterator->requiresInitialization()) {

							cint64 schedulingIndividualCount = 0;
							processing = ensureIteratorPositionRealization(iteratorReqDataCallback, newReqProcDat, iterator, initializationCheckIteratorList);
							if (processing) {
								++schedulingIndividualCount;
							}

							if (processing && (realReqInstCount < 0 || realReqInstCount > 1 && iteratorReqDataCallback->getFoundInstancesCount() + schedulingIndividualCount < realReqInstCount)) {
								CRealizationIndividualInstanceItemReferenceIterator* iteratorCopy = iterator->getCopy();
								//CRealizationIndividualInstanceItemReferenceIterator* iteratorCopy2 = iterator->getCopy();
								iteratorCopy->moveNext();
								QList<CRealizationIndividualInstanceItemReferenceIterator*> initializationCheckIteratorCopyList;
								COptimizedRepresentativeKPSetIntersectionCombinationIterator* combinationIteratorCopy = dynamic_cast<COptimizedRepresentativeKPSetIntersectionCombinationIterator*>(iteratorCopy);
								if (combinationIteratorCopy) {
									initializationCheckIteratorCopyList.append(*combinationIteratorCopy->getCombinedIteratorList());
								} else {
									initializationCheckIteratorCopyList.append(iteratorCopy);
								}
								while (!iteratorCopy->atEnd() && (realReqInstCount < 0 || realReqInstCount > 1 && iteratorReqDataCallback->getFoundInstancesCount() + schedulingIndividualCount < realReqInstCount)) {
									CRealizationIndividualInstanceItemReference currentCopyPosIndiItemRef = iteratorCopy->currentIndividualInstanceItemReference();
									if (currentCopyPosIndiItemRef.getIndividualID() == -1 || iteratorCopy->requiresInitialization()) {
										if (ensureIteratorPositionRealization(iteratorReqDataCallback, newReqProcDat, iteratorCopy, initializationCheckIteratorCopyList)) {
											++schedulingIndividualCount;
										}
									}
									if (currentCopyPosIndiItemRef.getIndividualID() == -1) {
										//iteratorCopy2->moveNext();
										//CRealizationIndividualInstanceItemReference currentPosIndiItemRef2 = iteratorCopy2->currentIndividualInstanceItemReference();
										break;
									}
									//iteratorCopy2->moveNext();
									//CRealizationIndividualInstanceItemReference currentPosIndiItemRef2 = iteratorCopy2->currentIndividualInstanceItemReference();
									iteratorCopy->moveNext();
								}
								//LOG(NOTICE, "::Konclude::Reasoner::Kernel::Realizer", logTr("Scheduling realization for %1 individuals for preparing realization iterator.").arg(schedulingIndividualCount), this);
								if (realReqInstCount < 0) {
									iteratorReqDataCallback->setRealizedAllInstances(true);
								}
								delete iteratorCopy;
							}

							if (procData->getStatistics()) {
								procData->getStatistics()->incmIteratorScheduledIndividualRealizationCount(schedulingIndividualCount);
							}

						} else {
							CRealizationIndividualInstanceItemReference currentPosIndiItemRef = iterator->currentIndividualInstanceItemReference();
							if (currentPosIndiItemRef.getIndividualID() != -1) {
								iteratorReqDataCallback->incFoundInstancesCount(1);
							}
						}
					}

					// TODO: do realization for the number of individuals for which it is requested, i.e., realReqInstCount
				}
				return processing;

			}



			COntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::logRequirementProcessingStartStatistics(COntologyRealizingDynamicRequirmentProcessingStatistics* procStats) {
				if (procStats) {
					if (procStats->getIteratorPreparationStartingCount() > 0) {
						LOG(NOTICE, "::Konclude::Reasoner::Kernel::Realizer", logTr("Starting preparation of %1 realization iterators.").arg(procStats->getIteratorPreparationStartingCount()), this);
					}
				}
				return this;
			}


			COntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::logRequirementProcessingFinishStatistics(COntologyRealizingDynamicRequirmentProcessingStatistics* procStats) {
				if (procStats) {
					if (procStats->getIteratorPreparationFinsihingCount() > 0) {
						LOG(NOTICE, "::Konclude::Reasoner::Kernel::Realizer", logTr("Scheduled realization for %1 individuals for preparing %2 realization iterator.").arg(procStats->getmIteratorScheduledIndividualRealizationCount()).arg(procStats->getIteratorPreparationFinsihingCount()), this);
						LOG(NOTICE, "::Konclude::Reasoner::Kernel::Realizer", logTr("Finished preparing of %1 realization iterator with %2 role realization initializations as well as %3 role and %4 concept possible instance realizations.")
							.arg(procStats->getIteratorPreparationFinsihingCount()).arg(procStats->getIteratorRoleInstancesInitializationCount()).arg(procStats->getIteratorRoleInstancesRealizationCount()).arg(procStats->getIteratorConceptInstancesRealizationCount()), this);
					}
				}
				return this;
			}



			COntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addProcessingRequirement(COntologyProcessingRequirement* ontoRequirement, COntologyRealizingDynamicRequirmentProcessingData* procData) {
				bool supportRequirement = false;
				COntologyProcessingStepRequirement* stepProcRequirement = dynamic_cast<COntologyProcessingStepRequirement*>(ontoRequirement);
				if (stepProcRequirement) {
					if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSINITREALIZE) {
						supportRequirement = true;
						if (!mInitRealizeProcessingStep->isStepFinished()) {
							procData->setAssociatedRelizationTestingStep(mInitRealizeProcessingStep);
							getInitRealizeProcessingStep()->addRequirementProcessingData(procData);
						}
					} else if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSCONCEPTREALIZE) {
						markIntanceItemForConceptRealization(procData, mOntology->getTBox()->getTopConcept());
						supportRequirement = true;
					} else if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSROLEREALIZE) {
						markIntanceItemForRoleRealization(procData, mOntology->getRBox()->getTopObjectRole());
						supportRequirement = true;
					} else if (stepProcRequirement->getRequiredProcessingStep()->getOntologyProcessingType() == COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE) {
						markInstantiatedItemForSameIndividualsRealization(procData, CIndividualReference());
						supportRequirement = true;
					}
				}

				COntologyProcessingDynamicRealizationRequirement* dynamicRealRequirment = dynamic_cast<COntologyProcessingDynamicRealizationRequirement*>(ontoRequirement);
				if (dynamicRealRequirment) {

					COntologyProcessingInitializedRealizationRequirement* initRequirement = dynamic_cast<COntologyProcessingInitializedRealizationRequirement*>(dynamicRealRequirment);
					if (initRequirement) {
						supportRequirement = true;
						if (!mInitRealizeProcessingStep->isStepFinished()) {
							procData->setAssociatedRelizationTestingStep(mInitRealizeProcessingStep);
							getInitRealizeProcessingStep()->addRequirementProcessingData(procData);
						}
					}


					COntologyRealizingDynamicIteratorRequirmentDataCallback* dynamicIteratorReapRealReq = dynamic_cast<COntologyRealizingDynamicIteratorRequirmentDataCallback*>(dynamicRealRequirment);
					if (dynamicIteratorReapRealReq) {
						procData->decProcessingItemCount(nullptr);
						bool finishIteratorHandling = false;
						if (!dynamicIteratorReapRealReq->hasRealizedAllInstances()) {
							COntologyRealizingDynamicRequirmentCallbackData* reqCallbackData = new COntologyRealizingDynamicRequirmentCallbackData(dynamicIteratorReapRealReq);
							reqCallbackData->incProcessingRequirmentCount();
							COntologyRealizingDynamicRequirmentProcessingData* newReqProcDat = new COntologyRealizingDynamicRequirmentProcessingData(dynamicIteratorReapRealReq, reqCallbackData);


							if (handleIteratorRequirement(dynamicIteratorReapRealReq, newReqProcDat, procData)) {
								procData->incProcessingItemCount(nullptr);
							} else {
								finishIteratorHandling = true;
								delete newReqProcDat;
								delete reqCallbackData;
							}
						} else {
							finishIteratorHandling = true;
						}
						if (finishIteratorHandling) {
							if (procData->getStatistics()) {
								procData->getStatistics()->incIteratorRoleInstancesInitializationCount(dynamicIteratorReapRealReq->getRoleInitializationCount());
								procData->getStatistics()->incIteratorRoleInstancesRealizationCount(dynamicIteratorReapRealReq->getRoleRealizationCount());
								procData->getStatistics()->incIteratorConceptInstancesRealizationCount(dynamicIteratorReapRealReq->getConceptRealizationCount());
								procData->getStatistics()->incIteratorPreparationFinsihingCount();
							}
						}

						supportRequirement = true;
					}



					COntologyProcessingIteratorRealizationRequirement* dynamicIteratorRealReq = dynamic_cast<COntologyProcessingIteratorRealizationRequirement*>(dynamicRealRequirment);
					if (dynamicIteratorRealReq) {

						COntologyRealizingDynamicIteratorRequirmentDataCallback* iteratorReqDataCallback = new COntologyRealizingDynamicIteratorRequirmentDataCallback(procData, this, dynamicIteratorRealReq);
						COntologyRealizingDynamicRequirmentCallbackData* reqCallbackData = new COntologyRealizingDynamicRequirmentCallbackData(iteratorReqDataCallback);
						reqCallbackData->incProcessingRequirmentCount();
						COntologyRealizingDynamicRequirmentProcessingData* newReqProcDat = new COntologyRealizingDynamicRequirmentProcessingData(iteratorReqDataCallback, reqCallbackData);

						if (procData->getStatistics()) {
							procData->getStatistics()->incIteratorPreparationStartingCount();
						}
						if (handleIteratorRequirement(iteratorReqDataCallback, newReqProcDat, procData)) {
							procData->incProcessingItemCount(nullptr);
						} else {
							if (!mIteratorNothingToDoWarning) {
								LOG(WARN, "::Konclude::Reasoner::Kernel::Realizer", logTr("Nothing to do for realization iterator."), this);
								mIteratorNothingToDoWarning = true;
							}
							delete newReqProcDat;
							delete reqCallbackData;
						}

						supportRequirement = true;
					}


					COntologyProcessingSameRealizationRequirement* dynamicSameRealReq = dynamic_cast<COntologyProcessingSameRealizationRequirement*>(dynamicRealRequirment);
					if (dynamicSameRealReq) {
						CIndividualReference indiSource = dynamicSameRealReq->getIndividualSourceReference();
						CIndividualReference indiDest = dynamicSameRealReq->getIndividualDestinationReference();
						if (indiDest.isNonEmpty() && indiSource.isEmpty()) {
							indiSource = indiDest;
							indiDest = CIndividualReference();
						}
						if (indiDest.isEmpty()) {
							markInstantiatedItemForSameIndividualsRealization(procData, indiSource);
						} else {
							queueSameInstanceRealization(procData, indiSource, indiDest);
						}
						supportRequirement = true;
					}
					COntologyProcessingConceptRealizationRequirement* dynamicConRealReq = dynamic_cast<COntologyProcessingConceptRealizationRequirement*>(dynamicRealRequirment);
					if (dynamicConRealReq) {
						CConcept* concept = dynamicConRealReq->getConcept();
						CIndividualReference indi = dynamicConRealReq->getIndividualReference();
						if (indi.isEmpty()) {
							markIntanceItemForConceptRealization(procData, concept);
						} else {
							bool subConceptRealisation = dynamicConRealReq->realizeSubConcepts();
							queueConceptInstanceRealization(procData, indi, concept, subConceptRealisation);
						}
						supportRequirement = true;
					}
					COntologyProcessingRoleRealizationRequirement* dynamicRoleRealReq = dynamic_cast<COntologyProcessingRoleRealizationRequirement*>(dynamicRealRequirment);
					if (dynamicRoleRealReq) {
						CIndividualReference indiSource = dynamicRoleRealReq->getIndividualSourceReference();
						CIndividualReference indiDestination = dynamicRoleRealReq->getIndividualDestinationReference();
						CRole* role = dynamicRoleRealReq->getRole();
						bool inversed = dynamicRoleRealReq->isRoleInversed();
						if (indiSource.isEmpty() && indiDestination.isEmpty()) {
							markIntanceItemForRoleRealization(procData, role);
						} else if (indiSource.isNonEmpty() && indiDestination.isNonEmpty()) {
							if (role) {
								queueRoleInstanceRealization(procData, indiSource, indiDestination, role, inversed);
							} else {
								for (QList<COptimizedKPSetRoleInstancesItem*>::const_iterator it = mRoleInstancesItemContainer.constBegin(), itEnd = mRoleInstancesItemContainer.constEnd(); it != itEnd; ++it) {
									COptimizedKPSetRoleInstancesItem* roleItem(*it);
									CRole* role = roleItem->getRole();
									queueRoleInstanceRealization(procData, indiSource, indiDestination, role, inversed);
								}
							}
						} else if (indiSource.isNonEmpty() || indiDestination.isNonEmpty()) {
							if (indiSource.isEmpty()) {
								indiSource = indiDestination;
								inversed = !inversed;
								indiDestination = CIndividualReference();
							}
							if (role) {
								queueRoleFillerInstanceRealization(procData, indiSource, role, inversed);
							} else {
								for (QList<COptimizedKPSetRoleInstancesItem*>::const_iterator it = mRoleInstancesItemContainer.constBegin(), itEnd = mRoleInstancesItemContainer.constEnd(); it != itEnd; ++it) {
									COptimizedKPSetRoleInstancesItem* roleItem(*it);
									CRole* role = roleItem->getRole();
									queueRoleFillerInstanceRealization(procData, indiSource, role, inversed);
								}
							}
						}
						supportRequirement = true;
					}
				}
				if (procData && !procData->hasCurrentProcessingItemCount()) {
					ontoRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSSUCESSFULL);
					if (procData->getCallbackData()) {
						COntologyRealizingDynamicRequirmentCallbackData* callbackData = procData->getCallbackData();
						callbackData->decProcessingRequirmentCount();
						if (!callbackData->hasCurrentProcessingRequirmentCount()) {
							logRequirementProcessingFinishStatistics(procData->getStatistics());
							callbackData->getProcessingFinishedCallback()->doCallback();
							delete callbackData;
						}
					}
					delete procData;
				}
				if (!supportRequirement) {
					ontoRequirement->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSUNSUPPORTED);
				}
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addRoleInstanceItemToProcessPossibleInstances(COptimizedKPSetRoleInstancesItem* instanceItem) {
				if (!instanceItem->hasPossibleInstancesProcessingQueuedFlag()) {
					if (instanceItem->hasPossibleInstances()) {
						mProcessingPossibleRoleInstancesItemList.append(instanceItem);
						instanceItem->setPossibleInstancesProcessingQueuedFlag(true);
					}
				}
				return this;
			}





			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addRoleInstanceItemToProcessComplexCandidateInstances(COptimizedKPSetRoleInstancesItem* instanceItem) {
				if (!instanceItem->hasComplexCandidateInstancesProcessingQueuedFlag()) {
					if (instanceItem->hasComplexInstanceCandidates()) {
						mCandidateConfirmationRoleInstancesItemList.append(instanceItem);
						instanceItem->setComplexCandidateProcessingQueuedFlag(true);
					}
				}
				return this;
			}




			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addConceptInstanceItemToProcessPossibleInstances(COptimizedKPSetConceptInstancesItem* instanceItem) {
				if (!instanceItem->hasPossibleInstancesProcessingQueuedFlag()) {
					if (instanceItem->hasPossibleInstances()) {
						mProcessingConceptInstancesItemList.append(instanceItem);
						instanceItem->setPossibleInstancesProcessingQueuedFlag(true);
					}
				}
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addRoleInstanceItemToInitializeCandidates(COptimizedKPSetRoleInstancesItem* instanceItem) {
				if (!instanceItem->hasInitializingQueuedFlag()) {
					if (instanceItem->requiresCandidateInitialization()) {
						mInitializingRoleInstancesItemList.append(instanceItem);
						instanceItem->setInitializingQueuedFlag(true);
					}
				}
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addRoleInstanceIndividualItemToInitializeCandidates(COptimizedKPSetIndividualItem* indiItem) {
				if (!indiItem->hasInitializingRoleCandidatesQueuedFlag()) {
					addRoleInstanceIndividualItemToInitializeCandidates(CRealizationIndividualInstanceItemReference(indiItem->getIndividualReference(), indiItem));
					indiItem->setInitializingRoleCandidatesQueuedFlag(true);
				}
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addRoleInstanceIndividualItemToInitializeCandidates(const CRealizationIndividualInstanceItemReference& indiRealItemRef) {
				mInitializingRoleInstancesIndividualItemReferenceList.append(indiRealItemRef);
				return this;
			}



			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addRoleSuccessorInstanceIndividualItemPairToInitializeCandidates(const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetRoleInstancesItem* instanceItem) {
				mInitializingRoleSuccessorInstancesIndividualItemReferencePairList.append(QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*>(indiRealItemRef, instanceItem));
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addRolePredecessorInstanceIndividualItemPairToInitializeCandidates(const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetRoleInstancesItem* instanceItem) {
				mInitializingRolePredecessorInstancesIndividualItemReferencePairList.append(QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*>(indiRealItemRef, instanceItem));
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addInstantiatedItemToProcessPossibleSameIndividuals(COptimizedKPSetIndividualItem* instantiatedItem) {
				if (!instantiatedItem->hasPossibleSameIndividualsProcessingQueuedFlag()) {
					if (instantiatedItem->hasPossibleSameIndividuals()) {
						mProcessingSameIndividualsItemList.append(instantiatedItem);
						instantiatedItem->setPossibleSameIndividualsProcessingQueuedFlag(true);
					}
				}
				return this;
			}



			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::markIntanceItemForRoleRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, CRole* role, bool subroleRealizationRequired) {
				if (role == mOntology->getRBox()->getTopObjectRole()) {
					setAllRoleInstancesProcessing(true);
				}
				bool roleItemQueued = false;
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
						if (procData && (item->hasPossibleInstances() || item->hasComplexInstanceCandidates())) {
							procData->incProcessingItemCount(mRealizeRoleProcessingStep);
							item->addRequirementProcessingDataLinker(createRequirementProcessingDataLinker(procData));
							roleItemQueued = true;
						}
						addRoleInstanceItemToInitializeCandidates(item);
						addRoleInstanceItemToProcessPossibleInstances(item);
						addRoleInstanceItemToProcessComplexCandidateInstances(item);
					}
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

				if (roleItemQueued || isAllRoleInstancesProcessing() && !getRealizeRoleProcessingStep()->isStepFinished()) {
					if (!roleItemQueued || isAllRoleInstancesProcessing()) {
						procData->incProcessingItemCount(mRealizeRoleProcessingStep);
					}
					procData->setAssociatedRelizationTestingStep(mRealizeRoleProcessingStep);
					getRealizeRoleProcessingStep()->addRequirementProcessingData(procData);
				}

				return this;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::queueConceptInstanceRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference& indi, CConcept* concept, bool subconceptRealizationRequired) {
				if (concept == nullptr) {
					concept = mOntology->getTBox()->getTopConcept();
					subconceptRealizationRequired = true;
				}
				COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)getInstanceItem(indi, true);

				bool conceptItemQueued = false;
				if (indiItem) {
					COptimizedKPSetConceptInstancesItem* initConceptItem = mConceptInstancesItemHash.value(concept);

					QList<COptimizedKPSetConceptInstancesItem*> itemList;
					QSet<COptimizedKPSetConceptInstancesItem*> itemSet;
					itemList.append(initConceptItem);
					itemSet.insert(initConceptItem);

					while (!itemList.isEmpty()) {
						COptimizedKPSetConceptInstancesItem* conceptItem = itemList.takeFirst();
						COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = indiItem->getKnownPossibleInstancesHash();
						COptimizedKPSetConceptInstancesHashData hashData = knownPossibleInstancesHash->value(conceptItem);
						if (hashData.mInstanceItemData) {
							if (!hashData.mInstanceItemData->mKnownInstance && hashData.mInstanceItemData->mPossibleInstance && !hashData.mInstanceItemData->mTestedInstance) {
								if (!subconceptRealizationRequired && !hashData.mInstanceItemData->mMostSpecific && !conceptItem->getPossibleInstancesMap()->contains(indi.getIndividualID())) {
									conceptItem->getPossibleInstancesMap()->insert(indi.getIndividualID(), indiItem);
								}
								CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem* incConInstTestItem = new CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem(indiItem, conceptItem, procData);
								procData->incProcessingItemCount(mRealizeConceptProcessingStep);
								addEntailmentIndividualConceptInstanceTestingItem(incConInstTestItem);
								conceptItemQueued = true;
							}
						}
						if (subconceptRealizationRequired) {
							QList<COptimizedKPSetConceptInstancesItem*>* succItemList = conceptItem->getSuccessorItemList();
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

				if (conceptItemQueued) {
					procData->setAssociatedRelizationTestingStep(mRealizeConceptProcessingStep);
					getRealizeConceptProcessingStep()->addRequirementProcessingData(procData);
					return true;
				} else {
					return false;
				}

			}




			bool COptimizedRepresentativeKPSetOntologyRealizingItem::queueSameInstanceRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference& indi1, const CIndividualReference& indi2) {
				COptimizedKPSetIndividualItem* indiItem1 = (COptimizedKPSetIndividualItem*)getInstanceItem(indi1, true);
				COptimizedKPSetIndividualItem* indiItem2 = (COptimizedKPSetIndividualItem*)getInstanceItem(indi2, true);

				bool sameItemQueued = false;

				if (indiItem1 && indiItem2) {
					QSet<COptimizedKPSetIndividualItem*>* possSameIndiItemSet = indiItem1->getPossibleSameInstantiatedItemSet();
					if (possSameIndiItemSet) {
						if (possSameIndiItemSet->contains(indiItem2)) {
							CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem* incSameInstTestItem = new CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem(indiItem1, indiItem2, procData);
							procData->incProcessingItemCount(mRealizeSameIndividualsProcessingStep);
							addEntailmentIndividualsSameInstanceTestingItem(incSameInstTestItem);
						}
					}

					if (sameItemQueued) {
						procData->setAssociatedRelizationTestingStep(mRealizeSameIndividualsProcessingStep);
						getRealizeSameIndividualsProcessingStep()->addRequirementProcessingData(procData);
						return true;
					} else {
						return false;
					}
				}
				return false;
			}








			bool COptimizedRepresentativeKPSetOntologyRealizingItem::queueRoleFillerInstanceRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference& indiSource, CRole* role, bool roleInversed, bool subRoleRealization) {
				if (role == nullptr) {
					role = mOntology->getRBox()->getTopObjectRole();
					subRoleRealization = true;
				}
				bool roleItemQueued = false;

				COptimizedKPSetIndividualItem* indiSourceItem = getInstanceItem(indiSource, true);
				CRealizationIndividualInstanceItemReference indiSourceItemRef = CRealizationIndividualInstanceItemReference(indiSource, indiSourceItem);

				COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = mRedirectedRoleInstancesItemHash.value(role);
				if (redRoleItem) {
					COptimizedKPSetRoleInstancesItem* initRoleItem = redRoleItem->getRedirectedItem();
					bool inversed = roleInversed^redRoleItem->isInversed();

					QList<TRoleItemInversionPair> itemList;
					QSet<TRoleItemInversionPair> itemSet;
					itemList.append(TRoleItemInversionPair(initRoleItem, inversed));
					itemSet.insert(TRoleItemInversionPair(initRoleItem, inversed));


					while (!itemList.isEmpty()) {
						TRoleItemInversionPair roleItemInvPair = itemList.takeFirst();
						COptimizedKPSetRoleInstancesItem* roleItem = roleItemInvPair.first;
						bool inversed = roleItemInvPair.second;

						if (indiSourceItem) {
							//if (!indiSourceItem->hasAllRoleCandidatesInitializedFlag()) {
							//} else {
								COptimizedKPSetRoleNeighbourInstancesHash* knownPossibleInstancesHash = indiSourceItem->getKnownPossibleRoleNeighboursInstancesHash();
								if (knownPossibleInstancesHash) {
									COptimizedKPSetRoleNeighbourInstancesHashData possNeighbourInstanceData = knownPossibleInstancesHash->value(roleItem);
									COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = possNeighbourInstanceData.getRoleNeighbourInstancesHash(inversed, false);
									if (possNeighbourInstanceHash) {
										for (COptimizedKPSetRoleInstancesHash::const_iterator itNeighInst = possNeighbourInstanceHash->constBegin(), itNeighInstEnd = possNeighbourInstanceHash->constEnd(); itNeighInst != itNeighInstEnd; ++itNeighInst) {
											const COptimizedKPSetRoleInstancesHashData& destIndiInstanceDat = itNeighInst.value();
											COptimizedKPSetRoleInstancesData* instanceData = destIndiInstanceDat.mInstanceItemData;
											if (instanceData && !instanceData->mKnownInstance && instanceData->mPossibleInstance && !instanceData->mTestedInstance) {
												COptimizedKPSetIndividualItem* destIndiItem = destIndiInstanceDat.mNeighbourIndividualItem;
												CRealizationIndividualInstanceItemReference indiDestItemRef = CRealizationIndividualInstanceItemReference(destIndiItem->getIndividualReference(), destIndiItem);
												CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem* incRoleInstTestItem = new CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem(indiSourceItemRef, indiDestItemRef, roleItem, inversed, procData);
												procData->incProcessingItemCount(mRealizeRoleProcessingStep);
												addEntailmentIndividualsRoleInstanceTestingItem(incRoleInstTestItem);
												roleItemQueued = true;
											}
										}
									}
								}
							//}
						}
						if (roleItem->hasComplexRoleData()) {
							QHash<cint64, COptimizedKPSetIndividualComplexRoleData*>* indiComplexRoleDataHash = roleItem->getIndividualIdComplexRoleDataHash();
							if (indiComplexRoleDataHash->contains(indiSource.getIndividualID())) {
								COptimizedKPSetIndividualComplexRoleData* indiComplexData = indiComplexRoleDataHash->value(indiSource.getIndividualID());
								if (!indiComplexData || !indiComplexData->isInitialized(inversed)) {
									// TODO: check inverse


									CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mBackendAssocCacheReader->getIndividualAssociationData(indiSource.getIndividualID());
									CBackendRepresentativeMemoryLabelCacheItem* combinedNeigRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);

									bool initializationRequired = false;
									if (roleItem->getComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet(inversed)->contains(combinedNeigRoleSetLabel)) {
										initializationRequired = true;
									}
									CBackendRepresentativeMemoryLabelCacheItem* combinedExistentailDeterministicRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
									if (combinedExistentailDeterministicRoleSetLabel && roleItem->getComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet(inversed)->contains(combinedExistentailDeterministicRoleSetLabel)) {
										initializationRequired = true;
									}
									CBackendRepresentativeMemoryLabelCacheItem* combinedExistentailNonDeterministicRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
									if (combinedExistentailNonDeterministicRoleSetLabel && roleItem->getComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet(inversed)->contains(combinedExistentailNonDeterministicRoleSetLabel)) {
										initializationRequired = true;
									}

									if (initializationRequired) {
										if (inversed) {
											addRolePredecessorInstanceIndividualItemPairToInitializeCandidates(indiSourceItemRef, roleItem);
										} else {
											addRoleSuccessorInstanceIndividualItemPairToInitializeCandidates(indiSourceItemRef, roleItem);
										}
										CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem* incRoleInstTestItem = new CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem(indiSourceItemRef, CRealizationIndividualInstanceItemReference(), roleItem, inversed, procData);
										procData->incProcessingItemCount(mRealizeRoleProcessingStep);
										addEntailmentIndividualsRoleInstanceTestingItem(incRoleInstTestItem);
										roleItemQueued = true;
									} else {
										// nothing to do, individual has no link with a starter role (it may have some links for the queried role, but they are already handled with other (iterator) data structures
										if (!indiComplexData) {
											indiComplexData = roleItem->getIndividualIdComplexRoleData(indiSource.getIndividualID(), true);
										}
										indiComplexData->setInitializing(inversed, true);
										indiComplexData->setInitialized(inversed, true);
									}

								} else {
									COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;

									COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(inversed, false);
									if (possNeighbourInstanceHash) {
										for (COptimizedKPSetRoleInstancesHash::const_iterator itNeighInst = possNeighbourInstanceHash->constBegin(), itNeighInstEnd = possNeighbourInstanceHash->constEnd(); itNeighInst != itNeighInstEnd; ++itNeighInst) {
											const COptimizedKPSetRoleInstancesHashData& destIndiInstanceDat = itNeighInst.value();
											COptimizedKPSetRoleInstancesData* instanceData = destIndiInstanceDat.mInstanceItemData;
											if (instanceData && !instanceData->mKnownInstance && instanceData->mPossibleInstance && !instanceData->mTestedInstance) {
												CRealizationIndividualInstanceItemReference indiDestItemRef = getInstanceItemReference(itNeighInst.key(), false);
												CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem* incRoleInstTestItem = new CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem(indiSourceItemRef, indiDestItemRef, roleItem, inversed, procData);
												procData->incProcessingItemCount(mRealizeRoleProcessingStep);
												addEntailmentIndividualsRoleInstanceTestingItem(incRoleInstTestItem);
												roleItemQueued = true;
											}
										}
									}

								}
							}
						}

						if (subRoleRealization) {
							QList<TRoleItemInversionPair>* succItemList = roleItem->getSuccessorItemList();
							for (QList<TRoleItemInversionPair>::const_iterator it = succItemList->constBegin(), itEnd = succItemList->constEnd(); it != itEnd; ++it) {
								TRoleItemInversionPair succItemPair(*it);
								TRoleItemInversionPair succInvItemPair(succItemPair.first, succItemPair.second^inversed);
								if (!itemSet.contains(succInvItemPair)) {
									itemSet.insert(succInvItemPair);
									itemList.append(succInvItemPair);
								}
							}
						}

					}
				}

				if (roleItemQueued) {
					procData->setAssociatedRelizationTestingStep(mRealizeRoleProcessingStep);
					getRealizeRoleProcessingStep()->addRequirementProcessingData(procData);
					return true;
				} else {
					return false;
				}

			}





			bool COptimizedRepresentativeKPSetOntologyRealizingItem::queueRoleInstanceRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference& indiSource, const CIndividualReference& indiDestination, CRole* role, bool roleInversed, bool subRoleRealization) {
				if (role == nullptr) {
					role = mOntology->getRBox()->getTopObjectRole();
					subRoleRealization = true;
				}

				COptimizedKPSetIndividualItem* indiSourceItem = getInstanceItem(indiSource, true);
				COptimizedKPSetIndividualItem* indiDestItem = getInstanceItem(indiDestination, true);

				CRealizationIndividualInstanceItemReference indiSourceItemRef = CRealizationIndividualInstanceItemReference(indiSource, indiSourceItem);
				CRealizationIndividualInstanceItemReference indiDestItemRef = CRealizationIndividualInstanceItemReference(indiDestination, indiDestItem);

				bool roleItemQueued = false;

				COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = mRedirectedRoleInstancesItemHash.value(role);
				COptimizedKPSetRoleInstancesItem* initRoleItem = redRoleItem->getRedirectedItem();
				bool inversed = roleInversed^redRoleItem->isInversed();


				QList<TRoleItemInversionPair> itemList;
				QSet<TRoleItemInversionPair> itemSet;
				itemList.append(TRoleItemInversionPair(initRoleItem, inversed));
				itemSet.insert(TRoleItemInversionPair(initRoleItem, inversed));


				while (!itemList.isEmpty()) {
					TRoleItemInversionPair roleItemInvPair = itemList.takeFirst();
					COptimizedKPSetRoleInstancesItem* roleItem = roleItemInvPair.first;
					bool inversed = roleItemInvPair.second;
					bool isInstance = false;
					bool isPossibleInstance = true;
					bool isInitialized = false;
					bool hasDirectNeighbourData = false;

					if (indiSourceItem && indiDestItem) {
						//if (indiSourceItem->hasAllRoleCandidatesInitializedFlag()) {
						//	isPossibleInstance = false;
						//	isInitialized = true;
						//}
						COptimizedKPSetRoleNeighbourInstancesHash* knownPossibleInstancesHash = indiSourceItem->getKnownPossibleRoleNeighboursInstancesHash();
						if (knownPossibleInstancesHash) {
							COptimizedKPSetRoleNeighbourInstancesHashData possNeighbourInstanceData = knownPossibleInstancesHash->value(roleItem);
							COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = possNeighbourInstanceData.getRoleNeighbourInstancesHash(inversed, false);
							if (possNeighbourInstanceHash) {
								COptimizedKPSetRoleInstancesData* instanceData = possNeighbourInstanceHash->value(indiDestItem->getIndividualId()).mInstanceItemData;
								if (instanceData) {
									isInstance = instanceData->mKnownInstance;
									isPossibleInstance = isInstance || instanceData->mPossibleInstance && !instanceData->mTestedInstance;
									hasDirectNeighbourData = true;
								}
							}
						}
					}
					if (!hasDirectNeighbourData && roleItem->hasComplexRoleData()) {
						QHash<cint64, COptimizedKPSetIndividualComplexRoleData*>* indiComplexRoleDataHash = roleItem->getIndividualIdComplexRoleDataHash();
						if (indiComplexRoleDataHash->contains(indiSource.getIndividualID())) {
							COptimizedKPSetIndividualComplexRoleData* indiComplexData = indiComplexRoleDataHash->value(indiSource.getIndividualID());
							if (indiComplexData && indiComplexData->isInitialized(inversed)) {

								COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;
								COptimizedKPSetRoleInstancesData* instanceData = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstanceItemData(inversed, indiDestination.getIndividualID(), false);
								if (instanceData) {
									isInstance = instanceData->mKnownInstance;
									isPossibleInstance = isInstance || instanceData->mPossibleInstance && !instanceData->mTestedInstance;
								}

							} else if (!indiComplexData || !indiComplexData->isInitializing(inversed)) {
								// TODO: do initialization only for the given role
								addRoleInstanceIndividualItemToInitializeCandidates(indiSourceItemRef);
							}
						}
					}

					if (!isInstance && isPossibleInstance) {
						CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem* incRoleInstTestItem = new CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem(indiSourceItemRef, indiDestItemRef, roleItem, inversed, procData);
						procData->incProcessingItemCount(mRealizeRoleProcessingStep);
						addEntailmentIndividualsRoleInstanceTestingItem(incRoleInstTestItem);
						roleItemQueued = true;
					}



					if (subRoleRealization) {
						QList<TRoleItemInversionPair>* succItemList = roleItem->getSuccessorItemList();
						for (QList<TRoleItemInversionPair>::const_iterator it = succItemList->constBegin(), itEnd = succItemList->constEnd(); it != itEnd; ++it) {
							TRoleItemInversionPair succItemPair(*it);
							TRoleItemInversionPair succInvItemPair(succItemPair.first, succItemPair.second^inversed);
							if (!itemSet.contains(succInvItemPair)) {
								itemSet.insert(succInvItemPair);
								itemList.append(succInvItemPair);
							}
						}
					}

				}

				if (roleItemQueued) {
					procData->setAssociatedRelizationTestingStep(mRealizeRoleProcessingStep);
					getRealizeRoleProcessingStep()->addRequirementProcessingData(procData);
					return true;
				} else {
					return false;
				}

			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::markIntanceItemForConceptRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, CConcept* concept, bool subconceptRealizationRequired) {
				if (concept == mOntology->getTBox()->getTopConcept()) {
					setAllConceptInstancesProcessing(true);
				}
				COptimizedKPSetConceptInstancesItem* initialItem = mConceptInstancesItemHash.value(concept);
				QList<COptimizedKPSetConceptInstancesItem*> itemList;
				QSet<COptimizedKPSetConceptInstancesItem*> itemSet;
				itemList.append(initialItem);
				itemSet.insert(initialItem);
				bool conceptItemQueued = false;
				while (!itemList.isEmpty()) {
					COptimizedKPSetConceptInstancesItem* item = itemList.takeFirst();
					if (!item->hasToProcessPossibleInstancesFlag()) {
						item->setToProcessPossibleInstancesFlag(true);
						if (procData && item->hasPossibleInstances()) {
							procData->incProcessingItemCount(mRealizeConceptProcessingStep);
							item->addRequirementProcessingDataLinker(createRequirementProcessingDataLinker(procData));
							conceptItemQueued = true;
						}
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


				if (conceptItemQueued || isAllConceptInstancesProcessing() && !getRealizeConceptProcessingStep()->isStepFinished()) {
					if (!conceptItemQueued || isAllConceptInstancesProcessing()) {
						procData->incProcessingItemCount(mRealizeConceptProcessingStep);
					}
					procData->setAssociatedRelizationTestingStep(mRealizeConceptProcessingStep);
					getRealizeConceptProcessingStep()->addRequirementProcessingData(procData);
				}

				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::markInstantiatedItemForSameIndividualsRealization(COntologyRealizingDynamicRequirmentProcessingData* procData, const CIndividualReference& indi) {
				bool itemQueued = false;
				if (indi.isNonEmpty()) {
					COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)getInstanceItem(indi, true);
					if (indiItem) {
						if (!indiItem->hasToProcessPossibleSameIndividualsFlag()) {
							indiItem->setToProcessPossibleSameIndividualsFlag(true);
							if (indiItem->hasPossibleSameIndividuals()) {
								procData->incProcessingItemCount(mRealizeSameIndividualsProcessingStep);
								indiItem->addRequirementProcessingDataLinker(createRequirementProcessingDataLinker(procData));
								itemQueued = true;
							}
							addInstantiatedItemToProcessPossibleSameIndividuals(indiItem);
						}
					}
				} else {
					setAllSameIndividualsProcessing(true);
					for (QHash<cint64, COptimizedKPSetIndividualItem*>::const_iterator it = mIndividualInstantiatedItemHash.constBegin(), itEnd = mIndividualInstantiatedItemHash.constEnd(); it != itEnd; ++it) {
						cint64 individualID = it.key();
						COptimizedKPSetIndividualItem* indiItem = it.value();
						if (!indiItem->hasToProcessPossibleSameIndividualsFlag()) {
							indiItem->setToProcessPossibleSameIndividualsFlag(true);
							if (procData && indiItem->hasPossibleSameIndividuals()) {
								procData->incProcessingItemCount(mRealizeSameIndividualsProcessingStep);
								indiItem->addRequirementProcessingDataLinker(createRequirementProcessingDataLinker(procData));
								itemQueued = true;
							}
							addInstantiatedItemToProcessPossibleSameIndividuals(indiItem);
						}
					}
				}

				if (itemQueued || isAllSameIndividualsProcessing() && !getRealizeSameIndividualsProcessingStep()->isStepFinished()) {
					if (!itemQueued || isAllSameIndividualsProcessing()) {
						procData->incProcessingItemCount(mRealizeSameIndividualsProcessingStep);
					}
					procData->setAssociatedRelizationTestingStep(mRealizeSameIndividualsProcessingStep);
					getRealizeSameIndividualsProcessingStep()->addRequirementProcessingData(procData);
				}

				return this;
			}

			CCalculationConfigurationExtension* COptimizedRepresentativeKPSetOntologyRealizingItem::getCalculationConfiguration() {
				return mCalculationConfig;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setRealization(CRealization* realization) {
				mRealization = realization;
				return this;
			}

			CRealization* COptimizedRepresentativeKPSetOntologyRealizingItem::getRealization() {
				return mRealization;
			}



			CRealizingTestingStep* COptimizedRepresentativeKPSetOntologyRealizingItem::getRealizeConceptProcessingStep() {
				return mRealizeConceptProcessingStep;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isRealizeConceptStepFinished() {
				return mRealizeConceptProcessingStep->isStepFinished();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isRealizeConceptStepRequired() {
				return mRealizeConceptProcessingStep->hasRequirements();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::areRealizeConceptStepProcessingRequirementSatisfied() {
				return mRealizeConceptProcessingStep->areStepProcessingRequirementSatisfied();
			}


			CRealizingTestingStep* COptimizedRepresentativeKPSetOntologyRealizingItem::getInitRealizeProcessingStep() {
				return mInitRealizeProcessingStep;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isInitRealizeStepFinished() {
				return mInitRealizeProcessingStep->isStepFinished();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isInitRealizeStepRequired() {
				return mInitRealizeProcessingStep->hasRequirements();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::areInitRealizeStepProcessingRequirementSatisfied() {
				return mInitRealizeProcessingStep->areStepProcessingRequirementSatisfied();
			}



			CRealizingTestingStep* COptimizedRepresentativeKPSetOntologyRealizingItem::getRealizeRoleProcessingStep() {
				return mRealizeRoleProcessingStep;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isRealizeRoleStepFinished() {
				return mRealizeRoleProcessingStep->isStepFinished();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isRealizeRoleStepRequired() {
				return mRealizeRoleProcessingStep->hasRequirements();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::areRealizeRoleStepProcessingRequirementSatisfied() {
				return mRealizeRoleProcessingStep->areStepProcessingRequirementSatisfied();
			}





			CRealizingTestingStep* COptimizedRepresentativeKPSetOntologyRealizingItem::getRealizeSameIndividualsProcessingStep() {
				return mRealizeSameIndividualsProcessingStep;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isRealizeSameIndividualsStepFinished() {
				return mRealizeSameIndividualsProcessingStep->isStepFinished();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isRealizeSameIndividualsStepRequired() {
				return mRealizeSameIndividualsProcessingStep->hasRequirements();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::areRealizeSameIndividualsStepProcessingRequirementSatisfied() {
				return mRealizeSameIndividualsProcessingStep->areStepProcessingRequirementSatisfied();
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingItem::areAllStepFinished() {
				for (QList<CRealizingTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if (!(*it)->isStepFinished()) {
						return false;
					}
				}
				return true;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingProcessingRequirements() {
				for (QList<CRealizingTestingStep*>::const_iterator it = mProcessingSteps.constBegin(), itEnd = mProcessingSteps.constEnd(); it != itEnd; ++it) {
					if ((*it)->hasRequirements()) {
						return true;
					}
				}
				return false;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingCandidateConfirmationRoleInstanceItems() {
				return !mCandidateConfirmationRoleInstancesItemList.isEmpty();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingInitializingRoleInstanceItems() {
				return !mInitializingRoleInstancesItemList.isEmpty();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingInitializingRoleInstanceIndividualItemReferences() {
				return !mInitializingRoleInstancesIndividualItemReferenceList.isEmpty();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingInitializingRoleSuccessorInstanceIndividualItemReferencePairs() {
				return !mInitializingRoleSuccessorInstancesIndividualItemReferencePairList.isEmpty();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingInitializingRolePredeccessorInstanceIndividualItemReferencePairs() {
				return !mInitializingRolePredecessorInstancesIndividualItemReferencePairList.isEmpty();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingProcessingRoleInstanceItems() {
				return !mProcessingPossibleRoleInstancesItemList.isEmpty();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingProcessingConceptInstanceItems() {
				return !mProcessingConceptInstancesItemList.isEmpty();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingProcessingConceptInstanceIndividualItems() {
				return !mProcessingPossibleConceptInstanceItemList.isEmpty();
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasRemainingProcessingSameIndividualsItems() {
				return !mProcessingSameIndividualsItemList.isEmpty();
			}

			QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getHierarchyNodeInstancesItemHash() {
				return &mHierNodeInstancesItemHash;
			}

			QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getConceptInstancesItemHash() {
				return &mConceptInstancesItemHash;
			}


			QHash<CRole*,COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleInstancesItemHash() {
				return &mRoleInstancesItemHash;
			}


			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleInstancesItemList() {
				return &mRoleInstancesItemContainer;
			}


			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getComplexRoleInstancesItemList() {
				return &mComplexRoleInstancesItemContainer;
			}


			QHash<cint64, COptimizedKPSetIndividualItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getIndividualInstantiatedItemHash() {
				return &mIndividualInstantiatedItemHash;
			}



			COptimizedKPSetIndividualItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getIndividualInstantiatedItem(CIndividual* individual, bool directCreate) {
				return getIndividualInstantiatedItem(individual->getIndividualID(), directCreate, individual);
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasIndividualInstantiatedItem(CIndividual* individual) {
				return hasIndividualInstantiatedItem(individual->getIndividualID());
			}





			COptimizedKPSetIndividualItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getIndividualInstantiatedItem(cint64 indiId, bool directCreate, CIndividual* individual) {
				COptimizedKPSetIndividualItem* instantiatedItem = nullptr;
				if (directCreate) {
					COptimizedKPSetIndividualItem*& instantiatedItemPointer = mIndividualInstantiatedItemHash[indiId];
					if (!instantiatedItemPointer) {
						instantiatedItemPointer = new COptimizedKPSetIndividualItem();
						instantiatedItemPointer->initInstantiatedItem(indiId, individual);
						mInstantiatedItemContainer.append(instantiatedItemPointer);
						mIndividualInstantiatedItemHash.insert(indiId, instantiatedItemPointer);
					}
					instantiatedItem = instantiatedItemPointer;
				} else {
					instantiatedItem = mIndividualInstantiatedItemHash.value(indiId);
				}
				return instantiatedItem;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasIndividualInstantiatedItem(cint64 indiId) {
				return mIndividualInstantiatedItemHash.contains(indiId);
			}


			QList<COptimizedKPSetConceptInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getProcessingPossibleConceptInstancesItemList() {
				return &mProcessingConceptInstancesItemList;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addPossibleConceptInstancesIndividualItem(COptimizedKPSetIndividualItem* indiItem) {
				mProcessingPossibleConceptInstanceItemList.append(indiItem);
				return this;
			}

			QList<COptimizedKPSetIndividualItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getPossibleConceptInstancesIndividualItemList() {
				return &mProcessingPossibleConceptInstanceItemList;
			}



			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getProcessingPossibleRoleInstancesItemList() {
				return &mProcessingPossibleRoleInstancesItemList;
			}

			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getInitializingRoleInstancesItemList() {
				return &mInitializingRoleInstancesItemList;
			}


			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getCandidateConfirmationRoleInstancesItemList() {
				return &mCandidateConfirmationRoleInstancesItemList;
			}

			QList<CRealizationIndividualInstanceItemReference>* COptimizedRepresentativeKPSetOntologyRealizingItem::getInitializingRoleInstancesIndividualItemReferenceList() {
				return &mInitializingRoleInstancesIndividualItemReferenceList;
			}



			QList<QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*>>* COptimizedRepresentativeKPSetOntologyRealizingItem::getInitializingRoleSuccessorInstancesIndividualItemReferencePairList() {
				return &mInitializingRoleSuccessorInstancesIndividualItemReferencePairList;
			}


			QList<QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*>>* COptimizedRepresentativeKPSetOntologyRealizingItem::getInitializingRolePredecessorInstancesIndividualItemReferencePairList() {
				return &mInitializingRolePredecessorInstancesIndividualItemReferencePairList;
			}


			QList<COptimizedKPSetIndividualItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getProcessingPossibleSameIndividualsItemList() {
				return &mProcessingSameIndividualsItemList;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getTestedPossibleConceptInstancesCount() {
				return mTestedPossibleConceptInstancesCount;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getOpenPossibleConceptInstancesCount() {
				return mOpenPossibleConceptInstancesCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incTestedPossibleConceptInstancesCount(cint64 incCount) {
				mTestedPossibleConceptInstancesCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incOpenPossibleConceptInstancesCount(cint64 incCount) {
				mOpenPossibleConceptInstancesCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decTestedPossibleConceptInstancesCount(cint64 decCount) {
				mTestedPossibleConceptInstancesCount -= decCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decOpenPossibleConceptInstancesCount(cint64 decCount) {
				mOpenPossibleConceptInstancesCount -= decCount;
				return this;
			}








			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getTestedPossibleSameIndividualsCount() {
				return mTestedPossibleSameIndividualsCount;
			}


			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getOpenPossibleSameIndividualsCount() {
				return mOpenPossibleSameIndividualsCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incTestedPossibleSameIndividualsCount(cint64 incCount) {
				mTestedPossibleSameIndividualsCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incOpenPossibleSameIndividualsCount(cint64 incCount) {
				mOpenPossibleSameIndividualsCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decTestedPossibleSameIndividualsCount(cint64 decCount) {
				mTestedPossibleSameIndividualsCount -= decCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decOpenPossibleSameIndividualsCount(cint64 decCount) {
				mOpenPossibleSameIndividualsCount -= decCount;
				return this;
			}





			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incProvidedPossibleInstancesMergingIndividualsCount(cint64 incCount) {
				mProvidedPossibleInstancesMergingIndividualsCount += incCount;
				return this;
			}
			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getProvidedPossibleInstancesMergingIndividualsCount() {
				return mProvidedPossibleInstancesMergingIndividualsCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incProvidedPossibleInstancesMergingsCount(cint64 incCount) {
				mProvidedPossibleInstancesMergingsCount += incCount;
				return this;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getProvidedPossibleInstancesMergingsCount() {
				return mProvidedPossibleInstancesMergingsCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incAttemptedPossibleInstancesMergingsCount(cint64 incCount) {
				mAttemptedPossibleInstancesMergingsCount += incCount;
				return this;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getAttemptedPossibleInstancesMergingsCount() {
				return mAttemptedPossibleInstancesMergingsCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incSucceddedPossibleInstancesMergingsCount(cint64 incCount) {
				mSucceddedPossibleInstancesMergingsCount += incCount;
				return this;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getSucceddedPossibleInstancesMergingsCount() {
				return mSucceddedPossibleInstancesMergingsCount;
			}














			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incModelMergingsTriedCount(cint64 incCount) {
				mModelMergingsTriedCount += incCount;
				return this;
			}
			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getModelMergingsTriedCount() {
				return mModelMergingsTriedCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incModelMergingsSucessCount(cint64 incCount) {
				mModelMergingsSucessCount += incCount;
				return this;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getModelMergingsSucessCount() {
				return mModelMergingsSucessCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incModelMergingsInstanceFoundCount(cint64 incCount) {
				mModelMergingsInstanceFoundCount += incCount;
				return this;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getModelMergingsInstanceFoundCount() {
				return mModelMergingsInstanceFoundCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incModelMergingsNonInstanceFoundCount(cint64 incCount) {
				mModelMergingsNonInstanceFoundCount += incCount;
				return this;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getModelMergingsNonInstanceFoundCount() {
				return mModelMergingsNonInstanceFoundCount;
			}


			QString COptimizedRepresentativeKPSetOntologyRealizingItem::getIndividualName(const CIndividualReference& indiRef, bool abbreviated) {
				CIndividual* indi = indiRef.getIndividual();
				QString individualString;
				if (!indi) {
					indi = mOntology->getABox()->getIndividualVector(false)->getData(indiRef.getIndividualID());
				}
				if (indi) {
					if (abbreviated) {
						individualString = CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(indi->getIndividualNameLinker());
					}
					if (individualString.isEmpty()) {
						individualString = CIRIName::getRecentIRIName(indi->getIndividualNameLinker());
					}
				} else {
					COntologyTriplesAssertionsAccessor* accessor = mOntology->getOntologyTriplesData()->getTripleAssertionAccessor();
					if (accessor) {
						accessor->visitIndividualName(indiRef.getIndividualID(), [&](const QString& indiName)->bool {
							individualString = indiName;
							return true;
						});
					}
				}
				return individualString;
			}


			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetOntologyRealizingItem::getInstanceItemReference(const CIndividualReference& indiRef) {
				return getInstanceItemReference(indiRef, true);
			}



			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetOntologyRealizingItem::getInstanceItemReference(const CIndividualReference& indiRef, bool correctSameIndividualsMerging) {
				COptimizedKPSetIndividualItem* indiItem = getInstanceItem(indiRef, correctSameIndividualsMerging);
				return CRealizationIndividualInstanceItemReference(indiRef, indiItem);
			}


			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleInstanceItemReference(const CIndividualReference& indiRef) {
				COptimizedKPSetIndividualItem* indiItem = getInstanceItem(indiRef, false);
				return CRealizationIndividualInstanceItemReference(indiRef, indiItem);
			}



			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetOntologyRealizingItem::getInstanceItemReference(cint64 indiId, bool correctSameIndividualsMerging) {
				CIndividualReference indiRef = CIndividualReference(mOntology->getABox()->getIndividualVector()->getData(indiId), indiId);
				return getInstanceItemReference(indiRef, correctSameIndividualsMerging);
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isConceptInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConcept* concept) {
				CConceptInstantiatedItem* conItem = getInstantiatedItem(concept);
				CConceptInstanceItem* indiItem = (CConceptInstanceItem*)indiRealItemRef.getRealizationInstanceItem();
				COptimizedKPSetConceptInstancesItem* conceptItem = (COptimizedKPSetConceptInstancesItem*)conItem;
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)indiItem;
				if (indiItem) {
					COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = individualItem->getKnownPossibleInstancesHash();
					if (knownPossibleInstancesHash) {
						const COptimizedKPSetConceptInstancesHashData& hashData = knownPossibleInstancesHash->value(conceptItem);
						COptimizedKPSetConceptInstancesData* instanceItemData = hashData.mInstanceItemData;
						if (instanceItemData) {
							return instanceItemData->mKnownInstance;
						}
					}
				} else {			
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = mBackendAssocCacheReader->getIndividualAssociatedCacheLabelItem(indiRealItemRef.getIndividualID(), CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
					if (labelItem) {
						COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* labelItemData = getRepresentativeCacheConceptSetLabelItemData(labelItem, false);
						if (labelItemData) {
							COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData* labelConInstItemData = labelItemData->getInstancesItemData(conceptItem, false);
							if (labelConInstItemData) {
								return !labelConInstItemData->hasPossibleInstances();
							}
						}
					}
				}
				return false;
			}





			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitDirectTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealizationInstantiatedVisitor* visitor) {
				return visitTypes(indiRealItemRef, [&](COptimizedKPSetConceptInstancesItem* conceptInstancesItem)->bool {
					return visitor->visitType(conceptInstancesItem, this);
				}, true);
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitAllTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealizationInstantiatedVisitor* visitor) {
				return visitTypes(indiRealItemRef, [&](COptimizedKPSetConceptInstancesItem* conceptInstancesItem)->bool {
					return visitor->visitType(conceptInstancesItem, this);
				}, false);
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, function<bool(COptimizedKPSetConceptInstancesItem* conceptInstancesItem)> visitFunc, bool visitDirectTypesOnly) {

				bool continueVisiting = true;
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)indiRealItemRef.getRealizationInstanceItem();
				if (individualItem) {
					COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = individualItem->getKnownPossibleInstancesHash();
					if (knownPossibleInstancesHash) {

						for (COptimizedKPSetConceptInstancesHash::const_iterator it = knownPossibleInstancesHash->constBegin(), itEnd = knownPossibleInstancesHash->constEnd(); it != itEnd && continueVisiting; ++it) {
							const COptimizedKPSetConceptInstancesHashData& hashData(it.value());
							COptimizedKPSetConceptInstancesData* instanceItemData = hashData.mInstanceItemData;
							if (instanceItemData->mKnownInstance && (!visitDirectTypesOnly || instanceItemData->mMostSpecific)) {
								COptimizedKPSetConceptInstancesItem* conInstItem(instanceItemData->mInstanceItem);
								continueVisiting = visitFunc(conInstItem);
							}
						}					

					}
				} else {
					CBackendRepresentativeMemoryLabelCacheItem* labelItem = mBackendAssocCacheReader->getIndividualAssociatedCacheLabelItem(indiRealItemRef.getIndividualID(), CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
					if (labelItem) {
						COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* labelItemData = getRepresentativeCacheConceptSetLabelItemData(labelItem, false);
						if (labelItemData) {
							QHash<COptimizedKPSetConceptInstancesItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData*>* instancesItemDataHash = labelItemData->getInstancesItemDataHash();
							for (QHash<COptimizedKPSetConceptInstancesItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData*>::const_iterator it = instancesItemDataHash->constBegin(), itEnd = instancesItemDataHash->constEnd(); it != itEnd && continueVisiting; ++it) {
								COptimizedKPSetConceptInstancesItem* conInstItem = it.key();
								COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData* conInstItemData = it.value();
								if (!conInstItemData->hasPossibleInstances() && (conInstItemData->hasMostSpecificInstances() || !visitDirectTypesOnly)) {
									continueVisiting = visitFunc(conInstItem);
								}
							}
							
						}
					}
				}
				return true;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitDirectInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor) {
				return visitInstances(item, [&](const CRealizationIndividualInstanceItemReference& indiInstItemRef)->bool {
					return visitor->visitInstance(indiInstItemRef, this);
				}, true, CRealizationIndividualSorting(CRealizationIndividualSorting::SORT_TYPE_INDIVIDUAL_ID, CRealizationIndividualSorting::SORT_ORDER_NOTHING, CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE));

			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitAllInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor) {		
				return visitInstances(item, [&](const CRealizationIndividualInstanceItemReference& indiInstItemRef)->bool {
					return visitor->visitInstance(indiInstItemRef, this);
				}, false, CRealizationIndividualSorting(CRealizationIndividualSorting::SORT_TYPE_INDIVIDUAL_ID, CRealizationIndividualSorting::SORT_ORDER_NOTHING, CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE));
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitInstances(CConceptInstantiatedItem* item, function<bool(const CRealizationIndividualInstanceItemReference& indiInstItemRef)> visitFunc, bool visitDirectInstancesOnly, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor) {
				// iterate over all representative cache items as well as the known/possible data and return the individuals (if sorting is used, then return them accordingly)
				COptimizedKPSetConceptInstancesItem* conceptItem = (COptimizedKPSetConceptInstancesItem*)item;

				COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator* cacheLabelInstancesIterator = (COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator*)getConceptInstancesIterator(conceptItem->getHierarchyNode()->getOneEquivalentConcept(), visitDirectInstancesOnly, sorting, indiInstItemRefCursor, true);
				cacheLabelInstancesIterator->begin();

				bool continueVisiting = true;
				while (!cacheLabelInstancesIterator->atEnd() && continueVisiting) {
					continueVisiting = visitFunc(cacheLabelInstancesIterator->currentIndividualInstanceItemReference());
					cacheLabelInstancesIterator->moveNext();
				}

				delete cacheLabelInstancesIterator;

				return true;
			}



			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetOntologyRealizingItem::getConceptInstancesIterator(CConcept* concept, bool onlyDirect, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {

				CIndividualVector* indiVector = mOntology->getABox()->getIndividualVector(false);
				COptimizedKPSetConceptInstancesItem* conceptItem = (COptimizedKPSetConceptInstancesItem*)getInstantiatedItem(concept);
				QList<CBackendRepresentativeMemoryLabelCacheItem*> knownCacheItemList;
				QList<CBackendRepresentativeMemoryLabelCacheItem*> possibleCacheItemList;
				if (!onlyDirect) {
					knownCacheItemList = *conceptItem->getKnownInstancesLabelCacheItemList();
					possibleCacheItemList = *conceptItem->getPossibleInstancesLabelCacheItemList();
				} else {
					knownCacheItemList = *conceptItem->getKnownMostSpecificInstancesLabelCacheItemList();
					possibleCacheItemList = *conceptItem->getPossibleMostSpecificInstancesLabelCacheItemList();
				}
				COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator* cacheLabelInstancesIterator = new COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator(knownCacheItemList, possibleCacheItemList, &mIndividualInstantiatedItemHash, indiVector, conceptItem, onlyDirect, sorting, indiInstItemRefCursor, moveOverCursor, mBackendAssocCacheReader);
				return cacheLabelInstancesIterator;

			}




			bool COptimizedRepresentativeKPSetOntologyRealizingItem::getPreferredRolePropagationDirection(CRole* role, bool& inversedPreferred) {
				CRoleInstantiatedItem* roleItem = getRoleInstantiatedItem(role);
				COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleItem;
				COptimizedKPSetRoleInstancesItem* roleInstItem = redRoleItem->getRedirectedItem();
				bool itemInversed = redRoleItem->isInversed();

				if (!roleInstItem->hasPropagationDirectionDetermined()) {
					cint64 instanceCount = 0;
					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* combinedNeighbourRoleSetLabelDataHash = roleInstItem->getCombinedNeighbourCacheLabelItemDataHash(false);
					for (COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash::const_iterator it = combinedNeighbourRoleSetLabelDataHash->constBegin(), itEnd = combinedNeighbourRoleSetLabelDataHash->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleSetLabel = it.key();
						instanceCount += combinedNeighbourRoleSetLabel->getIndividualAssociationCount();
					}


					cint64 inverseInstanceCount = 0;
					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* inversedCombinedNeighbourRoleSetLabelDataHash = roleInstItem->getCombinedNeighbourCacheLabelItemDataHash(true);
					for (COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash::const_iterator it = inversedCombinedNeighbourRoleSetLabelDataHash->constBegin(), itEnd = inversedCombinedNeighbourRoleSetLabelDataHash->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleSetLabel = it.key();
						inverseInstanceCount += combinedNeighbourRoleSetLabel->getIndividualAssociationCount();
					}

					if (instanceCount >= inverseInstanceCount * 1.3) {
						roleInstItem->setPreferredPropagationDirection(COptimizedKPSetRoleInstancesItem::STRAIGHT);
					} else if (inverseInstanceCount >= instanceCount * 1.3) {
						roleInstItem->setPreferredPropagationDirection(COptimizedKPSetRoleInstancesItem::INVERSE);
					} else {
						roleInstItem->setPreferredPropagationDirection(COptimizedKPSetRoleInstancesItem::NONE);
					}
					roleInstItem->setPropagationDirectionDetermined(true);
				}


				COptimizedKPSetRoleInstancesItem::RolePreferredPropagationDirection preferredPropagationDirection = roleInstItem->getPreferredPropagationDirection();
				if (preferredPropagationDirection == COptimizedKPSetRoleInstancesItem::NONE) {
					return false;
				}

				COptimizedKPSetRoleInstancesItem::RolePreferredPropagationDirection compDirection = COptimizedKPSetRoleInstancesItem::STRAIGHT;
				if (itemInversed) {
					compDirection = COptimizedKPSetRoleInstancesItem::INVERSE;
				}
				if (preferredPropagationDirection == compDirection) {
					inversedPreferred = false;
				} else {
					inversedPreferred = true;
				}

				return true;


			}



			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleNeighbouringInstancesIterator(CRole* role, bool inversed, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {
				CIndividualVector* indiVector = mOntology->getABox()->getIndividualVector(false);
				CRoleInstantiatedItem* roleItem = getRoleInstantiatedItem(role);

				COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleItem;
				COptimizedKPSetRoleInstancesItem* roleInstItem = redRoleItem->getRedirectedItem();
				bool itemInversed = redRoleItem->isInversed() ^ inversed;

				QSet<CBackendRepresentativeMemoryLabelCacheItem*> possibleAndCandidateLabelSet;
				QSet<CBackendRepresentativeMemoryLabelCacheItem*> knownLabelSet;
				QSet<CBackendRepresentativeMemoryLabelCacheItem*>* possibleExtLabelSet = roleInstItem->getComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet(itemInversed);
				if (possibleExtLabelSet) {
					possibleAndCandidateLabelSet.unite(*possibleExtLabelSet);
				}

				QSet<CBackendRepresentativeMemoryLabelCacheItem*>* possibleNeighbourLabelSet = roleInstItem->getComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet(itemInversed);
				if (possibleNeighbourLabelSet) {
					possibleAndCandidateLabelSet.unite(*possibleNeighbourLabelSet);
				}

				
				COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* combinedNeighbourRoleSetLabelDataHash = roleInstItem->getCombinedNeighbourCacheLabelItemDataHash(itemInversed);
				for (COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash::const_iterator it = combinedNeighbourRoleSetLabelDataHash->constBegin(), itEnd = combinedNeighbourRoleSetLabelDataHash->constEnd(); it != itEnd; ++it) {
					CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleSetLabel = it.key();
					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* combinedNeighbourRoleSetLabelData = it.value();
					if (combinedNeighbourRoleSetLabelData->hasPossibleInstancesLabelItems()) {
						possibleAndCandidateLabelSet.insert(combinedNeighbourRoleSetLabel);
					}
					if (combinedNeighbourRoleSetLabelData->hasKnownInstancesLabelItems()) {
						knownLabelSet.insert(combinedNeighbourRoleSetLabel);
					}
				}				
				QList<CBackendRepresentativeMemoryLabelCacheItem*> knownCacheItemList = knownLabelSet.toList();
				QList<CBackendRepresentativeMemoryLabelCacheItem*> possibleCacheItemList;
				for (CBackendRepresentativeMemoryLabelCacheItem* item : possibleAndCandidateLabelSet) {
					if (!knownLabelSet.contains(item)) {
						possibleCacheItemList.append(item);
					}
				}


				COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator* cacheLabelInstancesIterator = new COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator(knownCacheItemList, possibleCacheItemList, &mIndividualInstantiatedItemHash, indiVector, roleInstItem, itemInversed, sorting, indiInstItemRefCursor, moveOverCursor);
				return cacheLabelInstancesIterator;
			}





			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleExistentialInstancesIterator(CRole* role, bool inversed, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {
				CIndividualVector* indiVector = mOntology->getABox()->getIndividualVector(false);
				CRoleInstantiatedItem* roleItem = getRoleInstantiatedItem(role);

				COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleItem;
				COptimizedKPSetRoleInstancesItem* roleInstItem = redRoleItem->getRedirectedItem();
				bool itemInversed = redRoleItem->isInversed() ^ inversed;

				QSet<CBackendRepresentativeMemoryLabelCacheItem*> knownLabelSet;
				QSet<CBackendRepresentativeMemoryLabelCacheItem*> possibleAndCandidateLabelSet;
				QSet<CBackendRepresentativeMemoryLabelCacheItem*>* possibleExtLabelSet = roleInstItem->getComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet(itemInversed);
				if (possibleExtLabelSet) {
					possibleAndCandidateLabelSet.unite(*possibleExtLabelSet);
				}

				QSet<CBackendRepresentativeMemoryLabelCacheItem*>* possibleNeighbourLabelSet = roleInstItem->getComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet(itemInversed);
				if (possibleNeighbourLabelSet) {
					possibleAndCandidateLabelSet.unite(*possibleNeighbourLabelSet);
				}

				COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* combinedNeighbourRoleSetLabelDataHash = roleInstItem->getCombinedNeighbourCacheLabelItemDataHash(itemInversed);
				for (COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash::const_iterator it = combinedNeighbourRoleSetLabelDataHash->constBegin(), itEnd = combinedNeighbourRoleSetLabelDataHash->constEnd(); it != itEnd; ++it) {
					CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleSetLabel = it.key();
					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* combinedNeighbourRoleSetLabelData = it.value();
					if (combinedNeighbourRoleSetLabelData->hasPossibleInstancesLabelItems()) {
						possibleAndCandidateLabelSet.insert(combinedNeighbourRoleSetLabel);
					}
					if (combinedNeighbourRoleSetLabelData->hasKnownInstancesLabelItems()) {
						knownLabelSet.insert(combinedNeighbourRoleSetLabel);
					}
				}

				QSet<CBackendRepresentativeMemoryLabelCacheItem*>* existentialRoleLabelCacheItemSet = roleInstItem->getExistentialRoleLabelCacheItemSet(itemInversed);
				if (existentialRoleLabelCacheItemSet) {
					possibleAndCandidateLabelSet.unite(*existentialRoleLabelCacheItemSet);
				}

				QList<CBackendRepresentativeMemoryLabelCacheItem*> knownCacheItemList = knownLabelSet.toList();
				QList<CBackendRepresentativeMemoryLabelCacheItem*> possibleCacheItemList;
				for (CBackendRepresentativeMemoryLabelCacheItem* item : possibleAndCandidateLabelSet) {
					if (!knownLabelSet.contains(item)) {
						possibleCacheItemList.append(item);
					}
				}

				COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator* cacheLabelInstancesIterator = new COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator(knownCacheItemList, possibleCacheItemList, &mIndividualInstantiatedItemHash, indiVector, sorting, indiInstItemRefCursor, moveOverCursor);
				return cacheLabelInstancesIterator;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasExistentiallyLinkedRoleInstances(CRole* role, bool inversed) {
				CRoleInstantiatedItem* roleItem = getRoleInstantiatedItem(role);

				COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleItem;
				COptimizedKPSetRoleInstancesItem* roleInstItem = redRoleItem->getRedirectedItem();
				bool itemInversed = redRoleItem->isInversed() ^ inversed;

				QSet<CBackendRepresentativeMemoryLabelCacheItem*>* possibleExtLabelSet = roleInstItem->getComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet(itemInversed);
				QSet<CBackendRepresentativeMemoryLabelCacheItem*>* existentialRoleLabelCacheItemSet = roleInstItem->getExistentialRoleLabelCacheItemSet(itemInversed);
				if ((existentialRoleLabelCacheItemSet && !existentialRoleLabelCacheItemSet->isEmpty()) || (possibleExtLabelSet && !possibleExtLabelSet->isEmpty())) {
					return true;
				}
				return false;
			}



			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleDataInstancesIterator(CRole* role, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {
				CIndividualVector* indiVector = mOntology->getABox()->getIndividualVector(false);
				QSet<CBackendRepresentativeMemoryLabelCacheItem*>* dataDetRoleLabelSet = mDataRoleDetCacheLabelItemSetHash.value(role);
				QSet<CBackendRepresentativeMemoryLabelCacheItem*>* dataNondetRoleLabelSet = mDataRoleNonDetCacheLabelItemSetHash.value(role);
				QList<CBackendRepresentativeMemoryLabelCacheItem*> knownCacheItemList;
				QList<CBackendRepresentativeMemoryLabelCacheItem*> possibleCacheItemList;
				if (dataDetRoleLabelSet) {
					knownCacheItemList = dataDetRoleLabelSet->toList();
				}
				if (dataNondetRoleLabelSet) {
					possibleCacheItemList = dataNondetRoleLabelSet->toList();
				}

				COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator* cacheLabelInstancesIterator = new COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator(knownCacheItemList, possibleCacheItemList, &mIndividualInstantiatedItemHash, indiVector, sorting, indiInstItemRefCursor, moveOverCursor);
				return cacheLabelInstancesIterator;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasOnlyDeterministicRoleDataInstances(CRole* role) {
				return !mDataRoleNondeterministicCacheLabelsSet.contains(role);
			}





			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleFillerInstancesIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CRole* role, bool inversed, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {

				CRoleInstantiatedItem* roleItem = getRoleInstantiatedItem(role);

				return getRoleFillerInstancesIterator(indiInstItemRef, roleItem, inversed, sorting, indiInstItemRefCursor, moveOverCursor);
			}


			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleFillerInstancesIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CRoleInstantiatedItem* roleInstancesItem, bool inversed, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {

				CIndividualVector* indiVector = mOntology->getABox()->getIndividualVector(false);

				COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleInstancesItem;
				COptimizedKPSetRoleInstancesItem* roleInstItem = redRoleItem->getRedirectedItem();
				bool itemInversed = redRoleItem->isInversed() ^ inversed;

				CRealizationIndividualInstanceItemReferenceIterator* fillerIterator = nullptr;
				bool considerSameMergedIndis = sorting.getGrouping() != CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE;
				
				if (sorting.isNothing()) {
					fillerIterator = new COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator(indiInstItemRef, mBackendAssocCacheReader, roleInstItem, itemInversed, &mIndividualInstantiatedItemHash, indiVector, true);
				} else if (sorting.getType() == CRealizationIndividualSorting::SORT_TYPE_NOTHING && sorting.getGrouping() == CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE && !considerSameMergedIndis) {
					fillerIterator = new COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator(indiInstItemRef, mBackendAssocCacheReader, roleInstItem, itemInversed, &mIndividualInstantiatedItemHash, indiVector, false);
				} else {
					fillerIterator = new COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator(indiInstItemRef, mBackendAssocCacheReader, roleInstItem, itemInversed, &mIndividualInstantiatedItemHash, indiVector, sorting, indiInstItemRefCursor, moveOverCursor);
				}
				return fillerIterator;
			}



			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetOntologyRealizingItem::getIterationCandidateIndividualIdIterator(const QMap<cint64, IndividualIdCandidateData>& candidateIndiIdsMap, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {
				CIndividualVector* indiVector = mOntology->getABox()->getIndividualVector(false);
				return new COptimizedRepresentativeKPSetIndividualCandiatesIterator(candidateIndiIdsMap, mBackendAssocCacheReader, &mIndividualInstantiatedItemHash, indiVector, sorting, indiInstItemRefCursor, moveOverCursor);
			}




			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetOntologyRealizingItem::getIterationIntersectionIterator(QList<CRealizationIndividualInstanceItemReferenceIterator*> takeIteratorList) {
				QList<CRealizationIndividualInstanceItemReferenceIterator*> emptyIteratorList;
				QList<CRealizationIndividualInstanceItemReferenceIterator*> unmergeableIteratorList;
				QList<COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator*> conceptSetCacheLabelIteratorList;
				QList<COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator*> roleNeighbouringCacheLabelIteratorList;

				CIndividualVector* indiVector = mOntology->getABox()->getIndividualVector(false);

				for (CRealizationIndividualInstanceItemReferenceIterator* iterator : takeIteratorList) {
					if (!iterator->requiresInitialization() && iterator->atEnd()) {
						emptyIteratorList.append(iterator);
					} else {

						COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator* conceptSetCacheLabelIterator = dynamic_cast<COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator*>(iterator);
						if (conceptSetCacheLabelIterator) {
							conceptSetCacheLabelIteratorList.append(conceptSetCacheLabelIterator);
						} else {
							COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator* roleNeighbouringCacheLabelIterator = dynamic_cast<COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator*>(iterator);
							if (roleNeighbouringCacheLabelIterator) {
								roleNeighbouringCacheLabelIteratorList.append(roleNeighbouringCacheLabelIterator);
							} else {
								unmergeableIteratorList.append(iterator);
							}
						}
					}
				}

				bool unsatisfiable = false;
				if (!emptyIteratorList.isEmpty()) {
					qDeleteAll(takeIteratorList);
					unmergeableIteratorList.clear();
					conceptSetCacheLabelIteratorList.clear();
					roleNeighbouringCacheLabelIteratorList.clear();
					unsatisfiable = true;
				}



				if (!unsatisfiable) {
					if (conceptSetCacheLabelIteratorList.size() > 1) {
						// merge concept set label iterators
						QSet<CBackendRepresentativeMemoryLabelCacheItem*> validKnownLabelSet;
						QSet<CBackendRepresentativeMemoryLabelCacheItem*> validPossibleLabelSet;
						bool hasValidLabelSets = false;

						QSet<CBackendRepresentativeMemoryLabelCacheItem*> nextValidKnownLabelSet;
						QSet<CBackendRepresentativeMemoryLabelCacheItem*> nextValidPossibleLabelSet;

						QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TConceptInstancesItemMostSpecificFlagPair>> possibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash;
						QSet<TConceptInstancesItemMostSpecificFlagPair> conceptInstancesItemOnlyMostSpecificFlagPairSet;


						// at the moment it is just assumed that all have the same values for cursor/sorting/moveOverCursor
						// TODO: make sure all have the same values for cursor/sorting/moveOverCursor or combine only those that have the same values or determine a new minimal/maximal cursor
						CRealizationIndividualSorting sorting = conceptSetCacheLabelIteratorList.first()->getSorting();
						CRealizationIndividualInstanceItemReference indiInstItemRefCursor = conceptSetCacheLabelIteratorList.first()->getIndividualInstanceItemReferenceCursor();
						bool moveOverCursor = conceptSetCacheLabelIteratorList.first()->hasMoveOverCursor();


						for (COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator* conceptSetCacheLabelIterator : conceptSetCacheLabelIteratorList) {

							conceptInstancesItemOnlyMostSpecificFlagPairSet.unite(*conceptSetCacheLabelIterator->getConceptInstancesItemOnlyMostSpecificFlagPairSet());
							QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TConceptInstancesItemMostSpecificFlagPair>>* possibleLabelConceptInstancesItemMostSpecificPairSetHash = conceptSetCacheLabelIterator->getPossibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash();
							for (QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TConceptInstancesItemMostSpecificFlagPair>>::const_iterator it = possibleLabelConceptInstancesItemMostSpecificPairSetHash->constBegin(), itEnd = possibleLabelConceptInstancesItemMostSpecificPairSetHash->constEnd(); it != itEnd; ++it) {
								QSet<TConceptInstancesItemMostSpecificFlagPair>& instancesItemMostSpecificPairSet = possibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash[it.key()];
								instancesItemMostSpecificPairSet.unite(it.value());
							}
												
							for (CBackendRepresentativeMemoryLabelCacheItem* label : *conceptSetCacheLabelIterator->getKnownInstancesLabelCacheItemList()) {
								if (!hasValidLabelSets || validKnownLabelSet.contains(label) && !nextValidPossibleLabelSet.contains(label)) {
									nextValidKnownLabelSet.insert(label);
								}
								if (validPossibleLabelSet.contains(label)) {
									nextValidPossibleLabelSet.insert(label);
								}
							}

							for (CBackendRepresentativeMemoryLabelCacheItem* label : *conceptSetCacheLabelIterator->getPossibleInstancesLabelCacheItemList()) {
								if (!hasValidLabelSets || validPossibleLabelSet.contains(label)) {
									nextValidPossibleLabelSet.insert(label);
								} else if (validKnownLabelSet.contains(label)) {
									nextValidKnownLabelSet.remove(label);
									nextValidPossibleLabelSet.insert(label);
								}
							}

							hasValidLabelSets = true;
							validKnownLabelSet = nextValidKnownLabelSet;
							validPossibleLabelSet = nextValidPossibleLabelSet;
							nextValidKnownLabelSet.clear();
							nextValidPossibleLabelSet.clear();
						}
					
						qDeleteAll(conceptSetCacheLabelIteratorList);
						conceptSetCacheLabelIteratorList.clear();
						if (!validKnownLabelSet.isEmpty() || !validPossibleLabelSet.isEmpty()) {
							COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator* cacheLabelInstancesIterator = new COptimizedRepresentativeKPSetCacheLabelConceptSetItemIterator(validKnownLabelSet.toList(), validPossibleLabelSet.toList(), &mIndividualInstantiatedItemHash, indiVector, possibleLabelConceptInstancesItemOnlyMostSpecificFlagPairSetHash, conceptInstancesItemOnlyMostSpecificFlagPairSet, sorting, indiInstItemRefCursor, moveOverCursor, mBackendAssocCacheReader);
							unmergeableIteratorList.append(cacheLabelInstancesIterator);
						} else {
							unsatisfiable = true;
						}

					} else if (!conceptSetCacheLabelIteratorList.isEmpty()) {
						unmergeableIteratorList.append(conceptSetCacheLabelIteratorList.first());
						conceptSetCacheLabelIteratorList.clear();
					}
				}




				if (!unsatisfiable) {
					if (roleNeighbouringCacheLabelIteratorList.size() > 1) {
						// merge concept set label iterators
						QSet<CBackendRepresentativeMemoryLabelCacheItem*> validKnownLabelSet;
						QSet<CBackendRepresentativeMemoryLabelCacheItem*> validPossibleLabelSet;
						bool hasValidLabelSets = false;

						QSet<CBackendRepresentativeMemoryLabelCacheItem*> nextValidKnownLabelSet;
						QSet<CBackendRepresentativeMemoryLabelCacheItem*> nextValidPossibleLabelSet;


						QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TRoleInstancesItemInversedFlagPair>> possibleLabelRoleInstancesItemInversedFlagPairSetHash;
						QSet<TRoleInstancesItemInversedFlagPair> roleInstancesItemInversedFlagPairSet;

						// at the moment it is just assumed that all have the same values for cursor/sorting/moveOverCursor
						// TODO: make sure all have the same values for cursor/sorting/moveOverCursor or combine only those that have the same values or determine a new minimal/maximal cursor
						CRealizationIndividualSorting sorting = roleNeighbouringCacheLabelIteratorList.first()->getSorting();
						CRealizationIndividualInstanceItemReference indiInstItemRefCursor = roleNeighbouringCacheLabelIteratorList.first()->getIndividualInstanceItemReferenceCursor();
						bool moveOverCursor = roleNeighbouringCacheLabelIteratorList.first()->hasMoveOverCursor();


						for (COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator* roleNeighbouringSetCacheLabelIterator : roleNeighbouringCacheLabelIteratorList) {

							roleInstancesItemInversedFlagPairSet.unite(*roleNeighbouringSetCacheLabelIterator->getRoleInstancesItemInversionPairSet());
							QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TRoleInstancesItemInversedFlagPair>>* possibleLabelRoleInstancesItemInversionPairSetHash = roleNeighbouringSetCacheLabelIterator->getPossibleLabelRoleInstancesItemInversionPairSetHash();
							for (QHash<CBackendRepresentativeMemoryLabelCacheItem*, QSet<TRoleInstancesItemInversedFlagPair>>::const_iterator it = possibleLabelRoleInstancesItemInversionPairSetHash->constBegin(), itEnd = possibleLabelRoleInstancesItemInversionPairSetHash->constEnd(); it != itEnd; ++it) {
								QSet<TRoleInstancesItemInversedFlagPair>& existSet = possibleLabelRoleInstancesItemInversedFlagPairSetHash[it.key()];
								existSet.unite(it.value());
							}
						
							for (CBackendRepresentativeMemoryLabelCacheItem* label : *roleNeighbouringSetCacheLabelIterator->getKnownInstancesLabelCacheItemList()) {
								if (!hasValidLabelSets || validKnownLabelSet.contains(label) && !nextValidPossibleLabelSet.contains(label)) {
									nextValidKnownLabelSet.insert(label);
								}
								if (validPossibleLabelSet.contains(label)) {
									nextValidPossibleLabelSet.insert(label);
								}
							}

							for (CBackendRepresentativeMemoryLabelCacheItem* label : *roleNeighbouringSetCacheLabelIterator->getPossibleInstancesLabelCacheItemList()) {
								if (!hasValidLabelSets || validPossibleLabelSet.contains(label)) {
									nextValidPossibleLabelSet.insert(label);
								} else if (validKnownLabelSet.contains(label)) {
									nextValidKnownLabelSet.remove(label);
									nextValidPossibleLabelSet.insert(label);
								}
							}


							hasValidLabelSets = true;
							validKnownLabelSet = nextValidKnownLabelSet;
							validPossibleLabelSet = nextValidPossibleLabelSet;
							nextValidKnownLabelSet.clear();
							nextValidPossibleLabelSet.clear();
						}

						qDeleteAll(roleNeighbouringCacheLabelIteratorList);
						roleNeighbouringCacheLabelIteratorList.clear();
						if (!validKnownLabelSet.isEmpty() || !validPossibleLabelSet.isEmpty()) {
							COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator* cacheLabelInstancesIterator = new COptimizedRepresentativeKPSetCacheLabelRoleNeighbouringItemIterator(validKnownLabelSet.toList(), validPossibleLabelSet.toList(), &mIndividualInstantiatedItemHash, indiVector, possibleLabelRoleInstancesItemInversedFlagPairSetHash, roleInstancesItemInversedFlagPairSet, sorting, indiInstItemRefCursor, moveOverCursor);
							unmergeableIteratorList.append(cacheLabelInstancesIterator);
						} else {
							unsatisfiable = true;
						}

					} else if (!roleNeighbouringCacheLabelIteratorList.isEmpty()) {
						unmergeableIteratorList.append(roleNeighbouringCacheLabelIteratorList.first());
						roleNeighbouringCacheLabelIteratorList.clear();
					}
				}

				if (unsatisfiable) {
					qDeleteAll(unmergeableIteratorList);
					qDeleteAll(conceptSetCacheLabelIteratorList);
					qDeleteAll(roleNeighbouringCacheLabelIteratorList);
					unmergeableIteratorList.clear();
					conceptSetCacheLabelIteratorList.clear();
					roleNeighbouringCacheLabelIteratorList.clear();
				}


				if (unmergeableIteratorList.size() == 1) {
					return unmergeableIteratorList.first();
				}

				COptimizedRepresentativeKPSetIntersectionCombinationIterator* combinedIterator = new COptimizedRepresentativeKPSetIntersectionCombinationIterator(unmergeableIteratorList);
				return combinedIterator;
			}



			CSameInstanceItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getSameInstanceItem(const CRealizationIndividualInstanceItemReference& indiRealItemRef) {
				return indiRealItemRef.getRealizationInstanceItem();
			}





			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isRoleInstance(CRole* role, const CRealizationIndividualInstanceItemReference& sourceIndiRealItemRef, const CRealizationIndividualInstanceItemReference& targetIndiRealItemRef) {
				CRoleInstantiatedItem* roleItem = getRoleInstantiatedItem(role);

				COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleItem;
				COptimizedKPSetRoleInstancesItem* roleInstItem = redRoleItem->getRedirectedItem();
				bool inversed = redRoleItem->isInversed();

				if (roleInstItem == mTopRoleInstancesItem) {
					return true;
				}

				COptimizedKPSetIndividualItem* sourceIndividualItem = (COptimizedKPSetIndividualItem*)sourceIndiRealItemRef.getRealizationInstanceItem();
				if (sourceIndividualItem) {
					COptimizedKPSetRoleNeighbourInstancesHash* knownPossibleInstancesHash = sourceIndividualItem->getKnownPossibleRoleNeighboursInstancesHash();
					COptimizedKPSetRoleNeighbourInstancesHashData possNeighbourInstanceData = knownPossibleInstancesHash->value(roleInstItem);
					COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = possNeighbourInstanceData.getRoleNeighbourInstancesHash(inversed, false);
					if (possNeighbourInstanceHash) {
						COptimizedKPSetRoleInstancesData* instanceData = possNeighbourInstanceHash->value(targetIndiRealItemRef.getIndividualID()).mInstanceItemData;
						if (instanceData) {
							return instanceData->mKnownInstance;
						}
					}
					// TODO: complex roles
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mBackendAssocCacheReader->getIndividualAssociationData(sourceIndiRealItemRef.getIndividualID());
				if (indiAssData) {
					CBackendRepresentativeMemoryLabelCacheItem* combinedNeigRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);

					CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetLabelHash = indiAssData->getNeighbourRoleSetHash();
					if (neighbourRoleSetLabelHash) {
						CBackendRepresentativeMemoryLabelCacheItem* singleNeigRoleSetLabel = neighbourRoleSetLabelHash->getNeighbourRoleSetLabel(targetIndiRealItemRef.getIndividualID());

						COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* roleItemCompNeighLabelData = roleInstItem->getCombinedNeighbourCacheLabelItemDataHash(inversed)->value(combinedNeigRoleSetLabel);
						if (roleItemCompNeighLabelData) {
							if (roleItemCompNeighLabelData->hasKnownInstancesLabelItem(singleNeigRoleSetLabel)) {
								return true;
							}
						}
					}

				}


				if (roleInstItem->hasComplexRoleData()) {
					COptimizedKPSetIndividualComplexRoleData* indiComplexData = roleInstItem->getIndividualIdComplexRoleData(sourceIndiRealItemRef.getIndividualID(), false);
					if (indiComplexData) {
						COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;
						COptimizedKPSetRoleInstancesData* instanceData = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstanceItemData(inversed, targetIndiRealItemRef.getIndividualID(), false);
						if (instanceData) {
							return instanceData->mKnownInstance;
						}
					}
				}

				return false;
			}







			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealizationIndividualVisitor* visitor) {
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)indiRealItemRef.getRealizationInstanceItem();
				bool continueVisiting = true;
				if (individualItem) {
					QSet<CIndividualReference>* individualSet = individualItem->getKnownSameIndividualSet();
					for (QSet<CIndividualReference>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd && continueVisiting; ++it) {
						CIndividualReference individual(*it);
						continueVisiting = visitor->visitIndividual(individual, this);
					}
				} else {
					CBackendRepresentativeMemoryLabelCacheItem* sameIndilabelItem = mBackendAssocCacheReader->getIndividualAssociatedCacheLabelItem(indiRealItemRef.getIndividualID(), CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
					if (sameIndilabelItem) {
						mBackendAssocCacheReader->visitIndividualIdsOfAssociatedIndividualSetLabel(nullptr, sameIndilabelItem, [&](cint64 indiId)->bool {
							continueVisiting = visitor->visitIndividual(indiId, this);
							return continueVisiting;
						});
					} else {
						continueVisiting = visitor->visitIndividual(indiRealItemRef, this);
					}
				}
				return true;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitConcepts(CConceptInstantiatedItem* item, CConceptRealizationConceptVisitor* visitor) {
				COptimizedKPSetConceptInstancesItem* conceptItem = (COptimizedKPSetConceptInstancesItem*)item;
				bool continueVisiting = true;
				CHierarchyNode* hierNode = conceptItem->getHierarchyNode();
				if (hierNode) {
					QList<CConcept*>* conList = hierNode->getEquivalentConceptList();
					for (QList<CConcept*>::const_iterator it = conList->constBegin(), itEnd = conList->constEnd(); it != itEnd && continueVisiting; ++it) {
						CConcept* concept(*it);
						continueVisiting = visitor->visitConcept(concept,this);
					}
				}
				return true;
			}




			CSameInstanceItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getSameInstanceItem(CRoleInstanceItem* instanceItem) {
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)instanceItem;
				return individualItem;
			}

			CRoleInstanceItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleInstanceItem(CSameInstanceItem* instanceItem) {
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)instanceItem;
				return individualItem;
			}


			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleInstanceItemReference(CSameInstanceItem* instanceItem) {
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)instanceItem;
				return CRealizationIndividualInstanceItemReference(individualItem->getIndividualReference(), individualItem);
			}



			CConceptInstantiatedItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getInstantiatedItem(CConcept* concept) {
				return mConceptInstancesItemHash.value(concept);
			}

			COptimizedKPSetIndividualItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getInstanceItem(const CIndividualReference& indiRef, bool correctSameIndividualsMerging) {
				COptimizedKPSetIndividualItem* indiRealItem = mIndividualInstantiatedItemHash.value(indiRef.getIndividualID());
				if (indiRealItem && correctSameIndividualsMerging && indiRealItem->getItemSameIndividualMerged()) {
					indiRealItem = indiRealItem->getItemSameIndividualMerged();
				}
				return indiRealItem;
			}


			CRoleInstantiatedItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleInstantiatedItem(CRole* role) {
				return mRedirectedRoleInstancesItemHash.value(role);
			}

			CRoleInstanceItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleInstanceItem(CIndividual* individual) {
				CRoleInstanceItem* roleItem = mIndividualInstantiatedItemHash.value(individual->getIndividualID());
				return roleItem;
			}



			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setPotentiallySameIndividuals(bool potentiallySameIndividuals) {
				mPotentiallySameIndividuals = potentiallySameIndividuals;
				return this;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasPotentiallySameIndividuals() {
				return mBackendAssocCacheReader->hasSameIndividualsMergings() || mPotentiallySameIndividuals;
			}



			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetOntologyRealizingItem::getSameInstanceItemReference(const CIndividualReference& indiRef) {
				COptimizedKPSetIndividualItem* indiItem = getInstanceItem(indiRef, false);
				return CRealizationIndividualInstanceItemReference(indiRef, indiItem);
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitSameIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CSameRealizationIndividualVisitor* visitor) {
				cint64 indiId = indiRealItemRef.getIndividualID();
				bool continueVisiting = visitor->visitIndividual(indiRealItemRef, this);
				if (continueVisiting) {
					COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)indiRealItemRef.getRealizationInstanceItem();
					if (individualItem) {
						QSet<CIndividualReference>* individualSet = individualItem->getKnownSameIndividualSet();
						for (QSet<CIndividualReference>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); continueVisiting && it != itEnd; ++it) {
							CIndividualReference individual(*it);
							if (individual.getIndividualID() != indiId) {
								continueVisiting &= visitor->visitIndividual(individual, this);
							}
						}
						return true;
					} else {

						CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mBackendAssocCacheReader->getIndividualAssociationData(indiRealItemRef.getIndividualID());
						if (indiAssData) {
							CBackendRepresentativeMemoryLabelCacheItem* sameIndiSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
							if (sameIndiSetLabel) {
								mBackendAssocCacheReader->visitIndividualIdsOfAssociatedIndividualSetLabel(nullptr, sameIndiSetLabel, [&](cint64 sameIndiId)->bool {
									if (sameIndiId != indiId) {
										if (!visitor->visitIndividual(sameIndiId, this)) {
											return false;
										}
									}
									return true;
								});
							}
							return true;
						}
					}
				}
				return false;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitSameIndividuals(CSameInstanceItem* item, CSameRealizationIndividualVisitor* visitor) {
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)item;
				QSet<CIndividualReference>* individualSet = individualItem->getKnownSameIndividualSet();
				bool continueVisiting = true;
				for (QSet<CIndividualReference>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd && continueVisiting; ++it) {
					CIndividualReference individual(*it);
					continueVisiting = visitor->visitIndividual(individual,this);
				}
				return true;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isSameIndividualsMerged(const CRealizationIndividualInstanceItemReference& indiInstItemRef) {
				if (indiInstItemRef.getRealizationInstanceItem()) {
					COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)indiInstItemRef.getRealizationInstanceItem();
					if (indiItem->getIndividualId() != indiInstItemRef.getIndividualID() || indiItem->isItemSameIndividualMerged()) {
						return true;
					}
					return false;
				} else {
					if (mSameMergedIndisInCache) {
						CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourIndiAssData = mBackendAssocCacheReader->getIndividualAssociationData(indiInstItemRef.getIndividualID());
						return neighbourIndiAssData->hasRepresentativeSameIndividualMerging();
					} else {
						return false;
					}
				}
			}






			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitAllIndividuals(function<bool(const CRealizationIndividualInstanceItemReference& indiInstItemRef)> visitFunc, bool visitSameIndividualMerged) {

				CBOXSET<CIndividual*>* activeIndiSet = mOntology->getABox()->getActiveIndividualSet();
				CIndividualVector* indiVec = mOntology->getABox()->getIndividualVector(false);
				cint64 indiCount = 0;
				if (indiVec) {
					indiCount = indiVec->getItemCount();
				}

				bool visited = false;
				bool continueVisiting = true;
				cint64 maxTriplesIndexedIndiId = 0;
				COntologyTriplesAssertionsAccessor* triplesAccessor = mOntology->getOntologyTriplesData()->getTripleAssertionAccessor();
				if (triplesAccessor) {
					maxTriplesIndexedIndiId = mOntology->getOntologyTriplesData()->getTripleAssertionAccessor()->getMaxIndexedIndividualId();
				}
				if (indiVec) {
					cint64 indiCount = indiVec->getItemCount();
					for (cint64 idx = 0; idx < indiCount && continueVisiting; ++idx) {
						CIndividual* indi = indiVec->getData(idx);

						CIndividualReference indiRef;
						if (indi && activeIndiSet->contains(indi)) {
							indiRef = CIndividualReference(indi);
							visited = true;
						} else if (idx <= maxTriplesIndexedIndiId) {
							visited = true;
							indiRef = CIndividualReference(idx);
						}

						if (indiRef.isNonEmpty()) {
							CRealizationIndividualInstanceItemReference indiInstItemRef = getInstanceItemReference(indiRef, false);
							if (visitSameIndividualMerged || !isSameIndividualsMerged(indiInstItemRef)) {
								continueVisiting = visitFunc(indiInstItemRef);
							}
						}

					}
				}
				return visited;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitTargetIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem, CRoleRealizationInstanceVisitor* visitor) {
				return visitSourceTargetIndividuals(indiRealItemRef, roleItem, false, visitor);
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitSourceIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem, CRoleRealizationInstanceVisitor* visitor) {
				return visitSourceTargetIndividuals(indiRealItemRef, roleItem, true, visitor);
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitSourceTargetIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleInstantiatedItem* roleItem, bool source, CRoleRealizationInstanceVisitor* visitor) {
				COptimizedKPSetRoleInstancesRedirectionItem* roleRedirectionItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleItem;
				COptimizedKPSetRoleInstancesItem* roleInstItem = roleRedirectionItem->getRedirectedItem();
				bool roleItemInversed = roleRedirectionItem->isInversed() ^ source;
				bool continueVisiting = true;
				if (roleInstItem == mTopRoleInstancesItem) {
					return visitAllIndividuals([&](const CRealizationIndividualInstanceItemReference& indiInstItemRef)->bool {
						continueVisiting = visitor->visitRoleInstance(indiInstItemRef, this);
						return continueVisiting;
					}, false);
				}


				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)indiRealItemRef.getRealizationInstanceItem();
				if (individualItem) {
					COptimizedKPSetRoleNeighbourInstancesHash* roleNeighbourHash = individualItem->getKnownPossibleRoleNeighboursInstancesHash();
					const COptimizedKPSetRoleNeighbourInstancesHashData& roleNeighbourHashData = roleNeighbourHash->value(roleInstItem);
					COptimizedKPSetRoleInstancesHash* neighbourHash = nullptr;
					if (roleItemInversed) {
						neighbourHash = roleNeighbourHashData.mRolePredecessorInstancesHash;
					} else {
						neighbourHash = roleNeighbourHashData.mRoleSuccessorInstancesHash;
					}
					if (neighbourHash) {
						for (COptimizedKPSetRoleInstancesHash::const_iterator it = neighbourHash->constBegin(), itEnd = neighbourHash->constEnd(); it != itEnd && continueVisiting; ++it) {
							const COptimizedKPSetRoleInstancesHashData& neighbourData = it.value();
							COptimizedKPSetIndividualItem* neighbourItem = neighbourData.mNeighbourIndividualItem;
							if (!neighbourItem->isItemSameIndividualMerged()) {
								COptimizedKPSetRoleInstancesData* roleInstData = neighbourData.mInstanceItemData;
								if (roleInstData && roleInstData->mKnownInstance) {
									continueVisiting = visitor->visitRoleInstance(CRealizationIndividualInstanceItemReference(neighbourItem->getIndividualReference(), neighbourItem), this);
								}
							}
						}
					}
				} 

				CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mBackendAssocCacheReader->getIndividualAssociationData(indiRealItemRef.getIndividualID());
				if (continueVisiting && indiAssData) {
					CBackendRepresentativeMemoryLabelCacheItem* combinedNeigRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();

					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* roleItemCompNeighLabelData = roleInstItem->getCombinedNeighbourCacheLabelItemDataHash(roleItemInversed)->value(combinedNeigRoleSetLabel);
					if (roleItemCompNeighLabelData && roleItemCompNeighLabelData->hasKnownInstancesLabelItems()) {
						QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* singleNeighLabelDataHash = roleItemCompNeighLabelData->getKnownInstancesLabelItemDataHash();

						for (auto it = singleNeighLabelDataHash->constBegin(), itEnd = singleNeighLabelDataHash->constEnd(); it != itEnd && continueVisiting; ++it) {
							roleSetNeighbourArray->at(it.value()->getLabelArrayIndex()).visitNeighbourIndividualIds([&](cint64 neighbourId)->bool {
								// check same individual merged
								CRealizationIndividualInstanceItemReference neighbourIndiItemRef = getInstanceItemReference(neighbourId, false);
								if (!isSameIndividualsMerged(neighbourIndiItemRef)) {
									continueVisiting = visitor->visitRoleInstance(neighbourIndiItemRef, this);
								}
								return continueVisiting;
							});
						}

					}
				}


				if (continueVisiting && roleInstItem->hasComplexRoleData()) {
					COptimizedKPSetIndividualComplexRoleData* indiComplexData = roleInstItem->getIndividualIdComplexRoleData(indiRealItemRef.getIndividualID(), false);
					if (indiComplexData) {

						COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;
						COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(roleItemInversed, false);
						if (possNeighbourInstanceHash) {
							for (COptimizedKPSetRoleInstancesHash::const_iterator itNeighInst = possNeighbourInstanceHash->constBegin(), itNeighInstEnd = possNeighbourInstanceHash->constEnd(); itNeighInst != itNeighInstEnd && continueVisiting; ++itNeighInst) {
								const COptimizedKPSetRoleInstancesHashData& destIndiInstanceDat = itNeighInst.value();
								cint64 destIndiId = itNeighInst.key();
								COptimizedKPSetRoleInstancesData* instanceData = destIndiInstanceDat.mInstanceItemData;
								if (instanceData && instanceData->mKnownInstance) {
									CRealizationIndividualInstanceItemReference destIndiRef = getInstanceItemReference(destIndiId, false);
									if (!isSameIndividualsMerged(destIndiRef)) {
										continueVisiting = visitor->visitRoleInstance(destIndiRef, this);
									}
								}
							}
						}

					}
				}


				return false;
			}












			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitRoles(CRoleInstantiatedItem* roleItem, CRoleRealizationRoleVisitor* visitor) {

				COptimizedKPSetRoleInstancesRedirectionItem* roleRedirectionItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleItem;
				COptimizedKPSetRoleInstancesItem* roleInstItem = roleRedirectionItem->getRedirectedItem();
				bool roleItemInversed = roleRedirectionItem->isInversed();
				bool continueVisiting = true;

				CRolePropertiesHierarchyNode* hierNode = nullptr;
				if (!roleItemInversed) {
					hierNode = roleInstItem->getRoleHierarchyNode();
				} else {
					hierNode = roleInstItem->getInverseRoleHierarchyNode();
				}
				if (hierNode) {
					QList<CRole*>* roleList = hierNode->getEquivalentRoleList();
					for (QList<CRole*>::const_iterator it = roleList->constBegin(), itEnd = roleList->constEnd(); it != itEnd && continueVisiting; ++it) {
						CRole* role(*it);
						continueVisiting = visitor->visitRole(role,this);
					}
				}
				return true;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitSourceIndividualRoles(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealizationInstantiatedVisitor* visitor) {
				// visit the outgoing roles for the given individual reference
				return visitSourceTargetIndividualRoles(indiRealItemRef, false, visitor);
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitTargetIndividualRoles(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealizationInstantiatedVisitor* visitor) {
				// visit the incoming roles for the given individual reference
				return visitSourceTargetIndividualRoles(indiRealItemRef, true, visitor);
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitSourceTargetIndividualRoles(const CRealizationIndividualInstanceItemReference& indiRealItemRef, bool target, CRoleRealizationInstantiatedVisitor* visitor) {
				QSet<CRoleInstantiatedItem*> visitRoleItemSet;
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)indiRealItemRef.getRealizationInstanceItem();
				bool continueVisiting = true;
				if (individualItem) {
					COptimizedKPSetRoleNeighbourInstancesHash* roleNeighInsHash = individualItem->getKnownPossibleRoleNeighboursInstancesHash();
					for (COptimizedKPSetRoleNeighbourInstancesHash::const_iterator it = roleNeighInsHash->constBegin(), itEnd = roleNeighInsHash->constEnd(); it != itEnd; ++it) {
						COptimizedKPSetRoleInstancesItem* roleItem(it.key());
						const COptimizedKPSetRoleNeighbourInstancesHashData& neighInstHashData(it.value());
						bool hasRoleNeighbour = false;
						bool hasInvRoleNeighbour = false;

						COptimizedKPSetRoleInstancesHash* neighbourHash = nullptr;
						COptimizedKPSetRoleInstancesHash* invNeighbourHash = nullptr;
						if (!target) {
							neighbourHash = neighInstHashData.mRoleSuccessorInstancesHash;
							invNeighbourHash = neighInstHashData.mRolePredecessorInstancesHash;
						} else {
							neighbourHash = neighInstHashData.mRolePredecessorInstancesHash;
							invNeighbourHash = neighInstHashData.mRoleSuccessorInstancesHash;
						}


						if (neighbourHash) {
							for (COptimizedKPSetRoleInstancesHash::const_iterator itNeigh = neighbourHash->constBegin(), itNeighEnd = neighbourHash->constEnd(); !hasRoleNeighbour && itNeigh != itNeighEnd; ++itNeigh) {
								const COptimizedKPSetRoleInstancesHashData& roleInstHashData(itNeigh.value());
								COptimizedKPSetRoleInstancesData* roleInstData = roleInstHashData.mInstanceItemData;
								if (roleInstData->mKnownInstance) {
									hasRoleNeighbour = true;
									visitRoleItemSet.insert(roleItem);
								}
							}
						}
						CRoleInstantiatedItem* invRoleRedirectedItem = roleItem->getInverseRoleRedirectedItem();
						if (invRoleRedirectedItem && invNeighbourHash && roleItem->getInverseRoleHierarchyNode()) {
							for (COptimizedKPSetRoleInstancesHash::const_iterator itNeigh = invNeighbourHash->constBegin(), itNeighEnd = invNeighbourHash->constEnd(); !hasInvRoleNeighbour && itNeigh != itNeighEnd; ++itNeigh) {
								const COptimizedKPSetRoleInstancesHashData& roleInstHashData(itNeigh.value());
								COptimizedKPSetRoleInstancesData* roleInstData = roleInstHashData.mInstanceItemData;
								if (roleInstData->mKnownInstance) {
									hasInvRoleNeighbour = true;
									visitRoleItemSet.insert(invRoleRedirectedItem);
								}
							}
						}
					}
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mBackendAssocCacheReader->getIndividualAssociationData(indiRealItemRef.getIndividualID());
				if (indiAssData) {
					CBackendRepresentativeMemoryLabelCacheItem* combinedNeigRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
					COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* combinedNeigRoleSetLabelData = getRepresentativeCacheCombinedNeighbourRoleSetLabelItemData(combinedNeigRoleSetLabel, false);
					if (combinedNeigRoleSetLabelData) {
						for (COptimizedKPSetRoleInstancesItem* roleItem : *combinedNeigRoleSetLabelData->getKnownInstancesItemSet(false)) {
							// inversed = target;
							if (target) {
								CRoleInstantiatedItem* invRoleRedirectedItem = roleItem->getInverseRoleRedirectedItem();
								if (invRoleRedirectedItem && roleItem->getInverseRoleHierarchyNode()) {
									visitRoleItemSet.insert(invRoleRedirectedItem);
								}
							} else {
								visitRoleItemSet.insert(roleItem);
							}
						}
						for (COptimizedKPSetRoleInstancesItem* roleItem : *combinedNeigRoleSetLabelData->getKnownInstancesItemSet(true)) {
							// inversed = !target;
							if (!target) {
								CRoleInstantiatedItem* invRoleRedirectedItem = roleItem->getInverseRoleRedirectedItem();
								if (invRoleRedirectedItem && roleItem->getInverseRoleHierarchyNode()) {
									visitRoleItemSet.insert(invRoleRedirectedItem);
								}
							} else {
								visitRoleItemSet.insert(roleItem);
							}
						}
					}




					CBackendRepresentativeMemoryLabelCacheItem* combinedDetExistRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
					CBackendRepresentativeMemoryLabelCacheItem* combinedNonDetExistRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
					

					COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData* combinedDetExistRoleSetLabelData = getRepresentativeCacheCombinedExistentialRoleSetLabelItemData(combinedDetExistRoleSetLabel, false);
					COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData* combinedNonDetExistRoleSetLabelData = getRepresentativeCacheCombinedExistentialRoleSetLabelItemData(combinedDetExistRoleSetLabel, false);

					if (combinedDetExistRoleSetLabelData && combinedDetExistRoleSetLabelData->hasComplexCandiateInstancesItems() || combinedNonDetExistRoleSetLabelData && combinedNonDetExistRoleSetLabelData->hasComplexCandiateInstancesItems() || combinedNeigRoleSetLabelData && combinedNeigRoleSetLabelData->hasComplexCandiateInstancesItems()) {
						QSet<COptimizedKPSetRoleInstancesItem*> complexRoleInstItemSet;
						QSet<COptimizedKPSetRoleInstancesItem*> inverseComplexRoleInstItemSet;
						if (combinedDetExistRoleSetLabelData) {
							complexRoleInstItemSet += (*combinedDetExistRoleSetLabelData->getComplexCandiateInstancesItemSet(false));
							inverseComplexRoleInstItemSet += (*combinedDetExistRoleSetLabelData->getComplexCandiateInstancesItemSet(true));
						}
						if (combinedNonDetExistRoleSetLabelData) {
							complexRoleInstItemSet += (*combinedNonDetExistRoleSetLabelData->getComplexCandiateInstancesItemSet(false));
							inverseComplexRoleInstItemSet += (*combinedNonDetExistRoleSetLabelData->getComplexCandiateInstancesItemSet(true));
						}
						if (combinedNeigRoleSetLabelData) {
							complexRoleInstItemSet += (*combinedNeigRoleSetLabelData->getComplexCandiateInstancesItemSet(false));
							inverseComplexRoleInstItemSet += (*combinedNeigRoleSetLabelData->getComplexCandiateInstancesItemSet(true));
						}

						// verify that these role have corresponding fillers
						function<bool(COptimizedKPSetRoleInstancesItem* complexRoleInstItem, bool handleInversed)> handleComplexRoleInstItemFunc = [&](COptimizedKPSetRoleInstancesItem* complexRoleInstItem, bool handleInversed)->bool {

							CRoleInstantiatedItem* visitRoleRedirectedItem = complexRoleInstItem;
							CRoleInstantiatedItem* invRoleRedirectedItem = complexRoleInstItem->getInverseRoleRedirectedItem();
							if (handleInversed) {
								if (invRoleRedirectedItem && complexRoleInstItem->getInverseRoleHierarchyNode()) {
									visitRoleRedirectedItem = invRoleRedirectedItem;
								} else {
									return false;
								}
							}


							if (!visitRoleItemSet.contains(visitRoleRedirectedItem)) {
								COptimizedKPSetIndividualComplexRoleData* indiComplexData = complexRoleInstItem->getIndividualIdComplexRoleData(indiRealItemRef.getIndividualID(), false);

								if (indiComplexData) {

									bool hasComplexInstance = false;
									COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;
									COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(handleInversed, false);
									if (possNeighbourInstanceHash) {
										// TODO: make more efficient by adding/maintaining/using pointer to first known neighbour filler/instance
										for (COptimizedKPSetRoleInstancesHash::const_iterator itNeighInst = possNeighbourInstanceHash->constBegin(), itNeighInstEnd = possNeighbourInstanceHash->constEnd(); itNeighInst != itNeighInstEnd && !hasComplexInstance; ++itNeighInst) {
											const COptimizedKPSetRoleInstancesHashData& destIndiInstanceDat = itNeighInst.value();
											cint64 destIndiId = itNeighInst.key();
											COptimizedKPSetRoleInstancesData* instanceData = destIndiInstanceDat.mInstanceItemData;
											if (instanceData && instanceData->mKnownInstance) {
												hasComplexInstance = true;
											}
										}
									}

									if (hasComplexInstance) {
										visitRoleItemSet.insert(visitRoleRedirectedItem);
									}
								}

								return true;
							}
							return false;
						};

						for (COptimizedKPSetRoleInstancesItem* complexRoleInstItem : complexRoleInstItemSet) {
							bool handleInversed = target;
							handleComplexRoleInstItemFunc(complexRoleInstItem, handleInversed);
						}
						for (COptimizedKPSetRoleInstancesItem* complexRoleInstItem : inverseComplexRoleInstItemSet) {
							bool handleInversed = !target;
							handleComplexRoleInstItemFunc(complexRoleInstItem, handleInversed);
						}
					}
				}


				for (QSet<CRoleInstantiatedItem*>::const_iterator it = visitRoleItemSet.constBegin(), itEnd = visitRoleItemSet.constEnd(); it != itEnd && continueVisiting; ++it) {
					CRoleInstantiatedItem* roleInstItem(*it);
					continueVisiting = visitor->visitRoleInstantiated(roleInstItem, this);
				}
				return true;
			}












			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitBetweeenIndividualRoles(const CRealizationIndividualInstanceItemReference& sourceIndiRealItemRef, const CRealizationIndividualInstanceItemReference& targetIndiRealItemRef, CRoleRealizationInstantiatedVisitor* visitor) {
				QSet<CRoleInstantiatedItem*> visitRoleItemSet;
				COptimizedKPSetIndividualItem* sourceIndiRealItem = (COptimizedKPSetIndividualItem*)sourceIndiRealItemRef.getRealizationInstanceItem();
				bool continueVisiting = true;
				if (sourceIndiRealItem) {
					COptimizedKPSetRoleNeighbourInstancesHash* roleNeighInsHash = sourceIndiRealItem->getKnownPossibleRoleNeighboursInstancesHash();
					for (COptimizedKPSetRoleNeighbourInstancesHash::const_iterator it = roleNeighInsHash->constBegin(), itEnd = roleNeighInsHash->constEnd(); it != itEnd; ++it) {
						COptimizedKPSetRoleInstancesItem* roleItem(it.key());
						const COptimizedKPSetRoleNeighbourInstancesHashData& neighInstHashData(it.value());

						COptimizedKPSetRoleInstancesHash* neighbourHash = nullptr;
						COptimizedKPSetRoleInstancesHash* invNeighbourHash = nullptr;
						neighbourHash = neighInstHashData.mRoleSuccessorInstancesHash;
						invNeighbourHash = neighInstHashData.mRolePredecessorInstancesHash;

						if (neighbourHash) {
							COptimizedKPSetRoleInstancesHashData hashData = neighbourHash->value(targetIndiRealItemRef.getIndividualID());
							if (hashData.mInstanceItemData && hashData.mInstanceItemData->mKnownInstance) {
								visitRoleItemSet.insert(roleItem);
							}
						}
						CRoleInstantiatedItem* invRoleRedirectedItem = roleItem->getInverseRoleRedirectedItem();
						if (invRoleRedirectedItem && roleItem->getInverseRoleHierarchyNode() && invNeighbourHash) {
							COptimizedKPSetRoleInstancesHashData hashData = invNeighbourHash->value(targetIndiRealItemRef.getIndividualID());
							if (hashData.mInstanceItemData && hashData.mInstanceItemData->mKnownInstance) {
								visitRoleItemSet.insert(invRoleRedirectedItem);
							}
						}
					}
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mBackendAssocCacheReader->getIndividualAssociationData(sourceIndiRealItemRef.getIndividualID());
				if (indiAssData) {
					CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetLabelHash = indiAssData->getNeighbourRoleSetHash();
					if (neighbourRoleSetLabelHash) {
						CBackendRepresentativeMemoryLabelCacheItem* singleNeigRoleSetLabel = neighbourRoleSetLabelHash->getNeighbourRoleSetLabel(targetIndiRealItemRef.getIndividualID());
						if (singleNeigRoleSetLabel) {

							COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData* singleNeigRoleSetLabelData = getRepresentativeCacheSingleNeighbourRoleSetLabelItemData(singleNeigRoleSetLabel, false);
							if (singleNeigRoleSetLabelData) {
								for (COptimizedKPSetRoleInstancesItem* roleItem : *singleNeigRoleSetLabelData->getKnownInstancesItemList(false)) {
									visitRoleItemSet.insert(roleItem);
								}
								for (COptimizedKPSetRoleInstancesItem* invRoleItem : *singleNeigRoleSetLabelData->getKnownInstancesItemList(true)) {
									CRoleInstantiatedItem* invRoleRedirectedItem = invRoleItem->getInverseRoleRedirectedItem();
									if (invRoleRedirectedItem && invRoleItem->getInverseRoleHierarchyNode()) {
										visitRoleItemSet.insert(invRoleRedirectedItem);
									}
								}
							}
						}
					}



					CBackendRepresentativeMemoryLabelCacheItem* combinedDetExistRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
					CBackendRepresentativeMemoryLabelCacheItem* combinedNonDetExistRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
					CBackendRepresentativeMemoryLabelCacheItem* combinedNeigRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);


					COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData* combinedDetExistRoleSetLabelData = getRepresentativeCacheCombinedExistentialRoleSetLabelItemData(combinedDetExistRoleSetLabel, false);
					COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData* combinedNonDetExistRoleSetLabelData = getRepresentativeCacheCombinedExistentialRoleSetLabelItemData(combinedDetExistRoleSetLabel, false);
					COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* combinedNeigRoleSetLabelData = getRepresentativeCacheCombinedNeighbourRoleSetLabelItemData(combinedNeigRoleSetLabel, false);

					if (combinedDetExistRoleSetLabelData && combinedDetExistRoleSetLabelData->hasComplexCandiateInstancesItems() || combinedNonDetExistRoleSetLabelData && combinedNonDetExistRoleSetLabelData->hasComplexCandiateInstancesItems() || combinedNeigRoleSetLabelData && combinedNeigRoleSetLabelData->hasComplexCandiateInstancesItems()) {
						QSet<COptimizedKPSetRoleInstancesItem*> complexRoleInstItemSet;
						QSet<COptimizedKPSetRoleInstancesItem*> inverseComplexRoleInstItemSet;
						if (combinedDetExistRoleSetLabelData) {
							complexRoleInstItemSet += (*combinedDetExistRoleSetLabelData->getComplexCandiateInstancesItemSet(false));
							inverseComplexRoleInstItemSet += (*combinedDetExistRoleSetLabelData->getComplexCandiateInstancesItemSet(true));
						}
						if (combinedNonDetExistRoleSetLabelData) {
							complexRoleInstItemSet += (*combinedNonDetExistRoleSetLabelData->getComplexCandiateInstancesItemSet(false));
							inverseComplexRoleInstItemSet += (*combinedNonDetExistRoleSetLabelData->getComplexCandiateInstancesItemSet(true));
						}
						if (combinedNeigRoleSetLabelData) {
							complexRoleInstItemSet += (*combinedNeigRoleSetLabelData->getComplexCandiateInstancesItemSet(false));
							inverseComplexRoleInstItemSet += (*combinedNeigRoleSetLabelData->getComplexCandiateInstancesItemSet(true));
						}


						// verify that these role have corresponding instances
						function<bool(COptimizedKPSetRoleInstancesItem* complexRoleInstItem, bool handleInversed)> handleComplexRoleInstItemFunc = [&](COptimizedKPSetRoleInstancesItem* complexRoleInstItem, bool handleInversed)->bool {

							CRoleInstantiatedItem* visitRoleRedirectedItem = complexRoleInstItem;
							CRoleInstantiatedItem* invRoleRedirectedItem = complexRoleInstItem->getInverseRoleRedirectedItem();
							if (handleInversed) {
								if (invRoleRedirectedItem && complexRoleInstItem->getInverseRoleHierarchyNode()) {
									visitRoleRedirectedItem = invRoleRedirectedItem;
								} else {
									return false;
								}
							}

							if (!visitRoleItemSet.contains(visitRoleRedirectedItem)) {
								COptimizedKPSetIndividualComplexRoleData* indiComplexData = complexRoleInstItem->getIndividualIdComplexRoleData(sourceIndiRealItemRef.getIndividualID(), false);

								if (indiComplexData) {

									COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;
									COptimizedKPSetRoleInstancesData* instanceData = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstanceItemData(handleInversed, targetIndiRealItemRef.getIndividualID(), false);

									if (instanceData && instanceData->mKnownInstance) {
										visitRoleItemSet.insert(visitRoleRedirectedItem);
									}
								}

								return true;
							}
							return false;
						};

						for (COptimizedKPSetRoleInstancesItem* complexRoleInstItem : complexRoleInstItemSet) {
							handleComplexRoleInstItemFunc(complexRoleInstItem, false);
						}
						for (COptimizedKPSetRoleInstancesItem* complexRoleInstItem : inverseComplexRoleInstItemSet) {
							handleComplexRoleInstItemFunc(complexRoleInstItem, true);
						}
					}
				}

				for (QSet<CRoleInstantiatedItem*>::const_iterator it = visitRoleItemSet.constBegin(), itEnd = visitRoleItemSet.constEnd(); it != itEnd && continueVisiting; ++it) {
					CRoleInstantiatedItem* roleInstItem(*it);
					continueVisiting = visitor->visitRoleInstantiated(roleInstItem, this);
				}
				return true;
			}










			bool COptimizedRepresentativeKPSetOntologyRealizingItem::visitIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealizationIndividualVisitor* visitor) {
				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)indiRealItemRef.getRealizationInstanceItem();
				bool continueVisiting = true;
				if (individualItem) {
					QSet<CIndividualReference>* individualSet = individualItem->getKnownSameIndividualSet();
					for (QSet<CIndividualReference>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd && continueVisiting; ++it) {
						CIndividualReference indiRef(*it);
						continueVisiting = visitor->visitIndividual(indiRef, this);
					}
				} else {
					CBackendRepresentativeMemoryLabelCacheItem* sameIndilabelItem = mBackendAssocCacheReader->getIndividualAssociatedCacheLabelItem(indiRealItemRef.getIndividualID(), CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
					if (sameIndilabelItem) {
						mBackendAssocCacheReader->visitIndividualIdsOfAssociatedIndividualSetLabel(nullptr, sameIndilabelItem, [&](cint64 indiId)->bool {
							continueVisiting = visitor->visitIndividual(indiId, this);
							return continueVisiting;
						});
					} else {
						continueVisiting = visitor->visitIndividual(indiRealItemRef, this);
					}
				}
				return true;
			}













			COptimizedKPSetConceptInstancesItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getTopInstancesItem() {
				return mTopConceptInstancesItem;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getBottomInstancesItem() {
				return mBottomConceptInstancesItem;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getTestingPossibleSameIndividualCount() {
				return mTestingPossibleSameIndividualCount;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getTestingPossibleConceptInstanceCount() {
				return mTestingPossibleConceptInstanceCount;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setTestingPossibleSameIndividualCount(cint64 testCount) {
				mTestingPossibleSameIndividualCount = testCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setTestingPossibleConceptInstanceCount(cint64 testCount) {
				mTestingPossibleConceptInstanceCount = testCount;
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incTestingPossibleSameIndividualCount(cint64 incCount) {
				mTestingPossibleSameIndividualCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decTestingPossibleSameIndividualCount(cint64 decCount) {
				mTestingPossibleSameIndividualCount -= decCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incTestingPossibleConceptInstanceCount(cint64 incCount) {
				mTestingPossibleConceptInstanceCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decTestingPossibleConceptInstanceCount(cint64 decCount) {
				mTestingPossibleConceptInstanceCount -= decCount;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasTestingPossibleSameIndividual() {
				return mTestingPossibleSameIndividualCount > 0;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasTestingPossibleConceptInstances() {
				return mTestingPossibleConceptInstanceCount > 0;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getTestingPossibleRoleInstanceCount() {
				return mTestingPossibleRoleInstanceCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setTestingPossibleRoleInstanceCount(cint64 testCount) {
				mTestingPossibleRoleInstanceCount = testCount;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasTestingPossibleRoleInstances() {
				return mTestingPossibleRoleInstanceCount > 0;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incTestingPossibleRoleInstanceCount(cint64 incCount) {
				mTestingPossibleRoleInstanceCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decTestingPossibleRoleInstanceCount(cint64 decCount) {
				mTestingPossibleRoleInstanceCount -= decCount;
				return this;
			}


			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getTestedPossibleRoleInstancesCount() {
				return mTestedPossibleRoleInstancesCount;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getOpenPossibleRoleInstancesCount() {
				return mOpenPossibleRoleInstancesCount;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getInitializedRoleInstancesCount() {
				return mInitializedRoleInstancesCount;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getRemaningInitalizingRoleInstancesCount() {
				return mRemainingInitalizingRoleInstancesCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incTestedPossibleRoleInstancesCount(cint64 incCount) {
				mTestedPossibleRoleInstancesCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incOpenPossibleRoleInstancesCount(cint64 incCount) {
				mOpenPossibleRoleInstancesCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decTestedPossibleRoleInstancesCount(cint64 decCount) {
				mTestedPossibleRoleInstancesCount -= decCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decOpenPossibleRoleInstancesCount(cint64 decCount) {
				mOpenPossibleRoleInstancesCount -= decCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incInitializedRoleInstancesCount(cint64 incCount) {
				mInitializedRoleInstancesCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decInitializedRoleInstancesCount(cint64 decCount) {
				mInitializedRoleInstancesCount -= decCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incRemaningInitalizingInstancesCount(cint64 incCount) {
				mRemainingInitalizingRoleInstancesCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decRemaningInitalizingInstancesCount(cint64 decCount) {
				mRemainingInitalizingRoleInstancesCount -= decCount;
				return this;
			}


			COptimizedKPSetRoleInstancesItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getTopRoleInstancesItem() {
				return mTopRoleInstancesItem;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedRepresentativeKPSetOntologyRealizingItem::getBottomRoleInstancesItem() {
				return mBottomRoleInstancesItem;
			}


			QHash<CConcept*,COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getMarkerConceptInstancesItemHash() {
				return &mMarkerConceptInstancesItemHash;
			}



			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addRoleCandidateInitalizingIndividualsItem(COptimizedKPSetIndividualItem* indiItem) {
				mRoleCandidateInitalizingIndividualsItemList.append(indiItem);
				return this;
			}

			QList<COptimizedKPSetIndividualItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getRoleCandidateInitalizingIndividualsItemList() {
				return &mRoleCandidateInitalizingIndividualsItemList;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isConceptRealizationInstalled() {
				return mConceptRealizationInstalled;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setConceptRealizationInstalled(bool installed) {
				mConceptRealizationInstalled = installed;;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isRoleRealizationInstalled() {
				return mRoleRealizationInstalled;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setRoleRealizationInstalled(bool installed) {
				mRoleRealizationInstalled = installed;
				return this;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isSameRealizationInstalled() {
				return mSameRealizationInstalled;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setSameRealizationInstalled(bool installed) {
				mSameRealizationInstalled = installed;;
				return this;
			}

			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getPropagatingRoleInstanceCandidatesCount() {
				return mPropagatingRoleInstanceCandidateCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setPropagatingRoleInstanceCandidatesCount(cint64 testCount) {
				mPropagatingRoleInstanceCandidateCount = testCount;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasPropagatingRoleInstanceCandidates() {
				return mPropagatingRoleInstanceCandidateCount > 0;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incPropagatingRoleInstanceCandidatesCount(cint64 incCount) {
				mPropagatingRoleInstanceCandidateCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decPropagatingRoleInstanceCandidatesCount(cint64 decCount) {
				mPropagatingRoleInstanceCandidateCount -= decCount;
				return this;
			}








			cint64 COptimizedRepresentativeKPSetOntologyRealizingItem::getTestingRoleInstanceCandidatesCount() {
				return mTestingRoleInstanceCandidateCount;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setTestingRoleInstanceCandidatesCount(cint64 testCount) {
				mTestingRoleInstanceCandidateCount = testCount;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasTestingRoleInstanceCandidates() {
				return mTestingRoleInstanceCandidateCount > 0;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::incTestingRoleInstanceCandidatesCount(cint64 incCount) {
				mTestingRoleInstanceCandidateCount += incCount;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::decTestingRoleInstanceCandidatesCount(cint64 decCount) {
				mTestingRoleInstanceCandidateCount -= decCount;
				return this;
			}








			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isCountingPossibleConceptInstancesTesting() {
				return mCountingPossibleConceptInstancesTesting;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isCountingPossibleRoleInstancesTesting() {
				return mCountingPossibleRoleInstancesTesting;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isCountingPossibleSameInstancesTesting() {
				return mCountingPossibleSameInstancesTesting;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setCountingPossibleConceptInstancesTesting(bool counting) {
				mCountingPossibleConceptInstancesTesting = counting;
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setCountingPossibleRoleInstancesTesting(bool counting) {
				mCountingPossibleRoleInstancesTesting = counting;
				return this;
			}
			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setCountingPossibleSameInstancesTesting(bool counting) {
				mCountingPossibleSameInstancesTesting = counting;
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setTemporarySameRealizationOntology(CConcreteOntology* tmpOntology) {
				mTempSameRealizationOntology = tmpOntology;
				return this;
			}

			CConcreteOntology* COptimizedRepresentativeKPSetOntologyRealizingItem::getTemporarySameRealizationOntology() {
				return mTempSameRealizationOntology;
			}


			QList<COptimizedKPSetIndividualItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getPossibleSameIndividualsItemList() {
				return &mPossibleSameIndiItemList;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addPossibleSameIndividualsItem(COptimizedKPSetIndividualItem* indiItem) {
				mPossibleSameIndiItemList.append(indiItem);
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::requiresIndividualDependenceTracking() {
				return mIndiDepTrackReq;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setIndividualDependenceTrackingRequired(bool indiDepTrackingRequired) {
				mIndiDepTrackReq = indiDepTrackingRequired;
				return this;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isExtraConsistencyTestingStepRequired() {
				return mExtraConsistencyTestingRequired;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setExtraConsistencyTestingStepRequired(bool required) {
				mExtraConsistencyTestingRequired = required;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isExtraConsistencyTesting() {
				return mExtraConsistencyTesting;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setExtraConsistencyTesting(bool testing) {
				mExtraConsistencyTesting = testing;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isExtraConsistencyTested() {
				return mExtraConsistencyTested;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setExtraConsistencyTested(bool tested) {
				mExtraConsistencyTested = tested;
				return this;
			}


			QList<COptimizedKPSetIndividualItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getInstantiatedItemList() {
				return &mInstantiatedItemContainer;
			}

			QTime* COptimizedRepresentativeKPSetOntologyRealizingItem::getInitializationTime() {
				return &mInitTime;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isAllSameIndividualsProcessing() {
				return mAllSameIndividualsProcessing;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setAllSameIndividualsProcessing(bool processing) {
				mAllSameIndividualsProcessing = processing;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isAllConceptInstancesProcessing() {
				return mAllConceptInstancesProcessing;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setAllConceptInstancesProcessing(bool processing) {
				mAllConceptInstancesProcessing = processing;
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::isAllRoleInstancesProcessing() {
				return mAllRoleInstancesProcessing;
			}

			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::setAllRoleInstancesProcessing(bool processing) {
				mAllRoleInstancesProcessing = processing;
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addEntailmentIndividualConceptInstanceTestingItem(CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem* queueItem) {
				mEntIndConInstTestList.append(queueItem);
				return this;
			}

			QList<CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getEntailmentIndividualConceptInstanceTestingItemList() {
				return &mEntIndConInstTestList;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasEntailmentIndividualConceptInstanceTestingItems() {
				return !mEntIndConInstTestList.isEmpty();
			}


			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addEntailmentIndividualsRoleInstanceTestingItem(CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem* queueItem) {
				mEntIndsRoleInstTestList.append(queueItem);
				return this;
			}

			QList<CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getEntailmentIndividualsRoleInstanceTestingItemList() {
				return &mEntIndsRoleInstTestList;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasEntailmentIndividualsRoleInstanceTestingItems() {
				return !mEntIndsRoleInstTestList.isEmpty();
			}




			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addEntailmentIndividualsSameInstanceTestingItem(CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem* queueItem) {
				mEntIndsSameInstTestList.append(queueItem);
				return this;
			}

			QList<CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getEntailmentIndividualsSameInstanceTestingItemList() {
				return &mEntIndsSameInstTestList;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingItem::hasEntailmentIndividualsSameInstanceTestingItems() {
				return !mEntIndsSameInstTestList.isEmpty();
			}




			QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getRepresentativeCacheConceptSetLabelItemDataHash() {
				return &mRepresentativeCacheConceptSetLabelItemDataHash;
			}

			COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* COptimizedRepresentativeKPSetOntologyRealizingItem::getRepresentativeCacheConceptSetLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool createIfNotExists) {
				COptimizedRepresentativeKPSetConceptSetCacheLabelItemData*& itemData = mRepresentativeCacheConceptSetLabelItemDataHash[labelItem];
				if (!itemData && createIfNotExists) {
					itemData = new COptimizedRepresentativeKPSetConceptSetCacheLabelItemData();
				}
				return itemData;
			}



			QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getRepresentativeCacheSingleNeighbourRoleSetLabelItemDataHash() {
				return &mRepresentativeCacheSingleNeighbourRoleSetLabelItemDataHash;
			}

			COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetOntologyRealizingItem::getRepresentativeCacheSingleNeighbourRoleSetLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool createIfNotExists) {
				COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData*& itemData = mRepresentativeCacheSingleNeighbourRoleSetLabelItemDataHash[labelItem];
				if (!itemData && createIfNotExists) {
					itemData = new COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData();
				}
				return itemData;
			}



			QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getRepresentativeCacheCombinedNeighbourRoleSetLabelItemDataHash() {
				return &mRepresentativeCacheCombinedNeighbourRoleSetLabelItemDataHash;
			}

			COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetOntologyRealizingItem::getRepresentativeCacheCombinedNeighbourRoleSetLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool createIfNotExists) {
				if (!labelItem) {
					return nullptr;
				}
				COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData*& itemData = mRepresentativeCacheCombinedNeighbourRoleSetLabelItemDataHash[labelItem];
				if (!itemData && createIfNotExists) {
					itemData = new COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData(labelItem);
				}
				return itemData;
			}



			QList<COptimizedKPSetRoleInstancesItem*> COptimizedRepresentativeKPSetOntologyRealizingItem::getComplexStarterCandidateRoleInstancesItems(CRole* role) {
				return mComplexStarterCandidateRoleInstancesItemHash.values(role);
			}

			QList<COptimizedKPSetRoleInstancesItem*> COptimizedRepresentativeKPSetOntologyRealizingItem::getInverseComplexStarterCandidateRoleInstancesItems(CRole* role) {
				return mInverseComplexStarterCandidateRoleInstancesItemHash.values(role);
			}
			


			QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getRepresentativeCacheCombinedExistentialRoleSetLabelItemDataHash() {
				return &mRepresentativeCacheCombinedExistentialRoleSetLabelItemDataHash;
			}

			COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetOntologyRealizingItem::getRepresentativeCacheCombinedExistentialRoleSetLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool createIfNotExists) {
				if (!labelItem) {
					return nullptr;
				}
				COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData*& itemData = mRepresentativeCacheCombinedExistentialRoleSetLabelItemDataHash[labelItem];
				if (!itemData && createIfNotExists) {
					itemData = new COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData(labelItem);
				}
				return itemData;
			}



			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addDataRoleCacheLabelItem(CRole* role, CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem, bool deterministic) {
				if (deterministic) {
					QSet<CBackendRepresentativeMemoryLabelCacheItem*>*& labelItemSet = mDataRoleDetCacheLabelItemSetHash[role];
					if (!labelItemSet) {
						labelItemSet = new QSet<CBackendRepresentativeMemoryLabelCacheItem *>();
					}
					labelItemSet->insert(cacheLabelItem);
				} else {
					QSet<CBackendRepresentativeMemoryLabelCacheItem*>*& labelItemSet = mDataRoleNonDetCacheLabelItemSetHash[role];
					if (!labelItemSet) {
						labelItemSet = new QSet<CBackendRepresentativeMemoryLabelCacheItem *>();
					}
					labelItemSet->insert(cacheLabelItem);
				}
				return this;
			}

			QHash<CRole*, QSet<CBackendRepresentativeMemoryLabelCacheItem*>* >* COptimizedRepresentativeKPSetOntologyRealizingItem::getDataRoleDeterministicCacheLabelItemSetHash() {
				return &mDataRoleDetCacheLabelItemSetHash;
			}

			QHash<CRole*, QSet<CBackendRepresentativeMemoryLabelCacheItem*>* >* COptimizedRepresentativeKPSetOntologyRealizingItem::getDataRoleNondeterministicCacheLabelItemSetHash() {
				return &mDataRoleNonDetCacheLabelItemSetHash;
			}

			QSet<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedRepresentativeKPSetOntologyRealizingItem::getDataRoleCacheLabelItemSet(CRole* role, bool deterministic) {
				if (deterministic) {
					return mDataRoleDetCacheLabelItemSetHash.value(role);
				} else {
					return mDataRoleNonDetCacheLabelItemSetHash.value(role);
				}
			}



			COptimizedRepresentativeKPSetOntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingItem::addDataRoleNondeterministicCacheLabelItems(CRole* role) {
				mDataRoleNondeterministicCacheLabelsSet.insert(role);
				return this;
			}


			CBackendRepresentativeMemoryCacheReader* COptimizedRepresentativeKPSetOntologyRealizingItem::getBackendAssociationCacheReader() {
				return mBackendAssocCacheReader;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
