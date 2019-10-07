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

#include "CSatisfiableTaskMarkerIndividualPropagationAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CSatisfiableTaskMarkerIndividualPropagationAnalyser::CSatisfiableTaskMarkerIndividualPropagationAnalyser() {
				}



				bool CSatisfiableTaskMarkerIndividualPropagationAnalyser::readCalculationConfig(CCalculationConfigurationExtension *config) {					
					if (config) {
						return true;
					}
					return false;
				}


				CIndividualProcessNode* CSatisfiableTaskMarkerIndividualPropagationAnalyser::getCorrectedIndividualNode(cint64 baseIndiID, CIndividualProcessNodeVector* indiNodeVec) {
					CIndividualProcessNode* indi = indiNodeVec->getData(baseIndiID);
					while (indi->hasMergedIntoIndividualNodeID()) {
						indi = indiNodeVec->getData(indi->getMergedIntoIndividualNodeID());
					}
					return indi;
				}


				bool CSatisfiableTaskMarkerIndividualPropagationAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter* realMessAdapter = statCalcTask->getRealizationMarkedCandidatesMessageAdapter();
					if (realMessAdapter) {
						CRealizationMessageObserver* realMessObserver = realMessAdapter->getRealizationMessageDataObserver();
						CIndividualReference individualReference = realMessAdapter->getTestingIndividualReference();
						CConcreteOntology* ontology = realMessAdapter->getTestingOntology();


						CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
						CTaskHandleMemoryAllocationManager* taskMemMan = calcAlgContext->getTemporaryMemoryAllocationManager();

						CMemoryPoolContainer memPoolCon;
						CTaskMemoryPoolAllocationManager realMessMemManCreaterMemMan(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						CTaskMemoryPoolAllocationManager* tempMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&realMessMemManCreaterMemMan);
						tempMemAllocMan->initTaskMemoryPoolAllocationManager(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						CContextBase* tmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(tempMemAllocMan,tempMemAllocMan);


						CRealizationMessageData* realMessageData = nullptr;



						CProcessingDataBox* procDataBox = calcAlgContext->getProcessingDataBox();
						cint64 maxDetBranchTag = procDataBox->getMaximumDeterministicBranchTag();
						CIndividualVector* indiVec = procDataBox->getOntology()->getABox()->getIndividualVector();
						CMarkerIndividualNodeHash* markerIndiNodeHash = procDataBox->getMarkerIndividualNodeHash(false);
						if (markerIndiNodeHash) {
							for (CPROCESSHASH< CConcept*, CMarkerIndividualNodeData >::const_iterator it = markerIndiNodeHash->constBegin(), itEnd = markerIndiNodeHash->constEnd(); it != itEnd; ++it) {
								CConcept* markerConcept = it.key();
								CIndividualRoleCandidateTestingData testingData = realMessAdapter->getIndividualRoleCandidateTestingDataByMarkerConcept(markerConcept);
								CXLinker<CIndividualReference>* detIndiCandLinker = nullptr;
								CXLinker<CIndividualReference>* ndetIndiCandLinker = nullptr;
								CMarkerIndividualNodeData data = it.value();
								CXNegLinker<CIndividualProcessNode*>* candIndiLinker = data.getMarkerIndividualNodeLinker();
								for (CXNegLinker<CIndividualProcessNode*>* candIndiLinkerIt = candIndiLinker; candIndiLinkerIt; candIndiLinkerIt = candIndiLinkerIt->getNext()) {
									CIndividualProcessNode* indiNode = candIndiLinkerIt->getData();
									CIndividual* candIndividual = indiNode->getNominalIndividual();
									if (candIndividual) {
										CXLinker<CIndividualReference>* tmpLinker = CObjectAllocator< CXLinker<CIndividualReference> >::allocateAndConstruct(tempMemAllocMan);
										if (candIndividual->isTemporaryIndividual()) {
											tmpLinker->initLinker(CIndividualReference(candIndividual->getIndividualID()));
										} else {
											tmpLinker->initLinker(candIndividual);
										}
										bool nondeterministicallyAdded = candIndiLinkerIt->isNegated();
										if (nondeterministicallyAdded) {
											ndetIndiCandLinker = tmpLinker->append(ndetIndiCandLinker);
										} else {
											detIndiCandLinker = tmpLinker->append(detIndiCandLinker);
										}


										CIndividualMergingHash* indiMergingHash = indiNode->getIndividualMergingHash(false);
										if (indiMergingHash) {

											for (CIndividualMergingHash::const_iterator indiIt = indiMergingHash->constBegin(), indiItEnd = indiMergingHash->constEnd(); indiIt != indiItEnd; ++indiIt) {

												if (indiIt.value().isMergedWithIndividual()) {
													cint64 indiIDIt = indiIt.key();
													CIndividual* mergedIndividual = indiVec->getData(indiIDIt);

													if (mergedIndividual != candIndividual) {

														CDependencyTrackPoint* mergeDepTrackPoint = indiIt.value().getDependencyTrackPoint();
														bool nonDetMerged = true;
														if (mergeDepTrackPoint && mergeDepTrackPoint->getBranchingTag() <= maxDetBranchTag) {
															nonDetMerged = false;
														}
														if (nonDetMerged) {
															CXLinker<CIndividualReference>* nomTmpLinker = CObjectAllocator< CXLinker<CIndividualReference> >::allocateAndConstruct(tempMemAllocMan);
															if (mergedIndividual->isTemporaryIndividual()) {
																nomTmpLinker->initLinker(CIndividualReference(mergedIndividual->getIndividualID()));
															} else {
																nomTmpLinker->initLinker(mergedIndividual);
															}
															ndetIndiCandLinker = nomTmpLinker->append(ndetIndiCandLinker);
														}
													}
												}
											}
										}

									}
								}

								CRealizationMarkerCandidatesMessageData* realMarkCandMessageData = CObjectAllocator<CRealizationMarkerCandidatesMessageData>::allocateAndConstruct(tempMemAllocMan);
								realMarkCandMessageData->initRealizationMarkerCandidatesMessageData(individualReference, testingData, markerConcept,detIndiCandLinker,ndetIndiCandLinker);
								realMessageData = realMarkCandMessageData->append(realMessageData);

							}

						}
						if (realMessageData) {
							realMessObserver->tellRealizationMessage(ontology,realMessageData,memPoolCon.takeMemoryPools());
						} else {
							taskMemMan->releaseTemporaryMemoryPools(memPoolCon.takeMemoryPools());
						}

						if (individualReference.isNonEmpty()) {
							return true;
						}
					}
					return false;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
