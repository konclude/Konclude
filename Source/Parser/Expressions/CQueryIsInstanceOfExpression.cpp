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

#include "CQueryIsInstanceOfExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryIsInstanceOfExpression::CQueryIsInstanceOfExpression(const QString& name, CIndividualTermExpression *individualExp, CClassTermExpression *classExp) 
					: CNameAssociator(name),CClassTermExpressionAssociator(classExp),CIndividualTermExpressionAssociator(individualExp) {
				mConstructedClassExp = nullptr;
			}


			CQueryIsInstanceOfExpression::~CQueryIsInstanceOfExpression() {
			}


			CBuildExpression::ExpressionType CQueryIsInstanceOfExpression::getType() {
				return BETISINSTANCEOF;
			}

			CClassTermExpression *CQueryIsInstanceOfExpression::getConstructedTestClassExpression() {
				return mConstructedClassExp;
			}

			CQueryIsInstanceOfExpression *CQueryIsInstanceOfExpression::setConstructedTestClassExpression(CClassTermExpression *conBuildExp) {
				mConstructedClassExp = conBuildExp;
				return this;
			}


			cint64 CQueryIsInstanceOfExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CClassTermExpressionAssociator::getStructuralHashValue(),CIndividualTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryIsInstanceOfExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryIsInstanceOfExpression* compExpCast = dynamic_cast<CQueryIsInstanceOfExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}

			bool CQueryIsInstanceOfExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
