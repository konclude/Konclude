/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYTRIPLESASSERTIONSACCESSOR_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYTRIPLESASSERTIONSACCESSOR_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyTriplesIndividualAssertionsVisitor.h"
#include "COntologyTriplesAssertedTypesVisitor.h"
#include "COntologyTriplesIndividualNamesVisitor.h"
#include "CIndividual.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyTriplesAssertionsAccessor
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyTriplesAssertionsAccessor {
				// public methods
				public:
					//! Constructor
					COntologyTriplesAssertionsAccessor();


					virtual bool visitIndividualAssertions(cint64 individualId, COntologyTriplesIndividualAssertionsVisitor* visitor, bool visitConceptAssertions = true, bool visitRoleAssertions = true, bool visitReverseRoleAssertions = true, bool visitDataAssertions = true) = 0;
					virtual bool visitIndividualAssertions(CIndividual* individual, COntologyTriplesIndividualAssertionsVisitor* visitor, bool visitConceptAssertions = true, bool visitRoleAssertions = true, bool visitReverseRoleAssertions = true, bool visitDataAssertions = true);

					virtual bool visitIndividualName(cint64 individualId, COntologyTriplesIndividualNamesVisitor* visitor) = 0;
					virtual bool visitIndividualName(cint64 individualId, function<bool(const QString& indiName)> visitFunc) = 0;
					virtual bool visitIndividualAnonymity(cint64 individualId, function<bool(bool anonymous)> visitFunc) = 0;

					virtual bool visitIndividualId(const QString& indiName, function<bool(cint64 indiId)> visitFunc) = 0;
					virtual bool hasIndividualId(const QString& indiName) = 0;
					virtual cint64 getIndividualId(const QString& indiName);

					virtual bool visitAllAssertedConcepts(COntologyTriplesAssertedTypesVisitor* visitor) = 0;
					virtual bool visitAllAssertedObjectRoles(COntologyTriplesAssertedTypesVisitor* visitor) = 0;
					virtual bool visitAllAssertedDataRoles(COntologyTriplesAssertedTypesVisitor* visitor) = 0;



					virtual cint64 getMaxIndexedIndividualId() = 0;
					virtual cint64 getIndexedIndividualCount() = 0;


					virtual bool visitDataValueIndividuals(CRole* dataRole, CDataLiteral* dataLiteral, function<bool(cint64 indiId)> visitFunc) = 0;


				// protected methods
				protected:

				// protected variables
				protected:


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYTRIPLESASSERTIONSACCESSOR_H
