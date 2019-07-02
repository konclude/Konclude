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

#include "CQueryGetObjectPropertiesOfTargetExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetObjectPropertiesOfTargetExpression::CQueryGetObjectPropertiesOfTargetExpression(const QString& name, CIndividualTermExpression* indiTermExp) 
					: CNameAssociator(name),CIndividualTermExpressionAssociator(indiTermExp) {
			}


			CQueryGetObjectPropertiesOfTargetExpression::~CQueryGetObjectPropertiesOfTargetExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetObjectPropertiesOfTargetExpression::getType() {
				return BETGETOBJECTPROPERTIESOFTARGET;
			}


			cint64 CQueryGetObjectPropertiesOfTargetExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CIndividualTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryGetObjectPropertiesOfTargetExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetObjectPropertiesOfTargetExpression* compExpCast = dynamic_cast<CQueryGetObjectPropertiesOfTargetExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CQueryGetObjectPropertiesOfTargetExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
