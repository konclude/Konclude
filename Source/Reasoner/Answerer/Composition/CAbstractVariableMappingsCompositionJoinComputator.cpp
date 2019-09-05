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

#include "CAbstractVariableMappingsCompositionJoinComputator.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CAbstractVariableMappingsCompositionJoinComputator::CAbstractVariableMappingsCompositionJoinComputator() : CLogIdentifier("::Konclude::Reasoner::Kernel::Answerer", this) {
				}



				CAbstractVariableMappingsCompositionComputator* CAbstractVariableMappingsCompositionJoinComputator::configureComputator(COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, CAnswererContext* answererContext) {
					CAbstractVariableMappingsCompositionComputator::configureComputator(ontoAnsweringItem, answererContext);

					mConfExtendedLogging = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ExtendedLogging", false);

					mConfMappingsComputationUnlimitedInterpretationSize = Q_UINT64_C(100000000000000);
					mConfMappingsComputationUnlimitedInterpretationSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MappingsComputationUnlimitedInterpretationSize", Q_UINT64_C(100000000000000));

					mConfSamplingBasedJoinMappingSize = 1000;
					mConfSamplingBasedJoinMappingSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.SamplingBasedJoinMappingSize", 1000);

					return this;
				}


				CAbstractVariableMappingsCompositionJoinComputator* CAbstractVariableMappingsCompositionJoinComputator::computeVariableMappingsComposition(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) {
					COptimizedComplexVariableJoiningItem* joiningItem = (COptimizedComplexVariableJoiningItem*)compVarItem;

					COptimizedComplexVariableCompositionItem* leftItem = joiningItem->getLeftItem();
					COptimizedComplexVariableCompositionItem* rightItem = joiningItem->getRightItem();

					COptimizedComplexVariableIndividualMappings* leftVarMapping = leftItem->getVariableMapping();
					COptimizedComplexVariableIndividualMappings* rightVarMapping = rightItem->getVariableMapping();

					cint64 leftSize = leftVarMapping->getBindingSize();
					cint64 rightSize = rightVarMapping->getBindingSize();


					if (leftVarMapping->getBindingCount() <= 0) {
						rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, leftItem, 1);
						return this;
					}
					if (rightVarMapping->getBindingCount() <= 0) {
						rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, rightItem, 1);
						return this;
					}


					bool mappingProcessed = computeVariableMappingsJoinComposition(joiningItem, buildingVarItem, answererContext, processing);

					COptimizedComplexVariableIndividualMappings* joinedVarMapping = joiningItem->getVariableMapping();
					cint64 joinSize = joinedVarMapping->getBindingSize();


					COptimizedComplexVariableCompositionItemDependence* leftItemDep = joiningItem->getLeftItemDependence();
					COptimizedComplexVariableCompositionItemDependence* rightItemDep = joiningItem->getRightItemDependence();


					// compute expected variable mappings from join
					cint64 joinedMappingsCount = joiningItem->getVariableMappingsCurrentCount();
					if (joinedMappingsCount > 0) {
						cint64 leftTotalProcessedMappingsCount = leftItemDep->getTotalProcessedBindingsCardinalityLinkerCount();
						cint64 rightTotalProcessedMappingsCount = rightItemDep->getTotalProcessedBindingsCardinalityLinkerCount();

						double leftMappingsJoinResultingsMappingFactor = (double)leftTotalProcessedMappingsCount / (double)joinedMappingsCount;
						double rightMappingsJoinResultingsMappingFactor = (double)rightTotalProcessedMappingsCount / (double)joinedMappingsCount;

						double leftMappingsExpectedCount = leftItem->getVariableMappingsExpectedCount() / leftMappingsJoinResultingsMappingFactor;
						double rightMappingsExpectedCount = rightItem->getVariableMappingsExpectedCount() / rightMappingsJoinResultingsMappingFactor;

						double totalMappingsExpectedCount = (leftMappingsExpectedCount + rightMappingsExpectedCount) / 2.;
						joiningItem->setVariableMappingsExpectedCount(totalMappingsExpectedCount);
					}

					// schedule dependency computations
					bool dependentItemRescheduled = false;
					if (joiningItem->requiresMoreVariableMappingsComputation()) {
						cint64 leftTotalProcessedMappingsCount = leftItemDep->getTotalProcessedBindingsCardinalityLinkerCount();
						cint64 rightTotalProcessedMappingsCount = rightItemDep->getTotalProcessedBindingsCardinalityLinkerCount();
						cint64 requiredMappingCount = joiningItem->getVariableMappingsComputationRequirement();

						double joinedVarMapCount = joinedMappingsCount;
						if (joinedMappingsCount <= 0) {
							joinedVarMapCount = qMax(0.01, 10. / ((double)leftTotalProcessedMappingsCount + rightTotalProcessedMappingsCount + 10));
						}

						double leftMappingsJoinResultingsMappingFactor = (double)leftTotalProcessedMappingsCount / (double)joinedVarMapCount;
						double rightMappingsJoinResultingsMappingFactor = (double)rightTotalProcessedMappingsCount / (double)joinedVarMapCount;

						double leftRequiredMappingsCount = qMax(requiredMappingCount * leftMappingsJoinResultingsMappingFactor + 1., leftTotalProcessedMappingsCount + 1.);
						double rightRequiredMappingsCount = qMax(requiredMappingCount * rightMappingsJoinResultingsMappingFactor + 1., rightTotalProcessedMappingsCount + 1.);

						leftRequiredMappingsCount = leftRequiredMappingsCount * joiningItem->repeatedRequirementInsufficientDependencyComputationIncreaseFactor();
						rightRequiredMappingsCount = rightRequiredMappingsCount * joiningItem->repeatedRequirementInsufficientDependencyComputationIncreaseFactor();


						if (requiredMappingCount == -1 || mConfMappingsComputationUnlimitedInterpretationSize >= 0 && leftTotalProcessedMappingsCount >= mConfMappingsComputationUnlimitedInterpretationSize) {
							leftRequiredMappingsCount = -1;
						}
						if (requiredMappingCount == -1 || mConfMappingsComputationUnlimitedInterpretationSize >= 0 && rightTotalProcessedMappingsCount >= mConfMappingsComputationUnlimitedInterpretationSize) {
							rightRequiredMappingsCount = -1;
						}


						bool leftInsertion = joiningItem->isInsertionSideLeft();
						if ((!joiningItem->isSamplingCompleted() || rightItemDep->isProcessingFinished(false) || leftInsertion) && !leftItemDep->isProcessingFinished(false) && !leftItemDep->hasMappingsCountComputationRequested(leftRequiredMappingsCount)) {
							leftItemDep->setLastRequestedMappingsComputationCount(leftRequiredMappingsCount);
							bool leftDependentItemRescheduled = rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, leftItem, leftRequiredMappingsCount);

							if (leftDependentItemRescheduled && mConfExtendedLogging) {
								LOG(INFO, getLogDomain(), logTr("Computation step %1 (attempt %2): Requesting %3 mappings from left step %4.").arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt(false)).arg(leftRequiredMappingsCount).arg(leftItem->getComputationStepId()), this);
							}
							dependentItemRescheduled |= leftDependentItemRescheduled;

						}
						if ((!joiningItem->isSamplingCompleted() || leftItemDep->isProcessingFinished(false) || !leftInsertion) && !rightItemDep->isProcessingFinished(false) && !rightItemDep->hasMappingsCountComputationRequested(rightRequiredMappingsCount)) {
							rightItemDep->setLastRequestedMappingsComputationCount(rightRequiredMappingsCount);
							bool rightDependentItemRescheduled = rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, rightItem, rightRequiredMappingsCount);

							if (rightDependentItemRescheduled && mConfExtendedLogging) {
								LOG(INFO, getLogDomain(), logTr("Computation step %1 (attempt %2): Requesting %3 mappings from right step %4.").arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt(false)).arg(rightRequiredMappingsCount).arg(rightItem->getComputationStepId()), this);
							}
							dependentItemRescheduled |= rightDependentItemRescheduled;
						}
					}


					if (!joiningItem->requiresMoreVariableMappingsComputation() || !dependentItemRescheduled && compVarItem->getDependencyUpdatingCount() <= 0) {
						finishVariableCompositionItemComputation(buildingVarItem, compVarItem, rightItemDep->isProcessingFinished() && leftItemDep->isProcessingFinished());

						if (mConfExtendedLogging) {
							LOG(INFO, getLogDomain(), logTr("Computation step %4 (attempt %5): Join computation for %1 and %2 mappings resulted in %3 mappings.").arg(leftVarMapping->getBindingCount()).arg(rightVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()), this);
						}
					}


					configureDependentComputationIncreaseFactor(dependentItemRescheduled, mappingProcessed, compVarItem);

					//if (joinedVarMapping->count() == 0) {
					//	bool debug = true;
					//}
					//debugCheckVariableMappingContainsSolution(buildingVarItem, compVarItem);
					return this;
				}




				bool CAbstractVariableMappingsCompositionJoinComputator::isSamplingLeftInsertionDecision(COptimizedComplexVariableJoiningItem* joiningItem) {
					COptimizedComplexVariableCompositionItem* leftItem = joiningItem->getLeftItem();
					COptimizedComplexVariableCompositionItem* rightItem = joiningItem->getRightItem();

					cint64 leftSampleKeyCount = joiningItem->getLeftSampleKeyCount();
					cint64 rightSampleKeyCount = joiningItem->getRightSampleKeyCount();

					cint64 leftSampleInsertionCount = joiningItem->getLeftSampleInsertionCount();
					cint64 rightSampleInsertionCount = joiningItem->getRightSampleInsertionCount();


					bool leftInsertion = joiningItem->isInsertionSideLeft();
					if (joiningItem->isSamplingCompleted() && !joiningItem->isInsertionSideDecided()) {
						double leftInsertionKeyFactor = (double)leftSampleKeyCount / (double)leftSampleInsertionCount;
						double rightInsertionKeyFactor = (double)rightSampleKeyCount / (double)rightSampleInsertionCount;
						double leftExpectedKeyCount = leftItem->getVariableMappingsExpectedCount() * leftInsertionKeyFactor;
						double rightExpectedKeyCount = rightItem->getVariableMappingsExpectedCount() * rightInsertionKeyFactor;
						if (leftExpectedKeyCount < rightExpectedKeyCount) {
							joiningItem->setInsertionSideLeft(true);
							leftInsertion = true;
						} else {
							joiningItem->setInsertionSideLeft(false);
							leftInsertion = false;

						}
						joiningItem->setInsertionSideDecided(true);
					}
					return leftInsertion;
				}



			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
