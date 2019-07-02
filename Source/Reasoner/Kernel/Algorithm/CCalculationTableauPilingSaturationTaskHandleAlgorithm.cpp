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

#include "CCalculationTableauPilingSaturationTaskHandleAlgorithm.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CCalculationTableauPilingSaturationTaskHandleAlgorithm::CCalculationTableauPilingSaturationTaskHandleAlgorithm() {
					mProcessingDataBox = nullptr;
					mCalcAlgContext = nullptr;

					mProcessingDataBox = nullptr;
					mCalcAlgContext = nullptr;

					mPosJumpFuncVec = &mPosTableauRuleJumpFuncVec[mRuleFuncCount/2];
					mNegJumpFuncVec = &mNegTableauRuleJumpFuncVec[mRuleFuncCount/2];

					for (cint64 i = 0; i < mRuleFuncCount; ++i) {
						mPosTableauRuleJumpFuncVec[i] = nullptr;
						mNegTableauRuleJumpFuncVec[i] = nullptr;
					}

					mConfAncestorBlockingSearch = true;
					mConfImplicationAddingSkipping = true;


					mPosJumpFuncVec[CCTOP] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCAND] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCAQAND] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCIMPLAQAND] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCBRANCHAQAND] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCSUB] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCALL] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCAQALL] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCSOME] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applySOMERule;
					mPosJumpFuncVec[CCAQSOME] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applySOMERule;
					mPosJumpFuncVec[CCAQCHOOCE] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyAutomatChooseRule;
					mPosJumpFuncVec[CCIMPLTRIG] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCIMPLALL] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCBRANCHALL] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCBRANCHAQALL] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCIMPLAQALL] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyALLRule;

					mNegJumpFuncVec[CCOR] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyANDRule;
					mNegJumpFuncVec[CCALL] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applySOMERule;
					mNegJumpFuncVec[CCSOME] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyALLRule;
					mNegJumpFuncVec[CCAQCHOOCE] = &CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyAutomatChooseRule;


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

					mDebugSaturationAlgorithm = nullptr;
					mDebugTestingSaturationTask = nullptr;

					mConfCreateSuccessorLinks = false;
				}

				CCalculationTableauPilingSaturationTaskHandleAlgorithm::~CCalculationTableauPilingSaturationTaskHandleAlgorithm() {
				}

				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::readCalculationConfig(CSatisfiableCalculationTask* satCalcTask) {
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


				CCalculationAlgorithmContextBase* CCalculationTableauPilingSaturationTaskHandleAlgorithm::createCalculationAlgorithmContext(CTaskProcessorContext *processorContext, CProcessContext* processContext, CSatisfiableCalculationTask* satCalcTask) {
					CCalculationAlgorithmContextBase* calcAlgContext = CObjectAllocator< CCalculationAlgorithmContextBase >::allocateAndConstruct(processContext->getUsedMemoryAllocationManager());
					calcAlgContext->initTaskProcessContext(processContext,satCalcTask);
					calcAlgContext->initCalculationAlgorithmContext(processorContext);
					return calcAlgContext;
				}



				bool CCalculationTableauPilingSaturationTaskHandleAlgorithm::handleTask(CTaskProcessorContext *processorContext, CTask* task) {

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


						//createDebugSatisfiableSaturationTestingTask(calcAlgContext);


						CIndividualPilingProcessNode* indiProcPilNode = nullptr;
						CConceptProcessDescriptor* conProcDes = nullptr;

						cint64 processRuleToTaskProcessingVerificationCount = 10;
						cint64 remainProcessRuleToTaskProcessingVerification = processRuleToTaskProcessingVerificationCount;

						CIndividualPilingProcessNodeLinker* indiProcessPilingNodeLinker = mProcessingDataBox->takeIndividualPilingProcessNodeLinker();
						CIndividualPilingProcessNode* lastindiProcPilNode = nullptr;
						try {
							STATINC(TASKPROCESSCHANGECOUNT,mCalcAlgContext);

							while (indiProcessPilingNodeLinker) {

								CIndividualPilingProcessNode* indiProcPilNode = indiProcessPilingNodeLinker->getData();
								lastindiProcPilNode = indiProcPilNode;
								STATINC(INDIVIDUALNODESWITCHCOUNT,mCalcAlgContext);
								++indiProcessedCount;

								if (individualNodeInitializing(indiProcPilNode,calcAlgContext)) {

									CConceptPilingProcessLinker* conceptPilingProcessLinker = indiProcPilNode->takeConceptPilingProcessLinker();
									while (conceptPilingProcessLinker) {
										STATINC(RULEAPPLICATIONCOUNT,mCalcAlgContext);
										KONCLUCE_TASK_ALGORITHM_PILING_SATURATION_MODEL_STRING_INSTRUCTION(mRuleBeginDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
										applyTableauSaturationRule(indiProcPilNode,conceptPilingProcessLinker);
										KONCLUCE_TASK_ALGORITHM_PILING_SATURATION_MODEL_STRING_INSTRUCTION(mRuleEndDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
										releaseConceptPilingProcessLinker(conceptPilingProcessLinker,calcAlgContext);
										conceptPilingProcessLinker = indiProcPilNode->takeConceptPilingProcessLinker();
									}
								}
								indiProcessPilingNodeLinker->clearProcessingQueued();
								individualNodeConclusion(indiProcPilNode,calcAlgContext);
								indiProcessPilingNodeLinker = mProcessingDataBox->takeIndividualPilingProcessNodeLinker();
							}

							//mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
							//QFile tmpFile("tmp3.txt");
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

							//analyseNodeExpressivityViolation(calcAlgContext);
							//analyseParallelizationPossibilities(calcAlgContext);

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



				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::analyseParallelizationPossibilities(CCalculationAlgorithmContextBase* calcAlgContext) {
					QStringList processOrderSizeList;
					QStringList depthOrderParallelSizeList;
					QMap<cint64,cint64> parallDepthCountMap;
					QHash<CIndividualPilingProcessNode*,cint64> parallDepthHash;
					CProcessingDataBox* dataBox = calcAlgContext->getProcessingDataBox();
					CIndividualPilingProcessNodeVector* indiVec = dataBox->getIndividualPilingProcessNodeVector(false);
					for (cint64 ib = 0, ie = indiVec->getItemCount(); ib < ie; ++ib) {
						CIndividualPilingProcessNode* indiNode = indiVec->getData(ib);
						if (!parallDepthHash.contains(indiNode)) {
							cint64 parallDepth = 0;
							QSet<CIndividualPilingProcessNode*> currSet;
							addParallelizationPossibilities(indiNode,currSet,parallDepthHash,parallDepth,calcAlgContext);
							foreach (CIndividualPilingProcessNode* node, currSet) {
								parallDepthHash.insert(node,parallDepth);
							}
							processOrderSizeList.append(QString("%1\t%2").arg(parallDepth).arg(currSet.count()));
							parallDepthCountMap.insertMulti(parallDepth,currSet.count());
						}
					}
					for (QMap<cint64,cint64>::const_iterator it = parallDepthCountMap.constBegin(), itEnd = parallDepthCountMap.constEnd(); it != itEnd; ++it) {
						cint64 parallDepth = it.key();
						cint64 parallSize = it.value();
						depthOrderParallelSizeList.append(QString("%1\t%2").arg(parallDepth).arg(parallSize));
					}
					QString processOrderSizeString = processOrderSizeList.join("\r\n");
					QString depthOrderParallelSizeString = depthOrderParallelSizeList.join("\r\n");
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addParallelizationPossibilities(CIndividualPilingProcessNode* indiNode, QSet<CIndividualPilingProcessNode*>& currSet, QHash<CIndividualPilingProcessNode*,cint64>& prevHash, cint64& maxParallelDepth, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!currSet.contains(indiNode)) {
						cint64 tmpDepth = prevHash.value(indiNode,-1);
						maxParallelDepth = qMax(tmpDepth+1,maxParallelDepth);
						if (tmpDepth < 0) {
							currSet.insert(indiNode);

							CLinkedRolePilingSuccessorHash* succLinkHash = indiNode->getLinkedRoleSuccessorHash(false);
							if (succLinkHash) {
								CPROCESSHASH<CRole*,CIndividualPilingProcessNode*>* succHash = succLinkHash->getLinkedRoleSuccessorHash();
								for (CPROCESSHASH<CRole*,CIndividualPilingProcessNode*>::const_iterator it = succHash->constBegin(), itEnd = succHash->constEnd(); it != itEnd; ++it) {
									CIndividualPilingProcessNode* succNode = it.value();
									addParallelizationPossibilities(succNode,currSet,prevHash,maxParallelDepth,calcAlgContext);
								}
							}

							CIndividualPilingProcessNode* subIndi = indiNode->getSubstituteIndividualNode();
							if (subIndi) {
								addParallelizationPossibilities(subIndi,currSet,prevHash,maxParallelDepth,calcAlgContext);
							}
							CIndividualPilingProcessNode* copyIndi = indiNode->getCopyIndividualNode();
							if (copyIndi) {
								addParallelizationPossibilities(copyIndi,currSet,prevHash,maxParallelDepth,calcAlgContext);
							}
						}
					}
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::analyseNodeExpressivityViolation(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* dataBox = calcAlgContext->getProcessingDataBox();
					CIndividualPilingProcessNodeVector* indiVec = dataBox->getIndividualPilingProcessNodeVector(false);
					for (cint64 ib = 0, ie = indiVec->getItemCount(); ib < ie; ++ib) {
						CIndividualPilingProcessNode* indiNode = indiVec->getData(ib);
						CLinkedRolePilingSuccessorHash* succLinkHash = indiNode->getLinkedRoleSuccessorHash(false);
						if (succLinkHash) {
							CPROCESSHASH<CRole*,CIndividualPilingProcessNode*>* succHash = succLinkHash->getLinkedRoleSuccessorHash();
							foreach (CRole* role, succHash->uniqueKeys()) {
								// check for violations
								CRoleBackwardPilingPropagationHash* backPropHash = indiNode->getRoleBackwardPropagationHash(false);
								CPROCESSHASH<CRole*,CRoleBackwardPilingPropagationHashData>* backPropDataHash = nullptr;
								CRoleBackwardPilingPropagationHashData backPropData;
								if (backPropHash) {
									backPropDataHash = backPropHash->getRoleBackwardPropagationDataHash();
									backPropData = backPropDataHash->value(role);
								}
								cint64 succCount = 0;
								if (role->isFunctional()) {
									// check functional role violation
									succCount = succHash->count(role);
									if (succCount > 1) {
										CIndividualNodeExpressivityViolationFlags* violationFlags = indiNode->getDirectViolationFlags(true);
										violationFlags->setViolationFlags(CIndividualNodeExpressivityViolationFlags::VFFUNCTIONALEXPRESSIVITYVIOLATED);
									} else if (succCount == 1) {
										if (backPropHash) {
											for (CBackwardPilingPropagationLink* backPropLinkIt = backPropData.mLinkLinker; backPropLinkIt; backPropLinkIt = backPropLinkIt->getNext()) {
												CIndividualPilingProcessNode* sourceIndi = backPropLinkIt->getSourceIndividual();
												// each source individual node is violated
												CIndividualNodeExpressivityViolationFlags* violationFlags = sourceIndi->getDirectViolationFlags(true);
												violationFlags->setViolationFlags(CIndividualNodeExpressivityViolationFlags::VFFUNCTIONALANCESTOREXPRESSIVITYVIOLATED);												
											}
										}
									}
								}
								if (backPropHash) {
									// test inverse propagations violation
									if (backPropData.mReapplyLinker) {
										CPROCESSHASH<CRole*,CIndividualPilingProcessNode*>::const_iterator beginSuccIt = succHash->constFind(role);
										CPROCESSHASH<CRole*,CIndividualPilingProcessNode*>::const_iterator endSuccIt = succHash->constEnd();
										for (CPROCESSHASH<CRole*,CIndividualPilingProcessNode*>::const_iterator it = beginSuccIt; it != endSuccIt && it.key() == role; ++it) {
											CIndividualPilingProcessNode* succNode = it.value();
											CReapplyConceptPilingLabelSet* labelSet = succNode->getReapplyConceptPilingLabelSet();
											for (CBackwardPilingPropagationReapplyDescriptor* backPropDesIt = backPropData.mReapplyLinker; backPropDesIt; backPropDesIt = backPropDesIt->getNext()) {
												CConceptPilingDescriptor* reapplyConPilDes = backPropDesIt->getReapplyConceptPilingDescriptor();
												CConcept* reapplyConcept = reapplyConPilDes->getConcept();
												bool reapplyConNegation = reapplyConPilDes->getNegation();
												if (!labelSet->containsConcept(reapplyConcept,reapplyConNegation)) {
													indiNode->getDirectViolationFlags(true)->setViolationFlags(CIndividualNodeExpressivityViolationFlags::VFINVERSEEXPRESSIVITYVIOLATED);
												}
											}
										}
									}									
								}
							}						
						}
					}

					// propagate all violations
					for (cint64 ib = 0, ie = indiVec->getItemCount(); ib < ie; ++ib) {
						CIndividualPilingProcessNode* indiNode = indiVec->getData(ib);
						propagateDirectExpressivityViolations(indiNode,calcAlgContext);
					}
					for (cint64 ib = 0, ie = indiVec->getItemCount(); ib < ie; ++ib) {
						CIndividualPilingProcessNode* indiNode = indiVec->getData(ib);
						CIndividualNodeExpressivityViolationFlags* violationFlags = indiNode->getDirectViolationFlags(false);
						if (violationFlags) {
							indiNode->getIndirectViolationFlags(true)->setViolationFlags(violationFlags);
						}
					}
					for (cint64 ib = 0, ie = indiVec->getItemCount(); ib < ie; ++ib) {
						CIndividualPilingProcessNode* indiNode = indiVec->getData(ib);
						propagateIndirectExpressivityViolations(indiNode,calcAlgContext);
					}
					for (cint64 ib = 0, ie = indiVec->getItemCount(); ib < ie; ++ib) {
						CIndividualPilingProcessNode* indiNode = indiVec->getData(ib);
						CIndividualPilingProcessNode* subIndiNode = indiNode->getSubstituteIndividualNode();
						if (subIndiNode) {
							CIndividualNodeExpressivityViolationFlags* violationFlags = subIndiNode->getDirectViolationFlags(false);
							if (violationFlags) {
								indiNode->getDirectViolationFlags(true)->setViolationFlags(violationFlags);
							}
							violationFlags = subIndiNode->getIndirectViolationFlags(false);
							if (violationFlags) {
								indiNode->getIndirectViolationFlags(true)->setViolationFlags(violationFlags);
							}
						}
					}


					// analyze violations
					cint64 violatedNodesCount = 0;
					cint64 totalNodesCount = 0;
					cint64 directViolatedNodesCount = 0;
					cint64 indirectViolatedNodesCount = 0;
					cint64 dirFunViolatedCount = 0;
					cint64 dirAncFunViolatedCount = 0;
					cint64 dirInverseViolatedCount = 0;

					for (cint64 ib = 0, ie = indiVec->getItemCount(); ib < ie; ++ib) {
						CIndividualPilingProcessNode* indiNode = indiVec->getData(ib);
						++totalNodesCount;
						bool violated = false;
						CIndividualNodeExpressivityViolationFlags* dirViolationFlags = indiNode->getDirectViolationFlags(false);
						if (dirViolationFlags && dirViolationFlags->hasPartialViolationFlags(CIndividualNodeExpressivityViolationFlags::VFEXPRESSIVITYVIOLATED)) {
							++directViolatedNodesCount;
							violated = true;
							if (dirViolationFlags->hasPartialViolationFlags(CIndividualNodeExpressivityViolationFlags::VFFUNCTIONALEXPRESSIVITYVIOLATED)) {
								++dirFunViolatedCount;
							}
							if (dirViolationFlags->hasPartialViolationFlags(CIndividualNodeExpressivityViolationFlags::VFFUNCTIONALANCESTOREXPRESSIVITYVIOLATED)) {
								++dirAncFunViolatedCount;
							}
							if (dirViolationFlags->hasPartialViolationFlags(CIndividualNodeExpressivityViolationFlags::VFINVERSEEXPRESSIVITYVIOLATED)) {
								++dirInverseViolatedCount;
							}
						}
						CIndividualNodeExpressivityViolationFlags* indViolationFlags = indiNode->getIndirectViolationFlags(false);
						if (indViolationFlags && indViolationFlags->hasPartialViolationFlags(CIndividualNodeExpressivityViolationFlags::VFEXPRESSIVITYVIOLATED)) {
							++indirectViolatedNodesCount;
							violated = true;
						}
						if (violated) {
							++violatedNodesCount;
						}
					}

				}

				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::propagateIndirectExpressivityViolations(CIndividualPilingProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualNodeExpressivityViolationFlags* violationFlags = indiNode->getIndirectViolationFlags(false);
					if (violationFlags) {
						// propagate to ancestor nodes
						CRoleBackwardPilingPropagationHash* backPropHash = indiNode->getRoleBackwardPropagationHash(false);
						if (backPropHash) {
							CPROCESSHASH<CRole*,CRoleBackwardPilingPropagationHashData>* backPropDataHash = backPropHash->getRoleBackwardPropagationDataHash();
							for (CPROCESSHASH<CRole*,CRoleBackwardPilingPropagationHashData>::iterator it = backPropDataHash->begin(), itEnd = backPropDataHash->end(); it != itEnd; ++it) {
								CRoleBackwardPilingPropagationHashData& backPropData = it.value();
								for (CBackwardPilingPropagationLink* backPropLinkIt = backPropData.mLinkLinker; backPropLinkIt; backPropLinkIt = backPropLinkIt->getNext()) {
									CIndividualPilingProcessNode* sourceNode = backPropLinkIt->getSourceIndividual();
									if (sourceNode->getIndirectViolationFlags(true)->setViolationFlags(violationFlags)) {
										propagateIndirectExpressivityViolations(indiNode,calcAlgContext);
									}
								}
							}
						}
						for (CXLinker<CIndividualPilingProcessNode*>* depIndiLinkerIt = indiNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
							CIndividualPilingProcessNode* depIndiNode = depIndiLinkerIt->getData();
							if (depIndiNode->getIndirectViolationFlags(true)->setViolationFlags(violationFlags)) {
								propagateIndirectExpressivityViolations(depIndiNode,calcAlgContext);
							}
						}
					}
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::propagateDirectExpressivityViolations(CIndividualPilingProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualNodeExpressivityViolationFlags* violationFlags = indiNode->getDirectViolationFlags(false);
					if (violationFlags) {
						for (CXLinker<CIndividualPilingProcessNode*>* depIndiLinkerIt = indiNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
							CIndividualPilingProcessNode* depIndiNode = depIndiLinkerIt->getData();
							if (depIndiNode->getDirectViolationFlags(true)->setViolationFlags(violationFlags)) {
								propagateDirectExpressivityViolations(depIndiNode,calcAlgContext);
							}
						}						
					}
				}

				bool CCalculationTableauPilingSaturationTaskHandleAlgorithm::individualNodeInitializing(CIndividualPilingProcessNode*& indiProcPilNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!indiProcPilNode->isInitialized()) {
						CXSortedNegLinker<CConcept*>* initConceptLinkerIt = indiProcPilNode->getInitializingConceptLinker();
						if (initConceptLinkerIt) {
							initializeInitializationConcepts(indiProcPilNode,initConceptLinkerIt,calcAlgContext);
							indiProcPilNode->clearInitializingConcepts();
						}

						indiProcPilNode->setInitialized(true);
					}
					return true;
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::initializeInitializationConcepts(CIndividualPilingProcessNode*& indiProcPilNode, CXSortedNegLinker<CConcept*>* initConceptLinker, CCalculationAlgorithmContextBase* calcAlgContext) {

					CXSortedNegLinker<CConcept*>* onlyInitConcept = nullptr;
					CXSortedNegLinker<CConcept*>* initConceptLinkerIt = initConceptLinker;
					while (initConceptLinkerIt) {
						if (!onlyInitConcept) {
							onlyInitConcept = initConceptLinkerIt;
						}
						initConceptLinkerIt = initConceptLinkerIt->getNext();
					}
					if (onlyInitConcept && onlyInitConcept->hasNext()) {
						onlyInitConcept = nullptr;
					}
					bool requiredBackProp = true;

					CIndividualPilingProcessNode* specialIndiNode = nullptr;
					CConceptSaturationReferenceLinkingData* confSatRefLinkingData = nullptr;
					COptimizedConceptSaturationSatisfiableTestingItem* conceptSatItem = nullptr;

					bool copyIndividualNode = false;
					bool substituiteIndividualNode = false;
					bool pilingIndividualNode = false;

					if (onlyInitConcept) {
						CConcept* concept = onlyInitConcept->getData();
						CConceptData* conceptData = concept->getConceptData();
						if (conceptData) {
							CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
							CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
							if (conRefLinking) {
								confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
								if (confSatRefLinkingData) {
									conceptSatItem = (COptimizedConceptSaturationSatisfiableTestingItem*)confSatRefLinkingData->getClassifierReferenceLinkingData();
								}
							}
						}

						if (conceptSatItem && concept->hasClassName()) {
							requiredBackProp = conceptSatItem->isPotentiallyExistInitializationConcept();
						}

						if (conceptSatItem) {
							COptimizedConceptSaturationSatisfiableTestingItem* specialRefItem = conceptSatItem->getSpecialItemReference();
							if (specialRefItem) {
								CConcept* refConcept = specialRefItem->getSatisfiableConcept();
								CConceptData* refConceptData = refConcept->getConceptData();
								if (refConceptData) {
									CConceptProcessData* refConProcData = (CConceptProcessData*)refConceptData;
									CConceptReferenceLinking* refConRefLinking = refConProcData->getConceptReferenceLinking();
									if (refConRefLinking) {
										CConceptSaturationReferenceLinkingData* refConfSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)refConRefLinking;
										CClassificationConceptReferenceLinking* classSatCalcRefLinkData = confSatRefLinkingData->getClassifierReferenceLinkingData();
										if (classSatCalcRefLinkData) {
											specialIndiNode = (CIndividualPilingProcessNode*)classSatCalcRefLinkData->getIndividualProcessNodeForConcept();
										}
									}
								}
							}
						}

						if (conceptSatItem) {
							COptimizedConceptSaturationSatisfiableTestingItem::SATURATIONITEMREFERENCESPECIALMODE mode = conceptSatItem->getSpecialReferenceMode();
							if (mode == COptimizedConceptSaturationSatisfiableTestingItem::SATURATIONCOPYMODE) {
								copyIndividualNode = true;
							} else if (mode == COptimizedConceptSaturationSatisfiableTestingItem::SATURATIONPILINGMODE) {
								pilingIndividualNode = true;
							} else if (mode == COptimizedConceptSaturationSatisfiableTestingItem::SATURATIONSUBSTITUTEMODE) {
								substituiteIndividualNode = true;
							}
						}
					}


					if (pilingIndividualNode) {
						pilingIndividualNode = false;
						copyIndividualNode = true;
						if (false) {
							bool bug = false;
						}
					}
					//if (substituiteIndividualNode) {
					//	substituiteIndividualNode = false;
					//	copyIndividualNode = true;
					//	if (false) {
					//		bool bug = false;
					//	}
					//}
					//specialIndiNode = nullptr;

					bool addInitializationConcepts = true;
					bool initialized = false;
					if (specialIndiNode && !specialIndiNode->isInitialized()) {
						specialIndiNode = nullptr;
					}
					if (specialIndiNode) {
						indiProcPilNode->setReferenceIndividualPilingProcessNode(specialIndiNode);
					}
					if (specialIndiNode && substituiteIndividualNode) {
						CConcept* concept = onlyInitConcept->getData();
						CIndividualPilingProcessNode* blockedIndiNode = specialIndiNode;
						while (blockedIndiNode->hasSubstituteIndividualNode()) {
							blockedIndiNode = blockedIndiNode->getSubstituteIndividualNode();
						}
						cint64 blockedPilingDepth = blockedIndiNode->getPilingDepth();
						CReapplyConceptPilingLabelSet* blockConSet = blockedIndiNode->getRootPilingIndividualNode()->getReapplyConceptPilingLabelSet(false);
						CConceptPilingDescriptor* conceptPilingDescriptor = nullptr;
						CImplicationReapplyConceptPilingDescriptor* impReapplyConPilDes = nullptr;
						cint64 pilingDepth = 0;
						bool contained = false;
						if (blockConSet->getConceptDescriptorAndReapplyQueue(concept,conceptPilingDescriptor,impReapplyConPilDes,pilingDepth)) {
							if (pilingDepth <= blockedPilingDepth) {
								contained = true;
							}
						}
						if (!contained) {
							++mSubstituitedIndiNodeCount;
							indiProcPilNode->clearConceptPilingProcessLinker();
							indiProcPilNode->setSubstituteIndividualNode(specialIndiNode);
							indiProcPilNode->setReferenceMode(1);
							addInitializationConcepts = false;
							initialized = true;
						} else {
							copyIndividualNode = true;
						}
					} 
					if (specialIndiNode && copyIndividualNode) {
						CIndividualPilingProcessNode* blockedIndiNode = specialIndiNode;
						while (blockedIndiNode->hasSubstituteIndividualNode()) {
							blockedIndiNode = blockedIndiNode->getSubstituteIndividualNode();
						}
						cint64 pilingDepth = blockedIndiNode->getPilingDepth();
						indiProcPilNode->initCopingIndividualPilingProcessNode(blockedIndiNode,pilingDepth);
						indiProcPilNode->setReferenceMode(2);
						indiProcPilNode->setCopyIndividualNode(blockedIndiNode);
						initialized = true;
						//testDebugSaturationTaskContainsConcepts(indiProcPilNode,calcAlgContext);
					} else if (specialIndiNode && pilingIndividualNode) {
						indiProcPilNode->setReferenceMode(3);
						indiProcPilNode->initPilingIndividualPilingProcessNode(specialIndiNode);
						initialized = true;
					}
					if (!initialized) {
						indiProcPilNode->initRootIndividualPilingProcessNode();
						indiProcPilNode->setReferenceMode(4);
						CConcept* topConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
						addConceptFilteredToIndividual(topConcept,false,indiProcPilNode,false,calcAlgContext);
						initialized = true;
					}
					if (addInitializationConcepts) {
						addConceptsFilteredToIndividual(initConceptLinker,false,indiProcPilNode,calcAlgContext);
					}
					indiProcPilNode->setRequiredBackwardPropagation(requiredBackProp);
					CBackwardPilingPropagationLink* backPilPropLinkIt = indiProcPilNode->getInitializingBackwardPropagationLinks();
					if (backPilPropLinkIt) {
						CRoleBackwardPilingPropagationHash* backPropHash = indiProcPilNode->getRootPilingIndividualNode()->getRoleBackwardPropagationHash(true);
						cint64 indiPilingDepth = indiProcPilNode->getPilingDepth();
						while (backPilPropLinkIt) {
							CBackwardPilingPropagationLink* backPropLink = backPilPropLinkIt;
							backPilPropLinkIt = backPilPropLinkIt->getNext();
							backPropLink->clearNext();
							backPropLink->setPilingDepth(indiPilingDepth);
							CBackwardPilingPropagationReapplyDescriptor* backPropReapplyDes = backPropHash->addBackwardPropagationLink(backPropLink->getLinkRole(),backPropLink);
							if (backPropReapplyDes) {
								applyBackwardPropagationConcepts(backPropLink,backPropReapplyDes,mCalcAlgContext);
							}
						}
					}
					indiProcPilNode->setInitializedConceptLinker(initConceptLinker);

				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::individualNodeConclusion(CIndividualPilingProcessNode*& indiProcPilNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptPilingProcessLinker* conPilProLinker = indiProcPilNode->getConceptPilingProcessLinker();
					if (conPilProLinker) {
						addIndividualToProcessingQueue(indiProcPilNode,calcAlgContext);
					}
				}



				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyTableauSaturationRule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conPilProLinker) {
					CConceptPilingDescriptor* conDes = conPilProLinker->getConceptPilingDescriptor();
					bool conNeg = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					cint64 conOpCode = concept->getOperatorCode();

					TableauRuleFunction func = nullptr;
					if (!conNeg) {
						func = mPosJumpFuncVec[conOpCode];
						if (func) {
							(this->*func)(processIndi,conPilProLinker);
						}
					} else {
						func = mNegJumpFuncVec[conOpCode];
						if (func) {
							(this->*func)(processIndi,conPilProLinker);
						}
					}
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyAutomatChooseRule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conPilProLinker) {
					STATINC(AUTOMATEINITCOUNT,mCalcAlgContext);
					CConceptPilingDescriptor* conDes = conPilProLinker->getConceptPilingDescriptor();
					cint64 pilingDepth = conPilProLinker->getPilingDepth();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					while (conceptOpLinkerIt) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opNegation = conceptOpLinkerIt->isNegated();
						if (opNegation == conNegation) {
							addConceptFilteredToIndividual(opConcept,false,processIndi,pilingDepth,false,mCalcAlgContext);
						}
						conceptOpLinkerIt = conceptOpLinkerIt->getNext();
					}
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyANDRule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conPilProLinker) {
					STATINC(ANDRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptPilingDescriptor* conDes = conPilProLinker->getConceptPilingDescriptor();
					cint64 pilingDepth = conPilProLinker->getPilingDepth();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					addConceptsFilteredToIndividual(conceptOpLinkerIt,conNegation,processIndi,pilingDepth,false,mCalcAlgContext);
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyALLRule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conPilProLinker) {
					STATINC(ALLRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptPilingDescriptor* conDes = conPilProLinker->getConceptPilingDescriptor();
					cint64 pilingDepth = conPilProLinker->getPilingDepth();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();

					CMemoryAllocationManager* taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CRoleBackwardPilingPropagationHash* backPropHash = processIndi->getRootPilingIndividualNode()->getRoleBackwardPropagationHash(true);
					CBackwardPilingPropagationReapplyDescriptor* backPropReapplyDes = CObjectAllocator<CBackwardPilingPropagationReapplyDescriptor>::allocateAndConstruct(taskMemMan);
					backPropReapplyDes->initBackwardPropagationReapplyDescriptor(conDes,pilingDepth);
					CBackwardPilingPropagationLink* backPropLinkIt = backPropHash->addBackwardPropagationConceptDescriptor(role,backPropReapplyDes);
					
					if (backPropLinkIt) {
						while (backPropLinkIt) {
							cint64 linkPilingDepth = backPropLinkIt->getPilingDepth();
							if (linkPilingDepth >= pilingDepth) {
								CIndividualPilingProcessNode* backPropIndiNode = backPropLinkIt->getSourceIndividual();

								CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
								while (conceptOpLinkerIt) {
									CConcept* opConcept = conceptOpLinkerIt->getData();
									bool opConNegation = conceptOpLinkerIt->isNegated()^conNegation;
									cint64 sourcePilingDepth = backPropLinkIt->getSourcePilingDepth();
									STATINC(ALLROLERESTRICTIONCOUNT,mCalcAlgContext);
									addConceptFilteredToIndividual(opConcept,opConNegation,backPropIndiNode,sourcePilingDepth,true,mCalcAlgContext);
									conceptOpLinkerIt = conceptOpLinkerIt->getNext();
								}
							}
							backPropLinkIt = backPropLinkIt->getNext();
						}
					}
				}



				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::applySOMERule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conPilProLinker) {
					STATINC(SOMERULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptPilingDescriptor* conDes = conPilProLinker->getConceptPilingDescriptor();
					cint64 pilingDepth = conPilProLinker->getPilingDepth();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					while (conceptOpLinkerIt) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opConNegation = conceptOpLinkerIt->isNegated()^conNegation;
						CConceptData* conceptData = opConcept->getConceptData();
						KONCLUDE_ASSERT_X(conceptData,"SOME saturation rule","concept data missing");
						if (conceptData) {
							CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
							CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
							KONCLUDE_ASSERT_X(conRefLinking,"SOME saturation rule","concept reference linking data missing");
							if (conRefLinking) {
								CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
								CClassificationConceptReferenceLinking* classSatCalcRefLinkData = confSatRefLinkingData->getClassifierReferenceLinkingData();
								if (classSatCalcRefLinkData) {
									CIndividualPilingProcessNode* existIndiNode = (CIndividualPilingProcessNode*)classSatCalcRefLinkData->getIndividualProcessNodeForConcept();
									KONCLUDE_ASSERT_X(existIndiNode,"SOME saturation rule","concept reference linked individual node missing");
									if (existIndiNode) {
										CMemoryAllocationManager* taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
										CRoleBackwardPilingPropagationHash* backPropHash = existIndiNode->getRootPilingIndividualNode()->getRoleBackwardPropagationHash(true);
										bool existIndiInitialized = existIndiNode->isInitialized();
										CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
										while (superRoleIt) {
											if (superRoleIt->isNegated()) {
												CRole* superRole = superRoleIt->getData();
												CBackwardPilingPropagationLink* backPropLink = CObjectAllocator<CBackwardPilingPropagationLink>::allocateAndConstruct(taskMemMan);
												backPropLink->initBackwardPropagationLink(processIndi,pilingDepth,superRole,0);
												if (!existIndiInitialized) {
													existIndiNode->addInitializingBackwardPropagationLinks(backPropLink);
												} else {
													cint64 existPilingDepth = existIndiNode->getPilingDepth();
													backPropLink->setPilingDepth(existPilingDepth);
													CBackwardPilingPropagationReapplyDescriptor* backPropReapplyDes = backPropHash->addBackwardPropagationLink(superRole,backPropLink);
													if (backPropReapplyDes) {
														applyBackwardPropagationConcepts(backPropLink,backPropReapplyDes,mCalcAlgContext);
													}
												}
											} else {
												if (mConfCreateSuccessorLinks) {
													CRole* superRole = superRoleIt->getData();
													CLinkedRolePilingSuccessorHash* succHash = processIndi->getLinkedRoleSuccessorHash(true);
													succHash->addLinkedSuccessor(superRole,existIndiNode,pilingDepth);
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


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::applyBackwardPropagationConcepts(CBackwardPilingPropagationLink* backPropLink, CBackwardPilingPropagationReapplyDescriptor* backPropReapplyDesIt, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualPilingProcessNode* sourceIndiNode = backPropLink->getSourceIndividual();
					cint64 linkPilingDepth = backPropLink->getPilingDepth();
					while (backPropReapplyDesIt) {
						CConceptPilingDescriptor* reapplyConPilDes = backPropReapplyDesIt->getReapplyConceptPilingDescriptor();
						CConcept* reapplyConcept = reapplyConPilDes->getConcept();
						bool reapplyConNegation = reapplyConPilDes->getNegation();
						cint64 reapplyPilingDepth = backPropReapplyDesIt->getPilingDepth();
						if (linkPilingDepth >= reapplyPilingDepth) {
							CSortedNegLinker<CConcept*>* reapplyOpLinker = reapplyConcept->getOperandList();
							while (reapplyOpLinker) {
								CConcept* opConcept = reapplyOpLinker->getData();
								bool opNegation = reapplyOpLinker->isNegated()^reapplyConNegation;
								STATINC(ALLROLERESTRICTIONCOUNT,calcAlgContext);
								cint64 sourcePilingDepth = backPropLink->getSourcePilingDepth();
								addConceptFilteredToIndividual(opConcept,opNegation,sourceIndiNode,sourcePilingDepth,true,calcAlgContext);
								reapplyOpLinker = reapplyOpLinker->getNext();
							}
						}
						backPropReapplyDesIt = backPropReapplyDesIt->getNext();
					}
				}

				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addIndividualToProcessingQueue(CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualPilingProcessNodeLinker* processNodeLinker = processIndi->getIndividualPilingProcessNodeLinker();
					if (!processNodeLinker->isProcessingQueued()) {
						processNodeLinker->setProcessingQueued();
						calcAlgContext->getUsedProcessingDataBox()->addIndividualPilingProcessNodeLinker(processNodeLinker);
					}
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addConceptsFilteredToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualPilingProcessNode*& processIndi, cint64 pilingDepth, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSortedNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptFilteredToIndividual(opConcept,opConNegation,processIndi,pilingDepth,updateCopyDependedIndividual,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}

				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addConceptsFilteredToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSortedNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptFilteredToIndividual(opConcept,opConNegation,processIndi,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}

				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addConceptsFilteredToIndividual(CXNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CXNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptFilteredToIndividual(opConcept,opConNegation,processIndi,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}

				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addConceptsFilteredToIndividual(CXSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CXSortedNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptFilteredToIndividual(opConcept,opConNegation,processIndi,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualPilingProcessNode* rootProcessIndi = processIndi->getRootPilingIndividualNode();
					CReapplyConceptPilingLabelSet* labelSet = rootProcessIndi->getReapplyConceptPilingLabelSet();
					cint64 pilingDepth = processIndi->getPilingDepth();
					addConceptFilteredToIndividual(addingConcept,negate,rootProcessIndi,labelSet,pilingDepth,true,calcAlgContext);
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& processIndi, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualPilingProcessNode* rootProcessIndi = processIndi->getRootPilingIndividualNode();
					CReapplyConceptPilingLabelSet* labelSet = rootProcessIndi->getReapplyConceptPilingLabelSet();
					cint64 pilingDepth = processIndi->getPilingDepth();
					addConceptFilteredToIndividual(addingConcept,negate,rootProcessIndi,labelSet,pilingDepth,updateCopyDependedIndividual,calcAlgContext);
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& rootProcessIndi, cint64 pilingDepth, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptPilingLabelSet* labelSet = rootProcessIndi->getReapplyConceptPilingLabelSet();
					addConceptFilteredToIndividual(addingConcept,negate,rootProcessIndi,labelSet,pilingDepth,updateCopyDependedIndividual,calcAlgContext);
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& rootProcessIndi, CReapplyConceptPilingLabelSet* labelSet, cint64 pilingDepth, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 opCode = addingConcept->getOperatorCode();
					bool insertConcept = false;
					CSortedNegLinker<CConcept*>* opConLinkerIt = addingConcept->getOperandList();
					if (!negate && (opCode == CCAND || opCode == CCAQAND || opCode == CCIMPLAQAND || opCode == CCBRANCHAQAND) || negate && opCode == CCOR) {
						addConceptsFilteredToIndividual(opConLinkerIt,negate,rootProcessIndi,pilingDepth,updateCopyDependedIndividual,calcAlgContext);
					} else if (opCode == CCAQCHOOCE) {
						while (opConLinkerIt) {
							CConcept* opConcept = opConLinkerIt->getData();
							bool opNegation = opConLinkerIt->isNegated();
							if (opNegation == negate) {
								addConceptFilteredToIndividual(opConcept,false,rootProcessIndi,labelSet,pilingDepth,updateCopyDependedIndividual,calcAlgContext);
							}
							opConLinkerIt = opConLinkerIt->getNext();
						}
					} else if (mConfImplicationAddingSkipping && opCode == CCIMPL) {
						insertConcept = false;
					} else {
						insertConcept = true;
					}
					if (insertConcept) {
						addConceptToIndividual(addingConcept,negate,rootProcessIndi,labelSet,pilingDepth,updateCopyDependedIndividual,calcAlgContext);
					}
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::addConceptToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& rootProcessIndi, CReapplyConceptPilingLabelSet* labelSet, cint64 pilingDepth, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);

					CConceptPilingDescriptor* conceptPilingDescriptor = createConceptPilingDescriptor(calcAlgContext);
					conceptPilingDescriptor->initConceptPilingDescriptor(addingConcept,negate);

					bool contained = insertConceptToIndividualConceptSet(conceptPilingDescriptor,pilingDepth,rootProcessIndi,labelSet,calcAlgContext);
					if (!contained) {
						STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);

						CConceptPilingProcessLinker* conceptPilingProcessLinker = createConceptPilingProcessLinker(calcAlgContext);
						conceptPilingProcessLinker->initConceptPilingProcessLinker(conceptPilingDescriptor,pilingDepth);
						rootProcessIndi->addConceptPilingProcessLinker(conceptPilingProcessLinker);
						addIndividualToProcessingQueue(rootProcessIndi,calcAlgContext);

						if (updateCopyDependedIndividual && rootProcessIndi->hasCopyDependingIndividualNodeLinker()) {
							// add concept to copy dependent individual nodes
							CXLinker<CIndividualPilingProcessNode*>* copyDepIndiLinkerIt = rootProcessIndi->getCopyDependingIndividualNodeLinker();
							while (copyDepIndiLinkerIt) {
								CIndividualPilingProcessNode* copyDepIndiNode = copyDepIndiLinkerIt->getData();
								cint64 copyDepPilingDepth = copyDepIndiNode->getPilingDepth();
								if (pilingDepth <= copyDepPilingDepth) {
									addConceptToIndividual(addingConcept,negate,copyDepIndiNode,copyDepIndiNode->getReapplyConceptPilingLabelSet(true),0,true,calcAlgContext);
								}
								copyDepIndiLinkerIt = copyDepIndiLinkerIt->getNext();
							}
						}

					} else {
						// TODO: may the concept piling descriptor can be released
						//releaseConceptPilingDescriptor(conceptPilingDescriptor,calcAlgContext);
					}
				}


				CConceptPilingDescriptor* CCalculationTableauPilingSaturationTaskHandleAlgorithm::createConceptPilingDescriptor(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CConceptPilingDescriptor* conPilDes = processingDataBox->takeRemainingConceptPilingDescriptor();
					if (!conPilDes) {
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						conPilDes = CObjectAllocator<CConceptPilingDescriptor>::allocateAndConstruct(taskMemMan);
					}
					return conPilDes;
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::releaseConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					conPilDes->clearNext();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					processingDataBox->addRemainingConceptPilingDescriptor(conPilDes);
				}

				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::releaseConceptPilingProcessLinker(CConceptPilingProcessLinker* conPilProcLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					conPilProcLinker->clearNext();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					processingDataBox->addRemainingConceptPilingProcessLinker(conPilProcLinker);
				}


				CConceptPilingProcessLinker* CCalculationTableauPilingSaturationTaskHandleAlgorithm::createConceptPilingProcessLinker(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CConceptPilingProcessLinker* conPilProcLinker = processingDataBox->takeRemainingConceptPilingProcessLinker();
					if (!conPilProcLinker) {
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						conPilProcLinker = CObjectAllocator<CConceptPilingProcessLinker>::allocateAndConstruct(taskMemMan);
					}
					return conPilProcLinker;
				}


				CImplicationReapplyConceptPilingDescriptor* CCalculationTableauPilingSaturationTaskHandleAlgorithm::createImplicationReapplyConceptPilingDescriptor(CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CImplicationReapplyConceptPilingDescriptor* impReaConPilDes = CObjectAllocator<CImplicationReapplyConceptPilingDescriptor>::allocateAndConstruct(taskMemMan);
					return impReaConPilDes;
				}




				bool CCalculationTableauPilingSaturationTaskHandleAlgorithm::insertConceptToIndividualConceptSet(CConceptPilingDescriptor* conceptPilingDescriptor, cint64 pilingDepth, CIndividualPilingProcessNode*& rootProcessIndi, CReapplyConceptPilingLabelSet* labelSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool contained = false;
					bool insertConcept = true;
					bool implTriggerGeneration = false;
					bool requiredBackProp = rootProcessIndi->getRequiredBackwardPropagation();
					CConcept* concept = conceptPilingDescriptor->getConcept();
					bool conNeg = conceptPilingDescriptor->getNegation();
					cint64 opCode = concept->getOperatorCode();
					if (opCode == CCATOM) {
						++mAddedSUBConcepts;
						contained = true;
					} else if (opCode == CCSUB) {
						++mAddedSUBConcepts;
					} else if (!conNeg && (opCode == CCALL || opCode == CCIMPLALL)) {
						++mAddedALLConcepts;
						insertConcept = false;
						contained = !requiredBackProp;
					} else if ((!conNeg && (opCode == CCSOME || opCode == CCAQSOME)) || (conNeg && opCode == CCALL)) {
						++mAddedSOMEConcepts;
						//insertConcept = false;
					} else if (opCode == CCIMPL) {
						++mAddedIMPLConcepts;
						if (!requiredBackProp && !hasConceptLocalImpact(concept,false,calcAlgContext)) {
							insertConcept = false;
							contained = true;
						} else {
							implTriggerGeneration = true;
							contained = true;
						}
					} else if (opCode == CCIMPLTRIG || opCode == CCBRANCHTRIG) {
						++mAddedTRIGGConcepts;
					} else if (opCode == CCAQCHOOCE) {
						insertConcept = false;
						++mAddedELSEConcepts;
					} else {
						++mAddedELSEConcepts;
					}
					if (insertConcept) {
						bool newInsertion = false;
						bool insertionUpdate = false;
						cint64 prevPilingDepth = pilingDepth;
						CImplicationReapplyConceptPilingDescriptor* reapplyImpReapplyConPilDes = nullptr;
						CImplicationReapplyConceptPilingDescriptor** reapplyImpReapplyConPilDesPtr = nullptr;
						bool clashed = labelSet->insertConceptReturnClashed(conceptPilingDescriptor,pilingDepth,&newInsertion,&reapplyImpReapplyConPilDes,&insertionUpdate,&prevPilingDepth,&reapplyImpReapplyConPilDesPtr);

						if (!clashed) {
							if (newInsertion && implTriggerGeneration) {
								// add first trigger
								CImplicationReapplyConceptPilingDescriptor* newReapplyImpReapplyConPilDes = createImplicationReapplyConceptPilingDescriptor(calcAlgContext);
								CConcept* implConcept = conceptPilingDescriptor->getConcept();
								CSortedNegLinker<CConcept*>* nextTriggerConcept = implConcept->getOperandList()->getNext();
								newReapplyImpReapplyConPilDes->initImplicationReapllyConceptPilingDescriptor(conceptPilingDescriptor,nextTriggerConcept,conceptPilingDescriptor,pilingDepth,0,nullptr);
								if (reapplyImpReapplyConPilDes) {
									*reapplyImpReapplyConPilDesPtr = newReapplyImpReapplyConPilDes->setNext(reapplyImpReapplyConPilDes);
									reapplyImpReapplyConPilDes = newReapplyImpReapplyConPilDes;
								} else {
									*reapplyImpReapplyConPilDesPtr = newReapplyImpReapplyConPilDes;
									reapplyImpReapplyConPilDes = newReapplyImpReapplyConPilDes;
								}
							}
							if (newInsertion && mConfImplicationAddingSkipping) {
								if (opCode == CCSUB || opCode == CCIMPLTRIG || opCode == CCBRANCHTRIG) {
									CSortedNegLinker<CConcept*>* opConceptLinkerIt = concept->getOperandList();
									while (opConceptLinkerIt) {
										if (!opConceptLinkerIt->isNegated()) {
											CConcept* opConcept = opConceptLinkerIt->getData();
											if (opConcept->getOperatorCode() == CCIMPL) {

												CConcept* implConcept = opConcept;

												CConceptPilingDescriptor* implicationConceptPilingDescriptor = createConceptPilingDescriptor(calcAlgContext);
												implicationConceptPilingDescriptor->initConceptPilingDescriptor(implConcept,false);
												CImplicationReapplyConceptPilingDescriptor* newReapplyImpReapplyConPilDes = createImplicationReapplyConceptPilingDescriptor(calcAlgContext);
												CSortedNegLinker<CConcept*>* nextTriggerConcept = implConcept->getOperandList()->getNext();
												newReapplyImpReapplyConPilDes->initImplicationReapllyConceptPilingDescriptor(conceptPilingDescriptor,nextTriggerConcept,implicationConceptPilingDescriptor,pilingDepth,0,nullptr);
												if (reapplyImpReapplyConPilDes) {
													*reapplyImpReapplyConPilDesPtr = newReapplyImpReapplyConPilDes->setNext(reapplyImpReapplyConPilDes);
													reapplyImpReapplyConPilDes = newReapplyImpReapplyConPilDes;
												} else {
													*reapplyImpReapplyConPilDesPtr = newReapplyImpReapplyConPilDes;
													reapplyImpReapplyConPilDes = newReapplyImpReapplyConPilDes;
												}

											}
										}
										opConceptLinkerIt = opConceptLinkerIt->getNext();
									}
								}
							}
							if (insertionUpdate) {

								// debug test call
								//testDebugSaturationTaskContainsConcept(concept,conNeg,rootProcessIndi,calcAlgContext);

								if (reapplyImpReapplyConPilDes) {
									CImplicationReapplyConceptPilingDescriptor* reapplyImpReapplyConPilDesIt = reapplyImpReapplyConPilDes;
									while (reapplyImpReapplyConPilDesIt) {
										bool updateTrigger = false;
										if (reapplyImpReapplyConPilDesIt->getPreviousMaxPilingDepth() <= pilingDepth) {
											updateTrigger = true;
										}
										reapplyImpReapplyConPilDesIt->setUpdatedPilingDepth(pilingDepth);
										if (updateTrigger) {
											// update trigger
											updateImplicationReapplyConceptPilingDescriptor(reapplyImpReapplyConPilDesIt,reapplyImpReapplyConPilDesIt->getPreviousMaxPilingDepth(),rootProcessIndi,labelSet,calcAlgContext);
										}
										reapplyImpReapplyConPilDesIt = reapplyImpReapplyConPilDesIt->getNext();
									}
								}
							} else {
								contained = true;
							}
						} else {
							// TODO: treat clash
						}
					}
					return contained;
				}

				bool CCalculationTableauPilingSaturationTaskHandleAlgorithm::updateImplicationReapplyConceptPilingDescriptor(CImplicationReapplyConceptPilingDescriptor* reapplyImpReapplyConPilDes, cint64 prevPilingDepth, CIndividualPilingProcessNode*& rootProcessIndi, CReapplyConceptPilingLabelSet* labelSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(IMPLICATIONTRIGGERINGCOUNT,calcAlgContext);
					CSortedNegLinker<CConcept*>* nextTriggerConcept = reapplyImpReapplyConPilDes->getNextTriggerConcept();
					cint64 nextPrevMaxPilingDepth = qMax(prevPilingDepth,reapplyImpReapplyConPilDes->getPilingDepth());
					if (!nextTriggerConcept) {
						// execute implication
						STATINC(IMPLICATIONEXECUTINGCOUNT,calcAlgContext);
						CConceptPilingDescriptor* impConPilDes = reapplyImpReapplyConPilDes->getImplicationConceptPilingDescriptor();
						CConcept* implConcept = impConPilDes->getConcept();
						CSortedNegLinker<CConcept*>* impExConOpLinker = implConcept->getOperandList();
						addConceptFilteredToIndividual(impExConOpLinker->getData(),impExConOpLinker->isNegated(),rootProcessIndi,labelSet,nextPrevMaxPilingDepth,false,calcAlgContext);
					} else {
						CImplicationReapplyConceptPilingDescriptor* nextReapplyImpReapplyConPilDes = reapplyImpReapplyConPilDes->getNextReapplyConceptPilingDescriptor();
						if (nextReapplyImpReapplyConPilDes) {
							if (nextPrevMaxPilingDepth <= nextReapplyImpReapplyConPilDes->getPreviousMaxPilingDepth()) {
								nextReapplyImpReapplyConPilDes->setUpdatedPreviousMaxPilingDepth(nextPrevMaxPilingDepth);
								updateImplicationReapplyConceptPilingDescriptor(nextReapplyImpReapplyConPilDes,nextPrevMaxPilingDepth,rootProcessIndi,labelSet,calcAlgContext);
							}
						} else {
							CConcept* nextTrigger = nextTriggerConcept->getData();
							CSortedNegLinker<CConcept*>* nextNextTriggerConcept = nextTriggerConcept->getNext();

							// add next trigger
							CImplicationReapplyConceptPilingDescriptor* newReapplyImpReapplyConPilDes = createImplicationReapplyConceptPilingDescriptor(calcAlgContext);
							newReapplyImpReapplyConPilDes->initImplicationReapllyConceptPilingDescriptor(nullptr,nextNextTriggerConcept,reapplyImpReapplyConPilDes->getImplicationConceptPilingDescriptor(),0,nextPrevMaxPilingDepth,nullptr);

							CConceptPilingDescriptor* conPilDes = nullptr;
							cint64 pilingDepth = 0;
							bool triggered = labelSet->insertConceptReapplicationReturnTriggered(nextTrigger->getConceptTag(),newReapplyImpReapplyConPilDes,&conPilDes,&pilingDepth);
							newReapplyImpReapplyConPilDes->setConceptPilingDescriptor(conPilDes);
							newReapplyImpReapplyConPilDes->setPilingDepth(pilingDepth);
							newReapplyImpReapplyConPilDes->setPreviousMaxPilingDepth(nextPrevMaxPilingDepth);
							reapplyImpReapplyConPilDes->setNextReapplyConceptPilingDescriptor(newReapplyImpReapplyConPilDes);
							if (triggered) {
								updateImplicationReapplyConceptPilingDescriptor(newReapplyImpReapplyConPilDes,nextPrevMaxPilingDepth,rootProcessIndi,labelSet,calcAlgContext);
							}
						}
					}
					return true;
				}


				bool CCalculationTableauPilingSaturationTaskHandleAlgorithm::hasConceptLocalImpact(CConcept* concept, bool conNeg, CCalculationAlgorithmContextBase* calcAlgContext) {
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



				cint64 CCalculationTableauPilingSaturationTaskHandleAlgorithm::getAppliedANDRuleCount() {
					return mAppliedANDRuleCount;
				}

				cint64 CCalculationTableauPilingSaturationTaskHandleAlgorithm::getAppliedORRuleCount() {
					return mAppliedORRuleCount;
				}

				cint64 CCalculationTableauPilingSaturationTaskHandleAlgorithm::getAppliedSOMERuleCount() {
					return mAppliedSOMERuleCount;
				}

				cint64 CCalculationTableauPilingSaturationTaskHandleAlgorithm::getAppliedATLEASTRuleCount() {
					return mAppliedATLEASTRuleCount;
				}

				cint64 CCalculationTableauPilingSaturationTaskHandleAlgorithm::getAppliedALLRuleCount() {
					return mAppliedALLRuleCount;
				}

				cint64 CCalculationTableauPilingSaturationTaskHandleAlgorithm::getAppliedATMOSTRuleCount() {
					return mAppliedATMOSTRuleCount;
				}

				cint64 CCalculationTableauPilingSaturationTaskHandleAlgorithm::getAppliedTotalRuleCount() {
					return mAppliedTOTALRuleCount;
				}


				QString CCalculationTableauPilingSaturationTaskHandleAlgorithm::generateExtendedDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext, cint64 firstIndiID, cint64 lastIndiID) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualPilingProcessNodeVector* indiVec = procDataBox->getIndividualPilingProcessNodeVector();
					cint64 indiCount = indiVec->getItemCount();
					if (lastIndiID >= 0) {
						indiCount = qMin(lastIndiID,indiCount);
					}
					firstIndiID = qMin(firstIndiID,indiCount);
					QStringList indiStringList;
					for (cint64 i = firstIndiID; i < indiCount; ++i) {
						CIndividualPilingProcessNode* indi = indiVec->getData(i);
						cint64 pilingDepth = indi->getPilingDepth();
						CReapplyConceptPilingLabelSet* conSet = indi->getRootPilingIndividualNode()->getReapplyConceptPilingLabelSet(false);
						if (conSet) {
							QString nominalString = QString("");
							CXSortedNegLinker<CConcept*>* initializedConceptLinker = indi->getInitializedConceptLinker();
							if (initializedConceptLinker) {
								CConcept* initializedConcept = initializedConceptLinker->getData();
								if (initializedConcept->hasClassName()) {
									nominalString = QString(", ")+CIRIName::getRecentIRIName(initializedConcept->getClassNameLinker());
								}
							}
							//if (indi->getNominalIndividual()) {
							//	nominalString = QString(", ")+CIRIName::getRecentIRIName(indi->getNominalIndividual()->getNameLinker());
							//}
							QString refModeString("[new root node]");
							CIndividualPilingProcessNode* refIndi = indi->getReferenceIndividualPilingProcessNode();
							if (refIndi) {
								cint64 refIndiID = refIndi->getIndividualID();
								if (refIndi->getReferenceMode() == 1) {
									refModeString = QString("[substitute node %1]").arg(refIndiID);
								} else if (refIndi->getReferenceMode() == 2) {
									refModeString = QString("[copy node %1]").arg(refIndiID);
								} else if (refIndi->getReferenceMode() == 3) {
									refModeString = QString("[piling node %1]").arg(refIndiID);
								}
							}
							QString indiString = QString("[ %1%2 ] = %3<br>\n").arg(indi->getIndividualID()).arg(nominalString).arg(refModeString);
							QString conSetString;
							CReapplyConceptPilingLabelSetIterator conSetIt = conSet->getIterator(true,false,pilingDepth);
							while (conSetIt.hasNext()) {
								CConceptPilingDescriptor* conDes = conSetIt.getConceptPilingDescriptor();
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
								conSetIt.moveNext();
							}
							QString propString;
							CRoleBackwardPilingPropagationHash* backwardPropHash = indi->getRoleBackwardPropagationHash(false);
							if (backwardPropHash) {
								CPROCESSHASH<CRole*,CRoleBackwardPilingPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
								for (CPROCESSHASH<CRole*,CRoleBackwardPilingPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
									CRole* backwardPropRole = it.key();
									CRoleBackwardPilingPropagationHashData backwardPropData = it.value();
									if (backwardPropData.mMinReapplyPilingDepth <= pilingDepth) {
										QString roleString;
										roleString += QString::number(backwardPropRole->getRoleTag());
										if (backwardPropRole->hasPropertyName()) {
											roleString += QString("~")+CIRIName::getRecentIRIName(backwardPropRole->getPropertyNameLinker());
										}

										QString sourceIndiListString;
										CBackwardPilingPropagationLink* backPropLinkIt = backwardPropData.mLinkLinker;
										while (backPropLinkIt) {
											if (backPropLinkIt->getPilingDepth() <= pilingDepth) {
												CIndividualPilingProcessNode* sourceIndiNode = backPropLinkIt->getSourceIndividual();
												if (!sourceIndiListString.isEmpty()) {
													sourceIndiListString.append(", ");
												} else {
													sourceIndiListString.append("::->");
												}
												sourceIndiListString += QString::number(sourceIndiNode->getIndividualID());
											}
											backPropLinkIt = backPropLinkIt->getNext();
										}

										QString roleBackPropString = QString("%1:%2").arg(roleString).arg(sourceIndiListString);
										propString += QString("<br>\n%1").arg(roleBackPropString);
									}
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



				QString CCalculationTableauPilingSaturationTaskHandleAlgorithm::generateDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualPilingProcessNodeVector* indiVec = procDataBox->getIndividualPilingProcessNodeVector();
					cint64 indiCount = indiVec->getItemCount();
					QStringList indiStringList;
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualPilingProcessNode* indi = indiVec->getData(i);
						cint64 pilingDepth = indi->getPilingDepth();
						CReapplyConceptPilingLabelSet* conSet = indi->getRootPilingIndividualNode()->getReapplyConceptPilingLabelSet(false);
						if (conSet) {
							QString nominalString = QString("");
							//if (indi->getNominalIndividual()) {
							//	nominalString = QString(", ")+CIRIName::getRecentIRIName(indi->getNominalIndividual()->getNameLinker());
							//}
							QString indiString = QString("[ %1%2 ] = ").arg(indi->getIndividualID()).arg(nominalString);
							QString conSetString;
							CReapplyConceptPilingLabelSetIterator conSetIt = conSet->getIterator(true,false,pilingDepth);
							while (conSetIt.hasNext()) {
								CConceptPilingDescriptor* conDes = conSetIt.getConceptPilingDescriptor();
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
								conSetIt.moveNext();
							}
							indiString += QString("{%1} ").arg(conSetString);
							indiStringList.append(indiString);
						}
					}

					mDebugIndiModelStringList = indiStringList;
					mDebugIndiModelString = mDebugIndiModelStringList.join("\n");
					return mDebugIndiModelString;
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::createDebugSatisfiableSaturationTestingTask(CCalculationAlgorithmContextBase* calcAlgContext) {
					CGeneratorTaskHandleContextBase genContextBase;
					CSatisfiableCalculationTaskFromCalculationJobGenerator gen(&genContextBase);
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualPilingProcessNodeVector* indiVec = processingDataBox->getIndividualPilingProcessNodeVector(false);
					if (indiVec) {
						CSaturationCalculationJob* satCalcJob = new CSaturationCalculationJob();
						satCalcJob->setCalculationConfiguration(calcAlgContext->getUsedSatisfiableCalculationTask()->getCalculationConfiguration());
						satCalcJob->setOntology(processingDataBox->getOntology());
						cint64 indiCount = indiVec->getItemCount();
						for (cint64 indiID = indiCount-1; indiID >= 0; --indiID) {
							CIndividualPilingProcessNode* indiNode = indiVec->getData(indiID);
							for (CXSortedNegLinker<CConcept*>* initConLinkerIt = indiNode->getInitializingConceptLinker(); initConLinkerIt; initConLinkerIt = initConLinkerIt->getNext()) {
								CSaturationCalculationConstruct* satCalcConstruct = new CSaturationCalculationConstruct();
								satCalcConstruct->setConstructConcept(initConLinkerIt->getData());
								satCalcConstruct->setConstructConceptNegation(initConLinkerIt->isNegated());	
								satCalcJob->addSatisfiableCalculationConstruct(satCalcConstruct);
							}
						}
						mDebugTestingSaturationTask = gen.createDefaultSaturationCalculationTask(processingDataBox->getOntology(),satCalcJob,nullptr);
						if (!mDebugSaturationAlgorithm) {
							mDebugSaturationAlgorithm = new CCalculationTableauSaturationTaskHandleAlgorithm();
						}
						mDebugSaturationAlgorithm->handleTask(calcAlgContext->getUsedTaskProcessorContext(),mDebugTestingSaturationTask);

						for (cint64 indiID = indiCount-1; indiID >= 0; --indiID) {
							CIndividualPilingProcessNode* indiNode = indiVec->getData(indiID);
							for (CXSortedNegLinker<CConcept*>* initConLinkerIt = indiNode->getInitializingConceptLinker(); initConLinkerIt; initConLinkerIt = initConLinkerIt->getNext()) {
								CConcept* concept = initConLinkerIt->getData();
								CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
								if (conProData) {
									CConceptSaturationReferenceLinkingData* conRefLinking = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
									if (conRefLinking) {
										CClassificationConceptReferenceLinking* classSatCalcRefLinkData = conRefLinking->getClassifierReferenceLinkingData();
										if (classSatCalcRefLinkData) {
											classSatCalcRefLinkData->setIndividualProcessNodeForConcept(indiNode);
										}
									}
								}
							}
						}

						delete satCalcJob;
					} else {
						mDebugTestingSaturationTask = nullptr;
					}
				}



				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::testDebugSaturationTaskContainsConcepts(CIndividualPilingProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptPilingLabelSet* conSet = indiNode->getReapplyConceptPilingLabelSet();
					if (conSet) {
						CReapplyConceptPilingLabelSetIterator it = conSet->getIterator(true,false,0);
						while (it.hasNext()) {
							CConceptPilingDescriptor* conPilDes = it.getConceptPilingDescriptor();
							if (conPilDes) {
								CConcept* concept = conPilDes->getConcept();
								bool conceptNegation = conPilDes->getNegation();
								testDebugSaturationTaskContainsConcept(concept,conceptNegation,indiNode,calcAlgContext);
							}
							it.moveNext();
						}
					}
				}


				void CCalculationTableauPilingSaturationTaskHandleAlgorithm::testDebugSaturationTaskContainsConcept(CConcept* concept, bool conNegation, CIndividualPilingProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mDebugTestingSaturationTask) {
						cint64 individualID = indiNode->getIndividualID();
						CProcessingDataBox* procDataBox = mDebugTestingSaturationTask->getProcessingDataBox();
						if (procDataBox) {
							CIndividualProcessNodeVector* indiVec = procDataBox->getIndividualProcessNodeVector();
							CIndividualProcessNode* indiNode = indiVec->getData(individualID);
							CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
							bool contained = conSet->containsConcept(concept,conNegation);
							if (!contained) {

								mDebugTestSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
								QFile tmpFile("tmp4.txt");
								if (tmpFile.open(QIODevice::WriteOnly)) {
									tmpFile.write(mDebugTestSaturationDebugIndiModelString.toLocal8Bit());
									tmpFile.close();
								}

								bool bug = true;
							}
						}
					}
				}

			}; // end namespace Algorithm 

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
