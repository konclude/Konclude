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

#include "CAsymmetricObjectPropertyExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CAsymmetricObjectPropertyExpression::CAsymmetricObjectPropertyExpression(CObjectPropertyTermExpression *expression)
					: CObjectPropertyTermExpressionAssociator(expression) {
			}


			CAsymmetricObjectPropertyExpression::~CAsymmetricObjectPropertyExpression() {
			}



			CBuildExpression::ExpressionType CAsymmetricObjectPropertyExpression::getType() {
				return BETASYMMETRICPROPERTY;
			}

			CObjectPropertyTermExpression *CAsymmetricObjectPropertyExpression::getAsymmetricObjectPropertyExpression() {
				return getObjectPropertyTermExpression();
			}


			cint64 CAsymmetricObjectPropertyExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CAsymmetricObjectPropertyExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CAsymmetricObjectPropertyExpression* compExpCast = dynamic_cast<CAsymmetricObjectPropertyExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CAsymmetricObjectPropertyExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
