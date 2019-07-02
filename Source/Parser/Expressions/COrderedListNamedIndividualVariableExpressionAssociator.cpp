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

#include "COrderedListNamedIndividualVariableExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			COrderedListNamedIndividualVariableExpressionAssociator::COrderedListNamedIndividualVariableExpressionAssociator(CNamedIndividualVariableExpression *expression1, CNamedIndividualVariableExpression *expression2, CNamedIndividualVariableExpression *expression3) 
					: COrderedListExpressionAssociator(expression1,expression2,expression3) {
			}


			COrderedListNamedIndividualVariableExpressionAssociator::COrderedListNamedIndividualVariableExpressionAssociator(const CEXPRESSIONLIST<CNamedIndividualVariableExpression*>& expressionList) : COrderedListExpressionAssociator(*(CEXPRESSIONLIST<CBuildExpression*>*)&expressionList) {
			}


			COrderedListNamedIndividualVariableExpressionAssociator::~COrderedListNamedIndividualVariableExpressionAssociator() {
			}

			CEXPRESSIONLIST<CNamedIndividualVariableExpression*>* COrderedListNamedIndividualVariableExpressionAssociator::getOrderedNamedIndividualVariableExpressionList() {
				// bad cast to avoid lot of reimplementation 
				return (CEXPRESSIONLIST<CNamedIndividualVariableExpression*>*)&mExpressionList;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
