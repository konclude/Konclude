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

#include "CAbstractVariableMappingsCompositionPropagationComputator.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CAbstractVariableMappingsCompositionPropagationComputator::CAbstractVariableMappingsCompositionPropagationComputator() : CLogIdentifier("::Konclude::Reasoner::Kernel::Answerer", this) {
					mComputerName = "Propagation";
				}



				CAbstractVariableMappingsCompositionComputator* CAbstractVariableMappingsCompositionPropagationComputator::configureComputator(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, CAbstractVariableMappingsCompositionItemRequirementProcessor* reqProcessor, CAnswererContext* answererContext) {
					CAbstractVariableMappingsCompositionComputator::configureComputator(ontoAnsweringItem, reqProcessor, answererContext);

					mConfExtendedLogging = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ExtendedLogging", false);

					mConfMappingsComputationUnlimitedInterpretationSize = Q_UINT64_C(100000000000000);
					mConfMappingsComputationUnlimitedInterpretationSize = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MappingsComputationUnlimitedInterpretationSize", Q_UINT64_C(100000000000000));

					mRoleFillerRealizationSorting = CRealizationIndividualSorting(CRealizationIndividualSorting::SORT_TYPE_NOTHING, CRealizationIndividualSorting::SORT_ORDER_NOTHING, CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE);
					return this;
				}


				CAbstractVariableMappingsCompositionPropagationComputator* CAbstractVariableMappingsCompositionPropagationComputator::computeVariableMappingsComposition(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildVarItem, CAnswererContext* answererContext, bool& processing) {
					COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem = (COptimizedComplexBuildingIndividualVariableCompositionsItem*)buildVarItem;
					COptimizedComplexVariableRolePropagationAbstractItem* rolePropItem = (COptimizedComplexVariableRolePropagationAbstractItem*)compVarItem;
					COptimizedComplexVariableCompositionItem* baseVarItem = rolePropItem->getBaseItem();
					CRole* role = rolePropItem->getRole();
					bool inversed = rolePropItem->getInversed();
					COptimizedComplexVariableIndividualMappings* baseVariableMapping = baseVarItem->getVariableMapping();
					COptimizedComplexVariableCompositionItemDependence* baseItemDep = rolePropItem->getBaseItemDependence();
					COptimizedComplexVariableIndividualMappings* variableMapping = rolePropItem->getVariableMapping();
					

					bool mappingProcessed = computeVariableMappingsPropagationComposition(rolePropItem, buildingVarItem, answererContext, processing);


					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataBase*>* waitingProcessedRealizationIteratorDataList = rolePropItem->getWaitingProcessedRealizationIteratorDataList();
					QList<COntologyProcessingRequirement*>* schedulingReqList = rolePropItem->geSchedulingRealizationRequirementIteratorDataList();
					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataBase*>* schedulingRealizationIteratorDataList = rolePropItem->getSchedulingRealizationIteratorDataList();


					bool dependentItemRescheduled = false;
					bool realizationScheduled = false;


					if (rolePropItem->requiresMoreVariableMappingsComputation()) {
						// determine for how many instances role realization has to be scheduled
						cint64 requiredMappingCount = rolePropItem->getVariableMappingsComputationRequirement();
						double requiredProcessingCount = 0;

						// estimated realization count based on expected fillers per propagation item
						double exprectedFillerPerPropagationItemCount = rolePropItem->getExpectedFillerPerPropagationItemCount();


						cint64 propagatedInstanceItemCount = rolePropItem->getPropagatedInstanceItemCount();
						double propagatedInstanceItemCreatedVariableMappingsFactor = 0.1;
						if (propagatedInstanceItemCount > 0) {
							double varMapCount = variableMapping->getBindingCount();
							if (variableMapping->getBindingCount() <= 0) {
								varMapCount = qMax(0.01, 10. / ((double)propagatedInstanceItemCount + 10));
							}
							propagatedInstanceItemCreatedVariableMappingsFactor = (double)propagatedInstanceItemCount / varMapCount;
						}

						requiredProcessingCount = qMax(requiredMappingCount * propagatedInstanceItemCreatedVariableMappingsFactor + 1., propagatedInstanceItemCount + 1.);

						requiredProcessingCount -= rolePropItem->getScheduledRealizationCount();
						

						requiredProcessingCount = requiredProcessingCount * rolePropItem->repeatedRequirementInsufficientDependencyComputationIncreaseFactor();

						cint64 requiredProcessingCountInteger = requiredProcessingCount;
						requiredProcessingCountInteger = qMax((cint64)-1, requiredProcessingCountInteger);
						if (requiredMappingCount == -1 || mConfMappingsComputationUnlimitedInterpretationSize >= 0 && requiredProcessingCount >= mConfMappingsComputationUnlimitedInterpretationSize) {
							requiredProcessingCountInteger = -1;
						}


						if ((requiredProcessingCountInteger > 0 || requiredProcessingCountInteger == -1) && !schedulingReqList->isEmpty()) {

							QList<COntologyProcessingRequirement*> realList;
							while ((requiredProcessingCountInteger > 0 || requiredProcessingCountInteger == -1) && !schedulingReqList->isEmpty()) {
								COntologyProcessingRequirement* realReq = schedulingReqList->takeFirst();
								if (requiredProcessingCountInteger != -1) {
									requiredProcessingCountInteger--;
								}
								realList.append(realReq);
								COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataBase* itData = schedulingRealizationIteratorDataList->takeFirst();
								waitingProcessedRealizationIteratorDataList->append(itData);
							}

							if (!realList.isEmpty()) {
								realizationScheduled = true;
								rolePropItem->incScheduledRealizationCount(realList.size());
								processRequirements(realList, compVarItem, buildingVarItem, answererContext);
								processing = true;
							}
						}

						if ((requiredProcessingCountInteger > 0 || requiredProcessingCountInteger == -1) && !baseVarItem->isVariableMappingsComputed() && !baseItemDep->hasMappingsCountComputationRequested(requiredProcessingCountInteger)) {
							baseItemDep->setLastRequestedMappingsComputationCount(requiredProcessingCountInteger);
							dependentItemRescheduled = rescheduleVariableCompositionItemComputation(buildingVarItem, compVarItem, baseVarItem, requiredProcessingCountInteger);

							if (dependentItemRescheduled && mConfExtendedLogging) {
								LOG(INFO, getLogDomain(), logTr("Computation step %1 (attempt %2): Requesting %3 mappings from step %4.").arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt(false)).arg(requiredProcessingCountInteger).arg(baseVarItem->getComputationStepId()), this);
							}

						}


					}

					if (variableMapping->getBindingCount() > 0) {
						cint64 propagatedInstanceItemCount = rolePropItem->getPropagatedInstanceItemCount();
						double propagatedInstanceItemCreatedVariableMappingsFactor = (double)propagatedInstanceItemCount / variableMapping->getBindingCount();
						rolePropItem->setVariableMappingsExpectedCount(baseVarItem->getVariableMappingsExpectedCount() / propagatedInstanceItemCreatedVariableMappingsFactor);
					} else {
						rolePropItem->setVariableMappingsExpectedCount(0);
					}


					bool computationRunning = realizationScheduled || dependentItemRescheduled || rolePropItem->getScheduledRealizationCount() > 0 || !schedulingReqList->isEmpty();
					if (!rolePropItem->requiresMoreVariableMappingsComputation() || !computationRunning && rolePropItem->getDependencyUpdatingCount() <= 0) {
						bool computationFinished = !computationRunning && baseItemDep->isProcessingFinished();
						finishVariableCompositionItemComputation(buildingVarItem, compVarItem, computationFinished);

						if (mConfExtendedLogging) {
							QString roleName = "?";
							if (role->getPropertyNameLinker()) {
								roleName = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
								if (inversed) {
									roleName = roleName + " inversed";
								}
							} else if (role->getInverseRole() && role->getInverseRole()->getPropertyNameLinker()) {
								roleName = CIRIName::getRecentIRIName(role->getInverseRole()->getPropertyNameLinker());
								if (!inversed) {
									roleName = roleName + " inversed";
								}
							}
							LOG(INFO, getLogDomain(), logTr("Computation step %4 (attempt %5): %6 of %1 mappings over role %2 resulted in %3 mappings.").arg(baseVariableMapping->getBindingCount()).arg(roleName).arg(compVarItem->getVariableMapping()->getBindingCount()).arg(compVarItem->getComputationStepId()).arg(compVarItem->getComputationAttempt()).arg(mComputerName), this);
						}
					}

					configureDependentComputationIncreaseFactor(dependentItemRescheduled, mappingProcessed, compVarItem);


					//if (CIRIName::getRecentIRIName(rolePropItem->getRole()->getNameLinker()) == "http://ontology.dumontierlab.com/isPartOf" && rolePropItem->isVariableMappingsComputed()) {
					//	debugCheckVariableMappingFromFile(buildingVarItem, compVarItem);

					//	debugWriteVariableMappingToFile(buildingVarItem, compVarItem);
					//}



					//if (variableMapping->count() == 0) {
					//	bool debug = true;
					//}
					//debugCheckVariableMappingContainsSolution(buildingVarItem, compVarItem);

					return this;

				}






			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
