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

#include "COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {

			namespace Persistence {

				COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer(CConfiguration* config) : CLogIdentifier("::Konclude::Reasoner::Persistence::RedlandExternalTriplesSPARQLServicePersistencer", this) {
					mConfig = config;


					mDefaultDatatypeString = PREFIX_XML_STRING_DATATYPE;
					mBlankNodeIdentifierPrefix = QString("kbni_%1_").arg(QUuid::createUuid().toString().replace("{", "").replace("}", ""));;


					mConfInitialLoading = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.InitialLoading");

					mRDFSerializingFormat = QString("turtle");

					mLoadingLimit = CConfigDataReader::readConfigInteger(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.LoadingLimitSize");
					mPersistingStepSize = CConfigDataReader::readConfigInteger(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.PersistingStepSize");

					mServiceUriData = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.ServiceAddress").toUtf8();
					mCheckingQueryString = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.CheckingQueryTemplate");
					mLoadingQueryString = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.LoadingQueryTemplate");
					mPersistQueryString = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.PersistingQueryTemplate");
					mDeletingQueryString = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.DeletingQueryTemplate");
					mCreatingQueryString = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.CreatingQueryTemplate");

					mDatabaseNameString = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.TriplesDatabaseName");


					mConfPersistPostBatchFormEncoded = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Persistence.RedlandExternalTriplesSPARQLServicePersistencer.HTTPPostFormEncodedPersisting");

					if (mConfPersistPostBatchFormEncoded) {
						mHttpManager = new CQtHttpTransactionManager();
					} else {
						mHttpManager = nullptr;
					}
				}

				COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::~COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer() {
					if (mHttpManager) {
						mHttpManager->stopThread(true);
						delete mHttpManager;
					}
				}



				COntologyRevisionRedlandExternalStoredTriplesConnectionData* COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::getOntologyPersistingTriplesData(const QString& ontName) {

					COntologyRevisionRedlandExternalStoredTriplesConnectionData* triplesData = mOntologyTriplesDataHash.value(ontName);

					if (!triplesData) {

						triplesData = new COntologyRevisionRedlandExternalStoredTriplesConnectionData();
						mOntologyTriplesDataHash.insert(ontName, triplesData);

					}
					return triplesData;
				}






				CRedlandStoredTriplesData* COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::getUpdatingTripleData() {

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




				bool COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::persistOntologyRevision(COntologyRevision* ontoRev, COntologyRevision* prevOntoRev) {

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

					COntologyRevisionRedlandExternalStoredTriplesConnectionData* revTriplesData = getOntologyPersistingTriplesData(ontName);
					if (revTriplesData) {
						COntologyAxiomExpressionsRenderVisitor renderer;
						COntologyRedlandRDFRenderer rdfRenderer(nullptr);

						CONTOLOGYAXIOMSET<CAxiomExpression*>* currAxiomSet = ontology->getBuildData()->getTellAxiomSet();
						CQtList<CAxiomExpression*> axiomList;
						for (CAxiomExpression* axiomExp : *currAxiomSet) {
							if (!revTriplesData->getPersistedAxiomExpressionSet()->contains(axiomExp)) {
								axiomList.append(axiomExp);
							}
						}
						QList<CTriplesData*> triplesDataList;
						COntologyTriplesData* ontTriplesData = ontoRev->getOntology()->getOntologyTriplesData();
						if (ontTriplesData) {
							for (CTriplesData* tripleData : *ontTriplesData->getAllTriplesData()) {
								if (!revTriplesData->getPersistedTriplesDataSet()->contains(tripleData)) {
									triplesDataList.append(tripleData);
								}
							}
						}


						LOG(INFO, getLogDomain(), logTr("Preparing new revision %2 of ontology '%1' for storing in external triple store").arg(ontName).arg(ontoVers), this);
						renderer.visitAndRender(axiomList, ontology, &rdfRenderer);
						renderer.visitAndRender(&triplesDataList, ontology, &rdfRenderer);

						if (!revTriplesData->isInitiallyLoaded()) {
							if (hasOntologyPersisted(ontName)) {
								if (mConfInitialLoading) {
									loadLatestOntologyRevision(ontName, ontoRev);
								}
							} else {
								createPersistingOntology(ontName);
							}
						}
						revTriplesData->setInitiallyLoaded(true);

						LOG(INFO, getLogDomain(), logTr("Trying saving new revision %2 of ontology '%1' in external triple store").arg(ontName).arg(ontoVers), this);
						CRedlandStoredTriplesData* triplesData = (COntologyRevisionRedlandExternalStoredTriplesConnectionData*)rdfRenderer.getRenderedTripleData(nullptr);


						QByteArray serFormByteArray = mRDFSerializingFormat.toUtf8();

						unsigned char* modelString = librdf_model_to_string(triplesData->getRedlandIndexedModel(), nullptr, serFormByteArray.constData(), nullptr, nullptr);
						QStringList tripleLines = QString::fromUtf8((char*)modelString).split("\n");


						QStringList prefixLines;




						QString firstTripleLine = tripleLines.first().trimmed();
						while (!tripleLines.isEmpty() && firstTripleLine.toUpper().startsWith("PREFIX") || firstTripleLine.toUpper().startsWith("@PREFIX")) {
							if (firstTripleLine.toUpper().startsWith("PREFIX")) {
								QString line = firstTripleLine.mid(6);
								if (line.endsWith(".")) {
									line = line.mid(0, line.length() - 1);
								}
								QString prefixLine = QString("PREFIX %1").arg(line);
								prefixLines.append(prefixLine);
							} else if (firstTripleLine.toUpper().startsWith("@PREFIX")) {
								QString line = firstTripleLine.mid(7);
								if (line.endsWith(".")) {
									line = line.mid(0, line.length() - 1);
								}
								QString prefixLine = QString("PREFIX %1").arg(line);
								prefixLines.append(prefixLine);
							}
							tripleLines.takeFirst();
							if (!tripleLines.isEmpty()) {
								firstTripleLine = tripleLines.first().trimmed();
							}
						}


						cint64 triplesSentCount = 0;
						cint64 persistingStep = 0;
						while (!tripleLines.isEmpty()) {
							persistingStep++;

							cint64 persistingCount = 0;
							QStringList persistingTripleLines;
							bool breakPoint = false;
							while (!tripleLines.isEmpty() && (persistingCount < mPersistingStepSize || !breakPoint || mPersistingStepSize <= 0)) {
								QString line = tripleLines.takeFirst().trimmed();
								if (!line.isEmpty()) {
									persistingTripleLines.append(line);
									if (line.endsWith(".")) {
										breakPoint = true;
									} else if (!line.isEmpty()) {
										breakPoint = false;
									}
									triplesSentCount++;
								}
								persistingCount++;
							}

							if (!persistingTripleLines.isEmpty()) {
								QString persistQueryString = mPersistQueryString;
								QString encodedOntologyName = ontName;
								persistQueryString = replaceQueryStringVariables(persistQueryString, encodedOntologyName, mConfPersistPostBatchFormEncoded);
								persistQueryString = persistQueryString.replace("$PrefixesString", prefixLines.join("\r\n"));
								persistQueryString = persistQueryString.replace("$TriplesString", persistingTripleLines.join("\r\n"));

								if (mConfPersistPostBatchFormEncoded) {
									sendPersistingHTTPPostFormEncodedRequest(persistQueryString, ontName);
								} else {
									sendPersistingRedlandClientRequest(persistQueryString, ontName);
								}

							}
						}

						LOG(INFO, getLogDomain(), logTr("Saved new revision %2 of ontology '%1' in external triple store by sending %3 triples").arg(ontName).arg(ontoVers).arg(triplesSentCount), this);

						for (CAxiomExpression* axiomExp : *currAxiomSet) {
							revTriplesData->getPersistedAxiomExpressionSet()->insert(axiomExp);
						}
						if (ontTriplesData) {
							for (CTriplesData* tripleData : *ontTriplesData->getAllTriplesData()) {
								revTriplesData->getPersistedTriplesDataSet()->insert(tripleData);
							}
						}

						CXLinker<librdf_statement*>* statementLinker = triplesData->getRedlandStatementLinker();
						for (CXLinker<librdf_statement*>* statementLinkerIt = statementLinker; statementLinkerIt; ) {
							CXLinker<librdf_statement*>* tmpStatementLinker = statementLinkerIt;
							statementLinkerIt = statementLinkerIt->getNext();

							librdf_statement* statement = tmpStatementLinker->getData();
							librdf_free_statement(statement);

							delete tmpStatementLinker;
						}
						triplesData->setRedlandStatementLinker(nullptr);
						delete triplesData;
					}

					return true;
				}



				bool COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::sendPersistingHTTPPostFormEncodedRequest(const QString &persistQueryString, const QString& ontName) {


					// may require some additional grants for user SPARQL for virtuoso, e.g., (can be executed in the SQL command line)
					//GRANT EXECUTE  ON DB.DBA.SPARQL_INSERT_CTOR_ACC TO "SPARQL";
					//GRANT EXECUTE  ON DB.DBA.SPARQL_INS_OR_DEL_OR_MODIFY_CTOR_FIN TO "SPARQL";


					QString databaseNameString = replaceQueryStringVariables(mDatabaseNameString, ontName);

					QByteArray postData;

					QUrlQuery query;
					query.addQueryItem("default-graph-uri", databaseNameString.replace("%","%25"));
					query.addQueryItem("query", persistQueryString);

					postData = query.toString(QUrl::FullyEncoded).toUtf8();

					CHttpRequest* request = mHttpManager->createRequest(mServiceUriData, postData);
					mHttpManager->setRequestHeader(request, "Content-Type", "application/x-www-form-urlencoded");

					CHttpResponse* response = mHttpManager->getResponse(request);
					mHttpManager->waitFinished(response);

					QString* resText = mHttpManager->getExtractedText(response);
					QString* errorString = mHttpManager->getErrorString(response);
					delete resText;
					delete errorString;


					mHttpManager->releaseResponse(response, true);
					return true;
				}



				bool COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::sendPersistingRedlandClientRequest(const QString &persistQueryString, const QString& ontName) {
					QByteArray persitQueryData = persistQueryString.toUtf8();


					CRedlandStoredTriplesData* newTriplesData = getUpdatingTripleData();
					librdf_world* world = newTriplesData->getRedlandWorld();


					rasqal_service* rasqalService = getRasqalService(world, ontName, persitQueryData);


					rasqal_query_results* results = rasqal_service_execute(rasqalService);

					// may require some additional grants for user SPARQL for virtuoso, e.g., (can be executed in the SQL command line)
					//GRANT EXECUTE  ON DB.DBA.SPARQL_INSERT_DICT_CONTENT TO "SPARQL";
					//GRANT EXECUTE  ON DB.DBA.L_O_LOOK TO "SPARQL";

					//if (!rasqal_query_results_finished(results)) {
					//	bool error = true;
					//}

					if (results) {
						rasqal_free_query_results(results);
					}

					rasqal_free_service(rasqalService);
					delete newTriplesData;
					return true;
				}

				QString COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::replaceQueryStringVariables(QString string, const QString& ontologyName, bool doublePercentageEncoding) {
					if (string.contains("$KnowledgeBaseName")) {
						string = string.replace("$KnowledgeBaseName", ontologyName);
					}
					if (string.contains("$PercentEncodedKnowledgeBaseName")) {
						QString encodedOntologyName = QUrl::toPercentEncoding(ontologyName);
						if (doublePercentageEncoding) {
							// qt somehow does not encode the % symbol correctly
							encodedOntologyName = encodedOntologyName.replace("%", "%25");
						}
						string = string.replace("$PercentEncodedKnowledgeBaseName", encodedOntologyName);
					}
					return string;
				}



				bool COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::hasOntologyPersisted(const QString& ontologyName) {

					bool hasOntologyPersisted = false;


					QString checkingQueryString = mCheckingQueryString;
					checkingQueryString = replaceQueryStringVariables(checkingQueryString, ontologyName);
					QByteArray checkingQueryData = checkingQueryString.toUtf8();

					CRedlandStoredTriplesData* newTriplesData = getUpdatingTripleData();
					librdf_world* world = newTriplesData->getRedlandWorld();

					rasqal_service* rasqalService = getRasqalService(world, ontologyName, checkingQueryData);


					rasqal_query_results* results = rasqal_service_execute(rasqalService);



					CXLinker<librdf_statement*>* statmentLinker = nullptr;
					CXLinker<librdf_statement*>* lastStatementLinker = nullptr;


					if (results && rasqal_query_results_is_bindings(results)) {

						if (!rasqal_query_results_finished(results)) {
							hasOntologyPersisted = true;
						}
					}
					if (results) {
						rasqal_free_query_results(results);
					}

					rasqal_free_service(rasqalService);
					delete newTriplesData;


					return hasOntologyPersisted;
				}



				bool COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::hasNewOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {

					if (hasOntologyPersisted(ontologyName)) {
						COntologyRevisionRedlandExternalStoredTriplesConnectionData* triplesData = getOntologyPersistingTriplesData(ontologyName);
						if (triplesData && !triplesData->isInitiallyLoaded()) {
							return true;
						}
					}

					return false;
				}


				bool COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::deletePersistedOntology(const QString& ontologyName) {

					QByteArray deleteQueryData = mDeletingQueryString.toUtf8();
					deleteQueryData = replaceQueryStringVariables(deleteQueryData, ontologyName).toUtf8();


					CRedlandStoredTriplesData* newTriplesData = getUpdatingTripleData();
					librdf_world* world = newTriplesData->getRedlandWorld();


					rasqal_service* rasqalService = getRasqalService(world, ontologyName, deleteQueryData);


					rasqal_query_results* results = rasqal_service_execute(rasqalService);

					if (results) {
						rasqal_free_query_results(results);
					}

					rasqal_free_service(rasqalService);
					delete newTriplesData;


					LOG(INFO, getLogDomain(), logTr("Deleted persisted version of ontology '%1' in external triple store").arg(ontologyName), this);

					return true;
				}


				bool COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::createPersistingOntology(const QString& ontologyName) {

					QByteArray creatingQueryData = mCreatingQueryString.toUtf8();
					creatingQueryData = replaceQueryStringVariables(creatingQueryData, ontologyName).toUtf8();

					CRedlandStoredTriplesData* newTriplesData = getUpdatingTripleData();
					librdf_world* world = newTriplesData->getRedlandWorld();

					rasqal_service* rasqalService = getRasqalService(world, ontologyName, creatingQueryData);

					rasqal_query_results* results = rasqal_service_execute(rasqalService);

					if (results) {
						rasqal_free_query_results(results);
					}

					rasqal_free_service(rasqalService);
					delete newTriplesData;
					LOG(INFO, getLogDomain(), logTr("Created database for persisting ontology '%1' in external triple store").arg(ontologyName), this);

					return true;
				}


				librdf_node* COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::createIRIResourceNode(const QString& iri, librdf_world* world) {
					QByteArray bytes = iri.toUtf8();
					raptor_uri* uri = (raptor_uri*)librdf_new_uri(world, (const unsigned char*)bytes.constData());
					librdf_node* node = librdf_new_node_from_uri(world, uri);
					return node;
				}


				librdf_node* COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::createBlankNode(const QString& identifier, librdf_world* world) {
					QByteArray bytes = identifier.toUtf8();
					librdf_node* node = librdf_new_node_from_blank_identifier(world, (const unsigned char*)bytes.constData());
					return node;
				}

				librdf_node* COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::createBlankNode(cint64 id, librdf_world* world) {
					QString identifier = QString("%1%2").arg(mBlankNodeIdentifierPrefix).arg(id);
					QByteArray bytes = identifier.toUtf8();
					librdf_node* node = librdf_new_node_from_blank_identifier(world, (const unsigned char*)bytes.constData());
					return node;
				}

				librdf_node* COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::createLiteralNode(const QString& literal, const QString& datatype, librdf_world* world) {
					QByteArray datatypeBytes = datatype.toUtf8();
					librdf_uri* datatypeUri = nullptr;
					if (datatype != mDefaultDatatypeString) {
						librdf_new_uri(world, (const unsigned char*)datatypeBytes.constData());
					}
					QByteArray literalBytes = literal.toUtf8();
					librdf_node* node = librdf_new_node_from_typed_literal(world, (const unsigned char*)literalBytes.constData(), NULL, datatypeUri);
					return node;
				};




				bool COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::loadLatestOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {
					COntologyRevisionRedlandExternalStoredTriplesConnectionData* triplesData = getOntologyPersistingTriplesData(ontologyName);
					if (triplesData && !triplesData->isInitiallyLoaded() && mConfInitialLoading && hasOntologyPersisted(ontologyName)) {

						LOG(INFO, getLogDomain(), logTr("Trying loading of initial revision of ontology '%1' from external SPARQL service").arg(ontologyName), this);

						CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ontoRev->getOntology());
						builder->initializeBuilding();

						CRedlandStoredTriplesData* newTriplesData = nullptr;
						cint64 statementCount = 0;
						newTriplesData = getUpdatingTripleData();

						cint64 offset = 0;
						bool continueLoading = true;
						while (continueLoading) {
							continueLoading = false;
							bool hasLoadingLimit = false;


							QString loadingQueryString = mLoadingQueryString;
							loadingQueryString = replaceQueryStringVariables(loadingQueryString, ontologyName);
							if (loadingQueryString.contains("$LoadingLimit") && loadingQueryString.contains("$LoadingOffset")) {
								hasLoadingLimit = true;
								loadingQueryString = loadingQueryString.replace("$LoadingLimit", QString::number(mLoadingLimit));
								loadingQueryString = loadingQueryString.replace("$LoadingOffset", QString::number(offset));
								offset += mLoadingLimit;
							}
							QByteArray loadingQueryData = loadingQueryString.toUtf8();

							librdf_world* world = newTriplesData->getRedlandWorld();
							rasqal_service* rasqalService = getRasqalService(world, ontologyName, loadingQueryData);


							rasqal_query_results* results = rasqal_service_execute(rasqalService);



							CXLinker<librdf_statement*>* statmentLinker = nullptr;
							CXLinker<librdf_statement*>* lastStatementLinker = nullptr;


							if (results && rasqal_query_results_is_bindings(results)) {

								int bindCount = rasqal_query_results_get_bindings_count(results);



								const unsigned char **names = nullptr;
								rasqal_literal** values = nullptr;



								QByteArray plainLiteralBytes = QString(PREFIX_XML_PLAINLITERAL_DATATYPE).toUtf8();
								librdf_uri *plainLiteralUri = librdf_new_uri(world, (const unsigned char*)plainLiteralBytes.constData());


								while (!rasqal_query_results_finished(results)) {

									if (rasqal_query_results_get_bindings(results, &names, &values))
										break;

									if (names) {
										cint64 nodeSet = 0;
										librdf_node* node[3] = { nullptr, nullptr, nullptr };
										if (statementCount > 10) {
											bool debug = true;
										}
										for (int i = 0; names[i]; i++) {
											rasqal_literal* valueNode = values[i];
											if (valueNode) {
												rasqal_literal_type type = rasqal_literal_get_type(valueNode);
												if (type == RASQAL_LITERAL_URI) {
													const unsigned char* chrString = rasqal_literal_as_string(valueNode);
													QString valueString = QString::fromUtf8((char*)chrString);
													if (valueString.startsWith("_:")) {
														node[i] = createBlankNode(valueString.mid(2), world);
													} else {
														node[i] = createIRIResourceNode(valueString, world);
													}
													nodeSet++;
												} else if (type == RASQAL_LITERAL_BLANK) {
													const unsigned char* chrString = rasqal_literal_as_string(valueNode);
													node[i] = createBlankNode(QString::fromUtf8((char*)chrString), world);
													nodeSet++;
												} else if (type == RASQAL_LITERAL_STRING) {
													const unsigned char* chrString = rasqal_literal_as_string(valueNode);
													raptor_uri* datatypeUri = rasqal_literal_datatype(valueNode);
													if (datatypeUri) {
														const unsigned char* datatypeString = raptor_uri_as_string(datatypeUri);
														node[i] = createLiteralNode(QString::fromUtf8((char*)chrString), QString::fromUtf8((char*)datatypeString), world);
													} else {
														node[i] = createLiteralNode(QString::fromUtf8((char*)chrString), "", world);
													}
													nodeSet++;
												} else {
													const unsigned char* chrString = rasqal_literal_as_string(valueNode);
													raptor_uri* datatypeUri = rasqal_literal_datatype(valueNode);
													if (datatypeUri) {
														const unsigned char* datatypeString = raptor_uri_as_string(datatypeUri);
														node[i] = createLiteralNode(QString::fromUtf8((char*)chrString), QString::fromUtf8((char*)datatypeString), world);
													}
												}
											}
											if (nodeSet >= 3) {
												if (hasLoadingLimit) {
													continueLoading = true;
												}
												librdf_statement* newStatement = librdf_new_statement_from_nodes(world, node[0], node[1], node[2]);
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

											}

										}
									}
									rasqal_query_results_next(results);
								}

							}
							newTriplesData->setRedlandStatementLinker(statmentLinker);


							if (results) {
								rasqal_free_query_results(results);
							}

							rasqal_free_service(rasqalService);
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

						LOG(INFO, getLogDomain(), logTr("Loaded initial revision of ontology '%1' from external SPARQL service").arg(ontologyName), this);

						triplesData->setInitiallyLoaded(true);
						return true;
					}
					return false;
				}

				rasqal_service* COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer::getRasqalService(librdf_world* world, const QString& ontologyName, QByteArray &queryData) {
					rasqal_world* rasqalWorld = librdf_world_get_rasqal(world);
					raptor_uri* serviceUri = (raptor_uri*)librdf_new_uri(world, (const unsigned char*)mServiceUriData.constData());

					QString databaseNameString = replaceQueryStringVariables(mDatabaseNameString, ontologyName);
					QByteArray databaseNameData = databaseNameString.toUtf8();
					raptor_uri* databaseNameUri = (raptor_uri*)librdf_new_uri(world, (const unsigned char*)databaseNameData.constData());


					rasqal_data_graph* dataGraph = rasqal_new_data_graph_from_uri(rasqalWorld, databaseNameUri, databaseNameUri, RASQAL_DATA_GRAPH_BACKGROUND, NULL, NULL, NULL);
					raptor_sequence* dataGraphs = raptor_new_sequence(nullptr, nullptr);
					raptor_sequence_set_at(dataGraphs, 0, dataGraph);
					rasqal_service* rasqalService = rasqal_new_service(rasqalWorld, serviceUri, (const unsigned char*)queryData.constData(), dataGraphs);
					return rasqalService;
				}


			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
