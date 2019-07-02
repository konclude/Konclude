/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CListClassVariableTermExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CListClassVariableTermExpressionAssociator::CListClassVariableTermExpressionAssociator(CObjectIndividualVariableExpression *expression1, CObjectIndividualVariableExpression *expression2, CObjectIndividualVariableExpression *expression3) 
					: CListExpressionAssociator(expression1,expression2,expression3) {
			}


			CListClassVariableTermExpressionAssociator::CListClassVariableTermExpressionAssociator(const CEXPRESSIONLIST<CObjectIndividualVariableExpression*>& expressionList) : CListExpressionAssociator(*(CEXPRESSIONLIST<CBuildExpression*>*)&expressionList) {
			}


			CListClassVariableTermExpressionAssociator::~CListClassVariableTermExpressionAssociator() {
			}

			CEXPRESSIONLIST<CObjectIndividualVariableExpression*>* CListClassVariableTermExpressionAssociator::getClassVariableTermExpressionList() {
				// bad cast to avoid lot of reimplementation 
				return (CEXPRESSIONLIST<CObjectIndividualVariableExpression*>*)&mExpressionList;
			}

			bool CListClassVariableTermExpressionAssociator::compareStructuralEquivalence(const CListClassVariableTermExpressionAssociator& listClassTermExpressionAssociator) {
				return CListExpressionAssociator::compareStructuralEquivalence(listClassTermExpressionAssociator);
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
