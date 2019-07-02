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

#include "CQueryAreClassesEquivalentExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryAreClassesEquivalentExpression::CQueryAreClassesEquivalentExpression(const QString& name, const CEXPRESSIONLIST<CClassTermExpression*>& classTermExpList) 
					: CNameAssociator(name),CListClassTermExpressionAssociator(classTermExpList) {
			}


			CQueryAreClassesEquivalentExpression::~CQueryAreClassesEquivalentExpression() {
			}


			CEXPRESSIONLIST<CClassTermExpression*>* CQueryAreClassesEquivalentExpression::getConstructedTestClasses() {
				return &mConstructedClassExpList;
			}


			CQueryAreClassesEquivalentExpression *CQueryAreClassesEquivalentExpression::addConstructedTestClass(CClassTermExpression *conBuildExp) {
				mConstructedClassExpList.append(conBuildExp);
				return this;
			}


			CBuildExpression::ExpressionType CQueryAreClassesEquivalentExpression::getType() {
				return BETARECLASSESEQUIVALENCE;
			}


			cint64 CQueryAreClassesEquivalentExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryAreClassesEquivalentExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryAreClassesEquivalentExpression* compExpCast = dynamic_cast<CQueryAreClassesEquivalentExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CQueryAreClassesEquivalentExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
