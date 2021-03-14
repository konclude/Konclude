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

#include "CAbstractVariableMappingsCompositionJoinComputator.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CAbstractVariableMappingsCompositionJoinComputator::CAbstractVariableMappingsCompositionJoinComputator() : CLogIdentifier("::Konclude::Reasoner::Kernel::Answerer", this) {
					mComputerName = "Join";
				}



				CAbstractVariableMappingsCompositionComputator* CAbstractVariableMappingsCompositionJoinComputator::configureComputator(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, CAbstractVariableMappingsCompositionItemRequirementProcessor* reqProcessor, CAnswererContext* answererContext) {
					CAbstractVariableMappingsCompositionComputator::configureComputator(ontoAnsweringItem, reqProcessor, answererContext);

					mConfExtendedLogging = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ExtendedLogging", false);

					mConfMappingsComputationUnlimitedInterpretationSize = Q_UINT64_C(100000000000000);
					mConfMappingsComputationUnlimitedInterpretationSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MappingsComputationUnlimitedInterpretationSize", Q_UINT64_C(100000000000000));

					mConfSamplingBasedJoinMappingSize = 1000;
					mConfSamplingBasedJoinMappingSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.SamplingBasedJoinMappingSize", 1000);

					mConfMappingCountSplitModeActivation = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.LastStepSplitComputationMappingsRequirement", 5000000);
					mConfExpectedInputMappingCountDirectSplitModeActivation = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.LastStepDirectSplitComputationWithExpectedInputMappings", 5000000);
					mConfAllowSplitModeActivation = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.AllowLastStepSplitComputation", true);
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

					bool depScheduled = false;
					if (!leftItem->isVariableMappingsComputed() && compVarItem->isSplitComputationMode()) {
						rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, leftItem, -1);
						depScheduled = true;
					}
					if (!rightItem->isVariableMappingsComputed() && compVarItem->isSplitComputationMode()) {
						rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, rightItem, -1);
						depScheduled = true;
					}

					if (!depScheduled && leftVarMapping->getBindingCount() <= 0) {
						rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, leftItem, 1);
						depScheduled = true;
					}
					if (!depScheduled && rightVarMapping->getBindingCount() <= 0) {
						rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, rightItem, 1);
						depScheduled = true;
					}

					if (depScheduled) {
						return this;
					}

					bool mappingProcessed = computeVariableMappingsJoinComposition(joiningItem, buildingVarItem, answererContext, processing);


					if (joiningItem->requiresSplitComputationActivation() && !joiningItem->hasSplitComputations()) {
						if (initSplitComputation(joiningItem)) {
							if (!leftItem->isVariableMappingsComputed() && joiningItem->hasSplitComputations()) {
								rescheduleVariableCompositionItemComputation(buildingVarItem, joiningItem, leftItem, -1);
								depScheduled = true;
							}
							if (!rightItem->isVariableMappingsComputed() && joiningItem->hasSplitComputations()) {
								rescheduleVariableCompositionItemComputation(buildingVarItem, joiningItem, rightItem, -1);
								depScheduled = true;
							}
							if (depScheduled) {
								return this;
							}
						}
					}

					if (joiningItem->hasSplitComputations()) {
						mappingProcessed = true;
						if (joiningItem->getCurrentSplitComputationPosition() != joiningItem->getNextSplitComputationPosition()) {
							nextSplitComputation(joiningItem);
						}
						computeVariableMappingsComposition(joiningItem->getCurrentSplitComputationItem(), buildingVarItem, answererContext, processing);
						joiningItem->incCompletedSplitComputationCount();
						while (joiningItem->getCurrentSplitComputationItem()->getVariableMappingsCurrentCount() <= 0 && joiningItem->getRemainingSplitComputationCount() > 0) {

							if (joiningItem->getCurrentSplitComputationItem()->getVariableMappingsCurrentCount() <= 0 && joiningItem->getRemainingSplitComputationCount() > 0) {
								joiningItem->setNextSplitComputationPosition(joiningItem->getNextSplitComputationPosition() + 1);
								nextSplitComputation(joiningItem);
							}
							computeVariableMappingsComposition(joiningItem->getCurrentSplitComputationItem(), buildingVarItem, answererContext, processing);
							joiningItem->incCompletedSplitComputationCount();
						}
					}


					COptimizedComplexVariableIndividualMappings* joinedVarMapping = joiningItem->getVariableMapping();
					cint64 joinSize = joinedVarMapping->getBindingSize();


					COptimizedComplexVariableCompositionItemDependence* leftItemDep = joiningItem->getLeftItemDependence();
					COptimizedComplexVariableCompositionItemDependence* rightItemDep = joiningItem->getRightItemDependence();
					

					// compute expected variable mappings from join
					cint64 joinedMappingsCount = joiningItem->getVariableMappingsCurrentCount();
					double totalMappingsExpectedCount = getUpdatedExpectedVariableMappingsCount(joiningItem);

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

						if (mConfExtendedLogging && !joiningItem->isSplitComputationMode()) {
							if (joiningItem->hasSplitComputations()) {
								COptimizedComplexVariableJoiningItem* splitItem = (COptimizedComplexVariableJoiningItem*)joiningItem->getCurrentSplitComputationItem();
								LOG(INFO, getLogDomain(), logTr("Computation step %4 (partition %7, attempt %5): %6 computation for %1 and %2 mappings resulted in %3 mappings.").arg(leftVarMapping->getBindingCount()).arg(rightVarMapping->getBindingCount()).arg(splitItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(mComputerName).arg(joiningItem->getCurrentSplitComputationPosition() + 1), this);
							} else {
								LOG(INFO, getLogDomain(), logTr("Computation step %4 (attempt %5): %6 computation for %1 and %2 mappings resulted in %3 mappings.").arg(leftVarMapping->getBindingCount()).arg(rightVarMapping->getBindingCount()).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(mComputerName), this);
							}
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




				bool CAbstractVariableMappingsCompositionJoinComputator::nextSplitComputation(COptimizedComplexVariableJoiningItem* joiningItem) {
					if (joiningItem->getNextSplitComputationPosition() != joiningItem->getCurrentSplitComputationPosition()) {
						joiningItem->setCurrentSplitComputationPosition(joiningItem->getNextSplitComputationPosition());
						COptimizedComplexVariableJoiningItem* splitItem = joiningItem->createSplitComputationItem();

						bool insertionSideNextSplit = false;
						if (splitItem->isInsertionSideLeft()) {
							cint64& checkingSide = splitItem->getRightSplitPosition();
							cint64& insertionSide = splitItem->getLeftSplitPosition();
							++checkingSide;
							if (checkingSide >= splitItem->getRightSplitCount()) {
								checkingSide = 0;
								++insertionSide;
								insertionSideNextSplit = true;
							}
						} else {
							cint64& checkingSide = splitItem->getLeftSplitPosition();
							cint64& insertionSide = splitItem->getRightSplitPosition();
							++checkingSide;
							if (checkingSide >= splitItem->getLeftSplitCount()) {
								checkingSide = 0;
								++insertionSide;
								insertionSideNextSplit = true;
							}
						}


						splitComputationReset(splitItem, insertionSideNextSplit);
						return true;
					}
					return false;
				}



				bool CAbstractVariableMappingsCompositionJoinComputator::initSplitComputation(COptimizedComplexVariableJoiningItem* joiningItem) {
					if (!joiningItem->hasSplitComputations()) {
						joiningItem->setSplitComputations(true);

						double epxectedMappingCount = getUpdatedExpectedVariableMappingsCount(joiningItem);
						double currentMappingCount = joiningItem->getVariableMappingsCurrentCount();

						cint64 splits = (epxectedMappingCount / mConfMappingSplitCount) + 1;
						splits = qMax((cint64)mConfMappingComputationMinSplitCount, splits);
						splits = qMin((cint64)mConfMappingComputationMaxSplitCount, splits);

						COptimizedComplexVariableJoiningItem* splitItem = joiningItem->createSplitComputationItem();
						splitItem->setSplitComputationMode(true);
						splitItem->setVariableMappingsComputationRequirement(-1);


						COptimizedComplexVariableJoiningBindingPositionMapping* variablePositionMapping = joiningItem->getPositionMapping();
						bool splitBothSides = variablePositionMapping->getRightRemainingBindingLinker() && variablePositionMapping->getLeftRemainingBindingLinker();
						if (splitBothSides) {

							cint64 insertionSplitCount = (cint64)(std::sqrt((double)splits) / 2.0) + 1;
							cint64 checkingSplitCount = splits / insertionSplitCount;
							splits = insertionSplitCount * checkingSplitCount;


							if (splitItem->isInsertionSideLeft()) {
								splitItem->getLeftSplitCount() = insertionSplitCount;
								splitItem->getRightSplitCount() = checkingSplitCount;
							} else {
								splitItem->getLeftSplitCount() = checkingSplitCount;
								splitItem->getRightSplitCount() = insertionSplitCount;
							}
						} else if (variablePositionMapping->getLeftRemainingBindingLinker()) {

							splitItem->getLeftSplitCount() = splits;
							splitItem->getRightSplitCount() = 1;

						} else if (variablePositionMapping->getRightRemainingBindingLinker()) {

							splitItem->getRightSplitCount() = splits;
							splitItem->getLeftSplitCount() = 1;

						}

						joiningItem->setTotalSplitComputationCount(splits);
						joiningItem->setCurrentSplitComputationPosition(0);


						COptimizedComplexVariableIndividualMappings* splitVarMapping = nullptr;
						COptimizedComplexVariableIndividualMappings* joinedVarMapping = joiningItem->getVariableMapping();
						COptimizedComplexVariableIndividualMappingsMultiHash* multiJoinedVarMapping = dynamic_cast<COptimizedComplexVariableIndividualMappingsMultiHash*>(joinedVarMapping);
						if (multiJoinedVarMapping) {
							splitVarMapping = new COptimizedComplexVariableIndividualMappingsMultiHash(joinedVarMapping->getBindingSize(), multiJoinedVarMapping->getMultiHashPartsCount());
						} else {
							splitVarMapping = new COptimizedComplexVariableIndividualMappingsHash(joinedVarMapping->getBindingSize());
						}
						for (cint64 i = 0; i < joinedVarMapping->getBindingSize(); ++i) {
							splitVarMapping->setBindingMapping(i, joinedVarMapping->getBindingMapping(i));
						}
						splitItem->setVariableMapping(splitVarMapping);


						if (mConfExtendedLogging) {
							LOG(INFO, getLogDomain(), logTr("Splitting computation of step %1 into %4 partitions (%5 for left side, %6 for right side) at %2 computed of %3 expected mappings.").arg(joiningItem->getComputationStepId()).arg(joiningItem->getVariableMappingsCurrentCount()).arg(joiningItem->getVariableMappingsExpectedCount()).arg(splits).arg(splitItem->getLeftSplitCount()).arg(splitItem->getRightSplitCount()), this);
						}

						return true;
					}
					return false;
				}


				bool CAbstractVariableMappingsCompositionJoinComputator::requiresMoreVariableMappingComputation(COptimizedComplexVariableJoiningItem* joiningItem, bool sampling) {
					if (sampling) {
						return joiningItem->requiresMoreVariableMappingsComputation();
					} else {
						if (joiningItem->hasSplitComputations() || joiningItem->requiresSplitComputationActivation()) {
							return false;
						}
						if (mConfAllowSplitModeActivation && joiningItem->isLastComputation()) {
							COptimizedComplexVariableCompositionItem* leftItem = joiningItem->getLeftItem();
							COptimizedComplexVariableCompositionItem* rightItem = joiningItem->getRightItem();
							if (!joiningItem->isSplitComputationMode() && rightItem->getVariableMappingsExpectedCount() + leftItem->getVariableMappingsExpectedCount() > mConfExpectedInputMappingCountDirectSplitModeActivation) {
								joiningItem->setRequiresSplitComputationActivation(true);
								return false;
							}
							if (!joiningItem->isSplitComputationMode()) {
								double epxectedMappingCount = getUpdatedExpectedVariableMappingsCount(joiningItem);
								if (epxectedMappingCount > mConfMappingCountExpectedFactorSplitModeActivation * joiningItem->getVariableMappingsCurrentCount() && joiningItem->getVariableMappingsCurrentCount() > mConfMappingCountSplitModeActivation) {
									joiningItem->setRequiresSplitComputationActivation(true);
									return false;
								}
							}
						}
						return joiningItem->requiresMoreVariableMappingsComputation();
					}
				}


				cint64 CAbstractVariableMappingsCompositionJoinComputator::getUpdatedExpectedVariableMappingsCount(COptimizedComplexVariableJoiningItem* joiningItem) {
					double totalMappingsExpectedCount = 0;
					// compute expected variable mappings from join
					cint64 joinedMappingsCount = joiningItem->getVariableMappingsCurrentCount();
					if (joinedMappingsCount > 0) {
						COptimizedComplexVariableCompositionItemDependence* leftItemDep = joiningItem->getLeftItemDependence();
						COptimizedComplexVariableCompositionItemDependence* rightItemDep = joiningItem->getRightItemDependence();

						COptimizedComplexVariableCompositionItem* leftItem = joiningItem->getLeftItem();
						COptimizedComplexVariableCompositionItem* rightItem = joiningItem->getRightItem();

						cint64 leftTotalProcessedMappingsCount = leftItemDep->getTotalProcessedBindingsCardinalityLinkerCount();
						cint64 rightTotalProcessedMappingsCount = rightItemDep->getTotalProcessedBindingsCardinalityLinkerCount();

						double leftMappingsJoinResultingsMappingFactor = (double)leftTotalProcessedMappingsCount / (double)joinedMappingsCount;
						double rightMappingsJoinResultingsMappingFactor = (double)rightTotalProcessedMappingsCount / (double)joinedMappingsCount;

						double leftMappingsExpectedCount = leftItem->getVariableMappingsExpectedCount() / leftMappingsJoinResultingsMappingFactor;
						double rightMappingsExpectedCount = rightItem->getVariableMappingsExpectedCount() / rightMappingsJoinResultingsMappingFactor;

						totalMappingsExpectedCount = (leftMappingsExpectedCount + rightMappingsExpectedCount) / 2.;
						joiningItem->setVariableMappingsExpectedCount(totalMappingsExpectedCount);
					}
					return totalMappingsExpectedCount;
				}


				bool CAbstractVariableMappingsCompositionJoinComputator::splitComputationReset(COptimizedComplexVariableJoiningItem* joiningItem, bool resetJoiningHash) {

					COptimizedComplexVariableCompositionItemDependence* leftItemDep = joiningItem->getLeftItemDependence();
					COptimizedComplexVariableCompositionItemDependence* rightItemDep = joiningItem->getRightItemDependence();

					if (resetJoiningHash || !joiningItem->isInsertionSideLeft()) {
						leftItemDep->reset();
					}
					if (resetJoiningHash || joiningItem->isInsertionSideLeft()) {
						rightItemDep->reset();
					}


					joiningItem->getVariableMapping()->clearComputedMappings();
					if (resetJoiningHash) {
						joiningItem->resetJoiningHash();
					}
					return this;
				}



			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
