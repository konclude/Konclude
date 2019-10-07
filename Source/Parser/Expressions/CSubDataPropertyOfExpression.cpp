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

#include "CSubDataPropertyOfExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CSubDataPropertyOfExpression::CSubDataPropertyOfExpression(CDataPropertyTermExpression *subObjectPropertyExpression1, CDataPropertyTermExpression *superbObjectPropertyExpression2) 
				: COrderedPairDataPropertyTermExpressionAssociator(subObjectPropertyExpression1,superbObjectPropertyExpression2) {
			}


			CSubDataPropertyOfExpression::~CSubDataPropertyOfExpression() {
			}

			CDataPropertyTermExpression* CSubDataPropertyOfExpression::getSubDataPropertyTermExpression() {
				return getFirstDataPropertyTermExpression();
			}

			CDataPropertyTermExpression* CSubDataPropertyOfExpression::getSuperDataPropertyTermExpression() {
				return getSecondDataPropertyTermExpression();
			}


			CBuildExpression::ExpressionType CSubDataPropertyOfExpression::getType() {
				return BETSUBDATAPROPERTYOF;
			}

			cint64 CSubDataPropertyOfExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),COrderedPairDataPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CSubDataPropertyOfExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CSubDataPropertyOfExpression* compExpCast = dynamic_cast<CSubDataPropertyOfExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return COrderedPairDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CSubDataPropertyOfExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					COrderedPairDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
