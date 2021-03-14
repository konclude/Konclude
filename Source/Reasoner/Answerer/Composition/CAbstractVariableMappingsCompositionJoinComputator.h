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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CABSTRACTVARIABLEMAPPINGSCOMPOSITIONJOINCOMPUTATOR_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CABSTRACTVARIABLEMAPPINGSCOMPOSITIONJOINCOMPUTATOR_H

// Libraries includes


// Namespace includes
#include "CompositionSettings.h"
#include "CAbstractVariableMappingsCompositionComputator.h"
#include <math.h> 

// Other includes
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualMappings.h"
#include "Reasoner/Answerer/COptimizedComplexVariableJoiningItem.h"
#include "Reasoner/Answerer/COptimizedComplexBuildingVariableCompositionsItem.h"
#include "Reasoner/Answerer/CAnswererContext.h"
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualMappingsMultiHash.h"
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualMappingsHash.h"



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


						virtual CAbstractVariableMappingsCompositionComputator* configureComputator(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, CAbstractVariableMappingsCompositionItemRequirementProcessor* reqProcessor, CAnswererContext* answererContext);
						virtual CAbstractVariableMappingsCompositionJoinComputator* computeVariableMappingsComposition(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing);


					// protected methods
					protected:

						virtual bool computeVariableMappingsJoinComposition(COptimizedComplexVariableJoiningItem* joiningItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) = 0;
						bool isSamplingLeftInsertionDecision(COptimizedComplexVariableJoiningItem* joiningItem);

						bool requiresMoreVariableMappingComputation(COptimizedComplexVariableJoiningItem* joiningItem, bool sampling);
						cint64 getUpdatedExpectedVariableMappingsCount(COptimizedComplexVariableJoiningItem* joiningItem);


						bool splitComputationReset(COptimizedComplexVariableJoiningItem* joiningItem, bool resetJoiningHash);
						bool initSplitComputation(COptimizedComplexVariableJoiningItem* joiningItem);
						bool nextSplitComputation(COptimizedComplexVariableJoiningItem* joiningItem);



					// protected variables
					protected:
						QString mComputerName;

						cint64 mConfMappingsComputationUnlimitedInterpretationSize;
						cint64 mConfSamplingBasedJoinMappingSize;
						bool mConfExtendedLogging;


						bool mConfAllowSplitModeActivation = true;
						double mConfMappingCountExpectedFactorSplitModeActivation = 10;
						double mConfMappingCountSplitModeActivation = 5000000;
						double mConfExpectedInputMappingCountDirectSplitModeActivation = 5000000;

						double mConfMappingSplitCount = 10000000;
						double mConfMappingComputationMinSplitCount = 10;
						double mConfMappingComputationMaxSplitCount = 100;


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
