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

#include "COptimizedKPSetOntologyConceptRealizingThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetOntologyConceptRealizingThread::COptimizedKPSetOntologyConceptRealizingThread(CReasonerManager *reasonerManager) : CRealizerThread(reasonerManager),CLogDomain("::Konclude::Reasoner::Kernel::Realizeror") {
				mTestedPossibleInstancesCount = 0;
				mOpenPossibleInstancesCount = 0;
				mRealizingCount = 0;
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
					reqConfPreCompItem->initItemsFromHierarchy();

					readCalculationConfig(reqConfPreCompItem->getCalculationConfiguration());

					initializeKPSetsFromCompletionGraph(reqConfPreCompItem);
					reqConfPreCompItem->setItemsInitialized(true);

					QList<COptimizedKPSetConceptInstancesItem*>* itemInstancesList = reqConfPreCompItem->getProcessingPossibleInstancesItemList();
					for (QList<COptimizedKPSetConceptInstancesItem*>::const_iterator it = itemInstancesList->constBegin(), itEnd = itemInstancesList->constEnd(); it != itEnd; ++it) {
						COptimizedKPSetConceptInstancesItem* item(*it);
						if (item->hasAllSuccessorProcessedFlag() && !item->hasPossibleInstances() && !item->hasTestingPossibleInstances()) {
							updateParentItemsSuccessorProcessed(item);
						}
					}


				}
				return true;
			}


			CIndividualProcessNode* COptimizedKPSetOntologyConceptRealizingThread::getMergeCorrectedIndividualProcessNode(CIndividualProcessNode* indiProcNode, CIndividualProcessNodeVector* indiProcVector, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				while (indiProcNode && indiProcNode->getMergedIntoIndividualNodeID() != indiProcNode->getIndividualID()) {
					cint64 mergeID = indiProcNode->getMergedIntoIndividualNodeID();
					indiProcNode = indiProcVector->getData(mergeID);
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
				COptimizedKPSetConceptInstantiatedItem* instantiatedItem = reqConfPreCompItem->getIndividualInstantiatedItem(individual,true);
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
							incOpenPossibleInstancesCount(reqConfPreCompItem);
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


			bool COptimizedKPSetOntologyConceptRealizingThread::initializeKPSetsFromCompletionGraph(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
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
								CIndividualProcessNodeVector* indiProcVector = procDataBox->getIndividualProcessNodeVector();
								if (indiProcVector) {

									QList<COptimizedKPSetConceptInstancesItem*> equivClassList;
									initializeEquivalentClassList(&equivClassList,reqConfPreCompItem);

									QHash<CConcept*,COptimizedKPSetConceptInstancesItem*>* conInstItemHash = reqConfPreCompItem->getConceptInstancesItemHash();

									QHash<CIndividual*,QList<CIndividual*> > individualPossibleSameIndividualListHash;


									cint64 indiCount = reqConfPreCompItem->getOntology()->getABox()->getIndividualCount();
									cint64 indiProcCount = indiProcVector->getItemCount();
									for (cint64 i = 0; i < indiCount; ++i) {
										CIndividualProcessNode* baseIndiProcNode = indiProcVector->getData(i);
										CIndividualProcessNode* indiProcNode = baseIndiProcNode;
										if (indiProcNode) {
											CIndividual* individual = indiProcNode->getNominalIndividual();

											if (individual && !individual->isAnonymousIndividual() && !reqConfPreCompItem->hasIndividualInstantiatedItem(individual)) {
												indiProcNode = getMergeCorrectedIndividualProcessNode(indiProcNode,indiProcVector,reqConfPreCompItem);

												QList<COptimizedKPSetConceptInstancesItem*> knownInstancesList;
												QList<COptimizedKPSetConceptInstancesItem*> possibleInstancesList;

												QList<CIndividual*> knownSameIndividualList;
												QList<CIndividual*> possibleSameIndividualList;

												bool nonDeterministicMerged = false;

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
															if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_NOMINAL)) {
																CIndividual* nominalIndividual = concept->getNominalIndividual();
																if (nominalIndividual == individual) {
																	if (!deterministicTrackPoint) {
																		nonDeterministicMerged = true;
																	}
																} else {
																	if (!deterministicTrackPoint) {
																		possibleSameIndividualList.append(nominalIndividual);
																	} else {
																		knownSameIndividualList.append(nominalIndividual);
																	}
																}
															}
															if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_ATOM | CConceptOperator::CCF_SUB | CConceptOperator::CCF_TOP | CConceptOperator::CCF_EQ | CConceptOperator::CCF_EQCAND)) {
																cint64 opCode = conOperator->getOperatorCode();
																if (opCode == CCEQCAND) {
																	resolvedConcept = concept->getOperandList()->getData();
																	resolved = true;
																}
																COptimizedKPSetConceptInstancesItem* instantiatedItem = conInstItemHash->value(resolvedConcept);
																if (deterministicTrackPoint && !resolved) {
																	knownInstancesList.append(instantiatedItem);
																} else {
																	possibleInstancesList.append(instantiatedItem);
																}
															}
														}

														conDesLinker = conDesLinker->getNextConceptDesciptor();
													}
												}

												if (nonDeterministicMerged) {
													possibleInstancesList.append(knownInstancesList);
													knownInstancesList.clear();
													possibleSameIndividualList.append(knownSameIndividualList);
													knownSameIndividualList.clear();

													QSet<CConcept*> knownConceptSet;
													QSet<CIndividual*> knownSameIndividualSet;

													CIndividualProcessNode* detIndiProcNode = baseIndiProcNode;
													bool stillDeterministicllyMerged = true;
													while (stillDeterministicllyMerged && detIndiProcNode) {

														CReapplyConceptLabelSet* detConLabelSet = detIndiProcNode->getReapplyConceptLabelSet(false);
														if (detConLabelSet) {

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
																	if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_NOMINAL)) {
																		CIndividual* nominalIndividual = concept->getNominalIndividual();
																		if (nominalIndividual == individual) {
																			if (!deterministicTrackPoint) {
																				stillDeterministicllyMerged = false;
																			}
																		}
																	}
																}
															}

															if (stillDeterministicllyMerged) {
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
																		if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_NOMINAL)) {
																			CIndividual* nominalIndividual = concept->getNominalIndividual();
																			if (nominalIndividual != individual) {
																				if (deterministicTrackPoint) {
																					knownSameIndividualSet.insert(nominalIndividual);
																				}
																			}
																		}
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
														}
													}

													for (QSet<CConcept*>::const_iterator it = knownConceptSet.constBegin(), itEnd = knownConceptSet.constEnd(); it != itEnd; ++it) {
														CConcept* concept(*it);
														COptimizedKPSetConceptInstancesItem* instantiatedItem = conInstItemHash->value(concept);
														knownInstancesList.append(instantiatedItem);
													}
													for (QSet<CIndividual*>::const_iterator it = knownSameIndividualSet.constBegin(), itEnd = knownSameIndividualSet.constEnd(); it != itEnd; ++it) {
														CIndividual* sameIndividual(*it);
														knownSameIndividualList.append(sameIndividual);
													}

													QList<CIndividual*> tmpPossibleSameIndividualList(possibleSameIndividualList);
													possibleSameIndividualList.clear();
													while (!tmpPossibleSameIndividualList.isEmpty()) {
														CIndividual* possSameIndividual = tmpPossibleSameIndividualList.takeFirst();
														if (!knownSameIndividualSet.contains(possSameIndividual)) {
															possibleSameIndividualList.append(possSameIndividual);
														}
													}
												}

												if (!equivClassList.isEmpty()) {
													possibleInstancesList.append(equivClassList);
												}

												initializeKPSetsForIndividual(reqConfPreCompItem,individual,knownInstancesList,possibleInstancesList,knownSameIndividualList);

												if (!possibleSameIndividualList.isEmpty()) {
													individualPossibleSameIndividualListHash.insert(individual,possibleSameIndividualList);
												}
											}
										}
									}

									if (!individualPossibleSameIndividualListHash.isEmpty()) {
										QSet<COptimizedKPSetConceptInstantiatedItem*> singleIndividualSet;
										for (QHash<CIndividual*,QList<CIndividual*> >::const_iterator it = individualPossibleSameIndividualListHash.constBegin(), itEnd = individualPossibleSameIndividualListHash.constEnd(); it != itEnd; ++it) {
											CIndividual* individual = it.key();
											COptimizedKPSetConceptInstantiatedItem* individualItem = reqConfPreCompItem->getIndividualInstantiatedItem(individual,false);
											if (!singleIndividualSet.contains(individualItem)) {
												singleIndividualSet.insert(individualItem);
												const QList<CIndividual*> possibleSameIndividualList = it.value();
												initializeSamePossibleIndividuals(reqConfPreCompItem,individualItem,possibleSameIndividualList);
											}
										}
									}
								}
							}
						}
					}
				}
				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingThread::initializeSamePossibleIndividuals(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstantiatedItem* individualItem, const QList<CIndividual*>& possibleSameIndividualList) {
				for (QList<CIndividual*>::const_iterator it = possibleSameIndividualList.constBegin(), itEnd = possibleSameIndividualList.constEnd(); it != itEnd; ++it) {
					CIndividual* possSameIndividual(*it);
					COptimizedKPSetConceptInstantiatedItem* possSameIndividualItem = reqConfPreCompItem->getIndividualInstantiatedItem(possSameIndividual,false);
					individualItem->addPossibleSameIndividualItem(possSameIndividualItem);
					possSameIndividualItem->addPossibleSameIndividualItem(individualItem);
				}
				return true;
			}

			bool COptimizedKPSetOntologyConceptRealizingThread::createNextInstantiationTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetConceptInstantiatedItem* instantiatedItem) {
				instancesItem->incTestingPossibleInstancesCount();
				instantiatedItem->incTestingPossibleInstantiatedCount();
				instantiatedItem->decPossibleInstantiatedCount();
				instantiatedItem->setTestingPossibleInstantiated(instancesItem);

				CConcept* concept = instancesItem->getHierarchyNode()->getOneEquivalentConcept();
				CIndividual* individual = instantiatedItem->getIndividual();

				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(concept,true,individual);

				CIndividualInstanceTestingItem* testItem = new CIndividualInstanceTestingItem(reqConfPreCompItem,instancesItem,instantiatedItem);
				reqConfPreCompItem->incTestingPossibleInstanceCount();

				processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
				return true;
			}


			bool COptimizedKPSetOntologyConceptRealizingThread::createNextSameIndividualsTest(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem, COptimizedKPSetConceptInstantiatedItem* instantiatedItem1, COptimizedKPSetConceptInstantiatedItem* instantiatedItem2) {
				instantiatedItem1->incTestingPossibleSameIndividualCount();
				instantiatedItem2->incTestingPossibleSameIndividualCount();


				CIndividual* individual1 = instantiatedItem1->getIndividual();
				CIndividual* individual2 = instantiatedItem2->getIndividual();
				CConcept* nominalConcept2 = individual2->getIndividualNominalConcept();

				CSatisfiableCalculationJob* satCalcJob = nullptr;
				CSatisfiableCalculationJobGenerator satCalcJobGen(reqConfPreCompItem->getOntology());
				satCalcJob = satCalcJobGen.getSatisfiableCalculationJob(nominalConcept2,true,individual1);

				CIndividualSameTestingItem* testItem = new CIndividualSameTestingItem(reqConfPreCompItem,instantiatedItem1,instantiatedItem2);
				reqConfPreCompItem->incTestingPossibleSameIndividualCount();

				processCalculationJob(satCalcJob,reqConfPreCompItem,testItem);				
				return true;
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


					if (!workTestCreated && reqConfPreCompItem->isRealizeSameIndividualsStepRequired()) {
						if (!reqConfPreCompItem->isRealizeSameIndividualsStepFinished()) {
							if (reqConfPreCompItem->areRealizeSameIndividualsStepProcessingRequirementSatisfied()) {

								if (!reqConfPreCompItem->hasRealizationSameIndividualsInitialized()) {
									reqConfPreCompItem->setRealizationSameIndividualsInitialized(true);
									LOG(INFO,getDomain(),logTr("Realize same individuals for ontology."),this);
								}

								if (reqConfPreCompItem->hasRemainingProcessingSameIndividualsItems()) {										
									QList<COptimizedKPSetConceptInstantiatedItem*>* processingSameIndividualItemList = reqConfPreCompItem->getProcessingPossibleSameIndividualsItemList();
									while (!workTestCreated && !processingSameIndividualItemList->isEmpty()) {
										COptimizedKPSetConceptInstantiatedItem* nextProcessingItem = processingSameIndividualItemList->first();

										if (!nextProcessingItem->isItemSameIndividualMerged()) {

											COptimizedKPSetConceptInstantiatedItem* nextPossibleSameIndividualTestItem = nextProcessingItem->takeTestingPossibleSameIndividualItem();
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



					if (!workTestCreated && reqConfPreCompItem->isRealizeConceptStepRequired()) {
						if (!reqConfPreCompItem->isRealizeConceptStepFinished()) {
							if (reqConfPreCompItem->areRealizeConceptStepProcessingRequirementSatisfied()) {

								if (!reqConfPreCompItem->hasRealizationConceptsInitialized()) {
									reqConfPreCompItem->setRealizationConceptsInitialized(true);
									LOG(INFO,getDomain(),logTr("Realize individual types for ontology."),this);
								}

								if (reqConfPreCompItem->hasRemainingProcessingInstanceItems()) {										
									QList<COptimizedKPSetConceptInstancesItem*>* processingInstancesItemList = reqConfPreCompItem->getProcessingPossibleInstancesItemList();
									while (!workTestCreated && !processingInstancesItemList->isEmpty()) {
										COptimizedKPSetConceptInstancesItem* nextProcessingItem = processingInstancesItemList->first();

										if (nextProcessingItem->hasPossibleInstances()) {
											COptimizedKPSetConceptInstantiatedItem* nextInstantiatedTestItem = nextProcessingItem->takeNextTestingPossibleInstance();

											if (!nextInstantiatedTestItem->isItemSameIndividualMerged()) {
												workTestCreated = createNextInstantiationTest(reqConfPreCompItem,nextProcessingItem,nextInstantiatedTestItem);
											} else {
												incTestedPossibleInstancesCount(reqConfPreCompItem);
											}
										}
										if (!nextProcessingItem->hasPossibleInstances()) {
											processingInstancesItemList->removeFirst();
											nextProcessingItem->setPossibleInstancesProcessingQueuedFlag(false);
										}
									}

								} 
								
								if (!workTestCreated && !reqConfPreCompItem->hasRemainingProcessingInstanceItems()) {
									if (!reqConfPreCompItem->hasTestingPossibleInstances()) {
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


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::updateCalculatedKnownInstance(COptimizedKPSetConceptInstantiatedItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
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
						instancesItem->removeKnownInstance(instantiatedItem);
						updateInstanceItemData->mMostSpecific = false;
					}
				}
				return this;
			}


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::updateCalculatedNotInstance(COptimizedKPSetConceptInstantiatedItem* instantiatedItem, COptimizedKPSetConceptInstancesItem* instancesItem, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
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
								incOpenPossibleInstancesCount(reqConfPreCompItem);
								instantiatedItem->incPossibleInstantiatedCount();
								updateInstanceItem->addPossibleInstance(instantiatedItem);
								if (updateInstanceItem->hasToProcessPossibleInstancesFlag()) {
									reqConfPreCompItem->addInstanceItemToProcessingPossibleInstances(updateInstanceItem);
								}
							}
						}
					}
				}
				return this;
			}


			bool COptimizedKPSetOntologyConceptRealizingThread::realizingTested(COntologyRealizingItem* ontPreCompItem, CRealizingTestingItem* preTestItem, CRealizingCalculatedCallbackEvent* pcce) {
				COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem = (COptimizedKPSetOntologyConceptRealizingItem*)ontPreCompItem;
				CIndividualInstanceTestingItem* indiInstTestItem = dynamic_cast<CIndividualInstanceTestingItem*>(preTestItem);
				if (indiInstTestItem) {
					bool isIndividualInstance = !pcce->getTestResultSatisfiable();

					COptimizedKPSetConceptInstancesItem* instancesItem = indiInstTestItem->getInstancesItem();
					COptimizedKPSetConceptInstantiatedItem* instantiatedItem = indiInstTestItem->getInstantiatedItem();

					instancesItem->decTestingPossibleInstancesCount();
					instantiatedItem->decTestingPossibleInstantiatedCount();
					reqConfPreCompItem->decTestingPossibleInstanceCount();

					incTestedPossibleInstancesCount(reqConfPreCompItem);

					if (!instantiatedItem->isItemSameIndividualMerged()) {
						if (isIndividualInstance) {
							updateCalculatedKnownInstance(instantiatedItem,instancesItem,reqConfPreCompItem);
						} else {
							updateCalculatedNotInstance(instantiatedItem,instancesItem,reqConfPreCompItem);
						}
					}

					if (!instancesItem->hasTestingPossibleInstances() && !instancesItem->hasPossibleInstances()) {
						if (instancesItem->hasAllSuccessorProcessedFlag()) {
							instancesItem->setSelfSuccessorProcessedFlag(true);
							updateParentItemsSuccessorProcessed(instancesItem);
						}
					}

					delete indiInstTestItem;
					return true;
				}
				CIndividualSameTestingItem* indiSameTestItem = dynamic_cast<CIndividualSameTestingItem*>(preTestItem);
				if (indiSameTestItem) {
					bool isIndividualSame = !pcce->getTestResultSatisfiable();

					COptimizedKPSetConceptInstantiatedItem* instantiatedItem1 = indiSameTestItem->getInstantiatedItem1();
					COptimizedKPSetConceptInstantiatedItem* instantiatedItem2 = indiSameTestItem->getInstantiatedItem2();

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
				return false;
			}


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::mergeSameIndividualItems(COptimizedKPSetConceptInstantiatedItem* instantiatedItem1, COptimizedKPSetConceptInstantiatedItem* instantiatedItem2, COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				COptimizedKPSetConceptInstantiatedItem* mergedIntoInstantiatedItem = instantiatedItem1;
				COptimizedKPSetConceptInstantiatedItem* mergingInstantiatedItem = instantiatedItem2;
				if (instantiatedItem2->getPossibleInstantiatedCount() + instantiatedItem2->getPossibleSameInstantiatedItemSet() < instantiatedItem1->getPossibleInstantiatedCount() + instantiatedItem1->getPossibleSameInstantiatedItemSet()) {
					mergedIntoInstantiatedItem = instantiatedItem2;
					mergingInstantiatedItem = instantiatedItem1;
				}
				mergingInstantiatedItem->setItemSameIndividualMerged(true);
				QHash<CIndividual*,COptimizedKPSetConceptInstantiatedItem*>* indiInstantiatedItemHash = reqConfPreCompItem->getIndividualInstantiatedItemHash();
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


			bool COptimizedKPSetOntologyConceptRealizingThread::finishOntologyRealizing(COptimizedKPSetOntologyConceptRealizingItem* reqConfPreCompItem) {
				if (reqConfPreCompItem->areAllStepFinished()) {
					reqConfPreCompItem->setRealizingFinished(true);
					CConcreteOntology* ontology = reqConfPreCompItem->getOntology();

					CRealization* realization = ontology->getRealization();
					realization->setConceptRealization(reqConfPreCompItem);
					realization->setRealized(true);
				}

				reqConfPreCompItem->doRealizingFinishedCallback(true);
				return true;
			}


			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::incTestedPossibleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount) {
				mTestedPossibleInstancesCount += incCount;
				item->incTestedPossibleInstancesCount(incCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::incOpenPossibleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 incCount) {
				mOpenPossibleInstancesCount += incCount;
				item->incOpenPossibleInstancesCount(incCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::decTestedPossibleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount) {
				mTestedPossibleInstancesCount -= decCount;
				item->decTestedPossibleInstancesCount(decCount);
				return this;
			}

			COptimizedKPSetOntologyConceptRealizingThread* COptimizedKPSetOntologyConceptRealizingThread::decOpenPossibleInstancesCount(COptimizedKPSetOntologyConceptRealizingItem* item, cint64 decCount) {
				mOpenPossibleInstancesCount -= decCount;
				item->decOpenPossibleInstancesCount(decCount);
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
