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

#ifndef KONCLUDE_PARSER_CQUERYBUILDER_H
#define KONCLUDE_PARSER_CQUERYBUILDER_H


// Libraries includes
#include <QHash>
#include <QList>
#include <QString>


// Namespace includes
#include "Expressions/CBuildExpression.h"
#include "Expressions/CClassExpression.h"


// Other includes
#include "Expressions/CQueryIsClassSatisfiableExpression.h"
#include "Expressions/CQueryAreClassesEquivalentExpression.h"
#include "Expressions/CQueryAreClassesDisjointExpression.h"
#include "Expressions/CQueryIsClassSubsumedByExpression.h"
#include "Expressions/CQueryIsInstanceOfExpression.h"
#include "Expressions/CQueryGetFlattenedTypesExpression.h"
#include "Expressions/CQueryGetFlattenedInstancesExpression.h"
#include "Expressions/CQueryGetTypesExpression.h"
#include "Expressions/CQueryGetInstancesExpression.h"
#include "Expressions/CQueryGetSuperClassesExpression.h"
#include "Expressions/CQueryGetSubClassesExpression.h"
#include "Expressions/CQueryGetEquivalentClassesExpression.h"
#include "Expressions/CQueryIsEntailedExpression.h"
#include "Expressions/CQueryGetObjectPropertyTargetsExpression.h"
#include "Expressions/CQueryGetFlattenedObjectPropertyTargetsExpression.h"
#include "Expressions/CQueryGetDeterministicIndividualsExpression.h"
#include "Expressions/CQueryGetNondeterministicIndividualsExpression.h"
#include "Expressions/CQueryGetDeterministicClassAssertionsExpression.h"
#include "Expressions/CQueryGetNondeterministicClassAssertionsExpression.h"
#include "Expressions/CQueryGetSameIndividualsExpression.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Parser {

		using namespace Expression;


		/*! 
		*
		*		\class		CQueryBuilder
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CQueryBuilder {
			// public methods
			public:
				//! Constructor
				CQueryBuilder();

				//! Destructor
				virtual ~CQueryBuilder();


				virtual CQueryIsClassSatisfiableExpression* getIsClassSatisfiableQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) = 0;
				virtual CQueryAreClassesEquivalentExpression* getAreClassesEquivalenceQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) = 0;
				virtual CQueryAreClassesDisjointExpression* getAreClassesDisjointQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) = 0;
				virtual CQueryIsClassSubsumedByExpression* getIsClassSubsumedByQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) = 0;
				virtual CQueryIsInstanceOfExpression* getIsInstanceOfQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) = 0;
				virtual CQueryGetFlattenedTypesExpression* getGetFlattenedTypesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) = 0;
				virtual CQueryGetFlattenedInstancesExpression* getGetFlattenedInstancesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) = 0;
				virtual CQueryGetTypesExpression* getGetTypesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) = 0;
				virtual CQueryGetSameIndividualsExpression* getGetSameIndividualsQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) = 0;
				virtual CQueryGetInstancesExpression* getGetInstancesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) = 0;
				virtual CQueryGetSubClassesExpression* getGetSubClassesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) = 0;
				virtual CQueryGetSuperClassesExpression* getGetSuperClassesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, bool direct, const QString& queryName) = 0;
				virtual CQueryGetEquivalentClassesExpression* getGetEquivalentClassesQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) = 0;
				virtual CQueryIsEntailedExpression* getIsEntailedQuery(const CEXPRESSIONLIST<CBuildExpression*>& testClassExpressions, const QString& queryName) = 0;
				virtual CQueryGetObjectPropertyTargetsExpression* getGetObjectPropertyTargetsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) = 0;
				virtual CQueryGetFlattenedObjectPropertyTargetsExpression* getGetFlattenedObjectPropertyTargetsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) = 0;


				virtual CQueryGetDeterministicIndividualsExpression* getGetDeterministicIndividualsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) = 0;
				virtual CQueryGetNondeterministicIndividualsExpression* getGetNondeterministicIndividualsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) = 0;
				virtual CQueryGetDeterministicClassAssertionsExpression* getGetDeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) = 0;
				virtual CQueryGetNondeterministicClassAssertionsExpression* getGetNondeterministicClassAssertionsQuery(const CEXPRESSIONLIST<CBuildExpression*>& expressions, const QString& queryName) = 0;


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

#endif // KONCLUDE_PARSER_CQUERYBUILDER_H
