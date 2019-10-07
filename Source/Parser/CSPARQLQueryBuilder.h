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

#ifndef KONCLUDE_PARSER_CSPARQLQUERYBUILDER_H
#define KONCLUDE_PARSER_CSPARQLQUERYBUILDER_H


// Libraries includes
#include <QHash>
#include <QList>
#include <QString>


// Namespace includes
#include "CQueryBuilder.h"


// Other includes
#include "Expressions/CBuildExpression.h"
#include "Expressions/CClassExpression.h"

#include "Expressions/CExpressionVariable.h"
#include "Expressions/CQuerySPARQLBasicGraphPatternExpression.h"
#include "Expressions/CAxiomExpression.h"
#include "Expressions/COrderingVariableExpression.h"
#include "Expressions/CFilteringComparisonDataLiteralExpression.h"
#include "Expressions/CQuerySPARQLSelectBasicGraphPatternExpression.h"
#include "Expressions/CQuerySPARQLAskBasicGraphPatternExpression.h"



// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Parser {

		using namespace Expression;


		/*! 
		*
		*		\class		CSPARQLQueryBuilder
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CSPARQLQueryBuilder {
			// public methods
			public:
				//! Constructor
				CSPARQLQueryBuilder();

				//! Destructor
				virtual ~CSPARQLQueryBuilder();


				virtual CQuerySPARQLSelectBasicGraphPatternExpression* getSPARQLBasicGraphPatternSelectQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QList<CExpressionVariable*>& disVarList, const CEXPRESSIONLIST<COrderingTermExpression*>& orderingList, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList, bool distinctModifier, cint64 limit, cint64 offset, const QString& queryName = "") = 0;
				virtual CQuerySPARQLSelectBasicGraphPatternExpression* getSPARQLBasicGraphPatternSelectQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const QList<CExpressionVariable*>& disVarList, const CEXPRESSIONLIST<COrderingTermExpression*>& orderingList, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList, bool distinct, const QString& queryName = "") = 0;
				virtual CQuerySPARQLAskBasicGraphPatternExpression* getSPARQLBasicGraphPatternAskQuery(const QList<CAxiomExpression*>& basicGraphPatternAxiomExp, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList, const QString& queryName = "") = 0;

				virtual COrderingVariableExpression* getOrderingVariableExpression(CExpressionVariable* variable, bool ascOrdering) = 0;
				virtual CFilteringComparisonDataLiteralExpression* getFilteringComparisonDataLiteralExpression(CDataLiteralVariableExpression* variable, CDataLiteralExpression* dataLiteralExp, CFilteringComparisonDataLiteralExpression::COMPARING_TYPE comparingType) = 0;


			// protected methods
			protected:

			// protected variables
			protected:



			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLQUERYBUILDER_H
