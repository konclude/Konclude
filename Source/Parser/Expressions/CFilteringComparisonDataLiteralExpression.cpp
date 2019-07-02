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

#include "CFilteringComparisonDataLiteralExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CFilteringComparisonDataLiteralExpression::CFilteringComparisonDataLiteralExpression(CDataLiteralExpression* dataLiteralExp, CDataLiteralVariableExpression* dataLiteralVariableExp, CFilteringComparisonDataLiteralExpression::COMPARING_TYPE comparingType) : CDataLiteralExpressionAssociator(dataLiteralExp), CDataLiteralVariableExpressionAssociator(dataLiteralVariableExp) {
				mComparingType = comparingType;
			}


			CFilteringComparisonDataLiteralExpression::~CFilteringComparisonDataLiteralExpression() {
			}


			CBuildExpression::ExpressionType CFilteringComparisonDataLiteralExpression::getType() {
				return BETFILTERINGCOMPARISONDATALITERAL;
			}

			CDataLiteralVariableExpression *CFilteringComparisonDataLiteralExpression::getComparingDataLiteralVariable() {
				return CDataLiteralVariableExpressionAssociator::getDataLiteralVariableExpression();
			}


			CDataLiteralExpression *CFilteringComparisonDataLiteralExpression::getComparingDataLiteral() {
				return CDataLiteralExpressionAssociator::getDataLiteralExpression();
			}



			cint64 CFilteringComparisonDataLiteralExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(), CDataLiteralExpressionAssociator::getStructuralHashValue(), CDataLiteralVariableExpressionAssociator::getStructuralHashValue(), mComparingType);
				return hashValue;
			}

			bool CFilteringComparisonDataLiteralExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CFilteringComparisonDataLiteralExpression* compExpCast = dynamic_cast<CFilteringComparisonDataLiteralExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				}
				if (mComparingType != compExpCast->mComparingType) {
					return false;
				}
				return CDataLiteralExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CDataLiteralVariableExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CFilteringComparisonDataLiteralExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					subExpressionVisitor->visitEntity(this, CVariableAssociator::getEntity(), subExpressionVisitor);
					CDataLiteralExpressionAssociator::visitSubExpressions(this, subExpressionVisitor);
					return true;
				}
				return false;
			}

			CFilteringComparisonDataLiteralExpression::COMPARING_TYPE CFilteringComparisonDataLiteralExpression::getComparingType() {
				return mComparingType;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
