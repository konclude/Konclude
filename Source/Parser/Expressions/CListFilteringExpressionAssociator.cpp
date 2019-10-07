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

#include "CListFilteringExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CListFilteringExpressionAssociator::CListFilteringExpressionAssociator(CFilteringTermExpression *expression1, CFilteringTermExpression *expression2, CFilteringTermExpression *expression3) 
					: CListExpressionAssociator(expression1,expression2,expression3) {
			}


			CListFilteringExpressionAssociator::CListFilteringExpressionAssociator(const CEXPRESSIONLIST<CFilteringTermExpression*>& expressionList) : CListExpressionAssociator(*(CEXPRESSIONLIST<CBuildExpression*>*)&expressionList) {
			}


			CListFilteringExpressionAssociator::~CListFilteringExpressionAssociator() {
			}

			CEXPRESSIONLIST<CFilteringTermExpression*>* CListFilteringExpressionAssociator::getFilteringExpressionList() {
				// bad cast to avoid lot of reimplementation 
				return (CEXPRESSIONLIST<CFilteringTermExpression*>*)&mExpressionList;
			}

			bool CListFilteringExpressionAssociator::compareStructuralEquivalence(const CListFilteringExpressionAssociator& listClassTermExpressionAssociator) {
				return CListExpressionAssociator::compareStructuralEquivalence(listClassTermExpressionAssociator);
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
