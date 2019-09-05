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

#include "COptimizedRepresentativeKPSetOntologyRealizingThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetOntologyRealizingThread::COptimizedRepresentativeKPSetOntologyRealizingThread(CReasonerManager *reasonerManager) : CRealizerThread(reasonerManager),CLogDomain("::Konclude::Reasoner::Kernel::Realizer") {
				mTestedPossibleInstancesCount = 0;
				mOpenPossibleInstancesCount = 0;
				mRealizingCount = 0;

				mBackendAssocCache = nullptr;
				CBackendCache* backendCache = reasonerManager->getBackendAssociationCache();
				if (backendCache) {
					mBackendAssocCache = dynamic_cast<CBackendRepresentativeMemoryCache*>(backendCache);
				}
			}


			COptimizedRepresentativeKPSetOntologyRealizingThread::~COptimizedRepresentativeKPSetOntologyRealizingThread() {
			}


			COntologyRealizingItem* COptimizedRepresentativeKPSetOntologyRealizingThread::initializeOntologyRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader = nullptr;
				if (mBackendAssocCache) {
					backendAssocCacheReader = mBackendAssocCache->createOntologyFixedCacheReader(ontology->getOntologyID());
				}

				COptimizedRepresentativeKPSetOntologyRealizingItem* item = new COptimizedRepresentativeKPSetOntologyRealizingItem();
				item->initRequirementConfigRealizingItem(ontology,config, backendAssocCacheReader);

				if (mTestedPossibleInstancesCount == mOpenPossibleInstancesCount) {
					mTestedPossibleInstancesCount = 0;
					mOpenPossibleInstancesCount = 0;
					mRealStartTime.start();
				}
				initializeItems(item);
				++mRealizingCount;


				CRealization* realization = ontology->getRealization();
				realization->setConceptRealization(item);
				realization->setRoleRealization(item);
				realization->setSameRealization(item);


				return item;
			}

			void COptimizedRepresentativeKPSetOntologyRealizingThread::readCalculationConfig(CConfigurationBase *config) {
				if (config) {
					mConfPossibleInstanceConceptsUpfrontMerging = CConfigDataReader::readConfigBoolean(config, "Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.PossibleInstanceConceptsUpfrontMerging", true);
					mConfPossibleInstanceIndividualsAfterwardsMerging = CConfigDataReader::readConfigBoolean(config, "Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.PossibleInstanceIndividualsAfterwardsMerging", true);
					mConfPossibleInstanceIndividualsAfterwardsMergingOnlyWithSameRepresentativeLabel = CConfigDataReader::readConfigBoolean(config, "Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.PossibleInstanceIndividualsAfterwardsMergingOnlyWithSameRepresentativeLabel", false);
					mAlternateShufflePossibleConceptInstanceTestingItemsOnCalculations = CConfigDataReader::readConfigBoolean(config, "Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.AlternatePossibleConceptInstanceTestingItemsOnCalculations", true);
					mConfPossibleInstanceIndividualsAfterwardsMergingProvidingCount = CConfigDataReader::readConfigInteger(config, "Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.PossibleInstanceIndividualsAfterwardsMergingProvidingCount", -1);
					mConfPossibleInstanceIndividualsAfterwardsMergingMaximumAttemptCount = CConfigDataReader::readConfigInteger(config, "Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.PossibleInstanceIndividualsAfterwardsMergingMaximumAttemptCount", 3);


					bool configErrorFlag = false;
					cint64 processorCount = 1;
					QString processorCountString = CConfigDataReader::readConfigString(config,"Konclude.Calculation.ProcessorCount",QString(),&configErrorFlag);
					if (!configErrorFlag) {
						if (processorCountString == "AUTO") {
							processorCount = CThread::idealThreadCount();
						} else {
							qint64 convertedWorkerCount = processorCountString.toInt(&configErrorFlag);
							if (configErrorFlag) {
								processorCount = convertedWorkerCount;
							}
						}
					}

					bool mulConfigErrorFlag = false;
					cint64 multiplicator = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.MultipliedUnitsParallelTestingCalculationCount",1,&mulConfigErrorFlag);
					mConfMaxTestParallelCount = processorCount*multiplicator;
					bool maxConfigErrorFlag = false;
					cint64 maxParallel = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.MaximumParallelTestingCalculationCount",1,&maxConfigErrorFlag);
					if (!maxConfigErrorFlag) {
						if (!mulConfigErrorFlag) {
							mConfMaxTestParallelCount = qMin(mConfMaxTestParallelCount,maxParallel);
						} else {
							mConfMaxTestParallelCount = maxParallel;
						}
					}
				} else {
					mConfPossibleInstanceIndividualsAfterwardsMerging = true;
					mConfPossibleInstanceConceptsUpfrontMerging = true;
					mAlternateShufflePossibleConceptInstanceTestingItemsOnCalculations = true;
					mConfPossibleInstanceIndividualsAfterwardsMergingOnlyWithSameRepresentativeLabel = false;
					mConfPossibleInstanceIndividualsAfterwardsMergingProvidingCount = -1;
					mConfPossibleInstanceIndividualsAfterwardsMergingMaximumAttemptCount = 3;

					mConfMaxTestParallelCount = 1;
				}

			}

			bool COptimizedRepresentativeKPSetOntologyRealizingThread::initializeItems(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				if (!reqConfPreCompItem->hasItemsInitialized()) {
					reqConfPreCompItem->initConceptItemsFromHierarchy();
					reqConfPreCompItem->initRoleItemsFromHierarchy();
					reqConfPreCompItem->initComplexRolesStarterCandidates();

					readCalculationConfig(reqConfPreCompItem->getCalculationConfiguration());

					initializeRepresentativeConceptSetCacheLabelItems(reqConfPreCompItem);
					initializeRepresentativeRoleSetCacheLabelItems(reqConfPreCompItem);
					initializeIndividualProcessingKPSetsFromConsistencyData(reqConfPreCompItem);
					reqConfPreCompItem->setItemsInitialized(true);


					QList<COptimizedKPSetConceptInstancesItem*>* itemInstancesList = reqConfPreCompItem->getProcessingPossibleConceptInstancesItemList();
					for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = itemInstancesList->constBegin(), itEnd = itemInstancesList->constEnd(); it != itEnd; ++it) {
						COptimizedKPSetConceptInstancesItem* item(*it);
						if (item->hasAllSuccessorProcessedFlag() && !item->hasPossibleInstances() && !item->hasTestingPossibleInstances()) {
							updateParentItemsSuccessorProcessed(item);
						}
					}

					CRealizingTestingStep* ontProcStep = reqConfPreCompItem->getInitRealizeProcessingStep();
					ontProcStep->setStepFinished(true);
					ontProcStep->submitRequirementsUpdate();
					LOG(INFO, getDomain(), logTr("Realization items initialized for ontology."), this);


					//COptimizedKPSetRoleInstancesItem* testRoleInstancesItem = nullptr;
					//bool testRoleInversion = false;
					//CRBox* rbox = reqConfPreCompItem->getOntology()->getRBox();
					//for (cint64 i = 0; i < rbox->getRoleCount(); i++) {
					//	CRole* role = rbox->getRoleVector()->getData(i);
					//	if (role && CIRIName::getRecentIRIName(role->getPropertyNameLinker())== "http://ontology.dumontierlab.com/hasPart") {
					//		COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)reqConfPreCompItem->getRoleInstantiatedItem(role);
					//		if (redRoleItem) {
					//			COptimizedKPSetRoleInstancesItem* roleInstancesItem = redRoleItem->getRedirectedItem();
					//			bool itemInversed = redRoleItem->isInversed();
					//			testRoleInstancesItem = roleInstancesItem;

					//			if (CIRIName::getRecentIRIName(role->getPropertyNameLinker()) == "http://ontology.dumontierlab.com/hasPart" && !itemInversed) {
					//				testRoleInversion = true;
					//			}
					//			break;
					//		}
					//	}
					//}
					//QList<CRealizationIndividualInstanceItemReference> list;
					//CABox* abox = reqConfPreCompItem->getOntology()->getABox();
					//for (cint64 i = 0; i < abox->getIndividualCount(); i++) {
					//	CIndividual* indi = abox->getIndividualVector()->getData(i);
					//	if (indi && CIRIName::getRecentIRIName(indi->getIndividualNameLinker()).contains("http://ontology.dumontierlab.com/eswc-example-graph-3#x") && !CIRIName::getRecentIRIName(indi->getIndividualNameLinker()).contains("http://ontology.dumontierlab.com/eswc-example-graph-3#xx")) {
					//		CRealizationIndividualInstanceItemReference indiItemRef(indi, i, nullptr);
					//		list.append(indiItemRef);
					//	}
					//}
					//		
					//for (CRealizationIndividualInstanceItemReference itemRef : list) {
					//	COptimizedKPSetIndividualComplexRoleData* indiComplexSubRoleData = testRoleInstancesItem->getIndividualIdComplexRoleData(itemRef.getIndividualID(), true);
					//	collectTransitiveLinks(reqConfPreCompItem, testRoleInstancesItem, testRoleInversion, itemRef, indiComplexSubRoleData);
					//}

				}
				return true;
			}

			bool itemSortLessThan(COptimizedKPSetConceptInstancesItem* item1, COptimizedKPSetConceptInstancesItem* item2) {
				return item1->getHierarchyNode()->getPredecessorNodeSet()->count() > item2->getHierarchyNode()->getPredecessorNodeSet()->count();
			}









			bool COptimizedRepresentativeKPSetOntologyRealizingThread::initializeRepresentativeRoleSetCacheLabelItems(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {

				reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelCacheEntries(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, [&](CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleLabelCacheItem)->bool {

					if (combinedNeighbourRoleLabelCacheItem->getIndividualAssociationCount() > 0) {

						COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* combinedNeighbourRoleLabelCacheItemData = reqConfPreCompItem->getRepresentativeCacheCombinedNeighbourRoleSetLabelItemData(combinedNeighbourRoleLabelCacheItem);

						reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelsOfAssociatedNeigbourRoleSetCombinationLabel(nullptr, combinedNeighbourRoleLabelCacheItem, [&](CBackendRepresentativeMemoryLabelCacheItem* singleNeighbourRoleLabelCacheItem)->bool {
							COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData* singleNeighbourRoleLabelCacheItemData = reqConfPreCompItem->getRepresentativeCacheSingleNeighbourRoleSetLabelItemData(singleNeighbourRoleLabelCacheItem);
							singleNeighbourRoleLabelCacheItemData->addReferencingCombinedNeighbourRoleLabelDataItem(combinedNeighbourRoleLabelCacheItemData);
							return true;
						});
					}
					return true;
				});






				QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData*>* singleNeighbourRoleLabelCacheItemDataHash = reqConfPreCompItem->getRepresentativeCacheSingleNeighbourRoleSetLabelItemDataHash();
				for (auto it = singleNeighbourRoleLabelCacheItemDataHash->constBegin(), itEnd = singleNeighbourRoleLabelCacheItemDataHash->constEnd(); it != itEnd; ++it) {

					COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData* singleNeighbourRoleLabelCacheItemData = it.value();
					CBackendRepresentativeMemoryLabelCacheItem* singleNeighbourRoleLabelCacheItem = it.key();


					QSet<COptimizedKPSetRoleInstancesItem*> singleLabelItemSet;
					QSet<COptimizedKPSetRoleInstancesItem*> singleInverseLabelItemSet;

					reqConfPreCompItem->getBackendAssociationCacheReader()->visitRolesOfAssociatedNeigbourRoleSetLabel(nullptr, singleNeighbourRoleLabelCacheItem, [&](CRole* role, bool roleInversed, bool assertionLinkBase, bool nominalLinkBase, bool nondeterministic)->bool {


						COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)reqConfPreCompItem->getRoleInstantiatedItem(role);
						if (redRoleItem) {
							COptimizedKPSetRoleInstancesItem* roleInstancesItem = redRoleItem->getRedirectedItem();
							bool itemInversed = redRoleItem->isInversed();

							bool inversed = roleInversed ^ itemInversed;
							QSet<COptimizedKPSetRoleInstancesItem*>* singleRoleLabelItemSet = &singleLabelItemSet;
							if (inversed) {
								singleRoleLabelItemSet = &singleInverseLabelItemSet;
							}


							if (!singleRoleLabelItemSet->contains(roleInstancesItem)) {
								singleRoleLabelItemSet->insert(roleInstancesItem);


								if (!nondeterministic) {
									singleNeighbourRoleLabelCacheItemData->addKnownInstancesItem(roleInstancesItem, inversed);
								} else {
									singleNeighbourRoleLabelCacheItemData->addPossibleInstancesItem(roleInstancesItem, inversed);
								}

								COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* itemLabelDataHash = roleInstancesItem->getCombinedNeighbourCacheLabelItemDataHash(inversed);

								for (COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* combinedNeighbourRoleLabelCacheItemData : *singleNeighbourRoleLabelCacheItemData->getReferencingCombinedNeighbourRoleLabelItemDataList()) {
									CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleLabelCacheItem = combinedNeighbourRoleLabelCacheItemData->getCombinedNeighbourRoleLabelCacheItem();
									COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData*& roleItemLabelData = (*itemLabelDataHash)[combinedNeighbourRoleLabelCacheItem];

									if (!roleItemLabelData) {
										roleItemLabelData = new COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData();
									}
							
									CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* arrayIndex = (CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData*)combinedNeighbourRoleLabelCacheItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::INDIVIDUAL_NEIGHBOUR_ARRAY_INDEX);

									COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData* singleLabelData = new COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData(arrayIndex->getIndex(singleNeighbourRoleLabelCacheItem), singleNeighbourRoleLabelCacheItem);

									if (!nondeterministic) {
										roleItemLabelData->addKnownInstancesLabelItemData(singleLabelData);
										itemLabelDataHash->setKnownInstances(true);
										combinedNeighbourRoleLabelCacheItemData->addKnownInstancesItem(roleInstancesItem, inversed);
									} else {
										itemLabelDataHash->setPossibleInstances(true);
										combinedNeighbourRoleLabelCacheItemData->setPossibleInstances(true);
										roleItemLabelData->addPossibleInstancesLabelItemData(singleLabelData);
										combinedNeighbourRoleLabelCacheItemData->addPossibleInstancesItem(roleInstancesItem, inversed);
									}
								}

							}
						}


						if (!roleInversed) {
							for (COptimizedKPSetRoleInstancesItem* complexRoleItemCandidate : reqConfPreCompItem->getComplexStarterCandidateRoleInstancesItems(role)) {
								for (COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* combinedNeighbourRoleLabelCacheItemData : *singleNeighbourRoleLabelCacheItemData->getReferencingCombinedNeighbourRoleLabelItemDataList()) {
									combinedNeighbourRoleLabelCacheItemData->addComplexCandiateInstancesItem(complexRoleItemCandidate, false);
									complexRoleItemCandidate->addComplexStarterCandidateCombinedNeighbourRoleLabelCacheItem(false, combinedNeighbourRoleLabelCacheItemData->getCombinedNeighbourRoleLabelCacheItem());
									if (nondeterministic) {
										complexRoleItemCandidate->setOnlyDeterministicComplexRoleStarterCandidatesUsage(false);
									}
								}
							}
							for (COptimizedKPSetRoleInstancesItem* complexRoleItemCandidate : reqConfPreCompItem->getInverseComplexStarterCandidateRoleInstancesItems(role)) {
								for (COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* combinedNeighbourRoleLabelCacheItemData : *singleNeighbourRoleLabelCacheItemData->getReferencingCombinedNeighbourRoleLabelItemDataList()) {
									combinedNeighbourRoleLabelCacheItemData->addComplexCandiateInstancesItem(complexRoleItemCandidate, true);
									complexRoleItemCandidate->addComplexStarterCandidateCombinedNeighbourRoleLabelCacheItem(true, combinedNeighbourRoleLabelCacheItemData->getCombinedNeighbourRoleLabelCacheItem());
									if (nondeterministic) {
										complexRoleItemCandidate->setOnlyDeterministicComplexRoleStarterCandidatesUsage(false);
									}
								}
							}
						}


						return true;
					});

				}



				function<void(CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem, bool deterministic)> existentialCacheLabelItemHandlingFunction = [&](CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem, bool deterministic) {
					reqConfPreCompItem->getBackendAssociationCacheReader()->visitRolesOfAssociatedCompinationRoleSetLabel(nullptr, cacheLabelItem, [&](CRole* role, bool inversed)->bool {

						if (!inversed) {

							COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData* combinedExistentialRoleLabelCacheItemData = reqConfPreCompItem->getRepresentativeCacheCombinedExistentialRoleSetLabelItemData(cacheLabelItem, true);
							combinedExistentialRoleLabelCacheItemData->setNondeterministic(!deterministic);

							for (COptimizedKPSetRoleInstancesItem* complexRoleItemCandidate : reqConfPreCompItem->getComplexStarterCandidateRoleInstancesItems(role)) {
								combinedExistentialRoleLabelCacheItemData->addComplexCandiateInstancesItem(complexRoleItemCandidate, false);
								complexRoleItemCandidate->addComplexStarterCandidateCombinedExistentialRoleLabelCacheItem(false, combinedExistentialRoleLabelCacheItemData->getCombinedNeighbourRoleLabelCacheItem());
							}

							for (COptimizedKPSetRoleInstancesItem* complexRoleItemCandidate : reqConfPreCompItem->getInverseComplexStarterCandidateRoleInstancesItems(role)) {
								combinedExistentialRoleLabelCacheItemData->addComplexCandiateInstancesItem(complexRoleItemCandidate, true);
								complexRoleItemCandidate->addComplexStarterCandidateCombinedExistentialRoleLabelCacheItem(true, combinedExistentialRoleLabelCacheItemData->getCombinedNeighbourRoleLabelCacheItem());
							}


							COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)reqConfPreCompItem->getRoleInstantiatedItem(role);
							if (redRoleItem) {
								COptimizedKPSetRoleInstancesItem* roleInstancesItem = redRoleItem->getRedirectedItem();
								bool itemInversed = redRoleItem->isInversed();

								roleInstancesItem->addExistentialRoleLabelCacheItem(itemInversed, cacheLabelItem);
							}
						}



						return true;
					});

				};

				reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelCacheEntries(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, [&](CBackendRepresentativeMemoryLabelCacheItem* detExistentialRoleLabelCacheItem)->bool {
					existentialCacheLabelItemHandlingFunction(detExistentialRoleLabelCacheItem, true);
					return true;
				});
				reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelCacheEntries(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL, [&](CBackendRepresentativeMemoryLabelCacheItem* detExistentialRoleLabelCacheItem)->bool {
					existentialCacheLabelItemHandlingFunction(detExistentialRoleLabelCacheItem, false);
					return true;
				});





				function<void(CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem, bool deterministic)> dataCacheLabelItemHandlingFunction = [&](CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem, bool deterministic) {
					reqConfPreCompItem->getBackendAssociationCacheReader()->visitRolesOfAssociatedCompinationRoleSetLabel(nullptr, cacheLabelItem, [&](CRole* role, bool inversed)->bool {

						if (!inversed) {
							reqConfPreCompItem->addDataRoleCacheLabelItem(role, cacheLabelItem, deterministic);

							if (!deterministic) {
								reqConfPreCompItem->addDataRoleNondeterministicCacheLabelItems(role);
							}
						}
						
						return true;
					});

				};


				reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelCacheEntries(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, [&](CBackendRepresentativeMemoryLabelCacheItem* detDataRoleLabelCacheItem)->bool {
					dataCacheLabelItemHandlingFunction(detDataRoleLabelCacheItem, true);
					return true;
				});
				reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelCacheEntries(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL, [&](CBackendRepresentativeMemoryLabelCacheItem* detDataRoleLabelCacheItem)->bool {
					dataCacheLabelItemHandlingFunction(detDataRoleLabelCacheItem, false);
					return true;
				});


				return this;
			}







			bool COptimizedRepresentativeKPSetOntologyRealizingThread::initializeRepresentativeConceptSetCacheLabelItems(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				QHash<CConcept*, COptimizedKPSetConceptInstancesItem*>* conceptInstancesItemHash = reqConfPreCompItem->getConceptInstancesItemHash();


				QList<COptimizedKPSetConceptInstancesItem*> equivClassList;
				initializeEquivalentClassList(&equivClassList, reqConfPreCompItem);

				reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelCacheEntries(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, [&](CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem)->bool {

					if (labelCacheItem->getIndividualAssociationCount() > 0) {

						COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* labelCacheItemData = reqConfPreCompItem->getRepresentativeCacheConceptSetLabelItemData(labelCacheItem);

						QList<COptimizedKPSetConceptInstancesItem*>* knownInstancesItemList = labelCacheItemData->getKnownInstancesItemList();
						QList<COptimizedKPSetConceptInstancesItem*>* possibleInstancesItemList = labelCacheItemData->getPossibleInstancesItemList();
						QSet<COptimizedKPSetConceptInstancesItem*> singleLabelItemSet;
						bool hasPossibleConceptCandidates = false;
						bool hasPossibleConceptInstantiations = false;
						reqConfPreCompItem->getBackendAssociationCacheReader()->visitConceptsOfFullConceptSetLabel(labelCacheItem, [&](CConcept* concept, bool negated, bool deterministic)->bool {

							if (!negated && concept->hasClassName()) {
								COptimizedKPSetConceptInstancesItem* conceptInstancesItem = conceptInstancesItemHash->value(concept);
								if (conceptInstancesItem) {
									if (deterministic) {
										if (!singleLabelItemSet.contains(conceptInstancesItem)) {
											singleLabelItemSet.insert(conceptInstancesItem);
											conceptInstancesItem->addKnownInstancesLabelCacheItem(labelCacheItem);
											knownInstancesItemList->append(conceptInstancesItem);
										}
									} else {
										hasPossibleConceptCandidates = true;
									}
								}
							}
							if (!negated && concept->getOperatorCode() == CCEQCAND) {
								hasPossibleConceptCandidates = true;
							}

							return true;
						}, true, true);


						if (hasPossibleConceptCandidates) {
							reqConfPreCompItem->getBackendAssociationCacheReader()->visitConceptsOfFullConceptSetLabel(labelCacheItem, [&](CConcept* concept, bool negated, bool deterministic)->bool {

								if (!negated && concept->hasClassName() && !deterministic) {
									COptimizedKPSetConceptInstancesItem* conceptInstancesItem = conceptInstancesItemHash->value(concept);
									if (conceptInstancesItem && !singleLabelItemSet.contains(conceptInstancesItem)) {
										singleLabelItemSet.insert(conceptInstancesItem);
										hasPossibleConceptInstantiations = true;
										conceptInstancesItem->addPossibleInstancesLabelCacheItem(labelCacheItem);
										possibleInstancesItemList->append(conceptInstancesItem);
									}
								}
								if (!negated && concept->getOperatorCode() == CCEQCAND) {
									CConcept* resolvedConcept = concept->getOperandList()->getData();
									COptimizedKPSetConceptInstancesItem* conceptInstancesItem = conceptInstancesItemHash->value(resolvedConcept);
									if (conceptInstancesItem && !singleLabelItemSet.contains(conceptInstancesItem)) {
										singleLabelItemSet.insert(conceptInstancesItem);
										hasPossibleConceptInstantiations = true;
										conceptInstancesItem->addPossibleInstancesLabelCacheItem(labelCacheItem);
										possibleInstancesItemList->append(conceptInstancesItem);
									}
								}

								return true;
							}, true, true);
						}
						if (!equivClassList.isEmpty()) {
							hasPossibleConceptInstantiations = true;
							possibleInstancesItemList->append(equivClassList);
						}

						labelCacheItemData->setPossibleInstances(hasPossibleConceptInstantiations);
						qSort(knownInstancesItemList->begin(), knownInstancesItemList->end(), itemSortLessThan);
						qSort(possibleInstancesItemList->begin(), possibleInstancesItemList->end(), itemSortLessThan);


						for (COptimizedKPSetConceptInstancesItem* conceptInstancesItem : *knownInstancesItemList) {
							addRepresentativeCacheLabelItemToConceptInstancesItem(reqConfPreCompItem, labelCacheItem, labelCacheItemData, conceptInstancesItem, true);
						}



						for (COptimizedKPSetConceptInstancesItem* conceptInstancesItem : *possibleInstancesItemList) {
							addRepresentativeCacheLabelItemToConceptInstancesItem(reqConfPreCompItem, labelCacheItem, labelCacheItemData, conceptInstancesItem, false);
						}


						QHash<COptimizedKPSetConceptInstancesItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData*>* instancesItemDataHash = labelCacheItemData->getInstancesItemDataHash();
						for (auto it = instancesItemDataHash->constBegin(), itEnd = instancesItemDataHash->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetConceptInstancesItem* conceptInstancesItem = it.key();
							if (it.value()->hasMostSpecificKnownInstances() && it.value()->hasMostSpecificInstances()) {
								conceptInstancesItem->addKnownMostSpecificInstancesLabelCacheItem(labelCacheItem);
							} else if (it.value()->hasMostSpecificKnownInstances() && !it.value()->hasMostSpecificInstances()) {
								conceptInstancesItem->addPossibleMostSpecificInstancesLabelCacheItem(labelCacheItem);
							} else if (it.value()->hasPossibleInstances()) {
								conceptInstancesItem->addPossibleMostSpecificInstancesLabelCacheItem(labelCacheItem);
							}
						}
					}



					return true;
				});
				return true;
			}







			bool COptimizedRepresentativeKPSetOntologyRealizingThread::addRepresentativeCacheLabelItemToConceptInstancesItem(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* labelCacheItemData, COptimizedKPSetConceptInstancesItem* instancesItem, bool knownInstancesItem) {
				QHash<COptimizedKPSetConceptInstancesItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData*>* instancesItemDataHash = labelCacheItemData->getInstancesItemDataHash();
				QList<COptimizedKPSetConceptInstancesItem*> tmpInstancesList;
				COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData*& instancesItemData = (*instancesItemDataHash)[instancesItem];
				if (!instancesItemData) {
					instancesItemData = new COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData();
					instancesItemData->setMostSpecificInstances(true);
					instancesItemData->setPossibleInstances(!knownInstancesItem);
					if (knownInstancesItem) {
						instancesItemData->setMostSpecificKnownInstances(true);
					}

					tmpInstancesList.append(*instancesItem->getParentItemList());
					while (!tmpInstancesList.isEmpty()) {
						COptimizedKPSetConceptInstancesItem* superInstanceItem = tmpInstancesList.takeFirst();
						COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData*& superInstancesItemData = (*instancesItemDataHash)[superInstanceItem];
						if (!superInstancesItemData) {
							superInstancesItemData = new COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData();
							superInstancesItemData->setMostSpecificInstances(false);
							superInstancesItemData->setMostSpecificKnownInstances(false);
							superInstancesItemData->setPossibleInstances(!knownInstancesItem);
							tmpInstancesList.append(*superInstanceItem->getParentItemList());
						} else {
							if (knownInstancesItem && superInstancesItemData->hasMostSpecificKnownInstances()) {
								superInstancesItemData->setMostSpecificKnownInstances(false);
							}

							if (superInstancesItemData->hasMostSpecificInstances()) {
								superInstancesItemData->setMostSpecificInstances(false);
							}
						}
					}
				}
				return true;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingThread::initializeRepresentativeIndividualProcessing(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, cint64 indiId, CBackendRepresentativeMemoryLabelCacheItem* conSetLabelItem, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* conSetLabelCacheItemData, QHash<cint64, QList<CIndividualReference> >* individualPossibleSameIndividualListHash) {

				CIndividualVector* indiVec = reqConfPreCompItem->getOntology()->getABox()->getIndividualVector(false);

				// TODO: the known instance data may should not be represented in the individual item since this information can be resolved via the individual association data and, therefore, it would just waste some memory
				QList<COptimizedKPSetConceptInstancesItem*>* knownInstancesItemList = conSetLabelCacheItemData->getKnownInstancesItemList();
				QList<COptimizedKPSetConceptInstancesItem*>* possibleInstancesItemList = conSetLabelCacheItemData->getPossibleInstancesItemList();

				QList<CIndividualReference> knownSameIndividualIdList;
				CBackendRepresentativeMemoryLabelCacheItem* knownSameIndilabelItem = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociatedCacheLabelItem(indiId, CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
				if (knownSameIndilabelItem) {
					reqConfPreCompItem->getBackendAssociationCacheReader()->visitIndividualIdsOfAssociatedIndividualSetLabel(nullptr, knownSameIndilabelItem, [&](cint64 sameIndiId)->bool {
						if (sameIndiId > indiId) {
							CIndividual* sameIndividual = indiVec->getData(indiId);
							knownSameIndividualIdList.append(CIndividualReference(sameIndividual, indiId));
							indiId = sameIndiId;
						} else if (sameIndiId != indiId) {
							CIndividual* sameIndividual = indiVec->getData(sameIndiId);
							knownSameIndividualIdList.append(CIndividualReference(sameIndividual, sameIndiId));
						}
						return true;
					});
				}
				CBackendRepresentativeMemoryLabelCacheItem* possibleSameIndilabelItem = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociatedCacheLabelItem(indiId, CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
				if (possibleSameIndilabelItem) {
					reqConfPreCompItem->getBackendAssociationCacheReader()->visitIndividualIdsOfAssociatedIndividualSetLabel(nullptr, possibleSameIndilabelItem, [&](cint64 possibleSameIndiId)->bool {
						if (possibleSameIndiId != indiId && !reqConfPreCompItem->getBackendAssociationCacheReader()->hasIndividualIdsInAssociatedIndividualSetLabel(nullptr, knownSameIndilabelItem, possibleSameIndiId)) {
							CIndividual* possibleSameIndividual = indiVec->getData(possibleSameIndiId);
							(*individualPossibleSameIndividualListHash)[indiId].append(CIndividualReference(possibleSameIndividual, possibleSameIndiId));
						}
						return true;
					});
				}

				CIndividual* individual = indiVec->getData(indiId);
				initializeKPSetsForIndividual(reqConfPreCompItem, indiId, individual, *knownInstancesItemList, *possibleInstancesItemList, knownSameIndividualIdList);

				return true;
			}







			CIndividualProcessNode* COptimizedRepresentativeKPSetOntologyRealizingThread::getMergeCorrectedIndividualProcessNode(CIndividualProcessNode* indiProcNode, CIndividualProcessNodeVector* indiProcVector, bool* nonDeterministicallyMergedFlag, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				bool nonDeterministicMerged = false;
				while (indiProcNode && indiProcNode->getMergedIntoIndividualNodeID() != indiProcNode->getIndividualNodeID()) {
					cint64 mergeID = indiProcNode->getMergedIntoIndividualNodeID();
					CDependencyTrackPoint* mergedDepTrackPoint = indiProcNode->getMergedDependencyTrackPoint();
					indiProcNode = indiProcVector->getData(mergeID);
					if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
						nonDeterministicMerged = true;
					}
				}
				if (nonDeterministicallyMergedFlag) {
					*nonDeterministicallyMergedFlag = nonDeterministicMerged;
				}
				return indiProcNode;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingThread::addKPSetDirectSuperInstances(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash, COptimizedKPSetConceptInstancesItem* instanceItem, bool knownInstance) {
				QList<COptimizedKPSetConceptInstancesItem*> tmpInstancesList;
				COptimizedKPSetConceptInstancesHashData& hashData = (*knownPossibleInstancesHash)[instanceItem];
				if (hashData.mInstanceItemData == nullptr) {
					COptimizedKPSetConceptInstancesData* data = new COptimizedKPSetConceptInstancesData();
					hashData.mInstanceItemData = data;
					data->mInstanceItem = instanceItem;
					data->mKnownInstance = knownInstance;
					data->mPossibleInstance = !knownInstance;
					data->mMostSpecific = true;
					tmpInstancesList.append(*instanceItem->getParentItemList());
					while (!tmpInstancesList.isEmpty()) {
						COptimizedKPSetConceptInstancesItem* superInstanceItem = tmpInstancesList.takeFirst();
						COptimizedKPSetConceptInstancesHashData& superHashData = (*knownPossibleInstancesHash)[superInstanceItem];
						if (superHashData.mInstanceItemData == nullptr) {
							COptimizedKPSetConceptInstancesData* superData = new COptimizedKPSetConceptInstancesData();
							superHashData.mInstanceItemData = superData;
							superData->mInstanceItem = superInstanceItem;
							superData->mKnownInstance = knownInstance;
							superData->mPossibleInstance = !knownInstance;
							superData->mMostSpecific = false;
							tmpInstancesList.append(*superInstanceItem->getParentItemList());
						} else {
							if (knownInstance && superHashData.mInstanceItemData->mKnownInstance && superHashData.mInstanceItemData->mMostSpecific) {
								superHashData.mInstanceItemData->mMostSpecific = false;
							}
							if (!knownInstance && superHashData.mInstanceItemData->mPossibleInstance && superHashData.mInstanceItemData->mMostSpecific) {
								superHashData.mInstanceItemData->mMostSpecific = false;
							}
						}
					}
				}
				return true;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingThread::initializeKPSetsForIndividual(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, cint64 indiId, CIndividual* individual, QList<COptimizedKPSetConceptInstancesItem*>& knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>& possibleInstancesList, QList<CIndividualReference>& knownSameIndividualIdList) {
				COptimizedKPSetIndividualItem* instantiatedItem = reqConfPreCompItem->getIndividualInstantiatedItem(indiId, true, individual);
				qSort(knownInstancesList.begin(),knownInstancesList.end(),itemSortLessThan);
				qSort(possibleInstancesList.begin(),possibleInstancesList.end(),itemSortLessThan);

				COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = instantiatedItem->getKnownPossibleInstancesHash();
				for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = knownInstancesList.constBegin(), itEnd = knownInstancesList.constEnd(); it != itEnd; ++it) {
					COptimizedKPSetConceptInstancesItem* instanceItem(*it);
					addKPSetDirectSuperInstances(reqConfPreCompItem,knownPossibleInstancesHash,instanceItem,true);
				}

				for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = possibleInstancesList.constBegin(), itEnd = possibleInstancesList.constEnd(); it != itEnd; ++it) {
					COptimizedKPSetConceptInstancesItem* instanceItem(*it);
					addKPSetDirectSuperInstances(reqConfPreCompItem,knownPossibleInstancesHash,instanceItem,false);
				}

				for (COptimizedKPSetConceptInstancesHash::const_iterator it = knownPossibleInstancesHash->constBegin(), itEnd = knownPossibleInstancesHash->constEnd(); it != itEnd; ++it) {
					const COptimizedKPSetConceptInstancesHashData& hashData = it.value();
					if (hashData.mInstanceItemData->mMostSpecific) {
						COptimizedKPSetConceptInstancesItem* instanceItem = hashData.mInstanceItemData->mInstanceItem;
						if (hashData.mInstanceItemData->mKnownInstance) {
							instanceItem->addKnownInstance(instantiatedItem);
						} else {
							incOpenPossibleConceptInstancesCount(reqConfPreCompItem);
							instantiatedItem->incPossibleInstantiatedCount();
							instanceItem->addPossibleInstance(instantiatedItem);
						}
					}
				}
				if (!knownSameIndividualIdList.isEmpty()) {
					for (QList<CIndividualReference>::const_iterator it = knownSameIndividualIdList.constBegin(), itEnd = knownSameIndividualIdList.constEnd(); it != itEnd; ++it) {
						CIndividualReference sameIndividualReference(*it);
						instantiatedItem->addKnownSameIndividual(sameIndividualReference);
						reqConfPreCompItem->getIndividualInstantiatedItemHash()->insert(sameIndividualReference.getIndividualID(),instantiatedItem);
					}
				}

				return true;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingThread::initializeEquivalentClassList(QList<COptimizedKPSetConceptInstancesItem*>* equivClassList, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				CTBox* tBox = reqConfPreCompItem->getOntology()->getTBox();
				CBOXHASH<CConcept*,CConcept*>* eqCandHash = tBox->getEquivalentConceptCandidateHash(false);
				QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*>* hierItemHash = reqConfPreCompItem->getHierarchyNodeInstancesItemHash();
				QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* conInstItemHash = reqConfPreCompItem->getConceptInstancesItemHash();
				for (QHash<CHierarchyNode*,COptimizedKPSetConceptInstancesItem*>::const_iterator it = hierItemHash->constBegin(), itEnd = hierItemHash->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetConceptInstancesItem* item = it.value();
					CHierarchyNode* hierNode = item->getHierarchyNode();
					QList<CConcept*>* eqConList = hierNode->getEquivalentConceptList();
					bool hasEqConCand = false;
					for (QList<CConcept*>::const_iterator itCon = eqConList->constBegin(), itConEnd = eqConList->constEnd(); !hasEqConCand && itCon != itConEnd; ++itCon) {
						CConcept* eqConcept(*itCon);
						if (eqConcept->getOperatorCode() == CCSUB || eqConcept->getOperatorCode() == CCATOM || eqConcept->getOperatorCode() == CCTOP || eqConcept->getOperatorCode() == CCBOTTOM) {
							hasEqConCand = true;
						} else if (eqConcept->getOperatorCode() == CCEQ) {
							if (eqCandHash->contains(eqConcept)) {
								hasEqConCand = true;
							}
						}
					}
					if (!hasEqConCand) {
						COptimizedKPSetConceptInstancesItem* instantiatedItem = conInstItemHash->value(eqConList->first());
						equivClassList->append(instantiatedItem);
					}
				}
				return true;
			}








			bool COptimizedRepresentativeKPSetOntologyRealizingThread::extractKnownPossibleIndividualDataFromConsistencyData(CIndividualProcessNode* indiProcNode, QList<COptimizedKPSetConceptInstancesItem*>* knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>* possibleInstancesList, QList<CIndividualReference>* knownSameIndividualList, QList<CIndividualReference>* possibleSameIndividualList, CIndividualProcessNodeVector* indiProcVector, const QList<COptimizedKPSetConceptInstancesItem*>& equivClassList, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* conInstItemHash = reqConfPreCompItem->getConceptInstancesItemHash();

				bool indiInitialized = false;
				if (indiProcVector) {

					CIndividualProcessNode* baseIndiProcNode = indiProcNode;
					if (indiProcNode) {
						CIndividual* individual = indiProcNode->getNominalIndividual();
						CIndividualVector* indiVec = reqConfPreCompItem->getOntology()->getABox()->getIndividualVector();
						bool nondeterministicallyMergedFlag = false;
						indiProcNode = getMergeCorrectedIndividualProcessNode(indiProcNode,indiProcVector,&nondeterministicallyMergedFlag,reqConfPreCompItem);


						CReapplyConceptLabelSet* conLabelSet = indiProcNode->getReapplyConceptLabelSet(false);
						if (conLabelSet) {

							CConceptDescriptor* conDesLinker = conLabelSet->getAddingSortedConceptDescriptionLinker();
							while (conDesLinker) {
								CConcept* concept = conDesLinker->getConcept();
								bool negated = conDesLinker->getNegation();
								CDependencyTrackPoint* depTrackPoint = conDesLinker->getDependencyTrackPoint();
								bool deterministicTrackPoint = false;
								if (depTrackPoint) {
									if (depTrackPoint->getBranchingTag() <= 0) {
										deterministicTrackPoint = true;
									}
								}
								if (!negated) {
									CConcept* resolvedConcept = concept;
									bool resolved = false;
									CConceptOperator* conOperator = concept->getConceptOperator();
									if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_ATOM | CConceptOperator::CCF_SUB | CConceptOperator::CCF_TOP | CConceptOperator::CCF_EQ | CConceptOperator::CCF_EQCAND)) {
										cint64 opCode = conOperator->getOperatorCode();
										if (opCode == CCEQCAND) {
											resolvedConcept = concept->getOperandList()->getData();
											resolved = true;
										}
										//if (CIRIName::getRecentIRIName(resolvedConcept->getClassNameLinker()) != "http://uob.iodt.ibm.com/univ-bench-dl.owl#PeopleWithManyHobbies") {
											COptimizedKPSetConceptInstancesItem* instantiatedItem = conInstItemHash->value(resolvedConcept);
											if (instantiatedItem) {
												if (deterministicTrackPoint && !resolved) {
													knownInstancesList->append(instantiatedItem);
												}
												else {
													possibleInstancesList->append(instantiatedItem);
												}
											}
										//}
									}
								}

								conDesLinker = conDesLinker->getNextConceptDesciptor();
							}
						}


						CIndividualMergingHash* indiMergingHash = indiProcNode->getIndividualMergingHash(false);
						if (indiMergingHash) {
							for (CIndividualMergingHash::const_iterator it = indiMergingHash->constBegin(), itEnd = indiMergingHash->constEnd(); it != itEnd; ++it) {
								if (it.value().isMergedWithIndividual()) {
									cint64 mergedIndiId = it.key();
									CIndividual* mergedIndi = indiVec->getData(mergedIndiId);
									CDependencyTrackPoint* mergedDepTrackPoint = it.value().getDependencyTrackPoint();
									bool nondeterministicallyMerged = false;
									if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
										nondeterministicallyMerged = true;
									}
									if (mergedIndi != individual && !mergedIndi->isAnonymousIndividual()) {
										if (nondeterministicallyMerged) {
											possibleSameIndividualList->append(mergedIndi);
										} else {
											knownSameIndividualList->append(mergedIndi);
										}
									}
								}
							}
						}
						if (indiProcNode != baseIndiProcNode) {
							if (nondeterministicallyMergedFlag) {
								possibleSameIndividualList->append(indiProcNode->getNominalIndividual());
							} else {
								knownSameIndividualList->append(indiProcNode->getNominalIndividual());
							}
						}

						if (nondeterministicallyMergedFlag) {
							possibleInstancesList->append(*knownInstancesList);
							knownInstancesList->clear();
							possibleSameIndividualList->append(*knownSameIndividualList);
							knownSameIndividualList->clear();

							QSet<CConcept*> knownConceptSet;
							QSet<CIndividualReference> knownSameIndividualSet;

							CIndividualProcessNode* detIndiProcNode = baseIndiProcNode;
							bool stillDeterministicllyMerged = true;
							while (stillDeterministicllyMerged && detIndiProcNode) {

								CReapplyConceptLabelSet* detConLabelSet = detIndiProcNode->getReapplyConceptLabelSet(false);
								if (detConLabelSet) {


									CIndividualMergingHash* detIndiMergingHash = detIndiProcNode->getIndividualMergingHash(false);
									if (detIndiMergingHash) {
										for (CIndividualMergingHash::const_iterator it = detIndiMergingHash->constBegin(), itEnd = detIndiMergingHash->constEnd(); it != itEnd; ++it) {
											if (it.value().isMergedWithIndividual()) {
												cint64 mergedIndiId = it.key();
												CIndividual* mergedIndi = indiVec->getData(mergedIndiId);
												CDependencyTrackPoint* mergedDepTrackPoint = it.value().getDependencyTrackPoint();
												bool nondeterministicallyMerged = false;
												if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
													nondeterministicallyMerged = true;
												}
												if (mergedIndi != individual && !mergedIndi->isAnonymousIndividual()) {
													if (!nondeterministicallyMerged) {
														knownSameIndividualSet.insert(mergedIndi);
													}
												}
											}
										}													
									}

									if (stillDeterministicllyMerged) {
										CConceptDescriptor* conDesLinker = detConLabelSet->getAddingSortedConceptDescriptionLinker();
										for (CConceptDescriptor* conDesLinkerIt = conDesLinker; conDesLinkerIt; conDesLinkerIt = conDesLinkerIt->getNext()) {
											CConcept* concept = conDesLinkerIt->getConcept();
											bool negated = conDesLinkerIt->getNegation();
											CDependencyTrackPoint* depTrackPoint = conDesLinkerIt->getDependencyTrackPoint();
											bool deterministicTrackPoint = false;
											if (depTrackPoint) {
												if (depTrackPoint->getBranchingTag() <= 0) {
													deterministicTrackPoint = true;
												}
											}
											if (!negated) {
												CConceptOperator* conOperator = concept->getConceptOperator();
												if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_ATOM | CConceptOperator::CCF_SUB | CConceptOperator::CCF_TOP | CConceptOperator::CCF_EQ)) {
													cint64 opCode = conOperator->getOperatorCode();
													if (deterministicTrackPoint) {
														knownConceptSet.insert(concept);
													}
												}
											}
										}
									}
								}

								if (stillDeterministicllyMerged) {
									detIndiProcNode = indiProcVector->getData(detIndiProcNode->getMergedIntoIndividualNodeID());
									CDependencyTrackPoint* mergedDepTrackPoint = detIndiProcNode->getMergedDependencyTrackPoint();
									if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
										stillDeterministicllyMerged = false;
									}
								}
							}

							for (QSet<CConcept*>::const_iterator it = knownConceptSet.constBegin(), itEnd = knownConceptSet.constEnd(); it != itEnd; ++it) {
								CConcept* concept(*it);
								COptimizedKPSetConceptInstancesItem* instantiatedItem = conInstItemHash->value(concept);
								knownInstancesList->append(instantiatedItem);
							}
							for (QSet<CIndividualReference>::const_iterator it = knownSameIndividualSet.constBegin(), itEnd = knownSameIndividualSet.constEnd(); it != itEnd; ++it) {
								CIndividualReference sameIndividual(*it);
								knownSameIndividualList->append(sameIndividual);
							}

							QList<CIndividualReference> tmpPossibleSameIndividualList(*possibleSameIndividualList);
							possibleSameIndividualList->clear();
							while (!tmpPossibleSameIndividualList.isEmpty()) {
								CIndividualReference possSameIndividual = tmpPossibleSameIndividualList.takeFirst();
								if (!knownSameIndividualSet.contains(possSameIndividual)) {
									possibleSameIndividualList->append(possSameIndividual);
								}
							}
						}
						indiInitialized = true;
					}
				}

				if (!equivClassList.isEmpty()) {
					possibleInstancesList->append(equivClassList);
				}

				return indiInitialized;
			}









			bool COptimizedRepresentativeKPSetOntologyRealizingThread::initializeIndividualProcessingKPSetsFromConsistencyData(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {


				CIndividualVector* indiVec = reqConfPreCompItem->getOntology()->getABox()->getIndividualVector(false);
				if (indiVec) {
					CIndividualProcessNodeVector* indiProcVector = nullptr;
					CConsistence* consistence = reqConfPreCompItem->getOntology()->getConsistence();
					if (consistence) {
						CConsistenceData* consData = consistence->getConsistenceModelData();
						if (consData) {
							CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
							CSatisfiableCalculationTask* satConsTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
							if (!satConsTask) {
								satConsTask = consTaskData->getDeterministicSatisfiableTask();
							}
							if (satConsTask) {
								CProcessingDataBox* procDataBox = satConsTask->getProcessingDataBox();
								if (procDataBox) {
									indiProcVector = procDataBox->getIndividualProcessNodeVector();
								}
							}
						}
					}


					QList<COptimizedKPSetConceptInstancesItem*> equivClassList;
					initializeEquivalentClassList(&equivClassList,reqConfPreCompItem);

					QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* conInstItemHash = reqConfPreCompItem->getConceptInstancesItemHash();

					QHash<cint64,QList<CIndividualReference> > individualPossibleSameIndividualListHash;

					CBOXSET<CIndividual*>* activeIndividualSet = reqConfPreCompItem->getOntology()->getABox()->getActiveIndividualSet(false);

					if (indiProcVector) {
						for (cint64 indiNodeID = indiProcVector->getItemMinIndex(); indiNodeID <= 0; ++indiNodeID) {

							CIndividualProcessNode* indiProcNode = indiProcVector->getData(indiNodeID);
							if (indiProcNode) {
								CIndividual* individual = indiProcNode->getNominalIndividual();
								if (individual && !individual->isAnonymousIndividual() && !reqConfPreCompItem->hasIndividualInstantiatedItem(individual) && activeIndividualSet->contains(individual)) {

									QList<COptimizedKPSetConceptInstancesItem*> knownInstancesList;
									QList<COptimizedKPSetConceptInstancesItem*> possibleInstancesList;

									QList<CIndividualReference> knownSameIndividualList;
									QList<CIndividualReference> possibleSameIndividualList;

									extractKnownPossibleIndividualDataFromConsistencyData(indiProcNode, &knownInstancesList, &possibleInstancesList, &knownSameIndividualList, &possibleSameIndividualList, indiProcVector, equivClassList, reqConfPreCompItem);

									initializeKPSetsForIndividual(reqConfPreCompItem, individual->getIndividualID(), individual, knownInstancesList, possibleInstancesList, knownSameIndividualList);

									if (!possibleSameIndividualList.isEmpty()) {
										individualPossibleSameIndividualListHash.insert(individual->getIndividualID(), possibleSameIndividualList);
									}
								}
							}
						}
					}


					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData*>* repCacheConSetLabelItemDataHash = reqConfPreCompItem->getRepresentativeCacheConceptSetLabelItemDataHash();
					for (QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetConceptSetCacheLabelItemData*>::const_iterator it = repCacheConSetLabelItemDataHash->constBegin(), itEnd = repCacheConSetLabelItemDataHash->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryLabelCacheItem* repConSetCacheLabelItem = it.key();
						COptimizedRepresentativeKPSetConceptSetCacheLabelItemData* repConSetCacheLabelItemData = it.value();
						if (repConSetCacheLabelItemData->hasPossibleInstances()) {
							reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelItemIndividualIdAssociations(repConSetCacheLabelItem, [&](cint64 indiId, bool sameIndividualMerged)->bool {
								if (!sameIndividualMerged) {
									initializeRepresentativeIndividualProcessing(reqConfPreCompItem, indiId, repConSetCacheLabelItem, repConSetCacheLabelItemData, &individualPossibleSameIndividualListHash);
								}
								return true;
							}, true, true, false);
						}
					}



					CRole* topPropertyRole = reqConfPreCompItem->getOntology()->getRBox()->getTopObjectRole();

					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData*>* repCacheCombNeighRoleSetLabelItemDataHash = reqConfPreCompItem->getRepresentativeCacheCombinedNeighbourRoleSetLabelItemDataHash();
					for (QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData*>::const_iterator it = repCacheCombNeighRoleSetLabelItemDataHash->constBegin(), itEnd = repCacheCombNeighRoleSetLabelItemDataHash->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryLabelCacheItem* repCombNeighRoleSetCacheLabelItem = it.key();
						COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* repCombNeighRoleSetCacheLabelItemData = it.value();
						if (repCombNeighRoleSetCacheLabelItemData->hasPossibleInstances()) {

							for (COptimizedKPSetRoleInstancesItem* roleInstItem : *repCombNeighRoleSetCacheLabelItemData->getPossibleInstancesItemSet(false)) {
								COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* combinedNeigRoleSetLabelDataHash = roleInstItem->getCombinedNeighbourCacheLabelItemDataHash(false);
								if (roleInstItem->getRole() != topPropertyRole && combinedNeigRoleSetLabelDataHash->hasPossibleInstances()) {
									COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* combinedNeigRoleSetLabelData = combinedNeigRoleSetLabelDataHash->value(repCombNeighRoleSetCacheLabelItem);
									if (combinedNeigRoleSetLabelData && combinedNeigRoleSetLabelData->hasPossibleInstancesLabelItems()) {


										reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelItemIndividualIdAssociations(repCombNeighRoleSetCacheLabelItem, [&](cint64 indiId, bool sameIndividualMerged)->bool {
											if (!sameIndividualMerged) {
												

												CIndividualVector* indiVec = reqConfPreCompItem->getOntology()->getABox()->getIndividualVector(false);

												COptimizedKPSetIndividualItem* individualItem = reqConfPreCompItem->getIndividualInstantiatedItem(indiId, true, indiVec->getData(indiId));
												COptimizedKPSetRoleNeighbourInstancesHash* roleNeighbourInstanceHash = individualItem->getKnownPossibleRoleNeighboursInstancesHash();
												COptimizedKPSetRoleNeighbourInstancesHashData& roleNeighbourInstanceHashData = (*roleNeighbourInstanceHash)[roleInstItem];

												roleNeighbourInstanceHashData.mRoleSuccessorInstancesInitializing = true;
												roleNeighbourInstanceHashData.mRoleSuccessorInstancesInitialized = true;


												CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(indiId);
												if (indiAssData) {
													CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighbourRoleArray = indiAssData->getRoleSetNeighbourArray();

													for (COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData* data : *combinedNeigRoleSetLabelData->getPossibleInstancesLabelItemDataHash()) {
														neighbourRoleArray->at(data->getLabelArrayIndex()).visitNeighbourIndividualIds([&](cint64 neighbourIndiId)->bool {

															COptimizedKPSetRoleInstancesHash* roleNeighbourSuccessorHash = roleNeighbourInstanceHashData.getRoleSuccessorInstancesHash(true);
															COptimizedKPSetIndividualItem* precIndiItem = reqConfPreCompItem->getIndividualInstantiatedItem(neighbourIndiId, true, indiVec->getData(neighbourIndiId));
															if (precIndiItem->getIndividualId() == neighbourIndiId) {
																COptimizedKPSetRoleInstancesHashData& roleNeighbourSuccessorHashData = (*roleNeighbourSuccessorHash)[neighbourIndiId];

																if (!roleNeighbourSuccessorHashData.mInstanceItemData) {

																	COptimizedKPSetRoleNeighbourInstancesHashData& precRoleNeighbourData = precIndiItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](roleInstItem);
																	COptimizedKPSetRoleInstancesHash* precNeighbourHash = precRoleNeighbourData.getRolePredecessorInstancesHash(true);
																	COptimizedKPSetRoleInstancesHashData& precHashData = (*precNeighbourHash)[indiId];

																	// only correct as long as all individuals are initialized
																	precRoleNeighbourData.mRolePredecessorInstancesInitializing = true;
																	precRoleNeighbourData.mRolePredecessorInstancesInitialized = true;


																	COptimizedKPSetRoleInstancesData* instanceData = new COptimizedKPSetRoleInstancesData();
																	instanceData->mPossibleInstance = true;
																	roleNeighbourSuccessorHashData.mInstanceItemData = instanceData;
																	precHashData.mInstanceItemData = instanceData;
																	precHashData.mNeighbourIndividualItem = individualItem;
																	roleNeighbourSuccessorHashData.mNeighbourIndividualItem = precIndiItem;


																	roleInstItem->addPossibleInstance(COptimizedKPSetIndividualItemPair(individualItem, precIndiItem), false);
																	individualItem->incPossibleRoleInstantiatedCount();
																	precIndiItem->incPossibleRoleInstantiatedCount();
																	//if (roleInstItem->hasToProcessPossibleInstancesFlag()) {
																	//	reqConfPreCompItem->addRoleInstanceItemToProcessPossibleInstances(roleInstItem);
																	//}
																}
															}


															return true;
														});
													}
												}


											}
											return true;
										}, true, true, false);

									}
								}
							}


						}
					}



					CIndividualVector* indiVec = reqConfPreCompItem->getOntology()->getABox()->getIndividualVector(false);

					function<void(COptimizedKPSetRoleInstancesItem* complexRoleInstItem, QSet<CBackendRepresentativeMemoryLabelCacheItem*>* labelCacheItemSet, QHash<cint64, COptimizedKPSetIndividualComplexRoleData*>* indiComplexRoleDataHash, QSet<CBackendRepresentativeMemoryLabelCacheItem*>& initLabelCacheItemSet)> individualComplexRoleInitHandlingFunction = [&](COptimizedKPSetRoleInstancesItem* complexRoleInstItem, QSet<CBackendRepresentativeMemoryLabelCacheItem*>* labelCacheItemSet, QHash<cint64, COptimizedKPSetIndividualComplexRoleData*>* indiComplexRoleDataHash, QSet<CBackendRepresentativeMemoryLabelCacheItem*>& initLabelCacheItemSet) {
						for (CBackendRepresentativeMemoryLabelCacheItem* candLabelCacheItem : *labelCacheItemSet) {
							if (!initLabelCacheItemSet.contains(candLabelCacheItem)) {
								initLabelCacheItemSet.insert(candLabelCacheItem);
								reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelItemIndividualIdAssociations(candLabelCacheItem, [&](cint64 indiId, bool sameIndividualMerged)->bool {
									indiComplexRoleDataHash->insert(indiId, nullptr);
									return true;
								});
							}
						}
					};


					for (COptimizedKPSetRoleInstancesItem* complexRoleInstItem : *reqConfPreCompItem->getComplexRoleInstancesItemList()) {

						QSet<CBackendRepresentativeMemoryLabelCacheItem*> initLabelCacheItemSet;

						QHash<cint64, COptimizedKPSetIndividualComplexRoleData*>* indiComplexRoleDataHash = complexRoleInstItem->getIndividualIdComplexRoleDataHash();

						QSet<CBackendRepresentativeMemoryLabelCacheItem*>* existentialLabelCacheItemSet = complexRoleInstItem->getComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet(false);
						QSet<CBackendRepresentativeMemoryLabelCacheItem*>* existentialInversedLabelCacheItemSet = complexRoleInstItem->getComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet(true);

						QSet<CBackendRepresentativeMemoryLabelCacheItem*>* neighbourLabelCacheItemSet = complexRoleInstItem->getComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet(false);
						QSet<CBackendRepresentativeMemoryLabelCacheItem*>* neighbourInversedLabelCacheItemSet = complexRoleInstItem->getComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet(true);

						bool initComplexInstanceCandidateData = false;
						if (existentialLabelCacheItemSet->isEmpty() && existentialInversedLabelCacheItemSet->isEmpty() && complexRoleInstItem->hasOnlyTransitiveComplexRoleCandidates()) {

							if (neighbourLabelCacheItemSet->intersects(*neighbourInversedLabelCacheItemSet)) {
								initComplexInstanceCandidateData = true;
							}

						} else {
							initComplexInstanceCandidateData = true;
						}

						if (initComplexInstanceCandidateData) {
							individualComplexRoleInitHandlingFunction(complexRoleInstItem, neighbourLabelCacheItemSet, indiComplexRoleDataHash, initLabelCacheItemSet);
							individualComplexRoleInitHandlingFunction(complexRoleInstItem, neighbourInversedLabelCacheItemSet, indiComplexRoleDataHash, initLabelCacheItemSet);

							individualComplexRoleInitHandlingFunction(complexRoleInstItem, neighbourLabelCacheItemSet, indiComplexRoleDataHash, initLabelCacheItemSet);
							individualComplexRoleInitHandlingFunction(complexRoleInstItem, existentialInversedLabelCacheItemSet, indiComplexRoleDataHash, initLabelCacheItemSet);
						}
					}




					reqConfPreCompItem->getBackendAssociationCacheReader()->visitLabelCacheEntries(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, [&](CBackendRepresentativeMemoryLabelCacheItem* possSameIndiLabelCacheItem)->bool {

						reqConfPreCompItem->getBackendAssociationCacheReader()->visitIndividualIdsOfAssociatedIndividualSetLabel(nullptr, possSameIndiLabelCacheItem, [&](cint64 indiId)->bool {

							if (possSameIndiLabelCacheItem->getIndividualAssociationCount() > 0 && !individualPossibleSameIndividualListHash.contains(indiId)) {

								QList<CIndividualReference> possibleSameIndividualList;


								CBackendRepresentativeMemoryLabelCacheItem* sameIndilabelItem = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociatedCacheLabelItem(indiId, CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
								CBackendRepresentativeMemoryLabelCacheItem* possibleSameIndilabelItem = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociatedCacheLabelItem(indiId, CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
								reqConfPreCompItem->getBackendAssociationCacheReader()->visitIndividualIdsOfAssociatedIndividualSetLabel(nullptr, possibleSameIndilabelItem, [&](cint64 possibleSameIndiId)->bool {

									if (possibleSameIndiId != indiId && !reqConfPreCompItem->getBackendAssociationCacheReader()->hasIndividualIdsInAssociatedIndividualSetLabel(nullptr, sameIndilabelItem, possibleSameIndiId)) {
										CIndividual* possibleSameIndividual = indiVec->getData(possibleSameIndiId);
										possibleSameIndividualList.append(CIndividualReference(possibleSameIndividual, possibleSameIndiId));
									}
									return true;
								});

								individualPossibleSameIndividualListHash.insert(indiId, possibleSameIndividualList);

							}



							return false;
						});


						return true;
					});



					if (!individualPossibleSameIndividualListHash.isEmpty()) {
						QSet<COptimizedKPSetIndividualItem*> singleIndividualSet;
						for (QHash<cint64,QList<CIndividualReference> >::const_iterator it = individualPossibleSameIndividualListHash.constBegin(), itEnd = individualPossibleSameIndividualListHash.constEnd(); it != itEnd; ++it) {
							cint64 individualId = it.key();
							COptimizedKPSetIndividualItem* individualItem = reqConfPreCompItem->getIndividualInstantiatedItem(individualId, true);
							if (!singleIndividualSet.contains(individualItem)) {
								singleIndividualSet.insert(individualItem);
								const QList<CIndividualReference> possibleSameIndividualList = it.value();
								initializeSamePossibleIndividuals(reqConfPreCompItem,individualItem,possibleSameIndividualList);
							}
						}
					}

				}

				return true;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingThread::initializeSamePossibleIndividuals(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* individualItem, const QList<CIndividualReference>& possibleSameIndividualList) {
				reqConfPreCompItem->addPossibleSameIndividualsItem(individualItem);
				for (QList<CIndividualReference>::const_iterator it = possibleSameIndividualList.constBegin(), itEnd = possibleSameIndividualList.constEnd(); it != itEnd; ++it) {
					CIndividualReference possSameIndividual(*it);
					COptimizedKPSetIndividualItem* possSameIndividualItem = reqConfPreCompItem->getIndividualInstantiatedItem(possSameIndividual.getIndividualID(),true);
					individualItem->addPossibleSameIndividualItem(possSameIndividualItem);
					possSameIndividualItem->addPossibleSameIndividualItem(individualItem);
					incOpenPossibleSameIndividualsCount(reqConfPreCompItem);
				}
				return true;
			}






			CPossibleInstancesIndividualsMergingData* COptimizedRepresentativeKPSetOntologyRealizingThread::getPossibleInstancesMergingData(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem) {
				if (!mConfPossibleInstanceIndividualsAfterwardsMerging) {
					return nullptr;
				}
				if (reqConfPreCompItem->requiresIndividualDependenceTracking()) {
					return nullptr;
				}
				QHash<COptimizedKPSetIndividualItem*, CPossibleInstancesIndividualsMergingLinker*>* mergingDataHash = instancesItem->getIndividualItemPossibleInstanceMergingLinkerHash(false);
				if (mergingDataHash && mergingDataHash->contains(instantiatedItem)) {
					CPossibleInstancesIndividualsMergingLinker* mergingIndiLinker = mergingDataHash->value(instantiatedItem);
					CPossibleInstancesIndividualsMergingData* mergingData = mergingIndiLinker->getMergingData();
					return mergingData;
				}
				// test whether it is worthwhile to try merging with other individuals
				QMap<cint64, COptimizedKPSetIndividualItem*>* possibleInstanceItemSet = instancesItem->getPossibleInstancesMap();
				QMap<cint64, COptimizedKPSetIndividualItem*>* prefferedInstanceItemTestingSet = instancesItem->getPrefferedPossibleInstanceTestingSet();

				QHash<CBackendRepresentativeMemoryLabelCacheItem*, bool>* testConceptLabelSetHash = nullptr;

				cint64 compatibleLabelItemCount = 0;
				cint64 clashingLabelItemCount = 0;
				cint64 possibleInstanceIndividualsMergingCount = 0;

				cint64 maxMergingCount = mConfPossibleInstanceIndividualsAfterwardsMergingProvidingCount;
				if (possibleInstanceItemSet && possibleInstanceItemSet->count() > 1 && maxMergingCount != 0) {
					CPossibleInstancesIndividualsMergingData* mergingData = nullptr;
					cint64 individualId = instantiatedItem->getIndividualId();
					CBackendRepresentativeMemoryCacheIndividualAssociationData* assData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(individualId);
					if (assData) {
						CBackendRepresentativeMemoryLabelCacheItem* conceptLabelDataItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
						// find the individuals with the same/a compatible representative label

						for (QMap<cint64, COptimizedKPSetIndividualItem*>::iterator possibleInstanceItemIt = possibleInstanceItemSet->begin(); possibleInstanceItemIt != possibleInstanceItemSet->end(); ) {
							COptimizedKPSetIndividualItem* otherPossibleInstanceItem(*possibleInstanceItemIt);

							bool iterateNextPossibleInstance = true;
						
							if (!mergingDataHash || !mergingDataHash->contains(otherPossibleInstanceItem)) {
								cint64 otherPossibleIndividualId = otherPossibleInstanceItem->getIndividualId();
								bool otherPossibleIndiSuitable = true;
								CBackendRepresentativeMemoryCacheIndividualAssociationData* otherPossInstAssData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(otherPossibleIndividualId);
								CBackendRepresentativeMemoryLabelCacheItem* oterConceptLabelItem = otherPossInstAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
								if (oterConceptLabelItem == conceptLabelDataItem) {
									otherPossibleIndiSuitable = true;
								} else if (mConfPossibleInstanceIndividualsAfterwardsMergingOnlyWithSameRepresentativeLabel) {
									otherPossibleIndiSuitable = false;
								} else {
									if (testConceptLabelSetHash && testConceptLabelSetHash->contains(oterConceptLabelItem)) {
										otherPossibleIndiSuitable = testConceptLabelSetHash->value(oterConceptLabelItem);
									} else {
										otherPossibleIndiSuitable = true;
										reqConfPreCompItem->getBackendAssociationCacheReader()->visitConceptsOfAssociatedConceptSetLabel(assData, conceptLabelDataItem, [&](CConcept* concept, bool negation)->bool {
											if (reqConfPreCompItem->getBackendAssociationCacheReader()->hasConceptInAssociatedFullConceptSetLabel(otherPossInstAssData, oterConceptLabelItem, concept, !negation)) {
												otherPossibleIndiSuitable = false;
												return false;
											}
											return true;
										});
										if (!testConceptLabelSetHash) {
											testConceptLabelSetHash = new QHash<CBackendRepresentativeMemoryLabelCacheItem*, bool>();
										}
										testConceptLabelSetHash->insert(oterConceptLabelItem, otherPossibleIndiSuitable);
										if (otherPossibleIndiSuitable) {
											compatibleLabelItemCount++;
										} else {
											clashingLabelItemCount++;
										}
									}
								}

								if (otherPossibleIndiSuitable) {
									possibleInstanceIndividualsMergingCount++;

									bool isInstanceByModelMerging = false;
									if (checkTrivialConceptInstanceModelMerging(reqConfPreCompItem, instancesItem, otherPossibleInstanceItem, isInstanceByModelMerging, true)) {

										instancesItem->incTestingPossibleInstancesCount();
										otherPossibleInstanceItem->incTestingPossibleInstantiatedCount();
										otherPossibleInstanceItem->decPossibleInstantiatedCount();
										otherPossibleInstanceItem->setTestingPossibleInstantiated(instancesItem);
										reqConfPreCompItem->incTestingPossibleConceptInstanceCount();
										reqConfPreCompItem->incModelMergingsSucessCount();

										possibleInstanceItemIt = possibleInstanceItemSet->erase(possibleInstanceItemIt);
										if (prefferedInstanceItemTestingSet) {
											prefferedInstanceItemTestingSet->remove(otherPossibleInstanceItem->getIndividualId());
										}
										iterateNextPossibleInstance = false;

										if (isInstanceByModelMerging) {
											reqConfPreCompItem->incModelMergingsInstanceFoundCount();
										} else {
											reqConfPreCompItem->incModelMergingsNonInstanceFoundCount();
										}
										interpretConceptInstantiationResult(reqConfPreCompItem, instancesItem, otherPossibleInstanceItem, isInstanceByModelMerging);

									} else {

										if (!mergingData) {
											reqConfPreCompItem->incProvidedPossibleInstancesMergingsCount();
											mergingData = new CPossibleInstancesIndividualsMergingData(mConfPossibleInstanceIndividualsAfterwardsMergingMaximumAttemptCount);

											CPossibleInstancesIndividualsMergingLinker* linker = new CPossibleInstancesIndividualsMergingLinker();
											linker->initPossibleInstancesIndividualsMergingLinker(individualId, mergingData);
											reqConfPreCompItem->incProvidedPossibleInstancesMergingIndividualsCount();
											mergingData->addPossibleInstancesIndividualsMergingLinker(linker);
											if (!mergingDataHash) {
												mergingDataHash = instancesItem->getIndividualItemPossibleInstanceMergingLinkerHash(true);
											}
											mergingDataHash->insert(instantiatedItem, linker);
										}
										CPossibleInstancesIndividualsMergingLinker* linker = new CPossibleInstancesIndividualsMergingLinker();
										linker->initPossibleInstancesIndividualsMergingLinker(otherPossibleIndividualId, mergingData);
										reqConfPreCompItem->incProvidedPossibleInstancesMergingIndividualsCount();
										mergingData->addPossibleInstancesIndividualsMergingLinker(linker);
										if (!mergingDataHash) {
											mergingDataHash = instancesItem->getIndividualItemPossibleInstanceMergingLinkerHash(true);
										}
										mergingDataHash->insert(otherPossibleInstanceItem, linker);

										if (mConfPossibleInstanceIndividualsAfterwardsMergingProvidingCount > 0 && maxMergingCount-- <= 0) {
											break;
										}
									}
								}
							}

							if (iterateNextPossibleInstance) {
								++possibleInstanceItemIt;
							}
						}

					}

					if (mergingData) {
						instancesItem->getPossibleInstanceMergingDataContainer(true)->append(mergingData);
					}
					return mergingData;
				}
				if (testConceptLabelSetHash) {
					delete testConceptLabelSetHash;
				}
				return nullptr;
			}





			bool COptimizedRepresentativeKPSetOntologyRealizingThread::visitRepresentativeCacheNeighbours(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CRole* role, CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, function<bool(cint64 neighbourIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssData, bool deterministicNeighbour)> visitFunc) {
				COptimizedKPSetRoleInstancesRedirectionItem* redRoleItem = (COptimizedKPSetRoleInstancesRedirectionItem*)reqConfPreCompItem->getRoleInstantiatedItem(role);
				if (redRoleItem) {
					COptimizedKPSetRoleInstancesItem* roleInstancesItem = redRoleItem->getRedirectedItem();
					bool itemInversed = redRoleItem->isInversed();

					bool inversed = itemInversed;

					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* roleItemCompNeighLabelData = roleInstancesItem->getCombinedNeighbourCacheLabelItemDataHash(inversed)->value(indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL));
					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();

					bool continueVisiting = true;
					if (roleItemCompNeighLabelData) {
						if (continueVisiting && roleItemCompNeighLabelData->hasKnownInstancesLabelItems()) {
							QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* singleNeighLabelDataHash = roleItemCompNeighLabelData->getKnownInstancesLabelItemDataHash();

							for (auto it = singleNeighLabelDataHash->constBegin(), itEnd = singleNeighLabelDataHash->constEnd(); it != itEnd && continueVisiting; ++it) {
								roleSetNeighbourArray->at(it.value()->getLabelArrayIndex()).visitNeighbourIndividualIds([&](cint64 neighbourId)->bool {
									CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourIndiAssData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(neighbourId);
									continueVisiting = visitFunc(neighbourId, neighbourIndiAssData, true);
									return continueVisiting;
								});
							}
						}
						if (continueVisiting && roleItemCompNeighLabelData->hasPossibleInstancesLabelItems()) {
							QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* singleNeighLabelDataHash = roleItemCompNeighLabelData->getPossibleInstancesLabelItemDataHash();

							for (auto it = singleNeighLabelDataHash->constBegin(), itEnd = singleNeighLabelDataHash->constEnd(); it != itEnd && continueVisiting; ++it) {
								roleSetNeighbourArray->at(it.value()->getLabelArrayIndex()).visitNeighbourIndividualIds([&](cint64 neighbourId)->bool {
									CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourIndiAssData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(neighbourId);
									continueVisiting = visitFunc(neighbourId, neighbourIndiAssData, false);
									return continueVisiting;
								});
							}
						}
					}
					return true;

				}
				return false;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingThread::checkTrivialInstanceConceptRepresentativeCacheBasedMerging(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CConcept* concept, bool conceptNegation, CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, bool* mergableFlag, bool* unmergableFlag, cint64* mergingOperationLimit) {
				if (mergingOperationLimit && mergingOperationLimit-- <= 0) {
					return false;
				}

				// mergeable means that the concept could safely be added without causing a clash
				bool mergeable = false;

				// unmergable means that the addition of the concept would certainly cause a clash
				bool unmergeable = false;

				cint64 opCode = concept->getOperatorCode();

				CBackendRepresentativeMemoryLabelCacheItem* conceptSetLabelItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);

				bool negatedConceptOccurrence = false;
				bool deterministicOccurrence = false;
				if (reqConfPreCompItem->getBackendAssociationCacheReader()->getConceptOccurrenceInAssociatedFullConceptSetLabel(assData, conceptSetLabelItem, concept, negatedConceptOccurrence, deterministicOccurrence)) {
					if (negatedConceptOccurrence == conceptNegation) {
						mergeable = true;
					} else {
						if (deterministicOccurrence) {
							unmergeable = true;
						}
					}
				} else {
					if (conceptNegation && (opCode == CCSUB)) {
						mergeable = true;

					} else if (!conceptNegation && (opCode == CCOR) || conceptNegation && (opCode == CCAND || opCode == CCEQ)) {
						// mergeable if we find one concept that is not in the label or the sub concept is mergeable

						bool oneMergable = false;
						bool allUnmergeable = true;

						if (conceptSetLabelItem) {
							for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && (!oneMergable || allUnmergeable); opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opConNegation = opConLinkerIt->isNegated() ^ conceptNegation;

								bool opConMergeable = false;
								bool opConUnmergeable = false;

								checkTrivialInstanceConceptRepresentativeCacheBasedMerging(reqConfPreCompItem, opConcept, opConNegation, assData, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
								oneMergable |= opConMergeable;
								allUnmergeable &= opConUnmergeable;
							}
						}

						mergeable = oneMergable;
						unmergeable = allUnmergeable;


					} else if (!conceptNegation && (opCode == CCOR) || conceptNegation && (opCode == CCAND || opCode == CCEQ)) {

						bool allMergable = true;
						bool oneUnmergeable = true;

						if (conceptSetLabelItem) {
							for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && (allMergable || !oneUnmergeable); opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opConNegation = opConLinkerIt->isNegated() ^ conceptNegation;

								bool opConMergeable = false;
								bool opConUnmergeable = false;

								checkTrivialInstanceConceptRepresentativeCacheBasedMerging(reqConfPreCompItem, opConcept, opConNegation, assData, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
								allMergable &= opConMergeable;
								oneUnmergeable |= opConUnmergeable;
							}
						}

						mergeable = allMergable;
						unmergeable = oneUnmergeable;


					} else if (!conceptNegation && (opCode == CCATMOST) || conceptNegation && (opCode == CCATLEAST)) {
						cint64 cardinality = concept->getParameter() - 1 * conceptNegation;
						// mergeable if the node does not have n corresponding neighbours
						CRole* role = concept->getRole();


						cint64 ignoreableNeighbourCount = 0;
						cint64 certainlyRelevantNeighbourCount = 0;
						cint64 succCount = 0;
						bool cancled = false;

						CBackendRepresentativeMemoryLabelCacheItem* detExtLabelItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
						CBackendRepresentativeMemoryLabelCacheItem* nondetExtLabelItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);

						if (!reqConfPreCompItem->getBackendAssociationCacheReader()->hasRoleInAssociatedCompinationRoleSetLabel(assData, detExtLabelItem, role, false) && !reqConfPreCompItem->getBackendAssociationCacheReader()->hasRoleInAssociatedCompinationRoleSetLabel(assData, nondetExtLabelItem, role, false)) {

							if (visitRepresentativeCacheNeighbours(reqConfPreCompItem, role, assData, [&](cint64 neighbourIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssData, bool deterministicNeighbour)->bool {

								for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opConNegation = opConLinkerIt->isNegated();

									bool opConMergeable = false;
									bool opConUnmergeable = false;

									checkTrivialInstanceConceptRepresentativeCacheBasedMerging(reqConfPreCompItem, opConcept, !opConNegation, neighbourAssData, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
									// if the negation of the qualifying concept can safely be added, then it does not have to be considered for the merging
									if (opConMergeable) {
										ignoreableNeighbourCount++;
									}
									if (opConUnmergeable) {
										certainlyRelevantNeighbourCount++;
									}
								}

								++succCount;


								if (mergingOperationLimit-- <= 0) {
									cancled = true;
									return false;
								}

								if (certainlyRelevantNeighbourCount > cardinality) {
									return false;
								} else {
									return true;
								}

							})) {

								if (!cancled) {
									if (cardinality == 0 && certainlyRelevantNeighbourCount >= 1) {
										unmergeable = true;
									}
									if (certainlyRelevantNeighbourCount <= cardinality && succCount - ignoreableNeighbourCount <= cardinality) {
										mergeable = true;
									}
								}

							}
						}

					} else if (!conceptNegation && (opCode == CCSOME) || conceptNegation && (opCode == CCALL)) {
						// mergeable if there is one neighbour that satisfies the qualifying restriction
						// unmergeable is non-trivial to detect (maybe we can use the saturation graph somehow?)

						CRole* role = concept->getRole();

						bool oneMergable = false;
						bool allUnmergeable = true;
						bool cancled = false;

						if (visitRepresentativeCacheNeighbours(reqConfPreCompItem, role, assData, [&](cint64 neighbourIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssData, bool deterministicNeighbour)->bool {

							for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && !oneMergable; opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opConNegation = opConLinkerIt->isNegated() ^ conceptNegation;

								bool opConMergeable = false;
								bool opConUnmergeable = false;

								checkTrivialInstanceConceptRepresentativeCacheBasedMerging(reqConfPreCompItem, opConcept, opConNegation, neighbourAssData, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
								oneMergable |= opConMergeable;
							}


							if (mergingOperationLimit-- <= 0) {
								cancled = true;
								return false;
							}


							return !oneMergable;

						})) {
							if (!cancled) {
								mergeable = oneMergable;
							}
						}


					} else if (!conceptNegation && (opCode == CCALL) || conceptNegation && (opCode == CCSOME)) {

						CRole* role = concept->getRole();

						bool allMergable = true;
						bool oneUnmergeable = false;
						bool cancled = false;

						CBackendRepresentativeMemoryLabelCacheItem* detExtLabelItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
						CBackendRepresentativeMemoryLabelCacheItem* nondetExtLabelItem = assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);

						if (!reqConfPreCompItem->getBackendAssociationCacheReader()->hasRoleInAssociatedCompinationRoleSetLabel(assData, detExtLabelItem, role, false) && !reqConfPreCompItem->getBackendAssociationCacheReader()->hasRoleInAssociatedCompinationRoleSetLabel(assData, nondetExtLabelItem, role, false)) {

							if (visitRepresentativeCacheNeighbours(reqConfPreCompItem, role, assData, [&](cint64 neighbourIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssData, bool deterministicNeighbour)->bool {
								for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && (allMergable || !oneUnmergeable); opConLinkerIt = opConLinkerIt->getNext()) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opConNegation = opConLinkerIt->isNegated() ^ conceptNegation;

									bool opConMergeable = false;
									bool opConUnmergeable = false;

									checkTrivialInstanceConceptRepresentativeCacheBasedMerging(reqConfPreCompItem, opConcept, opConNegation, neighbourAssData, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
									allMergable &= opConMergeable;
									if (deterministicNeighbour) {
										oneUnmergeable |= opConUnmergeable;
									}
								}

								if (mergingOperationLimit-- <= 0) {
									cancled = true;
									return false;
								}

								return (allMergable || !oneUnmergeable);
							})) {
								if (!cancled) {
									mergeable = allMergable;
									unmergeable = oneUnmergeable;
								}
							}
						}


					} else if (!conceptNegation && (opCode == CCNOMINAL)) {

						CIndividual* nominalIndi = concept->getNominalIndividual();
						if (nominalIndi->getIndividualID() == assData->getAssociatedIndividualId()) {
							mergeable = true;
						} else {
							if (reqConfPreCompItem->getBackendAssociationCacheReader()->hasIndividualIdsInAssociatedIndividualSetLabel(assData, assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL), nominalIndi->getIndividualID()) ||
								reqConfPreCompItem->getBackendAssociationCacheReader()->hasIndividualIdsInAssociatedIndividualSetLabel(assData, assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL), nominalIndi->getIndividualID())) {
								mergeable = true;
							} else if (reqConfPreCompItem->getBackendAssociationCacheReader()->hasIndividualIdsInAssociatedIndividualSetLabel(assData, assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL), nominalIndi->getIndividualID())) {
								unmergeable = true;
							}
						}

					} else if (conceptNegation && (opCode == CCNOMINAL)) {

						CIndividual* nominalIndi = concept->getNominalIndividual();
						if (nominalIndi->getIndividualID() == assData->getAssociatedIndividualId()) {
							unmergeable = true;
						} else {
							if (reqConfPreCompItem->getBackendAssociationCacheReader()->hasIndividualIdsInAssociatedIndividualSetLabel(assData, assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL), nominalIndi->getIndividualID()) ||
								reqConfPreCompItem->getBackendAssociationCacheReader()->hasIndividualIdsInAssociatedIndividualSetLabel(assData, assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL), nominalIndi->getIndividualID())) {
								mergeable = true;
							} else if (reqConfPreCompItem->getBackendAssociationCacheReader()->hasIndividualIdsInAssociatedIndividualSetLabel(assData, assData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL), nominalIndi->getIndividualID())) {
								unmergeable = true;
							}
						}

					}
				}

				if (mergableFlag) {
					*mergableFlag = mergeable;
				}
				if (unmergableFlag) {
					*unmergableFlag = unmergeable;
				}

				if (mergeable || unmergeable) {
					return true;
				} else {
					return false;
				}
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingThread::checkTrivialInstanceConceptCachedCompletionGraphBasedMerging(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CConcept* concept, bool conceptNegation, CIndividualProcessNode* indiNode, CIndividualProcessNodeVector* indiProcVector, bool* mergableFlag, bool* unmergableFlag, cint64* mergingOperationLimit) {
				if (mergingOperationLimit && mergingOperationLimit-- <= 0) {
					return false;
				}

				// mergeable means that the concept could safely be added without causing a clash
				bool mergeable = false;

				// unmergable means that the addition of the concept would certainly cause a clash
				bool unmergeable = false;

				cint64 opCode = concept->getOperatorCode();
				CReapplyConceptLabelSet* labelSet = indiNode->getReapplyConceptLabelSet(false);

				CConceptDescriptor* conDes = nullptr;
				CDependencyTrackPoint* depTrackPoint = nullptr;
				if (labelSet->getConceptDescriptor(concept, conDes, depTrackPoint)) {
					if (conDes->getNegation() == conceptNegation) {
						mergeable = true;
					} else {
						if (depTrackPoint && depTrackPoint->getBranchingTag() <= 0) {
							unmergeable = true;
						}
					}
				} else {
					if (conceptNegation && (opCode == CCSUB)) {
						mergeable = true;

					} else if (!conceptNegation && (opCode == CCOR) || conceptNegation && (opCode == CCAND || opCode == CCEQ)) {
						// mergeable if we find one concept that is not in the label or the sub concept is mergeable

						bool oneMergable = false;
						bool allUnmergeable = true;

						if (labelSet) {
							for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && (!oneMergable || allUnmergeable); opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opConNegation = opConLinkerIt->isNegated() ^ conceptNegation;

								bool opConMergeable = false;
								bool opConUnmergeable = false;

								checkTrivialInstanceConceptCachedCompletionGraphBasedMerging(reqConfPreCompItem, opConcept, opConNegation, indiNode, indiProcVector, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
								oneMergable |= opConMergeable;
								allUnmergeable &= opConUnmergeable;
							}
						}

						mergeable = oneMergable;
						unmergeable = allUnmergeable;


					} else if (!conceptNegation && (opCode == CCOR) || conceptNegation && (opCode == CCAND || opCode == CCEQ)) {

						bool allMergable = true;
						bool oneUnmergeable = true;

						if (labelSet) {
							for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && (allMergable || !oneUnmergeable); opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opConNegation = opConLinkerIt->isNegated() ^ conceptNegation;

								bool opConMergeable = false;
								bool opConUnmergeable = false;

								checkTrivialInstanceConceptCachedCompletionGraphBasedMerging(reqConfPreCompItem, opConcept, opConNegation, indiNode, indiProcVector, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
								allMergable &= opConMergeable;
								oneUnmergeable |= opConUnmergeable;
							}
						}

						mergeable = allMergable;
						unmergeable = oneUnmergeable;


					} else if (!conceptNegation && (opCode == CCATMOST) || conceptNegation && (opCode == CCATLEAST)) {
						cint64 cardinality = concept->getParameter() - 1 * conceptNegation;
						// mergeable if the node does not have n corresponding neighbours
						CRole* role = concept->getRole();
						CReapplyRoleSuccessorHash* roleSuccHash = indiNode->getReapplyRoleSuccessorHash(false);
						if (roleSuccHash) {
							cint64 succCount = roleSuccHash->getRoleSuccessorCount(role);
							if (succCount <= cardinality) {
								mergeable = true;
							} else {

								cint64 ignoreableNeighbourCount = 0;
								cint64 certainlyRelevantNeighbourCount = 0;


								CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
								if (opConLinker) {
									// count the number of neighbours without the qualifying concept or mergeable
									CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
									while (roleSuccIt.hasNext() && succCount - ignoreableNeighbourCount > cardinality && certainlyRelevantNeighbourCount <= cardinality) {
										CIndividualLinkEdge* neighbourLink = roleSuccIt.next();
										cint64 neighbourId = neighbourLink->getOppositeIndividualID(indiNode->getIndividualNodeID());
										CIndividualProcessNode* neighbourIndiNode = indiProcVector->getData(neighbourId);

										for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
											CConcept* opConcept = opConLinkerIt->getData();
											bool opConNegation = opConLinkerIt->isNegated();

											bool opConMergeable = false;
											bool opConUnmergeable = false;

											checkTrivialInstanceConceptCachedCompletionGraphBasedMerging(reqConfPreCompItem, opConcept, !opConNegation, neighbourIndiNode, indiProcVector, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
											// if the negation of the qualifying concept can safely be added, then it does not have to be considered for the merging
											if (opConMergeable) {
												ignoreableNeighbourCount++;
											}
											if (opConUnmergeable) {
												certainlyRelevantNeighbourCount++;
											}
										}


									}
								}

								if (succCount - ignoreableNeighbourCount <= cardinality) {
									mergeable = true;
								}
								if (cardinality == 0 && certainlyRelevantNeighbourCount >= 1) {
									unmergeable = true;
								}

							}
						} else if (cardinality >= 0) {
							mergeable = true;
						}

					} else if (!conceptNegation && (opCode == CCSOME) || conceptNegation && (opCode == CCALL)) {
						// mergeable if there is one neighbour that satisfies the qualifying restriction
						// unmergeable is non-trivial to detect (maybe we can use the saturation graph somehow?)

						CRole* role = concept->getRole();
						CReapplyRoleSuccessorHash* roleSuccHash = indiNode->getReapplyRoleSuccessorHash(false);

						bool oneMergable = false;
						bool allUnmergeable = true;

						CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
						while (roleSuccIt.hasNext() && !oneMergable) {

							CIndividualLinkEdge* neighbourLink = roleSuccIt.next();
							cint64 neighbourId = neighbourLink->getOppositeIndividualID(indiNode->getIndividualNodeID());
							CIndividualProcessNode* neighbourIndiNode = indiProcVector->getData(neighbourId);

							for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && !oneMergable; opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opConNegation = opConLinkerIt->isNegated() ^ conceptNegation;

								bool opConMergeable = false;
								bool opConUnmergeable = false;

								checkTrivialInstanceConceptCachedCompletionGraphBasedMerging(reqConfPreCompItem, opConcept, opConNegation, neighbourIndiNode, indiProcVector, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
								oneMergable |= opConMergeable;
							}
						}

						mergeable = oneMergable;

					} else if (!conceptNegation && (opCode == CCALL) || conceptNegation && (opCode == CCSOME)) {

						CRole* role = concept->getRole();
						CReapplyRoleSuccessorHash* roleSuccHash = indiNode->getReapplyRoleSuccessorHash(false);

						bool allMergable = true;
						bool oneUnmergeable = false;

						CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role);
						while (roleSuccIt.hasNext() && (allMergable || !oneUnmergeable)) {

							CIndividualLinkEdge* neighbourLink = roleSuccIt.next();
							cint64 neighbourId = neighbourLink->getOppositeIndividualID(indiNode->getIndividualNodeID());
							CIndividualProcessNode* neighbourIndiNode = indiProcVector->getData(neighbourId);

							for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt && (allMergable || !oneUnmergeable); opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opConNegation = opConLinkerIt->isNegated() ^ conceptNegation;

								bool opConMergeable = false;
								bool opConUnmergeable = false;

								checkTrivialInstanceConceptCachedCompletionGraphBasedMerging(reqConfPreCompItem, opConcept, opConNegation, neighbourIndiNode, indiProcVector, &opConMergeable, &opConUnmergeable, mergingOperationLimit);
								allMergable &= opConMergeable;
								if (opConUnmergeable && neighbourLink->getDependencyTrackPoint() && neighbourLink->getDependencyTrackPoint()->getBranchingTag() <= 0) {
									oneUnmergeable = true;
								}
							}
						}

						mergeable = allMergable;
						unmergeable = oneUnmergeable;


					} else if (!conceptNegation && (opCode == CCNOMINAL)) {

						CIndividual* nominalIndi = concept->getNominalIndividual();
						if (-nominalIndi->getIndividualID() == indiNode->getIndividualNodeID()) {
							mergeable = true;
						} else {
							CIndividualMergingHash* indiMergingHash = indiNode->getIndividualMergingHash(false);
							if (indiMergingHash->value(-nominalIndi->getIndividualID()).isMergedWithIndividual()) {
								mergeable = true;
							}
						}

					} else if (conceptNegation && (opCode == CCNOMINAL)) {

						CIndividual* nominalIndi = concept->getNominalIndividual();
						if (-nominalIndi->getIndividualID() == indiNode->getIndividualNodeID()) {
							unmergeable = true;
						} else {
							CIndividualMergingHash* indiMergingHash = indiNode->getIndividualMergingHash(false);
							if (indiMergingHash->value(-nominalIndi->getIndividualID()).isMergedWithIndividual()) {
								unmergeable = true;
							}
						}

					}
				}

				if (mergableFlag) {
					*mergableFlag = mergeable;
				}
				if (unmergableFlag) {
					*unmergableFlag = unmergeable;
				}

				if (mergeable || unmergeable) {
					return true;
				} else {
					return false;
				}

			}




			bool COptimizedRepresentativeKPSetOntologyRealizingThread::checkTrivialConceptInstanceModelMerging(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem, bool& isInstance, bool shouldRememberChecking) {

				if (mConfPossibleInstanceConceptsUpfrontMerging) {
					QMap<cint64, COptimizedKPSetIndividualItem*>* modelMergingCheckedSet = instancesItem->getModelMergingCheckedSet(false);
					if (!modelMergingCheckedSet || !modelMergingCheckedSet->contains(instantiatedItem->getIndividualId())) {
						if (shouldRememberChecking) {
							if (!modelMergingCheckedSet) {
								modelMergingCheckedSet = instancesItem->getModelMergingCheckedSet(true);
							}
							modelMergingCheckedSet->insert(instantiatedItem->getIndividualId(), instantiatedItem);
						}
						cint64 indiID = -instantiatedItem->getIndividualId();

						CIndividualProcessNodeVector* indiProcVector = nullptr;
						CConsistence* consistence = reqConfPreCompItem->getOntology()->getConsistence();
						if (consistence) {
							CConsistenceData* consData = consistence->getConsistenceModelData();
							if (consData) {
								CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
								CSatisfiableCalculationTask* satConsTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
								if (!satConsTask) {
									satConsTask = consTaskData->getDeterministicSatisfiableTask();
								}
								if (satConsTask) {
									CProcessingDataBox* procDataBox = satConsTask->getProcessingDataBox();
									if (procDataBox) {
										indiProcVector = procDataBox->getIndividualProcessNodeVector();
									}
								}
							}
						}


						if (indiProcVector) {
							CIndividualProcessNode* baseIndiProcNode = indiProcVector->getData(indiID);
							CIndividualProcessNode* indiProcNode = baseIndiProcNode;
							if (indiProcNode) {
								reqConfPreCompItem->incModelMergingsTriedCount();
								bool nondeterministicallyMergedFlag = false;
								indiProcNode = getMergeCorrectedIndividualProcessNode(indiProcNode, indiProcVector, &nondeterministicallyMergedFlag, reqConfPreCompItem);

								CConcept* concept = instancesItem->getHierarchyNode()->getOneEquivalentConcept();

								//if (CIRIName::getRecentIRIName(concept->getNameLinker()) == "http://www.reactome.org/biopax/46/170905#Protein19") {
								//	bool debug = true;
								//}

								bool mergeable = false;
								bool unmergeable = false;
								cint64 mergeOperationLimit = 15;
								if (checkTrivialInstanceConceptCachedCompletionGraphBasedMerging(reqConfPreCompItem, concept, true, indiProcNode, indiProcVector, &mergeable, &unmergeable, &mergeOperationLimit)) {
									if (mergeable) {
										isInstance = false;
									}
									if (unmergeable) {
										isInstance = true;
									}
									return true;
								}
							}
						} else {

							CBackendRepresentativeMemoryCacheIndividualAssociationData* assData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(instantiatedItem->getIndividualId());
							CConcept* concept = instancesItem->getHierarchyNode()->getOneEquivalentConcept();

							bool mergeable = false;
							bool unmergeable = false;
							cint64 mergeOperationLimit = 15;
							if (checkTrivialInstanceConceptRepresentativeCacheBasedMerging(reqConfPreCompItem, concept, true, assData, &mergeable, &unmergeable, &mergeOperationLimit)) {
								if (mergeable) {
									isInstance = false;
								}
								if (unmergeable) {
									isInstance = true;
								}
								return true;
							}

						}
					}
				}
				return false;
			}





			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createNextConceptInstantiationTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem, COntologyRealizingDynamicRequirmentProcessingData* procData) {
				instancesItem->incTestingPossibleInstancesCount();
				instantiatedItem->incTestingPossibleInstantiatedCount();
				instantiatedItem->decPossibleInstantiatedCount();
				instantiatedItem->setTestingPossibleInstantiated(instancesItem);
				reqConfPreCompItem->incTestingPossibleConceptInstanceCount();

				COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = instantiatedItem->getKnownPossibleInstancesHash();
				COptimizedKPSetConceptInstancesData* instanceItemData = knownPossibleInstancesHash->getInstanceItemData(instancesItem);
				instanceItemData->mTestingInstance = true;


				bool isInstanceByModelMerging = false;
				if (checkTrivialConceptInstanceModelMerging(reqConfPreCompItem, instancesItem, instantiatedItem, isInstanceByModelMerging)) {
					reqConfPreCompItem->incModelMergingsSucessCount();

					if (isInstanceByModelMerging) {
						reqConfPreCompItem->incModelMergingsInstanceFoundCount();
					} else {
						reqConfPreCompItem->incModelMergingsNonInstanceFoundCount();
					}
					interpretConceptInstantiationResult(reqConfPreCompItem, instancesItem, instantiatedItem, isInstanceByModelMerging);
					//setDynamicRequirementProcessed(reqConfPreCompItem, procData);
					return false;
				}

				bool alreadySatisfiable = false;
				CPossibleInstancesIndividualsMergingLinker* possibleInstanceMergingLinker = nullptr;
				CPossibleInstancesIndividualsMergingData* possInstanceMergingData = getPossibleInstancesMergingData(reqConfPreCompItem, instancesItem, instantiatedItem);
				if (possInstanceMergingData) {
					QHash<COptimizedKPSetIndividualItem*, CPossibleInstancesIndividualsMergingLinker*>* mergingDataHash = instancesItem->getIndividualItemPossibleInstanceMergingLinkerHash(false);
					possibleInstanceMergingLinker = mergingDataHash->value(instantiatedItem);
					if (possibleInstanceMergingLinker) {
						if (possibleInstanceMergingLinker->isSatisfiableMerged()) {
							alreadySatisfiable = true;
						} else {
							possibleInstanceMergingLinker->setFullTestingScheduled();
						}
					}

					if (alreadySatisfiable) {
						reqConfPreCompItem->incSucceddedPossibleInstancesMergingsCount();
						interpretConceptInstantiationResult(reqConfPreCompItem, instancesItem, instantiatedItem, false);
						//setDynamicRequirementProcessed(reqConfPreCompItem, procData);
						return false;
					}
				}


				//if (CIRIName::getRecentIRIName(instancesItem->getHierarchyNode()->getOneEquivalentConcept()->getClassNameLinker()) == "http://uob.iodt.ibm.com/univ-bench-dl.owl#PeopleWithManyHobbies") {

				//	interpretConceptInstantiationResult(reqConfPreCompItem, instancesItem, instantiatedItem, false);
				//	return false;
				//}


				CConcept* concept = instancesItem->getHierarchyNode()->getOneEquivalentConcept();
				CIndividualReference indiRef = instantiatedItem->getIndividualReference();

				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(concept,true, indiRef);
				satCalcJob->setCalculationConfiguration(reqConfPreCompItem->getCalculationConfiguration());
				if (reqConfPreCompItem->getPossibleAssertionCollectionSet()) {
					satCalcJob->setPossibleAssertionCollectionAdapter(new CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter(reqConfPreCompItem->getOntology(),reqConfPreCompItem->getPossibleAssertionCollectionSet()));
				}
				if (reqConfPreCompItem->requiresIndividualDependenceTracking()) {
					if (!instantiatedItem->getIndividualDependenceTrackingCollector()) {
						instantiatedItem->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
					}
				}
				if (instantiatedItem->getIndividualDependenceTrackingCollector()) {
					satCalcJob->setSatisfiableTaskIndividualDependenceTrackingAdapter(new CSatisfiableTaskIndividualDependenceTrackingAdapter(instantiatedItem->getIndividualDependenceTrackingCollector()));
				}
				CSatisfiableTaskRealizationPossibleInstancesMergingAdapter* possInstanceMergingDataAdapter = nullptr;
				if (possInstanceMergingData) {
					possInstanceMergingDataAdapter = new CSatisfiableTaskRealizationPossibleInstancesMergingAdapter(possInstanceMergingData, indiRef, possibleInstanceMergingLinker);
					satCalcJob->setSatisfiablePossibleInstancesMergingAdapter(possInstanceMergingDataAdapter);
				}



				CIndividualConceptInstanceTestingItem* testItem = new CIndividualConceptInstanceTestingItem(reqConfPreCompItem,instancesItem,instantiatedItem,procData, possInstanceMergingDataAdapter);

				processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
				return true;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createNextRoleInstantiationTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetIndividualItemPair itemPair, COntologyRealizingDynamicRequirmentProcessingData* procData) {
				instancesItem->incTestingPossibleInstancesCount();

				COptimizedKPSetIndividualItem* indItem1 = itemPair.first;
				COptimizedKPSetIndividualItem* indItem2 = itemPair.second;

				indItem1->incTestingPossibleRoleInstantiatedCount();
				indItem1->decPossibleRoleInstantiatedCount();
				indItem1->setTestingPossibleInstantiated(instancesItem,indItem2);

				CConcept* propConcept = instancesItem->getTemporaryPropagationConcept();
				CConcept* markConcept = instancesItem->getTemporaryMarkerConcept();
				CIndividual* indi1 = indItem1->getIndividual();
				CIndividual* indi2 = indItem2->getIndividual();

				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getTemporaryRoleRealizationOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(propConcept,false,indi1);
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(markConcept,true,indi2,satCalcJob);
				satCalcJob->setCalculationConfiguration(reqConfPreCompItem->getCalculationConfiguration());
				if (reqConfPreCompItem->requiresIndividualDependenceTracking()) {
					if (!indItem1->getIndividualDependenceTrackingCollector()) {
						indItem1->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
					}
				}
				if (indItem1->getIndividualDependenceTrackingCollector()) {
					satCalcJob->setSatisfiableTaskIndividualDependenceTrackingAdapter(new CSatisfiableTaskIndividualDependenceTrackingAdapter(indItem1->getIndividualDependenceTrackingCollector()));
				}

				CIndividualPairRoleInstanceTestingItem* testItem = new CIndividualPairRoleInstanceTestingItem(reqConfPreCompItem,instancesItem,itemPair, procData);
				reqConfPreCompItem->incTestingPossibleRoleInstanceCount();

				processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
				return true;
			}






			CIndividualProcessNode* COptimizedRepresentativeKPSetOntologyRealizingThread::getCompletionGraphCachedIndividualProcessNode(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* indiItem, bool* nondeterministicNodeFlag) {

				CIndividual* individual = indiItem->getIndividual();
				cint64 indiID = -individual->getIndividualID();

				CIndividualProcessNodeVector* indiProcVector = nullptr;
				CConsistence* consistence = reqConfPreCompItem->getOntology()->getConsistence();
				if (consistence) {
					CConsistenceData* consData = consistence->getConsistenceModelData();
					if (consData) {
						CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
						CSatisfiableCalculationTask* satConsTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
						if (!satConsTask) {
							satConsTask = consTaskData->getDeterministicSatisfiableTask();
						}
						if (satConsTask) {
							CProcessingDataBox* procDataBox = satConsTask->getProcessingDataBox();
							if (procDataBox) {
								indiProcVector = procDataBox->getIndividualProcessNodeVector();
							}
						}
					}
				}

				if (indiProcVector) {
					CIndividualProcessNode* baseIndiProcNode = indiProcVector->getData(indiID);
					CIndividualProcessNode* indiProcNode = baseIndiProcNode;
					bool nondeterministicallyMergedFlag = false;
					if (indiProcNode) {
						indiProcNode = getMergeCorrectedIndividualProcessNode(indiProcNode,indiProcVector,&nondeterministicallyMergedFlag,reqConfPreCompItem);
					}
					if (nondeterministicNodeFlag) {
						*nondeterministicNodeFlag = nondeterministicallyMergedFlag;
					}
					return indiProcNode;
				}
				return nullptr;
			}
























































			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createNextRoleInitializingTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, const CRealizationIndividualInstanceItemReference& indiRealItemRef) {
				bool workCreated = false;
				QSet<COptimizedKPSetRoleInstancesItem*>* complexRoleCandidateItemSet = nullptr;
				QSet<COptimizedKPSetRoleInstancesItem*>* inverseComplexRoleCandidateItemSet = nullptr;


				function<void(QSet<COptimizedKPSetRoleInstancesItem*>* roleCandidateItemSet, bool inverse)> complexRoleCandidateItemsHandlingFunction = [&](QSet<COptimizedKPSetRoleInstancesItem*>* roleCandidateItemSet, bool inverse) {
					if (roleCandidateItemSet && !roleCandidateItemSet->isEmpty()) {
						if (!complexRoleCandidateItemSet && !inverse) {
							complexRoleCandidateItemSet = new QSet<COptimizedKPSetRoleInstancesItem *>();
						}
						if (!inverseComplexRoleCandidateItemSet && inverse) {
							inverseComplexRoleCandidateItemSet = new QSet<COptimizedKPSetRoleInstancesItem *>();
						}
						for (COptimizedKPSetRoleInstancesItem* roleCandidateItem : *roleCandidateItemSet) {
							if (!inverse) {
								complexRoleCandidateItemSet->insert(roleCandidateItem);
							} else {
								inverseComplexRoleCandidateItemSet->insert(roleCandidateItem);
							}
						}
					}
				};

				cint64 indiId = indiRealItemRef.getIndividualID();



				CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(indiId);
				if (indiAssData) {
					CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
					if (combinedNeighbourRoleSetLabel) {
						COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* combinedNeighbourRoleLabelCacheItemData = reqConfPreCompItem->getRepresentativeCacheCombinedNeighbourRoleSetLabelItemData(combinedNeighbourRoleSetLabel);
						if (combinedNeighbourRoleLabelCacheItemData->hasComplexCandiateInstancesItems()) {
							complexRoleCandidateItemsHandlingFunction(combinedNeighbourRoleLabelCacheItemData->getComplexCandiateInstancesItemSet(false), false);
							complexRoleCandidateItemsHandlingFunction(combinedNeighbourRoleLabelCacheItemData->getComplexCandiateInstancesItemSet(true), true);
						}
					}



					CBackendRepresentativeMemoryLabelCacheItem* combinedDeterministicExistentialRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
					if (combinedDeterministicExistentialRoleSetLabel) {
						COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData* combinedDeterministicExistentialRoleLabelCacheItemData = reqConfPreCompItem->getRepresentativeCacheCombinedExistentialRoleSetLabelItemData(combinedDeterministicExistentialRoleSetLabel);
						if (combinedDeterministicExistentialRoleLabelCacheItemData->hasComplexCandiateInstancesItems()) {
							complexRoleCandidateItemsHandlingFunction(combinedDeterministicExistentialRoleLabelCacheItemData->getComplexCandiateInstancesItemSet(false), false);
							complexRoleCandidateItemsHandlingFunction(combinedDeterministicExistentialRoleLabelCacheItemData->getComplexCandiateInstancesItemSet(true), true);
						}
					}


					CBackendRepresentativeMemoryLabelCacheItem* combinedNondeterministicExistentialRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL);
					if (combinedNondeterministicExistentialRoleSetLabel) {
						COptimizedRepresentativeKPSetCombinedExistentialRoleSetCacheLabelItemData* combinedNondeterministicExistentialRoleLabelCacheItemData = reqConfPreCompItem->getRepresentativeCacheCombinedExistentialRoleSetLabelItemData(combinedNondeterministicExistentialRoleSetLabel);
						if (combinedNondeterministicExistentialRoleLabelCacheItemData->hasComplexCandiateInstancesItems()) {
							complexRoleCandidateItemsHandlingFunction(combinedNondeterministicExistentialRoleLabelCacheItemData->getComplexCandiateInstancesItemSet(false), false);
							complexRoleCandidateItemsHandlingFunction(combinedNondeterministicExistentialRoleLabelCacheItemData->getComplexCandiateInstancesItemSet(true), true);
						}
					}
				}

				if (complexRoleCandidateItemSet || inverseComplexRoleCandidateItemSet) {

					QList<CIndividualRoleCandidateTestingData> indiRoleCandTestDataList;


					if (complexRoleCandidateItemSet) {
						for (COptimizedKPSetRoleInstancesItem* complexRoleCandidateItem : *complexRoleCandidateItemSet) {
							COptimizedKPSetIndividualComplexRoleData* indiComplexRoleData = complexRoleCandidateItem->getIndividualIdComplexRoleData(indiId, true);
							if (!indiComplexRoleData->isInitializing(false)) {
								indiComplexRoleData->setInitializing(false, true);
								prepareRoleInitializingTestData(reqConfPreCompItem, complexRoleCandidateItem, false, indiRealItemRef, indiComplexRoleData, indiRoleCandTestDataList);
							}
						}
					}

					if (inverseComplexRoleCandidateItemSet) {
						for (COptimizedKPSetRoleInstancesItem* complexRoleCandidateItem : *inverseComplexRoleCandidateItemSet) {
							COptimizedKPSetIndividualComplexRoleData* indiComplexRoleData = complexRoleCandidateItem->getIndividualIdComplexRoleData(indiId, true);
							if (!indiComplexRoleData->isInitializing(true)) {
								indiComplexRoleData->setInitializing(true, true);
								prepareRoleInitializingTestData(reqConfPreCompItem, complexRoleCandidateItem, true, indiRealItemRef, indiComplexRoleData, indiRoleCandTestDataList);
							}
						}
					}


					workCreated = createNextRoleInitializingTest(reqConfPreCompItem, indiRealItemRef, indiRoleCandTestDataList);					

					delete complexRoleCandidateItemSet;
					delete inverseComplexRoleCandidateItemSet;
				}

				return workCreated;

			}



			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createNextRoleInitializingTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, const CRealizationIndividualInstanceItemReference& indiRealItemRef, const QList<CIndividualRoleCandidateTestingData>& indiRoleCandTestDataList) {
				bool workCreated = false;
				if (!indiRoleCandTestDataList.isEmpty()) {
					reqConfPreCompItem->incPropagatingRoleInstanceCandidatesCount();
					CIndividualReference indiRef = indiRealItemRef;

					//if (CIRIName::getRecentIRIName(indiRef.getIndividual()->getIndividualNameLinker()) == "http://ontology.dumontierlab.com/eswc-example-graph-3#datapoint1") {
					//	bool debug = true;
					//}

					CSatisfiableCalculationJob* satCalcJob = nullptr;
					CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getTemporaryRoleRealizationOntology());
					for (QList<CIndividualRoleCandidateTestingData>::const_iterator it = indiRoleCandTestDataList.constBegin(), itEnd = indiRoleCandTestDataList.constEnd(); it != itEnd; ++it) {
						CIndividualRoleCandidateTestingData indiRoleCandTestData(*it);
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(indiRoleCandTestData.getPropagationConcept(), false, indiRef, satCalcJob);
					}

					CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter* adapter = new CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter(indiRoleCandTestDataList, indiRef, reqConfPreCompItem->getOntology(), this);
					CIndividualRoleCandidatePropagationTestingItem* testItem = new CIndividualRoleCandidatePropagationTestingItem(reqConfPreCompItem, indiRealItemRef, indiRoleCandTestDataList, adapter);
					satCalcJob->setRealizationMarkedCandidatesMessageAdapter(adapter);
					satCalcJob->setCalculationConfiguration(reqConfPreCompItem->getCalculationConfiguration());
					//if (reqConfPreCompItem->requiresIndividualDependenceTracking()) {
					//	if (!individualItem->getIndividualDependenceTrackingCollector()) {
					//		individualItem->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
					//	}
					//}
					//if (individualItem->getIndividualDependenceTrackingCollector()) {
					//	satCalcJob->setSatisfiableTaskIndividualDependenceTrackingAdapter(new CSatisfiableTaskIndividualDependenceTrackingAdapter(individualItem->getIndividualDependenceTrackingCollector()));
					//}

					processCalculationJob(satCalcJob, reqConfPreCompItem, testItem);
					workCreated = true;
				}
				return workCreated;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createNextRoleInitializingTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem) {
				bool workCreated = false;

				COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator* iterator = roleInstancesItem->getIndividualComplexCandidateIterator();
				while (iterator && !workCreated && !iterator->atEnd()) {
					CRealizationIndividualInstanceItemReference indiRealItemRef = iterator->currentIndividualInstanceItemReference();
					workCreated = createNextRoleInitializingTest(reqConfPreCompItem, roleInstancesItem, false, indiRealItemRef);
					iterator->moveNext();
				}				


				if (iterator && iterator->atEnd()) {
					delete iterator;
					roleInstancesItem->setIndividualComplexCandidateIterator(nullptr);
					roleInstancesItem->setCandidateSuccessorInitializationCompleted(true);
				}

				return workCreated;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createNextRoleInitializingTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, const CRealizationIndividualInstanceItemReference& indiRealItemRef) {
				bool workCreated = false;
				cint64 indiId = indiRealItemRef.getIndividualID();

				//QString indiString = CIRIName::getRecentIRIName(indiRealItemRef.getIndividual()->getIndividualNameLinker());
				//QString roleString = CIRIName::getRecentIRIName(roleInstancesItem->getRole()->getPropertyNameLinker());
				//QString inverseRoleString = CIRIName::getRecentIRIName(roleInstancesItem->getInverseRole()->getPropertyNameLinker());
				//if (indiString == "http://purl.obolibrary.org/obo/CHROMOSOME-7227_X" && (roleString == "http://purl.obolibrary.org/obo/RO_0002131" || inverseRoleString == "http://purl.obolibrary.org/obo/RO_0002131")) {
				//	bool debug = true;
				//}


				COptimizedKPSetIndividualComplexRoleData* indiComplexRoleData = roleInstancesItem->getIndividualIdComplexRoleData(indiId, true);
				if (!indiComplexRoleData->isInitializing(inversed)) {
					indiComplexRoleData->setInitializing(inversed, true);
					QList<CIndividualRoleCandidateTestingData> indiRoleCandTestDataList;
					prepareRoleInitializingTestData(reqConfPreCompItem, roleInstancesItem, inversed, indiRealItemRef, indiComplexRoleData, indiRoleCandTestDataList);
					workCreated = createNextRoleInitializingTest(reqConfPreCompItem, indiRealItemRef, indiRoleCandTestDataList);
				}
				return workCreated;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingThread::prepareRoleInitializingTestData(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetIndividualComplexRoleData* indiComplexRoleData, QList<CIndividualRoleCandidateTestingData>& indiRoleCandTestDataList) {
				bool workCreated = false;
				cint64 indiId = indiRealItemRef.getIndividualID();


				//if (CIRIName::getRecentIRIName(indiRealItemRef.getIndividual()->getIndividualNameLinker()) == "http://ontology.dumontierlab.com/eswc-example-graph-3#datapoint1") {
				//	bool debug = true;
				//}


				bool directCollectable = true;
				if (roleInstancesItem->hasOnlyTransitiveComplexRoleCandidates() && roleInstancesItem->hasOnlyDeterministicComplexRoleStarterCandidatesUsage()) {
					for (TRoleItemInversionPair subRoleItemInversePair : *roleInstancesItem->getOnlyTransitiveComplexSubRoleItems()) {
						COptimizedKPSetRoleInstancesItem* subRoleInstancesItem = subRoleItemInversePair.first;
						bool subRoleItemInversed = subRoleItemInversePair.second ^ inversed;
						if (!subRoleInstancesItem->hasOnlyTransitiveComplexRoleCandidates() || !subRoleInstancesItem->hasOnlyDeterministicComplexRoleStarterCandidatesUsage()) {
							directCollectable = false;
							break;
						}
					}

					if (directCollectable) {
						bool linksTransfered = false;
						for (TRoleItemInversionPair subRoleItemInversePair : *roleInstancesItem->getOnlyTransitiveComplexSubRoleItems()) {
							COptimizedKPSetRoleInstancesItem* subRoleInstancesItem = subRoleItemInversePair.first;
							bool subRoleItemInversed = subRoleItemInversePair.second ^ inversed;

							COptimizedKPSetIndividualComplexRoleData* indiComplexSubRoleData = subRoleInstancesItem->getIndividualIdComplexRoleData(indiId, true);
							if (!indiComplexSubRoleData->isInitializing(subRoleItemInversed)) {
								indiComplexSubRoleData->setInitializing(subRoleItemInversed, true);
							}
							if (!indiComplexSubRoleData->isInitialized(subRoleItemInversed)) {
								collectTransitiveLinks(reqConfPreCompItem, subRoleInstancesItem, subRoleItemInversed, indiRealItemRef, indiComplexSubRoleData);
							}

							// TODO: if the (transitive) sub role always occurs in the same as the questioned role, then simplify every sub role relation (as long as there is only one) to the following
							if (subRoleInstancesItem == roleInstancesItem && inversed == subRoleItemInversed && roleInstancesItem->getOnlyTransitiveComplexSubRoleItems()->size() == 1) {
								linksTransfered = true;
							}
						}
						if (!linksTransfered) {
							// guaranteed to be deterministic, so no need to check possible neighbour instances
							COptimizedKPSetRoleNeighbourInstancesHashData* indiRoleNeighbourHashData = nullptr;

							COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* combinedNeighbourCacheLabelItemDataHash = roleInstancesItem->getCombinedNeighbourCacheLabelItemDataHash(inversed);
				
							CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(indiRealItemRef.getIndividualID());
							CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* assNeighbourRoleSetHash = indiAssData->getNeighbourRoleSetHash();
							CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCompinationLabelItem = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
							COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* combinedNeighbourCacheLabelItemData = combinedNeighbourCacheLabelItemDataHash->value(neighbourRoleSetCompinationLabelItem);


							for (TRoleItemInversionPair subRoleItemInversePair : *roleInstancesItem->getOnlyTransitiveComplexSubRoleItems()) {
								COptimizedKPSetRoleInstancesItem* subRoleInstancesItem = subRoleItemInversePair.first;
								bool subRoleItemInversed = subRoleItemInversePair.second ^ inversed;

								COptimizedKPSetIndividualComplexRoleData* indiComplexSubRoleData = subRoleInstancesItem->getIndividualIdComplexRoleData(indiId, true);
								COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRoleRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexRoleData;
								COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexSubRoleRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexSubRoleData;
								COptimizedKPSetRoleInstancesHash* subRoleComplexInstancesHash = indiExplicitIndirectLinkComplexSubRoleRepresentationData->getRoleNeighbourInstancesHash(inversed, true);
								for (QHash<cint64, COptimizedKPSetRoleInstancesHashData>::const_iterator it = subRoleComplexInstancesHash->constBegin(), itEnd = subRoleComplexInstancesHash->constEnd(); it != itEnd; ++it) {
									cint64 succIndiId = it.key();
									CRealizationIndividualInstanceItemReference succIndiItemRef = reqConfPreCompItem->getInstanceItemReference(succIndiId, true);
									addComplexRoleExplicitIndirectNeighbourLink(roleInstancesItem, inversed, indiRealItemRef, indiExplicitIndirectLinkComplexRoleRepresentationData, indiRoleNeighbourHashData, combinedNeighbourCacheLabelItemData, assNeighbourRoleSetHash, succIndiItemRef, nullptr, true, reqConfPreCompItem);
								}

							}
						}
						indiComplexRoleData->setInitialized(inversed, true);
					}
				} else {
					directCollectable = false;
				}
				if (!directCollectable) {
					if (!inversed) {
						indiRoleCandTestDataList.append(CIndividualRoleCandidateTestingData(roleInstancesItem->getTemporaryPropagationConcept(), roleInstancesItem->getTemporaryMarkerConcept(), indiComplexRoleData, roleInstancesItem, false));
					} else {
						indiRoleCandTestDataList.append(CIndividualRoleCandidateTestingData(roleInstancesItem->getTemporaryInversePropagationConcept(), roleInstancesItem->getTemporaryInverseMarkerConcept(), indiComplexRoleData, roleInstancesItem, true));
					}
					workCreated = true;
				}
				return workCreated;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingThread::collectTransitiveLinks(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetIndividualComplexRoleData* indiComplexRoleData) {
				bool workCreated = false;
				cint64 indiId = indiRealItemRef.getIndividualID();


#ifdef REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS
				mDebugTransitiveCollectionStringList.append(QString("Starting collection over %2 %3for %1").arg(CIRIName::getRecentIRIName(indiRealItemRef.getIndividual()->getIndividualNameLinker())).arg(CIRIName::getRecentIRIName(roleInstancesItem->getRole()->getPropertyNameLinker())).arg(inversed? "inversed " : ""));
				mDebugTransitiveCollectionString = mDebugTransitiveCollectionStringList.join("\r\n");
#endif // REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS
				//if (CIRIName::getRecentIRIName(indiRealItemRef.getIndividual()->getIndividualNameLinker()) == "http://ontology.dumontierlab.com/eswc-example-graph-3#datapoint1") {
				//	bool debug = true;
				//}


				if (roleInstancesItem->hasOnlyTransitiveComplexRoleCandidates() && roleInstancesItem->hasOnlyDeterministicComplexRoleStarterCandidatesUsage()) {

					class RecursiveComplexTransitiveProcessingData {
					public:
						~RecursiveComplexTransitiveProcessingData() {
							if (mAdditionalPropgationList) {
								delete mAdditionalPropgationList;
							}
						}

						CRealizationIndividualInstanceItemReference mIndiItemRef;
						COptimizedKPSetIndividualComplexRoleData* mIndiComplexRoleData = nullptr;
						CBackendRepresentativeMemoryCacheIndividualAssociationData* mIndiAssData = nullptr;

						bool mExpanded = false;

						RecursiveComplexTransitiveProcessingData* mParentData = nullptr;
						QList<RecursiveComplexTransitiveProcessingData*>* mAdditionalPropgationList = nullptr;
					};



					QList<RecursiveComplexTransitiveProcessingData*> processingList;
					RecursiveComplexTransitiveProcessingData* basicProcessingData = new RecursiveComplexTransitiveProcessingData();
					basicProcessingData->mIndiItemRef = indiRealItemRef;
					basicProcessingData->mIndiComplexRoleData = indiComplexRoleData;
					processingList.append(basicProcessingData);

					QList<RecursiveComplexTransitiveProcessingData*> propagationList;

					QHash<cint64, RecursiveComplexTransitiveProcessingData*> indiProcessingDataHash;
					indiProcessingDataHash.insert(indiRealItemRef.getIndividualID(), basicProcessingData);

					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* combinedNeighbourCacheLabelItemDataHash = roleInstancesItem->getCombinedNeighbourCacheLabelItemDataHash(inversed);

					while (!processingList.isEmpty()) {
						RecursiveComplexTransitiveProcessingData* nextProcessingData = processingList.takeFirst();

#ifdef REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS
						mDebugTransitiveCollectionStringList.append(QString("Analysing %1").arg(CIRIName::getRecentIRIName(nextProcessingData->mIndiItemRef.getIndividual()->getIndividualNameLinker())));
						mDebugTransitiveCollectionString = mDebugTransitiveCollectionStringList.join("\r\n");
#endif // REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS

						propagationList.append(nextProcessingData);
						if (!nextProcessingData->mIndiComplexRoleData->isInitialized(inversed)) {

							CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(nextProcessingData->mIndiItemRef.getIndividualID());
							nextProcessingData->mIndiAssData = indiAssData;
							CBackendRepresentativeMemoryLabelCacheItem* combinedNeigRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();

#ifdef REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS
							mDebugTransitiveCollectionStringList.append(QString("Analysing neighours of %1").arg(CIRIName::getRecentIRIName(nextProcessingData->mIndiItemRef.getIndividual()->getIndividualNameLinker())));
							mDebugTransitiveCollectionString = mDebugTransitiveCollectionStringList.join("\r\n");
#endif // REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS


							COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* roleItemCompNeighLabelData = roleInstancesItem->getCombinedNeighbourCacheLabelItemDataHash(inversed)->value(combinedNeigRoleSetLabel);
							if (roleItemCompNeighLabelData && roleItemCompNeighLabelData->hasKnownInstancesLabelItems()) {
								QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* singleNeighLabelDataHash = roleItemCompNeighLabelData->getKnownInstancesLabelItemDataHash();

								for (auto it = singleNeighLabelDataHash->constBegin(), itEnd = singleNeighLabelDataHash->constEnd(); it != itEnd; ++it) {
									roleSetNeighbourArray->at(it.value()->getLabelArrayIndex()).visitNeighbourIndividualIds([&](cint64 neighbourId)->bool {
										// check same individual merged
										CRealizationIndividualInstanceItemReference neighbourIndiItemRef = reqConfPreCompItem->getInstanceItemReference(neighbourId, false);
										if (!reqConfPreCompItem->isSameIndividualsMerged(neighbourIndiItemRef)) {
											// create new processing data

											RecursiveComplexTransitiveProcessingData*& successorProcessingData = indiProcessingDataHash[neighbourId];

											if (successorProcessingData) {
												// TODO: check and handle cycles
												if (!successorProcessingData->mParentData) {
													successorProcessingData->mParentData = nextProcessingData;


#ifdef REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS
													mDebugTransitiveCollectionStringList.append(QString("Readding %1 as single").arg(CIRIName::getRecentIRIName(neighbourIndiItemRef.getIndividual()->getIndividualNameLinker())));
													mDebugTransitiveCollectionString = mDebugTransitiveCollectionStringList.join("\r\n");
#endif // REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS

												} else {
													if (!successorProcessingData->mAdditionalPropgationList) {
														successorProcessingData->mAdditionalPropgationList = new QList<RecursiveComplexTransitiveProcessingData*>();
													}
													successorProcessingData->mAdditionalPropgationList->append(nextProcessingData);

#ifdef REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS
													mDebugTransitiveCollectionStringList.append(QString("Readding %1 as multiple").arg(CIRIName::getRecentIRIName(neighbourIndiItemRef.getIndividual()->getIndividualNameLinker())));
													mDebugTransitiveCollectionString = mDebugTransitiveCollectionStringList.join("\r\n");
#endif // REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS

												}

											} else {
												successorProcessingData = new RecursiveComplexTransitiveProcessingData();
												successorProcessingData->mIndiItemRef = neighbourIndiItemRef;

#ifdef REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS
												mDebugTransitiveCollectionStringList.append(QString("Adding %1").arg(CIRIName::getRecentIRIName(neighbourIndiItemRef.getIndividual()->getIndividualNameLinker())));
												mDebugTransitiveCollectionString = mDebugTransitiveCollectionStringList.join("\r\n");
#endif // REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS

												successorProcessingData->mIndiComplexRoleData = roleInstancesItem->getIndividualIdComplexRoleData(neighbourIndiItemRef.getIndividualID(), true);
												successorProcessingData->mParentData = nextProcessingData;
												successorProcessingData->mIndiComplexRoleData->setInitializing(inversed, true);
												processingList.append(successorProcessingData);
											}

										}
										return true;
									});
								}
							}
						}
					}


					while (!propagationList.isEmpty()) {
						RecursiveComplexTransitiveProcessingData* nextProcessingData = propagationList.takeLast();


						//if (CIRIName::getRecentIRIName(nextProcessingData->mIndiItemRef.getIndividual()->getIndividualNameLinker()) == "http://ontology.dumontierlab.com/eswc-example-graph-3#plot") {
						//	bool debug = true;
						//}


						if (nextProcessingData->mParentData) {
							QList<RecursiveComplexTransitiveProcessingData*> parentDataList;
							parentDataList.append(nextProcessingData->mParentData);
							if (nextProcessingData->mAdditionalPropgationList) {
								parentDataList.append(*nextProcessingData->mAdditionalPropgationList);
							}

							QList<RecursiveComplexTransitiveProcessingData*> propagationDataList;
							for (RecursiveComplexTransitiveProcessingData* parentData : parentDataList) {
								if (!nextProcessingData->mExpanded) {
									propagationDataList.append(parentData);
								} else {
									if (parentData->mParentData) {
										propagationDataList.append(parentData->mParentData);
									}
									if (parentData->mAdditionalPropgationList) {
										propagationDataList.append(*parentData->mAdditionalPropgationList);
									}
								}
							}

							if (!propagationDataList.isEmpty()) {

								// TODO: add successor itself

								QList<CRealizationIndividualInstanceItemReference> linkAddingSuccessorIndividualReferenceList;
								linkAddingSuccessorIndividualReferenceList.append(nextProcessingData->mIndiItemRef);


								if (!nextProcessingData->mExpanded) {
									CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = reqConfPreCompItem->getBackendAssociationCacheReader()->getIndividualAssociationData(nextProcessingData->mIndiItemRef.getIndividualID());
									nextProcessingData->mIndiAssData = indiAssData;
									CBackendRepresentativeMemoryLabelCacheItem* combinedNeigRoleSetLabel = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();

									COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* roleItemCompNeighLabelData = roleInstancesItem->getCombinedNeighbourCacheLabelItemDataHash(inversed)->value(combinedNeigRoleSetLabel);
									if (roleItemCompNeighLabelData && roleItemCompNeighLabelData->hasKnownInstancesLabelItems()) {
										QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* singleNeighLabelDataHash = roleItemCompNeighLabelData->getKnownInstancesLabelItemDataHash();

										for (auto it = singleNeighLabelDataHash->constBegin(), itEnd = singleNeighLabelDataHash->constEnd(); it != itEnd; ++it) {
											roleSetNeighbourArray->at(it.value()->getLabelArrayIndex()).visitNeighbourIndividualIds([&](cint64 neighbourId)->bool {
												// check same individual merged
												CRealizationIndividualInstanceItemReference neighbourIndiItemRef = reqConfPreCompItem->getInstanceItemReference(neighbourId, false);
												if (!reqConfPreCompItem->isSameIndividualsMerged(neighbourIndiItemRef)) {
													linkAddingSuccessorIndividualReferenceList.append(neighbourIndiItemRef);
												}
												return true;
											});
										}
									}
								}

								COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* successorIndiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)nextProcessingData->mIndiComplexRoleData;
								COptimizedKPSetRoleInstancesHash* succInstanceDatahash = successorIndiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(inversed, false);
								if (succInstanceDatahash) {
									for (COptimizedKPSetRoleInstancesHash::iterator it = succInstanceDatahash->begin(), itEnd = succInstanceDatahash->end(); it != itEnd; ++it) {
										cint64 indiId = it.key();
										COptimizedKPSetRoleInstancesHashData& instanceHashData = it.value();
										if (instanceHashData.mInstanceItemData->mKnownInstance) {

											CRealizationIndividualInstanceItemReference succIndiItemRef = reqConfPreCompItem->getInstanceItemReference(indiId, true);
											linkAddingSuccessorIndividualReferenceList.append(succIndiItemRef);
										}
									}
								}


								for (CRealizationIndividualInstanceItemReference succIndiItemRef : linkAddingSuccessorIndividualReferenceList) {


									QSet<RecursiveComplexTransitiveProcessingData*> propagatedDataSet;
									QList<RecursiveComplexTransitiveProcessingData*> processingPropagationDataList(propagationDataList);
									while (!processingPropagationDataList.isEmpty()) {
										RecursiveComplexTransitiveProcessingData* nextPropagationData = processingPropagationDataList.takeFirst();

										if (!propagatedDataSet.contains(nextPropagationData)) {
											propagatedDataSet.insert(nextPropagationData);


											COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)nextPropagationData->mIndiComplexRoleData;
											CRealizationIndividualInstanceItemReference indiRealItemRef = nextPropagationData->mIndiItemRef;
											CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = nextPropagationData->mIndiAssData;

											// guaranteed to be deterministic, so no need to check possible neighbour instances
											COptimizedKPSetRoleNeighbourInstancesHashData* indiRoleNeighbourHashData = nullptr;

											CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* assNeighbourRoleSetHash = indiAssData->getNeighbourRoleSetHash();
											CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCompinationLabelItem = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
											COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* combinedNeighbourCacheLabelItemData = combinedNeighbourCacheLabelItemDataHash->value(neighbourRoleSetCompinationLabelItem);


											bool addedLink = addComplexRoleExplicitIndirectNeighbourLink(roleInstancesItem, inversed, indiRealItemRef, indiExplicitIndirectLinkComplexRepresentationData, indiRoleNeighbourHashData, combinedNeighbourCacheLabelItemData, assNeighbourRoleSetHash, succIndiItemRef, nullptr, true, reqConfPreCompItem);

											if (nextPropagationData->mParentData) {
												processingPropagationDataList.append(nextPropagationData->mParentData);
											}
											if (nextPropagationData->mAdditionalPropgationList) {
												processingPropagationDataList.append(*nextPropagationData->mAdditionalPropgationList);
											}
										}
									}
								}

							}
						}
					}



					for (auto processingData : indiProcessingDataHash) {
						processingData->mIndiComplexRoleData->setInitialized(inversed, true);
						delete processingData;
					}
					

					
					return true;
				}
				return false;
			}














			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createNextRoleComplexCandidateConfirmationTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstancesItem, COptimizedKPSetIndividualItemReferencePair candidatePair, COntologyRealizingDynamicRequirmentProcessingData* procData) {
				bool workCreated = false;
				CRealizationIndividualInstanceItemReference item1Ref = candidatePair.first;
				CRealizationIndividualInstanceItemReference item2Ref = candidatePair.second;

				reqConfPreCompItem->incTestingRoleInstanceCandidatesCount();


				COptimizedKPSetIndividualComplexRoleData* indiComplexData = roleInstancesItem->getIndividualIdComplexRoleData(item1Ref.getIndividualID(), true);
				COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;

				COptimizedKPSetRoleInstancesData* instanceData = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstanceItemData(false, item2Ref.getIndividualID(), true);
				if (!instanceData->mTestedInstance && !instanceData->mTestingInstance && !instanceData->mKnownInstance) {
					instanceData->mTestingInstance = true;

					CSatisfiableCalculationJob* satCalcJob = nullptr;
					CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getTemporaryRoleRealizationOntology());
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(roleInstancesItem->getTemporaryPropagationConcept(), false, item1Ref, satCalcJob);
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(roleInstancesItem->getTemporaryMarkerConcept(), true, item2Ref, satCalcJob);

					CIndividualRoleCandidateConfirmationTestingItem* testItem = new CIndividualRoleCandidateConfirmationTestingItem(reqConfPreCompItem, item1Ref, item2Ref, roleInstancesItem, procData);
					processCalculationJob(satCalcJob, reqConfPreCompItem, testItem);
					workCreated = true;
				}

				return workCreated;
			}
















			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createIndividualsConsistencyTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {

				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getOntology());


				CIndividualVector* indiVec = reqConfPreCompItem->getOntology()->getABox()->getIndividualVector(false);
				cint64 indiCount = 0;
				if (indiVec) {
					indiCount = indiVec->getItemCount();


					QList<CIndividual*> indiList;
					for (cint64 idx = 0; idx < indiCount; ++idx) {
						CIndividual* indi = indiVec->getData(idx);
						indiList.append(indi);
					}
					CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getOntology());
					if (!indiList.isEmpty()) {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(indiList);
						satCalcJob->setCalculationConfiguration(reqConfPreCompItem->getCalculationConfiguration());
						if (reqConfPreCompItem->getPossibleAssertionCollectionSet()) {
							satCalcJob->setPossibleAssertionCollectionAdapter(new CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter(reqConfPreCompItem->getOntology(),reqConfPreCompItem->getPossibleAssertionCollectionSet()));
						}

						CIndividualsConsistencyTestingItem* testItem = new CIndividualsConsistencyTestingItem(reqConfPreCompItem);
						processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
						return true;
					}
				}

				return false;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createNextSameIndividualsTest(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* instantiatedItem1, COptimizedKPSetIndividualItem* instantiatedItem2, COntologyRealizingDynamicRequirmentProcessingData* procData) {
				instantiatedItem1->incTestingPossibleSameIndividualCount();
				instantiatedItem2->incTestingPossibleSameIndividualCount();

				bool temporaryNominalConcept = false;
				CIndividual* individual1 = instantiatedItem1->getIndividual();
				CIndividual* individual2 = instantiatedItem2->getIndividual();
				CConcept* nominalConcept2 = nullptr;
				if (individual2) {
					nominalConcept2 = individual2->getIndividualNominalConcept();
				}
				if (!nominalConcept2) {
					nominalConcept2 = instantiatedItem2->getTemporaryIndividualNominalConcept();
					temporaryNominalConcept = true;
				}

				CConcept* testingNominalConcept = nominalConcept2;
				CIndividualReference testingIndividualReference = instantiatedItem1->getIndividualReference();
				CIndividualReference otherTestringIndividualReference = instantiatedItem2->getIndividualReference();

				if (!testingNominalConcept) {
					testingIndividualReference = instantiatedItem2->getIndividualReference();
					otherTestringIndividualReference = instantiatedItem1->getIndividualReference();
					temporaryNominalConcept = false;
					testingNominalConcept = nullptr;
					if (individual1) {
						testingNominalConcept = individual1->getIndividualNominalConcept();
					}
					if (!testingNominalConcept) {
						temporaryNominalConcept = true;
						testingNominalConcept = instantiatedItem1->getTemporaryIndividualNominalConcept();
					}
				}


				if (testingNominalConcept) {
					CSatisfiableCalculationJob* satCalcJob = nullptr;
					CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getOntology());
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(testingNominalConcept,true,testingIndividualReference);

					if (temporaryNominalConcept) {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(testingNominalConcept,false,otherTestringIndividualReference, satCalcJob);
					}

					satCalcJob->setCalculationConfiguration(reqConfPreCompItem->getCalculationConfiguration());
					if (reqConfPreCompItem->getPossibleAssertionCollectionSet()) {
						satCalcJob->setPossibleAssertionCollectionAdapter(new CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter(reqConfPreCompItem->getOntology(),reqConfPreCompItem->getPossibleAssertionCollectionSet()));
					}
					if (reqConfPreCompItem->requiresIndividualDependenceTracking()) {
						if (!instantiatedItem1->getIndividualDependenceTrackingCollector()) {
							instantiatedItem1->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
						}
					}
					if (instantiatedItem1->getIndividualDependenceTrackingCollector()) {
						satCalcJob->setSatisfiableTaskIndividualDependenceTrackingAdapter(new CSatisfiableTaskIndividualDependenceTrackingAdapter(instantiatedItem1->getIndividualDependenceTrackingCollector()));
					}

					CIndividualSameTestingItem* testItem = new CIndividualSameTestingItem(reqConfPreCompItem,instantiatedItem1,instantiatedItem2, procData);
					reqConfPreCompItem->incTestingPossibleSameIndividualCount();

					processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
					return true;
				} else {
					LOG(ERROR,getDomain(),logTr("Failed testing whether '%1' and '%2' are same individuals, nominal concepts are not available.").arg(CIRIName::getRecentIRIName(individual1->getIndividualNameLinker())).arg(CIRIName::getRecentIRIName(individual2->getIndividualNameLinker())),this);
					return false;
				}
			}




			COptimizedKPSetIndividualItem* COptimizedRepresentativeKPSetOntologyRealizingThread::getMergingResolvedIndividualItem(COptimizedKPSetIndividualItem* indiItem) {
				while (indiItem->isItemSameIndividualMerged()) {
					indiItem = indiItem->getItemSameIndividualMerged();
				}
				return indiItem;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingThread::createNextTest() {


				COntologyRealizingItem* loopOntPreCompItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !mProcessingOntItemList.isEmpty()) {
					COntologyRealizingItem* ontPreCompItem = mProcessingOntItemList.first();

					if (ontPreCompItem == loopOntPreCompItem) {
						// don't run into infinite loop without doing something
						break;
					}

					COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem = (COptimizedRepresentativeKPSetOntologyRealizingItem*)ontPreCompItem;



					if (!workTestCreated && reqConfPreCompItem->isExtraConsistencyTestingStepRequired()) {
						if (!reqConfPreCompItem->isExtraConsistencyTesting() && !reqConfPreCompItem->isExtraConsistencyTested()) {
							// create consistency test
							if (createIndividualsConsistencyTest(reqConfPreCompItem)) {
								reqConfPreCompItem->setExtraConsistencyTesting(true);
								workTestCreated = true;
							} else {
								reqConfPreCompItem->setExtraConsistencyTested(true);
							}
						}
					}


					if (!workTestCreated && reqConfPreCompItem->isRealizeSameIndividualsStepRequired() && !reqConfPreCompItem->isExtraConsistencyTesting()) {
						if (!reqConfPreCompItem->isRealizeSameIndividualsStepFinished()) {
							if (reqConfPreCompItem->areRealizeSameIndividualsStepProcessingRequirementSatisfied()) {

								if (!reqConfPreCompItem->hasRealizationSameIndividualsInitialized()) {
									createTemporarySameRealizationOntology(reqConfPreCompItem);
									reqConfPreCompItem->setRealizationSameIndividualsInitialized(true);

									if (reqConfPreCompItem->isAllSameIndividualsProcessing()) {
										LOG(INFO, getDomain(), logTr("Realize same individuals for ontology."), this);
									}
								}




								while (!workTestCreated && reqConfPreCompItem->hasEntailmentIndividualsSameInstanceTestingItems()) {
									QList<CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem*>* entIndSameInstTestList = reqConfPreCompItem->getEntailmentIndividualsSameInstanceTestingItemList();
									CRealizationEntailmentQueuedIndividualsSameInstanceTestingItem* sameInstTestItem = entIndSameInstTestList->takeFirst();
									COptimizedKPSetIndividualItem* indiItem1 = sameInstTestItem->getIndividualSourceItem();
									COptimizedKPSetIndividualItem* indiItem2 = sameInstTestItem->getIndividualDestinationItem();
									COntologyRealizingDynamicRequirmentProcessingData* procData = sameInstTestItem->getProcessingData();
									indiItem1 = getMergingResolvedIndividualItem(indiItem1);
									indiItem2 = getMergingResolvedIndividualItem(indiItem2);
									if (!indiItem1->isItemSameIndividualMerged() && !indiItem2->isItemSameIndividualMerged() && indiItem1 != indiItem2 && indiItem1->getPossibleSameInstantiatedItemSet()->contains(indiItem2)) {
										indiItem1->getPossibleSameInstantiatedItemSet()->remove(indiItem2);
										workTestCreated = createNextSameIndividualsTest(reqConfPreCompItem, indiItem1, indiItem2, procData);
									} else if (procData) {
										setDynamicRequirementProcessed(ontPreCompItem, procData);
									}
									delete sameInstTestItem;
								}



								if (reqConfPreCompItem->hasRemainingProcessingSameIndividualsItems()) {										
									QList<COptimizedKPSetIndividualItem*>* processingSameIndividualItemList = reqConfPreCompItem->getProcessingPossibleSameIndividualsItemList();
									while (!workTestCreated && !processingSameIndividualItemList->isEmpty()) {
										COptimizedKPSetIndividualItem* nextProcessingItem = processingSameIndividualItemList->first();
										COptimizedKPSetIndividualItem* nextPossibleSameIndividualTestItem = nullptr;

										if (!nextProcessingItem->isItemSameIndividualMerged()) {

											nextPossibleSameIndividualTestItem = nextProcessingItem->takeTestingPossibleSameIndividualItem();
											if (nextPossibleSameIndividualTestItem) {
												nextPossibleSameIndividualTestItem->removePossibleSameIndividualItem(nextProcessingItem);

												if (!nextPossibleSameIndividualTestItem->isItemSameIndividualMerged()) {
													workTestCreated = createNextSameIndividualsTest(reqConfPreCompItem,nextProcessingItem,nextPossibleSameIndividualTestItem);
												}
											}

										}
										if (nextProcessingItem->isItemSameIndividualMerged() || !nextProcessingItem->hasPossibleSameIndividuals()) {
											processingSameIndividualItemList->removeFirst();
											nextProcessingItem->setPossibleSameIndividualsProcessingQueuedFlag(false);
										}
										checkFinishSameIndividualProcessing(ontPreCompItem, nextProcessingItem);
										checkFinishSameIndividualProcessing(ontPreCompItem, nextPossibleSameIndividualTestItem);

									}
								}

									
								if (!workTestCreated && !reqConfPreCompItem->hasRemainingProcessingSameIndividualsItems()) {										
									if (!reqConfPreCompItem->hasTestingPossibleSameIndividual()) {


										if (reqConfPreCompItem->isAllSameIndividualsProcessing()) {
											if (!reqConfPreCompItem->isSameRealizationInstalled()) {
												reqConfPreCompItem->setSameRealizationInstalled(true);

												CRealizingTestingStep* ontProcStep = reqConfPreCompItem->getRealizeSameIndividualsProcessingStep();
												addRealizationStatistics(reqConfPreCompItem, ontProcStep);

												LOG(INFO, getDomain(), logTr("Same individual realization finished for ontology."), this);
												reqConfPreCompItem->getRealizeSameIndividualsProcessingStep()->setStepFinished(true);
											}
										}
										reqConfPreCompItem->getRealizeSameIndividualsProcessingStep()->submitRequirementsUpdate();
									}
								}
							} else {
								reqConfPreCompItem->getRealizeSameIndividualsProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							reqConfPreCompItem->getRealizeSameIndividualsProcessingStep()->submitRequirementsUpdate();
						}
					}



					if (!workTestCreated && reqConfPreCompItem->isRealizeConceptStepRequired() && !reqConfPreCompItem->isExtraConsistencyTesting()) {
						if (!reqConfPreCompItem->isRealizeConceptStepFinished()) {
							if (reqConfPreCompItem->areRealizeConceptStepProcessingRequirementSatisfied()) {

								if (!reqConfPreCompItem->hasRealizationConceptsInitialized()) {
									if (!reqConfPreCompItem->isCountingPossibleConceptInstancesTesting()) {
										mOpenPossibleInstancesCount += reqConfPreCompItem->getOpenPossibleConceptInstancesCount();
										mTestedPossibleInstancesCount += reqConfPreCompItem->getTestedPossibleConceptInstancesCount();
										reqConfPreCompItem->setCountingPossibleConceptInstancesTesting(true);
									}
									reqConfPreCompItem->setRealizationConceptsInitialized(true);


									if (reqConfPreCompItem->isAllConceptInstancesProcessing()) {
										LOG(INFO, getDomain(), logTr("Realize individual types for ontology."), this);
									}
								}

								while (!workTestCreated && (reqConfPreCompItem->hasEntailmentIndividualConceptInstanceTestingItems() || reqConfPreCompItem->hasRemainingProcessingConceptInstanceItems() || (!reqConfPreCompItem->hasTestingPossibleConceptInstances() && reqConfPreCompItem->isRealizeConceptStepRequired()))) {

									while (!workTestCreated && reqConfPreCompItem->hasEntailmentIndividualConceptInstanceTestingItems()) {
										QList<CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem*>* entIndConInstTestList = reqConfPreCompItem->getEntailmentIndividualConceptInstanceTestingItemList();
										CRealizationEntailmentQueuedIndividualConceptInstanceTestingItem* conInstTestItem = entIndConInstTestList->takeFirst();
										COptimizedKPSetConceptInstancesItem* conceptItem = conInstTestItem->getConceptItem();
										COptimizedKPSetIndividualItem* indiItem = conInstTestItem->getIndividualItem();
										COntologyRealizingDynamicRequirmentProcessingData* procData = conInstTestItem->getProcessingData();
										indiItem = getMergingResolvedIndividualItem(indiItem);
										if (!indiItem->isItemSameIndividualMerged() && conceptItem->getPossibleInstancesMap()->contains(indiItem->getIndividualId())) {
											conceptItem->getPossibleInstancesMap()->remove(indiItem->getIndividualId());
											workTestCreated = createNextConceptInstantiationTest(reqConfPreCompItem, conceptItem, indiItem, procData);
										} else if (procData) {
											setDynamicRequirementProcessed(ontPreCompItem, procData);
										}
										delete conInstTestItem;
									}

									if (!workTestCreated && reqConfPreCompItem->hasRemainingProcessingConceptInstanceItems()) {
										QList<COptimizedKPSetConceptInstancesItem*>* processingInstancesItemList = reqConfPreCompItem->getProcessingPossibleConceptInstancesItemList();
										while (!workTestCreated && !processingInstancesItemList->isEmpty()) {
											COptimizedKPSetConceptInstancesItem* nextProcessingItem = processingInstancesItemList->first();

											if (nextProcessingItem->hasPossibleInstances()) {
												COptimizedKPSetIndividualItem* nextInstantiatedTestItem = nextProcessingItem->takeNextTestingPossibleInstance();

												if (!nextInstantiatedTestItem->isItemSameIndividualMerged()) {
													workTestCreated = createNextConceptInstantiationTest(reqConfPreCompItem, nextProcessingItem, nextInstantiatedTestItem);
												} else {
													incTestedPossibleConceptInstancesCount(reqConfPreCompItem);
												}
											}
											if (!nextProcessingItem->hasPossibleInstances()) {
												processingInstancesItemList->removeFirst();
												nextProcessingItem->setPossibleInstancesProcessingQueuedFlag(false);
											} else if (workTestCreated && mAlternateShufflePossibleConceptInstanceTestingItemsOnCalculations) {
												processingInstancesItemList->removeFirst();
												processingInstancesItemList->append(nextProcessingItem);
											}
											checkFinishConceptInstancesProcessing(ontPreCompItem, nextProcessingItem);

										}
									}

									if (!workTestCreated && !reqConfPreCompItem->hasRemainingProcessingConceptInstanceItems() && !reqConfPreCompItem->hasEntailmentIndividualConceptInstanceTestingItems()) {
										if (!reqConfPreCompItem->hasTestingPossibleConceptInstances()) {

											if (reqConfPreCompItem->isAllConceptInstancesProcessing()) {
												if (!reqConfPreCompItem->isConceptRealizationInstalled()) {
													reqConfPreCompItem->setConceptRealizationInstalled(true);

													CRealizingTestingStep* ontProcStep = reqConfPreCompItem->getRealizeConceptProcessingStep();
													addRealizationStatistics(reqConfPreCompItem, ontProcStep);

													LOG(INFO, getDomain(), logTr("Concept realization finished for ontology."), this);
													if (reqConfPreCompItem->getProvidedPossibleInstancesMergingsCount() > 0) {
														LOG(INFO, getDomain(), logTr("Provided %1 possible instance mergings containing %2 individuals, %3 mergings attempted and %4 succeeded.").arg(reqConfPreCompItem->getProvidedPossibleInstancesMergingsCount()).arg(reqConfPreCompItem->getProvidedPossibleInstancesMergingIndividualsCount()).arg(reqConfPreCompItem->getAttemptedPossibleInstancesMergingsCount()).arg(reqConfPreCompItem->getSucceddedPossibleInstancesMergingsCount()), this);
													}

													if (reqConfPreCompItem->getModelMergingsTriedCount() > 0) {
														LOG(INFO, getDomain(), logTr("Tried %1 trivial pseudo model mergings, %2 succeeded by determing %3 instances and %4 non-instances.").arg(reqConfPreCompItem->getModelMergingsTriedCount()).arg(reqConfPreCompItem->getModelMergingsSucessCount()).arg(reqConfPreCompItem->getModelMergingsInstanceFoundCount()).arg(reqConfPreCompItem->getModelMergingsNonInstanceFoundCount()), this);
													}
													reqConfPreCompItem->getRealizeConceptProcessingStep()->setStepFinished(true);
												}
											}
											reqConfPreCompItem->getRealizeConceptProcessingStep()->submitRequirementsUpdate();
										}
									}
								}
							} else {
								reqConfPreCompItem->getRealizeConceptProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							reqConfPreCompItem->getRealizeConceptProcessingStep()->submitRequirementsUpdate();
						}
					}





					if (!workTestCreated && reqConfPreCompItem->isRealizeRoleStepRequired() && !reqConfPreCompItem->isExtraConsistencyTesting()) {
						if (!reqConfPreCompItem->isRealizeRoleStepFinished()) {
							if (reqConfPreCompItem->areRealizeRoleStepProcessingRequirementSatisfied()) {

								if (!reqConfPreCompItem->hasRealizationRolesInitialized()) {
									LOG(INFO,getDomain(),logTr("Create temporary ontology extension for role realization."),this);
									createTemporaryRoleRealizationOntology(reqConfPreCompItem);
									//reqConfPreCompItem->initRoleCandidateInitializingIndividualItems();
									reqConfPreCompItem->setRealizationRolesInitialized(true);
									if (!reqConfPreCompItem->isCountingPossibleRoleInstancesTesting()) {
										mOpenPossibleInstancesCount += reqConfPreCompItem->getOpenPossibleRoleInstancesCount();
										mTestedPossibleInstancesCount += reqConfPreCompItem->getTestedPossibleRoleInstancesCount();
										reqConfPreCompItem->setCountingPossibleRoleInstancesTesting(true);
									}

									if (!reqConfPreCompItem->isRoleRealizationInstalled()) {
										LOG(INFO, getDomain(), logTr("Realizing role instances for ontology."), this);
									}
								}





								if (reqConfPreCompItem->hasRemainingCandidateConfirmationRoleInstanceItems()) {
									QList<COptimizedKPSetRoleInstancesItem*>* candidateConfirmationRoleInstancesItemList = reqConfPreCompItem->getCandidateConfirmationRoleInstancesItemList();
									while (!workTestCreated && !candidateConfirmationRoleInstancesItemList->isEmpty()) {
										COptimizedKPSetRoleInstancesItem* nextCandidateConfirmationRoleItem = candidateConfirmationRoleInstancesItemList->first();

										if (nextCandidateConfirmationRoleItem->hasComplexInstanceCandidates()) {
											COptimizedKPSetIndividualItemReferencePair candidatePair = nextCandidateConfirmationRoleItem->takeNextTestingComplexInstanceCandidate();
											workTestCreated = createNextRoleComplexCandidateConfirmationTest(reqConfPreCompItem, nextCandidateConfirmationRoleItem, candidatePair);
										}
										if (!nextCandidateConfirmationRoleItem->hasComplexInstanceCandidates()) {
											candidateConfirmationRoleInstancesItemList->removeFirst();
										}
									}
								}




								if (reqConfPreCompItem->hasRemainingInitializingRoleInstanceItems()) {										
									QList<COptimizedKPSetRoleInstancesItem*>* initializingInstancesItemList = reqConfPreCompItem->getInitializingRoleInstancesItemList();
									while (!workTestCreated && !initializingInstancesItemList->isEmpty()) {
										COptimizedKPSetRoleInstancesItem* nextInitializingItem = initializingInstancesItemList->first();

										if (!nextInitializingItem->isCandidateSuccessorInitializationCompleted()) {


											if (!nextInitializingItem->isCandidateSuccessorInitializationStarted()) {

												QSet<CBackendRepresentativeMemoryLabelCacheItem*>* neighbourRoleLabelCacheItemSet = nextInitializingItem->getComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet(false);
												QSet<CBackendRepresentativeMemoryLabelCacheItem*>* existentialRoleLabelCacheItemSet = nextInitializingItem->getComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet(false);

												QList<CBackendRepresentativeMemoryLabelCacheItem*> roleLabelCacheItemList;
												roleLabelCacheItemList.append(neighbourRoleLabelCacheItemSet->toList());
												roleLabelCacheItemList.append(existentialRoleLabelCacheItemSet->toList());
												COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator* iterator = new COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator(roleLabelCacheItemList, reqConfPreCompItem->getIndividualInstantiatedItemHash(), reqConfPreCompItem->getOntology()->getABox()->getIndividualVector());
												nextInitializingItem->setIndividualComplexCandidateIterator(iterator);

												nextInitializingItem->setCandidateSuccessorInitializationStarted(true);
											}

											workTestCreated = createNextRoleInitializingTest(reqConfPreCompItem, nextInitializingItem);

										}
										if (nextInitializingItem->isCandidateSuccessorInitializationCompleted()) {
											initializingInstancesItemList->removeFirst();
										}
									}
								} 


								while (!workTestCreated && (reqConfPreCompItem->hasRemainingInitializingRoleInstanceIndividualItemReferences() || reqConfPreCompItem->hasRemainingInitializingRolePredeccessorInstanceIndividualItemReferencePairs() || 
										(reqConfPreCompItem->hasEntailmentIndividualsRoleInstanceTestingItems() && !reqConfPreCompItem->hasRemainingInitializingRoleInstanceItems() && !reqConfPreCompItem->hasPropagatingRoleInstanceCandidates()))) {

									if (reqConfPreCompItem->hasRemainingInitializingRoleInstanceIndividualItemReferences()) {										
										QList<CRealizationIndividualInstanceItemReference>* initializingIndividualItemList = reqConfPreCompItem->getInitializingRoleInstancesIndividualItemReferenceList();
										while (!workTestCreated && !initializingIndividualItemList->isEmpty()) {
											CRealizationIndividualInstanceItemReference nextInitializingItem = initializingIndividualItemList->first();
										
											workTestCreated = createNextRoleInitializingTest(reqConfPreCompItem, nextInitializingItem);

											initializingIndividualItemList->removeFirst();
										}
									} 


									if (reqConfPreCompItem->hasRemainingInitializingRoleSuccessorInstanceIndividualItemReferencePairs()) {
										QList<QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*>>* initializingIndividualItemPairList = reqConfPreCompItem->getInitializingRoleSuccessorInstancesIndividualItemReferencePairList();
										while (!workTestCreated && !initializingIndividualItemPairList->isEmpty()) {
											QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*> nextInitializingItemPair = initializingIndividualItemPairList->first();
											CRealizationIndividualInstanceItemReference nextInitializingItem = nextInitializingItemPair.first;
											COptimizedKPSetRoleInstancesItem* roleIntancesItem = nextInitializingItemPair.second;

											workTestCreated = createNextRoleInitializingTest(reqConfPreCompItem, roleIntancesItem, false, nextInitializingItem);

											initializingIndividualItemPairList->removeFirst();
										}
									}


									if (reqConfPreCompItem->hasRemainingInitializingRolePredeccessorInstanceIndividualItemReferencePairs()) {
										QList<QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*>>* initializingIndividualItemPairList = reqConfPreCompItem->getInitializingRolePredecessorInstancesIndividualItemReferencePairList();
										while (!workTestCreated && !initializingIndividualItemPairList->isEmpty()) {
											QPair<CRealizationIndividualInstanceItemReference, COptimizedKPSetRoleInstancesItem*> nextInitializingItemPair = initializingIndividualItemPairList->first();
											CRealizationIndividualInstanceItemReference nextInitializingItem = nextInitializingItemPair.first;
											COptimizedKPSetRoleInstancesItem* roleIntancesItem = nextInitializingItemPair.second;



											workTestCreated = createNextRoleInitializingTest(reqConfPreCompItem, roleIntancesItem, true, nextInitializingItem);

											initializingIndividualItemPairList->removeFirst();
										}
									}

									while (!workTestCreated && reqConfPreCompItem->hasEntailmentIndividualsRoleInstanceTestingItems() && !reqConfPreCompItem->hasRemainingInitializingRoleInstanceItems() && !reqConfPreCompItem->hasPropagatingRoleInstanceCandidates() &&
											!reqConfPreCompItem->hasRemainingInitializingRoleInstanceIndividualItemReferences() && !reqConfPreCompItem->hasRemainingInitializingRolePredeccessorInstanceIndividualItemReferencePairs() && !reqConfPreCompItem->hasRemainingInitializingRoleSuccessorInstanceIndividualItemReferencePairs()) {

										QList<CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem*>* entIndsRoleInstTestList = reqConfPreCompItem->getEntailmentIndividualsRoleInstanceTestingItemList();
										CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem* roleInstTestItem = entIndsRoleInstTestList->takeFirst();
										COptimizedKPSetRoleInstancesItem* roleItem = roleInstTestItem->getRoleItem();
										CRealizationIndividualInstanceItemReference indiSourceItemRef = roleInstTestItem->getIndividualSourceItemReference();
										CRealizationIndividualInstanceItemReference indiDestItemRef = roleInstTestItem->getIndividualDestinationItemReference();
										COntologyRealizingDynamicRequirmentProcessingData* procData = roleInstTestItem->getProcessingData();
										bool inversed = roleInstTestItem->isRoleInversed();
										COptimizedKPSetIndividualItem* indiSourceItem = (COptimizedKPSetIndividualItem*)indiSourceItemRef.getRealizationInstanceItem();

										//QString indiString = CIRIName::getRecentIRIName(indiSourceItemRef.getIndividual()->getIndividualNameLinker());
										//QString roleString = CIRIName::getRecentIRIName(roleItem->getRole()->getPropertyNameLinker());
										//QString inverseRoleString = CIRIName::getRecentIRIName(roleItem->getInverseRole()->getPropertyNameLinker());
										//if (indiString == "http://purl.obolibrary.org/obo/CHROMOSOME-7227_X" && (roleString == "http://purl.obolibrary.org/obo/RO_0002131" || inverseRoleString == "http://purl.obolibrary.org/obo/RO_0002131")) {
										//	bool debug = true;
										//}

										if (indiSourceItem) {
											indiSourceItem = getMergingResolvedIndividualItem(indiSourceItem);
										}
										if (indiDestItemRef.isNonEmpty()) {
											COptimizedKPSetIndividualItem* indiDestItem = dynamic_cast<COptimizedKPSetIndividualItem*>(indiDestItemRef.getRealizationInstanceItem());
											if (indiDestItem) {
												indiDestItem = getMergingResolvedIndividualItem(indiDestItem);
											}

											bool isInstance = false;
											bool isPossibleInstance = false;

											if (indiSourceItem && indiDestItem) {
												COptimizedKPSetRoleNeighbourInstancesHash* knownPossibleInstancesHash = indiSourceItem->getKnownPossibleRoleNeighboursInstancesHash();
												if (knownPossibleInstancesHash) {
													COptimizedKPSetRoleNeighbourInstancesHashData possNeighbourInstanceData = knownPossibleInstancesHash->value(roleItem);
													COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = possNeighbourInstanceData.getRoleNeighbourInstancesHash(inversed, false);
													if (possNeighbourInstanceHash) {
														COptimizedKPSetRoleInstancesData* instanceData = possNeighbourInstanceHash->value(indiDestItem->getIndividualId()).mInstanceItemData;
														if (instanceData) {
															isInstance = instanceData->mKnownInstance;
															isPossibleInstance = isInstance || instanceData->mPossibleInstance && !instanceData->mTestedInstance;
														}
													}
												}

												if (!indiSourceItem->isItemSameIndividualMerged() && !indiDestItem->isItemSameIndividualMerged() && !isInstance && isPossibleInstance) {
													COptimizedKPSetIndividualItemPair indiPair = COptimizedKPSetIndividualItemPair(indiSourceItem, indiDestItem);
													if (inversed) {
														indiPair = COptimizedKPSetIndividualItemPair(indiDestItem, indiSourceItem);
													}
													roleItem->getPossibleInstancesSet()->remove(indiPair);
													workTestCreated = createNextRoleInstantiationTest(reqConfPreCompItem, roleItem, indiPair, procData);
												}
											}
											if (!workTestCreated && !isInstance && !isPossibleInstance) {
												if (roleItem->getIndividualIdComplexRoleDataHash()->contains(indiSourceItemRef.getIndividualID())) {
													COptimizedKPSetIndividualComplexRoleData* indiComplexData = roleItem->getIndividualIdComplexRoleData(indiSourceItemRef.getIndividualID(), true);
													COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;

													COptimizedKPSetRoleInstancesData* instanceData = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstanceItemData(false, indiDestItemRef.getIndividualID(), false);
													if (instanceData) {
														isInstance = instanceData->mKnownInstance;
														isPossibleInstance = isInstance || instanceData->mPossibleInstance && !instanceData->mTestedInstance;
													}


													if (!isInstance && isPossibleInstance) {
														COptimizedKPSetIndividualItemReferencePair candidatePair = COptimizedKPSetIndividualItemReferencePair(indiSourceItemRef, indiDestItemRef);
														if (inversed) {
															candidatePair = COptimizedKPSetIndividualItemReferencePair(indiDestItemRef, indiSourceItemRef);
														}
														roleItem->removeComplexCandidateInstance(candidatePair, false);
														workTestCreated = createNextRoleComplexCandidateConfirmationTest(reqConfPreCompItem, roleItem, candidatePair, procData);
													}
												}
											}


											if (!workTestCreated && procData) {
												setDynamicRequirementProcessed(ontPreCompItem, procData);
											}
										} else {
											if (indiSourceItem) {
												COptimizedKPSetRoleNeighbourInstancesHash* knownPossibleInstancesHash = indiSourceItem->getKnownPossibleRoleNeighboursInstancesHash();
												if (knownPossibleInstancesHash) {
													COptimizedKPSetRoleNeighbourInstancesHashData possNeighbourInstanceData = knownPossibleInstancesHash->value(roleItem);
													COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = possNeighbourInstanceData.getRoleNeighbourInstancesHash(inversed, false);
													if (possNeighbourInstanceHash) {

														for (COptimizedKPSetRoleInstancesHash::const_iterator itNeighInst = possNeighbourInstanceHash->constBegin(), itNeighInstEnd = possNeighbourInstanceHash->constEnd(); itNeighInst != itNeighInstEnd; ++itNeighInst) {
															const COptimizedKPSetRoleInstancesHashData& destIndiInstanceDat = itNeighInst.value();
															COptimizedKPSetIndividualItem* destIndiItem = destIndiInstanceDat.mNeighbourIndividualItem;
															COptimizedKPSetRoleInstancesData* instanceData = destIndiInstanceDat.mInstanceItemData;
															if (instanceData && !instanceData->mKnownInstance && instanceData->mPossibleInstance && !instanceData->mTestedInstance) {
																CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem* incRoleInstTestItem = new CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem(indiSourceItemRef, CRealizationIndividualInstanceItemReference(destIndiItem->getIndividualReference(), destIndiItem), roleItem, inversed, procData);
																procData->incProcessingItemCount();
																reqConfPreCompItem->addEntailmentIndividualsRoleInstanceTestingItem(incRoleInstTestItem);
															}
														}
													}
												}
											}
											if (roleItem->getIndividualIdComplexRoleDataHash()->contains(indiSourceItemRef.getIndividualID())) {
												COptimizedKPSetIndividualComplexRoleData* indiComplexData = roleItem->getIndividualIdComplexRoleData(indiSourceItemRef.getIndividualID(), true);
												COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;

		
												COptimizedKPSetRoleInstancesHash* possNeighbourInstanceHash = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(inversed, false);
												if (possNeighbourInstanceHash) {

													for (COptimizedKPSetRoleInstancesHash::const_iterator itNeighInst = possNeighbourInstanceHash->constBegin(), itNeighInstEnd = possNeighbourInstanceHash->constEnd(); itNeighInst != itNeighInstEnd; ++itNeighInst) {
														const COptimizedKPSetRoleInstancesHashData& destIndiInstanceDat = itNeighInst.value();
														cint64 destIndiId = itNeighInst.key();
														COptimizedKPSetRoleInstancesData* instanceData = destIndiInstanceDat.mInstanceItemData;
														if (instanceData && !instanceData->mKnownInstance && instanceData->mPossibleInstance && !instanceData->mTestedInstance) {
															CRealizationIndividualInstanceItemReference destIndiRef = reqConfPreCompItem->getInstanceItemReference(destIndiId, false);
															CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem* incRoleInstTestItem = new CRealizationEntailmentQueuedIndividualsRoleInstanceTestingItem(indiSourceItemRef, destIndiRef, roleItem, inversed, procData);
															procData->incProcessingItemCount();
															reqConfPreCompItem->addEntailmentIndividualsRoleInstanceTestingItem(incRoleInstTestItem);
														}
													}
												}

											}


											if (procData) {
												setDynamicRequirementProcessed(reqConfPreCompItem, procData);
											}


										}
										delete roleInstTestItem;
									}
								}


								if (reqConfPreCompItem->hasRemainingProcessingRoleInstanceItems()) {										
									QList<COptimizedKPSetRoleInstancesItem*>* processingInstancesItemList = reqConfPreCompItem->getProcessingPossibleRoleInstancesItemList();
									while (!workTestCreated && !processingInstancesItemList->isEmpty()) {
										COptimizedKPSetRoleInstancesItem* nextProcessingItem = processingInstancesItemList->first();

										if (nextProcessingItem->hasPossibleInstances()) {
											COptimizedKPSetIndividualItemPair nextTestItemPair = nextProcessingItem->takeNextTestingPossibleInstance();
											COptimizedKPSetIndividualItem* firstIndiItem = nextTestItemPair.first;
											COptimizedKPSetIndividualItem* secondIndiItem = nextTestItemPair.second;

											if (!firstIndiItem->isItemSameIndividualMerged()) {
												workTestCreated = createNextRoleInstantiationTest(reqConfPreCompItem,nextProcessingItem,nextTestItemPair);
											} else {
												incTestedPossibleRoleInstancesCount(reqConfPreCompItem);
											}
										}
										if (!nextProcessingItem->hasPossibleInstances()) {
											processingInstancesItemList->removeFirst();
											nextProcessingItem->setPossibleInstancesProcessingQueuedFlag(false);
										}
									}

								} 

								if (!workTestCreated && !reqConfPreCompItem->hasRemainingInitializingRoleInstanceItems() && !reqConfPreCompItem->hasRemainingProcessingRoleInstanceItems() && !reqConfPreCompItem->hasRemainingInitializingRoleInstanceIndividualItemReferences() && !reqConfPreCompItem->hasRemainingCandidateConfirmationRoleInstanceItems()) {
									if (!reqConfPreCompItem->hasTestingPossibleRoleInstances() && !reqConfPreCompItem->hasPropagatingRoleInstanceCandidates() && !reqConfPreCompItem->hasTestingRoleInstanceCandidates()) {

										if (reqConfPreCompItem->isAllRoleInstancesProcessing()) {
											if (!reqConfPreCompItem->isRoleRealizationInstalled()) {
												reqConfPreCompItem->setRoleRealizationInstalled(true);

												LOG(INFO, getDomain(), logTr("Role realization finished for ontology."), this);
												reqConfPreCompItem->getRealizeRoleProcessingStep()->setStepFinished(true);
											}
										}
										reqConfPreCompItem->getRealizeRoleProcessingStep()->submitRequirementsUpdate();
									}
								}
							} else {
								reqConfPreCompItem->getRealizeRoleProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							reqConfPreCompItem->getRealizeRoleProcessingStep()->submitRequirementsUpdate();
						}
					}









					if (!workTestCreated) {
						if (!reqConfPreCompItem->hasRemainingProcessingRequirements()) {
							finishOntologyRealizing(reqConfPreCompItem);
							mProcessingOntItemList.removeFirst();
						} else {
							mProcessingOntItemList.removeFirst();
							mProcessingOntItemList.append(reqConfPreCompItem);
							loopOntPreCompItem = ontPreCompItem;
						}
					}
				}
				return workTestCreated;
			}


			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::addRealizationStatistics(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CRealizingTestingStep* ontProcStep) {
				COntologyProcessingStatistics* ontProcStats = ontProcStep->getProcessingStepData()->getProcessingStatistics(true);
				ontProcStats->setProcessingCountStatisticValue("realization-time",reqConfPreCompItem->getInitializationTime()->elapsed());
				if (reqConfPreCompItem->getRealizeConceptProcessingStep() == ontProcStep) {
					ontProcStats->setProcessingCountStatisticValue("realization-concept-instances-possible-testing-count",reqConfPreCompItem->getTestedPossibleConceptInstancesCount());
					addIndividualDependencyTrackingStatistics(reqConfPreCompItem,ontProcStep,"realization-concept-instances");
					ontProcStats->setProcessingCountStatisticValue("realization-concept-instances-time",reqConfPreCompItem->getInitializationTime()->elapsed());
				} else if (reqConfPreCompItem->getRealizeSameIndividualsProcessingStep() == ontProcStep) {
					addIndividualDependencyTrackingStatistics(reqConfPreCompItem,ontProcStep,"realization-same-individuals");
					ontProcStats->setProcessingCountStatisticValue("realization-same-individuals-possible-testing-count",reqConfPreCompItem->getTestedPossibleSameIndividualsCount());
				}
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::addIndividualDependencyTrackingStatistics(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, CRealizingTestingStep* ontProcStep, const QString& realizationTypeString) {
				if (reqConfPreCompItem->requiresIndividualDependenceTracking()) {
					COntologyProcessingStatistics* ontProcStats = ontProcStep->getProcessingStepData()->getProcessingStatistics(true);
					cint64 dependencyCount = 0;
					cint64 dependencySize = 0;
					for (QList<COptimizedKPSetIndividualItem*>::const_iterator indiItemIt = reqConfPreCompItem->getInstantiatedItemList()->constBegin(), indiItemItEnd = reqConfPreCompItem->getInstantiatedItemList()->constEnd(); indiItemIt != indiItemItEnd; ++indiItemIt) {
						COptimizedKPSetIndividualItem* indiItem = (*indiItemIt);
						if (indiItem->getIndividualDependenceTrackingCollector()) {
							CIndividualDependenceTracking* indiDepTack = indiItem->getIndividualDependenceTrackingCollector()->getExtendingIndividualDependenceTracking();
							if (indiDepTack) {
								dependencySize += indiDepTack->getDependenceSize();
								++dependencyCount;
							}
						}
					}
					ontProcStats->setProcessingCountStatisticValue(QString("%1-tracked-node-dependency-size").arg(realizationTypeString),dependencySize);
					ontProcStats->setProcessingCountStatisticValue(QString("%1-tracked-node-dependency-count").arg(realizationTypeString),dependencyCount);
				}
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::createTemporarySameRealizationOntology(COptimizedRepresentativeKPSetOntologyRealizingItem* item) {
				CConcreteOntology* ontology = item->getOntology();
				CConcreteOntology* tmpSameRealOntology = item->getTemporarySameRealizationOntology();
				if (!tmpSameRealOntology) {


					QList<COptimizedKPSetIndividualItem*> indiItemNominalCreationList;
					QList<COptimizedKPSetIndividualItem*>* possibleSameIndiItemList = item->getPossibleSameIndividualsItemList();
					if (possibleSameIndiItemList && !possibleSameIndiItemList->isEmpty()) {
						for (QList<COptimizedKPSetIndividualItem*>::const_iterator it = possibleSameIndiItemList->constBegin(), itEnd = possibleSameIndiItemList->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetIndividualItem* indiItem(*it);
							CIndividual* indi = indiItem->getIndividual();
							if (!indi || indi->getIndividualNominalConcept() == nullptr) {
								indiItemNominalCreationList.append(indiItem);
							}
						}
					}

					if (!indiItemNominalCreationList.isEmpty()) {
						tmpSameRealOntology = new CConcreteOntology(ontology,ontology->getConfiguration());		
						tmpSameRealOntology->setConsistence(ontology->getConsistence());
						item->setTemporarySameRealizationOntology(tmpSameRealOntology);

						for (QList<COptimizedKPSetIndividualItem*>::const_iterator it = indiItemNominalCreationList.constBegin(), itEnd = indiItemNominalCreationList.constEnd(); it != itEnd; ++it) {
							COptimizedKPSetIndividualItem* indiItem(*it);
							CIndividual* indi = indiItem->getIndividual();

							if (!indi) {
								indi = new CIndividual(indiItem->getIndividualId());
								indi->setTemporaryFakeIndividual(true);
							}

							CConcept* nominalConcept = createTemporaryConcept(item,tmpSameRealOntology);
							nominalConcept->setNominalIndividual(indi);
							nominalConcept->setOperatorCode(CCNOMINAL);

							indiItem->setTemporaryIndividualNominalConcept(nominalConcept);
						}

					} else {
						item->setTemporarySameRealizationOntology(ontology);
					}
				}

				return this;
			}





			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::createTemporaryRoleRealizationOntology(COptimizedRepresentativeKPSetOntologyRealizingItem* item) {
				CConcreteOntology* ontology = item->getOntology();
				CConcreteOntology* tmpRoleRealOntology = item->getTemporaryRoleRealizationOntology();
				if (!tmpRoleRealOntology) {
					tmpRoleRealOntology = new CConcreteOntology(ontology,ontology->getConfiguration());		
					tmpRoleRealOntology->setOntologyID(ontology->getOntologyID());
					tmpRoleRealOntology->setConsistence(ontology->getConsistence());

					QSet<CConcept*> compTransformConceptSet;
					QHash<CConcept*,COptimizedKPSetRoleInstancesItem*>* markerConRolInsItemHash = item->getMarkerConceptInstancesItemHash();

					COptimizedKPSetRoleInstancesItem* topRoleInstItem = item->getTopRoleInstancesItem();
					QList<COptimizedKPSetRoleInstancesItem*>* roleItemList = item->getRoleInstancesItemList();
					for (QList<COptimizedKPSetRoleInstancesItem*>::const_iterator itemIt = roleItemList->constBegin(), itemItEnd = roleItemList->constEnd(); itemIt != itemItEnd; ++itemIt) {
						COptimizedKPSetRoleInstancesItem* roleItem(*itemIt);
						if (roleItem != topRoleInstItem) {
							CRole* role = roleItem->getRole();
							CConcept* markerConcept = createTemporaryConcept(item,tmpRoleRealOntology);
							markerConcept->setOperatorCode(CCMARKER);
							CConcept* propConcept = createTemporaryConcept(item,tmpRoleRealOntology);
							propConcept->setOperatorCode(CCALL);
							propConcept->setRole(role);
							addTemporaryConceptOperand(propConcept,markerConcept,false,item,tmpRoleRealOntology);
							roleItem->setTemporaryMarkerConcept(markerConcept);
							markerConRolInsItemHash->insert(markerConcept,roleItem);
							roleItem->setTemporaryPropagationConcept(propConcept);
							if (role->isComplexRole()) {
								compTransformConceptSet.insert(propConcept);
							}

							if (roleItem->hasInverseRole()) {
								CRole* invRole = roleItem->getInverseRole();
								CConcept* invMarkerConcept = createTemporaryConcept(item,tmpRoleRealOntology);
								invMarkerConcept->setOperatorCode(CCMARKER);
								CConcept* invPropConcept = createTemporaryConcept(item,tmpRoleRealOntology);
								invPropConcept->setOperatorCode(CCALL);
								invPropConcept->setRole(invRole);
								addTemporaryConceptOperand(invPropConcept,invMarkerConcept,false,item,tmpRoleRealOntology);
								markerConRolInsItemHash->insert(invMarkerConcept,roleItem);
								roleItem->setTemporaryInverseMarkerConcept(invMarkerConcept);
								roleItem->setTemporaryInversePropagationConcept(invPropConcept);
								if (invRole->isComplexRole()) {
									compTransformConceptSet.insert(invPropConcept);
								}
							}
						}
					}

					if (!compTransformConceptSet.isEmpty()) {
						CPreProcessContextBase* preprocessContext = new CPreProcessContextBase(tmpRoleRealOntology,ontology->getConfiguration());
						CRoleChainAutomataTransformationPreProcess* roleChainAutomataTransformPreprocessor = new CRoleChainAutomataTransformationPreProcess();
						roleChainAutomataTransformPreprocessor->preprocess(tmpRoleRealOntology,&compTransformConceptSet,preprocessContext);
						delete preprocessContext;
						delete roleChainAutomataTransformPreprocessor;						
					}

					item->setTemporaryRoleRealizationOntology(tmpRoleRealOntology);
				}
				return this;
			}



			CConcept* COptimizedRepresentativeKPSetOntologyRealizingThread::createTemporaryConcept(COptimizedRepresentativeKPSetOntologyRealizingItem* item, CConcreteOntology* tmpRoleRealOntology) {
				CConceptVector* conVec = tmpRoleRealOntology->getTBox()->getConceptVector();
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(tmpRoleRealOntology->getOntologyContext()->getMemoryAllocationManager());
				concept->initConcept();
				cint64 newConTag = conVec->getItemCount();
				CConsistence* consistence = tmpRoleRealOntology->getConsistence();
				if (consistence) {
					CConsistenceData* consData = consistence->getConsistenceModelData();
					if (consData) {
						CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
						CSatisfiableCalculationTask* satConsTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
						newConTag = qMax(newConTag, satConsTask->getProcessingDataBox()->getExtendedConceptVector()->getItemCount());
					}
				}

				concept->setConceptTag(newConTag);
				conVec->setData(concept->getConceptTag(),concept);
				return concept;
			}



			void COptimizedRepresentativeKPSetOntologyRealizingThread::addTemporaryConceptOperand(CConcept* concept, CConcept* opConcept, bool negated, COptimizedRepresentativeKPSetOntologyRealizingItem* item, CConcreteOntology* tmpRoleRealOntology) {
				CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(tmpRoleRealOntology->getOntologyContext()->getMemoryAllocationManager());
				opConLinker->init(opConcept,negated);
				concept->addOperandLinker(opConLinker);
				concept->incOperandCount();
			}



			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::updateCalculatedKnownConceptInstance(COptimizedKPSetIndividualItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = instantiatedItem->getKnownPossibleInstancesHash();
				COptimizedKPSetConceptInstancesData* instanceItemData = knownPossibleInstancesHash->getInstanceItemData(instancesItem);
				QList<COptimizedKPSetConceptInstancesItem*> updateItemList;
				instanceItemData->mTestingInstance = false;
				if (!instanceItemData->mKnownInstance) {
					instancesItem->addKnownInstance(instantiatedItem);
					instanceItemData->mKnownInstance = true;
					instanceItemData->mPossibleInstance = false;
					instanceItemData->mMostSpecific = true;
					instanceItemData->mTestedInstance = true;
					updateItemList.append(*instancesItem->getParentItemList());
				}
				while (!updateItemList.isEmpty()) {
					COptimizedKPSetConceptInstancesItem* updateInstanceItem = updateItemList.takeFirst();
					COptimizedKPSetConceptInstancesData* updateInstanceItemData = knownPossibleInstancesHash->getInstanceItemData(updateInstanceItem);
					if (!updateInstanceItemData->mKnownInstance) {
						updateInstanceItemData->mKnownInstance = true;
						updateInstanceItemData->mPossibleInstance = false;
						updateInstanceItemData->mMostSpecific = false;
						updateInstanceItemData->mDerived = true;
						updateItemList.append(*updateInstanceItem->getParentItemList());
					} else if (updateInstanceItemData->mMostSpecific) {
						updateInstanceItem->removeKnownInstance(instantiatedItem);
						updateInstanceItemData->mMostSpecific = false;
					}
				}
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::updateCalculatedNonConceptInstance(COptimizedKPSetIndividualItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = instantiatedItem->getKnownPossibleInstancesHash();
				COptimizedKPSetConceptInstancesData* instanceItemData = knownPossibleInstancesHash->getInstanceItemData(instancesItem);
				QList<COptimizedKPSetConceptInstancesItem*> updateItemList;
				instanceItemData->mTestingInstance = false;
				if (!instanceItemData->mKnownInstance) {
					if (instanceItemData->mPossibleInstance) {
						instanceItemData->mPossibleInstance = false;
						instanceItemData->mTestedInstance = true;
						updateItemList.append(*instancesItem->getParentItemList());
					}
				}
				while (!updateItemList.isEmpty()) {
					COptimizedKPSetConceptInstancesItem* updateInstanceItem = updateItemList.takeFirst();
					COptimizedKPSetConceptInstancesData* updateInstanceItemData = knownPossibleInstancesHash->getInstanceItemData(updateInstanceItem);
					if (!updateInstanceItemData->mKnownInstance) {
						if (updateInstanceItemData->mPossibleInstance) {
							if (!updateInstanceItemData->mMostSpecific && !updateInstanceItemData->mTestingInstance) {
								updateInstanceItemData->mMostSpecific = true;								
								incOpenPossibleConceptInstancesCount(reqConfPreCompItem);
								instantiatedItem->incPossibleInstantiatedCount();
								updateInstanceItem->addPossibleInstance(instantiatedItem);
								if (updateInstanceItem->hasToProcessPossibleInstancesFlag()) {
									reqConfPreCompItem->addConceptInstanceItemToProcessPossibleInstances(updateInstanceItem);
								}
							}
						}
					}
				}
				return this;
			}








			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::updateCalculatedKnownRoleInstance(COptimizedKPSetIndividualItemPair individualItemPair, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetIndividualItem* indiItem1 = individualItemPair.first;
				COptimizedKPSetIndividualItem* indiItem2 = individualItemPair.second;
				COptimizedKPSetRoleNeighbourInstancesHash* knownPossibleRoleNeighbourInstancesHash = indiItem1->getKnownPossibleRoleNeighboursInstancesHash();
				COptimizedKPSetRoleInstancesData* instanceItemData = knownPossibleRoleNeighbourInstancesHash->getRoleSuccessorInstanceItemData(instancesItem,indiItem2);
				QList<TRoleItemInversionPair> updateItemList;
				instanceItemData->mTestingInstance = false;
				if (!instanceItemData->mKnownInstance) {
					instancesItem->addKnownInstance(COptimizedKPSetIndividualItemPair(indiItem1,indiItem2));
					instanceItemData->mKnownInstance = true;
					instanceItemData->mPossibleInstance = false;
					instanceItemData->mMostSpecific = true;
					instanceItemData->mTestedInstance = true;
					updateItemList.append(*instancesItem->getParentItemList());
				}
				while (!updateItemList.isEmpty()) {
					TRoleItemInversionPair updateInstanceItemInversionPair = updateItemList.takeFirst();
					COptimizedKPSetRoleInstancesItem* updateInstanceItem = updateInstanceItemInversionPair.first;
					if (updateInstanceItem != reqConfPreCompItem->getTopRoleInstancesItem()) {
						bool updateInstanceItemInversion = updateInstanceItemInversionPair.second;
						COptimizedKPSetRoleInstancesData* updateInstanceItemData = nullptr;
						if (!updateInstanceItemInversion) {
							updateInstanceItemData = knownPossibleRoleNeighbourInstancesHash->getRoleSuccessorInstanceItemData(updateInstanceItem,indiItem2, false);
						} else {
							updateInstanceItemData = knownPossibleRoleNeighbourInstancesHash->getRolePredecessorInstanceItemData(updateInstanceItem,indiItem2, false);
						}
						if (updateInstanceItemData) {
							if (!updateInstanceItemData->mKnownInstance) {
								updateInstanceItemData->mKnownInstance = true;
								updateInstanceItemData->mPossibleInstance = false;
								updateInstanceItemData->mMostSpecific = false;
								updateInstanceItemData->mDerived = true;
								updateItemList.append(*updateInstanceItem->getParentItemList());
							} else if (updateInstanceItemData->mMostSpecific) {
								updateInstanceItem->removeKnownInstance(COptimizedKPSetIndividualItemPair(indiItem1, indiItem2));
								updateInstanceItemData->mMostSpecific = false;
							}
						}
					}
				}
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::updateCalculatedNonRoleInstance(COptimizedKPSetIndividualItemPair individualItemPair, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetIndividualItem* indiItem1 = individualItemPair.first;
				COptimizedKPSetIndividualItem* indiItem2 = individualItemPair.second;
				COptimizedKPSetRoleNeighbourInstancesHash* knownPossibleRoleNeighbourInstancesHash = indiItem1->getKnownPossibleRoleNeighboursInstancesHash();
				COptimizedKPSetRoleInstancesData* instanceItemData = knownPossibleRoleNeighbourInstancesHash->getRoleSuccessorInstanceItemData(instancesItem,indiItem2);
				QList<TRoleItemInversionPair> updateItemList;
				instanceItemData->mTestingInstance = false;
				if (!instanceItemData->mKnownInstance) {
					if (instanceItemData->mPossibleInstance) {
						instanceItemData->mPossibleInstance = false;
						instanceItemData->mTestedInstance = true;
						updateItemList.append(*instancesItem->getParentItemList());
					}
				}
				while (!updateItemList.isEmpty()) {
					TRoleItemInversionPair updateInstanceItemInversionPair = updateItemList.takeFirst();
					COptimizedKPSetRoleInstancesItem* updateInstanceItem = updateInstanceItemInversionPair.first;
					if (updateInstanceItem != reqConfPreCompItem->getTopRoleInstancesItem()) {
						bool updateInstanceItemInversion = updateInstanceItemInversionPair.second;
						COptimizedKPSetRoleInstancesData* updateInstanceItemData = nullptr;
						if (!updateInstanceItemInversion) {
							updateInstanceItemData = knownPossibleRoleNeighbourInstancesHash->getRoleSuccessorInstanceItemData(updateInstanceItem,indiItem2, false);
						} else {
							updateInstanceItemData = knownPossibleRoleNeighbourInstancesHash->getRolePredecessorInstanceItemData(updateInstanceItem,indiItem2, false);
						}
						if (updateInstanceItemData && !updateInstanceItemData->mKnownInstance) {
							if (updateInstanceItemData->mPossibleInstance) {
								if (!updateInstanceItemData->mMostSpecific && !updateInstanceItemData->mTestingInstance) {
									updateInstanceItemData->mMostSpecific = true;								
									incOpenPossibleRoleInstancesCount(reqConfPreCompItem);
									indiItem1->incPossibleRoleInstantiatedCount();
									indiItem2->incPossibleRoleInstantiatedCount();
									if (!updateInstanceItemInversion) {
										updateInstanceItem->addPossibleInstance(COptimizedKPSetIndividualItemPair(indiItem1,indiItem2));
									} else {
										updateInstanceItem->addPossibleInstance(COptimizedKPSetIndividualItemPair(indiItem2,indiItem1));
									}
									if (updateInstanceItem->hasToProcessPossibleInstancesFlag()) {
										reqConfPreCompItem->addRoleInstanceItemToProcessPossibleInstances(updateInstanceItem);
									}
								}
							}
						}
					}
				}
				return this;
			}






			bool COptimizedRepresentativeKPSetOntologyRealizingThread::interpretConceptInstantiationResult(COntologyRealizingItem* ontPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem, bool isIndividualInstance) {
				COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem = (COptimizedRepresentativeKPSetOntologyRealizingItem*)ontPreCompItem;


				//CConcept* concept = instancesItem->getHierarchyNode()->getOneEquivalentConcept();
				//if (CIRIName::getRecentIRIName(concept->getNameLinker()) == "http://www.reactome.org/biopax/46/170905#Protein19") {
				//	bool debug = true;
				//}


				instancesItem->decTestingPossibleInstancesCount();
				instantiatedItem->decTestingPossibleInstantiatedCount();
				reqConfPreCompItem->decTestingPossibleConceptInstanceCount();

				incTestedPossibleConceptInstancesCount(reqConfPreCompItem);

				if (!instantiatedItem->isItemSameIndividualMerged()) {
					if (isIndividualInstance) {
						updateCalculatedKnownConceptInstance(instantiatedItem, instancesItem, reqConfPreCompItem);
					}
					else {
						updateCalculatedNonConceptInstance(instantiatedItem, instancesItem, reqConfPreCompItem);
					}
				}

				if (!instancesItem->hasTestingPossibleInstances() && !instancesItem->hasPossibleInstances()) {
					QList<CPossibleInstancesIndividualsMergingData*>* possInstMergDataList = instancesItem->getPossibleInstanceMergingDataContainer();
					if (possInstMergDataList) {
						for (CPossibleInstancesIndividualsMergingData* mergingData : *possInstMergDataList) {
							reqConfPreCompItem->incAttemptedPossibleInstancesMergingsCount(mergingData->getMergingAttemptCount());
						}
					}
					instancesItem->clearPossibleInstanceMergingData();
					if (instancesItem->hasAllSuccessorProcessedFlag()) {
						instancesItem->setSelfSuccessorProcessedFlag(true);
						updateParentItemsSuccessorProcessed(instancesItem);
					}
				}
				return true;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingThread::realizingTested(COntologyRealizingItem* ontPreCompItem, CRealizingTestingItem* preTestItem, CRealizingCalculatedCallbackEvent* pcce) {
				COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem = (COptimizedRepresentativeKPSetOntologyRealizingItem*)ontPreCompItem;
				COntologyRealizingDynamicRequirmentProcessingData* procData = preTestItem->getProcessingData();

				bool processed = false;
				if (!processed) {
					CIndividualRoleCandidateConfirmationTestingItem* indiRoleCandConfTestItem = dynamic_cast<CIndividualRoleCandidateConfirmationTestingItem*>(preTestItem);
					if (indiRoleCandConfTestItem) {
						bool isIndividualInstance = !pcce->getTestResultSatisfiable();
						reqConfPreCompItem->decTestingRoleInstanceCandidatesCount();

						CRealizationIndividualInstanceItemReference sourceIndividualItemReference = indiRoleCandConfTestItem->getSourceInstantiatedItemReference();
						COptimizedKPSetIndividualItem* sourceIndividualItem = (COptimizedKPSetIndividualItem*)sourceIndividualItemReference.getRealizationInstanceItem();

						CRealizationIndividualInstanceItemReference targetIndividualItemReference = indiRoleCandConfTestItem->getTargetInstantiatedItemReference();
						COptimizedKPSetIndividualItem* targetIndividualItem = (COptimizedKPSetIndividualItem*)targetIndividualItemReference.getRealizationInstanceItem();

						COptimizedKPSetRoleInstancesItem* roleInstancesItem = indiRoleCandConfTestItem->getRoleInstancesItem();
						if (roleInstancesItem) {
							COptimizedKPSetIndividualComplexRoleData* indiComplexData = roleInstancesItem->getIndividualIdComplexRoleData(sourceIndividualItemReference.getIndividualID(), true);
							COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;

							COptimizedKPSetRoleInstancesData* instanceData = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstanceItemData(false, targetIndividualItemReference.getIndividualID(), true);
							instanceData->mKnownInstance = isIndividualInstance;
							instanceData->mTestedInstance = true;
						}

						delete indiRoleCandConfTestItem;
						processed = true;
					}
				}

				if (!processed) {
					CIndividualRoleCandidatePropagationTestingItem* indiRoleCandTestItem = dynamic_cast<CIndividualRoleCandidatePropagationTestingItem*>(preTestItem);
					if (indiRoleCandTestItem) {
						reqConfPreCompItem->decPropagatingRoleInstanceCandidatesCount();
						CRealizationIndividualInstanceItemReference individualItemReference = indiRoleCandTestItem->getInstantiatedItemReference();
						COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)individualItemReference.getRealizationInstanceItem();

						QList<CIndividualRoleCandidateTestingData>* indiRoleCandTestDataList = indiRoleCandTestItem->geIndividualRoleCandidateTestingDataList();

						for (CIndividualRoleCandidateTestingData& indiRoleCandTestData : *indiRoleCandTestDataList) {
							CConcept* markerConcept = indiRoleCandTestData.getMarkerConcept();
							COptimizedKPSetIndividualComplexRoleData* indiComplexData = indiRoleCandTestData.getIndividualComplexData();
							bool inversed = indiRoleCandTestData.getInversed();


							COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;
							indiExplicitIndirectLinkComplexRepresentationData->setInitialized(inversed, true);
							if (inversed) {
								indiExplicitIndirectLinkComplexRepresentationData->mRolePredecessorInstancesInitialized = true;
							} else {
								indiExplicitIndirectLinkComplexRepresentationData->mRoleSuccessorInstancesInitialized = true;
							}

						}

						if (individualItem) {
							individualItem->decInitializingRoleCandidateCount();
						}

						CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter* adapter = indiRoleCandTestItem->getRealizationMarkedCandidatesMessageAdapter();
						delete adapter;
						delete indiRoleCandTestItem;
						processed = true;
					}
				}

				if (!processed) {
					CIndividualConceptInstanceTestingItem* indiConInstTestItem = dynamic_cast<CIndividualConceptInstanceTestingItem*>(preTestItem);
					if (indiConInstTestItem) {
						bool isIndividualInstance = !pcce->getTestResultSatisfiable();



						COptimizedKPSetConceptInstancesItem* instancesItem = indiConInstTestItem->getInstancesItem();
						COptimizedKPSetIndividualItem* instantiatedItem = indiConInstTestItem->getInstantiatedItem();

						//if (!isIndividualInstance) {
						//	createNextConceptInstantiationTest(reqConfPreCompItem, instancesItem, instantiatedItem);
						//	return true;
						//}


						KONCLUCE_DEBUG_PRINTING_REALIZATION_CONCEPT_INSTANCE_TESTING_TIME(
							QString conceptName = CIRIName::getRecentIRIName(instancesItem->getHierarchyNode()->getOneEquivalentConcept()->getClassNameLinker());
							QString indiName = CIRIName::getRecentIRIName(instantiatedItem->getIndividual()->getIndividualNameLinker());

							QFile testTimeReportFile("./Debugging/Realizing/individual-concept-possible-instance-testing-times.tsv");
							if (testTimeReportFile.open(QFile::Append)) {
								QString reportingLine = QString("%1 \t%2 \t%3\t%4\t%5\r\n").arg(conceptName).arg(indiName).arg(indiConInstTestItem->startTime.elapsed()).arg(isIndividualInstance ? 1 : 0).arg(mCurrRunningTestParallelCount);
								testTimeReportFile.write(reportingLine.toLocal8Bit());
								testTimeReportFile.close();
							}
						)


						interpretConceptInstantiationResult(ontPreCompItem, instancesItem, instantiatedItem, isIndividualInstance);

						delete indiConInstTestItem;
						processed = true;
					}
				}

				if (!processed) {
					CIndividualPairRoleInstanceTestingItem* indiPairRoleInstTestItem = dynamic_cast<CIndividualPairRoleInstanceTestingItem*>(preTestItem);
					if (indiPairRoleInstTestItem) {
						bool isIndividualPairInstance = !pcce->getTestResultSatisfiable();

						COptimizedKPSetRoleInstancesItem* instancesItem = indiPairRoleInstTestItem->getInstancesItem();
						COptimizedKPSetIndividualItemPair indiItemPair = indiPairRoleInstTestItem->getIndividualItemPair();
						COptimizedKPSetIndividualItem* indiItem1 = indiItemPair.first;
						COptimizedKPSetIndividualItem* indiItem2 = indiItemPair.second;

						instancesItem->decTestingPossibleInstancesCount();
						indiItem1->decTestingPossibleRoleInstantiatedCount();
						indiItem2->decTestingPossibleRoleInstantiatedCount();
						reqConfPreCompItem->decTestingPossibleRoleInstanceCount();

						incTestedPossibleRoleInstancesCount(reqConfPreCompItem);

						//bool repeat = false;
						//if (repeat) {
						//	createNextRoleInstantiationTest(reqConfPreCompItem, instancesItem, COptimizedKPSetIndividualItemPair(indiItem1, indiItem2), nullptr);
						//	return true;
						//}

						if (!indiItem1->isItemSameIndividualMerged() && !indiItem2->isItemSameIndividualMerged()) {
							if (isIndividualPairInstance) {
								updateCalculatedKnownRoleInstance(indiItemPair, instancesItem, reqConfPreCompItem);
							} else {
								updateCalculatedNonRoleInstance(indiItemPair, instancesItem, reqConfPreCompItem);
							}
						}

						if (!instancesItem->hasTestingPossibleInstances() && !instancesItem->hasPossibleInstances()) {
							if (instancesItem->hasAllSuccessorProcessedFlag()) {
								instancesItem->setSelfSuccessorProcessedFlag(true);
								updateParentItemsSuccessorProcessed(instancesItem);
							}
						}

						delete indiPairRoleInstTestItem;
						processed = true;
					}
				}


				if (!processed) {
					CIndividualSameTestingItem* indiSameTestItem = dynamic_cast<CIndividualSameTestingItem*>(preTestItem);
					if (indiSameTestItem) {
						bool isIndividualSame = !pcce->getTestResultSatisfiable();

						COptimizedKPSetIndividualItem* instantiatedItem1 = indiSameTestItem->getInstantiatedItem1();
						COptimizedKPSetIndividualItem* instantiatedItem2 = indiSameTestItem->getInstantiatedItem2();


						//if (!isIndividualSame) {
						//	createNextSameIndividualsTest(reqConfPreCompItem, instantiatedItem1, instantiatedItem2);
						//	return true;
						//}


						incTestedPossibleSameIndividualsCount(reqConfPreCompItem);

						instantiatedItem1->decTestingPossibleSameIndividualCount();
						instantiatedItem2->decTestingPossibleSameIndividualCount();
						reqConfPreCompItem->decTestingPossibleSameIndividualCount();

						if (isIndividualSame) {
							if (!instantiatedItem1->isItemSameIndividualMerged() && !instantiatedItem2->isItemSameIndividualMerged()) {
								mergeSameIndividualItems(instantiatedItem1, instantiatedItem2, reqConfPreCompItem);
							}
						}

						checkFinishSameIndividualProcessing(ontPreCompItem, instantiatedItem1);
						checkFinishSameIndividualProcessing(ontPreCompItem, instantiatedItem2);

						delete indiSameTestItem;
						processed = true;
					}
				}
				if (!processed) {
					CIndividualsConsistencyTestingItem* indiConsTest = dynamic_cast<CIndividualsConsistencyTestingItem*>(preTestItem);
					if (indiConsTest) {
						reqConfPreCompItem->setExtraConsistencyTested(true);
						reqConfPreCompItem->setExtraConsistencyTesting(false);
					}
				}
				if (procData) {
					setDynamicRequirementProcessed(ontPreCompItem, procData);
				}
				return processed;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingThread::checkFinishSameIndividualProcessing(COntologyRealizingItem* ontRealItem, COptimizedKPSetIndividualItem* instantiatedItem) {
				if (instantiatedItem) {
					if (instantiatedItem->isItemSameIndividualMerged() || (!instantiatedItem->hasToProcessPossibleSameIndividualsFlag() && !instantiatedItem->hasPossibleSameIndividualsTesting() && !instantiatedItem->hasPossibleSameIndividuals())) {
						setDynamicRequirementProcessed(ontRealItem, instantiatedItem->takeRequirmentProcessingDataLinkers());
						return true;
					}
				}
				return false;
			}


			bool COptimizedRepresentativeKPSetOntologyRealizingThread::checkFinishConceptInstancesProcessing(COntologyRealizingItem* ontRealItem, COptimizedKPSetConceptInstancesItem* conceptItem) {
				if (conceptItem) {
					if (!conceptItem->hasPossibleInstances() && !conceptItem->hasTestingPossibleInstances() && !conceptItem->hasPossibleInstances()) {
						conceptItem->setConceptInstancesTestingFinished(true);
						setDynamicRequirementProcessed(ontRealItem, conceptItem->takeRequirmentProcessingDataLinkers());
						return true;
					}
				}
				return false;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::setDynamicRequirementProcessed(COntologyRealizingItem* ontRealItem, COntologyRealizingDynamicRequirmentProcessingData* procData) {
				procData->decProcessingItemCount();
				if (!procData->hasCurrentProcessingItemCount()) {
					CRealizingTestingStep* realStep = procData->getAssociatedRelizationTestingStep();
					if (realStep) {
						realStep->removeRequirementProcessingData(procData);
					}
					COntologyProcessingRequirement* procReq = procData->getProcessingRequirement();
					if (procReq) {
						procReq->submitRequirementUpdate(COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, COntologyProcessingStatus::PSSUCESSFULL);
					}
					COntologyRealizingDynamicRequirmentCallbackData* callbackData = procData->getCallbackData();
					if (callbackData) {
						callbackData->decProcessingRequirmentCount();
						if (!callbackData->hasCurrentProcessingRequirmentCount()) {
							ontRealItem->logRequirementProcessingFinishStatistics(procData->getStatistics());
							CCallbackData* callback = callbackData->getProcessingFinishedCallback();
							callback->doCallback();
							delete callbackData;
						}
					}
					delete procData;
				}
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::setDynamicRequirementProcessed(COntologyRealizingItem* ontRealItem, CLinker<COntologyRealizingDynamicRequirmentProcessingData*>* procDataLinker) {
				CLinker<COntologyRealizingDynamicRequirmentProcessingData*>* procDataLinkerIt = procDataLinker;
				while (procDataLinkerIt) {
					COntologyRealizingDynamicRequirmentProcessingData* procData = procDataLinkerIt->getData();
					setDynamicRequirementProcessed(ontRealItem, procData);
					procDataLinkerIt = procDataLinkerIt->getNext();
				}
				if (procDataLinker) {
					ontRealItem->releaseRequirementProcessingDataLinker(procDataLinker);
				}
				return this;
			}







			bool COptimizedRepresentativeKPSetOntologyRealizingThread::addComplexRoleExplicitIndirectNeighbourLink(COptimizedKPSetRoleInstancesItem* roleInstItem, bool inversed, 
					const CRealizationIndividualInstanceItemReference& indiRealItemRef, COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData, 
					COptimizedKPSetRoleNeighbourInstancesHashData* indiRoleNeighbourHashData, COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* combinedNeighbourCacheLabelItemData, CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* assNeighbourRoleSetHash,
					const CIndividualReference& neighbourIndi, COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* neighbourIndiExplicitIndirectLinkComplexRepresentationData, 
					bool deterministic, COptimizedRepresentativeKPSetOntologyRealizingItem* realItem) {

				COptimizedKPSetIndividualItem* individualItem = (COptimizedKPSetIndividualItem*)indiRealItemRef.getRealizationInstanceItem();
				CIndividualReference individualReference = indiRealItemRef;


				//if (CIRIName::getRecentIRIName(indiRealItemRef.getIndividual()->getIndividualNameLinker()) == "http://www.w3.org/TR/2003/PR-owl-guide-20031209/wine#BordeauxRegion") {
				//	bool debug = true;
				//}


				bool isAlreadyDirectLink = false;
				if (indiRoleNeighbourHashData) {
					COptimizedKPSetRoleInstancesData* instanceData = indiRoleNeighbourHashData->getRoleNeighbourInstanceItemData(inversed, neighbourIndi.getIndividualID(), false);
					if (instanceData && (instanceData->mPossibleInstance || instanceData->mKnownInstance)) {
						isAlreadyDirectLink = true;
					}
				}
				if (!isAlreadyDirectLink && combinedNeighbourCacheLabelItemData) {
					CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem = assNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndi.getIndividualID());
					if (cacheLabelItem) {
						if (combinedNeighbourCacheLabelItemData->hasKnownInstancesLabelItem(cacheLabelItem)) {
							// must be deterministic, otherwise the neighbour would be referenced in the individual item
							isAlreadyDirectLink = true;
						}
					}
				}

				if (!isAlreadyDirectLink) {
					COptimizedKPSetRoleInstancesHashData& instanceHashData = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(inversed, true)->operator[](neighbourIndi.getIndividualID());
					COptimizedKPSetRoleInstancesData* instanceData = instanceHashData.mInstanceItemData;

					if (instanceData && instanceData->mKnownInstance) {
						return false;
					}
					if (instanceData && instanceData->mPossibleInstance && !deterministic) {
						return false;
					}

					if (!instanceData) {
						instanceData = new COptimizedKPSetRoleInstancesData();
						instanceData->mInstanceItem = roleInstItem;

						instanceHashData.mInstanceItemData = instanceData;

						COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* otherIndiExplicitIndirectLinkComplexRepresentationData = neighbourIndiExplicitIndirectLinkComplexRepresentationData;
						if (!otherIndiExplicitIndirectLinkComplexRepresentationData) {
							otherIndiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)roleInstItem->getIndividualIdComplexRoleData(neighbourIndi.getIndividualID(), true);
						}
						COptimizedKPSetRoleInstancesHashData& otherInstanceHashData = otherIndiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(!inversed, true)->operator[](indiRealItemRef.getIndividualID());
						otherInstanceHashData.mInstanceItemData = instanceData;


#ifdef REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS
						mDebugTransitiveCollectionStringList.append(QString("Added connection between %1 and %2 over role %3%4").arg(CIRIName::getRecentIRIName(indiRealItemRef.getIndividual()->getIndividualNameLinker())).arg(CIRIName::getRecentIRIName(neighbourIndi.getIndividual()->getIndividualNameLinker())).arg(CIRIName::getRecentIRIName(roleInstItem->getRole()->getPropertyNameLinker())).arg(inversed ? " inversed" : ""));
						mDebugTransitiveCollectionString = mDebugTransitiveCollectionStringList.join("\r\n");
#endif // REALIZATION_TRANSITIVE_EXTRACTION_DEBUG_STRINGS

					}

					bool wasAlreadyPossibleMarked = instanceData->mPossibleInstance;
					instanceData->mPossibleInstance = true;
					if (deterministic) {
						instanceData->mKnownInstance = true;
						instanceData->mPossibleInstance = false;
					}

					if (!wasAlreadyPossibleMarked && !instanceData->mKnownInstance) {
						// add to testing queue
						roleInstItem->addComplexCandidateInstance(COptimizedKPSetIndividualItemReferencePair(indiRealItemRef, CRealizationIndividualInstanceItemReference(neighbourIndi, realItem->getIndividualInstantiatedItem(neighbourIndi.getIndividual(), false))), inversed);
					}
					if (wasAlreadyPossibleMarked && instanceData->mKnownInstance) {
						// remove from testing queue
						roleInstItem->removeComplexCandidateInstance(COptimizedKPSetIndividualItemReferencePair(indiRealItemRef, CRealizationIndividualInstanceItemReference(neighbourIndi, realItem->getIndividualInstantiatedItem(neighbourIndi.getIndividual(), false))), inversed);
					}

					return true;
				}
				return false;

			}





			bool COptimizedRepresentativeKPSetOntologyRealizingThread::processRealizationMessage(COntologyRealizingItem* ontRealItem, CRealizationMessageData* messageData, CMemoryPool* memoryPools) {
				COptimizedRepresentativeKPSetOntologyRealizingItem* realItem = (COptimizedRepresentativeKPSetOntologyRealizingItem*)ontRealItem;
				for (CRealizationMessageData* messageDataIt = messageData; messageDataIt; messageDataIt = messageDataIt->getNext()) {
					if (messageDataIt->getRealizationMessageDataType() == CRealizationMessageData::TELLREALIZATIONMARKERCANDIDATES) {
						CRealizationMarkerCandidatesMessageData* rmcmd = (CRealizationMarkerCandidatesMessageData*)messageDataIt;
						CIndividualReference individualReference = rmcmd->getIndividualReference();
						CConcept* markerConcept = rmcmd->getMarkerConcept();
						CIndividualRoleCandidateTestingData indiRoleCandTestData = rmcmd->getIndividualRoleCandidateTestingData();
						COptimizedKPSetRoleInstancesItem* roleInstItem = indiRoleCandTestData.getRoleInstancesItem();
						bool inversed = indiRoleCandTestData.getInversed();
						COptimizedKPSetIndividualComplexRoleData* indiComplexData = indiRoleCandTestData.getIndividualComplexData();
						COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)indiComplexData;



						COptimizedKPSetIndividualItem* individualItem = realItem->getIndividualInstantiatedItem(individualReference.getIndividualID(),false);
						CRealizationIndividualInstanceItemReference indiRealItemRef(individualReference, individualItem);
						COptimizedKPSetRoleNeighbourInstancesHashData* indiRoleNeighbourHashData = nullptr;
						if (individualItem && individualItem->getKnownPossibleRoleNeighboursInstancesHash()->contains(roleInstItem)) {
							COptimizedKPSetRoleNeighbourInstancesHashData& roleNeighbourHashData = individualItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](roleInstItem);
							indiRoleNeighbourHashData = &roleNeighbourHashData;
						}


						CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = realItem->getBackendAssociationCacheReader()->getIndividualAssociationData(individualReference.getIndividualID());
						CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* assNeighbourRoleSetHash = indiAssData->getNeighbourRoleSetHash();
						CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCompinationLabelItem = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);

						COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* combinedNeighbourCacheLabelItemDataHash = roleInstItem->getCombinedNeighbourCacheLabelItemDataHash(inversed);
						COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* combinedNeighbourCacheLabelItemData = combinedNeighbourCacheLabelItemDataHash->value(neighbourRoleSetCompinationLabelItem);


						function<void(const CIndividualReference& candIndi, bool deterministic)> candidateIndiHandleFunction = [&](const CIndividualReference& candIndi, bool deterministic) {
							addComplexRoleExplicitIndirectNeighbourLink(roleInstItem, inversed, indiRealItemRef, indiExplicitIndirectLinkComplexRepresentationData, indiRoleNeighbourHashData, combinedNeighbourCacheLabelItemData, assNeighbourRoleSetHash, candIndi, nullptr, deterministic, realItem);
						};

						CXLinker<CIndividualReference>* detIndiLinker = rmcmd->getDeterministicIndividualReferenceLinker();
						for (CXLinker<CIndividualReference>* detIndiLinkerIt = detIndiLinker; detIndiLinkerIt; detIndiLinkerIt = detIndiLinkerIt->getNext()) {
							CIndividualReference candIndi = detIndiLinkerIt->getData();
							candidateIndiHandleFunction(candIndi, true);
						}
						CXLinker<CIndividualReference>* ndetIndiLinker = rmcmd->getNondeterministicIndividualReferenceLinker();
						for (CXLinker<CIndividualReference>* ndetIndiLinkerIt = ndetIndiLinker; ndetIndiLinkerIt; ndetIndiLinkerIt = ndetIndiLinkerIt->getNext()) {
							CIndividualReference candIndi = ndetIndiLinkerIt->getData();
							candidateIndiHandleFunction(candIndi, false);
						}

						realItem->incInitializedRoleInstancesCount();

					}
				}
				return true;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingThread::addKPSetDirectSuperInstances(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstItem, bool inversed, COptimizedKPSetIndividualItem* indiItem1, COptimizedKPSetIndividualItem* indiItem2, bool knownInstance) {
				QList<TRoleItemInversionPair> tmpInstancesList;
				COptimizedKPSetIndividualItem* succIndiItem = indiItem1;
				COptimizedKPSetIndividualItem* precIndiItem = indiItem2;
				COptimizedKPSetRoleInstancesHash* succNeighbourHash = nullptr;
				COptimizedKPSetRoleInstancesHash* precNeighbourHash = nullptr;
				if (inversed) {
					succIndiItem = indiItem2;
					precIndiItem = indiItem1;
				}


				//QString indi1String = CIRIName::getRecentIRIName(indiItem1->getIndividual()->getIndividualNameLinker());
				//QString indi2String = CIRIName::getRecentIRIName(indiItem2->getIndividual()->getIndividualNameLinker());
				//QString roleString = CIRIName::getRecentIRIName(roleInstItem->getRole()->getPropertyNameLinker());
				//QString inverseRoleString;
				//if (roleInstItem->getInverseRole()) {
				//	inverseRoleString = CIRIName::getRecentIRIName(roleInstItem->getInverseRole()->getPropertyNameLinker());
				//}

				////if (indi2String == "file:/Users/liebig/Ontologien/reasoner-eval/30.owl#Act5") {
				////	if ((roleString == "file:/Users/liebig/Ontologien/reasoner-eval/30.owl#inv-first")) {
				////		bool bug = true;
				////	}
				////}
				//if (indi2String == "http://www.semanticweb.org/ontologies/2008/1/Ontology1204037102846.owl#Denied" || indi1String == "http://www.semanticweb.org/ontologies/2008/1/Ontology1204037102846.owl#Denied") {
				//	if ((roleString == "http://www.semanticweb.org/ontologies/2008/1/Ontology1204037102846.owl#hasEmployer" || inverseRoleString == "http://www.semanticweb.org/ontologies/2008/1/Ontology1204037102846.owl#hasEmployer")) {
				//		bool bug = true;
				//	}
				//}

				COptimizedKPSetRoleNeighbourInstancesHashData& succRoleNeighbourData = succIndiItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](roleInstItem);
				succNeighbourHash = succRoleNeighbourData.getRoleSuccessorInstancesHash(true);

				COptimizedKPSetRoleInstancesHashData& succHashData = (*succNeighbourHash)[precIndiItem->getIndividualId()];

				//if (indi1String == "http://www.co-ode.org/roberts/family-tree.owl#john_archer_1804") {
				//	if ((roleString == "http://www.co-ode.org/roberts/family-tree.owl#isFatherOf" || inverseRoleString == "http://www.co-ode.org/roberts/family-tree.owl#isFatherOf")) {
				//		if (succNeighbourHash->size() >= 10) {
				//			bool bug = true;
				//		}
				//	}
				//}

				if (succHashData.mInstanceItemData == nullptr) {

					COptimizedKPSetRoleNeighbourInstancesHashData& precRoleNeighbourData = precIndiItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](roleInstItem);
					precNeighbourHash = precRoleNeighbourData.getRolePredecessorInstancesHash(true);
					COptimizedKPSetRoleInstancesHashData& precHashData = (*precNeighbourHash)[succIndiItem->getIndividualId()];

					COptimizedKPSetRoleInstancesData* data = new COptimizedKPSetRoleInstancesData();
					succHashData.mNeighbourIndividualItem = precIndiItem;
					precHashData.mNeighbourIndividualItem = succIndiItem;
					succHashData.mInstanceItemData = data;
					precHashData.mInstanceItemData = data;
					data->mInstanceItem = roleInstItem;
					data->mKnownInstance = knownInstance;
					data->mPossibleInstance = !knownInstance;
					data->mMostSpecific = true;
					if (knownInstance) {
						roleInstItem->addKnownInstance(COptimizedKPSetIndividualItemPair(succIndiItem,precIndiItem),inversed);
					} else {
						roleInstItem->addPossibleInstance(COptimizedKPSetIndividualItemPair(succIndiItem,precIndiItem),inversed);
						incOpenPossibleRoleInstancesCount(reqConfPreCompItem);
						succIndiItem->incPossibleRoleInstantiatedCount();
						precIndiItem->incPossibleRoleInstantiatedCount();
						if (roleInstItem->hasToProcessPossibleInstancesFlag()) {
							reqConfPreCompItem->addRoleInstanceItemToProcessPossibleInstances(roleInstItem);
						}
					}
					tmpInstancesList.append(*roleInstItem->getParentItemList());
				}

				if (succHashData.mInstanceItemData->mPossibleInstance && knownInstance) {
					if (succHashData.mInstanceItemData->mMostSpecific) {
						decOpenPossibleRoleInstancesCount(reqConfPreCompItem);
						succIndiItem->decPossibleRoleInstantiatedCount();
						precIndiItem->decPossibleRoleInstantiatedCount();
						roleInstItem->removePossibleInstance(COptimizedKPSetIndividualItemPair(succIndiItem,precIndiItem),inversed);
					}
					roleInstItem->addKnownInstance(COptimizedKPSetIndividualItemPair(succIndiItem,precIndiItem),inversed);
					succHashData.mInstanceItemData->mKnownInstance = true;
					succHashData.mInstanceItemData->mPossibleInstance = false;
					tmpInstancesList.append(*roleInstItem->getParentItemList());
				}


				while (!tmpInstancesList.isEmpty()) {

					TRoleItemInversionPair superRoleItemInvPair = tmpInstancesList.takeFirst();
					bool superRoleItemInversed = superRoleItemInvPair.second;
					COptimizedKPSetRoleInstancesItem* superRoleInstanceItem = superRoleItemInvPair.first;

					if (superRoleInstanceItem != reqConfPreCompItem->getTopRoleInstancesItem()) {

						COptimizedKPSetIndividualItem* superSuccIndiItem = indiItem1;
						COptimizedKPSetIndividualItem* superPrecIndiItem = indiItem2;
						COptimizedKPSetRoleInstancesHash* superSuccNeighbourHash = nullptr;
						COptimizedKPSetRoleInstancesHash* superPrecNeighbourHash = nullptr;
						if (superRoleItemInversed ^ inversed) {
							superSuccIndiItem = indiItem2;
							superPrecIndiItem = indiItem1;
						}
						COptimizedKPSetRoleNeighbourInstancesHashData& superSuccRoleNeighbourData = superSuccIndiItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](superRoleInstanceItem);
						superSuccNeighbourHash = superSuccRoleNeighbourData.getRoleSuccessorInstancesHash(true);
						COptimizedKPSetRoleInstancesHashData& superSuccHashData = (*superSuccNeighbourHash)[superPrecIndiItem->getIndividualId()];

						//QString superRoleString = CIRIName::getRecentIRIName(superRoleInstanceItem->getRole()->getPropertyNameLinker());
						//QString superInverseRoleString;
						//if (superRoleInstanceItem->getInverseRole()) {
						//	superInverseRoleString = CIRIName::getRecentIRIName(superRoleInstanceItem->getInverseRole()->getPropertyNameLinker());
						//}
						//if (indi2String == "http://www.co-ode.org/roberts/family-tree.owl#john_archer_1804" && indi1String == "http://www.co-ode.org/roberts/family-tree.owl#john_archer_1804" && (superRoleString == "http://www.co-ode.org/roberts/family-tree.owl#isFatherOf" || superInverseRoleString == "http://www.co-ode.org/roberts/family-tree.owl#isFatherOf")) {
						//	bool bug = true;
						//}


						if (superSuccHashData.mInstanceItemData == nullptr) {

							COptimizedKPSetRoleNeighbourInstancesHashData& superPrecRoleNeighbourData = superPrecIndiItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](superRoleInstanceItem);
							superPrecNeighbourHash = superPrecRoleNeighbourData.getRolePredecessorInstancesHash(true);
							COptimizedKPSetRoleInstancesHashData& superPrecHashData = (*superPrecNeighbourHash)[superSuccIndiItem->getIndividualId()];

							COptimizedKPSetRoleInstancesData* superData = new COptimizedKPSetRoleInstancesData();
							superSuccHashData.mNeighbourIndividualItem = superPrecIndiItem;
							superPrecHashData.mNeighbourIndividualItem = superSuccIndiItem;
							superSuccHashData.mInstanceItemData = superData;
							superPrecHashData.mInstanceItemData = superData;
							superData->mInstanceItem = superRoleInstanceItem;
							superData->mKnownInstance = knownInstance;
							superData->mPossibleInstance = !knownInstance;
							superData->mMostSpecific = false;
							tmpInstancesList.append(*superRoleInstanceItem->getParentItemList());
						} else {
							if (knownInstance) {
								if (superSuccHashData.mInstanceItemData->mPossibleInstance) {
									superSuccHashData.mInstanceItemData->mPossibleInstance = false;
									superSuccHashData.mInstanceItemData->mKnownInstance = true;
									if (superSuccHashData.mInstanceItemData->mMostSpecific) {
										decOpenPossibleRoleInstancesCount(reqConfPreCompItem);
										superSuccIndiItem->decPossibleRoleInstantiatedCount();
										superPrecIndiItem->decPossibleRoleInstantiatedCount();
										superRoleInstanceItem->removePossibleInstance(COptimizedKPSetIndividualItemPair(superSuccIndiItem,superPrecIndiItem),inversed);
										superSuccHashData.mInstanceItemData->mMostSpecific = false;
									}
									tmpInstancesList.append(*superRoleInstanceItem->getParentItemList());
								} else if (superSuccHashData.mInstanceItemData->mKnownInstance && superSuccHashData.mInstanceItemData->mMostSpecific) {
									superSuccHashData.mInstanceItemData->mMostSpecific = false;
									superRoleInstanceItem->removeKnownInstance(COptimizedKPSetIndividualItemPair(superSuccIndiItem,superPrecIndiItem),superRoleItemInversed);
								}
							} else {
								if (superSuccHashData.mInstanceItemData->mPossibleInstance && superSuccHashData.mInstanceItemData->mMostSpecific) {
									superRoleInstanceItem->removePossibleInstance(COptimizedKPSetIndividualItemPair(superSuccIndiItem,superPrecIndiItem),inversed);
									superSuccHashData.mInstanceItemData->mMostSpecific = false;
								}
							}
						}
					}
				}
				return true;
			}






			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::mergeSameIndividualItems(COptimizedKPSetIndividualItem* instantiatedItem1, COptimizedKPSetIndividualItem* instantiatedItem2, COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetIndividualItem* mergedIntoInstantiatedItem = instantiatedItem1;
				COptimizedKPSetIndividualItem* mergingInstantiatedItem = instantiatedItem2;
				if (instantiatedItem2->getPossibleInstantiatedCount() + instantiatedItem2->getPossibleSameInstantiatedItemSet() < instantiatedItem1->getPossibleInstantiatedCount() + instantiatedItem1->getPossibleSameInstantiatedItemSet()) {
					mergedIntoInstantiatedItem = instantiatedItem2;
					mergingInstantiatedItem = instantiatedItem1;
				}
				mergingInstantiatedItem->setItemSameIndividualMerged(mergedIntoInstantiatedItem);
				QHash<cint64,COptimizedKPSetIndividualItem*>* indiInstantiatedItemHash = reqConfPreCompItem->getIndividualInstantiatedItemHash();
				QSet<CIndividualReference>* individualSet = mergingInstantiatedItem->getKnownSameIndividualSet();
				for (QSet<CIndividualReference>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
					CIndividualReference sameIndividual(*it);
					mergedIntoInstantiatedItem->addKnownSameIndividual(sameIndividual);
					indiInstantiatedItemHash->insert(sameIndividual.getIndividualID(),mergedIntoInstantiatedItem);
				}
				return this;
			}


			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::updateParentItemsSuccessorProcessed(COptimizedKPSetConceptInstancesItem* item) {
				QList<COptimizedKPSetConceptInstancesItem*> updateList;
				QList<COptimizedKPSetConceptInstancesItem*>* parentItemList = item->getParentItemList();
				for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = parentItemList->constBegin(), itEnd = parentItemList->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetConceptInstancesItem* parentItem = *it;
					updateList.append(parentItem);
				}
				while (!updateList.isEmpty()) {
					COptimizedKPSetConceptInstancesItem* parentItem = updateList.takeFirst();
					parentItem->decUnprocessedSuccessorItemCount();
					if (parentItem->getUnprocessedSuccessorItemCount() <= 0) {
						parentItem->setAllSuccessorProcessedFlag(true);
						if (!parentItem->hasPossibleInstances() && !parentItem->hasTestingPossibleInstances()) {
							parentItem->setSelfSuccessorProcessedFlag(true);
							QList<COptimizedKPSetConceptInstancesItem*>* parentParentItemList = parentItem->getParentItemList();
							for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = parentParentItemList->constBegin(), itEnd = parentParentItemList->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetConceptInstancesItem* parentParentItem = *it;
								updateList.append(parentParentItem);
							}
						}
					}
				}
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::updateParentItemsSuccessorProcessed(COptimizedKPSetRoleInstancesItem* item) {
				QList<COptimizedKPSetRoleInstancesItem*> updateList;
				QList<TRoleItemInversionPair>* parentItemList = item->getParentItemList();
				for (QList<TRoleItemInversionPair>::const_iterator it = parentItemList->constBegin(), itEnd = parentItemList->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetRoleInstancesItem* parentItem = it->first;
					updateList.append(parentItem);
				}
				while (!updateList.isEmpty()) {
					COptimizedKPSetRoleInstancesItem* parentItem = updateList.takeFirst();
					parentItem->decUnprocessedSuccessorItemCount();
					if (parentItem->getUnprocessedSuccessorItemCount() <= 0) {
						parentItem->setAllSuccessorProcessedFlag(true);
						if (!parentItem->hasPossibleInstances() && !parentItem->hasTestingPossibleInstances()) {
							parentItem->setSelfSuccessorProcessedFlag(true);
							QList<TRoleItemInversionPair>* parentParentItemList = parentItem->getParentItemList();
							for (QList<TRoleItemInversionPair>::const_iterator it = parentParentItemList->constBegin(), itEnd = parentParentItemList->constEnd(); it != itEnd; ++it) {
								COptimizedKPSetRoleInstancesItem* parentParentItem = it->first;
								updateList.append(parentParentItem);
							}
						}
					}
				}
				return this;
			}

			bool COptimizedRepresentativeKPSetOntologyRealizingThread::finishOntologyRealizing(COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem) {
				if (reqConfPreCompItem->areAllStepFinished()) {
					if (reqConfPreCompItem->isRealizingFinished()) {
						reqConfPreCompItem->setRealizingFinished(true);
						CConcreteOntology* ontology = reqConfPreCompItem->getOntology();
						CRealization* realization = ontology->getRealization();
						realization->setRealized(true);
					}
				}
				mActiveOntItemSet.remove(reqConfPreCompItem);
				mInactiveOntItemSet.insert(reqConfPreCompItem);


				//debugWriteIndividualTypesToFile(reqConfPreCompItem); 
				//debugCheckIndividualTypesFromFile(reqConfPreCompItem);

				reqConfPreCompItem->doRealizingFinishedCallback(true);
				return true;
			}


			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::incTestedPossibleConceptInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleConceptInstancesTesting()) {
					mTestedPossibleInstancesCount += incCount;
				}
				item->incTestedPossibleConceptInstancesCount(incCount);
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::incOpenPossibleConceptInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleConceptInstancesTesting()) {
					mOpenPossibleInstancesCount += incCount;
				}
				item->incOpenPossibleConceptInstancesCount(incCount);
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::decTestedPossibleConceptInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleConceptInstancesTesting()) {
					mTestedPossibleInstancesCount -= decCount;
				}
				item->decTestedPossibleConceptInstancesCount(decCount);
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::decOpenPossibleConceptInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleConceptInstancesTesting()) {
					mOpenPossibleInstancesCount -= decCount;
				}
				item->decOpenPossibleConceptInstancesCount(decCount);
				return this;
			}







			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::incTestedPossibleRoleInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleRoleInstancesTesting()) {
					mTestedPossibleInstancesCount += incCount;
				}
				item->incTestedPossibleRoleInstancesCount(incCount);
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::incOpenPossibleRoleInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleRoleInstancesTesting()) {
					mOpenPossibleInstancesCount += incCount;
				}
				item->incOpenPossibleRoleInstancesCount(incCount);
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::decTestedPossibleRoleInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleRoleInstancesTesting()) {
					mTestedPossibleInstancesCount -= decCount;
				}
				item->decTestedPossibleRoleInstancesCount(decCount);
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::decOpenPossibleRoleInstancesCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleRoleInstancesTesting()) {
					mOpenPossibleInstancesCount -= decCount;
				}
				item->decOpenPossibleRoleInstancesCount(decCount);
				return this;
			}








			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::incTestedPossibleSameIndividualsCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleSameInstancesTesting()) {
					mTestedPossibleInstancesCount += incCount;
				}
				item->incTestedPossibleSameIndividualsCount(incCount);
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::incOpenPossibleSameIndividualsCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleSameInstancesTesting()) {
					mOpenPossibleInstancesCount += incCount;
				}
				item->incOpenPossibleSameIndividualsCount(incCount);
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::decTestedPossibleSameIndividualsCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleSameInstancesTesting()) {
					mTestedPossibleInstancesCount -= decCount;
				}
				item->decTestedPossibleSameIndividualsCount(decCount);
				return this;
			}

			COptimizedRepresentativeKPSetOntologyRealizingThread* COptimizedRepresentativeKPSetOntologyRealizingThread::decOpenPossibleSameIndividualsCount(COptimizedRepresentativeKPSetOntologyRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleSameInstancesTesting()) {
					mOpenPossibleInstancesCount -= decCount;
				}
				item->decOpenPossibleSameIndividualsCount(decCount);
				return this;
			}






			CRealizationProgress* COptimizedRepresentativeKPSetOntologyRealizingThread::getRealizationProgress() {

				bool hasProcessingRequirements = false;

				for (COntologyRealizingItem* ontPreCompItem : mActiveOntItemSet) {
					COptimizedRepresentativeKPSetOntologyRealizingItem* reqConfPreCompItem = (COptimizedRepresentativeKPSetOntologyRealizingItem*)ontPreCompItem;
					if (reqConfPreCompItem->hasRemainingProcessingRequirements()) {
						hasProcessingRequirements = true;
					}
				}

				if (mOpenPossibleInstancesCount != mTestedPossibleInstancesCount && hasProcessingRequirements) {
					cint64 tested = mTestedPossibleInstancesCount;
					cint64 total = mOpenPossibleInstancesCount;
					cint64 elapsedTime = mRealStartTime.elapsed();
					double remainingMilliSeconds = 0.;
					double progessPercent = 0.;
					if (elapsedTime <= 0 && total > 0) {
						remainingMilliSeconds = 0.;
					} else {
						remainingMilliSeconds = ((double)elapsedTime / (double)tested) * (total-tested);
					}
					if (total <= 0) {
						progessPercent = 0.;
					} else {
						progessPercent = ((double)tested / (double)total) * 100.;
					}
					cint64 classificCount = mRealizingCount;
					mRealProgress.setRealizationCount(classificCount);
					mRealProgress.setProgessPercent(progessPercent);
					mRealProgress.setRemainingMilliSeconds(remainingMilliSeconds);
					mRealProgress.setTestedClasses(tested);
					mRealProgress.setTotalClasses(total);
				} else {
					mRealProgress.setProgessPercent(0.);
					mRealProgress.setRemainingMilliSeconds(0.);
					mRealProgress.setTestedClasses(mTestedPossibleInstancesCount);
					mRealProgress.setTotalClasses(mTestedPossibleInstancesCount);
				}
				return &mRealProgress;
			}





			QSet<QString> COptimizedRepresentativeKPSetOntologyRealizingThread::debugGetIndividualTypesStringSet(COptimizedRepresentativeKPSetOntologyRealizingItem* realizingItem) {
				QSet<QString> indiTypeStringSet;
				QHash<CConcept*, COptimizedKPSetConceptInstancesItem*>* conInstHash = realizingItem->getConceptInstancesItemHash();
				for (QHash<CConcept*, COptimizedKPSetConceptInstancesItem*>::const_iterator it = conInstHash->constBegin(), itEnd = conInstHash->constEnd(); it != itEnd; ++it) {
					CConcept* concept = it.key();
					COptimizedKPSetConceptInstancesItem* instanceItem = it.value();
					QMap<cint64, COptimizedKPSetIndividualItem*>* knownInstancesSet = instanceItem->getKnownInstancesMap();
					for (QMap<cint64, COptimizedKPSetIndividualItem*>::const_iterator indiItemIt = knownInstancesSet->constBegin(), indiItemItEnd = knownInstancesSet->constEnd(); indiItemIt != indiItemItEnd; ++indiItemIt) {
						COptimizedKPSetIndividualItem* indiItem(*indiItemIt);
						QSet<CIndividualReference>* sameIndiSet = indiItem->getKnownSameIndividualSet();
						for (QSet<CIndividualReference>::const_iterator sameIndiIt = sameIndiSet->constBegin(), sameIndiItEnd = sameIndiSet->constEnd(); sameIndiIt != sameIndiItEnd; ++ sameIndiIt) {
							CIndividualReference indiRef = *sameIndiIt;
							CIndividual* indi = realizingItem->getOntology()->getABox()->getIndividualVector(false)->getData(indiRef.getIndividualID());
							indiTypeStringSet.insert(CIRIName::getRecentIRIName(indi->getIndividualNameLinker()) + " rdf:type " + CIRIName::getRecentIRIName(concept->getClassNameLinker()) + " .");
						}
					}
				}
				return indiTypeStringSet;
			}



			bool COptimizedRepresentativeKPSetOntologyRealizingThread::debugWriteIndividualTypesToFile(COptimizedRepresentativeKPSetOntologyRealizingItem* realizingItem) {
				QFile indiTypesFile("Debugging/Realizing/individual-types.ttl");
				if (indiTypesFile.open(QIODevice::WriteOnly)) {
					QSet<QString> indiTypeStringSet = debugGetIndividualTypesStringSet(realizingItem);
					for (QString indiTypeString : indiTypeStringSet) {
						indiTypesFile.write(indiTypeString.toLocal8Bit());
						indiTypesFile.write(QString("\r\n").toLocal8Bit());
					}
					indiTypesFile.close();
				}
				return true;
			}




			bool COptimizedRepresentativeKPSetOntologyRealizingThread::debugCheckIndividualTypesFromFile(COptimizedRepresentativeKPSetOntologyRealizingItem* realizingItem) {

				QFile indiTypesFile("Debugging/Realizing/individual-types.ttl");

				QSet<QString> indiTypesStringSet = debugGetIndividualTypesStringSet(realizingItem);
				QSet<QString> fileIndiTypesStringSet;

				if (indiTypesFile.open(QIODevice::ReadOnly)) {
					while (!indiTypesFile.atEnd()) {
						fileIndiTypesStringSet.insert(indiTypesFile.readLine().trimmed());
					}
					indiTypesFile.close();
				}

				QStringList itemAdditionalIndiTypes;
				for (QString indiType : indiTypesStringSet) {
					if (!fileIndiTypesStringSet.contains(indiType)) {
						itemAdditionalIndiTypes.append(indiType);
					}
				}

				QStringList fileAdditionalIndiTypes;
				for (QString indiType : fileIndiTypesStringSet) {
					if (!indiTypesStringSet.contains(indiType)) {
						fileAdditionalIndiTypes.append(indiType);
					}
				}

				if (fileAdditionalIndiTypes.size() > 0 || itemAdditionalIndiTypes.size() > 0) {
					LOG(ERROR, getDomain(), logTr("Additional individual types in file:\r\n %1").arg(fileAdditionalIndiTypes.join("\r\n")), this);
					LOG(ERROR, getDomain(), logTr("Additional individual types in current process:\r\n %1").arg(itemAdditionalIndiTypes.join("\r\n")), this);
					bool debug = true;
				}

				//QSet<QString> fileAdditionalIndiNames;
				//for (QString fileAdditionalIndiType : fileAdditionalIndiTypes) {
				//	QString indiName = fileAdditionalIndiType.split(" ")[0];
				//	fileAdditionalIndiNames.insert(indiName);
				//}


				//QList<COptimizedKPSetConceptInstancesItem*> equivClassList;
				//initializeEquivalentClassList(&equivClassList, realizingItem);

				//CIndividualVector* indiVec = realizingItem->getOntology()->getABox()->getIndividualVector(false);
				//CIndividualProcessNodeVector* indiProcVector = nullptr;
				//CConsistence* consistence = realizingItem->getOntology()->getConsistence();
				//if (consistence) {
				//	CConsistenceData* consData = consistence->getConsistenceModelData();
				//	if (consData) {
				//		CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
				//		CSatisfiableCalculationTask* satConsTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
				//		if (!satConsTask) {
				//			satConsTask = consTaskData->getDeterministicSatisfiableTask();
				//		}
				//		if (satConsTask) {
				//			CProcessingDataBox* procDataBox = satConsTask->getProcessingDataBox();
				//			if (procDataBox) {
				//				indiProcVector = procDataBox->getIndividualProcessNodeVector();
				//			}
				//		}
				//	}
				//}

				//
				//cint64 indiCount = indiVec->getItemCount();
				//for (cint64 indiID = 0; indiID < indiCount; ++indiID) {

				//	CIndividual* individual = indiVec->getData(indiID);
				//	if (individual) {
				//		QString indiName = CIRIName::getRecentIRIName(individual->getIndividualNameLinker());

				//		if (fileAdditionalIndiNames.contains(indiName)) {


				//			QList<COptimizedKPSetConceptInstancesItem*> knownInstancesList;
				//			QList<COptimizedKPSetConceptInstancesItem*> possibleInstancesList;

				//			QList<CIndividual*> knownSameIndividualList;
				//			QList<CIndividual*> possibleSameIndividualList;

				//			extractKnownPossibleIndividualDataFromConsistencyData(individual, &knownInstancesList, &possibleInstancesList, &knownSameIndividualList, &possibleSameIndividualList, indiProcVector, equivClassList, realizingItem);
				//		}
				//	}
				//}


				return true;
			}




		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
