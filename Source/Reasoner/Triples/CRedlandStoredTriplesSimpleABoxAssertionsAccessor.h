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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_REASONER_GENERATOR_CREDLANDSTOREDTRIPLESSIMPLEABOXASSERTIONSACCESSOR_H
#define KONCLUDE_REASONER_GENERATOR_CREDLANDSTOREDTRIPLESSIMPLEABOXASSERTIONSACCESSOR_H


// Libraries includes
#include <QHash>
#include <QCache>


// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandUriHasher.h"
#include "CRedlandStoredTriplesIndividualAssertionIndexCache.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyTriplesAssertionsAccessor.h"
#include "Reasoner/Ontology/CIRIName.h"

#include "Reasoner/Triples/CRedlandStoredTriplesData.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Utilities::Container;
	using namespace Parser;

	namespace Reasoner {

		using namespace Ontology;

		namespace Triples {


			/*! 
			 *
			 *		\class		CRedlandStoredTriplesSimpleABoxAssertionsAccessor
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesSimpleABoxAssertionsAccessor : public COntologyTriplesAssertionsAccessor {
				// public methods
				public:
					CRedlandStoredTriplesSimpleABoxAssertionsAccessor(CConcreteOntology* ontology, CRedlandStoredTriplesIndividualAssertionIndexCache* individualAssertionIndexCache, CRedlandStoredTriplesData* tripleData);
					virtual ~CRedlandStoredTriplesSimpleABoxAssertionsAccessor();


					virtual bool visitIndividualAssertions(cint64 individualId, COntologyTriplesIndividualAssertionsVisitor* visitor, bool visitConceptAssertions = true, bool visitRoleAssertions = true, bool visitReverseRoleAssertions = true, bool visitDataAssertions = true);

					virtual bool visitIndividualName(cint64 individualId, COntologyTriplesIndividualNamesVisitor* visitor);
					virtual bool visitIndividualName(cint64 individualId, function<bool(const QString& indiName)> visitFunc);

					virtual bool visitIndividualAnonymity(cint64 individualId, function<bool(bool anonymous)> visitFunc);



					virtual bool visitIndividualId(const QString& indiName, function<bool(cint64 indiId)> visitFunc);
					virtual bool hasIndividualId(const QString& indiName);


					virtual bool visitAllAssertedConcepts(COntologyTriplesAssertedTypesVisitor* visitor);
					virtual bool visitAllAssertedObjectRoles(COntologyTriplesAssertedTypesVisitor* visitor);
					virtual bool visitAllAssertedDataRoles(COntologyTriplesAssertedTypesVisitor* visitor);


					virtual cint64 getMaxIndexedIndividualId();
					virtual cint64 getIndexedIndividualCount();



					virtual bool visitDataValueIndividuals(CRole* dataRole, CDataLiteral* dataLiteral, function<bool(cint64 indiId)> visitFunc);



				// protected functions
				protected:
					librdf_node* getRolePredicateNode(CRole* role);
					CDatatype* getDatatypeFromDatatypeUri(librdf_uri* uri);
					librdf_uri* getDatatypeUriFromDatatype(CDatatype* datatype);

					bool initializeDatatypeMaps();

					QString getAnonymousResolvedIndividualName(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData);

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					CRedlandStoredTriplesData* mTripleData;
					CRedlandStoredTriplesIndividualAssertionIndexCache* mIndividualAssertionIndexCache;

					QHash<CRole*, librdf_node*> mRolePredicateNodeHash;
					QHash<CRedlandUriHasher, CDatatype*> mDatatypeIriDatatypeHash;
					QHash<CDatatype*, librdf_uri*> mDatatypeDatatypeIriHash;
					bool mDatatypeHashInitialized = false;

					CDatatype* mDefaultDatatype;
					librdf_uri* mDefaultDatatypeUri;

					QString mAnonymousOntologyIdentifier;

			};



		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CREDLANDSTOREDTRIPLESSIMPLEABOXASSERTIONSACCESSOR_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
