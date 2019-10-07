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

#include "CDataAllValuesFromExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CDataAllValuesFromExpression::CDataAllValuesFromExpression(CDataPropertyTermExpression *expression1, CDataRangeTermExpression *expression2) 
				: CDataPropertyTermExpressionAssociator(expression1), CDataRangeTermExpressionAssociator(expression2) {
			}


			CDataAllValuesFromExpression::~CDataAllValuesFromExpression() {
			}



			CBuildExpression::ExpressionType CDataAllValuesFromExpression::getType() {
				return BETDATAALLVALUEFROM;
			}

			cint64 CDataAllValuesFromExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CDataPropertyTermExpressionAssociator::getStructuralHashValue(),CDataRangeTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CDataAllValuesFromExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CDataAllValuesFromExpression* compExpCast = dynamic_cast<CDataAllValuesFromExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CDataRangeTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CDataAllValuesFromExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CDataRangeTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
