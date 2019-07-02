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

#include "CSatisfiableTaskComplexAnsweringMessageAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CSatisfiableTaskComplexAnsweringMessageAnalyser::CSatisfiableTaskComplexAnsweringMessageAnalyser() {
				}



				bool CSatisfiableTaskComplexAnsweringMessageAnalyser::readCalculationConfig(CCalculationConfigurationExtension *config) {					
					if (config) {
						return true;
					}
					return false;
				}


				CIndividualProcessNode* CSatisfiableTaskComplexAnsweringMessageAnalyser::getCorrectedIndividualID(CIndividualProcessNode* baseIndiNode, CIndividualProcessNodeVector* indiNodeVec, bool* nondetMergedFlag) {
					CIndividualProcessNode* indi = indiNodeVec->getData(baseIndiNode->getIndividualNodeID());
					while (indi->hasMergedIntoIndividualNodeID()) {
						if (nondetMergedFlag && *nondetMergedFlag == false) {
							if (!indi->getMergedDependencyTrackPoint() || indi->getMergedDependencyTrackPoint()->getBranchingTag() > 0) {
								*nondetMergedFlag = true;
							}
						}
						indi = indiNodeVec->getData(indi->getMergedIntoIndividualNodeID());
					}
					return indi;
				}



				CAnsweringMessageData* CSatisfiableTaskComplexAnsweringMessageAnalyser::extractPossibleSubsumptionInformation(CConcept* testingConcept, bool testingConceptNegation, CIndividualProcessNode* indiNode, CReapplyConceptLabelSet* conSet, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(ANALYSEPOSSIBLESUBSUMPTIONSCOUNT,calcAlgContext);

					CAnsweringMessageData* messageDataLinker = nullptr;


					// initialize possible subsumption list
					CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>* possSubsumerList = nullptr;


					CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true,true,false);
				
					// create new initialization list for possible subsumers
					while (conSetIt.hasNext()) {
						CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
						CConcept* concept = conDes->getConcept();
						bool negated = conDes->getNegation();


						if (!negated && concept->hasClassName()) {
							if (!possSubsumerList) {
								possSubsumerList = CObjectParameterizingAllocator< CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CAnsweringMessageDataCalculationPossibleClassSubsumersData*>,CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTmpContext);
							}
							CAnsweringMessageDataCalculationPossibleClassSubsumersData* possSubsumData = CObjectAllocator< CAnsweringMessageDataCalculationPossibleClassSubsumersData >::allocateAndConstruct(mTempMemAllocMan);
							possSubsumData->initClassificationPossibleSubsumptionData(concept);
							possSubsumerList->append(possSubsumData);
						}
						conSetIt.moveNext();
					}



					CAnsweringMessageDataCalculationPossibleClassSubsumers* possSubsumMessageData = CObjectAllocator<CAnsweringMessageDataCalculationPossibleClassSubsumers>::allocateAndConstruct(mTempMemAllocMan);
					possSubsumMessageData->initAnsweringPossibleSubsumptionMessageData(testingConcept,testingConceptNegation,possSubsumerList);
					messageDataLinker = possSubsumMessageData->append(messageDataLinker);
					return messageDataLinker;
				}



				bool CSatisfiableTaskComplexAnsweringMessageAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					bool analysed = false;
					mStatCalcTask = statCalcTask;
					analysed |= analyseSatisfiableTaskSubsumption(mStatCalcTask, calcAlgContext);
					analysed |= analyseSatisfiableTaskInstancePropagation(mStatCalcTask, calcAlgContext);
					return analysed;
				}








				bool CSatisfiableTaskComplexAnsweringMessageAnalyser::analyseSatisfiableTaskInstancePropagation(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					mStatCalcTask = statCalcTask;
					CSatisfiableTaskAnswererInstancePropagationMessageAdapter* answererInstancePropagationAdapter = statCalcTask->getSatisfiableAnswererInstancePropagationMessageAdapter();
					if (answererInstancePropagationAdapter) {
						CAnsweringMessageDataObserver* conceptMessClassObserver = answererInstancePropagationAdapter->getAnswererMessageDataObserver();
						CAnsweringMessageData* propagatedIndiMessageDataLinker = nullptr;
						CAnsweringMessageData* possiblePropagatedIndiMessageDataLinker = nullptr;


						CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
						CTaskHandleMemoryAllocationManager* taskMemMan = calcAlgContext->getTemporaryMemoryAllocationManager();
						CConcept* testingConcept = answererInstancePropagationAdapter->getTestingConcept();
						bool candidatePropagation = answererInstancePropagationAdapter->isCandidatePropagation();
						bool testingConceptNegation = answererInstancePropagationAdapter->getTestingConceptNegation();

						//QString iriClassNameString = CIRIName::getRecentIRIName(testingConcept->getClassNameLinker());
						//if (iriClassNameString == "http://www.owllink.org/testsuite/galen#LowerLobeOfLeftLung") {
						//	bool bug = true;
						//}
						//} 
						//considerOtherNode = false;

						CAnsweringHandler* answeringHandler = answererInstancePropagationAdapter->getAnsweringHandler();
						CConcreteOntology* testOntology = answererInstancePropagationAdapter->getTestingOntology();
						CProcessingDataBox* processingDataBox = statCalcTask->getProcessingDataBox();
						CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();

						cint64 maxDetBranchTag = processingDataBox->getMaximumDeterministicBranchTag();




						if (testOntology && testingConcept) {

							CMemoryPoolContainer memPoolCon;
							CTaskMemoryPoolAllocationManager classMessMemManCreaterMemMan(&memPoolCon, calcAlgContext->getTemporaryMemoryAllocationManager());
							mTempMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&classMessMemManCreaterMemMan);
							mTempMemAllocMan->initTaskMemoryPoolAllocationManager(&memPoolCon, calcAlgContext->getTemporaryMemoryAllocationManager());
							mTmpContext = CObjectParameterizingAllocator< CContextBase, CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mTempMemAllocMan, mTempMemAllocMan);


							CProcessingDataBox* procDataBox = calcAlgContext->getProcessingDataBox();
							CIndividualVector* indiVec = procDataBox->getIndividualVector();
							CMarkerIndividualNodeHash* markerIndiNodeHash = procDataBox->getMarkerIndividualNodeHash(false);

							bool hasMarkedPropagatedConceptIndis = false;
							if (markerIndiNodeHash) {

								for (CPROCESSHASH< CConcept*, CMarkerIndividualNodeData >::const_iterator it = markerIndiNodeHash->constBegin(), itEnd = markerIndiNodeHash->constEnd(); it != itEnd; ++it) {
									CConcept* markerConcept = it.key();
									CXLinker<CIndividualReference>* detIndiCandLinker = nullptr;
									CXLinker<CIndividualReference>* ndetIndiCandLinker = nullptr;
									CXNegLinker<CIndividualProcessNode*>* candIndiLinker = it.value().getMarkerIndividualNodeLinker();
									CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>* indiList = nullptr;
									CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>* possIndiList = nullptr;

									for (CXNegLinker<CIndividualProcessNode*>* candIndiLinkerIt = candIndiLinker; candIndiLinkerIt; candIndiLinkerIt = candIndiLinkerIt->getNext()) {
										CIndividualProcessNode* indiNode = candIndiLinkerIt->getData();
										CIndividual* candIndividual = indiNode->getNominalIndividual();
										if (candIndividual) {

											hasMarkedPropagatedConceptIndis = true;

											bool nondeterministicallyAdded = candIndiLinkerIt->isNegated();
											if (nondeterministicallyAdded || candidatePropagation) {
												addIndividualToReferenceList(possIndiList, candIndividual);
											} else {
												addIndividualToReferenceList(indiList, candIndividual);
											}


											CIndividualMergingHash* indiMergingHash = indiNode->getIndividualMergingHash(false);
											if (indiMergingHash) {

												for (CIndividualMergingHash::const_iterator indiIt = indiMergingHash->constBegin(), indiItEnd = indiMergingHash->constEnd(); indiIt != indiItEnd; ++indiIt) {
													cint64 indiIDIt = indiIt.key();

													if (indiIt.value().isMergedWithIndividual()) {


														if (indiIDIt != candIndividual->getIndividualID()) {
															CIndividual* mergedIndividual = indiVec->getData(indiIDIt);

															CDependencyTrackPoint* mergeDepTrackPoint = indiIt.value().getDependencyTrackPoint();
															bool nonDetMerged = true;
															if (mergeDepTrackPoint && mergeDepTrackPoint->getBranchingTag() <= maxDetBranchTag) {
																nonDetMerged = false;
															}

															if (nonDetMerged || candidatePropagation) {
																addIndividualToReferenceList(possIndiList, mergedIndividual);
															} else {
																addIndividualToReferenceList(indiList, mergedIndividual);
															}
														}
													}
													

												}
											}


										}
									}

									if (possIndiList) {
										CAnsweringMessageDataInstancePossiblePropagationsData* possInstancePropMessageData = CObjectAllocator<CAnsweringMessageDataInstancePossiblePropagationsData>::allocateAndConstruct(mTempMemAllocMan);
										possInstancePropMessageData->initInstancePropagationsData(testingConcept, testingConceptNegation, markerConcept, possIndiList);
										possiblePropagatedIndiMessageDataLinker = possInstancePropMessageData->append(possiblePropagatedIndiMessageDataLinker);
									}

									if (indiList) {
										CAnsweringMessageDataInstanceCertainPropagationsData* certainInstancePropMessageData = CObjectAllocator<CAnsweringMessageDataInstanceCertainPropagationsData>::allocateAndConstruct(mTempMemAllocMan);
										certainInstancePropMessageData->initInstancePropagationsData(testingConcept, testingConceptNegation, markerConcept, indiList);
										propagatedIndiMessageDataLinker = certainInstancePropMessageData->append(propagatedIndiMessageDataLinker);
									}

								}
							}


							


							CAnsweringMessageData* messageDataLinker = possiblePropagatedIndiMessageDataLinker;
							if (propagatedIndiMessageDataLinker) {
								messageDataLinker = propagatedIndiMessageDataLinker->append(messageDataLinker);
							}

							if (messageDataLinker) {
								conceptMessClassObserver->sendMessage(answeringHandler, messageDataLinker, memPoolCon.takeMemoryPools());
							} else {
								taskMemMan->releaseTemporaryMemoryPools(memPoolCon.takeMemoryPools());
							}


							return true;

						}
					}
					return false;
				}








				bool CSatisfiableTaskComplexAnsweringMessageAnalyser::addIndividualToReferenceList(CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>*& indiRefList, CIndividual* candIndividual) {
					if (!indiRefList) {
						indiRefList = CObjectParameterizingAllocator< CCLASSSUBSUMPTIONMESSAGELIST<CIndividualReference>, CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan, mTmpContext);
					}
					if (!candIndividual->isTemporaryIndividual()) {
						indiRefList->append(CIndividualReference(candIndividual));
					} else {
						indiRefList->append(CIndividualReference(candIndividual->getIndividualID()));
					}
					return true;
				}







				bool CSatisfiableTaskComplexAnsweringMessageAnalyser::analyseSatisfiableTaskSubsumption(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					mStatCalcTask = statCalcTask;
					CSatisfiableTaskAnswererSubsumptionMessageAdapter* answererConceptMarkedAdapter = statCalcTask->getSatisfiableAnswererSubsumptionMessageAdapter();
					if (answererConceptMarkedAdapter) {
						CAnsweringMessageDataObserver* conceptMessClassObserver = answererConceptMarkedAdapter->getAnswererMessageDataObserver();
						CAnsweringMessageData* subsumMessageDataLinker = nullptr;
						CAnsweringMessageData* possSubsumMessageDataLinker = nullptr;


						CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
						CTaskHandleMemoryAllocationManager* taskMemMan = calcAlgContext->getTemporaryMemoryAllocationManager();
						CConcept* testingConcept = answererConceptMarkedAdapter->getTestingConcept();
						bool testingConceptNegation = answererConceptMarkedAdapter->getTestingConceptNegation();
						CIndividualProcessNode* baseIndi = statCalcTask->getProcessingDataBox()->getConstructedIndividualNode();

						//QString iriClassNameString = CIRIName::getRecentIRIName(testingConcept->getClassNameLinker());
						//if (iriClassNameString == "http://www.owllink.org/testsuite/galen#LowerLobeOfLeftLung") {
						//	bool bug = true;
						//}
						//} 
						//considerOtherNode = false;

						CAnsweringHandler* answeringHandler = answererConceptMarkedAdapter->getAnsweringHandler();
						CConcreteOntology* testOntology = answererConceptMarkedAdapter->getTestingOntology();
						CProcessingDataBox* processingDataBox = statCalcTask->getProcessingDataBox();
						CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();

						cint64 maxDetBranchTag = processingDataBox->getMaximumDeterministicBranchTag();




						if (testOntology && testingConcept && baseIndi) {
							cint64 baseIndiID = baseIndi->getIndividualNodeID();
							CIndividualProcessNode* baseIndiNode = indiNodeVec->getData(baseIndiID);
							bool nondetMerged = false;
							baseIndiNode = getCorrectedIndividualID(baseIndiNode,indiNodeVec,&nondetMerged);
							if (nondetMerged) {
								maxDetBranchTag = 0;
							}


							CMemoryPoolContainer memPoolCon;
							CTaskMemoryPoolAllocationManager classMessMemManCreaterMemMan(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
							mTempMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&classMessMemManCreaterMemMan);
							mTempMemAllocMan->initTaskMemoryPoolAllocationManager(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
							mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTempMemAllocMan);

							CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList = nullptr;
							
							CReapplyConceptLabelSet* conSet = baseIndiNode->getReapplyConceptLabelSet(false);
							if (conSet) {
								CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true,true,true);
								while (conSetIt.hasNext()) {
									CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
									if (conDes) {
										CConcept* concept = conDes->getConcept();
										bool negated = conDes->getNegation();

										if (!negated && concept->hasClassName()) {
											CDependencyTrackPoint* depTrackPoint = conSetIt.getDependencyTrackPoint();
											bool deterministic = false;
											if (depTrackPoint) {
												if (depTrackPoint->getBranchingTag() <= maxDetBranchTag) {
													deterministic = true;
												}
											}

											if (deterministic) {
												if (!subsumerList) {
													subsumerList = CObjectParameterizingAllocator< CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>,CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTmpContext);
												}
												subsumerList->append(concept);
											}
										}
									}
									conSetIt.moveNext();
								}

								CAnsweringMessageData* messageData = extractPossibleSubsumptionInformation(testingConcept, testingConceptNegation,baseIndiNode,conSet,calcAlgContext);
								if (messageData) {
									possSubsumMessageDataLinker = messageData->append(possSubsumMessageDataLinker);
								}

							}

							CAnsweringMessageDataCalculationClassSubsumers* subsumMessageData = CObjectAllocator<CAnsweringMessageDataCalculationClassSubsumers>::allocateAndConstruct(mTempMemAllocMan);
							subsumMessageData->initAnsweringSubsumptionMessageData(testingConcept,testingConceptNegation,subsumerList);
							subsumMessageDataLinker = subsumMessageData->append(subsumMessageDataLinker);
							subsumerList = nullptr;





							CAnsweringMessageData* messageDataLinker = possSubsumMessageDataLinker;
							if (subsumMessageDataLinker) {
								messageDataLinker = subsumMessageDataLinker->append(messageDataLinker);
							}

							if (messageDataLinker) {
								conceptMessClassObserver->sendMessage(answeringHandler,messageDataLinker,memPoolCon.takeMemoryPools());
							} else {
								taskMemMan->releaseTemporaryMemoryPools(memPoolCon.takeMemoryPools());
							}



							return true;

						}
					}
					return false;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
