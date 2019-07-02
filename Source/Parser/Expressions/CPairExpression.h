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

#ifndef KONCLUDE_PARSER_EXPRESSION_CPAIREXPRESSION_H
#define KONCLUDE_PARSER_EXPRESSION_CPAIREXPRESSION_H


// Libraries includes
#include <QString>


// Namespace includes
#include "CBuildExpression.h"
#include "CListExpression.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *	\class		CPairExpression
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		TODO
			 */
			class CPairExpression : public CListExpression {
				// public methods
				public:
					//! Constructor
					CPairExpression(CBuildExpression *expression1, CBuildExpression *expression2);

					//! Destructor
					virtual ~CPairExpression();

					void setFirstExpression(CBuildExpression *expression1);
					void setSecondExpression(CBuildExpression *expression2);
					CBuildExpression *getFirstExpression();
					CBuildExpression *getSecondExpression();


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

#endif // KONCLUDE_PARSER_EXPRESSION_CPAIREXPRESSION_H
