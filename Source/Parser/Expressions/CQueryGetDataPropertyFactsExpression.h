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

#ifndef KONCLUDE_PARSER_EXPRESSION_CQUERYGETDATAPROPERTYFACTSEXPRESSION_H
#define KONCLUDE_PARSER_EXPRESSION_CQUERYGETDATAPROPERTYFACTSEXPRESSION_H

// Namespace includes
#include "CQueryExpression.h"
#include "CIndividualTermExpressionAssociator.h"
#include "CDataPropertyTermExpressionAssociator.h"
#include "CDataLiteralTermExpressionAssociator.h"
#include "CNameAssociator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *
			 *		\class		CQueryGetDataPropertyFactsExpression
			 *
			 *		\brief		OWLlink data property fact requests that are answered from the data literal
			 *					realization: the data properties of a source individual, the data
			 *					properties of a literal, the data property sources of a literal and the
			 *					flattened data property sources of a literal.
			 *
			 *		The requests share the argument kinds (individual, data property, data literal), so
			 *		one expression class with a request kind is used. The individual, the data property
			 *		and the data literal are optional depending on the kind.
			 *
			 */
			class CQueryGetDataPropertyFactsExpression : public CQueryExpression, public CIndividualTermExpressionAssociator, public CDataPropertyTermExpressionAssociator, public CDataLiteralTermExpressionAssociator, public CNameAssociator {
				// public methods
				public:
					enum DATAPROPERTYFACTTYPE {
						DPFTDATAPROPERTIESOFSOURCE,
						DPFTDATAPROPERTIESBETWEEN,
						DPFTDATAPROPERTIESOFLITERAL,
						DPFTDATAPROPERTYSOURCES,
						DPFTFLATTENEDDATAPROPERTYSOURCES
					};

					//! Constructor
					CQueryGetDataPropertyFactsExpression(const QString& name, DATAPROPERTYFACTTYPE factType, CIndividualTermExpression* indiTermExp, CDataPropertyTermExpression* dataPropTermExp, CDataLiteralTermExpression* dataLiteralTermExp, bool negative = false);

					//! Destructor
					virtual ~CQueryGetDataPropertyFactsExpression();

					virtual ExpressionType getType();

					DATAPROPERTYFACTTYPE getDataPropertyFactType();

					//! whether the negative flag of a source request is set
					bool isNegative();

					//! the name of the request kind for logging
					static QString getDataPropertyFactTypeString(DATAPROPERTYFACTTYPE factType);

					virtual cint64 getStructuralHashValue();
					virtual bool compareStructuralEquivalence(CBuildExpression* compareExpression);

					virtual bool visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor);

				// protected methods
				protected:

				// protected variables
				protected:
					DATAPROPERTYFACTTYPE mFactType;
					bool mNegative;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_EXPRESSION_CQUERYGETDATAPROPERTYFACTSEXPRESSION_H
