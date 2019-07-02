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

#include "CObjectOneOfExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectOneOfExpression::CObjectOneOfExpression(CIndividualTermExpression *expression1, CIndividualTermExpression *expression2) 
					: CListIndividualTermExpressionAssociator(expression1,expression2) {
			}

			CObjectOneOfExpression::CObjectOneOfExpression(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressionList) 
					: CListIndividualTermExpressionAssociator(expressionList) {
			}


			CObjectOneOfExpression* CObjectOneOfExpression::initObjectOneOfExpression(CIndividualTermExpression *expression1, CIndividualTermExpression *expression2) {
				CListIndividualTermExpressionAssociator::initIndividualTermExpressionAssociator(expression1,expression2);
				return this;
			}



			CBuildExpression::ExpressionType CObjectOneOfExpression::getType() {
				return BETOBJECTONEOF;
			}


			cint64 CObjectOneOfExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListIndividualTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CObjectOneOfExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectOneOfExpression* compExpCast = dynamic_cast<CObjectOneOfExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CObjectOneOfExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
