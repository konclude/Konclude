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



			CSatisfiableCalculationTask* CSatisfiableCalculationTaskFromCalculationJobGenerator::createDefaultSaturationCalculationTask(CConcreteOntology *ontology, CSaturationCalculationJob* saturCalcJob, CCallbackData* additionalCalculatedCallback) {
				CSatisfiableCalculationTask* satCalcTask = CObjectMemoryPoolAllocator<CSatisfiableCalculationTask>::allocateAndConstructWithMemroyPool(mGenTaskHandleContext->getTaskHandleMemoryAllocationManager());
				satCalcTask->initSatisfiableCalculationTask(ontology,saturCalcJob->getCalculationConfiguration(),saturCalcJob->getCalclulationStatisticsCollector(),mGenTaskHandleContext);
				CProcessContext* procContext = satCalcTask->getProcessContext(mGenTaskHandleContext);

				satCalcTask->setSatisfiableSubsumptionIdentifierAdapter(saturCalcJob->getSatisfiableSubsumptionIdentifierAdapter());
				CProcessingDataBox* dataBox = satCalcTask->getProcessingDataBox();

				CIndividualProcessNodeVector* indiNodeVec = dataBox->getIndividualProcessNodeVector();


				CMemoryAllocationManager* memMan = procContext->getUsedMemoryAllocationManager();
				cint64 nextIndiID = qMax(ontology->getABox()->getIndividualCount(),indiNodeVec->getItemCount());


				CSaturationCalculationConstruct* satCalcConstruct = saturCalcJob->getSatisfiableCalculationConstructs();
				while (satCalcConstruct) {
					CConcept* conConcept = satCalcConstruct->getConstructConcept();
					bool conNegation = satCalcConstruct->getConstructConceptNegation();
					CIndividual* individual = satCalcConstruct->getIndividual();
					cint64 individualID = nextIndiID;
					if (individual) {
						individualID = individual->getIndividualID();
					} else {
						cint64 fixedIndiID = satCalcConstruct->getIndividualID();
						if (fixedIndiID >= 0) {
							individualID = fixedIndiID;
						} else {
							++nextIndiID;
						}
					}

					CIndividualProcessNode* refIndi = indiNodeVec->getData(individualID);

					CIndividualProcessNode* indi = CObjectParameterizingAllocator< CIndividualProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(memMan,procContext);
					if (refIndi) {
						while (refIndi->hasMergedIntoIndividualNodeID()) {
							cint64 corrIndiID = refIndi->getMergedIntoIndividualNodeID();
							refIndi = indiNodeVec->getData(corrIndiID);
							individualID = corrIndiID;
						}
						indi->initIndividualProcessNode(refIndi);
					} else if (individual) {
						indi->setAssertionConceptLinkerIt(individual->getAssertionConceptLinker());
						indi->setAssertionRoleLinkerIt(individual->getAssertionRoleLinker());
					}

					CXSortedNegLinker<CConcept*>* initConLinker = CObjectAllocator< CXSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
					initConLinker->initNegLinker(conConcept,conNegation);
					CConceptProcessData* conProcData = (CConceptProcessData*)conConcept->getConceptData();
					CConceptSaturationReferenceLinkingData* conSatRefLinkData = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
					conSatRefLinkData->getClassifierReferenceLinkingData()->setIndividualProcessNodeForConcept(indi);

					indi->setInitializingConceptLinkerIt(initConLinker);
					indi->setIndividualID(individualID);
					if (individual) {
						indi->setNominalIndividual(individual);
						indi->setIndividualType(CIndividualProcessNode::NOMINALINDIVIDUALTYPE);
					}
					indiNodeVec->setLocalData(individualID,indi);

					CIndividualProcessNodeLinker* indiProcNodeLinker = CObjectAllocator< CIndividualProcessNodeLinker >::allocateAndConstruct(memMan);
					indiProcNodeLinker->initProcessNodeLinker(indi,true);
					indi->setIndividualProcessNodeLinker(indiProcNodeLinker);
					dataBox->addIndividualProcessNodeLinker(indiProcNodeLinker);


					satCalcConstruct = satCalcConstruct->getNextConstruct();
				}

				if (additionalCalculatedCallback) {
					satCalcTask->addCallbackLinker(additionalCalculatedCallback);
				}

				satCalcTask->setCalculationTaskType(CSatisfiableCalculationTask::CALCULATIONTABLEAUDEFAULTSATURATIONTASK);

				return satCalcTask;
			}





			CSatisfiableCalculationTask* CSatisfiableCalculationTaskFromCalculationJobGenerator::createPilingSaturationCalculationTask(CConcreteOntology *ontology, CSaturationCalculationJob* saturCalcJob, CCallbackData* additionalCalculatedCallback) {
				CSatisfiableCalculationTask* satCalcTask = CObjectMemoryPoolAllocator<CSatisfiableCalculationTask>::allocateAndConstructWithMemroyPool(mGenTaskHandleContext->getTaskHandleMemoryAllocationManager());
				satCalcTask->initSatisfiableCalculationTask(ontology,saturCalcJob->getCalculationConfiguration(),saturCalcJob->getCalclulationStatisticsCollector(),mGenTaskHandleContext);
				CProcessContext* procContext = satCalcTask->getProcessContext(mGenTaskHandleContext);

				satCalcTask->setSatisfiableSubsumptionIdentifierAdapter(saturCalcJob->getSatisfiableSubsumptionIdentifierAdapter());
				CProcessingDataBox* dataBox = satCalcTask->getProcessingDataBox();

				CIndividualPilingProcessNodeVector* indiNodeVec = dataBox->getIndividualPilingProcessNodeVector();


				CMemoryAllocationManager* memMan = procContext->getUsedMemoryAllocationManager();
				cint64 nextIndiID = qMax(ontology->getABox()->getIndividualCount(),indiNodeVec->getItemCount());


				CSaturationCalculationConstruct* satCalcConstruct = saturCalcJob->getSatisfiableCalculationConstructs();
				while (satCalcConstruct) {
					CConcept* conConcept = satCalcConstruct->getConstructConcept();
					bool conNegation = satCalcConstruct->getConstructConceptNegation();
					cint64 individualID = nextIndiID++;

					CIndividualPilingProcessNode* indi = CObjectParameterizingAllocator< CIndividualPilingProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(memMan,procContext);
					indi->initIndividualPilingProcessNode(individualID);

					CXSortedNegLinker<CConcept*>* initConLinker = CObjectAllocator< CXSortedNegLinker<CConcept*> >::allocateAndConstruct(memMan);
					initConLinker->initNegLinker(conConcept,conNegation);
					CConceptProcessData* conProcData = (CConceptProcessData*)conConcept->getConceptData();
					CConceptSaturationReferenceLinkingData* conSatRefLinkData = (CConceptSaturationReferenceLinkingData*)conProcData->getConceptReferenceLinking();
					conSatRefLinkData->getClassifierReferenceLinkingData()->setIndividualProcessNodeForConcept(indi);

					indi->setInitializingConceptLinker(initConLinker);
					indiNodeVec->setLocalData(individualID,indi);

					CIndividualPilingProcessNodeLinker* indiProcNodeLinker = CObjectAllocator< CIndividualPilingProcessNodeLinker >::allocateAndConstruct(memMan);
					indiProcNodeLinker->initProcessNodeLinker(indi,true);
					indi->setIndividualPilingProcessNodeLinker(indiProcNodeLinker);
					dataBox->addIndividualPilingProcessNodeLinker(indiProcNodeLinker);

					satCalcConstruct = satCalcConstruct->getNextConstruct();
				}

				if (additionalCalculatedCallback) {
					satCalcTask->addCallbackLinker(additionalCalculatedCallback);
				}

				satCalcTask->setCalculationTaskType(CSatisfiableCalculationTask::CALCULATIONTABLEAUPILINGSATURATIONTASK);

				return satCalcTask;
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
				cint64 nextIndiID = qMax(ontology->getABox()->getIndividualCount(),indiNodeVec->getItemCount());


				CApproximatedSaturationCalculationConstructionConstruct* satCalcConsConstruct = approxSaturCalcJob->getSatisfiableCalculationConstructionConstructs();
				while (satCalcConsConstruct) {
					CSaturationConceptReferenceLinking* satConRefLink = satCalcConsConstruct->getSaturationConceptReferenceLinking();
					cint64 individualID = nextIndiID++;

					CIndividualSaturationProcessNode* indi = CObjectParameterizingAllocator< CIndividualSaturationProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(memMan,procContext);
					indi->initIndividualSaturationProcessNode(individualID,satConRefLink);
					satConRefLink->setIndividualProcessNodeForConcept(indi);

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
					CIndividualSaturationProcessNode* indiNode = (CIndividualSaturationProcessNode*)satConRefLink->getIndividualProcessNodeForConcept();
					if (!indiNode->isInitialized()) {
						CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = indiNode->getIndividualSaturationProcessNodeLinker();
						indiProcNodeLinker->setNegation(true);
						dataBox->addIndividualSaturationProcessNodeLinker(indiProcNodeLinker);
					}
					satCalcProcConstruct = satCalcProcConstruct->getNextConstruct();
				}

				if (additionalCalculatedCallback) {
					satCalcTask->addCallbackLinker(additionalCalculatedCallback);
				}

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
							clearIndiProcessingQueue = lastConDesReapplication;
						}
					}

					return createSatisfiableCalculationTaskExtension(ontology,calculationJob,baseTask,lastConDesReapplication,clearIndiProcessingQueue,additionalCalculatedCallback);
				} 

				CSaturationCalculationJob* saturCalcJob = dynamic_cast<CSaturationCalculationJob*>(calculationJob);
				if (saturCalcJob) {
					CCalculationConfigurationExtension* calcConfig = saturCalcJob->getCalculationConfiguration();
					CSatisfiableCalculationTask* satCalcTask = nullptr;
					if (calcConfig->isSaturationPilingActivated()) {
						satCalcTask = createPilingSaturationCalculationTask(ontology,saturCalcJob,additionalCalculatedCallback);
					} else {
						satCalcTask = createDefaultSaturationCalculationTask(ontology,saturCalcJob,additionalCalculatedCallback);
					}
					return satCalcTask;

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

					bool requiresTaskCalc = false;

					CIndividualVector* baseIndiVec = nullptr;
					if (baseTask) {
						baseIndiVec = baseTask->getProcessingDataBox()->getOntology()->getABox()->getIndividualVector(false);
					}

					CSatisfiableCalculationTask* satCalcTask = CObjectMemoryPoolAllocator<CSatisfiableCalculationTask>::allocateAndConstructWithMemroyPool(mGenTaskHandleContext->getTaskHandleMemoryAllocationManager());

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
					satCalcTask->setSatisfiableSubsumptionIdentifierAdapter(satCalcJob->getSatisfiableSubsumptionIdentifierAdapter());
					satCalcTask->setClassificationMessageAdapter(satCalcJob->getSatisfiableClassificationMessageAdapter());


					CProcessingDataBox* dataBox = satCalcTask->getProcessingDataBox();
					dataBox->setConstructedIndividualNode(nullptr);
					dataBox->setReapplicationLastConceptDesciptorOnLastIndividualNodeRequired(lastConDesReapplication);

					CIndividualProcessNodeVector* indiNodeVec = dataBox->getIndividualProcessNodeVector();

					if (clearIndiProcessingQueue) {
						dataBox->clearIndividualProcessingQueues();
					}

					CIndividualUnsortedProcessingQueue* indiNodeQueue = dataBox->getIndividualImmediatelyProcessingQueue(true);

					CDependencyTrackPoint* independentBaseDepTrackPoint = dataBox->getBranchingTree(true)->getBaseDependencyNode(true)->getContinueDependencyTrackPoint();

					CMemoryAllocationManager* memMan = procContext->getUsedMemoryAllocationManager();

					cint64 baseIndiID = qMax(ontology->getABox()->getIndividualCount(),indiNodeVec->getItemCount());

					CSatisfiableCalculationConstruct* satCalcConstruct = satCalcJob->getSatisfiableCalculationConstructs();
					while (satCalcConstruct) {
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
								} else if (individual) {
									localIndi->setAssertionConceptLinkerIt(individual->getAssertionConceptLinker());
									localIndi->setAssertionRoleLinkerIt(individual->getAssertionRoleLinker());
								}

								localIndi->setIndividualID(individualID);
								if (individual) {
									localIndi->setNominalIndividual(individual);
									localIndi->setIndividualType(CIndividualProcessNode::NOMINALINDIVIDUALTYPE);
								}
								if (!individual || baseTask) {
									localIndi->addProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDBLOCKINGORCACHING);
								}

								localIndi->setInitializingConceptLinkerIt(nullptr);
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
							localIndi->addInitializingConceptLinkerIt(initConLinker);

						}


						satCalcConstruct = satCalcConstruct->getNextConstruct();
					}

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
