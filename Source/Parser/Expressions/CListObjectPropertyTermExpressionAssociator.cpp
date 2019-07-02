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

#include "CListObjectPropertyTermExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CListObjectPropertyTermExpressionAssociator::CListObjectPropertyTermExpressionAssociator(CObjectPropertyTermExpression *expression1, CObjectPropertyTermExpression *expression2, CObjectPropertyTermExpression *expression3) 
					: CListExpressionAssociator(expression1,expression2,expression3) {
			}


			CListObjectPropertyTermExpressionAssociator::CListObjectPropertyTermExpressionAssociator(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressionList) : CListExpressionAssociator(*(CEXPRESSIONLIST<CBuildExpression*>*)&expressionList) {
			}


			CListObjectPropertyTermExpressionAssociator::~CListObjectPropertyTermExpressionAssociator() {
			}

			CEXPRESSIONLIST<CObjectPropertyTermExpression*>* CListObjectPropertyTermExpressionAssociator::getObjectPropertyTermExpressionList() {
				// bad cast to avoid lot of reimplementation 
				return (CEXPRESSIONLIST<CObjectPropertyTermExpression*>*)&mExpressionList;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
