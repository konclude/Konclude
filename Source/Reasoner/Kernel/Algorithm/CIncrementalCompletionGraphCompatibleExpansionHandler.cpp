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

#include "CIncrementalCompletionGraphCompatibleExpansionHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CIncrementalCompletionGraphCompatibleExpansionHandler::CIncrementalCompletionGraphCompatibleExpansionHandler() {
				}


				CIncrementalCompletionGraphCompatibleExpansionHandler::~CIncrementalCompletionGraphCompatibleExpansionHandler() {
				}



				CSatisfiableCalculationTask* CIncrementalCompletionGraphCompatibleExpansionHandler::getPreviousDeterministicCompletionGraphTask(CCalculationAlgorithmContext* calcAlgContext) {
					CSatisfiableTaskIncrementalConsistencyTestingAdapter* incConsTestAdapter = calcAlgContext->getSatisfiableCalculationTask()->getSatisfiableTaskIncrementalConsistencyTestingAdapter();
					if (incConsTestAdapter) {
						CConcreteOntology* prevConsOntology = incConsTestAdapter->getPreviousConsistentOntology();
						if (prevConsOntology) {
							CConsistence* consistence = prevConsOntology->getConsistence();
							if (consistence) {
								CConsistenceData* consData = consistence->getConsistenceModelData();
								if (consData) {
									CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
									if (consTaskData) {
										CSatisfiableCalculationTask* prevCompGraphCalcTask = consTaskData->getDeterministicSatisfiableTask();				
										return prevCompGraphCalcTask;
									}
								}
							}
						}
					}
					return nullptr;
				}

				CSatisfiableCalculationTask* CIncrementalCompletionGraphCompatibleExpansionHandler::getPreviousNondeterministicCompletionGraphTask(CCalculationAlgorithmContext* calcAlgContext) {
					CSatisfiableTaskIncrementalConsistencyTestingAdapter* incConsTestAdapter = calcAlgContext->getSatisfiableCalculationTask()->getSatisfiableTaskIncrementalConsistencyTestingAdapter();
					if (incConsTestAdapter) {
						CConcreteOntology* prevConsOntology = incConsTestAdapter->getPreviousConsistentOntology();
						if (prevConsOntology) {
							CConsistence* consistence = prevConsOntology->getConsistence();
							if (consistence) {
								CConsistenceData* consData = consistence->getConsistenceModelData();
								if (consData) {
									CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
									if (consTaskData) {
										CSatisfiableCalculationTask* prevCompGraphCalcTask = consTaskData->getCompletionGraphCachedSatisfiableTask();				
										return prevCompGraphCalcTask;
									}
								}
							}
						}
					}
					return nullptr;
				}


				CIndividualProcessNode* CIncrementalCompletionGraphCompatibleExpansionHandler::getPreviousDeterministicCompletionGraphCorrespondingIndividualNode(CIndividualProcessNode* individualNode, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualProcessNode* prevIndiNode = nullptr;
					CIndividualNodeIncrementalExpansionData* incExpData = individualNode->getIncrementalExpansionData(false);
					if (!incExpData || !incExpData->isPreviousCompletionGraphCorrespondenceIndividualNodeLoaded()) {
						CIndividualNodeIncrementalExpansionData* locIncExpData = individualNode->getIncrementalExpansionData(true);
						CSatisfiableCalculationTask* prevCompGraphCalcTask = getPreviousDeterministicCompletionGraphTask(calcAlgContext);
						if (prevCompGraphCalcTask) {
							CIndividualProcessNodeVector* prevCompGraphProcNodeVec = prevCompGraphCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
							if (prevCompGraphProcNodeVec) {
								prevIndiNode = prevCompGraphProcNodeVec->getData(individualNode->getIndividualNodeID());
							}
						}

						locIncExpData->setPreviousCompletionGraphCorrespondenceIndividualNode(prevIndiNode);
						locIncExpData->setPreviousCompletionGraphCorrespondenceIndividualNodeLoaded(true);
						incExpData = locIncExpData;
					}
					prevIndiNode = incExpData->getPreviousCompletionGraphCorrespondenceIndividualNode();
					return prevIndiNode;
				}


				bool CIncrementalCompletionGraphCompatibleExpansionHandler::isIndividualNodePreviousCompletionGraphCompatible(CIndividualProcessNode* individualNode, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualNodeIncrementalExpansionData* incExpData = individualNode->getIncrementalExpansionData(false);
					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					CConceptDescriptor* lastConDes = conSet->getAddingSortedConceptDescriptionLinker();
					CConceptDescriptor* lastCheckedConDes = incExpData->getLastCompatibleCheckedConceptDescriptor();
					bool compatible = incExpData->isPreviousCompletionGraphCompatible();
					if (!individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED) && lastCheckedConDes != lastConDes) {
						// recheck compatibility
						CIndividualProcessNode* prevIndiNode = getPreviousDeterministicCompletionGraphCorrespondingIndividualNode(individualNode,calcAlgContext);
						if (prevIndiNode && !prevIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED)) {
							bool incompatibleConcepts = false;
							CReapplyConceptLabelSet* prevConSet = prevIndiNode->getReapplyConceptLabelSet(false);
							if (conSet->getConceptSignatureValue() == prevConSet->getConceptSignatureValue() && prevConSet->getConceptCount() == conSet->getConceptCount()) {
								CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true);
								CReapplyConceptLabelSetIterator prevConSetIt = prevConSet->getConceptLabelSetIterator(true);
								while (conSetIt.hasNext() && prevConSetIt.hasNext()) {
									CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
									CConceptDescriptor* prevConDes = prevConSetIt.getConceptDescriptor();
									if (conDes->getConcept() != prevConDes->getConcept() || conDes->isNegated() != prevConDes->isNegated()) {
										incompatibleConcepts = true;
									}
									conSetIt.moveNext();
									prevConSetIt.moveNext();
								}
							} else {
								incompatibleConcepts = true;
							}
							if (!incompatibleConcepts) {
								compatible = true;
							}
						}
						CIndividualNodeIncrementalExpansionData* locIncExpData = individualNode->getIncrementalExpansionData(true);
						locIncExpData->setPreviousCompletionGraphCompatible(compatible);
						locIncExpData->setLastCompatibleCheckedConceptDescriptor(lastConDes);
					}
					return compatible;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
