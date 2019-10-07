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

#include "CSatisfiableTaskPropertyClassificationMessageAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CSatisfiableTaskPropertyClassificationMessageAnalyser::CSatisfiableTaskPropertyClassificationMessageAnalyser() {
				}



				bool CSatisfiableTaskPropertyClassificationMessageAnalyser::readCalculationConfig(CCalculationConfigurationExtension *config) {					
					if (config) {
						return true;
					}
					return false;
				}


				CIndividualProcessNode* CSatisfiableTaskPropertyClassificationMessageAnalyser::getCorrectedIndividualID(CIndividualProcessNode* baseIndiNode, CIndividualProcessNodeVector* indiNodeVec, bool* nondetMergedFlag) {
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



				CClassificationMessageData* CSatisfiableTaskPropertyClassificationMessageAnalyser::extractPossibleSubsumptionInformation(CRole* testingRole, CIndividualProcessNode* indiNode, CReapplyConceptLabelSet* conSet, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(ANALYSEPOSSIBLESUBSUMPTIONSCOUNT,calcAlgContext);

					CClassificationMessageData* messageDataLinker = nullptr;


					// initialize possible subsumption list
					CPROPERTYPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossiblePropertySubsumptionData*>* possSubsumerList = nullptr;


					CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true,true,false);
				
					// create new initialization list for possible subsumers
					while (conSetIt.hasNext()) {
						CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
						CConcept* concept = conDes->getConcept();
						bool negated = conDes->getNegation();


						CRole* markerRole = concept->getRole();
						if (!negated && concept->getOperatorCode() == CCMARKER && markerRole && markerRole != testingRole) {
							if (!possSubsumerList) {
								possSubsumerList = CObjectParameterizingAllocator< CPROPERTYPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossiblePropertySubsumptionData*>,CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTmpContext);
							}
							CClassificationInitializePossiblePropertySubsumptionData* possSubsumData = CObjectAllocator< CClassificationInitializePossiblePropertySubsumptionData >::allocateAndConstruct(mTempMemAllocMan);
							possSubsumData->initClassificationPossibleSubsumptionData(markerRole);
							possSubsumerList->append(possSubsumData);
						}
						conSetIt.moveNext();
					}



					CClassificationInitializePossiblePropertySubsumptionMessageData* possSubsumMessageData = CObjectAllocator<CClassificationInitializePossiblePropertySubsumptionMessageData>::allocateAndConstruct(mTempMemAllocMan);
					possSubsumMessageData->initClassificationPossibleSubsumptionMessageData(testingRole,possSubsumerList);
					messageDataLinker = possSubsumMessageData->append(messageDataLinker);
					return messageDataLinker;
				}




				bool CSatisfiableTaskPropertyClassificationMessageAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					mStatCalcTask = statCalcTask;
					CSatisfiableTaskClassificationRoleMarkedMessageAdapter* classMessRoleMarkedAdapter = statCalcTask->getSatisfiableClassificationRoleMarkedMessageAdapter();
					if (classMessRoleMarkedAdapter) {
						CClassificationMessageDataObserver* roleMessClassObserver = classMessRoleMarkedAdapter->getRoleClassificationMessageDataObserver();
						CClassificationMessageData* subsumMessageDataLinker = nullptr;
						CClassificationMessageData* possSubsumMessageDataLinker = nullptr;
						CClassificationMessageData* pmMessageDataLinker = nullptr;


						CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
						CTaskHandleMemoryAllocationManager* taskMemMan = calcAlgContext->getTemporaryMemoryAllocationManager();
						CRole* testingRole = classMessRoleMarkedAdapter->getTestingRole();
						CIndividual* markerIndi = classMessRoleMarkedAdapter->getMarkerIndividual();

						//QString iriClassNameString = CIRIName::getRecentIRIName(testingConcept->getClassNameLinker());
						//if (iriClassNameString == "http://www.owllink.org/testsuite/galen#LowerLobeOfLeftLung") {
						//	bool bug = true;
						//}
						//} 
						//considerOtherNode = false;

						CConcreteOntology* testOntology = classMessRoleMarkedAdapter->getTestingOntology();
						CProcessingDataBox* processingDataBox = statCalcTask->getProcessingDataBox();
						CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();

						cint64 maxDetBranchTag = processingDataBox->getMaximumDeterministicBranchTag();




						if (testOntology && testingRole && markerIndi) {
							cint64 markerIndiID = markerIndi->getIndividualID();
							CIndividualProcessNode* markerIndiNode = indiNodeVec->getData(-markerIndiID);
							bool nondetMerged = false;
							markerIndiNode = getCorrectedIndividualID(markerIndiNode,indiNodeVec,&nondetMerged);
							if (nondetMerged) {
								maxDetBranchTag = 0;
							}


							CMemoryPoolContainer memPoolCon;
							CTaskMemoryPoolAllocationManager classMessMemManCreaterMemMan(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
							mTempMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&classMessMemManCreaterMemMan);
							mTempMemAllocMan->initTaskMemoryPoolAllocationManager(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
							mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTempMemAllocMan);

							CPROPERTYSUBSUMPTIONMESSAGELIST<CRole*>* subsumerList = nullptr;
							
							CReapplyConceptLabelSet* conSet = markerIndiNode->getReapplyConceptLabelSet(false);
							if (conSet) {
								CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true,true,true);
								while (conSetIt.hasNext()) {
									CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
									if (conDes) {
										CConcept* concept = conDes->getConcept();
										bool negated = conDes->getNegation();
										CRole* markerRole = concept->getRole();

										if (!negated && concept->getOperatorCode() == CCMARKER && markerRole) {
											CDependencyTrackPoint* depTrackPoint = conSetIt.getDependencyTrackPoint();
											bool deterministic = false;
											if (depTrackPoint) {
												if (depTrackPoint->getBranchingTag() <= maxDetBranchTag) {
													deterministic = true;
												}
											}

											if (deterministic && markerRole != testingRole) {
												if (!subsumerList) {
													subsumerList = CObjectParameterizingAllocator< CPROPERTYSUBSUMPTIONMESSAGELIST<CRole*>,CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTmpContext);
												}
												subsumerList->append(markerRole);
											}
										}
									}
									conSetIt.moveNext();
								}

								CClassificationMessageData* messageData = extractPossibleSubsumptionInformation(testingRole,markerIndiNode,conSet,calcAlgContext);
								if (messageData) {
									possSubsumMessageDataLinker = messageData->append(possSubsumMessageDataLinker);
								}

							}

							CClassificationPropertySubsumptionMessageData* subsumMessageData = CObjectAllocator<CClassificationPropertySubsumptionMessageData>::allocateAndConstruct(mTempMemAllocMan);
							subsumMessageData->initClassificationSubsumptionMessageData(testingRole,subsumerList);
							subsumMessageDataLinker = subsumMessageData->append(subsumMessageDataLinker);
							subsumerList = nullptr;





							CClassificationMessageData* messageDataLinker = subsumMessageDataLinker;
							if (pmMessageDataLinker) {
								messageDataLinker = pmMessageDataLinker->append(messageDataLinker);
							}
							if (possSubsumMessageDataLinker) {
								messageDataLinker = possSubsumMessageDataLinker->append(messageDataLinker);
							}

							if (messageDataLinker) {
								roleMessClassObserver->tellClassificationMessage(testOntology,messageDataLinker,memPoolCon.takeMemoryPools());
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
