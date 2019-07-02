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

#ifndef KONCLUDE_PARSER_CTBOXAXIOMBUILDER_H
#define KONCLUDE_PARSER_CTBOXAXIOMBUILDER_H


// Libraries includes
#include <QHash>
#include <QList>


// Namespace includes
#include "Parser/Expressions/CBuildExpression.h"

#include "Parser/Expressions/CClassExpression.h"
#include "Parser/Expressions/CObjectIndividualVariableExpression.h"

#include "Parser/Expressions/CSubClassOfExpression.h"
#include "Parser/Expressions/CEquivalentClassesExpression.h"
#include "Parser/Expressions/CDisjointClassesExpression.h"
#include "Parser/Expressions/CDisjointUnionExpression.h"

#include "Parser/Expressions/CObjectIntersectionOfExpression.h"
#include "Parser/Expressions/CObjectUnionOfExpression.h"
#include "Parser/Expressions/CObjectComplementOfExpression.h"
#include "Parser/Expressions/CObjectMaxCardinalityExpression.h"
#include "Parser/Expressions/CObjectMinCardinalityExpression.h"
#include "Parser/Expressions/CObjectExactlyCardinalityExpression.h"
#include "Parser/Expressions/CObjectAllValuesFromExpression.h"
#include "Parser/Expressions/CObjectSomeValuesFromExpression.h"
#include "Parser/Expressions/CObjectOneOfExpression.h"
#include "Parser/Expressions/CObjectHasValueExpression.h"
#include "Parser/Expressions/CObjectHasSelfExpression.h"




// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Parser {

		using namespace Expression;


		/*! 
		 *	\class		CTBoxAxiomBuilder
		 *	\author		Andreas Steigmiller
		 *	\version	0.1
		 *	\brief		TODO
		 */
		class CTBoxAxiomBuilder {
			// public methods
			public:
				//! Constructor
				CTBoxAxiomBuilder();

				//! Destructor
				virtual ~CTBoxAxiomBuilder();

				virtual CClassExpression* getClass(const QString &className) = 0;
				virtual CClassExpression* getClass(const QStringRef &className) = 0;
				virtual CObjectIndividualVariableExpression* getIndividualVariable(const QStringRef &individualVariableName, cint64 axiomNumber) = 0;
				virtual CObjectIndividualVariableExpression* getIndividualVariable(const QString &individualVariableName, cint64 axiomNumber) = 0;

				virtual CEquivalentClassesExpression *getEquivalentClasses(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CEquivalentClassesExpression *getEquivalentClasses(CBuildExpression *expression1, CBuildExpression *expression2) = 0;
				virtual CSubClassOfExpression *getSubClassOf(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CSubClassOfExpression *getSubClassOf(CBuildExpression *expression1, CBuildExpression *expression2) = 0;

				virtual CDisjointClassesExpression *getDisjointClasses(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CDisjointUnionExpression *getDisjointUnion(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;

				virtual CObjectComplementOfExpression *getObjectComplementOf(CBuildExpression *expression) = 0;
				virtual CObjectComplementOfExpression *getObjectComplementOf(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CObjectIntersectionOfExpression *getObjectIntersectionOf(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CObjectUnionOfExpression *getObjectUnionOf(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CObjectMaxCardinalityExpression *getObjectMaxCardinality(const CEXPRESSIONLIST<CBuildExpression *> &expressions, int cardinality) = 0;
				virtual CObjectMaxCardinalityExpression *getObjectMaxCardinality(CBuildExpression *expression1, CBuildExpression *expression2, int cardinality) = 0;
				virtual CObjectMinCardinalityExpression *getObjectMinCardinality(const CEXPRESSIONLIST<CBuildExpression *> &expressions, int cardinality) = 0;
				virtual CObjectMinCardinalityExpression *getObjectMinCardinality(CBuildExpression *expression1, CBuildExpression *expression2, int cardinality) = 0;
				virtual CObjectExactlyCardinalityExpression *getObjectExactlyCardinality(const CEXPRESSIONLIST<CBuildExpression *> &expressions, int cardinality) = 0;
				virtual CObjectExactlyCardinalityExpression *getObjectExactlyCardinality(CBuildExpression *expression1, CBuildExpression *expression2, int cardinality) = 0;
				virtual CObjectAllValuesFromExpression *getObjectAllValuesFrom(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CObjectAllValuesFromExpression *getObjectAllValuesFrom(CBuildExpression *expression1, CBuildExpression *expression2) = 0;
				virtual CObjectSomeValuesFromExpression *getObjectSomeValuesFrom(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CObjectSomeValuesFromExpression *getObjectSomeValuesFrom(CBuildExpression *expression1, CBuildExpression *expression2) = 0;
				virtual CObjectOneOfExpression *getObjectOneOf(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CObjectHasValueExpression *getObjectHasValue(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CObjectHasValueExpression *getObjectHasValue(CBuildExpression *expression1, CBuildExpression *expression2) = 0;
				virtual CObjectHasSelfExpression *getObjectHasSelf(const CEXPRESSIONLIST<CBuildExpression *> &expressions) = 0;
				virtual CObjectHasSelfExpression *getObjectHasSelf(CBuildExpression *expression) = 0;

				virtual CClassTermExpression *getTopClass() = 0;
				virtual CClassTermExpression *getBottomClass() = 0;

				virtual CEquivalentClassesExpression* getEquivalentClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) = 0;
				virtual CSubClassOfExpression* getSubClassOf(CClassTermExpression* expression1, CClassTermExpression* expression2) = 0;
				virtual CDisjointClassesExpression* getDisjointClasses(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) = 0;
				virtual CDisjointUnionExpression *getDisjointUnion(CClassExpression* classExpression, const CEXPRESSIONLIST<CClassTermExpression*>& expressions) = 0;
				virtual CObjectComplementOfExpression* getObjectComplementOf(CClassTermExpression* expression) = 0;
				virtual CObjectIntersectionOfExpression* getObjectIntersectionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) = 0;
				virtual CObjectUnionOfExpression* getObjectUnionOf(const CEXPRESSIONLIST<CClassTermExpression*>& expressions) = 0;
				virtual CObjectMaxCardinalityExpression* getObjectMaxCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) = 0;
				virtual CObjectMinCardinalityExpression* getObjectMinCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) = 0;
				virtual CObjectExactlyCardinalityExpression* getObjectExactlyCardinality(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2, int cardinality) = 0;
				virtual CObjectAllValuesFromExpression* getObjectAllValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) = 0;
				virtual CObjectSomeValuesFromExpression* getObjectSomeValuesFrom(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) = 0;
				virtual CObjectOneOfExpression *getObjectOneOf(const CEXPRESSIONLIST<CIndividualTermExpression*> &expressions) = 0;
				virtual CObjectHasValueExpression *getObjectHasValue(CObjectPropertyTermExpression *expression1, CIndividualTermExpression *expression2) = 0;
				virtual CObjectHasSelfExpression *getObjectHasSelf(CObjectPropertyTermExpression *expression) = 0;


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

#endif // KONCLUDE_PARSER_CTBOXAXIOMBUILDER_H
