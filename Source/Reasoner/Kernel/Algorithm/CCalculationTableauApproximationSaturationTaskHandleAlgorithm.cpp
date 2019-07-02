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

#include "CCalculationTableauApproximationSaturationTaskHandleAlgorithm.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CCalculationTableauApproximationSaturationTaskHandleAlgorithm::CCalculationTableauApproximationSaturationTaskHandleAlgorithm() {
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


					mPosJumpFuncVec[CCBOTTOM] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyBOTTOMRule;
					mPosJumpFuncVec[CCATOM] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyNONERule;
					mPosJumpFuncVec[CCTOP] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCAND] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCAQAND] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCIMPLAQAND] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCBRANCHAQAND] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCSUB] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCALL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCAQALL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCSOME] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applySOMERule;
					mPosJumpFuncVec[CCAQSOME] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applySOMERule;
					mPosJumpFuncVec[CCAQCHOOCE] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyAutomatChooseRule;
					mPosJumpFuncVec[CCIMPLTRIG] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCBRANCHTRIG] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCIMPLALL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCBRANCHALL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCBRANCHAQALL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCIMPLAQALL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyALLRule;
					mPosJumpFuncVec[CCOR] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyORRule;
					mPosJumpFuncVec[CCIMPL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyIMPLICATIONRule;
					mPosJumpFuncVec[CCBRANCHIMPL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyIMPLICATIONRule;
					mPosJumpFuncVec[CCEQCAND] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyEQCANDRule;
					mPosJumpFuncVec[CCEQ] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mPosJumpFuncVec[CCSELF] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applySELFRule;
				
					mPosJumpFuncVec[CCATLEAST] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyATLEASTRule;
					mNegJumpFuncVec[CCATMOST] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyATLEASTRule;

					mPosJumpFuncVec[CCATMOST] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyATMOSTRule;
					mNegJumpFuncVec[CCATLEAST] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyATMOSTRule;

					mNegJumpFuncVec[CCOR] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule;
					mNegJumpFuncVec[CCALL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applySOMERule;
					mNegJumpFuncVec[CCSOME] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyALLRule;
					mNegJumpFuncVec[CCAQCHOOCE] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyAutomatChooseRule;

					mNegJumpFuncVec[CCAND] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyORRule;
					mNegJumpFuncVec[CCEQ] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyORRule;
					mNegJumpFuncVec[CCSUB] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyNONERule;
					mNegJumpFuncVec[CCATOM] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyNONERule;

					mPosJumpFuncVec[CCVALUE] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyVALUERule;
					mPosJumpFuncVec[CCNOMINAL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyNOMINALRule;


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
					mCopiedIndiNodeCount = 0;

					mALLSuccExtInitializedCount = 0;

					mDirectUpdatedStatusIndiNodeCount = 0;
					mIndirectUpdatedStatusIndiNodeCount = 0;

					mDisjunctionInitializedSkippedCount = 0;

					mDebugSaturationAlgorithm = nullptr;
					mDebugTestingSaturationTask = nullptr;

					mConfImplicationAddingSkipping = true;
					mConfForceAllConceptInsertion = false;
					mConfDebuggingWriteData = false;
					mConfForceAllCopyInsteadOfSubstituition = false;

					mConfAddCriticalConceptsToQueues = false;
					mConfDirectlyCriticalToInsufficient = true;
					mConfCheckCriticalConcepts = false;
					mConfALLConceptsExtensionProcessing = false;
					mConfFUNCTIONALConceptsExtensionProcessing = false;
					mConfConceptsExtensionProcessing = false;
				}


				CCalculationTableauApproximationSaturationTaskHandleAlgorithm::~CCalculationTableauApproximationSaturationTaskHandleAlgorithm() {
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::readCalculationConfig(CSatisfiableCalculationTask* satCalcTask) {
					CCalculationConfigurationExtension *config = satCalcTask->getCalculationConfiguration();
					if (config != mLastConfig) {
						if (config) {
							CConcreteOntology* ontology = satCalcTask->getProcessingDataBox()->getOntology();
							COntologyStructureSummary* ontStructureSummary = ontology->getStructureSummary();
							mConfForceAllConceptInsertion = false;
							mConfForceAllCopyInsteadOfSubstituition = false;
							mConfImplicationAddingSkipping = true;
							mConfDebuggingWriteData = config->isDebuggingWriteDataActivated();
							if (!ontStructureSummary || !ontStructureSummary->hasOnlyELConceptClasses() || ontology->getABox()->getIndividualCount() > 0) {
								mConfForceAllConceptInsertion = true;
								mConfImplicationAddingSkipping = false;
							}

							mConfDirectlyCriticalToInsufficient = config->isSaturationDirectCriticalToInsufficientActivated();
							mConfAddCriticalConceptsToQueues = config->isSaturationCriticalConceptTestingActivated();
							mConfCheckCriticalConcepts = config->isSaturationCriticalConceptTestingActivated();
							mConfConceptsExtensionProcessing = config->isSaturationSuccessorExtensionActivated();
							mConfNominalProcessing = config->isNominalSaturationActivated();

						} else {
							mConfForceAllCopyInsteadOfSubstituition = false;
							mConfForceAllConceptInsertion = true;
							mConfImplicationAddingSkipping = true;
							mConfNominalProcessing = true;
							mConfDebuggingWriteData = false;

							mConfDirectlyCriticalToInsufficient = true;
							mConfAddCriticalConceptsToQueues = false;
							mConfCheckCriticalConcepts = false;
							mConfConceptsExtensionProcessing = false;
						}

						mConfALLConceptsExtensionProcessing = mConfConceptsExtensionProcessing;
						mConfFUNCTIONALConceptsExtensionProcessing = mConfConceptsExtensionProcessing;

						mLastConfig = config;
					}

				}


				CCalculationAlgorithmContextBase* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createCalculationAlgorithmContext(CTaskProcessorContext *processorContext, CProcessContext* processContext, CSatisfiableCalculationTask* satCalcTask) {
					CCalculationAlgorithmContextBase* calcAlgContext = CObjectAllocator< CCalculationAlgorithmContextBase >::allocateAndConstruct(processContext->getUsedMemoryAllocationManager());
					calcAlgContext->initTaskProcessContext(processContext,satCalcTask);
					calcAlgContext->initCalculationAlgorithmContext(processorContext);
					return calcAlgContext;
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::handleTask(CTaskProcessorContext *processorContext, CTask* task) {

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


						mCachedCompletionGraphLoaded = false;
						mCachedCompletionGraphMissing = false;
						mDetCachedCGIndiVector = nullptr;
						mNonDetCachedCGIndiVector = nullptr;
						mNonDetConsistencyCG = false;
						mDetConsistencyCG = false;


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

						CIndividualSaturationProcessNode* indiProcSatNode = nullptr;
						CConceptProcessDescriptor* conProcDes = nullptr;

						cint64 processRuleToTaskProcessingVerificationCount = 10;
						cint64 remainProcessRuleToTaskProcessingVerification = processRuleToTaskProcessingVerificationCount;

						CIndividualSaturationProcessNode* lastindiProcSatNode = nullptr;
						try {
							STATINC(TASKPROCESSCHANGECOUNT,calcAlgContext);


							continueNominalDelayedIndividualNodeProcessing(mProcessingDataBox,calcAlgContext);


							while (hasRemainingExtensionProcessingNodes(mProcessingDataBox,calcAlgContext)) {

								while (hasRemainingProcessingNodes(mProcessingDataBox,calcAlgContext)) {

									while (mProcessingDataBox->hasIndividualSaturationProcessNodeLinker()) {
										CIndividualSaturationProcessNodeLinker* indiProcessSaturationNodeLinker = mProcessingDataBox->takeIndividualSaturationProcessNodeLinker();
										CIndividualSaturationProcessNode* indiProcSatNode = indiProcessSaturationNodeLinker->getData();
										lastindiProcSatNode = indiProcSatNode;
										STATINC(INDIVIDUALNODESWITCHCOUNT,calcAlgContext);
										++indiProcessedCount;
										KONCLUCE_TASK_ALGORITHM_SATURATION_SATURATION_MODEL_STRING_INSTRUCTION(mRuleBeginDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
										if (individualNodeInitializing(indiProcSatNode,calcAlgContext)) {
											CConceptSaturationProcessLinker* conceptSaturationProcessLinker = indiProcSatNode->takeConceptSaturationProcessLinker();
											while (conceptSaturationProcessLinker) {
												STATINC(RULEAPPLICATIONCOUNT,calcAlgContext);
												KONCLUCE_TASK_ALGORITHM_SATURATION_SATURATION_MODEL_STRING_INSTRUCTION(mRuleBeginDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
												applyTableauSaturationRule(indiProcSatNode,conceptSaturationProcessLinker);
												KONCLUCE_TASK_ALGORITHM_SATURATION_SATURATION_MODEL_STRING_INSTRUCTION(mRuleEndDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
												releaseConceptSaturationProcessLinker(conceptSaturationProcessLinker,calcAlgContext);
												conceptSaturationProcessLinker = indiProcSatNode->takeConceptSaturationProcessLinker();
											}
										}
										indiProcessSaturationNodeLinker->clearProcessingQueued();
										individualNodeConclusion(indiProcSatNode,calcAlgContext);
									}


									if (mProcessingDataBox->hasIndividualDisjunctCommonConceptExtractProcessLinker()) {
										CIndividualSaturationProcessNodeLinker* indiDisjCommonConExtProcessLinker = mProcessingDataBox->takeIndividualDisjunctCommonConceptExtractProcessLinker();
										indiDisjCommonConExtProcessLinker->setProcessingQueued(false);
										CIndividualSaturationProcessNode* indiProcSatNode = indiDisjCommonConExtProcessLinker->getData();
										lastindiProcSatNode = indiProcSatNode;
										STATINC(INDIVIDUALNODESWITCHCOUNT,calcAlgContext);
										++indiProcessedCount;
										if (individualNodeInitializing(indiProcSatNode,calcAlgContext)) {
											updateExtractDisjunctCommonConcept(indiProcSatNode,calcAlgContext);
										}
										individualNodeConclusion(indiProcSatNode,calcAlgContext);
									}
								}


								processNextSuccessorExtensions(calcAlgContext);

							}


							if (mConfCheckCriticalConcepts && hasNextCriticalConcepts(mCalcAlgContext)) {
								while (hasNextCriticalConcepts(mCalcAlgContext)) {
									checkNextCriticalConcepts(mCalcAlgContext);
								}
							}


							completeSaturatedIndividualNodes(mProcessingDataBox,mCalcAlgContext);


							if (mConfDebuggingWriteData) {
								mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
								QFile tmpFile("saturation-model.txt");
								if (tmpFile.open(QIODevice::WriteOnly)) {
									tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
									tmpFile.close();
								}
							}
							satisfiable = true;

							//testRelevantConceptRoleRatio(calcAlgContext);

						} catch (const CCalculationErrorProcessingException& calcErrorProcException) {
							if (calcErrorProcException.hasError()) {
								error = true;
								errorCode = (cint64)calcErrorProcException.getErrorCode();
								LOG(ERROR,"::Konclude::Reasoner::Kernel::Algorithm::TableauSaturationAlgorihm",logTr("Error occured, computation stopped."),this);
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

							//mSubsumAnalyser.analyseSatisfiableTask(satCalcTask,calcAlgContext);

							mSatTaskSaturationPreyAnalyser.analyseSatisfiableTask(satCalcTask,mCalcAlgContext);

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


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::testRelevantConceptRoleRatio(CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 totalConceptCount = 0;
					cint64 relevantConceptCount = 0;
					cint64 totalRoleCount = 0;
					cint64 relevantRoleCount = 0;
					CIndividualSaturationProcessNodeVector* indiVec = calcAlgContext->getProcessingDataBox()->getIndividualSaturationProcessNodeVector(false);
					if (indiVec) {
						cint64 indiCount = indiVec->getItemCount();
						for (cint64 i = 0; i < indiCount; ++i) {
							CIndividualSaturationProcessNode* indi = indiVec->getData(i);
							if (indi) {
								CReapplyConceptSaturationLabelSet* conSet = indi->getReapplyConceptSaturationLabelSet(false);
								if (conSet) {
									CConceptSaturationDescriptor* conDesIt = conSet->getConceptSaturationDescriptionLinker();
									while (conDesIt) {
										bool relevantFlag = false;
										CConceptSaturationDescriptor* conDes = conDesIt;
										CConcept* concept = conDes->getConcept();
										CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
										if (conProData) {
											if (conProData->hasInferRelevantFlag()) {
												relevantFlag = true;
											}
										}
										++totalConceptCount;
										if (relevantFlag) {
											++relevantConceptCount;
										}
										conDesIt = conDesIt->getNextConceptDesciptor();
									}
								}

								CRoleBackwardSaturationPropagationHash* roleBackSatPropHash = indi->getRoleBackwardPropagationHash(false);
								if (roleBackSatPropHash) {
									CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropDataHash = roleBackSatPropHash->getRoleBackwardPropagationDataHash();
									for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropDataHash->constBegin(), itEnd = backPropDataHash->constEnd(); it != itEnd; ++it) {
										CRole* role = it.key();
										const CRoleBackwardSaturationPropagationHashData& hashData = it.value();
										bool relevantFlag = false;
										CRoleProcessData* roleProData = (CRoleProcessData*)role->getRoleData();
										if (roleProData) {
											relevantFlag = roleProData->hasInferRelevantFlag();
										}
										for (CBackwardSaturationPropagationLink* linkLinker = hashData.mLinkLinker; linkLinker; linkLinker = linkLinker->getNext()) {
											++totalRoleCount;
											if (relevantFlag) {
												++relevantRoleCount;
											}
										}
									}
								}
							}
						}
					}
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::hasRemainingExtensionProcessingNodes(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (hasRemainingProcessingNodes(processingDataBox,calcAlgContext)) {
						return true;
					}
					CSaturationSuccessorExtensionIndividualNodeProcessingQueue* extProIndiQueue = processingDataBox->getSaturationSucessorExtensionIndividualNodeProcessingQueue(false);
					if (extProIndiQueue && !extProIndiQueue->isEmpty()) {
						return true;
					}
					return false;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::continueNominalDelayedIndividualNodeProcessing(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext)  {
					bool nominalDelayedIndividualNodeProcessingContinued = false;
					if (processingDataBox->hasNominalDelayedIndividualSaturationProcessNodeLinker()) {
						if (loadConsistenceModelData(calcAlgContext)) {
							while (processingDataBox->hasNominalDelayedIndividualSaturationProcessNodeLinker()) {
								CIndividualSaturationProcessNodeLinker* nominalDelayedIndSatProcNodeLinker = processingDataBox->takeNominalDelayedIndividualSaturationProcessNodeLinker();
								CIndividualSaturationProcessNode* indiProcessNode = nominalDelayedIndSatProcNodeLinker->getProcessingIndividual();
								nominalDelayedIndSatProcNodeLinker->setProcessingQueued(false);

								CCriticalSaturationConceptTypeQueues* criticalConceptQueues = indiProcessNode->getCriticalConceptTypeQueues(false);
								if (criticalConceptQueues) {
									if (criticalConceptQueues->hasCriticalSaturationConceptsQueued()) {
										CCriticalIndividualNodeProcessingQueue* criticalIndNodProcQueue = processingDataBox->getSaturationCriticalIndividualNodeProcessingQueue(true);
										criticalIndNodProcQueue->insertProcessIndiviudal(indiProcessNode);
										criticalConceptQueues->setProcessNodeQueued(true);
									}
								}

								CSaturationIndividualNodeNominalHandlingData* nominalHandlingData = indiProcessNode->getNominalHandlingData(false);
								if (nominalHandlingData) {
									if (nominalHandlingData->getDelayedNominalConceptSaturationProcessLinker()) {
										while (nominalHandlingData->getDelayedNominalConceptSaturationProcessLinker()) {
											CConceptSaturationProcessLinker* delConSatProLinker = nominalHandlingData->takeDelayedNominalConceptSaturationProcessLinker();
											delConSatProLinker->clearNext();
											indiProcessNode->addConceptSaturationProcessLinker(delConSatProLinker);
										}
										addIndividualToProcessingQueue(indiProcessNode,calcAlgContext);
									}
								}
								indiProcessNode->getIndividualSaturationCompletionNodeLinker(true)->setProcessingQueued(false);
								addIndividualToCompletionQueue(indiProcessNode,calcAlgContext);
								nominalDelayedIndividualNodeProcessingContinued = true;
							}

							calcAlgContext->getUsedProcessingDataBox()->setDelayedNominalProcessingOccured(false);

						}
					}
					return nominalDelayedIndividualNodeProcessingContinued;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::completeSaturatedIndividualNodes(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext) {
					
					cint64 completedIndividualCount = 0;
					if (processingDataBox->hasIndividualSaturationCompletionNodeLinker()) {
						while (processingDataBox->hasIndividualSaturationCompletionNodeLinker()) {
							CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker = processingDataBox->takeIndividualSaturationCompletionNodeLinker();
							CIndividualSaturationProcessNode* indiProcessNode = indiProcessNodeLinker->getProcessingIndividual();
							bool completeIndividual = true;
							if (indiProcessNode->getIndirectStatusFlags()->hasMissedABoxConsistencyFlag()) {
								if (!loadConsistenceModelData(calcAlgContext)) {
									completeIndividual = false;
									processingDataBox->addNominalDelayedIndividualSaturationProcessNodeLinker(indiProcessNodeLinker);
								}
							} 
							if (completeIndividual) {
								completedIndividualCount++;
								indiProcessNode->setCompleted(true);
							}
						}
					}
					return completedIndividualCount > 0;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::hasRemainingProcessingNodes(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (processingDataBox->hasIndividualSaturationProcessNodeLinker() || processingDataBox->hasIndividualDisjunctCommonConceptExtractProcessLinker()) {
						return true;
					}
					return false;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::individualNodeInitializing(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!indiProcSatNode->isInitialized()) {
						initializeInitializationConcepts(indiProcSatNode,calcAlgContext);

						indiProcSatNode->setInitialized(true);
					}
					return true;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::hasNextCriticalConcepts(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CCriticalIndividualNodeProcessingQueue* critIndNodeProcQueue = procDataBox->getSaturationCriticalIndividualNodeProcessingQueue(false);
					if (critIndNodeProcQueue) {
						if (!critIndNodeProcQueue->isEmpty()) {
							return true;
						}
					}
					return false;
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::checkNextCriticalConcepts(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CCriticalIndividualNodeProcessingQueue* critIndNodeProcQueue = procDataBox->getSaturationCriticalIndividualNodeProcessingQueue(false);
					if (critIndNodeProcQueue) {
						CIndividualSaturationProcessNode* indiProcSatNode = critIndNodeProcQueue->takeNextProcessIndividual();
						bool checkCriticalConcepts = true;
						if (indiProcSatNode->getDirectStatusFlags()->hasMissedABoxConsistencyFlag()) {
							if (!loadConsistenceModelData(calcAlgContext)) {
								checkCriticalConcepts = false;
								CCriticalSaturationConceptTypeQueues* criticalConceptQueues = indiProcSatNode->getCriticalConceptTypeQueues(false);
								if (criticalConceptQueues) {
									criticalConceptQueues->setProcessNodeQueued(false);
								}
							}
						}
						if (checkCriticalConcepts) {
							checkCriticalConceptsForNode(indiProcSatNode,calcAlgContext);
						}
					}
				}


				CSaturationSuccessorExtensionData* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getSucessorExtensionData(CLinkedRoleSaturationSuccessorData* succData, bool create, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationSuccessorExtensionData*& succExtData = succData->mExtensionData;
					if (!succExtData && create) {
						succExtData = CObjectParameterizingAllocator<CSaturationSuccessorExtensionData,CProcessContext*>::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getProcessContext());
						succExtData->initSuccessorExtensionData();
					}
					return succExtData;
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::initializeSuccessorALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationIndividualNodeSuccessorExtensionData* indiProcSatNodeSuccExt = indiProcSatNode->getSuccessorExtensionData();
					CSaturationIndividualNodeALLConceptsExtensionData* indiProcSatNodeALLConSuccExt = indiProcSatNodeSuccExt->getALLConceptsExtensionData(true);

					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					if (linkedSuccHash) {
						CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
						CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
						if (backwardPropHash) {
							CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backwardPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();

							for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator itSucc = succHash->constBegin(), itSuccEnd = succHash->constEnd(); itSucc != itSuccEnd; ++itSucc) {
								CRole* role(itSucc.key());
								CLinkedRoleSaturationSuccessorData* succData(itSucc.value());
								CRoleBackwardSaturationPropagationHashData* backwardPropData(backwardPropDataHash->valuePointer(role));
								if (backwardPropData && backwardPropData->mReapplyLinker) {
									backwardPropData->mRoleALLConceptsProcessingQueued = false;
									updateSuccessorRoleALLConceptsExtensions(indiProcSatNode,role,succData,*backwardPropData,calcAlgContext);
								}
							}
						}
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateSuccessorRoleALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CLinkedRoleSaturationSuccessorData* succData, const CRoleBackwardSaturationPropagationHashData& backwardPropData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationIndividualNodeSuccessorExtensionData* indiProcSatNodeSuccExt = indiProcSatNode->getSuccessorExtensionData();
					CSaturationIndividualNodeALLConceptsExtensionData* indiProcSatNodeALLConSuccExt = indiProcSatNodeSuccExt->getALLConceptsExtensionData(true);
					if (succData) {
						if (backwardPropData.mReapplyLinker) {
							bool succLinksUpdated = false;
							bool allConUpdated = false;
							CSaturationSuccessorExtensionData* satSuccExtData = getSucessorExtensionData(succData,true,calcAlgContext);
							CSaturationSuccessorData* lastExaminedLinkLinker = satSuccExtData->getLastExaminedLinkLinker();
							CBackwardSaturationPropagationReapplyDescriptor* lastExaminedALLConReaDes = satSuccExtData->getLastExaminedALLConceptReapplyDescriptor();
							bool iterateFullAllConReaDes = false;
							bool continueIterateLinks = false;
							if (lastExaminedALLConReaDes != backwardPropData.mReapplyLinker) {
								allConUpdated = true;
								continueIterateLinks = true;
							}
							if (lastExaminedLinkLinker != succData->mLastLink) {
								continueIterateLinks = true;
								succLinksUpdated = true;
								iterateFullAllConReaDes = true;
							}
							for (CSaturationSuccessorData* linkLinkerIt = succData->mLastLink; linkLinkerIt && continueIterateLinks; ) {

								if (!linkLinkerIt->mExtension && !linkLinkerIt->mVALUENominalConnection) {

									CIndividualSaturationProcessNode* succIndiNode = linkLinkerIt->mSuccIndiNode;
									for (CXNegLinker<CRole*>* creationRoleLinkerIt = linkLinkerIt->mCreationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
										CRole* creationRole = creationRoleLinkerIt->getData();
										CSaturationSuccessorALLConceptExtensionData* allConSuccExtData = indiProcSatNodeALLConSuccExt->getALLConceptsExtensionData(succIndiNode)->getRoleSuccessorALLConceptExtensionData(creationRole);
										
										bool conceptsForSuccIndiNodeModified = false;

										conceptsForSuccIndiNodeModified |= allConSuccExtData->addRequiredSuccessorCardinality(linkLinkerIt->mSuccCount);

										bool continueIterateALLReapConDes = true;
										for (CBackwardSaturationPropagationReapplyDescriptor* backReapplyIt = backwardPropData.mReapplyLinker; backReapplyIt && continueIterateALLReapConDes; ) {
											CConceptSaturationDescriptor* reapplyConDes = backReapplyIt->getReapplyConceptSaturationDescriptor();
											CConcept* concept = reapplyConDes->getConcept();
											bool conceptNegation = reapplyConDes->getNegation();
											conceptsForSuccIndiNodeModified |= addSuccessorExtensionsALLConcept(indiProcSatNode,concept,conceptNegation,allConSuccExtData,calcAlgContext);

											backReapplyIt = backReapplyIt->getNext();
											if (backReapplyIt == lastExaminedALLConReaDes) {
												if (!iterateFullAllConReaDes) {
													continueIterateALLReapConDes = false;
												}
											}
										}

										if (conceptsForSuccIndiNodeModified) {
											if (!allConSuccExtData->isExtensionProcessingQueued()) {
												allConSuccExtData->setExtensionProcessingQueued(true);
												indiProcSatNodeALLConSuccExt->addExtensionProcessData(allConSuccExtData);
											}
										}
									}
								}

								linkLinkerIt = linkLinkerIt->mNextLink;
								if (linkLinkerIt == lastExaminedLinkLinker) {
									if (!allConUpdated) {
										continueIterateLinks = false;
									} else {
										iterateFullAllConReaDes = false;
									}
								}
							}
							satSuccExtData->setLastExaminedLinkLinker(succData->mLastLink);
							satSuccExtData->setLastExaminedALLConceptReapplyDescriptor(backwardPropData.mReapplyLinker);
						}
					}
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::installSuccessorRoleFunctionalityConceptsExtension(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool installed = false;
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					if (linkedSuccHash) {
						CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
						CLinkedRoleSaturationSuccessorData* succData = linkedSuccHash->getLinkedRoleSuccessorData(role,true);
						if (succData) {
							if (!succData->mRoleFUNCTIONALConceptsQueuingRequired) {
								succData->mRoleFUNCTIONALConceptsQueuingRequired = true;
								installed = true;
							}
						}
					}
					return installed;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateSuccessorRoleFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool updated = false;
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					if (linkedSuccHash) {
						CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
						CLinkedRoleSaturationSuccessorData* succData = linkedSuccHash->getLinkedRoleSuccessorData(role,false);
						if (succData && succData->mRoleFUNCTIONALConceptsQueuingRequired) {
							succData->mRoleFUNCTIONALConceptsProcessingQueued = false;
							if (succData->mSuccCount > 1) {
								updated |= updateSuccessorRoleFUNCTIONALConceptsExtensions(indiProcSatNode,role,succData,calcAlgContext);
							}
						}
					}
					return updated;
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateSuccessorRoleFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CLinkedRoleSaturationSuccessorData* succData, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool updated = false;
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succDataHash = linkedSuccHash->getLinkedRoleSuccessorHash();
					CSaturationIndividualNodeSuccessorExtensionData* indiProcSatNodeSuccExt = indiProcSatNode->getSuccessorExtensionData();
					CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* indiProcSatNodeFunctionalConSuccExt = indiProcSatNodeSuccExt->getFUNCTIONALConceptsExtensionData(true);
					if (succData) {
						CSaturationSuccessorFUNCTIONALConceptExtensionData* roleFuncConExtData = indiProcSatNodeFunctionalConSuccExt->getFUNCTIONALConceptsExtensionData(role,true);
						CSaturationSuccessorData* lastExaminedLinkedSucc = roleFuncConExtData->getLastExaminedLinkedSuccessorData();
						CSaturationSuccessorData* lastLinkedSucc = succData->getLastSuccessorLinkData();
						if (lastLinkedSucc != lastExaminedLinkedSucc) {

							cint64 succCount = 0;
							cint64 maxSuccCardinality = CINT64_MIN;
							for (CSaturationSuccessorData* lastLinkedSuccIt = lastLinkedSucc; lastLinkedSuccIt; lastLinkedSuccIt = lastLinkedSuccIt->mNextLink) {
								if (lastLinkedSuccIt->mActiveCount >= 1 && !lastLinkedSuccIt->mVALUENominalConnection) {
									succCount++;
									maxSuccCardinality = qMax(maxSuccCardinality,lastLinkedSuccIt->mSuccCount);
								}
							}
							if (succCount > 1 && maxSuccCardinality <= 1) {

								//if (getDebugIndividualConceptName(indiProcSatNode,calcAlgContext) == "http://www.owllink.org/testsuite/galen#Kussmaul-KienRespiration") {
								//	bool debug = true;
								//}

								CIndividualSaturationProcessNode* copyIndiProcSatNode = nullptr;
								CIndividualSaturationProcessNode* firstIndiProcSatNode = nullptr;
								CSaturationIndividualNodeExtensionResolveData* resolveData = nullptr; 

								CPROCESSMAP<cint64,CSaturationSuccessorData*>* succDataMap = succData->getSuccessorNodeDataMap(false);
								// resolve new individual node
								for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succDataMap->constBegin(), itEnd = succDataMap->constEnd(); it != itEnd; ++it) {
									cint64 succID = it.key();
									CSaturationSuccessorData* linkedSuccData = it.value();
									if (linkedSuccData->mActiveCount >= 1 && !linkedSuccData->mVALUENominalConnection) {
										CIndividualSaturationProcessNode* succNode = linkedSuccData->mSuccIndiNode;
										if (!resolveData) {
											resolveData = succNode->getSuccessorExtensionData(true)->getBaseExtensionResolveData(true);
											copyIndiProcSatNode = succNode;
											firstIndiProcSatNode = succNode;
										} else {
											resolveData = getResolvedIndividualNodeExtension(resolveData,succNode,copyIndiProcSatNode,calcAlgContext);
										}
									}
								}

								CIndividualSaturationProcessNode* resolvedIndiNode = resolveData->getProcessingIndividualNode();
								roleFuncConExtData->setLastResolvedIndividualNode(resolvedIndiNode);

								CRoleBackwardSaturationPropagationHash* resolvedIndiBackPropHash = resolvedIndiNode->getRoleBackwardPropagationHash(true);

								bool backwardLinkConnected = false;
								bool connectionAlreadyExist = false;
								// deactivate previous successor connections, activate new ones
								for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succDataMap->constBegin(), itEnd = succDataMap->constEnd(); it != itEnd; ++it) {
									cint64 succID = it.key();
									CSaturationSuccessorData* linkedSuccData = it.value();
									if (linkedSuccData->mActiveCount >= 1 && !linkedSuccData->mVALUENominalConnection) {
										cint64 succCount = linkedSuccData->mSuccCount;
										CIndividualSaturationProcessNode* succNode = linkedSuccData->mSuccIndiNode;
										for (CXNegLinker<CRole*>* creationRoleLinkerIt = linkedSuccData->mCreationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
											if (!creationRoleLinkerIt->isNegated()) {
												CRole* creationRole = creationRoleLinkerIt->getData();
												bool makeNewSuccessorConnections = true;
												bool removePreviousSuccessorConnections = true;
												// test whether creation role already exists for this successor connection
												if (linkedSuccHash->hasActiveLinkedSuccessor(creationRole,resolvedIndiNode)) {
													connectionAlreadyExist = true;
													makeNewSuccessorConnections = false;
													if (succNode == resolvedIndiNode) {
														removePreviousSuccessorConnections = false;
													}
												}
												for (CSortedNegLinker<CRole*>* creationSuperRoleIt = creationRole->getIndirectSuperRoleList(); creationSuperRoleIt; creationSuperRoleIt = creationSuperRoleIt->getNext()) {
													CRole* creationSuperRole = creationSuperRoleIt->getData();
													if (!creationSuperRoleIt->isNegated()) {
														if (removePreviousSuccessorConnections) {
															linkedSuccHash->deactivateLinkedSuccessor(creationSuperRole,succNode,creationRole);
														}
														if (makeNewSuccessorConnections) {
															linkedSuccHash->addExtensionSuccessor(creationSuperRole,resolvedIndiNode,creationRole,succCount);
														}
													} else {

														if (makeNewSuccessorConnections) {
															backwardLinkConnected = true;
															CBackwardSaturationPropagationLink* backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
															backPropLink->initBackwardPropagationLink(indiProcSatNode,creationSuperRole);
															CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDes = resolvedIndiBackPropHash->addBackwardPropagationLink(creationSuperRole,backPropLink);
															if (backPropReapplyDes) {
																applyBackwardPropagationConcepts(backPropLink,backPropReapplyDes,mCalcAlgContext);
															}
														}

													}

												}

											}
										}
									}
								}

								//if (succData->mSuccCount >= 2) {
								//	bool debug = true;
								//}
								//cint64 countedSucc = 0;
								//for (CSaturationSuccessorData* linkedSuccDataIt = succData->mLastLink; linkedSuccDataIt; linkedSuccDataIt = linkedSuccDataIt->mNextLink) {
								//	if (linkedSuccDataIt->mActiveCount >= 1) {
								//		countedSucc += linkedSuccDataIt->mSuccCount;
								//	}
								//}
								//if (countedSucc >= 2) {

								//	mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
								//	QFile tmpFile("saturation-model.txt");
								//	if (tmpFile.open(QIODevice::WriteOnly)) {
								//		tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
								//		tmpFile.close();
								//	}
								//	bool bug = true;
								//}

								updateIndirectAddingIndividualStatusFlags(indiProcSatNode,resolvedIndiNode->getIndirectStatusFlags(),mCalcAlgContext);

								if (!connectionAlreadyExist && !backwardLinkConnected) {
									CXLinker<CIndividualSaturationProcessNode*>* nonInvConnectedIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
									nonInvConnectedIndiNodeLinker->initLinker(indiProcSatNode);
									resolvedIndiNode->addNonInverseConnectedIndividualNodeLinker(nonInvConnectedIndiNodeLinker);
								}

								updated = true;
							}
						}
					}
					return updated;
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateSuccessorRoleALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {

					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					if (linkedSuccHash) {
						CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
						CLinkedRoleSaturationSuccessorData* succData = succHash->value(role,nullptr);
						if (succData) {
							succData->mRoleALLConceptsProcessingQueued = false;
							CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
							if (backwardPropHash) {
								CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backwardPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();
								CRoleBackwardSaturationPropagationHashData* backwardPropData(backwardPropDataHash->valuePointer(role));
								if (backwardPropData && backwardPropData->mReapplyLinker) {
									backwardPropData->mRoleALLConceptsProcessingQueued = false;
									updateSuccessorRoleALLConceptsExtensions(indiProcSatNode,role,succData,*backwardPropData,calcAlgContext);
								}
							}
						}
					}
				}

				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateSuccessorALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool updated = false;

					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					if (linkedSuccHash) {
						CSaturationIndividualNodeSuccessorExtensionData* indiProcSatNodeSuccExt = indiProcSatNode->getSuccessorExtensionData(false);
						if (indiProcSatNodeSuccExt) {
							CSaturationIndividualNodeALLConceptsExtensionData* indiProcSatNodeALLConSuccExt = indiProcSatNodeSuccExt->getALLConceptsExtensionData(false);
							if (indiProcSatNodeALLConSuccExt) {
								CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();

								while (indiProcSatNodeALLConSuccExt->hasExtensionProcessData()) {
									CSaturationSuccessorALLConceptExtensionData* satSucALLConExtData = indiProcSatNodeALLConSuccExt->takeNextExtensionProcessData();
									satSucALLConExtData->setExtensionProcessingQueued(false);

									CSaturationSuccessorConceptExtensionMap* satSucConExtMap = satSucALLConExtData->getSuccessorConceptExtensionMap();
									CIndividualSaturationProcessNode* indiNode = satSucALLConExtData->getIndividualNode();
									CRole* role = satSucALLConExtData->getRole();
									CIndividualSaturationProcessNode* lastResolvedIndiNode = satSucALLConExtData->getLastResolvedIndividualNode();
									if (!lastResolvedIndiNode) {
										lastResolvedIndiNode = indiNode;
									}

									cint64 lastSuccCard = satSucALLConExtData->getLastConnectedSuccessorCardinality();
									cint64 requiredSuccCard = satSucALLConExtData->getRequiredSuccessorCardinality();




									//CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
									//if (backwardPropHash) {
									//	CPROCESSHASH<cint64,CSaturationSuccessorConceptExtensionMapData>* extMap = satSucConExtMap->getSuccessorConceptExtensionMap();
									//	CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();
									//	if (backPropDataHash) {
									//		for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
									//			CRole* superRole = superRoleIt->getData();
									//			if (!superRoleIt->isNegated()) {
									//				const CRoleBackwardSaturationPropagationHashData& backPropData = backPropDataHash->value(superRole);
									//				for (CBackwardSaturationPropagationReapplyDescriptor* backPropIt = backPropData.mReapplyLinker; backPropIt; backPropIt = backPropIt->getNext()) {
									//					CConceptSaturationDescriptor* conDes = backPropIt->getReapplyConceptSaturationDescriptor();
									//					CConcept* concept = conDes->getConcept();
									//					bool negation = conDes->isNegated();
									//					for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
									//						CConcept* opConcept = opLinkerIt->getData();
									//						bool opNegation = opLinkerIt->isNegated()^negation;
									//						const CSaturationSuccessorConceptExtensionMapData& satSuccConExtMapData = extMap->value(opConcept->getConceptTag());
									//						if (opNegation && !satSuccConExtMapData.mNegative) {
									//							bool bug = true;
									//						}
									//						if (!opNegation && !satSuccConExtMapData.mPositive) {
									//							bool bug = true;
									//						}
									//					}
									//				}
									//			}
									//		}
									//	}
									//}


									bool onlySuccessorCardinalityUpdated = true;
									CIndividualSaturationProcessNode* resolvedIndiNode = lastResolvedIndiNode;
									if (satSucALLConExtData->hasConceptsUpdatedFlag()) {
										resolvedIndiNode = getResolvedIndividualNodeExtensionSuccessor(indiNode,satSucConExtMap,calcAlgContext);
										if (lastResolvedIndiNode != resolvedIndiNode) {
											onlySuccessorCardinalityUpdated = false;
										}
									}
									if (lastResolvedIndiNode != resolvedIndiNode || lastSuccCard != requiredSuccCard) {
										updated = true;
										bool backwardLinkConnected = false;
										CRoleBackwardSaturationPropagationHash* resolvedIndiBackPropHash = resolvedIndiNode->getRoleBackwardPropagationHash(true);
										if (lastResolvedIndiNode) {
											for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
												if (!superRoleIt->isNegated()) {
													CRole* superRole = superRoleIt->getData();
													linkedSuccHash->deactivateLinkedSuccessor(superRole,lastResolvedIndiNode,role);
												}
											}
										}
										for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
											CRole* superRole = superRoleIt->getData();
											if (!superRoleIt->isNegated()) {
												linkedSuccHash->addExtensionSuccessor(superRole,resolvedIndiNode,role,requiredSuccCard);
												addNewLinkedExtensionProcessingRole(superRole,resolvedIndiNode,false,true,mCalcAlgContext);

											} else {

												if (!onlySuccessorCardinalityUpdated) {
													backwardLinkConnected = true;
													CBackwardSaturationPropagationLink* backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
													backPropLink->initBackwardPropagationLink(indiProcSatNode,superRole);
													CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDes = resolvedIndiBackPropHash->addBackwardPropagationLink(superRole,backPropLink);
													if (backPropReapplyDes) {
														applyBackwardPropagationConcepts(backPropLink,backPropReapplyDes,mCalcAlgContext);
													}
												}

											}
										}
										updateIndirectAddingIndividualStatusFlags(indiProcSatNode,resolvedIndiNode->getIndirectStatusFlags(),mCalcAlgContext);
										if (!onlySuccessorCardinalityUpdated && !backwardLinkConnected) {
											CXLinker<CIndividualSaturationProcessNode*>* nonInvConnectedIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
											nonInvConnectedIndiNodeLinker->initLinker(indiProcSatNode);
											resolvedIndiNode->addNonInverseConnectedIndividualNodeLinker(nonInvConnectedIndiNodeLinker);
										}
										satSucALLConExtData->setLastResolvedIndividualNode(resolvedIndiNode);
										satSucALLConExtData->setLastConnectedSuccessorCardinality(requiredSuccCard);
									}
								}
							}
						}
					}
					return updated;
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::initializeIndividualNodeByCoping(CIndividualSaturationProcessNode* indiProcSatNode, CIndividualSaturationProcessNode* copyFromIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					++mCopiedIndiNodeCount;
					indiProcSatNode->initCopingIndividualSaturationProcessNode(copyFromIndiProcSatNode);
					indiProcSatNode->setReferenceMode(2);
					indiProcSatNode->setCopyIndividualNode(copyFromIndiProcSatNode);
					indiProcSatNode->setReferenceIndividualSaturationProcessNode(copyFromIndiProcSatNode);

					updateDirectAddingIndividualStatusFlags(indiProcSatNode,copyFromIndiProcSatNode->getDirectStatusFlags(),mCalcAlgContext);							
					updateIndirectAddingIndividualStatusFlags(indiProcSatNode,copyFromIndiProcSatNode->getIndirectStatusFlags(),mCalcAlgContext);							


					for (CConceptSaturationProcessLinker* conSatProLinkerIt = copyFromIndiProcSatNode->getConceptSaturationProcessLinker(); conSatProLinkerIt; conSatProLinkerIt = conSatProLinkerIt->getNext()) {
						CConceptSaturationProcessLinker* conSatProLinker = createConceptSaturationProcessLinker(calcAlgContext);
						conSatProLinker->initLinker(conSatProLinkerIt->getConceptSaturationDescriptor());
						indiProcSatNode->addConceptSaturationProcessLinker(conSatProLinker);
					}

					CSaturationIndividualNodeSuccessorExtensionData* copySuccExtensionData = copyFromIndiProcSatNode->getSuccessorExtensionData(false);
					if (copySuccExtensionData) {
						CSaturationIndividualNodeALLConceptsExtensionData* copyALLConceptsExtension = copySuccExtensionData->getALLConceptsExtensionData(false);
						if (copyALLConceptsExtension) {
							if (copyALLConceptsExtension->isSuccessorExtensionInitialized()) {
								addSuccessorExtensionToProcessingQueue(indiProcSatNode,calcAlgContext);
							}
						}
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* copyFUNCTIONALConceptsExtension = copySuccExtensionData->getFUNCTIONALConceptsExtensionData(false);
						if (copyFUNCTIONALConceptsExtension) {
							if (copyFUNCTIONALConceptsExtension->isSuccessorExtensionInitialized()) {

								CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* functionalConceptsExtension = indiProcSatNode->getSuccessorExtensionData(true)->getFUNCTIONALConceptsExtensionData(true);

								addSuccessorExtensionToProcessingQueue(indiProcSatNode,calcAlgContext);
								for (CRoleSaturationProcessLinker* roleLinkerIt = copyFUNCTIONALConceptsExtension->getCopyingInitializingRoleProcessLinker(); roleLinkerIt; roleLinkerIt = roleLinkerIt->getNext()) {
									CRole* role = roleLinkerIt->getRole();
									CRoleSaturationProcessLinker* tmpRoleLinker = createRoleSaturationProcessLinker(calcAlgContext);
									tmpRoleLinker->initRoleProcessLinker(role);
									functionalConceptsExtension->addFunctionalityAddedRoleProcessLinker(tmpRoleLinker);
								}
							}
						}
					}
				}



				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeExtensionSuccessor(CIndividualSaturationProcessNode* indiProcSatNode, CSaturationSuccessorConceptExtensionMap* succConExtMap, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* resolvedNode = nullptr;
					CSaturationIndividualNodeExtensionResolveData* resolveData = indiProcSatNode->getSuccessorExtensionData(true)->getBaseExtensionResolveData(true);
					CPROCESSHASH<cint64,CSaturationSuccessorConceptExtensionMapData>* succConExtDataMap = succConExtMap->getSuccessorConceptExtensionMap();
					CIndividualSaturationProcessNode* copyIndiProcSatNode = indiProcSatNode;
					// resolve individual
					for (CPROCESSHASH<cint64,CSaturationSuccessorConceptExtensionMapData>::const_iterator it = succConExtDataMap->constBegin(), itEnd = succConExtDataMap->constEnd(); it != itEnd; ++it) {
						const CSaturationSuccessorConceptExtensionMapData& conExtDat(it.value());
						CConcept* concept = conExtDat.mConcept;
						bool addPositive = conExtDat.mPositive;
						bool addNegative = conExtDat.mNegative;
						if (addPositive) {
							resolveData = getResolvedIndividualNodeExtension(resolveData,concept,false,copyIndiProcSatNode,calcAlgContext);
						}
						if (addNegative) {
							resolveData = getResolvedIndividualNodeExtension(resolveData,concept,true,copyIndiProcSatNode,calcAlgContext);
						}
					}
					if (!resolveData->hasProcessingIndividualNode()) {
						// create individual
						resolvedNode = createResolvedIndividualNode(resolveData,copyIndiProcSatNode,true,calcAlgContext);
						CReapplyConceptSaturationLabelSet* conSet = resolvedNode->getReapplyConceptSaturationLabelSet(true);

						// add all concepts to individual
						for (CPROCESSHASH<cint64,CSaturationSuccessorConceptExtensionMapData>::const_iterator it = succConExtDataMap->constBegin(), itEnd = succConExtDataMap->constEnd(); it != itEnd; ++it) {
							const CSaturationSuccessorConceptExtensionMapData& conExtDat(it.value());
							CConcept* concept = conExtDat.mConcept;
							bool addPositive = conExtDat.mPositive;
							bool addNegative = conExtDat.mNegative;
							if (addPositive) {
								addConceptFilteredToIndividual(concept,false,resolvedNode,calcAlgContext);
							}
							if (addNegative) {
								addConceptFilteredToIndividual(concept,true,resolvedNode,calcAlgContext);
							}
						}

					}
					resolvedNode = resolveData->getProcessingIndividualNode();
					return resolvedNode;
				}



				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createResolvedIndividualNode(CSaturationIndividualNodeExtensionResolveData* resolveData, CIndividualSaturationProcessNode*& copyIndiProcSatNode, bool queueProcessing, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* resolvedNode = nullptr;
					resolvedNode = CObjectParameterizingAllocator< CIndividualSaturationProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
					resolvedNode->initIndividualSaturationProcessNode(resolveData->getProcessingIndividualNodeID(),nullptr);
					initializeIndividualNodeByCoping(resolvedNode,copyIndiProcSatNode,calcAlgContext);

					resolveData->setProcessingIndividualNode(resolvedNode);
					resolvedNode->getSuccessorExtensionData(true)->setExtensionResolveData(resolveData);
					resolvedNode->setInitialized(true);
					resolvedNode->setRequiredBackwardPropagation(true);

					CIndividualSaturationProcessNodeLinker* resolveNodeProcessLiner = CObjectAllocator<CIndividualSaturationProcessNodeLinker>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
					resolveNodeProcessLiner->initProcessNodeLinker(resolvedNode,queueProcessing);
					resolvedNode->setIndividualSaturationProcessNodeLinker(resolveNodeProcessLiner);
					if (queueProcessing) {
						calcAlgContext->getUsedProcessingDataBox()->addIndividualSaturationProcessNodeLinker(resolveNodeProcessLiner);
					}
					CIndividualSaturationProcessNodeVector* indiVec = calcAlgContext->getUsedProcessingDataBox()->getIndividualSaturationProcessNodeVector(true);
					indiVec->setData(resolvedNode->getIndividualID(),resolvedNode);
					return resolvedNode;
				}


				CSaturationIndividualNodeExtensionResolveData* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CIndividualSaturationProcessNode* extensionNode, CIndividualSaturationProcessNode*& copyIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationIndividualNodeExtensionResolveHashData& resolveHashData = resolveData->getIndividualNodeExtensionResolveHash(true)->getResolvedIndividualNodeExtensionData(extensionNode);
					if (!resolveHashData.mResolveData) {
						CReapplyConceptSaturationLabelSet* extConSet = extensionNode->getReapplyConceptSaturationLabelSet(false);

						for (CConceptSaturationDescriptor* conIt = extConSet->getConceptSaturationDescriptionLinker(); conIt; conIt = conIt->getNextConceptDesciptor()) {
							CConceptSaturationDescriptor* containedConDes = nullptr;
							CImplicationReapplyConceptSaturationDescriptor* containedImpReaDes = nullptr;
							bool insertionRequired = false;
							CReapplyConceptSaturationLabelSet* baseConSet = copyIndiProcSatNode->getReapplyConceptSaturationLabelSet(false);
							if (baseConSet->getConceptSaturationDescriptor(conIt->getConcept(),containedConDes,containedImpReaDes)) {
								if (!containedConDes) {
									insertionRequired = true;
								} else if (containedConDes->isNegated() != conIt->isNegated()) {
									insertionRequired = true;
								}
							} else {
								insertionRequired = true;
							}

							if (insertionRequired) {
								if (!resolveHashData.mResolveData) {
									resolveHashData.mResolveData = CObjectParameterizingAllocator< CSaturationIndividualNodeExtensionResolveData,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
									cint64 nextResolveIndiID = calcAlgContext->getUsedProcessingDataBox()->getNextSaturationResolvedSuccessorExtensionIndividualNodeID();
									resolveHashData.mResolveData->initExtensionResolveData(nextResolveIndiID);
								}

								CIndividualSaturationProcessNode* resolvedNode = resolveHashData.mResolveData->getProcessingIndividualNode();
								if (!resolvedNode) {
									resolvedNode = createResolvedIndividualNode(resolveHashData.mResolveData,copyIndiProcSatNode,true,calcAlgContext);
									resolveHashData.mResolveData->setProcessingIndividualNode(resolvedNode);
								}

								CReapplyConceptSaturationLabelSet* resolvedConSet = resolvedNode->getReapplyConceptSaturationLabelSet(true);
								addConceptFilteredToIndividual(conIt->getConcept(),conIt->isNegated(),resolvedNode,resolvedConSet,false,calcAlgContext);
								copyIndiProcSatNode = resolvedNode;
							}
						} 
					
						if (!resolveHashData.mResolveData) {
							resolveHashData.mResolveData = resolveData;
						}

					}
					copyIndiProcSatNode = resolveHashData.mResolveData->getProcessingIndividualNode();
					return resolveHashData.mResolveData;
				}




				CSaturationIndividualNodeExtensionResolveData* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CConcept* concept, bool negation, CIndividualSaturationProcessNode*& copyIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationIndividualNodeExtensionResolveHashData& resolveHashData = resolveData->getIndividualNodeExtensionResolveHash(true)->getResolvedIndividualNodeExtensionData(concept,negation);
					if (!resolveHashData.mResolveData) {
						CReapplyConceptSaturationLabelSet* conSet = copyIndiProcSatNode->getReapplyConceptSaturationLabelSet(false);
						if (conSet && conSet->containsConcept(concept,negation)) {
							resolveHashData.mResolveData = resolveData;
						} else {
							resolveHashData.mResolveData = CObjectParameterizingAllocator< CSaturationIndividualNodeExtensionResolveData,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
							cint64 nextResolveIndiID = calcAlgContext->getUsedProcessingDataBox()->getNextSaturationResolvedSuccessorExtensionIndividualNodeID();
							resolveHashData.mResolveData->initExtensionResolveData(nextResolveIndiID);
						}
					}
					if (resolveHashData.mResolveData->hasProcessingIndividualNode()) {
						copyIndiProcSatNode = resolveHashData.mResolveData->getProcessingIndividualNode();
					}
					return resolveHashData.mResolveData;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addSuccessorExtensionsALLConcept(CIndividualSaturationProcessNode*& indiProcSatNode, CConcept* concept, bool conceptNegation, CSaturationSuccessorALLConceptExtensionData* allConSuccExtData, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool newConceptAdded = false;
					bool addOperandConcepts = false;
					bool useNegatedOperandConcepts = false;
					CConceptOperator* conOp = concept->getConceptOperator();
					if (!conceptNegation && conOp->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) {
						addOperandConcepts = true;
						useNegatedOperandConcepts = false;
					}
					if (conceptNegation && conOp->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_SOME_TYPE)) {
						addOperandConcepts = true;
						useNegatedOperandConcepts = true;
					}
					if (addOperandConcepts) {
						for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
							CConcept* opConcept = opLinkerIt->getData();
							bool opConceptNegation = opLinkerIt->isNegated()^useNegatedOperandConcepts;
							newConceptAdded |= allConSuccExtData->addExtensionConcept(opConcept,opConceptNegation);
						}
					}
					return newConceptAdded;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::processSuccessorFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationIndividualNodeSuccessorExtensionData* succExtensionData = indiProcSatNode->getSuccessorExtensionData();
					CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* functionalConceptsExtension = succExtensionData->getFUNCTIONALConceptsExtensionData();

					bool updated = false;
					bool initialized = false;

					collectLinkedSuccessorNodes(indiProcSatNode,calcAlgContext);

					if (!functionalConceptsExtension->isSuccessorExtensionInitialized()) {
						functionalConceptsExtension->setSuccessorExtensionInitialized(true);
					}


					CRoleSaturationProcessLinker* functionalityRoleSatProcLinker = functionalConceptsExtension->takeFunctionalityAddedRoleProcessLinker();
					while (functionalityRoleSatProcLinker) {
						CRole* role = functionalityRoleSatProcLinker->getRole();
						CRoleSaturationProcessLinker* tmpRoleSatProcLinker = functionalityRoleSatProcLinker;
						functionalityRoleSatProcLinker = functionalityRoleSatProcLinker->getNext();
						tmpRoleSatProcLinker->clearNext();
						if (installSuccessorRoleFunctionalityConceptsExtension(indiProcSatNode,role,calcAlgContext)) {
							functionalConceptsExtension->addLinkedSuccessorAddedRoleProcessLinker(tmpRoleSatProcLinker);
							addFUNCTIONALProcessRoleExtensionFunctionalityAddedToDependentIndividuals(indiProcSatNode,role,calcAlgContext);
							CRoleSaturationProcessLinker* copyInitRoleLinker = createRoleSaturationProcessLinker(calcAlgContext);
							copyInitRoleLinker->initRoleProcessLinker(role);
							functionalConceptsExtension->addCopyingInitializingRoleProcessLinker(copyInitRoleLinker);
						}
					}


					CRoleSaturationProcessLinker* succLinkedAddedRoleSatProcLinker = functionalConceptsExtension->takeLinkedSuccessorAddedRoleProcessLinker();
					while (succLinkedAddedRoleSatProcLinker) {
						CRole* role = succLinkedAddedRoleSatProcLinker->getRole();
						CRoleSaturationProcessLinker* tmpRoleSatProcLinker = succLinkedAddedRoleSatProcLinker;
						succLinkedAddedRoleSatProcLinker = succLinkedAddedRoleSatProcLinker->getNext();
						tmpRoleSatProcLinker->clearNext();
						updated |= updateSuccessorRoleFUNCTIONALConceptsExtensions(indiProcSatNode,role,calcAlgContext);
						releaseRoleSaturationProcessLinker(tmpRoleSatProcLinker,calcAlgContext);
						addFUNCTIONALProcessRoleExtensionLinkedSuccessorAddedToDependentIndividuals(indiProcSatNode,role,calcAlgContext);
					}


					return updated;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::processNextSuccessorExtensions(CCalculationAlgorithmContextBase* calcAlgContext) {
					bool extensionProcessed = false;
					CIndividualSaturationProcessNode* indiProcSatNode = nullptr;
					CSaturationSuccessorExtensionIndividualNodeProcessingQueue* extProIndiQueue = mProcessingDataBox->getSaturationSucessorExtensionIndividualNodeProcessingQueue(false);
					while (!extensionProcessed && extProIndiQueue && !extProIndiQueue->isEmpty()) {
						indiProcSatNode = extProIndiQueue->takeNextToCurrentProcessIndividual();
						if (indiProcSatNode) {
							if (!extensionProcessed && mConfALLConceptsExtensionProcessing) {
								extensionProcessed |= processSuccessorALLConceptsExtensions(indiProcSatNode,calcAlgContext);
							}
							if (!extensionProcessed && mConfFUNCTIONALConceptsExtensionProcessing) {
								extensionProcessed |= processSuccessorFUNCTIONALConceptsExtensions(indiProcSatNode,calcAlgContext);
							}
						}
						if (!extensionProcessed) {
							extProIndiQueue->clearCurrentProcessIndividual();
						}
					}
					return extensionProcessed;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::processSuccessorALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationIndividualNodeSuccessorExtensionData* succExtensionData = indiProcSatNode->getSuccessorExtensionData();
					CSaturationIndividualNodeALLConceptsExtensionData* allConceptsExtension = succExtensionData->getALLConceptsExtensionData();

					bool updated = false;
					bool initialized = false;

					collectLinkedSuccessorNodes(indiProcSatNode,calcAlgContext);


					//QString indiDebugNameString = getDebugIndividualConceptName(indiProcSatNode,calcAlgContext);
					//if (indiDebugNameString == "http://www.co-ode.org/ontologies/galen#Atlas") {
					//	bool debug = true;
					//}

					if (!allConceptsExtension->isSuccessorExtensionInitialized()) {
						allConceptsExtension->setSuccessorExtensionInitialized(true);
						initializeSuccessorALLConceptsExtensions(indiProcSatNode,calcAlgContext);
						addProcessExtensionToDependentIndividuals(indiProcSatNode,calcAlgContext);
						initialized = true;
						mALLSuccExtInitializedCount++;
					}

					CRoleSaturationProcessLinker* roleSatProcLinker = allConceptsExtension->takeRoleProcessLinker();
					while (roleSatProcLinker) {
						CRole* role = roleSatProcLinker->getRole();
						CRoleSaturationProcessLinker* tmpRoleSatProcLinker = roleSatProcLinker;
						roleSatProcLinker = roleSatProcLinker->getNext();
						tmpRoleSatProcLinker->clearNext();
						updateSuccessorRoleALLConceptsExtensions(indiProcSatNode,role,calcAlgContext);
						releaseRoleSaturationProcessLinker(tmpRoleSatProcLinker,calcAlgContext);
						//addALLProcessRoleExtensionToDependentIndividuals(indiProcSatNode,role,calcAlgContext);
					}
					if (allConceptsExtension->isExtensionProcessingQueued()) {
						allConceptsExtension->setExtensionProcessingQueued(false);
					}
					if (succExtensionData->isExtensionProcessingQueued()) {
						succExtensionData->setExtensionProcessingQueued(false);
					}
					updated = updateSuccessorALLConceptsExtensions(indiProcSatNode,calcAlgContext);
					//testInsufficientALLConcepts(indiProcSatNode,calcAlgContext);
					return updated;

				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addSuccessorExtensionToProcessingQueue(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationIndividualNodeSuccessorExtensionData* succExtData = indiProcSatNode->getSuccessorExtensionData(true);
					CSaturationIndividualNodeALLConceptsExtensionData* succIndiALLConExtData = succExtData->getALLConceptsExtensionData(true);
					if (!succExtData->isExtensionProcessingQueued()) {
						succExtData->setExtensionProcessingQueued(true);
						calcAlgContext->getUsedProcessingDataBox()->getSaturationSucessorExtensionIndividualNodeProcessingQueue(true)->insertProcessIndiviudal(indiProcSatNode);
						return true;
					}
					return false;
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addProcessExtensionToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							addSuccessorExtensionToProcessingQueue(depIndi,calcAlgContext);
						}
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addALLProcessRoleExtensionToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							CSaturationIndividualNodeSuccessorExtensionData* succExtData = depIndi->getSuccessorExtensionData(true);
							CSaturationIndividualNodeALLConceptsExtensionData* succIndiALLConExtData = succExtData->getALLConceptsExtensionData(true);
							addSuccessorExtensionToProcessingQueue(indiProcSatNode,calcAlgContext);
							if (succIndiALLConExtData->isSuccessorExtensionInitialized()) {
								if (!succIndiALLConExtData->hasProcessLinkerForRole(role)) {
									CRoleSaturationProcessLinker* roleProcLinker = createRoleSaturationProcessLinker(calcAlgContext);
									roleProcLinker->initRoleProcessLinker(role);
									succIndiALLConExtData->addRoleProcessLinker(roleProcLinker);
								}
							}
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addFUNCTIONALProcessRoleExtensionLinkedSuccessorAddedToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							CSaturationIndividualNodeSuccessorExtensionData* succExtData = depIndi->getSuccessorExtensionData(true);
							CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* succIndiFunctionalConExtData = succExtData->getFUNCTIONALConceptsExtensionData(true);
							if (succIndiFunctionalConExtData->isSuccessorExtensionInitialized()) {
								addSuccessorExtensionToProcessingQueue(indiProcSatNode,calcAlgContext);
								if (!succIndiFunctionalConExtData->hasLinkedSuccessorAddedProcessLinkerForRole(role)) {
									CRoleSaturationProcessLinker* roleProcLinker = createRoleSaturationProcessLinker(calcAlgContext);
									roleProcLinker->initRoleProcessLinker(role);
									succIndiFunctionalConExtData->addLinkedSuccessorAddedRoleProcessLinker(roleProcLinker);
								}
							}
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addFUNCTIONALProcessRoleExtensionFunctionalityAddedToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							CSaturationIndividualNodeSuccessorExtensionData* succExtData = depIndi->getSuccessorExtensionData(true);
							CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* succIndiFunctionalConExtData = succExtData->getFUNCTIONALConceptsExtensionData(true);
							addSuccessorExtensionToProcessingQueue(indiProcSatNode,calcAlgContext);
							if (!succIndiFunctionalConExtData->hasFunctionalityAddedProcessLinkerForRole(role)) {
								CRoleSaturationProcessLinker* roleProcLinker = createRoleSaturationProcessLinker(calcAlgContext);
								roleProcLinker->initRoleProcessLinker(role);
								succIndiFunctionalConExtData->addFunctionalityAddedRoleProcessLinker(roleProcLinker);
							}
						}
					}
				}

				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::markNominalATMOSTRestrictedAncestorsAsInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					return markATMOSTRestrictedAncestorsAsInsufficient(conDes,indiProcSatNode,calcAlgContext);
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::markATMOSTRestrictedAncestorsAsInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool ancestorsRestricted = false;
					CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
					if (backwardPropHash) {
						CConcept* concept = conDes->getConcept();
						bool conceptNegation = conDes->isNegated();
						CRole* role = concept->getRole();
						CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();
						if (backPropDataHash) {
							for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropDataHash->constFind(role), itEnd = backPropDataHash->constEnd(); it != itEnd && it.key() == role; ++it) {
								const CRoleBackwardSaturationPropagationHashData& backPropData = it.value();
								for (CBackwardSaturationPropagationLink* backPropLinkIt = backPropData.mLinkLinker; backPropLinkIt; backPropLinkIt = backPropLinkIt->getNext()) {
									CBackwardSaturationPropagationLink* backPropLink = backPropLinkIt;
									CIndividualSaturationProcessNode* sourceIndi = backPropLink->getSourceIndividual();
									updateDirectAddingIndividualStatusFlags(sourceIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
									setInsufficientNodeOccured(mCalcAlgContext);
									ancestorsRestricted = true;
								}
							}
						}
					}
					return ancestorsRestricted;
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addCriticalConceptForDependentNodes(CConceptSaturationDescriptor* conDes, CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType, CIndividualSaturationProcessNode*& indiProcSatNode, bool directFlagsCheck, cint64 checkFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXLinker<CIndividualSaturationProcessNode*>* depIndiIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiIt; depIndiIt = depIndiIt->getNext()) {
						CIndividualSaturationProcessNode* dependingIndiNode = depIndiIt->getData();
						CIndividualSaturationProcessNodeStatusFlags* statusFlag = nullptr;
						if (directFlagsCheck) {
							statusFlag = dependingIndiNode->getDirectStatusFlags();
						} else {
							statusFlag = dependingIndiNode->getIndirectStatusFlags();
						}
						if (checkFlags == 0 || !statusFlag->hasFlags(checkFlags,false)) {
							addCriticalConceptDescriptor(conDes,conceptType,dependingIndiNode,calcAlgContext);
						}
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::checkCriticalConceptsForNode(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CCriticalSaturationConceptTypeQueues* criticalConceptQueues = indiProcSatNode->getCriticalConceptTypeQueues(false);
					CIndividualSaturationProcessNodeStatusFlags* indirectFlags = indiProcSatNode->getIndirectStatusFlags();
					CIndividualSaturationProcessNodeStatusFlags* directFlags = indiProcSatNode->getDirectStatusFlags();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CCriticalIndividualNodeConceptTestSet* criticalIndiNodeConTestSet = processingDataBox->getSaturationCriticalIndividualNodeConceptTestSet(true);

					criticalConceptQueues->setProcessNodeQueued(false);
					if (!indirectFlags->hasInsufficientFlag()) {
						CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType = CCriticalSaturationConceptTypeQueues::CCT_FORALL;
						CCriticalSaturationConceptQueue* criticalConTypeQueue = criticalConceptQueues->getCriticalSaturationConceptQueue(conceptType,false);
						if (criticalConTypeQueue) {
							while (criticalConTypeQueue->hasCriticalConceptDescriptorLinker() && !indirectFlags->hasInsufficientFlag()) {
								CConceptSaturationProcessLinker* criticalConProcDes = criticalConTypeQueue->takeNextCriticalConceptDescriptor();
								CConceptSaturationDescriptor* criticalConDes = criticalConProcDes->getConceptSaturationDescriptor();

								if (!criticalIndiNodeConTestSet->isConceptTestedForIndividual(criticalConDes,indiProcSatNode)) {
									criticalIndiNodeConTestSet->insertConceptTestedForIndividual(criticalConDes,indiProcSatNode);
									STATINC(SATURATIONCRITICALTESTCOUNT,calcAlgContext);
									if (isCriticalALLConceptDescriptorInsufficient(criticalConDes,indiProcSatNode,calcAlgContext)) {
										updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
										setInsufficientNodeOccured(calcAlgContext);
									} else {
										addCriticalConceptForDependentNodes(criticalConDes,conceptType,indiProcSatNode,false,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
									}
								}

								releaseConceptSaturationProcessLinker(criticalConProcDes,calcAlgContext);
							}
						}
					}

					if (!indirectFlags->hasInsufficientFlag()) {
						CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType = CCriticalSaturationConceptTypeQueues::CCT_ATMOST;
						CCriticalSaturationConceptQueue* criticalConTypeQueue = criticalConceptQueues->getCriticalSaturationConceptQueue(conceptType,false);
						if (criticalConTypeQueue) {
							while (criticalConTypeQueue->hasCriticalConceptDescriptorLinker() && !indirectFlags->hasInsufficientFlag()) {
								CConceptSaturationProcessLinker* criticalConProcDes = criticalConTypeQueue->takeNextCriticalConceptDescriptor();
								CConceptSaturationDescriptor* criticalConDes = criticalConProcDes->getConceptSaturationDescriptor();

								if (!criticalIndiNodeConTestSet->isConceptTestedForIndividual(criticalConDes,indiProcSatNode)) {
									criticalIndiNodeConTestSet->insertConceptTestedForIndividual(criticalConDes,indiProcSatNode);
									bool ancestorPossiblyInsufficient = false;
									STATINC(SATURATIONCRITICALTESTCOUNT,calcAlgContext);
									if (isCriticalATMOSTConceptDescriptorInsufficient(criticalConDes,ancestorPossiblyInsufficient,indiProcSatNode,calcAlgContext)) {
										updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
										setInsufficientNodeOccured(calcAlgContext);
									} else {
										addCriticalConceptForDependentNodes(criticalConDes,conceptType,indiProcSatNode,false,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
									}
									if (indiProcSatNode->hasNominalIntegrated()) {
										markNominalATMOSTRestrictedAncestorsAsInsufficient(criticalConDes,indiProcSatNode,calcAlgContext);
									}
									if (ancestorPossiblyInsufficient) {
										markATMOSTRestrictedAncestorsAsInsufficient(criticalConDes,indiProcSatNode,calcAlgContext);
										updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYPROPLEMATIC,calcAlgContext);
									}
								}

								releaseConceptSaturationProcessLinker(criticalConProcDes,calcAlgContext);
							}
						}
					}




					if (!indirectFlags->hasInsufficientFlag()) {
						CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType = CCriticalSaturationConceptTypeQueues::CCT_VALUE;
						CCriticalSaturationConceptQueue* criticalConTypeQueue = criticalConceptQueues->getCriticalSaturationConceptQueue(conceptType,false);
						if (criticalConTypeQueue) {
							while (criticalConTypeQueue->hasCriticalConceptDescriptorLinker() && !indirectFlags->hasInsufficientFlag()) {
								CConceptSaturationProcessLinker* criticalConProcDes = criticalConTypeQueue->takeNextCriticalConceptDescriptor();
								CConceptSaturationDescriptor* criticalConDes = criticalConProcDes->getConceptSaturationDescriptor();

								if (!criticalIndiNodeConTestSet->isConceptTestedForIndividual(criticalConDes,indiProcSatNode)) {
									criticalIndiNodeConTestSet->insertConceptTestedForIndividual(criticalConDes,indiProcSatNode);
									STATINC(SATURATIONCRITICALTESTCOUNT,calcAlgContext);
									if (isCriticalVALUEConceptDescriptorInsufficient(criticalConDes,indiProcSatNode,calcAlgContext)) {
										updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
										setInsufficientNodeOccured(calcAlgContext);
									} else {
										addCriticalConceptForDependentNodes(criticalConDes,conceptType,indiProcSatNode,false,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
									}
								}

								releaseConceptSaturationProcessLinker(criticalConProcDes,calcAlgContext);
							}
						}
					}


					if (!indirectFlags->hasInsufficientFlag()) {
						CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType = CCriticalSaturationConceptTypeQueues::CCT_NOMINAL;
						CCriticalSaturationConceptQueue* criticalConTypeQueue = criticalConceptQueues->getCriticalSaturationConceptQueue(conceptType,false);
						if (criticalConTypeQueue) {
							while (criticalConTypeQueue->hasCriticalConceptDescriptorLinker() && !indirectFlags->hasInsufficientFlag()) {
								CConceptSaturationProcessLinker* criticalConProcDes = criticalConTypeQueue->takeNextCriticalConceptDescriptor();
								CConceptSaturationDescriptor* criticalConDes = criticalConProcDes->getConceptSaturationDescriptor();

								if (!criticalIndiNodeConTestSet->isConceptTestedForIndividual(criticalConDes,indiProcSatNode)) {
									criticalIndiNodeConTestSet->insertConceptTestedForIndividual(criticalConDes,indiProcSatNode);
									STATINC(SATURATIONCRITICALTESTCOUNT,calcAlgContext);
									if (isCriticalNOMINALConceptDescriptorInsufficient(criticalConDes,indiProcSatNode,calcAlgContext)) {
										updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
										setInsufficientNodeOccured(calcAlgContext);
									} else {
										addCriticalConceptForDependentNodes(criticalConDes,conceptType,indiProcSatNode,false,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
									}
								}

								releaseConceptSaturationProcessLinker(criticalConProcDes,calcAlgContext);
							}
						}
					}




					if (true) {
						CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType = CCriticalSaturationConceptTypeQueues::CCT_DISJUNCTION;
						CCriticalSaturationConceptQueue* criticalConTypeQueue = criticalConceptQueues->getCriticalSaturationConceptQueue(conceptType,false);
						if (criticalConTypeQueue) {
							while (criticalConTypeQueue->hasCriticalConceptDescriptorLinker()) {
								CConceptSaturationProcessLinker* criticalConProcDes = criticalConTypeQueue->takeNextCriticalConceptDescriptor();
								CConceptSaturationDescriptor* criticalConDes = criticalConProcDes->getConceptSaturationDescriptor();

								if (!criticalIndiNodeConTestSet->isConceptTestedForIndividual(criticalConDes,indiProcSatNode)) {
									criticalIndiNodeConTestSet->insertConceptTestedForIndividual(criticalConDes,indiProcSatNode);
									STATINC(SATURATIONCRITICALTESTCOUNT,calcAlgContext);
									if (isCriticalORConceptDescriptorInsufficient(criticalConDes,indiProcSatNode,calcAlgContext)) {
										updateDirectNotDependentAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
										setInsufficientNodeOccured(calcAlgContext);
										addCriticalConceptForDependentNodes(criticalConDes,conceptType,indiProcSatNode,true,0,calcAlgContext);
									}
								}
								releaseConceptSaturationProcessLinker(criticalConProcDes,calcAlgContext);
							}
						}
					}


					if (true) {
						CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType = CCriticalSaturationConceptTypeQueues::CCT_EQCANDIDATE;
						CCriticalSaturationConceptQueue* criticalConTypeQueue = criticalConceptQueues->getCriticalSaturationConceptQueue(conceptType,false);
						if (criticalConTypeQueue) {
							while (criticalConTypeQueue->hasCriticalConceptDescriptorLinker()) {
								CConceptSaturationProcessLinker* criticalConProcDes = criticalConTypeQueue->takeNextCriticalConceptDescriptor();
								CConceptSaturationDescriptor* criticalConDes = criticalConProcDes->getConceptSaturationDescriptor();

								if (!criticalIndiNodeConTestSet->isConceptTestedForIndividual(criticalConDes,indiProcSatNode)) {
									criticalIndiNodeConTestSet->insertConceptTestedForIndividual(criticalConDes,indiProcSatNode);
									if (isCriticalEQCANDConceptDescriptorProblematic(criticalConDes,indiProcSatNode,calcAlgContext)) {
										updateDirectNotDependentAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGEQCANDPROPLEMATIC,calcAlgContext);
										setProblematicEQCandidateOccured(calcAlgContext);
										addCriticalConceptForDependentNodes(criticalConDes,conceptType,indiProcSatNode,true,0,calcAlgContext);
									}
								}
								releaseConceptSaturationProcessLinker(criticalConProcDes,calcAlgContext);
							}
						}
					}
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::collectLinkedSuccessorNodes(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CLinkedRoleSaturationSuccessorHash* linkedRoleSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(true);
					CConceptSaturationDescriptor* lastExaminedConDes = linkedRoleSuccHash->getLastExaminedConceptDescriptor();
					CReapplyConceptSaturationLabelSet* conSet = indiProcSatNode->getReapplyConceptSaturationLabelSet(true);
					CConceptSaturationDescriptor* conDesLinker = conSet->getConceptSaturationDescriptionLinker();
					for (CConceptSaturationDescriptor* conDesIt = conDesLinker; conDesIt && conDesIt != lastExaminedConDes; conDesIt = conDesIt->getNext()) {
						CConceptSaturationDescriptor* conDes = conDesIt;
						CConcept* concept = conDes->getConcept();
						bool conNegation = conDes->isNegated();

						cint64 conCode = concept->getOperatorCode();
						if (!conNegation && (conCode == CCSOME || conCode == CCAQSOME) || conNegation && (conCode == CCALL)) {
							addLinkedSuccessorNodeForConcept(conDes,linkedRoleSuccHash,indiProcSatNode,calcAlgContext);
						}
						if (!conNegation && conCode == CCATLEAST || conNegation && conCode == CCATMOST) {
							addLinkedSuccessorNodeForConcept(conDes,linkedRoleSuccHash,indiProcSatNode,calcAlgContext);
						}
						if (!conNegation && conCode == CCVALUE) {
							addLinkedSuccessorNodeForConcept(conDes,linkedRoleSuccHash,indiProcSatNode,calcAlgContext);
						}
					}
					linkedRoleSuccHash->setLastExaminedConceptDescriptor(conDesLinker);
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addLinkedSuccessorNodeForConcept(CConceptSaturationDescriptor* conDes, CLinkedRoleSaturationSuccessorHash* linkedRoleSuccHash, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					bool conNegation = conDes->isNegated();
					CRole* role = concept->getRole();
					cint64 param = concept->getParameter();
					cint64 cardinality = param + 1*conNegation;

					cint64 conCode = concept->getOperatorCode();

					bool addSuccessor = false;
					bool nominalSuccessor = false;
					cint64 successorCount = 0;
					cint64 nominalID = 0;
					if (!conNegation && (conCode == CCVALUE)) {
						addSuccessor = true;
						successorCount = 1;
						nominalSuccessor = true;
						CIndividual* nominalIndividual = concept->getNominalIndividual();
						nominalID = nominalIndividual->getIndividualID();
					}
					if (!conNegation && (conCode == CCSOME || conCode == CCAQSOME) || conNegation && (conCode == CCALL)) {
						addSuccessor = true;
						successorCount = 1;
						nominalSuccessor = false;
					}
					if (cardinality >= 1 && (!conNegation && conCode == CCATLEAST || conNegation && conCode == CCATMOST)) {
						addSuccessor = true;
						successorCount = cardinality;
						nominalSuccessor = false;
					}

					if (addSuccessor) {
						bool foundSpecialIndiNode = false;
						bool foundOperandIndiNode = false;
						CConceptData* conceptData = concept->getConceptData();
						if (conceptData) {
							CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
							CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
							if (conRefLinking) {
								CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
								CSaturationConceptReferenceLinking* extSatCalcRefLinkData = confSatRefLinkingData->getExistentialSuccessorConceptSaturationReferenceLinkingData();
								if (extSatCalcRefLinkData) {
									CIndividualSaturationProcessNode* existIndiNode = (CIndividualSaturationProcessNode*)extSatCalcRefLinkData->getIndividualProcessNodeForConcept();
									if (existIndiNode) {
										foundSpecialIndiNode = true;
										CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
										while (superRoleIt) {
											CRole* superRole = superRoleIt->getData();
											if (!superRoleIt->isNegated()) {
												if (nominalSuccessor) {
													linkedRoleSuccHash->addLinkedVALUESuccessor(superRole,nominalID,role);
												} else {
													linkedRoleSuccHash->addLinkedSuccessor(superRole,existIndiNode,role,successorCount);
												}
											}
											superRoleIt = superRoleIt->getNext();
										}
									}
								}
							}
						}
						if (!foundSpecialIndiNode) {
							CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
							while (conceptOpLinkerIt) {
								foundOperandIndiNode = true;
								CConcept* opConcept = conceptOpLinkerIt->getData();
								bool opConNegation = conceptOpLinkerIt->isNegated()^conNegation;
								CConceptData* opConceptData = opConcept->getConceptData();
								if (opConceptData) {
									CConceptProcessData* conProcData = (CConceptProcessData*)opConceptData;
									CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
									if (conRefLinking) {
										CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
										CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(opConNegation);
										if (satCalcRefLinkData) {
											CIndividualSaturationProcessNode* existIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
											if (existIndiNode) {
												CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
												while (superRoleIt) {
													CRole* superRole = superRoleIt->getData();
													if (!superRoleIt->isNegated()) {
														if (nominalSuccessor) {
															linkedRoleSuccHash->addLinkedVALUESuccessor(superRole,nominalID,role);
														} else {
															linkedRoleSuccHash->addLinkedSuccessor(superRole,existIndiNode,role,successorCount);
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
						if (!foundSpecialIndiNode && !foundOperandIndiNode) {
							CConcept* opConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
							bool opConNegation = false;
							CConceptData* opConceptData = opConcept->getConceptData();
							if (opConceptData) {
								CConceptProcessData* conProcData = (CConceptProcessData*)opConceptData;
								CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
								if (conRefLinking) {
									CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
									CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(opConNegation);
									if (satCalcRefLinkData) {
										CIndividualSaturationProcessNode* existIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
										if (existIndiNode) {
											CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
											while (superRoleIt) {
												CRole* superRole = superRoleIt->getData();
												if (!superRoleIt->isNegated()) {
													if (nominalSuccessor) {
														linkedRoleSuccHash->addLinkedVALUESuccessor(superRole,nominalID,role);
													} else {
														linkedRoleSuccHash->addLinkedSuccessor(superRole,existIndiNode,role,successorCount);
													}
												}
												superRoleIt = superRoleIt->getNext();
											}
										}
									}
								}
							}
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addCriticalConceptDescriptor(CConceptSaturationDescriptor* conDes, CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mConfAddCriticalConceptsToQueues) {
						STATINC(SATURATIONCRITICALADDCOUNT,calcAlgContext);
						CConceptSaturationProcessLinker* conDesProLinker = createConceptSaturationProcessLinker(calcAlgContext);
						conDesProLinker->initConceptSaturationProcessLinker(conDes);
						CCriticalSaturationConceptTypeQueues* queues = indiProcSatNode->getCriticalConceptTypeQueues(true);
						CCriticalSaturationConceptQueue* criticalConceptQueue = queues->getCriticalSaturationConceptQueue(conceptType,true);
						criticalConceptQueue->addCriticalConceptDescriptorLinker(conDesProLinker);
						if (!queues->isProcessNodeQueued()) {
							// TODO: insert into a queue
							CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
							CCriticalIndividualNodeProcessingQueue* criticalIndNodProcQueue = processingDataBox->getSaturationCriticalIndividualNodeProcessingQueue(true);
							criticalIndNodProcQueue->insertProcessIndiviudal(indiProcSatNode);
							queues->setProcessNodeQueued(true);
						}
					}
					if (mConfDirectlyCriticalToInsufficient) {
						updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
						setInsufficientNodeOccured(mCalcAlgContext);
					}
				}





				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::testInsufficientALLConcepts(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					collectLinkedSuccessorNodes(indiProcSatNode,calcAlgContext);
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
					if (backwardPropHash && linkedSuccHash) {
						CSaturationIndividualNodeSuccessorExtensionData* succExtData = indiProcSatNode->getSuccessorExtensionData(false);
						if (succExtData && !succExtData->isExtensionProcessingQueued()) {
							CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();
							if (backPropDataHash) {
								CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
								for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator itSucc = succHash->constBegin(), itSuccEnd = succHash->constEnd(); itSucc != itSuccEnd; ++itSucc) {
									CRole* role(itSucc.key());
									CLinkedRoleSaturationSuccessorData* succData(itSucc.value());
									for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator itSuccIndi = succData->mSuccNodeDataMap.constBegin(), itSuccIndiEnd = succData->mSuccNodeDataMap.constEnd(); itSuccIndi != itSuccIndiEnd; ++itSuccIndi) {
										CSaturationSuccessorData* succDataIndi(itSuccIndi.value());
										if (succDataIndi->mActiveCount >= 1) {
											if (succDataIndi->mVALUENominalConnection) {
											} else {
												CIndividualSaturationProcessNode* succIndi = succDataIndi->mSuccIndiNode;
												CReapplyConceptSaturationLabelSet* succConSet = succIndi->getReapplyConceptSaturationLabelSet(false);
												const CRoleBackwardSaturationPropagationHashData& backPropData = backPropDataHash->value(role);
												for (CBackwardSaturationPropagationReapplyDescriptor* backPropIt = backPropData.mReapplyLinker; backPropIt; backPropIt = backPropIt->getNext()) {
													CConceptSaturationDescriptor* conDes = backPropIt->getReapplyConceptSaturationDescriptor();
													CConcept* concept = conDes->getConcept();
													bool negation = conDes->isNegated();
													for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
														CConcept* opConcept = opLinkerIt->getData();
														bool opNegation = opLinkerIt->isNegated()^negation;
														if (!succConSet || !succConSet->containsConcept(opConcept,opNegation)) {
															mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
															QFile tmpFile("saturation-model.txt");
															if (tmpFile.open(QIODevice::WriteOnly)) {
																tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
																tmpFile.close();
															}
															bool bug = true;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isCriticalALLConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(SATURATIONCRITICALALLCOUNT,calcAlgContext);
					if (!indiProcSatNode->hasSubstituteIndividualNode()) {
						collectLinkedSuccessorNodes(indiProcSatNode,calcAlgContext);
						CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
						if (linkedSuccHash) {
							CConcept* concept = conDes->getConcept();
							bool conceptNegation = conDes->isNegated();
							CRole* role = concept->getRole();
							CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
							CLinkedRoleSaturationSuccessorData* succData = succHash->value(role);
							if (succData) {
								for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator itSucc = succData->mSuccNodeDataMap.constBegin(), itSuccEnd = succData->mSuccNodeDataMap.constEnd(); itSucc != itSuccEnd; ++itSucc) {
									CSaturationSuccessorData* indiSuccData(itSucc.value());
									if (indiSuccData->mActiveCount >= 1) {
										if (indiSuccData->mVALUENominalConnection) {
											if (loadConsistenceModelData(calcAlgContext)) {
												CIndividualProcessNode* indiProcNode = getCorrectedNode(indiSuccData->mVALUENominalID,mDetCachedCGIndiVector,mCalcAlgContext);
												CReapplyConceptLabelSet* reapplyConSet = indiProcNode->getReapplyConceptLabelSet(false);
												bool operantsContained = true;
												if (reapplyConSet) {
													for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt && operantsContained; opLinkerIt = opLinkerIt->getNext()) {
														CConcept* opConcept = opLinkerIt->getData();
														bool opConceptNegation = opLinkerIt->isNegated()^conceptNegation;
														if (!reapplyConSet->containsConcept(opConcept,opConceptNegation)) {
															operantsContained = false;
														}
													}
												} else {
													operantsContained = false;
												}
												if (!operantsContained) {
													return true;
												}
											}
										} else {
											CIndividualSaturationProcessNode* succNode = indiSuccData->mSuccIndiNode;
											CReapplyConceptSaturationLabelSet* succConSet = succNode->getReapplyConceptSaturationLabelSet(false);
											bool operantsContained = true;
											if (succConSet) {
												for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt && operantsContained; opLinkerIt = opLinkerIt->getNext()) {
													CConcept* opConcept = opLinkerIt->getData();
													bool opConceptNegation = opLinkerIt->isNegated()^conceptNegation;
													if (!succConSet->containsConcept(opConcept,opConceptNegation)) {
														operantsContained = false;
													}
												}
											} else {
												operantsContained = false;
											}
											if (!operantsContained) {

												//mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
												//QFile tmpFile("saturation-model.txt");
												//if (tmpFile.open(QIODevice::WriteOnly)) {
												//	tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
												//	tmpFile.close();
												//}
												//bool bug = true;


												//CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
												//if (backwardPropHash) {
												//	CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backwardPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();

												//	CRoleBackwardSaturationPropagationHashData* backwardPropData(backwardPropDataHash->valuePointer(role));
												//	if (backwardPropData) {
												//		bool debug = true;
												//	}
												//}

												return true;
											}
										}
									}
								}
							}
						}
					}
					return false;
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isCriticalORConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					STATINC(SATURATIONCRITICALORCOUNT,calcAlgContext);
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = conDes->isNegated();
					CReapplyConceptSaturationLabelSet* conSet = indiProcSatNode->getReapplyConceptSaturationLabelSet(false);
					if (conSet) {
						for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
							CConcept* opConcept = opLinkerIt->getData();
							bool opConceptNegation = opLinkerIt->isNegated()^conceptNegation;
							if (conSet->containsConcept(opConcept,opConceptNegation)) {
								return false;
							}
						}
					}
					return true;
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isCriticalEQCANDConceptDescriptorProblematic(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					STATINC(SATURATIONCRITICALORCOUNT,calcAlgContext);
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = conDes->isNegated();
					CReapplyConceptSaturationLabelSet* conSet = indiProcSatNode->getReapplyConceptSaturationLabelSet(false);
					if (conSet) {
						for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
							CConcept* opConcept = opLinkerIt->getData();
							bool opConceptNegation = opLinkerIt->isNegated()^conceptNegation;
							if (conSet->containsConcept(opConcept,opConceptNegation)) {
								return false;
							}
						}
					}
					return true;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isCriticalATMOSTConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, bool& ancestorPossiblyCriticalFlag, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(SATURATIONCRITICALATMOSTCOUNT,calcAlgContext);
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = conDes->isNegated();
					CRole* role = concept->getRole();
					cint64 allowedCardinality = concept->getParameter() - 1*conceptNegation;
					if (allowedCardinality < 0) {
						return true;
					}
					if (!indiProcSatNode->hasSubstituteIndividualNode()) {
						collectLinkedSuccessorNodes(indiProcSatNode,calcAlgContext);
						CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
						if (linkedSuccHash) {
							cint64 foundCardinality = 0;
							CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
							CLinkedRoleSaturationSuccessorData* succData = succHash->value(role);
							if (succData) {
								if (succData->mSuccCount >= allowedCardinality) {
									for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator itSucc = succData->mSuccNodeDataMap.constBegin(), itSuccEnd = succData->mSuccNodeDataMap.constEnd(); itSucc != itSuccEnd; ++itSucc) {
										CSaturationSuccessorData* succRoleData = itSucc.value();
										if (succRoleData->mActiveCount >= 1) {
											cint64 succCardinality(succRoleData->mSuccCount);
											bool operantsContainedNegative = true;
											bool operantsContainedPositive = true;
											if (succRoleData->mVALUENominalConnection) {
												CIndividualProcessNode* indiProcNode = getCorrectedNode(succRoleData->mVALUENominalID,mDetCachedCGIndiVector,mCalcAlgContext);
												CReapplyConceptLabelSet* reapplyConSet = indiProcNode->getReapplyConceptLabelSet(false);
												if (reapplyConSet) {
													if (concept->getOperandList()) {
														for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
															CConcept* opConcept = opLinkerIt->getData();
															bool opConceptNegation = opLinkerIt->isNegated()^conceptNegation;
															bool containedNegation = false;
															if (!reapplyConSet->containsConcept(opConcept,&containedNegation)) {
																if (containedNegation == opConceptNegation) {
																	operantsContainedNegative = false;
																} else {
																	operantsContainedPositive = false;
																}
															} else {
																operantsContainedPositive = false;
																operantsContainedNegative = false;
															}
														}
													} else {
														operantsContainedNegative = false;
													}
												} else {
													operantsContainedPositive = false;
													operantsContainedNegative = false;
												}

											} else {
												CIndividualSaturationProcessNode* succNode(succRoleData->mSuccIndiNode);							
												CReapplyConceptSaturationLabelSet* succConSet = succNode->getReapplyConceptSaturationLabelSet(false);
												if (succConSet) {
													if (concept->getOperandList()) {
														for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
															CConcept* opConcept = opLinkerIt->getData();
															bool opConceptNegation = opLinkerIt->isNegated()^conceptNegation;
															bool containedNegation = false;
															if (!succConSet->containsConcept(opConcept,&containedNegation)) {
																if (containedNegation == opConceptNegation) {
																	operantsContainedNegative = false;
																} else {
																	operantsContainedPositive = false;
																}
															} else {
																operantsContainedPositive = false;
																operantsContainedNegative = false;
															}
														}
													} else {
														operantsContainedNegative = false;

													}
												} else {
													operantsContainedPositive = false;
													operantsContainedNegative = false;
												}
											}
											if (!operantsContainedNegative) {
												foundCardinality += succCardinality;
											}
											if (foundCardinality == allowedCardinality) {
												ancestorPossiblyCriticalFlag = true;
											}
											if (foundCardinality > allowedCardinality) {

												//mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
												//QFile tmpFile("saturation-model.txt");
												//if (tmpFile.open(QIODevice::WriteOnly)) {
												//	tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
												//	tmpFile.close();
												//}
												//bool bug = true;

												return true;
											}
										}
									}
								}
							}
						}
					}
					return false;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isCriticalNOMINALConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CIndividual* nominal = concept->getNominalIndividual();
					cint64 nominalID = nominal->getIndividualID();

					if (loadConsistenceModelData(calcAlgContext)) {
						CIndividualProcessNode* detNominalProcessNode = getCorrectedNode(nominalID,mDetCachedCGIndiVector,mCalcAlgContext);
						CIndividualProcessNode* nonDetNominalProcessNode = getCorrectedNode(nominalID,mNonDetCachedCGIndiVector,mCalcAlgContext);
						CReapplyConceptLabelSet* detNominalReapplyConSet = detNominalProcessNode->getReapplyConceptLabelSet(false);
						CReapplyConceptLabelSet* nonDetNominalReapplyConSet = nonDetNominalProcessNode->getReapplyConceptLabelSet(false);

						CReapplyConceptSaturationLabelSet* satIndiNodeConSet = indiProcSatNode->getReapplyConceptSaturationLabelSet(false);

						CConceptDescriptor* lastDetConDesIt = detNominalReapplyConSet->getAddingSortedConceptDescriptionLinker();
						for (CConceptDescriptor* conDesIt = nonDetNominalReapplyConSet->getAddingSortedConceptDescriptionLinker(); conDesIt != lastDetConDesIt; conDesIt = conDesIt->getNext()) {
							CConcept* nominalConcept = conDesIt->getConcept();
							bool nominalConceptNegation = conDesIt->isNegated();
							if (!satIndiNodeConSet->containsConcept(nominalConcept,nominalConceptNegation)) {
								return true;
							}
						}
						return false;
					} else {
						return true;
					}
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isCriticalVALUEConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CIndividual* nominal = concept->getNominalIndividual();
					cint64 nominalID = nominal->getIndividualID();

					if (loadConsistenceModelData(calcAlgContext)) {
						CIndividualProcessNode* detNominalProcessNode = getCorrectedNode(nominalID,mDetCachedCGIndiVector,mCalcAlgContext);
						CIndividualProcessNode* nonDetNominalProcessNode = getCorrectedNode(nominalID,mNonDetCachedCGIndiVector,mCalcAlgContext);
						CReapplyConceptLabelSet* detNominalReapplyConSet = detNominalProcessNode->getReapplyConceptLabelSet(false);
						CReapplyRoleSuccessorHash* nonDetNominalReapplyRoleSuccHash = nonDetNominalProcessNode->getReapplyRoleSuccessorHash(false);

						CReapplyConceptSaturationLabelSet* satIndiNodeConSet = indiProcSatNode->getReapplyConceptSaturationLabelSet(false);

						if (nonDetNominalReapplyRoleSuccHash) {
							for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
								CRole* superRole = superRoleIt->getData();
								bool inversedSuperRole = superRoleIt->isNegated();
								if (!inversedSuperRole) {
									CReapplyQueueIterator reapplyRoleIt(nonDetNominalReapplyRoleSuccHash->getRoleReapplyIterator(superRole));
									while (reapplyRoleIt.hasNext()) {
										CReapplyConceptDescriptor* reapplyDes = reapplyRoleIt.next();
										CConceptDescriptor* reapplyConDes = reapplyDes->getConceptDescriptor();
										CConcept* reapplyConcept = reapplyConDes->getConcept();
										bool reapplyConceptNegation = reapplyConDes->isNegated();
										if (!reapplyConceptNegation && detNominalReapplyConSet->containsConcept(reapplyConcept,reapplyConceptNegation)) {

											cint64 reapplyConceptCode = reapplyConcept->getOperatorCode();
											CConceptOperator* reapplyConceptOperator = reapplyConcept->getConceptOperator();
											if (!reapplyConceptNegation && reapplyConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_ALL_TYPE)) {
												return true;
											} else if (!reapplyConceptNegation && reapplyConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE) || reapplyConceptNegation && reapplyConceptCode == CCSOME) {
												for (CSortedNegLinker<CConcept*>* reapplyConceptOpLinkerIt = reapplyConcept->getOperandList(); reapplyConceptOpLinkerIt; reapplyConceptOpLinkerIt = reapplyConceptOpLinkerIt->getNext()) {
													CConcept* reapplyOperandConcept = reapplyConceptOpLinkerIt->getData();
													bool reapplyOperandConceptNegation = reapplyConceptOpLinkerIt->isNegated()^reapplyConceptNegation;
													if (!satIndiNodeConSet->containsConcept(reapplyOperandConcept,reapplyOperandConceptNegation)) {
														return true;
													}
												}
											} else if (!reapplyConceptNegation && reapplyConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE)) {
												testAutomateTransitionOperandsAddable(indiProcSatNode,reapplyConcept,role,mCalcAlgContext);
											} else {
												return true;
											}
											return false;
										}
									}
								}
							}
						}
						return false;
					} else {
						return true;
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateExtractDisjunctCommonConcept(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)indiProcSatNode->getSaturationConceptReferenceLinking();;
					CConcept* disjunctionConcept = conceptSatItem->getSaturationConcept();
					bool disjunctionNegation = conceptSatItem->getSaturationNegation();
					CSortedNegLinker<CConcept*>* disjunctConceptLinker = disjunctionConcept->getOperandList();
					CSaturationDisjunctCommonConceptExtractionData* extractionData = indiProcSatNode->getDisjunctCommonConceptExtractionData(false);

					if (extractionData) {
						CReapplyConceptSaturationLabelSet* disjunctionConSet = indiProcSatNode->getReapplyConceptSaturationLabelSet(true);
						CSaturationDisjunctCommonConceptCountHash* commonConceptCountHash = extractionData->getSaturationDisjunctCommonConceptCountHash();

						for (CSaturationDisjunctExtractionLinker* satDisjExtLinkerIt = extractionData->getDisjunctIndividualNodeExtractionLinker(); satDisjExtLinkerIt; satDisjExtLinkerIt = satDisjExtLinkerIt->getNext()) {
							CIndividualSaturationProcessNode* disjConIndiNode = satDisjExtLinkerIt->getDisjunctIndividualSaturationProcessNode();

							CConceptSaturationDescriptor* lastExaminedDisjConSatDes = satDisjExtLinkerIt->getLastExaminedConceptSaturationDescriptor();
							CReapplyConceptSaturationLabelSet* disjConConSet = disjConIndiNode->getReapplyConceptSaturationLabelSet(false);
							if (disjConConSet) {
								CConceptSaturationDescriptor* newLastExaminedDisjConSatDes = disjConConSet->getConceptSaturationDescriptionLinker();
								satDisjExtLinkerIt->setLastExaminedConceptSaturationDescriptor(newLastExaminedDisjConSatDes);
								for (CConceptSaturationDescriptor* disjConSatDesIt = newLastExaminedDisjConSatDes; disjConSatDesIt != lastExaminedDisjConSatDes; disjConSatDesIt = disjConSatDesIt->getNext()) {
									CConcept* disjConcept = disjConSatDesIt->getConcept();
									bool disjNegation = disjConSatDesIt->isNegated();
									if (commonConceptCountHash->incCommonConceptCountReturnMaxReached(disjConSatDesIt)) {
										addConceptFilteredToIndividual(disjConcept,disjNegation,indiProcSatNode,disjunctionConSet,true,calcAlgContext);
									}
								}
							}
						}
					}
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::initializeExtractDisjunctCommonConcept(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CSaturationDisjunctCommonConceptExtractionData* extractionData = indiProcSatNode->getDisjunctCommonConceptExtractionData(true);
					CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)indiProcSatNode->getSaturationConceptReferenceLinking();;
					CConcept* disjunctionConcept = conceptSatItem->getSaturationConcept();
					bool disjunctionNegation = conceptSatItem->getSaturationNegation();

					CSortedNegLinker<CConcept*>* disjunctConceptLinker = disjunctionConcept->getOperandList();
					cint64 disjCount = 0;
					for (CSortedNegLinker<CConcept*>* disjunctConceptLinkerIt = disjunctConceptLinker; disjunctConceptLinkerIt; disjunctConceptLinkerIt = disjunctConceptLinkerIt->getNext()) {
						++disjCount;
						CConcept* disjunctConcept = disjunctConceptLinkerIt->getData();
						bool disjunctNegation = disjunctConceptLinkerIt->isNegated()^disjunctionNegation;
						CConceptProcessData* conProData = (CConceptProcessData*)disjunctConcept->getConceptData();
						CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
						CExtendedConceptReferenceLinkingData* disjunctConceptRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(disjunctNegation);
						CIndividualSaturationProcessNode* disConIndiNode = (CIndividualSaturationProcessNode*)disjunctConceptRefLinkData->getIndividualProcessNodeForConcept();
						addUninitializedIndividualToProcessingQueue(disConIndiNode,calcAlgContext);

						CSaturationDisjunctExtractionLinker* disNodeExtLinker = CObjectAllocator<CSaturationDisjunctExtractionLinker>::allocateAndConstruct(taskMemMan);
						disNodeExtLinker->initSaturationDisjunctExtractionLinker(disConIndiNode,nullptr);
						extractionData->addDisjunctIndividualNodeExtractionLinker(disNodeExtLinker);

						CSaturationModifiedProcessUpdateLinker* modProcUpdLinker = createModifiedProcessUpdateLinker(calcAlgContext);
						modProcUpdLinker->initProcessUpdateLinker(indiProcSatNode,CSaturationModifiedProcessUpdateLinker::UPDATEPDISJUNCTCOMMONCONCEPTSEXTRACTION);
						disConIndiNode->getReapplyConceptSaturationLabelSet(true)->addModifiedUpdateLinker(modProcUpdLinker);

					}
					extractionData->getSaturationDisjunctCommonConceptCountHash()->setDisjunctCount(disjCount);

					updateExtractDisjunctCommonConcept(indiProcSatNode,calcAlgContext);
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addDisjunctCommonConceptExtractionToProcessingQueue(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationDisjunctCommonConceptExtractionData* extractionData = indiProcSatNode->getDisjunctCommonConceptExtractionData(false);
					if (extractionData) {
						CIndividualSaturationProcessNodeLinker* processNodeLinker = extractionData->getExtractionContinueProcessLinker();
						if (!processNodeLinker->isProcessingQueued()) {
							processNodeLinker->setProcessingQueued();
							calcAlgContext->getUsedProcessingDataBox()->addIndividualDisjunctCommonConceptExtractProcessLinker(processNodeLinker);
						}
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::initializeInitializationConcepts(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					bool requiredBackProp = true;

					CIndividualSaturationProcessNode* specialIndiNode = nullptr;
					CSaturationConceptReferenceLinking* confSatRefLinkingData = nullptr;
					CSaturationConceptDataItem* conceptSatItem = nullptr;

					bool copyIndividualNode = false;
					bool substituiteIndividualNode = false;

					confSatRefLinkingData = (CSaturationConceptReferenceLinking*)indiProcSatNode->getSaturationConceptReferenceLinking();
					conceptSatItem = (CSaturationConceptDataItem*)confSatRefLinkingData;

					CConcept* initConcept = nullptr;
					bool initNegated = false;
					CRole* initRole = nullptr;

					if (conceptSatItem) {
						initConcept = conceptSatItem->getSaturationConcept();
						initNegated = conceptSatItem->getSaturationNegation();
						initRole = conceptSatItem->getSaturationRoleRanges();

						if (!mConfForceAllConceptInsertion) {
							requiredBackProp = conceptSatItem->isPotentiallyExistInitializationConcept();
						}
					}

					if (conceptSatItem) {
						CSaturationConceptDataItem* specialRefItem = conceptSatItem->getSpecialItemReference();
						if (specialRefItem) {
							specialIndiNode = (CIndividualSaturationProcessNode*)specialRefItem->getIndividualProcessNodeForConcept();
						}
					}

					if (conceptSatItem) {
						CSaturationConceptDataItem::SATURATIONITEMREFERENCESPECIALMODE mode = conceptSatItem->getSpecialReferenceMode();
						if (mode == CSaturationConceptDataItem::SATURATIONCOPYMODE) {
							copyIndividualNode = true;
						} else if (mode == CSaturationConceptDataItem::SATURATIONSUBSTITUTEMODE) {
							substituiteIndividualNode = true;
						}
					}

					bool addInitializationConcepts = true;
					bool initialized = false;
					if (specialIndiNode && !specialIndiNode->isInitialized()) {
						specialIndiNode = nullptr;
					}
					if (specialIndiNode) {
						indiProcSatNode->setReferenceIndividualSaturationProcessNode(specialIndiNode);
					}
					if (specialIndiNode && substituiteIndividualNode && (mConfForceAllCopyInsteadOfSubstituition || initRole)) {
						substituiteIndividualNode = false;
						copyIndividualNode = true;
					}
					if (specialIndiNode && substituiteIndividualNode) {
						CIndividualSaturationProcessNode* blockedIndiNode = specialIndiNode;
						while (blockedIndiNode->hasSubstituteIndividualNode()) {
							blockedIndiNode = blockedIndiNode->getSubstituteIndividualNode();
						}
						CReapplyConceptSaturationLabelSet* blockConSet = blockedIndiNode->getReapplyConceptSaturationLabelSet(false);
						CConceptSaturationDescriptor* conceptSaturationDescriptor = nullptr;
						CImplicationReapplyConceptSaturationDescriptor* impReapplyConSatDes = nullptr;
						bool contained = false;
						if (blockConSet->getConceptDescriptorAndReapplyQueue(initConcept,conceptSaturationDescriptor,impReapplyConSatDes)) {
							contained = true;
						}
						if (!contained) {
							++mSubstituitedIndiNodeCount;
							indiProcSatNode->initSubstituitingIndividualSaturationProcessNode(blockedIndiNode);
							indiProcSatNode->setSubstituteIndividualNode(specialIndiNode);
							indiProcSatNode->setReferenceMode(1);
							indiProcSatNode->clearConceptSaturationProcessLinker();
							addInitializationConcepts = false;
							initialized = true;

							updateDirectAddingIndividualStatusFlags(indiProcSatNode,blockedIndiNode->getDirectStatusFlags(),mCalcAlgContext);							
							updateIndirectAddingIndividualStatusFlags(indiProcSatNode,blockedIndiNode->getIndirectStatusFlags(),mCalcAlgContext);							

						} else {
							copyIndividualNode = true;
						}
					} 
					if (specialIndiNode && copyIndividualNode) {
						CIndividualSaturationProcessNode* blockedIndiNode = specialIndiNode;
						while (blockedIndiNode->hasSubstituteIndividualNode()) {
							blockedIndiNode = blockedIndiNode->getSubstituteIndividualNode();
						}
						initializeIndividualNodeByCoping(indiProcSatNode,blockedIndiNode,mCalcAlgContext);
						//indiProcSatNode->initCopingIndividualSaturationProcessNode(blockedIndiNode);
						//indiProcSatNode->setReferenceMode(2);
						//indiProcSatNode->setCopyIndividualNode(blockedIndiNode);
						initialized = true;
						//testDebugSaturationTaskContainsConcepts(indiProcSatNode,calcAlgContext);

						//updateDirectAddingIndividualStatusFlags(indiProcSatNode,blockedIndiNode->getDirectStatusFlags(),mCalcAlgContext);							
						//updateIndirectAddingIndividualStatusFlags(indiProcSatNode,blockedIndiNode->getIndirectStatusFlags(),mCalcAlgContext);							

					}
					if (!initialized) {
						indiProcSatNode->initRootIndividualSaturationProcessNode();
						indiProcSatNode->setReferenceMode(4);
						CConcept* topConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
						addConceptFilteredToIndividual(topConcept,false,indiProcSatNode,false,calcAlgContext);
						CConcept* univConnNomValueConcept = calcAlgContext->getProcessingDataBox()->getOntology()->getTBox()->getUniversalConnectionNominalValueConcept();
						if (univConnNomValueConcept) {
							addConceptFilteredToIndividual(univConnNomValueConcept,false,indiProcSatNode,false,calcAlgContext);
						}
						initialized = true;
					}

					addIndividualToCompletionQueue(indiProcSatNode,calcAlgContext);

					if (addInitializationConcepts) {
						if (initConcept) {
							addConceptFilteredToIndividual(initConcept,initNegated,indiProcSatNode,indiProcSatNode->getReapplyConceptSaturationLabelSet(true),false,calcAlgContext);
						}
						if (initRole) {
							for (CSortedNegLinker<CRole*>* superRoleIt = initRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
								CRole* superRole = superRoleIt->getData();
								bool roleInversed = superRoleIt->isNegated();
								for (CSortedNegLinker<CConcept*>* rangeConceptIt = superRole->getDomainRangeConceptList(!roleInversed); rangeConceptIt; rangeConceptIt = rangeConceptIt->getNext()) {
									CConcept* rangeConcept = rangeConceptIt->getData();
									bool rangeConceptNegation = rangeConceptIt->isNegated();
									addConceptFilteredToIndividual(rangeConcept,rangeConceptNegation,indiProcSatNode,calcAlgContext);
								}
							}
						}
					}
					indiProcSatNode->setRequiredBackwardPropagation(requiredBackProp);
					CBackwardSaturationPropagationLink* backSatPropLinkIt = indiProcSatNode->getInitializingBackwardPropagationLinks();
					if (backSatPropLinkIt) {
						CRoleBackwardSaturationPropagationHash* backPropHash = indiProcSatNode->getRoleBackwardPropagationHash(true);
						while (backSatPropLinkIt) {
							CBackwardSaturationPropagationLink* backPropLink = backSatPropLinkIt;
							backSatPropLinkIt = backSatPropLinkIt->getNext();
							backPropLink->clearNext();
							CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDes = backPropHash->addBackwardPropagationLink(backPropLink->getLinkRole(),backPropLink);
							if (backPropReapplyDes) {
								applyBackwardPropagationConcepts(backPropLink,backPropReapplyDes,mCalcAlgContext);
							}
							updateIndirectAddingIndividualStatusFlags(backPropLink->getSourceIndividual(),indiProcSatNode->getIndirectStatusFlags(),mCalcAlgContext);							
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::individualNodeConclusion(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptSaturationProcessLinker* conSatProLinker = indiProcSatNode->getConceptSaturationProcessLinker();
					if (conSatProLinker) {
						addIndividualToProcessingQueue(indiProcSatNode,calcAlgContext);
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyTableauSaturationRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNeg = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					cint64 conOpCode = concept->getOperatorCode();

					TableauRuleFunction func = nullptr;
					if (!conNeg) {
						func = mPosJumpFuncVec[conOpCode];
						if (func) {
							(this->*func)(processIndi,conSatProLinker);
						} else {
							applyELSERule(processIndi,conSatProLinker);
						}
					} else {
						func = mNegJumpFuncVec[conOpCode];
						if (func) {
							(this->*func)(processIndi,conSatProLinker);
						} else {
							applyELSERule(processIndi,conSatProLinker);
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyAutomatChooseRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(AUTOMATEINITCOUNT,mCalcAlgContext);
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					while (conceptOpLinkerIt) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opNegation = conceptOpLinkerIt->isNegated();
						if (opNegation == conNegation) {
							addConceptFilteredToIndividual(opConcept,false,processIndi,false,mCalcAlgContext);
						}
						conceptOpLinkerIt = conceptOpLinkerIt->getNext();
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyANDRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(ANDRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					addConceptsFilteredToIndividual(conceptOpLinkerIt,conNegation,processIndi,false,mCalcAlgContext);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyNONERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyIMPLICATIONRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(ANDRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					CImplicationReapplyConceptSaturationDescriptor tmpNewReapplyImpReapplyConSatDes;
					CConcept* implConcept = conDes->getConcept();
					CSortedNegLinker<CConcept*>* nextTriggerConcept = implConcept->getOperandList();
					tmpNewReapplyImpReapplyConSatDes.initImplicationReapllyConceptSaturationDescriptor(implConcept,nextTriggerConcept);
					updateImplicationReapplyConceptSaturationDescriptor(&tmpNewReapplyImpReapplyConSatDes,processIndi,processIndi->getReapplyConceptSaturationLabelSet(true),mCalcAlgContext);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyORRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					if (concept->getOperandCount() == 0) {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,mCalcAlgContext);
					} else if (concept->getOperandCount() == 1) {
						STATINC(ANDRULEAPPLICATIONCOUNT,mCalcAlgContext);
						CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
						addConceptsFilteredToIndividual(conceptOpLinkerIt,conNegation,processIndi,false,mCalcAlgContext);
					} else {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCRITICAL,mCalcAlgContext);
						addCriticalConceptDescriptor(conDes,CCriticalSaturationConceptTypeQueues::CCT_DISJUNCTION,processIndi,mCalcAlgContext);
						CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)processIndi->getSaturationConceptReferenceLinking();
						if (conceptSatItem) {
							CConcept* indiConcept = conceptSatItem->getSaturationConcept();
							bool indiConNegation = conceptSatItem->getSaturationNegation();

							if (concept == indiConcept && conNegation == indiConNegation) {
								initializeExtractDisjunctCommonConcept(processIndi,mCalcAlgContext);
							} else {
								if (indiConcept) {
									CConceptProcessData* disjunctionConProData = (CConceptProcessData*)concept->getConceptData();
									CConceptSaturationReferenceLinkingData* disjConRefLinking = (CConceptSaturationReferenceLinkingData*)disjunctionConProData->getConceptReferenceLinking();
									if (disjConRefLinking) {
										CSaturationConceptDataItem* disjConceptSatItem = (CSaturationConceptDataItem*)disjConRefLinking->getConceptSaturationReferenceLinkingData(conNegation);
										if (disjConceptSatItem) {
											CIndividualSaturationProcessNode* disjunctionIndiNode = (CIndividualSaturationProcessNode*)disjConceptSatItem->getIndividualProcessNodeForConcept();
											addUninitializedIndividualToProcessingQueue(disjunctionIndiNode,mCalcAlgContext);

											CMemoryAllocationManager* taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
											bool requiresDisjunctionProcessing = true;
											CIndividualSaturationProcessNode* copiedIndividualNode = processIndi->getCopyIndividualNode();
											if (copiedIndividualNode) {
												CReapplyConceptSaturationLabelSet* copiedNodeConSet = copiedIndividualNode->getReapplyConceptSaturationLabelSet(false);
												if (copiedNodeConSet->containsConcept(concept,conNegation)) {
													requiresDisjunctionProcessing = false;
													++mDisjunctionInitializedSkippedCount;
												}
											}

											if (requiresDisjunctionProcessing) {
												CXLinker<CIndividualSaturationProcessNode*>* depCopyLinker = CObjectAllocator< CXLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(taskMemMan);
												depCopyLinker->initLinker(processIndi);
												disjunctionIndiNode->addCopyDependingIndividualNodeLinker(depCopyLinker);

												// add already extracted common disjunct concepts
												CReapplyConceptSaturationLabelSet* disjConSet = disjunctionIndiNode->getReapplyConceptSaturationLabelSet(false);
												if (disjConSet) {
													CReapplyConceptSaturationLabelSet* conSet = processIndi->getReapplyConceptSaturationLabelSet(true);
													for (CConceptSaturationDescriptor* disjConDesIt = disjConSet->getConceptSaturationDescriptionLinker(); disjConDesIt; disjConDesIt = disjConDesIt->getNextConceptDesciptor()) {
														addConceptToIndividual(disjConDesIt->getConcept(),disjConDesIt->isNegated(),processIndi,conSet,true,mCalcAlgContext);
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyELSERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
					setInsufficientNodeOccured(mCalcAlgContext);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyATMOSTRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(ATMOSTRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					cint64 cardinality = concept->getParameter() - 1*conNegation;
					if (cardinality < 0) {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,mCalcAlgContext);
					} else {
						if (cardinality == 1) {
							if (!concept->getOperandList()) {
								addFUNCTIONALConceptExtensionProcessingRole(role,processIndi,mCalcAlgContext);
							}
						}
						addCriticalConceptDescriptor(conDes,CCriticalSaturationConceptTypeQueues::CCT_ATMOST,processIndi,mCalcAlgContext);
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyATLEASTRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(ATLEASTRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					cint64 cardinality = concept->getParameter() + 1*conNegation;
					if (cardinality > 0) {
						createSuccessorForConcept(processIndi,conSatProLinker,cardinality,mCalcAlgContext);
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyEQCANDRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					addCriticalConceptDescriptor(conDes,CCriticalSaturationConceptTypeQueues::CCT_EQCANDIDATE,processIndi,mCalcAlgContext);
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyBOTTOMRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker) {
					updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,mCalcAlgContext);
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyALLRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(ALLRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();

					CMemoryAllocationManager* taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CRoleBackwardSaturationPropagationHash* backPropHash = processIndi->getRoleBackwardPropagationHash(true);
					CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDes = CObjectAllocator<CBackwardSaturationPropagationReapplyDescriptor>::allocateAndConstruct(taskMemMan);
					backPropReapplyDes->initBackwardPropagationReapplyDescriptor(conDes);
					CRoleBackwardSaturationPropagationHashData& backPropHashData = backPropHash->addBackwardPropagationConceptDescriptor(role,backPropReapplyDes);
					CBackwardSaturationPropagationLink* backPropLinkIt = backPropHashData.mLinkLinker;
					

					if (backPropLinkIt) {
						while (backPropLinkIt) {
							CIndividualSaturationProcessNode* backPropIndiNode = backPropLinkIt->getSourceIndividual();

							CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
							while (conceptOpLinkerIt) {
								CConcept* opConcept = conceptOpLinkerIt->getData();
								bool opConNegation = conceptOpLinkerIt->isNegated()^conNegation;
								STATINC(ALLROLERESTRICTIONCOUNT,mCalcAlgContext);
								addConceptFilteredToIndividual(opConcept,opConNegation,backPropIndiNode,true,mCalcAlgContext);
								conceptOpLinkerIt = conceptOpLinkerIt->getNext();
							}
							backPropLinkIt = backPropLinkIt->getNext();
						}
					}

					CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
					if (conProData) {
						if (conProData->hasPropagationIntoCreationDirection()) {
							addALLConceptExtensionProcessingRole(role,backPropHashData,processIndi,mCalcAlgContext);
							updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCRITICAL,mCalcAlgContext);
							addCriticalConceptDescriptor(conDes,CCriticalSaturationConceptTypeQueues::CCT_FORALL,processIndi,mCalcAlgContext);
						}
					}

				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addALLConceptExtensionProcessingRole(CRole* role, CRoleBackwardSaturationPropagationHashData& backPropHashData, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mConfALLConceptsExtensionProcessing) {
						if (!backPropHashData.mRoleALLConceptsProcessingQueued) {
							backPropHashData.mRoleALLConceptsProcessingQueued = true;
							CSaturationIndividualNodeSuccessorExtensionData* succExtData = indiProcSatNode->getSuccessorExtensionData(true);
							CSaturationIndividualNodeALLConceptsExtensionData* succIndiALLConExtData = succExtData->getALLConceptsExtensionData(true);
							addSuccessorExtensionToProcessingQueue(indiProcSatNode,calcAlgContext);


							if (succIndiALLConExtData->isSuccessorExtensionInitialized()) {
								if (!succIndiALLConExtData->hasProcessLinkerForRole(role)) {

									//QString indiDebugNameString = getDebugIndividualConceptName(indiProcSatNode,calcAlgContext);
									//if (indiDebugNameString == "http://www.co-ode.org/ontologies/galen#Atlas" || indiDebugNameString == "http://www.co-ode.org/ontologies/galen#CervicalSpine") {
									//	bool debug = true;
									//}

									CRoleSaturationProcessLinker* roleProcessLinker = createRoleSaturationProcessLinker(calcAlgContext);
									roleProcessLinker->initRoleProcessLinker(role);
									succIndiALLConExtData->addRoleProcessLinker(roleProcessLinker);
								}
							}
						}
					}
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addFUNCTIONALConceptExtensionProcessingRole(CRole* role, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mConfFUNCTIONALConceptsExtensionProcessing) {
						CSaturationIndividualNodeSuccessorExtensionData* succExtData = indiProcSatNode->getSuccessorExtensionData(true);
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* succIndiFUNCTIONALConExtData = succExtData->getFUNCTIONALConceptsExtensionData(true);
						addSuccessorExtensionToProcessingQueue(indiProcSatNode,calcAlgContext);

						if (!succIndiFUNCTIONALConExtData->hasFunctionalityAddedProcessLinkerForRole(role)) {
							CRoleSaturationProcessLinker* roleProcessLinker = createRoleSaturationProcessLinker(calcAlgContext);
							roleProcessLinker->initRoleProcessLinker(role);
							succIndiFUNCTIONALConExtData->addFunctionalityAddedRoleProcessLinker(roleProcessLinker);
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addNewLinkedExtensionProcessingRole(CRole* role, CIndividualSaturationProcessNode*& indiProcSatNode, bool queueALLExtension, bool queueFUNCTIONALExtension, CCalculationAlgorithmContextBase* calcAlgContext) {
					
					if (mConfConceptsExtensionProcessing) {
						CSaturationIndividualNodeSuccessorExtensionData* succExtData = indiProcSatNode->getSuccessorExtensionData(false);
						if (succExtData) {
							CSaturationIndividualNodeALLConceptsExtensionData* succIndiALLConExtData = succExtData->getALLConceptsExtensionData(false);
							CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* succIndiFUNCTIONALConExtData = succExtData->getFUNCTIONALConceptsExtensionData(false);
							bool succExtensionInitialized = false;
							bool succALLExtensionInitialized = false;
							bool succFUNCTIONALExtensionInitialized = false;
							if (succIndiALLConExtData) {
								if (succIndiALLConExtData->isSuccessorExtensionInitialized()) {
									succExtensionInitialized = true;
									succALLExtensionInitialized = true;
								}
							}
							if (succIndiFUNCTIONALConExtData) {
								if (succIndiFUNCTIONALConExtData->isSuccessorExtensionInitialized()) {
									succExtensionInitialized = true;
									succFUNCTIONALExtensionInitialized = true;
								}
							}

							if (succExtensionInitialized) {

								CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
								if (linkedSuccHash) {
									CLinkedRoleSaturationSuccessorData*& succData = linkedSuccHash->getLinkedRoleSuccessorData(role,true);
									if (succData) {
										bool queueProcessing = false;
										if (succALLExtensionInitialized && !succData->mRoleALLConceptsProcessingQueued && succData->mRoleALLConceptsQueuingRequired && queueALLExtension) {
											succData->mRoleALLConceptsProcessingQueued = true;
											queueProcessing = true;

											if (succIndiALLConExtData->isSuccessorExtensionInitialized()) {
												if (!succIndiALLConExtData->hasProcessLinkerForRole(role)) {
													CRoleSaturationProcessLinker* roleProcessLinker = createRoleSaturationProcessLinker(calcAlgContext);
													roleProcessLinker->initRoleProcessLinker(role);
													succIndiALLConExtData->addRoleProcessLinker(roleProcessLinker);
												}
											}
										}

										if (succFUNCTIONALExtensionInitialized && !succData->mRoleFUNCTIONALConceptsProcessingQueued && succData->mRoleFUNCTIONALConceptsQueuingRequired /*&& succData->mSuccCount > 1 */&& queueFUNCTIONALExtension) {
											succData->mRoleFUNCTIONALConceptsProcessingQueued = true;
											queueProcessing = true;

											if (succIndiFUNCTIONALConExtData->isSuccessorExtensionInitialized()) {
												if (!succIndiFUNCTIONALConExtData->hasLinkedSuccessorAddedProcessLinkerForRole(role)) {
													CRoleSaturationProcessLinker* roleProcessLinker = createRoleSaturationProcessLinker(calcAlgContext);
													roleProcessLinker->initRoleProcessLinker(role);
													succIndiFUNCTIONALConExtData->addLinkedSuccessorAddedRoleProcessLinker(roleProcessLinker);
												}
											}
										}


										if (queueProcessing) {
											addSuccessorExtensionToProcessingQueue(indiProcSatNode,calcAlgContext);
										}
									}
								}
							}
						}
					}
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::loadConsistenceModelData(CCalculationAlgorithmContext* calcAlgContext) {
					bool consCompGraphAvailable = false;
					if (!mCachedCompletionGraphLoaded) {
						CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
						CConcreteOntology* ontology = processingDataBox->getOntology();
						CConsistence* consistence = ontology->getConsistence();
						if (consistence) {
							CConsistenceData* consData = consistence->getConsistenceModelData();
							if (consData) {
								CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
								if (consTaskData) {
									CSatisfiableCalculationTask* detSatCalcTask = consTaskData->getDeterministicSatisfiableTask();
									CSatisfiableCalculationTask* compGraphCachedCalcTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
									if (detSatCalcTask) {
										mDetCachedCGIndiVector = detSatCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
										mDetConsistencyCG = true;
									} else {
										mDetConsistencyCG = false;
									}
									if (compGraphCachedCalcTask && compGraphCachedCalcTask != detSatCalcTask) {
										mNonDetCachedCGIndiVector = compGraphCachedCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
										mNonDetConsistencyCG = true;
									} else {
										mNonDetConsistencyCG = false;
									}
								}
							}
						}
						mCachedCompletionGraphLoaded = true;
					}
					if (mDetConsistencyCG) {
						consCompGraphAvailable = true;
					} else {
						mCachedCompletionGraphMissing = true;
					}
					return consCompGraphAvailable;
				}






				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addNominalDependentIndividualNode(cint64 nominalID, CIndividualSaturationProcessNode* dependentIndiNode, CSaturationNominalDependentNodeData::NOMINALCONNECTIONTYPE connectionType, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CSaturationNominalDependentNodeHash* nominalDependentNodeHash = processingDataBox->getSaturationNominalDependentNodeHash(true);
					nominalDependentNodeHash->addNominalDependentNode(nominalID,dependentIndiNode,connectionType);
					CSaturationInfluencedNominalSet* influencedNominalSet = processingDataBox->getSaturationInfluencedNominalSet(true);
					if (influencedNominalSet->isNominalInfluenced(nominalID)) {
						// insufficient
						updateDirectAddingIndividualStatusFlags(dependentIndiNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
						setInsufficientNodeOccured(calcAlgContext);
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addInfluencedNominal(cint64 influencedNominalID, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CSaturationInfluencedNominalSet* influencedNominalSet = processingDataBox->getSaturationInfluencedNominalSet(true);
					bool firstInfluence = influencedNominalSet->setNominalInfluenced(influencedNominalID);
					if (firstInfluence) {
						CSaturationNominalDependentNodeHash* nominalDependentNodeHash = processingDataBox->getSaturationNominalDependentNodeHash(true);
						for (CSaturationNominalDependentNodeData* nominalDepNodeDataIt = nominalDependentNodeHash->getNominalDependentNodeData(influencedNominalID); nominalDepNodeDataIt; nominalDepNodeDataIt = nominalDepNodeDataIt->getNext()) {
							CIndividualSaturationProcessNode* dependentIndSatProcNode = nominalDepNodeDataIt->getDependentIndividualSaturationNode();
							// insufficient
							updateDirectAddingIndividualStatusFlags(dependentIndSatProcNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
							setInsufficientNodeOccured(calcAlgContext);
						}
					}
				}



				CIndividualProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getCorrectedNode(cint64 individualID, CIndividualProcessNodeVector* indiVec, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessNode* indiNode = indiVec->getData(individualID);
					if (indiNode->getMergedIntoIndividualNodeID() != indiNode->getIndividualID()) {
						indiNode = indiVec->getData(indiNode->getMergedIntoIndividualNodeID());
					}
					return indiNode;
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyVALUERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(VALUERULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CIndividual* nominalIndividual = concept->getNominalIndividual();
					cint64 nominalID = nominalIndividual->getIndividualID();

					if (mConfNominalProcessing) {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGNOMINALCONNECTION,mCalcAlgContext);
						updateAddingSuccessorConnectedNominal(processIndi,nominalID,mCalcAlgContext);

						if (loadConsistenceModelData(mCalcAlgContext)) {

							bool nominalInfluenced = false;
							bool insufficientNominalConnection = false;

							CIndividualProcessNode* nominalProcessNode = getCorrectedNode(nominalID,mDetCachedCGIndiVector,mCalcAlgContext);
							CReapplyRoleSuccessorHash* nominalReapplyRoleSuccHash = nominalProcessNode->getReapplyRoleSuccessorHash(false);
							for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
								CRole* superRole = superRoleIt->getData();
								bool inversedSuperRole = superRoleIt->isNegated();

								if (superRole->hasDisjointRoles()) {
									insufficientNominalConnection = true;
								}

								CIndividualSaturationProcessNodeStatusFlags* directStatFlags = processIndi->getDirectStatusFlags();

								for (CSortedNegLinker<CConcept*>* domainConLinkerIt = superRole->getDomainRangeConceptList(inversedSuperRole); domainConLinkerIt; domainConLinkerIt = domainConLinkerIt->getNext()) {
									CConcept* domainConcept = domainConLinkerIt->getData();
									bool domainConceptNegation = domainConLinkerIt->isNegated();
									addConceptFilteredToIndividual(domainConcept,domainConceptNegation,processIndi,mCalcAlgContext);
								}

								for (CSortedNegLinker<CConcept*>* rangeConLinkerIt = superRole->getDomainRangeConceptList(!inversedSuperRole); rangeConLinkerIt && !directStatFlags->hasInsufficientFlag(); rangeConLinkerIt = rangeConLinkerIt->getNext()) {
									CConcept* rangeConcept = rangeConLinkerIt->getData();
									bool rangeConceptNegation = rangeConLinkerIt->isNegated();
									CReapplyConceptLabelSet* nominalConSet = nominalProcessNode->getReapplyConceptLabelSet(false);
									if (!nominalConSet->containsConcept(rangeConcept,rangeConceptNegation)) {
										nominalInfluenced = true;
									}
								}
					
								if (inversedSuperRole) {
									if (nominalReapplyRoleSuccHash) {

										CReapplyQueueIterator reapplyRoleIt(nominalReapplyRoleSuccHash->getRoleReapplyIterator(superRole));
										while (reapplyRoleIt.hasNext()) {
											CReapplyConceptDescriptor* reapplyDes = reapplyRoleIt.next();
											CConceptDescriptor* reapplyConDes = reapplyDes->getConceptDescriptor();
											CConcept* reapplyConcept = reapplyConDes->getConcept();
											bool reapplyConceptNegation = reapplyConDes->isNegated();
											cint64 reapplyConceptCode = reapplyConcept->getOperatorCode();
											CConceptOperator* reapplyConceptOperator = reapplyConcept->getConceptOperator();
											if (!reapplyConceptNegation && reapplyConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_ALL_TYPE)) {
												// cannot be handled with this algorithm, hence mark as insufficient
												insufficientNominalConnection = true;

											} else if (!reapplyConceptNegation && reapplyConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE) || reapplyConceptNegation && reapplyConceptCode == CCSOME) {
												for (CSortedNegLinker<CConcept*>* reapplyConceptOpLinkerIt = reapplyConcept->getOperandList(); reapplyConceptOpLinkerIt; reapplyConceptOpLinkerIt = reapplyConceptOpLinkerIt->getNext()) {
													CConcept* reapplyOperandConcept = reapplyConceptOpLinkerIt->getData();
													bool reapplyOperandConceptNegation = reapplyConceptOpLinkerIt->isNegated()^reapplyConceptNegation;
													addConceptFilteredToIndividual(reapplyOperandConcept,reapplyOperandConceptNegation,processIndi,mCalcAlgContext);
												}
											} else if (!reapplyConceptNegation && reapplyConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE)) {
												addAutomateTransitionOperands(processIndi,reapplyConcept,role,mCalcAlgContext);
											} else {
												insufficientNominalConnection = true;
											}
										}
									}
								}
							}


							if (nominalInfluenced) {
								insufficientNominalConnection = true;
								addInfluencedNominal(nominalID,mCalcAlgContext);
							}

							if (insufficientNominalConnection) {
								updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
								setInsufficientNodeOccured(mCalcAlgContext);
							}



							if (mNonDetConsistencyCG) {
								addCriticalConceptDescriptor(conDes,CCriticalSaturationConceptTypeQueues::CCT_VALUE,processIndi,mCalcAlgContext);
							}
							addNominalDependentIndividualNode(nominalID,processIndi,CSaturationNominalDependentNodeData::VALUECONNECTION,mCalcAlgContext);

						} else {
							delayNominalSaturationConceptProcessing(processIndi,conSatProLinker,nominalID,mCalcAlgContext);
						}
					} else {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
						setInsufficientNodeOccured(mCalcAlgContext);
					}

				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::delayNominalSaturationConceptProcessing(CIndividualSaturationProcessNode* processIndi, CConceptSaturationProcessLinker* conProLinker, cint64 nominalID, CCalculationAlgorithmContextBase* calcAlgContext) {
					propagateUnloadedABoxCompletionGraphDependentIndividualNodeFlag(processIndi,conProLinker,nominalID,calcAlgContext);
					CConceptSaturationProcessLinker* nomDelayedConSatProcLinker = createConceptSaturationProcessLinker(calcAlgContext);
					nomDelayedConSatProcLinker->initConceptSaturationProcessLinker(conProLinker->getConceptSaturationDescriptor());
					processIndi->getNominalHandlingData(true)->addDelayedNominalConceptSaturationProcessLinker(nomDelayedConSatProcLinker);
					setDelayedNominalProcessingOccured(calcAlgContext);
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addAutomateTransitionOperands(CIndividualSaturationProcessNode*& processIndi, CConcept* automatConcept, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptOperator* automatConceptOperator = automatConcept->getConceptOperator();
					if (automatConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE)) {
						for (CSortedNegLinker<CConcept*>* automateConceptOpLinkerIt = automatConcept->getOperandList(); automateConceptOpLinkerIt; automateConceptOpLinkerIt = automateConceptOpLinkerIt->getNext()) {
							CConcept* automateOperandConcept = automateConceptOpLinkerIt->getData();
							addAutomateTransitionOperands(processIndi,automateOperandConcept,role,calcAlgContext);
						}
					} else if (automatConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQALL_TYPE)) {
						CRole* automateRole = automatConcept->getRole();
						if (automateRole == role) {
							for (CSortedNegLinker<CConcept*>* automateConceptOpLinkerIt = automatConcept->getOperandList(); automateConceptOpLinkerIt; automateConceptOpLinkerIt = automateConceptOpLinkerIt->getNext()) {
								CConcept* automateOperandConcept = automateConceptOpLinkerIt->getData();
								bool automateOperandConceptNegation = automateConceptOpLinkerIt->isNegated();
								addConceptFilteredToIndividual(automateOperandConcept,automateOperandConceptNegation,processIndi,calcAlgContext);
							}
						}
					}
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::testAutomateTransitionOperandsAddable(CIndividualSaturationProcessNode*& processIndi, CConcept* automatConcept, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptOperator* automatConceptOperator = automatConcept->getConceptOperator();
					if (automatConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE)) {
						for (CSortedNegLinker<CConcept*>* automateConceptOpLinkerIt = automatConcept->getOperandList(); automateConceptOpLinkerIt; automateConceptOpLinkerIt = automateConceptOpLinkerIt->getNext()) {
							CConcept* automateOperandConcept = automateConceptOpLinkerIt->getData();
							if (testAutomateTransitionOperandsAddable(processIndi,automateOperandConcept,role,calcAlgContext)) {
								return true;
							}
						}
					} else if (automatConceptOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQALL_TYPE)) {
						CRole* automateRole = automatConcept->getRole();
						if (automateRole == role) {
							CReapplyConceptSaturationLabelSet* conSet = processIndi->getReapplyConceptSaturationLabelSet(false);
							for (CSortedNegLinker<CConcept*>* automateConceptOpLinkerIt = automatConcept->getOperandList(); automateConceptOpLinkerIt; automateConceptOpLinkerIt = automateConceptOpLinkerIt->getNext()) {
								CConcept* automateOperandConcept = automateConceptOpLinkerIt->getData();
								bool automateOperandConceptNegation = automateConceptOpLinkerIt->isNegated();
								if (!conSet->containsConcept(automateOperandConcept,automateOperandConceptNegation)) {
									return true;
								}
							}
						}
					}
					return false;
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyNOMINALRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(NOMINALRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CIndividual* nominalIndividual = concept->getNominalIndividual();
					cint64 nominalID = nominalIndividual->getIndividualID();


					//mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(mCalcAlgContext);
					//QFile tmpFile("saturation-model.txt");
					//if (tmpFile.open(QIODevice::WriteOnly)) {
					//	tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
					//	tmpFile.close();
					//}

					if (mConfNominalProcessing) {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGNOMINALCONNECTION,mCalcAlgContext);
						updateAddingSuccessorConnectedNominal(processIndi,nominalID,mCalcAlgContext);


						for (CXLinker<CIndividualSaturationProcessNode*>* multipleCardinalityAncestorNodesLinkerIt = processIndi->getMultipleCardinalityAncestorNodesLinker(); multipleCardinalityAncestorNodesLinkerIt; multipleCardinalityAncestorNodesLinkerIt = multipleCardinalityAncestorNodesLinkerIt->getNext()) {
							CIndividualSaturationProcessNode* multipleCardinalityAncestorNode = multipleCardinalityAncestorNodesLinkerIt->getData();
							updateDirectAddingIndividualStatusFlags(multipleCardinalityAncestorNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,mCalcAlgContext);
						}


						if (loadConsistenceModelData(mCalcAlgContext)) {
							CIndividualProcessNode* nominalProcessNode = getCorrectedNode(nominalID,mDetCachedCGIndiVector,mCalcAlgContext);
							CReapplyConceptLabelSet* nominalConSet = nominalProcessNode->getReapplyConceptLabelSet(false);
							if (nominalConSet) {

								bool nominalInfluenced = false;
								CReapplyConceptSaturationLabelSet* satConSet = processIndi->getReapplyConceptSaturationLabelSet(false);
								for (CConceptSaturationDescriptor* satConDesIt = satConSet->getConceptSaturationDescriptionLinker(); satConDesIt && !nominalInfluenced; satConDesIt = satConDesIt->getNext()) {
									CConcept* satConcept = satConDesIt->getConcept();
									bool satConceptNegation = satConDesIt->isNegated();
									if (!nominalConSet->containsConcept(satConcept,satConceptNegation)) {
										nominalInfluenced = true;
									}
								}

								if (nominalInfluenced) {
									addInfluencedNominal(nominalID,mCalcAlgContext);
								}

								for (CConceptDescriptor* nominalConDesIt = nominalConSet->getAddingSortedConceptDescriptionLinker(); nominalConDesIt; nominalConDesIt = nominalConDesIt->getNextConceptDesciptor()) {
									CConcept* nominalConcept = nominalConDesIt->getConcept();
									bool nominalConceptNegation = nominalConDesIt->isNegated();
									addConceptFilteredToIndividual(nominalConcept,nominalConceptNegation,processIndi,mCalcAlgContext);
								}
							}

							if (mNonDetConsistencyCG) {
								addCriticalConceptDescriptor(conDes,CCriticalSaturationConceptTypeQueues::CCT_NOMINAL,processIndi,mCalcAlgContext);
							}
							addNominalDependentIndividualNode(nominalID,processIndi,CSaturationNominalDependentNodeData::NOMINALCONNECTION,mCalcAlgContext);

						} else {
							delayNominalSaturationConceptProcessing(processIndi,conSatProLinker,nominalID,mCalcAlgContext);
						}
					} else {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
						setInsufficientNodeOccured(mCalcAlgContext);
					}	

				}





				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::propagateUnloadedABoxCompletionGraphDependentIndividualNodeFlag(CIndividualSaturationProcessNode* processIndi, CConceptSaturationProcessLinker* conProLinker, cint64 nominalID, CCalculationAlgorithmContextBase* calcAlgContext) {
					updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDMISSEDABOXCONSISTENCYDATA,calcAlgContext);
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applySELFRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(SELFRULEAPPLICATIONCOUNT,mCalcAlgContext);
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();


					CMemoryAllocationManager* taskMemMan = nullptr;
					CRoleBackwardSaturationPropagationHash* backPropHash = nullptr;
					CReapplyConceptSaturationLabelSet* conSet = nullptr;
					CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
					while (superRoleIt) {
						CRole* superRole = superRoleIt->getData();
						CSortedNegLinker<CConcept*>* domainConLinker = superRole->getDomainRangeConceptList(superRoleIt->isNegated());
						for (CSortedNegLinker<CConcept*>* domainConLinkerIt = domainConLinker; domainConLinkerIt; domainConLinkerIt = domainConLinkerIt->getNext()) {
							CConcept* domainConcept = domainConLinkerIt->getData();
							bool domainConceptNegation = domainConLinkerIt->isNegated();
							if (!conSet) {
								conSet = processIndi->getReapplyConceptSaturationLabelSet(true);
							}
							addConceptFilteredToIndividual(domainConcept,domainConceptNegation,processIndi,conSet,false,mCalcAlgContext);
						}
						CSortedNegLinker<CConcept*>* rangeConLinker = superRole->getDomainRangeConceptList(!superRoleIt->isNegated());
						for (CSortedNegLinker<CConcept*>* rangeConLinkerIt = rangeConLinker; rangeConLinkerIt; rangeConLinkerIt = rangeConLinkerIt->getNext()) {
							CConcept* rangeConcept = rangeConLinkerIt->getData();
							bool rangeConceptNegation = rangeConLinkerIt->isNegated();
							if (!conSet) {
								conSet = processIndi->getReapplyConceptSaturationLabelSet(true);
							}
							addConceptFilteredToIndividual(rangeConcept,rangeConceptNegation,processIndi,conSet,false,mCalcAlgContext);
						}

						if (!taskMemMan) {
							taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						}
						CBackwardSaturationPropagationLink* backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(taskMemMan);
						backPropLink->initBackwardPropagationLink(processIndi,superRole);

						if (!backPropHash) {
							backPropHash = processIndi->getRoleBackwardPropagationHash(true);
						}
						CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDes = backPropHash->addSelfConnectedBackwardPropagationLink(superRole,backPropLink);
						if (backPropReapplyDes) {
							applyBackwardPropagationConcepts(backPropLink,backPropReapplyDes,mCalcAlgContext);
						}

						superRoleIt = superRoleIt->getNext();
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::setDelayedNominalProcessingOccured(CCalculationAlgorithmContextBase* calcAlgContext) {
					calcAlgContext->getUsedProcessingDataBox()->setDelayedNominalProcessingOccured(true);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::setInsufficientNodeOccured(CCalculationAlgorithmContextBase* calcAlgContext) {
					calcAlgContext->getUsedProcessingDataBox()->setInsufficientNodeOccured(true);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::setProblematicEQCandidateOccured(CCalculationAlgorithmContextBase* calcAlgContext) {
					calcAlgContext->getUsedProcessingDataBox()->setProblematicEQCandidateOccured(true);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applySOMERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker) {
					STATINC(SOMERULEAPPLICATIONCOUNT,mCalcAlgContext);
					createSuccessorForConcept(processIndi,conSatProLinker,1,mCalcAlgContext);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createSuccessorForConcept(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conSatProLinker, cint64 cardinality, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConceptSaturationDescriptor* conDes = conSatProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();

					CIndividualSaturationProcessNode* existIndiNode = nullptr;

					bool specializedExistentialRestriction = false;
					bool operandExistentialRestriction = false;
					CConceptData* conceptData = concept->getConceptData();
					if (!existIndiNode && conceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* extSatCalcRefLinkData = confSatRefLinkingData->getExistentialSuccessorConceptSaturationReferenceLinkingData();
							if (extSatCalcRefLinkData) {
								existIndiNode = (CIndividualSaturationProcessNode*)extSatCalcRefLinkData->getIndividualProcessNodeForConcept();
							}
						}
					}



					if (!existIndiNode && !specializedExistentialRestriction) {
						while (!existIndiNode && conceptOpLinkerIt) {
							CConcept* opConcept = conceptOpLinkerIt->getData();
							bool opConNegation = conceptOpLinkerIt->isNegated()^conNegation;
							CConceptData* opConceptData = opConcept->getConceptData();
							KONCLUDE_ASSERT_X(opConceptData,"SOME saturation rule","concept data missing");
							if (opConceptData) {
								CConceptProcessData* conProcData = (CConceptProcessData*)opConceptData;
								CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
								KONCLUDE_ASSERT_X(conRefLinking,"SOME saturation rule","concept reference linking data missing");
								if (conRefLinking) {
									CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
									CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(opConNegation);
									if (satCalcRefLinkData) {
										existIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
									}
								}
							}
							conceptOpLinkerIt = conceptOpLinkerIt->getNext();
						}
					}

					if (!existIndiNode && !specializedExistentialRestriction && !operandExistentialRestriction) {
						CConcept* opConcept = mCalcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
						bool opConNegation = false;
						CConceptData* opConceptData = opConcept->getConceptData();
						CConceptProcessData* conProcData = (CConceptProcessData*)opConceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						KONCLUDE_ASSERT_X(conRefLinking,"SOME saturation rule","concept reference linking data missing");
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(opConNegation);
							if (satCalcRefLinkData) {
								existIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
							}					
						}
					}


					KONCLUDE_ASSERT_X(existIndiNode,"SOME saturation rule","concept reference linked individual node missing");
					if (existIndiNode) {
						addUninitializedIndividualToProcessingQueue(existIndiNode,mCalcAlgContext);

						updateIndirectAddingIndividualStatusFlags(processIndi,existIndiNode->getIndirectStatusFlags(),mCalcAlgContext);
						updateAddingSuccessorConnectedNominal(processIndi,existIndiNode->getSuccessorConnectedNominalSet(false),mCalcAlgContext);


						CMemoryAllocationManager* taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						if (cardinality > 1) {
							if (existIndiNode->hasNominalIntegrated()) {
								updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,calcAlgContext);
							} else {
								CXLinker<CIndividualSaturationProcessNode*>* multipleCardinalityAncestorNodesLinker = CObjectAllocator< CXLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(taskMemMan);
								multipleCardinalityAncestorNodesLinker->initLinker(processIndi);
								existIndiNode->addMultipleCardinalityAncestorNodesLinker(multipleCardinalityAncestorNodesLinker);
							}
						}

						bool existIndiInitialized = existIndiNode->isInitialized();
						CRoleBackwardSaturationPropagationHash* backPropHash = nullptr;
						CReapplyConceptSaturationLabelSet* conSet = nullptr;
						CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
						bool connected = false;
						while (superRoleIt) {
							CRole* superRole = superRoleIt->getData();
							if (superRole->getDisjointRoleList() != nullptr) {
								updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
								setInsufficientNodeOccured(mCalcAlgContext);
							}
							CSortedNegLinker<CConcept*>* domainConLinker = superRole->getDomainRangeConceptList(superRoleIt->isNegated());
							for (CSortedNegLinker<CConcept*>* domainConLinkerIt = domainConLinker; domainConLinkerIt; domainConLinkerIt = domainConLinkerIt->getNext()) {
								CConcept* domainConcept = domainConLinkerIt->getData();
								bool domainConceptNegation = domainConLinkerIt->isNegated();
								if (!conSet) {
									conSet = processIndi->getReapplyConceptSaturationLabelSet(true);
								}
								addConceptFilteredToIndividual(domainConcept,domainConceptNegation,processIndi,conSet,false,mCalcAlgContext);
							}


							if (superRoleIt->isNegated()) {
								connected = true;
								CBackwardSaturationPropagationLink* backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(taskMemMan);
								backPropLink->initBackwardPropagationLink(processIndi,superRole);
								if (!existIndiInitialized) {
									existIndiNode->addInitializingBackwardPropagationLinks(backPropLink);
								} else {
									if (!backPropHash) {
										backPropHash = existIndiNode->getRoleBackwardPropagationHash(true);
									}
									CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDes = backPropHash->addBackwardPropagationLink(superRole,backPropLink);
									if (backPropReapplyDes) {
										applyBackwardPropagationConcepts(backPropLink,backPropReapplyDes,mCalcAlgContext);
									}
								}
							} else {
								addNewLinkedExtensionProcessingRole(superRole,processIndi,true,true,mCalcAlgContext);
							}
							superRoleIt = superRoleIt->getNext();
						}
						if (!connected) {
							CXLinker<CIndividualSaturationProcessNode*>* nonInvConnectedIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(taskMemMan);
							nonInvConnectedIndiNodeLinker->initLinker(processIndi);
							existIndiNode->addNonInverseConnectedIndividualNodeLinker(nonInvConnectedIndiNodeLinker);
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyBackwardPropagationConcepts(CBackwardSaturationPropagationLink* backPropLink, CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDesIt, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* sourceIndiNode = backPropLink->getSourceIndividual();
					while (backPropReapplyDesIt) {
						CConceptSaturationDescriptor* reapplyConSatDes = backPropReapplyDesIt->getReapplyConceptSaturationDescriptor();
						CConcept* reapplyConcept = reapplyConSatDes->getConcept();
						bool reapplyConNegation = reapplyConSatDes->getNegation();

						CSortedNegLinker<CConcept*>* reapplyOpLinker = reapplyConcept->getOperandList();
						while (reapplyOpLinker) {
							CConcept* opConcept = reapplyOpLinker->getData();
							bool opNegation = reapplyOpLinker->isNegated()^reapplyConNegation;
							STATINC(ALLROLERESTRICTIONCOUNT,calcAlgContext);
							addConceptFilteredToIndividual(opConcept,opNegation,sourceIndiNode,true,calcAlgContext);
							reapplyOpLinker = reapplyOpLinker->getNext();
						}
						backPropReapplyDesIt = backPropReapplyDesIt->getNext();
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addIndividualToProcessingQueue(CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNodeLinker* processNodeLinker = processIndi->getIndividualSaturationProcessNodeLinker();
					if (!processNodeLinker->isProcessingQueued()) {
						processNodeLinker->setProcessingQueued();
						calcAlgContext->getUsedProcessingDataBox()->addIndividualSaturationProcessNodeLinker(processNodeLinker);
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addUninitializedIndividualToProcessingQueue(CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!processIndi->isInitialized()) {
						addIndividualToProcessingQueue(processIndi,calcAlgContext);
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addIndividualToCompletionQueue(CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (!processIndi->isIndividualSaturationCompletionNodeLinkerQueued()) {
						CIndividualSaturationProcessNodeLinker* processNodeLinker = processIndi->getIndividualSaturationCompletionNodeLinker(true);
						processNodeLinker->setProcessingQueued();
						calcAlgContext->getUsedProcessingDataBox()->addIndividualSaturationCompletionNodeLinker(processNodeLinker);
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addConceptsFilteredToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualSaturationProcessNode*& processIndi, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSortedNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptFilteredToIndividual(opConcept,opConNegation,processIndi,updateCopyDependedIndividual,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addConceptsFilteredToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSortedNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptFilteredToIndividual(opConcept,opConNegation,processIndi,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addConceptsFilteredToIndividual(CXNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CXNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptFilteredToIndividual(opConcept,opConNegation,processIndi,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addConceptsFilteredToIndividual(CXSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CXSortedNegLinker<CConcept*>* conceptAddLinkerIt = conceptAddLinker;
					while (conceptAddLinkerIt) {
						CConcept* opConcept = conceptAddLinkerIt->getData();
						bool opConNegation = conceptAddLinkerIt->isNegated()^negate;
						addConceptFilteredToIndividual(opConcept,opConNegation,processIndi,calcAlgContext);
						conceptAddLinkerIt = conceptAddLinkerIt->getNext();
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* labelSet = processIndi->getReapplyConceptSaturationLabelSet();
					addConceptFilteredToIndividual(addingConcept,negate,processIndi,labelSet,true,calcAlgContext);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualSaturationProcessNode*& processIndi, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* labelSet = processIndi->getReapplyConceptSaturationLabelSet();
					addConceptFilteredToIndividual(addingConcept,negate,processIndi,labelSet,updateCopyDependedIndividual,calcAlgContext);
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 opCode = addingConcept->getOperatorCode();
					bool insertConcept = mConfForceAllConceptInsertion;
					if (!insertConcept) {
						CSortedNegLinker<CConcept*>* opConLinkerIt = addingConcept->getOperandList();
						if (!negate && (opCode == CCAND || opCode == CCAQAND || opCode == CCIMPLAQAND || opCode == CCBRANCHAQAND) || negate && opCode == CCOR) {
							addConceptsFilteredToIndividual(opConLinkerIt,negate,rootProcessIndi,updateCopyDependedIndividual,calcAlgContext);
						} else if (opCode == CCAQCHOOCE) {
							while (opConLinkerIt) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegation = opConLinkerIt->isNegated();
								if (opNegation == negate) {
									addConceptFilteredToIndividual(opConcept,false,rootProcessIndi,labelSet,updateCopyDependedIndividual,calcAlgContext);
								}
								opConLinkerIt = opConLinkerIt->getNext();
							}
						} else if (mConfImplicationAddingSkipping && opCode == CCIMPL) {
							insertConcept = false;
						} else {
							insertConcept = true;
						}
					}
					if (insertConcept) {
						addConceptToIndividual(addingConcept,negate,rootProcessIndi,labelSet,updateCopyDependedIndividual,calcAlgContext);
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addConceptToIndividual(CConcept* addingConcept, bool negate, CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(CONCEPTSADDEDINDINODELABELSETCOUNT,calcAlgContext);

					KONCLUDE_ASSERT_X(addingConcept,"adding concept to individual","concept missing");


					CConceptSaturationDescriptor* conceptSaturationDescriptor = createConceptSaturationDescriptor(calcAlgContext);
					conceptSaturationDescriptor->initConceptSaturationDescriptor(addingConcept,negate);

					bool contained = insertConceptToIndividualConceptSet(conceptSaturationDescriptor,rootProcessIndi,labelSet,calcAlgContext);
					if (!contained) {
						STATINC(CONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,calcAlgContext);

						CConceptSaturationProcessLinker* conceptSaturationProcessLinker = createConceptSaturationProcessLinker(calcAlgContext);
						conceptSaturationProcessLinker->initConceptSaturationProcessLinker(conceptSaturationDescriptor);
						rootProcessIndi->addConceptSaturationProcessLinker(conceptSaturationProcessLinker);
						addIndividualToProcessingQueue(rootProcessIndi,calcAlgContext);

						if (updateCopyDependedIndividual && rootProcessIndi->hasCopyDependingIndividualNodeLinker()) {
							// add concept to copy dependent individual nodes
							CXLinker<CIndividualSaturationProcessNode*>* copyDepIndiLinkerIt = rootProcessIndi->getCopyDependingIndividualNodeLinker();
							while (copyDepIndiLinkerIt) {
								CIndividualSaturationProcessNode* copyDepIndiNode = copyDepIndiLinkerIt->getData();
								addConceptToIndividual(addingConcept,negate,copyDepIndiNode,copyDepIndiNode->getReapplyConceptSaturationLabelSet(true),true,calcAlgContext);
								copyDepIndiLinkerIt = copyDepIndiLinkerIt->getNext();
							}
						}

					} else {
						// TODO: may release the concept saturation descriptor 
						//releaseConceptSaturationDescriptor(conceptSaturationDescriptor,calcAlgContext);
					}
				}


				CConceptSaturationDescriptor* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createConceptSaturationDescriptor(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CConceptSaturationDescriptor* conSatDes = processingDataBox->takeRemainingConceptSaturationDescriptor();
					if (!conSatDes) {
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						conSatDes = CObjectAllocator<CConceptSaturationDescriptor>::allocateAndConstruct(taskMemMan);
					}
					return conSatDes;
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::releaseConceptSaturationDescriptor(CConceptSaturationDescriptor* conSatDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					conSatDes->clearNext();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					processingDataBox->addRemainingConceptSaturationDescriptor(conSatDes);
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::releaseConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conSatProcLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					conSatProcLinker->clearNext();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					processingDataBox->addRemainingConceptSaturationProcessLinker(conSatProcLinker);
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::releaseRoleSaturationProcessLinker(CRoleSaturationProcessLinker* roleSatProcLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					roleSatProcLinker->clearNext();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					processingDataBox->addRemainingRoleSaturationProcessLinker(roleSatProcLinker);
				}

				CRoleSaturationProcessLinker* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createRoleSaturationProcessLinker(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CRoleSaturationProcessLinker* roleSatProcLinker = processingDataBox->takeRemainingRoleSaturationProcessLinker();
					if (!roleSatProcLinker) {
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						roleSatProcLinker = CObjectAllocator<CRoleSaturationProcessLinker>::allocateAndConstruct(taskMemMan);
					}
					return roleSatProcLinker;
				}

				CConceptSaturationProcessLinker* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createConceptSaturationProcessLinker(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CConceptSaturationProcessLinker* conSatProcLinker = processingDataBox->takeRemainingConceptSaturationProcessLinker();
					if (!conSatProcLinker) {
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						conSatProcLinker = CObjectAllocator<CConceptSaturationProcessLinker>::allocateAndConstruct(taskMemMan);
					}
					return conSatProcLinker;
				}


				CIndividualSaturationProcessNodeStatusUpdateLinker* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createIndividualSaturationUpdateLinker(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualSaturationProcessNodeStatusUpdateLinker* conSatUpdateLinker = processingDataBox->takeRemainingIndividualSaturationUpdateLinker();
					if (!conSatUpdateLinker) {
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						conSatUpdateLinker = CObjectAllocator<CIndividualSaturationProcessNodeStatusUpdateLinker>::allocateAndConstruct(taskMemMan);
					}
					return conSatUpdateLinker;
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::releaseIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* conSatUpdateLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					conSatUpdateLinker->clearNext();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					processingDataBox->addRemainingIndividualSaturationUpdateLinker(conSatUpdateLinker);
				}


				CSaturationModifiedProcessUpdateLinker* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createModifiedProcessUpdateLinker(CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					return CObjectAllocator<CSaturationModifiedProcessUpdateLinker>::allocateAndConstruct(taskMemMan);
				}


				CImplicationReapplyConceptSaturationDescriptor* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createImplicationReapplyConceptSaturationDescriptor(CCalculationAlgorithmContextBase* calcAlgContext) {
					CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					CImplicationReapplyConceptSaturationDescriptor* impReaConSatDes = CObjectAllocator<CImplicationReapplyConceptSaturationDescriptor>::allocateAndConstruct(taskMemMan);
					return impReaConSatDes;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::insertConceptToIndividualConceptSet(CConceptSaturationDescriptor* conceptSaturationDescriptor, CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool contained = false;
					bool insertConcept = true;
					bool implTriggerGeneration = false;
					bool requiredBackProp = rootProcessIndi->getRequiredBackwardPropagation();
					CConcept* concept = conceptSaturationDescriptor->getConcept();
					bool conNeg = conceptSaturationDescriptor->getNegation();
					cint64 opCode = concept->getOperatorCode();
					if (!mConfForceAllConceptInsertion) {
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
					}
					if (insertConcept) {
						bool newInsertion = false;
						CImplicationReapplyConceptSaturationDescriptor* reapplyImpReapplyConSatDes = nullptr;
						CImplicationReapplyConceptSaturationDescriptor** reapplyImpReapplyConSatDesPtr = nullptr;
						bool clashed = labelSet->insertConceptReturnClashed(conceptSaturationDescriptor,&newInsertion,&reapplyImpReapplyConSatDesPtr);

						if (!clashed) {
							if (reapplyImpReapplyConSatDesPtr) {
								reapplyImpReapplyConSatDes = *reapplyImpReapplyConSatDesPtr;
							}

							if (newInsertion) {
								CImplicationReapplyConceptSaturationDescriptor* reapplyImpReapplyConSatDesIt = reapplyImpReapplyConSatDes;
								while (reapplyImpReapplyConSatDesIt) {
									CSortedNegLinker<CConcept*>* triggerConLinker = reapplyImpReapplyConSatDesIt->getNextTriggerConcept();
									if (triggerConLinker->getData() == conceptSaturationDescriptor->getConcept() && triggerConLinker->isNegated() != conceptSaturationDescriptor->isNegated()) {
										updateImplicationReapplyConceptSaturationDescriptor(reapplyImpReapplyConSatDesIt,rootProcessIndi,labelSet,calcAlgContext);
									}
									reapplyImpReapplyConSatDesIt = reapplyImpReapplyConSatDesIt->getNext();
								}
								if (implTriggerGeneration) {
									// add first trigger
									CImplicationReapplyConceptSaturationDescriptor tmpNewReapplyImpReapplyConSatDes;
									CConcept* implConcept = conceptSaturationDescriptor->getConcept();
									CSortedNegLinker<CConcept*>* nextTriggerConcept = implConcept->getOperandList();
									tmpNewReapplyImpReapplyConSatDes.initImplicationReapllyConceptSaturationDescriptor(implConcept,nextTriggerConcept);
									updateImplicationReapplyConceptSaturationDescriptor(&tmpNewReapplyImpReapplyConSatDes,rootProcessIndi,labelSet,calcAlgContext);
								}
								if (mConfImplicationAddingSkipping) {
									if (!conNeg && (opCode == CCSUB || opCode == CCIMPLTRIG || opCode == CCBRANCHTRIG)) {
										CSortedNegLinker<CConcept*>* opConceptLinkerIt = concept->getOperandList();
										while (opConceptLinkerIt) {
											if (!opConceptLinkerIt->isNegated()) {
												CConcept* opConcept = opConceptLinkerIt->getData();
												if (opConcept->getOperatorCode() == CCIMPL) {

													CImplicationReapplyConceptSaturationDescriptor tmpNewReapplyImpReapplyConSatDes;
													CConcept* implConcept = opConcept;
													CSortedNegLinker<CConcept*>* nextTriggerConcept = implConcept->getOperandList();
													tmpNewReapplyImpReapplyConSatDes.initImplicationReapllyConceptSaturationDescriptor(implConcept,nextTriggerConcept);

													updateImplicationReapplyConceptSaturationDescriptor(&tmpNewReapplyImpReapplyConSatDes,rootProcessIndi,labelSet,calcAlgContext);

												}
											}
											opConceptLinkerIt = opConceptLinkerIt->getNext();
										}
									}
								}

								if (labelSet->hasModifiedUpdateLinkers()) {
									processModificationUpdateLinkers(rootProcessIndi,labelSet,labelSet->getModifiedUpdateLinker(),calcAlgContext);
								}

							} else {
								contained = true;
							}	

						} else {
							contained = true;
							rootProcessIndi->addClashedConceptSaturationDescriptorLinker(conceptSaturationDescriptor);
							//mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
							//QFile tmpFile("saturation-model.txt");
							//if (tmpFile.open(QIODevice::WriteOnly)) {
							//	tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
							//	tmpFile.close();
							//}
							updateDirectAddingIndividualStatusFlags(rootProcessIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,calcAlgContext);
						}
					}

					return contained;
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::processModificationUpdateLinkers(CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, CSaturationModifiedProcessUpdateLinker* modProcUpdateLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CSaturationModifiedProcessUpdateLinker* modProcUpdateLinkerIt = modProcUpdateLinker; modProcUpdateLinkerIt; modProcUpdateLinkerIt = modProcUpdateLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* indiProcNode = modProcUpdateLinkerIt->getProcessingIndividual();
						CSaturationModifiedProcessUpdateLinker::MODIFICATIONPROCESSUPDATETYPE updateType = modProcUpdateLinkerIt->getUpdateType();
						if (updateType == CSaturationModifiedProcessUpdateLinker::UPDATEPDISJUNCTCOMMONCONCEPTSEXTRACTION) {
							addDisjunctCommonConceptExtractionToProcessingQueue(indiProcNode,calcAlgContext);
						}
					}
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateImplicationReapplyConceptSaturationDescriptor(CImplicationReapplyConceptSaturationDescriptor* reapplyImpReapplyConSatDes, CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(IMPLICATIONTRIGGERINGCOUNT,calcAlgContext);
					CSortedNegLinker<CConcept*>* currTriggerConcept = reapplyImpReapplyConSatDes->getNextTriggerConcept();
					CSortedNegLinker<CConcept*>* nextTriggerConcept = currTriggerConcept->getNext();
					CConcept* implConcept = reapplyImpReapplyConSatDes->getImplicationConcept();
					if (!nextTriggerConcept) {
						// execute implication
						STATINC(IMPLICATIONEXECUTINGCOUNT,calcAlgContext);
						CSortedNegLinker<CConcept*>* impExConOpLinker = implConcept->getOperandList();
						addConceptFilteredToIndividual(impExConOpLinker->getData(),impExConOpLinker->isNegated(),rootProcessIndi,labelSet,false,calcAlgContext);
					} else {
						CConcept* nextTrigger = nextTriggerConcept->getData();

						// add next trigger
						CImplicationReapplyConceptSaturationDescriptor* newReapplyImpReapplyConSatDes = createImplicationReapplyConceptSaturationDescriptor(calcAlgContext);
						newReapplyImpReapplyConSatDes->initImplicationReapllyConceptSaturationDescriptor(implConcept,nextTriggerConcept);

						CConceptSaturationDescriptor* conSatDes = nullptr;
						bool triggered = labelSet->insertConceptReapplicationReturnTriggered(nextTrigger->getConceptTag(),newReapplyImpReapplyConSatDes,&conSatDes);
						if (triggered) {
							updateImplicationReapplyConceptSaturationDescriptor(newReapplyImpReapplyConSatDes,rootProcessIndi,labelSet,calcAlgContext);
						}
					}
					return true;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::hasConceptLocalImpact(CConcept* concept, bool conNeg, CCalculationAlgorithmContextBase* calcAlgContext) {
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


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateDirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, cint64 flags, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNodeStatusFlags addingFlags;
					addingFlags.initStatusFlags();
					addingFlags.addFlags(flags);
					updateDirectAddingIndividualStatusFlags(indiNode,&addingFlags,calcAlgContext);
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateDirectNotDependentAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, cint64 flags, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNodeStatusFlags addingFlags;
					addingFlags.initStatusFlags();
					addingFlags.addFlags(flags);
					updateDirectNotDependentAddingIndividualStatusFlags(indiNode,&addingFlags,calcAlgContext);
				}

				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::requiresDirectAddingIndividualStatusFlagsUpdate(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNodeStatusFlags* indiDirectFlags = indiNode->getDirectStatusFlags();
					return !indiDirectFlags->hasFlags(addingFlags,true);
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::requiresIndirectAddingIndividualStatusFlagsUpdate(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNodeStatusFlags* indiIndirectFlags = indiNode->getIndirectStatusFlags();
					return !indiIndirectFlags->hasFlags(addingFlags,true);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateDirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (requiresDirectAddingIndividualStatusFlagsUpdate(indiNode,addingFlags,calcAlgContext)) {
						CIndividualSaturationProcessNodeStatusUpdateLinker* directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext);
						directUpdateLinker->initUpdateNodeLinker(indiNode);
						CIndividualSaturationProcessNodeStatusFlags* directIndiFlags = indiNode->getDirectStatusFlags();
						directIndiFlags->addFlags(addingFlags);
						++mDirectUpdatedStatusIndiNodeCount;

						while (directUpdateLinker) {
							CIndividualSaturationProcessNodeStatusUpdateLinker* nextUpdateLinker = directUpdateLinker;
							directUpdateLinker = directUpdateLinker->getNext();
							CIndividualSaturationProcessNode* updateIndiNode = nextUpdateLinker->getData();
							nextUpdateLinker->clearNext();
							releaseIndividualSaturationUpdateLinker(nextUpdateLinker,calcAlgContext);

							for (CXLinker<CIndividualSaturationProcessNode*>* depdendingIndiLinkerIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depdendingIndiLinkerIt; depdendingIndiLinkerIt = depdendingIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* dependingIndi = depdendingIndiLinkerIt->getData();
								if (requiresDirectAddingIndividualStatusFlagsUpdate(dependingIndi,addingFlags,calcAlgContext)) {
									CIndividualSaturationProcessNodeStatusFlags* directIndiFlags = dependingIndi->getDirectStatusFlags();
									directIndiFlags->addFlags(addingFlags);
									++mDirectUpdatedStatusIndiNodeCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(dependingIndi)->append(directUpdateLinker);
								}
							}

							updateIndirectAddingIndividualStatusFlags(updateIndiNode,addingFlags,calcAlgContext);
						}
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateDirectNotDependentAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (requiresDirectAddingIndividualStatusFlagsUpdate(indiNode,addingFlags,calcAlgContext)) {
						CIndividualSaturationProcessNodeStatusFlags* directIndiFlags = indiNode->getDirectStatusFlags();
						directIndiFlags->addFlags(addingFlags);
						if (requiresIndirectAddingIndividualStatusFlagsUpdate(indiNode,addingFlags,calcAlgContext)) {
							CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = indiNode->getIndirectStatusFlags();
							indirectIndiFlags->addFlags(addingFlags);
							++mIndirectUpdatedStatusIndiNodeCount;
						}
						++mDirectUpdatedStatusIndiNodeCount;

						CRoleBackwardSaturationPropagationHash* backwardPropHash = indiNode->getRoleBackwardPropagationHash(false);
						if (backwardPropHash) {
							CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
							for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
								CRole* backwardPropRole = it.key();
								const CRoleBackwardSaturationPropagationHashData& backwardPropData = it.value();
								for (CBackwardSaturationPropagationLink* linkIt = backwardPropData.mLinkLinker; linkIt; linkIt = linkIt->getNext()) {
									CIndividualSaturationProcessNode* sourceIndividual = linkIt->getSourceIndividual();
									updateIndirectAddingIndividualStatusFlags(sourceIndividual,addingFlags,calcAlgContext);
								}
							}
						}

						for (CXLinker<CIndividualSaturationProcessNode*>* nonInvConnIndiLinkerIt = indiNode->getNonInverseConnectedIndividualNodeLinker(); nonInvConnIndiLinkerIt; nonInvConnIndiLinkerIt = nonInvConnIndiLinkerIt->getNext()) {
							CIndividualSaturationProcessNode* sourceIndividual = nonInvConnIndiLinkerIt->getData();
							updateIndirectAddingIndividualStatusFlags(sourceIndividual,addingFlags,calcAlgContext);
						}
					}
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateIndirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (requiresIndirectAddingIndividualStatusFlagsUpdate(indiNode,addingFlags,calcAlgContext)) {
						CIndividualSaturationProcessNodeStatusUpdateLinker* directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext);
						directUpdateLinker->initUpdateNodeLinker(indiNode);
						CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = indiNode->getIndirectStatusFlags();
						indirectIndiFlags->addFlags(addingFlags);
						++mIndirectUpdatedStatusIndiNodeCount;

						while (directUpdateLinker) {
							CIndividualSaturationProcessNodeStatusUpdateLinker* nextUpdateLinker = directUpdateLinker;
							directUpdateLinker = directUpdateLinker->getNext();
							CIndividualSaturationProcessNode* updateIndiNode = nextUpdateLinker->getData();
							nextUpdateLinker->clearNext();
							releaseIndividualSaturationUpdateLinker(nextUpdateLinker,calcAlgContext);


							for (CXLinker<CIndividualSaturationProcessNode*>* depdendingIndiLinkerIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depdendingIndiLinkerIt; depdendingIndiLinkerIt = depdendingIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* dependingIndi = depdendingIndiLinkerIt->getData();
								if (requiresIndirectAddingIndividualStatusFlagsUpdate(dependingIndi,addingFlags,calcAlgContext)) {
									CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = dependingIndi->getIndirectStatusFlags();
									indirectIndiFlags->addFlags(addingFlags);
									++mIndirectUpdatedStatusIndiNodeCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(dependingIndi)->append(directUpdateLinker);
								}
							}


							CRoleBackwardSaturationPropagationHash* backwardPropHash = updateIndiNode->getRoleBackwardPropagationHash(false);
							if (backwardPropHash) {
								CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
								for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
									CRole* backwardPropRole = it.key();
									const CRoleBackwardSaturationPropagationHashData& backwardPropData = it.value();
									for (CBackwardSaturationPropagationLink* linkIt = backwardPropData.mLinkLinker; linkIt; linkIt = linkIt->getNext()) {
										CIndividualSaturationProcessNode* sourceIndividual = linkIt->getSourceIndividual();
										if (requiresIndirectAddingIndividualStatusFlagsUpdate(sourceIndividual,addingFlags,calcAlgContext)) {
											CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = sourceIndividual->getIndirectStatusFlags();
											indirectIndiFlags->addFlags(addingFlags);
											++mIndirectUpdatedStatusIndiNodeCount;
											directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
										}
									}
								}
							}


							for (CXLinker<CIndividualSaturationProcessNode*>* nonInvConnIndiLinkerIt = updateIndiNode->getNonInverseConnectedIndividualNodeLinker(); nonInvConnIndiLinkerIt; nonInvConnIndiLinkerIt = nonInvConnIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* sourceIndividual = nonInvConnIndiLinkerIt->getData();
								if (requiresIndirectAddingIndividualStatusFlagsUpdate(sourceIndividual,addingFlags,calcAlgContext)) {
									CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = sourceIndividual->getIndirectStatusFlags();
									indirectIndiFlags->addFlags(addingFlags);
									++mIndirectUpdatedStatusIndiNodeCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
								}
							}


						}
					}
				}















				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::requiresAddingSuccessorConnectedNominals(CIndividualSaturationProcessNode* indiNode, cint64 addingNominalID, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSuccessorConnectedNominalSet* succConnNomSet = indiNode->getSuccessorConnectedNominalSet(false);
					if (succConnNomSet) {
						if (succConnNomSet->hasSuccessorConnectedNominal(addingNominalID)) {
							return false;
						}
					}
					return true;
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateAddingSuccessorConnectedNominal(CIndividualSaturationProcessNode* indiNode, CSuccessorConnectedNominalSet* succConnNomSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (succConnNomSet) {
						for (CSuccessorConnectedNominalSet::const_iterator it = succConnNomSet->constBegin(), itEnd = succConnNomSet->constEnd(); it != itEnd; ++it) {
							cint64 connectedNominalID(*it);
							updateAddingSuccessorConnectedNominal(indiNode,connectedNominalID,calcAlgContext);
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateAddingSuccessorConnectedNominal(CIndividualSaturationProcessNode* indiNode, cint64 addingNominalID, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (requiresAddingSuccessorConnectedNominals(indiNode,addingNominalID,calcAlgContext)) {
						CIndividualSaturationProcessNodeStatusUpdateLinker* directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext);
						directUpdateLinker->initUpdateNodeLinker(indiNode);

						CSuccessorConnectedNominalSet* succConnNomSet = indiNode->getSuccessorConnectedNominalSet(true);
						succConnNomSet->addSuccessorConnectedNominal(addingNominalID);

						++mSuccessorConnectedNominalUpdatedCount;

						while (directUpdateLinker) {
							CIndividualSaturationProcessNodeStatusUpdateLinker* nextUpdateLinker = directUpdateLinker;
							directUpdateLinker = directUpdateLinker->getNext();
							CIndividualSaturationProcessNode* updateIndiNode = nextUpdateLinker->getData();
							nextUpdateLinker->clearNext();
							releaseIndividualSaturationUpdateLinker(nextUpdateLinker,calcAlgContext);


							for (CXLinker<CIndividualSaturationProcessNode*>* depdendingIndiLinkerIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depdendingIndiLinkerIt; depdendingIndiLinkerIt = depdendingIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* dependingIndi = depdendingIndiLinkerIt->getData();
								if (requiresAddingSuccessorConnectedNominals(dependingIndi,addingNominalID,calcAlgContext)) {
									CSuccessorConnectedNominalSet* depIndiSuccConnNomSet = dependingIndi->getSuccessorConnectedNominalSet(true);
									depIndiSuccConnNomSet->addSuccessorConnectedNominal(addingNominalID);
									++mSuccessorConnectedNominalUpdatedCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(dependingIndi)->append(directUpdateLinker);
								}
							}


							CRoleBackwardSaturationPropagationHash* backwardPropHash = updateIndiNode->getRoleBackwardPropagationHash(false);
							if (backwardPropHash) {
								CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
								for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
									CRole* backwardPropRole = it.key();
									const CRoleBackwardSaturationPropagationHashData& backwardPropData = it.value();
									for (CBackwardSaturationPropagationLink* linkIt = backwardPropData.mLinkLinker; linkIt; linkIt = linkIt->getNext()) {
										CIndividualSaturationProcessNode* sourceIndividual = linkIt->getSourceIndividual();
										if (requiresAddingSuccessorConnectedNominals(sourceIndividual,addingNominalID,calcAlgContext)) {
											CSuccessorConnectedNominalSet* sourceIndiSuccConnNomSet = sourceIndividual->getSuccessorConnectedNominalSet(true);
											sourceIndiSuccConnNomSet->addSuccessorConnectedNominal(addingNominalID);
											++mSuccessorConnectedNominalUpdatedCount;
											directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
										}
									}
								}
							}


							for (CXLinker<CIndividualSaturationProcessNode*>* nonInvConnIndiLinkerIt = updateIndiNode->getNonInverseConnectedIndividualNodeLinker(); nonInvConnIndiLinkerIt; nonInvConnIndiLinkerIt = nonInvConnIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* sourceIndividual = nonInvConnIndiLinkerIt->getData();
								if (requiresAddingSuccessorConnectedNominals(sourceIndividual,addingNominalID,calcAlgContext)) {
									CSuccessorConnectedNominalSet* sourceIndiSuccConnNomSet = sourceIndividual->getSuccessorConnectedNominalSet(true);
									sourceIndiSuccConnNomSet->addSuccessorConnectedNominal(addingNominalID);
									++mSuccessorConnectedNominalUpdatedCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
								}
							}
						}
					}
				}

















				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getAppliedANDRuleCount() {
					return mAppliedANDRuleCount;
				}

				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getAppliedORRuleCount() {
					return mAppliedORRuleCount;
				}

				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getAppliedSOMERuleCount() {
					return mAppliedSOMERuleCount;
				}

				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getAppliedATLEASTRuleCount() {
					return mAppliedATLEASTRuleCount;
				}

				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getAppliedALLRuleCount() {
					return mAppliedALLRuleCount;
				}

				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getAppliedATMOSTRuleCount() {
					return mAppliedATMOSTRuleCount;
				}

				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getAppliedTotalRuleCount() {
					return mAppliedTOTALRuleCount;
				}







				QString CCalculationTableauApproximationSaturationTaskHandleAlgorithm::generateExtendedDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext, cint64 firstIndiID, cint64 lastIndiID) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualSaturationProcessNodeVector* indiVec = procDataBox->getIndividualSaturationProcessNodeVector();
					cint64 indiCount = indiVec->getItemCount();
					if (lastIndiID >= 0) {
						indiCount = qMin(lastIndiID,indiCount);
					}
					firstIndiID = qMin(firstIndiID,indiCount);
					QStringList indiStringList;
					for (cint64 i = firstIndiID; i < indiCount; ++i) {
						CIndividualSaturationProcessNode* indi = indiVec->getData(i);
						if (indi) {
							CExtendedConceptReferenceLinkingData* extConRefLinkData = indi->getSaturationConceptReferenceLinking();
							CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)extConRefLinkData;
							QString nominalString = QString("");
							if (conceptSatItem) {
								CConcept* initializedConcept = conceptSatItem->getSaturationConcept();
								bool initializedNegation = conceptSatItem->getSaturationNegation();
								if (initializedConcept->hasClassName() && !initializedNegation) {
									nominalString = QString(", ")+CIRIName::getRecentIRIName(initializedConcept->getClassNameLinker());
								} else {
									if (!initializedNegation) {
										nominalString = QString(", %1").arg(initializedConcept->getConceptTag());
									} else {
										nominalString = QString(", %1%2").arg(QChar(0x00AC)).arg(initializedConcept->getConceptTag());
									}
								}
								//if (indi->getNominalIndividual()) {
								//	nominalString = QString(", ")+CIRIName::getRecentIRIName(indi->getNominalIndividual()->getNameLinker());
								//}
							} else {
								nominalString = QString(",Resolved-Extension-Node");
							}
							QString refModeString("[new root node]");
							CIndividualSaturationProcessNode* refIndi = indi->getReferenceIndividualSaturationProcessNode();
							if (refIndi) {
								cint64 refIndiID = refIndi->getIndividualID();
								if (refIndi->getReferenceMode() == 1) {
									refModeString = QString("[substitute node %1]").arg(refIndiID);
								} else if (refIndi->getReferenceMode() == 2) {
									refModeString = QString("[copy node %1]").arg(refIndiID);
								}
							}
							QString indiString = QString("[ %1%2 ] <- %3<br>\n").arg(indi->getIndividualID()).arg(nominalString).arg(refModeString);
							QString conSetString;
							CReapplyConceptSaturationLabelSet* conSet = indi->getReapplyConceptSaturationLabelSet(false);
							if (conSet) {
								CConceptSaturationDescriptor* conDesIt = conSet->getConceptSaturationDescriptionLinker();
								while (conDesIt) {
									CConceptSaturationDescriptor* conDes = conDesIt;
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
									conDesIt = conDesIt->getNextConceptDesciptor();
								}
							}
							QString propString;
							CRoleBackwardSaturationPropagationHash* backwardPropHash = indi->getRoleBackwardPropagationHash(false);
							if (backwardPropHash) {
								CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
								for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
									CRole* backwardPropRole = it.key();
									const CRoleBackwardSaturationPropagationHashData& backwardPropData = it.value();
									QString roleString;
									roleString += QString::number(backwardPropRole->getRoleTag());
									if (backwardPropRole->hasPropertyName()) {
										roleString += QString("~")+CIRIName::getRecentIRIName(backwardPropRole->getPropertyNameLinker());
									}

									QString sourceIndiListString;
									CBackwardSaturationPropagationLink* backPropLinkIt = backwardPropData.mLinkLinker;
									while (backPropLinkIt) {
										CIndividualSaturationProcessNode* sourceIndiNode = backPropLinkIt->getSourceIndividual();
										if (!sourceIndiListString.isEmpty()) {
											sourceIndiListString.append(", ->");
										} else {
											sourceIndiListString.append(" ->");
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





							QString succString;
							CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indi->getLinkedRoleSuccessorHash(false);
							if (linkedSuccHash) {
								QStringList succRoleStringList;
								CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
								for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator it = succHash->constBegin(), itEnd = succHash->constEnd(); it != itEnd; ++it) {
									CRole* role(it.key());
									CLinkedRoleSaturationSuccessorData* succLinkData(it.value());
									CPROCESSMAP<cint64,CSaturationSuccessorData*>* succDataMap = succLinkData->getSuccessorNodeDataMap(false);
									QStringList linkedSuccIndiStringList;
									for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator itSucc = succDataMap->constBegin(), itSuccEnd = succDataMap->constEnd(); itSucc != itSuccEnd; ++itSucc) {
										cint64 succIndiID = itSucc.key();
										CSaturationSuccessorData* succData(itSucc.value());
										QStringList indiSuccStatusStringList;
										if (succData->mExtension) {
											indiSuccStatusStringList << QString("~extension");
										} else {
											indiSuccStatusStringList << QString("~base");
										}
										if (succData->mActiveCount > 0) {
											indiSuccStatusStringList << QString("~active");
										} else {
											indiSuccStatusStringList << QString("~inactive");
										}
										if (succLinkData->mRoleALLConceptsProcessingQueued) {
											indiSuccStatusStringList << QString("~all-ext-queued");
										}
										QStringList createRoleStingList;
										for (CXNegLinker<CRole*>* roleCreationLinkerIt = succData->mCreationRoleLinker; roleCreationLinkerIt; roleCreationLinkerIt = roleCreationLinkerIt->getNext()) {
											QString creationRoleString;
											CRole* creationRole = roleCreationLinkerIt->getData();
											creationRoleString += QString::number(creationRole->getRoleTag());
											if (creationRole->hasPropertyName()) {
												creationRoleString += QString("~")+CIRIName::getRecentIRIName(creationRole->getPropertyNameLinker());
											}
											createRoleStingList << creationRoleString;
										}
										QString linkedSuccIndiString = QString("\t~>%1; link-status: %2; creation-roles: %3;").arg(succIndiID).arg(indiSuccStatusStringList.join(", ")).arg(createRoleStingList.join(", "));
										linkedSuccIndiStringList.append(linkedSuccIndiString);
									}

									if (!linkedSuccIndiStringList.isEmpty()) {
										QString roleString;
										roleString += QString::number(role->getRoleTag());
										if (role->hasPropertyName()) {
											roleString += QString("~")+CIRIName::getRecentIRIName(role->getPropertyNameLinker());
										}

										QString roleSuccString = QString("<br>\n%1:<br>\n%2").arg(roleString).arg(linkedSuccIndiStringList.join("<br>\n"));
										succRoleStringList.append(roleSuccString);
									}
								}
								succString = QString("<br>\n%1").arg(succRoleStringList.join(""));
							}
							if (succString.isEmpty()) {
								succString = "-";
							}


							QString depIndiString;
							CXLinker<CIndividualSaturationProcessNode*>* depIndiLinker = indi->getCopyDependingIndividualNodeLinker();
							if (depIndiLinker) {
								QString depIndiListString;
								while (depIndiLinker) {
									CIndividualSaturationProcessNode* depIndi = depIndiLinker->getData();
									depIndiLinker = depIndiLinker->getNext();
									if (!depIndiListString.isEmpty()) {
										depIndiListString.append(", ");
									} else {
										depIndiListString.append(">>");
									}
									depIndiListString.append(QString::number(depIndi->getIndividualID()));
								}
								depIndiString = QString("<br>\nForwarding to: %1<br>\n").arg(depIndiListString);
							}

							CIndividualSaturationProcessNodeStatusFlags* directStatusFlags = indi->getDirectStatusFlags();
							QString directFlagString;
							QStringList directFlagsStringList = generateStatusFlagsStringList(directStatusFlags,calcAlgContext);
							if (!directFlagsStringList.isEmpty()) {
								directFlagString = QString("<br>\nDirect Flags: %1<br>\n").arg(directFlagsStringList.join(","));
							}
							CIndividualSaturationProcessNodeStatusFlags* indirectStatusFlags = indi->getIndirectStatusFlags();
							QString indirectFlagString;
							QStringList indirectFlagsStringList = generateStatusFlagsStringList(indirectStatusFlags,calcAlgContext);
							if (!indirectFlagsStringList.isEmpty()) {
								indirectFlagString = QString("<br>\nIndirect Flags: %1<br>\n").arg(indirectFlagsStringList.join(","));
							}


							QString clashedString;
							CConceptSaturationDescriptor* clashConDesLinker = indi->getClashedConceptSaturationDescriptorLinker();
							if (clashConDesLinker) {
								QString clashedConDesString;
								for (CConceptSaturationDescriptor* clashConDesLinkerIt = clashConDesLinker; clashConDesLinkerIt; clashConDesLinkerIt = clashConDesLinkerIt->getNext()) {
									CConcept* concept = clashConDesLinkerIt->getConcept();
									cint64 conTag = clashConDesLinkerIt->getConceptTag();
									QString conceptString("null");
									if (clashConDesLinkerIt) {
										conceptString = CConceptTextFormater::getConceptString(clashConDesLinkerIt->getConcept(),clashConDesLinkerIt->isNegated());
									}
									if (!clashedConDesString.isEmpty()) {
										clashedConDesString += QString("<br>\n");
									}
									clashedConDesString += conceptString;
								}
								clashedString += QString("<br>\nClashed Inserted Concepts: {<br>\n%1<br>\n}<br>\n").arg(clashedConDesString);
							}


							indiString += QString("{%1} <br>\n<br>\nBackwardlinks %2<br>\nSuccessorLinks %3<br>\n%4%5%6%7").arg(conSetString).arg(propString).arg(succString).arg(depIndiString).arg(directFlagString).arg(indirectFlagString).arg(clashedString);
							indiStringList.append(indiString);
						}
					}


					mDebugIndiModelStringList = indiStringList;
					mDebugIndiModelString = mDebugIndiModelStringList.join("<br>\n<p><br>\n\r\n");
					return mDebugIndiModelString;
				}

				QStringList CCalculationTableauApproximationSaturationTaskHandleAlgorithm::generateStatusFlagsStringList(CIndividualSaturationProcessNodeStatusFlags* statusFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					QStringList flagsStringList;
					if (statusFlags->hasClashedFlag()) {
						flagsStringList += QString("clashed");
					}
					if (statusFlags->hasInsufficientFlag()) {
						flagsStringList += QString("insufficient");
					}
					if (statusFlags->hasNominalConnectionFlag()) {
						flagsStringList += QString("nominal connection");
					}
					if (statusFlags->hasCriticalFlag()) {
						flagsStringList += QString("critical");
					}
					if (statusFlags->hasCardinalityProplematicFlag()) {
						flagsStringList += QString("cardinality problematic");
					}
					if (statusFlags->hasEQCandidateProplematicFlag()) {
						flagsStringList += QString("eqCandidate problematic");
					}
					if (statusFlags->hasMissedABoxConsistencyFlag()) {
						flagsStringList += QString("missed ABox consistency data");
					}
					if (statusFlags->hasInitializedFlag()) {
						flagsStringList += QString("initialized");
					}
					if (statusFlags->hasCompletedFlag()) {
						flagsStringList += QString("completed");
					}
					if (flagsStringList.isEmpty()) {
						flagsStringList += QString("none");
					}
					return flagsStringList;
				}

				QString CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getDebugIndividualConceptName(CIndividualSaturationProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CExtendedConceptReferenceLinkingData* extConRefLinkData = indi->getSaturationConceptReferenceLinking();
					CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)extConRefLinkData;
					QString nominalString = QString("");
					if (conceptSatItem) {
						CConcept* initializedConcept = conceptSatItem->getSaturationConcept();
						bool initializedNegation = conceptSatItem->getSaturationNegation();
						if (initializedConcept->hasClassName() && !initializedNegation) {
							nominalString = CIRIName::getRecentIRIName(initializedConcept->getClassNameLinker());
						}
					}
					return nominalString;
				}

				QString CCalculationTableauApproximationSaturationTaskHandleAlgorithm::generateDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualSaturationProcessNodeVector* indiVec = procDataBox->getIndividualSaturationProcessNodeVector();
					cint64 indiCount = indiVec->getItemCount();
					QStringList indiStringList;
					for (cint64 i = 0; i < indiCount; ++i) {
						CIndividualSaturationProcessNode* indi = indiVec->getData(i);
						CReapplyConceptSaturationLabelSet* conSet = indi->getReapplyConceptSaturationLabelSet(false);
						if (conSet) {
							QString nominalString = QString("");
							//if (indi->getNominalIndividual()) {
							//	nominalString = QString(", ")+CIRIName::getRecentIRIName(indi->getNominalIndividual()->getNameLinker());
							//}
							QString indiString = QString("[ %1%2 ] = ").arg(indi->getIndividualID()).arg(nominalString);
							QString conSetString;
							CReapplyConceptSaturationLabelSetIterator conSetIt = conSet->getIterator(true,false);
							while (conSetIt.hasNext()) {
								CConceptSaturationDescriptor* conDes = conSetIt.getConceptSaturationDescriptor();
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




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::testDebugSaturationTaskContainsConcepts(CIndividualSaturationProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* conSet = indiNode->getReapplyConceptSaturationLabelSet();
					if (conSet) {
						CReapplyConceptSaturationLabelSetIterator it = conSet->getIterator(true,false);
						while (it.hasNext()) {
							CConceptSaturationDescriptor* conSatDes = it.getConceptSaturationDescriptor();
							if (conSatDes) {
								CConcept* concept = conSatDes->getConcept();
								bool conceptNegation = conSatDes->getNegation();
								testDebugSaturationTaskContainsConcept(concept,conceptNegation,indiNode,calcAlgContext);
							}
							it.moveNext();
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::testDebugSaturationTaskContainsConcept(CConcept* concept, bool conNegation, CIndividualSaturationProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext) {
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
