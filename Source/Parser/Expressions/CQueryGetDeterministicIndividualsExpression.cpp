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

#include "CQueryGetDeterministicIndividualsExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQueryGetDeterministicIndividualsExpression::CQueryGetDeterministicIndividualsExpression(const QString& name) 
				: CNameAssociator(name),CListIndividualTermExpressionAssociator() {
			}

			CQueryGetDeterministicIndividualsExpression::CQueryGetDeterministicIndividualsExpression(const QString& name, const CEXPRESSIONLIST<CIndividualTermExpression*>& expressionList) 
					: CNameAssociator(name),CListIndividualTermExpressionAssociator(expressionList) {
			}


			CQueryGetDeterministicIndividualsExpression::~CQueryGetDeterministicIndividualsExpression() {
			}


			CBuildExpression::ExpressionType CQueryGetDeterministicIndividualsExpression::getType() {
				return BETGETDETERMINISTICINDIVIDUALS;
			}


			cint64 CQueryGetDeterministicIndividualsExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(),CListIndividualTermExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQueryGetDeterministicIndividualsExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQueryGetDeterministicIndividualsExpression* compExpCast = dynamic_cast<CQueryGetDeterministicIndividualsExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListIndividualTermExpressionAssociator::compareStructuralEquivalence(*compExpCast);
			}


			bool CQueryGetDeterministicIndividualsExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListIndividualTermExpressionAssociator::visitSubExpressions(this,subExpressionVisitor);
					return true;
				}
				return false;
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
