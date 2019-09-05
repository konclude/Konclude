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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CABSTRACTVARIABLEMAPPINGSCOMPOSITIONJOINCOMPUTATOR_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CABSTRACTVARIABLEMAPPINGSCOMPOSITIONJOINCOMPUTATOR_H

// Libraries includes


// Namespace includes
#include "CompositionSettings.h"
#include "CAbstractVariableMappingsCompositionComputator.h"


// Other includes
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualMappings.h"
#include "Reasoner/Answerer/COptimizedComplexVariableJoiningItem.h"
#include "Reasoner/Answerer/COptimizedComplexBuildingVariableCompositionsItem.h"
#include "Reasoner/Answerer/CAnswererContext.h"



// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {


				/*! 
				 *
				 *		\class		CAbstractVariableMappingsCompositionJoinComputator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CAbstractVariableMappingsCompositionJoinComputator : public CAbstractVariableMappingsCompositionComputator, public CLogIdentifier {
					// public methods
					public:
						//! Constructor
						CAbstractVariableMappingsCompositionJoinComputator();


						virtual CAbstractVariableMappingsCompositionComputator* configureComputator(COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, CAnswererContext* answererContext);
						virtual CAbstractVariableMappingsCompositionJoinComputator* computeVariableMappingsComposition(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing);


					// protected methods
					protected:

						virtual bool computeVariableMappingsJoinComposition(COptimizedComplexVariableJoiningItem* joiningItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) = 0;
						bool isSamplingLeftInsertionDecision(COptimizedComplexVariableJoiningItem* joiningItem);


					// protected variables
					protected:

						cint64 mConfMappingsComputationUnlimitedInterpretationSize;
						cint64 mConfSamplingBasedJoinMappingSize;
						bool mConfExtendedLogging;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CABSTRACTVARIABLEMAPPINGSCOMPOSITIONJOINCOMPUTATOR_H
