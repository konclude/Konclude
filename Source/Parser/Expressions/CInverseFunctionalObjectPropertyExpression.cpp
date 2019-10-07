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

#include "CInverseFunctionalObjectPropertyExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CInverseFunctionalObjectPropertyExpression::CInverseFunctionalObjectPropertyExpression(CObjectPropertyTermExpression *expression)
					: CObjectPropertyTermExpressionAssociator(expression) {
			}


			CInverseFunctionalObjectPropertyExpression::~CInverseFunctionalObjectPropertyExpression() {
			}



			CBuildExpression::ExpressionType CInverseFunctionalObjectPropertyExpression::getType() {
				return BETINVERSEFUNCTIONALPROPERTY;
			}

			CObjectPropertyTermExpression *CInverseFunctionalObjectPropertyExpression::getInverseFunctionalObjectPropertyExpression() {
				return getObjectPropertyTermExpression();
			}


			cint64 CInverseFunctionalObjectPropertyExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CInverseFunctionalObjectPropertyExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CInverseFunctionalObjectPropertyExpression* compExpCast = dynamic_cast<CInverseFunctionalObjectPropertyExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CInverseFunctionalObjectPropertyExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
