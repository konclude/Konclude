/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CObjectAllValuesFromExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectAllValuesFromExpression::CObjectAllValuesFromExpression(CObjectPropertyTermExpression *expression1, CClassTermExpression *expression2) 
					: CObjectPropertyTermExpressionAssociator(expression1), CClassTermExpressionAssociator(expression2) {
			}


			CObjectAllValuesFromExpression::~CObjectAllValuesFromExpression() {
			}



			CBuildExpression::ExpressionType CObjectAllValuesFromExpression::getType() {
				return BETOBJECTALLVALUEFROM;
			}


			cint64 CObjectAllValuesFromExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue(),CClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CObjectAllValuesFromExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectAllValuesFromExpression* compExpCast = dynamic_cast<CObjectAllValuesFromExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CObjectAllValuesFromExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
