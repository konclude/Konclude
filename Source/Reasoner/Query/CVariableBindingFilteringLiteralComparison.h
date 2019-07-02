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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGFILTERINGLITERALCOMPARISON_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGFILTERINGLITERALCOMPARISON_H

// Libraries includes


// Namespace includes
#include "QuerySettings.h"
#include "CVariableBindingFiltering.h"
#include "CVariableBindingFilteringIndexArrayAnswerMapping.h"
#include "CVariableBindingsListAnswerResult.h"
#include "CVariableBindingStringDataResult.h"


// Other includes
#include "Parser/Expressions/CExpressionVariable.h"
#include "Parser/Expressions/CDataLiteralExpression.h"

#include "Reasoner/Ontology/CDataLiteral.h"
#include "Reasoner/Ontology/CDataLiteralCompareValue.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CVariableBindingFilteringLiteralComparison
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingFilteringLiteralComparison : public CVariableBindingFiltering {
				// public methods
				public:

					enum COMPARING_TYPE {  LESS, LESS_EQUAL, EQUAL, UNEQUAL, GREATER_EQUAL, GREATER };

					//! Constructor
					CVariableBindingFilteringLiteralComparison(COMPARING_TYPE filteringType, CExpressionVariable* variableExp, CDataLiteralExpression* dataLiteralExp);

					COMPARING_TYPE getFilteringType();

					CExpressionVariable* getVariableExpression();
					CDataLiteralExpression* getDataLiteralExpression();

					CDataLiteral* getDataLiteral();
					CVariableBindingFilteringLiteralComparison* setDataLiteral(CDataLiteral* dataLiteral);


					virtual CVariableBindingFilteringAnswerMapping* createFitleringAnswerMapping(QHash<CExpressionVariable*, cint64> varPosHash);
					virtual bool isFiltered(CVariableBindingFilteringAnswerMapping* answeringMapping, CVariableBindingsAnswerResult* bindingsAnswer);


				// protected methods
				protected:

				// protected variables
				protected:
					COMPARING_TYPE mComparingType;
					CExpressionVariable* mVariableExp;
					CDataLiteralExpression* mDataLiteralExp;
					CDataLiteral* mDataLiteral;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGFILTERINGLITERALCOMPARISON_H
