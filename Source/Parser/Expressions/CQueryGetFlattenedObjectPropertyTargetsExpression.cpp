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

#include "CQueryGetFlattenedObjectPropertyTargetsExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetFlattenedObjectPropertyTargetsExpression::CQueryGetFlattenedObjectPropertyTargetsExpression(const QString& name, CObjectPropertyTermExpression* objPropTermExp, CIndividualTermExpression* indiTermExp) 
					: CNameAssociator(name),CObjectPropertyTermExpressionAssociator(objPropTermExp),CIndividualTermExpressionAssociator(indiTermExp) {
			}


			CQueryGetFlattenedObjectPropertyTargetsExpression::~CQueryGetFlattenedObjectPropertyTargetsExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetFlattenedObjectPropertyTargetsExpression::getType() {
				return BETGETFLATTENEDOBJECTPROPERTYTARGETS;
			}


			cint64 CQueryGetFlattenedObjectPropertyTargetsExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CObjectPropertyTermExpressionAssociator::getStructuralHashValue(),CIndividualTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryGetFlattenedObjectPropertyTargetsExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetFlattenedObjectPropertyTargetsExpression* compExpCast = dynamic_cast<CQueryGetFlattenedObjectPropertyTargetsExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CObjectPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CQueryGetFlattenedObjectPropertyTargetsExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CObjectPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
