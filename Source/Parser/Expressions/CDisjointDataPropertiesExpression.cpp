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

#include "CDisjointDataPropertiesExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CDisjointDataPropertiesExpression::CDisjointDataPropertiesExpression(CDataPropertyTermExpression *expression1, CDataPropertyTermExpression *expression2, CDataPropertyTermExpression *expression3) 
					: CListDataPropertyTermExpressionAssociator(expression1,expression2,expression3) {
			}

			CDisjointDataPropertiesExpression::CDisjointDataPropertiesExpression(const CEXPRESSIONLIST<CDataPropertyTermExpression*>& expressionList)
					: CListDataPropertyTermExpressionAssociator(expressionList) {
			}


			CDisjointDataPropertiesExpression::~CDisjointDataPropertiesExpression() {
			}


			cint64 CDisjointDataPropertiesExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListDataPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CDisjointDataPropertiesExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CDisjointDataPropertiesExpression* compExpCast = dynamic_cast<CDisjointDataPropertiesExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			CBuildExpression::ExpressionType CDisjointDataPropertiesExpression::getType() {
				return BETDISJOINTDATAPROPERTIES;
			}

			bool CDisjointDataPropertiesExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
