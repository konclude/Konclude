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
 */

#include "CDeclarationAxiomExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CDeclarationAxiomExpression::CDeclarationAxiomExpression(CExpressionEntity* entity)
					: CEntityAssociator(entity) {
			}


			CDeclarationAxiomExpression::~CDeclarationAxiomExpression() {
			}


			cint64 CDeclarationAxiomExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CEntityAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CDeclarationAxiomExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CDeclarationAxiomExpression* compExpCast = dynamic_cast<CDeclarationAxiomExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CEntityAssociator::compareStructuralEquivalence(*compExpCast);
			}


			CBuildExpression::ExpressionType CDeclarationAxiomExpression::getType() {
				return BETDECLARATION;
			}



			bool CDeclarationAxiomExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					subExpressionVisitor->visitEntity(this,mEntity,subExpressionVisitor);
				}
				return true;
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
