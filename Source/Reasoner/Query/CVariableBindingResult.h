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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGRESULT_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGRESULT_H

// Libraries includes
#include "CQueryResult.h"

// Namespace includes

// Other includes
#include "Reasoner/Ontology/CDataLiteral.h"
#include "Reasoner/Ontology/CIndividual.h"
#include "Reasoner/Ontology/CIndividualReference.h"
#include "Reasoner/Ontology/CIndividualNameResolver.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CVariableBindingResult
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingResult : public CQueryResult {
				// public methods
				public:
					//! Constructor
					CVariableBindingResult();

					//! Destructor
					virtual ~CVariableBindingResult();

					virtual CVariableBindingResult* initVariableBinding(const CIndividualReference& indiRef, CIndividualNameResolver* indiNameResolver) = 0;
					virtual CVariableBindingResult* initVariableBinding(const CIndividualReference& indiRef, const QString& resolvedIndiName) = 0;
					virtual CVariableBindingResult* initVariableBinding(CIndividual* individual) = 0;
					virtual CVariableBindingResult* initVariableBinding(CDataLiteral* dataLiteral) = 0;


					enum VariableBindingType { VBTNAMEDINDIVIDUAL, VBTANONYMOUSINDIVIDUAL, VBTLITERAL };

					virtual VariableBindingType getVariableBindingType() = 0;

					bool isNamedIndividualBindingType();
					bool isAnonymousIndividualBindingType();
					bool isLiteralBindingType();

					virtual const QString& getBindingString() = 0;
					virtual QString getNamedIndividualBindingString() = 0;
					virtual QString getLiteralDatatypeBindingString() = 0;
					virtual QString getLiteralDatavalueBindingString() = 0;
					virtual QPair<QString, QString> getLiteralDatatypeDatavalueBindingStringPair();

					virtual CVariableBindingResult* createCopy() = 0;

				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGRESULT_H
