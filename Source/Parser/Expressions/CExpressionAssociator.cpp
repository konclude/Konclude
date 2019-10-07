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

#include "CExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CExpressionAssociator::CExpressionAssociator(CBuildExpression *expression) 
					: mExpression(expression) {

			}

			CExpressionAssociator* CExpressionAssociator::setExpression(CBuildExpression *expression) {
				mExpression = expression;
				return this;
			}

			CBuildExpression *CExpressionAssociator::getExpression() {
				return mExpression;
			}


			cint64 CExpressionAssociator::getStructuralHashValue() {
				cint64 hashValue = 0;
				cint64 tmpValue1 = 0;
				cint64 tmpValue2 = 1;
				if (mExpression) {
					tmpValue1 += cint64(mExpression);
					tmpValue2 *= cint64(mExpression);
				}
				hashValue = tmpValue1*tmpValue2;
				return hashValue;
			}


			bool CExpressionAssociator::visitSubExpressions(CBuildExpression* expression, CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {		
					if (mExpression) {
						subExpressionVisitor->visitSubExpression(expression,mExpression,subExpressionVisitor);
						return true;
					}
				}
				return false;
			}


			bool CExpressionAssociator::compareStructuralEquivalence(const CExpressionAssociator& expressionAssociator) {
				if (mExpression != expressionAssociator.mExpression) {
					return false;
				}
				return true;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
