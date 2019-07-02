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

#include "CObjectHasSelfExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectHasSelfExpression::CObjectHasSelfExpression(CObjectPropertyTermExpression *expression1) 
				: CObjectPropertyTermExpressionAssociator(expression1) {
			}


			CObjectHasSelfExpression::~CObjectHasSelfExpression() {
			}



			CBuildExpression::ExpressionType CObjectHasSelfExpression::getType() {
				return BETOBJECTHASSELF;
			}

			cint64 CObjectHasSelfExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CObjectHasSelfExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectHasSelfExpression* compExpCast = dynamic_cast<CObjectHasSelfExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CObjectHasSelfExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
