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

#include "CQuerySPARQLBasicGraphPatternExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {



			CQuerySPARQLBasicGraphPatternExpression::CQuerySPARQLBasicGraphPatternExpression(const QString& name, const QList<CAxiomExpression*>& axiomExpressionList, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList)
				: CNameAssociator(name), CListAxiomExpressionAssociator(axiomExpressionList), CListFilteringExpressionAssociator(filteringList) {
			}


			CQuerySPARQLBasicGraphPatternExpression::~CQuerySPARQLBasicGraphPatternExpression() {
			}




			cint64 CQuerySPARQLBasicGraphPatternExpression::getStructuralHashValue() {
				cint64 hashValue = CExpressionHashMixer::getHashValueMixed((cint64)getType(), CListAxiomExpressionAssociator::getStructuralHashValue(), CListFilteringExpressionAssociator::getStructuralHashValue());
				return hashValue;
			}

			bool CQuerySPARQLBasicGraphPatternExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQuerySPARQLBasicGraphPatternExpression* compExpCast = dynamic_cast<CQuerySPARQLBasicGraphPatternExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CListAxiomExpressionAssociator::compareStructuralEquivalence(*compExpCast) && CListFilteringExpressionAssociator::compareStructuralEquivalence(*(CListFilteringExpressionAssociator*)compExpCast);
			}


			bool CQuerySPARQLBasicGraphPatternExpression::visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor) {
				if (subExpressionVisitor) {
					CListAxiomExpressionAssociator::visitSubExpressions(this, subExpressionVisitor);
					CListFilteringExpressionAssociator::visitSubExpressions(this, subExpressionVisitor);
					return true;
				}
				return false;
			}




		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
