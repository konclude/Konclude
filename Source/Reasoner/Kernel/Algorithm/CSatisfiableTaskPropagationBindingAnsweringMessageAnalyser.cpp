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

#include "CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser::CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser() {
				}



				bool CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser::readCalculationConfig(CCalculationConfigurationExtension *config) {					
					if (config) {
						return true;
					}
					return false;
				}


				CIndividualProcessNode* CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser::getCorrectedIndividualID(CIndividualProcessNode* baseIndiNode, CIndividualProcessNodeVector* indiNodeVec, bool* nondetMergedFlag) {
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

				



				CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser* CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser::appendVariableBindingsPropagation(CVariableBindingPath* varBindPath, CVARIABLEBINDINGMESSAGELIST<CVariable*>*& variablesList,
						CVARIABLEBINDINGMESSAGELIST<CAnsweringMessageDataVariableBindingPropagationsData*>*& individualBindingList, cint64 maxDetBranchTag, CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {

					if (!variablesList) {
						variablesList = CObjectParameterizingAllocator< CVARIABLEBINDINGMESSAGELIST<CVariable*>, CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan, mTmpContext);
						for (CVariableBindingDescriptor* varBindDesIt = varBindPath->getVariableBindingDescriptorLinker(); varBindDesIt; varBindDesIt = varBindDesIt->getNext()) {
							CVariableBinding* varBind = varBindDesIt->getVariableBinding();
							CVariable* variable = varBind->getBindedVariable();
							variablesList->append(variable);
						}
					}

					CProcessingDataBox* processingDataBox = statCalcTask->getProcessingDataBox();
					CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();



					CPROCESSINGLIST<CIndividualReference*>* nonDetMergedIndiBindingsVecList = nullptr;

					CIndividualReference* indiBindingsVec = CObjectAllocator< CIndividualReference >::allocateAndConstructArray(mTempMemAllocMan, variablesList->count());
					cint64 nextIndiBindingPos = 0;

					bool varNonDeterministicallyDerived = false;
					for (CVariableBindingDescriptor* varBindDesIt = varBindPath->getVariableBindingDescriptorLinker(); varBindDesIt; varBindDesIt = varBindDesIt->getNext()) {
						CVariableBinding* varBind = varBindDesIt->getVariableBinding();
						CVariable* variable = varBind->getBindedVariable();
						CIndividualProcessNode* bindedIndiNode = varBind->getBindedIndividual();
						bool nondeterMerged = false;
						bindedIndiNode = getCorrectedIndividualID(bindedIndiNode, indiNodeVec, &nondeterMerged);
						if (nondeterMerged) {
							varNonDeterministicallyDerived = true;
						}
						CIndividualReference indiRef = bindedIndiNode->getNominalIndividual();
						if (bindedIndiNode->getNominalIndividual() && bindedIndiNode->getNominalIndividual()->isTemporaryIndividual()) {
							indiRef = bindedIndiNode->getNominalIndividual()->getIndividualID();
						}
						if (nonDetMergedIndiBindingsVecList) {
							for (CPROCESSINGLIST<CIndividualReference*>::const_iterator it = nonDetMergedIndiBindingsVecList->constBegin(), itEnd = nonDetMergedIndiBindingsVecList->constEnd(); it != itEnd; ++it) {
								CIndividualReference* mergedIndiBindingsVec = (*it);
								mergedIndiBindingsVec[nextIndiBindingPos] = indiRef;
							}
						} else {
							indiBindingsVec[nextIndiBindingPos] = indiRef;
						}
						CDependencyTrackPoint* varDepTrackPoint = varBind->getDependencyTrackPoint();
						if (varDepTrackPoint) {
							if (varDepTrackPoint->getBranchingTag() > maxDetBranchTag) {
								varNonDeterministicallyDerived = true;
							}
						} else {
							varNonDeterministicallyDerived = true;
						}

						CIndividualMergingHash* indiMergHash = bindedIndiNode->getIndividualMergingHash(false);
						if (indiMergHash) {
							for (CIndividualMergingHash::const_iterator it = indiMergHash->constBegin(), itEnd = indiMergHash->constEnd(); it != itEnd; ++it) {
								if (it.value().isMergedWithIndividual()) {
									cint64 mergedIndiId = it.key();
									CDependencyTrackPoint* mergeDepTrackPoint = it.value().getDependencyTrackPoint();
									bool nondeterMerged = false;
									if (!mergeDepTrackPoint || mergeDepTrackPoint->getBranchingTag() > maxDetBranchTag) {
										nondeterMerged = true;
									}
									if (nondeterMerged) {
										if (!nonDetMergedIndiBindingsVecList) {
											nonDetMergedIndiBindingsVecList = CObjectParameterizingAllocator< CPROCESSINGLIST<CIndividualReference*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(), calcAlgContext->getUsedTaskProcessorContext());
											nonDetMergedIndiBindingsVecList->append(indiBindingsVec);
										}

										CPROCESSINGLIST<CIndividualReference*>::iterator it = nonDetMergedIndiBindingsVecList->begin();
										for (cint64 i = nonDetMergedIndiBindingsVecList->count(); i > 0; --i) {
											CIndividualReference* nextIndiBindingsVec = *it;
											++it;
											CIndividualReference* newIndiBindingsVec = CObjectAllocator< CIndividualReference >::allocateAndConstructArray(mTempMemAllocMan, variablesList->count());
											for (cint64 i = 0; i < nextIndiBindingPos; ++i) {
												newIndiBindingsVec[i] = nextIndiBindingsVec[i];
											}
											CIndividualProcessNode* mergedIndiNode = indiNodeVec->getData(-mergedIndiId);
											CIndividualReference mergedIndiRef = mergedIndiNode->getNominalIndividual();
											if (mergedIndiNode->getNominalIndividual() && mergedIndiNode->getNominalIndividual()->isTemporaryIndividual()) {
												mergedIndiRef = mergedIndiNode->getNominalIndividual()->getIndividualID();
											}
											newIndiBindingsVec[nextIndiBindingPos] = mergedIndiRef;
											nonDetMergedIndiBindingsVecList->append(newIndiBindingsVec);
										}
									}
								}
							}
						}
						nextIndiBindingPos++;

					}

					CAnsweringMessageDataVariableBindingPropagationsData* dataVariableBindingPropagationData = CObjectParameterizingAllocator< CAnsweringMessageDataVariableBindingPropagationsData, CContext* >::allocateAndConstruct(mTempMemAllocMan);
					dataVariableBindingPropagationData->initBindingPropagationsData(varNonDeterministicallyDerived, indiBindingsVec);
					if (!individualBindingList) {
						individualBindingList = CObjectParameterizingAllocator< CVARIABLEBINDINGMESSAGELIST<CAnsweringMessageDataVariableBindingPropagationsData*>, CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan, mTmpContext);
					}
					individualBindingList->append(dataVariableBindingPropagationData);


					if (nonDetMergedIndiBindingsVecList) {
						CPROCESSINGLIST<CIndividualReference*>::const_iterator it = nonDetMergedIndiBindingsVecList->constBegin(), itEnd = nonDetMergedIndiBindingsVecList->constEnd();
						for (++it; it != itEnd; ++it) {
							CIndividualReference* mergedIndiBindingsVec = (*it);
							CAnsweringMessageDataVariableBindingPropagationsData* mergedDataVariableBindingPropagationData = CObjectParameterizingAllocator< CAnsweringMessageDataVariableBindingPropagationsData, CContext* >::allocateAndConstruct(mTempMemAllocMan);
							mergedDataVariableBindingPropagationData->initBindingPropagationsData(true, mergedIndiBindingsVec);
							individualBindingList->append(mergedDataVariableBindingPropagationData);
						}
					}

					return this;
				}





				bool CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					mStatCalcTask = statCalcTask;
					CSatisfiableTaskAnswererBindingPropagationAdapter* answererConceptMarkedAdapter = statCalcTask->getSatisfiableAnswererBindingPropagationAdapter();
					if (answererConceptMarkedAdapter) {
						CAnsweringMessageDataObserver* answererMessPropagationObserver = answererConceptMarkedAdapter->getAnswererMessageDataObserver();
						CAnsweringMessageData* subsumMessageDataLinker = nullptr;
						CAnsweringMessageData* possSubsumMessageDataLinker = nullptr;


						CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
						CTaskHandleMemoryAllocationManager* taskMemMan = calcAlgContext->getTemporaryMemoryAllocationManager();
						CConcept* testingConcept = answererConceptMarkedAdapter->getTestingConcept();
						CConcept* finalPropagationConcept = answererConceptMarkedAdapter->getPropagationFinalizationConcept();



						CAnsweringHandler* answeringHandler = answererConceptMarkedAdapter->getAnsweringHandler();
						CConcreteOntology* testOntology = answererConceptMarkedAdapter->getTestingOntology();
						CProcessingDataBox* processingDataBox = statCalcTask->getProcessingDataBox();
						CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();

						cint64 maxDetBranchTag = processingDataBox->getMaximumDeterministicBranchTag();


						CMarkerIndividualNodeHash* markerIndiNodeHash = statCalcTask->getProcessingDataBox()->getMarkerIndividualNodeHash(false);


						if (testOntology && testingConcept && finalPropagationConcept && markerIndiNodeHash) {

							CXNegLinker<CIndividualProcessNode*>* indiProcNodeLinker = markerIndiNodeHash->getMarkerIndividualNodeLinker(finalPropagationConcept);



							CMemoryPoolContainer memPoolCon;
							CTaskMemoryPoolAllocationManager classMessMemManCreaterMemMan(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
							mTempMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&classMessMemManCreaterMemMan);
							mTempMemAllocMan->initTaskMemoryPoolAllocationManager(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
							mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTempMemAllocMan);

							CVARIABLEBINDINGMESSAGELIST<CVariable*>* variablesList = nullptr;
							CVARIABLEBINDINGMESSAGELIST<CAnsweringMessageDataVariableBindingPropagationsData*>* individualBindingList = nullptr;
							cint64 indiCount = 0;
							
							for (CXNegLinker<CIndividualProcessNode*>* indiProcNodeLinkerIt = indiProcNodeLinker; indiProcNodeLinkerIt; indiProcNodeLinkerIt = indiProcNodeLinkerIt->getNext()) {
								++indiCount;
								CIndividualProcessNode* indiProcNode = indiProcNodeLinkerIt->getData();
								bool nonDeterministic = indiProcNodeLinkerIt->isNegated();

								indiProcNode = getCorrectedIndividualID(indiProcNode, indiNodeVec, &nonDeterministic);
								CConceptVariableBindingPathSetHash* conVarBindPathSetHash = indiProcNode->getConceptVariableBindingPathSetHash(false);
								if (conVarBindPathSetHash) {
									CVariableBindingPathSet* varBindPathSet = conVarBindPathSetHash->getVariableBindingPathSet(finalPropagationConcept, false);

									CVariableBindingPathMap* varBindPathMap = varBindPathSet->getVariableBindingPathMap();
									for (CVariableBindingPathMap::iterator it = varBindPathMap->begin(), itEnd = varBindPathMap->end(); it != itEnd; ++it) {
										CVariableBindingPathMapData& varBindPathData = it.value();
										CVariableBindingPathDescriptor* varBindPathDes = varBindPathData.getVariableBindingPathDescriptor();
										CVariableBindingPath* varBindPath = varBindPathDes->getVariableBindingPath();

										appendVariableBindingsPropagation(varBindPath, variablesList, individualBindingList, maxDetBranchTag, statCalcTask, calcAlgContext);

									}
								}



								CConceptRepresentativePropagationSetHash* repPropSetHash = indiProcNode->getConceptRepresentativePropagationSetHash(false);
								if (repPropSetHash) {
									CRepresentativePropagationSet* repPropSet = repPropSetHash->getRepresentativePropagationSet(finalPropagationConcept, false);


									if (repPropSet) {

										CRepresentativePropagationDescriptor* outRepPropDes = repPropSet->getOutgoingRepresentativePropagationDescriptorLinker();
										if (outRepPropDes) {
											CRepresentativeVariableBindingPathSetData* repVarBindPathSetData = outRepPropDes->getRepresentativeVariableBindingPathSetData();
											if (repVarBindPathSetData) {
												CRepresentativeVariableBindingPathSetMigrateData* repVarBindPathSetMigData = repVarBindPathSetData->getMigrateData(false);
												if (repVarBindPathSetMigData) {
													CRepresentativeVariableBindingPathMap* repVarBindPathSetMap = repVarBindPathSetMigData->getRepresentativeVariableBindingPathMap();

													for (CRepresentativeVariableBindingPathMap::const_iterator it = repVarBindPathSetMap->constBegin(), itEnd = repVarBindPathSetMap->constEnd(); it != itEnd; ++it) {
														const CRepresentativeVariableBindingPathMapData& varBindPathData = it.value();
														CVariableBindingPath* varBindPath = varBindPathData.getVariableBindingPath();

														appendVariableBindingsPropagation(varBindPath, variablesList, individualBindingList, maxDetBranchTag, statCalcTask, calcAlgContext);
													}
												}
											}
										}
									}
								}
							}

							CAnsweringMessageDataVariableBindingPropagations* variableBindingPropagationData = CObjectAllocator<CAnsweringMessageDataVariableBindingPropagations>::allocateAndConstruct(mTempMemAllocMan);
							variableBindingPropagationData->initAnsweringVariableBindingMessageData(finalPropagationConcept, variablesList, individualBindingList);
							answererMessPropagationObserver->sendMessage(answeringHandler, variableBindingPropagationData, memPoolCon.takeMemoryPools());

							return true;

						}
					}
					return false;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
