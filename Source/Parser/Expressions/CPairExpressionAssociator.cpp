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

#include "CPairExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CPairExpressionAssociator::CPairExpressionAssociator(CBuildExpression *expression1, CBuildExpression *expression2) 
					: mFirstExpression(expression1),mSecondExpression(expression2) {
			}


			CPairExpressionAssociator* CPairExpressionAssociator::setFirstExpression(CBuildExpression *expression1) {
				mFirstExpression = expression1;
				return this;
			}

			CBuildExpression *CPairExpressionAssociator::getFirstExpression() const {
				return mFirstExpression;
			}

			CPairExpressionAssociator* CPairExpressionAssociator::setSecondExpression(CBuildExpression *expression2) {
				mSecondExpression = expression2;
				return this;
			}

			CBuildExpression *CPairExpressionAssociator::getSecondExpression() const {
				return mSecondExpression;
			}



			cint64 CPairExpressionAssociator::getStructuralHashValue() {
				cint64 hashValue = 0;
				cint64 tmpValue1 = 0;
				cint64 tmpValue2 = 1;
				if (mFirstExpression) {
					tmpValue1 += cint64(mFirstExpression);
					tmpValue2 *= cint64(mFirstExpression);
				}
				if (mSecondExpression) {
					tmpValue1 += cint64(mSecondExpression);
					tmpValue2 *= cint64(mSecondExpression);
				}
				hashValue = tmpValue1*tmpValue2;
				return hashValue;
			}

			bool CPairExpressionAssociator::compareStructuralEquivalence(const CPairExpressionAssociator& pairExpressionAssociator) {
				if (mFirstExpression != pairExpressionAssociator.mFirstExpression) {
					if (mFirstExpression != pairExpressionAssociator.mSecondExpression) {
						return false;
					}
					if (mSecondExpression != pairExpressionAssociator.mFirstExpression) {
						return false;
					}
				} else {
					if (mSecondExpression != pairExpressionAssociator.mSecondExpression) {
						return false;
					}
				}
				return true;
			}

			bool CPairExpressionAssociator::visitSubExpressions(CBuildExpression* expression, CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					if (mFirstExpression) {
						subExpressionVisitor->visitSubExpression(expression,mFirstExpression,subExpressionVisitor);
					}
					if (mSecondExpression) {
						subExpressionVisitor->visitSubExpression(expression,mSecondExpression,subExpressionVisitor);
					}
					return true;
				}
				return false;
			}



		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
