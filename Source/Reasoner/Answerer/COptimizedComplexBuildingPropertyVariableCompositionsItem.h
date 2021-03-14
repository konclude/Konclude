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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGPROPERTYVARIABLECOMPOSITIONSITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGPROPERTYVARIABLECOMPOSITIONSITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexBuildingVariableCompositionsItem.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"
#include "Reasoner/Ontology/OntologySettings.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Parser/Expressions/CPropertyVariableExpression.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexBuildingPropertyVariableCompositionsItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexBuildingPropertyVariableCompositionsItem : public COptimizedComplexBuildingVariableCompositionsItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexBuildingPropertyVariableCompositionsItem(CComplexQueryExpressionProcessingData* queryProcessingData);
					virtual ~COptimizedComplexBuildingPropertyVariableCompositionsItem();



					virtual bool isWaitingComputation();

					COptimizedComplexBuildingPropertyVariableCompositionsItem* addVariableComplexSuperRole(CPropertyVariableExpression* varExp, CRole* role);
					COptimizedComplexBuildingPropertyVariableCompositionsItem* addVariableComplexSubRole(CPropertyVariableExpression* varExp, CRole* role);

					QSet<CPropertyVariableExpression*>* getRemainingVariableExpressionSet();
					QSet<CPropertyVariableExpression*>* getAllVariableSet();

					QList<CRole*> getVariableSuperRoles(CPropertyVariableExpression* varExp);
					QList<CRole*> getVariableSubRoles(CPropertyVariableExpression* varExp);

					bool isInitializedBaseRoleVariableItems();
					COptimizedComplexBuildingPropertyVariableCompositionsItem* setInitializedBaseRoleVariableItems(bool initialized);



					QSet<QPair<CExpressionVariable*, CExpressionVariable*>>* getVariableSubSuperPropagationSet();
					COptimizedComplexBuildingPropertyVariableCompositionsItem* addVariableSubSuperPropagation(CExpressionVariable* subVar, CExpressionVariable* superVar);
					COptimizedComplexBuildingPropertyVariableCompositionsItem* removeVariableSubSuperPropagation(CExpressionVariable* subVar, CExpressionVariable* superVar);
					bool hasVariableSubSuperPropagation(CExpressionVariable* subVar, CExpressionVariable* superVar);

					QSet<QPair<CExpressionVariable*, CExpressionVariable*>> getVariableRemainingSubSuperPropagationSet(CExpressionVariable* varExp);
					bool isRemovingPropagationDisconnecting(CExpressionVariable* varExp, CExpressionVariable* subVar, CExpressionVariable* superVar);

					COptimizedComplexBuildingPropertyVariableCompositionsItem* setVariableExpressionUnhandled(CPropertyVariableExpression* varExp);
					COptimizedComplexBuildingPropertyVariableCompositionsItem* setVariableExpressionHandled(CPropertyVariableExpression* varExp);


				// protected methods
				protected:

				// protected variables
				protected:
					QHash<CPropertyVariableExpression*, CRole*> mVarSuperRoleHash;
					QHash<CPropertyVariableExpression*, CRole*> mVarSubRoleHash;


					bool mInitializedBaseRoleVariableItems;

					QSet<CPropertyVariableExpression*> mRemainingVarExpSet;
					QSet<CPropertyVariableExpression*> mAllVarExpSet;


					QSet<QPair<CExpressionVariable*, CExpressionVariable*>> mVariableSubSuperPropagationSet;
					QHash<CExpressionVariable*, CExpressionVariable*> mVariableSubPropagationHash;
					QHash<CExpressionVariable*, CExpressionVariable*> mVariableSuperPropagationHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGPROPERTYVARIABLECOMPOSITIONSITEM_H
