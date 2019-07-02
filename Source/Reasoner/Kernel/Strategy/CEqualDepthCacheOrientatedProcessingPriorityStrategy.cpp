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

#include "CEqualDepthCacheOrientatedProcessingPriorityStrategy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Strategy {

				CEqualDepthCacheOrientatedProcessingPriorityStrategy::CEqualDepthCacheOrientatedProcessingPriorityStrategy() {
				}

				CEqualDepthCacheOrientatedProcessingPriorityStrategy::~CEqualDepthCacheOrientatedProcessingPriorityStrategy() {
				}

				double CEqualDepthCacheOrientatedProcessingPriorityStrategy::getPriorityForTaskBranching(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, CIndividualProcessNode* indiProcessNode, CConceptDescriptor* branchingConcept, CSortedNegLinker<CConcept*>* branchedConcept, cint64 branchNumber) {
					double parentDepth = parentTask->getTaskDepth();
					double priority = 0.;
					double maxBranchCount = branchingConcept->getConcept()->getOperandCount();
					double parentPriority = parentTask->getTaskPriority();

					bool disjunctCached = false;

					CConcreteOntology* ontology = branchingTask->getProcessingDataBox()->getOntology();
					CConsistence* consistence = ontology->getConsistence();
					if (consistence) {
						CConsistenceData* consData = consistence->getConsistenceModelData();
						if (consData) {
							CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
							if (consTaskData) {
								CSatisfiableCalculationTask* compGraphCachedCalcTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
								if (compGraphCachedCalcTask) {
									CIndividualProcessNodeVector* compGraphCachedProcNodeVec = compGraphCachedCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
									if (indiProcessNode->getIndividualID() < compGraphCachedProcNodeVec->getItemCount()) {
										CIndividualProcessNode* compIndiProcNode = compGraphCachedProcNodeVec->getData(indiProcessNode->getIndividualID());
										if (compIndiProcNode && compIndiProcNode->getReapplyConceptLabelSet(false)->containsConcept(branchedConcept->getData(),branchedConcept->isNegated())) {
											disjunctCached = true;
										}
									}
								}
							}
						}
					}

					double prioritySubOffset = 0.2;
					if (disjunctCached) {
						prioritySubOffset = 0.3;
					}

					priority = parentDepth + 1. + (prioritySubOffset - branchNumber / (10 * maxBranchCount));
					return priority;
				}



			}; // end namespace Strategy

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
