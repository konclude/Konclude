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

#include "CRuleClassAtomTermExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CRuleClassAtomTermExpression::CRuleClassAtomTermExpression(CClassTermExpression* expression1, CObjectIndividualVariableExpression* expression2) : CClassTermExpressionAssociator(expression1),CObjectIndividualVariableTermExpressionAssociator(expression2) {
			}


			CRuleClassAtomTermExpression::~CRuleClassAtomTermExpression() {
			}




			CBuildExpression::ExpressionType CRuleClassAtomTermExpression::getType() {
				return BETRULECLASSATOM;
			}


			cint64 CRuleClassAtomTermExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CClassTermExpressionAssociator::getStructuralHashValue(),CObjectIndividualVariableTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CRuleClassAtomTermExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CRuleClassAtomTermExpression* compExpCast = dynamic_cast<CRuleClassAtomTermExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CObjectIndividualVariableTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CRuleClassAtomTermExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CObjectIndividualVariableTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
