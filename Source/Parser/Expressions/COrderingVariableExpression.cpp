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

#include "COrderingVariableExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			COrderingVariableExpression::COrderingVariableExpression(CExpressionVariable* variable, COrderingVariableExpression::ORDERING_TYPE orderingType) : CVariableAssociator(variable) {
				mOrderingType = orderingType;
			}

		
			CBuildExpression::ExpressionType COrderingVariableExpression::getType() {
				return BETORDERINGVARIABLE;
			}

			CExpressionVariable *COrderingVariableExpression::getOrderingVariable() {
				return CVariableAssociator::getEntity();
			}


			cint64 COrderingVariableExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(), CVariableAssociator::getStructuralHashValue(),(cint64)mOrderingType);
				return hashValue;
			}

			bool COrderingVariableExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				COrderingVariableExpression* compExpCast = dynamic_cast<COrderingVariableExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				}
				if (compExpCast->mOrderingType != mOrderingType) {
					return false;
				}
				return CVariableAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool COrderingVariableExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					subExpressionVisitor->visitEntity(this, CVariableAssociator::getEntity(), subExpressionVisitor);
					return true;
				}
				return false;
			}

			COrderingVariableExpression::ORDERING_TYPE COrderingVariableExpression::getOrderingType() {
				return mOrderingType;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
