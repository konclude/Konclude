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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSTRINGRESULT_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSTRINGRESULT_H

// Libraries includes
#include "CVariableBindingResult.h"

// Namespace includes

// Other includes
#include "Reasoner/Ontology/CIRIName.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CVariableBindingNamedIndividualResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingStringResult : public CVariableBindingResult {
				// public methods
				public:
					//! Constructor
					CVariableBindingStringResult();
					CVariableBindingStringResult(const QString& bindingString, VariableBindingType type);


					//! Destructor
					virtual ~CVariableBindingStringResult();

					CVariableBindingResult* initVariableBinding(const QString& bindingString, VariableBindingType type);
					virtual CVariableBindingResult* initVariableBinding(const CIndividualReference& indiRef, CIndividualNameResolver* indiNameResolver);
					virtual CVariableBindingResult* initVariableBinding(const CIndividualReference& indiRef, const QString& resolvedIndiName);
					virtual CVariableBindingResult* initVariableBinding(CIndividual* individual);
					virtual CVariableBindingResult* initVariableBinding(CConcept* concept);
					virtual CVariableBindingResult* initVariableBinding(CRole* role);
					virtual CVariableBindingResult* initVariableBinding(CDataLiteral* dataLiteral);


					virtual VariableBindingType getVariableBindingType();
					CVariableBindingStringResult* setVariableBindingType(VariableBindingType type);


					const QString& getBindingString();
					CVariableBindingStringResult* setBindingString(const QString& bindingString);


					virtual QString getQueryResultString();
					virtual bool isResultEquivalentTo(CQueryResult *otherQueryResult);

					virtual QString getClassBindingString();
					virtual QString getPropertyBindingString();
					virtual QString getNamedIndividualBindingString();
					virtual QString getLiteralDatatypeBindingString();
					virtual QString getLiteralDatavalueBindingString();

					virtual QPair<QString, QString> getLiteralDatatypeDatavalueBindingStringPair();



					virtual CVariableBindingResult* createCopy();

				// protected methods
				protected:
					virtual bool enfoceBindingString();

				// protected variables
				protected:
					QString mBindingString;
					VariableBindingType mType;


					static const QString PREFIX_XML_PLAINLITERAL_DATATYPE_STRING;
					static const QStringRef PREFIX_XML_PLAINLITERAL_DATATYPE_STRINGREF;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSTRINGRESULT_H
