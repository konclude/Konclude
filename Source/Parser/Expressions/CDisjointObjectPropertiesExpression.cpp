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

#include "CDisjointObjectPropertiesExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CDisjointObjectPropertiesExpression::CDisjointObjectPropertiesExpression(CObjectPropertyTermExpression *expression1, CObjectPropertyTermExpression *expression2, CObjectPropertyTermExpression *expression3) 
					: CListObjectPropertyTermExpressionAssociator(expression1,expression2,expression3) {
			}

			CDisjointObjectPropertiesExpression::CDisjointObjectPropertiesExpression(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressionList)
					: CListObjectPropertyTermExpressionAssociator(expressionList) {
			}


			CDisjointObjectPropertiesExpression::~CDisjointObjectPropertiesExpression() {
			}


			cint64 CDisjointObjectPropertiesExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListObjectPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CDisjointObjectPropertiesExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CDisjointObjectPropertiesExpression* compExpCast = dynamic_cast<CDisjointObjectPropertiesExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			CBuildExpression::ExpressionType CDisjointObjectPropertiesExpression::getType() {
				return BETDISJOINTOBJECTPROPERTIES;
			}

			bool CDisjointObjectPropertiesExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
