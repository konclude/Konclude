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

#include "CPrecomputedSaturationSubsumerExtractor.h"

namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CPrecomputedSaturationSubsumerExtractor::CPrecomputedSaturationSubsumerExtractor(CConcreteOntology* ontology) {
				mSatCalcTask = nullptr;
				mOntology = ontology;
				CSaturationData* saturationData = mOntology->getPrecomputation()->getSaturationModelData();
				CSaturationTaskData* saturationTaskData = (CSaturationTaskData*)saturationData;
				if (saturationTaskData) {
					mSatCalcTask = saturationTaskData->getSaturationTask();
				}
			}

			bool CPrecomputedSaturationSubsumerExtractor::extractSubsumers(CConcept* concept, CConceptSubsumerObserver* subsumerObserver, bool* possibleSubsumerFlag, CIndividualDependenceTrackingObserver* indDepTrackingObserver, CIndividualDependenceTrackingMarker* indiDepTrackMarker) {
				if (mSatCalcTask) {
					CProcessingDataBox* procDataBox = mSatCalcTask->getProcessingDataBox();
					CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
					if (conProData) {
						CConceptSaturationReferenceLinkingData* conSatRefLinking = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
						if (conSatRefLinking) {
							CSaturationConceptReferenceLinking* saturationConceptRefLinkingData = (CSaturationConceptReferenceLinking*)conSatRefLinking->getPositiveConceptSaturationReferenceLinkingData();
							if (saturationConceptRefLinkingData) {
								CIndividualSaturationProcessNode* baseIndiNode = (CIndividualSaturationProcessNode*)saturationConceptRefLinkingData->getIndividualProcessNodeForConcept();
								if (baseIndiNode) {
									CIndividualSaturationProcessNode* indiNode = baseIndiNode;
									while (indiNode->hasSubstituteIndividualNode()) {
										if (indiNode != baseIndiNode) {
											CSaturationConceptDataItem* subsumerSatConDataItem = (CSaturationConceptDataItem*)indiNode->getSaturationConceptReferenceLinking();
											if (!subsumerSatConDataItem->getSaturationNegation() && !subsumerSatConDataItem->getSaturationRoleRanges()) {
												CConcept* subsumerConcept = subsumerSatConDataItem->getSaturationConcept();
												if (subsumerConcept != concept && subsumerConcept->hasClassName()) {
													subsumerObserver->tellConceptSupsumption(concept,subsumerConcept);
												}
											}
										}
										indiNode = indiNode->getSubstituteIndividualNode();
									}
									CReapplyConceptSaturationLabelSet* conSet = indiNode->getReapplyConceptSaturationLabelSet(false);
									if (conSet) {
										for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
											if (!conSatDesIt->isNegated()) {
												CConcept* subsumerConcept = conSatDesIt->getConcept();
												if (subsumerConcept != concept && subsumerConcept->hasClassName()) {
													subsumerObserver->tellConceptSupsumption(concept,subsumerConcept);
												} else {
													if (subsumerConcept->getOperatorCode() == CCEQCAND) {
														CIndividualSaturationProcessNodeStatusFlags* directFlags = indiNode->getDirectStatusFlags();
														if (directFlags->hasEQCandidateProplematicFlag()) {
															if (possibleSubsumerFlag) {
																*possibleSubsumerFlag = true;
															}
														}
													}
												}
											}
										}
									}
									if (indiDepTrackMarker) {
										CSuccessorConnectedNominalSet* succConnNomSet = indiNode->getSuccessorConnectedNominalSet(false);
										if (succConnNomSet) {
											indiDepTrackMarker->setIndividualDependenceTracked();
										}
									}
									if (indDepTrackingObserver) {
										CSuccessorConnectedNominalSet* succConnNomSet = indiNode->getSuccessorConnectedNominalSet(false);
										if (succConnNomSet) {
											CIndividualDependenceTracking* indiDepTracking = indDepTrackingObserver->getExtendingIndividualDependenceTracking();
											CReferredIndividualTrackingVector* refIndiDepTrackVec = nullptr;
											if (indiDepTracking) {
												refIndiDepTrackVec = dynamic_cast<CReferredIndividualTrackingVector*>(indiDepTracking);
											}
											if (!refIndiDepTrackVec) {
												cint64 aboxIndiCount = mOntology->getABox()->getIndividualCount();
												cint64 trackIndiCount = aboxIndiCount;
												CConsistenceData* consData = mOntology->getConsistence()->getConsistenceModelData();
												if (consData) {
													CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
													if (consTaskData) {
														CSatisfiableCalculationTask* cachedSatTask = consTaskData->getDeterministicSatisfiableTask();
														if (cachedSatTask) {
															trackIndiCount = qMax(trackIndiCount,cachedSatTask->getProcessingDataBox()->getIndividualProcessNodeVector()->getItemCount());
														}
													}
												}
												refIndiDepTrackVec = new CReferredIndividualTrackingVector();
												refIndiDepTrackVec->initReferredIndividualTrackingVector(trackIndiCount,aboxIndiCount);
												refIndiDepTrackVec = dynamic_cast<CReferredIndividualTrackingVector*>(indDepTrackingObserver->installIndividualDependenceTracking(refIndiDepTrackVec));
											}
											if (refIndiDepTrackVec) {
												for (CSuccessorConnectedNominalSet::const_iterator it = succConnNomSet->constBegin(), itEnd = succConnNomSet->constEnd(); it != itEnd; ++it) {
													cint64 nominalIndiID(*it);
													refIndiDepTrackVec->setIndividualReferred(nominalIndiID);
												}
											}
										}
									}
									return true;
								}
							}
						}
					}
				}
				return false;
			}



			cint64 CPrecomputedSaturationSubsumerExtractor::getSubsumerCount(CConcept* concept) {
				cint64 subsumerCount = 0;
				if (mSatCalcTask) {
					CProcessingDataBox* procDataBox = mSatCalcTask->getProcessingDataBox();
					CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
					if (conProData) {
						CConceptSaturationReferenceLinkingData* conSatRefLinking = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
						if (conSatRefLinking) {
							CSaturationConceptReferenceLinking* saturationConceptRefLinkingData = (CSaturationConceptReferenceLinking*)conSatRefLinking->getPositiveConceptSaturationReferenceLinkingData();
							if (saturationConceptRefLinkingData) {
								CIndividualSaturationProcessNode* baseIndiNode = (CIndividualSaturationProcessNode*)saturationConceptRefLinkingData->getIndividualProcessNodeForConcept();
								if (baseIndiNode) {
									CIndividualSaturationProcessNode* indiNode = baseIndiNode;
									while (indiNode->hasSubstituteIndividualNode()) {
										if (indiNode != baseIndiNode) {
											CSaturationConceptDataItem* subsumerSatConDataItem = (CSaturationConceptDataItem*)indiNode->getSaturationConceptReferenceLinking();
											if (!subsumerSatConDataItem->getSaturationNegation() && !subsumerSatConDataItem->getSaturationRoleRanges()) {
												CConcept* subsumerConcept = subsumerSatConDataItem->getSaturationConcept();
												if (subsumerConcept != concept && subsumerConcept->hasClassName()) {
													++subsumerCount;
												}
											}
										}
										indiNode = indiNode->getSubstituteIndividualNode();
									}
									CReapplyConceptSaturationLabelSet* conSet = indiNode->getReapplyConceptSaturationLabelSet(false);
									if (conSet) {
										for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
											if (!conSatDesIt->isNegated()) {
												CConcept* subsumerConcept = conSatDesIt->getConcept();
												if (subsumerConcept != concept && subsumerConcept->hasClassName()) {
													++subsumerCount;
												}
											}
										}
									}
								}
							}
						}
					}
				}
				return subsumerCount;
			}



			bool CPrecomputedSaturationSubsumerExtractor::hasSubsumerConcept(CConcept* concept, CConcept* testSubsumerConcept) {
				if (mSatCalcTask) {
					CProcessingDataBox* procDataBox = mSatCalcTask->getProcessingDataBox();
					CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
					if (conProData) {
						CConceptSaturationReferenceLinkingData* conSatRefLinking = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
						if (conSatRefLinking) {
							CSaturationConceptReferenceLinking* saturationConceptRefLinkingData = (CSaturationConceptReferenceLinking*)conSatRefLinking->getPositiveConceptSaturationReferenceLinkingData();
							if (saturationConceptRefLinkingData) {
								CIndividualSaturationProcessNode* baseIndiNode = (CIndividualSaturationProcessNode*)saturationConceptRefLinkingData->getIndividualProcessNodeForConcept();
								if (baseIndiNode) {
									CIndividualSaturationProcessNode* indiNode = baseIndiNode;
									while (indiNode->hasSubstituteIndividualNode()) {
										if (indiNode != baseIndiNode) {
											CSaturationConceptDataItem* subsumerSatConDataItem = (CSaturationConceptDataItem*)indiNode->getSaturationConceptReferenceLinking();
											if (!subsumerSatConDataItem->getSaturationNegation() && !subsumerSatConDataItem->getSaturationRoleRanges()) {
												CConcept* subsumerConcept = subsumerSatConDataItem->getSaturationConcept();
												if (subsumerConcept == testSubsumerConcept) {
													return true;
												}
											}
										}
										indiNode = indiNode->getSubstituteIndividualNode();
									}
									CReapplyConceptSaturationLabelSet* conSet = indiNode->getReapplyConceptSaturationLabelSet(false);
									if (conSet) {
										if (conSet->containsConcept(testSubsumerConcept,false)) {
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



			CConcept* CPrecomputedSaturationSubsumerExtractor::getDirectSubsumerCount(CConcept* concept, bool (*isDirectSubsumer)(CConcept*, CConcept*)) {
				if (mSatCalcTask) {
					CProcessingDataBox* procDataBox = mSatCalcTask->getProcessingDataBox();
					CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
					if (conProData) {
						CConceptSaturationReferenceLinkingData* conSatRefLinking = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
						if (conSatRefLinking) {
							CSaturationConceptReferenceLinking* saturationConceptRefLinkingData = (CSaturationConceptReferenceLinking*)conSatRefLinking->getPositiveConceptSaturationReferenceLinkingData();
							if (saturationConceptRefLinkingData) {
								CIndividualSaturationProcessNode* baseIndiNode = (CIndividualSaturationProcessNode*)saturationConceptRefLinkingData->getIndividualProcessNodeForConcept();
								if (baseIndiNode) {
									CIndividualSaturationProcessNode* indiNode = baseIndiNode;
									while (indiNode->hasSubstituteIndividualNode()) {
										if (indiNode != baseIndiNode) {
											CSaturationConceptDataItem* subsumerSatConDataItem = (CSaturationConceptDataItem*)indiNode->getSaturationConceptReferenceLinking();
											if (!subsumerSatConDataItem->getSaturationNegation() && !subsumerSatConDataItem->getSaturationRoleRanges()) {
												CConcept* subsumerConcept = subsumerSatConDataItem->getSaturationConcept();
												if (subsumerConcept != concept && subsumerConcept->hasClassName()) {
													if (isDirectSubsumer(concept,subsumerConcept)) {
														return subsumerConcept;
													}
												}
											}
										}
										indiNode = indiNode->getSubstituteIndividualNode();
									}
									CReapplyConceptSaturationLabelSet* conSet = indiNode->getReapplyConceptSaturationLabelSet(false);
									if (conSet) {
										for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
											if (!conSatDesIt->isNegated()) {
												CConcept* subsumerConcept = conSatDesIt->getConcept();
												if (subsumerConcept != concept && subsumerConcept->hasClassName()) {
													if (isDirectSubsumer(concept,subsumerConcept)) {
														return subsumerConcept;
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
				return nullptr;
			}


			bool CPrecomputedSaturationSubsumerExtractor::getConceptFlags(CConcept* concept, bool* unsatisfiableFlag, bool* insufficientFlag, bool* incompleteProcessedFlag) {
				if (mSatCalcTask) {
					CProcessingDataBox* procDataBox = mSatCalcTask->getProcessingDataBox();
					CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
					if (conProData) {
						CConceptSaturationReferenceLinkingData* conSatRefLinking = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
						if (conSatRefLinking) {
							CSaturationConceptReferenceLinking* saturationConceptRefLinkingData = (CSaturationConceptReferenceLinking*)conSatRefLinking->getPositiveConceptSaturationReferenceLinkingData();
							if (saturationConceptRefLinkingData) {
								CIndividualSaturationProcessNode* baseIndiNode = (CIndividualSaturationProcessNode*)saturationConceptRefLinkingData->getIndividualProcessNodeForConcept();
								if (baseIndiNode) {
									CIndividualSaturationProcessNode* indiNode = baseIndiNode;
									while (indiNode->hasSubstituteIndividualNode()) {
										indiNode = indiNode->getSubstituteIndividualNode();
									}
									CIndividualSaturationProcessNodeStatusFlags* baseFlags = baseIndiNode->getIndirectStatusFlags();
									CIndividualSaturationProcessNodeStatusFlags* flags = indiNode->getIndirectStatusFlags();
									if ((baseFlags->hasClashedFlag() || flags->hasClashedFlag()) && unsatisfiableFlag) {
										*unsatisfiableFlag = true;
									}
									if ((baseFlags->hasInsufficientFlag() || flags->hasInsufficientFlag()) && insufficientFlag) {
										*insufficientFlag = true;
									}
									if ((baseFlags->hasUnprocessedFlag() || flags->hasUnprocessedFlag()) && incompleteProcessedFlag) {
										*incompleteProcessedFlag = true;
									}
									return true;
								}
							}
						}
					}
				}
				return false;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
