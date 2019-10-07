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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSTRACTABSORPTIONBASEDQUERYPARTHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSTRACTABSORPTIONBASEDQUERYPARTHANDLER_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingNeighbouringData.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"
#include "COptimizedComplexConceptOntologyAnsweringItem.h"


// Other includes
#include "Parser/Expressions/CExpressionVariable.h"
#include "Parser/Expressions/CObjectPropertyTermExpression.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* queryPart, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem);

					virtual COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler* readConfig(CCalculationConfigurationExtension* config);

				// protected methods
				protected:

					void addConceptOperand(CConcept* concept, CConcept* operandConcept, bool operandNegation);
					CRole* getInverseRole(CRole* role);


					bool checkHasTopObjectPropertyPropagation();
					bool checkHasNewTopRolePropagation(CConcept* concept, bool conceptNegation);



				// protected variables
				protected:
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* mQueryPart;
					COptimizedComplexConceptOntologyAnsweringItem* mOntoAnsweringItem;

					CMemoryAllocationManager* mMemMan;
					CConceptVector* mConceptVec;
					CTBox* mTBox;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSTRACTABSORPTIONBASEDQUERYPARTHANDLER_H
