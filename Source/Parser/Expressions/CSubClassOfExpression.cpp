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

#include "CSubClassOfExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CSubClassOfExpression::CSubClassOfExpression(CClassTermExpression *subClassExpression1, CClassTermExpression *superClassExpression2) 
					: COrderedPairClassTermExpressionAssociator(subClassExpression1,superClassExpression2) {
			}


			CSubClassOfExpression::~CSubClassOfExpression() {
			}

			CClassTermExpression* CSubClassOfExpression::getSubClassTermExpression() {
				return getFirstClassTermExpression();
			}

			CClassTermExpression* CSubClassOfExpression::getSuperClassTermExpression() {
				return getSecondClassTermExpression();
			}


			CBuildExpression::ExpressionType CSubClassOfExpression::getType() {
				return BETESUBCLASSOF;
			}

			cint64 CSubClassOfExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),COrderedPairClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CSubClassOfExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CSubClassOfExpression* compExpCast = dynamic_cast<CSubClassOfExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return COrderedPairClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CSubClassOfExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					COrderedPairClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
