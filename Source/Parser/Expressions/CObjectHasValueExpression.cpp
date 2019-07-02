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

#include "CObjectHasValueExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectHasValueExpression::CObjectHasValueExpression(CObjectPropertyTermExpression *expression1, CIndividualTermExpression *expression2) 
				: CObjectPropertyTermExpressionAssociator(expression1), CIndividualTermExpressionAssociator(expression2) {
			}


			CObjectHasValueExpression::~CObjectHasValueExpression() {
			}



			CBuildExpression::ExpressionType CObjectHasValueExpression::getType() {
				return BETOBJECTHASVALUE;
			}

			cint64 CObjectHasValueExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue(),CIndividualTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CObjectHasValueExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectHasValueExpression* compExpCast = dynamic_cast<CObjectHasValueExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CObjectHasValueExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
