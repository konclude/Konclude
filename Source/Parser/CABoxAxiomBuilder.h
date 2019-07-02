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

#ifndef KONCLUDE_PARSER_CABOXAXIOMBUILDER_H
#define KONCLUDE_PARSER_CABOXAXIOMBUILDER_H

// Libraries includes


// Namespace includes


// Other includes
#include "Parser/Expressions/CNamedIndividualExpression.h"
#include "Parser/Expressions/CAnonymousIndividualExpression.h"
#include "Parser/Expressions/CClassAssertionExpression.h"
#include "Parser/Expressions/CObjectPropertyAssertionExpression.h"
#include "Parser/Expressions/CNegativeObjectPropertyAssertionExpression.h"
#include "Parser/Expressions/CSameIndividualExpression.h"
#include "Parser/Expressions/CDifferentIndividualsExpression.h"
#include "Parser/Expressions/CDataPropertyAssertionExpression.h"
#include "Parser/Expressions/CNegativeDataPropertyAssertionExpression.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Parser {

		using namespace Expression;


		/*! 
		*
		*		\class		CABoxAxiomBuilder
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CABoxAxiomBuilder {
			// public methods
			public:
				//! Constructor
				CABoxAxiomBuilder();

				//! Destructor
				virtual ~CABoxAxiomBuilder();

				virtual CNamedIndividualExpression* getNamedIndividual(const QString& individualName) = 0;
				virtual CNamedIndividualExpression* getNamedIndividual(const QStringRef& individualName) = 0;
				virtual CAnonymousIndividualExpression* getAnonymousIndividual(const QString& ontologyName, const QString& individualName) = 0;
				virtual CAnonymousIndividualExpression* getAnonymousIndividual(const QStringRef& ontologyName, const QStringRef& individualName) = 0;

				virtual CClassAssertionExpression* getClassAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CClassAssertionExpression* getClassAssertion(CBuildExpression* expression1, CBuildExpression* expression2) = 0;
				virtual CObjectPropertyAssertionExpression* getObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CObjectPropertyAssertionExpression* getObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) = 0;
				virtual CNegativeObjectPropertyAssertionExpression* getNegativeObjectPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CNegativeObjectPropertyAssertionExpression* getNegativeObjectPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) = 0;
				virtual CSameIndividualExpression* getSameIndividual(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CDifferentIndividualsExpression* getDifferentIndividuals(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CDataPropertyAssertionExpression* getDataPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CDataPropertyAssertionExpression* getDataPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) = 0;
				virtual CNegativeDataPropertyAssertionExpression* getNegativeDataPropertyAssertion(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CNegativeDataPropertyAssertionExpression* getNegativeDataPropertyAssertion(CBuildExpression* expression1, CBuildExpression* expression2, CBuildExpression* expression3) = 0;


				virtual CClassAssertionExpression* getClassAssertion(CIndividualTermExpression* expression1, CClassTermExpression* expression2) = 0;
				virtual CObjectPropertyAssertionExpression* getObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3) = 0;
				virtual CNegativeObjectPropertyAssertionExpression* getNegativeObjectPropertyAssertion(CIndividualTermExpression* expression1, CIndividualTermExpression* expression2, CObjectPropertyTermExpression* expression3) = 0;
				virtual CSameIndividualExpression* getSameIndividual(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions) = 0;
				virtual CDifferentIndividualsExpression* getDifferentIndividuals(const CEXPRESSIONLIST<CIndividualTermExpression*>& expressions) = 0;
				virtual CDataPropertyAssertionExpression* getDataPropertyAssertion(CIndividualTermExpression* expression1, CDataLiteralExpression* expression2, CDataPropertyTermExpression* expression3) = 0;
				virtual CNegativeDataPropertyAssertionExpression* getNegativeDataPropertyAssertion(CIndividualTermExpression* expression1, CDataLiteralExpression* expression2, CDataPropertyTermExpression* expression3) = 0;

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

#endif // KONCLUDE_PARSER_CABOXAXIOMBUILDER_H
