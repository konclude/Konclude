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

#include "CCalculationTableauSaturationTaskHandleAlgorithm.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CCalculationTableauSaturationTaskHandleAlgorithm::CCalculationTableauSaturationTaskHandleAlgorithm() {
					mProcessingDataBox = nullptr;
					mCalcAlgContext = nullptr;


					mConceptPriorityStrategy = nullptr;
					mTaskProcessingStrategy = nullptr;
					mUnsatCachRetStrategy = nullptr;
					mProcessingDataBox = nullptr;
					mCalcAlgContext = nullptr;

					mClashDesFactory = nullptr;
					mIndiNodeManager = nullptr;
					mDependencyFactory = nullptr;
					mSatExpCacheHandler = nullptr;
					mUnsatCacheHandler = nullptr;

					mCompGraphCacheHandler = nullptr;
					mIndividualPriorityStrategy = nullptr;



					mPosJumpFuncVec = &mPosTableauRuleJumpFuncVec[mRuleFuncCount/2];
					mNegJumpFuncVec = &mNegTableauRuleJumpFuncVec[mRuleFuncCount/2];

					for (cint64 i = 0; i < mRuleFuncCount; ++i) {
						mPosTableauRuleJumpFuncVec[i] = nullptr;
						mNegTableauRuleJumpFuncVec[i] = nullptr;
					}

					mConfAncestorBlockingSearch = true;


					mPosJumpFuncVec[CCTOP] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCAND] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCAQAND] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCIMPLAQAND] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCBRANCHAQAND] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCSUB] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCALL] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCAQALL] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCSOME] = &CCalculationTableauSaturationTaskHandleAlgorithm::applySOMERule;
					mPosJumpFuncVec[CCAQSOME] = &CCalculationTableauSaturationTaskHandleAlgorithm::applySOMERule;
					mPosJumpFuncVec[CCAQCHOOCE] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyAutomatChooseRule;
					mPosJumpFuncVec[CCIMPL] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyIMPLICATIONRule;
					mPosJumpFuncVec[CCIMPLTRIG] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCIMPLALL] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCBRANCHALL] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCBRANCHAQALL] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCIMPLAQALL] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyALLRule;

					mNegJumpFuncVec[CCOR] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyANDRule;
					mNegJumpFuncVec[CCALL] = &CCalculationTableauSaturationTaskHandleAlgorithm::applySOMERule;
					mNegJumpFuncVec[CCSOME] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyALLRule;
					mNegJumpFuncVec[CCAQCHOOCE] = &CCalculationTableauSaturationTaskHandleAlgorithm::applyAutomatChooseRule;


					mLastConfig = nullptr;

					mAppliedALLRuleCount = 0;
					mAppliedSOMERuleCount = 0;
					mAppliedANDRuleCount = 0;
					mAppliedORRuleCount = 0;
					mAppliedATLEASTRuleCount = 0;
					mAppliedATMOSTRuleCount = 0;
					mAppliedTOTALRuleCount = 0;

					mAddedALLConcepts = 0;
					mAddedSOMEConcepts = 0;
					mAddedIMPLConcepts = 0;
					mAddedTRIGGConcepts = 0;
					mAddedSUBConcepts = 0;
					mAddedELSEConcepts = 0;
					mSubstituitedIndiNodeCount = 0;
				}

				CCalculationTableauSaturationTaskHandleAlgorithm::~CCalculationTableauSaturationTaskHandleAlgorithm() {
				}

				void CCalculationTableauSaturationTaskHandleAlgorithm::readCalculationConfig(CSatisfiableCalculationTask* satCalcTask) {
					CCalculationConfigurationExtension *config = satCalcTask->getCalculationConfiguration();
					if (config != mLastConfig) {
						if (config) {
							mConfAncestorBlockingSearch = config->isAncestorBlockingSearchActivated();
						} else {
							mConfAncestorBlockingSearch = true;
						}
						mLastConfig = config;
					}

				}


				CCalculationAlgorithmContextBase* CCalculationTableauSaturationTaskHandleAlgorithm::createCalculationAlgorithmContext(CTaskProcessorContext *processorContext, CProcessContext* processContext, CSatisfiableCalculationTask* satCalcTask) {
					CCalculationAlgorithmContextBase* calcAlgContext = CObjectAllocator< CCalculationAlgorithmContextBase >::allocateAndConstruct(processContext->getUsedMemoryAllocationManager());
					calcAlgContext->initTaskProcessContext(processContext,satCalcTask);
					calcAlgContext->initCalculationAlgorithmContext(processorContext,mConceptPriorityStrategy,mIndividualPriorityStrategy,mTaskProcessingStrategy,mUnsatCachRetStrategy,
								mIndiNodeManager,mClashDesFactory,mDependencyFactory,mUnsatCacheHandler,mSatExpCacheHandler);
					return calcAlgContext;
				}



				bool CCalculationTableauSaturationTaskHandleAlgorithm::handleTask(CTaskProcessorContext *processorContext, CTask* task) {

					//lastBranchingMergingProcRest = nullptr;

					CTaskHandleMemoryAllocationManager* processorMemoryManager = processorContext->getTaskHandleMemoryAllocationManager();
					CTaskProcessorCommunicator* processorCommunicator = processorContext->getTaskProcessorCommunicator();

					CTaskHandleMemoryAllocationManager* taskHandleMemMan = processorContext->getTaskHandleMemoryAllocationManager();
					taskHandleMemMan->releaseAllMemory();

					CSatisfiableCalculationTask* satCalcTask = dynamic_cast<CSatisfiableCalculationTask*>(task);
					if (satCalcTask) {

						if (!processorCommunicator->verifyContinueTaskProcessing(satCalcTask)) {
							if (!satCalcTask->getTaskStatus()->isProcessable()) {
								processorCommunicator->communicateTaskComplete(satCalcTask);
								return false;
							} else {
								// continue later
								return true;
							}
						}

						readCalculationConfig(satCalcTask);


						CProcessContext* processContext = satCalcTask->getProcessContext(processorContext);
						CCalculationAlgorithmContextBase* calcAlgContext = createCalculationAlgorithmContext(processorContext,processContext,satCalcTask);
						mCalcAlgContext = calcAlgContext;
						mProcessingDataBox = satCalcTask->getProcessingDataBox();
						bool clashed = false;
						bool satisfiable = false;
						bool completed = false;
						bool paused = false;
						bool error = false;
						cint64 errorCode = 0;

						CProcessTagger* processTagger = calcAlgContext->getUsedProcessTagger();

						CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();

						CNodeSwitchHistory* nodeSwitchHistory = processingDataBox->getNodeSwitchHistory(true);

						cint64 indiProcessedCount = 0;


						CIndividualProcessNode* indiProcNode = nullptr;
						CConceptProcessDescriptor* conProcDes = nullptr;

						cint64 processRuleToTaskProcessingVerificationCount = 10;
						cint64 remainProcessRuleToTaskProcessingVerification = processRuleToTaskProcessingVerificationCount;

						CIndividualProcessNodeLinker* indiProcessNodeLinker = mProcessingDataBox->takeIndividualProcessNodeLinker();
						CIndividualProcessNode* lastIndiProcNode = nullptr;
						try {
							STATINC(TASKPROCESSCHANGECOUNT,mCalcAlgContext);

							while (indiProcessNodeLinker) {

								CIndividualProcessNode* indiProcNode = indiProcessNodeLinker->getData();
								lastIndiProcNode = indiProcNode;
								STATINC(INDIVIDUALNODESWITCHCOUNT,mCalcAlgContext);
								++indiProcessedCount;

								if (individualNodeInitializing(indiProcNode,calcAlgContext)) {

									CConceptProcessLinker* conceptProcessLinker = indiProcNode->takeConceptProcessLinker();
									while (conceptProcessLinker) {
										STATINC(RULEAPPLICATIONCOUNT,mCalcAlgContext);
										KONCLUCE_TASK_ALGORITHM_SATURATION_MODEL_STRING_INSTRUCTION(mRuleBeginDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
										applyTableauSaturationRule(indiProcNode,conceptProcessLinker);
										KONCLUCE_TASK_ALGORITHM_SATURATION_MODEL_STRING_INSTRUCTION(mRuleEndDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
										conceptProcessLinker = indiProcNode->takeConceptProcessLinker();
									}
								}
								indiProcessNodeLinker->clearProcessingQueued();
								individualNodeConclusion(indiProcNode,calcAlgContext);
								indiProcessNodeLinker = mProcessingDataBox->takeIndividualProcessNodeLinker();
							}

							//mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
							//QFile tmpFile("tmp1.txt");
							//if (tmpFile.open(QIODevice::WriteOnly)) {
							//	tmpFile.write(mEndSaturationDebugIndiModelString.toLocal8Bit());
							//	tmpFile.close();
							//}
							satisfiable = true;

						} catch (const CCalculationErrorProcessingException& calcErrorProcException) {
							if (calcErrorProcException.hasError()) {
								error = true;
								errorCode = (cint64)calcErrorProcException.getErrorCode();
							}
						} catch (const CMemoryAllocationException& memAllocException) {
							error = true;
							errorCode = 2;
						} catch (...) {
							error = true;
							errorCode = 3;
						}



						CBooleanTaskResult* satResult = satCalcTask->getSatisfiableCalculationTaskResult();
						if (clashed) {
							if (!satResult->hasResult()) {
								satResult->installResult(false);
							}
							completed = true;
						}
						if (satisfiable) {
							STATINC(ROOTTASKSATISFIABLECOUNT,calcAlgContext);

							mSubsumAnalyser.analyseSatisfiableTask(satCalcTask,calcAlgContext);

							satResult->installResult(true);
							completed = true;
						}

						if (error) {
							satCalcTask->getTaskStatus()->setError(error,errorCode);
							processorCommunicator->communicateTaskError(satCalcTask);
							return false;
						}

						if (completed) {
							processorCommunicator->communicateTaskComplete(satCalcTask);
							return false;
						} 
						return true;

					}

					return false;
				}



				bool CCalculationTableauSaturationTaskHandleAlgorithm::individualNodeInitializing(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptAssertionLinker* assertionConceptLinkerIt = indiProcNode->getAssertionConceptLinkerIt();
					CXSortedNegLinker<CConcept*>* initConceptLinkerIt = indiProcNode->getInitializingConceptLinkerIt();
					CReapplyConceptLabelSet* conSet = indiProcNode->getReapplyConceptLabelSet(false);
					if (assertionConceptLinkerIt || initConceptLinkerIt || !conSet) {
						CConcept* topConcept = calcAlgContext->getProcessingDataBox()->getOntologyTopConcept();
						addConceptToIndividual(topConcept,false,indiProcNode,false,calcAlgContext);
					}
					if (assertionConceptLinkerIt) {
						addConceptsToIndividual(assertionConceptLinkerIt,false,indiProcNode,false,calcAlgContext);
						indiProcNode->clearAssertionConcepts();
					}
					if (initConceptLinkerIt) {
						initializeInitializationConcepts(indiProcNode,initConceptLinkerIt,calcAlgContext);
						indiProcNode->clearInitializingConcepts();
					}
					return true;
				}


				void CCalculationTableauSaturationTaskHandleAlgorithm::initializeInitializationConcepts(CIndividualProcessNode*& indiProcNode, CXSortedNegLinker<CConcept*>* initConceptLinkerIt, CCalculationAlgorithmContextBase* calcAlgContext) {
					addConceptsToIndividual(initConceptLinkerIt,false,indiProcNode,true,calcAlgContext);

					CXSortedNegLinker<CConcept*>* initConceptLinkerIt3 = initConceptLinkerIt;
					bool requiredBackProp = false;
					while (initConceptLinkerIt3 && !requiredBackProp) {
						CConcept* concept = initConceptLinkerIt3->getData();

						if (!concept->hasClassName()) {
							requiredBackProp = true;
						} else {
							CConceptData* conceptData = concept->getConceptData();
							if (conceptData) {
								CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
								CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
								if (conRefLinking) {
									CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
									CClassificationConceptReferenceLinking* classSatCalcRefLinkData = confSatRefLinkingData->getClassifierReferenceLinkingData();
									if (classSatCalcRefLinkData) {
										requiredBackProp = classSatCalcRefLinkData->isPotentiallyExistInitializationConcept();
									}
								}
							}
						}

						initConceptLinkerIt3 = initConceptLinkerIt3->getNext();
					}
					indiProcNode->setRequiredBackwardPropagation(requiredBackProp);
					CIndividualProcessNode* substituiteIndiNode = nullptr;
					if (!requiredBackProp) {
						CXSortedNegLinker<CConcept*>* initConceptLinkerIt2 = initConceptLinkerIt;
						if (initConceptLinkerIt2) {
							CConcept* concept = initConceptLinkerIt2->getData();

							if (concept->hasClassName() && !initConceptLinkerIt2->hasNext()) {
								cint64 opCode = concept->getOperatorCode();
								cint64 opCount = concept->getOperandCount();
								if (opCode == CCSUB && opCount == 1) {
									CSortedNegLinker<CConcept*>* opConLinker = concept->getOperandList();
									CConcept* opConcept = opConLinker->getData();
									bool opConNeg = opConLinker->isNegated();
									if (opConcept->getOperatorCode() == CCSUB && !opConNeg && opConcept->hasClassName()) {

										CConceptData* conceptData = opConcept->getConceptData();
										if (conceptData) {
											CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
											CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
											if (conRefLinking) {
												CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
												CClassificationConceptReferenceLinking* classSatCalcRefLinkData = confSatRefLinkingData->getClassifierReferenceLinkingData();
												if (classSatCalcRefLinkData) {
													substituiteIndiNode = (CIndividualProcessNode*)classSatCalcRefLinkData->getIndividualProcessNodeForConcept();
													if (substituiteIndiNode->getIndividualID() <= indiProcNode->getIndividualID()) {
														substituiteIndiNode = nullptr;
													}
												}
											}
										}

									}
								}
							}

							if (substituiteIndiNode) {
								CIndividualProcessNode* blockedIndiNode = substituiteIndiNode;
								while (blockedIndiNode->hasSubstituteIndividualNode()) {
									blockedIndiNode = blockedIndiNode->getSubstituteIndividualNode();
								}
								CReapplyConceptLabelSet* blockConSet = blockedIndiNode->getReapplyConceptLabelSet(false);
								CConceptDescriptor* conceptDescriptor = nullptr;
								CCondensedReapplyQueue* conReapplyQueue = blockConSet->getConceptDescriptorAndReapplyQueue(concept,conceptDescriptor,false);
								if (!conceptDescriptor) {
									if (conReapplyQueue && !conReapplyQueue->isEmpty()) {
										substituiteIndiNode = nullptr;
									}
								} else {
									substituiteIndiNode = nullptr;
								}
							}
						}
					}
					if (substituiteIndiNode) {
						++mSubstituitedIndiNodeCount;
						indiProcNode->clearConceptProcessLinker();
						indiProcNode->setSubstituteIndividualNode(substituiteIndiNode);
					}
				}


				void CCalculationTableauSaturationTaskHandleAlgorithm::individualNodeConclusion(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptProcessLinker* conProLinker = indiProcNode->getConceptProcessLinker();
					if (conProLinker) {
						addIndividualToProcessingQueue(indiProcNode,calcAlgContext);
					}
				}



				void CCalculationTableauSaturationTaskHandleAlgorithm::applyTableauSaturationRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker) {
					CConceptDescriptor* conDes = conProLinker->getConceptDescriptor();
					bool conNeg = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					cint64 conOpCode = concept->getOperatorCode();

					TableauRuleFunction func = nullptr;
					if (!conNeg) {
						func = mPosJumpFuncVec[conOpCode];
						if (func) {
							(this->*func)(processIndi,conProLinker);
						}
					} else {
						func = mNegJumpFuncVec[conOpCode];
						if (func) {
							(this->*func)(processIndi,conProLinker);
						}
					}
				}


				void CCalculationTableauSaturationTaskHandleAlgorithm::applyAutomatChooseRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker) {
					STATINC(AUTOMATEINITCOUNT,mCalcAlgContext);
					CConceptDescriptor* conDes = conProLinker->getConceptDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					while (conceptOpLinkerIt) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opNegation = conceptOpLinkerIt->isNegated();
						if (opNegation == conNegation) {
							addConceptToIndividual(opConcept,false,processIndi,false,mCalcAlgContext);
						}
						conceptOpLinkerIt = conceptOpLinkerIt->getNext();
					}
				}


				void CCalculationTableauSaturationTaskHandleAlgorithm::applyANDRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker) {
					STATINC(ANDRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptDescriptor* conDes = conProLinker->getConceptDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					addConceptsToIndividual(conceptOpLinkerIt,conNegation,processIndi,false,mCalcAlgContext);
				}


				void CCalculationTableauSaturationTaskHandleAlgorithm::applyALLRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker) {
					STATINC(ALLRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptDescriptor* conDes = conProLinker->getConceptDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();

					CMemoryAllocationManager* taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CRoleBackwardPropagationHash* backPropHash = processIndi->getRoleBackwardPropagationHash(true);
					CBackwardPropagationReapplyDescriptor* backPropReapplyDes = CObjectAllocator<CBackwardPropagationReapplyDescriptor>::allocateAndConstruct(taskMemMan);
					backPropReapplyDes->initBackwardPropagationReapplyDescriptor(conDes);
					CBackwardPropagationLink* backPropLinkIt = backPropHash->addBackwardPropagationConceptDescriptor(role,backPropReapplyDes);
					
					if (backPropLinkIt) {
						while (backPropLinkIt) {
							CIndividualProcessNode* backPropIndiNode = backPropLinkIt->getSourceIndividual();
							CReapplyConceptLabelSet* conSet = backPropIndiNode->getReapplyConceptLabelSet(true);

							CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
							while (conceptOpLinkerIt) {
								CConcept* opConcept = conceptOpLinkerIt->getData();
								bool opConNegation = conceptOpLinkerIt->isNegated()^conNegation;
								//if (!conSet->containsConcept(opConcept,opConNegation)) {
									STATINC(ALLROLERESTRICTIONCOUNT,mCalcAlgContext);
									addConceptToIndividual(opConcept,opConNegation,backPropIndiNode,false,mCalcAlgContext);
								//}
								conceptOpLinkerIt = conceptOpLinkerIt->getNext();
							}

							backPropLinkIt = backPropLinkIt->getNext();
						}
					}
				}



				void CCalculationTableauSaturationTaskHandleAlgorithm::applySOMERule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker) {
					STATINC(SOMERULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptDescriptor* conDes = conProLinker->getConceptDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					while (conceptOpLinkerIt) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opConNegation = conceptOpLinkerIt->isNegated()^conNegation;
						CConceptData* conceptData = opConcept->getConceptData();
						KONCLUDE_ASSERT_X(conceptData,"some saturation rule","concept data missing");
						if (conceptData) {
							CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
							CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
							KONCLUDE_ASSERT_X(conRefLinking,"some saturation rule","concept reference linking data missing");
							if (conRefLinking) {
								CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
								CClassificationConceptReferenceLinking* classSatCalcRefLinkData = confSatRefLinkingData->getClassifierReferenceLinkingData();
								if (classSatCalcRefLinkData) {
									CIndividualProcessNode* existIndiNode = (CIndividualProcessNode*)classSatCalcRefLinkData->getIndividualProcessNodeForConcept();
									KONCLUDE_ASSERT_X(existIndiNode,"some saturation rule","concept reference linked individual node missing");
									if (existIndiNode) {
										CMemoryAllocationManager* taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
										CRoleBackwardPropagationHash* backPropHash = existIndiNode->getRoleBackwardPropagationHash(true);
										CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
										while (superRoleIt) {
											if (superRoleIt->isNegated()) {
												CRole* superRole = superRoleIt->getData();
												CBackwardPropagationLink* backPropLink = CObjectAllocator<CBackwardPropagationLink>::allocateAndConstruct(taskMemMan);
												backPropLink->initBackwardPropagationLink(processIndi,superRole);
												CBackwardPropagationReapplyDescriptor* backPropReapplyDes = backPropHash->addBackwardPropagationLink(superRole,backPropLink);
												if (backPropReapplyDes) {
													applyBackwardPropagationConcepts(processIndi,backPropReapplyDes,mCalcAlgContext);
												}
											}
											superRoleIt = superRoleIt->getNext();
										}
									}
								}
							}
						}
						conceptOpLinkerIt = conceptOpLinkerIt->getNext();
					}
				}




				void CCalculationTableauSaturationTaskHandleAlgorithm::applyIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker) {
					STATINC(IMPLICATIONRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptDescriptor* conDes = conProLinker->getConceptDescriptor();
					CConcept* concept = conDes->getConcept();
					CProcessingRestrictionSpecification* procRest = conProLinker->getProcessingRestriction();
					cint64 opCount = concept->getOperandCount();
					CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
					CTriggeredImplicationProcessingRestrictionSpecification* triggImpProcRes = nullptr;
					CMemoryAllocationManager* taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					if (!procRest) {
						if (opCount > 1) {
							triggImpProcRes = CObjectAllocator< CTriggeredImplicationProcessingRestrictionSpecification >::allocateAndConstruct(taskMemMan);
							triggImpProcRes->setConceptImplicationTrigger(opLinker->getNext());
						}
					} else {
						triggImpProcRes = (CTriggeredImplicationProcessingRestrictionSpecification*)procRest;
					}

					// search next not existing trigger
					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(true);
					while (triggImpProcRes && triggImpProcRes->hasConceptImplicationTrigger()) {
						CSortedNegLinker<CConcept*>* nextTrigger = triggImpProcRes->getConceptImplicationTrigger();
						triggImpProcRes->setConceptImplicationTrigger(nextTrigger->getNext());
						CConcept* triggerConcept = nextTrigger->getData();
						CConceptDescriptor* triggerConDes = nullptr;
						CCondensedReapplyQueue* reapplyQueue = conSet->getConceptDescriptorAndReapplyQueue(triggerConcept,triggerConDes,true);
						if (triggerConDes) {
							if (triggerConDes->isNegated() == nextTrigger->isNegated()) {
								return;
							}
						} else {
							STATINC(IMPLICATIONTRIGGERINGCOUNT,mCalcAlgContext);
							CCondensedReapplyConceptDescriptor* reapplyConDes = CObjectAllocator<CCondensedReapplyConceptDescriptor>::allocateAndConstruct(taskMemMan);
							CConcept* triggerConcept = nextTrigger->getData();
							bool triggerNegation = nextTrigger->isNegated();
							reapplyConDes->initReapllyDescriptor(conDes,nullptr,triggerNegation,triggImpProcRes);
							reapplyQueue->addReapplyConceptDescriptor(reapplyConDes);
							return;
						}
					}

					STATINC(IMPLICATIONEXECUTINGCOUNT,mCalcAlgContext);
					CConcept* implConcept = opLinker->getData();
					bool impConNeg = opLinker->isNegated();
					addConceptToIndividual(implConcept,impConNeg,processIndi,false,mCalcAlgContext);
				}



				void CCalculationTableauSaturationTaskHandleAlgorithm::applyBackwardPropagationConcepts(CIndividualProcessNode*& processIndi, CBackwardPropagationReapplyDescriptor* backPropReapplyDesIt, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = processIndi->getReapplyConceptLabelSet(true);
					while (backPropReapplyDesIt) {
						CConceptDescriptor* reapplyConDes = backPropReapplyDesIt->getReapllyConceptDescriptor();
						CConcept* reapplyConcept = reapplyConDes->getConcept();
						bool reapplyConNegation = reapplyConDes->getNegation();
						CSortedNegLinker<CConcept*>* reapplyOpLinker = reapplyConcept->getOperandList();
						while (reapplyOpLinker) {
							CConcept* opConcept = reapplyOpLinker->getData();
							bool opNegation = reapplyOpLinker->isNegated()^reapplyConNegation;
							//if (!conSet->containsConcept(opConcept,opNegation)) {
								STATINC(ALLROLERESTRICTIONCOUNT,mCalcAlgContext);
								addConceptToIndividual(opConcept,opNegation,processIndi,false,calcAlgContext);
							//}
							reapplyOpLinker = reapplyOpLinker->getNext();
						}
						backPropReapplyDesIt = backPropReapplyDesIt->getNext();
					}
				}



				void CCalculationTableauSaturationTaskHandleAlgorithm::addIndividualToProcessingQueue(CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNodeLinker* processNodeLinker = processIndi->getIndividualProcessNodeLinker();
					if (!processNodeLinker->isProcessingQueued()) {
						processNodeLinker->setProcessingQueued();
						calcAlgContext->getUsedProcessingDataBox()->addIndividualProcessNodeLinker(processNodeLinker);
					}
				}


				void CCalculationTableauSaturationTaskHandleAlgorithm::addConceptsToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSortedNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptToIndividual(opConcept,opConNegation,processIndi,nodeInitialization,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}

				void CCalculationTableauSaturationTaskHandleAlgorithm::addConceptsToIndividual(CConceptAssertionLinker* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptAssertionLinker* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptToIndividual(opConcept,opConNegation,processIndi,nodeInitialization,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}

				void CCalculationTableauSaturationTaskHandleAlgorithm::addConceptsToIndividual(CXNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext) {
					CXNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptToIndividual(opConcept,opConNegation,processIndi,nodeInitialization,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}

				void CCalculationTableauSaturationTaskHandleAlgorithm::addConceptsToIndividual(CXSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext) {
					CXSortedNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptToIndividual(opConcept,opConNegation,processIndi,nodeInitialization,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}




				void CCalculationTableauSaturationTaskHandleAlgorithm::addConceptToIndividual(CConcept* addingConcept, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 opCode = addingConcept->getOperatorCode();
					bool insertConcept = false;
					if (!nodeInitialization) {
						CSortedNegLinker<CConcept*>* opConLinkerIt = addingConcept->getOperandList();
						if (!negate && (opCode == CCAND || opCode == CCAQAND || opCode == CCIMPLAQAND || opCode == CCBRANCHAQAND) || negate && opCode == CCOR) {
							addConceptsToIndividual(opConLinkerIt,negate,processIndi,false,calcAlgContext);
						} else if (opCode == CCAQCHOOCE) {
							while (opConLinkerIt) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegation = opConLinkerIt->isNegated();
								if (opNegation == negate) {
									addConceptToIndividual(opConcept,false,processIndi,false,calcAlgContext);
								}
								opConLinkerIt = opConLinkerIt->getNext();
							}
						} else {
							insertConcept = true;
						}
					} else {
						insertConcept = true;
					}
					if (insertConcept) {
						STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						TConceptDescriptorProcessLinkerPairStruct* conceptDescriptorProcessLinkerPair = CObjectAllocator<TConceptDescriptorProcessLinkerPairStruct>::allocateAndConstruct(taskMemMan);
						CConceptDescriptor* conceptDescriptor = &(conceptDescriptorProcessLinkerPair->conceptDescriptor);
						CConceptProcessLinker* conceptProcessLinker = &(conceptDescriptorProcessLinkerPair->conceptProcessLinker);
						conceptDescriptor->initConceptDescriptor(addingConcept,negate,nullptr);
						CCondensedReapplyQueueIterator reapplyIt;
						CReapplyConceptLabelSet* reapplyConSet = processIndi->getReapplyConceptLabelSet(true);
						bool contained = insertConceptToIndividualConceptSet(conceptDescriptor,processIndi,reapplyConSet,&reapplyIt,nodeInitialization,calcAlgContext);
						if (!contained) {

							conceptProcessLinker->initConceptProcessLinker(conceptDescriptor,nullptr);
							processIndi->addConceptProcessLinker(conceptProcessLinker);

							if (reapplyIt.hasNext()) {
								applyReapplyQueueConcepts(processIndi,&reapplyIt,calcAlgContext);
							}
							addIndividualToProcessingQueue(processIndi,calcAlgContext);
						}
					}
				}



				void CCalculationTableauSaturationTaskHandleAlgorithm::applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CCondensedReapplyQueueIterator* reapplyQueueIt, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CProcessingRestrictionSpecification* procRest = nullptr;
					while (reapplyQueueIt->hasNext()) {
						STATINC(REAPPLIEDCONCEPTSCOUNT,calcAlgContext);
						CReapplyConceptDescriptor* reapplyConceptDes = reapplyQueueIt->next();
						CConceptDescriptor* conDes = reapplyConceptDes->getConceptDescriptor();
						procRest = reapplyConceptDes->getReapplyProcessingRestriction();
						addConceptToProcessingQueue(conDes,processIndi,calcAlgContext);
					}
				}



				bool CCalculationTableauSaturationTaskHandleAlgorithm::hasConceptLocalImpact(CConcept* concept, bool conNeg, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 opCode = concept->getOperatorCode();
					if (opCode == CCSUB || opCode == CCIMPLTRIG || opCode == CCBRANCHTRIG) {
						return true;
					} else if (!conNeg && (opCode == CCALL || opCode == CCIMPLALL || opCode == CCBRANCHALL) || conNeg && (opCode == CCSOME)) {
						return false;
					} else if (!conNeg && (opCode == CCAND || opCode == CCAQAND || opCode == CCBRANCHAQAND || opCode == CCIMPLAQAND) || conNeg && opCode == CCOR) {
						CSortedNegLinker<CConcept*>* opConList = concept->getOperandList();
						while (opConList) {
							CConcept* opConcept = opConList->getData();
							bool opConNeg = opConList->isNegated()^conNeg;
							if (hasConceptLocalImpact(opConcept,opConNeg,calcAlgContext)) {
								return true;
							}
							opConList = opConList->getNext();
						}
						return false;
					} else if (!conNeg && (opCode == CCSOME || opCode == CCAQSOME) || conNeg && (opCode == CCALL)) {
						return true;
					} else if (!conNeg && (opCode == CCAQALL || opCode == CCIMPLAQALL || opCode == CCBRANCHAQALL) || conNeg && (opCode == CCSOME || opCode == CCAQSOME)) {
						return false;
					} else if (opCode == CCAQCHOOCE) {
						CSortedNegLinker<CConcept*>* opConList = concept->getOperandList();
						while (opConList) {
							CConcept* opConcept = opConList->getData();
							bool opNegation = opConList->isNegated();
							if (opNegation == conNeg) {
								if (hasConceptLocalImpact(opConcept,false,calcAlgContext)) {
									return true;
								}
							}
							opConList = opConList->getNext();
						}
						return false;
					} else if (opCode == CCIMPL) {
						CSortedNegLinker<CConcept*>* opConList = concept->getOperandList();
						CConcept* opConcept = opConList->getData();
						bool opNegation = opConList->isNegated();
						if (hasConceptLocalImpact(opConcept,opNegation,calcAlgContext)) {
							return true;
						}
						return false;
					}
					return false;
				}

				bool CCalculationTableauSaturationTaskHandleAlgorithm::insertConceptToIndividualConceptSet(CConceptDescriptor* conceptDescriptor, CIndividualProcessNode*& processIndi, CReapplyConceptLabelSet* conLabelSet, CCondensedReapplyQueueIterator* reapplyIt, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool contained = false;
					bool insertConcept = true;
					bool requiredBackProp = processIndi->getRequiredBackwardPropagation();
					CConcept* concept = conceptDescriptor->getConcept();
					bool conNeg = conceptDescriptor->getNegation();
					cint64 opCode = concept->getOperatorCode();
					if (opCode == CCSUB) {
						++mAddedSUBConcepts;
					} else if (!conNeg && (opCode == CCALL || opCode == CCIMPLALL)) {
						++mAddedALLConcepts;
						contained = !requiredBackProp;
						insertConcept = false;
					} else if (opCode == CCSOME) {
						++mAddedSOMEConcepts;
						insertConcept = false;
					} else if (opCode == CCIMPL) {
						++mAddedIMPLConcepts;
						if (!requiredBackProp && !hasConceptLocalImpact(concept,false,calcAlgContext)) {
							contained = true;
						}
						insertConcept = false;
					} else if (opCode == CCIMPLTRIG) {
						++mAddedTRIGGConcepts;
					} else {
						++mAddedELSEConcepts;
					}
					if (insertConcept) {
						CConceptDescriptor* clashedConceptDescriptor = nullptr;
						CDependencyTrackPoint* clashedDependencyTrackPoint = nullptr;
						// no clash expected
						contained = conLabelSet->insertConceptGetClash(conceptDescriptor,nullptr,reapplyIt,&clashedConceptDescriptor,&clashedDependencyTrackPoint);
					}
					if (nodeInitialization) {
						if (!processIndi->getIndividualInitializationConcept()) {
							processIndi->setIndividualInitializationConcept(conceptDescriptor);
						}
					}
					return contained;
				}


				void CCalculationTableauSaturationTaskHandleAlgorithm::addConceptToProcessingQueue(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConcept* concept = conceptDescriptor->getConcept();
					cint64 opCode = concept->getOperatorCode();
					bool conNegation = conceptDescriptor->getNegation();
					bool processConcept = true;
					if (opCode == CCSUB && conNegation) {
						processConcept = false;
					}
					if (processConcept) {
						STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						CConceptProcessLinker* conProcessLinker = CObjectAllocator<CConceptProcessLinker>::allocateAndConstruct(taskMemMan);
						conProcessLinker->initConceptProcessLinker(conceptDescriptor,nullptr);
						processIndi->addConceptProcessLinker(conProcessLinker);
					}
				}


				void CCalculationTableauSaturationTaskHandleAlgorithm::addConceptRestrictedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CProcessingRestrictionSpecification* procRest, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CConceptProcessLinker* conProcessLinker = CObjectAllocator<CConceptProcessLinker>::allocateAndConstruct(taskMemMan);
					conProcessLinker->initConceptProcessLinker(conceptDescriptor,procRest);
					processIndi->addConceptProcessLinker(conProcessLinker);
				}


				cint64 CCalculationTableauSaturationTaskHandleAlgorithm::getAppliedANDRuleCount() {
					return mAppliedANDRuleCount;
				}

				cint64 CCalculationTableauSaturationTaskHandleAlgorithm::getAppliedORRuleCount() {
					return mAppliedORRuleCount;
				}

				cint64 CCalculationTableauSaturationTaskHandleAlgorithm::getAppliedSOMERuleCount() {
					return mAppliedSOMERuleCount;
				}

				cint64 CCalculationTableauSaturationTaskHandleAlgorithm::getAppliedATLEASTRuleCount() {
					return mAppliedATLEASTRuleCount;
				}

				cint64 CCalculationTableauSaturationTaskHandleAlgorithm::getAppliedALLRuleCount() {
					return mAppliedALLRuleCount;
				}

				cint64 CCalculationTableauSaturationTaskHandleAlgorithm::getAppliedATMOSTRuleCount() {
					return mAppliedATMOSTRuleCount;
				}

				cint64 CCalculationTableauSaturationTaskHandleAlgorithm::getAppliedTotalRuleCount() {
					return mAppliedTOTALRuleCount;
				}


				QString CCalculationTableauSaturationTaskHandleAlgorithm::generateExtendedDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext, cint64 firstIndiID, cint64 lastIndiID) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualProcessNodeVector* indiVec = procDataBox->getIndividualProcessNodeVector();
					cint64 indiCount = indiVec->getItemCount();
					if (lastIndiID >= 0) {
						indiCount = qMin(lastIndiID,indiCount);
					}
					firstIndiID = qMin(firstIndiID,indiCount);
					QStringList indiStringList;
					for (cint64 i = firstIndiID; i < indiCount; ++i) {
						CIndividualProcessNode* indi = indiVec->getData(i);
						CReapplyConceptLabelSet* conSet = indi->getReapplyConceptLabelSet(false);
						if (conSet) {
							QString nominalString = QString("");
							if (indi->getNominalIndividual()) {
								nominalString = QString(", ")+CIRIName::getRecentIRIName(indi->getNominalIndividual()->getNameLinker());
							}
							QString indiString = QString("[ %1%2 ] = <br>\n").arg(indi->getIndividualID()).arg(nominalString);
							QString conSetString;
							CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(false,false,false);
							while (conSetIt.hasNext()) {
								CConceptDescriptor* conDes = conSetIt.next();
								CConcept* concept = conDes->getConcept();
								cint64 conTag = conDes->getConceptTag();
								if (conTag != 1) {
									QString conceptString("null");
									if (conDes) {
										conceptString = CConceptTextFormater::getConceptString(conDes->getConcept(),conDes->isNegated());
									}
									if (!conSetString.isEmpty()) {
										conSetString += QString("<br>\n");
									}
									conSetString += conceptString;
								}
							}
							QString propString;
							CRoleBackwardPropagationHash* backwardPropHash = indi->getRoleBackwardPropagationHash(false);
							if (backwardPropHash) {
								CPROCESSHASH<CRole*,CRoleBackwardPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
								for (CPROCESSHASH<CRole*,CRoleBackwardPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
									CRole* backwardPropRole = it.key();
									CRoleBackwardPropagationHashData backwardPropData = it.value();
									QString roleString;
									roleString += QString::number(backwardPropRole->getRoleTag());
									if (backwardPropRole->hasPropertyName()) {
										roleString += QString("~")+CIRIName::getRecentIRIName(backwardPropRole->getPropertyNameLinker());
									}

									QString sourceIndiListString;
									CBackwardPropagationLink* backPropLinkIt = backwardPropData.mLinkLinker;
									while (backPropLinkIt) {
										CIndividualProcessNode* sourceIndiNode = backPropLinkIt->getSourceIndividual();
										if (!sourceIndiListString.isEmpty()) {
											sourceIndiListString.append(", ");
										} else {
											sourceIndiListString.append("::->");
										}
										sourceIndiListString += QString::number(sourceIndiNode->getIndividualID());
										backPropLinkIt = backPropLinkIt->getNext();
									}

									QString roleBackPropString = QString("%1:%2").arg(roleString).arg(sourceIndiListString);
									propString += QString("<br>\n%1").arg(roleBackPropString);
								}
							}
							if (propString.isEmpty()) {
								propString = "-";
							}
							indiString += QString("{%1} <br>\n<br>\n--------> %2<br>\n").arg(conSetString).arg(propString);
							indiStringList.append(indiString);
						}
					}


					mDebugIndiModelStringList = indiStringList;
					mDebugIndiModelString = mDebugIndiModelStringList.join("<br>\n<p><br>\n\r\n");
					return mDebugIndiModelString;
				}



				QString CCalculationTableauSaturationTaskHandleAlgorithm::generateDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualProcessNodeVector* indiVec = procDataBox->getIndividualProcessNodeVector();
					cint64 indiCount = indiVec->getItemCount();
					QStringList indiStringList;
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualProcessNode* indi = indiVec->getData(i);
						CReapplyConceptLabelSet* conSet = indi->getReapplyConceptLabelSet(false);
						if (conSet) {
							QString nominalString = QString("");
							if (indi->getNominalIndividual()) {
								nominalString = QString(", ")+CIRIName::getRecentIRIName(indi->getNominalIndividual()->getNameLinker());
							}
							QString indiString = QString("[ %1%2 ] = ").arg(indi->getIndividualID()).arg(nominalString);
							QString conSetString;
							CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(false,false,false);
							while (conSetIt.hasNext()) {
								CConceptDescriptor* conDes = conSetIt.next();
								CConcept* concept = conDes->getConcept();
								cint64 conTag = conDes->getConceptTag();
								if (conTag != 1) {
									QString conString = QString("%1%2").arg((conDes->isNegated())?"-":"").arg(conTag);
									if (concept->hasClassName()) {
										conString += CIRIName::getRecentIRIName(concept->getClassNameLinker());
									}
									if (!conSetString.isEmpty()) {
										conSetString += QString(", ");
									}
									conSetString += conString;
								}
							}
							indiString += QString("{%1} ").arg(conSetString);
							indiStringList.append(indiString);
						}
					}

					mDebugIndiModelStringList = indiStringList;
					mDebugIndiModelString = mDebugIndiModelStringList.join("\n");
					return mDebugIndiModelString;
				}


			}; // end namespace Algorithm 

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
