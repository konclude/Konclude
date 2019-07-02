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

#include "CCalculationTableauApproximationSaturationTaskHandleAlgorithm.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CCalculationTableauApproximationSaturationTaskHandleAlgorithm::CCalculationTableauApproximationSaturationTaskHandleAlgorithm(CSaturationNodeBackendAssociationCacheHandler* backendAssCaceHandler, CSatisfiableTaskSaturationOccurrenceStatisticsCollector* satTaskOccStatCollector) {
					mProcessingDataBox = nullptr;
					mCalcAlgContext = nullptr;

					mBackendAssCaceHandler = backendAssCaceHandler;
					mSatTaskOccStatCollector = satTaskOccStatCollector;

					mProcessingDataBox = nullptr;
					mCalcAlgContext = nullptr;

					mPosJumpFuncVec = &mPosTableauRuleJumpFuncVec[mRuleFuncCount/2];
					mNegJumpFuncVec = &mNegTableauRuleJumpFuncVec[mRuleFuncCount/2];

					for (cint64 i = 0; i < mRuleFuncCount; ++i) {
						mPosTableauRuleJumpFuncVec[i] = nullptr;
						mNegTableauRuleJumpFuncVec[i] = nullptr;
					}


					mPosJumpFuncVec[CCDATATYPE] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyDATATYPERule;
					//mNegJumpFuncVec[CCDATATYPE] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyNotDATATYPERule;
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
					mPosJumpFuncVec[CCDATALITERAL] = &CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyDATALITERALRule;


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

					mDebugTestingSaturationTask = nullptr;

					mConfImplicationAddingSkipping = true;
					mConfForceAllConceptInsertion = false;
					mConfDebuggingWriteData = false;
					mConfDebuggingWriteDataSaturationTasks = false;
					mConfForceAllCopyInsteadOfSubstituition = false;

					mConfAddCriticalConceptsToQueues = false;
					mConfDirectlyCriticalToInsufficient = true;
					mConfCheckCriticalConcepts = false;
					mConfALLConceptsExtensionProcessing = false;
					mConfFUNCTIONALConceptsExtensionProcessing = false;
					mConfConceptsExtensionProcessing = false;

					mConfResolveOperandConceptSize = 100;
					mConfReferredNodeManyConceptCount = 500;
					mConfManyConceptReferredNodeCountProcessLimit = 2;
					mConfReferredNodeConceptCountProcessLimit = 1500;
					mConfReferredNodeUnprocessedCountProcessLimit = 1;
					mConfReferredNodeCheckingDepth = 5;
					mConfOccurrenceStatisticsCollection = true;

					mConfCopyNodeFromTopIndividualForManyConcepts = true;
					mConfForceManyConceptSaturation = false;
					mWroteFunctionalSuccPredMergingDebugString = false;

					mConfDetailedMergingTestForATMOSTCriticalTesting = true;
					mConfSimpleMergingTestForATMOSTCriticalTesting = true;

					mConfDelayedMergingCriticalATMOSTConcepts = true;
					mConfDelayedMergingCriticalATMOSTConceptsCardinalitySize = 100;

					mSuccessorConnectedNominalUpdatedCount = 0;
					mMaximumCardinalityCandidatesUpdatedCount = 0;
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
							mConfDebuggingWriteDataSaturationTasks = config->isDebuggingWriteDataSaturationTasksActivated();
							if (!ontStructureSummary || !ontStructureSummary->hasOnlyELConceptClasses() || ontology->getABox()->getIndividualCount() > 0) {
								mConfForceAllConceptInsertion = true;
								mConfImplicationAddingSkipping = false;
							}

							mConfDirectlyCriticalToInsufficient = config->isSaturationDirectCriticalToInsufficientActivated();
							mConfAddCriticalConceptsToQueues = config->isSaturationCriticalConceptTestingActivated();
							mConfCheckCriticalConcepts = config->isSaturationCriticalConceptTestingActivated();
							mConfConceptsExtensionProcessing = config->isSaturationSuccessorExtensionActivated();
							mConfNominalProcessing = config->isNominalSaturationActivated();

							mConfReferredNodeManyConceptCount = config->getSaturationReferredNodeManyConceptCount();
							mConfManyConceptReferredNodeCountProcessLimit = config->getSaturationManyConceptReferredNodeCountProcessLimit();
							mConfReferredNodeConceptCountProcessLimit = config->getSaturationReferredNodeConceptCountProcessLimit();
							mConfReferredNodeUnprocessedCountProcessLimit = config->getSaturationReferredNodeUnprocessedCountProcessLimit();
							mConfReferredNodeCheckingDepth = config->getSaturationReferredNodeCheckingDepth();

							mConfOccurrenceStatisticsCollection = config->isOccurrenceStatisticsCollectionActivated();

						} else {
							mConfForceAllCopyInsteadOfSubstituition = false;
							mConfForceAllConceptInsertion = true;
							mConfImplicationAddingSkipping = true;
							mConfNominalProcessing = true;
							mConfDebuggingWriteData = false;
							mConfDebuggingWriteDataSaturationTasks = false;

							mConfDirectlyCriticalToInsufficient = true;
							mConfAddCriticalConceptsToQueues = false;
							mConfCheckCriticalConcepts = false;
							mConfConceptsExtensionProcessing = false;

							mConfOccurrenceStatisticsCollection = true;

							mConfReferredNodeManyConceptCount = 500;
							mConfManyConceptReferredNodeCountProcessLimit = 2;
							mConfReferredNodeConceptCountProcessLimit = 1500;
							mConfReferredNodeUnprocessedCountProcessLimit = 1;
							mConfReferredNodeCheckingDepth = 5;

						}

						mConfALLConceptsExtensionProcessing = mConfConceptsExtensionProcessing;
						mConfFUNCTIONALConceptsExtensionProcessing = mConfConceptsExtensionProcessing;

						mLastConfig = config;
						mBackendAssCaceHandler->readCalculationConfig(config);
						mBackendAssCaceHandler->setWorkingOntology(satCalcTask->getProcessingDataBox()->getOntology());
					}


					//mConfForceAllConceptInsertion = true;
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


						mInsufficientALLCount = 0;
						mInsufficientATMOSTCount = 0;


						mCachedCompletionGraphLoaded = false;
						mRepresentativeDataLoaded = false;
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
						bool separatedSaturation = false;
						bool hasFirstProcessedNodeID = false;
						cint64 firstProcessedNodeID = 0;

						CIndividualSaturationProcessNode* lastindiProcSatNode = nullptr;
						try {
							STATINC(TASKPROCESSCHANGECOUNT,calcAlgContext);


							continueNominalDelayedIndividualNodeProcessing(mProcessingDataBox,calcAlgContext);

							bool wroteExtensionProcDebugString = false;

							while (hasRemainingMergingCriticalExtensionProcessingNodes(mProcessingDataBox,calcAlgContext)) {

								while (hasRemainingExtensionProcessingNodes(mProcessingDataBox,calcAlgContext)) {

									while (hasRemainingProcessingNodes(mProcessingDataBox,calcAlgContext)) {

										while (mProcessingDataBox->hasIndividualSaturationProcessNodeLinker()) {
											CIndividualSaturationProcessNodeLinker* indiProcessSaturationNodeLinker = mProcessingDataBox->takeIndividualSaturationProcessNodeLinker();
											CIndividualSaturationProcessNode* indiProcSatNode = indiProcessSaturationNodeLinker->getData();
											if (indiProcSatNode->isSeparated()) {
												separatedSaturation = true;
											}
											if (!hasFirstProcessedNodeID || indiProcSatNode->getIndividualID() < firstProcessedNodeID) {
												hasFirstProcessedNodeID = true;
												firstProcessedNodeID = indiProcSatNode->getIndividualID();
											}

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

											//if ((indiProcessedCount % 100000) == 0) {
											//	writeIndividualSaturationStatistics(calcAlgContext);
											//}
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

									//if (mConfDebuggingWriteData && mConfDebuggingWriteDataSaturationTasks && !wroteExtensionProcDebugString) {
									//	wroteExtensionProcDebugString = true;
									//	mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
									//	QString fileName("./Debugging/SaturationTasks/saturation-model-ext-proc.txt");
									//	QFile tmpFile(fileName);
									//	if (tmpFile.open(QIODevice::WriteOnly)) {
									//		tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
									//		tmpFile.close();
									//	}
									//}

									processNextSuccessorExtensions(calcAlgContext);

								}


								//if (mConfDebuggingWriteData && mConfDebuggingWriteDataSaturationTasks) {
								//	wroteExtensionProcDebugString = true;
								//	mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
								//	QString fileName("./Debugging/SaturationTasks/saturation-model-bc.txt");
								//	QFile tmpFile(fileName);
								//	if (tmpFile.open(QIODevice::WriteOnly)) {
								//		tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
								//		tmpFile.close();
								//	}
								//}

								if (mConfCheckCriticalConcepts && hasNextCriticalConcepts(mCalcAlgContext)) {
									while (hasNextCriticalConcepts(mCalcAlgContext)) {
										checkNextCriticalConcepts(mCalcAlgContext);
									}
								}


								//if (mConfDebuggingWriteData && mConfDebuggingWriteDataSaturationTasks) {
								//	wroteExtensionProcDebugString = true;
								//	mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
								//	QString fileName("./Debugging/SaturationTasks/saturation-model-ac.txt");
								//	QFile tmpFile(fileName);
								//	if (tmpFile.open(QIODevice::WriteOnly)) {
								//		tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
								//		tmpFile.close();
								//	}
								//}

								if (mProcessingDataBox->hasSaturationATMOSTMergingProcessLinker()) {
									tryATMOSTConceptSuccessorMerging(calcAlgContext);
								}

							}


							completeSaturatedIndividualNodes(mProcessingDataBox,mCalcAlgContext);

							if (mConfOccurrenceStatisticsCollection && satCalcTask->getOccurrenceStatisticsCollectingAdapter()) {
								mSatTaskOccStatCollector->analyseSatisfiableTask(satCalcTask, calcAlgContext);
							}


							if (mConfDebuggingWriteData && mConfDebuggingWriteDataSaturationTasks) {
								writeIndividualSaturationStatistics(calcAlgContext);
								mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
								QString fileName("./Debugging/SaturationTasks/saturation-model.txt");
								if (separatedSaturation) {
									fileName = QString("./Debugging/SaturationTasks/saturation-model-individuals-%1.txt").arg(firstProcessedNodeID);
								}
								QFile tmpFile(fileName);
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
								LOG(ERROR,"::Konclude::Reasoner::Kernel::Algorithm::TableauSaturationAlgorihm",logTr("Error occurred, computation stopped."),this);
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

							mSatTaskSaturationIndiAnalyser.analyseSatisfiableTask(satCalcTask,mCalcAlgContext);

							mSatTaskSaturationPreyAnalyser.analyseSatisfiableTask(satCalcTask,mCalcAlgContext);

							tryAssociateIndividualNodesWithBackendCache(satCalcTask,mCalcAlgContext);

							//testInsufficientIndividuls(mCalcAlgContext);

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

















				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::writeIndividualSaturationStatistics(CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNodeVector* indiVec = calcAlgContext->getProcessingDataBox()->getIndividualSaturationProcessNodeVector(false);
					cint64 insufficientCount = 0;
					QFile tmpFile("./Debugging/SaturationTasks/saturation-statistics.txt");
					if (tmpFile.open(QIODevice::WriteOnly)) {
						QString headerString = QString("IndividualID\tIndividualName\tTotalCount\tConceptCount\tTriggerCount\tRoleBackConnectionCount\tRoleBackPropagationCount\tRoleBackLinkedCount\r\n");
						tmpFile.write(headerString.toUtf8());
						if (indiVec) {
							cint64 indiCount = indiVec->getItemCount();
							for (cint64 i = 0; i < indiCount; ++i) {
								CIndividualSaturationProcessNode* indi = indiVec->getData(i);
								if (indi) {
									QString indiName = getDebugIndividualConceptName(indi,calcAlgContext);
									if (indiName == "") {
										indiName = "-";
									}
									cint64 indiID = indi->getIndividualID();
									cint64 conCount = 0;
									cint64 totalCount = 0;
									cint64 implCount = 0;
									cint64 roleBackCount = 0;
									cint64 roleBackPropCount = 0;
									cint64 roleBackLinkCount = 0;
									QString indiDirectFlags = generateStatusFlagsStringList(indi->getDirectStatusFlags(),calcAlgContext).join(",");
									QString indiIndirectFlags = generateStatusFlagsStringList(indi->getIndirectStatusFlags(),calcAlgContext).join(",");
									CReapplyConceptSaturationLabelSet* labelConSet = indi->getReapplyConceptSaturationLabelSet(false);
									if (labelConSet) {
										conCount = labelConSet->getConceptCount();
										totalCount = labelConSet->getTotalCount();
										implCount = totalCount-conCount;
									}
									CRoleBackwardSaturationPropagationHash* roleBackPropHash = indi->getRoleBackwardPropagationHash(false);
									if (roleBackPropHash) {
										CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* rolePropHash = roleBackPropHash->getRoleBackwardPropagationDataHash();
										roleBackCount = rolePropHash->size();
										for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator rolePropHashIt = rolePropHash->constBegin(), rolePropHashEndIt = rolePropHash->constEnd(); rolePropHashIt != rolePropHashEndIt; ++rolePropHashIt) {
											CRole* role = rolePropHashIt.key();
											const CRoleBackwardSaturationPropagationHashData& roleBackData = rolePropHashIt.value();
											for (CBackwardSaturationPropagationLink* linkLinkerIt = roleBackData.mLinkLinker; linkLinkerIt; linkLinkerIt = linkLinkerIt->getNext()) {
												++roleBackLinkCount;
											}
											for (CBackwardSaturationPropagationReapplyDescriptor* reapDesIt = roleBackData.mReapplyLinker; reapDesIt; reapDesIt = reapDesIt->getNext()) {
												++roleBackPropCount;
											}
										}
									}
									QString indiString = QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t").arg(indiID).arg(indiName).arg(totalCount).arg(conCount).arg(implCount).arg(roleBackCount).arg(roleBackPropCount).arg(roleBackLinkCount);
									indiString += QString("%1\t%2\r\n").arg(indiDirectFlags).arg(indiIndirectFlags);
									tmpFile.write(indiString.toUtf8());

								}
							}
						}
						tmpFile.close();
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::testInsufficientIndividuls(CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNodeVector* indiVec = calcAlgContext->getProcessingDataBox()->getIndividualSaturationProcessNodeVector(false);
					cint64 insufficientCount = 0;
					if (indiVec) {
						cint64 indiCount = indiVec->getItemCount();
						for (cint64 i = 0; i < indiCount; ++i) {
							CIndividualSaturationProcessNode* indi = indiVec->getData(i);
							if (indi) {
								CIndividual* nominalIndi = indi->getNominalIndividual();
								if (nominalIndi) {
									if (indi->getIndirectStatusFlags()->hasInsufficientFlag()) {
										++insufficientCount;
									}
								}
							}
						}
					}
					if (insufficientCount > 0) {
						bool bug = true;
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::tryAssociateIndividualNodesWithBackendCache(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = statCalcTask->getProcessingDataBox();
					CIndividualSaturationProcessNodeLinker* indiSaturationAnalysingNodeLinker = procDataBox->getIndividualSaturationAnalysationNodeLinker();
					if (indiSaturationAnalysingNodeLinker && calcAlgContext->getSatisfiableCalculationTask()->getSaturationIndividualsAnalysationObserver()) {
						for (CIndividualSaturationProcessNodeLinker* indiSaturationAnalysingNodeLinkerIt = indiSaturationAnalysingNodeLinker; indiSaturationAnalysingNodeLinkerIt; indiSaturationAnalysingNodeLinkerIt = indiSaturationAnalysingNodeLinkerIt->getNext()) {
							CIndividualSaturationProcessNode* satIndiNode = indiSaturationAnalysingNodeLinkerIt->getProcessingIndividual();
							CIndividualSaturationProcessNodeStatusFlags* indStatFlags = satIndiNode->getIndirectStatusFlags();
							if (indStatFlags->hasClashedFlag()) {
								return;
							}
						}
						mBackendAssCaceHandler->tryAssociateNodesWithBackendCache(indiSaturationAnalysingNodeLinker, calcAlgContext->getSatisfiableCalculationTask()->getSatisfiableRepresentativeBackendCacheUpdatingAdapter(), calcAlgContext);
					}
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



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::hasRemainingMergingCriticalExtensionProcessingNodes(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mConfCheckCriticalConcepts && hasNextCriticalConcepts(calcAlgContext)) {
						return true;
					}
					if (processingDataBox->hasSaturationATMOSTMergingProcessLinker()) {
						return true;
					}
					if (hasRemainingExtensionProcessingNodes(processingDataBox,calcAlgContext)) {
						return true;
					}
					return false;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::continueNominalDelayedIndividualNodeProcessing(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext)  {
					bool nominalDelayedIndividualNodeProcessingContinued = false;
					if (processingDataBox->hasNominalDelayedIndividualSaturationProcessNodeLinker()) {
						if (isConsistenceDataAvailable(calcAlgContext)) {
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
								if (!isConsistenceDataAvailable(calcAlgContext)) {
									completeIndividual = false;
									processingDataBox->addNominalDelayedIndividualSaturationProcessNodeLinker(indiProcessNodeLinker);
									if (indiProcessNode->getReapplyConceptSaturationLabelSet(false)) {
										indiProcessNode->getReapplyConceptSaturationLabelSet(false)->setLastNominalIndependentConceptSaturationDescriptorLinker(indiProcessNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker());
									}
								}
							} 
							if (completeIndividual) {
								processingDataBox->addIndividualSaturationCompletedNodeLinker(indiProcessNodeLinker);
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
						
						if (indiProcSatNode->hasNominalIndividualTriplesAssertions() && !indiProcSatNode->areNominalIndividualTriplesAssertionsLoaded()) {

							CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
							COntologyTriplesData* ontologyTriplesData = ontology->getOntologyTriplesData();
							if (ontologyTriplesData) {
								COntologyTriplesAssertionsAccessor* triplesAssertionAccessor = ontologyTriplesData->getTripleAssertionAccessor();
								if (triplesAssertionAccessor) {
									if (!indiProcSatNode->getNominalIndividual()) {
										CIndexedIndividualAssertionConvertionVisitor indiAssConvVisitor(indiProcSatNode->getIndividualID(), calcAlgContext);
										triplesAssertionAccessor->visitIndividualAssertions(indiProcSatNode->getIndividualID(), &indiAssConvVisitor);
										indiProcSatNode->setNominalIndividual(indiAssConvVisitor.getRetrievalIndividual());
									} else {
										CIndexedIndividualAssertionConvertionVisitor indiAssConvVisitor(indiProcSatNode->getNominalIndividual(), calcAlgContext);
										triplesAssertionAccessor->visitIndividualAssertions(indiProcSatNode->getIndividualID(), &indiAssConvVisitor);
										indiProcSatNode->setNominalIndividual(indiAssConvVisitor.getRetrievalIndividual());
									}
								}
							}

							indiProcSatNode->setNominalIndividualTriplesAssertionsLoaded(true);
						}

						initializeInitializationConcepts(indiProcSatNode,calcAlgContext);
						initializeRoleAssertions(indiProcSatNode,calcAlgContext);
						initializeDataAssertions(indiProcSatNode, calcAlgContext);

						indiProcSatNode->setInitialized(true);

						if (indiProcSatNode->getNominalIndividual() && calcAlgContext->getSatisfiableCalculationTask()->getSaturationIndividualsAnalysationObserver()) {
							CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
							CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker = CObjectAllocator< CIndividualSaturationProcessNodeLinker >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
							indiProcessNodeLinker->initProcessNodeLinker(indiProcSatNode,true);
							processingDataBox->addIndividualSaturationAnalysationNodeLinker(indiProcessNodeLinker);
						}
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
							if (!isConsistenceDataAvailable(calcAlgContext)) {
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

								if (!linkLinkerIt->mVALUENominalConnection) {

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



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::installSuccessorPredecessorRoleFunctionalityConceptsExtension(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
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
					CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(true);
					if (backwardPropHash) {
						CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backwardPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();
						if (backwardPropDataHash) {
							CRoleBackwardSaturationPropagationHashData& backPropData = (*backwardPropDataHash)[role];
							if (!backPropData.mRolePredecessorMergingQueuingRequired) {
								backPropData.mRolePredecessorMergingQueuingRequired = true;
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


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateSuccessorRoleQualifiedFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CSortedNegLinker<CConcept*>* qualifiyConLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool updated = false;
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					if (linkedSuccHash) {
						CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
						CLinkedRoleSaturationSuccessorData* succData = linkedSuccHash->getLinkedRoleSuccessorData(role,false);
						if (succData && succData->mSuccCount > 1) {
							updated |= updateSuccessorRoleQualifiedFUNCTIONALConceptsExtensions(indiProcSatNode,role,qualifiyConLinker,succData,calcAlgContext);
						}
					}
					return updated;
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updatePredecessorRoleFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool updated = false;
					CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(true);
					if (backwardPropHash) {
						CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backwardPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();
						if (backwardPropDataHash) {
							CRoleBackwardSaturationPropagationHashData& backPropData = (*backwardPropDataHash)[role];
							if (backPropData.mRolePredecessorMergingQueuingRequired) {
								backPropData.mRolePredecessorMergingProcessingQueued = false;
								if (backPropData.mLinkLinker) {
									CBackwardSaturationPropagationLink* backPropLinkLinker = backPropData.mLinkLinker;
									CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
									if (linkedSuccHash) {
										CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
										CLinkedRoleSaturationSuccessorData* succData = linkedSuccHash->getLinkedRoleSuccessorData(role,false);
										if (succData->mSuccCount >= 1) {
											updated |= updatePredecessorRoleFUNCTIONALConceptsExtensions(indiProcSatNode,role,succData,backPropLinkLinker,calcAlgContext);
										}
									}
								}
							}
						}
					}
					return updated;
				}









				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updatePredecessorRoleFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CLinkedRoleSaturationSuccessorData* succData, CBackwardSaturationPropagationLink* backPropLinkLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool updated = false;
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succDataHash = linkedSuccHash->getLinkedRoleSuccessorHash();
					CSaturationIndividualNodeSuccessorExtensionData* indiProcSatNodeSuccExt = indiProcSatNode->getSuccessorExtensionData();
					CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* indiProcSatNodeFunctionalConSuccExt = indiProcSatNodeSuccExt->getFUNCTIONALConceptsExtensionData(true);
					if (succData) {
						CSaturationPredecessorFUNCTIONALConceptExtensionData* predRoleFuncConExtData = indiProcSatNodeFunctionalConSuccExt->getPredecessorFUNCTIONALConceptsExtensionData(role,true);
						CSaturationSuccessorData* lastLinkedSucc = succData->getLastSuccessorLinkData();

						CSaturationSuccessorData* activeLinkedSucc = nullptr;						
						for (CSaturationSuccessorData* linkedSuccIt = lastLinkedSucc; linkedSuccIt && !activeLinkedSucc; linkedSuccIt = linkedSuccIt->mNextLink) {
							if (linkedSuccIt->mActiveCount >= 1) {
								activeLinkedSucc = linkedSuccIt;
							}
						}

						if (activeLinkedSucc) {
							CIndividualSaturationProcessNode* succIndiNode = activeLinkedSucc->mSuccIndiNode;
							if (succIndiNode) {
								for (CBackwardSaturationPropagationLink* backPropLinkLinkerIt = backPropLinkLinker; backPropLinkLinkerIt; backPropLinkLinkerIt = backPropLinkLinkerIt->getNext()) {
									CIndividualSaturationProcessNode* predAncIndiNode = backPropLinkLinkerIt->getSourceIndividual();
									if (succIndiNode != predAncIndiNode) {
										updated |= createAncestorSuccessorMergingExtension(indiProcSatNode,role,succIndiNode,predAncIndiNode,activeLinkedSucc->mCreationRoleLinker,calcAlgContext);
									}
								}
							}
						}
					}
					return updated;
				}































				CRole* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getInverseRole(CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					CRole* invRole = role->getInverseRole();
					if (!invRole) {
						for (CSortedNegLinker<CRole*>* invEqRoleLinkerIt = role->getInverseEquivalentRoleList(); !invRole && invEqRoleLinkerIt; invEqRoleLinkerIt = invEqRoleLinkerIt->getNext()) {
							if (invEqRoleLinkerIt->isNegated()) {
								invRole = invEqRoleLinkerIt->getData();
							}
						}
					}
					if (!invRole) {
						for (CSortedNegLinker<CRole*>* invSuperRoleLinkerIt = role->getIndirectSuperRoleList(); !invRole && invSuperRoleLinkerIt; invSuperRoleLinkerIt = invSuperRoleLinkerIt->getNext()) {
							if (invSuperRoleLinkerIt->isNegated()) {
								CRole* invSuperRole = invSuperRoleLinkerIt->getData();
								for (CSortedNegLinker<CRole*>* superSuperRoleLinkerIt = invSuperRole->getIndirectSuperRoleList(); !invRole && superSuperRoleLinkerIt; superSuperRoleLinkerIt = superSuperRoleLinkerIt->getNext()) {
									if (superSuperRoleLinkerIt->isNegated() && superSuperRoleLinkerIt->getData() == role) {
										invRole = invSuperRole;
									}
								}
							}
						}
					}
					return invRole;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createAncestorSuccessorMergingExtension(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CIndividualSaturationProcessNode* succSatNode, CIndividualSaturationProcessNode* ancSatNode, CXNegLinker<CRole*>* creationRoleLinker, CCalculationAlgorithmContextBase* calcAlgContext) {


					CSaturationSuccessorData* ancSuccLinkData = nullptr;
					collectLinkedSuccessorNodes(ancSatNode,calcAlgContext);

					CRole* invRole = getInverseRole(role,calcAlgContext);

					CLinkedRoleSaturationSuccessorData* ancSuccData = nullptr;
					CLinkedRoleSaturationSuccessorHash* ancLinkedSuccHash = ancSatNode->getLinkedRoleSuccessorHash(true);
					if (ancLinkedSuccHash) {
						ancSuccData = ancLinkedSuccHash->getLinkedRoleSuccessorData(invRole,false);
						if (ancSuccData) {
							ancSuccLinkData = ancSuccData->mSuccNodeDataMap.value(indiProcSatNode->getIndividualID());
						}
					}

					CXNegLinker<CRole*>* ancSuccCreationRoleLinker = nullptr;
					if (ancSuccLinkData && ancSuccLinkData->mActiveCount >= 1) {
						ancSuccCreationRoleLinker = ancSuccLinkData->mCreationRoleLinker;


						bool updated = false;

						//if (mConfDebuggingWriteData && mConfDebuggingWriteDataSaturationTasks && !mWroteFunctionalSuccPredMergingDebugString) {
						//	mWroteFunctionalSuccPredMergingDebugString = true;
						//	mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
						//	QString fileName("./Debugging/SaturationTasks/saturation-model-fun-succ-pred-merged.txt");
						//	QFile tmpFile(fileName);
						//	if (tmpFile.open(QIODevice::WriteOnly)) {
						//		tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
						//		tmpFile.close();
						//	}
						//}

						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* funcConExtData = succSatNode->getSuccessorExtensionData(true)->getFUNCTIONALConceptsExtensionData(true);
						if (!funcConExtData->hasIndividualNodeForwardingPredecessorMerged(ancSatNode)) {


							CReapplyConceptSaturationLabelSet* succConSet = succSatNode->getReapplyConceptSaturationLabelSet(false);
							if (succConSet) {
								for (CConceptSaturationDescriptor* conSatDesIt = succConSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
									CConcept* concept = conSatDesIt->getConcept();
									bool negation = conSatDesIt->isNegated();
									addConceptFilteredToIndividual(concept,negation,ancSatNode,calcAlgContext);
								}
							}

							CXNegLinker<CIndividualSaturationProcessNode*>* depCopyLinker = CObjectAllocator< CXNegLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
							depCopyLinker->initNegLinker(ancSatNode,false);
							succSatNode->addCopyDependingIndividualNodeLinker(depCopyLinker);

							preprocessResolvedIndividualNode(ancSatNode,calcAlgContext);
						}



						for (CXNegLinker<CRole*>* creationRoleLinkerIt = creationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
							if (!creationRoleLinkerIt->isNegated()) {
								CRole* creationRole = creationRoleLinkerIt->getData();

								if (!funcConExtData->hasIndividualNodeForwardingPredecessorMerged(ancSatNode,creationRole)) {
									updated = true;
									funcConExtData->setIndividualNodeForwardingPredecessorMerged(ancSatNode,creationRole);

									CRole* invCreationRole = getInverseRole(creationRole,calcAlgContext);
									if (invCreationRole && !ancLinkedSuccHash->hasActiveLinkedSuccessor(invCreationRole,indiProcSatNode)) {
										for (CSortedNegLinker<CRole*>* invCreationSuperRoleIt = invCreationRole->getIndirectSuperRoleList(); invCreationSuperRoleIt; invCreationSuperRoleIt = invCreationSuperRoleIt->getNext()) {
											CRole* creationSuperRole = invCreationSuperRoleIt->getData();
											if (!invCreationSuperRoleIt->isNegated()) {
												ancLinkedSuccHash->addExtensionSuccessor(creationSuperRole,indiProcSatNode,invCreationRole,1);
												//ancLinkedSuccHash->setSuccessorMergedCreation(creationSuperRole,indiProcSatNode);
												addNewLinkedExtensionProcessingRole(creationSuperRole,ancSatNode,true,true,calcAlgContext);
											} else {
												CBackwardSaturationPropagationLink* backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
												backPropLink->initBackwardPropagationLink(ancSatNode,creationSuperRole);

												installBackwardPropagationLink(ancSatNode,indiProcSatNode,creationSuperRole,backPropLink,true,false,calcAlgContext);
											}
										}
									}
								}
							}
						}


						return updated;
					}

					return false;
				}










































				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isLinkedIndividualSuccessorNodeMergingSubset(CIndividualSaturationProcessNode*& indiProcSatNode, CSaturationSuccessorData* subsetIndiSuccData, CSaturationSuccessorData* superIndiSuccData, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* subsetIndiSuccNode = subsetIndiSuccData->mSuccIndiNode;
					CIndividualSaturationProcessNode* superIndiSuccNode = superIndiSuccData->mSuccIndiNode;
					return isLinkedIndividualSuccessorNodeMergingSubset(indiProcSatNode,subsetIndiSuccNode,subsetIndiSuccData,superIndiSuccNode,superIndiSuccData,role,calcAlgContext);
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isLinkedIndividualSuccessorNodeMergingSubset(CIndividualSaturationProcessNode*& indiProcSatNode, CIndividualSaturationProcessNode* subsetIndiSuccNode, CSaturationSuccessorData* subsetIndiSuccData, CIndividualSaturationProcessNode* superIndiSuccNode, CSaturationSuccessorData* superIndiSuccData, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (subsetIndiSuccData->mVALUENominalConnection || superIndiSuccData->mVALUENominalConnection) {
						return false;
					}
					if (subsetIndiSuccNode->hasNominalIntegrated()) {
						return false;
					}
					if (subsetIndiSuccNode->hasDataValueApplied()) {
						return false;
					}
					if (superIndiSuccData->mActiveCount <= 0) {
						return false;
					}
					if (subsetIndiSuccData->mSuccCount > superIndiSuccData->mSuccCount) {
						return false;
					}
					if (!isSuccessorCreationRoleMergingSubset(role,superIndiSuccData->mCreationRoleLinker,calcAlgContext)) {
						return false;
					}
					if (!isIndividualNodeLabelMergingSubset(subsetIndiSuccNode,superIndiSuccNode,false,calcAlgContext)) {
						return false;
					}
					return true;
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isSuccessorCreationRoleMergingSubset(CXNegLinker<CRole*>* subCreationRoleLinker, CXNegLinker<CRole*>* superCreationRoleLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXNegLinker<CRole*>* subCreationRoleLinkerIt = superCreationRoleLinker; subCreationRoleLinkerIt; subCreationRoleLinkerIt = subCreationRoleLinkerIt->getNext()) {
						if (!subCreationRoleLinkerIt->isNegated()) {
							CRole* subCreationRole = subCreationRoleLinkerIt->getData();
							if (!isSuccessorCreationRoleMergingSubset(subCreationRole,superCreationRoleLinker,calcAlgContext)) {
								return false;
							}
						}
					}
					return true;
				}

				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isSuccessorCreationRoleMergingSubset(CRole* subCreationRole, CXNegLinker<CRole*>* superCreationRoleLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXNegLinker<CRole*>* superCreationRoleLinkerIt = superCreationRoleLinker; superCreationRoleLinkerIt; superCreationRoleLinkerIt = superCreationRoleLinkerIt->getNext()) {
						if (!superCreationRoleLinkerIt->isNegated()) {
							if (superCreationRoleLinkerIt->getData() == subCreationRole) {
								return true;
							}
						}
					}
					return false;
				}

				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isIndividualNodeLabelMergingSubset(CIndividualSaturationProcessNode* subsetIndiSuccNode, CIndividualSaturationProcessNode* superIndiSuccNode, bool ignoreANDConcepts, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* subsetConSet = subsetIndiSuccNode->getReapplyConceptSaturationLabelSet(false);
					CReapplyConceptSaturationLabelSet* superConSet = superIndiSuccNode->getReapplyConceptSaturationLabelSet(false);
					if (!superConSet && subsetConSet) {
						return false;
					}
					if (subsetConSet && superConSet) {
						if (subsetConSet->getConceptCount() <= superConSet->getConceptCount()) {
							for (CConceptSaturationDescriptor* conDesIt = subsetConSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
								CConcept* concept = conDesIt->getConcept();
								bool negation = conDesIt->isNegated();
								cint64 conCode = concept->getOperatorCode();
								if (!ignoreANDConcepts || (!negation && (conCode != CCAND && conCode != CCAQAND && conCode != CCIMPLAQAND && conCode != CCBRANCHAQAND)) || (negation && (conCode != CCOR))) {
									if (!superConSet->containsConcept(concept,negation)) {
										return false;
									}
								}
							}
						} else {
							return false;
						}
					}
					return true;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::deactivateSubsetMergeableSuccessorLinks(CIndividualSaturationProcessNode*& indiProcSatNode, CLinkedRoleSaturationSuccessorHash* linkedSuccHash, CPROCESSMAP<cint64,CSaturationSuccessorData*>* succDataMap, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool linksDeactivated = false;
					cint64 removedSuccCardCount = 0;
					for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator succDataIt = succDataMap->constBegin(), succDataItEnd = succDataMap->constEnd(); succDataIt != succDataItEnd; ++succDataIt) {
						cint64 indiID = succDataIt.key();
						CSaturationSuccessorData* succLinkData = succDataIt.value();
						if (succLinkData->mActiveCount > 0) {
							cint64 succCard = succLinkData->mSuccCount;

							bool nodeMergeable = true;
							if (succLinkData->mSuccIndiNode && (succLinkData->mSuccIndiNode->hasDataValueApplied() || succLinkData->mSuccIndiNode->hasNominalIntegrated())) {
								nodeMergeable = false;
							}
							if (succLinkData->mVALUENominalConnection) {
								nodeMergeable = false;
							}

							if (nodeMergeable) {
								for (CXNegLinker<CRole*>* creationRoleLinkerIt = succLinkData->mCreationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
									if (!creationRoleLinkerIt->isNegated()) {
										CRole* creationRole = creationRoleLinkerIt->getData();

										bool deactivateLink = false;
										for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator mergeSuccDataIt = succDataMap->constBegin(); !deactivateLink && mergeSuccDataIt != succDataItEnd; ++mergeSuccDataIt) {
											cint64 mergeIndiID = mergeSuccDataIt.key();
											if (indiID != mergeIndiID) {
												CSaturationSuccessorData* mergeSuccLinkData = mergeSuccDataIt.value();
												if (mergeSuccLinkData->mActiveCount > 0) {
													if (isLinkedIndividualSuccessorNodeMergingSubset(indiProcSatNode,succLinkData,mergeSuccLinkData,creationRole,calcAlgContext)) {
														deactivateLink = true;
													}
												}
											}
										}
										if (deactivateLink) {
											for (CSortedNegLinker<CRole*>* creationSuperRoleIt = creationRole->getIndirectSuperRoleList(); creationSuperRoleIt; creationSuperRoleIt = creationSuperRoleIt->getNext()) {
												CRole* creationSuperRole = creationSuperRoleIt->getData();
												if (!creationSuperRoleIt->isNegated()) {
													linkedSuccHash->deactivateLinkedSuccessor(creationSuperRole,succLinkData->mSuccIndiNode,creationRole);
												}
											}
										}
									}
								}
							}

							if (succLinkData->mActiveCount <= 0) {
								removedSuccCardCount += succCard;
							}
						}
					}
					return linksDeactivated;
				}








































				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateSuccessorRoleFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CLinkedRoleSaturationSuccessorData* succData, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool updated = false;
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succDataHash = linkedSuccHash->getLinkedRoleSuccessorHash();
					CSaturationIndividualNodeSuccessorExtensionData* indiProcSatNodeSuccExt = indiProcSatNode->getSuccessorExtensionData();
					CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* indiProcSatNodeFunctionalConSuccExt = indiProcSatNodeSuccExt->getFUNCTIONALConceptsExtensionData(true);
					if (succData) {
						CSaturationSuccessorFUNCTIONALConceptExtensionData* roleFuncConExtData = indiProcSatNodeFunctionalConSuccExt->getSuccessorFUNCTIONALConceptsExtensionData(role,true);
						CSaturationSuccessorData* lastExaminedLinkedSucc = roleFuncConExtData->getLastExaminedLinkedSuccessorData();
						CSaturationSuccessorData* lastLinkedSucc = succData->getLastSuccessorLinkData();
						if (lastLinkedSucc != lastExaminedLinkedSucc) {

							CPROCESSMAP<cint64,CSaturationSuccessorData*>* succDataMap = succData->getSuccessorNodeDataMap(false);
							deactivateSubsetMergeableSuccessorLinks(indiProcSatNode,linkedSuccHash,succDataMap,role,calcAlgContext);

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
								CSaturationIndividualNodeExtensionResolveData* resolveData = nullptr; 


								CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinker = nullptr;

								CSaturationSuccessorData* maxLabelResolveIndiLinkedSuccData = nullptr;
								cint64 maxLabelResolveIndiConceptCount = 0;

								// resolve new individual node
								for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succDataMap->constBegin(), itEnd = succDataMap->constEnd(); it != itEnd; ++it) {
									cint64 succID = it.key();
									CSaturationSuccessorData* linkedSuccData = it.value();
									if (linkedSuccData->mActiveCount >= 1 && !linkedSuccData->mVALUENominalConnection) {

										cint64 succLabelConCount = linkedSuccData->mSuccIndiNode->getReapplyConceptSaturationLabelSet(false)->getConceptCount();
										if (!maxLabelResolveIndiLinkedSuccData && succLabelConCount > maxLabelResolveIndiConceptCount) {
											maxLabelResolveIndiConceptCount = succLabelConCount;
											maxLabelResolveIndiLinkedSuccData = linkedSuccData;
										}

										CIndividualSaturationSuccessorLinkDataLinker* tmpMergingSuccDataLinker = createIndividualSaturationSuccessorLinkDataLinker(calcAlgContext);
										tmpMergingSuccDataLinker->initSuccessorLinkDataLinker(linkedSuccData);
										mergingSuccDataLinker = tmpMergingSuccDataLinker->append(mergingSuccDataLinker);
									}
								}


								CPROCESSINGHASH<cint64,CConceptNegationPair>* conExtMap = nullptr;

								CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker;
								CSaturationSuccessorData* resolveLinkedSuccData = maxLabelResolveIndiLinkedSuccData;
								copyIndiProcSatNode = resolveLinkedSuccData->mSuccIndiNode;
								
								for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker; mergingSuccDataLinkerIt; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
									CSaturationSuccessorData* linkedSuccData = mergingSuccDataLinkerIt->getData();
									if (linkedSuccData != resolveLinkedSuccData) {
										CIndividualSaturationProcessNode* succNode = linkedSuccData->mSuccIndiNode;

										collectResolveIndividualExtendableConceptMap(copyIndiProcSatNode,succNode,conExtMap,calcAlgContext);
									}
								}
								releaseIndividualSaturationSuccessorLinkDataLinker(mergingSuccDataLinker,calcAlgContext);

								resolveData = copyIndiProcSatNode->getSuccessorExtensionData(true)->getBaseExtensionResolveData(true);
								resolveData = getResolvedIndividualNodeExtension(resolveData,conExtMap,copyIndiProcSatNode,calcAlgContext);



								CIndividualSaturationProcessNode* resolvedIndiNode = resolveData->getProcessingIndividualNode();
								roleFuncConExtData->setLastResolvedIndividualNode(resolvedIndiNode);

								bool backwardLinkConnected = false;
								bool connectionAlreadyExist = false;
								// deactivate previous successor connections, activate new ones
								for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succDataMap->constBegin(), itEnd = succDataMap->constEnd(); it != itEnd; ++it) {
									cint64 succID = it.key();
									CSaturationSuccessorData* linkedSuccData = it.value();
									if (linkedSuccData->mActiveCount >= 1 && !linkedSuccData->mVALUENominalConnection) {
										cint64 linkSuccCount = linkedSuccData->mSuccCount;
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
															linkedSuccHash->addExtensionSuccessor(creationSuperRole,resolvedIndiNode,creationRole,1);
															//linkedSuccHash->setSuccessorMergedCreation(creationSuperRole,resolvedIndiNode);
														}
													} else {

														if (makeNewSuccessorConnections) {
															backwardLinkConnected = true;
															CBackwardSaturationPropagationLink* backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
															backPropLink->initBackwardPropagationLink(indiProcSatNode,creationSuperRole);

															installBackwardPropagationLink(indiProcSatNode,resolvedIndiNode,creationSuperRole,backPropLink,true,true,calcAlgContext);

														}

													}

												}

											}
										}
									}
								}
								//linkedSuccHash->setSuccessorMergedCreation(role,resolvedIndiNode);

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
								updateAddingSuccessorConnectedNominal(indiProcSatNode,resolvedIndiNode->getSuccessorConnectedNominalSet(false),mCalcAlgContext);
								updateMaxCardinalityCandidates(indiProcSatNode,resolvedIndiNode->getMaxAtleastCardinalityCandidate(),resolvedIndiNode->getMaxAtmostCardinalityCandidate(),mCalcAlgContext);

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






				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateSuccessorRoleQualifiedFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CSortedNegLinker<CConcept*>* qualifiyConLinker, CLinkedRoleSaturationSuccessorData* succData, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool updated = false;
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succDataHash = linkedSuccHash->getLinkedRoleSuccessorHash();
					CSaturationIndividualNodeSuccessorExtensionData* indiProcSatNodeSuccExt = indiProcSatNode->getSuccessorExtensionData();
					CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* indiProcSatNodeFunctionalConSuccExt = indiProcSatNodeSuccExt->getFUNCTIONALConceptsExtensionData(true);
					if (succData) {
						CPROCESSMAP<cint64,CSaturationSuccessorData*>* succDataMap = succData->getSuccessorNodeDataMap(false);

						cint64 succCount = 0;
						cint64 maxSuccCardinality = CINT64_MIN;
						CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinker = nullptr;
						CSaturationSuccessorData* maxLabelResolveIndiLinkedSuccData = nullptr;
						cint64 maxLabelResolveIndiConceptCount = 0;

						for (CSaturationSuccessorData* lastLinkedSuccIt = succData->getLastSuccessorLinkData(); lastLinkedSuccIt; lastLinkedSuccIt = lastLinkedSuccIt->mNextLink) {
							if (lastLinkedSuccIt->mActiveCount >= 1 && !lastLinkedSuccIt->mVALUENominalConnection) {
								CReapplyConceptSaturationLabelSet* succConSet = lastLinkedSuccIt->mSuccIndiNode->getReapplyConceptSaturationLabelSet(false);
								bool containsQualification = false;
								for (CSortedNegLinker<CConcept*>* qualifiyConLinkerIt = qualifiyConLinker; !containsQualification && qualifiyConLinkerIt; qualifiyConLinkerIt = qualifiyConLinkerIt->getNext()) {
									CConcept* qualCon = qualifiyConLinkerIt->getData();
									bool qualNegation = qualifiyConLinkerIt->isNegated();
									if (succConSet->containsConcept(qualCon,qualNegation)) {
										containsQualification = true;
									}
								}
								if (containsQualification) {
									succCount++;
									maxSuccCardinality = qMax(maxSuccCardinality,lastLinkedSuccIt->mSuccCount);

									cint64 succLabelConCount = succConSet->getConceptCount();
									if (!maxLabelResolveIndiLinkedSuccData && succLabelConCount > maxLabelResolveIndiConceptCount) {
										maxLabelResolveIndiConceptCount = succLabelConCount;
										maxLabelResolveIndiLinkedSuccData = lastLinkedSuccIt;
									}

									CIndividualSaturationSuccessorLinkDataLinker* tmpMergingSuccDataLinker = createIndividualSaturationSuccessorLinkDataLinker(calcAlgContext);
									tmpMergingSuccDataLinker->initSuccessorLinkDataLinker(lastLinkedSuccIt);
									mergingSuccDataLinker = tmpMergingSuccDataLinker->append(mergingSuccDataLinker);

								}
							}
						}
						if (succCount > 1 && maxSuccCardinality <= 1) {


							CIndividualSaturationProcessNode* copyIndiProcSatNode = nullptr;
							CSaturationIndividualNodeExtensionResolveData* resolveData = nullptr; 


							CPROCESSINGHASH<cint64,CConceptNegationPair>* conExtMap = nullptr;

							CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker;
							CSaturationSuccessorData* resolveLinkedSuccData = maxLabelResolveIndiLinkedSuccData;
							copyIndiProcSatNode = resolveLinkedSuccData->mSuccIndiNode;

							for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker; mergingSuccDataLinkerIt; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
								CSaturationSuccessorData* linkedSuccData = mergingSuccDataLinkerIt->getData();
								if (linkedSuccData != resolveLinkedSuccData) {
									CIndividualSaturationProcessNode* succNode = linkedSuccData->mSuccIndiNode;

									collectResolveIndividualExtendableConceptMap(copyIndiProcSatNode,succNode,conExtMap,calcAlgContext);
								}
							}

							resolveData = copyIndiProcSatNode->getSuccessorExtensionData(true)->getBaseExtensionResolveData(true);
							resolveData = getResolvedIndividualNodeExtension(resolveData,conExtMap,copyIndiProcSatNode,calcAlgContext);



							CIndividualSaturationProcessNode* resolvedIndiNode = resolveData->getProcessingIndividualNode();

							bool backwardLinkConnected = false;
							bool connectionAlreadyExist = false;
							// deactivate previous successor connections, activate new ones
							for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succDataMap->constBegin(), itEnd = succDataMap->constEnd(); it != itEnd; ++it) {
								cint64 succID = it.key();
								CSaturationSuccessorData* linkedSuccData = it.value();
								if (linkedSuccData->mActiveCount >= 1 && !linkedSuccData->mVALUENominalConnection) {
									cint64 linkSuccCount = linkedSuccData->mSuccCount;
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
														linkedSuccHash->addExtensionSuccessor(creationSuperRole,resolvedIndiNode,creationRole,1);
														//linkedSuccHash->setSuccessorMergedCreation(creationSuperRole,resolvedIndiNode);
													}
												} else {

													if (makeNewSuccessorConnections) {
														backwardLinkConnected = true;
														CBackwardSaturationPropagationLink* backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
														backPropLink->initBackwardPropagationLink(indiProcSatNode,creationSuperRole);

														installBackwardPropagationLink(indiProcSatNode,resolvedIndiNode,creationSuperRole,backPropLink,true,true,calcAlgContext);

													}

												}

											}

										}
									}
								}
							}

							updateIndirectAddingIndividualStatusFlags(indiProcSatNode,resolvedIndiNode->getIndirectStatusFlags(),mCalcAlgContext);
							updateAddingSuccessorConnectedNominal(indiProcSatNode,resolvedIndiNode->getSuccessorConnectedNominalSet(false),mCalcAlgContext);
							updateMaxCardinalityCandidates(indiProcSatNode,resolvedIndiNode->getMaxAtleastCardinalityCandidate(),resolvedIndiNode->getMaxAtmostCardinalityCandidate(),mCalcAlgContext);


							updated = true;
						}
						releaseIndividualSaturationSuccessorLinkDataLinker(mergingSuccDataLinker,calcAlgContext);

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
												addNewLinkedExtensionProcessingRole(superRole,indiProcSatNode,false,true,calcAlgContext);

											} else {

												if (!onlySuccessorCardinalityUpdated) {
													backwardLinkConnected = true;
													CBackwardSaturationPropagationLink* backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
													backPropLink->initBackwardPropagationLink(indiProcSatNode,superRole);

													installBackwardPropagationLink(indiProcSatNode,resolvedIndiNode,superRole,backPropLink,true,true,calcAlgContext);

												}

											}
										}
										updateIndirectAddingIndividualStatusFlags(indiProcSatNode,resolvedIndiNode->getIndirectStatusFlags(),mCalcAlgContext);
										updateAddingSuccessorConnectedNominal(indiProcSatNode,resolvedIndiNode->getSuccessorConnectedNominalSet(false),mCalcAlgContext);
										updateMaxCardinalityCandidates(indiProcSatNode,resolvedIndiNode->getMaxAtleastCardinalityCandidate(),resolvedIndiNode->getMaxAtmostCardinalityCandidate(),mCalcAlgContext);
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







				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::installBackwardPropagationLink(CIndividualSaturationProcessNode* sourceIndiProcSatNode, CIndividualSaturationProcessNode* destIndiProcSatNode, CRole* role, CBackwardSaturationPropagationLink* link, bool applyBackPropDes, bool queueFunctionalProcessing, CCalculationAlgorithmContextBase* calcAlgContext) {

					CRoleBackwardSaturationPropagationHash* resolvedIndiBackPropHash = destIndiProcSatNode->getRoleBackwardPropagationHash(true);
					CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropDataHash = resolvedIndiBackPropHash->getRoleBackwardPropagationDataHash();
					CRoleBackwardSaturationPropagationHashData& backPropData = (*backPropDataHash)[role];
					bool installLink = true;
					bool linkInstalled = false;
					if (backPropData.mLinkLinker) {
						if (backPropData.mLinkLinker->getSourceIndividual() == link->getSourceIndividual()) {
							installLink = false;
						}
					}
					if (installLink) {
						linkInstalled = true;
						backPropData.mLinkLinker = link->append(backPropData.mLinkLinker);
						CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDes = backPropData.mReapplyLinker;
						if (backPropReapplyDes && applyBackPropDes) {
							applyBackwardPropagationConcepts(sourceIndiProcSatNode,backPropReapplyDes,calcAlgContext);
						}
					}
					if (backPropData.mRolePredecessorMergingQueuingRequired && queueFunctionalProcessing) {
						if (!backPropData.mRolePredecessorMergingProcessingQueued) {
							backPropData.mRolePredecessorMergingProcessingQueued = true;

							addSuccessorExtensionToProcessingQueue(destIndiProcSatNode,calcAlgContext);
							CSaturationIndividualNodeSuccessorExtensionData* succExtData = destIndiProcSatNode->getSuccessorExtensionData(false);
							if (succExtData) {
								CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* succIndiFUNCTIONALConExtData = succExtData->getFUNCTIONALConceptsExtensionData(false);
								if (succIndiFUNCTIONALConExtData) {
									if (succIndiFUNCTIONALConExtData->isSuccessorExtensionInitialized()) {
										if (!succIndiFUNCTIONALConExtData->hasLinkedPredecessorAddedProcessLinkerForRole(role)) {
											CRoleSaturationProcessLinker* roleProcessLinker = createRoleSaturationProcessLinker(calcAlgContext);
											roleProcessLinker->initRoleProcessLinker(role);
											succIndiFUNCTIONALConExtData->addLinkedPredecessorAddedRoleProcessLinker(roleProcessLinker);
										}
									}
								}
							}
						}
					}
					return linkInstalled;

				}





				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::initializeIndividualNodeByCoping(CIndividualSaturationProcessNode* indiProcSatNode, CIndividualSaturationProcessNode* copyFromIndiProcSatNode, bool tryFlatLabelCopy, CCalculationAlgorithmContextBase* calcAlgContext) {
					++mCopiedIndiNodeCount;
					indiProcSatNode->initCopingIndividualSaturationProcessNode(copyFromIndiProcSatNode,tryFlatLabelCopy);
					indiProcSatNode->setReferenceMode(2);
					indiProcSatNode->setCopyIndividualNode(copyFromIndiProcSatNode);
					indiProcSatNode->setReferenceIndividualSaturationProcessNode(copyFromIndiProcSatNode);

					updateDirectAddingIndividualStatusFlags(indiProcSatNode,copyFromIndiProcSatNode->getDirectStatusFlags(),mCalcAlgContext);							
					updateIndirectAddingIndividualStatusFlags(indiProcSatNode,copyFromIndiProcSatNode->getIndirectStatusFlags(),mCalcAlgContext);							
					updateAddingSuccessorConnectedNominal(indiProcSatNode,copyFromIndiProcSatNode->getSuccessorConnectedNominalSet(false),mCalcAlgContext);
					updateMaxCardinalityCandidates(indiProcSatNode,copyFromIndiProcSatNode->getMaxAtleastCardinalityCandidate(),copyFromIndiProcSatNode->getMaxAtmostCardinalityCandidate(),mCalcAlgContext);


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




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::preprocessResolvedIndividualNode(CIndividualSaturationProcessNode* resolvedIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool processed = false;
					CConceptSaturationProcessLinker* conceptSaturationProcessLinker = resolvedIndiProcSatNode->takeConceptSaturationProcessLinker();
					while (conceptSaturationProcessLinker) {
						STATINC(RULEAPPLICATIONCOUNT,calcAlgContext);
						KONCLUCE_TASK_ALGORITHM_SATURATION_SATURATION_MODEL_STRING_INSTRUCTION(mRuleBeginDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
						applyTableauSaturationRule(resolvedIndiProcSatNode,conceptSaturationProcessLinker);
						KONCLUCE_TASK_ALGORITHM_SATURATION_SATURATION_MODEL_STRING_INSTRUCTION(mRuleEndDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext));
						releaseConceptSaturationProcessLinker(conceptSaturationProcessLinker,calcAlgContext);
						conceptSaturationProcessLinker = resolvedIndiProcSatNode->takeConceptSaturationProcessLinker();
						processed = true;
					}
					return processed;
				}




				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeRepresentativeRangeAssertion(CIndividualSaturationProcessNode* indiProcSatNode, CIndividual* nominalIndi, CRole* role, bool inversedRole, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* resolvedNode = nullptr;
					CSaturationIndividualNodeExtensionResolveData* resolveData = indiProcSatNode->getSuccessorExtensionData(true)->getBaseExtensionResolveData(true);
					CIndividualSaturationProcessNode* copyIndiProcSatNode = indiProcSatNode;
					CConceptAssertionLinker* conAssLinker = nominalIndi->getAssertionConceptLinker();
					CConcept* topConcept = calcAlgContext->getProcessingDataBox()->getOntologyTopConcept();
					CConcept* nominalConcept = nominalIndi->getIndividualNominalConcept();
					// resolve individual
					for (CConceptAssertionLinker* conAssLinkerIt = conAssLinker; conAssLinkerIt; conAssLinkerIt = conAssLinkerIt->getNext()) {
						CConcept* concept = conAssLinkerIt->getData();
						bool negated = conAssLinkerIt->isNegated();

						if (concept == nominalConcept) {
							if (negated) {
								resolveData = getResolvedIndividualNodeExtension(resolveData,topConcept,true,copyIndiProcSatNode,calcAlgContext);
							}
						} else {
							if (concept->getOperatorCode() == CCNOMINAL) {
								if (concept->getNominalIndividual() == nominalIndi) {
									if (negated) {
										resolveData = getResolvedIndividualNodeExtension(resolveData,topConcept,true,copyIndiProcSatNode,calcAlgContext);
									}
								} else if (!negated) {
									resolveData = getResolvedIndividualNodeExtension(resolveData,concept,negated,copyIndiProcSatNode,calcAlgContext);
								}
							} else {
								resolveData = getResolvedIndividualNodeExtension(resolveData,concept,negated,copyIndiProcSatNode,calcAlgContext);
							}
						}
					}

					for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
						CRole* superRole = superRoleIt->getData();
						CSortedNegLinker<CConcept*>* rangeConLinker = superRole->getRelativeRangeConceptList(superRoleIt->isNegated()^inversedRole);
						for (CSortedNegLinker<CConcept*>* rangeConLinkerIt = rangeConLinker; rangeConLinkerIt; rangeConLinkerIt = rangeConLinkerIt->getNext()) {
							CConcept* rangeConcept = rangeConLinkerIt->getData();
							bool rangeConceptNegation = rangeConLinkerIt->isNegated();
							resolveData = getResolvedIndividualNodeExtension(resolveData,rangeConcept,rangeConceptNegation,copyIndiProcSatNode,calcAlgContext);
						}
					}

					resolveData = getResolvedNeighbourIndividualNodeExtension(resolveData, copyIndiProcSatNode, calcAlgContext);


					if (!resolveData->hasProcessingIndividualNode()) {
						// create individual
						resolvedNode = createResolvedIndividualNode(resolveData,copyIndiProcSatNode,true,calcAlgContext);
						resolvedNode->setABoxIndividualRepresentationNode(true);
						resolvedNode->setSeparated(indiProcSatNode->isSeparated());
						CReapplyConceptSaturationLabelSet* conSet = resolvedNode->getReapplyConceptSaturationLabelSet(true);


						// add all concepts to individual
						for (CConceptAssertionLinker* conAssLinkerIt = conAssLinker; conAssLinkerIt; conAssLinkerIt = conAssLinkerIt->getNext()) {
							CConcept* concept = conAssLinkerIt->getData();
							bool negated = conAssLinkerIt->isNegated();
							if (concept == nominalConcept) {
								if (negated) {
									addConceptFilteredToIndividual(topConcept,true,resolvedNode,calcAlgContext);
								}
							} else {
								if (concept->getOperatorCode() == CCNOMINAL) {
									if (concept->getNominalIndividual() == nominalIndi) {
										if (negated) {
											addConceptFilteredToIndividual(topConcept,true,resolvedNode,calcAlgContext);
										}
									} else if (!negated) {
										addConceptFilteredToIndividual(concept,negated,resolvedNode,calcAlgContext);
									}
								} else {
									addConceptFilteredToIndividual(concept,negated,resolvedNode,calcAlgContext);
								}
							}
						}



						for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
							CRole* superRole = superRoleIt->getData();
							CSortedNegLinker<CConcept*>* rangeConLinker = superRole->getRelativeRangeConceptList(superRoleIt->isNegated()^inversedRole);
							for (CSortedNegLinker<CConcept*>* rangeConLinkerIt = rangeConLinker; rangeConLinkerIt; rangeConLinkerIt = rangeConLinkerIt->getNext()) {
								CConcept* rangeConcept = rangeConLinkerIt->getData();
								bool rangeConceptNegation = rangeConLinkerIt->isNegated();
								addConceptFilteredToIndividual(rangeConcept,rangeConceptNegation,resolvedNode,calcAlgContext);
							}
						}

						preprocessResolvedIndividualNode(resolvedNode,calcAlgContext);

					}
					resolvedNode = resolveData->getProcessingIndividualNode();
					return resolvedNode;
				}





				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeRepresentativeAssertion(CIndividualSaturationProcessNode* indiProcSatNode, CIndividual* nominalIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* resolvedNode = nullptr;
					CSaturationIndividualNodeExtensionResolveData* resolveData = indiProcSatNode->getSuccessorExtensionData(true)->getBaseExtensionResolveData(true);
					CIndividualSaturationProcessNode* copyIndiProcSatNode = indiProcSatNode;
					CConceptAssertionLinker* conAssLinker = nominalIndi->getAssertionConceptLinker();
					CConcept* topConcept = calcAlgContext->getProcessingDataBox()->getOntologyTopConcept();
					CConcept* nominalConcept = nominalIndi->getIndividualNominalConcept();
					// resolve individual
					for (CConceptAssertionLinker* conAssLinkerIt = conAssLinker; conAssLinkerIt; conAssLinkerIt = conAssLinkerIt->getNext()) {
						CConcept* concept = conAssLinkerIt->getData();
						bool negated = conAssLinkerIt->isNegated();

						if (concept == nominalConcept) {
							if (negated) {
								resolveData = getResolvedIndividualNodeExtension(resolveData,topConcept,true,copyIndiProcSatNode,calcAlgContext);
							}
						} else {
							if (concept->getOperatorCode() == CCNOMINAL) {
								if (concept->getNominalIndividual() == nominalIndi) {
									if (negated) {
										resolveData = getResolvedIndividualNodeExtension(resolveData,topConcept,true,copyIndiProcSatNode,calcAlgContext);
									}
								} else {
									resolveData = getResolvedIndividualNodeExtension(resolveData,concept,negated,copyIndiProcSatNode,calcAlgContext);
								}
							} else {
								resolveData = getResolvedIndividualNodeExtension(resolveData,concept,negated,copyIndiProcSatNode,calcAlgContext);
							}
						}
					}

					resolveData = getResolvedNeighbourIndividualNodeExtension(resolveData, copyIndiProcSatNode, calcAlgContext);

					if (!resolveData->hasProcessingIndividualNode()) {
						// create individual
						resolvedNode = createResolvedIndividualNode(resolveData,copyIndiProcSatNode,true,calcAlgContext);
						resolvedNode->setABoxIndividualRepresentationNode(true);
						resolvedNode->setSeparated(indiProcSatNode->isSeparated());
						CReapplyConceptSaturationLabelSet* conSet = resolvedNode->getReapplyConceptSaturationLabelSet(true);


						// add all concepts to individual
						for (CConceptAssertionLinker* conAssLinkerIt = conAssLinker; conAssLinkerIt; conAssLinkerIt = conAssLinkerIt->getNext()) {
							CConcept* concept = conAssLinkerIt->getData();
							bool negated = conAssLinkerIt->isNegated();
							if (concept == nominalConcept) {
								if (negated) {
									addConceptFilteredToIndividual(topConcept,true,resolvedNode,calcAlgContext);
								}
							} else {
								if (concept->getOperatorCode() == CCNOMINAL) {
									if (concept->getNominalIndividual() == nominalIndi) {
										if (negated) {
											addConceptFilteredToIndividual(topConcept,true,resolvedNode,calcAlgContext);
										}
									} else {
										addConceptFilteredToIndividual(concept,negated,resolvedNode,calcAlgContext);
									}
								} else {
									addConceptFilteredToIndividual(concept,negated,resolvedNode,calcAlgContext);
								}
							}
						}

						preprocessResolvedIndividualNode(resolvedNode,calcAlgContext);
					}
					resolvedNode = resolveData->getProcessingIndividualNode();
					return resolvedNode;
				}



				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeAssertion(CIndividualSaturationProcessNode* indiProcSatNode, CIndividual* nominalIndi, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* resolvedNode = nullptr;
					CSaturationIndividualNodeExtensionResolveData* resolveData = indiProcSatNode->getSuccessorExtensionData(true)->getBaseExtensionResolveData(true);
					CIndividualSaturationProcessNode* copyIndiProcSatNode = indiProcSatNode;
					CConceptAssertionLinker* conAssLinker = nominalIndi->getAssertionConceptLinker();
					CConcept* nominalConcept = nominalIndi->getIndividualNominalConcept();
					// resolve individual
					for (CConceptAssertionLinker* conAssLinkerIt = conAssLinker; conAssLinkerIt; conAssLinkerIt = conAssLinkerIt->getNext()) {
						CConcept* concept = conAssLinkerIt->getData();
						bool negated = conAssLinkerIt->isNegated();
						if (negated || concept != nominalConcept) {
							resolveData = getResolvedIndividualNodeExtension(resolveData,concept,negated,copyIndiProcSatNode,calcAlgContext);
						}
					}
					if (!resolveData->hasProcessingIndividualNode()) {
						// create individual
						resolvedNode = createResolvedIndividualNode(resolveData,copyIndiProcSatNode,true,calcAlgContext);
						resolvedNode->setSeparated(indiProcSatNode->isSeparated());
						CReapplyConceptSaturationLabelSet* conSet = resolvedNode->getReapplyConceptSaturationLabelSet(true);

						// add all concepts to individual
						for (CConceptAssertionLinker* conAssLinkerIt = conAssLinker; conAssLinkerIt; conAssLinkerIt = conAssLinkerIt->getNext()) {
							CConcept* concept = conAssLinkerIt->getData();
							bool negated = conAssLinkerIt->isNegated();
							if (negated || concept != nominalConcept) {
								addConceptFilteredToIndividual(concept,negated,resolvedNode,calcAlgContext);
							}
						}

						preprocessResolvedIndividualNode(resolvedNode,calcAlgContext);
					}
					resolvedNode = resolveData->getProcessingIndividualNode();
					return resolvedNode;
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

						preprocessResolvedIndividualNode(resolvedNode,calcAlgContext);
					}
					resolvedNode = resolveData->getProcessingIndividualNode();
					return resolvedNode;
				}



				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createResolvedIndividualNode(CSaturationIndividualNodeExtensionResolveData* resolveData, CIndividualSaturationProcessNode*& copyIndiProcSatNode, bool queueProcessing, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* resolvedNode = nullptr;
					resolvedNode = CObjectParameterizingAllocator< CIndividualSaturationProcessNode,CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
					resolvedNode->initIndividualSaturationProcessNode(resolveData->getProcessingIndividualNodeID(),nullptr,nullptr);
					initializeIndividualNodeByCoping(resolvedNode,copyIndiProcSatNode,false,calcAlgContext);

					resolveData->setProcessingIndividualNode(resolvedNode);
					resolvedNode->getSuccessorExtensionData(true)->setExtensionResolveData(resolveData);
					resolvedNode->setInitialized(true);
					resolvedNode->setRequiredBackwardPropagation(true);
					addIndividualToCompletionQueue(resolvedNode,calcAlgContext);

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







				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::collectResolveIndividualExtendableConceptMap(CIndividualSaturationProcessNode* baseIndiNode, CIndividualSaturationProcessNode* extensionIndiNode, CPROCESSINGHASH<cint64,CConceptNegationPair>*& conExtMap, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* extConSet = extensionIndiNode->getReapplyConceptSaturationLabelSet(false);
					bool extensionRequired = false;
					for (CConceptSaturationDescriptor* conIt = extConSet->getConceptSaturationDescriptionLinker(); conIt; conIt = conIt->getNextConceptDesciptor()) {
						CConceptSaturationDescriptor* containedConDes = nullptr;
						CImplicationReapplyConceptSaturationDescriptor* containedImpReaDes = nullptr;
						bool insertionRequired = false;
						CReapplyConceptSaturationLabelSet* baseConSet = baseIndiNode->getReapplyConceptSaturationLabelSet(false);
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
							extensionRequired = true;
							if (!conExtMap) {
								conExtMap = CObjectParameterizingAllocator< CPROCESSINGHASH<cint64,CConceptNegationPair>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getTaskProcessorContext());
							}
							conExtMap->insert(conIt->getConceptTag(),CConceptNegationPair(conIt->getConcept(),conIt->isNegated()));
						}
					}
					return extensionRequired;
				}


				CSaturationIndividualNodeExtensionResolveData* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CPROCESSINGHASH<cint64,CConceptNegationPair>* conExtensionMap, CIndividualSaturationProcessNode*& copyIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					return getResolvedIndividualNodeExtension(resolveData,conExtensionMap,copyIndiProcSatNode,nullptr,calcAlgContext);
				}

				CSaturationIndividualNodeExtensionResolveData* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CPROCESSINGHASH<cint64,CConceptNegationPair>* conExtensionMap, CIndividualSaturationProcessNode*& copyIndiProcSatNode, bool* newNodeExpansionCreated, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (conExtensionMap) {

						for (CPROCESSHASH<cint64,CConceptNegationPair>::const_iterator it = conExtensionMap->constBegin(), itEnd = conExtensionMap->constEnd(); it != itEnd; ++it) {
							const CConceptNegationPair& conExtDat(it.value());
							CConcept* concept = conExtDat.first;
							bool negation = conExtDat.second;

							resolveData = getResolvedIndividualNodeExtension(resolveData,concept,negation,copyIndiProcSatNode,calcAlgContext);
						}

						if (!resolveData->getProcessingIndividualNode()) {
							CIndividualSaturationProcessNode* resolvedNode = createResolvedIndividualNode(resolveData,copyIndiProcSatNode,true,calcAlgContext);
							resolveData->setProcessingIndividualNode(resolvedNode);


							for (CPROCESSHASH<cint64,CConceptNegationPair>::const_iterator it = conExtensionMap->constBegin(), itEnd = conExtensionMap->constEnd(); it != itEnd; ++it) {
								const CConceptNegationPair& conExtDat(it.value());
								CConcept* concept = conExtDat.first;
								bool negation = conExtDat.second;
								addConceptFilteredToIndividual(concept,negation,resolvedNode,calcAlgContext);
							}
							preprocessResolvedIndividualNode(resolvedNode,calcAlgContext);

							if (newNodeExpansionCreated) {
								*newNodeExpansionCreated = true;
							}
						}

						copyIndiProcSatNode = resolveData->getProcessingIndividualNode();
					}

					return resolveData;
				}




				CSaturationIndividualNodeExtensionResolveData* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CIndividualSaturationProcessNode* extensionNode, CIndividualSaturationProcessNode*& copyIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					return getResolvedIndividualNodeExtension(resolveData,extensionNode,copyIndiProcSatNode,nullptr,calcAlgContext);
				}



				CSaturationIndividualNodeExtensionResolveData* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CIndividualSaturationProcessNode* extensionNode, CIndividualSaturationProcessNode*& copyIndiProcSatNode, bool* newNodeExpansionCreated, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationIndividualNodeExtensionResolveHashData& resolveHashData = resolveData->getIndividualNodeExtensionResolveHash(true)->getResolvedIndividualNodeExtensionData(extensionNode);
					if (!resolveHashData.mResolveData) {
						CReapplyConceptSaturationLabelSet* extConSet = extensionNode->getReapplyConceptSaturationLabelSet(false);

						CPROCESSINGHASH<cint64,CConceptNegationPair>* conExtensionMap = nullptr;

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
								if (!conExtensionMap) {
									conExtensionMap = CObjectParameterizingAllocator< CPROCESSINGHASH<cint64,CConceptNegationPair>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getTaskProcessorContext());
								}
								conExtensionMap->insert(conIt->getConceptTag(),CConceptNegationPair(conIt->getConcept(),conIt->isNegated()));
							}
						}


						resolveData = getResolvedIndividualNodeExtension(resolveData,conExtensionMap,copyIndiProcSatNode,newNodeExpansionCreated,calcAlgContext);

						
						resolveHashData.mResolveData = resolveData;

					}
					return resolveHashData.mResolveData;
				}









				CSaturationIndividualNodeExtensionResolveData* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getResolvedNeighbourIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CIndividualSaturationProcessNode*& copyIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationIndividualNodeExtensionResolveHashData& resolveHashData = resolveData->getIndividualNodeExtensionResolveHash(true)->getResolvedNeighbourIndividualNodeExtensionData();
					if (!resolveHashData.mResolveData) {
						resolveHashData.mResolveData = CObjectParameterizingAllocator< CSaturationIndividualNodeExtensionResolveData, CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(), calcAlgContext->getUsedProcessContext());
						cint64 nextResolveIndiID = calcAlgContext->getUsedProcessingDataBox()->getNextSaturationResolvedSuccessorExtensionIndividualNodeID();
						resolveHashData.mResolveData->initExtensionResolveData(nextResolveIndiID);
					}
					if (resolveHashData.mResolveData->hasProcessingIndividualNode()) {
						copyIndiProcSatNode = resolveHashData.mResolveData->getProcessingIndividualNode();
					}
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
						if (installSuccessorPredecessorRoleFunctionalityConceptsExtension(indiProcSatNode,role,calcAlgContext)) {
							functionalConceptsExtension->addLinkedSuccessorAddedRoleProcessLinker(tmpRoleSatProcLinker);
							addFUNCTIONALProcessRoleExtensionFunctionalityAddedToDependentIndividuals(indiProcSatNode,role,calcAlgContext);

							CRoleSaturationProcessLinker* predFuncRoleProcLinker = createRoleSaturationProcessLinker(calcAlgContext);
							predFuncRoleProcLinker->initRoleProcessLinker(role);
							functionalConceptsExtension->addLinkedPredecessorAddedRoleProcessLinker(predFuncRoleProcLinker);

							CRoleSaturationProcessLinker* copyInitRoleLinker = createRoleSaturationProcessLinker(calcAlgContext);
							copyInitRoleLinker->initRoleProcessLinker(role);
							functionalConceptsExtension->addCopyingInitializingRoleProcessLinker(copyInitRoleLinker);
						}
					}


					if (!updated) {
						CRoleSaturationProcessLinker* succLinkedAddedRoleSatProcLinker = functionalConceptsExtension->takeLinkedSuccessorAddedRoleProcessLinker();
						while (succLinkedAddedRoleSatProcLinker) {
							CRole* role = succLinkedAddedRoleSatProcLinker->getRole();
							CRoleSaturationProcessLinker* tmpRoleSatProcLinker = succLinkedAddedRoleSatProcLinker;
							succLinkedAddedRoleSatProcLinker = succLinkedAddedRoleSatProcLinker->getNext();
							tmpRoleSatProcLinker->clearNext();
							updated |= updateSuccessorRoleFUNCTIONALConceptsExtensions(indiProcSatNode,role,calcAlgContext);
							releaseRoleSaturationProcessLinker(tmpRoleSatProcLinker,calcAlgContext);
							//addFUNCTIONALProcessRoleExtensionLinkedSuccessorAddedToDependentIndividuals(indiProcSatNode,role,calcAlgContext);
						}
					}


					if (!updated) {
						CRoleSaturationProcessLinker* predLinkedAddedRoleSatProcLinker = functionalConceptsExtension->takeLinkedPredecessorAddedRoleProcessLinker();
						while (predLinkedAddedRoleSatProcLinker) {
							CRole* role = predLinkedAddedRoleSatProcLinker->getRole();
							CRoleSaturationProcessLinker* tmpRoleSatProcLinker = predLinkedAddedRoleSatProcLinker;
							predLinkedAddedRoleSatProcLinker = predLinkedAddedRoleSatProcLinker->getNext();
							tmpRoleSatProcLinker->clearNext();
							updated |= updatePredecessorRoleFUNCTIONALConceptsExtensions(indiProcSatNode,role,calcAlgContext);
							releaseRoleSaturationProcessLinker(tmpRoleSatProcLinker,calcAlgContext);
							//addFUNCTIONALProcessRoleExtensionLinkedPredecessorAddedToDependentIndividuals(indiProcSatNode,role,calcAlgContext);
						}
					}



					if (!updated) {
						CConceptSaturationProcessLinker* funcQualAtmostConSatProcLinker = functionalConceptsExtension->takeQualifiedFunctionalAtmostConceptProcessLinker();
						while (funcQualAtmostConSatProcLinker) {
							CConceptSaturationDescriptor* conDes = funcQualAtmostConSatProcLinker->getConceptSaturationDescriptor();
							CConceptSaturationProcessLinker* tmpConDesSatProcLinker = funcQualAtmostConSatProcLinker;
							funcQualAtmostConSatProcLinker = funcQualAtmostConSatProcLinker->getNext();
							tmpConDesSatProcLinker->clearNext();
							CConcept* funcQualAtleastConcept = conDes->getConcept();
							updated |= updateSuccessorRoleQualifiedFUNCTIONALConceptsExtensions(indiProcSatNode,funcQualAtleastConcept->getRole(),funcQualAtleastConcept->getOperandList(),calcAlgContext);
							releaseConceptSaturationProcessLinker(tmpConDesSatProcLinker,calcAlgContext);
							addFUNCTIONALQualifiedProcessAtmostConceptExtensionToDependentIndividuals(indiProcSatNode,conDes,calcAlgContext);
						}
					}

					if (succExtensionData->isExtensionProcessingQueued()) {
						succExtensionData->setExtensionProcessingQueued(false);
					}


					return updated;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::processNextSuccessorExtensions(CCalculationAlgorithmContextBase* calcAlgContext) {
					bool extensionProcessed = false;
					CIndividualSaturationProcessNode* indiProcSatNode = nullptr;
					CSaturationSuccessorExtensionIndividualNodeProcessingQueue* extProIndiQueue = mProcessingDataBox->getSaturationSucessorExtensionIndividualNodeProcessingQueue(false);
					while (!extensionProcessed && extProIndiQueue && !extProIndiQueue->isEmpty()) {
						indiProcSatNode = extProIndiQueue->takeNextToCurrentProcessIndividual();
						if (indiProcSatNode && !indiProcSatNode->isSeparated()) {
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
					for (CXNegLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							addSuccessorExtensionToProcessingQueue(depIndi,calcAlgContext);
						}
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addALLProcessRoleExtensionToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXNegLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							CSaturationIndividualNodeSuccessorExtensionData* succExtData = depIndi->getSuccessorExtensionData(true);
							CSaturationIndividualNodeALLConceptsExtensionData* succIndiALLConExtData = succExtData->getALLConceptsExtensionData(true);
							addSuccessorExtensionToProcessingQueue(depIndi,calcAlgContext);
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
					for (CXNegLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							CSaturationIndividualNodeSuccessorExtensionData* succExtData = depIndi->getSuccessorExtensionData(true);
							CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* succIndiFunctionalConExtData = succExtData->getFUNCTIONALConceptsExtensionData(true);
							addSuccessorExtensionToProcessingQueue(depIndi,calcAlgContext);
							if (succIndiFunctionalConExtData->isSuccessorExtensionInitialized()) {
								if (!succIndiFunctionalConExtData->hasLinkedSuccessorAddedProcessLinkerForRole(role)) {
									CRoleSaturationProcessLinker* roleProcLinker = createRoleSaturationProcessLinker(calcAlgContext);
									roleProcLinker->initRoleProcessLinker(role);
									succIndiFunctionalConExtData->addLinkedSuccessorAddedRoleProcessLinker(roleProcLinker);
								}
							}
						}
					}
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addFUNCTIONALQualifiedProcessAtmostConceptExtensionToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CConceptSaturationDescriptor* conDes, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXNegLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							addQualifiedFUNCTIONALAtmostConceptExtensionProcessing(conDes,depIndi,calcAlgContext);
						}
					}
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addFUNCTIONALProcessRoleExtensionLinkedPredecessorAddedToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXNegLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							CSaturationIndividualNodeSuccessorExtensionData* succExtData = depIndi->getSuccessorExtensionData(true);
							CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* succIndiFunctionalConExtData = succExtData->getFUNCTIONALConceptsExtensionData(true);
							if (succIndiFunctionalConExtData->isSuccessorExtensionInitialized()) {
								addSuccessorExtensionToProcessingQueue(depIndi,calcAlgContext);
								if (!succIndiFunctionalConExtData->hasLinkedPredecessorAddedProcessLinkerForRole(role)) {
									CRoleSaturationProcessLinker* roleProcLinker = createRoleSaturationProcessLinker(calcAlgContext);
									roleProcLinker->initRoleProcessLinker(role);
									succIndiFunctionalConExtData->addLinkedPredecessorAddedRoleProcessLinker(roleProcLinker);
								}
							}
						}
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addFUNCTIONALProcessRoleExtensionFunctionalityAddedToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXNegLinker<CIndividualSaturationProcessNode*>* depIndiLinkerIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiLinkerIt; depIndiLinkerIt = depIndiLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* depIndi = depIndiLinkerIt->getData();
						if (depIndi) {
							CSaturationIndividualNodeSuccessorExtensionData* succExtData = depIndi->getSuccessorExtensionData(true);
							CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* succIndiFunctionalConExtData = succExtData->getFUNCTIONALConceptsExtensionData(true);
							addSuccessorExtensionToProcessingQueue(depIndi,calcAlgContext);
							if (!succIndiFunctionalConExtData->hasFunctionalityAddedProcessLinkerForRole(role)) {
								CRoleSaturationProcessLinker* roleProcLinker = createRoleSaturationProcessLinker(calcAlgContext);
								roleProcLinker->initRoleProcessLinker(role);
								succIndiFunctionalConExtData->addFunctionalityAddedRoleProcessLinker(roleProcLinker);
							}
						}
					}
				}

				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::markNominalATMOSTRestrictedAncestorsAsInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool ancestorsRestricted = false;
					CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = conDes->isNegated();
					CRole* role = concept->getRole();
					if (backwardPropHash && !indiProcSatNode->isABoxIndividualRepresentationNode()) {
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
					CCriticalPredecessorRoleCardinalityHash* critPredRolCardHash = indiProcSatNode->getCriticalPredecessorRoleCardinalityHash(true);
					critPredRolCardHash->addCriticalPredecessorRoleCardinality(role,concept,!conceptNegation);
					return ancestorsRestricted;				
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::markATMOSTRestrictedAncestorsAsInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode* functionallyRestrictedSuccessorNode, CXNegLinker<CRole*>* functionallyRestrictedSuccessorCreationRoleLinker, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool ancestorsRestricted = false;
					CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = conDes->isNegated();
					CRole* role = concept->getRole();
					if (backwardPropHash && !indiProcSatNode->isABoxIndividualRepresentationNode()) {
						CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();
						if (backPropDataHash) {
							for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropDataHash->constFind(role), itEnd = backPropDataHash->constEnd(); it != itEnd && it.key() == role; ++it) {
								const CRoleBackwardSaturationPropagationHashData& backPropData = it.value();
								for (CBackwardSaturationPropagationLink* backPropLinkIt = backPropData.mLinkLinker; backPropLinkIt; backPropLinkIt = backPropLinkIt->getNext()) {
									CBackwardSaturationPropagationLink* backPropLink = backPropLinkIt;
									CIndividualSaturationProcessNode* sourceIndi = backPropLink->getSourceIndividual();
									bool ancestorInsufficient = true;										
									cint64 allowedCardinality = concept->getParameter() - 1*conceptNegation;

									if (!sourceIndi->getIndirectStatusFlags()->hasInsufficientFlag()) {
										if (allowedCardinality == 1 && !concept->getOperandList()) {
											if (sourceIndi == functionallyRestrictedSuccessorNode) {
												ancestorInsufficient = false;
											} else {
												collectLinkedSuccessorNodes(sourceIndi,calcAlgContext);
												CLinkedRoleSaturationSuccessorHash* linkedSuccHash = sourceIndi->getLinkedRoleSuccessorHash(false);
												if (linkedSuccHash) {
													CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
													CRole* inverseRole = getInverseRole(role,calcAlgContext);
													CLinkedRoleSaturationSuccessorData* succData = succHash->value(inverseRole);
													if (succData) {
														CSaturationSuccessorData* succRoleData = succData->mSuccNodeDataMap.value(indiProcSatNode->getIndividualID());
														if (succRoleData) {
															if (succRoleData->mActiveCount <= 0) {
																ancestorInsufficient = false;
															} else {


																bool funcSuccAllRolePredMerged = false;
																CSaturationIndividualNodeSuccessorExtensionData* succExtData = functionallyRestrictedSuccessorNode->getSuccessorExtensionData(false);
																if (succExtData) {
																	CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* funcConExtData = succExtData->getFUNCTIONALConceptsExtensionData(false);
																	if (funcConExtData) {
																		funcSuccAllRolePredMerged = true;
																		for (CXNegLinker<CRole*>* functionallyRestrictedSuccessorCreationRoleIt = functionallyRestrictedSuccessorCreationRoleLinker; funcSuccAllRolePredMerged && functionallyRestrictedSuccessorCreationRoleIt; functionallyRestrictedSuccessorCreationRoleIt = functionallyRestrictedSuccessorCreationRoleIt->getNext()) {
																			if (!functionallyRestrictedSuccessorCreationRoleIt->isNegated()) {
																				if (!funcConExtData->hasIndividualNodeForwardingPredecessorMerged(sourceIndi,functionallyRestrictedSuccessorCreationRoleIt->getData())) {
																					funcSuccAllRolePredMerged = false;
																				}
																			}
																		}
																	}
																}


																if (funcSuccAllRolePredMerged || isIndividualNodeLabelMergingSubset(functionallyRestrictedSuccessorNode,sourceIndi,true,calcAlgContext)) {
																	if (funcSuccAllRolePredMerged || isSuccessorCreationRoleMergingSubset(functionallyRestrictedSuccessorCreationRoleLinker,succRoleData->mCreationRoleLinker,calcAlgContext)) {
																		ancestorInsufficient = false;
																	}
																}
															}
														} 
													}
												}
											}
										}
									}

									if (ancestorInsufficient) {
										updateDirectAddingIndividualStatusFlags(sourceIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
										setInsufficientNodeOccured(mCalcAlgContext);
										ancestorsRestricted = true;
									}
								}
							}
						}
					}
					CCriticalPredecessorRoleCardinalityHash* critPredRolCardHash = indiProcSatNode->getCriticalPredecessorRoleCardinalityHash(true);
					critPredRolCardHash->addCriticalPredecessorRoleCardinality(role,concept,!conceptNegation);
					return ancestorsRestricted;
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addCriticalORConceptTestedForDependentNodes(CConceptSaturationDescriptor* conDes, CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType, CIndividualSaturationProcessNode*& indiProcSatNode, CCriticalIndividualNodeConceptTestSet* criticalIndiNodeConTestSet, CCalculationAlgorithmContextBase* calcAlgContext) {

					cint64 updatedNodes = 0;
					cint64 currentUpdateLinkerCount = 1;

					CIndividualSaturationProcessNodeStatusUpdateLinker* directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext);
					directUpdateLinker->initUpdateNodeLinker(indiProcSatNode);

					while (directUpdateLinker) {
						CIndividualSaturationProcessNodeStatusUpdateLinker* nextUpdateLinker = directUpdateLinker;
						directUpdateLinker = directUpdateLinker->getNext();
						CIndividualSaturationProcessNode* updateIndiNode = nextUpdateLinker->getData();
						nextUpdateLinker->clearNext();
						releaseIndividualSaturationUpdateLinker(nextUpdateLinker,calcAlgContext);

						--currentUpdateLinkerCount;
						++updatedNodes;

						for (CXNegLinker<CIndividualSaturationProcessNode*>* depIndiIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depIndiIt; depIndiIt = depIndiIt->getNext()) {
							if (depIndiIt->isNegated()) {
								CIndividualSaturationProcessNode* dependingIndiNode = depIndiIt->getData();
								CIndividualSaturationProcessNodeStatusFlags* statusFlag = dependingIndiNode->getDirectStatusFlags();
								bool continueDepending = false;
								if (!statusFlag->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,false)) {

									if (!criticalIndiNodeConTestSet->isConceptTestedForIndividual(conDes,dependingIndiNode)) {
										criticalIndiNodeConTestSet->insertConceptTestedForIndividual(conDes,dependingIndiNode);
										STATINC(SATURATIONCRITICALTESTCOUNT,calcAlgContext);
										if (isCriticalORConceptDescriptorInsufficient(conDes,dependingIndiNode,calcAlgContext)) {
											updateDirectNotDependentAddingIndividualStatusFlags(dependingIndiNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
											setInsufficientNodeOccured(calcAlgContext);
											continueDepending = true;
										}
									}
								} else {
									continueDepending = true;
								}

								if (continueDepending) {
									CIndividualSaturationProcessNodeStatusUpdateLinker* nextUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext);
									nextUpdateLinker->initUpdateNodeLinker(dependingIndiNode);
									directUpdateLinker = nextUpdateLinker->append(directUpdateLinker);

									++currentUpdateLinkerCount;
								}
							}
						}
					}
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addCriticalConceptForDependentNodes(CConceptSaturationDescriptor* conDes, CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType, CIndividualSaturationProcessNode*& indiProcSatNode, bool directFlagsCheck, cint64 checkFlags, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CXNegLinker<CIndividualSaturationProcessNode*>* depIndiIt = indiProcSatNode->getCopyDependingIndividualNodeLinker(); depIndiIt; depIndiIt = depIndiIt->getNext()) {
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
					if (!indirectFlags->hasInsufficientFlag() && !indirectFlags->hasClashedFlag()) {
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
										++mInsufficientALLCount;
									} else {
										addCriticalConceptForDependentNodes(criticalConDes,conceptType,indiProcSatNode,false,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
									}
								}

								releaseConceptSaturationProcessLinker(criticalConProcDes,calcAlgContext);
							}
						}
					}

					if (!indirectFlags->hasInsufficientFlag() && !indirectFlags->hasClashedFlag()) {
						CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType = CCriticalSaturationConceptTypeQueues::CCT_ATMOST;
						CCriticalSaturationConceptQueue* criticalConTypeQueue = criticalConceptQueues->getCriticalSaturationConceptQueue(conceptType,false);
						if (criticalConTypeQueue) {
							while (criticalConTypeQueue->hasCriticalConceptDescriptorLinker() && !indirectFlags->hasInsufficientFlag()) {
								CConceptSaturationProcessLinker* criticalConProcDes = criticalConTypeQueue->takeNextCriticalConceptDescriptor();
								CConceptSaturationDescriptor* criticalConDes = criticalConProcDes->getConceptSaturationDescriptor();

								if (!criticalIndiNodeConTestSet->isConceptTestedForIndividual(criticalConDes,indiProcSatNode)) {
									criticalIndiNodeConTestSet->insertConceptTestedForIndividual(criticalConDes,indiProcSatNode);
									bool ancestorPossiblyInsufficient = false;
									CIndividualSaturationProcessNode* functionallyRestrictedSuccessorNode = nullptr;
									CXNegLinker<CRole*>* functionallyRestrictedSuccessorCreationRoleLinker = nullptr;
									STATINC(SATURATIONCRITICALTESTCOUNT,calcAlgContext);
									if (isCriticalATMOSTConceptDescriptorInsufficient(criticalConDes,ancestorPossiblyInsufficient,functionallyRestrictedSuccessorNode,functionallyRestrictedSuccessorCreationRoleLinker,indiProcSatNode,calcAlgContext)) {
										if (mConfDelayedMergingCriticalATMOSTConcepts && indiProcSatNode->getMaxAtleastCardinalityCandidate() > mConfDelayedMergingCriticalATMOSTConceptsCardinalitySize) {
											if (!indirectFlags->hasInsufficientFlag() && !indirectFlags->hasClashedFlag()) {
												CSaturationATMOSTSuccessorMergingData* atmostSuccMergData = indiProcSatNode->getATMOSTSuccessorMergingData(true);
												if (!atmostSuccMergData->isMergingProcessingQueued()) {
													processingDataBox->addSaturationATMOSTMergingProcessLinker(atmostSuccMergData->getMergingIndividualProcessLinker()->setProcessingQueued(true));
												}
												CSaturationATMOSTSuccessorMergingHashData& atmostSuccConHashData = atmostSuccMergData->getATMOSTConceptMergingDataHash()->getATMOSTConceptMergingData(criticalConDes);
												if (!atmostSuccConHashData.mQueued) {
													atmostSuccConHashData.mQueued = true;
													atmostSuccMergData->addMergingProcessingConcept(criticalConDes);
												}
											}
										} else {
											++mInsufficientATMOSTCount;
											//bool bug = false;
											//while (bug) {
											//	isCriticalATMOSTConceptDescriptorInsufficient(criticalConDes,ancestorPossiblyInsufficient,functionallyRestrictedSuccessorNode,functionallyRestrictedSuccessorCreationRoleLinker,indiProcSatNode,calcAlgContext);
											//}
											updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
											setInsufficientNodeOccured(calcAlgContext);
										}
									} else {
										addCriticalConceptForDependentNodes(criticalConDes,conceptType,indiProcSatNode,false,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
									}
									if (indiProcSatNode->hasNominalIntegrated()) {
										markNominalATMOSTRestrictedAncestorsAsInsufficient(criticalConDes,indiProcSatNode,calcAlgContext);
									}
									if (ancestorPossiblyInsufficient) {
										markATMOSTRestrictedAncestorsAsInsufficient(criticalConDes,functionallyRestrictedSuccessorNode,functionallyRestrictedSuccessorCreationRoleLinker,indiProcSatNode,calcAlgContext);
										updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYPROPLEMATIC,calcAlgContext);
									}
								}

								releaseConceptSaturationProcessLinker(criticalConProcDes,calcAlgContext);
							}
						}
					}




					if (!indirectFlags->hasInsufficientFlag() && !indirectFlags->hasClashedFlag()) {
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


					if (!indirectFlags->hasInsufficientFlag() && !indirectFlags->hasClashedFlag()) {
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




					if (!indirectFlags->hasClashedFlag()) {
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
										addCriticalORConceptTestedForDependentNodes(criticalConDes,conceptType,indiProcSatNode,criticalIndiNodeConTestSet,calcAlgContext);
									}
								}
								releaseConceptSaturationProcessLinker(criticalConProcDes,calcAlgContext);
							}
						}
					}


					if (!indirectFlags->hasClashedFlag()) {
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




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::collectLinkedSuccessorNodes(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext, CLinkedRoleSaturationSuccessorHash* linkedRoleSuccHash) {
					if (!linkedRoleSuccHash) {
						linkedRoleSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(true);
					}
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
					CSaturationSuccessorRoleAssertionLinker* lastSatSuccRoleAssLinker = linkedRoleSuccHash->getLastExaminedRoleAssertionLinker();
					CSaturationSuccessorRoleAssertionLinker* satSuccRoleAssLinker = indiProcSatNode->getRoleAssertionLinker();
					for (CSaturationSuccessorRoleAssertionLinker* satSuccRoleAssLinkerIt = satSuccRoleAssLinker; satSuccRoleAssLinkerIt && satSuccRoleAssLinkerIt != lastSatSuccRoleAssLinker; satSuccRoleAssLinkerIt = satSuccRoleAssLinkerIt->getNext()) {
						CRole* role = satSuccRoleAssLinkerIt->getAssertionRole();
						bool roleNegation = satSuccRoleAssLinkerIt->getAssertionRoleNegation();
						CIndividualSaturationProcessNode* destNode = satSuccRoleAssLinkerIt->getAssertionDestinationNode();
						addLinkedSuccessorNodeForRoleAssertion(destNode,role,roleNegation,linkedRoleSuccHash,indiProcSatNode,calcAlgContext);
					}
					linkedRoleSuccHash->setLastExaminedRoleAssertionLinker(satSuccRoleAssLinker);
					linkedRoleSuccHash->setLastExaminedConceptDescriptor(conDesLinker);
				}






				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addLinkedSuccessorNodeForRoleAssertion(CIndividualSaturationProcessNode* destNode, CRole* role, bool roleInversion, CLinkedRoleSaturationSuccessorHash* linkedRoleSuccHash, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
					while (superRoleIt) {
						CRole* superRole = superRoleIt->getData();
						if (!superRoleIt->isNegated() ^ roleInversion) {
							linkedRoleSuccHash->addLinkedSuccessor(superRole,destNode,role,1,true);
						}
						superRoleIt = superRoleIt->getNext();
					}
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
													linkedRoleSuccHash->addLinkedSuccessor(superRole,existIndiNode,role,successorCount,false);
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
															linkedRoleSuccHash->addLinkedSuccessor(superRole,existIndiNode,role,successorCount,false);
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
							CConcept* baseTopConcept = nullptr;
							if (!role->isDataRole()) {
								baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
							} else {
								baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopDataRangeConcept();
							}
							CConcept* opConcept = baseTopConcept;
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
														linkedRoleSuccHash->addLinkedSuccessor(superRole,existIndiNode,role,successorCount,false);
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
											if (isConsistenceDataAvailable(calcAlgContext)) {
												CIndividualProcessNode* indiProcNode = getCorrectedNode(indiSuccData->mVALUENominalID,mDetCachedCGIndiVector,mCalcAlgContext);
												if (indiProcNode) {
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
												} else {
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


												//CRoleBackwardSaturationPropagationHashData* backwardPropData = nullptr;
												//CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
												//if (backwardPropHash) {
												//	CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backwardPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();

												//	backwardPropData = backwardPropDataHash->valuePointer(role);
												//}

												//CSaturationSuccessorALLConceptExtensionData* satSucALLConExtData = nullptr;
												//CSaturationLinkedSuccessorIndividualALLConceptsExtensionData* satSucALLConExtRoleData = nullptr;
												//CSaturationIndividualNodeSuccessorExtensionData* indiProcSatNodeSuccExt = indiProcSatNode->getSuccessorExtensionData(false);
												//if (indiProcSatNodeSuccExt) {
												//	CSaturationIndividualNodeALLConceptsExtensionData* indiProcSatNodeALLConSuccExt = indiProcSatNodeSuccExt->getALLConceptsExtensionData(false);
												//	if (indiProcSatNodeALLConSuccExt) {

												//		satSucALLConExtRoleData = indiProcSatNodeALLConSuccExt->getALLConceptsExtensionData(succNode,false);
												//		if (satSucALLConExtRoleData) {
												//			satSucALLConExtData = satSucALLConExtRoleData->getRoleSuccessorALLConceptExtensionData(role,false);
												//		}
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


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isCriticalATMOSTConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, bool& ancestorPossiblyCriticalFlag, CIndividualSaturationProcessNode*& functionallyRestrictedSuccessorNode, CXNegLinker<CRole*>*& functionallyRestrictedSuccessorCreationRoleLinker, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(SATURATIONCRITICALATMOSTCOUNT,calcAlgContext);
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = conDes->isNegated();
					CRole* role = concept->getRole();
					cint64 allowedCardinality = concept->getParameter() - 1*conceptNegation;
					if (allowedCardinality < 0) {
						return true;
					}
					if (!indiProcSatNode->hasSubstituteIndividualNode()) {

						//if (getDebugIndividualConceptName(indiProcSatNode,calcAlgContext) == "http://www.projecthalo.com/aura#Hydrogenated-Margarine") {
						//	bool debug = true;
						//}

						collectLinkedSuccessorNodes(indiProcSatNode,calcAlgContext);
						CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
						if (linkedSuccHash) {

							cint64 foundCardinality = 0;
							cint64 minCardinality = 0;
							CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinker = nullptr;

							CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
							CLinkedRoleSaturationSuccessorData* succData = succHash->value(role);
							if (succData) {
								if (succData->mSuccCount >= allowedCardinality) {

									CIndividualSaturationProcessNode* lastSuccessorNode = nullptr;
									CXNegLinker<CRole*>* lastSuccessorCreationRoleLinker = nullptr;

									foundCardinality = collectATMOSTConceptRelevantSuccessors(conDes,indiProcSatNode,succData,mergingSuccDataLinker,lastSuccessorNode,lastSuccessorCreationRoleLinker,minCardinality,calcAlgContext);


									cint64 mergeableCardinality = 0;
									if (foundCardinality >= allowedCardinality && foundCardinality > 1) {

										// check whether some trivial merging is possible
										CPROCESSHASH<CSaturationSuccessorData*,cint64>* remainMergeableCardHash = CObjectParameterizingAllocator< CPROCESSHASH<CSaturationSuccessorData*,cint64>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getTaskProcessorContext());

										CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>* mergeDistintHash = CObjectParameterizingAllocator< CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getTaskProcessorContext());
										CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >* mergeDistintSet = CObjectParameterizingAllocator< CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getTaskProcessorContext());

										if (mergingSuccDataLinker) {

											if (mConfSimpleMergingTestForATMOSTCriticalTesting) {
												for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker; mergingSuccDataLinkerIt && foundCardinality-mergeableCardinality >= allowedCardinality; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
													CSaturationSuccessorData* succLinkData = mergingSuccDataLinkerIt->getData();
													if (succLinkData->mSuccCount >= 1) {
														cint64 maxRequiredMergingCardinality = foundCardinality-mergeableCardinality-(allowedCardinality-1);
														cint64 mergingCardinality = getSuccessorLinkSimplyMergeableCardinalityCount(indiProcSatNode,succLinkData,mergingSuccDataLinker,remainMergeableCardHash,role,maxRequiredMergingCardinality,mergeDistintHash,mergeDistintSet,calcAlgContext);
														cint64 remainingCardinality = succLinkData->mSuccCount-mergingCardinality;
														remainMergeableCardHash->insert(succLinkData,remainingCardinality);
														mergeableCardinality += mergingCardinality;
													}
												}
											}



											if (mConfDetailedMergingTestForATMOSTCriticalTesting && foundCardinality-mergeableCardinality >= allowedCardinality && foundCardinality-mergeableCardinality <= allowedCardinality*2) {
												for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker; mergingSuccDataLinkerIt && foundCardinality-mergeableCardinality >= allowedCardinality; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
													CSaturationSuccessorData* succLinkData = mergingSuccDataLinkerIt->getData();
													if (succLinkData->mSuccCount >= 1) {
														cint64 succRemainingCardinality = remainMergeableCardHash->value(succLinkData,succLinkData->mSuccCount);
														if (succRemainingCardinality > 0) {
															cint64 maxRequiredMergingCardinality = foundCardinality-mergeableCardinality-(allowedCardinality-1);

															//for (CIndividualSaturationSuccessorLinkDataLinker* otherSuccDataLinkerIt = mergingSuccDataLinker; otherSuccDataLinkerIt && foundCardinality-mergeableCardinality >= allowedCardinality; otherSuccDataLinkerIt = otherSuccDataLinkerIt->getNext()) {
															//	if (otherSuccDataLinkerIt != mergingSuccDataLinkerIt) {
															//		CSaturationSuccessorData* otherSuccLinkData = otherSuccDataLinkerIt->getData();
															//		if (otherSuccLinkData->mSuccCount >= 1) {
															//			cint64 otherSuccRemainingCardinality = remainMergeableCardHash->value(otherSuccLinkData,otherSuccLinkData->mSuccCount);
															//			if (otherSuccRemainingCardinality > 0) {



															cint64 mergingCardinality = getSuccessorLinkExtendedMergeableCardinalityCount(indiProcSatNode,succLinkData,nullptr,mergingSuccDataLinkerIt->getNext(),remainMergeableCardHash,role,maxRequiredMergingCardinality,mergeDistintHash,mergeDistintSet,calcAlgContext);
															if (mergingCardinality > 0) {
																cint64 newSuccCard = qMax(succRemainingCardinality,mergingCardinality);
																remainMergeableCardHash->insert(succLinkData,newSuccCard);
																cint64 removedSuccCard = qMin(succRemainingCardinality,mergingCardinality);
																mergeableCardinality += removedSuccCard;
															}
														}
													}
												}
											}

										}
										if (mergingSuccDataLinker) {
											releaseIndividualSaturationSuccessorLinkDataLinker(mergingSuccDataLinker,calcAlgContext);
										}
									}


									if (foundCardinality-mergeableCardinality == allowedCardinality || minCardinality >= allowedCardinality) {
										ancestorPossiblyCriticalFlag = true;
										if (allowedCardinality == 1) {
											functionallyRestrictedSuccessorNode = lastSuccessorNode;
											functionallyRestrictedSuccessorCreationRoleLinker = lastSuccessorCreationRoleLinker;
										}
									}
									if (foundCardinality-mergeableCardinality > allowedCardinality) {

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
					return false;
				}








				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::collectATMOSTConceptRelevantSuccessors(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CLinkedRoleSaturationSuccessorData* succData, CIndividualSaturationSuccessorLinkDataLinker*& mergingSuccDataLinker, CIndividualSaturationProcessNode*& lastSuccessorNode, CXNegLinker<CRole*>*& lastSuccessorCreationRoleLinker, cint64& minCardinality, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConcept* concept = conDes->getConcept();
					bool conceptNegation = conDes->isNegated();
					CRole* role = concept->getRole();
					cint64 allowedCardinality = concept->getParameter() - 1*conceptNegation;

					cint64 foundCardinality = 0;
					minCardinality = 0;

					bool trivialQualification = true;
					CConceptRoleBranchingTrigger* chooseTriggerLinker = nullptr;
					for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); trivialQualification && opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
						CConcept* opConcept = opLinkerIt->getData();
						bool opConceptNegation = opLinkerIt->isNegated();
						cint64 opCode = opConcept->getOperatorCode();
						if (opConceptNegation || (opCode != CCATOM && opCode != CCSUB)) {
							trivialQualification = false;
						}
					}
					if (!trivialQualification) {
						CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
						if (conProData) {
							chooseTriggerLinker = conProData->getConceptRoleBranchTrigger();
						}
					}

					for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator itSucc = succData->mSuccNodeDataMap.constBegin(), itSuccEnd = succData->mSuccNodeDataMap.constEnd(); itSucc != itSuccEnd; ++itSucc) {
						CSaturationSuccessorData* succRoleData = itSucc.value();
						if (succRoleData->mActiveCount >= 1) {
							cint64 succCardinality(succRoleData->mSuccCount);
							bool operantsContainedNegative = true;
							bool operantsContainedPositive = true;
							bool operantsContained = true;
							bool qualificationRepresentitiveSuccessorIndi = false;
							if (succRoleData->mVALUENominalConnection) {
								lastSuccessorNode = nullptr;
								lastSuccessorCreationRoleLinker = nullptr;
								CIndividualProcessNode* indiProcNode = getCorrectedNode(succRoleData->mVALUENominalID,mDetCachedCGIndiVector,mCalcAlgContext);
								if (indiProcNode) {
									CReapplyConceptLabelSet* reapplyConSet = indiProcNode->getReapplyConceptLabelSet(false);
									if (reapplyConSet) {
										if (concept->getOperandList()) {
											for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
												CConcept* opConcept = opLinkerIt->getData();
												bool opConceptNegation = opLinkerIt->isNegated();
												bool containedNegation = false;
												if (reapplyConSet->containsConcept(opConcept,&containedNegation)) {
													if (containedNegation == opConceptNegation) {
														operantsContainedNegative = false;
													} else {
														operantsContainedPositive = false;
													}
												} else {
													if (trivialQualification) {
														operantsContainedPositive = false;
													} else {
														bool isChooseTriggered = false;
														for (CConceptRoleBranchingTrigger* chooseTriggerIt = chooseTriggerLinker; isChooseTriggered && chooseTriggerIt; chooseTriggerIt = chooseTriggerIt->getNext()) {
															if (chooseTriggerIt->isConceptTrigger()) {
																if (reapplyConSet->containsConcept(chooseTriggerIt->getTriggerConcept(),&containedNegation)) {
																	if (chooseTriggerIt->getTriggerNegation() == containedNegation) {
																		isChooseTriggered = true;
																	}
																}
															} else {
																isChooseTriggered = true;
															}
														}
														if (!chooseTriggerLinker || isChooseTriggered) {
															operantsContained = false;
														}
													}
												}
											}
										} else {
											operantsContainedNegative = false;
										}
									} else {
										if (trivialQualification) {
											operantsContainedPositive = false;
										} else {
											operantsContained = false;
										}
									}
								}

							} else {
								CIndividualSaturationProcessNode* succNode(succRoleData->mSuccIndiNode);			
								lastSuccessorNode = succNode;
								lastSuccessorCreationRoleLinker = succRoleData->mCreationRoleLinker;
								CReapplyConceptSaturationLabelSet* succConSet = succNode->getReapplyConceptSaturationLabelSet(false);
								CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)succNode->getSaturationConceptReferenceLinking();
								if (succConSet) {
									if (concept->getOperandList()) {
										for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
											CConcept* opConcept = opLinkerIt->getData();
											bool opConceptNegation = opLinkerIt->isNegated();
											bool containedNegation = false;

											if (conceptSatItem) {
												CConcept* indiConcept = conceptSatItem->getSaturationConcept();
												bool indiConNegation = conceptSatItem->getSaturationNegation();
												CRole* indiRole = conceptSatItem->getSaturationRoleRanges();
												if (opConcept == indiConcept && opConceptNegation == indiConNegation && (indiRole == nullptr || indiRole == role)) {
													qualificationRepresentitiveSuccessorIndi = true;
													operantsContainedNegative = false;
												}
											}

											if (!qualificationRepresentitiveSuccessorIndi) {
												if (succConSet->containsConcept(opConcept,&containedNegation)) {
													if (containedNegation == opConceptNegation) {
														operantsContainedNegative = false;
													} else {
														operantsContainedPositive = false;
													}
												} else {
													if (trivialQualification) {
														operantsContainedPositive = false;
													} else {
														bool isChooseTriggered = false;
														for (CConceptRoleBranchingTrigger* chooseTriggerIt = chooseTriggerLinker; isChooseTriggered && chooseTriggerIt; chooseTriggerIt = chooseTriggerIt->getNext()) {
															if (chooseTriggerIt->isConceptTrigger()) {
																if (succConSet->containsConcept(chooseTriggerIt->getTriggerConcept(),&containedNegation)) {
																	if (chooseTriggerIt->getTriggerNegation() == containedNegation) {
																		isChooseTriggered = true;
																	}
																}
															} else {
																isChooseTriggered = true;
															}
														}
														if (!chooseTriggerLinker || isChooseTriggered) {
															operantsContained = false;
														}
													}
												}
											}
										}
									} else {

										if (conceptSatItem) {
											CConcept* indiConcept = conceptSatItem->getSaturationConcept();
											bool indiConNegation = conceptSatItem->getSaturationNegation();
											CRole* indiRole = conceptSatItem->getSaturationRoleRanges();
											CConcept* topConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
											if (topConcept == indiConcept && !indiConNegation && (indiRole == nullptr || indiRole == role)) {
												qualificationRepresentitiveSuccessorIndi = true;
											}
										}
										operantsContainedNegative = false;

									}
								} else {
									if (trivialQualification) {
										operantsContainedPositive = false;
									} else {
										operantsContained = false;
									}
								}
							}
							if (operantsContainedPositive || !operantsContained) {

								minCardinality = qMax(minCardinality,succCardinality);
								if (!indiProcSatNode->getNominalIndividual() && succCardinality > allowedCardinality) {
									updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,calcAlgContext);
								}

								if (!qualificationRepresentitiveSuccessorIndi) {
									foundCardinality += succCardinality;
									CIndividualSaturationSuccessorLinkDataLinker* newMergingSuccDataLinker = createIndividualSaturationSuccessorLinkDataLinker(calcAlgContext);
									newMergingSuccDataLinker->initSuccessorLinkDataLinker(succRoleData);
									mergingSuccDataLinker = newMergingSuccDataLinker->append(mergingSuccDataLinker);
								}
							}


						}
					}

					return foundCardinality;
				}







				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::tryATMOSTConceptSuccessorMerging(CCalculationAlgorithmContextBase* calcAlgContext) {
					bool nodeSaturation = false;
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					while (!nodeSaturation && processingDataBox->hasSaturationATMOSTMergingProcessLinker()) {
						CIndividualSaturationProcessNodeLinker* mergingProcessLinker = processingDataBox->getSaturationATMOSTMergingProcessLinker();
						CIndividualSaturationProcessNode* indiProcSatNode = mergingProcessLinker->getData();

						CIndividualSaturationProcessNodeStatusFlags* indirectFlags = indiProcSatNode->getIndirectStatusFlags();

						if (!indirectFlags->hasInsufficientFlag() && !indirectFlags->hasClashedFlag()) {

							CSaturationATMOSTSuccessorMergingData* atmostSuccMergingData = indiProcSatNode->getATMOSTSuccessorMergingData(false);
							if (atmostSuccMergingData) {
								CConceptSaturationProcessLinker* conProcLinker = atmostSuccMergingData->getMergingConceptLinker();
								while (!nodeSaturation && conProcLinker) {
									CConceptSaturationDescriptor* conDes = conProcLinker->getConceptSaturationDescriptor();


									CSaturationATMOSTSuccessorMergingHashData& mergingSuccData = atmostSuccMergingData->getATMOSTConceptMergingData(conDes);

									bool nodeInsufficient = false;
									bool ancestorPossiblyInsufficient = false;
									CIndividualSaturationProcessNode* functionallyRestrictedSuccessorNode = nullptr;
									CXNegLinker<CRole*>* functionallyRestrictedSuccessorCreationRoleLinker = nullptr;
									STATINC(SATURATIONCRITICALTESTCOUNT,calcAlgContext);
									nodeSaturation = tryIndividiualATMOSTConceptSuccessorMerging(conDes,&mergingSuccData,nodeInsufficient,ancestorPossiblyInsufficient,functionallyRestrictedSuccessorNode,functionallyRestrictedSuccessorCreationRoleLinker,indiProcSatNode,calcAlgContext);
									if (!nodeSaturation) {
										if (nodeInsufficient) {
											++mInsufficientATMOSTCount;
											updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
											setInsufficientNodeOccured(calcAlgContext);
										} else {
											addCriticalConceptForDependentNodes(conDes,CCriticalSaturationConceptTypeQueues::CCT_ATMOST,indiProcSatNode,false,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
										}
										if (indiProcSatNode->hasNominalIntegrated()) {
											markNominalATMOSTRestrictedAncestorsAsInsufficient(conDes,indiProcSatNode,calcAlgContext);
										}
										if (ancestorPossiblyInsufficient) {
											markATMOSTRestrictedAncestorsAsInsufficient(conDes,functionallyRestrictedSuccessorNode,functionallyRestrictedSuccessorCreationRoleLinker,indiProcSatNode,calcAlgContext);
											updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYPROPLEMATIC,calcAlgContext);
										}
									}


									if (!nodeSaturation) {
										conProcLinker = atmostSuccMergingData->takeNextMergingConceptLinker();
									}
								}
							}
						}

						if (!nodeSaturation) {
							mergingProcessLinker = processingDataBox->takeSaturationATMOSTMergingProcessLinker();
							mergingProcessLinker->setProcessingQueued(false);
						}

					}
					return nodeSaturation;
				}






				CIndividualSaturationSuccessorLinkDataLinker* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::reconnectMergedLinkedSuccessors(CSaturationSuccessorData* succLinkData, CSaturationSuccessorData* mergedSuccLinkData, cint64 newSuccCard, cint64 incrSuccCard, CLinkedRoleSaturationSuccessorHash* linkedSuccHash, CLinkedRoleSaturationSuccessorData* succData, CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>* mergeDistintHash, CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >* mergeDistintSet, CPROCESSHASH<CSaturationSuccessorData*,cint64>* remainMergeableCardHash, CIndividualSaturationProcessNode*& indiProcSatNode, CIndividualSaturationProcessNode* resolvedIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinker = nullptr;

					CSaturationSuccessorData* resolvedSuccLinkData = nullptr;
					if (resolvedIndiProcSatNode != succLinkData->mSuccIndiNode) {
						for (CXNegLinker<CRole*>* creationRoleIt = succLinkData->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
							if (!creationRoleIt->isNegated()) {
								CRole* creationRole = creationRoleIt->getData();
								for (CSortedNegLinker<CRole*>* superRoleIt = creationRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
									if (!superRoleIt->isNegated()) {
										CRole* superRole = superRoleIt->getData();
										linkedSuccHash->deactivateLinkedSuccessor(superRole,succLinkData->mSuccIndiNode,creationRole);
									}
								}
								if (!linkedSuccHash->hasActiveLinkedSuccessor(creationRole,resolvedIndiProcSatNode,creationRole,newSuccCard)) {
									for (CSortedNegLinker<CRole*>* superRoleIt = creationRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
										if (!superRoleIt->isNegated()) {
											CRole* superRole = superRoleIt->getData();
											linkedSuccHash->addExtensionSuccessor(superRole,resolvedIndiProcSatNode,creationRole,newSuccCard);
										}
									}
								}
							}
						}	
						resolvedSuccLinkData = succData->getSuccessorNodeDataMap()->value(resolvedIndiProcSatNode->getIndividualID());
						for (CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>::const_iterator mDIt = mergeDistintHash->constFind(succLinkData), mDItEnd = mergeDistintHash->constEnd(); mDIt != mDItEnd && mDIt.key() == succLinkData; ++mDIt) {
							CSaturationSuccessorData* distSuccData = mDIt.value();
							mergeDistintHash->insertMulti(distSuccData,resolvedSuccLinkData);
							mergeDistintHash->insertMulti(resolvedSuccLinkData,distSuccData);
							mergeDistintSet->insert(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(resolvedSuccLinkData,distSuccData),qMax(resolvedSuccLinkData,distSuccData)));
						}
					}
					if (resolvedIndiProcSatNode != mergedSuccLinkData->mSuccIndiNode) {
						for (CXNegLinker<CRole*>* creationRoleIt = mergedSuccLinkData->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
							if (!creationRoleIt->isNegated()) {
								CRole* creationRole = creationRoleIt->getData();
								for (CSortedNegLinker<CRole*>* superRoleIt = creationRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
									if (!superRoleIt->isNegated()) {
										CRole* superRole = superRoleIt->getData();
										linkedSuccHash->deactivateLinkedSuccessor(superRole,mergedSuccLinkData->mSuccIndiNode,creationRole);
									}
								}
								if (!linkedSuccHash->hasActiveLinkedSuccessor(creationRole,resolvedIndiProcSatNode,creationRole,newSuccCard)) {
									for (CSortedNegLinker<CRole*>* superRoleIt = creationRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
										if (!superRoleIt->isNegated()) {
											CRole* superRole = superRoleIt->getData();
											linkedSuccHash->addExtensionSuccessor(superRole,resolvedIndiProcSatNode,creationRole,newSuccCard);
										}
									}
								}
							}
						}	
						resolvedSuccLinkData = succData->getSuccessorNodeDataMap()->value(resolvedIndiProcSatNode->getIndividualID());
						for (CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>::const_iterator mDIt = mergeDistintHash->constFind(mergedSuccLinkData), mDItEnd = mergeDistintHash->constEnd(); mDIt != mDItEnd && mDIt.key() == mergedSuccLinkData; ++mDIt) {
							CSaturationSuccessorData* distSuccData = mDIt.value();
							mergeDistintHash->insertMulti(distSuccData,resolvedSuccLinkData);
							mergeDistintHash->insertMulti(resolvedSuccLinkData,distSuccData);
							mergeDistintSet->insert(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(resolvedSuccLinkData,distSuccData),qMax(resolvedSuccLinkData,distSuccData)));
						}
					}

					if (resolvedIndiProcSatNode == succLinkData->mSuccIndiNode || resolvedIndiProcSatNode == mergedSuccLinkData->mSuccIndiNode) {
						CSaturationSuccessorData* sameSuccLinkData = succLinkData;
						CSaturationSuccessorData* otherSuccLinkData = mergedSuccLinkData;
						if (resolvedIndiProcSatNode != mergedSuccLinkData->mSuccIndiNode) {
							sameSuccLinkData = mergedSuccLinkData;
							otherSuccLinkData = succLinkData;
						}
						if (incrSuccCard > 0) {
							for (CXNegLinker<CRole*>* creationRoleIt = sameSuccLinkData->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
								if (!creationRoleIt->isNegated()) {
									CRole* creationRole = creationRoleIt->getData();
									for (CSortedNegLinker<CRole*>* superRoleIt = creationRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
										if (!superRoleIt->isNegated()) {
											CRole* superRole = superRoleIt->getData();
											linkedSuccHash->increaseLinkedSuccessorCount(superRole,sameSuccLinkData->mSuccIndiNode,creationRole,incrSuccCard);
										}
									}
								}
							}	
						}
						for (CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>::const_iterator mDIt = mergeDistintHash->constFind(otherSuccLinkData), mDItEnd = mergeDistintHash->constEnd(); mDIt != mDItEnd && mDIt.key() == otherSuccLinkData; ++mDIt) {
							CSaturationSuccessorData* distSuccData = mDIt.value();
							mergeDistintHash->insertMulti(distSuccData,sameSuccLinkData);
							mergeDistintHash->insertMulti(sameSuccLinkData,distSuccData);
							mergeDistintSet->insert(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(sameSuccLinkData,distSuccData),qMax(sameSuccLinkData,distSuccData)));
						}
						remainMergeableCardHash->insert(sameSuccLinkData,newSuccCard);
					} else {

						CIndividualSaturationSuccessorLinkDataLinker* newMergingSuccDataLinker = createIndividualSaturationSuccessorLinkDataLinker(calcAlgContext);
						newMergingSuccDataLinker->initSuccessorLinkDataLinker(resolvedSuccLinkData);
						mergingSuccDataLinker = newMergingSuccDataLinker->append(mergingSuccDataLinker);
						remainMergeableCardHash->insert(resolvedSuccLinkData,newSuccCard);
					}
					return mergingSuccDataLinker;
				}




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::testMergedSuccessorLinkingProblematic(CConceptSaturationDescriptor* conDes, CSaturationSuccessorData* succLinkData, CSaturationSuccessorData* mergedSuccLinkData, CIndividualSaturationProcessNode* resolvedIndiProcSatNode, CLinkedRoleSaturationSuccessorData* succData, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CSaturationSuccessorData* resolvedSuccLinkData = succData->getSuccessorNodeDataMap()->value(resolvedIndiProcSatNode->getIndividualID());
					CRoleBackwardSaturationPropagationHash* propTestBackwardPropHash = resolvedIndiProcSatNode->getRoleBackwardPropagationHash(false);
					if (propTestBackwardPropHash) {
						CReapplyConceptSaturationLabelSet* indiConSet = indiProcSatNode->getReapplyConceptSaturationLabelSet(false);
						CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backwardPropDataHash = propTestBackwardPropHash->getRoleBackwardPropagationDataHash();
						for (CXNegLinker<CRole*>* creationRoleIt = succLinkData->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
							if (!creationRoleIt->isNegated()) {
								CRole* creationRole = creationRoleIt->getData();																			
								for (CSortedNegLinker<CRole*>* superRoleIt = creationRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
									if (!superRoleIt->isNegated()) {
										CRole* superRole = superRoleIt->getData();
										CRoleBackwardSaturationPropagationHashData* backwardPropData = backwardPropDataHash->valuePointer(superRole);
										if (backwardPropData && backwardPropData->mReapplyLinker) {
											for (CBackwardSaturationPropagationReapplyDescriptor* backPropIt = backwardPropData->mReapplyLinker; backPropIt; backPropIt = backPropIt->getNext()) {
												CConceptSaturationDescriptor* backConDes = backPropIt->getReapplyConceptSaturationDescriptor();
												CConcept* concept = backConDes->getConcept();
												bool negation = backConDes->isNegated();
												for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
													CConcept* opConcept = opLinkerIt->getData();
													bool opNegation = opLinkerIt->isNegated()^negation;
													if (!indiConSet->containsConcept(opConcept,opNegation)) {
														return true;
													}
												}
											}
										}
									}
								}
							}
						}
						for (CXNegLinker<CRole*>* creationRoleIt = mergedSuccLinkData->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
							if (!creationRoleIt->isNegated()) {
								CRole* creationRole = creationRoleIt->getData();																			
								for (CSortedNegLinker<CRole*>* superRoleIt = creationRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
									if (!superRoleIt->isNegated()) {
										CRole* superRole = superRoleIt->getData();
										CRoleBackwardSaturationPropagationHashData* backwardPropData = backwardPropDataHash->valuePointer(superRole);
										if (backwardPropData && backwardPropData->mReapplyLinker) {
											for (CBackwardSaturationPropagationReapplyDescriptor* backPropIt = backwardPropData->mReapplyLinker; backPropIt; backPropIt = backPropIt->getNext()) {
												CConceptSaturationDescriptor* backConDes = backPropIt->getReapplyConceptSaturationDescriptor();
												CConcept* concept = backConDes->getConcept();
												bool negation = backConDes->isNegated();
												for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
													CConcept* opConcept = opLinkerIt->getData();
													bool opNegation = opLinkerIt->isNegated()^negation;
													if (!indiConSet->containsConcept(opConcept,opNegation)) {
														return true;
													}
												}
											}
										}
									}
								}
							}
						}
					}


					CConceptSaturationDescriptor* lastConDesIt1 = succLinkData->mSuccIndiNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker();
					CConceptSaturationDescriptor* lastConDesIt2 = mergedSuccLinkData->mSuccIndiNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker();
					for (CConceptSaturationDescriptor* conDesIt = resolvedIndiProcSatNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker(); conDesIt && conDesIt != lastConDesIt1 && conDesIt != lastConDesIt2; conDesIt = conDesIt->getNextConceptDesciptor()) {
						CConcept* concept = conDesIt->getConcept();

						for (CConceptSaturationDescriptor* predConDesIt = indiProcSatNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker(); predConDesIt; predConDesIt = predConDesIt->getNextConceptDesciptor()) {
							CConcept* predConcept = predConDesIt->getConcept();
							bool predConNegation = predConDesIt->isNegated();
							cint64 predConOpCode = predConcept->getOperatorCode();
							if (predConNegation && predConOpCode == CCATLEAST || !predConNegation && predConOpCode == CCATMOST) {
								CConcept* predOpCon = nullptr;
								if (predConcept->getOperandList()) {
									predOpCon = predConcept = predConcept->getOperandList()->getData();
								}
								if (!predOpCon || predOpCon == concept) {
									for (CXNegLinker<CRole*>* creationRoleLinkerIt = succLinkData->mCreationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
										if (!creationRoleLinkerIt->isNegated()) {
											CRole* creationRole = creationRoleLinkerIt->getData();
											for (CSortedNegLinker<CRole*>* creationSuperRoleIt = creationRole->getIndirectSuperRoleList(); creationSuperRoleIt; creationSuperRoleIt = creationSuperRoleIt->getNext()) {
												if (!creationSuperRoleIt->isNegated()) {
													CRole* creationSuperRole = creationSuperRoleIt->getData();
													if (creationSuperRole == predConcept->getRole()) {
														cint64 allowedCardinality = predConcept->getParameter() - 1*predConNegation;
														if (getIndividualNodeQualifiedSuccessorCount(indiProcSatNode,creationSuperRole,predConcept->getOperandList(),calcAlgContext) > allowedCardinality) {
															return true;
														}
													}
												}
											}
										}
									}
									for (CXNegLinker<CRole*>* creationRoleLinkerIt = mergedSuccLinkData->mCreationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
										if (!creationRoleLinkerIt->isNegated()) {
											CRole* creationRole = creationRoleLinkerIt->getData();
											for (CSortedNegLinker<CRole*>* creationSuperRoleIt = creationRole->getIndirectSuperRoleList(); creationSuperRoleIt; creationSuperRoleIt = creationSuperRoleIt->getNext()) {
												if (!creationSuperRoleIt->isNegated()) {
													CRole* creationSuperRole = creationSuperRoleIt->getData();
													if (creationSuperRole == predConcept->getRole()) {
														cint64 allowedCardinality = predConcept->getParameter() - 1*predConNegation;
														if (getIndividualNodeQualifiedSuccessorCount(indiProcSatNode,creationSuperRole,predConcept->getOperandList(),calcAlgContext) > allowedCardinality) {
															return true;
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
					return false;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::tryIndividiualATMOSTConceptSuccessorMerging(CConceptSaturationDescriptor* conDes, CSaturationATMOSTSuccessorMergingHashData* mergingSuccData, bool& nodeInsufficient, bool& ancestorPossiblyCriticalFlag, CIndividualSaturationProcessNode*& functionallyRestrictedSuccessorNode, CXNegLinker<CRole*>*& functionallyRestrictedSuccessorCreationRoleLinker, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					STATINC(SATURATIONCRITICALATMOSTCOUNT,calcAlgContext);
					CConcept* concept = conDes->getConcept();
					bool conceptNegation = conDes->isNegated();
					CRole* role = concept->getRole();
					cint64 allowedCardinality = concept->getParameter() - 1*conceptNegation;
					if (allowedCardinality < 0) {
						nodeInsufficient = true;
						return false;
					}
					if (!indiProcSatNode->hasSubstituteIndividualNode()) {


						CSaturationATMOSTSuccessorMergingData* atmostSuccMergingData = indiProcSatNode->getATMOSTSuccessorMergingData();
						CLinkedRoleSaturationSuccessorHash* linkedSuccHash = atmostSuccMergingData->getMergedLinkedRoleSaturationSuccessorHash();
						collectLinkedSuccessorNodes(indiProcSatNode,calcAlgContext,linkedSuccHash);
						if (linkedSuccHash) {

							cint64& foundCardinality = mergingSuccData->mFoundCardinality;
							cint64& mergeableCardinality = mergingSuccData->mMergeableCardinality;
							cint64& minCardinality = mergingSuccData->mMinCardinality;

							CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
							CLinkedRoleSaturationSuccessorData* succData = succHash->value(role);
							if (succData) {
								if (succData->mSuccCount >= allowedCardinality) {

									CIndividualSaturationSuccessorLinkDataLinker*& mergingSuccDataLinker = mergingSuccData->mSuccessorLinkMergingLinker;


									CIndividualSaturationProcessNode*& lastSuccessorNode = mergingSuccData->mLastSuccessorNode;
									CXNegLinker<CRole*>*& lastSuccessorCreationRoleLinker = mergingSuccData->mLastSuccessorCreationRoleLinker;

									CPROCESSHASH<CSaturationSuccessorData*,cint64>* remainMergeableCardHash = atmostSuccMergingData->getRemainingMergeableCardinalityHash();

									CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>* mergeDistintHash = atmostSuccMergingData->getMergingDistintHash();
									CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >* mergeDistintSet = atmostSuccMergingData->getMergingDistintSet();

									if (!mergingSuccData->mInitialized) {
										mergingSuccData->mInitialized = true;


										foundCardinality = collectATMOSTConceptRelevantSuccessors(conDes,indiProcSatNode,succData,mergingSuccDataLinker,lastSuccessorNode,lastSuccessorCreationRoleLinker,minCardinality,calcAlgContext);

										if (foundCardinality >= allowedCardinality && foundCardinality > 1) {

											// check whether some trivial merging is possible

											if (mergingSuccDataLinker) {

												if (mConfSimpleMergingTestForATMOSTCriticalTesting) {
													for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker; mergingSuccDataLinkerIt && foundCardinality-mergeableCardinality >= allowedCardinality; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
														CSaturationSuccessorData* succLinkData = mergingSuccDataLinkerIt->getData();
														cint64 succCount = succLinkData->mSuccCount;
														if (succCount >= 1) {
															cint64 maxRequiredMergingCardinality = foundCardinality-mergeableCardinality-(allowedCardinality-1);
															cint64 mergingCardinality = getSuccessorLinkSimplyMergeableCardinalityCount(indiProcSatNode,succLinkData,mergingSuccDataLinker,remainMergeableCardHash,role,maxRequiredMergingCardinality,mergeDistintHash,mergeDistintSet,calcAlgContext);
															cint64 remainingCardinality = succLinkData->mSuccCount-mergingCardinality;
															remainMergeableCardHash->insert(succLinkData,remainingCardinality);
															if (remainingCardinality <= 0) {
																for (CXNegLinker<CRole*>* creationRoleIt = succLinkData->mCreationRoleLinker; creationRoleIt; creationRoleIt = creationRoleIt->getNext()) {
																	if (!creationRoleIt->isNegated()) {
																		CRole* creationRole = creationRoleIt->getData();
																		for (CSortedNegLinker<CRole*>* superRoleIt = creationRole->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
																			if (!superRoleIt->isNegated()) {
																				CRole* superRole = superRoleIt->getData();
																				linkedSuccHash->deactivateLinkedSuccessor(superRole,succLinkData->mSuccIndiNode,creationRole);
																			}
																		}
																	}
																}	
															}
															mergeableCardinality += mergingCardinality;
														}
													}
												}



												if (mConfDetailedMergingTestForATMOSTCriticalTesting && foundCardinality-mergeableCardinality >= allowedCardinality && foundCardinality-mergeableCardinality <= allowedCardinality*2) {
													for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker; mergingSuccDataLinkerIt && foundCardinality-mergeableCardinality >= allowedCardinality; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
														CSaturationSuccessorData* succLinkData = mergingSuccDataLinkerIt->getData();
														CSaturationSuccessorData* mergedSuccLinkData = nullptr;
														if (succLinkData->mSuccCount >= 1) {
															cint64 succRemainingCardinality = remainMergeableCardHash->value(succLinkData,succLinkData->mSuccCount);
															if (succRemainingCardinality > 0) {
																cint64 maxRequiredMergingCardinality = foundCardinality-mergeableCardinality-(allowedCardinality-1);

																cint64 mergingCardinality = getSuccessorLinkExtendedMergeableCardinalityCount(indiProcSatNode,succLinkData,&mergedSuccLinkData,mergingSuccDataLinkerIt->getNext(),remainMergeableCardHash,role,maxRequiredMergingCardinality,mergeDistintHash,mergeDistintSet,calcAlgContext);
																if (mergingCardinality > 0) {
																	cint64 newSuccCard = qMax(succRemainingCardinality,mergingCardinality);

																	CIndividualSaturationProcessNode* copyIndiProcSatNode = succLinkData->mSuccIndiNode;
																	CSaturationIndividualNodeExtensionResolveData* resolveData = copyIndiProcSatNode->getSuccessorExtensionData(true)->getBaseExtensionResolveData(true);
																	resolveData = getResolvedIndividualNodeExtension(resolveData,mergedSuccLinkData->mSuccIndiNode,copyIndiProcSatNode,calcAlgContext);

																	
																	CIndividualSaturationProcessNode* resolvedIndiProcSatNode = resolveData->getProcessingIndividualNode();
																	cint64 incrSuccCard = qMax(mergingCardinality,succRemainingCardinality)-qMin(mergingCardinality,succRemainingCardinality);
																	CIndividualSaturationSuccessorLinkDataLinker* newMergingSuccDataLinker = reconnectMergedLinkedSuccessors(succLinkData,mergedSuccLinkData,newSuccCard,incrSuccCard,linkedSuccHash,succData,mergeDistintHash,mergeDistintSet,remainMergeableCardHash,indiProcSatNode,resolvedIndiProcSatNode,calcAlgContext);


																	if (newMergingSuccDataLinker) {
																		mergingSuccDataLinker = newMergingSuccDataLinker->append(mergingSuccDataLinker);
																	}

																	



																	cint64 removedSuccCard = qMin(succRemainingCardinality,mergingCardinality);
																	mergeableCardinality += removedSuccCard;
																}
															}
														}
													}
												}
											}
										}
									}



									bool nodeExpansionRequired = false;
									while (mergingSuccDataLinker && foundCardinality-mergeableCardinality >= allowedCardinality && !nodeExpansionRequired) {

										CSaturationSuccessorData* succLinkData = mergingSuccDataLinker->getData();
										if (succLinkData->mSuccCount >= 1) {
											cint64 succRemainingCardinality = remainMergeableCardHash->value(succLinkData,succLinkData->mSuccCount);
											if (succRemainingCardinality > 0) {

												for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker->getNext(); mergingSuccDataLinkerIt && foundCardinality-mergeableCardinality >= allowedCardinality && foundCardinality-mergeableCardinality > minCardinality; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
													CSaturationSuccessorData* mergeSuccLinkData = mergingSuccDataLinkerIt->getData();

													if (mergeSuccLinkData->mSuccCount >= 1) {
														if (!mergeDistintSet->contains(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(mergeSuccLinkData,succLinkData),qMax(mergeSuccLinkData,succLinkData)))) {

															cint64 mergingCardinality = remainMergeableCardHash->value(mergeSuccLinkData,mergeSuccLinkData->mSuccCount);
															if (mergingCardinality > 0) {


																bool newNodeExpansionCreated = false;
																CIndividualSaturationProcessNode* copyIndiProcSatNode = succLinkData->mSuccIndiNode;
																CSaturationIndividualNodeExtensionResolveData* resolveData = copyIndiProcSatNode->getSuccessorExtensionData(true)->getBaseExtensionResolveData(true);
																resolveData = getResolvedIndividualNodeExtension(resolveData,mergeSuccLinkData->mSuccIndiNode,copyIndiProcSatNode,&newNodeExpansionCreated,calcAlgContext);

																if (newNodeExpansionCreated) {
																	nodeExpansionRequired = true;
																} else {

																	CIndividualSaturationProcessNode* resolvedIndiProcSatNode = resolveData->getProcessingIndividualNode();

																	CIndividualSaturationProcessNodeStatusFlags* resolvedNodesFlags = resolvedIndiProcSatNode->getIndirectStatusFlags();
																	if (!resolvedNodesFlags->hasClashedFlag() && !resolvedNodesFlags->hasInsufficientFlag()) {

																		CIndividualSaturationProcessNode* resolvedIndiProcSatNode = resolveData->getProcessingIndividualNode();
																		if (!testMergedSuccessorLinkingProblematic(conDes,succLinkData,mergeSuccLinkData,resolvedIndiProcSatNode,succData,indiProcSatNode,calcAlgContext)) {
																			// use new merged successor node instead of the previous ones

																			cint64 newSuccCard = qMax(mergingCardinality,succRemainingCardinality);
																			cint64 incrSuccCard = qMax(mergingCardinality,succRemainingCardinality)-qMin(mergingCardinality,succRemainingCardinality);
																			CIndividualSaturationSuccessorLinkDataLinker* newMergingSuccDataLinker = reconnectMergedLinkedSuccessors(succLinkData,mergeSuccLinkData,newSuccCard,incrSuccCard,linkedSuccHash,succData,mergeDistintHash,mergeDistintSet,remainMergeableCardHash,indiProcSatNode,resolvedIndiProcSatNode,calcAlgContext);

																			if (newMergingSuccDataLinker) {
																				mergingSuccDataLinker = newMergingSuccDataLinker->append(mergingSuccDataLinker);
																			}
																			cint64 removedSuccCard = qMin(succRemainingCardinality,mergingCardinality);
																			mergeableCardinality += removedSuccCard;
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




									if (foundCardinality-mergeableCardinality == allowedCardinality || minCardinality >= allowedCardinality) {
										ancestorPossiblyCriticalFlag = true;
										if (allowedCardinality == 1) {
											functionallyRestrictedSuccessorNode = lastSuccessorNode;
											functionallyRestrictedSuccessorCreationRoleLinker = lastSuccessorCreationRoleLinker;
										}
									}
									if (foundCardinality-mergeableCardinality > allowedCardinality) {
										nodeInsufficient = true;
									}

									return nodeExpansionRequired;

								}
							}
						}
					}
					return false;
				}













				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isIndividualSuccessorLinkCardinalityMergeable(CSaturationSuccessorData* subsetIndiSuccData, CSaturationSuccessorData* superIndiSuccData, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* subsetIndiSuccNode = subsetIndiSuccData->mSuccIndiNode;
					CIndividualSaturationProcessNode* superIndiSuccNode = superIndiSuccData->mSuccIndiNode;
					return isIndividualSuccessorLinkCardinalityMergeable(subsetIndiSuccNode,subsetIndiSuccData,superIndiSuccNode,superIndiSuccData,calcAlgContext);
				}

				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isIndividualSuccessorLinkCardinalityMergeable(CIndividualSaturationProcessNode* subsetIndiSuccNode, CSaturationSuccessorData* subsetIndiSuccData, CIndividualSaturationProcessNode* superIndiSuccNode, CSaturationSuccessorData* superIndiSuccData, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (subsetIndiSuccData->mVALUENominalConnection || superIndiSuccData->mVALUENominalConnection) {
						return false;
					}
					if (subsetIndiSuccNode->hasNominalIntegrated() || superIndiSuccNode->hasNominalIntegrated()) {
						return false;
					}
					if (subsetIndiSuccNode->isABoxIndividualRepresentationNode() || superIndiSuccNode->isABoxIndividualRepresentationNode()) {
						return false;
					}
					if (subsetIndiSuccNode->hasDataValueApplied() || superIndiSuccNode->hasDataValueApplied()) {
						return false;
					}
					if (!isSuccessorCreationRoleMergingSubset(subsetIndiSuccData->mCreationRoleLinker,superIndiSuccData->mCreationRoleLinker,calcAlgContext)) {
						return false;
					}
					if (!isIndividualNodeLabelMergingSubset(subsetIndiSuccNode,superIndiSuccNode,true,calcAlgContext)) {
						return false;
					}
					return true;
				}




				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getSuccessorLinkSimplyMergeableCardinalityCount(CIndividualSaturationProcessNode*& indiProcSatNode, CSaturationSuccessorData* succLinkData, CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinker, CPROCESSHASH<CSaturationSuccessorData*,cint64>* remainMergeableCardHash, CRole* role, cint64 maxRequiredMergingCardinality, CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>* mergeDistintHash, CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >* mergeDistintSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					cint64 remainingCardinality = succLinkData->mSuccCount;

					bool intoAllMergeable = false;
					bool intoAllMergeableChecked = false;

					for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker; mergingSuccDataLinkerIt; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
						CSaturationSuccessorData* mergeSuccLinkData = mergingSuccDataLinkerIt->getData();
						if (mergeSuccLinkData != succLinkData) {
							cint64 mergeableCardinality = remainMergeableCardHash->value(mergeSuccLinkData,mergeSuccLinkData->mSuccCount);
							if (mergeableCardinality >= succLinkData->mSuccCount) {
								if (!mergeDistintSet->contains(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(mergeSuccLinkData,succLinkData),qMax(mergeSuccLinkData,succLinkData)))) {
									if (isIndividualSuccessorLinkCardinalityMergeable(succLinkData,mergeSuccLinkData,calcAlgContext)) {
										return remainingCardinality;
									} else {
										intoAllMergeableChecked = true;
									}
								} else {
									intoAllMergeableChecked = true;
								}
							}
						}
					}



					cint64 mergedCardinality = 0;
					for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker; mergingSuccDataLinkerIt && remainingCardinality > 0 && mergedCardinality < maxRequiredMergingCardinality; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
						CSaturationSuccessorData* mergeSuccLinkData = mergingSuccDataLinkerIt->getData();
						if (mergeSuccLinkData != succLinkData) {
							cint64 mergeableCardinality = remainMergeableCardHash->value(mergeSuccLinkData,mergeSuccLinkData->mSuccCount);
							if (mergeableCardinality > 0 && mergeableCardinality < succLinkData->mSuccCount) {

								if (!mergeDistintSet->contains(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(mergeSuccLinkData,succLinkData),qMax(mergeSuccLinkData,succLinkData)))) {

									if (intoAllMergeable || isIndividualSuccessorLinkCardinalityMergeable(succLinkData,mergeSuccLinkData,calcAlgContext)) {

										if (!intoAllMergeableChecked) {
											intoAllMergeable = true;
											for (CIndividualSaturationSuccessorLinkDataLinker* remTestSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext(); remTestSuccDataLinkerIt; remTestSuccDataLinkerIt = remTestSuccDataLinkerIt->getNext()) {
												CSaturationSuccessorData* remTestSuccLinkData = remTestSuccDataLinkerIt->getData();
												if (remTestSuccLinkData != succLinkData && remTestSuccLinkData != mergeSuccLinkData) {
													cint64 mergeableCardinality = remainMergeableCardHash->value(remTestSuccLinkData,remTestSuccLinkData->mSuccCount);
													if (mergeableCardinality > 0 && mergeableCardinality < succLinkData->mSuccCount) {

														if (!mergeDistintSet->contains(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(remTestSuccLinkData,succLinkData),qMax(remTestSuccLinkData,succLinkData)))) {

															if (!isIndividualSuccessorLinkCardinalityMergeable(succLinkData,mergeSuccLinkData,calcAlgContext)) {
																intoAllMergeable = false;
															}
														}
													}
												}
											}
											intoAllMergeableChecked = true;
										}

										if (!intoAllMergeable && succLinkData->mSuccCount > 1) {
											mergeDistintSet->insert(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(mergeSuccLinkData,succLinkData),qMax(mergeSuccLinkData,succLinkData)));
											for (CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>::const_iterator mDIt = mergeDistintHash->constFind(succLinkData), mDItEnd = mergeDistintHash->constEnd(); mDIt != mDItEnd && mDIt.key() == succLinkData; ++mDIt) {
												CSaturationSuccessorData* distSuccData = mDIt.value();
												mergeDistintHash->insertMulti(distSuccData,mergeSuccLinkData);
												mergeDistintHash->insertMulti(mergeSuccLinkData,distSuccData);
												mergeDistintSet->insert(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(mergeSuccLinkData,distSuccData),qMax(mergeSuccLinkData,distSuccData)));
											}
											mergeDistintHash->insertMulti(succLinkData,mergeSuccLinkData);
											mergeDistintHash->insertMulti(mergeSuccLinkData,succLinkData);
										}

										cint64 mergingCardinality = qMin(remainingCardinality,mergeableCardinality);
										remainingCardinality -= mergingCardinality;
										mergedCardinality += mergingCardinality;
									} else {
										intoAllMergeableChecked = true;
									}
								} else {
									intoAllMergeableChecked = true;
								}
							}
						}
					}
					return mergedCardinality;
				}


















				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isIndividualSuccessorLinkCardinalityExtendedMergeable(CIndividualSaturationProcessNode*& indiProcSatNode, CSaturationSuccessorData* indiSuccData1, CSaturationSuccessorData* indiSuccData2, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* indiSuccNode1 = indiSuccData1->mSuccIndiNode;
					CIndividualSaturationProcessNode* indiSuccNode2 = indiSuccData2->mSuccIndiNode;
					return isIndividualSuccessorLinkCardinalityExtendedMergeable(indiProcSatNode,indiSuccNode1,indiSuccData1,indiSuccNode2,indiSuccData2,calcAlgContext);
				}

				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isIndividualSuccessorLinkCardinalityExtendedMergeable(CIndividualSaturationProcessNode*& indiProcSatNode, CIndividualSaturationProcessNode* indiSuccNode1, CSaturationSuccessorData* indiSuccData1, CIndividualSaturationProcessNode* indiSuccNode2, CSaturationSuccessorData* indiSuccData2, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (indiSuccData1->mVALUENominalConnection || indiSuccData2->mVALUENominalConnection) {
						return false;
					}
					if (indiSuccNode1->hasNominalIntegrated() || indiSuccNode2->hasNominalIntegrated()) {
						return false;
					}
					if (indiSuccNode1->hasDataValueApplied() || indiSuccNode2->hasDataValueApplied()) {
						return false;
					}
					if (indiSuccNode1->isABoxIndividualRepresentationNode() || indiSuccNode2->isABoxIndividualRepresentationNode()) {
						return false;
					}
					if (!isSuccessorCreationRoleMergingSubset(indiSuccData1->mCreationRoleLinker,indiSuccData2->mCreationRoleLinker,calcAlgContext)) {
						return false;
					}
					if (!isSuccessorCreationRoleMergingSubset(indiSuccData2->mCreationRoleLinker,indiSuccData1->mCreationRoleLinker,calcAlgContext)) {
						return false;
					}


					

					if (isIndividualNodeLabelMergingProblematic(indiProcSatNode,indiSuccNode1,indiSuccNode2,indiSuccData1->mCreationRoleLinker,calcAlgContext)) {
						return false;
					}
					if (isIndividualNodeLabelMergingProblematic(indiProcSatNode,indiSuccNode2,indiSuccNode1,indiSuccData2->mCreationRoleLinker,calcAlgContext)) {
						return false;
					}
					return true;
				}

				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getIndividualNodeQualifiedSuccessorCount(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CSortedNegLinker<CConcept*>* conQualificationLinker, CCalculationAlgorithmContextBase* calcAlgContext) {

					cint64 matchingSuccCount = 0;
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = indiProcSatNode->getLinkedRoleSuccessorHash(false);
					if (linkedSuccHash) {

						CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
						CLinkedRoleSaturationSuccessorData* predSuccData = succHash->value(role);
						if (predSuccData) {

							bool trivialQualification = true;
							for (CSortedNegLinker<CConcept*>* opLinkerIt = conQualificationLinker; trivialQualification && opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
								CConcept* opConcept = opLinkerIt->getData();
								bool opConceptNegation = opLinkerIt->isNegated();
								cint64 opCode = opConcept->getOperatorCode();
								if (opConceptNegation || (opCode != CCATOM && opCode != CCSUB)) {
									trivialQualification = false;
								}
							}

							CIndividualSaturationProcessNode* lastSuccessorNode = nullptr;
							CXNegLinker<CRole*>* lastSuccessorCreationRoleLinker = nullptr;
							for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator itSucc = predSuccData->mSuccNodeDataMap.constBegin(), itSuccEnd = predSuccData->mSuccNodeDataMap.constEnd(); itSucc != itSuccEnd; ++itSucc) {
								CSaturationSuccessorData* succRoleData = itSucc.value();
								if (succRoleData->mActiveCount >= 1) {
									cint64 succCardinality(succRoleData->mSuccCount);
									bool operantsContainedNegative = true;
									bool operantsContainedPositive = true;
									bool operantsContained = true;
									if (succRoleData->mVALUENominalConnection) {
										operantsContainedPositive = true;
									} else {
										CIndividualSaturationProcessNode* succNode(succRoleData->mSuccIndiNode);			
										lastSuccessorNode = succNode;
										lastSuccessorCreationRoleLinker = succRoleData->mCreationRoleLinker;
										CReapplyConceptSaturationLabelSet* succConSet = succNode->getReapplyConceptSaturationLabelSet(false);
										if (succConSet) {
											if (conQualificationLinker) {
												for (CSortedNegLinker<CConcept*>* opLinkerIt = conQualificationLinker; opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
													CConcept* opConcept = opLinkerIt->getData();
													bool opConceptNegation = opLinkerIt->isNegated();
													bool containedNegation = false;
													if (succConSet->containsConcept(opConcept,&containedNegation)) {
														if (containedNegation == opConceptNegation) {
															operantsContainedNegative = false;
														} else {
															operantsContainedPositive = false;
														}
													} else {
														if (trivialQualification) {
															operantsContainedPositive = false;
														} else {
															operantsContained = false;
														}
													}
												}
											} else {
												operantsContainedNegative = false;

											}
										} else {
											if (trivialQualification) {
												operantsContainedPositive = false;
											} else {
												operantsContained = false;
											}
										}
									}
									if (operantsContainedPositive || !operantsContained) {
										matchingSuccCount += succCardinality;
									}
								}
							}
						}
					}
					return matchingSuccCount;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isIndividualNodeLabelMergingProblematic(CIndividualSaturationProcessNode*& indiProcSatNode, CIndividualSaturationProcessNode* mergingSuccNode, CIndividualSaturationProcessNode* probTestingSuccNode, CXNegLinker<CRole*>* creationRoleLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* mergingConSet = mergingSuccNode->getReapplyConceptSaturationLabelSet(false);
					CReapplyConceptSaturationLabelSet* propTestConSet = probTestingSuccNode->getReapplyConceptSaturationLabelSet(false);
					for (CConceptSaturationDescriptor* conDesIt = mergingConSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNextConceptDesciptor()) {
						CConcept* concept = conDesIt->getConcept();
						bool negation = conDesIt->isNegated();
						CConceptSaturationDescriptor* propTestConDes = nullptr;
						CImplicationReapplyConceptSaturationDescriptor* propTestImpReapDes = nullptr;
						if (propTestConSet->getConceptDescriptorAndReapplyQueue(concept,propTestConDes,propTestImpReapDes)) {
							if (propTestConDes) {
								if (propTestConDes->isNegated() != negation) {
									return true;
								}
							} else if (!negation && propTestImpReapDes) {
								CConcept* propTestImpCon = propTestImpReapDes->getImplicationConcept();
								if (!propTestConSet->containsConcept(propTestImpCon->getOperandList()->getData()) && !mergingConSet->containsConcept(propTestImpCon->getOperandList()->getData())) {
									return true;
								}
							}
						} else {
							// test whether concept is critical

							for (CConceptSaturationDescriptor* predConDesIt = indiProcSatNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker(); predConDesIt; predConDesIt = predConDesIt->getNextConceptDesciptor()) {
								CConcept* predConcept = predConDesIt->getConcept();
								bool predConNegation = predConDesIt->isNegated();
								cint64 predConOpCode = predConcept->getOperatorCode();
								if (predConNegation && predConOpCode == CCATLEAST || !predConNegation && predConOpCode == CCATMOST) {
									CConcept* predOpCon = nullptr;
									if (predConcept->getOperandList()) {
										predOpCon = predConcept = predConcept->getOperandList()->getData();
									}
									if (!predOpCon || predOpCon == concept) {
										for (CXNegLinker<CRole*>* creationRoleLinkerIt = creationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
											if (!creationRoleLinkerIt->isNegated()) {
												CRole* creationRole = creationRoleLinkerIt->getData();
												for (CSortedNegLinker<CRole*>* creationSuperRoleIt = creationRole->getIndirectSuperRoleList(); creationSuperRoleIt; creationSuperRoleIt = creationSuperRoleIt->getNext()) {
													if (!creationSuperRoleIt->isNegated()) {
														CRole* creationSuperRole = creationSuperRoleIt->getData();
														if (creationSuperRole == predConcept->getRole()) {
															cint64 allowedCardinality = predConcept->getParameter() - 1*predConNegation;
															if (getIndividualNodeQualifiedSuccessorCount(indiProcSatNode,creationSuperRole,predConcept->getOperandList(),calcAlgContext) > allowedCardinality) {
																return true;
															}
														}
													}
												}
											}
										}
									}
								}
							}


							cint64 opCode = concept->getOperatorCode();
							CConceptOperator* conOp = concept->getConceptOperator();

							if (!negation && conOp->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQALL_TYPE) || negation && conOp->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_SOME_TYPE) || !negation && opCode == CCATMOST || negation && opCode == CCATLEAST) {
								collectLinkedSuccessorNodes(probTestingSuccNode,calcAlgContext);
								CLinkedRoleSaturationSuccessorHash* propTestLinkedSuccHash = probTestingSuccNode->getLinkedRoleSuccessorHash(false);
								if (propTestLinkedSuccHash) {
									CRole* role = concept->getRole();
									if (propTestLinkedSuccHash->hasLinkedRoleSuccessorData(role)) {
										return true;
									}
								}
							}


							if (!negation && conOp->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_SOME_TYPE | CConceptOperator::CCF_SELF | CConceptOperator::CCF_ATLEAST) || negation && conOp->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQALL_TYPE | CConceptOperator::CCF_ATMOST)) {
								CRoleBackwardSaturationPropagationHash* propTestBackwardPropHash = probTestingSuccNode->getRoleBackwardPropagationHash(false);
								if (propTestBackwardPropHash) {
									CRole* role = concept->getRole();
									CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backwardPropDataHash = propTestBackwardPropHash->getRoleBackwardPropagationDataHash();
									for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
										if (!superRoleIt->isNegated()) {
											CRole* superRole = superRoleIt->getData();
											CRoleBackwardSaturationPropagationHashData* backwardPropData = backwardPropDataHash->valuePointer(superRole);
											if (backwardPropData && backwardPropData->mReapplyLinker) {
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




				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getSuccessorLinkExtendedMergeableCardinalityCount(CIndividualSaturationProcessNode*& indiProcSatNode, CSaturationSuccessorData* succLinkData, CSaturationSuccessorData** mergedSuccLinkData, CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinker, CPROCESSHASH<CSaturationSuccessorData*,cint64>* remainMergeableCardHash, CRole* role, cint64 maxRequiredMergingCardinality, CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>* mergeDistintHash, CPROCESSSET< QPair<CSaturationSuccessorData*,CSaturationSuccessorData*> >* mergeDistintSet, CCalculationAlgorithmContextBase* calcAlgContext) {
					for (CIndividualSaturationSuccessorLinkDataLinker* mergingSuccDataLinkerIt = mergingSuccDataLinker; mergingSuccDataLinkerIt; mergingSuccDataLinkerIt = mergingSuccDataLinkerIt->getNext()) {
						CSaturationSuccessorData* mergeSuccLinkData = mergingSuccDataLinkerIt->getData();
						cint64 mergeableCardinality = remainMergeableCardHash->value(mergeSuccLinkData,mergeSuccLinkData->mSuccCount);
						if (mergeableCardinality > 0) {
							if (!mergeDistintSet->contains(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(mergeSuccLinkData,succLinkData),qMax(mergeSuccLinkData,succLinkData)))) {
								if (isIndividualSuccessorLinkCardinalityExtendedMergeable(indiProcSatNode,succLinkData,mergeSuccLinkData,calcAlgContext)) {
									remainMergeableCardHash->insert(mergeSuccLinkData,0);
									for (CPROCESSHASH<CSaturationSuccessorData*,CSaturationSuccessorData*>::const_iterator mDIt = mergeDistintHash->constFind(succLinkData), mDItEnd = mergeDistintHash->constEnd(); mDIt != mDItEnd && mDIt.key() == succLinkData; ++mDIt) {
										CSaturationSuccessorData* distSuccData = mDIt.value();
										mergeDistintHash->insertMulti(distSuccData,mergeSuccLinkData);
										mergeDistintHash->insertMulti(mergeSuccLinkData,distSuccData);
										mergeDistintSet->insert(QPair<CSaturationSuccessorData*,CSaturationSuccessorData*>(qMin(mergeSuccLinkData,distSuccData),qMax(mergeSuccLinkData,distSuccData)));
									}
									if (mergedSuccLinkData) {
										*mergedSuccLinkData = mergeSuccLinkData;
									}
									return mergeableCardinality;
								}
							}
						}
					}
					return 0;
				}











				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isCriticalNOMINALConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CIndividual* nominal = concept->getNominalIndividual();
					cint64 nominalID = nominal->getIndividualID();

					if (isConsistenceDataAvailable(calcAlgContext)) {
						CIndividualProcessNode* detNominalProcessNode = getCorrectedNode(nominalID,mDetCachedCGIndiVector,mCalcAlgContext);
						if (detNominalProcessNode) {
							CIndividualProcessNode* nonDetNominalProcessNode = getCorrectedNode(nominalID,mNonDetCachedCGIndiVector,mCalcAlgContext);
							CReapplyConceptLabelSet* detNominalReapplyConSet = detNominalProcessNode->getReapplyConceptLabelSet(false);
							CReapplyConceptLabelSet* nonDetNominalReapplyConSet = nonDetNominalProcessNode->getReapplyConceptLabelSet(false);

							CReapplyConceptSaturationLabelSet* satIndiNodeConSet = indiProcSatNode->getReapplyConceptSaturationLabelSet(false);

							CConceptDescriptor* lastDetConDesIt = detNominalReapplyConSet->getAddingSortedConceptDescriptionLinker();
							for (CConceptDescriptor* conDesIt = nonDetNominalReapplyConSet->getAddingSortedConceptDescriptionLinker(); conDesIt && conDesIt != lastDetConDesIt; conDesIt = conDesIt->getNext()) {
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
					} else {
						return true;
					}
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isCriticalVALUEConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					CConcept* concept = conDes->getConcept();
					CRole* role = concept->getRole();
					CIndividual* nominal = concept->getNominalIndividual();
					cint64 nominalID = nominal->getIndividualID();

					if (isConsistenceDataAvailable(calcAlgContext)) {
						CIndividualProcessNode* detNominalProcessNode = getCorrectedNode(nominalID,mDetCachedCGIndiVector,mCalcAlgContext);
						if (detNominalProcessNode) {
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
					CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)indiProcSatNode->getSaturationConceptReferenceLinking();
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
						CSaturationConceptReferenceLinking* disjunctConceptRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(disjunctNegation);
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





				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createRoleAssertionLink(CIndividualSaturationProcessNode*& sourceNode, CIndividualSaturationProcessNode*& destinationNode, CRole* role, bool roleInversed, CCalculationAlgorithmContextBase* calcAlgContext) {
					// status propagation not required since it is tracked for each abox individual separately
					//updateIndirectAddingIndividualStatusFlags(sourceNode,destinationNode->getIndirectStatusFlags(),mCalcAlgContext);

					CMemoryAllocationManager* taskMemMan = mCalcAlgContext->getUsedProcessTaskMemoryAllocationManager();
					bool existIndiInitialized = destinationNode->isInitialized() || destinationNode == sourceNode;
					CReapplyConceptSaturationLabelSet* conSet = nullptr;
					CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
					bool connected = false;
					while (superRoleIt) {
						CRole* superRole = superRoleIt->getData();
						if (superRole->getDisjointRoleList() != nullptr) {
							updateDirectAddingIndividualStatusFlags(sourceNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
							setInsufficientNodeOccured(mCalcAlgContext);
						}
						CSortedNegLinker<CConcept*>* domainConLinker = superRole->getDomainRangeConceptList(superRoleIt->isNegated()^roleInversed);
						for (CSortedNegLinker<CConcept*>* domainConLinkerIt = domainConLinker; domainConLinkerIt; domainConLinkerIt = domainConLinkerIt->getNext()) {
							CConcept* domainConcept = domainConLinkerIt->getData();
							bool domainConceptNegation = domainConLinkerIt->isNegated();
							if (!conSet) {
								conSet = sourceNode->getReapplyConceptSaturationLabelSet(true);
							}
							addConceptFilteredToIndividual(domainConcept,domainConceptNegation,sourceNode,conSet,false,mCalcAlgContext);
						}


						if (superRoleIt->isNegated()^roleInversed) {
							connected = true;
							CBackwardSaturationPropagationLink* backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(taskMemMan);
							backPropLink->initBackwardPropagationLink(sourceNode,superRole);
							if (!existIndiInitialized) {
								destinationNode->addInitializingBackwardPropagationLinks(backPropLink);
							} else {
								installBackwardPropagationLink(sourceNode,destinationNode,superRole,backPropLink,true,true,calcAlgContext);
							}
						} else {
							addNewLinkedExtensionProcessingRole(superRole,sourceNode,true,true,mCalcAlgContext);
						}
						superRoleIt = superRoleIt->getNext();
					}
					if (!connected) {
						CXLinker<CIndividualSaturationProcessNode*>* nonInvConnectedIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(taskMemMan);
						nonInvConnectedIndiNodeLinker->initLinker(sourceNode);
						destinationNode->addNonInverseConnectedIndividualNodeLinker(nonInvConnectedIndiNodeLinker);
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::initializeRoleAssertions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					CIndividual* nominalIndi = indiProcSatNode->getNominalIndividual();
					if (nominalIndi) {

						cint64 saturationID = getSaturationIDForIndividualNode(nominalIndi,calcAlgContext);

						//if (CIRIName::getRecentIRIName(nominalIndi->getIndividualNameLinker()) == "http://www.biopax.org/release/biopax-level2.owl#protein1189") {
						//	bool bug = true;
						//}

						CIndividualSaturationProcessNode* resolveNode = nullptr;
						if (indiProcSatNode->isSeparated()) {
							resolveNode = getSeparatedSaturationConceptAssertionResolveNode(calcAlgContext);
						} else {
							CConcept* topConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
							resolveNode = getIndividualNodeForConcept(topConcept,false,calcAlgContext);
						}

						CIndividualSaturationProcessNodeExtensionData* extensionData = indiProcSatNode->getIndividualExtensionData(true);
						CLinkedNeighbourRoleAssertionSaturationHash* neighbourRoleAssertionHash = extensionData->getLinkedNeighbourRoleAssertionHash(true);

						for (CRoleAssertionLinker* assRoleLinkerIt = nominalIndi->getAssertionRoleLinker(); assRoleLinkerIt; assRoleLinkerIt = assRoleLinkerIt->getNext()) {
							CRole* role = assRoleLinkerIt->getRole();
							CIndividual* othIndi = assRoleLinkerIt->getIndividual();


							CIndividualSaturationProcessNode* othIndiNode = getIndividualNodeForIndividual(indiProcSatNode,othIndi,saturationID,calcAlgContext);
							if (othIndiNode && (othIndiNode->isInitialized() || othIndiNode == indiProcSatNode)) {
								createRoleAssertionLink(indiProcSatNode,othIndiNode,role,false,calcAlgContext);
								indiProcSatNode->addRoleAssertion(othIndiNode,role,false);
								createRoleAssertionLink(othIndiNode,indiProcSatNode,role,true,calcAlgContext);
								othIndiNode->addRoleAssertion(indiProcSatNode,role,true);
							} else if (!othIndiNode) {
								othIndiNode = getResolvedIndividualNodeRepresentativeRangeAssertion(resolveNode,othIndi,role,false,calcAlgContext);
								createRoleAssertionLink(indiProcSatNode, othIndiNode,role,false,calcAlgContext);
								indiProcSatNode->addRoleAssertion(othIndiNode,role,false);
							}
							neighbourRoleAssertionHash->addNeighbourRoleAssertion(nominalIndi->getIndividualID(), othIndi->getIndividualID(), role, false, othIndiNode);
						}

						for (CReverseRoleAssertionLinker* reverseAssRoleLinkerIt = nominalIndi->getReverseAssertionRoleLinker(); reverseAssRoleLinkerIt; reverseAssRoleLinkerIt = reverseAssRoleLinkerIt->getNext()) {
							CRole* role = reverseAssRoleLinkerIt->getRole();
							CIndividual* othIndi = reverseAssRoleLinkerIt->getIndividual();


							CIndividualSaturationProcessNode* othIndiNode = getIndividualNodeForIndividual(indiProcSatNode,othIndi,saturationID,calcAlgContext);
							if (othIndiNode && othIndiNode->isInitialized()) {								
								createRoleAssertionLink(othIndiNode,indiProcSatNode,role,false,calcAlgContext);
								othIndiNode->addRoleAssertion(indiProcSatNode,role,false);
								createRoleAssertionLink(indiProcSatNode,othIndiNode,role,true,calcAlgContext);
								indiProcSatNode->addRoleAssertion(othIndiNode,role,true);
							} else if (!othIndiNode) {
								othIndiNode = getResolvedIndividualNodeRepresentativeRangeAssertion(resolveNode,othIndi,role,true,calcAlgContext);
								createRoleAssertionLink(indiProcSatNode, othIndiNode,role,true,calcAlgContext);
								indiProcSatNode->addRoleAssertion(othIndiNode,role,true);
							}
							neighbourRoleAssertionHash->addNeighbourRoleAssertion(nominalIndi->getIndividualID(), othIndi->getIndividualID(), role, true, othIndiNode);

						}
					}
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::initializeDataAssertions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {

					CIndividual* nominalIndi = indiProcSatNode->getNominalIndividual();
					if (nominalIndi) {

						cint64 saturationID = getSaturationIDForIndividualNode(nominalIndi, calcAlgContext);

						CIndividualSaturationProcessNode* resolveNode = nullptr;
						if (indiProcSatNode->isSeparated()) {
							resolveNode = getSeparatedSaturationConceptAssertionResolveNode(calcAlgContext);
						} else {
							CConcept* topConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
							resolveNode = getIndividualNodeForConcept(topConcept, false, calcAlgContext);
						}

						for (CDataAssertionLinker* assDataLinkerIt = nominalIndi->getAssertionDataLinker(); assDataLinkerIt; assDataLinkerIt = assDataLinkerIt->getNext()) {
							CRole* role = assDataLinkerIt->getRole();
							CDataLiteral* dataLiteral = assDataLinkerIt->getDataLiteral();

							createSuccessorForDataLiteral(indiProcSatNode, role, dataLiteral, calcAlgContext);							
						}
					}
				}






				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createSuccessorForDataLiteral(CIndividualSaturationProcessNode*& processIndi, CRole* role, CDataLiteral* dataLiteral, CCalculationAlgorithmContextBase* calcAlgContext) {


					cint64 nextResolveIndiID = calcAlgContext->getUsedProcessingDataBox()->getNextSaturationResolvedSuccessorExtensionIndividualNodeID();

					CLinkedDataValueAssertionSaturationData* linkedDataValueAssertionData = processIndi->getIndividualExtensionData(true)->getLinkedDataValueAssertionData(true);
					linkedDataValueAssertionData->addDataValueAssertion(role, dataLiteral);

					CIndividualSaturationProcessNode* dataValueIndiNode = CObjectParameterizingAllocator< CIndividualSaturationProcessNode, CProcessContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(), calcAlgContext->getUsedProcessContext());
					dataValueIndiNode->initIndividualSaturationProcessNode(nextResolveIndiID, nullptr, nullptr);
					dataValueIndiNode->initRootIndividualSaturationProcessNode();
					dataValueIndiNode->setReferenceMode(4);

					CIndividualSaturationProcessNodeLinker* dataNodeProcessLiner = CObjectAllocator<CIndividualSaturationProcessNodeLinker>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
					bool queueProcessing = true;
					dataNodeProcessLiner->initProcessNodeLinker(dataValueIndiNode, queueProcessing);
					dataValueIndiNode->setIndividualSaturationProcessNodeLinker(dataNodeProcessLiner);
					if (queueProcessing) {
						calcAlgContext->getUsedProcessingDataBox()->addIndividualSaturationProcessNodeLinker(dataNodeProcessLiner);
					}


					CConcept* baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopDataRangeConcept();
					if (baseTopConcept) {
						addConceptFilteredToIndividual(baseTopConcept, false, dataValueIndiNode, false, calcAlgContext);
					}
					if (dataLiteral->getDatatype() && dataLiteral->getDatatype()->getDatatypeConcept()) {
						addConceptFilteredToIndividual(dataLiteral->getDatatype()->getDatatypeConcept(), false, dataValueIndiNode, false, calcAlgContext);
					}
					dataValueIndiNode->setInitialized(true);
					dataValueIndiNode->setRequiredBackwardPropagation(true);
					addIndividualToCompletionQueue(dataValueIndiNode, calcAlgContext);

					CIndividualSaturationProcessNodeVector* indiVec = calcAlgContext->getUsedProcessingDataBox()->getIndividualSaturationProcessNodeVector(true);
					indiVec->setData(dataValueIndiNode->getIndividualID(), dataValueIndiNode);

					dataValueIndiNode->setSeparated(processIndi->isSeparated());
					preprocessResolvedIndividualNode(dataValueIndiNode, calcAlgContext);
					associateDataLiteralWithNode(dataValueIndiNode, dataLiteral, dataLiteral->getDatatype(), calcAlgContext);


					updateIndirectAddingIndividualStatusFlags(processIndi, dataValueIndiNode->getIndirectStatusFlags(), mCalcAlgContext);
					updateAddingSuccessorConnectedNominal(processIndi, dataValueIndiNode->getSuccessorConnectedNominalSet(false), mCalcAlgContext);
					updateMaxCardinalityCandidates(processIndi, dataValueIndiNode->getMaxAtleastCardinalityCandidate(), dataValueIndiNode->getMaxAtmostCardinalityCandidate(), mCalcAlgContext);
					
					CRoleBackwardSaturationPropagationHash* backPropHash = nullptr;
					CReapplyConceptSaturationLabelSet* processIndiConSet = nullptr;
					CReapplyConceptSaturationLabelSet* rangeIndiConSet = nullptr;
					CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList();
					bool connected = false;
					while (superRoleIt) {
						CRole* superRole = superRoleIt->getData();
						if (superRole->getDisjointRoleList() != nullptr) {
							updateDirectAddingIndividualStatusFlags(processIndi, CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT, mCalcAlgContext);
							setInsufficientNodeOccured(mCalcAlgContext);
						}
						CSortedNegLinker<CConcept*>* domainConLinker = superRole->getDomainRangeConceptList(superRoleIt->isNegated());
						for (CSortedNegLinker<CConcept*>* domainConLinkerIt = domainConLinker; domainConLinkerIt; domainConLinkerIt = domainConLinkerIt->getNext()) {
							CConcept* domainConcept = domainConLinkerIt->getData();
							bool domainConceptNegation = domainConLinkerIt->isNegated();
							if (!processIndiConSet) {
								processIndiConSet = processIndi->getReapplyConceptSaturationLabelSet(true);
							}
							addConceptFilteredToIndividual(domainConcept, domainConceptNegation, processIndi, processIndiConSet, false, mCalcAlgContext);
						}
						CSortedNegLinker<CConcept*>* rangeConLinker = superRole->getDomainRangeConceptList(!superRoleIt->isNegated());
						for (CSortedNegLinker<CConcept*>* rangeConLinkerIt = rangeConLinker; rangeConLinkerIt; rangeConLinkerIt = rangeConLinkerIt->getNext()) {
							CConcept* rangeConcept = rangeConLinkerIt->getData();
							bool rangeConceptNegation = rangeConLinkerIt->isNegated();
							if (!rangeIndiConSet) {
								rangeIndiConSet = dataValueIndiNode->getReapplyConceptSaturationLabelSet(true);
							}
							addConceptFilteredToIndividual(rangeConcept, rangeConceptNegation, processIndi, rangeIndiConSet, false, mCalcAlgContext);
						}

						if (superRoleIt->isNegated()) {
							connected = true;
							CBackwardSaturationPropagationLink* backPropLink = nullptr;
							backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
							backPropLink->initBackwardPropagationLink(processIndi, superRole);
							installBackwardPropagationLink(processIndi, dataValueIndiNode, superRole, backPropLink, true, true, calcAlgContext);
						} else {
							addNewLinkedExtensionProcessingRole(superRole, processIndi, true, true, mCalcAlgContext);
						}
						superRoleIt = superRoleIt->getNext();
					}
					if (!connected) {
						CXLinker<CIndividualSaturationProcessNode*>* nonInvConnectedIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						nonInvConnectedIndiNodeLinker->initLinker(processIndi);
						dataValueIndiNode->addNonInverseConnectedIndividualNodeLinker(nonInvConnectedIndiNodeLinker);
					}
				}




				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getSeparatedSaturationConceptAssertionResolveNode(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* procDataBox = calcAlgContext->getUsedProcessingDataBox();
					CConcept* topConcept = procDataBox->getOntologyTopConcept();
					CIndividualSaturationProcessNode* resolveNode = procDataBox->getSeparatedSaturationConceptAssertionResolveNode();
					if (!resolveNode) {
						resolveNode = CObjectParameterizingAllocator<CIndividualSaturationProcessNode,CProcessContext*>::allocateAndConstructAndParameterize(calcAlgContext->getUsedProcessTaskMemoryAllocationManager(),calcAlgContext->getUsedProcessContext());
						cint64 nextResolveIndiID = calcAlgContext->getUsedProcessingDataBox()->getNextSaturationResolvedSuccessorExtensionIndividualNodeID();

						resolveNode->initIndividualSaturationProcessNode(nextResolveIndiID,nullptr,nullptr);
						resolveNode->setInitialized(true);
						resolveNode->setSeparated(true);
						resolveNode->setRequiredBackwardPropagation(true);

						addIndividualToCompletionQueue(resolveNode,calcAlgContext);

						CIndividualSaturationProcessNodeLinker* resolveNodeProcessLiner = CObjectAllocator<CIndividualSaturationProcessNodeLinker>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						resolveNodeProcessLiner->initProcessNodeLinker(resolveNode,true);
						resolveNode->setIndividualSaturationProcessNodeLinker(resolveNodeProcessLiner);
						calcAlgContext->getUsedProcessingDataBox()->addIndividualSaturationProcessNodeLinker(resolveNodeProcessLiner);
						CIndividualSaturationProcessNodeVector* indiVec = calcAlgContext->getUsedProcessingDataBox()->getIndividualSaturationProcessNodeVector(true);
						indiVec->setData(resolveNode->getIndividualID(),resolveNode);

						CReapplyConceptSaturationLabelSet* resolveConSet = resolveNode->getReapplyConceptSaturationLabelSet(true);
						addConceptFilteredToIndividual(topConcept,false,resolveNode,resolveConSet,false,calcAlgContext);
						
						procDataBox->setSeparatedSaturationConceptAssertionResolveNode(resolveNode);
					}
					return resolveNode;
				}


				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getIndividualNodeForConcept(CConcept* concept, bool negated, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualSaturationProcessNode* node = nullptr;
					CConceptData* conceptData = concept->getConceptData();
					if (conceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(negated);
							if (satCalcRefLinkData) {
								node = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
							}
						}
					}
					return node;
				}


				cint64 CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getSaturationIDForIndividualNode(CIndividual* individual, CCalculationAlgorithmContextBase* calcAlgContext) {
					CIndividualProcessData* indiProcData = (CIndividualProcessData*)individual->getIndividualData();
					bool hasRefLinking = false;
					if (indiProcData) {
						CConceptReferenceLinking* refLinking = indiProcData->getSaturationReferenceLinkingData();
						if (refLinking) {
							CIndividualSaturationReferenceLinkingData* satCalcRefLinkData = (CIndividualSaturationReferenceLinkingData*)refLinking;
							if (satCalcRefLinkData) {
								hasRefLinking = true;
								return satCalcRefLinkData->getSaturationID();
							}
						}
					}
					return -1;
				}


				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::getIndividualNodeForIndividual(CIndividualSaturationProcessNode* indiProcSatNode, CIndividual* individual, cint64 saturationID, CCalculationAlgorithmContextBase* calcAlgContext) {

					CIndividualSaturationProcessNode* node = nullptr;
					if (indiProcSatNode->getNominalIndividual() == individual) {
						node = indiProcSatNode;
					} else {
						CIndividualProcessData* indiProcData = (CIndividualProcessData*)individual->getIndividualData();
						bool hasRefLinking = false;
						if (indiProcData) {
							CConceptReferenceLinking* refLinking = indiProcData->getSaturationReferenceLinkingData();
							if (refLinking) {
								CIndividualSaturationReferenceLinkingData* satCalcRefLinkData = (CIndividualSaturationReferenceLinkingData*)refLinking;
								if (satCalcRefLinkData) {
									hasRefLinking = true;
									if (satCalcRefLinkData->getSaturationID() == saturationID) {
										node = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
										if (node->getNominalIndividual() != individual) {
											node = nullptr;
										}
									}
								}
							}
						}
					}
					//if (!hasRefLinking) {
					//	CIndividualSaturationProcessNodeVector* indiNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualSaturationProcessNodeVector(false);
					//	node = indiNodeVec->getData(individual->getIndividualID());
					//}
					return node;
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::countConceptsOfReferredNodes(CConcept* concept, bool negation, cint64 depth, cint64& manyConceptRefIndiNodeCount, cint64& totalRefConceptCount, cint64& unprocessedRefCount, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (depth <= 0) {
						return false;
					}
					if (concept) {
						cint64 initConCode = concept->getOperatorCode();
						if (!negation && (initConCode == CCSUB || initConCode == CCEQ || initConCode == CCAND) || negation && (initConCode == CCEQ || initConCode == CCOR)) {
							for (CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
								CConcept* opConcept = opConLinkerIt->getData();
								bool opNegation = opConLinkerIt->isNegated()^negation;

								CIndividualSaturationProcessNode* opIndiNode = getIndividualNodeForConcept(opConcept,opNegation,calcAlgContext);
								if (opIndiNode && opIndiNode->isInitialized()) {
									if (opIndiNode->getDirectStatusFlags()->hasUnprocessedFlag()) {
										++unprocessedRefCount;
									}
									CReapplyConceptSaturationLabelSet* opConSet = opIndiNode->getReapplyConceptSaturationLabelSet(false);
									if (opConSet) { 
										cint64 opConCount = opConSet->getConceptCount();
										totalRefConceptCount += opConCount;
										if (opConCount >= mConfReferredNodeManyConceptCount) {
											++manyConceptRefIndiNodeCount;
										}
									}
								} else {
									countConceptsOfReferredNodes(opConcept,opNegation,depth-1,manyConceptRefIndiNodeCount,totalRefConceptCount,unprocessedRefCount,calcAlgContext);
								}
							}
						}
					}
					return false;
				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isProcessingCritical(CIndividualSaturationProcessNode* indiProcSatNode, CSaturationConceptDataItem* conceptSatItem, CIndividualSaturationProcessNode* specRefIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mConfForceManyConceptSaturation && specRefIndiProcSatNode && conceptSatItem) {
						CConcept* initConcept = conceptSatItem->getSaturationConcept();
						bool initNegated = conceptSatItem->getSaturationNegation();


						cint64 manyRefIndiConCount = 0;
						cint64 totalRefConCount = 0;
						cint64 unprocessedRefCount = 0;

						countConceptsOfReferredNodes(initConcept,initNegated,mConfReferredNodeCheckingDepth,manyRefIndiConCount,totalRefConCount,unprocessedRefCount,calcAlgContext);

						if (manyRefIndiConCount >= mConfManyConceptReferredNodeCountProcessLimit || totalRefConCount >= mConfReferredNodeConceptCountProcessLimit || unprocessedRefCount >= mConfReferredNodeUnprocessedCountProcessLimit) {
							return true;
						}
					}
					return false;
				}



				CIndividualSaturationProcessNode* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::resolveSpecialInitializationIndividualNode(CIndividualSaturationProcessNode* indiProcSatNode, CSaturationConceptDataItem* conceptSatItem, CIndividualSaturationProcessNode* specRefIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (specRefIndiProcSatNode && conceptSatItem) {
						CConcept* initConcept = conceptSatItem->getSaturationConcept();
						bool initNegated = conceptSatItem->getSaturationNegation();


						CIndividualSaturationProcessNode* maxConCountSpecRefIndiNode = nullptr;
						cint64 maxConCount = 0;

						if (initConcept) {
							cint64 initConCode = initConcept->getOperatorCode();
							if (!initNegated && (initConCode == CCSUB || initConCode == CCEQ || initConCode == CCAND) || initNegated && (initConCode == CCEQ || initConCode == CCOR)) {
								for (CSortedNegLinker<CConcept*>* opConLinkerIt = initConcept->getOperandList(); opConLinkerIt; opConLinkerIt = opConLinkerIt->getNext()) {
									CConcept* opConcept = opConLinkerIt->getData();
									bool opNegation = opConLinkerIt->isNegated()^initNegated;

									CIndividualSaturationProcessNode* opIndiNode = getIndividualNodeForConcept(opConcept,opNegation,calcAlgContext);
									if (opIndiNode && opIndiNode->isInitialized()) {
										CReapplyConceptSaturationLabelSet* opConSet = opIndiNode->getReapplyConceptSaturationLabelSet(false);
										if (opConSet) {
											cint64 opConCount = opConSet->getConceptCount();
											if (!maxConCountSpecRefIndiNode || opConCount > maxConCount) {
												maxConCount = opConCount;
												maxConCountSpecRefIndiNode = opIndiNode;
											}
										}
									}
								}
							}
						}

						if (maxConCountSpecRefIndiNode) {
							specRefIndiProcSatNode = maxConCountSpecRefIndiNode;
						}
					}
					return specRefIndiProcSatNode;
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
					bool dataRangeConcept = false;
					CRole* initRole = nullptr;

					if (conceptSatItem) {
						initConcept = conceptSatItem->getSaturationConcept();
						initNegated = conceptSatItem->getSaturationNegation();
						initRole = conceptSatItem->getSaturationRoleRanges();

						if (!mConfForceAllConceptInsertion) {
							requiredBackProp = conceptSatItem->isPotentiallyExistInitializationConcept();
						}
						dataRangeConcept = conceptSatItem->isDataRangeConcept();
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
							//specialIndiNode = resolveSpecialInitializationIndividualNode(indiProcSatNode,conceptSatItem,specialIndiNode,calcAlgContext);
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
						if (blockConSet && blockConSet->getConceptDescriptorAndReapplyQueue(initConcept,conceptSaturationDescriptor,impReapplyConSatDes)) {
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
							updateAddingSuccessorConnectedNominal(indiProcSatNode,blockedIndiNode->getSuccessorConnectedNominalSet(false),mCalcAlgContext);

						} else {
							copyIndividualNode = true;
						}
					} 

					bool tryFlatLabelCopy = false;
					indiProcSatNode->setRequiredBackwardPropagation(requiredBackProp);

					if (!specialIndiNode) {
						CIndividual* nominalIndi = indiProcSatNode->getNominalIndividual();
						if (nominalIndi) {
							indiProcSatNode->setABoxIndividualRepresentationNode(true);

							if (nominalIndi->hasIndividualName()) {
								CIndividualSaturationProcessNodeVector* indiNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualSaturationProcessNodeVector(false);
								CIndividualSaturationProcessNode* resolveNode = nullptr;
								if (indiProcSatNode->isSeparated()) {
									resolveNode = getSeparatedSaturationConceptAssertionResolveNode(calcAlgContext);
								} else {
									CConcept* topConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
									resolveNode = getIndividualNodeForConcept(topConcept,false,calcAlgContext);
								}

								CIndividualSaturationProcessNode* assertionResolvedNode = getResolvedIndividualNodeRepresentativeAssertion(resolveNode,nominalIndi,calcAlgContext);
								specialIndiNode = assertionResolvedNode;
								copyIndividualNode = true;
								tryFlatLabelCopy = true;
							} else {
								specialIndiNode = nullptr;
								copyIndividualNode = false;

								for (CConceptAssertionLinker* conAssLinkerIt = nominalIndi->getAssertionConceptLinker(); conAssLinkerIt; conAssLinkerIt = conAssLinkerIt->getNext()) {
									CConcept* assConcept = conAssLinkerIt->getData();
									bool assNegation = conAssLinkerIt->isNegated();
									addConceptFilteredToIndividual(assConcept,assNegation,indiProcSatNode,indiProcSatNode->getReapplyConceptSaturationLabelSet(true),false,calcAlgContext);
								}
							}
						}
					}


					if (!specialIndiNode && !dataRangeConcept && mConfCopyNodeFromTopIndividualForManyConcepts) {
						CConcept* topConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();

						CConceptData* topConceptData = topConcept->getConceptData();
						if (topConceptData) {
							CConceptProcessData* conProcData = (CConceptProcessData*)topConceptData;
							CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
							if (conRefLinking) {
								CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
								CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(false);
								if (satCalcRefLinkData) {
									specialIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
								}
							}
						}
						if (specialIndiNode && specialIndiNode->isInitialized()) {
							CReapplyConceptSaturationLabelSet* topConSet = specialIndiNode->getReapplyConceptSaturationLabelSet(false);
							if (topConSet) {
								if (topConSet->getConceptCount() > 10) {
									copyIndividualNode = true;
								}
							}
						} else {
							specialIndiNode = nullptr;
						}

					}

					if (specialIndiNode && copyIndividualNode) {

						if (isProcessingCritical(indiProcSatNode,conceptSatItem,specialIndiNode,calcAlgContext)) {
							updateDirectAddingIndividualStatusFlags(indiProcSatNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGUNPROCESSED | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,calcAlgContext);
							setInsufficientNodeOccured(calcAlgContext);
							addInitializationConcepts = false;
							indiProcSatNode->clearConceptSaturationProcessLinker();
						} else {
							CIndividualSaturationProcessNode* blockedIndiNode = specialIndiNode;
							while (blockedIndiNode->hasSubstituteIndividualNode()) {
								blockedIndiNode = blockedIndiNode->getSubstituteIndividualNode();
							}
							initializeIndividualNodeByCoping(indiProcSatNode,blockedIndiNode,tryFlatLabelCopy,mCalcAlgContext);
						}
						initialized = true;
					}
					if (!initialized) {
						specialIndiNode = nullptr;
						indiProcSatNode->initRootIndividualSaturationProcessNode();
						indiProcSatNode->setReferenceMode(4);
						CConcept* baseTopConcept = nullptr;
						if (!dataRangeConcept) {
							baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
						} else {
							baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopDataRangeConcept();
						}
						if (baseTopConcept) {
							addConceptFilteredToIndividual(baseTopConcept,false,indiProcSatNode,false,calcAlgContext);
						}
						CConcept* univConnNomValueConcept = calcAlgContext->getProcessingDataBox()->getOntology()->getTBox()->getUniversalConnectionNominalValueConcept();
						if (univConnNomValueConcept && !dataRangeConcept) {
							addConceptFilteredToIndividual(univConnNomValueConcept,false,indiProcSatNode,false,calcAlgContext);
						}
						initialized = true;
					}

					addIndividualToCompletionQueue(indiProcSatNode,calcAlgContext);




					if (addInitializationConcepts) {
						if (initConcept) {
							addConceptFilteredToIndividual(initConcept,initNegated,indiProcSatNode,indiProcSatNode->getReapplyConceptSaturationLabelSet(true),false,calcAlgContext);
							if (specialIndiNode) {
								bool initConceptDisjunction = false;
								cint64 initConOpCode = initConcept->getOperatorCode();
								if (initNegated && (initConOpCode == CCAND || initConOpCode == CCEQ) || !initNegated && (initConOpCode == CCOR)) {
									initConceptDisjunction = true;
								}
								if (initConceptDisjunction) {
									CReapplyConceptSaturationLabelSet* specConSet = specialIndiNode->getReapplyConceptSaturationLabelSet(false);
									CConceptSaturationDescriptor* initConSatDes = nullptr;
									CImplicationReapplyConceptSaturationDescriptor* initConImpDes = nullptr;
									if (specConSet->getConceptSaturationDescriptor(initConcept,initConSatDes,initConImpDes)) {
										if (initConSatDes->isNegated() == initNegated) {
											CConceptSaturationProcessLinker* conceptSaturationProcessLinker = createConceptSaturationProcessLinker(calcAlgContext);
											conceptSaturationProcessLinker->initConceptSaturationProcessLinker(initConSatDes);
											indiProcSatNode->addConceptSaturationProcessLinker(conceptSaturationProcessLinker);
										}
									}
								}
							}
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
					CBackwardSaturationPropagationLink* backSatPropLinkIt = indiProcSatNode->getInitializingBackwardPropagationLinks();
					if (backSatPropLinkIt) {
						while (backSatPropLinkIt) {
							CBackwardSaturationPropagationLink* backPropLink = backSatPropLinkIt;
							backSatPropLinkIt = backSatPropLinkIt->getNext();
							backPropLink->clearNext();

							installBackwardPropagationLink(backPropLink->getSourceIndividual(),indiProcSatNode,backPropLink->getLinkRole(),backPropLink,true,true,calcAlgContext);

							updateIndirectAddingIndividualStatusFlags(backPropLink->getSourceIndividual(),indiProcSatNode->getIndirectStatusFlags(),mCalcAlgContext);							
							updateAddingSuccessorConnectedNominal(backPropLink->getSourceIndividual(),indiProcSatNode->getSuccessorConnectedNominalSet(false),mCalcAlgContext);
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


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyDATATYPERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker) {

					CConceptSaturationDescriptor* conDes = conProLinker->getConceptSaturationDescriptor();
					CConcept* concept = conDes->getConcept();
					CDatatype* datatype = concept->getDatatype();

					if (datatype && datatype->getDatatypeTag() != 1) {

						CSaturationIndividualNodeDatatypeData* appliedDatatypeData = processIndi->getAppliedDatatypeData(true);
						if (!appliedDatatypeData->getAppliedDatatype()) {
							processIndi->setDataValueApplied(true);

							bool dataValueTriviallySat = true;
							bool dataValueTriviallyUnsat = false;

							if (dataValueTriviallySat && !dataValueTriviallyUnsat) {
								handleDatatypeValueSpaceTriggers(processIndi, datatype, dataValueTriviallySat, dataValueTriviallyUnsat, mCalcAlgContext);
							}


							if (dataValueTriviallyUnsat) {
								updateDirectAddingIndividualStatusFlags(processIndi, CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED, mCalcAlgContext);
							} else if (!dataValueTriviallySat) {
								updateDirectAddingIndividualStatusFlags(processIndi, CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT, mCalcAlgContext);
								setInsufficientNodeOccured(mCalcAlgContext);
							}

							appliedDatatypeData->setAppliedDatatype(datatype);
						} else if (datatype != appliedDatatypeData->getAppliedDatatype()) {
							updateDirectAddingIndividualStatusFlags(processIndi, CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT, mCalcAlgContext);
							setInsufficientNodeOccured(mCalcAlgContext);
						}
					}
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyDATALITERALRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker) {
					CConceptSaturationDescriptor* conDes = conProLinker->getConceptSaturationDescriptor();
					bool conNegation = conDes->getNegation();
					CConcept* concept = conDes->getConcept();
					CDataLiteral* dataLiteral = concept->getDataLiteral();
					CDatatype* datatype = concept->getDatatype();

					associateDataLiteralWithNode(processIndi, dataLiteral, datatype, mCalcAlgContext);
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::handleDatatypeValueSpaceTriggers(CIndividualSaturationProcessNode*& processIndi, CDatatype* datatype, bool &dataValueTriviallySat, bool &dataValueTriviallyUnsat, CCalculationAlgorithmContextBase* calcAlgContext) {
					CDatatypeValueSpacesTriggers* valueSpaceTriggers = mCalcAlgContext->getUsedProcessingDataBox()->getOntology()->getDataBoxes()->getMBox()->getValueSpacesTriggers(false);
					if (valueSpaceTriggers) {
						CDatatypeValueSpaceTriggers* datatypeValueSpaceTrigger = valueSpaceTriggers->getValueSpaceTriggers(datatype->getValueSpaceType());
						if (datatypeValueSpaceTrigger) {
							if (datatypeValueSpaceTrigger->getConceptTriggerCount() > 0) {

								bool triggersHandled = tryHandleDatatypeValueSpaceTriggers(processIndi, datatypeValueSpaceTrigger, valueSpaceTriggers, datatype, mCalcAlgContext);

								if (!triggersHandled) {
									dataValueTriviallySat = false;
								}

							}
						}
					}

				}


				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::tryHandleDatatypeValueSpaceTriggers(CIndividualSaturationProcessNode*& processIndi, CDatatypeValueSpaceTriggers* datatypeValueSpaceTrigger, CDatatypeValueSpacesTriggers* valueSpaceTriggers, CDatatype* datatype, CCalculationAlgorithmContextBase* calcAlgContext) {
					bool triggersHandled = false;

					if (datatype->getValueSpaceType()->getValueSpaceType() == CDatatypeValueSpaceType::VALUESPACESTRINGTYPE) {
						CDatatypeValueSpaceStringTriggers* stringDatatypeValueSpaceTrigger = valueSpaceTriggers->getStringValueSpaceTriggers();
						if (!stringDatatypeValueSpaceTrigger->getNonLanguageTagConceptTriggeringData() || !stringDatatypeValueSpaceTrigger->getNonLanguageTagConceptTriggeringData()->hasPartialConceptTriggers()) {
							if (!stringDatatypeValueSpaceTrigger->getValueSpaceTriggeringMap() || stringDatatypeValueSpaceTrigger->getValueSpaceTriggeringMap()->isEmpty()) {

								for (CDatatypeValueSpaceConceptTriggerLinker* triggerLinkerIt = datatypeValueSpaceTrigger->getValueSpaceConceptTriggeringData()->getPartialConceptTriggerLinker(); triggerLinkerIt; triggerLinkerIt = triggerLinkerIt->getNext()) {
									addConceptFilteredToIndividual(triggerLinkerIt->getTriggerConcept(), false, processIndi, calcAlgContext);
								}
								triggersHandled = true;

							}
						}


					}

					return triggersHandled;
				}



				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::associateDataLiteralWithNode(CIndividualSaturationProcessNode*& processIndi, CDataLiteral* dataLiteral, CDatatype* datatype, CCalculationAlgorithmContextBase* calcAlgContext) {

					CSaturationIndividualNodeDatatypeData* appliedDatatypeData = processIndi->getAppliedDatatypeData(true);
					if (!datatype) {
						datatype = dataLiteral->getDatatype();
					} else if (datatype != dataLiteral->getDatatype()) {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
						setInsufficientNodeOccured(mCalcAlgContext);
					}

					if (!appliedDatatypeData->getAppliedDatatype()) {
						processIndi->setDataValueApplied(true);
						appliedDatatypeData->setAppliedDatatype(datatype);
					} else if (datatype != appliedDatatypeData->getAppliedDatatype()) {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
						setInsufficientNodeOccured(mCalcAlgContext);
					}

					if (!appliedDatatypeData->getAppliedDataLiteral()) {
						processIndi->setDataValueApplied(true);
						appliedDatatypeData->setAppliedDataLiteral(dataLiteral);

						CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();


						bool dataValueTriviallySat = true;
						bool dataValueTriviallyUnsat = false;

						if (!dataLitValue && datatype) {
							dataValueTriviallySat = false;
						}

						if (dataValueTriviallySat && !dataValueTriviallyUnsat && datatype) {
							handleDatatypeValueSpaceTriggers(processIndi, datatype, dataValueTriviallySat, dataValueTriviallyUnsat, calcAlgContext);
						}




						if (dataValueTriviallySat && !dataValueTriviallyUnsat && datatype) {
							bool valueValid = false;
							CDatatype::DATATYPE_TYPE datatypeType = datatype->getDatatypeType();
							if (datatypeType == CDatatype::DT_BOOLEAN) {
								CDataLiteralBooleanValue* booleanDataLitValue = dynamic_cast<CDataLiteralBooleanValue*>(dataLitValue);
								if (booleanDataLitValue) {
									valueValid = true;
								}
							} else if (datatypeType == CDatatype::DT_FLOAT) {
								CDataLiteralFloatValue* floatDataLitValue = dynamic_cast<CDataLiteralFloatValue*>(dataLitValue);
								if (floatDataLitValue) {
									valueValid = true;
								}
							} else if (datatypeType == CDatatype::DT_DOUBLE) {
								CDataLiteralDoubleValue* doubleDataLitValue = dynamic_cast<CDataLiteralDoubleValue*>(dataLitValue);
								if (doubleDataLitValue) {
									valueValid = true;
								}
							} else if (datatypeType == CDatatype::DT_PLAINLITERAL) {
								CDataLiteralStringValue* stringDataLitValue = dynamic_cast<CDataLiteralStringValue*>(dataLitValue);
								if (stringDataLitValue) {
									valueValid = true;
								}
							} else if (datatypeType == CDatatype::DT_STRING) {
								CDataLiteralStringValue* stringDataLitValue = dynamic_cast<CDataLiteralStringValue*>(dataLitValue);
								if (stringDataLitValue) {
									if (stringDataLitValue->hasLanguageTag()) {
										dataValueTriviallyUnsat = true;
									} else {
										valueValid = true;
									}
								}
							} else if (datatypeType == CDatatype::DT_RATIONAL) {
								CDataLiteralRealValue* realDataLitValue = dynamic_cast<CDataLiteralRealValue*>(dataLitValue);
								if (realDataLitValue) {
									if (realDataLitValue->hasFlag(CDataLiteralRealValue::DLRV_RATIONAL_FLAG)) {
										valueValid = true;
									} else {
										dataValueTriviallyUnsat = true;
									}
								}
							} else if (datatypeType == CDatatype::DT_DECIMAL) {
								CDataLiteralRealValue* realDataLitValue = dynamic_cast<CDataLiteralRealValue*>(dataLitValue);
								if (realDataLitValue) {
									if (realDataLitValue->hasFlag(CDataLiteralRealValue::DLRV_DECIMAL_FLAG)) {
										valueValid = true;
									} else {
										dataValueTriviallyUnsat = true;
									}
								}
							} else if (datatypeType == CDatatype::DT_INTEGER) {
								CDataLiteralRealValue* realDataLitValue = dynamic_cast<CDataLiteralRealValue*>(dataLitValue);
								if (realDataLitValue) {
									if (realDataLitValue->hasFlag(CDataLiteralRealValue::DLRV_INTEGER_FLAG)) {
										valueValid = true;
									} else {
										dataValueTriviallyUnsat = true;
									}
								}
							}

							if (!valueValid) {
								dataValueTriviallySat = false;
							}
						}


						if (dataValueTriviallyUnsat) {
							updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,mCalcAlgContext);
						} else if (!dataValueTriviallySat) {
							updateDirectAddingIndividualStatusFlags(processIndi, CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT, mCalcAlgContext);
							setInsufficientNodeOccured(mCalcAlgContext);
						}

					} else if (dataLiteral != appliedDatatypeData->getAppliedDataLiteral()) {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
						setInsufficientNodeOccured(mCalcAlgContext);
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


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyNotDATATYPERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker) {
					updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
					setInsufficientNodeOccured(mCalcAlgContext);
					applyORRule(processIndi,conProLinker);
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

											bool separatedMode = processIndi->isSeparated() && !disjunctionIndiNode->isSeparated();
											if (!separatedMode) {
												addUninitializedIndividualToProcessingQueue(disjunctionIndiNode,mCalcAlgContext);
											}

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
												if (!separatedMode) {
													CXNegLinker<CIndividualSaturationProcessNode*>* depCopyLinker = CObjectAllocator< CXNegLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(taskMemMan);
													depCopyLinker->initNegLinker(processIndi,false);
													disjunctionIndiNode->addCopyDependingIndividualNodeLinker(depCopyLinker);
												}

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
						updateMaxCardinalityCandidates(processIndi,0,cardinality,mCalcAlgContext);
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYRESTRICTED,mCalcAlgContext);
						if (cardinality == 1) {
							if (!concept->getOperandList()) {
								addFUNCTIONALConceptExtensionProcessingRole(role,processIndi,mCalcAlgContext);
							} else {
								addQualifiedFUNCTIONALAtmostConceptExtensionProcessing(conDes,processIndi,mCalcAlgContext);
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
						updateMaxCardinalityCandidates(processIndi,cardinality,0,mCalcAlgContext);
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




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::addQualifiedFUNCTIONALAtmostConceptExtensionProcessing(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (mConfFUNCTIONALConceptsExtensionProcessing) {
						CSaturationIndividualNodeSuccessorExtensionData* succExtData = indiProcSatNode->getSuccessorExtensionData(true);
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* succIndiFUNCTIONALConExtData = succExtData->getFUNCTIONALConceptsExtensionData(true);
						addSuccessorExtensionToProcessingQueue(indiProcSatNode,calcAlgContext);

						if (!succIndiFUNCTIONALConExtData->hasQualifiedFunctionalAtmostConceptProcessLinkerForConcept(conDes)) {
							CConceptSaturationProcessLinker* conProcessLinker = createConceptSaturationProcessLinker(calcAlgContext);
							conProcessLinker->initConceptSaturationProcessLinker(conDes);
							succIndiFUNCTIONALConExtData->addQualifiedFunctionalAtmostConceptProcessLinker(conProcessLinker);
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
										if (succALLExtensionInitialized && !succData->mRoleALLConceptsProcessingQueued && queueALLExtension) {

											bool allQueueingRequired = succData->mRoleALLConceptsQueuingRequired;
											if (!allQueueingRequired) {
												CRoleBackwardSaturationPropagationHash* backwardPropHash = indiProcSatNode->getRoleBackwardPropagationHash(false);
												if (backwardPropHash) {
													CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backwardPropDataHash = backwardPropHash->getRoleBackwardPropagationDataHash();
													CRoleBackwardSaturationPropagationHashData* backwardPropData(backwardPropDataHash->valuePointer(role));
													if (backwardPropData && backwardPropData->mReapplyLinker) {
														allQueueingRequired = true;
														succData->mRoleALLConceptsQueuingRequired = true;
													}
												}
											}

											if (allQueueingRequired) {
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
												if (!succIndiFUNCTIONALConExtData->hasLinkedPredecessorAddedProcessLinkerForRole(role)) {
													CRoleSaturationProcessLinker* roleProcessLinker = createRoleSaturationProcessLinker(calcAlgContext);
													roleProcessLinker->initRoleProcessLinker(role);
													succIndiFUNCTIONALConExtData->addLinkedPredecessorAddedRoleProcessLinker(roleProcessLinker);
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




				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::loadConsistenceRepresentativeData(CCalculationAlgorithmContext* calcAlgContext) {
					bool consRepDataAvailable = false;
					if (!mRepresentativeDataLoaded) {
						mRepresentativeDataAvailable = false;
						CProcessingDataBox* processingDataBox = calcAlgContext->getProcessingDataBox();
						CConcreteOntology* ontology = processingDataBox->getOntology();
						CConsistence* consistence = ontology->getConsistence();
						if (consistence) {
							mRepresentativeDataAvailable = consistence->areIndividualsRepresentativelyStored();
						}
						mRepresentativeDataLoaded = true;
					}
					consRepDataAvailable = mRepresentativeDataAvailable;
					return consRepDataAvailable;
				}



				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::isConsistenceDataAvailable(CCalculationAlgorithmContext* calcAlgContext) {
					if (loadConsistenceModelData(calcAlgContext) || loadConsistenceRepresentativeData(calcAlgContext)) {
						return true;
					}
					return false;
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
					CIndividualProcessNode* indiNode = nullptr;
					if (indiVec) {
						indiNode = indiVec->getData(-individualID);
						if (indiNode && indiNode->getMergedIntoIndividualNodeID() != indiNode->getIndividualNodeID()) {
							indiNode = indiVec->getData(indiNode->getMergedIntoIndividualNodeID());
						}
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

						if (isConsistenceDataAvailable(mCalcAlgContext)) {

							CIndividualProcessNode* nominalProcessNode = getCorrectedNode(nominalID,mDetCachedCGIndiVector,mCalcAlgContext);
							if (nominalProcessNode) {

								bool nominalInfluenced = false;
								bool insufficientNominalConnection = false;

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
										if (!nominalConSet || !nominalConSet->containsConcept(rangeConcept,rangeConceptNegation)) {
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
									if (!processIndi->getNominalIndividual()) {
										addInfluencedNominal(nominalID,mCalcAlgContext);
									}
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

								bool nominalInfluenced = false;
								bool insufficientNominalConnection = false;

								CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mBackendAssCaceHandler->getIndividualAssociationData(nominalIndividual,mCalcAlgContext);

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

										if (!mBackendAssCaceHandler->hasConceptInAssociatedFullConceptSetLabel(indiAssData, indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL), rangeConcept, rangeConceptNegation, mCalcAlgContext)) {
											nominalInfluenced = true;
										}
									}

									if (inversedSuperRole) {

										mBackendAssCaceHandler->visitConceptsOfAssociatedFullConceptSetLabel(indiAssData, indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL), [&](CConcept* reapplyConcept, bool reapplyConceptNegation, bool deterministic) -> bool {

											if (reapplyConcept->getRole() == superRole) {
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
											return true;

										}, true, false, mCalcAlgContext);

									}
								}


								if (nominalInfluenced) {
									insufficientNominalConnection = true;
									if (!processIndi->getNominalIndividual()) {
										addInfluencedNominal(nominalID,mCalcAlgContext);
									}
								}

								if (insufficientNominalConnection) {
									updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
									setInsufficientNodeOccured(mCalcAlgContext);
								}

								addNominalDependentIndividualNode(nominalID,processIndi,CSaturationNominalDependentNodeData::VALUECONNECTION,mCalcAlgContext);
							}

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

					if (!processIndi->hasNominalIntegrated()) {
						processIndi->setIntegratedNominal(nominalIndividual);
					}
					if (mConfNominalProcessing) {
						updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGNOMINALCONNECTION,mCalcAlgContext);
						updateAddingSuccessorConnectedNominal(processIndi,nominalID,mCalcAlgContext);


						for (CXLinker<CIndividualSaturationProcessNode*>* multipleCardinalityAncestorNodesLinkerIt = processIndi->getMultipleCardinalityAncestorNodesLinker(); multipleCardinalityAncestorNodesLinkerIt; multipleCardinalityAncestorNodesLinkerIt = multipleCardinalityAncestorNodesLinkerIt->getNext()) {
							CIndividualSaturationProcessNode* multipleCardinalityAncestorNode = multipleCardinalityAncestorNodesLinkerIt->getData();
							updateDirectAddingIndividualStatusFlags(multipleCardinalityAncestorNode,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,mCalcAlgContext);
						}


						CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = nullptr;

						if (isConsistenceDataAvailable(mCalcAlgContext)) {
							CIndividualProcessNode* nominalProcessNode = getCorrectedNode(nominalID,mDetCachedCGIndiVector,mCalcAlgContext);

							if (nominalProcessNode) {
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
										if (!processIndi->getNominalIndividual()) {
											addInfluencedNominal(nominalID,mCalcAlgContext);
										}
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
							} else {

								CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mBackendAssCaceHandler->getIndividualAssociationData(nominalIndividual,mCalcAlgContext);
								if (indiAssData) {
									CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem = indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
									if (labelCacheItem) {
										bool nominalInfluenced = false;
										CReapplyConceptSaturationLabelSet* satConSet = processIndi->getReapplyConceptSaturationLabelSet(false);
										for (CConceptSaturationDescriptor* satConDesIt = satConSet->getConceptSaturationDescriptionLinker(); satConDesIt && !nominalInfluenced; satConDesIt = satConDesIt->getNext()) {
											CConcept* satConcept = satConDesIt->getConcept();
											bool satConceptNegation = satConDesIt->isNegated();

											if (satConceptNegation || satConcept->getOperatorCode() != CCNOMINAL || satConcept->getNominalIndividual() != nominalIndividual) {

												if (!mBackendAssCaceHandler->hasConceptInAssociatedFullConceptSetLabel(indiAssData, labelCacheItem, satConcept, satConceptNegation, mCalcAlgContext)) {
													nominalInfluenced = true;
												}

											}

										}
										if (nominalInfluenced) {
											if (!processIndi->getNominalIndividual()) {
												addInfluencedNominal(nominalID,mCalcAlgContext);
											}
										}


										mBackendAssCaceHandler->visitConceptsOfAssociatedFullConceptSetLabel(indiAssData, indiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL), [&](CConcept* addConcept, bool addConceptNegation, bool determinstic)->bool {
											addConceptFilteredToIndividual(addConcept, addConceptNegation, processIndi, mCalcAlgContext);
											return true;
										}, true, false, mCalcAlgContext);

									}
								}
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
							applyBackwardPropagationConcepts(processIndi,backPropReapplyDes,mCalcAlgContext);
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
						CConcept* baseTopConcept = nullptr;
						if (!role->isDataRole()) {
							baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
						} else {
							baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopDataRangeConcept();
						}
						CConcept* opConcept = baseTopConcept;
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


						bool separatedMode = processIndi->isSeparated() && !existIndiNode->isSeparated();
						if (!separatedMode) {
							addUninitializedIndividualToProcessingQueue(existIndiNode,mCalcAlgContext);
						}

						updateIndirectAddingIndividualStatusFlags(processIndi,existIndiNode->getIndirectStatusFlags(),mCalcAlgContext);
						updateAddingSuccessorConnectedNominal(processIndi,existIndiNode->getSuccessorConnectedNominalSet(false),mCalcAlgContext);
						updateMaxCardinalityCandidates(processIndi,existIndiNode->getMaxAtleastCardinalityCandidate(),existIndiNode->getMaxAtmostCardinalityCandidate(),mCalcAlgContext);


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
						bool existIndiCompleted = existIndiNode->isCompleted();
						if (separatedMode && !existIndiCompleted) {
							updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
							setInsufficientNodeOccured(mCalcAlgContext);
						} else {
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
									CBackwardSaturationPropagationLink* backPropLink = nullptr;
									if (!separatedMode) {
										backPropLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(taskMemMan);
										backPropLink->initBackwardPropagationLink(processIndi,superRole);
										if (!existIndiInitialized) {
											existIndiNode->addInitializingBackwardPropagationLinks(backPropLink);
										} else {
											installBackwardPropagationLink(processIndi,existIndiNode,superRole,backPropLink,true,true,calcAlgContext);

										}
									} else {
										if (!backPropHash) {
											backPropHash = existIndiNode->getRoleBackwardPropagationHash(false);
										}
										if (backPropHash) {
											CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDes = backPropHash->getBackwardPropagationBackwardPropagationConceptDescriptor(superRole);
											if (backPropReapplyDes) {
												applyBackwardPropagationConcepts(processIndi,backPropReapplyDes,mCalcAlgContext);
											}
										}
										CCriticalPredecessorRoleCardinalityHash* criticalPredRoleCardHash = existIndiNode->getCriticalPredecessorRoleCardinalityHash(false);
										if (criticalPredRoleCardHash) {
											CCriticalPredecessorRoleCardinalityData* criticalPredRoleCardData = criticalPredRoleCardHash->getCriticalPredecessorRoleCardinalityData(role,false);
											if (criticalPredRoleCardData) {
												updateDirectAddingIndividualStatusFlags(processIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT,mCalcAlgContext);
												setInsufficientNodeOccured(mCalcAlgContext);
											}
										}
									}
								} else {
									addNewLinkedExtensionProcessingRole(superRole,processIndi,true,true,mCalcAlgContext);
								}
								superRoleIt = superRoleIt->getNext();
							}
							if (!connected && !separatedMode) {
								CXLinker<CIndividualSaturationProcessNode*>* nonInvConnectedIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(taskMemMan);
								nonInvConnectedIndiNodeLinker->initLinker(processIndi);
								existIndiNode->addNonInverseConnectedIndividualNodeLinker(nonInvConnectedIndiNodeLinker);
							}
						}
					}
				}


				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::applyBackwardPropagationConcepts(CIndividualSaturationProcessNode* sourceIndiNode, CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDesIt, CCalculationAlgorithmContextBase* calcAlgContext) {
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


					//if (rootProcessIndi->getNominalIndividual()) {
					//	if (CIRIName::getRecentIRIName(rootProcessIndi->getNominalIndividual()->getIndividualNameLinker()).contains("http://www.biopax.org/release/biopax-level2.owl#protein")) {
					//		if (CIRIName::getRecentIRIName(addingConcept->getClassNameLinker()) == "http://www.biopax.org/release/biopax-level2.owl#physicalEntityParticipant") {
					//			bool bug = true;
					//		}
					//		if (addingConcept->getOperandCount() == 1 && CIRIName::getRecentIRIName(addingConcept->getOperandList()->getData()->getClassNameLinker()) == "http://www.biopax.org/release/biopax-level2.owl#physicalEntityParticipant") {
					//			bool bug = true;
					//		}
					//	}
					//}
					//if (rootProcessIndi->getIndividualID() == 3087) {
					//	if (addingConcept->getOperatorCode() == CCALL) {
					//		if (CIRIName::getRecentIRIName(addingConcept->getRole()->getPropertyNameLinker()) == "http://www.science.uva.nl/~jliem/ontologies/QRvocabulary.owl#hasConfiguration") {
					//			bool bug = true;
					//		}
					//	}
					//}


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
							CXNegLinker<CIndividualSaturationProcessNode*>* copyDepIndiLinkerIt = rootProcessIndi->getCopyDependingIndividualNodeLinker();
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









				CIndividualSaturationProcessNodeLinker* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createIndividualSaturationNodeLinker(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualSaturationProcessNodeLinker* indiSatNodeLinker = processingDataBox->takeRemainingIndividualSaturationNodeLinker();
					if (!indiSatNodeLinker) {
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						indiSatNodeLinker = CObjectAllocator<CIndividualSaturationProcessNodeLinker>::allocateAndConstruct(taskMemMan);
					}
					return indiSatNodeLinker;
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::releaseIndividualSaturationNodeLinker(CIndividualSaturationProcessNodeLinker* indSatNodeLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					indSatNodeLinker->clearNext();
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					processingDataBox->addRemainingIndividualSaturationNodeLinker(indSatNodeLinker);
				}





				CIndividualSaturationSuccessorLinkDataLinker* CCalculationTableauApproximationSaturationTaskHandleAlgorithm::createIndividualSaturationSuccessorLinkDataLinker(CCalculationAlgorithmContextBase* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualSaturationSuccessorLinkDataLinker* succLinkDataLinker = processingDataBox->takeRemainingIndividualSuccessorLinkDataLinker();
					if (!succLinkDataLinker) {
						CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
						succLinkDataLinker = CObjectAllocator<CIndividualSaturationSuccessorLinkDataLinker>::allocateAndConstruct(taskMemMan);
					}
					return succLinkDataLinker;
				}

				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::releaseIndividualSaturationSuccessorLinkDataLinker(CIndividualSaturationSuccessorLinkDataLinker* succLinkDataLinker, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (succLinkDataLinker) { 
						succLinkDataLinker->clearNext();
						CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
						processingDataBox->addRemainingIndividualSuccessorLinkDataLinker(succLinkDataLinker);
					}
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
							if (newInsertion) {
								if (reapplyImpReapplyConSatDesPtr) {
									reapplyImpReapplyConSatDes = *reapplyImpReapplyConSatDesPtr;
								}

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
									if (!conNeg && (opCode == CCATOM || opCode == CCSUB || opCode == CCIMPLTRIG || opCode == CCBRANCHTRIG)) {
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
							rootProcessIndi->addClashedConceptSaturationDescriptorLinker(conceptSaturationDescriptor);
							//mEndSaturationDebugIndiModelString = generateExtendedDebugIndiModelStringList(calcAlgContext);
							//QFile tmpFile("saturation-model.txt");
							//if (tmpFile.open(QIODevice::WriteOnly)) {
							//	tmpFile.write(mEndSaturationDebugIndiModelString.replace("<br>","").replace("<p>","\n").toLocal8Bit());
							//	tmpFile.close();
							//}
							updateDirectAddingIndividualStatusFlags(rootProcessIndi,CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,calcAlgContext);
							contained = true;
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

							for (CXNegLinker<CIndividualSaturationProcessNode*>* depdendingIndiLinkerIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depdendingIndiLinkerIt; depdendingIndiLinkerIt = depdendingIndiLinkerIt->getNext()) {
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

						if (!indiNode->isABoxIndividualRepresentationNode()) {
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
							for (CXNegLinker<CIndividualSaturationProcessNode*>* depdendingIndiLinkerIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depdendingIndiLinkerIt; depdendingIndiLinkerIt = depdendingIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* dependingIndi = depdendingIndiLinkerIt->getData();
								if (requiresIndirectAddingIndividualStatusFlagsUpdate(dependingIndi,addingFlags,calcAlgContext)) {
									CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = dependingIndi->getIndirectStatusFlags();
									indirectIndiFlags->addFlags(addingFlags);
									++mIndirectUpdatedStatusIndiNodeCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(dependingIndi)->append(directUpdateLinker);
								}
							}


							CRoleBackwardSaturationPropagationHash* backwardPropHash = updateIndiNode->getRoleBackwardPropagationHash(false);
							if (backwardPropHash && !updateIndiNode->isABoxIndividualRepresentationNode()) {
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


							if (!updateIndiNode->isABoxIndividualRepresentationNode()) {
								for (CXLinker<CIndividualSaturationProcessNode*>* nonInvConnIndiLinkerIt = updateIndiNode->getNonInverseConnectedIndividualNodeLinker(); nonInvConnIndiLinkerIt; nonInvConnIndiLinkerIt = nonInvConnIndiLinkerIt->getNext()) {
									CIndividualSaturationProcessNode* sourceIndividual = nonInvConnIndiLinkerIt->getData();
									if (requiresIndirectAddingIndividualStatusFlagsUpdate(sourceIndividual, addingFlags, calcAlgContext)) {
										CIndividualSaturationProcessNodeStatusFlags* indirectIndiFlags = sourceIndividual->getIndirectStatusFlags();
										indirectIndiFlags->addFlags(addingFlags);
										++mIndirectUpdatedStatusIndiNodeCount;
										directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
									}
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


							for (CXNegLinker<CIndividualSaturationProcessNode*>* depdendingIndiLinkerIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depdendingIndiLinkerIt; depdendingIndiLinkerIt = depdendingIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* dependingIndi = depdendingIndiLinkerIt->getData();
								if (requiresAddingSuccessorConnectedNominals(dependingIndi,addingNominalID,calcAlgContext)) {
									CSuccessorConnectedNominalSet* depIndiSuccConnNomSet = dependingIndi->getSuccessorConnectedNominalSet(true);
									depIndiSuccConnNomSet->addSuccessorConnectedNominal(addingNominalID);
									++mSuccessorConnectedNominalUpdatedCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(dependingIndi)->append(directUpdateLinker);
								}
							}


							if (!updateIndiNode->isABoxIndividualRepresentationNode()) {
								CRoleBackwardSaturationPropagationHash* backwardPropHash = updateIndiNode->getRoleBackwardPropagationHash(false);
								if (backwardPropHash) {
									CPROCESSHASH<CRole*, CRoleBackwardSaturationPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
									for (CPROCESSHASH<CRole*, CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
										CRole* backwardPropRole = it.key();
										const CRoleBackwardSaturationPropagationHashData& backwardPropData = it.value();
										for (CBackwardSaturationPropagationLink* linkIt = backwardPropData.mLinkLinker; linkIt; linkIt = linkIt->getNext()) {
											CIndividualSaturationProcessNode* sourceIndividual = linkIt->getSourceIndividual();
											if (requiresAddingSuccessorConnectedNominals(sourceIndividual, addingNominalID, calcAlgContext)) {
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
									if (requiresAddingSuccessorConnectedNominals(sourceIndividual, addingNominalID, calcAlgContext)) {
										CSuccessorConnectedNominalSet* sourceIndiSuccConnNomSet = sourceIndividual->getSuccessorConnectedNominalSet(true);
										sourceIndiSuccConnNomSet->addSuccessorConnectedNominal(addingNominalID);
										++mSuccessorConnectedNominalUpdatedCount;
										directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
									}
								}
							}
						}
					}
				}
















				bool CCalculationTableauApproximationSaturationTaskHandleAlgorithm::requiresMaxCardinalityCandidatePropagation(CIndividualSaturationProcessNode* indiNode, cint64 atleastCandidate, cint64 atmostCandidate, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (atleastCandidate > indiNode->getMaxAtleastCardinalityCandidate() || atmostCandidate > indiNode->getMaxAtmostCardinalityCandidate()) {
						return true;
					}
					return false;
				}




				void CCalculationTableauApproximationSaturationTaskHandleAlgorithm::updateMaxCardinalityCandidates(CIndividualSaturationProcessNode* indiNode, cint64 atleastCandidate, cint64 atmostCandidate, CCalculationAlgorithmContextBase* calcAlgContext) {
					if (requiresMaxCardinalityCandidatePropagation(indiNode,atleastCandidate,atmostCandidate,calcAlgContext)) {
						CIndividualSaturationProcessNodeStatusUpdateLinker* directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext);
						directUpdateLinker->initUpdateNodeLinker(indiNode);

						indiNode->addMaxAtleastCardinalityCandidate(atleastCandidate);
						indiNode->addMaxAtmostCardinalityCandidate(atmostCandidate);

						while (directUpdateLinker) {
							CIndividualSaturationProcessNodeStatusUpdateLinker* nextUpdateLinker = directUpdateLinker;
							directUpdateLinker = directUpdateLinker->getNext();
							CIndividualSaturationProcessNode* updateIndiNode = nextUpdateLinker->getData();
							nextUpdateLinker->clearNext();
							releaseIndividualSaturationUpdateLinker(nextUpdateLinker,calcAlgContext);
							++mMaximumCardinalityCandidatesUpdatedCount;


							for (CXNegLinker<CIndividualSaturationProcessNode*>* depdendingIndiLinkerIt = updateIndiNode->getCopyDependingIndividualNodeLinker(); depdendingIndiLinkerIt; depdendingIndiLinkerIt = depdendingIndiLinkerIt->getNext()) {
								CIndividualSaturationProcessNode* dependingIndi = depdendingIndiLinkerIt->getData();
								if (requiresMaxCardinalityCandidatePropagation(dependingIndi,atleastCandidate,atmostCandidate,calcAlgContext)) {
									dependingIndi->addMaxAtleastCardinalityCandidate(atleastCandidate);
									dependingIndi->addMaxAtmostCardinalityCandidate(atmostCandidate);
									++mMaximumCardinalityCandidatesUpdatedCount;
									directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(dependingIndi)->append(directUpdateLinker);
								}
							}


							if (!updateIndiNode->isABoxIndividualRepresentationNode()) {
								CRoleBackwardSaturationPropagationHash* backwardPropHash = updateIndiNode->getRoleBackwardPropagationHash(false);
								if (backwardPropHash) {
									CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* backPropHash = backwardPropHash->getRoleBackwardPropagationDataHash();
									for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = backPropHash->constBegin(), itEnd = backPropHash->constEnd(); it != itEnd; ++it) {
										CRole* backwardPropRole = it.key();
										const CRoleBackwardSaturationPropagationHashData& backwardPropData = it.value();
										for (CBackwardSaturationPropagationLink* linkIt = backwardPropData.mLinkLinker; linkIt; linkIt = linkIt->getNext()) {
											CIndividualSaturationProcessNode* sourceIndividual = linkIt->getSourceIndividual();
											if (requiresMaxCardinalityCandidatePropagation(sourceIndividual,atleastCandidate,atmostCandidate,calcAlgContext)) {
												sourceIndividual->addMaxAtleastCardinalityCandidate(atleastCandidate);
												sourceIndividual->addMaxAtmostCardinalityCandidate(atmostCandidate);
												++mMaximumCardinalityCandidatesUpdatedCount;
												directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
											}
										}
									}
								}


								for (CXLinker<CIndividualSaturationProcessNode*>* nonInvConnIndiLinkerIt = updateIndiNode->getNonInverseConnectedIndividualNodeLinker(); nonInvConnIndiLinkerIt; nonInvConnIndiLinkerIt = nonInvConnIndiLinkerIt->getNext()) {
									CIndividualSaturationProcessNode* sourceIndividual = nonInvConnIndiLinkerIt->getData();
									if (requiresMaxCardinalityCandidatePropagation(sourceIndividual,atleastCandidate,atmostCandidate,calcAlgContext)) {
										sourceIndividual->addMaxAtleastCardinalityCandidate(atleastCandidate);
										sourceIndividual->addMaxAtmostCardinalityCandidate(atmostCandidate);
										++mMaximumCardinalityCandidatesUpdatedCount;
										directUpdateLinker = createIndividualSaturationUpdateLinker(calcAlgContext)->initUpdateNodeLinker(sourceIndividual)->append(directUpdateLinker);
									}
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
								if (indi->getNominalIndividual()) {
									nominalString = QString(", ")+CIRIName::getRecentIRIName(indi->getNominalIndividual()->getNameLinker());
								} else {
									nominalString = QString(",Resolved-Extension-Node");
								}
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


							QString appliedDataLiteralString;
							if (indi->getAppliedDatatypeData(false)) {
								if (indi->getAppliedDatatypeData(false)->getAppliedDataLiteral() && indi->getAppliedDatatypeData(false)->getAppliedDatatype()) {
									appliedDataLiteralString = QString("AppliedDataLiteral: %1^^%2<br>\n").arg(indi->getAppliedDatatypeData(false)->getAppliedDataLiteral()->getLexicalDataLiteralValueString()).arg(indi->getAppliedDatatypeData(false)->getAppliedDatatype()->getDatatypeIRI());
								} else if (indi->getAppliedDatatypeData(false)->getAppliedDataLiteral()) {
									appliedDataLiteralString = QString("AppliedDataLiteral: %1 (without datatype)<br>\n").arg(indi->getAppliedDatatypeData(false)->getAppliedDataLiteral()->getLexicalDataLiteralValueString());
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
							CXNegLinker<CIndividualSaturationProcessNode*>* depIndiLinker = indi->getCopyDependingIndividualNodeLinker();
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
							if (indi->isABoxIndividualRepresentationNode()) {
								directFlagsStringList += "individual representation node";
							}
							if (indi->isOccurrenceStatisticsCollectingRequired()) {
								directFlagsStringList += "occurrence statistics collection marked";
							}
							if (indi->isOccurrenceStatisticsCollected()) {
								directFlagsStringList += "occurrence statistics collected";
							}

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


							indiString += QString("{%1} <br>\n<br>\n%2Backwardlinks %3<br>\nSuccessorLinks %4<br>\n%5%6%7%8").arg(conSetString).arg(appliedDataLiteralString).arg(propString).arg(succString).arg(depIndiString).arg(directFlagString).arg(indirectFlagString).arg(clashedString);
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
					if (statusFlags->hasUnprocessedFlag()) {
						flagsStringList += QString("unprocessed");
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
