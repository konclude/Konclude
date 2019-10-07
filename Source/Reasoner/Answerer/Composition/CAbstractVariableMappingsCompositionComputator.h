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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CABSTRACTVARIABLEMAPPINGSCOMPOSITIONCOMPUTATOR_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CABSTRACTVARIABLEMAPPINGSCOMPOSITIONCOMPUTATOR_H

// Libraries includes


// Namespace includes
#include "CompositionSettings.h"


// Other includes
#include "Reasoner/Answerer/CAnswererContext.h"
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualMappings.h"
#include "Reasoner/Answerer/COptimizedComplexVariableJoiningItem.h"
#include "Reasoner/Answerer/COptimizedComplexBuildingVariableCompositionsItem.h"
#include "Reasoner/Answerer/COptimizedComplexConceptOntologyAnsweringItem.h"



// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {


				/*! 
				 *
				 *		\class		CAbstractVariableMappingsCompositionComputator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CAbstractVariableMappingsCompositionComputator {
					// public methods
					public:
						//! Constructor
						CAbstractVariableMappingsCompositionComputator();

						virtual CAbstractVariableMappingsCompositionComputator* configureComputator(COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, CAnswererContext* answererContext);

						virtual CAbstractVariableMappingsCompositionComputator* computeVariableMappingsComposition(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing);


					// protected methods
					protected:

						bool queueVariableItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem);
						bool rescheduleVariableCompositionItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItem* depSchedulingCompItem, cint64 additionalRequiredMappingCount);

						COptimizedComplexVariableIndividualBindings* createBindingsForVariableCompositionItems(cint64 bindingSize, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CMemoryAllocationManager* memMan = nullptr);
						COptimizedComplexVariableIndividualBindingsCardinality* createBindingsCardinalitesForVariableCompositionItems(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CMemoryAllocationManager* memMan = nullptr);
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* createBindingsLinkerForVariableCompositionItems(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinality, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CMemoryAllocationManager* memMan = nullptr);

						bool finishVariableCompositionItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, bool allMappingsComputed);
						void configureDependentComputationIncreaseFactor(bool dependentItemRescheduled, bool mappingProcessed, COptimizedComplexVariableCompositionItem* compVarItem);


					// protected variables
					protected:
						COptimizedComplexConceptOntologyAnsweringItem* mOntoAnsweringItem;

						class CInstanceBindingIndividualCountingVisitor : public CSameRealizationIndividualVisitor {
						public:
							cint64 individualCount = 0;

							bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
								individualCount++;;
								return true;
							}
						};


						double mConfMappingsRepeatedlyInsufficientDependencyComputationIncreasingFactor;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CABSTRACTVARIABLEMAPPINGSCOMPOSITIONCOMPUTATOR_H
