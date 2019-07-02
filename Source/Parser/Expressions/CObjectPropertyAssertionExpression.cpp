/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CObjectPropertyAssertionExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectPropertyAssertionExpression::CObjectPropertyAssertionExpression(CObjectPropertyTermExpression *propertyExpression1, CIndividualTermExpression *sourceExpression2, CIndividualTermExpression *targetExpression3) 
					: CObjectPropertyTermExpressionAssociator(propertyExpression1),COrderedPairIndividualTermExpressionAssociator(sourceExpression2,targetExpression3) {
			}


			CObjectPropertyAssertionExpression::~CObjectPropertyAssertionExpression() {
			}


			CBuildExpression::ExpressionType CObjectPropertyAssertionExpression::getType() {
				return BETOBJECTPROPERTYASSERTION;
			}

			cint64 CObjectPropertyAssertionExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue(),COrderedPairIndividualTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CObjectPropertyAssertionExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectPropertyAssertionExpression* compExpCast = dynamic_cast<CObjectPropertyAssertionExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && COrderedPairIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CObjectPropertyAssertionExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					COrderedPairIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
