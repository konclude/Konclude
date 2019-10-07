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

#ifndef KONCLUDE_PARSER_EXPRESSION_CDATAALLVALUESFROMEXPRESSION_H
#define KONCLUDE_PARSER_EXPRESSION_CDATAALLVALUESFROMEXPRESSION_H


// Libraries includes
#include <QString>


// Namespace includes
#include "CClassTermExpression.h"
#include "CDataPropertyTermExpression.h"
#include "CDataPropertyTermExpressionAssociator.h"
#include "CDataRangeTermExpressionAssociator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {


			/*! 
			 *	\class		CDataAllValuesFromExpression
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		TODO
			 */
			class CDataAllValuesFromExpression : public CClassTermExpression, public CDataPropertyTermExpressionAssociator, public CDataRangeTermExpressionAssociator {
				// public methods
				public:
					//! Constructor
					CDataAllValuesFromExpression(CDataPropertyTermExpression *expression1, CDataRangeTermExpression *expression2);

					//! Destructor
					virtual ~CDataAllValuesFromExpression();

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

#endif // KONCLUDE_PARSER_EXPRESSION_CDATAALLVALUESFROMEXPRESSION_H
