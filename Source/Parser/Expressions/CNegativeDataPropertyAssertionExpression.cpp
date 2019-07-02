/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CNegativeDataPropertyAssertionExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CNegativeDataPropertyAssertionExpression::CNegativeDataPropertyAssertionExpression(CDataPropertyTermExpression *propertyExpression1, CIndividualTermExpression *sourceExpression2, CDataLiteralExpression *targetExpression3) 
					: CDataPropertyTermExpressionAssociator(propertyExpression1), CIndividualTermExpressionAssociator(sourceExpression2), CDataLiteralExpressionAssociator(targetExpression3) {
			}


			CNegativeDataPropertyAssertionExpression::~CNegativeDataPropertyAssertionExpression() {
			}


			CBuildExpression::ExpressionType CNegativeDataPropertyAssertionExpression::getType() {
				return BETNEGATIVEDATAPROPERTYASSERTION;
			}

			cint64 CNegativeDataPropertyAssertionExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CDataPropertyTermExpressionAssociator::getStructuralHashValue(),CIndividualTermExpressionAssociator::getStructuralHashValue(),CDataLiteralExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CNegativeDataPropertyAssertionExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CNegativeDataPropertyAssertionExpression* compExpCast = dynamic_cast<CNegativeDataPropertyAssertionExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CDataLiteralExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CNegativeDataPropertyAssertionExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CDataLiteralExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
