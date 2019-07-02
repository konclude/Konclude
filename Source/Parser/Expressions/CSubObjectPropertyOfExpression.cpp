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

#include "CSubObjectPropertyOfExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CSubObjectPropertyOfExpression::CSubObjectPropertyOfExpression(CObjectPropertyTermExpression *subObjectPropertyExpression1, CObjectPropertyTermExpression *superbObjectPropertyExpression2) 
				: CObjectPropertyTermExpressionAssociator(superbObjectPropertyExpression2), COrderedListObjectPropertyTermExpressionAssociator(subObjectPropertyExpression1) {
			}

			CSubObjectPropertyOfExpression::CSubObjectPropertyOfExpression(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& subObjectPropertyExpressionList, CObjectPropertyTermExpression *superbObjectPropertyExpression) 
				: CObjectPropertyTermExpressionAssociator(superbObjectPropertyExpression), COrderedListObjectPropertyTermExpressionAssociator(subObjectPropertyExpressionList) {
			}

			CSubObjectPropertyOfExpression::~CSubObjectPropertyOfExpression() {
			}

			CEXPRESSIONLIST<CObjectPropertyTermExpression*>* CSubObjectPropertyOfExpression::getSubObjectPropertyTermExpressionList() {
				return getOrderedObjectPropertyTermExpressionList();
			}

			CObjectPropertyTermExpression* CSubObjectPropertyOfExpression::getSuperObjectPropertyTermExpression() {
				return getObjectPropertyTermExpression();
			}


			CBuildExpression::ExpressionType CSubObjectPropertyOfExpression::getType() {
				return BETSUBOBJECTPROPERTYOF;
			}

			cint64 CSubObjectPropertyOfExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue(),COrderedListObjectPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CSubObjectPropertyOfExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CSubObjectPropertyOfExpression* compExpCast = dynamic_cast<CSubObjectPropertyOfExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && COrderedListObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CSubObjectPropertyOfExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					COrderedListObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
