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

#include "CObjectPropertyChainExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectPropertyChainExpression::CObjectPropertyChainExpression(CObjectPropertyTermExpression *objectPropertyExpression1, CObjectPropertyTermExpression *objectPropertyExpression2, CObjectPropertyTermExpression *objectPropertyExpression3) 
				: COrderedListObjectPropertyTermExpressionAssociator(objectPropertyExpression1,objectPropertyExpression2,objectPropertyExpression3) {
			}

			CObjectPropertyChainExpression::CObjectPropertyChainExpression(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& objectPropertyExpressionList) 
				: COrderedListObjectPropertyTermExpressionAssociator(objectPropertyExpressionList) {
			}

			CObjectPropertyChainExpression::~CObjectPropertyChainExpression() {
			}

			CEXPRESSIONLIST<CObjectPropertyTermExpression*>* CObjectPropertyChainExpression::getChainObjectPropertyTermExpressionList() {
				return getOrderedObjectPropertyTermExpressionList();
			}


			CBuildExpression::ExpressionType CObjectPropertyChainExpression::getType() {
				return BETOBJECTPROPERTYCHAIN;
			}

			cint64 CObjectPropertyChainExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),COrderedListObjectPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CObjectPropertyChainExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectPropertyChainExpression* compExpCast = dynamic_cast<CObjectPropertyChainExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return COrderedListObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CObjectPropertyChainExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					COrderedListObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
