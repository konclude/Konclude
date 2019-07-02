/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CPairExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CPairExpressionAssociator::CPairExpressionAssociator(CBuildExpression *expression1, CBuildExpression *expression2) 
					: CListExpressionAssociator() {

				mExpressionList.append(expression1);
				mExpressionList.append(expression2);
			}


			CPairExpressionAssociator::~CPairExpressionAssociator() {
			}


			CPairExpressionAssociator* CPairExpressionAssociator::setFirstExpression(CBuildExpression *expression1) {
				mExpressionList.replace(0,expression1);
				return this;
			}

			CBuildExpression *CPairExpressionAssociator::getFirstExpression() const {
				return mExpressionList.first();
			}

			CPairExpressionAssociator* CPairExpressionAssociator::setSecondExpression(CBuildExpression *expression2) {
				mExpressionList.replace(0,expression2);
				return this;
			}

			CBuildExpression *CPairExpressionAssociator::getSecondExpression() const {
				return mExpressionList.at(1);
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
