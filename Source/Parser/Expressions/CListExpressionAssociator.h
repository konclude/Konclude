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

#ifndef KONCLUDE_PARSER_EXPRESSION_CLISTEXPRESSIONASSOCIATOR_H
#define KONCLUDE_PARSER_EXPRESSION_CLISTEXPRESSIONASSOCIATOR_H


// Libraries includes


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
			 *		\class		CListExpressionAssociator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CListExpressionAssociator {
				// public methods
				public:
					//! Constructor
					CListExpressionAssociator(CBuildExpression *expression1 = 0, CBuildExpression *expression2 = 0, CBuildExpression *expression3 = 0);
					CListExpressionAssociator(const CEXPRESSIONLIST<CBuildExpression*>& expressionList);
					CListExpressionAssociator(CEXPRESSIONLIST<CBuildExpression*>* expressionList);

					CListExpressionAssociator* initExpressionAssociator(CBuildExpression *expression1 = 0, CBuildExpression *expression2 = 0, CBuildExpression *expression3 = 0);

					CEXPRESSIONLIST<CBuildExpression*>* getExpressionList();
					const CEXPRESSIONLIST<CBuildExpression*>* getConstExpressionList() const;
					cint64 getExpressionCount();

					virtual cint64 getStructuralHashValue();
					virtual bool compareStructuralEquivalence(const CListExpressionAssociator& listExpressionAssociator);

					virtual bool visitSubExpressions(CBuildExpression* expression, CSubExpressionVisitor* subExpressionVisitor);

				// protected methods
				protected:

				// protected variables
				protected:
					CEXPRESSIONLIST<CBuildExpression*> mExpressionList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_EXPRESSION_CLISTEXPRESSIONASSOCIATOR_H
