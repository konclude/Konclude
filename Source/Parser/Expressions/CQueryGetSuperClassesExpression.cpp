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

#include "CQueryGetSuperClassesExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetSuperClassesExpression::CQueryGetSuperClassesExpression(const QString& name, bool direct, CClassTermExpression* classTermExp) 
					: CNameAssociator(name),CClassTermExpressionAssociator(classTermExp),mDirect(direct) {
			}


			CQueryGetSuperClassesExpression::~CQueryGetSuperClassesExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetSuperClassesExpression::getType() {
				return BETGETSUPERCLASSES;
			}


			cint64 CQueryGetSuperClassesExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CClassTermExpressionAssociator::getStructuralHashValue(),(cint64)mDirect);
				return hashValue;
			}

			bool CQueryGetSuperClassesExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetSuperClassesExpression* compExpCast = dynamic_cast<CQueryGetSuperClassesExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				if (compExpCast->mDirect != mDirect) {
					return false;
				}
				return CClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CQueryGetSuperClassesExpression::isDirect() {
				return mDirect;
			}

			bool CQueryGetSuperClassesExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
