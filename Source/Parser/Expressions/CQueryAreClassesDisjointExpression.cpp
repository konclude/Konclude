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

#include "CQueryAreClassesDisjointExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryAreClassesDisjointExpression::CQueryAreClassesDisjointExpression(const QString& name, const CEXPRESSIONLIST<CClassTermExpression*>& classTermExpList) 
					: CNameAssociator(name),CListClassTermExpressionAssociator(classTermExpList) {
			}


			CQueryAreClassesDisjointExpression::~CQueryAreClassesDisjointExpression() {
			}

			CEXPRESSIONLIST<CClassTermExpression*>* CQueryAreClassesDisjointExpression::getConstructedTestClasses() {
				return &mConstructedClassExpList;
			}


			CQueryAreClassesDisjointExpression *CQueryAreClassesDisjointExpression::addConstructedTestClass(CClassTermExpression *conBuildExp) {
				mConstructedClassExpList.append(conBuildExp);
				return this;
			}

			CBuildExpression::ExpressionType CQueryAreClassesDisjointExpression::getType() {
				return BETARECLASSESDISJOINT;
			}


			cint64 CQueryAreClassesDisjointExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryAreClassesDisjointExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryAreClassesDisjointExpression* compExpCast = dynamic_cast<CQueryAreClassesDisjointExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CQueryAreClassesDisjointExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
