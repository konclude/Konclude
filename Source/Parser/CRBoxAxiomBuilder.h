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

#ifndef KONCLUDE_PARSER_CRBOXAXIOMBUILDER_H
#define KONCLUDE_PARSER_CRBOXAXIOMBUILDER_H


// Libraries includes
#include <QHash>
#include <QList>


// Namespace includes
#include "Parser/Expressions/CBuildExpression.h"

#include "Parser/Expressions/CObjectPropertyExpression.h"
#include "Parser/Expressions/CDataPropertyExpression.h"
#include "Parser/Expressions/CSubObjectPropertyOfExpression.h"
#include "Parser/Expressions/CInverseObjectPropertyOfExpression.h"
#include "Parser/Expressions/CDisjointObjectPropertiesExpression.h"
#include "Parser/Expressions/CEquivalentObjectPropertiesExpression.h"

#include "Parser/Expressions/CObjectPropertyChainExpression.h"

#include "Parser/Expressions/CObjectPropertyDomainExpression.h"
#include "Parser/Expressions/CObjectPropertyRangeExpression.h"

#include "Parser/Expressions/CInverseObjectPropertiesExpression.h"
#include "Parser/Expressions/CTransetiveObjectPropertyExpression.h"
#include "Parser/Expressions/CFunctionalObjectPropertyExpression.h"
#include "Parser/Expressions/CSymmetricObjectPropertyExpression.h"
#include "Parser/Expressions/CAsymmetricObjectPropertyExpression.h"
#include "Parser/Expressions/CReflexiveObjectPropertyExpression.h"
#include "Parser/Expressions/CIrreflexiveObjectPropertyExpression.h"
#include "Parser/Expressions/CInverseFunctionalObjectPropertyExpression.h"

#include "Parser/Expressions/CSubDataPropertyOfExpression.h"
#include "Parser/Expressions/CFunctionalDataPropertyExpression.h"
#include "Parser/Expressions/CDisjointDataPropertiesExpression.h"
#include "Parser/Expressions/CEquivalentDataPropertiesExpression.h"
#include "Parser/Expressions/CDataPropertyDomainExpression.h"
#include "Parser/Expressions/CDataPropertyRangeExpression.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Parser {

		using namespace Expression;


		/*! 
		 *	\class		CRBoxAxiomBuilder
		 *	\author		Andreas Steigmiller
		 *	\version	0.1
		 *	\brief		TODO
		 */
		class CRBoxAxiomBuilder {
			// public methods
			public:
				//! Constructor
				CRBoxAxiomBuilder();

				//! Destructor
				virtual ~CRBoxAxiomBuilder();


				virtual CObjectPropertyExpression* getObjectProberty(const QString& probertyName) = 0;
				virtual CObjectPropertyExpression* getObjectProberty(const QStringRef& probertyName) = 0;
				virtual CObjectPropertyTermExpression* getTopObjectProberty() = 0;


				virtual CDataPropertyExpression* getDataProberty(const QString& dataPropertyName) = 0;
				virtual CDataPropertyExpression* getDataProberty(const QStringRef& dataPropertyName) = 0;

				virtual CSubDataPropertyOfExpression* getSubDataPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CSubDataPropertyOfExpression* getSubDataPropertyOf(CBuildExpression* expression1, CBuildExpression* expression2) = 0;

				virtual CEquivalentDataPropertiesExpression* getEquivalentDataProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CEquivalentDataPropertiesExpression* getEquivalentDataProperties(CBuildExpression* expression1, CBuildExpression* expression2) = 0;
				virtual CDisjointDataPropertiesExpression* getDisjointDataProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CDisjointDataPropertiesExpression* getDisjointDataProperties(CBuildExpression* expression1, CBuildExpression* expression2) = 0;

				virtual CDataPropertyDomainExpression* getDataPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2) = 0;
				virtual CDataPropertyDomainExpression* getDataPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CDataPropertyRangeExpression* getDataPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2) = 0;
				virtual CDataPropertyRangeExpression* getDataPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;

				virtual CFunctionalDataPropertyExpression* getFunctionalDataProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CFunctionalDataPropertyExpression* getFunctionalDataProperty(CBuildExpression* expression) = 0;



				virtual CSubDataPropertyOfExpression* getSubDataPropertyOf(CDataPropertyTermExpression* expression1, CDataPropertyTermExpression* expression2) = 0;

				virtual CEquivalentDataPropertiesExpression* getEquivalentDataProperties(const CEXPRESSIONLIST<CDataPropertyTermExpression*>& expressions) = 0;
				virtual CDisjointDataPropertiesExpression* getDisjointDataProperties(const CEXPRESSIONLIST<CDataPropertyTermExpression*>& expressions) = 0;


				virtual CDataPropertyDomainExpression* getDataPropertyDomainExpression(CDataPropertyTermExpression* expression1, CClassTermExpression* expression2) = 0;
				virtual CDataPropertyRangeExpression* getDataPropertyRangeExpression(CDataPropertyTermExpression* expression1, CDataRangeTermExpression* expression2) = 0;
				virtual CFunctionalDataPropertyExpression* getFunctionalDataProperty(CDataPropertyTermExpression* expression) = 0;






				virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(CBuildExpression* expression1, CBuildExpression* expression2) = 0;

				virtual CObjectPropertyChainExpression* getObjectPropertyChain(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CObjectPropertyChainExpression* getObjectPropertyChain(CBuildExpression* expression1, CBuildExpression* expression2) = 0;

				virtual CEquivalentObjectPropertiesExpression* getEquivalentObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CEquivalentObjectPropertiesExpression* getEquivalentObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) = 0;
				virtual CDisjointObjectPropertiesExpression* getDisjointObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CDisjointObjectPropertiesExpression* getDisjointObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) = 0;
				virtual CInverseObjectPropertiesExpression* getInverseObjectProperties(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CInverseObjectPropertiesExpression* getInverseObjectProperties(CBuildExpression* expression1, CBuildExpression* expression2) = 0;

				virtual CObjectPropertyDomainExpression* getObjectPropertyDomainExpression(CBuildExpression* expression1, CBuildExpression* expression2) = 0;
				virtual CObjectPropertyDomainExpression* getObjectPropertyDomainExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CObjectPropertyRangeExpression* getObjectPropertyRangeExpression(CBuildExpression* expression1, CBuildExpression* expression2) = 0;
				virtual CObjectPropertyRangeExpression* getObjectPropertyRangeExpression(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;


				virtual CTransetiveObjectPropertyExpression* getTransetiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CTransetiveObjectPropertyExpression* getTransetiveObjectProperty(CBuildExpression* expression) = 0;

				virtual CFunctionalObjectPropertyExpression* getFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CFunctionalObjectPropertyExpression* getFunctionalObjectProperty(CBuildExpression* expression) = 0;
				virtual CInverseFunctionalObjectPropertyExpression* getInverseFunctionalObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CInverseFunctionalObjectPropertyExpression* getInverseFunctionalObjectProperty(CBuildExpression* expression) = 0;
				virtual CSymmetricObjectPropertyExpression* getSymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CSymmetricObjectPropertyExpression* getSymmetricObjectProperty(CBuildExpression* expression) = 0;
				virtual CAsymmetricObjectPropertyExpression* getAsymmetricObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CAsymmetricObjectPropertyExpression* getAsymmetricObjectProperty(CBuildExpression* expression) = 0;
				virtual CReflexiveObjectPropertyExpression* getReflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CReflexiveObjectPropertyExpression* getReflexiveObjectProperty(CBuildExpression* expression) = 0;
				virtual CIrreflexiveObjectPropertyExpression* getIrreflexiveObjectProperty(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CIrreflexiveObjectPropertyExpression* getIrreflexiveObjectProperty(CBuildExpression* expression) = 0;

				virtual CInverseObjectPropertyOfExpression* getInverseObjectPropertyOf(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CInverseObjectPropertyOfExpression* getInverseObjectPropertyOf(CBuildExpression* expression) = 0;



				virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions, CObjectPropertyTermExpression* expression) = 0;
				virtual CSubObjectPropertyOfExpression* getSubObjectPropertyOf(CObjectPropertyChainExpression* expressions, CObjectPropertyTermExpression* expression) = 0;
				virtual CObjectPropertyChainExpression* getObjectPropertyChain(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) = 0;

				virtual CEquivalentObjectPropertiesExpression* getEquivalentObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) = 0;
				virtual CDisjointObjectPropertiesExpression* getDisjointObjectProperties(const CEXPRESSIONLIST<CObjectPropertyTermExpression*>& expressions) = 0;
				virtual CInverseObjectPropertiesExpression* getInverseObjectProperties(CObjectPropertyTermExpression* expression1, CObjectPropertyTermExpression* expression2) = 0;

				virtual CInverseObjectPropertyOfExpression* getInverseObjectPropertyOf(CObjectPropertyTermExpression* expression) = 0;

				virtual CObjectPropertyDomainExpression* getObjectPropertyDomainExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) = 0;
				virtual CObjectPropertyRangeExpression* getObjectPropertyRangeExpression(CObjectPropertyTermExpression* expression1, CClassTermExpression* expression2) = 0;
				virtual CTransetiveObjectPropertyExpression* getTransetiveObjectProperty(CObjectPropertyTermExpression* expression) = 0;
				virtual CFunctionalObjectPropertyExpression* getFunctionalObjectProperty(CObjectPropertyTermExpression* expression) = 0;
				virtual CInverseFunctionalObjectPropertyExpression* getInverseFunctionalObjectProperty(CObjectPropertyTermExpression* expression) = 0;
				virtual CSymmetricObjectPropertyExpression* getSymmetricObjectProperty(CObjectPropertyTermExpression* expression) = 0;
				virtual CAsymmetricObjectPropertyExpression* getAsymmetricObjectProperty(CObjectPropertyTermExpression* expression) = 0;
				virtual CReflexiveObjectPropertyExpression* getReflexiveObjectProperty(CObjectPropertyTermExpression* expression) = 0;
				virtual CIrreflexiveObjectPropertyExpression* getIrreflexiveObjectProperty(CObjectPropertyTermExpression* expression) = 0;


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

#endif // KONCLUDE_PARSER_CRBOXAXIOMBUILDER_H
