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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CSEQUENTIALVARIABLEMAPPINGSCOMPOSITIONPROPAGATIONCOMPUTATOR_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CSEQUENTIALVARIABLEMAPPINGSCOMPOSITIONPROPAGATIONCOMPUTATOR_H

// Libraries includes


// Namespace includes
#include "CompositionSettings.h"
#include "CAbstractVariableMappingsCompositionPropagationComputator.h"


// Other includes
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualMappings.h"
#include "Reasoner/Answerer/COptimizedComplexVariableJoiningItem.h"
#include "Reasoner/Answerer/COptimizedComplexBuildingIndividualVariableCompositionsItem.h"
#include "Reasoner/Answerer/CAnswererContext.h"
#include "Reasoner/Answerer/COptimizedComplexVariableRolePropagationJoiningItem.h"
#include "Reasoner/Answerer/COptimizedComplexVariableRolePropagationReplacementItem.h"



#include "Reasoner/Ontology/COntologyProcessingIteratorRealizationRequirement.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {

			namespace Composition {


				/*! 
				 *
				 *		\class		CSequentialVariableMappingsCompositionPropagationComputator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSequentialVariableMappingsCompositionPropagationComputator : public CAbstractVariableMappingsCompositionPropagationComputator {
					// public methods
					public:
						//! Constructor
						CSequentialVariableMappingsCompositionPropagationComputator();


						virtual CAbstractVariableMappingsCompositionComputator* configureComputator(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, CAbstractVariableMappingsCompositionItemRequirementProcessor* reqProcessor, CAnswererContext* answererContext);


					// protected methods
					protected:

						virtual bool computeVariableMappingsPropagationComposition(COptimizedComplexVariableRolePropagationAbstractItem* propagationItem, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing);


					// protected variables
					protected:


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CSEQUENTIALVARIABLEMAPPINGSCOMPOSITIONPROPAGATIONCOMPUTATOR_H
