/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_PARSER_EXPRESSION_CEXPRESSIONASSOCIATOR_H
#define KONCLUDE_PARSER_EXPRESSION_CEXPRESSIONASSOCIATOR_H


// Libraries includes
#include <QString>


// Namespace includes
#include "ExpressionSettings.h"
#include "CBuildExpression.h"
#include "CSubExpressionVisitor.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *
			 *		\class		CExpressionAssociator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CExpressionAssociator {
				// public methods
				public:
					//! Constructor
					CExpressionAssociator(CBuildExpression* expression);

					CExpressionAssociator* setExpression(CBuildExpression* expression);
					CBuildExpression* getExpression();

					virtual cint64 getStructuralHashValue();
					virtual bool visitSubExpressions(CBuildExpression* expression, CSubExpressionVisitor* subExpressionVisitor);
					virtual bool compareStructuralEquivalence(const CExpressionAssociator& expressionAssociator);

				// protected methods
				protected:

				// private methods
				private:

				// protected variables
				protected:
					CBuildExpression* mExpression;

				// private variables
				private:

			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_EXPRESSION_CEXPRESSIONASSOCIATOR_H
