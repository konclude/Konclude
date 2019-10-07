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

#include "CRedlandStoredTriplesIndividualAssertionIndexCache.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			CRedlandStoredTriplesIndividualAssertionIndexCache::CRedlandStoredTriplesIndividualAssertionIndexCache(CRedlandStoredTriplesData* tripleData) : mIndividualAssertionsIndexCache(1500000) {
				mTripleData = tripleData;
				initRepresentativeStore();
			}


			CRedlandStoredTriplesIndividualAssertionIndexCache* CRedlandStoredTriplesIndividualAssertionIndexCache::beginIndexing() {
				return this;
			}

			CRedlandStoredTriplesIndividualAssertionIndexCache* CRedlandStoredTriplesIndividualAssertionIndexCache::completeIndexing() {
				for (QHash<cint64, CRedlandStoredTriplesIndividualAssertionIndexCacheData*>::const_iterator it = mIndividualIdAssertionsIndexDataHash.constBegin(), itEnd = mIndividualIdAssertionsIndexDataHash.constEnd(); it != itEnd; ++it) {
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData = it.value();
					if (cacheData->getExtensionData(false)) {
						updateCacheData(cacheData);
					}
				}
				return this;
			}



			CRedlandStoredTriplesIndividualAssertionIndexCache* CRedlandStoredTriplesIndividualAssertionIndexCache::initRepresentativeStore() {


				librdf_world* world = mTripleData->getRedlandWorld();

				librdf_storage* storage = mTripleData->getRedlandIndexedStorage();
				if (world) {
					mRepresentativeStorage = librdf_new_storage(world, "hashes", NULL, "hash-type='memory'");
				}

				if (mRepresentativeStorage) {
					mRepresentativeModel = librdf_new_model(world, mRepresentativeStorage, NULL);
				}
				

				if (mRepresentativeModel) {
					mIndividualNodeRetrievalByIdStatement = initPartialFilteringStatement(nullptr, mIndividualIdPredicateUri, nullptr);
					mIndividualIdRetrievalByNodeStatement = initPartialFilteringStatement(nullptr, mIndividualIdPredicateUri, nullptr);
					mIndividualIdInsertionStatement = initPartialFilteringStatement(nullptr, mIndividualIdPredicateUri, nullptr);
					mRepresentativeTypeFilteringStatement = initPartialFilteringStatement(nullptr, mRepresentativeIdObjectPredicateUri, nullptr);

					mRepresentativeNonAnonymousObjectNode = librdf_new_node_from_uri_string(mTripleData->getRedlandWorld(), (const unsigned char*)mRepresentativeNonAnonymousObjectUri);
					mIndividualNonAnonymityInsertionStatement = initPartialFilteringStatement(nullptr, mRepresentativeNonAnonymousPredicateUri, nullptr);
					librdf_statement_set_object(mIndividualNonAnonymityInsertionStatement, librdf_new_node_from_node(mRepresentativeNonAnonymousObjectNode));
				}

				return this;

			}



			librdf_statement* CRedlandStoredTriplesIndividualAssertionIndexCache::initPartialFilteringStatement(const char* subjectFilterUri, const char* predicateFilterUri, const char* objectFilterUri) {
				librdf_statement* partial_statement = librdf_new_statement(mTripleData->getRedlandWorld());
				librdf_node* subject = nullptr;
				if (subjectFilterUri) {
					subject = librdf_new_node_from_uri_string(mTripleData->getRedlandWorld(), (const unsigned char*)subjectFilterUri);
					librdf_statement_set_subject(partial_statement, subject);
				}
				librdf_node* predicate = nullptr;
				if (predicateFilterUri) {
					predicate = librdf_new_node_from_uri_string(mTripleData->getRedlandWorld(), (const unsigned char*)predicateFilterUri);
					librdf_statement_set_predicate(partial_statement, predicate);
				}
				librdf_node* object = nullptr;
				if (objectFilterUri) {
					object = librdf_new_node_from_uri_string(mTripleData->getRedlandWorld(), (const unsigned char*)objectFilterUri);
					librdf_statement_set_object(partial_statement, object);
				}
				return partial_statement;
			}





			CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler* CRedlandStoredTriplesIndividualAssertionIndexCache::releaseCacheData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData) {
				if (cacheData->getExtensionData(false)) {
					updateCacheData(cacheData);
				}
				CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* currentRepIndiAssIndData = cacheData->getCurrentRepresentativeIndividualAssertionsIndexData();
				CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* storedRepIndiAssIndData = cacheData->getStoredRepresentativeIndividualAssertionsIndexData();
				librdf_node* indiNode = cacheData->getIndividualNode();
				if (currentRepIndiAssIndData != storedRepIndiAssIndData) {
					// write update to store/model

					if (storedRepIndiAssIndData->getRepresentativeTypeConceptsSet() != currentRepIndiAssIndData->getRepresentativeTypeConceptsSet()) {
						librdf_statement* representativeTypeStatement = librdf_new_statement_from_statement(mRepresentativeTypeFilteringStatement);
						librdf_statement_set_subject(representativeTypeStatement, librdf_new_node_from_node(indiNode));

						if (storedRepIndiAssIndData->getRepresentativeTypeConceptsSet() != mTagTypeConceptsSetHash.getBaseTagSet()) {
							librdf_statement* representativeTypeObjectStatement = librdf_new_statement_from_statement(representativeTypeStatement);
							QByteArray prevRepTypeIdCharData = QString("t:%1").arg(storedRepIndiAssIndData->getRepresentativeTypeConceptsSet()->getRepresentativeId()).toUtf8();
							librdf_node* prevRepTypeNode = librdf_new_node_from_uri_string(mTripleData->getRedlandWorld(), (const unsigned char*)prevRepTypeIdCharData.constData());
							librdf_statement_set_object(representativeTypeObjectStatement, prevRepTypeNode);
							librdf_model_remove_statement(mRepresentativeModel, representativeTypeObjectStatement);
							librdf_free_statement(representativeTypeObjectStatement);
						}

						if (currentRepIndiAssIndData->getRepresentativeTypeConceptsSet() != mTagTypeConceptsSetHash.getBaseTagSet()) {
							librdf_statement* representativeTypeObjectStatement = librdf_new_statement_from_statement(representativeTypeStatement);
							QByteArray newRepTypeIdCharData = QString("t:%1").arg(currentRepIndiAssIndData->getRepresentativeTypeConceptsSet()->getRepresentativeId()).toUtf8();
							librdf_node* newRepTypeNode = librdf_new_node_from_uri_string(mTripleData->getRedlandWorld(), (const unsigned char*)newRepTypeIdCharData.constData());
							librdf_statement_set_object(representativeTypeObjectStatement, newRepTypeNode);
							librdf_model_add_statement(mRepresentativeModel, representativeTypeObjectStatement);
							mIndividualDataWritenToStore = true;
							librdf_free_statement(representativeTypeObjectStatement);
						}

						//librdf_free_statement(representativeTypeStatement);
					}
					currentRepIndiAssIndData->getRepresentativeTypeConceptsSet()->removeIndividualCacheData(cacheData, true);

					writeRepresentativeTagRoleSetToStore('o', cacheData, currentRepIndiAssIndData->getRepresentativeOutgoingObjectRolesSet(), storedRepIndiAssIndData->getRepresentativeOutgoingObjectRolesSet(), mTagOutgoingObjectRolesSetHash.getBaseTagSet());
					writeRepresentativeTagRoleSetToStore('i', cacheData, currentRepIndiAssIndData->getRepresentativeIncomingObjectRolesSet(), storedRepIndiAssIndData->getRepresentativeIncomingObjectRolesSet(), mTagIncomingObjectRolesSetHash.getBaseTagSet());
					writeRepresentativeTagRoleSetToStore('d', cacheData, currentRepIndiAssIndData->getRepresentativeOutgoingDataRolesSet(), storedRepIndiAssIndData->getRepresentativeOutgoingDataRolesSet(), mTagOutgoingDataRolesSetHash.getBaseTagSet());
				} else {
					currentRepIndiAssIndData->getRepresentativeTypeConceptsSet()->removeIndividualCacheData(cacheData, true);
					currentRepIndiAssIndData->getRepresentativeOutgoingObjectRolesSet()->removeIndividualCacheData(cacheData, true);
					currentRepIndiAssIndData->getRepresentativeIncomingObjectRolesSet()->removeIndividualCacheData(cacheData, true);
					currentRepIndiAssIndData->getRepresentativeOutgoingDataRolesSet()->removeIndividualCacheData(cacheData, true);
				}

				if (!cacheData->isAnonymous() && !cacheData->isIndividualNonAnonymityStored()) {
					librdf_statement* indiNonAnonStatement = librdf_new_statement_from_statement(mIndividualNonAnonymityInsertionStatement);
					librdf_statement_set_subject(indiNonAnonStatement, librdf_new_node_from_node(indiNode));
					librdf_model_add_statement(mRepresentativeModel, indiNonAnonStatement);
					//librdf_free_statement(indiNonAnonStatement);
					mIndividualDataWritenToStore = true;
				}

				if (!cacheData->isIndividualIdStored()) {
					librdf_statement* indiIdStatement = librdf_new_statement_from_statement(mIndividualIdInsertionStatement);
					librdf_statement_set_subject(indiIdStatement, librdf_new_node_from_node(indiNode));


					QByteArray indiIdCharData = QString::number(cacheData->getIndividualId()).toUtf8();
					librdf_node* indiIdNode = librdf_new_node_from_typed_counted_literal(mTripleData->getRedlandWorld(), (const unsigned char*)indiIdCharData.constData(), indiIdCharData.length(), nullptr, 0, nullptr);
					librdf_statement_set_object(indiIdStatement, indiIdNode);
					librdf_model_add_statement(mRepresentativeModel, indiIdStatement);
					//librdf_free_statement(indiIdStatement);
					mIndividualDataWritenToStore = true;
				}
				mIndividualIdAssertionsIndexDataHash.remove(cacheData->getIndividualId());
				librdf_free_node(indiNode);

				return this;
			}




			CRedlandStoredTriplesIndividualAssertionIndexCache* CRedlandStoredTriplesIndividualAssertionIndexCache::writeRepresentativeTagRoleSetToStore(char encodingChar, CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData, CRedlandStoredTriplesRepresentativeTagRolesSet* newTagRoleSet, CRedlandStoredTriplesRepresentativeTagRolesSet* prevTagRoleSet, CRedlandStoredTriplesRepresentativeTagRolesSet* baseTagRoleSet) {

				librdf_node* indiNode = cacheData->getIndividualNode();
				if (newTagRoleSet != prevTagRoleSet) {
					librdf_statement* representativeTypeStatement = librdf_new_statement_from_statement(mRepresentativeTypeFilteringStatement);
					librdf_statement_set_subject(representativeTypeStatement, librdf_new_node_from_node(indiNode));

					if (prevTagRoleSet != baseTagRoleSet) {
						librdf_node*& prevRepTypeNode = prevTagRoleSet->getRedlandTagNode();
						if (!prevRepTypeNode) {
							QByteArray prevRepTypeIdCharData = QString("%1:%2").arg(encodingChar).arg(prevTagRoleSet->getRepresentativeId()).toUtf8();
							prevRepTypeNode = librdf_new_node_from_uri_string(mTripleData->getRedlandWorld(), (const unsigned char*)prevRepTypeIdCharData.constData());
						}
						librdf_statement* representativeTypeObjectStatement = librdf_new_statement_from_statement(representativeTypeStatement);
						librdf_statement_set_object(representativeTypeObjectStatement, librdf_new_node_from_node(prevRepTypeNode));
						librdf_model_remove_statement(mRepresentativeModel, representativeTypeObjectStatement);
						librdf_free_statement(representativeTypeObjectStatement);
					}

					if (newTagRoleSet != baseTagRoleSet) {
						librdf_node*& newRepTypeNode = newTagRoleSet->getRedlandTagNode();
						if (!newRepTypeNode) {
							QByteArray newRepTypeIdCharData = QString("%1:%2").arg(encodingChar).arg(newTagRoleSet->getRepresentativeId()).toUtf8();
							newRepTypeNode = librdf_new_node_from_uri_string(mTripleData->getRedlandWorld(), (const unsigned char*)newRepTypeIdCharData.constData());
						}
						librdf_statement* representativeTypeObjectStatement = librdf_new_statement_from_statement(representativeTypeStatement);
						librdf_statement_set_object(representativeTypeObjectStatement, librdf_new_node_from_node(newRepTypeNode));
						librdf_model_add_statement(mRepresentativeModel, representativeTypeObjectStatement);
						mIndividualDataWritenToStore = true;
						librdf_free_statement(representativeTypeObjectStatement);
					}

					//librdf_free_statement(representativeTypeStatement);
				}
				newTagRoleSet->removeIndividualCacheData(cacheData, true);
				return this;
			}




			CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler* CRedlandStoredTriplesIndividualAssertionIndexCache::updateCacheData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData) {
				CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* storedRepIndiAssIndData = cacheData->getStoredRepresentativeIndividualAssertionsIndexData();
				CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* currentRepIndiAssIndData = cacheData->getCurrentRepresentativeIndividualAssertionsIndexData();
				CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData* extensionData = cacheData->getExtensionData(false);
				if (extensionData) {
					if (storedRepIndiAssIndData == currentRepIndiAssIndData) {
						currentRepIndiAssIndData = new CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData(storedRepIndiAssIndData);
						cacheData->setCurrentRepresentativeIndividualAssertionsIndexData(currentRepIndiAssIndData);
					}

					QMap<cint64, CConcept*>* typeConceptExtensionMap = extensionData->getTypeConceptExtensionMap(false);
					if (typeConceptExtensionMap) {
						CRedlandStoredTriplesRepresentativeTagTypeConceptsSet* repConSet = mTagTypeConceptsSetHash.update(currentRepIndiAssIndData->getRepresentativeTypeConceptsSet(), typeConceptExtensionMap, cacheData);
						currentRepIndiAssIndData->setRepresentativeTypeConceptsSet(repConSet);
					}

					QMap<cint64, CRole*>* outObjRoleExtensionMap = extensionData->getOutgoingObjectRoleExtensionMap(false);
					if (outObjRoleExtensionMap) {
						CRedlandStoredTriplesRepresentativeTagRolesSet* repRoleSet = mTagOutgoingObjectRolesSetHash.update(currentRepIndiAssIndData->getRepresentativeOutgoingObjectRolesSet(), outObjRoleExtensionMap, cacheData);
						currentRepIndiAssIndData->setRepresentativeOutgoingObjectRolesSet(repRoleSet);
					}

					QMap<cint64, CRole*>* incObjRoleExtensionMap = extensionData->getIncomingObjectRoleExtensionMap(false);
					if (incObjRoleExtensionMap) {
						CRedlandStoredTriplesRepresentativeTagRolesSet* repRoleSet = mTagIncomingObjectRolesSetHash.update(currentRepIndiAssIndData->getRepresentativeIncomingObjectRolesSet(), incObjRoleExtensionMap, cacheData);
						currentRepIndiAssIndData->setRepresentativeIncomingObjectRolesSet(repRoleSet);
					}

					QMap<cint64, CRole*>* outDataRoleExtensionMap = extensionData->getOutgoingDataRoleExtensionMap(false);
					if (outDataRoleExtensionMap) {
						CRedlandStoredTriplesRepresentativeTagRolesSet* repRoleSet = mTagOutgoingDataRolesSetHash.update(currentRepIndiAssIndData->getRepresentativeOutgoingDataRolesSet(), outDataRoleExtensionMap, cacheData);
						currentRepIndiAssIndData->setRepresentativeOutgoingDataRolesSet(repRoleSet);
					}

					cacheData->clearExtensionData();
				}
				return this;
			}




			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCache::getIndividualAssertionIndexCacheData(cint64 individualId) {
				CRedlandStoredTriplesIndividualAssertionIndexCacheData*& cacheData = mIndividualIdAssertionsIndexDataHash[individualId];
				if (!cacheData && mIndividualDataWritenToStore) {
					librdf_node* individualNode = nullptr;

					// retrieve individual node from model/store
					QByteArray individualIdCharData = QString::number(individualId).toUtf8();
					librdf_node* idNode = librdf_new_node_from_typed_counted_literal(mTripleData->getRedlandWorld(), (const unsigned char*)individualIdCharData.constData(), individualIdCharData.length(), nullptr, 0, nullptr);

					librdf_statement* individualNodeRetrievalByIdStatement = librdf_new_statement_from_statement(mIndividualNodeRetrievalByIdStatement);
					librdf_statement_set_object(individualNodeRetrievalByIdStatement, idNode);
					librdf_stream* stream = librdf_model_find_statements(mRepresentativeModel, individualNodeRetrievalByIdStatement);

					if (!librdf_stream_end(stream)) {
						individualNode = librdf_statement_get_subject(librdf_stream_get_object(stream));
						cacheData = createWithRetrievalIndividualAssertionIndexCacheData(individualNode, &individualId, true);
					}
					librdf_free_stream(stream);
					//librdf_free_statement(individualNodeRetrievalByIdStatement);

				}
				if (cacheData && cacheData->getExtensionData(false)) {
					updateCacheData(cacheData);
				}
				return cacheData;
			}


			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* CRedlandStoredTriplesIndividualAssertionIndexCache::getRepresentativeIndividualAssertionsIndexData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData) {
				if (indiData->getExtensionData(false)) {
					updateCacheData(indiData);
				}
				return indiData->getCurrentRepresentativeIndividualAssertionsIndexData();
			}



			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCache::associateIndividualWithId(librdf_node* indiNode, cint64 indiId) {
				return createWithRetrievalIndividualAssertionIndexCacheData(indiNode, &indiId, false);
			}

			CRedlandStoredTriplesIndividualAssertionIndexCache* CRedlandStoredTriplesIndividualAssertionIndexCache::setNextIndividualId(cint64 indiId) {
				mNextIndividualId = indiId;
				return this;
			}



			cint64 CRedlandStoredTriplesIndividualAssertionIndexCache::getIndividualIdFromIndividualName(const QString& indiName) {
				QByteArray indiNameByteArray = indiName.toUtf8();
				librdf_uri* indiUri = librdf_new_uri(mTripleData->getRedlandWorld(), (const unsigned char*)indiNameByteArray.data());
				librdf_node* indiNode = librdf_new_node_from_uri(mTripleData->getRedlandWorld(), indiUri);
				cint64 indiId = getIndividualIdFromIndividualNode(indiNode);
				librdf_free_node(indiNode);
				return indiId;
			}


			cint64 CRedlandStoredTriplesIndividualAssertionIndexCache::getIndividualIdFromIndividualNode(librdf_node* node) {
				CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData = getIndividualAssertionIndexCacheData(node, false);
				if (cacheData) {
					return cacheData->getIndividualId();
				}
				return -1;
			}


			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCache::getIndividualAssertionIndexCacheData(librdf_node* node, bool createIfNotExists) {
				CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData = mIndividualAssertionsIndexCache.object(CRedlandNodeHasher(node));
				if (!cacheData) {

					if (mIndividualDataWritenToStore) {
						librdf_node* indiNode = librdf_new_node_from_node(node);
						librdf_statement* individualIdRetrievalByNodeStatement = librdf_new_statement_from_statement(mIndividualIdRetrievalByNodeStatement);
						librdf_statement_set_subject(individualIdRetrievalByNodeStatement, indiNode);
						librdf_stream* stream = librdf_model_find_statements(mRepresentativeModel, individualIdRetrievalByNodeStatement);

						if (!librdf_stream_end(stream)) {
							librdf_node* individualIdNode = librdf_statement_get_object(librdf_stream_get_object(stream));
							cint64 individualId = QString::fromUtf8((const char*)librdf_node_get_literal_value(individualIdNode)).toLongLong();

							cacheData = createWithRetrievalIndividualAssertionIndexCacheData(node, &individualId, true);
						}
						librdf_free_stream(stream);
						//librdf_free_statement(individualIdRetrievalByNodeStatement);
					}

					if (!cacheData && createIfNotExists) {
						cacheData = createWithRetrievalIndividualAssertionIndexCacheData(node, nullptr, false);
					}

				}
				return cacheData;
			}



			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCache::createWithRetrievalIndividualAssertionIndexCacheData(librdf_node* node, cint64* individualIdPointer, bool idStored) {
				CRedlandStoredTriplesIndividualAssertionIndexCacheData* cacheData = mIndividualAssertionsIndexCache.object(CRedlandNodeHasher(node));
				if (!cacheData) {
					cint64 individualId = 0;
					if (individualIdPointer) {
						individualId = *individualIdPointer;
					} else {
						individualId = mNextIndividualId++;
					}

					librdf_node* individualNode = librdf_new_node_from_node(node);
					cacheData = new CRedlandStoredTriplesIndividualAssertionIndexCacheData(individualNode, individualId, this);
					if (individualIdPointer && idStored) {
						cacheData->setIndividualIdStored(true);
					}

					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* repIndiAssIndData = new CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData();
					cacheData->setCurrentRepresentativeIndividualAssertionsIndexData(repIndiAssIndData);
					cacheData->setStoredRepresentativeIndividualAssertionsIndexData(repIndiAssIndData);

					CRedlandStoredTriplesRepresentativeTagTypeConceptsSet* repTypeSet = mTagTypeConceptsSetHash.getBaseTagSet();
					CRedlandStoredTriplesRepresentativeTagRolesSet* outObjRoleSet = mTagOutgoingObjectRolesSetHash.getBaseTagSet();
					CRedlandStoredTriplesRepresentativeTagRolesSet* incObjRoleSet = mTagIncomingObjectRolesSetHash.getBaseTagSet();
					CRedlandStoredTriplesRepresentativeTagRolesSet* outDatRoleSet = mTagOutgoingDataRolesSetHash.getBaseTagSet();

					if (mIndividualDataWritenToStore && idStored) {
						// retrieve data from model/store
						librdf_node* subjectNode = librdf_new_node_from_node(node);
						librdf_statement* representativeTypeStatement = librdf_new_statement_from_statement(mRepresentativeTypeFilteringStatement);
						librdf_statement_set_subject(representativeTypeStatement, subjectNode);
						librdf_stream* stream = librdf_model_find_statements(mRepresentativeModel, representativeTypeStatement);

						while (!librdf_stream_end(stream)) {
							librdf_node* repIdNode = librdf_statement_get_object(librdf_stream_get_object(stream));
							if (repIdNode == mRepresentativeNonAnonymousObjectNode || librdf_node_equals(mRepresentativeNonAnonymousObjectNode, repIdNode)) {
								cacheData->setAnonymous(false);
								cacheData->setIndividualNonAnonymityStored(true);
							} else {
								librdf_uri* repIdUri = librdf_node_get_uri(repIdNode);
								const char* repChar = (const char*)librdf_uri_as_string(repIdUri);
								size_t repTypeCharLength = strlen(repChar);
								if (repTypeCharLength > 2) {
									if (repChar[0] == 't') {
										// types representative id object					
										cint64 repTypeId = QString::fromUtf8(repChar + 2).toLongLong();
										repTypeSet = mTagTypeConceptsSetHash.getRepresentativeTagSet(repTypeId);
									} else if (repChar[0] == 'o') {
										cint64 repId = QString::fromUtf8(repChar + 2).toLongLong();
										outObjRoleSet = mTagOutgoingObjectRolesSetHash.getRepresentativeTagSet(repId);
									} else if (repChar[0] == 'i') {
										cint64 repId = QString::fromUtf8(repChar + 2).toLongLong();
										incObjRoleSet = mTagIncomingObjectRolesSetHash.getRepresentativeTagSet(repId);
									} else if (repChar[0] == 'd') {
										cint64 repId = QString::fromUtf8(repChar + 2).toLongLong();
										outDatRoleSet = mTagOutgoingDataRolesSetHash.getRepresentativeTagSet(repId);
									}
								}
							}
							librdf_stream_next(stream);
						}
						librdf_free_stream(stream);
						//librdf_free_statement(representativeTypeStatement);
					}

					repIndiAssIndData->setRepresentativeTypeConceptsSet(repTypeSet);
					repIndiAssIndData->setRepresentativeOutgoingObjectRolesSet(outObjRoleSet);
					repIndiAssIndData->setRepresentativeIncomingObjectRolesSet(incObjRoleSet);
					repIndiAssIndData->setRepresentativeOutgoingDataRolesSet(outDatRoleSet);
					repTypeSet->addIndividualCacheData(cacheData, idStored);
					outObjRoleSet->addIndividualCacheData(cacheData, idStored);
					incObjRoleSet->addIndividualCacheData(cacheData, idStored);
					outDatRoleSet->addIndividualCacheData(cacheData, idStored);


					mIndividualIdAssertionsIndexDataHash[individualId] = cacheData;
					mIndividualAssertionsIndexCache.insert(CRedlandNodeHasher(individualNode), cacheData, 1);
				}
				return cacheData;
			}


			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCache::extendIndividualAssertionIndexCacheDataByTypeConcept(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData, CConcept* concept) {
				if (!indiData->getCurrentRepresentativeIndividualAssertionsIndexData()->getRepresentativeTypeConceptsSet()->hasTagElement(concept->getConceptTag())) {
					if (concept->getConceptTag() == 1) {
						bool debug = true;
					}
					indiData->getExtensionData(true)->getTypeConceptExtensionMap(true)->insert(concept->getConceptTag(), concept);
				}
				return indiData;
			}



			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCache::extendIndividualAssertionIndexCacheDataByOutgoingObjectRole(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData, CRole* role) {
				if (!indiData->getCurrentRepresentativeIndividualAssertionsIndexData()->getRepresentativeOutgoingObjectRolesSet()->hasTagElement(role->getRoleTag())) {
					indiData->getExtensionData(true)->getOutgoingObjectRoleExtensionMap(true)->insert(role->getRoleTag(), role);
				}
				return indiData;
			}


			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCache::extendIndividualAssertionIndexCacheDataByIncomingObjectRole(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData, CRole* role) {
				if (!indiData->getCurrentRepresentativeIndividualAssertionsIndexData()->getRepresentativeIncomingObjectRolesSet()->hasTagElement(role->getRoleTag())) {
					indiData->getExtensionData(true)->getIncomingObjectRoleExtensionMap(true)->insert(role->getRoleTag(), role);
				}
				return indiData;
			}


			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCache::extendIndividualAssertionIndexCacheDataByOutgoingDataRole(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiData, CRole* role) {
				if (!indiData->getCurrentRepresentativeIndividualAssertionsIndexData()->getRepresentativeOutgoingDataRolesSet()->hasTagElement(role->getRoleTag())) {
					indiData->getExtensionData(true)->getOutgoingDataRoleExtensionMap(true)->insert(role->getRoleTag(), role);
				}
				return indiData;
			}



			QSet<CConcept*>* CRedlandStoredTriplesIndividualAssertionIndexCache::getAssertedConceptSet() {
				return mTagTypeConceptsSetHash.getOccurringTagElementSet();
			}

			QSet<CRole*>* CRedlandStoredTriplesIndividualAssertionIndexCache::getAssertedObjectRoleSet() {
				return mTagOutgoingObjectRolesSetHash.getOccurringTagElementSet();
			}

			QSet<CRole*>* CRedlandStoredTriplesIndividualAssertionIndexCache::getAssertedDataRoleSet() {
				return mTagOutgoingDataRolesSetHash.getOccurringTagElementSet();
			}

			cint64 CRedlandStoredTriplesIndividualAssertionIndexCache::getNextIndividualId() {
				return mNextIndividualId;
			}

			QString CRedlandStoredTriplesIndividualAssertionIndexCache::getRepresentativeCacheModelString() {
				mIndividualAssertionsIndexCache.clear();
				librdf_uri* uri = librdf_new_uri(mTripleData->getRedlandWorld(), (const unsigned char*)"http://konclude.com/reprasentative-cache/model");
				unsigned char *string = librdf_model_to_string(mRepresentativeModel, uri, "turtle", NULL, NULL);
				librdf_free_uri(uri);
				return QString::fromUtf8((const char*)string);
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
