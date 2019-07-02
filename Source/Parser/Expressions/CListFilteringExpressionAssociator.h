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

#ifndef KONCLUDE_PARSER_EXPRESSION_CLISTFILTERINGEXPRESSIONASSOCIATOR_H
#define KONCLUDE_PARSER_EXPRESSION_CLISTFILTERINGEXPRESSIONASSOCIATOR_H


// Libraries includes
#include <QString>


// Namespace includes
#include "ExpressionSettings.h"
#include "CFilteringTermExpression.h"
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
			 *		\class		CListFilteringExpressionAssociator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CListFilteringExpressionAssociator : public CListExpressionAssociator {
				// public methods
				public:
					//! Constructor
					CListFilteringExpressionAssociator(CFilteringTermExpression* expression1 = nullptr, CFilteringTermExpression* expression2 = nullptr, CFilteringTermExpression* expression3 = nullptr);
					CListFilteringExpressionAssociator(const CEXPRESSIONLIST<CFilteringTermExpression*>& expressionList);

					//! Destructor
					virtual ~CListFilteringExpressionAssociator();

					CEXPRESSIONLIST<CFilteringTermExpression*>* getFilteringExpressionList();

					virtual bool compareStructuralEquivalence(const CListFilteringExpressionAssociator& listClassTermExpressionAssociator);

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

#endif // KONCLUDE_PARSER_EXPRESSION_CLISTFILTERINGEXPRESSIONASSOCIATOR_H
