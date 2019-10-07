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

#ifndef KONCLUDE_PARSER_EXPRESSION_CQuerySPARQLSelectBasicGraphPatternExpression_H
#define KONCLUDE_PARSER_EXPRESSION_CQuerySPARQLSelectBasicGraphPatternExpression_H


// Libraries includes


// Namespace includes
#include "CQueryExpression.h"
#include "CAxiomExpression.h"
#include "CNameAssociator.h"
#include "COrderedListVariableAssociator.h"
#include "CListAxiomExpressionAssociator.h"
#include "COrderedListOrderingTermExpressionAssociator.h"
#include "CListFilteringExpressionAssociator.h"
#include "CExpressionVariable.h"
#include "CQuerySPARQLBasicGraphPatternExpression.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *	\class		CQuerySPARQLSelectBasicGraphPatternExpression
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		TODO
			 */
			class CQuerySPARQLSelectBasicGraphPatternExpression : public CQuerySPARQLBasicGraphPatternExpression, public COrderedListVariableAssociator, public COrderedListOrderingTermExpressionAssociator {
				// public methods
				public:
					//! Constructor
					CQuerySPARQLSelectBasicGraphPatternExpression(const QString& name, const QList<CAxiomExpression*>& axiomExpressionList, const QList<CExpressionVariable*>& answerVariables, const CEXPRESSIONLIST<COrderingTermExpression*>& orderingList, const CEXPRESSIONLIST<CFilteringTermExpression*>& filteringList, bool distinct, cint64 limit = -1, cint64 offset = 0);

					//! Destructor
					virtual ~CQuerySPARQLSelectBasicGraphPatternExpression();

					virtual ExpressionType getType();

					virtual cint64 getStructuralHashValue();
					virtual bool compareStructuralEquivalence(CBuildExpression* compareExpression);

					bool isDistinct();

					cint64 getLimit();
					cint64 getOffset();


					virtual bool visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor);


				// protected methods
				protected:

				// protected variables
				protected:
					bool mDistinct;
					cint64 mLimit;
					cint64 mOffset;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_EXPRESSION_CQuerySPARQLSelectBasicGraphPatternExpression_H
