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

#include "COrderedListObjectPropertyTermExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			COrderedListObjectPropertyTermExpressionAssociator::COrderedListObjectPropertyTermExpressionAssociator(CObjectPropertyTermExpression *expression1, CObjectPropertyTermExpression *expression2, CObjectPropertyTermExpression *expression3) 
					: COrderedListExpressionAssociator(expression1,expression2,expression3) {
			}


			COrderedListObjectPropertyTermExpressionAssociator::COrderedListObjectPropertyTermExpressionAssociator(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressionList) : COrderedListExpressionAssociator(*(CEXPRESSIONLIST<CBuildExpression*>*)&expressionList) {
			}


			COrderedListObjectPropertyTermExpressionAssociator::~COrderedListObjectPropertyTermExpressionAssociator() {
			}

			CEXPRESSIONLIST<CObjectPropertyTermExpression*>* COrderedListObjectPropertyTermExpressionAssociator::getOrderedObjectPropertyTermExpressionList() {
				// bad cast to avoid lot of reimplementation 
				return (CEXPRESSIONLIST<CObjectPropertyTermExpression*>*)&mExpressionList;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
