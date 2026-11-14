/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */#include "CQueryGetDataPropertyTargetsExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetDataPropertyTargetsExpression::CQueryGetDataPropertyTargetsExpression(const QString& name, CIndividualTermExpression* indiTermExp, CDataPropertyTermExpression* dataPropTermExp) 
					: CNameAssociator(name),CIndividualTermExpressionAssociator(indiTermExp),CDataPropertyTermExpressionAssociator(dataPropTermExp) {
			}


			CQueryGetDataPropertyTargetsExpression::~CQueryGetDataPropertyTargetsExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetDataPropertyTargetsExpression::getType() {
				return BETGETDATAPROPERTYTARGETS;
			}


			cint64 CQueryGetDataPropertyTargetsExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CIndividualTermExpressionAssociator::getStructuralHashValue());
				hashValue = CExpressionHashMixer::getHashValueMixed(hashValue,CDataPropertyTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryGetDataPropertyTargetsExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetDataPropertyTargetsExpression* compExpCast = dynamic_cast<CQueryGetDataPropertyTargetsExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CDataPropertyTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CQueryGetDataPropertyTargetsExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					CDataPropertyTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude