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

#include "CSPARQLRedlandRasqalQueryParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLRedlandRasqalQueryParser::CSPARQLRedlandRasqalQueryParser(CConcreteOntology* baseOntology, CConcreteOntology* ontology, CConfigurationBase* config) {
			mBaseOntology = baseOntology;
			mOntology = ontology;
			mConfig = config;
			mQuery = nullptr;
		}


		CSPARQLRedlandRasqalQueryParser::~CSPARQLRedlandRasqalQueryParser() {
		}



		bool CSPARQLRedlandRasqalQueryParser::parseQueryFile(const QString& filename) {
			bool successfullParsed = true;

			// parse file
			QFile file(filename);
			if (file.open(QIODevice::ReadOnly)) {

				QStringList partStringList;
				while (!file.atEnd()) {
					QString line(file.readLine());
					if (!line.isEmpty()) {
						partStringList.append(line);
					}
				}
				if (!partStringList.isEmpty()) {
					processQueryText(partStringList.join("\r\n"));
				}

				file.close();

			} else {
				successfullParsed = false;
				LOG(ERROR,"::Konclude::Parser::SPARQLSimpleQueryParser",logTr("File '%1' not found or couldn't be opened.").arg(filename),this);
			}

			return successfullParsed;
		}


		bool CSPARQLRedlandRasqalQueryParser::parseQueryText(const QString& filetext) {
			return processQueryText(filetext);
		}


		bool CSPARQLRedlandRasqalQueryParser::parseQueryTextList(const QStringList& queryTextList) {
			return processQueryText(queryTextList.join(" "));
		}

		CQuery* CSPARQLRedlandRasqalQueryParser::getQuery() {
			return mQuery;
		}


		/** A query object */
		struct librdf_query_s {
			librdf_world *world;
			int usage;
			void *context;
			struct librdf_query_factory_s* factory;

			/* list of all the results for this query */
			librdf_query_results* results;
		};

		typedef struct {
			librdf_query *query;        /* librdf query object */
			librdf_model *model;
			rasqal_query *rq;
			rasqal_query_results *results;
			char *language;            /* rasqal query language name to use */
			unsigned char *query_string;
			librdf_uri *uri;           /* base URI or NULL */

			int errors;
			int warnings;
		} librdf_query_rasqal_context;




		struct QueryGenerationData {
			QSet<QString> additionalKBDependencies;
			rasqal_world* rasqal_world_ptr;
		};



		int	rasqal_graph_pattern_visit_fn(rasqal_query *query, rasqal_graph_pattern *gp, void *user_data) {
			QueryGenerationData* queryGenData = (QueryGenerationData*)user_data;
			rasqal_world* rasqal_world_ptr = queryGenData->rasqal_world_ptr;
			raptor_world* raptor_world_ptr = rasqal_world_get_raptor(rasqal_world_ptr);

			if (RASQAL_GRAPH_PATTERN_OPERATOR_BASIC == rasqal_graph_pattern_get_operator(gp)) {
				raptor_sequence* tripleSeq = rasqal_graph_pattern_get_triples(query, gp);
				// check if it can be processed by Konclude
				bool supported = true;
				int seqSize = raptor_sequence_size(tripleSeq);
				for (int i = 0; i < seqSize; ++i) {
					rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(tripleSeq, i);
					if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->predicate)) {
						supported = false;
					} else if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->object)) {
						if (RASQAL_LITERAL_URI == rasqal_literal_get_type(triple->predicate)) {
							const char* predicateString = (const char*)rasqal_literal_as_string(triple->predicate);
							if (strcmp(predicateString, "http://www.w3.org/1999/02/22-rdf-syntax-ns#type") == 0) {
								supported = false;
							}
						}
					}
					
				}

				if (!supported) {
					return -1;
				}

			} else if (RASQAL_GRAPH_PATTERN_OPERATOR_GRAPH == rasqal_graph_pattern_get_operator(gp)) {
				rasqal_literal* lit = rasqal_graph_pattern_get_origin(gp);
				if (rasqal_literal_get_type(lit) == RASQAL_LITERAL_VARIABLE) {
					return -1;
				}
				const unsigned char* graphUri = rasqal_literal_as_string(lit);
				QString graphUriString = QString::fromUtf8((const char*)graphUri);
				queryGenData->additionalKBDependencies.insert(graphUriString);
			}

			return 0;
		}




		void rasqal_log_handler_parsing_error_notifier(void *user_data, raptor_log_message *message) {
			QStringList* messageList = (QStringList*)user_data;
			if (message->level <= 50 && messageList) {
				messageList->append(QString(QString("%1").arg(message->text)));
			}
		}



		int redland_log_handler_parsing_error_notifier(void *user_data, librdf_log_message *message) {
			QStringList* messageList = (QStringList*)user_data;
			if (message->level <= 50 && messageList) {
				messageList->append(QString(QString("%1").arg(message->message)));
			}
			return 1;
		}




		bool CSPARQLRedlandRasqalQueryParser::processQueryText(const QString& queryText) {

			QList<CQuerySPARQLBasicGraphPatternExpression*> sparqlQueryList;

			librdf_world* world;

			world = librdf_new_world();
			librdf_world_open(world);
			rasqal_world* rasqal_world_ptr = librdf_world_get_rasqal(world);

			QueryGenerationData queryGenFunColl;
			queryGenFunColl.rasqal_world_ptr = rasqal_world_ptr;


			QStringList messageList;
			rasqal_world_set_log_handler(rasqal_world_ptr, &messageList, &rasqal_log_handler_parsing_error_notifier);
			librdf_world_set_logger(world, &messageList, &redland_log_handler_parsing_error_notifier);

			QByteArray queryByteDataArray = queryText.toUtf8();
			const unsigned char* queryString = (const unsigned char *)queryByteDataArray.constData();
			librdf_query* redlandQuery = librdf_new_query(world, "sparql", NULL, queryString, NULL);


			rasqal_query *rq;


			librdf_query_rasqal_context *context = (librdf_query_rasqal_context*)((librdf_query_s*)redlandQuery)->context;
			int err = rasqal_query_prepare(context->rq, context->query_string, (raptor_uri*)context->uri);

			rasqal_query *rasqalQuery = context->rq;
			if (!err && rasqalQuery) {
				if (rasqal_query_graph_pattern_visit2(rasqalQuery, rasqal_graph_pattern_visit_fn, &queryGenFunColl) == 0) {
					queryGenFunColl.additionalKBDependencies.remove(mOntology->getOntologyName());
					mQuery = new CRedlandRasqalBGPsCompositionQuery(mBaseOntology, mOntology, queryGenFunColl.additionalKBDependencies, queryText, world, redlandQuery, rasqalQuery, mConfig);
					if (!messageList.isEmpty()) {
						LOG(WARN, "Konclude::Parser::RedlandRasqalSPARQLQueryParser", logTr("Redland Rasqal query parsing and preparation warnings: '%1'.").arg(messageList.join("; ")), this);
					}
					return true;
				} else {
					LOG(ERROR, "Konclude::Parser::RedlandRasqalSPARQLQueryParser", logTr("Failed to create query because of unsupported variable/term usage in graph patterns."), this);
				}
			} else {
				if (!messageList.isEmpty()) {
					LOG(ERROR, "Konclude::Parser::RedlandRasqalSPARQLQueryParser", logTr("Query parsing and preparation failed due to Redland Rasqal parsing errors: '%1'.").arg(messageList.join("; ")), this);
				} else {
					LOG(ERROR, "Konclude::Parser::RedlandRasqalSPARQLQueryParser", logTr("Failed to create query because of unknown error."), this);
				}
			}

			return false;
		}

	


	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_REDLAND_INTEGRATION
