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

#include "CNamedIndividualExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CNamedIndividualExpression::CNamedIndividualExpression(const QString& namedIndividual) 
					: CExpressionEntity(namedIndividual) {
			}


			CNamedIndividualExpression::~CNamedIndividualExpression() {
			}

			CBuildExpression::ExpressionType CNamedIndividualExpression::getType() {
				return BETNAMEDINDIVIDUAL;
			}



			cint64 CNamedIndividualExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType());
				return hashValue;
			}

			bool CNamedIndividualExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CNamedIndividualExpression* compExpCast = dynamic_cast<CNamedIndividualExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return true;
			}


			bool CNamedIndividualExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					subExpressionVisitor->visitEntity(this,this,subExpressionVisitor);
					return true;
				}
				return false;		
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
