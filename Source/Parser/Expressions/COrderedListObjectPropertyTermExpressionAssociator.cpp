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

#include "COrderedListObjectPropertyTermExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			COrderedListObjectPropertyTermExpressionAssociator::COrderedListObjectPropertyTermExpressionAssociator(CObjectPropertyTermExpression *expression1, CObjectPropertyTermExpression *expression2, CObjectPropertyTermExpression *expression3) 
					: CListExpressionAssociator(expression1,expression2,expression3) {
			}


			COrderedListObjectPropertyTermExpressionAssociator::COrderedListObjectPropertyTermExpressionAssociator(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressionList) : CListExpressionAssociator(*(CEXPRESSIONLIST<CBuildExpression*>*)&expressionList) {
			}


			COrderedListObjectPropertyTermExpressionAssociator::~COrderedListObjectPropertyTermExpressionAssociator() {
			}

			CEXPRESSIONLIST<CObjectPropertyTermExpression*>* COrderedListObjectPropertyTermExpressionAssociator::getOrderedObjectPropertyTermExpressionList() {
				// bad cast to avoid lot of reimplementation 
				return (CEXPRESSIONLIST<CObjectPropertyTermExpression*>*)&mExpressionList;
			}

			cint64 COrderedListObjectPropertyTermExpressionAssociator::getStructuralHashValue() {
				cint64 hashValue = 0;
				FOREACHIT (CBuildExpression* expression, mExpressionList) {
					hashValue += CExpressionHashMixer::getHashValueMixed(hashValue,cint64(expression));
				}
				return hashValue;
			}

			bool COrderedListObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(const CListExpressionAssociator& listExpressionAssociator) {
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

			bool COrderedListObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(const COrderedListObjectPropertyTermExpressionAssociator& orderedListExpressionAssociator) {
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
