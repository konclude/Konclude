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

#include "CQueryIsClassSubsumedByExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryIsClassSubsumedByExpression::CQueryIsClassSubsumedByExpression(const QString& name, CClassTermExpression *subsummerClasss, CClassTermExpression *subsumedClass) 
				: CNameAssociator(name),COrderedPairClassTermExpressionAssociator(subsummerClasss,subsumedClass) {
			}


			CQueryIsClassSubsumedByExpression::~CQueryIsClassSubsumedByExpression() {
			}



			CClassTermExpression* CQueryIsClassSubsumedByExpression::getSubsumerClassExpression() {
				return getFirstClassTermExpression();
			}

			CClassTermExpression* CQueryIsClassSubsumedByExpression::getSubsumedClassExpression() {
				return getSecondClassTermExpression();
			}


			CClassTermExpression *CQueryIsClassSubsumedByExpression::getConstructedTestClassExpression() {
				return mConstructedClassExp;
			}

			CQueryIsClassSubsumedByExpression *CQueryIsClassSubsumedByExpression::setConstructedTestClassExpression(CClassTermExpression *conBuildExp) {
				mConstructedClassExp = conBuildExp;
				return this;
			}


			CBuildExpression::ExpressionType CQueryIsClassSubsumedByExpression::getType() {
				return BETISCLASSSUBSUMTIONBY;
			}

			cint64 CQueryIsClassSubsumedByExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),COrderedPairClassTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryIsClassSubsumedByExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryIsClassSubsumedByExpression* compExpCast = dynamic_cast<CQueryIsClassSubsumedByExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return COrderedPairClassTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CQueryIsClassSubsumedByExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					COrderedPairClassTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
