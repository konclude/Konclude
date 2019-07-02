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

#include "CDisjointUnionExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CDisjointUnionExpression::CDisjointUnionExpression(CClassExpression* classExpression, const CEXPRESSIONLIST<CClassTermExpression*>& expressionList) 
					: CClassExpressionAssociator(classExpression), CListClassTermExpressionAssociator(expressionList) {
			}


			CDisjointUnionExpression::~CDisjointUnionExpression() {
			}



			CBuildExpression::ExpressionType CDisjointUnionExpression::getType() {
				return BETDISJOINTUNION;
			}

			cint64 CDisjointUnionExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CClassExpressionAssociator::getStructuralHashValue(),CListClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CDisjointUnionExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CDisjointUnionExpression* compExpCast = dynamic_cast<CDisjointUnionExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CClassExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CListClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CDisjointUnionExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CClassExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CListClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
