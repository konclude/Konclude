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

#include "COptimizedKPSetOntologyConceptRealizingThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetOntologyConceptRealizingThread::COptimizedKPSetOntologyConceptRealizingThread(CReasonerManager *reasonerManager) : CRealizerThread(reasonerManager),CLogDomain("::Konclude::Reasoner::Kernel::Realizeror") {
				mTestedPossibleInstancesCount = 0;
				mOpenPossibleInstancesCount = 0;
				mRealizingCount = 0;

				mBackendAssocCache = nullptr;
				CBackendCache* backendCache = reasonerManager->getBackendAssociationCache();
				if (backendCache) {
					mBackendAssocCache = dynamic_cast<CBackendRepresentativeMemoryCache*>(backendCache);
					if (mBackendAssocCache) {
						mBackendAssocCacheReader = mBackendAssocCache->createCacheReader();
					}
				}

			}


			COptimizedKPSetOntologyConceptRealizingThread::~COptimizedKPSetOntologyConceptRealizingThread() {
			}


			COntologyRealizingItem* COptimizedKPSetOntologyConceptRealizingThread::initializeOntologyRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				COptimizedKPSetOntologyConceptRealizingItem* item = new COptimizedKPSetOntologyConceptRealizingItem();
				item->initRequirementConfigRealizingItem(ontology,config);

				if (mTestedPossibleInstancesCount == mOpenPossibleInstancesCount) {
					mTestedPossibleInstancesCount = 0;
					mOpenPossibleInstancesCount = 0;
					mRealStartTime.start();
				}
				initializeItems(item);
				++mRealizingCount;
				return item;
			}

			void COptimizedKPSetOntologyConceptRealizingThread::readCalculationConfig(CConfigurationBase *config) {
				if (config) {
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
					mConfMaxTestParallelCount = 1;
				}

			}

			bool COptimizedKPSetOntologyConceptRealizingThread::initializeItems(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				if (!reqConfPreCompItem->hasItemsInitialized()) {
					reqConfPreCompItem->initConceptItemsFromHierarchy();
					reqConfPreCompItem->initRoleItemsFromHierarchy();
					reqConfPreCompItem->initComplexRolesStarterCandidates();

					readCalculationConfig(reqConfPreCompItem->getCalculationConfiguration());

					initializeKPSetsFromConsistencyData(reqConfPreCompItem);
					reqConfPreCompItem->setItemsInitialized(true);

					QList<COptimizedKPSetConceptInstancesItem*>* itemInstancesList = reqConfPreCompItem->getProcessingPossibleConceptInstancesItemList();
					for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = itemInstancesList->constBegin(), itEnd = itemInstancesList->constEnd(); it != itEnd; ++it) {
						COptimizedKPSetConceptInstancesItem* item(*it);
						if (item->hasAllSuccessorProcessedFlag() && !item->hasPossibleInstances() && !item->hasTestingPossibleInstances()) {
							updateParentItemsSuccessorProcessed(item);
						}
					}


				}
				return true;
			}


			CIndividualProcessNode* COptimizedKPSetOntologyConceptRealizingThread::getMergeCorrectedIndividualProcessNode(CIndividualProcessNode* indiProcNode, CIndividualProcessNodeVector* indiProcVector, bool* nonDeterministicallyMergedFlag, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				bool nonDeterministicMerged = false;
				while (indiProcNode && indiProcNode->getMergedIntoIndividualNodeID() != indiProcNode->getIndividualID()) {
					cint64 mergeID = indiProcNode->getMergedIntoIndividualNodeID();
					indiProcNode = indiProcVector->getData(mergeID);
					CDependencyTrackPoint* mergedDepTrackPoint = indiProcNode->getMergedDependencyTrackPoint();
					if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
						nonDeterministicMerged = true;
					}
				}
				if (nonDeterministicallyMergedFlag) {
					*nonDeterministicallyMergedFlag = nonDeterministicMerged;
				}
				return indiProcNode;
			}

			bool itemSortLessThan(COptimizedKPSetConceptInstancesItem* item1, COptimizedKPSetConceptInstancesItem* item2) {
				return item1->getHierarchyNode()->getParentNodeSet()->count() > item2->getHierarchyNode()->getParentNodeSet()->count();
			}



			bool COptimizedKPSetOntologyConceptRealizingThread::addKPSetDirectSuperInstances(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash, COptimizedKPSetConceptInstancesItem* instanceItem, bool knownInstance) {
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



			bool COptimizedKPSetOntologyConceptRealizingThread::initializeKPSetsForIndividual(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, CIndividual* individual, QList<COptimizedKPSetConceptInstancesItem*>& knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>& possibleInstancesList, QList<CIndividual*>& knownSameIndividualList) {
				COptimizedKPSetIndividualItem* instantiatedItem = reqConfPreCompItem->getIndividualInstantiatedItem(individual,true);
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
				if (!knownSameIndividualList.isEmpty()) {
					for (QList<CIndividual*>::const_iterator it = knownSameIndividualList.constBegin(), itEnd = knownSameIndividualList.constEnd(); it != itEnd; ++it) {
						CIndividual* sameIndividual(*it);
						instantiatedItem->addKnownSameIndividual(sameIndividual);
						reqConfPreCompItem->getIndividualInstantiatedItemHash()->insert(sameIndividual,instantiatedItem);
					}
				}

				return true;
			}


			bool COptimizedKPSetOntologyConceptRealizingThread::initializeEquivalentClassList(QList<COptimizedKPSetConceptInstancesItem*>* equivClassList, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
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








			bool COptimizedKPSetOntologyConceptRealizingThread::extractKnownPossibleIndividualDataFromConsistencyData(CIndividual* individual, QList<COptimizedKPSetConceptInstancesItem*>* knownInstancesList, QList<COptimizedKPSetConceptInstancesItem*>* possibleInstancesList, QList<CIndividual*>* knownSameIndividualList, QList<CIndividual*>* possibleSameIndividualList, CIndividualProcessNodeVector* indiProcVector, const QList<COptimizedKPSetConceptInstancesItem*>& equivClassList, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* conInstItemHash = reqConfPreCompItem->getConceptInstancesItemHash();

				cint64 indiID = individual->getIndividualID();
				bool indiInitialized = false;
				if (!indiInitialized) {

					if (indiProcVector) {

						CIndividualProcessNode* baseIndiProcNode = indiProcVector->getData(indiID);
						CIndividualProcessNode* indiProcNode = baseIndiProcNode;
						if (indiProcNode) {
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
											COptimizedKPSetConceptInstancesItem* instantiatedItem = conInstItemHash->value(resolvedConcept);
											if (instantiatedItem) {
												if (deterministicTrackPoint && !resolved) {
													knownInstancesList->append(instantiatedItem);
												} else {
													possibleInstancesList->append(instantiatedItem);
												}
											}
										}
									}

									conDesLinker = conDesLinker->getNextConceptDesciptor();
								}
							}


							CIndividualMergingHash* indiMergingHash = indiProcNode->getIndividualMergingHash(false);
							if (indiMergingHash) {
								for (CIndividualMergingHash::const_iterator it = indiMergingHash->constBegin(), itEnd = indiMergingHash->constEnd(); it != itEnd; ++it) {
									CIndividual* mergedIndi = it.key();
									CDependencyTrackPoint* mergedDepTrackPoint = it.value();
									bool nondeterministicallyMerged = false;
									if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
										nondeterministicallyMerged = true;
									}
									if (mergedIndi != individual) {
										if (nondeterministicallyMerged) {
											possibleSameIndividualList->append(mergedIndi);
										} else {
											knownSameIndividualList->append(mergedIndi);
										}
									}
								}
							}

							if (nondeterministicallyMergedFlag) {
								possibleInstancesList->append(*knownInstancesList);
								knownInstancesList->clear();
								possibleSameIndividualList->append(*knownSameIndividualList);
								knownSameIndividualList->clear();

								QSet<CConcept*> knownConceptSet;
								QSet<CIndividual*> knownSameIndividualSet;

								CIndividualProcessNode* detIndiProcNode = baseIndiProcNode;
								bool stillDeterministicllyMerged = true;
								while (stillDeterministicllyMerged && detIndiProcNode) {

									CReapplyConceptLabelSet* detConLabelSet = detIndiProcNode->getReapplyConceptLabelSet(false);
									if (detConLabelSet) {


										CIndividualMergingHash* detIndiMergingHash = detIndiProcNode->getIndividualMergingHash(false);
										if (detIndiMergingHash) {
											for (CIndividualMergingHash::const_iterator it = detIndiMergingHash->constBegin(), itEnd = detIndiMergingHash->constEnd(); it != itEnd; ++it) {
												CIndividual* mergedIndi = it.key();
												CDependencyTrackPoint* mergedDepTrackPoint = it.value();
												bool nondeterministicallyMerged = false;
												if (!mergedDepTrackPoint || mergedDepTrackPoint->getBranchingTag() > 0) {
													nondeterministicallyMerged = true;
												}
												if (mergedIndi != individual) {
													if (!nondeterministicallyMerged) {
														knownSameIndividualSet.insert(mergedIndi);
													}
												}
											}													}

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
								for (QSet<CIndividual*>::const_iterator it = knownSameIndividualSet.constBegin(), itEnd = knownSameIndividualSet.constEnd(); it != itEnd; ++it) {
									CIndividual* sameIndividual(*it);
									knownSameIndividualList->append(sameIndividual);
								}

								QList<CIndividual*> tmpPossibleSameIndividualList(*possibleSameIndividualList);
								possibleSameIndividualList->clear();
								while (!tmpPossibleSameIndividualList.isEmpty()) {
									CIndividual* possSameIndividual = tmpPossibleSameIndividualList.takeFirst();
									if (!knownSameIndividualSet.contains(possSameIndividual)) {
										possibleSameIndividualList->append(possSameIndividual);
									}
								}
							}
							indiInitialized = true;
						}
					}
				}

				if (!indiInitialized) {
					CBackendRepresentativeMemoryCacheIndividualAssociationData* assData = mBackendAssocCacheReader->getIndividualAssociation(individual);
					if (assData) {
						CBackendRepresentativeMemoryLabelCacheItem* labelDataItem = assData->getBackendLabelCacheEntry();
						if (labelDataItem) {
							for (CBackendRepresentativeMemoryLabelValueLinker* valueLinker = labelDataItem->getDeterministicCacheValueLinker(); valueLinker; valueLinker = valueLinker->getNext()) {
								CCacheValue& cacheValue = valueLinker->getCacheValue();
								CConcept* concept = (CConcept*)cacheValue.getIdentification();
								bool negation = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
								if (!negation) {
									CConceptOperator* conOperator = concept->getConceptOperator();
									if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_NOMINAL)) {
										CIndividual* nominalIndividual = concept->getNominalIndividual();
										if (nominalIndividual != individual) {
											knownSameIndividualList->append(nominalIndividual);
										}
									}
									if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_ATOM | CConceptOperator::CCF_SUB | CConceptOperator::CCF_TOP | CConceptOperator::CCF_EQ | CConceptOperator::CCF_EQCAND)) {
										cint64 opCode = conOperator->getOperatorCode();
										CConcept* resolvedConcept = concept;
										bool resolved = false;
										if (opCode == CCEQCAND) {
											resolved = true;
											resolvedConcept = concept->getOperandList()->getData();
										}
										COptimizedKPSetConceptInstancesItem* instantiatedItem = conInstItemHash->value(resolvedConcept);
										if (!resolved) {
											knownInstancesList->append(instantiatedItem);
										} else {
											possibleInstancesList->append(instantiatedItem);
										}
									}
								}	
							}
							indiInitialized = true;
						}
					}
				}

				if (!equivClassList.isEmpty()) {
					possibleInstancesList->append(equivClassList);
				}

				return true;
			}









			bool COptimizedKPSetOntologyConceptRealizingThread::initializeKPSetsFromConsistencyData(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {


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

					QHash<CIndividual*,QList<CIndividual*> > individualPossibleSameIndividualListHash;

					CBOXSET<CIndividual*>* activeIndividualSet = reqConfPreCompItem->getOntology()->getABox()->getActiveIndividualSet(false);

					cint64 indiCount = indiVec->getItemCount();
					for (cint64 indiID = 0; indiID < indiCount; ++indiID) {

						CIndividual* individual = indiVec->getData(indiID);
						if (individual && !individual->isAnonymousIndividual() && !reqConfPreCompItem->hasIndividualInstantiatedItem(individual) && activeIndividualSet->contains(individual)) {


							QList<COptimizedKPSetConceptInstancesItem*> knownInstancesList;
							QList<COptimizedKPSetConceptInstancesItem*> possibleInstancesList;

							QList<CIndividual*> knownSameIndividualList;
							QList<CIndividual*> possibleSameIndividualList;

							extractKnownPossibleIndividualDataFromConsistencyData(individual,&knownInstancesList,&possibleInstancesList,&knownSameIndividualList,&possibleSameIndividualList,indiProcVector,equivClassList,reqConfPreCompItem);

							initializeKPSetsForIndividual(reqConfPreCompItem,individual,knownInstancesList,possibleInstancesList,knownSameIndividualList);

							if (!possibleSameIndividualList.isEmpty()) {
								individualPossibleSameIndividualListHash.insert(individual,possibleSameIndividualList);
							}
						}
					}

					if (!individualPossibleSameIndividualListHash.isEmpty()) {
						QSet<COptimizedKPSetIndividualItem*> singleIndividualSet;
						for (QHash<CIndividual*,QList<CIndividual*> >::const_iterator it = individualPossibleSameIndividualListHash.constBegin(), itEnd = individualPossibleSameIndividualListHash.constEnd(); it != itEnd; ++it) {
							CIndividual* individual = it.key();
							COptimizedKPSetIndividualItem* individualItem = reqConfPreCompItem->getIndividualInstantiatedItem(individual,false);
							if (!singleIndividualSet.contains(individualItem)) {
								singleIndividualSet.insert(individualItem);
								const QList<CIndividual*> possibleSameIndividualList = it.value();
								initializeSamePossibleIndividuals(reqConfPreCompItem,individualItem,possibleSameIndividualList);
							}
						}
					}

				}

				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingThread::initializeSamePossibleIndividuals(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* individualItem, const QList<CIndividual*>& possibleSameIndividualList) {
				reqConfPreCompItem->addPossibleSameIndividualsItem(individualItem);
				for (QList<CIndividual*>::const_iterator it = possibleSameIndividualList.constBegin(), itEnd = possibleSameIndividualList.constEnd(); it != itEnd; ++it) {
					CIndividual* possSameIndividual(*it);
					COptimizedKPSetIndividualItem* possSameIndividualItem = reqConfPreCompItem->getIndividualInstantiatedItem(possSameIndividual,false);
					individualItem->addPossibleSameIndividualItem(possSameIndividualItem);
					possSameIndividualItem->addPossibleSameIndividualItem(individualItem);
					incOpenPossibleSameIndividualsCount(reqConfPreCompItem);
				}
				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingThread::createNextConceptInstantiationTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetIndividualItem* instantiatedItem) {
				instancesItem->incTestingPossibleInstancesCount();
				instantiatedItem->incTestingPossibleInstantiatedCount();
				instantiatedItem->decPossibleInstantiatedCount();
				instantiatedItem->setTestingPossibleInstantiated(instancesItem);

				CConcept* concept = instancesItem->getHierarchyNode()->getOneEquivalentConcept();
				CIndividual* individual = instantiatedItem->getIndividual();

				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(concept,true,individual);
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

				CIndividualConceptInstanceTestingItem* testItem = new CIndividualConceptInstanceTestingItem(reqConfPreCompItem,instancesItem,instantiatedItem);
				reqConfPreCompItem->incTestingPossibleConceptInstanceCount();

				processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
				return true;
			}



			bool COptimizedKPSetOntologyConceptRealizingThread::createNextRoleInstantiationTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetIndividualItemPair itemPair) {
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

				CIndividualPairRoleInstanceTestingItem* testItem = new CIndividualPairRoleInstanceTestingItem(reqConfPreCompItem,instancesItem,itemPair);
				reqConfPreCompItem->incTestingPossibleRoleInstanceCount();

				processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
				return true;
			}






			CIndividualProcessNode* COptimizedKPSetOntologyConceptRealizingThread::getCompletionGraphCachedIndividualProcessNode(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* indiItem, bool* nondeterministicNodeFlag) {

				CIndividual* individual = indiItem->getIndividual();
				cint64 indiID = individual->getIndividualID();

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






			bool COptimizedKPSetOntologyConceptRealizingThread::createNextRoleInitializingTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* individualItem) {

				bool nondeterministicIndividual = false;
				CIndividualProcessNode* indiNode = getCompletionGraphCachedIndividualProcessNode(reqConfPreCompItem,individualItem,&nondeterministicIndividual);
				individualItem->setAllRoleCandidatesInitializedFlag(true);

				//QString indiString = CIRIName::getRecentIRIName(individualItem->getIndividual()->getIndividualNameLinker());
				//if (indiString == "http://www.co-ode.org/roberts/family-tree.owl#john_archer_1804") {
				//	bool bug = true;
				//}

				QList<CConcept*> propConceptList;
				QList<CConcept*> markerConceptList;
				QList<COptimizedKPSetRoleInstancesItem*>* roleInstItemList = reqConfPreCompItem->getRoleInstancesItemList();
				for (QList<COptimizedKPSetRoleInstancesItem*>::const_iterator it = roleInstItemList->constBegin(), itEnd = roleInstItemList->constEnd(); it != itEnd; ++it) {
					COptimizedKPSetRoleInstancesItem* roleInstItem(*it);

					//QString roleString = CIRIName::getRecentIRIName(roleInstItem->getRole()->getPropertyNameLinker());
					//QString inverseRoleString = CIRIName::getRecentIRIName(roleInstItem->getInverseRole()->getPropertyNameLinker());
					//if (roleString == "http://www.co-ode.org/roberts/family-tree.owl#isFatherOf" || inverseRoleString == "http://www.co-ode.org/roberts/family-tree.owl#isFatherOf") {
					//	bool bug = true;
					//}

					COptimizedKPSetRoleNeighbourInstancesHashData& roleNeighbourHashData = individualItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](roleInstItem);
					if (!roleNeighbourHashData.mRoleSuccessorInstancesInitializing) {
						roleNeighbourHashData.mRoleSuccessorInstancesInitializing = true;

						if (roleInstItem == reqConfPreCompItem->getTopRoleInstancesItem()) {
							roleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
						} else {
							CRole* role = roleInstItem->getRole();
							if (!role->isComplexRole()) {

								if (indiNode) {
									CRoleSuccessorLinkIterator roleSuccIt = indiNode->getRoleSuccessorLinkIterator(role);
									while (roleSuccIt.hasNext()) {
										CIndividualLinkEdge* succLink = roleSuccIt.next();
										CIndividualProcessNode* succIndiNode = succLink->getOppositeIndividual(indiNode);
										CIndividual* succIndi = succIndiNode->getNominalIndividual();
										if (succIndi) {
											bool nondet = true;
											CDependencyTrackPoint* linkDepTrackPoint = succLink->getDependencyTrackPoint();
											if (!nondeterministicIndividual && linkDepTrackPoint && linkDepTrackPoint->getBranchingTag() <= 0) {
												nondet = false;
											}
											COptimizedKPSetIndividualItem* succIndiItem = reqConfPreCompItem->getIndividualInstantiatedItem(succIndi,false);
											if (succIndiItem) {
												addKPSetDirectSuperInstances(reqConfPreCompItem,roleInstItem,false,individualItem,succIndiItem,!nondet);
											}
										}

									}
								} else {
									CIndividual* individual = individualItem->getIndividual();
									for (CRoleAssertionLinker* roleAssLinkerIt = individual->getAssertionRoleLinker(); roleAssLinkerIt; roleAssLinkerIt = roleAssLinkerIt->getNext()) {
										CRole* assRole = roleAssLinkerIt->getRole();
										for (CSortedNegLinker<CRole*>* assSuperRoleIt = assRole->getIndirectSuperRoleList(); assSuperRoleIt; assSuperRoleIt = assSuperRoleIt->getNext()) {
											CRole* assSuperRole = assSuperRoleIt->getData();
											bool assSuperRoleInversed = assSuperRoleIt->isNegated();
											if (assSuperRole == role && !assSuperRoleInversed) {
												CIndividual* succIndi = roleAssLinkerIt->getIndividual();
												COptimizedKPSetIndividualItem* succIndiItem = reqConfPreCompItem->getIndividualInstantiatedItem(succIndi,false);
												if (succIndiItem) {
													addKPSetDirectSuperInstances(reqConfPreCompItem,roleInstItem,false,individualItem,succIndiItem,false);
													break;
												}
											}
										}
									}
								}
								roleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;

							} else {
								bool foundStarterCandidate = false;
								if (indiNode) {
									CReapplyRoleSuccessorHash* roleSuccHash = indiNode->getReapplyRoleSuccessorHash(false);
									if (roleSuccHash) {
										CRoleSuccessorIterator roleSuccIt = roleSuccHash->getRoleIterator();
										while (roleSuccIt.hasNext() && !foundStarterCandidate) {
											CRole* candRole = roleSuccIt.next();
											if (roleInstItem->hasComplexRoleStarterCandidate(candRole)) {
												foundStarterCandidate = true;
											}
										}
									}
								} else {
									CIndividual* individual = individualItem->getIndividual();
									for (CRoleAssertionLinker* roleAssLinkerIt = individual->getAssertionRoleLinker(); roleAssLinkerIt && !foundStarterCandidate; roleAssLinkerIt = roleAssLinkerIt->getNext()) {
										CRole* assRole = roleAssLinkerIt->getRole();
										for (CSortedNegLinker<CRole*>* assSuperRoleIt = assRole->getIndirectSuperRoleList(); assSuperRoleIt && !foundStarterCandidate; assSuperRoleIt = assSuperRoleIt->getNext()) {
											CRole* assSuperRole = assSuperRoleIt->getData();
											bool assSuperRoleInversed = assSuperRoleIt->isNegated();
											if (!assSuperRoleInversed) {
												if (roleInstItem->hasComplexRoleStarterCandidate(assSuperRole)) {
													foundStarterCandidate = true;
												}
											}
										}
									}
								}

								if (foundStarterCandidate) {
									CConcept* propConcept = roleInstItem->getTemporaryPropagationConcept();
									CConcept* markerConcept = roleInstItem->getTemporaryMarkerConcept();
									propConceptList.append(propConcept);
									markerConceptList.append(markerConcept);
								} else {
									roleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
								}
							}
						}
						reqConfPreCompItem->incInitializedRoleInstancesCount();
					}


					if (!roleNeighbourHashData.mRolePredecessorInstancesInitializing && roleInstItem->hasInverseRole()) {
						roleNeighbourHashData.mRolePredecessorInstancesInitializing = true;

						if (roleInstItem == reqConfPreCompItem->getTopRoleInstancesItem()) {
							roleNeighbourHashData.mRolePredecessorInstancesInitialized = true;
						} else {

							CRole* invRole = roleInstItem->getInverseRole();
							if (invRole) {
								if (!invRole->isComplexRole()) {

									if (indiNode) {
										CRoleSuccessorLinkIterator roleSuccIt = indiNode->getRoleSuccessorLinkIterator(invRole);
										while (roleSuccIt.hasNext()) {
											CIndividualLinkEdge* succLink = roleSuccIt.next();
											CIndividualProcessNode* succIndiNode = succLink->getOppositeIndividual(indiNode);
											CIndividual* succIndi = succIndiNode->getNominalIndividual();
											if (succIndi) {
												bool nondet = true;
												CDependencyTrackPoint* linkDepTrackPoint = succLink->getDependencyTrackPoint();
												if (!nondeterministicIndividual && linkDepTrackPoint && linkDepTrackPoint->getBranchingTag() <= 0) {
													nondet = false;
												}
												COptimizedKPSetIndividualItem* succIndiItem = reqConfPreCompItem->getIndividualInstantiatedItem(succIndi,false);
												if (succIndiItem) {
													addKPSetDirectSuperInstances(reqConfPreCompItem,roleInstItem,true,individualItem,succIndiItem,!nondet);
												}
											}

										}
									} else {
										CIndividual* individual = individualItem->getIndividual();
										for (CReverseRoleAssertionLinker* reverseRoleAssLinkerIt = individual->getReverseAssertionRoleLinker(); reverseRoleAssLinkerIt; reverseRoleAssLinkerIt = reverseRoleAssLinkerIt->getNext()) {
											CRole* assRole = reverseRoleAssLinkerIt->getRole();
											for (CSortedNegLinker<CRole*>* assSuperRoleIt = assRole->getIndirectSuperRoleList(); assSuperRoleIt; assSuperRoleIt = assSuperRoleIt->getNext()) {
												CRole* assSuperRole = assSuperRoleIt->getData();
												bool assSuperRoleInversed = assSuperRoleIt->isNegated();
												if (assSuperRole == invRole && assSuperRoleInversed) {
													CIndividual* succIndi = reverseRoleAssLinkerIt->getIndividual();
													COptimizedKPSetIndividualItem* succIndiItem = reqConfPreCompItem->getIndividualInstantiatedItem(succIndi,false);
													if (succIndiItem) {
														addKPSetDirectSuperInstances(reqConfPreCompItem,roleInstItem,true,individualItem,succIndiItem,false);
														break;
													}
												}
											}
										}
									}
									roleNeighbourHashData.mRolePredecessorInstancesInitialized = true;

								} else {

									bool foundStarterCandidate = false;
									if (indiNode) {
										CReapplyRoleSuccessorHash* roleSuccHash = indiNode->getReapplyRoleSuccessorHash(false);
										if (roleSuccHash) {
											CRoleSuccessorIterator roleSuccIt = roleSuccHash->getRoleIterator();
											while (roleSuccIt.hasNext() && !foundStarterCandidate) {
												CRole* candRole = roleSuccIt.next();
												if (roleInstItem->hasComplexInverseRoleStarterCandidate(candRole)) {
													foundStarterCandidate = true;
												}
											}
										}
									} else {
										CIndividual* individual = individualItem->getIndividual();
										for (CReverseRoleAssertionLinker* reverseRoleAssLinkerIt = individual->getReverseAssertionRoleLinker(); reverseRoleAssLinkerIt; reverseRoleAssLinkerIt = reverseRoleAssLinkerIt->getNext()) {
											CRole* assRole = reverseRoleAssLinkerIt->getRole();
											for (CSortedNegLinker<CRole*>* assSuperRoleIt = assRole->getIndirectSuperRoleList(); assSuperRoleIt && !foundStarterCandidate; assSuperRoleIt = assSuperRoleIt->getNext()) {
												CRole* assSuperRole = assSuperRoleIt->getData();
												bool assSuperRoleInversed = assSuperRoleIt->isNegated();
												if (assSuperRoleInversed) {
													if (roleInstItem->hasComplexInverseRoleStarterCandidate(assSuperRole)) {
														foundStarterCandidate = true;
													}
												}
											}
										}
									}

									if (foundStarterCandidate) {
										CConcept* propConcept = roleInstItem->getTemporaryInversePropagationConcept();
										CConcept* markerConcept = roleInstItem->getTemporaryInverseMarkerConcept();
										propConceptList.append(propConcept);
										markerConceptList.append(markerConcept);
									} else {
										roleNeighbourHashData.mRolePredecessorInstancesInitialized = true;
									}
								}	
							} else {
								roleNeighbourHashData.mRolePredecessorInstancesInitialized = true;
							}
						}
					}
				}


				if (!propConceptList.isEmpty()) {
					reqConfPreCompItem->incTestingRoleInstanceCandidatesCount();
					individualItem->incInitializingRoleCandidateCount();
					CIndividual* indi = individualItem->getIndividual();
					QList<CConcept*> markerConceptList;

					CSatisfiableCalculationJob* satCalcJob = nullptr;
					CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getTemporaryRoleRealizationOntology());
					for (QList<CConcept*>::const_iterator it = propConceptList.constBegin(), itEnd = propConceptList.constEnd(); it != itEnd; ++it) {
						CConcept* propConcept(*it);
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(propConcept,false,indi,satCalcJob);
					}

					CIndividualRoleCandidateTestingItem* testItem = new CIndividualRoleCandidateTestingItem(reqConfPreCompItem,individualItem,markerConceptList);
					satCalcJob->setRealizationMarkedCandidatesMessageAdapter(new CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter(indi,reqConfPreCompItem->getOntology(),this));
					satCalcJob->setCalculationConfiguration(reqConfPreCompItem->getCalculationConfiguration());
					if (reqConfPreCompItem->requiresIndividualDependenceTracking()) {
						if (!individualItem->getIndividualDependenceTrackingCollector()) {
							individualItem->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
						}
					}
					if (individualItem->getIndividualDependenceTrackingCollector()) {
						satCalcJob->setSatisfiableTaskIndividualDependenceTrackingAdapter(new CSatisfiableTaskIndividualDependenceTrackingAdapter(individualItem->getIndividualDependenceTrackingCollector()));
					}

					processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
					return true;
				}
				return false;
			}




			bool COptimizedKPSetOntologyConceptRealizingThread::createNextRoleInitializingTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetIndividualItem* individualItem) {
				instancesItem->decUninitializedSuccessorItemCount();

				//QString indiString = CIRIName::getRecentIRIName(individualItem->getIndividual()->getIndividualNameLinker());
				//QString roleString = CIRIName::getRecentIRIName(instancesItem->getRole()->getPropertyNameLinker());
				//QString inverseRoleString;
				//if (instancesItem->getInverseRole()) {
				//	inverseRoleString = CIRIName::getRecentIRIName(instancesItem->getInverseRole()->getPropertyNameLinker());
				//}
				//if (indiString == "file:/Users/liebig/Ontologien/reasoner-eval/30.owl#Act5" && (roleString == "file:/Users/liebig/Ontologien/reasoner-eval/30.owl#first")) {
				//	bool bug = true;
				//}
				//if (indiString == "file:/Users/liebig/Ontologien/reasoner-eval/30.owl#Madonna" && (roleString == "file:/Users/liebig/Ontologien/reasoner-eval/30.owl#inv-first")) {
				//	bool bug = true;
				//}


				COptimizedKPSetRoleNeighbourInstancesHashData& roleNeighbourHashData = individualItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](instancesItem);
				if (!roleNeighbourHashData.mRoleSuccessorInstancesInitializing) {
					roleNeighbourHashData.mRoleSuccessorInstancesInitializing = true;

					if (instancesItem == reqConfPreCompItem->getTopRoleInstancesItem()) {
						roleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
					} else {
						CRole* role = instancesItem->getRole();

						bool nondeterministicIndividual = false;
						CIndividualProcessNode* indiNode = getCompletionGraphCachedIndividualProcessNode(reqConfPreCompItem,individualItem,&nondeterministicIndividual);

						if (!role->isComplexRole()) {

							if (indiNode) {
								CRoleSuccessorLinkIterator roleSuccIt = indiNode->getRoleSuccessorLinkIterator(role);
								while (roleSuccIt.hasNext()) {
									CIndividualLinkEdge* succLink = roleSuccIt.next();
									CIndividualProcessNode* succIndiNode = succLink->getOppositeIndividual(indiNode);
									CIndividual* succIndi = succIndiNode->getNominalIndividual();
									if (succIndi) {
										bool nondet = true;
										CDependencyTrackPoint* linkDepTrackPoint = succLink->getDependencyTrackPoint();
										if (!nondeterministicIndividual && linkDepTrackPoint && linkDepTrackPoint->getBranchingTag() <= 0) {
											nondet = false;
										}
										COptimizedKPSetIndividualItem* succIndiItem = reqConfPreCompItem->getIndividualInstantiatedItem(succIndi,false);
										if (succIndiItem) {
											addKPSetDirectSuperInstances(reqConfPreCompItem,instancesItem,false,individualItem,succIndiItem,!nondet);
										}
									}
											
								}
							} else {
								CIndividual* individual = individualItem->getIndividual();
								for (CRoleAssertionLinker* roleAssLinkerIt = individual->getAssertionRoleLinker(); roleAssLinkerIt; roleAssLinkerIt = roleAssLinkerIt->getNext()) {
									CRole* assRole = roleAssLinkerIt->getRole();
									for (CSortedNegLinker<CRole*>* assSuperRoleIt = assRole->getIndirectSuperRoleList(); assSuperRoleIt; assSuperRoleIt = assSuperRoleIt->getNext()) {
										CRole* assSuperRole = assSuperRoleIt->getData();
										bool assSuperRoleInversed = assSuperRoleIt->isNegated();
										if (assSuperRole == role && !assSuperRoleInversed) {
											CIndividual* succIndi = roleAssLinkerIt->getIndividual();
											COptimizedKPSetIndividualItem* succIndiItem = reqConfPreCompItem->getIndividualInstantiatedItem(succIndi,false);
											if (succIndiItem) {
												addKPSetDirectSuperInstances(reqConfPreCompItem,instancesItem,false,individualItem,succIndiItem,false);
												break;
											}
										}
									}
								}
								roleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
							}
							roleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
							reqConfPreCompItem->incInitializedRoleInstancesCount();

						} else {


							//QStringList compRoleNameList;
							//QStringList invCompRoleNameList;
							//QStringList markingCompRoleNameList;
							//QStringList invMarkingCompRoleNameList;


							//QString roleString = CIRIName::getRecentIRIName(instancesItem->getRole()->getPropertyNameLinker());
							//QString inverseRoleString;
							//if (instancesItem->getInverseRole()) {
							//	inverseRoleString = CIRIName::getRecentIRIName(instancesItem->getInverseRole()->getPropertyNameLinker());
							//}
							//if (indiString == "http://www.co-ode.org/roberts/family-tree.owl#john_archer_1804" && (roleString == "http://www.co-ode.org/roberts/family-tree.owl#isRelationOf" || inverseRoleString == "http://www.co-ode.org/roberts/family-tree.owl#isRelationOf")) {
							//	bool bug = true;
							//}
							//compRoleNameList.append(roleString);
							//invCompRoleNameList.append(inverseRoleString);


							bool foundStarterCandidate = false;
							if (indiNode) {
								CReapplyRoleSuccessorHash* roleSuccHash = indiNode->getReapplyRoleSuccessorHash(false);
								if (roleSuccHash) {
									CRoleSuccessorIterator roleSuccIt = roleSuccHash->getRoleIterator();
									while (roleSuccIt.hasNext() && !foundStarterCandidate) {
										CRole* candRole = roleSuccIt.next();
										if (instancesItem->hasComplexRoleStarterCandidate(candRole)) {
											foundStarterCandidate = true;
										}
									}
								}
							} else {
								CIndividual* individual = individualItem->getIndividual();
								for (CRoleAssertionLinker* roleAssLinkerIt = individual->getAssertionRoleLinker(); roleAssLinkerIt && !foundStarterCandidate; roleAssLinkerIt = roleAssLinkerIt->getNext()) {
									CRole* assRole = roleAssLinkerIt->getRole();
									for (CSortedNegLinker<CRole*>* assSuperRoleIt = assRole->getIndirectSuperRoleList(); assSuperRoleIt && !foundStarterCandidate; assSuperRoleIt = assSuperRoleIt->getNext()) {
										CRole* assSuperRole = assSuperRoleIt->getData();
										bool assSuperRoleInversed = assSuperRoleIt->isNegated();
										if (!assSuperRoleInversed) {
											if (instancesItem->hasComplexRoleStarterCandidate(assSuperRole)) {
												foundStarterCandidate = true;
											}
										}
									}
								}
							}

							if (!foundStarterCandidate) {
								roleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
								reqConfPreCompItem->incInitializedRoleInstancesCount();
							} else {

								individualItem->incInitializingRoleCandidateCount();
								reqConfPreCompItem->incTestingRoleInstanceCandidatesCount();
								QList<CConcept*> markerConceptList;

								CConcept* propConcept = instancesItem->getTemporaryPropagationConcept();
								CIndividual* indi = individualItem->getIndividual();
								markerConceptList.append(instancesItem->getTemporaryMarkerConcept());

								//markingCompRoleNameList.append(roleString+QString("- P:%1 - M:%2").arg(propConcept->getConceptTag()).arg(instancesItem->getTemporaryMarkerConcept()->getConceptTag()));
								//invMarkingCompRoleNameList.append(inverseRoleString);

								CSatisfiableCalculationJob* satCalcJob = nullptr;
								CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getTemporaryRoleRealizationOntology());
								satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(propConcept,false,indi);

								QList<COptimizedKPSetRoleInstancesItem*>* compRoleInstItemList = reqConfPreCompItem->getComplexRoleInstancesItemList();
								for (QList<COptimizedKPSetRoleInstancesItem*>::const_iterator it = compRoleInstItemList->constBegin(), itEnd = compRoleInstItemList->constEnd(); it != itEnd; ++it) {
									COptimizedKPSetRoleInstancesItem* compRoleInstItem(*it);
									if (compRoleInstItem != instancesItem && compRoleInstItem->hasToProcessPossibleInstancesFlag()) {
										COptimizedKPSetRoleNeighbourInstancesHashData& compRoleNeighbourHashData = individualItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](compRoleInstItem);
										if (!compRoleNeighbourHashData.mRoleSuccessorInstancesInitializing) {
											compRoleNeighbourHashData.mRoleSuccessorInstancesInitializing = true;


											//QString compRoleString = CIRIName::getRecentIRIName(compRoleInstItem->getRole()->getPropertyNameLinker());
											//QString complInverseRoleString;
											//if (compRoleInstItem->getInverseRole()) {
											//	complInverseRoleString = CIRIName::getRecentIRIName(compRoleInstItem->getInverseRole()->getPropertyNameLinker());
											//}
											//if (indiString == "http://www.co-ode.org/roberts/family-tree.owl#john_archer_1804" && (compRoleString == "http://www.co-ode.org/roberts/family-tree.owl#isRelationOf" || complInverseRoleString == "http://www.co-ode.org/roberts/family-tree.owl#isRelationOf")) {
											//	bool bug = true;
											//}
											//compRoleNameList.append(compRoleString);
											//invCompRoleNameList.append(complInverseRoleString);

											if (compRoleInstItem == reqConfPreCompItem->getTopRoleInstancesItem()) {
												compRoleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
											} else {

												bool compRoleFoundStarterCandidate = false;
												if (indiNode) {
													CReapplyRoleSuccessorHash* roleSuccHash = indiNode->getReapplyRoleSuccessorHash(false);
													if (roleSuccHash) {
														CRoleSuccessorIterator roleSuccIt = roleSuccHash->getRoleIterator();
														while (roleSuccIt.hasNext() && !compRoleFoundStarterCandidate) {
															CRole* compCandRole = roleSuccIt.next();
															if (compRoleInstItem->hasComplexInverseRoleStarterCandidate(compCandRole)) {
																compRoleFoundStarterCandidate = true;
															}
														}
													}
												} else {
													CIndividual* individual = individualItem->getIndividual();
													for (CRoleAssertionLinker* roleAssLinkerIt = individual->getAssertionRoleLinker(); roleAssLinkerIt && !foundStarterCandidate; roleAssLinkerIt = roleAssLinkerIt->getNext()) {
														CRole* assRole = roleAssLinkerIt->getRole();
														for (CSortedNegLinker<CRole*>* assSuperRoleIt = assRole->getIndirectSuperRoleList(); assSuperRoleIt && !foundStarterCandidate; assSuperRoleIt = assSuperRoleIt->getNext()) {
															CRole* assSuperRole = assSuperRoleIt->getData();
															bool assSuperRoleInversed = assSuperRoleIt->isNegated();
															if (!assSuperRoleInversed) {
																if (compRoleInstItem->hasComplexRoleStarterCandidate(assSuperRole)) {
																	compRoleFoundStarterCandidate = true;
																}
															}
														}
													}
												}
												if (compRoleFoundStarterCandidate) {
													//markingCompRoleNameList.append(compRoleString+QString("- P:%1 - M:%2").arg(compRoleInstItem->getTemporaryPropagationConcept()->getConceptTag()).arg(compRoleInstItem->getTemporaryMarkerConcept()->getConceptTag()));
													//invMarkingCompRoleNameList.append(complInverseRoleString);

													CConcept* compPropConcept = compRoleInstItem->getTemporaryPropagationConcept();
													markerConceptList.append(compRoleInstItem->getTemporaryMarkerConcept());
													satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(compPropConcept,false,indi,satCalcJob);
												} else {
													compRoleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
												}
											}
										}
									}
								}

								//QString compRoleNameListString = compRoleNameList.join("\r\n");
								//QString invCompRoleNameListString = invCompRoleNameList.join("\r\n");
								//QString markingCompRoleNameListString = markingCompRoleNameList.join("\r\n");
								//QString invMarkingCompRoleNameListString = invMarkingCompRoleNameList.join("\r\n");

								CIndividualRoleCandidateTestingItem* testItem = new CIndividualRoleCandidateTestingItem(reqConfPreCompItem,individualItem,markerConceptList);
								satCalcJob->setRealizationMarkedCandidatesMessageAdapter(new CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter(indi,reqConfPreCompItem->getOntology(),this));
								satCalcJob->setCalculationConfiguration(reqConfPreCompItem->getCalculationConfiguration());
								if (reqConfPreCompItem->requiresIndividualDependenceTracking()) {
									if (!individualItem->getIndividualDependenceTrackingCollector()) {
										individualItem->setIndividualDependenceTrackingCollector(new CIndividualDependenceTrackingCollector());
									}
								}
								if (individualItem->getIndividualDependenceTrackingCollector()) {
									satCalcJob->setSatisfiableTaskIndividualDependenceTrackingAdapter(new CSatisfiableTaskIndividualDependenceTrackingAdapter(individualItem->getIndividualDependenceTrackingCollector()));
								}

								processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
								return true;
							}
						}
					}
				}
				return false;
			}






			bool COptimizedKPSetOntologyConceptRealizingThread::createIndividualsConsistencyTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {

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




			bool COptimizedKPSetOntologyConceptRealizingThread::createNextSameIndividualsTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetIndividualItem* instantiatedItem1, COptimizedKPSetIndividualItem* instantiatedItem2) {
				instantiatedItem1->incTestingPossibleSameIndividualCount();
				instantiatedItem2->incTestingPossibleSameIndividualCount();

				bool temporaryNominalConcept = false;
				CIndividual* individual1 = instantiatedItem1->getIndividual();
				CIndividual* individual2 = instantiatedItem2->getIndividual();
				CConcept* nominalConcept2 = individual2->getIndividualNominalConcept();
				if (!nominalConcept2) {
					nominalConcept2 = instantiatedItem2->getTemporaryIndividualNominalConcept();
					temporaryNominalConcept = true;
				}

				CConcept* testingNominalConcept = nominalConcept2;
				CIndividual* testingIndividual = individual1;
				CIndividual* otherTestringIndividual = individual2;

				if (!testingNominalConcept) {
					testingIndividual = individual2;
					otherTestringIndividual = individual1;
					temporaryNominalConcept = false;
					testingNominalConcept = individual1->getIndividualNominalConcept();
					if (!testingNominalConcept) {
						temporaryNominalConcept = true;
						testingNominalConcept = instantiatedItem1->getTemporaryIndividualNominalConcept();
					}
				}


				if (testingNominalConcept) {
					CSatisfiableCalculationJob* satCalcJob = nullptr;
					CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getOntology());
					satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(testingNominalConcept,true,testingIndividual);

					if (temporaryNominalConcept) {
						satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(testingNominalConcept,false,otherTestringIndividual);
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

					CIndividualSameTestingItem* testItem = new CIndividualSameTestingItem(reqConfPreCompItem,instantiatedItem1,instantiatedItem2);
					reqConfPreCompItem->incTestingPossibleSameIndividualCount();

					processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
					return true;
				} else {
					LOG(ERROR,getDomain(),logTr("Failed testing whether '%1' and '%2' are the same individuals, nominal concepts are not available.").arg(CIRIName::getRecentIRIName(individual1->getIndividualNameLinker())).arg(CIRIName::getRecentIRIName(individual2->getIndividualNameLinker())),this);
					return false;
				}
			}


			bool COptimizedKPSetOntologyConceptRealizingThread::createNextTest() {


				COntologyRealizingItem* loopOntPreCompItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !mProcessingOntItemList.isEmpty()) {
					COntologyRealizingItem* ontPreCompItem = mProcessingOntItemList.first();

					if (ontPreCompItem == loopOntPreCompItem) {
						// don't run into infinite loop without doing something
						break;
					}

					COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem = (COptimizedKPSetOntologyConceptRealizingItem*)ontPreCompItem;



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
									LOG(INFO,getDomain(),logTr("Realize same individuals for ontology."),this);
								}

								if (reqConfPreCompItem->hasRemainingProcessingSameIndividualsItems()) {										
									QList<COptimizedKPSetIndividualItem*>* processingSameIndividualItemList = reqConfPreCompItem->getProcessingPossibleSameIndividualsItemList();
									while (!workTestCreated && !processingSameIndividualItemList->isEmpty()) {
										COptimizedKPSetIndividualItem* nextProcessingItem = processingSameIndividualItemList->first();

										if (!nextProcessingItem->isItemSameIndividualMerged()) {

											COptimizedKPSetIndividualItem* nextPossibleSameIndividualTestItem = nextProcessingItem->takeTestingPossibleSameIndividualItem();
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
									}
								}
									
								if (!workTestCreated && !reqConfPreCompItem->hasRemainingProcessingSameIndividualsItems()) {										
									if (!reqConfPreCompItem->hasTestingPossibleSameIndividual()) {


										if (!reqConfPreCompItem->isSameRealizationInstalled()) {
											reqConfPreCompItem->setSameRealizationInstalled(true);
											CConcreteOntology* ontology = reqConfPreCompItem->getOntology();

											CRealizingTestingStep* ontProcStep = reqConfPreCompItem->getRealizeSameIndividualsProcessingStep();
											addRealizationStatistics(reqConfPreCompItem,ontProcStep);

											CRealization* realization = ontology->getRealization();
											realization->setSameRealization(reqConfPreCompItem);
											LOG(INFO,getDomain(),logTr("Same individual realization finished for ontology."),this);
										}



										reqConfPreCompItem->getRealizeSameIndividualsProcessingStep()->setStepFinished(true);
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
									LOG(INFO,getDomain(),logTr("Realize individual types for ontology."),this);
								}

								if (reqConfPreCompItem->hasRemainingProcessingConceptInstanceItems()) {										
									QList<COptimizedKPSetConceptInstancesItem*>* processingInstancesItemList = reqConfPreCompItem->getProcessingPossibleConceptInstancesItemList();
									while (!workTestCreated && !processingInstancesItemList->isEmpty()) {
										COptimizedKPSetConceptInstancesItem* nextProcessingItem = processingInstancesItemList->first();

										if (nextProcessingItem->hasPossibleInstances()) {
											COptimizedKPSetIndividualItem* nextInstantiatedTestItem = nextProcessingItem->takeNextTestingPossibleInstance();

											if (!nextInstantiatedTestItem->isItemSameIndividualMerged()) {
												workTestCreated = createNextConceptInstantiationTest(reqConfPreCompItem,nextProcessingItem,nextInstantiatedTestItem);
											} else {
												incTestedPossibleConceptInstancesCount(reqConfPreCompItem);
											}
										}
										if (!nextProcessingItem->hasPossibleInstances()) {
											processingInstancesItemList->removeFirst();
											nextProcessingItem->setPossibleInstancesProcessingQueuedFlag(false);
										}
									}

								} 
								
								if (!workTestCreated && !reqConfPreCompItem->hasRemainingProcessingConceptInstanceItems()) {
									if (!reqConfPreCompItem->hasTestingPossibleConceptInstances()) {

										if (!reqConfPreCompItem->isConceptRealizationInstalled()) {
											reqConfPreCompItem->setConceptRealizationInstalled(true);
											CConcreteOntology* ontology = reqConfPreCompItem->getOntology();

											CRealizingTestingStep* ontProcStep = reqConfPreCompItem->getRealizeConceptProcessingStep();
											addRealizationStatistics(reqConfPreCompItem,ontProcStep);

											CRealization* realization = ontology->getRealization();
											realization->setConceptRealization(reqConfPreCompItem);
											LOG(INFO,getDomain(),logTr("Concept realization finished for ontology."),this);
										}

										reqConfPreCompItem->getRealizeConceptProcessingStep()->setStepFinished(true);
										reqConfPreCompItem->getRealizeConceptProcessingStep()->submitRequirementsUpdate();
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
									reqConfPreCompItem->initRoleCandidateInitializingIndividualItems();
									reqConfPreCompItem->setRealizationRolesInitialized(true);
									if (!reqConfPreCompItem->isCountingPossibleRoleInstancesTesting()) {
										mOpenPossibleInstancesCount += reqConfPreCompItem->getOpenPossibleRoleInstancesCount();
										mTestedPossibleInstancesCount += reqConfPreCompItem->getTestedPossibleRoleInstancesCount();
										reqConfPreCompItem->setCountingPossibleRoleInstancesTesting(true);
									}
									LOG(INFO,getDomain(),logTr("Realizing role instances for ontology."),this);
								}



								if (reqConfPreCompItem->hasRemainingInitializingRoleInstanceItems()) {										
									QList<COptimizedKPSetRoleInstancesItem*>* initializingInstancesItemList = reqConfPreCompItem->getInitializingRoleInstancesItemList();
									while (!workTestCreated && !initializingInstancesItemList->isEmpty()) {
										COptimizedKPSetRoleInstancesItem* nextInitializingItem = initializingInstancesItemList->first();

										if (!nextInitializingItem->isCandidateSuccessorInitializationCompleted()) {


											if (!nextInitializingItem->isCandidateSuccessorInitializationStarted()) {
												nextInitializingItem->addRemainingCandidateInitializationSuccessors(reqConfPreCompItem->getRoleCandidateInitalizingIndividualsItemList());
												nextInitializingItem->setUninitializedSuccessorItemCount(reqConfPreCompItem->getRoleCandidateInitalizingIndividualsItemList()->count());
												nextInitializingItem->setCandidateSuccessorInitializationStarted(true);
											}

											COptimizedKPSetIndividualItem* indiItem = nextInitializingItem->takeNextRemainingCandidateInitializationSuccessor();

											if (!indiItem->isItemSameIndividualMerged()) {
												workTestCreated = createNextRoleInitializingTest(reqConfPreCompItem,nextInitializingItem,indiItem);
											}
										}
										if (nextInitializingItem->isCandidateSuccessorInitializationCompleted()) {
											initializingInstancesItemList->removeFirst();
										}
									}
								} 



								if (reqConfPreCompItem->hasRemainingInitializingRoleInstanceIndividualItems()) {										
									QList<COptimizedKPSetIndividualItem*>* initializingIndividualItemList = reqConfPreCompItem->getInitializingRoleInstancesIndividualItemList();
									while (!workTestCreated && !initializingIndividualItemList->isEmpty()) {
										COptimizedKPSetIndividualItem* nextInitializingItem = initializingIndividualItemList->first();

										if (!nextInitializingItem->hasAllRoleCandidatesInitializedFlag()) {


											if (!nextInitializingItem->isItemSameIndividualMerged()) {
												workTestCreated = createNextRoleInitializingTest(reqConfPreCompItem,nextInitializingItem);
											}
										}
										if (nextInitializingItem->hasAllRoleCandidatesInitializedFlag()) {
											initializingIndividualItemList->removeFirst();
										}
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

								if (!workTestCreated && !reqConfPreCompItem->hasRemainingInitializingRoleInstanceItems() && !reqConfPreCompItem->hasRemainingProcessingRoleInstanceItems() && !reqConfPreCompItem->hasRemainingInitializingRoleInstanceIndividualItems()) {
									if (!reqConfPreCompItem->hasTestingPossibleRoleInstances() && !reqConfPreCompItem->hasTestingRoleInstanceCandidates()) {

										if (!reqConfPreCompItem->isRoleRealizationInstalled()) {
											reqConfPreCompItem->setRoleRealizationInstalled(true);
											CConcreteOntology* ontology = reqConfPreCompItem->getOntology();

											CRealization* realization = ontology->getRealization();
											realization->setRoleRealization(reqConfPreCompItem);
											LOG(INFO,getDomain(),logTr("Role realization finished for ontology."),this);
										}

										reqConfPreCompItem->getRealizeRoleProcessingStep()->setStepFinished(true);
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


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::addRealizationStatistics(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, CRealizingTestingStep* ontProcStep) {
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


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::addIndividualDependencyTrackingStatistics(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, CRealizingTestingStep* ontProcStep, const QString& realizationTypeString) {
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


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::createTemporarySameRealizationOntology(COptimizedKPSetOntologyConceptRealizingItem* item) {
				CConcreteOntology* ontology = item->getOntology();
				CConcreteOntology* tmpSameRealOntology = item->getTemporarySameRealizationOntology();
				if (!tmpSameRealOntology) {


					QList<COptimizedKPSetIndividualItem*> indiItemNominalCreationList;
					QList<COptimizedKPSetIndividualItem*>* possibleSameIndiItemList = item->getPossibleSameIndividualsItemList();
					if (possibleSameIndiItemList && !possibleSameIndiItemList->isEmpty()) {
						for (QList<COptimizedKPSetIndividualItem*>::const_iterator it = possibleSameIndiItemList->constBegin(), itEnd = possibleSameIndiItemList->constEnd(); it != itEnd; ++it) {
							COptimizedKPSetIndividualItem* indiItem(*it);
							CIndividual* indi = indiItem->getIndividual();
							if (indi->getIndividualNominalConcept() == nullptr) {
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





			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::createTemporaryRoleRealizationOntology(COptimizedKPSetOntologyConceptRealizingItem* item) {
				CConcreteOntology* ontology = item->getOntology();
				CConcreteOntology* tmpRoleRealOntology = item->getTemporaryRoleRealizationOntology();
				if (!tmpRoleRealOntology) {
					tmpRoleRealOntology = new CConcreteOntology(ontology,ontology->getConfiguration());		

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
								invPropConcept->setRole(role);
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

					tmpRoleRealOntology->setConsistence(ontology->getConsistence());
					item->setTemporaryRoleRealizationOntology(tmpRoleRealOntology);
				}
				return this;
			}



			CConcept* COptimizedKPSetOntologyConceptRealizingThread::createTemporaryConcept(COptimizedKPSetOntologyConceptRealizingItem* item, CConcreteOntology* tmpRoleRealOntology) {
				CConceptVector* conVec = tmpRoleRealOntology->getTBox()->getConceptVector();
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(tmpRoleRealOntology->getOntologyContext()->getMemoryAllocationManager());
				concept->initConcept();
				concept->setConceptTag(conVec->getItemCount());
				conVec->setData(concept->getConceptTag(),concept);
				return concept;
			}



			void COptimizedKPSetOntologyConceptRealizingThread::addTemporaryConceptOperand(CConcept* concept, CConcept* opConcept, bool negated, COptimizedKPSetOntologyConceptRealizingItem* item, CConcreteOntology* tmpRoleRealOntology) {
				CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(tmpRoleRealOntology->getOntologyContext()->getMemoryAllocationManager());
				opConLinker->init(opConcept,negated);
				concept->addOperandLinker(opConLinker);
				concept->incOperandCount();
			}



			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::updateCalculatedKnownConceptInstance(COptimizedKPSetIndividualItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = instantiatedItem->getKnownPossibleInstancesHash();
				COptimizedKPSetConceptInstancesData* instanceItemData = knownPossibleInstancesHash->getInstanceItemData(instancesItem);
				QList<COptimizedKPSetConceptInstancesItem*> updateItemList;
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


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::updateCalculatedNonConceptInstance(COptimizedKPSetIndividualItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetConceptInstancesHash* knownPossibleInstancesHash = instantiatedItem->getKnownPossibleInstancesHash();
				COptimizedKPSetConceptInstancesData* instanceItemData = knownPossibleInstancesHash->getInstanceItemData(instancesItem);
				QList<COptimizedKPSetConceptInstancesItem*> updateItemList;
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








			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::updateCalculatedKnownRoleInstance(COptimizedKPSetIndividualItemPair individualItemPair, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetIndividualItem* indiItem1 = individualItemPair.first;
				COptimizedKPSetIndividualItem* indiItem2 = individualItemPair.second;
				COptimizedKPSetRoleNeighbourInstancesHash* knownPossibleRoleNeighbourInstancesHash = indiItem1->getKnownPossibleRoleNeighboursInstancesHash();
				COptimizedKPSetRoleInstancesData* instanceItemData = knownPossibleRoleNeighbourInstancesHash->getRoleSuccessorInstanceItemData(instancesItem,indiItem2);
				QList<TRoleItemInversionPair> updateItemList;
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
							updateInstanceItemData = knownPossibleRoleNeighbourInstancesHash->getRoleSuccessorInstanceItemData(updateInstanceItem,indiItem2);
						} else {
							updateInstanceItemData = knownPossibleRoleNeighbourInstancesHash->getRolePredecessorInstanceItemData(updateInstanceItem,indiItem2);
						}
						if (!updateInstanceItemData->mKnownInstance) {
							updateInstanceItemData->mKnownInstance = true;
							updateInstanceItemData->mPossibleInstance = false;
							updateInstanceItemData->mMostSpecific = false;
							updateInstanceItemData->mDerived = true;
							updateItemList.append(*updateInstanceItem->getParentItemList());
						} else if (updateInstanceItemData->mMostSpecific) {
							updateInstanceItem->removeKnownInstance(COptimizedKPSetIndividualItemPair(indiItem1,indiItem2));
							updateInstanceItemData->mMostSpecific = false;
						}
					}
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::updateCalculatedNonRoleInstance(COptimizedKPSetIndividualItemPair individualItemPair, COptimizedKPSetRoleInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetIndividualItem* indiItem1 = individualItemPair.first;
				COptimizedKPSetIndividualItem* indiItem2 = individualItemPair.second;
				COptimizedKPSetRoleNeighbourInstancesHash* knownPossibleRoleNeighbourInstancesHash = indiItem1->getKnownPossibleRoleNeighboursInstancesHash();
				COptimizedKPSetRoleInstancesData* instanceItemData = knownPossibleRoleNeighbourInstancesHash->getRoleSuccessorInstanceItemData(instancesItem,indiItem2);
				QList<TRoleItemInversionPair> updateItemList;
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
							updateInstanceItemData = knownPossibleRoleNeighbourInstancesHash->getRoleSuccessorInstanceItemData(updateInstanceItem,indiItem2);
						} else {
							updateInstanceItemData = knownPossibleRoleNeighbourInstancesHash->getRolePredecessorInstanceItemData(updateInstanceItem,indiItem2);
						}
						if (!updateInstanceItemData->mKnownInstance) {
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







			bool COptimizedKPSetOntologyConceptRealizingThread::realizingTested(COntologyRealizingItem* ontPreCompItem, CRealizingTestingItem* preTestItem, CRealizingCalculatedCallbackEvent* pcce) {
				COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem = (COptimizedKPSetOntologyConceptRealizingItem*)ontPreCompItem;
				CIndividualRoleCandidateTestingItem* indiRoleCandTestItem = dynamic_cast<CIndividualRoleCandidateTestingItem*>(preTestItem);
				if (indiRoleCandTestItem) {
					reqConfPreCompItem->decTestingRoleInstanceCandidatesCount();
					COptimizedKPSetIndividualItem* individualItem = indiRoleCandTestItem->getInstantiatedItem();
					QList<CConcept*>* markerConceptList = indiRoleCandTestItem->getMarkerConceptList();
					QHash<CConcept*,COptimizedKPSetRoleInstancesItem*>* markerConRolInsItemHash = reqConfPreCompItem->getMarkerConceptInstancesItemHash();


					for (QList<CConcept*>::const_iterator markerIt = markerConceptList->constBegin(), markerItEnd = markerConceptList->constEnd(); markerIt != markerItEnd; ++markerIt) {
						CConcept* markerConcept(*markerIt);

						COptimizedKPSetRoleInstancesItem* roleInstItem = markerConRolInsItemHash->value(markerConcept);
						bool inversed = false;
						if (roleInstItem->getTemporaryInverseMarkerConcept() == markerConcept) {
							inversed = true;
						}

						COptimizedKPSetRoleNeighbourInstancesHashData& roleNeighbourHashData = individualItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](roleInstItem);
						if (inversed) {
							roleNeighbourHashData.mRolePredecessorInstancesInitialized = true;
						} else {
							roleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
						}
					}

					individualItem->decInitializingRoleCandidateCount();
					return true;
				}
				CIndividualConceptInstanceTestingItem* indiConInstTestItem = dynamic_cast<CIndividualConceptInstanceTestingItem*>(preTestItem);
				if (indiConInstTestItem) {
					bool isIndividualInstance = !pcce->getTestResultSatisfiable();

					COptimizedKPSetConceptInstancesItem* instancesItem = indiConInstTestItem->getInstancesItem();
					COptimizedKPSetIndividualItem* instantiatedItem = indiConInstTestItem->getInstantiatedItem();

					instancesItem->decTestingPossibleInstancesCount();
					instantiatedItem->decTestingPossibleInstantiatedCount();
					reqConfPreCompItem->decTestingPossibleConceptInstanceCount();

					incTestedPossibleConceptInstancesCount(reqConfPreCompItem);

					if (!instantiatedItem->isItemSameIndividualMerged()) {
						if (isIndividualInstance) {
							updateCalculatedKnownConceptInstance(instantiatedItem,instancesItem,reqConfPreCompItem);
						} else {
							updateCalculatedNonConceptInstance(instantiatedItem,instancesItem,reqConfPreCompItem);
						}
					}

					if (!instancesItem->hasTestingPossibleInstances() && !instancesItem->hasPossibleInstances()) {
						if (instancesItem->hasAllSuccessorProcessedFlag()) {
							instancesItem->setSelfSuccessorProcessedFlag(true);
							updateParentItemsSuccessorProcessed(instancesItem);
						}
					}

					delete indiConInstTestItem;
					return true;
				}
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

					if (!indiItem1->isItemSameIndividualMerged() && !indiItem2->isItemSameIndividualMerged()) {
						if (isIndividualPairInstance) {
							updateCalculatedKnownRoleInstance(indiItemPair,instancesItem,reqConfPreCompItem);
						} else {
							updateCalculatedNonRoleInstance(indiItemPair,instancesItem,reqConfPreCompItem);
						}
					}

					if (!instancesItem->hasTestingPossibleInstances() && !instancesItem->hasPossibleInstances()) {
						if (instancesItem->hasAllSuccessorProcessedFlag()) {
							instancesItem->setSelfSuccessorProcessedFlag(true);
							updateParentItemsSuccessorProcessed(instancesItem);
						}
					}

					delete indiPairRoleInstTestItem;
					return true;
				}				
				CIndividualSameTestingItem* indiSameTestItem = dynamic_cast<CIndividualSameTestingItem*>(preTestItem);
				if (indiSameTestItem) {
					bool isIndividualSame = !pcce->getTestResultSatisfiable();

					COptimizedKPSetIndividualItem* instantiatedItem1 = indiSameTestItem->getInstantiatedItem1();
					COptimizedKPSetIndividualItem* instantiatedItem2 = indiSameTestItem->getInstantiatedItem2();

					incTestedPossibleSameIndividualsCount(reqConfPreCompItem);

					instantiatedItem1->decTestingPossibleSameIndividualCount();
					instantiatedItem2->decTestingPossibleSameIndividualCount();
					reqConfPreCompItem->decTestingPossibleSameIndividualCount();

					if (isIndividualSame) {
						if (!instantiatedItem1->isItemSameIndividualMerged() && !instantiatedItem2->isItemSameIndividualMerged()) {
							mergeSameIndividualItems(instantiatedItem1,instantiatedItem2,reqConfPreCompItem);
						}
					}

					delete indiSameTestItem;
					return true;
				}				
				CIndividualsConsistencyTestingItem* indiConsTest = dynamic_cast<CIndividualsConsistencyTestingItem*>(preTestItem);
				if (indiConsTest) {
					reqConfPreCompItem->setExtraConsistencyTested(true);
					reqConfPreCompItem->setExtraConsistencyTesting(false);
				}
				return false;
			}


			bool COptimizedKPSetOntologyConceptRealizingThread::processRealizationMessage(COntologyRealizingItem* ontRealItem, CRealizationMessageData* messageData, CMemoryPool* memoryPools) {
				COptimizedKPSetOntologyConceptRealizingItem* realItem = (COptimizedKPSetOntologyConceptRealizingItem*)ontRealItem;
				for (CRealizationMessageData* messageDataIt = messageData; messageDataIt; messageDataIt = messageDataIt->getNext()) {
					if (messageDataIt->getRealizationMessageDataType() == CRealizationMessageData::TELLREALIZATIONMARKERCANDIDATES) {
						CRealizationMarkerCandidatesMessageData* rmcmd = (CRealizationMarkerCandidatesMessageData*)messageDataIt;
						CIndividual* individual = rmcmd->getIndividual();
						CConcept* markerConcept = rmcmd->getMarkerConcept();

						QHash<CConcept*,COptimizedKPSetRoleInstancesItem*>* markerConRolInsItemHash = realItem->getMarkerConceptInstancesItemHash();
						COptimizedKPSetRoleInstancesItem* roleInstItem = markerConRolInsItemHash->value(markerConcept);
						bool inversed = false;
						if (roleInstItem->getTemporaryInverseMarkerConcept() == markerConcept) {
							inversed = true;
						}

						COptimizedKPSetIndividualItem* individualItem = realItem->getIndividualInstantiatedItem(individual,false);
						CXLinker<CIndividual*>* detIndiLinker = rmcmd->getDeterministicIndividualLinker();
						for (CXLinker<CIndividual*>* detIndiLinkerIt = detIndiLinker; detIndiLinkerIt; detIndiLinkerIt = detIndiLinkerIt->getNext()) {
							CIndividual* candIndi = detIndiLinkerIt->getData();
							COptimizedKPSetIndividualItem* candIndividualItem = realItem->getIndividualInstantiatedItem(candIndi,false);
							addKPSetDirectSuperInstances(realItem,roleInstItem,inversed,individualItem,candIndividualItem,true);
						}
						CXLinker<CIndividual*>* ndetIndiLinker = rmcmd->getNondeterministicIndividualLinker();
						for (CXLinker<CIndividual*>* ndetIndiLinkerIt = ndetIndiLinker; ndetIndiLinkerIt; ndetIndiLinkerIt = ndetIndiLinkerIt->getNext()) {
							CIndividual* candIndi = ndetIndiLinkerIt->getData();
							COptimizedKPSetIndividualItem* candIndividualItem = realItem->getIndividualInstantiatedItem(candIndi,false);
							addKPSetDirectSuperInstances(realItem,roleInstItem,inversed,individualItem,candIndividualItem,false);
						}

						COptimizedKPSetRoleNeighbourInstancesHashData& roleNeighbourHashData = individualItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](roleInstItem);
						if (inversed) {
							roleNeighbourHashData.mRolePredecessorInstancesInitialized = true;
						} else {
							roleNeighbourHashData.mRoleSuccessorInstancesInitialized = true;
						}
						realItem->incInitializedRoleInstancesCount();


					}
				}
				return true;
			}




			bool COptimizedKPSetOntologyConceptRealizingThread::addKPSetDirectSuperInstances(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetRoleInstancesItem* roleInstItem, bool inversed, COptimizedKPSetIndividualItem* indiItem1, COptimizedKPSetIndividualItem* indiItem2, bool knownInstance) {
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

				//if (indi2String == "file:/Users/liebig/Ontologien/reasoner-eval/30.owl#Act5") {
				//	if ((roleString == "file:/Users/liebig/Ontologien/reasoner-eval/30.owl#inv-first")) {
				//		bool bug = true;
				//	}
				//}
				//if (indi2String == "http://www.co-ode.org/roberts/family-tree.owl#john_archer_1804" && indi1String == "http://www.co-ode.org/roberts/family-tree.owl#john_archer_1804") {
				//	if ((roleString == "http://www.co-ode.org/roberts/family-tree.owl#isFatherOf" || inverseRoleString == "http://www.co-ode.org/roberts/family-tree.owl#isFatherOf")) {
				//		bool bug = true;
				//	}
				//}

				COptimizedKPSetRoleNeighbourInstancesHashData& succRoleNeighbourData = succIndiItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](roleInstItem);
				succNeighbourHash = succRoleNeighbourData.getRoleSuccessorInstancesHash(true);

				COptimizedKPSetRoleInstancesHashData& succHashData = (*succNeighbourHash)[precIndiItem];

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
					COptimizedKPSetRoleInstancesHashData& precHashData = (*precNeighbourHash)[succIndiItem];

					COptimizedKPSetRoleInstancesData* data = new COptimizedKPSetRoleInstancesData();
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
						COptimizedKPSetRoleInstancesHashData& superSuccHashData = (*superSuccNeighbourHash)[superPrecIndiItem];

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
							COptimizedKPSetRoleInstancesHashData& superPrecHashData = (*superPrecNeighbourHash)[superSuccIndiItem];

							COptimizedKPSetRoleInstancesData* superData = new COptimizedKPSetRoleInstancesData();
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






			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::mergeSameIndividualItems(COptimizedKPSetIndividualItem* instantiatedItem1, COptimizedKPSetIndividualItem* instantiatedItem2, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetIndividualItem* mergedIntoInstantiatedItem = instantiatedItem1;
				COptimizedKPSetIndividualItem* mergingInstantiatedItem = instantiatedItem2;
				if (instantiatedItem2->getPossibleInstantiatedCount() + instantiatedItem2->getPossibleSameInstantiatedItemSet() < instantiatedItem1->getPossibleInstantiatedCount() + instantiatedItem1->getPossibleSameInstantiatedItemSet()) {
					mergedIntoInstantiatedItem = instantiatedItem2;
					mergingInstantiatedItem = instantiatedItem1;
				}
				mergingInstantiatedItem->setItemSameIndividualMerged(true);
				QHash<CIndividual*,COptimizedKPSetIndividualItem*>* indiInstantiatedItemHash = reqConfPreCompItem->getIndividualInstantiatedItemHash();
				QSet<CIndividual*>* individualSet = mergingInstantiatedItem->getKnownSameIndividualSet();
				for (QSet<CIndividual*>::const_iterator it = individualSet->constBegin(), itEnd = individualSet->constEnd(); it != itEnd; ++it) {
					CIndividual* sameIndividual(*it);
					mergedIntoInstantiatedItem->addKnownSameIndividual(sameIndividual);
					indiInstantiatedItemHash->insert(sameIndividual,mergedIntoInstantiatedItem);
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::updateParentItemsSuccessorProcessed(COptimizedKPSetConceptInstancesItem* item) {
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

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::updateParentItemsSuccessorProcessed(COptimizedKPSetRoleInstancesItem* item) {
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

			bool COptimizedKPSetOntologyConceptRealizingThread::finishOntologyRealizing(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
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

				reqConfPreCompItem->doRealizingFinishedCallback(true);
				return true;
			}


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::incTestedPossibleConceptInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleConceptInstancesTesting()) {
					mTestedPossibleInstancesCount += incCount;
				}
				item->incTestedPossibleConceptInstancesCount(incCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::incOpenPossibleConceptInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleConceptInstancesTesting()) {
					mOpenPossibleInstancesCount += incCount;
				}
				item->incOpenPossibleConceptInstancesCount(incCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::decTestedPossibleConceptInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleConceptInstancesTesting()) {
					mTestedPossibleInstancesCount -= decCount;
				}
				item->decTestedPossibleConceptInstancesCount(decCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::decOpenPossibleConceptInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleConceptInstancesTesting()) {
					mOpenPossibleInstancesCount -= decCount;
				}
				item->decOpenPossibleConceptInstancesCount(decCount);
				return this;
			}







			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::incTestedPossibleRoleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleRoleInstancesTesting()) {
					mTestedPossibleInstancesCount += incCount;
				}
				item->incTestedPossibleRoleInstancesCount(incCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::incOpenPossibleRoleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleRoleInstancesTesting()) {
					mOpenPossibleInstancesCount += incCount;
				}
				item->incOpenPossibleRoleInstancesCount(incCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::decTestedPossibleRoleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleRoleInstancesTesting()) {
					mTestedPossibleInstancesCount -= decCount;
				}
				item->decTestedPossibleRoleInstancesCount(decCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::decOpenPossibleRoleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleRoleInstancesTesting()) {
					mOpenPossibleInstancesCount -= decCount;
				}
				item->decOpenPossibleRoleInstancesCount(decCount);
				return this;
			}








			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::incTestedPossibleSameIndividualsCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleSameInstancesTesting()) {
					mTestedPossibleInstancesCount += incCount;
				}
				item->incTestedPossibleSameIndividualsCount(incCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::incOpenPossibleSameIndividualsCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount) {
				if (item->isCountingPossibleSameInstancesTesting()) {
					mOpenPossibleInstancesCount += incCount;
				}
				item->incOpenPossibleSameIndividualsCount(incCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::decTestedPossibleSameIndividualsCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleSameInstancesTesting()) {
					mTestedPossibleInstancesCount -= decCount;
				}
				item->decTestedPossibleSameIndividualsCount(decCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::decOpenPossibleSameIndividualsCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount) {
				if (item->isCountingPossibleSameInstancesTesting()) {
					mOpenPossibleInstancesCount -= decCount;
				}
				item->decOpenPossibleSameIndividualsCount(decCount);
				return this;
			}






			CRealizationProgress* COptimizedKPSetOntologyConceptRealizingThread::getRealizationProgress() {
				if (mOpenPossibleInstancesCount != mTestedPossibleInstancesCount) {					
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
					mRealProgress.setTotalClasses(mOpenPossibleInstancesCount);
				}
				return &mRealProgress;
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
