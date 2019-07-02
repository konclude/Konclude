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

#include "CAnonymousIndividualExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CAnonymousIndividualExpression::CAnonymousIndividualExpression(const QString& ontologyName, const QString& nodeName) {
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


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
