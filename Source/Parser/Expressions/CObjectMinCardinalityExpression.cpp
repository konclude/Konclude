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

#include "CObjectMinCardinalityExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectMinCardinalityExpression::CObjectMinCardinalityExpression(CObjectPropertyTermExpression *expression1, CClassTermExpression *expression2, cint64 cardinality) 
					: CObjectCardinalityExpression(expression1,expression2,cardinality) {
			}


			CObjectMinCardinalityExpression::~CObjectMinCardinalityExpression() {
			}

			CBuildExpression::ExpressionType CObjectMinCardinalityExpression::getType() {
				return BETOBJECTMINCARDINALITY;
			}

			cint64 CObjectMinCardinalityExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue(),CClassTermExpressionAssociator::getStructuralHashValue(),mCard);
				return hashValue;
			}

			bool CObjectMinCardinalityExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectMinCardinalityExpression* compExpCast = dynamic_cast<CObjectMinCardinalityExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && compExpCast->mCard == mCard;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
