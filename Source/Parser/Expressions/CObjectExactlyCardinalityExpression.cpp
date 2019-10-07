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

#include "CObjectExactlyCardinalityExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectExactlyCardinalityExpression::CObjectExactlyCardinalityExpression(CObjectPropertyTermExpression *expression1, CClassTermExpression *expression2, cint64 cardinality) 
					: CObjectCardinalityExpression(expression1,expression2,cardinality) {
			}


			CObjectExactlyCardinalityExpression::~CObjectExactlyCardinalityExpression() {
			}

			CBuildExpression::ExpressionType CObjectExactlyCardinalityExpression::getType() {
				return BETOBJECTEXACTLYCARDINALITY;
			}

			cint64 CObjectExactlyCardinalityExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue(),CClassTermExpressionAssociator::getStructuralHashValue(),mCard);
				return hashValue;
			}

			bool CObjectExactlyCardinalityExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectExactlyCardinalityExpression* compExpCast = dynamic_cast<CObjectExactlyCardinalityExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && compExpCast->mCard == mCard;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
