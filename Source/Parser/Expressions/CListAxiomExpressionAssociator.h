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

#ifndef KONCLUDE_PARSER_EXPRESSION_CLISTAXIOMEXPRESSIONASSOCIATOR_H
#define KONCLUDE_PARSER_EXPRESSION_CLISTAXIOMEXPRESSIONASSOCIATOR_H


// Libraries includes
#include <QString>


// Namespace includes
#include "ExpressionSettings.h"
#include "CAxiomExpression.h"
#include "CListExpressionAssociator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *
			 *		\class		CListAxiomExpressionAssociator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CListAxiomExpressionAssociator : public CListExpressionAssociator {
				// public methods
				public:
					//! Constructor
					CListAxiomExpressionAssociator(CAxiomExpression* expression1 = nullptr, CAxiomExpression* expression2 = nullptr, CAxiomExpression* expression3 = nullptr);
					CListAxiomExpressionAssociator(const CEXPRESSIONLIST<CAxiomExpression*>& expressionList);

					//! Destructor
					virtual ~CListAxiomExpressionAssociator();

					CEXPRESSIONLIST<CAxiomExpression*>* getAxiomExpressionList();

					virtual bool compareStructuralEquivalence(const CListAxiomExpressionAssociator& listClassTermExpressionAssociator);

				// protected methods
				protected:

				// private methods
				private:

				// protected variables
				protected:

				// private variables
				private:

			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_EXPRESSION_CLISTAXIOMEXPRESSIONASSOCIATOR_H
