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

#include "COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {

			namespace Persistence {

				COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer(CConfiguration* config) : CLogIdentifier("::Konclude::Reasoner::Persistence::RedlandExternalTriplesDSNOnlyWritingPersistencer", this) {
					mConfig = config;

				
					
					mDsnConfig = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.RedlandExternalTriplesDSNOnlyWritingPersistencer.DSNConfig");
					mTriplesDBName = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.RedlandExternalTriplesDSNOnlyWritingPersistencer.TriplesDatabaseName");

					mConfInitialLoading = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Persistence.RedlandExternalTriplesDSNOnlyWritingPersistencer.InitialLoading");
					mConfDirectFullLoading = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Persistence.RedlandExternalTriplesDSNOnlyWritingPersistencer.DirectFullTriplesLoading");
				}

				COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::~COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer() {
				}



				COntologyRevisionRedlandExternalStoredTriplesConnectionData* COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::getOntologyPersistingTriplesData(const QString& ontName) {

					COntologyRevisionRedlandExternalStoredTriplesConnectionData* triplesData = mOntologyTriplesDataHash.value(ontName);

					if (!triplesData) {

						triplesData = new COntologyRevisionRedlandExternalStoredTriplesConnectionData();
						initTriplesData(triplesData, ontName);
						mOntologyTriplesDataHash.insert(ontName, triplesData);

					}
					return triplesData;
				}






				CRedlandStoredTriplesData* COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::getUpdatingTripleData() {

					bool localTriplesDataFlag = false;
					CRedlandStoredTriplesData* tripleData = nullptr;

					if (!tripleData) {
						tripleData = new CRedlandStoredTriplesData();
						tripleData->initTriplesData(TRIPLES_DATA_ADDITION, nullptr);
					}

					librdf_world* world = tripleData->getRedlandWorld();
					if (!world) {
						world = librdf_new_world();
						librdf_world_open(world);
						tripleData->setRedlandWorldData(world);
					}

					librdf_storage* indexedStorage = tripleData->getRedlandIndexedStorage();
					if (!indexedStorage && world) {
						indexedStorage = librdf_new_storage(world, "hashes", NULL, "hash-type='memory',index-predicates='yes'");
						tripleData->setRedlandIndexedStorageData(indexedStorage);
					}

					librdf_model* indexedModel = tripleData->getRedlandIndexedModel();
					if (!indexedModel && indexedStorage) {
						indexedModel = librdf_new_model(world, indexedStorage, NULL);
						tripleData->setRedlandIndexedModelData(indexedModel);
					}
					/*else {
					model = librdf_new_model_from_model(prevModel);
					tripleData->setRedlandModelData(model);
					}*/


					if (!indexedModel) {
						delete tripleData;
						tripleData = nullptr;
					}

					return tripleData;

				}



				COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer* COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::initTriplesData(CRedlandStoredTriplesData* triplesData, const QString& ontName) {
					librdf_world* world = triplesData->getRedlandWorld();
					if (!world) {
						world = librdf_new_world();
						librdf_world_open(world);
						triplesData->setRedlandWorldData(world);
					}


					librdf_storage* indexedStorage = triplesData->getRedlandIndexedStorage();
					if (!indexedStorage && world) {
						//indexedStorage = librdf_new_storage(world, "virtuoso", "http://LUBM-test", "dsn='VirtuosoTest',user='dba',password='dba'");

						QString dsnConfigString = mDsnConfig;
						QString dbNameString = mTriplesDBName;

						if (dbNameString.contains("$KnowledgeBaseName")) {
							dbNameString = dbNameString.replace("$KnowledgeBaseName", ontName);
						}
						if (dbNameString.contains("$PercentEncodedKnowledgeBaseName")) {
							dbNameString = dbNameString.replace("$PercentEncodedKnowledgeBaseName", QUrl::toPercentEncoding(ontName));
						}

						QByteArray triplesDBNameArray = dbNameString.toUtf8();
						QByteArray dsnConfigArray = dsnConfigString.toUtf8();

						indexedStorage = librdf_new_storage(world, "virtuoso", triplesDBNameArray.constData(), dsnConfigArray.constData());
						triplesData->setRedlandIndexedStorageData(indexedStorage);
					}

					librdf_model* indexedModel = triplesData->getRedlandIndexedModel();
					if (!indexedModel && indexedStorage) {
						indexedModel = librdf_new_model(world, indexedStorage, NULL);
						triplesData->setRedlandIndexedModelData(indexedModel);
					}
					return this;
				}




				bool COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::persistOntologyRevision(COntologyRevision* ontoRev, COntologyRevision* prevOntoRev) {

					if (mOntologyNameFileHash.contains(prevOntoRev) && ontoRev->getOntology()->getBuildData()->getChangeAxiomList()->size() == prevOntoRev->getOntology()->getBuildData()->getChangeAxiomList()->size()) {
						COntologyTriplesData* triplesData = ontoRev->getOntology()->getOntologyTriplesData();
						COntologyTriplesData* prevTriplesData = prevOntoRev->getOntology()->getOntologyTriplesData();
						if (prevTriplesData && triplesData && triplesData->getLatestTriplesData(false) == prevTriplesData->getLatestTriplesData(false) || !prevTriplesData && !triplesData) {
							mOntologyNameFileHash.insert(ontoRev, mOntologyNameFileHash.value(prevOntoRev));
							return true;
						}
					}
					
					CConcreteOntology* ontology = ontoRev->getOntology();
					QString ontName = ontology->getOntologyName();
					cint64 ontoVers = ontoRev->getOntologyRevisionDepth();

					COntologyRevisionRedlandExternalStoredTriplesConnectionData* triplesData = getOntologyPersistingTriplesData(ontName);
					if (triplesData) {
						COntologyAxiomExpressionsRenderVisitor renderer;
						COntologyRedlandRDFRenderer rdfRenderer(triplesData->getRedlandWorld());

						CONTOLOGYAXIOMSET<CAxiomExpression*>* currAxiomSet = ontology->getBuildData()->getTellAxiomSet();
						CQtList<CAxiomExpression*> axiomList;
						for (CAxiomExpression* axiomExp : *currAxiomSet) {
							if (!triplesData->getPersistedAxiomExpressionSet()->contains(axiomExp)) {
								axiomList.append(axiomExp);
							}
						}
						QList<CTriplesData*> triplesDataList;
						COntologyTriplesData* ontTriplesData = ontoRev->getOntology()->getOntologyTriplesData();
						if (ontTriplesData) {
							for (CTriplesData* tripleData : *ontTriplesData->getAllTriplesData()) {
								if (!triplesData->getPersistedTriplesDataSet()->contains(tripleData)) {
									triplesDataList.append(tripleData);
								}
							}
						}


						LOG(INFO, getLogDomain(), logTr("Preparing new revision %2 of ontology '%1' for storing in external triple store").arg(ontName).arg(ontoVers), this);
						renderer.visitAndRender(axiomList, ontology, &rdfRenderer);
						renderer.visitAndRender(&triplesDataList, ontology, &rdfRenderer);

						if (!triplesData->isInitiallyLoaded() && mConfInitialLoading) {
							loadLatestOntologyRevision(ontName, ontoRev);
						}
						triplesData->setInitiallyLoaded(true);

						LOG(INFO, getLogDomain(), logTr("Trying saving new revision %2 of ontology '%1' in external triple store").arg(ontName).arg(ontoVers), this);
						triplesData = (COntologyRevisionRedlandExternalStoredTriplesConnectionData*)rdfRenderer.getRenderedTripleData(triplesData);
						LOG(INFO, getLogDomain(), logTr("Saved new revision %2 of ontology '%1' in external triple store").arg(ontName).arg(ontoVers), this);


						CXLinker<librdf_statement*>* statementLinker = triplesData->getRedlandStatementLinker();
						for (CAxiomExpression* axiomExp : *currAxiomSet) {
							triplesData->getPersistedAxiomExpressionSet()->insert(axiomExp);
						}
						if (ontTriplesData) {
							for (CTriplesData* tripleData : *ontTriplesData->getAllTriplesData()) {
								triplesData->getPersistedTriplesDataSet()->insert(tripleData);
							}
						}

						for (CXLinker<librdf_statement*>* statementLinkerIt = statementLinker; statementLinkerIt; ) {
							CXLinker<librdf_statement*>* tmpStatementLinker = statementLinkerIt;
							statementLinkerIt = statementLinkerIt->getNext();

							librdf_statement* statement = tmpStatementLinker->getData();
							librdf_free_statement(statement);

							delete tmpStatementLinker;
						}
						triplesData->setRedlandStatementLinker(nullptr);
					}

					return true;
				}



				bool COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::hasOntologyPersisted(const QString& ontologyName) {

					COntologyRevisionRedlandExternalStoredTriplesConnectionData* triplesData = getOntologyPersistingTriplesData(ontologyName);
					bool hasOntologyPersisted = false;
					if (triplesData) {

						librdf_stream* stream = librdf_model_as_stream(triplesData->getRedlandIndexedModel());
						if (stream) {
							if (!librdf_stream_end(stream)) {
								hasOntologyPersisted = true;
							}
							librdf_free_stream(stream);
						}
					}

					return hasOntologyPersisted;
				}



				bool COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::hasNewOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {

					if (hasOntologyPersisted(ontologyName)) {
						COntologyRevisionRedlandExternalStoredTriplesConnectionData* triplesData = getOntologyPersistingTriplesData(ontologyName);
						if (triplesData && !triplesData->isInitiallyLoaded()) {
							return true;
						}
					}

					return false;
				}


				bool COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::deletePersistedOntology(const QString& ontologyName) {
					CRedlandStoredTriplesData* triplesData = new CRedlandStoredTriplesData();
					initTriplesData(triplesData, ontologyName);

					librdf_stream* stream = librdf_model_as_stream(triplesData->getRedlandIndexedModel());
					if (stream) {
						while (!librdf_stream_end(stream)) {
							librdf_statement* statement = librdf_stream_get_object(stream);
							librdf_model_remove_statement(triplesData->getRedlandIndexedModel(), statement);
							librdf_stream_next(stream);
						}
						librdf_free_stream(stream);
					}
					delete triplesData;
					return true;
				}

				librdf_node* COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::getLocalizedNode(librdf_node* node, CRedlandStoredTriplesData* newTriplesData) {


					librdf_world* newWorld = newTriplesData->getRedlandWorld();

					if (librdf_node_is_resource(node)) {

						raptor_uri* prevUri = librdf_node_get_uri(node);
						raptor_uri* newUri = (raptor_uri*)librdf_new_uri(newWorld, librdf_uri_as_string(prevUri));
						librdf_node* newNode = librdf_new_node_from_uri(newWorld, newUri);
						return newNode;

					} else if (librdf_node_is_blank(node)) {						
						librdf_node* newNode = librdf_new_node_from_blank_identifier(newWorld, librdf_node_get_blank_identifier(node));
						return newNode;

					} else if (librdf_node_is_literal(node)) {
						unsigned char* literalValue = librdf_node_get_literal_value(node);
						raptor_uri* datatypeUri = librdf_node_get_literal_value_datatype_uri(node);
						raptor_uri* newDatatypeUri = nullptr;

						if (datatypeUri) {
							newDatatypeUri = (raptor_uri*)librdf_new_uri(newWorld, librdf_uri_as_string(datatypeUri));
						} else {
							newDatatypeUri = (raptor_uri*)librdf_new_uri(newWorld, (unsigned char*)PREFIX_XML_STRING_DATATYPE);
						}

						librdf_node* newNode = librdf_new_node_from_typed_literal(newWorld, literalValue, NULL, newDatatypeUri);

						return newNode;

					}
					return nullptr;
				}


				bool COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer::loadLatestOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {
					COntologyRevisionRedlandExternalStoredTriplesConnectionData* triplesData = getOntologyPersistingTriplesData(ontologyName);
					if (triplesData && !triplesData->isInitiallyLoaded() && mConfInitialLoading && hasOntologyPersisted(ontologyName)) {

						LOG(INFO, getLogDomain(), logTr("Trying loading of initial revision of ontology '%1' from external triple store").arg(ontologyName), this);

						CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ontoRev->getOntology());
						builder->initializeBuilding();

						CRedlandStoredTriplesData* newTriplesData = nullptr;
						cint64 statementCount = 0;
						if (mConfDirectFullLoading) {
							newTriplesData = getUpdatingTripleData();

							CRedlandStoredTriplesData* refTriplesData = new CRedlandStoredTriplesData();
							refTriplesData->initTriplesData(TRIPLES_DATA_ADDITION);
							initTriplesData(refTriplesData, ontologyName);

							CXLinker<librdf_statement*>* statmentLinker = nullptr;
							CXLinker<librdf_statement*>* lastStatementLinker = nullptr;

							librdf_stream* stream = librdf_model_as_stream(refTriplesData->getRedlandIndexedModel());
							if (stream) {
								while (!librdf_stream_end(stream)) {
									librdf_statement* statement = librdf_stream_get_object(stream);

									librdf_node* subjectNode = librdf_statement_get_subject(statement);
									librdf_node* locSubjectNode = getLocalizedNode(subjectNode, newTriplesData);

									librdf_node* predicateNode = librdf_statement_get_predicate(statement);
									librdf_node* locPredicateNode = getLocalizedNode(predicateNode, newTriplesData);

									librdf_node* objectNode = librdf_statement_get_object(statement);
									librdf_node* locObjectNode = getLocalizedNode(objectNode, newTriplesData);

									librdf_statement* newStatement = librdf_new_statement_from_nodes(newTriplesData->getRedlandWorld(), locSubjectNode, locPredicateNode, locObjectNode);
									librdf_model_add_statement(newTriplesData->getRedlandIndexedModel(), newStatement);

									CXLinker<librdf_statement*>* tmpRenderedLinker = new CXLinker<librdf_statement *>();
									tmpRenderedLinker->initLinker(newStatement);
									statementCount++;

									if (tmpRenderedLinker) {
										if (!statmentLinker) {
											statmentLinker = tmpRenderedLinker;
											lastStatementLinker = statmentLinker->getLastListLink();
										} else {
											lastStatementLinker->append(tmpRenderedLinker);
											lastStatementLinker = tmpRenderedLinker->getLastListLink();
										}
									}


									librdf_stream_next(stream);
								}
								librdf_free_stream(stream);
								newTriplesData->setRedlandStatementLinker(statmentLinker);
							}
							delete refTriplesData;


						} else {
							newTriplesData = new CRedlandStoredTriplesData();
							newTriplesData->initTriplesData(TRIPLES_DATA_ADDITION);
							initTriplesData(newTriplesData, ontologyName);
						}


						builder->addTriplesData(newTriplesData);
						triplesData->getPersistedTriplesDataSet()->insert(newTriplesData);

						CConcreteOntologyRedlandTriplesDataExpressionMapper* triplesMapper = new CConcreteOntologyRedlandTriplesDataExpressionMapper(builder);
						triplesMapper->mapTriples(ontoRev->getOntology(), ontoRev->getOntology()->getOntologyTriplesData());


						QSet<CAxiomExpression*>* axioms = builder->getAddedAxiomSet();
						for (CAxiomExpression* axiom : *axioms) {
							triplesData->getPersistedAxiomExpressionSet()->insert(axiom);
						}

						builder->completeBuilding();
						delete builder;

						LOG(INFO, getLogDomain(), logTr("Loaded initial revision of ontology '%1' from external triple store").arg(ontologyName), this);

						triplesData->setInitiallyLoaded(true);
						return true;
					}
					return false;
				}



			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
