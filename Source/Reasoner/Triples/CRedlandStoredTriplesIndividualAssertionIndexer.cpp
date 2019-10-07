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

#include "CRedlandStoredTriplesIndividualAssertionIndexer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			CRedlandStoredTriplesIndividualAssertionIndexer::CRedlandStoredTriplesIndividualAssertionIndexer() {
			}

			CRedlandStoredTriplesIndividualAssertionIndexer::~CRedlandStoredTriplesIndividualAssertionIndexer() {
			}


			bool CRedlandStoredTriplesIndividualAssertionIndexer::indexABoxIndividuals(CConcreteOntology* updateConcreteOntology, COntologyTriplesData* ontologyTripleData) {
				
				bool newTriplesMapped = false;
				for (CTriplesData* tripleData : *ontologyTripleData->getAllTriplesData()) {
					CRedlandStoredTriplesData* redlandTriplesData = dynamic_cast<CRedlandStoredTriplesData*>(tripleData);

					mIndividualAssertionIndexCache = new CRedlandStoredTriplesIndividualAssertionIndexCache(redlandTriplesData);

					librdf_world* world = redlandTriplesData->getRedlandWorld();


					librdf_node* rdfTypePredicate = librdf_new_node_from_uri_string(world, (const unsigned char*)PREFIX_RDF_TYPE);
					librdf_node* namedIndividualObject = librdf_new_node_from_uri_string(world, (const unsigned char*)PREFIX_OWL_NAMED_INDIVIDUAL);


					QSet<CRole*>* activeRoleSet = updateConcreteOntology->getRBox()->getActivePropertyRoleSet(false);
					if (activeRoleSet) {
						for (CRole* role : *activeRoleSet) {
							QByteArray roleCharData = CIRIName::getRecentIRIName(role->getPropertyNameLinker()).toUtf8();
							librdf_node* roleNode = librdf_new_node_from_uri_string(world, (const unsigned char*)roleCharData.constData());
							mRoleNodeDataHash.insert(CRedlandNodeHasher(roleNode), new CRoleNodeData(roleNode, role, role->isDataRole()));
						}
					}


					CConceptNodeData* topConceptData = nullptr;
					QSet<CConcept*>* activeConceptSet = updateConcreteOntology->getTBox()->getActiveClassConceptSet(false);
					if (activeConceptSet) {
						for (CConcept* concept : *activeConceptSet) {
							QByteArray conceptCharData = CIRIName::getRecentIRIName(concept->getClassNameLinker()).toUtf8();
							librdf_node* conceptNode = librdf_new_node_from_uri_string(world, (const unsigned char*)conceptCharData.constData());
							CConceptNodeData* conceptData = new CConceptNodeData(conceptNode, concept);
							mConceptNodeDataHash.insert(CRedlandNodeHasher(conceptNode), conceptData);
							if (concept->getConceptTag() == 1) {
								topConceptData = conceptData;
							}
						}
					}


					QSet<CIndividual*>* activeIndiSet = updateConcreteOntology->getABox()->getActiveIndividualSet(false);
					if (activeIndiSet && !activeIndiSet->isEmpty()) {
						cint64 maxIndividualId = updateConcreteOntology->getABox()->getNextIndividualId(false);
						for (CIndividual* indi : *activeIndiSet) {
							maxIndividualId = qMax(indi->getIndividualID(), maxIndividualId);
							QByteArray indiCharData = CIRIName::getRecentIRIName(indi->getIndividualNameLinker()).toUtf8();
							librdf_node* indiNode = librdf_new_node_from_uri_string(world, (const unsigned char*)indiCharData.constData());
							mIndividualAssertionIndexCache->associateIndividualWithId(indiNode, indi->getIndividualID());
						}
						mIndividualAssertionIndexCache->setNextIndividualId(maxIndividualId + 1);
					}

					mIndividualAssertionIndexCache->beginIndexing();

					//for (cint64 loop = 0; true; loop++) {
					//	bool debug = true;

						for (CXLinker<librdf_statement*>* statementLinkerIt = redlandTriplesData->getRedlandStatementLinker(); statementLinkerIt; statementLinkerIt = statementLinkerIt->getNext()) {

						//librdf_stream* stream = librdf_model_as_stream(redlandTriplesData->getRedlandIndexedModel());
						//if (stream) {
							CRedlandStoredTriplesIndividualAssertionIndexCacheData* lastIndiCacheData = nullptr;
							librdf_node* lastSubjectNode = nullptr;

						//	while (!librdf_stream_end(stream)) {

						//		librdf_statement* statement = librdf_stream_get_object(stream);

								librdf_statement* statement = statementLinkerIt->getData();
								librdf_node* predicateNode = librdf_statement_get_predicate(statement);
								librdf_node* objectNode = librdf_statement_get_object(statement);
								librdf_node* subjectNode = librdf_statement_get_subject(statement);

								if (librdf_node_equals(predicateNode, rdfTypePredicate)) {
									if (librdf_node_equals(objectNode, namedIndividualObject)) {
										lastIndiCacheData = getIndividualAssertionIndexcacheData(subjectNode, lastIndiCacheData, lastSubjectNode);
										lastIndiCacheData->setAnonymous(false);
									} else {
										CConceptNodeData* conceptData = mConceptNodeDataHash.value(CRedlandNodeHasher(objectNode));
										if (conceptData) {
											lastIndiCacheData = getIndividualAssertionIndexcacheData(subjectNode, lastIndiCacheData, lastSubjectNode);
											if (librdf_node_is_resource(subjectNode)) {
												lastIndiCacheData->setAnonymous(false);
											}
											mIndividualAssertionIndexCache->extendIndividualAssertionIndexCacheDataByTypeConcept(lastIndiCacheData, conceptData->mConcept);
										}
									}
								} else {
									CRoleNodeData* roleData = mRoleNodeDataHash.value(CRedlandNodeHasher(predicateNode));
									if (roleData) {
										lastIndiCacheData = getIndividualAssertionIndexcacheData(subjectNode, lastIndiCacheData, lastSubjectNode);
										if (librdf_node_is_resource(subjectNode)) {
											lastIndiCacheData->setAnonymous(false);
										}
										if (roleData->mDataRole) {
											if (librdf_node_is_literal(objectNode)) {
												mIndividualAssertionIndexCache->extendIndividualAssertionIndexCacheDataByOutgoingDataRole(lastIndiCacheData, roleData->mRole);
											}
										} else {
											if (!librdf_node_is_literal(objectNode)) {
												mIndividualAssertionIndexCache->extendIndividualAssertionIndexCacheDataByOutgoingObjectRole(lastIndiCacheData, roleData->mRole);
												CRedlandStoredTriplesIndividualAssertionIndexCacheData* otherIndiCacheData = mIndividualAssertionIndexCache->getIndividualAssertionIndexCacheData(objectNode);
												if (librdf_node_is_resource(objectNode)) {
													otherIndiCacheData->setAnonymous(false);
												}
												mIndividualAssertionIndexCache->extendIndividualAssertionIndexCacheDataByIncomingObjectRole(otherIndiCacheData, roleData->mRole);
											}
										}
									}
								}

								/*librdf_stream_next(stream);
							}*/
						}
					//}

					mIndividualAssertionIndexCache->completeIndexing();


					updateConcreteOntology->getABox()->setNextIndividualId(mIndividualAssertionIndexCache->getNextIndividualId());

					ontologyTripleData->setTripleAssertionAccessor(new CRedlandStoredTriplesSimpleABoxAssertionsAccessor(updateConcreteOntology, mIndividualAssertionIndexCache, redlandTriplesData));


#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
					QString modelString = mIndividualAssertionIndexCache->getRepresentativeCacheModelString();
					QFile modelFile("./Debugging/RepresentativeCache/assertion-index-data.txt");
					if (modelFile.open(QIODevice::WriteOnly)) {
						modelFile.write(modelString.toUtf8());
					}
#endif

				}

				return newTriplesMapped;
			}

			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexer::getIndividualAssertionIndexcacheData(librdf_node* subjectNode, CRedlandStoredTriplesIndividualAssertionIndexCacheData*& lastIndiCacheData, librdf_node*& lastSubjectNode) {
				if (!lastIndiCacheData || !librdf_node_equals(lastSubjectNode, subjectNode)) {
					lastIndiCacheData = mIndividualAssertionIndexCache->getIndividualAssertionIndexCacheData(subjectNode);
					if (lastSubjectNode) {
						librdf_free_node(lastSubjectNode);
					}
					lastSubjectNode = librdf_new_node_from_node(subjectNode);
				}
				return lastIndiCacheData;
			}



		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
