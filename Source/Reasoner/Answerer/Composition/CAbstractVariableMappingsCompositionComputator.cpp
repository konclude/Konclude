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

#include "CAbstractVariableMappingsCompositionComputator.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CAbstractVariableMappingsCompositionComputator::CAbstractVariableMappingsCompositionComputator() {
				}

				CAbstractVariableMappingsCompositionComputator* CAbstractVariableMappingsCompositionComputator::configureComputator(COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, CAnswererContext* answererContext) {
					mOntoAnsweringItem = ontoAnsweringItem;

					mConfMappingsRepeatedlyInsufficientDependencyComputationIncreasingFactor = 1.3;
					mConfMappingsRepeatedlyInsufficientDependencyComputationIncreasingFactor = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.MappingsRepeatedlyInsufficientDependencyComputationIncreasingFactorPercent", 130) / 100.;

					return this;
				}

				CAbstractVariableMappingsCompositionComputator* CAbstractVariableMappingsCompositionComputator::computeVariableMappingsComposition(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) {
					return this;
				}





				bool CAbstractVariableMappingsCompositionComputator::queueVariableItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem) {
					if (!varCompItem->isComputationQueued()) {
						varCompItem->setComputationQueued(true);
						buildingVarItem->addComputeVariableMappingItem(varCompItem);
						return true;
					}
					return false;
				}

				bool CAbstractVariableMappingsCompositionComputator::rescheduleVariableCompositionItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItem* depSchedulingCompItem, cint64 additionalRequiredMappingCount) {
					if (!depSchedulingCompItem->isVariableMappingsComputed()) {
						if (additionalRequiredMappingCount != -1) {
							depSchedulingCompItem->setVariableMappingsComputationRequirement(qMax(depSchedulingCompItem->getVariableMappingsComputationRequirement(), additionalRequiredMappingCount));
						} else {
							depSchedulingCompItem->setVariableMappingsComputationRequirement(-1);
						}
						depSchedulingCompItem->addComputationUpdateItem(varCompItem);
						varCompItem->incDependencyUpdatingCount();
						queueVariableItemComputation(buildingVarItem, depSchedulingCompItem);
						return true;
					}
					return false;
				}


				COptimizedComplexVariableIndividualBindings* CAbstractVariableMappingsCompositionComputator::createBindingsForVariableCompositionItems(cint64 bindingSize, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CMemoryAllocationManager* memMan) {
					if (!memMan) {
						return new COptimizedComplexVariableIndividualBindings(bindingSize);
					} else {
						TIndividualInstanceItemDataBinding* bindingArray = CObjectAllocator<TIndividualInstanceItemDataBinding>::allocateAndConstructArray(memMan, bindingSize);
						COptimizedComplexVariableIndividualBindings* bindings = CObjectAllocator<COptimizedComplexVariableIndividualBindings>::allocateAndConstruct(memMan);
						bindings->initIndividualBindings(bindingSize, bindingArray);
						return bindings;
					}
				}



				COptimizedComplexVariableIndividualBindingsCardinality* CAbstractVariableMappingsCompositionComputator::createBindingsCardinalitesForVariableCompositionItems(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CMemoryAllocationManager* memMan) {
					if (!memMan) {
						return new COptimizedComplexVariableIndividualBindingsCardinality();
					} else {
						return CObjectAllocator<COptimizedComplexVariableIndividualBindingsCardinality>::allocateAndConstruct(memMan);
					}
				}



				COptimizedComplexVariableIndividualBindingsCardinalityLinker* CAbstractVariableMappingsCompositionComputator::createBindingsLinkerForVariableCompositionItems(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinality, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CMemoryAllocationManager* memMan) {
					if (!memMan) {
						return new COptimizedComplexVariableIndividualBindingsCardinalityLinker(bindings, cardinality);
					} else {
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = CObjectAllocator<COptimizedComplexVariableIndividualBindingsCardinalityLinker>::allocateAndConstruct(memMan);
						linker->initBindingsCardinalityLinker(bindings, cardinality);
						return linker;
					}
				}





				bool CAbstractVariableMappingsCompositionComputator::finishVariableCompositionItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, bool allMappingsComputed) {
					if (allMappingsComputed) {
						varCompItem->setVariableMappingsComputed(true);
					}
					if (varCompItem->getVariableMapping()->getBindingCount() <= 0) {
						buildingVarItem->setSatisfiability(false);
					}
					QList<COptimizedComplexVariableCompositionItem*>* updateList = varCompItem->getComputationUpdateItemList();
					for (COptimizedComplexVariableCompositionItem* updateItem : *updateList) {
						updateItem->decDependencyUpdatingCount();
						if (updateItem->getDependencyUpdatingCount() == 0) {
							queueVariableItemComputation(buildingVarItem, updateItem);
						}
					}
					updateList->clear();
					return true;
				}

				void CAbstractVariableMappingsCompositionComputator::configureDependentComputationIncreaseFactor(bool dependentItemRescheduled, bool mappingProcessed, COptimizedComplexVariableCompositionItem* compVarItem) {
					if (dependentItemRescheduled && mappingProcessed && compVarItem->requiresMoreVariableMappingsComputation() && !compVarItem->isVariableMappingsComputed()) {
						compVarItem->increaseRepeatedRequirementInsufficientDependencyComputationIncreaseFactor(mConfMappingsRepeatedlyInsufficientDependencyComputationIncreasingFactor);
					} else if (!compVarItem->requiresMoreVariableMappingsComputation()) {
						compVarItem->setRepeatedRequirementInsufficientDependencyComputationIncreaseFactor(1.);
					}
				}


			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
