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

#include "CEqualDepthCacheOrientatedProcessingPriorityStrategy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Strategy {

				CEqualDepthCacheOrientatedProcessingPriorityStrategy::CEqualDepthCacheOrientatedProcessingPriorityStrategy() {
				}

				CEqualDepthCacheOrientatedProcessingPriorityStrategy::~CEqualDepthCacheOrientatedProcessingPriorityStrategy() {
				}

				double CEqualDepthCacheOrientatedProcessingPriorityStrategy::getPriorityForTaskBranching(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, CIndividualProcessNode* indiProcessNode, CConceptDescriptor* branchingConcept, CSortedNegLinker<CConcept*>* branchedConcept, cint64 branchNumber, CDisjunctBranchingStatistics* branchStats) {
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
									if (indiProcessNode->getIndividualNodeID() <= compGraphCachedProcNodeVec->getItemMaxIndex()) {
										CIndividualProcessNode* compIndiProcNode = compGraphCachedProcNodeVec->getData(indiProcessNode->getIndividualNodeID());
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

					double learningOffset = 0.0;
					double branchOffset = -branchNumber / (10 * maxBranchCount);
					if (branchStats) {
						cint64 clashCount = branchStats->getClashInvolvedCount();
						cint64 expandedCount = branchStats->getExpandedCount();
						cint64 satCount = branchStats->getSatisfiableOccurrenceCount();
						if (clashCount != 0 || expandedCount != 0 || satCount != 0) {
							branchOffset = -branchNumber / (100000 * maxBranchCount);
						}
						double clashFactor = 0;
						if (expandedCount != 0) {
							clashFactor = qMax(qMin(clashCount / (double)expandedCount,1.0),0.0);
						} else if (clashCount != 0) {
							clashFactor = (1./(clashCount*10));
						}
						double satFactor = 0;
						if (expandedCount != 0) {
							satFactor = qMax(qMin(satCount / (double)expandedCount,1.0),0.0);
						} else if (satFactor != 0) {
							satFactor = (1./(satCount*10));
						}
						learningOffset = 0.0;
						learningOffset += satFactor - clashFactor;
						learningOffset = learningOffset/10.;
						if (expandedCount != 0) {
							learningOffset += 1./(expandedCount*10000);
						}
					}

					priority = parentDepth + 1. + prioritySubOffset + learningOffset + branchOffset;
					return priority;
				}



			}; // end namespace Strategy

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
