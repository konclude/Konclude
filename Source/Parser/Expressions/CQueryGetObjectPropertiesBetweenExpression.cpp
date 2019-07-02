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

#include "CQueryGetObjectPropertiesBetweenExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetObjectPropertiesBetweenExpression::CQueryGetObjectPropertiesBetweenExpression(const QString& name, CIndividualTermExpression* sourceIndiTermExp, CIndividualTermExpression* targetIndiTermExp) 
					: CNameAssociator(name),COrderedPairIndividualTermExpressionAssociator(sourceIndiTermExp,targetIndiTermExp) {
			}


			CQueryGetObjectPropertiesBetweenExpression::~CQueryGetObjectPropertiesBetweenExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetObjectPropertiesBetweenExpression::getType() {
				return BETGETOBJECTPROPERTIESBETWEEN;
			}


			cint64 CQueryGetObjectPropertiesBetweenExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),COrderedPairIndividualTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryGetObjectPropertiesBetweenExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetObjectPropertiesBetweenExpression* compExpCast = dynamic_cast<CQueryGetObjectPropertiesBetweenExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return COrderedPairIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CQueryGetObjectPropertiesBetweenExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					COrderedPairIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

			CIndividualTermExpression* CQueryGetObjectPropertiesBetweenExpression::getSourceIndividualTermExpression() {
				return getFirstIndividualTermExpression();
			}

			CIndividualTermExpression* CQueryGetObjectPropertiesBetweenExpression::getTargetIndividualTermExpression() {
				return getSecondIndividualTermExpression();
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
