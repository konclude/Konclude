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

#include "CSatisfiableCalculationTaskFromCalculationJobGenerator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			CSatisfiableCalculationTaskFromCalculationJobGenerator::CSatisfiableCalculationTaskFromCalculationJobGenerator(CGeneratorTaskHandleContextBase* genTaskHandleContext) {
				mGenTaskHandleContext = genTaskHandleContext;
			}


			CSatisfiableCalculationTask* CSatisfiableCalculationTaskFromCalculationJobGenerator::createSatisfiableCalculationTask(CCalculationJob* calculationJob, CCallbackData* additionalCalculatedCallback) {
				return createSatisfiableCalculationTask(calculationJob->getOntology(),calculationJob,additionalCalculatedCallback);
			}







			CSatisfiableCalculationTask* CSatisfiableCalculationTaskFromCalculationJobGenerator::createApproximatedSaturationCalculationTask(CConcreteOntology *ontology, CApproximatedSaturationCalculationJob* approxSaturCalcJob, CCallbackData* additionalCalculatedCallback) {

				CSatisfiableCalculationTask* baseTask = nullptr;
				CSaturationData* saturData = approxSaturCalcJob->getContinueSaturationData();
				if (saturData) {
					CSaturationTaskData* taskSaturData = dynamic_cast<CSaturationTaskData*>(saturData);
					if (taskSaturData) {
						baseTask = taskSaturData->getSaturationTask();
					}
				}
 
				CSatisfiableCalculationTask* satCalcTask = CObjectMemoryPoolAllocator<CSatisfiableCalculationTask>::allocateAndConstructWithMemroyPool(mGenTaskHandleContext->getTaskHandleMemoryAllocationManager());
				if (baseTask) {
					satCalcTask->initUndependedSatisfiableCalculationTask(baseTask,approxSaturCalcJob->getCalculationConfiguration(),approxSaturCalcJob->getCalclulationStatisticsCollector(),mGenTaskHandleContext);
				} else {
					satCalcTask->initSatisfiableCalculationTask(ontology,approxSaturCalcJob->getCalculationConfiguration(),approxSaturCalcJob->getCalclulationStatisticsCollector(),mGenTaskHandleContext);
				}
				CProcessContext* procContext = satCalcTask->getProcessContext(mGenTaskHandleContext);


				CProcessingDataBox* dataBox = satCalcTask->getProcessingDataBox();

				CIndividualSaturationProcessNodeVector* indiNodeVec = dataBox->getIndividualSaturationProcessNodeVector();

				CMemoryAllocationManager* memMan = procContext->getUsedMemoryAllocationManager();
				cint64 nextIndiID = qMax(ontology->getABox()->getIndividualCount()+1,indiNodeVec->getItemCount());

				bool separateSaturation = approxSaturCalcJob->isSeparateSaturation();

				CApproximatedSaturationCalculationConstructionConstruct* satCalcConsConstruct = approxSaturCalcJob->getSatisfiableCalculationConstructionConstructs();
				while (satCalcConsConstruct) {
					CSaturationConceptReferenceLinking* satConRefLink = satCalcConsConstruct->getSaturationConceptReferenceLinking();
					CSaturationIndividualReferenceLinking* satIndiRefLink = satCalcConsConstruct->getSaturationIndividualReferenceLinking();
					CIndividual* nominalIndi = satCalcConsConstruct->getIndividual();
					cint64 individualID = 0;
					if (nominalIndi) {
						individualID = nominalIndi->getIndividualID();
					} else {
						individualID = nextIndiID++;
					}

					CIndividualSaturationProcessNode* indi = CObjectParameterizingAllocator< CIndividualSaturationProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(memMan,procContext);
					indi->initIndividualSaturationProcessNode(individualID,satConRefLink,satIndiRefLink);
					if (nominalIndi) {
						indi->setNominalIndividual(nominalIndi);
					}
					indi->setSeparated(separateSaturation);

					if (satConRefLink) {
						satConRefLink->setIndividualProcessNodeForConcept(indi);
					}
					if (satIndiRefLink) {
						satIndiRefLink->setIndividualProcessNodeForIndividual(indi);
					}

					indiNodeVec->setLocalData(individualID,indi);

					bool processing = satCalcConsConstruct->getQueueProcessing();
					CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = CObjectAllocator< CIndividualSaturationProcessNodeLinker >::allocateAndConstruct(memMan);
					indiProcNodeLinker->initProcessNodeLinker(indi,processing);
					indi->setIndividualSaturationProcessNodeLinker(indiProcNodeLinker);
					if (processing) {
						dataBox->addIndividualSaturationProcessNodeLinker(indiProcNodeLinker);
					}

					satCalcConsConstruct = satCalcConsConstruct->getNextConstruct();
				}

				CApproximatedSaturationCalculationProcessingConstruct* satCalcProcConstruct = approxSaturCalcJob->getSatisfiableCalculationProcessingConstructs();
				while (satCalcProcConstruct) {
					CSaturationConceptReferenceLinking* satConRefLink = satCalcProcConstruct->getSaturationConceptReferenceLinking();
					if (satConRefLink) {
						CIndividualSaturationProcessNode* indiNode = (CIndividualSaturationProcessNode*)satConRefLink->getIndividualProcessNodeForConcept();
						if (!indiNode->isInitialized()) {
							CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = indiNode->getIndividualSaturationProcessNodeLinker();
							indiProcNodeLinker->setNegation(true);
							dataBox->addIndividualSaturationProcessNodeLinker(indiProcNodeLinker);
						}
					}
					CSaturationIndividualReferenceLinking* satIndiRefLink = satCalcProcConstruct->getSaturationIndividualReferenceLinking();
					if (satIndiRefLink) {
						CIndividualSaturationProcessNode* indiNode = (CIndividualSaturationProcessNode*)satIndiRefLink->getIndividualProcessNodeForConcept();
						if (!indiNode->isInitialized()) {
							CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = indiNode->getIndividualSaturationProcessNodeLinker();
							indiProcNodeLinker->setNegation(true);
							dataBox->addIndividualSaturationProcessNodeLinker(indiProcNodeLinker);
						}
					}
					satCalcProcConstruct = satCalcProcConstruct->getNextConstruct();
				}

				if (additionalCalculatedCallback) {
					satCalcTask->addCallbackLinker(additionalCalculatedCallback);
				}

				satCalcTask->setSaturationIndividualsAnalysationObserver(approxSaturCalcJob->getSaturationIndividualsAnalysationObserver());

				satCalcTask->setConsistenceAdapter(approxSaturCalcJob->getSaturationTaskPreyingAdapter());

				satCalcTask->setCalculationTaskType(CSatisfiableCalculationTask::CALCULATIONTABLEAUAPPROXIMATEDSATURATIONTASK);

				return satCalcTask;
			}




			CSatisfiableCalculationTask* CSatisfiableCalculationTaskFromCalculationJobGenerator::createSatisfiableCalculationTask(CConcreteOntology *ontology, CCalculationJob* calculationJob, CCallbackData* additionalCalculatedCallback) {
				CSatisfiableCalculationJob* satCalcJob = dynamic_cast<CSatisfiableCalculationJob*>(calculationJob);
				if (satCalcJob) {
					CSatisfiableCalculationTask* baseTask = nullptr;
					bool lastConDesReapplication = false;
					bool clearIndiProcessingQueue = false;
					if (satCalcJob->getConsecutivelyCalculationJobInstantiation()) {
						baseTask = dynamic_cast<CSatisfiableCalculationTask*>(satCalcJob->getConsecutivelyCalculationJobInstantiation());
						lastConDesReapplication = false;
					}
					if (!baseTask) {
						CConsistence* consistence = ontology->getConsistence();
						CConsistenceTaskData* consTaskData = nullptr;
						if (consistence) {
							CConsistenceData* consData = consistence->getConsistenceModelData();
							if (consData) {
								consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
							}
						}
						if (consTaskData) {
							baseTask = consTaskData->getDeterministicSatisfiableTask();
							lastConDesReapplication = consTaskData->getDeterministicSatisfiableTask() != consTaskData->getCompletionGraphCachedSatisfiableTask();
							clearIndiProcessingQueue = true;
						}
					}

					return createSatisfiableCalculationTaskExtension(ontology,calculationJob,baseTask,lastConDesReapplication,clearIndiProcessingQueue,additionalCalculatedCallback);
				} 


				CApproximatedSaturationCalculationJob* approxSaturCalcJob = dynamic_cast<CApproximatedSaturationCalculationJob*>(calculationJob);
				if (approxSaturCalcJob) {
					CCalculationConfigurationExtension* calcConfig = approxSaturCalcJob->getCalculationConfiguration();
					CSatisfiableCalculationTask* satCalcTask = nullptr;
					satCalcTask = createApproximatedSaturationCalculationTask(ontology,approxSaturCalcJob,additionalCalculatedCallback);
					return satCalcTask;

				}


				return nullptr;
			}


			CSatisfiableCalculationTask* CSatisfiableCalculationTaskFromCalculationJobGenerator::createSatisfiableCalculationTaskExtension(CCalculationJob* calculationJob, CSatisfiableCalculationTask* baseTask, bool lastConDesReapplication, bool clearIndiProcessingQueue, CCallbackData* additionalCalculatedCallback) {
				return createSatisfiableCalculationTaskExtension(calculationJob->getOntology(),calculationJob,baseTask,lastConDesReapplication,clearIndiProcessingQueue,additionalCalculatedCallback);
			}



			CSatisfiableCalculationTask* CSatisfiableCalculationTaskFromCalculationJobGenerator::createSatisfiableCalculationTaskExtension(CConcreteOntology *ontology, CCalculationJob* calculationJob, CSatisfiableCalculationTask* baseTask, bool lastConDesReapplication, bool clearIndiProcessingQueue, CCallbackData* additionalCalculatedCallback) {

				CSatisfiableCalculationJob* satCalcJob = dynamic_cast<CSatisfiableCalculationJob*>(calculationJob);

				if (satCalcJob) {

					cint64 firstPossibleNewIndividualID = ontology->getABox()->getIndividualCount();

					bool requiresTaskCalc = false;

					CIndividualVector* baseIndiVec = nullptr;
					if (baseTask) {
						baseIndiVec = baseTask->getProcessingDataBox()->getOntology()->getABox()->getIndividualVector(false);
					}

					CSatisfiableCalculationTask* satCalcTask = CObjectMemoryPoolAllocator<CSatisfiableCalculationTask>::allocateAndConstructWithMemroyPool(mGenTaskHandleContext->getTaskHandleMemoryAllocationManager());



					bool recreateNodesForIndividuals = false;
					if (clearIndiProcessingQueue) {
						bool avoidIndividualReprocessing = satCalcJob->getCalculationConfiguration()->isAvoidRepeatedIndividualProcessingActivated();
						if (!avoidIndividualReprocessing) {
							clearIndiProcessingQueue = false;

							if (satCalcJob->getCalculationConfiguration()->isForceNodesRecreationForRepeatedIndividualProcessingActivated()) {
								baseTask = nullptr;

								if (!recreateNodesForIndividuals) {
									for (CSatisfiableCalculationConstruct* satCalcConstructIt = satCalcJob->getSatisfiableCalculationConstructs(); satCalcConstructIt && !clearIndiProcessingQueue; satCalcConstructIt = satCalcConstructIt->getNextConstruct()) {
										CIndividual* individual = satCalcConstructIt->getIndividual();
										if (individual) {
											recreateNodesForIndividuals = true;
										}
									}
								}

								if (!recreateNodesForIndividuals) {
									if (calculationJob->getOntology()->getStructureSummary()->hasNominalOccurrence() || calculationJob->getOntology()->getTBox()->getUniversalConnectionNominalValueConcept()) {
										recreateNodesForIndividuals = true;
									}
								}

							}
						} else {

							bool confCompletionGraphCaching = satCalcJob->getCalculationConfiguration()->isCompletionGraphCachingActivated();
							if (!confCompletionGraphCaching) {

								if (clearIndiProcessingQueue) {
									for (CSatisfiableCalculationConstruct* satCalcConstructIt = satCalcJob->getSatisfiableCalculationConstructs(); satCalcConstructIt && !clearIndiProcessingQueue; satCalcConstructIt = satCalcConstructIt->getNextConstruct()) {
										CIndividual* individual = satCalcConstructIt->getIndividual();
										if (individual) {
											clearIndiProcessingQueue = false;
										}
									}
								}

								if (clearIndiProcessingQueue) {
									if (calculationJob->getOntology()->getStructureSummary()->hasNominalOccurrence() || calculationJob->getOntology()->getTBox()->getUniversalConnectionNominalValueConcept()) {
										clearIndiProcessingQueue = false;
									}
								}
							}
						}
					}


					if (baseTask) {
						satCalcTask->initUndependedSatisfiableCalculationTask(baseTask,satCalcJob->getCalculationConfiguration(),satCalcJob->getCalclulationStatisticsCollector(),mGenTaskHandleContext);
					} else {
						satCalcTask->initSatisfiableCalculationTask(ontology,satCalcJob->getCalculationConfiguration(),satCalcJob->getCalclulationStatisticsCollector(),mGenTaskHandleContext);
					}
					CProcessContext* procContext = satCalcTask->getProcessContext(mGenTaskHandleContext);
					if (baseTask) {
						CProcessTagger* processTagger = procContext->getUsedProcessTagger();
						processTagger->incLocalizationTag();
					}

					satCalcTask->setConsistenceAdapter(satCalcJob->getSatisfiableTaskPreyingAdapter());
					satCalcTask->setClassificationMessageAdapter(satCalcJob->getSatisfiableClassificationMessageAdapter());
					satCalcTask->setRealizationMarkedCandidatesMessageAdapter(satCalcJob->getRealizationMarkedCandidatesMessageAdapter());
					satCalcTask->setSatisfiableTaskIncrementalConsistencyTestingAdapter(satCalcJob->getSatisfiableTaskIncrementalConsistencyTestingAdapter());
					satCalcTask->setSatisfiableTaskIndividualDependenceTrackingAdapter(satCalcJob->getSatisfiableTaskIndividualDependenceTrackingAdapter());
					satCalcTask->setPossibleAssertionCollectionAdapter(satCalcJob->getPossibleAssertionCollectionAdapter());


					CProcessingDataBox* dataBox = satCalcTask->getProcessingDataBox();
					dataBox->setConstructedIndividualNode(nullptr);
					dataBox->setReapplicationLastConceptDesciptorOnLastIndividualNodeRequired(lastConDesReapplication);

					if (satCalcTask->getSatisfiableTaskIncrementalConsistencyTestingAdapter()) {
						CConcreteOntology* prevOntology = satCalcTask->getSatisfiableTaskIncrementalConsistencyTestingAdapter()->getPreviousConsistentOntology();

						CSatisfiableCalculationTask* prevDetConsTask = nullptr;
						CSatisfiableCalculationTask* prevNondetConsTask = nullptr;
						CConsistence* prevConsistence = prevOntology->getConsistence();
						CConsistenceTaskData* prevConsTaskData = nullptr;
						if (prevConsistence) {
							CConsistenceData* prevConsData = prevConsistence->getConsistenceModelData();
							if (prevConsData) {
								prevConsTaskData = dynamic_cast<CConsistenceTaskData*>(prevConsData);
							}
						}
						if (prevConsTaskData) {
							prevDetConsTask = prevConsTaskData->getDeterministicSatisfiableTask();
							prevNondetConsTask = prevConsTaskData->getCompletionGraphCachedSatisfiableTask();
						}
						if (prevDetConsTask) {
							CProcessTagger* prevProcessTagger = prevDetConsTask->getProcessingDataBox()->getProcessContext()->getUsedProcessTagger();
							CProcessTagger* processTagger = procContext->getUsedProcessTagger();
							processTagger->initProcessTagger(prevProcessTagger);
							processTagger->incLocalizationTag();
						}
						if (prevNondetConsTask) {
							firstPossibleNewIndividualID = prevNondetConsTask->getProcessingDataBox()->getNextIndividualNodeID(false);
						}
						prevDetConsTask->getProcessingDataBox()->setIncrementalExpansionID(satCalcTask->getSatisfiableTaskIncrementalConsistencyTestingAdapter()->getIncrementalRevisionID());
					}
					if (satCalcTask->getSatisfiableTaskIndividualDependenceTrackingAdapter()) {
						dataBox->setIndividualDependenceTrackingRequired(true);
					}

					CIndividualProcessNodeVector* indiNodeVec = dataBox->getIndividualProcessNodeVector();

					if (clearIndiProcessingQueue) {
						dataBox->clearIndividualProcessingQueues();
					}


					CIndividualUnsortedProcessingQueue* indiNodeQueue = dataBox->getIndividualImmediatelyProcessingQueue(true);

					CDependencyTrackPoint* independentBaseDepTrackPoint = dataBox->getBranchingTree(true)->getBaseDependencyNode(true)->getContinueDependencyTrackPoint();

					CMemoryAllocationManager* memMan = procContext->getUsedMemoryAllocationManager();


					firstPossibleNewIndividualID = qMax(firstPossibleNewIndividualID,indiNodeVec->getItemCount());

					cint64 baseIndiID = firstPossibleNewIndividualID;
					cint64 constructionIndiCount = 0;

					CSatisfiableCalculationConstruct* satCalcConstruct = satCalcJob->getSatisfiableCalculationConstructs();
					while (satCalcConstruct) {
						++constructionIndiCount;
						CIndividual* individual = satCalcConstruct->getIndividual();
						cint64 individualID = baseIndiID+satCalcConstruct->getRelativeNewNodeID();
						if (individual) {
							individualID = individual->getIndividualID();
						} else {
							cint64 fixedIndiID = satCalcConstruct->getIndividualID();
							if (fixedIndiID >= 0) {
								individualID = fixedIndiID;
							}
						}
						firstPossibleNewIndividualID = qMax(firstPossibleNewIndividualID,individualID+1);

						CXSortedNegLinker<CConcept*>* satConJobLinker = nullptr;
						CSatisfiableCalculationConceptConstruct* satConCalcConstuct = dynamic_cast<CSatisfiableCalculationConceptConstruct*>(satCalcConstruct);
						if (satConCalcConstuct) {
							satConJobLinker = satConCalcConstuct->getConstructConceptLinker();
						}

						CIndividualProcessNode* refIndi = indiNodeVec->getData(individualID);
						// check whether individual is already processed
						if (satConJobLinker || !refIndi || !baseIndiVec || baseIndiVec->getData(individualID) != individual) {
							requiresTaskCalc = true;
							CIndividualProcessNode* indi = indiNodeVec->getLocalData(individualID);
							CIndividualProcessNode* localIndi = indi;
							if (!localIndi) {
								localIndi = CObjectParameterizingAllocator< CIndividualProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(memMan,procContext);
								localIndi->initDependencyTracker(independentBaseDepTrackPoint);
								if (refIndi) {
									while (refIndi->hasMergedIntoIndividualNodeID()) {
										cint64 corrIndiID = refIndi->getMergedIntoIndividualNodeID();
										refIndi = indiNodeVec->getData(corrIndiID);
										individualID = corrIndiID;
									}
									localIndi->initIndividualProcessNode(refIndi);
									localIndi->clearProcessingQueued();
									localIndi->clearProcessingRestrictionFlags(CIndividualProcessNode::PRFCACHEDCOMPUTEDTYPESADDED);
								} else if (individual) {
									localIndi->setAssertionConceptLinker(individual->getAssertionConceptLinker());
									localIndi->setAssertionRoleLinker(individual->getAssertionRoleLinker());
									localIndi->setReverseAssertionRoleLinker(individual->getReverseAssertionRoleLinker());
								}

								localIndi->setIndividualID(individualID);
								if (individual) {
									localIndi->setNominalIndividual(individual);
									localIndi->setIndividualType(CIndividualProcessNode::NOMINALINDIVIDUALTYPE);
								}
								if (!individual || baseTask) {
									localIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING);
								}

								localIndi->setInitializingConceptLinker(nullptr);
								indiNodeVec->setLocalData(individualID,localIndi);
								indiNodeQueue->insertIndiviudalProcessNode(localIndi);

								if (dataBox->getConstructedIndividualNode() == nullptr) {
									dataBox->setConstructedIndividualNode(localIndi);
								}					
							}

							CXSortedNegLinker<CConcept*>* initConLinker = nullptr;
							while (satConJobLinker) {
								CXSortedNegLinker<CConcept*>* tmpConLinker = CObjectAllocator< CXSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
								initConLinker = tmpConLinker->initNegLinker(satConJobLinker->getData(),satConJobLinker->getNegation(),initConLinker);
								satConJobLinker = satConJobLinker->getNext();
							}
							localIndi->addInitializingConceptLinker(initConLinker);

						}


						satCalcConstruct = satCalcConstruct->getNextConstruct();
					}

					if (recreateNodesForIndividuals) {
						CIndividualVector* indiVec = ontology->getABox()->getIndividualVector(false);
						if (indiVec) {
							cint64 indiCount = indiVec->getItemCount();
							for (cint64 i = 0; i < indiCount; ++i) {
								CIndividual* indi = indiVec->getData(i);
								if (indi) {
									CIndividualProcessNode* indiNode = indiNodeVec->getData(i);
									if (!indiNode) {
										indiNode = CObjectParameterizingAllocator< CIndividualProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(memMan,procContext);
										indiNode->initDependencyTracker(independentBaseDepTrackPoint);
										indiNode->setAssertionConceptLinker(indi->getAssertionConceptLinker());
										indiNode->setAssertionRoleLinker(indi->getAssertionRoleLinker());
										indiNode->setReverseAssertionRoleLinker(indi->getReverseAssertionRoleLinker());

										indiNode->setIndividualID(i);
										indiNode->setNominalIndividual(indi);
										indiNode->setIndividualType(CIndividualProcessNode::NOMINALINDIVIDUALTYPE);

										indiNodeVec->setLocalData(i,indiNode);
										indiNodeQueue->insertIndiviudalProcessNode(indiNode);
									}
								}
							}
						}
					}

					dataBox->setFirstPossibleIndividualNodeID(firstPossibleNewIndividualID);
					dataBox->setMultipleConstructionIndividualNodes(constructionIndiCount > 1);

					if (additionalCalculatedCallback) {
						satCalcTask->addCallbackLinker(additionalCalculatedCallback);
					}

					return satCalcTask;

				}
				return nullptr;
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude
