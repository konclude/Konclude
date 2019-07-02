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

#include "COrderedPairExpressionAssociator.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			COrderedPairExpressionAssociator::COrderedPairExpressionAssociator(CBuildExpression *expression1, CBuildExpression *expression2) 
					: CPairExpressionAssociator(expression1,expression2) {
			}

			COrderedPairExpressionAssociator::~COrderedPairExpressionAssociator() {
			}


			cint64 COrderedPairExpressionAssociator::getStructuralHashValue() {
				return CExpressionHashMixer::getHashValueMixed(cint64(getFirstExpression()),cint64(getSecondExpression()));
			}

			bool COrderedPairExpressionAssociator::compareStructuralEquivalence(const CListExpressionAssociator& listExpressionAssociator) {
				if (listExpressionAssociator.getConstExpressionList()->count() != mExpressionList.count()) {
					return false;
				}
				if (listExpressionAssociator.getConstExpressionList()->first() != mExpressionList.first()) {
					return false;
				}
				if (listExpressionAssociator.getConstExpressionList()->last() != mExpressionList.last()) {
					return false;
				}
				return true;
			}

			bool COrderedPairExpressionAssociator::compareStructuralEquivalence(const COrderedPairExpressionAssociator& orderedPairExpressionAssociator) {
				if (orderedPairExpressionAssociator.getFirstExpression() != getFirstExpression()) {
					return false;
				}
				if (orderedPairExpressionAssociator.getSecondExpression() != getSecondExpression()) {
					return false;
				}
				return true;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
