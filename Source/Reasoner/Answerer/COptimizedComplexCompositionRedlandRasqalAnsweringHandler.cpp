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

#include "COptimizedComplexCompositionRedlandRasqalAnsweringHandler.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			



			COptimizedComplexCompositionRedlandRasqalAnsweringHandler::COptimizedComplexCompositionRedlandRasqalAnsweringHandler(COptimizedComplexCompositionOntologyAnsweringItem* ontoAnsweringItem) : COptimizedComplexCompositionAnsweringHandler(ontoAnsweringItem) {
				mCurrentProcessingRedlandRasqalQueryProcData = nullptr;
			}


			COptimizedComplexCompositionRedlandRasqalAnsweringHandler::~COptimizedComplexCompositionRedlandRasqalAnsweringHandler() {
			}



			bool COptimizedComplexCompositionRedlandRasqalAnsweringHandler::canAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery) {
				if (dynamic_cast<CRedlandRasqalBGPsCompositionQuery*>(complexAnsweringQuery)) {
					return true;
				}
				return false;
			}

			bool COptimizedComplexCompositionRedlandRasqalAnsweringHandler::addAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback) {
				mPendingQueryProcessingList.append(new CComplexQueryCompositionRedlandRasqalProcessingData((CRedlandRasqalBGPsCompositionQuery*)complexAnsweringQuery, callback));
				return true;
			}




			int redland_log_handler_processing_error_notifier(void *user_data, librdf_log_message *message) {
				QStringList* messageList = (QStringList*)user_data;
				if (message->level <= 50 && messageList) {
					messageList->append(QString(QString("%1").arg(message->message)));
				}
				return 1;
			}

			void rasqal_log_handler_processing_error_notifier(void *user_data, raptor_log_message *message) {
				QStringList* messageList = (QStringList*)user_data;
				if (message->level <= 50 && messageList) {
					messageList->append(QString(QString("%1").arg(message->text)));
				}
			}





			COptimizedComplexCompositionRedlandRasqalAnsweringHandler* COptimizedComplexCompositionRedlandRasqalAnsweringHandler::initializeQueryProcessing(CComplexQueryCompositionProcessingData* queryData, CAnswererContext* answererContext) {
				CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData = (CComplexQueryCompositionRedlandRasqalProcessingData*)queryData;

				CRedlandRasqalBGPsCompositionQuery* redlandRasqalQuery = redlandRasqalQueryProcData->getRedlandRasqalQuery();
				CConcreteOntology* baseOnto = mOntoAnsweringItem->getOntology();
				CConcreteOntology* exppOnto = redlandRasqalQuery->getExrpessionOntology();
				CConfigurationBase* config = redlandRasqalQuery->getConfiguration();
				redlandRasqalQueryProcData->setProcessingStarted();

				rasqal_world_set_log_handler(librdf_world_get_rasqal(redlandRasqalQuery->getRedlandWorld()), redlandRasqalQueryProcData->getRedlandRasqalErrorWarningMessageList(), &rasqal_log_handler_processing_error_notifier);
				librdf_world_set_logger(redlandRasqalQuery->getRedlandWorld(), redlandRasqalQueryProcData->getRedlandRasqalErrorWarningMessageList(), &redland_log_handler_processing_error_notifier);







				mOntoAnsweringItem->addScheduledQueriesProcessingData(redlandRasqalQueryProcData);

				mOntoAnsweringItem->incCurrentlyAnsweringQueryCount();
				return this;
			}




			bool COptimizedComplexCompositionRedlandRasqalAnsweringHandler::continueCalculationCreation(CAnswererContext* answererContext) {
				bool processing = false;

				
				QList<CComplexQueryCompositionProcessingData*>* scheduledQueriesProcDataList = mOntoAnsweringItem->getScheduledQueriesProcessingDataList();
				if (!scheduledQueriesProcDataList->isEmpty()) {
					CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData = (CComplexQueryCompositionRedlandRasqalProcessingData*)scheduledQueriesProcDataList->takeFirst();
					CRedlandRasqalBGPsCompositionQuery* query = redlandRasqalQueryProcData->getRedlandRasqalQuery();


					

					if (!processing && !redlandRasqalQueryProcData->hasSubQueriesGenerated()) {

						bool subQueryProcessing = generateSubQueries(redlandRasqalQueryProcData, answererContext);
						if (subQueryProcessing) {
							processing = true;
						}
						redlandRasqalQueryProcData->setSubQueriesGenerated(true);
					}


					if (!processing && redlandRasqalQueryProcData->hasRemainingSubQueriesProcessing() && !redlandRasqalQueryProcData->isSubQueryWaiting()) {

						bool subQueryProcessing = scheduleNextSubQueryProcessing(redlandRasqalQueryProcData, answererContext);
						if (subQueryProcessing) {
							processing = true;
						}
						redlandRasqalQueryProcData->setSubQueriesGenerated(true);
					}


					if (!processing && !redlandRasqalQueryProcData->isQueryProcessed() && redlandRasqalQueryProcData->isSubQueryProcessingFinished()) {
						if (redlandRasqalQueryProcData->areResultDirectlyFromSubQueryStreamed()) {
							LOG(INFO, getDomain(), logTr("Redland Rasqal query completely answered with computation of sub-query '%1' in %2 ms.").arg(query->getQueryName()).arg(redlandRasqalQueryProcData->getSubQueryProcessingTime()), this);
						} else {
							processRasqal(query, redlandRasqalQueryProcData);
						}
						redlandRasqalQueryProcData->setQueryProcessed(true);
					}


					if (!processing && redlandRasqalQueryProcData->isQueryProcessed()) {

						CCallbackData* callback = redlandRasqalQueryProcData->getCallback();
						if (callback) {
							callback->doCallback();
						}
						delete redlandRasqalQueryProcData;
						mOntoAnsweringItem->decCurrentlyAnsweringQueryCount();
						mOntoAnsweringItem->getAnsweringHandlingStatistics()->incTotalAnsweredQueriesCount();
					}


				}


				return processing;
			}


			bool COptimizedComplexCompositionRedlandRasqalAnsweringHandler::scheduleNextSubQueryProcessing(CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData, CAnswererContext* answererContext) {
				QSet<CRedlandRasqalSubQueryProcessingData*>* remainingSubQueryProcSet = redlandRasqalQueryProcData->getRemainingSubQueriesProcessingSet();
				QSet<cint64>* processedSubQueryIdSet = redlandRasqalQueryProcData->getProcessedSubQueryIdSet();
				QVector<CRedlandRasqalSubQueryProcessingData*>* subQueryProcDataVec = redlandRasqalQueryProcData->getSubQueryProcessingDataVector();


				QSet<CRedlandRasqalSubQueryProcessingData*> withoutUnprocessedDependencySubQuerySet;
				QSet<CRedlandRasqalSubQueryProcessingData*> atleastOneProcessedDependencySubQuerySet;
				QSet<CRedlandRasqalSubQueryProcessingData*> remainingSubQuerySet;
				for (CRedlandRasqalSubQueryProcessingData* remainingSubQueryProc : *remainingSubQueryProcSet) {
					bool withoutUnprocessedDependency = true;
					bool atleastOneProcessedDependency = false;
					QSet<cint64>* depSet = remainingSubQueryProc->getDependentSubQueryIdSet();
					if (!depSet->isEmpty()) {
						for (cint64 depSubQuery : *depSet) {
							if (!processedSubQueryIdSet->contains(depSubQuery)) {
								withoutUnprocessedDependency = false;
							} else {
								atleastOneProcessedDependency = false;
							}
						}
					}
					if (withoutUnprocessedDependency) {
						withoutUnprocessedDependencySubQuerySet.insert(remainingSubQueryProc);
					} else if (atleastOneProcessedDependency) {
						atleastOneProcessedDependencySubQuerySet.insert(remainingSubQueryProc);
					} else {
						remainingSubQuerySet.insert(remainingSubQueryProc);
					}
				}

				QList<CRedlandRasqalSubQueryProcessingData*> subqueries;
				if (!withoutUnprocessedDependencySubQuerySet.isEmpty()) {
					for (CRedlandRasqalSubQueryProcessingData* subQueryData : withoutUnprocessedDependencySubQuerySet) {
						subqueries.append(subQueryData);
					}
				}
				if (subqueries.isEmpty() && !atleastOneProcessedDependencySubQuerySet.isEmpty()) {
					for (CRedlandRasqalSubQueryProcessingData* subQueryData : atleastOneProcessedDependencySubQuerySet) {
						subqueries.append(subQueryData);
						break;
					}
				}

				if (subqueries.isEmpty()) {
					for (CRedlandRasqalSubQueryProcessingData* subQueryData : remainingSubQuerySet) {
						subqueries.append(subQueryData);
						break;
					}
				}

				QSet<cint64> tmpProcessedSubQueryIdSet(*processedSubQueryIdSet);
				QHash<CQuery*, CRedlandRasqalSubQueryProcessingData*>* queryDataHash = redlandRasqalQueryProcData->getSubQueryDataHash();
				bool subQueryProcessing = false;
				for (CRedlandRasqalSubQueryProcessingData* subqueryData : subqueries) {
					CQuery* subquery = subqueryData->getSubQuery();
					CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery* extSubQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery*>(subquery);
					if (extSubQuery) {
						CComplexQueryExtendibleData* extData = extSubQuery->getExtendibleData();
						QSet<cint64>* depSet = subqueryData->getDependentSubQueryIdSet();

						for (cint64 depQueryId : *depSet) {
							if (tmpProcessedSubQueryIdSet.contains(depQueryId)) {
								extData->addExtendingQuery((*subQueryProcDataVec)[depQueryId]->getSubQuery());
							}
						}
					}
					redlandRasqalQueryProcData->incSubQueryWaitingCount();
					processedSubQueryIdSet->insert(subqueryData->getSubQueryId());
					CRedlandRasqalSubQueryProcessingData* subQueryProcData = queryDataHash->value(subquery);
					remainingSubQueryProcSet->remove(subQueryProcData);
					CAnsweringMessageDataRequirementCompletedSubQueryAnswering* subQueryAnsweringCompMess = new CAnsweringMessageDataRequirementCompletedSubQueryAnswering(redlandRasqalQueryProcData, subquery, subQueryProcData->getSubQueryId());
					subQueryProcessing |= processSubQuery(answererContext, subquery, redlandRasqalQueryProcData, subQueryAnsweringCompMess);
				}
				return subQueryProcessing;
			}


			bool COptimizedComplexCompositionRedlandRasqalAnsweringHandler::generateSubQueries(CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData, CAnswererContext* answererContext) {

				CRedlandRasqalBGPsCompositionQuery* redlandRasqalQuery = redlandRasqalQueryProcData->getRedlandRasqalQuery();
				CConcreteOntology* queryBaseOnto = mOntoAnsweringItem->getOntology();
				CConcreteOntology* queryExppOnto = redlandRasqalQuery->getExrpessionOntology();
				CConfigurationBase* config = redlandRasqalQuery->getConfiguration();
				QList<CQuery*> subqueries;
				cint64 nextSubQueryId = 0;
				QHash<QString, CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData*> subqueryNameBGPHash;

				QHash<QString, TDependentOntologyPair>* queryKbNameDepOntPair = redlandRasqalQuery->getDependentKnowledgeBaseRevisionHash();
				QHash<QString, TDependentOntologyPair> kbNameDepOntPair(*queryKbNameDepOntPair);
				kbNameDepOntPair.insert(queryBaseOnto->getOntologyName(), TDependentOntologyPair(queryBaseOnto, queryExppOnto));

				QStringList missingKBNameList;
				for (QHash<QString, TDependentOntologyPair>::const_iterator it = kbNameDepOntPair.constBegin(), itEnd = kbNameDepOntPair.constEnd(); it != itEnd; ++it) {
					QString kbName = it.key();
					TDependentOntologyPair depOntPair = it.value();
					CConcreteOntology* baseOnto = depOntPair.first;
					CConcreteOntology* exppOnto = depOntPair.second;

					if (!baseOnto || !exppOnto) {
						missingKBNameList.append(kbName);
					} else {
						CConcreteOntologyUpdateSeparateHashingCollectorBuilder *builder = new CConcreteOntologyUpdateSeparateHashingCollectorBuilder(exppOnto);
						CConcreteOntologyQueryExtendedBuilder* queryBuilderGen = new CConcreteOntologyExtendibleQueryExtendedBuilder(baseOnto, exppOnto, config, builder);
						CSPARQLRedlandRasqalReasoningBGPQueryParser* bgpSubQueryParser = new CSPARQLRedlandRasqalReasoningBGPQueryParser(queryBuilderGen, builder, exppOnto, config);
						builder->initializeBuilding();
						QHash<QString, CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData*> tmpSubqueryNameBGPHash = bgpSubQueryParser->parseBGPReasoningSubQueries(redlandRasqalQuery->getRedlandWorld(), redlandRasqalQuery->getRedlandQuery(), redlandRasqalQuery->getRasqalQuery(), redlandRasqalQuery->getQueryText(), kbName, kbName == queryBaseOnto->getOntologyName(), nextSubQueryId);
						subqueryNameBGPHash.unite(tmpSubqueryNameBGPHash);
						builder->completeBuilding();
						subqueries.append(queryBuilderGen->generateQuerys());
					}
				}

				if (!missingKBNameList.isEmpty()) {
					if (missingKBNameList.size() > 1) {
						LOG(WARN, getDomain(), logTr("Ontologies '%1' referred in query are not available.").arg(missingKBNameList.join("', '")), this);
					} else {
						LOG(WARN, getDomain(), logTr("Ontology '%1' referred in query is not available.").arg(missingKBNameList.first()), this);
					}
				}


				QVector<CRedlandRasqalSubQueryProcessingData*>* subQueryProcDataVec = redlandRasqalQueryProcData->getSubQueryProcessingDataVector();
				subQueryProcDataVec->resize(subqueries.size());
				QHash<CQuery*, CRedlandRasqalSubQueryProcessingData*>* queryDataHash = redlandRasqalQueryProcData->getSubQueryDataHash();
				QHash<cint64, CRedlandRasqalSubQueryProcessingData*>* idSubQueryDataHash = redlandRasqalQueryProcData->getIdSubQueryDataHash();
				QSet<CRedlandRasqalSubQueryProcessingData*>* subQueryProcSet = redlandRasqalQueryProcData->getRemainingSubQueriesProcessingSet();

				for (CQuery* subquery : subqueries) {
					const QString& queryName = subquery->getQueryName();
					CComplexAnsweringExpressionQuery* compAnsSubQuery = (CComplexAnsweringExpressionQuery*)subquery;
					int subQueryId = queryName.mid(queryName.lastIndexOf("-") + 1).toInt();
					CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData* bgpData = subqueryNameBGPHash.value(queryName);
					CRedlandRasqalSubQueryProcessingData* subQueryProcData = new CRedlandRasqalSubQueryProcessingData(subQueryId, compAnsSubQuery, bgpData->bgp, bgpData->tripleStartPos, bgpData->tripleEndPos, redlandRasqalQuery->getRedlandWorld());
					if (bgpData->completelyHandledResultsTransfer) {
						CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndiVarAnsSubQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringQuery*>(subquery);
						if (compAssIndiVarAnsSubQuery) {
							CRedlandRasqalBGPsCompositionQuery* query = redlandRasqalQueryProcData->getRedlandRasqalQuery();
							CVariableBindingsAnswersStreamingResult* existBindsAnswersStreamingResult = dynamic_cast<CVariableBindingsAnswersStreamingResult*>(query->getQueryResult());
							if (existBindsAnswersStreamingResult) {
								compAssIndiVarAnsSubQuery->setQueryResult(existBindsAnswersStreamingResult);
								redlandRasqalQueryProcData->setResultDirectlyFromSubQueryStreamed(true);
							}
						}
					}
					*subQueryProcData->getDependentSubQueryIdSet() = bgpData->dependentSubQueryIds.toSet();
					delete bgpData;
					(*subQueryProcDataVec)[subQueryId] = subQueryProcData;
					queryDataHash->insert(subquery, subQueryProcData);
					subQueryProcSet->insert(subQueryProcData);
					idSubQueryDataHash->insert(subQueryId, subQueryProcData);
				}

				if (!subqueries.isEmpty()) {
					LOG(INFO, getDomain(), logTr("Extracted '%1' sub-queries from BGPs of query '%2'.").arg(subqueries.size()).arg(redlandRasqalQuery->getQueryName()), this);
				}

				CRedlandRasqalModelSubQueryResolver* resolver = new CRedlandRasqalModelSubQueryResolver(subQueryProcDataVec, redlandRasqalQuery->getRedlandWorld(), redlandRasqalQuery->getRedlandQuery(), redlandRasqalQuery->getRasqalQuery(), queryKbNameDepOntPair);
				redlandRasqalQueryProcData->setResolver(resolver);

				return false;
			}

			bool COptimizedComplexCompositionRedlandRasqalAnsweringHandler::processSubQuery(CAnswererContext* answererContext, CQuery* subQuery, CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData, CAnsweringMessageDataRequirementCompleted* completedMessage) {
				QString subQueryDepString;
				CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery* extSubQuery = dynamic_cast<CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery*>(subQuery);
				if (extSubQuery) {
					QList<CQuery*>* depQueries = extSubQuery->getExtendibleData()->getExtendingQueryList();
					if (depQueries && !depQueries->isEmpty()) {
						QHash<CQuery*, CRedlandRasqalSubQueryProcessingData*>* queryDataHash = redlandRasqalQueryProcData->getSubQueryDataHash();
						QStringList depQueryStringList;
						for (CQuery* depSubQuery : *depQueries) {
							CRedlandRasqalSubQueryProcessingData* depQueryData = queryDataHash->value(depSubQuery);
							if (depQueryData) {
								depQueryStringList += QString::number(depQueryData->getSubQueryId());
							}
						}
						if (!depQueryStringList.isEmpty()) {
							if (depQueryStringList.size() > 1) {
								subQueryDepString = QString(", based upon computed sub-queries %1").arg(depQueryStringList.join(", "));
							} else {
								subQueryDepString = QString(", based upon computed sub-query %1").arg(depQueryStringList.first());
							}
						}

					}
				}
				LOG(INFO, getDomain(), logTr("Scheduling processing of sub-query '%1'%2.").arg(subQuery->getQueryName()).arg(subQueryDepString), this);
				mOntoAnsweringItem->getAnsweringHandlingStatistics()->incRequestedRealizationRequirementCount(1);
				answererContext->getAnsweringCalculationHandler()->answerSubQuery(this, subQuery, completedMessage);
				return true;
			}


			void COptimizedComplexCompositionRedlandRasqalAnsweringHandler::processRasqal(CRedlandRasqalBGPsCompositionQuery* query, CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData) {
				LOG(INFO, getDomain(), logTr("Sub-queries of '%1' computed in %2 ms, starting answer resolving with Redland Rasqal.").arg(query->getQueryName()).arg(redlandRasqalQueryProcData->getSubQueryProcessingTime()), this);

				mCurrentProcessingRedlandRasqalQueryProcData = redlandRasqalQueryProcData;
				CRedlandRasqalModelSubQueryResolver* resolver = redlandRasqalQueryProcData->getResolver();
				librdf_model* model = resolver->getRedlandResolvingModel();


				QHash<QString, TDependentOntologyPair>* queryKbNameDepOntPair = query->getDependentKnowledgeBaseRevisionHash();
				librdf_world* redlandWorld = query->getRedlandWorld();
				raptor_world* raptorWorld = librdf_world_get_raptor(redlandWorld);
				rasqal_world* rasqalWorld = librdf_world_get_rasqal(redlandWorld);

				for (QHash<QString, TDependentOntologyPair>::const_iterator it = queryKbNameDepOntPair->constBegin(), itEnd = queryKbNameDepOntPair->constEnd(); it != itEnd; ++it) {
					QString kbName = it.key();
					QByteArray kbNameData = kbName.toUtf8();
					raptor_uri* source_uri = raptor_new_uri(raptorWorld, (const unsigned char*)kbNameData.constData());
					rasqal_data_graph* dg = rasqal_new_data_graph_from_uri(rasqalWorld, source_uri, source_uri, RASQAL_DATA_GRAPH_NAMED, NULL, NULL, NULL);
					rasqal_query_add_data_graph(query->getRasqalQuery(), dg);
				}



				librdf_query_results* results = librdf_model_query_execute(model, query->getRedlandQuery());


				cint64 answerCount = 0;
				if (results && librdf_query_results_is_bindings(results)) {

					int bindCount = librdf_query_results_get_bindings_count(results);


					QStringList varList;
					bool answersWriteable = true;
					CVariableBindingsAnswersResult* bindsAnswersResult = nullptr;
					CVariableBindingsAnswersStreamingResult* existBindsAnswersStreamingResult = nullptr;

					CVariableBindingsQueryResultWriter resultWriter;

					const char **names = NULL;
					librdf_node** values = new librdf_node*[bindCount];

					CVariableBindingsListAnswerResult* reuseBindAnsRes = nullptr;
					cint64 flushSize = 100;
					double flushSizeIncreasingFactor = 2.0;


					QByteArray plainLiteralBytes = QString(PREFIX_XML_PLAINLITERAL_DATATYPE).toUtf8();
					librdf_uri *plainLiteralUri = librdf_new_uri(query->getRedlandWorld(), (const unsigned char*)plainLiteralBytes.constData());


					QList<CVariableBindingStringResult*> reserveBindingsList;
					while (!librdf_query_results_finished(results) && answersWriteable) {

						if (librdf_query_results_get_bindings(results, &names, values))
							break;

						if (names) {
							int i;

							if (!bindsAnswersResult) {
								for (i = 0; names[i]; i++) {
									varList.append(QString::fromUtf8(names[i]));
								}

								existBindsAnswersStreamingResult = dynamic_cast<CVariableBindingsAnswersStreamingResult*>(query->getQueryResult());
								if (existBindsAnswersStreamingResult) {
									answersWriteable &= existBindsAnswersStreamingResult->initResult(varList);
									bindsAnswersResult = existBindsAnswersStreamingResult;

									reuseBindAnsRes = new CVariableBindingsListAnswerResult();

									for (i = 0; names[i]; i++) {
										reserveBindingsList.append(new CVariableBindingStringResult());
									}
								} else {
									bindsAnswersResult = new CVariableBindingsAnswersListResult(varList);
									query->setQueryResult(bindsAnswersResult);
								}
							}

							++answerCount;
							CVariableBindingsListAnswerResult* bindAnsRes = nullptr;
							if (!existBindsAnswersStreamingResult) {
								bindAnsRes = new CVariableBindingsListAnswerResult();
							} else {
								bindAnsRes = reuseBindAnsRes;
							}


							for (i = 0; names[i]; i++) {
								librdf_node* valueNode = values[i];
								if (valueNode) {
									librdf_node_type type = librdf_node_get_type(valueNode);
									if (type == LIBRDF_NODE_TYPE_RESOURCE) {
										librdf_uri* uri = librdf_node_get_uri(valueNode);
										unsigned char* uristring = librdf_uri_as_string(uri);
										QString uriString = QString::fromUtf8((const char*)uristring);
										CVariableBindingStringResult* bindingResult = nullptr;
										if (!existBindsAnswersStreamingResult) {
											bindingResult = new CVariableBindingStringResult(uriString, CVariableBindingResult::VBTNAMEDINDIVIDUAL);
											bindAnsRes->addResultVariableBinding(bindingResult);
										} else {
											if (bindAnsRes->getResultCount() <= i) {
												bindingResult = reserveBindingsList.takeFirst();
												bindingResult->initVariableBinding(uriString, CVariableBindingResult::VBTNAMEDINDIVIDUAL);
												bindAnsRes->addResultVariableBinding(bindingResult);
											} else {
												bindingResult = (CVariableBindingStringResult*)bindAnsRes->getResult(i);
												if (!bindingResult) {
													bindingResult = reserveBindingsList.takeFirst();
													bindAnsRes->setResult(i, bindingResult);
												}
												bindingResult->initVariableBinding(uriString, CVariableBindingResult::VBTNAMEDINDIVIDUAL);
											}
										}
									} else if (type == LIBRDF_NODE_TYPE_LITERAL) {
										const char* literalValue = (const char*)librdf_node_get_literal_value(valueNode);
										librdf_uri* datatypeUri = librdf_node_get_literal_value_datatype_uri(valueNode);
										QString literalString = QString::fromUtf8((const char*)literalValue);

										QString literalDatatypeString = "\"" + literalString;
										if (!librdf_uri_equals(plainLiteralUri, datatypeUri)) {
											unsigned char* datatypeString = librdf_uri_as_string(datatypeUri);
											QString datatypeQString = QString::fromUtf8((const char*)datatypeString);
											literalDatatypeString += "\"^^" + datatypeQString;
										} else {
											literalDatatypeString += "\"";
										}

										CVariableBindingStringResult* bindingResult = nullptr;
										if (!existBindsAnswersStreamingResult) {
											bindingResult = new CVariableBindingStringResult(literalDatatypeString, CVariableBindingResult::VBTLITERAL);
											bindAnsRes->addResultVariableBinding(bindingResult);
										} else {
											if (bindAnsRes->getResultCount() <= i) {
												bindingResult = reserveBindingsList.takeFirst();
												bindingResult->initVariableBinding(literalDatatypeString, CVariableBindingResult::VBTLITERAL);
												bindAnsRes->addResultVariableBinding(bindingResult);
											} else {
												bindingResult = (CVariableBindingStringResult*)bindAnsRes->getResult(i);
												if (!bindingResult) {
													bindingResult = reserveBindingsList.takeFirst();
													bindAnsRes->setResult(i, bindingResult);
												}
												bindingResult->initVariableBinding(literalDatatypeString, CVariableBindingResult::VBTLITERAL);
											}
										}
									}
									librdf_free_node(valueNode);
								} else {
									if (!existBindsAnswersStreamingResult) {
										bindAnsRes->addResultVariableBinding(nullptr);
									} else {
										if (bindAnsRes->getResultCount() <= i) {
											bindAnsRes->addResultVariableBinding(nullptr);
										} else {
											CVariableBindingStringResult* bindingResult = (CVariableBindingStringResult*)bindAnsRes->getResult(i);
											if (bindingResult) {
												reserveBindingsList.append(bindingResult);
											}
											bindAnsRes->setResult(i, nullptr);
										}
									}
								}
							}

							if (!existBindsAnswersStreamingResult) {
								resultWriter.addVariableBindingAnswerToResult(bindsAnswersResult, bindAnsRes);
							} else {
								resultWriter.addReusedVariableBindingAnswerToResult(bindsAnswersResult, bindAnsRes);
							}

						}

						if (answerCount % flushSize == 0 && existBindsAnswersStreamingResult) {
							answersWriteable &= existBindsAnswersStreamingResult->flushResults();
							flushSize = flushSize * flushSizeIncreasingFactor;
						}

						librdf_query_results_next(results);
					}
					delete[] values;
					if (existBindsAnswersStreamingResult) {
						delete reuseBindAnsRes;
					}

				} else if (results && librdf_query_results_is_boolean(results)) {
					++answerCount;
					CBooleanQueryResult* boolRes = nullptr;
					int result = librdf_query_results_get_boolean(results);
					if (result > 0) {
						boolRes = new CBooleanQueryResult();
						boolRes->setResult(true);
					} else if (result == 0) {
						boolRes = new CBooleanQueryResult();
						boolRes->setResult(false);
					}
					if (boolRes) {
						query->setQueryResult(boolRes);
					}
				}

				if (results) {
					librdf_free_query_results(results);
				}
				librdf_free_query(query->getRedlandQuery());

				redlandRasqalQueryProcData->setProcessingFinished();
				cint64 processingTime = redlandRasqalQueryProcData->getTotalProcessingTime();

				QString realReqString;
				CAnsweringHandlingStatistics* stats = mOntoAnsweringItem->getAnsweringHandlingStatistics();
				if (stats) {
					CAnsweringHandlingStatistics* initialStats = redlandRasqalQueryProcData->getQueryProcessingInitializedAnsweringStatistics();
					if (initialStats) {
						cint64 reqRealReqCount = stats->getRequestedRealizationRequirementCount() - initialStats->getRequestedRealizationRequirementCount();
						if (reqRealReqCount > 0) {
							realReqString += QString(", using %1 reasoning-based sub-queries").arg(reqRealReqCount);
						}
					}
				}


				LOG(INFO, getDomain(), logTr("Complex query processing with Redland Rasqal determined %3 answers in %1 ms%2.").arg(processingTime).arg(realReqString).arg(answerCount), this);
				if (mOntoAnsweringItem->isLogProcessingFinishedStatisticsConfigured()) {
					QString statusString = redlandRasqalQueryProcData->getResolver()->getIterationStatusString();
					LOG(INFO, getDomain(), statusString, this);
				}

			}

			bool COptimizedComplexCompositionRedlandRasqalAnsweringHandler::processRequirementMessage(CAnsweringMessageData* message, CAnswererContext* answererContext) {
				while (message) {
					CAnsweringMessageData::ANSWERINGMESSAGEDATA messageType = message->getAnsweringMessageDataType();
					if (messageType == CAnsweringMessageData::REQUIREMENTCOMPLETED) {
						CAnsweringMessageDataRequirementCompleted* reqCompMessage = (CAnsweringMessageDataRequirementCompleted*)message;
						CAnsweringMessageDataRequirementCompleted::REQUIREMENTTYPE reqType = reqCompMessage->getRequirementType();
						if (reqType == CAnsweringMessageDataRequirementCompletedSubQueryAnswering::SUBQUERYANSWERINGREQUIREMENT) {
							CAnsweringMessageDataRequirementCompletedSubQueryAnswering* compSubQueryAnsReqMess = (CAnsweringMessageDataRequirementCompletedSubQueryAnswering*)reqCompMessage;
							CComplexQueryCompositionRedlandRasqalProcessingData* redlandRasqalQueryProcData = (CComplexQueryCompositionRedlandRasqalProcessingData*)compSubQueryAnsReqMess->getProcessingData();
							redlandRasqalQueryProcData->decSubQueryWaitingCount();
							if (!redlandRasqalQueryProcData->isSubQueryWaiting()) {
								mOntoAnsweringItem->addScheduledQueriesProcessingData(redlandRasqalQueryProcData);
								if (!redlandRasqalQueryProcData->hasRemainingSubQueriesProcessing()) {
									redlandRasqalQueryProcData->setSubQueryProcessingFinished(true);
								}
							}
						}
					}
					message = message->getNext();
				}
				return false;				
			}



			

			bool COptimizedComplexCompositionRedlandRasqalAnsweringHandler::isAnsweringFinished(CAnswererContext* answererContext) {
				return !mOntoAnsweringItem->isCurrentlyAnsweringQueryCount();
			}






			CAnsweringProgress* COptimizedComplexCompositionRedlandRasqalAnsweringHandler::getAnsweringProgress() {
				if (mCurrentProcessingRedlandRasqalQueryProcData && mOntoAnsweringItem->isLogProcessingProgressStatisticsConfigured()) {
					QString statusString = mCurrentProcessingRedlandRasqalQueryProcData->getResolver()->getIterationStatusString();
					LOG(INFO, getDomain(), statusString, this);
				}
				return COptimizedComplexCompositionAnsweringHandler::getAnsweringProgress();
			}








		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REDLAND_INTEGRATION
