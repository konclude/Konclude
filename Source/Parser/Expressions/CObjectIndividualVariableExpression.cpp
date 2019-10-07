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

#include "CObjectIndividualVariableExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CObjectIndividualVariableExpression::CObjectIndividualVariableExpression(const QString& variableName, cint64 axiomNumber) 
					: CExpressionEntity(variableName),mAxiomNumber(axiomNumber) {
			}


			CObjectIndividualVariableExpression::~CObjectIndividualVariableExpression() {
			}


			CBuildExpression::ExpressionType CObjectIndividualVariableExpression::getType() {
				return BETNOMINALINDIVIDUALVARIABLE;
			}


			cint64 CObjectIndividualVariableExpression::getAxiomNumber() {
				return mAxiomNumber;
			}


			cint64 CObjectIndividualVariableExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),mAxiomNumber);
				return hashValue;
			}

			bool CObjectIndividualVariableExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CObjectIndividualVariableExpression* compExpCast = dynamic_cast<CObjectIndividualVariableExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} else {
					if (mAxiomNumber != compExpCast->getAxiomNumber()) {
						return false;
					}
				}
				return true;
			}

			bool CObjectIndividualVariableExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					subExpressionVisitor->visitEntity(this,this,subExpressionVisitor);
					return true;
				}
				return false;		
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
