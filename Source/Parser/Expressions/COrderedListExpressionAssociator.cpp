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

#include "COrderedListExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			COrderedListExpressionAssociator::COrderedListExpressionAssociator(CBuildExpression *expression1, CBuildExpression *expression2, CBuildExpression *expression3) 
					: CListExpressionAssociator(expression1,expression2,expression3) {
			}


			COrderedListExpressionAssociator::COrderedListExpressionAssociator(const CEXPRESSIONLIST<CBuildExpression*>& expressionList) : CListExpressionAssociator(*(CEXPRESSIONLIST<CBuildExpression*>*)&expressionList) {
			}


			COrderedListExpressionAssociator::~COrderedListExpressionAssociator() {
			}


			cint64 COrderedListExpressionAssociator::getStructuralHashValue() {
				cint64 hashValue = 0;
				FOREACHIT (CBuildExpression* expression, mExpressionList) {
					hashValue += CExpressionHashMixer::getHashValueMixed(hashValue,cint64(expression));
				}
				return hashValue;
			}

			bool COrderedListExpressionAssociator::compareStructuralEquivalence(const CListExpressionAssociator& listExpressionAssociator) {
				const CEXPRESSIONLIST<CBuildExpression*>* otherListExp = listExpressionAssociator.getConstExpressionList();
				if (mExpressionList.count() != otherListExp->count()) {
					return false;
				}
				for (CEXPRESSIONLIST<CBuildExpression*>::const_iterator it1 = mExpressionList.constBegin(), it1End = mExpressionList.constEnd(),
					it2 = otherListExp->constBegin(), it2End = otherListExp->constEnd(); it1 != it1End && it2 != it2End; ++it1,++it2) {
						if (*it1 != *it2) {
							return false;
						}
				}
				return true;
			}

			bool COrderedListExpressionAssociator::compareStructuralEquivalence(const COrderedListExpressionAssociator& orderedListExpressionAssociator) {
				const CEXPRESSIONLIST<CBuildExpression*>* otherListExp = orderedListExpressionAssociator.getConstExpressionList();
				if (mExpressionList.count() != otherListExp->count()) {
					return false;
				}
				for (CEXPRESSIONLIST<CBuildExpression*>::const_iterator it1 = mExpressionList.constBegin(), it1End = mExpressionList.constEnd(),
					it2 = otherListExp->constBegin(), it2End = otherListExp->constEnd(); it1 != it1End && it2 != it2End; ++it1,++it2) {
						if (*it1 != *it2) {
							return false;
						}
				}
				return true;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
