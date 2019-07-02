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
						CIndividual* individual = realMessAdapter->getTestingIndividual();
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
						CMarkerIndividualNodeHash* markerIndiNodeHash = procDataBox->getMarkerIndividualNodeHash(false);
						if (markerIndiNodeHash) {
							for (CPROCESSHASH< CConcept*,CXNegLinker<CIndividualProcessNode*>* >::const_iterator it = markerIndiNodeHash->constBegin(), itEnd = markerIndiNodeHash->constEnd(); it != itEnd; ++it) {
								CConcept* markerConcept = it.key();
								CXLinker<CIndividual*>* detIndiCandLinker = nullptr;
								CXLinker<CIndividual*>* ndetIndiCandLinker = nullptr;
								CXNegLinker<CIndividualProcessNode*>* candIndiLinker = it.value();
								for (CXNegLinker<CIndividualProcessNode*>* candIndiLinkerIt = candIndiLinker; candIndiLinkerIt; candIndiLinkerIt = candIndiLinkerIt->getNext()) {
									CIndividualProcessNode* indiNode = candIndiLinkerIt->getData();
									CIndividual* candIndividual = indiNode->getNominalIndividual();
									if (candIndividual) {
										CXLinker<CIndividual*>* tmpLinker = CObjectAllocator< CXLinker<CIndividual*> >::allocateAndConstruct(tempMemAllocMan);
										tmpLinker->initLinker(candIndividual);
										bool nondeterministicallyAdded = candIndiLinkerIt->isNegated();
										if (nondeterministicallyAdded) {
											ndetIndiCandLinker = tmpLinker->append(ndetIndiCandLinker);
										} else {
											detIndiCandLinker = tmpLinker->append(detIndiCandLinker);
										}

										for (CConceptDescriptor* conDesIt = indiNode->getReapplyConceptLabelSet(false)->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
											if (!conDesIt->isNegated()) {
												bool nondeterministicallyMerged = true;
												CDependencyTrackPoint* depTrackPoint = conDesIt->getDependencyTrackPoint();
												if (depTrackPoint && depTrackPoint->getBranchingTag() <= 0) {
													nondeterministicallyMerged = false;
												}
												if (nondeterministicallyMerged) {
													CConcept* concept = conDesIt->getConcept();
													if (concept->getOperatorCode() == CCNOMINAL) {
														CIndividual* nomIndividual = concept->getNominalIndividual();
														if (nomIndividual != candIndividual) {
															CXLinker<CIndividual*>* nomTmpLinker = CObjectAllocator< CXLinker<CIndividual*> >::allocateAndConstruct(tempMemAllocMan);
															nomTmpLinker->initLinker(nomIndividual);
															ndetIndiCandLinker = nomTmpLinker->append(ndetIndiCandLinker);
														}
													}
												}
											}
										}
									}
								}

								CRealizationMarkerCandidatesMessageData* realMarkCandMessageData = CObjectAllocator<CRealizationMarkerCandidatesMessageData>::allocateAndConstruct(tempMemAllocMan);
								realMarkCandMessageData->initRealizationMarkerCandidatesMessageData(individual,markerConcept,detIndiCandLinker,ndetIndiCandLinker);
								realMessageData = realMarkCandMessageData->append(realMessageData);

							}

						}
						if (realMessageData) {
							realMessObserver->tellRealizationMessage(ontology,realMessageData,memPoolCon.takeMemoryPools());
						} else {
							taskMemMan->releaseTemporaryMemoryPools(memPoolCon.takeMemoryPools());
						}

						if (individual) {
							return true;
						}
					}
					return false;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
