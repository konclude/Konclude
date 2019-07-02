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

#ifndef KONCLUDE_PARSER_EXPRESSION_CEQUIVALENTOBJECTPROPERTIESEXPRESSION_H
#define KONCLUDE_PARSER_EXPRESSION_CEQUIVALENTOBJECTPROPERTIESEXPRESSION_H


// Libraries includes


// Namespace includes
#include "CObjectPropertyAxiomExpression.h"
#include "CListObjectPropertyTermExpressionAssociator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *	\class		CEquivalentObjectPropertiesExpression
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		TODO
			 */
			class CEquivalentObjectPropertiesExpression : public CObjectPropertyAxiomExpression, public CListObjectPropertyTermExpressionAssociator {
				// public methods
				public:
					//! Constructor
					CEquivalentObjectPropertiesExpression(CObjectPropertyTermExpression *expression1, CObjectPropertyTermExpression *expression2, CObjectPropertyTermExpression *expression3 = nullptr);
					CEquivalentObjectPropertiesExpression(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressionList);

					//! Destructor
					virtual ~CEquivalentObjectPropertiesExpression();

					virtual ExpressionType getType();

					virtual cint64 getStructuralHashValue();
					virtual bool compareStructuralEquivalence(CBuildExpression* compareExpression);

					virtual bool visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor);

				// protected methods
				protected:

				// private methods
				private:


				// private variables
				private:

			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_EXPRESSION_CEQUIVALENTOBJECTPROPERTIESEXPRESSION_H
