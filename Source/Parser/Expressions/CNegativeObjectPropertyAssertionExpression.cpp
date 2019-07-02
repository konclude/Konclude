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

#include "CNegativeObjectPropertyAssertionExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CNegativeObjectPropertyAssertionExpression::CNegativeObjectPropertyAssertionExpression(CObjectPropertyTermExpression *propertyExpression1, CIndividualTermExpression *sourceExpression2, CIndividualTermExpression *targetExpression3) 
					: CObjectPropertyTermExpressionAssociator(propertyExpression1),COrderedPairIndividualTermExpressionAssociator(sourceExpression2,targetExpression3) {
			}


			CNegativeObjectPropertyAssertionExpression::~CNegativeObjectPropertyAssertionExpression() {
			}


			CBuildExpression::ExpressionType CNegativeObjectPropertyAssertionExpression::getType() {
				return BETNEGATIVEOBJECTPROPERTYASSERTION;
			}

			cint64 CNegativeObjectPropertyAssertionExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue(),COrderedPairIndividualTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CNegativeObjectPropertyAssertionExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CNegativeObjectPropertyAssertionExpression* compExpCast = dynamic_cast<CNegativeObjectPropertyAssertionExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && COrderedPairIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CNegativeObjectPropertyAssertionExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
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
