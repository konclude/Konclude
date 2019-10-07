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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONINDEXCACHE_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONINDEXCACHE_H

// Libraries includes
#include <QCache>
#include <QHash>

#include <Redland.h>

// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandStoredTriplesData.h"
#include "CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler.h"
#include "CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData.h"
#include "CRedlandStoredTriplesIndividualAssertionIndexCacheData.h"
#include "CRedlandNodeHasher.h"
#include "CRedlandStoredTriplesRepresentativeTagSetHash.h"
#include "CRedlandStoredTriplesRepresentativeTagRolesSet.h"
#include "CRedlandStoredTriplesRepresentativeTagTypeConceptsSet.h"


// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Triples {


			/*! 
			 *
			 *		\class		CRedlandStoredTriplesIndividualAssertionIndexCache
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesIndividualAssertionIndexCache : public CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesIndividualAssertionIndexCache(CRedlandStoredTriplesData* tripleData);


					CRedlandStoredTriplesIndividualAssertionIndexCache* beginIndexing();
					CRedlandStoredTriplesIndividualAssertionIndexCache* completeIndexing();


					CRedlandStoredTriplesIndividualAssertionIndexCacheData* getIndividualAssertionIndexCacheData(cint64 individualId);
					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* getRepresentativeIndividualAssertionsIndexData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData);


					CRedlandStoredTriplesIndividualAssertionIndexCacheData* extendIndividualAssertionIndexCacheDataByTypeConcept(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData, CConcept* concept);
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* extendIndividualAssertionIndexCacheDataByOutgoingObjectRole(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData, CRole* role);
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* extendIndividualAssertionIndexCacheDataByIncomingObjectRole(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData, CRole* role);
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* extendIndividualAssertionIndexCacheDataByOutgoingDataRole(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData, CRole* role);


					cint64 getIndividualIdFromIndividualNode(librdf_node* node);
					cint64 getIndividualIdFromIndividualName(const QString& indiName);
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* getIndividualAssertionIndexCacheData(librdf_node* node, bool createIfNotExists = true);

					CRedlandStoredTriplesIndividualAssertionIndexCacheData* associateIndividualWithId(librdf_node* indiNode, cint64 indiId);
					CRedlandStoredTriplesIndividualAssertionIndexCache* setNextIndividualId(cint64 indiId);

					QSet<CConcept*>* getAssertedConceptSet();
					QSet<CRole*>* getAssertedObjectRoleSet();
					QSet<CRole*>* getAssertedDataRoleSet();

					cint64 getNextIndividualId();

					QString getRepresentativeCacheModelString();

				// protected methods
				protected:
					CRedlandStoredTriplesIndividualAssertionIndexCache* initRepresentativeStore();
					librdf_statement* initPartialFilteringStatement(const char* subjectFilterUri, const char* predicateFilterUri, const char* objectFilterUri);


					CRedlandStoredTriplesIndividualAssertionIndexCacheData* createWithRetrievalIndividualAssertionIndexCacheData(librdf_node* node, cint64* individualId, bool idStored);


					CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler* releaseCacheData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData);
					CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler* updateCacheData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData);


					CRedlandStoredTriplesIndividualAssertionIndexCache* writeRepresentativeTagRoleSetToStore(char encodingChar, CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData, CRedlandStoredTriplesRepresentativeTagRolesSet* newTagRoleSet, CRedlandStoredTriplesRepresentativeTagRolesSet* prevTagRoleSet, CRedlandStoredTriplesRepresentativeTagRolesSet* baseTagRoleSet);



				// protected variables
				protected:
					CRedlandStoredTriplesData* mTripleData;
					librdf_storage* mRepresentativeStorage = nullptr;
					librdf_model* mRepresentativeModel = nullptr;

					const char* mIndividualIdPredicateUri = "i:id";
					const char* mRepresentativeIdObjectPredicateUri = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
					const char* mRepresentativeNonAnonymousPredicateUri = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
					const char* mRepresentativeNonAnonymousObjectUri = "n";
					librdf_node* mRepresentativeNonAnonymousObjectNode = nullptr;

					librdf_statement* mIndividualNodeRetrievalByIdStatement;
					librdf_statement* mIndividualIdRetrievalByNodeStatement;
					librdf_statement* mRepresentativeTypeFilteringStatement;
					librdf_statement* mIndividualIdInsertionStatement;
					librdf_statement* mIndividualNonAnonymityInsertionStatement;


					QCache<CRedlandNodeHasher, CRedlandStoredTriplesIndividualAssertionIndexCacheData> mIndividualAssertionsIndexCache;
					QHash<cint64, CRedlandStoredTriplesIndividualAssertionIndexCacheData*> mIndividualIdAssertionsIndexDataHash;


					CRedlandStoredTriplesRepresentativeTagSetHash<CConcept*, CRedlandStoredTriplesRepresentativeTagTypeConceptsSet> mTagTypeConceptsSetHash;
					CRedlandStoredTriplesRepresentativeTagSetHash<CRole*, CRedlandStoredTriplesRepresentativeTagRolesSet> mTagOutgoingObjectRolesSetHash;
					CRedlandStoredTriplesRepresentativeTagSetHash<CRole*, CRedlandStoredTriplesRepresentativeTagRolesSet> mTagIncomingObjectRolesSetHash;
					CRedlandStoredTriplesRepresentativeTagSetHash<CRole*, CRedlandStoredTriplesRepresentativeTagRolesSet> mTagOutgoingDataRolesSetHash;


					cint64 mNextIndividualId = 0;

					bool mIndividualDataWritenToStore = false;

					

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONINDEXCACHE_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
