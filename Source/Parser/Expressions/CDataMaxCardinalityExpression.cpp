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

#include "CDataMaxCardinalityExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CDataMaxCardinalityExpression::CDataMaxCardinalityExpression(CDataPropertyTermExpression *expression1, CDataRangeTermExpression *expression2, cint64 cardinality) 
					: CDataCardinalityExpression(expression1,expression2,cardinality) {
			}


			CDataMaxCardinalityExpression::~CDataMaxCardinalityExpression() {
			}

			CBuildExpression::ExpressionType CDataMaxCardinalityExpression::getType() {
				return BETDATAMAXCARDINALITY;
			}

			cint64 CDataMaxCardinalityExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CDataPropertyTermExpressionAssociator::getStructuralHashValue(),CDataRangeTermExpressionAssociator::getStructuralHashValue(),mCard);
				return hashValue;
			}

			bool CDataMaxCardinalityExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CDataMaxCardinalityExpression* compExpCast = dynamic_cast<CDataMaxCardinalityExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CDataRangeTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && compExpCast->mCard == mCard;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
