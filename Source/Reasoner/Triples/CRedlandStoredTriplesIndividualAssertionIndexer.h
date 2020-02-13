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

#ifndef KONCLUDE_REASONER_GENERATOR_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONINDEXER_H
#define KONCLUDE_REASONER_GENERATOR_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONINDEXER_H


// Libraries includes
#include <QHash>
#include <QCache>


// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandNodeHasher.h"
#include "CRedlandStoredTriplesData.h"
#include "CRedlandStoredTriplesIndividualAssertionIndexCache.h"
#include "CRedlandStoredTriplesSimpleABoxAssertionsAccessor.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Triples {


			/*! 
			 *
			 *		\class		CRedlandStoredTriplesIndividualAssertionIndexer
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesIndividualAssertionIndexer {
				// public methods
				public:
					CRedlandStoredTriplesIndividualAssertionIndexer();
					virtual ~CRedlandStoredTriplesIndividualAssertionIndexer();


					bool indexABoxIndividuals(CConcreteOntology* updateConcreteOntology, COntologyTriplesData* ontologyTripleData);

					CRedlandStoredTriplesIndividualAssertionIndexCacheData* handleStatement(librdf_statement* statement, librdf_node* rdfTypePredicate, librdf_node* namedIndividualObject, CRedlandStoredTriplesIndividualAssertionIndexCacheData* lastIndiCacheData, librdf_node* lastSubjectNode);



				// protected functions
				protected:

					CRedlandStoredTriplesIndividualAssertionIndexCacheData* getIndividualAssertionIndexcacheData(librdf_node* subjectNode, CRedlandStoredTriplesIndividualAssertionIndexCacheData*& lastIndiCacheData, librdf_node*& lastSubjectNode);



				// protected variables
				protected:

					CRedlandStoredTriplesIndividualAssertionIndexCache* mIndividualAssertionIndexCache;

					
					class CRoleNodeData {
					public:
						CRoleNodeData(librdf_node* roleNode, CRole* role, bool dataRole) {
							mRole = role;
							mDataRole = dataRole;
							mRoleNode = roleNode;
						}

						librdf_node* mRoleNode;
						CRole* mRole;
						bool mDataRole;
					};


					class CConceptNodeData {
					public:
						CConceptNodeData(librdf_node* conceptNode, CConcept* concept) {
							mConcept = concept;
							mConceptNode = conceptNode;
						}

						librdf_node* mConceptNode;
						CConcept* mConcept;
					};

					QHash<CRedlandNodeHasher, CRoleNodeData*> mRoleNodeDataHash;
					QHash<CRedlandNodeHasher, CConceptNodeData*> mConceptNodeDataHash;


			};



		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONINDEXER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
