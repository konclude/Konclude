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

#include "CQuerySPARQLAskBasicGraphPatternExpression.h"


namespace Konclude {

	namespace Parser {

		namespace Expression {


			CQuerySPARQLAskBasicGraphPatternExpression::CQuerySPARQLAskBasicGraphPatternExpression(const QString& name, const QList<CAxiomExpression*>& axiomExpressionList, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList)
					: CQuerySPARQLBasicGraphPatternExpression(name, axiomExpressionList, filteringList) {
			}

			

			CQuerySPARQLAskBasicGraphPatternExpression::~CQuerySPARQLAskBasicGraphPatternExpression() {
			}


			CBuildExpression::ExpressionType CQuerySPARQLAskBasicGraphPatternExpression::getType() {
				return BETSPARQLASKBASICGRAPHPATTERN;
			}


			bool CQuerySPARQLAskBasicGraphPatternExpression::compareStructuralEquivalence(CBuildExpression* compareExpression) {
				CQuerySPARQLAskBasicGraphPatternExpression* compExpCast = dynamic_cast<CQuerySPARQLAskBasicGraphPatternExpression*>(compareExpression);
				if (!compExpCast) {
					return false;
				} 
				return CQuerySPARQLBasicGraphPatternExpression::compareStructuralEquivalence(compExpCast);
			}


		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude
