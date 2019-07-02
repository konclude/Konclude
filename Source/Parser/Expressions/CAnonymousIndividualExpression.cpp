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

#include "CAnonymousIndividualExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CAnonymousIndividualExpression::CAnonymousIndividualExpression(const QString& ontologyName, const QString& nodeName)
					: CExpressionEntity(ontologyName+":"+nodeName){
				mOntologyName = ontologyName;
				mNodeName = nodeName;
			}


			CAnonymousIndividualExpression::~CAnonymousIndividualExpression() {
			}

			CBuildExpression::ExpressionType CAnonymousIndividualExpression::getType() {
				return BETANONYMOUSINDIVIDUAL;
			}



			cint64 CAnonymousIndividualExpression::getStructuralHashValue() {
				return (cint64)this;
			}

			bool CAnonymousIndividualExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CAnonymousIndividualExpression* compExpCast = dynamic_cast<CAnonymousIndividualExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				}
				if (compExpCast != this) {
					return false;
				}
				return true;
			}

			const QString& CAnonymousIndividualExpression::getOntologyName() {
				return mOntologyName;
			}

			const QString& CAnonymousIndividualExpression::getNodeName() {
				return mNodeName;
			}


			bool CAnonymousIndividualExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					subExpressionVisitor->visitEntity(this,this,subExpressionVisitor);
					return true;
				}
				return false;		
			}

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
