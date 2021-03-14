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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSTRINGDATARESULT_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSTRINGDATARESULT_H

// Libraries includes
#include "CVariableBindingStringResult.h"

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
			class CVariableBindingStringDataResult : public CVariableBindingStringResult {
				// public methods
				public:
					//! Constructor
					CVariableBindingStringDataResult();
					CVariableBindingStringDataResult(const QString& bindingString, VariableBindingType type);


					//! Destructor
					virtual ~CVariableBindingStringDataResult();

					virtual CVariableBindingResult* initVariableBinding(const CIndividualReference& indiRef, CIndividualNameResolver* indiNameResolver);
					virtual CVariableBindingResult* initVariableBinding(const CIndividualReference& indiRef, const QString& resolvedIndiName);

					virtual CVariableBindingResult* initVariableBinding(CConcept* concept);
					virtual CVariableBindingResult* initVariableBinding(CIndividual* individual);
					virtual CVariableBindingResult* initVariableBinding(CDataLiteral* dataLiteral);
					virtual CVariableBindingResult* initVariableBinding(CRole* role);


					void* getData();
					CDataLiteral* getDataLiteral();
					cint64 getDataIndividualId();

					virtual CVariableBindingResult* createCopy();

					virtual QPair<QString, QString> getLiteralDatatypeDatavalueBindingStringPair();

				// protected methods
				protected:
					virtual bool enfoceBindingString();

				// protected variables
				protected:
					void* mData;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSTRINGDATARESULT_H
