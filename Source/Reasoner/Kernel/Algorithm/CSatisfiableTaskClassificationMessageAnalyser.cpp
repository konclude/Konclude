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

#include "CSatisfiableTaskClassificationMessageAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CSatisfiableTaskClassificationMessageAnalyser::CSatisfiableTaskClassificationMessageAnalyser() {
					mConfAnalyseSubSumptions = true;
					mConfAnalyseAllSubSumptions = false;
					mConfAnalyseIdentifierOccurence = true;
					mConfAnalysePossibleSubSumptions = true;

					mMaxPossibleSubsumerNegationCheckingDepth = 5;
					mMaxModelAnalysingDepth = 20;

					mConfStopAnalyseWhenNotChanging = true;
					mEquivalentAlternativesSaturationMerging = true;

					mStatSuccessorModelMergingTestCount = 0;
				}



				bool CSatisfiableTaskClassificationMessageAnalyser::readCalculationConfig(CCalculationConfigurationExtension *config) {					
					mConfAnalyseSubSumptions = true;
					mConfAnalyseAllSubSumptions = false;
					mConfAnalyseIdentifierOccurence = true;
					mConfAnalysePossibleSubSumptions = true;
					mConfStopAnalyseWhenNotChanging = true;
					mEquivalentAlternativesSaturationMerging = true;
					if (config) {
						mEquivalentAlternativesSaturationMerging = config->isEquivalentAlternativesSaturationMergingActivated();
						return true;
					}
					return false;
				}


				CIndividualProcessNode* CSatisfiableTaskClassificationMessageAnalyser::getCorrectedIndividualID(CIndividualProcessNode* baseIndiNode, CIndividualProcessNodeVector* indiNodeVec, bool* nondetMergedFlag) {
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



				bool CSatisfiableTaskClassificationMessageAnalyser::hasDependencyToAncestor(CIndividualProcessNode* individualNode, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 ancDepth = individualNode->getIndividualAncestorDepth();
					bool dependencyToAncestor = false;
					if (ancDepth <= 0) {
						dependencyToAncestor = depTrackPoint->isPointingToIndependentDependencyNode();
					} else {
						CDependencyNode* depNode = depTrackPoint->getDependencyNode();
						if (depNode->hasAppropriateIndividualNode()) {
							CIndividualProcessNode* appIndiNode = depNode->getAppropriateIndividualNode();
							cint64 appIndiAncDepth = appIndiNode->getIndividualAncestorDepth();
							dependencyToAncestor = appIndiAncDepth < ancDepth;
						} else {
							CDependencyNode::DEPENDENCNODEYTYPE depType = depNode->getDependencyType();
							if (depType == CDependencyNode::DNTMERGEDCONCEPT) {
								dependencyToAncestor = hasDependencyToAncestor(individualNode,depNode->getPreviousDependencyTrackPoint(),calcAlgContext);
							}
						}
					}
					return dependencyToAncestor;
				}



				CConceptDescriptor* CSatisfiableTaskClassificationMessageAnalyser::getIndividualProcessNodeConceptWithSingleAncestorDependency(CIndividualProcessNode* indiNode, CIndividualAnalyseProcessItem* indiAnProcItem, CCalculationAlgorithmContext* calcAlgContext) {
					if (indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
						return nullptr;
					}
					CConceptDescriptor* singleAncDepConceptDes = nullptr;
					CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
					CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker();
					while (conDesIt) {
						CConceptDescriptor* conDes = conDesIt;
						if (conDes->getConceptTag() != 1) {
							CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
							if (depTrackPoint) {
								if (hasDependencyToAncestor(indiNode,depTrackPoint,calcAlgContext)) {
									if (singleAncDepConceptDes != nullptr) {
										return nullptr;
									}
									singleAncDepConceptDes = conDes;
								}
							} else {
								return nullptr;
							}
						}
						conDesIt = conDesIt->getNext();
					}
					return singleAncDepConceptDes;
				}



				bool CSatisfiableTaskClassificationMessageAnalyser::testSubsumerCandidatePossibleWithMergedSaturatedModel(CIndividualProcessNode* indiNode, CConcept* equivConcept, CCalculationAlgorithmContext* calcAlgContext) {
					CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
					
					if (mEquivalentAlternativesSaturationMerging) {
						if (!mTestSaturatedMergedHash) {
							mTestSaturatedMergedHash = CObjectParameterizingAllocator< CPROCESSINGHASH< QPair<CConcept*,bool>,CSaturatedMergedTestItem >,CContext* >::allocateAndConstructAndParameterize(taskProcessorContext->getMemoryAllocationManager(),taskProcessorContext);
						}
						if (mLastMergingTestNode != indiNode) {
							mLastMergingTestNode = indiNode;
							mTestSaturatedMergedHash->clear();
						}
						STATINC(ANALYSECANDIDATECONCEPTSATURATEDALTERNATIVETESTCOUNT,calcAlgContext);

						if (equivConcept->getOperatorCode() == CCEQ) {
							CPROCESSINGSET< QPair<CConcept*,bool> > alternativesSet(taskProcessorContext);
							bool oneMergeSatisfieableFlag = false;
							bool allMergeUnsatisfieableFlag = true;
							if (collectEquivalenceConceptAlternatives(indiNode,equivConcept,true,alternativesSet,mTestSaturatedMergedHash,oneMergeSatisfieableFlag,allMergeUnsatisfieableFlag,calcAlgContext)) {
								if (oneMergeSatisfieableFlag) {
									STATINC(ANALYSECANDIDATECONCEPTSATURATEDALTERNATIVEFOUNDCOUNT,calcAlgContext);
									return false;
								}
							} else {
								if (testEquivalenceConceptAlternatives(indiNode,alternativesSet,mTestSaturatedMergedHash,oneMergeSatisfieableFlag,allMergeUnsatisfieableFlag,calcAlgContext)) {
									if (oneMergeSatisfieableFlag) {
										STATINC(ANALYSECANDIDATECONCEPTSATURATEDALTERNATIVEFOUNDCOUNT,calcAlgContext);
										return false;
									}
								}
							}
						}
					}
					return true;
				}



				bool CSatisfiableTaskClassificationMessageAnalyser::testConceptSetWithSaturatedModelMergable(CIndividualProcessNode* indiNode, CIndividualSaturationProcessNode* saturationIndiNode, bool& mergeClashedFlag, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(ANALYSECANDIDATECONCEPTSATURATEDCONCEPTSETMERGINGCOUNT,calcAlgContext);
					CReapplyConceptSaturationLabelSet* saturatedConSet = saturationIndiNode->getReapplyConceptSaturationLabelSet(false);
					CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);

					bool mergedConceptsClashed = saturationIndiNode->getIndirectStatusFlags()->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,false);

					if (saturatedConSet && conSet) {
						CReapplyConceptSaturationLabelSetIterator satConSetIt(saturatedConSet->getIterator(true,true));
						while (satConSetIt.hasNext() && !mergedConceptsClashed) {
							cint64 dataTag = satConSetIt.getDataTag();
							CConceptSaturationDescriptor* satConDes = satConSetIt.getConceptSaturationDescriptor();
							CImplicationReapplyConceptSaturationDescriptor* satReapplyDes = satConSetIt.getImplicationReapplyConceptSaturationDescriptor();

							CConceptDescriptor* conDes = nullptr;
							CDependencyTrackPoint* depTrackPoint = nullptr;
							CCondensedReapplyQueue* reapplyQueue = nullptr;
							if (conSet->getConceptDescriptorOrReapplyQueue(dataTag,conDes,depTrackPoint,reapplyQueue)) {
								if (conDes && satConDes) {
									if (conDes->isNegated() != satConDes->isNegated()) {
										mergedConceptsClashed = true;
									}
								} else if (conDes) {
									if (!conDes->isNegated() && satReapplyDes) {
										return false;
									}
								} else if (satConDes) {
									if (!satConDes->isNegated() && reapplyQueue && !reapplyQueue->isEmpty()) {
										return false;
									}
								}
							}

							satConSetIt.moveNext();
						}

						if (mergedConceptsClashed) {
							mergeClashedFlag = true;
						}
						return true;

					}				
					return false;
				}



				bool CSatisfiableTaskClassificationMessageAnalyser::testRoleSuccessorsWithSaturatedModelMergable(CIndividualProcessNode* indiNode, CIndividualSaturationProcessNode* saturationIndiNode, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(ANALYSECANDIDATECONCEPTSATURATEDROLESUCCESSORMERGINGCOUNT,calcAlgContext);
					CReapplyRoleSuccessorHash* roleSuccHash = indiNode->getReapplyRoleSuccessorHash(false);
					CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
					CRoleBackwardSaturationPropagationHash* roleBackPropHash = saturationIndiNode->getRoleBackwardPropagationHash(false);
					if (roleSuccHash && roleBackPropHash) {
						CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* roleBackPropDataHash = roleBackPropHash->getRoleBackwardPropagationDataHash();
						for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::const_iterator it = roleBackPropDataHash->constBegin(), itEnd = roleBackPropDataHash->constEnd(); it != itEnd; ++it) {
							CRole* role = it.key();
							const CRoleBackwardSaturationPropagationHashData& backPropData = it.value();
							CBackwardSaturationPropagationReapplyDescriptor* reapplyLinker = backPropData.mReapplyLinker;
							if (reapplyLinker) {
								CRoleSuccessorLinkIterator roleSuccIt(roleSuccHash->getRoleSuccessorLinkIterator(role));
								if (roleSuccIt.hasNext()) {
									for (CBackwardSaturationPropagationReapplyDescriptor* reapplyLinkerIt = reapplyLinker; reapplyLinkerIt; reapplyLinkerIt = reapplyLinkerIt->getNext()) {
										CConceptSaturationDescriptor* conSatDes = reapplyLinkerIt->getReapplyConceptSaturationDescriptor();
										CConcept* concept = conSatDes->getConcept();
										bool negation = conSatDes->isNegated();
										if (!conSet || !conSet->containsConcept(concept,negation)) {

											CRoleSuccessorLinkIterator roleSucc2It(roleSuccIt); 
											while (roleSucc2It.hasNext()) {
												CIndividualLinkEdge* succLink = roleSucc2It.next(true);
												CIndividualProcessNode* succIndi = nullptr;
												if (succLink->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag())) {
													succIndi = succLink->getOppositeIndividual(indiNode);
												} else {
													if (!succIndi->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag()) && succIndi->isRelocalized()) {
														cint64 succIndiId = succLink->getOppositeIndividualID(indiNode);
														CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
														succIndi = indiProcNodeVec->getData(succIndiId);
													}
												}
												CReapplyConceptLabelSet* succConSet = succIndi->getReapplyConceptLabelSet(false);
												for (CSortedNegLinker<CConcept*>* opConIt = concept->getOperandList(); opConIt; opConIt = opConIt->getNext()) {
													CConcept* opConcept = opConIt->getData();
													bool opNegation = conSatDes->isNegated()^negation;

													if (!succConSet || !succConSet->containsConcept(opConcept,opNegation)) {
														return false;
													}
												}
											}

										}
									}
								}
								//if (roleSuccHash->hasRoleSuccessor(role)) {
								//	return false;
								//}
							}
						}		
					}
					return true;
				}




				bool CSatisfiableTaskClassificationMessageAnalyser::collectSuccessorMergingNodesAndConcepts(CIndividualProcessNode* indiNode, CIndividualSaturationProcessNode* saturationIndiNode, CReapplyRoleSuccessorHash* roleSuccHash, CRole* role,
						CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>& trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet, CCalculationAlgorithmContext* calcAlgContext) {

					for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
						CRole* superRole = superRoleIt->getData();
						bool superRoleInversed = superRoleIt->isNegated();
						if (roleSuccHash && !superRoleInversed) {
							CReapplyQueueIterator reapplyIt(roleSuccHash->getRoleReapplyIterator(superRole,false));
							while (reapplyIt.hasNext()) {
								CReapplyConceptDescriptor* reapplyConDes = reapplyIt.next();
								CConceptDescriptor* conDes = reapplyConDes->getConceptDescriptor();
								CConcept* reapplyConcept = conDes->getConcept();
								bool reapplyConceptNegation = conDes->isNegated();
								cint64 reapplyConceptOpCode = reapplyConcept->getOperatorCode();
								if (reapplyConceptNegation && (reapplyConceptOpCode == CCSOME) || !reapplyConceptNegation && (reapplyConceptOpCode == CCALL || reapplyConceptOpCode == CCIMPLALL || reapplyConceptOpCode == CCBRANCHALL || reapplyConceptOpCode == CCAQALL || reapplyConceptOpCode == CCIMPLAQALL || reapplyConceptOpCode == CCBRANCHAQALL)) {
									for (CSortedNegLinker<CConcept*>* reapplyOperandConLinkerIt = reapplyConcept->getOperandList(); reapplyOperandConLinkerIt; reapplyOperandConLinkerIt = reapplyOperandConLinkerIt->getNext()) {
										CConcept* reapplyOperandCon = reapplyOperandConLinkerIt->getData();
										bool reapplyOperandConNeg = reapplyOperandConLinkerIt->isNegated()^reapplyConceptNegation;

										CIndividualSaturationProcessNode* satIndiNode = getSaturatedIndividualNodeForConcept(reapplyOperandCon,reapplyOperandConNeg,calcAlgContext);
										if (!satIndiNode) {
											if (!collectTrivialPropagationTestingConcepts(reapplyOperandCon,reapplyOperandConNeg,trivialSuccessorPropagatedConceptList,calcAlgContext)) {
												return false;
											}
										} else if (!satIndiNode->isCompleted() || satIndiNode->getIndirectStatusFlags()->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYPROPLEMATIC | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGNOMINALCONNECTION,false)) {
											return false;
										}

										if (satIndiNode) {
											successorList.append(satIndiNode);
										}
									}
								} else if (!reapplyConceptNegation && (reapplyConceptOpCode == CCAQAND || reapplyConceptOpCode == CCIMPLAQAND || reapplyConceptOpCode == CCBRANCHAQAND)) {
									if (!addAutomateTransactionConcepts(reapplyConcept,reapplyConceptNegation,superRole,successorList,trivialSuccessorPropagatedConceptList,calcAlgContext)) {
										return false;
									}
								} else {
									return false;
								}
							}
						} else {
							backwardRoleSet.insert(superRole);
						}
					}

					return true;
				}



				bool CSatisfiableTaskClassificationMessageAnalyser::testSaturatedExistentialsModelMergable(CIndividualProcessNode* indiNode, CIndividualSaturationProcessNode* saturationIndiNode, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(ANALYSECANDIDATECONCEPTSATURATEDROLESUCCESSOREXTENDEDMERGINGCOUNT,calcAlgContext);
					CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();

					cint64 remainingMergingDepth = 5;
					cint64 remainingMergingCount = 100;

					CReapplyRoleSuccessorHash* roleSuccHash = indiNode->getReapplyRoleSuccessorHash(false);


					if (saturationIndiNode->getDirectStatusFlags()->hasSuccessorNodeExtensionsFlag()) {
						CLinkedRoleSaturationSuccessorHash* linkedSuccHash = saturationIndiNode->getLinkedRoleSuccessorHash(false);
						if (linkedSuccHash) {
							CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* roleLinkedSuccHash = linkedSuccHash->getLinkedRoleSuccessorHash();
							for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator itSucc = roleLinkedSuccHash->constBegin(), itEndSucc = roleLinkedSuccHash->constEnd(); itSucc != itEndSucc; ++itSucc) {
								CRole* role = itSucc.key();
								CLinkedRoleSaturationSuccessorData* linkedSuccData = itSucc.value();
								CPROCESSMAP<cint64,CSaturationSuccessorData*>* succDataMap = linkedSuccData->getSuccessorNodeDataMap(false);
								if (succDataMap) {
									for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succDataMap->constBegin(), itEnd = succDataMap->constEnd(); it != itEnd; ++it) {
										CSaturationSuccessorData* succData = it.value();
										if (succData->mActiveCount >= 1) {
											for (CXNegLinker<CRole*>* creationRoleLinkerIt = succData->mCreationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
												if (!creationRoleLinkerIt->isNegated()) {
													CRole* creationRole = creationRoleLinkerIt->getData();
													if (creationRole == role) {

														CPROCESSINGLIST<CIndividualSaturationProcessNode*> successorList(taskProcessorContext);
														CPROCESSINGLIST<TConceptNegationPair> trivialSuccessorPropagatedConceptList(taskProcessorContext);
														CPROCESSINGSET<CRole*> backwardRoleSet(taskProcessorContext);

														if (!collectSuccessorMergingNodesAndConcepts(indiNode,saturationIndiNode,roleSuccHash,role,successorList,trivialSuccessorPropagatedConceptList,backwardRoleSet,calcAlgContext)) {
															return false;
														}

														if (!successorList.isEmpty() || !trivialSuccessorPropagatedConceptList.isEmpty()) {
															CIndividualSaturationProcessNode* extSatIndiNode = succData->mSuccIndiNode;
															successorList.prepend(extSatIndiNode);

															if (!testSaturatedSuccessorModelMergable(extSatIndiNode,successorList,&trivialSuccessorPropagatedConceptList,backwardRoleSet,remainingMergingDepth,remainingMergingCount,calcAlgContext)) {
																return false;
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
					} else {
						CReapplyConceptSaturationLabelSet* saturatedConSet = saturationIndiNode->getReapplyConceptSaturationLabelSet(false);
						for (CConceptSaturationDescriptor* conSatDesIt = saturatedConSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
							CConceptSaturationDescriptor* conSatDes = conSatDesIt;
							CConcept* concept = conSatDes->getConcept();
							bool negated = conSatDes->isNegated();
							cint64 opCode = concept->getOperatorCode();
							cint64 card = concept->getParameter();
							if (!negated && (opCode == CCSOME || opCode == CCAQSOME || opCode == CCATLEAST && card > 0) || negated && (opCode == CCALL || opCode == CCATMOST && card >= 0)) {

								CPROCESSINGLIST<CIndividualSaturationProcessNode*> successorList(taskProcessorContext);
								CPROCESSINGLIST<TConceptNegationPair> trivialSuccessorPropagatedConceptList(taskProcessorContext);
								CPROCESSINGSET<CRole*> backwardRoleSet(taskProcessorContext);

								CRole* role = concept->getRole();
								if (!collectSuccessorMergingNodesAndConcepts(indiNode,saturationIndiNode,roleSuccHash,role,successorList,trivialSuccessorPropagatedConceptList,backwardRoleSet,calcAlgContext)) {
									return false;
								}

								if (!successorList.isEmpty() || !trivialSuccessorPropagatedConceptList.isEmpty()) {
									CIndividualSaturationProcessNode* extSatIndiNode = getExistentialSaturatedIndividualNodeForConcept(concept,calcAlgContext);
									if (!extSatIndiNode) {
										return false;
									}
									successorList.prepend(extSatIndiNode);

									if (!testSaturatedSuccessorModelMergable(extSatIndiNode,successorList,&trivialSuccessorPropagatedConceptList,backwardRoleSet,remainingMergingDepth,remainingMergingCount,calcAlgContext)) {
										return false;
									}
								}

							}
						}
					}
					return true;
				}



				bool CSatisfiableTaskClassificationMessageAnalyser::collectTrivialPropagationTestingConcepts(CConcept* concept, bool negation, CPROCESSINGLIST<TConceptNegationPair>& trivialConceptTestingList, CCalculationAlgorithmContext* calcAlgContext) {
					CConceptOperator* conOperator = concept->getConceptOperator();
					cint64 conOpCode = concept->getOperatorCode();
					if (negation && conOpCode == CCSUB) {
						trivialConceptTestingList.append(TConceptNegationPair(concept,negation));
						return true;
					} else if (!negation && (conOpCode == CCALL || conOpCode == CCIMPLALL || conOpCode == CCBRANCHALL || conOpCode == CCAQALL || conOpCode == CCIMPLAQALL || conOpCode == CCBRANCHAQALL)) {
						trivialConceptTestingList.append(TConceptNegationPair(concept,negation));
						return true;
					} else if (!negation && (conOpCode == CCAQAND || conOpCode == CCIMPLAQAND || conOpCode == CCBRANCHAQAND)) {
						bool allOperandsSucceeded = true;
						for (CSortedNegLinker<CConcept*>* operandConLinkerIt = concept->getOperandList(); operandConLinkerIt; operandConLinkerIt = operandConLinkerIt->getNext()) {
							CConcept* operandCon = operandConLinkerIt->getData();
							bool operandConNeg = operandConLinkerIt->isNegated();
							allOperandsSucceeded &= collectTrivialPropagationTestingConcepts(operandCon,operandConNeg,trivialConceptTestingList,calcAlgContext);
						}
						return allOperandsSucceeded;
					}
					return false;
				}


				bool CSatisfiableTaskClassificationMessageAnalyser::addAutomateTransactionConcepts(CConcept* concept, bool negation, CRole* role, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>& trivialConceptTestingList, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 conOpCode = concept->getOperatorCode();
					if (!negation && (conOpCode == CCAQALL || conOpCode == CCIMPLAQALL || conOpCode == CCBRANCHAQALL)) {

						if (concept->getRole() == role) {

							for (CSortedNegLinker<CConcept*>* operandConLinkerIt = concept->getOperandList(); operandConLinkerIt; operandConLinkerIt = operandConLinkerIt->getNext()) {
								CConcept* reapplyOperandCon = operandConLinkerIt->getData();
								bool reapplyOperandConNeg = operandConLinkerIt->isNegated();

								CIndividualSaturationProcessNode* satIndiNode = getSaturatedIndividualNodeForConcept(reapplyOperandCon,reapplyOperandConNeg,calcAlgContext);
								if (!satIndiNode) {
									if (!collectTrivialPropagationTestingConcepts(reapplyOperandCon,reapplyOperandConNeg,trivialConceptTestingList,calcAlgContext)) {
										return false;
									}
								} else if (!satIndiNode->isCompleted() || satIndiNode->getIndirectStatusFlags()->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYPROPLEMATIC | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGNOMINALCONNECTION,false)) {
									return false;
								}
								
								if (satIndiNode) {
									successorList.append(satIndiNode);
								}
							}
						}
					} else if (!negation && (conOpCode == CCAQAND || conOpCode == CCIMPLAQAND || conOpCode == CCBRANCHAQAND)) {
						bool allOperandsSucceeded = true;
						for (CSortedNegLinker<CConcept*>* operandConLinkerIt = concept->getOperandList(); operandConLinkerIt; operandConLinkerIt = operandConLinkerIt->getNext()) {
							CConcept* reapplyOperandCon = operandConLinkerIt->getData();
							bool reapplyOperandConNeg = operandConLinkerIt->isNegated();
							allOperandsSucceeded &= addAutomateTransactionConcepts(reapplyOperandCon,reapplyOperandConNeg,role,successorList,trivialConceptTestingList,calcAlgContext);
						}
						return allOperandsSucceeded;
					}
					return true;
				}



				bool CSatisfiableTaskClassificationMessageAnalyser::testSaturatedSuccessorModelMergable(CIndividualSaturationProcessNode* existentialIndiNode, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>* trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet, cint64 remainingMergingDepth, cint64& remainingMergingCount, CCalculationAlgorithmContext* calcAlgContext) {
					++mStatSuccessorModelMergingTestCount;

					if (--remainingMergingDepth < 0) {
						return false;
					}
					if (--remainingMergingCount < 0) {
						return false;
					}

					if (successorList.count() == 1) {
						return testSingleSaturatedSuccessorModelMergable(existentialIndiNode,successorList,trivialSuccessorPropagatedConceptList,backwardRoleSet,remainingMergingDepth,remainingMergingCount,calcAlgContext);
					} else {
						return testMultipleSaturatedSuccessorModelMergable(existentialIndiNode,successorList,trivialSuccessorPropagatedConceptList,backwardRoleSet,remainingMergingDepth,remainingMergingCount,calcAlgContext);
					}
				}



				bool CSatisfiableTaskClassificationMessageAnalyser::testSingleSaturatedSuccessorModelMergable(CIndividualSaturationProcessNode* existentialIndiNode, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>* trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet, cint64 remainingMergingDepth, cint64& remainingMergingCount, CCalculationAlgorithmContext* calcAlgContext) {

					CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();

					CIndividualSaturationProcessNode* subResolvedExistentialIndiNode = existentialIndiNode;
					while (subResolvedExistentialIndiNode->hasSubstituteIndividualNode()) {
						subResolvedExistentialIndiNode = subResolvedExistentialIndiNode->getSubstituteIndividualNode();
					}
					CReapplyConceptSaturationLabelSet* satConSet = subResolvedExistentialIndiNode->getReapplyConceptSaturationLabelSet(false);
					CPROCESSINGHASH<CRole*,TConceptNegationPair> successorInfluenceConceptsHash(taskProcessorContext);
					if (trivialSuccessorPropagatedConceptList) {
						for (CPROCESSINGLIST<TConceptNegationPair>::const_iterator trivConIt = trivialSuccessorPropagatedConceptList->constBegin(), trivConItEnd = trivialSuccessorPropagatedConceptList->constEnd(); trivConIt != trivConItEnd; ++trivConIt) {
							TConceptNegationPair conNegPair(*trivConIt);
							CConcept* concept = conNegPair.first;
							bool negation = conNegPair.second;
							cint64 conOpCode = concept->getOperatorCode();

							if (negation && conOpCode == CCSUB) {
								CConceptSaturationDescriptor* conSatDes = nullptr;
								CImplicationReapplyConceptSaturationDescriptor* impConSatDes = nullptr;
								if (satConSet->getConceptSaturationDescriptor(concept,conSatDes,impConSatDes)) {
									if (conSatDes && conSatDes->isNegated() != negation) {
										return false;
									}
									if (!conSatDes && impConSatDes) {
										return false;
									}
								} else {
									CIndividualSaturationProcessNode* tmpSubExistentialIndiNode = existentialIndiNode;
									while (tmpSubExistentialIndiNode->hasSubstituteIndividualNode()) {
										CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)tmpSubExistentialIndiNode->getSaturationConceptReferenceLinking();
										CConcept* satConcept = conceptSatItem->getSaturationConcept();
										bool satNegation = conceptSatItem->getSaturationNegation();
										if (satConcept == concept && negation != satNegation) {
											return false;
										}
										tmpSubExistentialIndiNode = tmpSubExistentialIndiNode->getSubstituteIndividualNode();
									}

								}

							} else if (!negation && (conOpCode == CCALL || conOpCode == CCIMPLALL || conOpCode == CCBRANCHALL || conOpCode == CCAQALL || conOpCode == CCIMPLAQALL || conOpCode == CCBRANCHAQALL)) {
								CRole* role = concept->getRole();
								if (backwardRoleSet.contains(role)) {
									return false;
								}
								for (CSortedNegLinker<CConcept*>* operandConLinkerIt = concept->getOperandList(); operandConLinkerIt; operandConLinkerIt = operandConLinkerIt->getNext()) {
									CConcept* operandCon = operandConLinkerIt->getData();
									bool operandConNeg = operandConLinkerIt->isNegated()^negation;
									successorInfluenceConceptsHash.insertMulti(role,TConceptNegationPair(operandCon,operandConNeg));
								}
							}
						}
					}



					if (!existentialIndiNode->getDirectStatusFlags()->hasSuccessorNodeExtensionsFlag()) {

						for (CConceptSaturationDescriptor* conSatDesIt = satConSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
							CConcept* concept = conSatDesIt->getConcept();
							bool negated = conSatDesIt->isNegated();

							cint64 opCode = concept->getOperatorCode();
							//if (!negated && (opCode == CCSOME || opCode == CCAQSOME) || negated && (opCode == CCALL)) {
							cint64 card = concept->getParameter();
							if (!negated && (opCode == CCSOME || opCode == CCAQSOME || opCode == CCATLEAST && card > 0) || negated && (opCode == CCALL || opCode == CCATMOST && card >= 0)) {

								CPROCESSINGLIST<CIndividualSaturationProcessNode*> nextSuccessorList(taskProcessorContext);
								CPROCESSINGLIST<TConceptNegationPair> nextTrivialSuccessorPropagatedConceptList(taskProcessorContext);
								CPROCESSINGSET<CRole*> nextBackwardRoleSet(taskProcessorContext);

								CRole* role = concept->getRole();

								if (!collectSuccessorMergingNodesAndConcepts(existentialIndiNode,role,successorList,*trivialSuccessorPropagatedConceptList,backwardRoleSet,nextSuccessorList,nextTrivialSuccessorPropagatedConceptList,nextBackwardRoleSet,successorInfluenceConceptsHash,calcAlgContext)) {
									return false;
								}							


								if (!nextSuccessorList.isEmpty() || !trivialSuccessorPropagatedConceptList->isEmpty()) {
									CIndividualSaturationProcessNode* extSatIndiNode = getExistentialSaturatedIndividualNodeForConcept(concept,calcAlgContext);
									nextSuccessorList.prepend(extSatIndiNode);

									if (!testSaturatedSuccessorModelMergable(extSatIndiNode,nextSuccessorList,&nextTrivialSuccessorPropagatedConceptList,nextBackwardRoleSet,remainingMergingDepth,remainingMergingCount,calcAlgContext)) {
										return false;
									}
								}

							}
						}
					} else {

						CLinkedRoleSaturationSuccessorHash* linkedSuccHash = existentialIndiNode->getLinkedRoleSuccessorHash(false);
						if (linkedSuccHash) {
							CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* roleLinkedSuccHash = linkedSuccHash->getLinkedRoleSuccessorHash();
							for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator itSucc = roleLinkedSuccHash->constBegin(), itEndSucc = roleLinkedSuccHash->constEnd(); itSucc != itEndSucc; ++itSucc) {
								CRole* role = itSucc.key();
								CLinkedRoleSaturationSuccessorData* linkedSuccData = itSucc.value();
								CPROCESSMAP<cint64,CSaturationSuccessorData*>* succDataMap = linkedSuccData->getSuccessorNodeDataMap(false);
								if (succDataMap) {
									for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succDataMap->constBegin(), itEnd = succDataMap->constEnd(); it != itEnd; ++it) {
										CSaturationSuccessorData* succData = it.value();
										if (succData->mActiveCount >= 1) {
											for (CXNegLinker<CRole*>* creationRoleLinkerIt = succData->mCreationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
												if (!creationRoleLinkerIt->isNegated()) {
													CRole* creationRole = creationRoleLinkerIt->getData();
													if (creationRole == role) {

														CPROCESSINGLIST<CIndividualSaturationProcessNode*> nextSuccessorList(taskProcessorContext);
														CPROCESSINGLIST<TConceptNegationPair> nextTrivialSuccessorPropagatedConceptList(taskProcessorContext);
														CPROCESSINGSET<CRole*> nextBackwardRoleSet(taskProcessorContext);

														if (!collectSuccessorMergingNodesAndConcepts(existentialIndiNode,role,successorList,*trivialSuccessorPropagatedConceptList,backwardRoleSet,nextSuccessorList,nextTrivialSuccessorPropagatedConceptList,nextBackwardRoleSet,successorInfluenceConceptsHash,calcAlgContext)) {
															return false;
														}

														if (!nextSuccessorList.isEmpty() || !trivialSuccessorPropagatedConceptList->isEmpty()) {
															CIndividualSaturationProcessNode* extSatIndiNode = succData->mSuccIndiNode;
															nextSuccessorList.prepend(extSatIndiNode);

															if (!testSaturatedSuccessorModelMergable(extSatIndiNode,nextSuccessorList,&nextTrivialSuccessorPropagatedConceptList,nextBackwardRoleSet,remainingMergingDepth,remainingMergingCount,calcAlgContext)) {
																return false;
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

					return true;
				}

				bool CSatisfiableTaskClassificationMessageAnalyser::testMultipleSaturatedSuccessorModelMergable(CIndividualSaturationProcessNode* existentialIndiNode, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>* trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet, cint64 remainingMergingDepth, cint64& remainingMergingCount, CCalculationAlgorithmContext* calcAlgContext) {

					CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
					// check for problematic forall concepts
					for (CPROCESSINGLIST<CIndividualSaturationProcessNode*>::const_iterator succIt = successorList.constBegin(), succItEnd = successorList.constEnd(); succIt != succItEnd; ++succIt) {
						CIndividualSaturationProcessNode* succIndiNode(*succIt);
						if (succIndiNode != existentialIndiNode) {
							while (succIndiNode->hasSubstituteIndividualNode()) {
								succIndiNode = succIndiNode->getSubstituteIndividualNode();
							}
							CRoleBackwardSaturationPropagationHash* roleBackwardPropHash = succIndiNode->getRoleBackwardPropagationHash(false);
							if (roleBackwardPropHash) {
								CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* roleBackwardPropDataHash = roleBackwardPropHash->getRoleBackwardPropagationDataHash();	
								for (CPROCESSINGSET<CRole*>::const_iterator it = backwardRoleSet.constBegin(), itEnd = backwardRoleSet.constEnd(); it != itEnd; ++it) {
									CRole* backwardRole(*it);
									const CRoleBackwardSaturationPropagationHashData& roleBackwardRoleData = roleBackwardPropDataHash->value(backwardRole);
									if (roleBackwardRoleData.mLinkLinker) {
										return false;
									}
								}
							}
						}
					}



					CPROCESSINGHASH<cint64,CConceptNegationTriggerItem> conceptNegationTriggerHash(taskProcessorContext);
					CPROCESSINGHASH<CRole*,TConceptNegationPair> successorInfluenceConceptsHash(taskProcessorContext);
					if (trivialSuccessorPropagatedConceptList) {
						for (CPROCESSINGLIST<TConceptNegationPair>::const_iterator trivConIt = trivialSuccessorPropagatedConceptList->constBegin(), trivConItEnd = trivialSuccessorPropagatedConceptList->constEnd(); trivConIt != trivConItEnd; ++trivConIt) {
							TConceptNegationPair conNegPair(*trivConIt);
							CConcept* concept = conNegPair.first;
							bool negation = conNegPair.second;
							cint64 conOpCode = concept->getOperatorCode();

							if (negation && conOpCode == CCSUB) {
								CConceptNegationTriggerItem& conNegTriggerItem = conceptNegationTriggerHash[concept->getConceptTag()];
								if (conNegTriggerItem.mTriggerFlag) {
									return false;
								}
								if (conNegTriggerItem.mConcept && conNegTriggerItem.mNegationFlag != negation) {
									return false;
								}
								conNegTriggerItem.mNegationFlag = negation;
								conNegTriggerItem.mConcept = concept;
							} else if (!negation && (conOpCode == CCALL || conOpCode == CCIMPLALL || conOpCode == CCBRANCHALL || conOpCode == CCAQALL || conOpCode == CCIMPLAQALL || conOpCode == CCBRANCHAQALL)) {
								CRole* role = concept->getRole();
								if (backwardRoleSet.contains(role)) {
									return false;
								}
								for (CSortedNegLinker<CConcept*>* operandConLinkerIt = concept->getOperandList(); operandConLinkerIt; operandConLinkerIt = operandConLinkerIt->getNext()) {
									CConcept* operandCon = operandConLinkerIt->getData();
									bool operandConNeg = operandConLinkerIt->isNegated()^negation;
									successorInfluenceConceptsHash.insertMulti(role,TConceptNegationPair(operandCon,operandConNeg));
								}
							}
						}
					}



					for (CPROCESSINGLIST<CIndividualSaturationProcessNode*>::const_iterator succIt = successorList.constBegin(), succItEnd = successorList.constEnd(); succIt != succItEnd; ++succIt) {
						CIndividualSaturationProcessNode* succIndiNode(*succIt);

						while (succIndiNode->hasSubstituteIndividualNode()) {
							CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)succIndiNode->getSaturationConceptReferenceLinking();
							CConcept* satConcept = conceptSatItem->getSaturationConcept();
							bool satNegation = conceptSatItem->getSaturationNegation();
							CConceptNegationTriggerItem& conNegTriggerItem = conceptNegationTriggerHash[satConcept->getConceptTag()];
							if (conNegTriggerItem.mTriggerFlag) {
								return false;
							}
							if (conNegTriggerItem.mConcept && conNegTriggerItem.mNegationFlag != satNegation) {
								return false;
							}
							if (!conNegTriggerItem.mIndiSatNode) {
								conNegTriggerItem.mIndiSatNode = succIndiNode;
							}
							conNegTriggerItem.mNegationFlag = satNegation;
							conNegTriggerItem.mConcept = satConcept;

							succIndiNode = succIndiNode->getSubstituteIndividualNode();
						}

						CReapplyConceptSaturationLabelSet* satConSet = succIndiNode->getReapplyConceptSaturationLabelSet(false);
						if (satConSet) {
							CReapplyConceptSaturationLabelSetIterator satConSetIt = satConSet->getIterator(true,true);
							while (satConSetIt.hasNext()) {
								CConceptSaturationDescriptor* conSatDes = satConSetIt.getConceptSaturationDescriptor();
								CImplicationReapplyConceptSaturationDescriptor* implTrigger = satConSetIt.getImplicationReapplyConceptSaturationDescriptor();
								CConceptNegationTriggerItem& conNegTriggerItem = conceptNegationTriggerHash[satConSetIt.getDataTag()];
								if (conSatDes) {
									CConcept* satConcept = conSatDes->getConcept();
									bool satNegation = conSatDes->isNegated();
									if (conNegTriggerItem.mTriggerFlag) {
										return false;
									}
									if (conNegTriggerItem.mConcept && conNegTriggerItem.mNegationFlag != satNegation) {
										return false;
									}
									if (!conNegTriggerItem.mIndiSatNode) {
										conNegTriggerItem.mIndiSatNode = succIndiNode;
									}
									conNegTriggerItem.mNegationFlag = satNegation;
									conNegTriggerItem.mConcept = satConcept;
								} else if (implTrigger) {
									if (conNegTriggerItem.mConcept) {
										return false;
									}
									conNegTriggerItem.mTriggerFlag = true;
								}
								satConSetIt.moveNext();
							}
						}
					}

					for (CPROCESSINGHASH<cint64,CConceptNegationTriggerItem>::const_iterator conIt = conceptNegationTriggerHash.constBegin(), conItEnd = conceptNegationTriggerHash.constEnd(); conIt != conItEnd; ++conIt) {
						CConceptNegationTriggerItem conNegTriggerItem(conIt.value());
						if (conNegTriggerItem.mConcept) {
							CConcept* concept = conNegTriggerItem.mConcept;
							bool negated = conNegTriggerItem.mNegationFlag;
							CIndividualSaturationProcessNode* satIndiNode = conNegTriggerItem.mIndiSatNode;

							cint64 opCode = concept->getOperatorCode();
							//if (!negated && (opCode == CCSOME || opCode == CCAQSOME) || negated && (opCode == CCALL)) {
							cint64 card = concept->getParameter();
							if (!negated && (opCode == CCSOME || opCode == CCAQSOME || opCode == CCATLEAST && card > 0) || negated && (opCode == CCALL || opCode == CCATMOST && card >= 0)) {

								CPROCESSINGLIST<CIndividualSaturationProcessNode*> nextSuccessorList(taskProcessorContext);
								CPROCESSINGLIST<TConceptNegationPair> nextTrivialSuccessorPropagatedConceptList(taskProcessorContext);
								CPROCESSINGSET<CRole*> nextBackwardRoleSet(taskProcessorContext);

								CRole* role = concept->getRole();

								if (!collectSuccessorMergingNodesAndConcepts(satIndiNode,role,successorList,*trivialSuccessorPropagatedConceptList,backwardRoleSet,nextSuccessorList,nextTrivialSuccessorPropagatedConceptList,nextBackwardRoleSet,successorInfluenceConceptsHash,calcAlgContext)) {
									return false;
								}							


								if (!nextSuccessorList.isEmpty() || !trivialSuccessorPropagatedConceptList->isEmpty()) {
									CIndividualSaturationProcessNode* extSatIndiNode = getExistentialSaturatedIndividualNodeForConcept(concept,calcAlgContext);
									nextSuccessorList.prepend(extSatIndiNode);

									if (!testSaturatedSuccessorModelMergable(extSatIndiNode,nextSuccessorList,&nextTrivialSuccessorPropagatedConceptList,nextBackwardRoleSet,remainingMergingDepth,remainingMergingCount,calcAlgContext)) {
										return false;
									}
								}

							}

						}
					}


					for (CPROCESSINGLIST<CIndividualSaturationProcessNode*>::const_iterator succIt = successorList.constBegin(), succItEnd = successorList.constEnd(); succIt != succItEnd; ++succIt) {
						CIndividualSaturationProcessNode* succIndiNode(*succIt);

						if (succIndiNode->getDirectStatusFlags()->hasSuccessorNodeExtensionsFlag()) {
							CLinkedRoleSaturationSuccessorHash* linkedSuccHash = succIndiNode->getLinkedRoleSuccessorHash(false);
							if (linkedSuccHash) {
								CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* roleLinkedSuccHash = linkedSuccHash->getLinkedRoleSuccessorHash();
								for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator itSucc = roleLinkedSuccHash->constBegin(), itEndSucc = roleLinkedSuccHash->constEnd(); itSucc != itEndSucc; ++itSucc) {
									CRole* role = itSucc.key();
									CLinkedRoleSaturationSuccessorData* linkedSuccData = itSucc.value();
									CPROCESSMAP<cint64,CSaturationSuccessorData*>* succDataMap = linkedSuccData->getSuccessorNodeDataMap(false);
									if (succDataMap) {
										for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succDataMap->constBegin(), itEnd = succDataMap->constEnd(); it != itEnd; ++it) {
											CSaturationSuccessorData* succData = it.value();
											if (succData->mActiveCount >= 1) {
												for (CXNegLinker<CRole*>* creationRoleLinkerIt = succData->mCreationRoleLinker; creationRoleLinkerIt; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
													if (!creationRoleLinkerIt->isNegated()) {
														CRole* creationRole = creationRoleLinkerIt->getData();
														if (creationRole == role) {

															CPROCESSINGLIST<CIndividualSaturationProcessNode*> nextSuccessorList(taskProcessorContext);
															CPROCESSINGLIST<TConceptNegationPair> nextTrivialSuccessorPropagatedConceptList(taskProcessorContext);
															CPROCESSINGSET<CRole*> nextBackwardRoleSet(taskProcessorContext);

															if (!collectSuccessorMergingNodesAndConcepts(succIndiNode,role,successorList,*trivialSuccessorPropagatedConceptList,backwardRoleSet,nextSuccessorList,nextTrivialSuccessorPropagatedConceptList,nextBackwardRoleSet,successorInfluenceConceptsHash,calcAlgContext)) {
																return false;
															}

															if (!nextSuccessorList.isEmpty() || !trivialSuccessorPropagatedConceptList->isEmpty()) {
																CIndividualSaturationProcessNode* extSatIndiNode = succData->mSuccIndiNode;
																nextSuccessorList.prepend(extSatIndiNode);

																if (!testSaturatedSuccessorModelMergable(extSatIndiNode,nextSuccessorList,&nextTrivialSuccessorPropagatedConceptList,nextBackwardRoleSet,remainingMergingDepth,remainingMergingCount,calcAlgContext)) {
																	return false;
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
					}

					return true;

				}













				bool CSatisfiableTaskClassificationMessageAnalyser::collectSuccessorMergingNodesAndConcepts(CIndividualSaturationProcessNode* excludeSaturationIndiNode, CRole* role, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>& trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet,
						CPROCESSINGLIST<CIndividualSaturationProcessNode*>& nextSuccessorList, CPROCESSINGLIST<TConceptNegationPair>& nextTrivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& nextBackwardRoleSet,
						CPROCESSINGHASH<CRole*,TConceptNegationPair>& successorInfluenceConceptsHash, CCalculationAlgorithmContext* calcAlgContext) {

					for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
						CRole* superRole = superRoleIt->getData();
						bool superRoleInversed = superRoleIt->isNegated();

						if (!superRoleInversed) {


							for (CPROCESSINGHASH<CRole*,TConceptNegationPair>::const_iterator it = successorInfluenceConceptsHash.constFind(superRole), itEnd = successorInfluenceConceptsHash.constEnd(); it != itEnd; ++it) {
								CRole* hashRole(it.key());
								if (role == hashRole) {
									TConceptNegationPair conNegPair(it.value());
									CConcept* reapplyOperandCon = conNegPair.first;
									bool reapplyOperandConNeg = conNegPair.second;

									CIndividualSaturationProcessNode* satIndiNode = getSaturatedIndividualNodeForConcept(reapplyOperandCon,reapplyOperandConNeg,calcAlgContext);
									if (!satIndiNode) {
										if (!collectTrivialPropagationTestingConcepts(reapplyOperandCon,reapplyOperandConNeg,nextTrivialSuccessorPropagatedConceptList,calcAlgContext)) {
											return false;
										}
									} else if (!satIndiNode->isCompleted() || satIndiNode->getIndirectStatusFlags()->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYPROPLEMATIC | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGNOMINALCONNECTION,false)) {
										return false;
									}

									if (satIndiNode) {
										nextSuccessorList.append(satIndiNode);
									}
								} else {
									break;
								}
							}


							for (CPROCESSINGLIST<CIndividualSaturationProcessNode*>::const_iterator succIt = successorList.constBegin(), succItEnd = successorList.constEnd(); succIt != succItEnd; ++succIt) {
								CIndividualSaturationProcessNode* succIndiNode(*succIt);

								while (succIndiNode->hasSubstituteIndividualNode()) {
									succIndiNode = succIndiNode->getSubstituteIndividualNode();
								}

								if (succIndiNode != excludeSaturationIndiNode) {

									CRoleBackwardSaturationPropagationHash* roleBackPropHash = succIndiNode->getRoleBackwardPropagationHash(false);
									if (roleBackPropHash) {
										CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* roleBackPropDataHash = roleBackPropHash->getRoleBackwardPropagationDataHash();
										const CRoleBackwardSaturationPropagationHashData& backPropData = roleBackPropDataHash->value(superRole);
										for (CBackwardSaturationPropagationReapplyDescriptor* reapplyLinkerIt = backPropData.mReapplyLinker; reapplyLinkerIt; reapplyLinkerIt = reapplyLinkerIt->getNext()) {
											CConceptSaturationDescriptor* reapplyConDes = reapplyLinkerIt->getReapplyConceptSaturationDescriptor();


											CConcept* reapplyConcept = reapplyConDes->getConcept();
											bool reapplyConceptNegation = reapplyConDes->isNegated();
											cint64 reapplyConceptOpCode = reapplyConcept->getOperatorCode();
											if (reapplyConceptNegation && (reapplyConceptOpCode == CCSOME) || !reapplyConceptNegation && (reapplyConceptOpCode == CCALL || reapplyConceptOpCode == CCIMPLALL || reapplyConceptOpCode == CCBRANCHALL || reapplyConceptOpCode == CCAQALL || reapplyConceptOpCode == CCIMPLAQALL || reapplyConceptOpCode == CCBRANCHAQALL)) {
												for (CSortedNegLinker<CConcept*>* reapplyOperandConLinkerIt = reapplyConcept->getOperandList(); reapplyOperandConLinkerIt; reapplyOperandConLinkerIt = reapplyOperandConLinkerIt->getNext()) {
													CConcept* reapplyOperandCon = reapplyOperandConLinkerIt->getData();
													bool reapplyOperandConNeg = reapplyOperandConLinkerIt->isNegated()^reapplyConceptNegation;

													CIndividualSaturationProcessNode* satIndiNode = getSaturatedIndividualNodeForConcept(reapplyOperandCon,reapplyOperandConNeg,calcAlgContext);
													if (!satIndiNode) {
														if (!collectTrivialPropagationTestingConcepts(reapplyOperandCon,reapplyOperandConNeg,nextTrivialSuccessorPropagatedConceptList,calcAlgContext)) {
															return false;
														}
													} else if (!satIndiNode->isCompleted() || satIndiNode->getIndirectStatusFlags()->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYPROPLEMATIC | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGNOMINALCONNECTION,false)) {
														return false;
													}

													nextSuccessorList.append(satIndiNode);
												}
											} else {
												return false;
											}

										}
									}
								}
							}
						} else {
							nextBackwardRoleSet.insert(superRole);
						}
					}

					return true;
				}





				CIndividualSaturationProcessNode* CSatisfiableTaskClassificationMessageAnalyser::getExistentialSaturatedIndividualNodeForConcept(CConcept* concept, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualSaturationProcessNode* satIndiNode = nullptr;
					CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
					if (conProcData) {
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getExistentialSuccessorConceptSaturationReferenceLinkingData();
							if (satCalcRefLinkData) {
								satIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
							}
						}
					}
					if (!satIndiNode) {
						CSortedNegLinker<CConcept*>* opConceptLinker = concept->getOperandList();
						CConcept* opConcept = nullptr;
						bool opNegation = false;
						if (!opConceptLinker) {
							opConcept = calcAlgContext->getProcessingDataBox()->getOntologyTopConcept();
						} else {
							if (!opConceptLinker->hasNext()) {
								opConcept = opConceptLinker->getData();
								bool negateOp = false;
								if (concept->getOperatorCode() == CCALL) {
									negateOp = true;
								}
								opNegation = opConceptLinker->isNegated()^negateOp;
							}
						}
						if (opConcept) {
							satIndiNode = getSaturatedIndividualNodeForConcept(opConcept,opNegation,calcAlgContext);
						}
					}
					return satIndiNode;
				}

				CIndividualSaturationProcessNode* CSatisfiableTaskClassificationMessageAnalyser::getSaturatedIndividualNodeForConcept(CConcept* concept, bool conceptNegation, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualSaturationProcessNode* satIndiNode = nullptr;
					CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
					if (conProcData) {
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(conceptNegation);
							if (satCalcRefLinkData) {
								satIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
							}
						}
					}
					return satIndiNode;
				}


				bool CSatisfiableTaskClassificationMessageAnalyser::testSubsumerCandidatePossibleWithMergedSaturatedModel(CIndividualProcessNode* indiNode, CConcept* testConcept, bool negation, bool& mergeSatisfieableFlag, CCalculationAlgorithmContext* calcAlgContext) {

					CIndividualSaturationProcessNode* saturationIndiNode = getSaturatedIndividualNodeForConcept(testConcept,negation,calcAlgContext);
					if (saturationIndiNode) {

						if (saturationIndiNode->isCompleted() && !saturationIndiNode->getIndirectStatusFlags()->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCARDINALITYPROPLEMATIC | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGNOMINALCONNECTION,false)) {


							bool clashedFlag = false;

							if (!testConceptSetWithSaturatedModelMergable(indiNode,saturationIndiNode,clashedFlag,calcAlgContext)) {
								return false;
							}
							if (clashedFlag) {
								mergeSatisfieableFlag = false;
								return true;
							}

							if (!testRoleSuccessorsWithSaturatedModelMergable(indiNode,saturationIndiNode,calcAlgContext)) {
								return false;
							}

							if (!testSaturatedExistentialsModelMergable(indiNode,saturationIndiNode,calcAlgContext)) {
								return false;
							}
							mergeSatisfieableFlag = true;
							return true;

						}
						return false;
					}
					return false;
				}

				bool CSatisfiableTaskClassificationMessageAnalyser::testEquivalenceConceptAlternatives(CIndividualProcessNode* indiNode, CPROCESSINGSET< QPair<CConcept*,bool> >& alternativesSet, CPROCESSINGHASH< QPair<CConcept*,bool>,CSaturatedMergedTestItem >* testSaturatedMergedHash, bool& oneMergeSatisfieableFlag, bool& allMergeUnsatisfieableFlag, CCalculationAlgorithmContext* calcAlgContext) {
					for (CPROCESSINGSET< QPair<CConcept*,bool> >::const_iterator it = alternativesSet.constBegin(), itEnd = alternativesSet.constEnd(); it != itEnd; ++it) {
						QPair<CConcept*,bool> conNegPair(*it);
						CConcept* concept = conNegPair.first;
						bool negation = conNegPair.second;
						bool mergedSatisfiableFlag = false;
						STATINC(ANALYSECANDIDATECONCEPTSATURATEDALTERNATIVEMERGINGCOUNT,calcAlgContext);
						bool mergedSuccessfullyFlag = testSubsumerCandidatePossibleWithMergedSaturatedModel(indiNode,concept,negation,mergedSatisfiableFlag,calcAlgContext);
						CSaturatedMergedTestItem& saturatedMergedTestItem = (*testSaturatedMergedHash)[conNegPair];
						saturatedMergedTestItem.mSuccessfullyMerged = mergedSuccessfullyFlag;
						saturatedMergedTestItem.mSatisfiableMerged = mergedSatisfiableFlag;
						if (!mergedSuccessfullyFlag) {
							allMergeUnsatisfieableFlag = false;
						} else {
							if (mergedSatisfiableFlag) {
								oneMergeSatisfieableFlag = true;
								allMergeUnsatisfieableFlag = false;
								return true;
							}
						}
					}
					if (allMergeUnsatisfieableFlag) {
						return true;
					}
					return false;
				}


				bool CSatisfiableTaskClassificationMessageAnalyser::collectEquivalenceConceptAlternatives(CIndividualProcessNode* indiNode, CConcept* testConcept, bool testConceptNegation, CPROCESSINGSET< QPair<CConcept*,bool> >& alternativesSet, CPROCESSINGHASH< QPair<CConcept*,bool>,CSaturatedMergedTestItem >* testSaturatedMergedHash, bool& oneMergeSatisfieableFlag, bool& allMergeUnsatisfieableFlag, CCalculationAlgorithmContext* calcAlgContext) {
					CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
					CPROCESSINGLIST< QPair<CConcept*,bool> > alternativeExpList(taskProcessorContext);
					CPROCESSINGSET< QPair<CConcept*,bool> > alternativeExpSet(taskProcessorContext);
					CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);

					allMergeUnsatisfieableFlag = true;
					QPair<CConcept*,bool> conNegPair(testConcept,testConceptNegation);
					alternativeExpList.append(conNegPair);
					alternativeExpSet.insert(conNegPair);
					while (!alternativeExpList.isEmpty()) {
						conNegPair = alternativeExpList.takeFirst();
						CConcept* concept = conNegPair.first;
						bool negation = conNegPair.second;
						cint64 conOpCode = concept->getOperatorCode();
						if (negation && (conOpCode == CCEQ || conOpCode == CCAND) || !negation && (conOpCode == CCOR)) {
							for (CSortedNegLinker<CConcept*>* operandConceptLinkerIt = concept->getOperandList(); operandConceptLinkerIt; operandConceptLinkerIt = operandConceptLinkerIt->getNext()) {
								CConcept* operandConcept = operandConceptLinkerIt->getData();
								bool operandNegation = operandConceptLinkerIt->isNegated()^negation;
								QPair<CConcept*,bool> opConNegPair(operandConcept,operandNegation);
								if (!alternativeExpSet.contains(opConNegPair)) {
									alternativeExpSet.insert(opConNegPair);
									alternativeExpList.append(opConNegPair);
								}
							}
						} else if (conOpCode == CCAQCHOOCE) {

							for (CSortedNegLinker<CConcept*>* operandConceptLinkerIt = concept->getOperandList(); operandConceptLinkerIt; operandConceptLinkerIt = operandConceptLinkerIt->getNext()) {
								CConcept* operandConcept = operandConceptLinkerIt->getData();
								bool operandNegation = operandConceptLinkerIt->isNegated();
								if (negation == operandNegation) {
									QPair<CConcept*,bool> opConNegPair(operandConcept,operandNegation);
									if (!alternativeExpSet.contains(opConNegPair)) {
										alternativeExpSet.insert(opConNegPair);
										alternativeExpList.append(opConNegPair);
									}
								}
							}

						} else if (negation && (conOpCode == CCALL) || !negation && (conOpCode == CCSOME || conOpCode == CCAQSOME)) {

							bool containsNegation = false;
							if (!conSet->containsConcept(concept,&containsNegation)) {
								CSaturatedMergedTestItem& saturatedMergedTestItem = (*testSaturatedMergedHash)[conNegPair];
								if (saturatedMergedTestItem.mSuccessfullyMerged) {
									if (saturatedMergedTestItem.mSatisfiableMerged) {
										oneMergeSatisfieableFlag = true;
										allMergeUnsatisfieableFlag = false;
										return true;
									}								
								} else {
									alternativesSet.insert(conNegPair);
								}
							} else {
								if (containsNegation == negation) {
									oneMergeSatisfieableFlag = true;
									allMergeUnsatisfieableFlag = false;
									return true;
								}
							}

						} else {
							bool containsNegation = false;
							if (!conSet->containsConcept(concept,&containsNegation)) {
								allMergeUnsatisfieableFlag = false;
							} else {
								if (containsNegation == negation) {
									oneMergeSatisfieableFlag = true;
									allMergeUnsatisfieableFlag = false;
									return true;
								}
							} 
						}
					}
					if (alternativesSet.isEmpty() && allMergeUnsatisfieableFlag) {
						return true;
					}
					return false;
				}



				CClassificationMessageData* CSatisfiableTaskClassificationMessageAnalyser::extractPossibleSubsumptionInformation(CConceptDescriptor* conDes, CIndividualProcessNode* indiNode, CReapplyConceptLabelSet* conSet, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(ANALYSEPOSSIBLESUBSUMPTIONSCOUNT,calcAlgContext);

					CClassificationMessageData* messageDataLinker = nullptr;

					CConcept* testingConcept = conDes->getConcept();
					bool negated = conDes->getNegation();

					//if (CIRIName::getRecentIRIName(testingConcept->getClassNameLinker()) == "http://www.owllink.org/testsuite/particle-D#Neutrino") {
					//	bool bug = true;
					//}
					CConceptReferenceLinking* conRefLinking = nullptr;
					CConceptData* conData = testingConcept->getConceptData();
					if (conData && !mUseAlwaysConRefLinkDataHash) {
						CConceptProcessData* conProcData = (CConceptProcessData*)conData;
						if (!conProcData->isInvalidatedReferenceLinking()) {
							conRefLinking = conProcData->getConceptReferenceLinking();
						}
					}
					CClassificationConceptReferenceLinking* classConRefLinkData = nullptr;
					if (conRefLinking) {
						CConceptSatisfiableReferenceLinkingData* conSatRefLinkData = (CConceptSatisfiableReferenceLinkingData*)conRefLinking;
						classConRefLinkData = conSatRefLinkData->getClassifierReferenceLinkingData();
					} else {
						classConRefLinkData = mConRefLinkDataHash->value(testingConcept);
					}
					if (classConRefLinkData) {
						CClassificationSatisfiablePossibleSubsumptionCalculationConceptReferenceLinking* possSubsumRefLinkData = (CClassificationSatisfiablePossibleSubsumptionCalculationConceptReferenceLinking*)classConRefLinkData;
						if (possSubsumRefLinkData) {
							if (!possSubsumRefLinkData->isPossibleSubsumptionMapInitialized()) {
								// initialize possible subsumption list
								CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleClassSubsumptionData*>* possSubsumerList = nullptr;

								if (!mMultiplePossSubsumInitAvoidHash) {
									mMultiplePossSubsumInitAvoidHash = CObjectParameterizingAllocator< CPROCESSINGHASH< CConcept*,CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleClassSubsumptionData*>* >,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getTemporaryMemoryAllocationManager(),calcAlgContext->getTaskProcessorContext());
								}
								possSubsumerList = mMultiplePossSubsumInitAvoidHash->value(testingConcept);

								CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true,true,false);
								if (possSubsumerList) {

									// prune already initialization list for possible subsumers
									CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleClassSubsumptionData*>::iterator itPossSubsum = possSubsumerList->begin(), itPossSubsumEnd = possSubsumerList->end();
									while (conSetIt.hasNext() && itPossSubsum != itPossSubsumEnd) {
										CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
										CConcept* concept = conDes->getConcept();
										bool negated = conDes->getNegation();

										CClassificationInitializePossibleClassSubsumptionData* possSubsumConData = *itPossSubsum;
										CConcept* possSubsumConcept = possSubsumConData->getPossibleSubsumerConcept();
										cint64 conDesTag = conDes->getConceptTag();
										cint64 possSubsumConTag = possSubsumConcept->getConceptTag();

										if (conDesTag < possSubsumConTag) {
											conSetIt.moveNext();
										} else if (conDesTag == possSubsumConTag) {

											//if (concept->getOperatorCode() == CCEQCAND) {
											//	if (!testSubsumerCandidatePossibleWithMergedSaturatedModel(indiNode,concept->getOperandList()->getData(),calcAlgContext)) {
											//		possSubsumConData->setPossibleSubsumerInvalid();
											//	}
											//}

											conSetIt.moveNext();
											++itPossSubsum;
										} else {
											possSubsumConData->setPossibleSubsumerInvalid();
											++itPossSubsum;
										}
									}
									while (itPossSubsum != itPossSubsumEnd) {
										CClassificationInitializePossibleClassSubsumptionData* subsumData(*itPossSubsum);
										subsumData->setPossibleSubsumerInvalid();
										++itPossSubsum;
									}

								} else {
									// create new initialization list for possible subsumers
									while (conSetIt.hasNext()) {
										CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
										CConcept* concept = conDes->getConcept();
										bool negated = conDes->getNegation();


										if (!negated && (concept->hasClassName() && concept->getConceptTag() != 1 && concept != testingConcept)) {
											if (!possSubsumerList) {
												possSubsumerList = CObjectParameterizingAllocator< CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleClassSubsumptionData*>,CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTmpContext);
											}
											CClassificationInitializePossibleClassSubsumptionData* possSubsumData = CObjectAllocator< CClassificationInitializePossibleClassSubsumptionData >::allocateAndConstruct(mTempMemAllocMan);
											possSubsumData->initClassificationPossibleSubsumptionData(concept);
											possSubsumerList->append(possSubsumData);
										}
										if (concept->getOperatorCode() == CCEQCAND) {
											CConcept* eqConcept = concept->getOperandList()->getData();

											//if (CIRIName::getRecentIRIName(testingConcept->getClassNameLinker()) == "http://oiled.man.example.net/facts#All+Friends+Happy+or+All+Friends+Students") {
											//	if (CIRIName::getRecentIRIName(eqConcept->getClassNameLinker()) == "http://oiled.man.example.net/facts#All+Friends+Happy+or+Students") {
											//		bool bug = true;
											//	}
											//}



											if (testSubsumerCandidatePossibleWithMergedSaturatedModel(indiNode,eqConcept,calcAlgContext)) {
												if (!possSubsumerList) {
													possSubsumerList = CObjectParameterizingAllocator< CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleClassSubsumptionData*>,CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTmpContext);
												}
												CClassificationInitializePossibleClassSubsumptionData* possSubsumData = CObjectAllocator< CClassificationInitializePossibleClassSubsumptionData >::allocateAndConstruct(mTempMemAllocMan);
												possSubsumData->initClassificationPossibleSubsumptionData(concept);
												possSubsumerList->append(possSubsumData);
											}
										}
										conSetIt.moveNext();
									}

									CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CConcept*>* eqConNonCandPossSubsumerList = nullptr;

									bool eqConceptsNonCandidatePossSubsumers = false;
									QSet<CConcept*>* equivConceptNonCandidateSet = calcAlgContext->getUsedProcessingDataBox()->getOntology()->getTBox()->getEquivalentConceptNonCandidateSet(false);
									if (equivConceptNonCandidateSet) {
										eqConceptsNonCandidatePossSubsumers = true;
										for (QSet<CConcept*>::const_iterator it = equivConceptNonCandidateSet->constBegin(), itEnd = equivConceptNonCandidateSet->constEnd(); it != itEnd; ++it) {
											CConcept* eqConcept = *it;

											//if (CIRIName::getRecentIRIName(testingConcept->getClassNameLinker()) == "http://oiled.man.example.net/facts#All+Friends+Happy+or+All+Friends+Students") {
											//	if (CIRIName::getRecentIRIName(eqConcept->getClassNameLinker()) == "http://oiled.man.example.net/facts#All+Friends+Happy+or+Students") {
											//		bool bug = true;
											//	}
											//}

											if (testSubsumerCandidatePossibleWithMergedSaturatedModel(indiNode,eqConcept,calcAlgContext)) {
												if (!eqConNonCandPossSubsumerList) {
													eqConNonCandPossSubsumerList = CObjectParameterizingAllocator< CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CConcept*>,CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTmpContext);
												}
												eqConNonCandPossSubsumerList->append(eqConcept);
											}
										}
									}

									if (possSubsumerList) {
										mMultiplePossSubsumInitAvoidHash->insert(testingConcept,possSubsumerList);
									}

									CClassificationInitializePossibleClassSubsumptionMessageData* possSubsumMessageData = CObjectAllocator<CClassificationInitializePossibleClassSubsumptionMessageData>::allocateAndConstruct(mTempMemAllocMan);
									possSubsumMessageData->initClassificationPossibleSubsumptionMessageData(testingConcept,possSubsumerList,eqConceptsNonCandidatePossSubsumers,eqConNonCandPossSubsumerList);
									messageDataLinker = possSubsumMessageData->append(messageDataLinker);
								}
							} else {
								CClassPossibleSubsumptionMap* possSubsumMap = possSubsumRefLinkData->getClassPossibleSubsumptionMap();
								if (possSubsumMap && possSubsumMap->hasRemainingPossibleSubsumptions()) {
									// update / prune possible subsumer list

									bool updatedPossSubsumptions = false;

									CClassPossibleSubsumptionMapIterator* possSubsumIt = possSubsumMap->getIterator(mReusePossSubsumMapIterator,calcAlgContext);
									mReusePossSubsumMapIterator = possSubsumIt;
									CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true,true,false);
									while (possSubsumIt->hasNext() && conSetIt.hasNext()) {
										CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
										CConcept* concept = conDes->getConcept();
										bool negated = conDes->getNegation();
										CConcept* possSubCon = possSubsumIt->getSubsumptionConcept();
										cint64 conDesTag = conDes->getConceptTag();
										cint64 possSubsumConTag = possSubCon->getConceptTag();

										if (conDesTag == possSubsumConTag) {

											//if (concept->getOperatorCode() == CCEQCAND) {
											//	if (!testSubsumerCandidatePossibleWithMergedSaturatedModel(indiNode,concept->getOperandList()->getData(),calcAlgContext)) {
											//		updatedPossSubsumptions |= possSubsumIt->invalidateSubsumption();
											//	}
											//}

											possSubsumIt->moveNext();
											conSetIt.moveNext();
										} else if (conDesTag < possSubsumConTag) {
											conSetIt.moveNext();
										} else if (possSubsumConTag < conDesTag) {
											if (possSubCon->getOperatorCode() != CCEQ) {
												updatedPossSubsumptions |= possSubsumIt->invalidateSubsumption();
											}
											possSubsumIt->moveNext();
										}
									}
									while (possSubsumIt->hasNext()) {
										CConcept* possSubCon = possSubsumIt->getSubsumptionConcept();
										if (possSubCon->getOperatorCode() != CCEQ) {
											possSubsumIt->invalidateSubsumption();
										}
										possSubsumIt->moveNext();
									}

									if (updatedPossSubsumptions) {
										CClassificationUpdatePossibleClassSubsumptionMessageData* possSubsumMessageData = CObjectAllocator<CClassificationUpdatePossibleClassSubsumptionMessageData>::allocateAndConstruct(mTempMemAllocMan);
										possSubsumMessageData->initClassificationPossibleSubsumptionMessageData(testingConcept);
										messageDataLinker = possSubsumMessageData->append(messageDataLinker);
									}

								}
							}
						}
					}
					return messageDataLinker;
				}



				void CSatisfiableTaskClassificationMessageAnalyser::verifySubsumptionPossibleWithModel(CIndividualProcessNode* indiNode, CConcept* testingConcept, CClassificationMessageData* possSubsumMessageData, CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList) {
					if (possSubsumMessageData) {
						if (possSubsumMessageData->getClassificationMessageDataType() == CClassificationInitializePossibleClassSubsumptionMessageData::TELLCLASSINITIALIZEPOSSIBLESUBSUM) {
							CClassificationInitializePossibleClassSubsumptionMessageData* initPossSubMessageData = (CClassificationInitializePossibleClassSubsumptionMessageData*)possSubsumMessageData;
							if (initPossSubMessageData) {
								CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleClassSubsumptionData*>* possSubsumList = initPossSubMessageData->getClassPossibleSubsumerList();
								if (possSubsumList) {
									for (CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleClassSubsumptionData*>::const_iterator it = possSubsumList->constBegin(), itEnd = possSubsumList->constEnd(); it != itEnd; ++it) {
										CClassificationInitializePossibleClassSubsumptionData* possSubsumData = *it;
										if (possSubsumData->isPossibleSubsumerValid()) {
											CConcept* possSubsumerConcept = possSubsumData->getPossibleSubsumerConcept();
											if (possSubsumerConcept->getOperatorCode() == CCEQCAND) {
												possSubsumerConcept = possSubsumerConcept->getOperandList()->getData();
											}										
										}
									}
								}
							}
						}
					}
				}



				void CSatisfiableTaskClassificationMessageAnalyser::checkCanHaveClashWithModel(CIndividualProcessNode* indiNode, CConcept* concept, bool negated, bool& clashFoundFlag, bool& unknownFlag, bool& clashFreeFlag, cint64 depth, QSet<CIndividualProcessNode*>* testedIndividualsSet, CIndividualProcessNode* lastNode, CIndividualProcessNodeVector* indiNodeVec) {
					if (depth > mMaxPossibleSubsumerNegationCheckingDepth) {
						unknownFlag = true;
					} else {
						if (indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION) || (lastNode != indiNode && testedIndividualsSet->contains(indiNode))) {
							unknownFlag = true;
						} else {
							if (lastNode != indiNode) {
								testedIndividualsSet->insert(indiNode);
							}
							CReapplyConceptLabelSet* labelSet = indiNode->getReapplyConceptLabelSet(false);
							bool containsNegated = false;
							CConceptDescriptor* conDes = nullptr;
							CCondensedReapplyQueue* reapplyQueue = labelSet->getConceptDescriptorAndReapplyQueue(concept,conDes,false);
							if (conDes || reapplyQueue) {
								if (conDes) {
									containsNegated = conDes->getNegation();
									if (containsNegated ^ negated) {
										CDependencyTrackPoint* depTrackPoint = conDes->getDependencyTrackPoint();
										if (!depTrackPoint || depTrackPoint->getBranchingTag() > 0) {
											unknownFlag = true;
										} else {
											clashFoundFlag = true;
										}
									} else {
										clashFreeFlag = true;
									}
								} else {
									unknownFlag = true;
								}
							} else {
								// not contained by the model
								cint64 opCode = concept->getOperatorCode();
								cint64 opCount = concept->getOperandCount();
								CConceptOperator* conOperator = concept->getConceptOperator();

								if (negated && (opCode == CCAND) || !negated && (opCode == CCOR || opCode == CCEQ)) {
									bool allClashFoundFlag = true;
									bool oneClashFreeFoundFlag = false;
									bool oneUnknownFound = false;
									for (CSortedNegLinker<CConcept*>* operandLinker = concept->getOperandList(); !oneClashFreeFoundFlag && operandLinker; operandLinker = operandLinker->getNext()) {
										CConcept* opConcept = operandLinker->getData();
										bool opConNegation = operandLinker->isNegated() ^ negated;

										bool tmpClashFoundFlag = false;
										bool tmpClashFreeFoundFlag = false;
										bool tmpUnknownFound = false;
										checkCanHaveClashWithModel(indiNode,opConcept,opConNegation,tmpClashFoundFlag,tmpUnknownFound,tmpClashFreeFoundFlag,depth+1,testedIndividualsSet,indiNode,indiNodeVec);

										oneClashFreeFoundFlag |= tmpClashFreeFoundFlag;
										oneUnknownFound |= tmpUnknownFound;
										allClashFoundFlag &= tmpClashFoundFlag;
									}

									if (oneClashFreeFoundFlag) {
										clashFreeFlag = true;
									} else if (allClashFoundFlag) {
										clashFoundFlag = true;
									} else {
										unknownFlag = true;
									}
								} else if (opCount == 1 && (opCode == CCOR || opCode == CCEQ || opCode == CCAND)) {

									CSortedNegLinker<CConcept*>* operandLinker = concept->getOperandList();
									CConcept* opConcept = operandLinker->getData();
									bool opConNegation = operandLinker->isNegated() ^ negated;
									checkCanHaveClashWithModel(indiNode,opConcept,opConNegation,clashFoundFlag,unknownFlag,clashFreeFlag,depth+1,testedIndividualsSet,indiNode,indiNodeVec);

								} else if (negated && (opCode == CCSUB || conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_TRIG_TYPE))) {
									clashFreeFlag = true;
								} else if (opCode == CCAQCHOOCE) {

									bool allClashFoundFlag = true;
									bool oneClashFreeFoundFlag = false;
									bool oneUnknownFound = false;
									for (CSortedNegLinker<CConcept*>* operandLinker = concept->getOperandList(); !oneClashFreeFoundFlag && operandLinker; operandLinker = operandLinker->getNext()) {
										CConcept* opConcept = operandLinker->getData();
										bool opConNegation = operandLinker->isNegated();

										if (opConNegation == negated) {

											bool tmpClashFoundFlag = false;
											bool tmpClashFreeFoundFlag = false;
											bool tmpUnknownFound = false;
											checkCanHaveClashWithModel(indiNode,opConcept,opConNegation,tmpClashFoundFlag,tmpUnknownFound,tmpClashFreeFoundFlag,depth+1,testedIndividualsSet,indiNode,indiNodeVec);

											oneClashFreeFoundFlag |= tmpClashFreeFoundFlag;
											oneUnknownFound |= tmpUnknownFound;
											allClashFoundFlag &= tmpClashFoundFlag;
										}
									}

									if (oneClashFreeFoundFlag) {
										clashFreeFlag = true;
									} else if (allClashFoundFlag) {
										clashFoundFlag = true;
									} else {
										unknownFlag = true;
									}

								} else if (negated && (opCode == CCSOME) || !negated && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE))) {
									CRole* role = concept->getRole();

									CRoleSuccessorLinkIterator linkIt = indiNode->getRoleSuccessorLinkIterator(role);
									if (!linkIt.hasNext()) {
										clashFreeFlag = true;
									} else {
										bool oneClashFoundFlag = false;
										bool allClashFreeFoundFlag = true;
										bool oneUnknownFound = false;

										while (!oneClashFoundFlag && linkIt.hasNext()) {
											CIndividualLinkEdge* link = linkIt.next(true);
											cint64 succIndiID = link->getOppositeIndividualID(indiNode->getIndividualNodeID());
											CIndividualProcessNode* succIndiNode = indiNodeVec->getData(succIndiID);											

											for (CSortedNegLinker<CConcept*>* operandLinker = concept->getOperandList(); !oneClashFoundFlag && operandLinker; operandLinker = operandLinker->getNext()) {
												CConcept* opConcept = operandLinker->getData();
												bool opConNegation = operandLinker->isNegated() ^ negated;

												bool tmpClashFoundFlag = false;
												bool tmpClashFreeFoundFlag = false;
												bool tmpUnknownFound = false;
												checkCanHaveClashWithModel(succIndiNode,opConcept,opConNegation,tmpClashFoundFlag,tmpUnknownFound,tmpClashFreeFoundFlag,depth+1,testedIndividualsSet,indiNode,indiNodeVec);

												allClashFreeFoundFlag &= tmpClashFreeFoundFlag;
												oneUnknownFound |= tmpUnknownFound;
												oneClashFoundFlag |= tmpClashFoundFlag;
											}
										}

										if (oneClashFoundFlag) {
											clashFreeFlag = true;
										} else if (allClashFreeFoundFlag) {
											clashFoundFlag = true;
										} else {
											unknownFlag = true;
										}

									}

								} else {
									unknownFlag = true;
								}
							}
						}
					}
				}


				bool CSatisfiableTaskClassificationMessageAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					mStatCalcTask = statCalcTask;
					mReusePossSubsumMapIterator = nullptr;
					CSatisfiableTaskClassificationMessageAdapter* classMessAdapter = statCalcTask->getClassificationMessageAdapter();
					if (classMessAdapter) {
						mConRefLinkDataHash = classMessAdapter->getConceptReferenceLinkingDataHash();
						CClassificationMessageDataObserver* classMessObserver = classMessAdapter->getClassificationMessageDataObserver();
						CClassificationMessageData* subsumMessageDataLinker = nullptr;
						CClassificationMessageData* possSubsumMessageDataLinker = nullptr;
						CClassificationMessageData* pmMessageDataLinker = nullptr;

						bool extractSubsumersRootNode = classMessAdapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTSUBSUMERSROOTNODE);
						bool extractSubsumersOtherNodes = classMessAdapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTSUBSUMERSOTHERNODES);

						bool extractPossibleSubsumersRootNode = classMessAdapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTPOSSIBLESUBSUMERSROOTNODE);
						bool extractPossibleSubsumersOtherNodes = classMessAdapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTPOSSIBLESUBSUMERSOTHERNODES);

						bool extractPseudoModelRootNode = classMessAdapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTIDENTIFIERPSEUDOMODEL);


						bool extractOtherNodesSingleDependency = classMessAdapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTOTHERNODESSINGLEDEPENDENCY);
						bool extractOtherNodesMultipleDependency = classMessAdapter->hasExtractionFlags(CSatisfiableTaskClassificationMessageAdapter::EFEXTRACTOTHERNODESMULTIPLEDEPENDENCY);


						bool considerRootNode = extractSubsumersRootNode | extractPossibleSubsumersRootNode;
						bool considerOtherNode = extractSubsumersOtherNodes | extractPossibleSubsumersOtherNodes;

						mMultiplePossSubsumInitAvoidHash = nullptr;
						mLastMergingTestNode = nullptr;
						mTestSaturatedMergedHash = nullptr;

						CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
						CTaskHandleMemoryAllocationManager* taskMemMan = calcAlgContext->getTemporaryMemoryAllocationManager();
						CConcept* testingConcept = classMessAdapter->getTestingConcept();

						//QString iriClassNameString = CIRIName::getRecentIRIName(testingConcept->getClassNameLinker());
						//if (iriClassNameString == "http://www.owllink.org/testsuite/galen#LowerLobeOfLeftLung") {
						//	bool bug = true;
						//}
						//} 
						//considerOtherNode = false;

						CConcreteOntology* testOntology = classMessAdapter->getTestingOntology();
						CProcessingDataBox* processingDataBox = statCalcTask->getProcessingDataBox();
						CIndividualProcessNode* constructedIndiNode = processingDataBox->getConstructedIndividualNode();
						CIndividualProcessNodeVector* indiNodeVec = processingDataBox->getIndividualProcessNodeVector();

						cint64 maxDetBranchTag = processingDataBox->getMaximumDeterministicBranchTag();

						if (processingDataBox->getOntology()->getDataBoxes()->getMBox()->getValueSpacesTriggers(false)) {
							extractPseudoModelRootNode = false;
						}


						mUseAlwaysConRefLinkDataHash = false;

						if (testOntology && testingConcept && constructedIndiNode) {
							cint64 constructedID = constructedIndiNode->getIndividualNodeID();
							bool nondetMerged = false;
							CIndividualProcessNode* baseIndi = getCorrectedIndividualID(constructedIndiNode,indiNodeVec,&nondetMerged);
							cint64 baseIndiID = baseIndi->getIndividualNodeID();
							if (nondetMerged) {
								maxDetBranchTag = 0;
							}

							CPROCESSINGSET<CConcept*>* analysedConceptSet = nullptr;


							CMemoryPoolContainer memPoolCon;
							CTaskMemoryPoolAllocationManager classMessMemManCreaterMemMan(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
							mTempMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&classMessMemManCreaterMemMan);
							mTempMemAllocMan->initTaskMemoryPoolAllocationManager(&memPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
							mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTempMemAllocMan);

							CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList = nullptr;
							
							if (considerRootNode) {
								CReapplyConceptLabelSet* conSet = baseIndi->getReapplyConceptLabelSet(false);
								if (conSet) {
									CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true,true,true);
									while (conSetIt.hasNext()) {
										CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
										if (conDes) {
											CConcept* concept = conDes->getConcept();
											bool negated = conDes->getNegation();

											if (concept->hasClassName()) {
												CDependencyTrackPoint* depTrackPoint = conSetIt.getDependencyTrackPoint();
												bool deterministic = false;
												if (depTrackPoint) {
													if (depTrackPoint->getBranchingTag() <= maxDetBranchTag) {
														deterministic = true;
													}
												}

												if (extractSubsumersRootNode && deterministic && !negated && concept != testingConcept && concept->getConceptTag() != 1) {
													if (!subsumerList) {
														subsumerList = CObjectParameterizingAllocator< CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>,CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTmpContext);
													}
													subsumerList->append(concept);
												}

												if (extractPossibleSubsumersRootNode && !negated) {
													CClassificationMessageData* messageData = extractPossibleSubsumptionInformation(conDes,baseIndi,conSet,calcAlgContext);
													if (messageData) {
														possSubsumMessageDataLinker = messageData->append(possSubsumMessageDataLinker);
													}
													if (conDes->getConcept() == testingConcept) {
														//verifySubsumptionPossibleWithModel(CIndividualProcessNode* indiNode, );
													}
												}
											}
										}
										conSetIt.moveNext();
									}
								}

								CClassificationClassSubsumptionMessageData* subsumMessageData = CObjectAllocator<CClassificationClassSubsumptionMessageData>::allocateAndConstruct(mTempMemAllocMan);
								subsumMessageData->initClassificationSubsumptionMessageData(testingConcept,subsumerList);
								subsumMessageDataLinker = subsumMessageData->append(subsumMessageDataLinker);
								subsumerList = nullptr;
								STATINC(ANALYSESUBSUMERCOUNT,calcAlgContext);
							}

							if (considerOtherNode) {

								cint64 consideredOtherNodeCount = 0;
								cint64 extractedPossibleSubsumerOtherNodeCount = 0;

								CSuccessorIterator succIt(baseIndi->getSuccessorIterator());
								if (succIt.hasNext()) {

									CPROCESSINGHASH<cint64,CIndividualAnalyseProcessItem*> succIndiProcHash(taskProcessorContext);
									CPROCESSINGLIST<CIndividualAnalyseProcessItem*> succIndiProcList(taskProcessorContext);

									CIndividualAnalyseProcessItem* baseAncItem = CObjectAllocator<CIndividualAnalyseProcessItem>::allocateAndConstruct(taskMemMan);
									baseAncItem->initIndividualAnalyseProcessItem(baseIndi->getIndividualNodeID(),0,nullptr,nullptr);

									succIndiProcHash.insert(0,baseAncItem);
									while (succIt.hasNext()) {
										CIndividualLinkEdge* succLink = succIt.nextLink();
										cint64 succIndiID = succLink->getOppositeIndividualID(baseIndiID);
										if (!succIndiProcHash.contains(succIndiID)) {
											CIndividualAnalyseProcessItem* indiAnProcItem = CObjectAllocator<CIndividualAnalyseProcessItem>::allocateAndConstruct(taskMemMan);
											indiAnProcItem->initIndividualAnalyseProcessItem(succIndiID,1,baseAncItem,succLink);
											succIndiProcHash.insert(succIndiID,indiAnProcItem);
											succIndiProcList.append(indiAnProcItem);
										}
									}


									while (!succIndiProcList.isEmpty()) {
										++consideredOtherNodeCount;

										CIndividualAnalyseProcessItem* indiAnProcItem = succIndiProcList.takeFirst();
										cint64 indiID = indiAnProcItem->mIndiID;
										CIndividualProcessNode* indiNode = indiNodeVec->getData(indiID);
										if (!indiNode->isNominalIndividualNode() && !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION)) {
											CConceptDescriptor* analyseConDes = nullptr;
											CConceptDescriptor* conDesSingleDep = nullptr;

											CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
											CConceptDescriptor* conDesMultipleIt = conSet->getAddingSortedConceptDescriptionLinker();

											if (extractOtherNodesMultipleDependency) {
												analyseConDes = conDesMultipleIt;
												if (conDesMultipleIt) {
													conDesMultipleIt = conDesMultipleIt->getNext();
												}
											} 
											if (conDesMultipleIt || extractOtherNodesSingleDependency) {
												if (!indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
													conDesSingleDep = getIndividualProcessNodeConceptWithSingleAncestorDependency(indiNode,indiAnProcItem,calcAlgContext);
												}
											}
											if (!conDesSingleDep) {
												analyseConDes = conDesMultipleIt;
											}
											while (analyseConDes) {
												if (analyseConDes && analyseConDes->getConcept() != testingConcept && analyseConDes->getConceptTag() != 1 && !analyseConDes->isNegated() && analyseConDes->getConcept()->hasClassName()) {
													CConcept* analyseConcept = analyseConDes->getConcept();

													// test whether the concept has already been analyzed
													bool analysationRequired = false;
													CConceptReferenceLinking* conRefLinking = nullptr;
													CConceptData* conData = analyseConcept->getConceptData();
													if (conData && !mUseAlwaysConRefLinkDataHash) {
														CConceptProcessData* conProcData = (CConceptProcessData*)conData;
														if (!conProcData->isInvalidatedReferenceLinking()) {
															conRefLinking = conProcData->getConceptReferenceLinking();
														}
													}
													CClassificationConceptReferenceLinking* classConRefLinkData = nullptr;
													if (!conRefLinking) {
														classConRefLinkData = mConRefLinkDataHash->value(analyseConcept);
													} else {
														CConceptSatisfiableReferenceLinkingData* conSatRefLinkData = (CConceptSatisfiableReferenceLinkingData*)conRefLinking;
														classConRefLinkData = conSatRefLinkData->getClassifierReferenceLinkingData();
													}
													if (classConRefLinkData) {
														CClassificationSatisfiableCalculationConceptReferenceLinking* classSatCalcConRefLinkData = (CClassificationSatisfiableCalculationConceptReferenceLinking*)classConRefLinkData;
														analysationRequired = classSatCalcConRefLinkData->isMoreConceptClassificationInformationRequired();
													}


													CDependencyTrackPoint* analyseDepTrackPoint = analyseConDes->getDependencyTrackPoint();
													if (analysationRequired && analyseDepTrackPoint) {
														cint64 analyseBranchTag = analyseDepTrackPoint->getBranchingTag();
														bool branchingError = false;

														if (!analysedConceptSet) {
															analysedConceptSet = CObjectParameterizingAllocator< CPROCESSINGSET<CConcept*>,CContext* >::allocateAndConstructAndParameterize(taskMemMan,taskProcessorContext);
														}
														if (!analysedConceptSet->contains(analyseConcept)) {
															analysedConceptSet->insert(analyseConcept);

															if (extractSubsumersOtherNodes && analyseConDes == conDesSingleDep) {

																if (conSet) {
																	CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true,true,false);
																	while (conSetIt.hasNext() && !branchingError) {
																		CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
																		if (conDes) {
																			CConcept* concept = conDes->getConcept();
																			bool negated = conDes->getNegation();

																			if (concept->hasClassName()) {
																				CDependencyTrackPoint* depTrackPoint = conSetIt.getDependencyTrackPoint();
																				bool deterministic = false;
																				if (depTrackPoint) {
																					if (depTrackPoint->getBranchingTag() == analyseBranchTag) {
																						deterministic = true;
																					} else if (depTrackPoint->getBranchingTag() < analyseBranchTag) {
																						branchingError = true;
																					}
																				}

																				if (deterministic && !negated && concept != analyseConcept && concept->getConceptTag() != 1) {
																					if (!subsumerList) {
																						subsumerList = CObjectParameterizingAllocator< CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>,CContext* >::allocateAndConstructAndParameterize(mTempMemAllocMan,mTmpContext);
																					}
																					subsumerList->append(concept);
																				}
																			}
																		}
																		conSetIt.moveNext();
																	}
																}
															}

															if (!branchingError && subsumerList) {
																CClassificationClassSubsumptionMessageData* subsumMessageData = CObjectAllocator<CClassificationClassSubsumptionMessageData>::allocateAndConstruct(mTempMemAllocMan);
																subsumMessageData->initClassificationSubsumptionMessageData(analyseConcept,subsumerList);
																subsumMessageDataLinker = subsumMessageData->append(subsumMessageDataLinker);
																subsumerList = nullptr;
																STATINC(ANALYSESUBSUMERCOUNT,calcAlgContext);
															}

															if (extractPossibleSubsumersOtherNodes) {
																CClassificationMessageData* messageData = extractPossibleSubsumptionInformation(analyseConDes,indiNode,conSet,calcAlgContext);
																if (messageData) {
																	++extractedPossibleSubsumerOtherNodeCount;
																	possSubsumMessageDataLinker = messageData->append(possSubsumMessageDataLinker);
																}
															}

														}
													}
												}

												if (extractOtherNodesMultipleDependency) {
													if (analyseConDes == conDesMultipleIt) {
														conDesMultipleIt = conDesMultipleIt->getNext();
													}
													analyseConDes = conDesMultipleIt;
													if (conDesMultipleIt) {
														conDesMultipleIt = conDesMultipleIt->getNext();
													}
												} else {
													analyseConDes = nullptr;
												}
											}


											// next successor nodes
											CSuccessorIterator nextSuccIt(indiNode->getSuccessorIterator());
											while (nextSuccIt.hasNext()) {
												CIndividualLinkEdge* nextSuccLink = nextSuccIt.nextLink();
												cint64 nextSuccIndiID = nextSuccLink->getOppositeIndividualID(indiID);
												if (!succIndiProcHash.contains(nextSuccIndiID)) {
													CIndividualAnalyseProcessItem* nextIndiAnProcItem = CObjectAllocator<CIndividualAnalyseProcessItem>::allocateAndConstruct(taskMemMan);
													nextIndiAnProcItem->initIndividualAnalyseProcessItem(nextSuccIndiID,indiAnProcItem->mRootDistance+1,indiAnProcItem,nextSuccLink);
													succIndiProcHash.insert(nextSuccIndiID,nextIndiAnProcItem);
													succIndiProcList.append(nextIndiAnProcItem);
												}
											}

										}
									}
								}

								mStatConsideredOtherNodeCount += consideredOtherNodeCount;
								mStatExtractedPossibleSubsumerOtherNodeCount += extractedPossibleSubsumerOtherNodeCount;
							}







							if (extractPseudoModelRootNode) {

								// create pseudo model

								mMaxPseudoModelDepth = 3;
								mMaxPseudoModelNodes = 30;

								cint64 currentPMModelNodesCount = 0;

								CConceptReferenceLinking* conRefLinking = nullptr;
								CConceptData* conData = testingConcept->getConceptData();
								if (conData && !mUseAlwaysConRefLinkDataHash) {
									CConceptProcessData* conProcData = (CConceptProcessData*)conData;
									if (!conProcData->isInvalidatedReferenceLinking()) {
										conRefLinking = conProcData->getConceptReferenceLinking();
									}
								}
								CClassificationConceptReferenceLinking* classConRefLinkData = nullptr;
								if (!conRefLinking) {
									classConRefLinkData = mConRefLinkDataHash->value(testingConcept);
								} else {
									CConceptSatisfiableReferenceLinkingData* conSatRefLinkData = (CConceptSatisfiableReferenceLinkingData*)conRefLinking;
									classConRefLinkData = conSatRefLinkData->getClassifierReferenceLinkingData();
								}
								if (classConRefLinkData) {
									CClassificationSatisfiablePossibleSubsumptionCalculationConceptReferenceLinking* possSubsumRefLinkData = (CClassificationSatisfiablePossibleSubsumptionCalculationConceptReferenceLinking*)classConRefLinkData;
									if (possSubsumRefLinkData) {

										CMemoryPoolContainer pmMemPoolCon;
										CTaskMemoryPoolAllocationManager classMessMemManCreaterMemMan(&pmMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
										mPMTempMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&classMessMemManCreaterMemMan);
										mPMTempMemAllocMan->initTaskMemoryPoolAllocationManager(&pmMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
										mPMTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mPMTempMemAllocMan,mPMTempMemAllocMan);


										CClassificationClassPseudoModel* pmModel = possSubsumRefLinkData->getClassPseudoModel();
										CClassificationClassPseudoModelHash* pmModelHash = CObjectParameterizingAllocator< CClassificationClassPseudoModelHash,CContext* >::allocateAndConstructAndParameterize(mPMTempMemAllocMan,mPMTmpContext);
										cint64 nextModelID = 1;

										CPseudoModelAnalyseProcessItem* basePMItem = CObjectAllocator<CPseudoModelAnalyseProcessItem>::allocateAndConstruct(taskMemMan);
										basePMItem->initPseudoModelAnalyseProcessItem(0,0);
										CXNegLinker<CIndividualProcessNode*>* pmBaseNodeLinker = CObjectAllocator< CXNegLinker<CIndividualProcessNode*> >::allocateAndConstruct(taskMemMan);
										pmBaseNodeLinker->initNegLinker(baseIndi,false);
										basePMItem->addNodeLinker(pmBaseNodeLinker);

										CXLinker<CPseudoModelAnalyseProcessItem*>* processItemLinker = CObjectAllocator< CXLinker<CPseudoModelAnalyseProcessItem*> >::allocateAndConstruct(taskMemMan);
										processItemLinker->initLinker(basePMItem);


										while (processItemLinker) {
											++currentPMModelNodesCount;
											CXLinker<CPseudoModelAnalyseProcessItem*>* tmpItemLinker = processItemLinker;
											processItemLinker = processItemLinker->getNext();


											CPseudoModelAnalyseProcessItem* pmAnalyseProcessItem = tmpItemLinker->getData();
											cint64 pmModelID = pmAnalyseProcessItem->mPMModelID;
											CClassificationClassPseudoModelData* pmModel = pmModelHash->getPseudoModelData(pmModelID,true);


											bool vaidConcepts = true;
											bool vaidSuccessors = true;


											for (CXNegLinker<CIndividualProcessNode*>* nodeIt = pmAnalyseProcessItem->mNodeLinker; nodeIt; nodeIt = nodeIt->getNext()) {
												CIndividualProcessNode* node = nodeIt->getData();

												bool blockedOrCached = node->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINVALIDATEBLOCKERFLAGSCOMPINATION | CIndividualProcessNode::PRFSATURATIONBLOCKINGCACHED | CIndividualProcessNode::PRFSYNCHRONIZEDBACKENDSUCCESSOREXPANSIONBLOCKED) || node->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED) && !node->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID |CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED);
												if (blockedOrCached) {
													vaidConcepts = false;
													vaidSuccessors = false;
												}
												bool nominalNode = node->isNominalIndividualNode();
												if (nominalNode) {
													vaidSuccessors = false;
												}
											}
											if (pmAnalyseProcessItem->mRootDistance > mMaxPseudoModelDepth || currentPMModelNodesCount > mMaxPseudoModelNodes) {
												vaidSuccessors = false;
											}

											pmModel->setValidConceptMap(vaidConcepts);
											pmModel->setValidRoleMap(vaidSuccessors);

											if (vaidConcepts) {
												// update pseudo model concept map
												CClassificationClassPseudoModelConceptMap* pmModelConMap = pmModel->getPseudoModelConceptMap(true);
												for (CXNegLinker<CIndividualProcessNode*>* nodeIt = pmAnalyseProcessItem->mNodeLinker; nodeIt; nodeIt = nodeIt->getNext()) {
													bool nonDeterministicConnected = nodeIt->isNegated();
													CIndividualProcessNode* node = nodeIt->getData();
													CReapplyConceptLabelSet* conSet = node->getReapplyConceptLabelSet(false);
													if (conSet) {
														CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true,true,false);
														while (conSetIt.hasNext()) {
															CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
															if (conDes) {
																CDependencyTrackPoint* depTrackPoint = conSetIt.getDependencyTrackPoint();
																bool deterministic = false;
																if (depTrackPoint) {
																	if (depTrackPoint->getBranchingTag() <= maxDetBranchTag) {
																		if (!nonDeterministicConnected) {
																			deterministic = true;
																		}
																	}
																}
																CConcept* concept = conDes->getConcept();
																cint64 conOpCode = concept->getOperatorCode();
																bool insertCon = false;
																if (!conDes->isNegated()) {
																	if ((concept->hasClassName() && conOpCode == CCATOM) || conOpCode == CCSUB || conOpCode == CCIMPLTRIG || conOpCode == CCEQCAND) {
																		insertCon = true;
																	}	
																}

																if (insertCon) {
																	pmModelConMap->insert(CConceptTagComparer(concept),CClassificationClassPseudoModelConceptData(deterministic));
																}
															}
															conSetIt.moveNext();
														}
													}
												}
											}

											if (vaidSuccessors) {
												// update pseudo model role map
												CPROCESSINGSET<CRole*> processedRoles(taskProcessorContext);
												CClassificationClassPseudoModelRoleMap* pmModelRoleMap = pmModel->getPseudoModelRoleMap(true);
												for (CXNegLinker<CIndividualProcessNode*>* nodeIt = pmAnalyseProcessItem->mNodeLinker; nodeIt; nodeIt = nodeIt->getNext()) {
													bool nonDeterministicConnected = nodeIt->isNegated();
													CIndividualProcessNode* node = nodeIt->getData();
													CRoleSuccessorIterator roleIt = node->getRoleIterator();
													while (roleIt.hasNext()) {
														CRole* role = roleIt.next();
														if (!role->isComplexRole()) {
															// do not analyze complex roles
															if (!processedRoles.contains(role)) {
																processedRoles.insert(role);


																CPROCESSINGSET<cint64> roleSuccessorIDSet(taskProcessorContext);
																// collect all successors for this role over all nodes

																cint64 lowerDetAtLeastBound = 0;
																cint64 upperAtLeastBound = 0;

																cint64 upperDetAtMostBound = CINT64_MAX;
																cint64 lowerAtMostBound = CINT64_MAX;

																CXNegLinker<CIndividualProcessNode*>* succNodeLinker = nullptr;

																bool hasDeterministicSuccessor = false;


																for (CXNegLinker<CIndividualProcessNode*>* nodeIt2 = pmAnalyseProcessItem->mNodeLinker; nodeIt2; nodeIt2 = nodeIt2->getNext()) {
																	CIndividualProcessNode* node2 = nodeIt2->getData();

																	CReapplyRoleSuccessorHash* roleSuccHash = node2->getReapplyRoleSuccessorHash(false);
																	if (roleSuccHash) {
																		cint64 linkCount = 0;
																		CRoleSuccessorLinkIterator roleSuccIt = roleSuccHash->getRoleSuccessorLinkIterator(role,&linkCount);
																		upperAtLeastBound = qMax(upperAtLeastBound,linkCount);
																		lowerAtMostBound = qMin(lowerAtMostBound,linkCount);
																		while (roleSuccIt.hasNext()) {
																			CIndividualLinkEdge* succLink = roleSuccIt.next();
																			cint64 succIndiID = succLink->getOppositeIndividualID(node2);
																			if (!roleSuccessorIDSet.contains(succIndiID)) {
																				roleSuccessorIDSet.insert(succIndiID);

																				bool deterministicLink = !nonDeterministicConnected;
																				CDependencyTrackPoint* depTrackPoint = succLink->getDependencyTrackPoint();
																				if (depTrackPoint && depTrackPoint->getBranchingTag() <= maxDetBranchTag) {
																					lowerDetAtLeastBound = qMax(lowerDetAtLeastBound,(cint64)1);
																					if (!nonDeterministicConnected) {
																						hasDeterministicSuccessor = true;
																					}
																					deterministicLink = false;
																				}

																				CIndividualProcessNode* succIndiNode = indiNodeVec->getData(succIndiID);
																				CXNegLinker<CIndividualProcessNode*>* nextSuccNodeLinker = CObjectAllocator< CXNegLinker<CIndividualProcessNode*> >::allocateAndConstruct(taskMemMan);
																				nextSuccNodeLinker->initNegLinker(succIndiNode,deterministicLink);
																				succNodeLinker = nextSuccNodeLinker->append(succNodeLinker);

																			}
																		}
																	} else {
																		lowerAtMostBound = 0;
																	}


																	CReapplyConceptLabelSet* conSet = node2->getReapplyConceptLabelSet(false);
																	if (conSet) {
																		for (CConceptDescriptor* conDesLinkerIt = conSet->getAddingSortedConceptDescriptionLinker(); conDesLinkerIt; conDesLinkerIt = conDesLinkerIt->getNextConceptDesciptor()) {
																			CConcept* concept = conDesLinkerIt->getConcept();
																			if (concept->getRole() == role) {
																				bool conNegation = conDesLinkerIt->getNegation();
																				cint64 opCode = concept->getOperatorCode();
																				CDependencyTrackPoint* depTrackPoint = conDesLinkerIt->getDependencyTrackPoint();
																				if (depTrackPoint && depTrackPoint->getBranchingTag() <= maxDetBranchTag) {
																					if (!conNegation && opCode == CCATLEAST || conNegation && opCode == CCATMOST) {
																						cint64 atLeastParam = concept->getParameter() + 1 * conNegation;
																						if (atLeastParam >= 0) {
																							lowerDetAtLeastBound = qMax(lowerDetAtLeastBound,atLeastParam);
																						}
																					} else if (conNegation && opCode == CCATLEAST || !conNegation && opCode == CCATMOST && concept->getOperandList() == nullptr) {
																						cint64 atMostParam = concept->getParameter() - 1 * conNegation;
																						if (atMostParam >= 0) {
																							upperDetAtMostBound = qMin(upperDetAtMostBound,atMostParam);
																						}
																					}
																				}
																			}
																		}
																	}
																}


																cint64 succPMModelID = 0;


																if (succPMModelID == 0) {
																	succPMModelID = nextModelID++;
																}

																CClassificationClassPseudoModelRoleData& roleSuccModelData = (*pmModelRoleMap)[CRoleTagComparer(role)];
																roleSuccModelData.setSuccessorModelID(succPMModelID);
																roleSuccModelData.setDeterministic(hasDeterministicSuccessor);
																roleSuccModelData.setLowerAtLeastBound(lowerDetAtLeastBound);
																roleSuccModelData.setUpperAtLeastBound(upperAtLeastBound);
																roleSuccModelData.setUpperAtMostBound(upperDetAtMostBound);
																roleSuccModelData.setLowerAtMostBound(lowerAtMostBound);
																
																CPseudoModelAnalyseProcessItem* roleSuccessorsPMItem = CObjectAllocator<CPseudoModelAnalyseProcessItem>::allocateAndConstruct(taskMemMan);
																roleSuccessorsPMItem->initPseudoModelAnalyseProcessItem(succPMModelID,pmAnalyseProcessItem->mRootDistance+1);
																roleSuccessorsPMItem->addNodeLinker(succNodeLinker);

																CXLinker<CPseudoModelAnalyseProcessItem*>* nextProcessItemLinker = CObjectAllocator< CXLinker<CPseudoModelAnalyseProcessItem*> >::allocateAndConstruct(taskMemMan);
																nextProcessItemLinker->initLinker(roleSuccessorsPMItem);
																processItemLinker = nextProcessItemLinker->append(processItemLinker);


															}
														}
													}
												}
											}

										}

										STATINC(ANALYSEPSEUDOMODELCOUNT,calcAlgContext);

										CClassificationPseudoModelIdentifierMessageData* pmMessageData = CObjectAllocator<CClassificationPseudoModelIdentifierMessageData>::allocateAndConstruct(mTempMemAllocMan);
										pmMessageData->initClassificationPseudoModelIdentifierMessageData(testingConcept,pmModelHash,pmMemPoolCon.takeMemoryPools());
										pmMessageDataLinker = pmMessageData->append(pmMessageDataLinker);


									}
								}
							}





							CClassificationMessageData* messageDataLinker = subsumMessageDataLinker;
							if (pmMessageDataLinker) {
								messageDataLinker = pmMessageDataLinker->append(messageDataLinker);
							}
							if (possSubsumMessageDataLinker) {
								messageDataLinker = possSubsumMessageDataLinker->append(messageDataLinker);
							}

							if (messageDataLinker) {
								classMessObserver->tellClassificationMessage(testOntology,messageDataLinker,memPoolCon.takeMemoryPools());
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
