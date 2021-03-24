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

#include "CSPARQLRedlandRasqalReasoningBGPQueryParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLRedlandRasqalReasoningBGPQueryParser::CSPARQLRedlandRasqalReasoningBGPQueryParser(CSPARQLQueryBuilder *queryBuilder, COntologyBuilder* ontoBuilder, CConcreteOntology* ontology, CConfigurationBase* config) : CSPARQLExtendedMappingBuildingParser(ontoBuilder, ontology) {
			mQueryBuilder = queryBuilder;
			mConfig = config;
			mConfWriteQueryStructure = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Debugging.WriteRedlandRasqalQueryStructure", false);
			mConfForceDistinctSubQueries = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Answering.RedlandRasqalReasoningSubQueriesForceDistinct", false);

			mConfMinimizeBGPTriples = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Answering.RedlandRasqalMinimizeBGPSubQueryTriples", true);
			mConfPropagateBGPSubQueryDistinct = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Answering.RedlandRasqalReasoningSubBGPQueryDistinctPropagation", true);
			mConfReduceBGPSubQueryAnswerVariables = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Answering.RedlandRasqalReasoningSubBGPQueryAnswerVariableReduction", true);
			mConfAllowResultStreamingFromSingleSubQuery = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Answering.RedlandRasqalSingleSubBGPQueryAllowsDirectResultStreaming", true);


			mConfSubQueriesDependencyIdentification = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Answering.RedlandRasqalSubQueriesDependencyIdentification", true);
			mConfSubQueriesFollowingRestrictionsIntegration = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Answering.RedlandRasqalSubQueriesFollowingRestrictionsIntegration", true);

			mConfWriteSubQueryTriples = mConfWriteQueryStructure;
		}


		CSPARQLRedlandRasqalReasoningBGPQueryParser::~CSPARQLRedlandRasqalReasoningBGPQueryParser() {
		}


		
		struct rasqal_literal_s {
			rasqal_world *world;

			int usage;

			rasqal_literal_type type;

			/* UTF-8 string, pattern, qname, blank, double, float, decimal, datetime */
			const unsigned char *string;
			unsigned int string_len;

			union {
				/* integer and boolean types */
				int integer;
				/* double and float */
				double floating;
				/* uri (can be temporarily NULL if a qname, see flags below) */
				raptor_uri* uri;
				/* variable */
				rasqal_variable* variable;
				/* decimal */
				rasqal_xsd_decimal* decimal;
				/* datetime  */
				rasqal_xsd_datetime* datetime;
				/* date  */
				rasqal_xsd_date* date;
			} value;

			/* for string */
			char *language;
			raptor_uri *datatype;

			/* various flags for literal types:
			*  pattern  regex flags
			*  string   datatype of qname
			*  uri      qname of URI not yet expanded (temporary)
			*/
			const unsigned char *flags;

			rasqal_literal_type parent_type;

			int valid;
		};


		QString	getRasqalVariableString(rasqal_query *query, rasqal_variable* variable) {
			QString string;
			if (variable->type == RASQAL_VARIABLE_TYPE_ANONYMOUS) {
				string = QString("_:") + QString::fromUtf8((const char*)variable->name);
			} else {
				string = QString("?") + QString::fromUtf8((const char*)variable->name);
			}
			return string;
		}


		QString	getRasqalLiteralString(rasqal_query *query, rasqal_literal* literal) {
			QString string;
			if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(literal)) {
				rasqal_literal_s* var = (rasqal_literal_s*)literal;
				if (var->value.variable->type == RASQAL_VARIABLE_TYPE_ANONYMOUS) {
					string = QString("_:") + QString::fromUtf8((const char*)var->value.variable->name);
				} else {
					string = QString("?") + QString::fromUtf8((const char*)var->value.variable->name);
				}
				// does not work somehow, seems to be a bug
				//string = QString::fromUtf8((const char*)rasqal_literal_as_string(literal));
			} else if (RASQAL_LITERAL_URI == rasqal_literal_get_type(literal)) {
				string = QString::fromUtf8((const char*)rasqal_literal_as_string(literal));
			} else if (RASQAL_LITERAL_STRING == rasqal_literal_get_type(literal)) {
				string = QString("\"%1\"").arg(QString::fromUtf8((const char*)rasqal_literal_as_string(literal)));
			} else if (RASQAL_LITERAL_XSD_STRING == rasqal_literal_get_type(literal)) {
				string = QString("\"%1\"^^xsd:string").arg(QString::fromUtf8((const char*)rasqal_literal_as_string(literal)));
			} else if (RASQAL_LITERAL_BOOLEAN == rasqal_literal_get_type(literal)) {
				string = QString("\"%1\"^^%2").arg(QString::fromUtf8((const char*)rasqal_literal_as_string(literal))).arg(PREFIX_XML_BOOLEAN_DATATYPE);
			} else if (RASQAL_LITERAL_INTEGER == rasqal_literal_get_type(literal)) {
				string = QString("\"%1\"^^%2").arg(QString::fromUtf8((const char*)rasqal_literal_as_string(literal))).arg(PREFIX_XML_INTEGER_DATATYPE);
			} else if (RASQAL_LITERAL_FLOAT == rasqal_literal_get_type(literal)) {
				string = QString("\"%1\"^^%2").arg(QString::fromUtf8((const char*)rasqal_literal_as_string(literal))).arg(PREFIX_XML_FLOAT_DATATYPE);
			} else if (RASQAL_LITERAL_DOUBLE == rasqal_literal_get_type(literal)) {
				string = QString("\"%1\"^^%2").arg(QString::fromUtf8((const char*)rasqal_literal_as_string(literal))).arg(PREFIX_XML_DOUBLE_DATATYPE);
			} else if (RASQAL_LITERAL_DECIMAL == rasqal_literal_get_type(literal)) {
				string = QString("\"%1\"^^%2").arg(QString::fromUtf8((const char*)rasqal_literal_as_string(literal))).arg(PREFIX_XML_DECIMAL_DATATYPE);
			} else if (RASQAL_LITERAL_DATETIME == rasqal_literal_get_type(literal)) {
				string = QString("\"%1\"^^%2").arg(QString::fromUtf8((const char*)rasqal_literal_as_string(literal))).arg(PREFIX_XML_DATETIME_DATATYPE);
			}
			return string;
		}



		typedef struct {
			rasqal_query* query;

			raptor_sequence* variables;

			unsigned int wildcard : 1;

			int distinct;
		} rasqal_projection;


		struct rasqal_query_s {
			rasqal_world* world; /* world object */

			int usage; /* reference count - 1 for itself, plus for query_results */

			unsigned char* query_string;
			size_t query_string_length; /* length including NULs */

			raptor_namespace_stack* namespaces;

			/* query graph pattern, containing the sequence of graph_patterns below */
			rasqal_graph_pattern* query_graph_pattern;

			/* the query verb - in SPARQL terms: SELECT, CONSTRUCT, DESCRIBE or ASK */
			rasqal_query_verb verb;

			/* WAS: selects - sequence of rasqal_variable# */
			raptor_sequence* unused10;     /* ... rasqal_variable* names only */

										   /* sequences of ... */
			raptor_sequence* data_graphs; /* ... rasqal_data_graph*          */
										  /* NOTE: Cannot assume that triples are in any of
										  * graph pattern use / query execution / document order
										  */
			raptor_sequence* triples;     /* ... rasqal_triple*              */
			raptor_sequence* prefixes;    /* ... rasqal_prefix*              */
			raptor_sequence* constructs;  /* ... rasqal_triple*       SPARQL */
			raptor_sequence* optional_triples; /* ... rasqal_triple*  SPARQL */
			raptor_sequence* describes;   /* ... rasqal_literal* (var or URIs) SPARQL */

										  /* WAS: distinct 0..2; now projection->distinct */
			unsigned int unused9;

			/* WAS: result limit LIMIT (>=0) or <0 if not given */
			int unused4;

			/* WAS: result offset OFFSET (>=0) or <0 if not given */
			int unused5;

			/* WAS: wildcard flag; now projection->wildcard  */
			int unused12;

			/* flag: non-0 if query has been prepared */
			int prepared;

			rasqal_variables_table* vars_table;

			/* WAS: The number of selected variables: these are always the first
			* in the variables table and are the ones returned to the user.
			*/
			int unused11;

			/* INTERNAL 2D array of:
			*   width (number of total variables)
			*   height (number of triples)
			* marking how a variable is mentioned/used in a TRIPLE PATTERN
			* Each triple pattern has a row and the short values are per-variable
			* with flags from #rasqal_triples_use_map_flags
			*/
			unsigned short* triples_use_map;

			/* can be filled with error location information */
			raptor_locator locator;

			/* base URI of this query for resolving relative URIs in queries */
			raptor_uri* base_uri;

			/* non 0 if query had fatal error in parsing and cannot be executed */
			int failed;

			/* stuff for our user */
			void* user_data;

			/* former state for generating blank node IDs; now in world object */
			int unused1;
			char *unused2;
			size_t unused3;

			/* query engine specific stuff */
			void* context;

			struct rasqal_query_language_factory_s* factory;

			rasqal_triples_source_factory* triples_source_factory;

			/* sequence of query results made from this query */
			raptor_sequence* results;

			/* incrementing counter for declaring prefixes in order of appearance */
			int prefix_depth;

			/* WAS: sequence of order condition expressions */
			void* unused6;

			/* WAS: sequence of group by condition expressions */
			void* unused7;

			/* INTERNAL rasqal_literal_compare / rasqal_expression_evaluate flags */
			int compare_flags;

			/* Number of graph patterns in this query */
			int graph_pattern_count;

			/* Graph pattern shared pointers by gp index (after prepare) */
			raptor_sequence* graph_patterns_sequence;

			/* Features */
			int features[RASQAL_FEATURE_LAST + 1];

			/* Name of requested query results syntax.  If present, this
			* is the name used for constructing a rasqal_query_formatter
			* from the results.
			*/
			char* query_results_formatter_name;

			/* flag: non-0 if EXPLAIN was given */
			int explain;

			/* INTERNAL lexer internal data */
			void* lexer_user_data;

			/* INTERNAL flag for now: non-0 to store results otherwise lazy eval results */
			int store_results;

			/* INTERNAL 2D array of:
			*   width (number of total variables)
			*   height (number of graph patterns)
			* marking how a variable is mentioned/used in a GP
			* Each graph pattern has a row and the short values are per-variable
			* with flags:
			*   1  RASQAL_USE_IN_SCOPE
			*   2  RASQAL_USE_MENTIONED_HERE
			*   4  RASQAL_USE_BOUND_HERE
			*/
			unsigned short* variables_use_map;

			/* sequence of #rasqal_update_operation when @verb is
			* INSERT (deprecated), DELETE (deprecated) or UPDATE
			*/
			raptor_sequence* updates;

			/* WAS: sequence of (group by ...) having condition expressions */
			raptor_sequence* unused8;

			/* INTERNAL solution modifier */
			//rasqal_solution_modifier* modifier;
			void* modifier;

			/* INTERNAL SELECT bindings */
			//rasqal_bindings* bindings;
			void* bindings;

			/* INTERNAL static structure for expression evaluation*/
			rasqal_evaluation_context *eval_context;

			/* INTERNAL flag: non-0 if user set a random seed via RASQAL_FEATURE_RAND_SEED */
			unsigned int user_set_rand : 1;

			/* Variable projection (or NULL when invalid such as for ASK) */
			rasqal_projection* projection;
		};


		struct rasqal_graph_pattern_s {
			rasqal_query* query;

			/* operator for this graph pattern's contents */
			rasqal_graph_pattern_operator op;

			raptor_sequence* triples;          /* ... rasqal_triple*         */
			raptor_sequence* graph_patterns;   /* ... rasqal_graph_pattern*  */

			int start_column;
			int end_column;

			/* the FILTER / LET expression */
			rasqal_expression* filter_expression;

			/* index of the graph pattern in the query (0.. query->graph_pattern_count-1) */
			int gp_index;

			/* Graph literal / SERVICE literal */
			rasqal_literal *origin;

			/* Variable for LET graph pattern */
			rasqal_variable *var;

			/* SELECT projection */
			rasqal_projection* projection;

			/* SELECT modifiers */
			//rasqal_solution_modifier* modifier;
			void* modifier;

			/* SILENT flag for SERVICE graph pattern */
			unsigned int silent : 1;

			/* SELECT graph pattern: sequence of #rasqal_data_graph */
			raptor_sequence* data_graphs;

			/* VALUES bindings for VALUES and sub-SELECT graph patterns */
			//rasqal_bindings* bindings;
			void* bindings;
		};


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


		class QueryGenerationData {
			public:
				QString knowledgeBase;
				rasqal_world* rasqal_world_ptr = nullptr;
				bool defaultKnowledgeBase = false;
				QSet<rasqal_graph_pattern*> knowledgeBaseRelevantBGPsSet;
				function<cint64(rasqal_query *query, CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData* bgpData, QList<CRDFStringTriple>* tripleStringList, QStringList* varStringList, QHash<QString, cint64>* varStringUsageCountHash)> selectQueryGenerationFn;
				function<cint64(rasqal_query *query, CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData* bgpData, QList<CRDFStringTriple>* tripleStringList)> askQueryGenerationFn;

				
				QueryGenerationData(const QString& kb, bool defaultKB, const QSet<rasqal_graph_pattern*>& kbRelevantBGPsSet) {
					knowledgeBase = kb;
					defaultKnowledgeBase = defaultKB;
					rasqal_world_ptr = nullptr;
					knowledgeBaseRelevantBGPsSet = kbRelevantBGPsSet;
				}
		};






		int	get_connected_triples_and_variables(const QString startVariableString, const QSet<QString>& variableSet, QList<CRDFStringTriple*>& remainingTripleStringList, QSet<QString>& connectedVariableSet, QList<CRDFStringTriple*>& connectedTripleStringList) {
			int connectedVariableCount = 0;
			QHash<QString, cint64> variableTriplePosHash;
			cint64 triplePos = 0;
			for (CRDFStringTriple* stringTriple : remainingTripleStringList) {
				for (cint64 i = 0; stringTriple && i < 3; ++i) {
					const QString& term = stringTriple->string[i];
					if (variableSet.contains(term)) {
						variableTriplePosHash.insertMulti(term, triplePos);
					}
				}
				triplePos++;
			}

			// handle blank nodes
			
			QList<cint64> processingTripleIds = variableTriplePosHash.values(startVariableString);
			if (!processingTripleIds.isEmpty()) {
				connectedVariableSet.insert(startVariableString);
				QSet<cint64> processedTripleIds;
				while (!processingTripleIds.isEmpty()) {
					cint64 tripleId = processingTripleIds.takeFirst();
					if (!processedTripleIds.contains(tripleId)) {
						processedTripleIds.insert(tripleId);

						CRDFStringTriple* stringTriple = remainingTripleStringList.at(tripleId);
						remainingTripleStringList.replace(tripleId, nullptr);
						connectedTripleStringList.replace(tripleId, stringTriple);
						for (cint64 i = 0; i < 3; ++i) {
							const QString& term = stringTriple->string[i];
							if (variableSet.contains(term) && !connectedVariableSet.contains(term)) {
								connectedVariableSet.insert(term);
								processingTripleIds.append(variableTriplePosHash.values(term));
							}
						}
					}
				}
			}
			return connectedVariableSet.count();
		}






		CSPARQLRedlandRasqalReasoningBGPQueryParser::CBGPSubQueryGenerationData* CSPARQLRedlandRasqalReasoningBGPQueryParser::createSubQueryGenerationData(rasqal_query *query, CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData* bgpData) {
			CBGPSubQueryGenerationData* genData = new CBGPSubQueryGenerationData();
			genData->bgpData = bgpData;
			genData->bgpData->subQueryId = mNextSubQueryId++;
			genData->subQueryId = genData->bgpData->subQueryId;
			mBGPSubQueryIdDataHash.insertMulti(bgpData->bgp, genData);
			mIdSubQueryIdDataHash.insert(genData->subQueryId, genData);
			return genData;
		}





		bool CSPARQLRedlandRasqalReasoningBGPQueryParser::identifyBGPBasedSubQueryTriples(rasqal_graph_pattern* gp) {

			raptor_sequence* tripleSeq = rasqal_graph_pattern_get_triples(mRasqalQuery, gp);
			// check if it can be processed by Konclude
			bool supported = true;
			int seqSize = raptor_sequence_size(tripleSeq);
			//for (int i = 0; i < seqSize; ++i) {
			//	rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(tripleSeq, i);
			//	if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->predicate)) {
			//		supported = false;
			//	} else if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->object)) {
			//		if (RASQAL_LITERAL_URI == rasqal_literal_get_type(triple->predicate)) {
			//			const char* predicateString = (const char*)rasqal_literal_as_string(triple->predicate);
			//			if (strcmp(predicateString, "http://www.w3.org/1999/02/22-rdf-syntax-ns#type") == 0) {
			//				supported = false;
			//			}
			//		}
			//	}

			//}

			if (supported) {
				QList<CRDFStringTriple*> tripleStringList;
				QHash<QString, rasqal_literal*> varStringLitHash;
				QHash<rasqal_literal*, QString> varLitStringHash;
				QHash<QString, cint64> varStringUsageCountHash;
				QStringList varStringList;
				for (int i = 0; i < seqSize; ++i) {
					rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(tripleSeq, i);
					CRDFStringTriple* stringTriple = new CRDFStringTriple();
					stringTriple->string[0] = getRasqalLiteralString(mRasqalQuery, triple->subject);
					stringTriple->string[1] = getRasqalLiteralString(mRasqalQuery, triple->predicate);
					stringTriple->string[2] = getRasqalLiteralString(mRasqalQuery, triple->object);
					tripleStringList.append(stringTriple);

					if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->subject)) {
						if (!varStringLitHash.contains(stringTriple->string[0])) {
							varStringList.append(stringTriple->string[0]);
						}
						varStringUsageCountHash[stringTriple->string[0]]++;
						varStringLitHash.insert(stringTriple->string[0], triple->subject);
						varLitStringHash.insert(triple->subject, stringTriple->string[0]);
					}
					if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->predicate)) {
						if (!varStringLitHash.contains(stringTriple->string[1])) {
							varStringList.append(stringTriple->string[1]);
						}
						varStringUsageCountHash[stringTriple->string[1]]++;
						varStringLitHash.insert(stringTriple->string[1], triple->predicate);
						varLitStringHash.insert(triple->predicate, stringTriple->string[1]);
					}
					if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->object)) {
						if (!varStringLitHash.contains(stringTriple->string[2])) {
							varStringList.append(stringTriple->string[2]);
						}
						varStringUsageCountHash[stringTriple->string[2]]++;
						varStringLitHash.insert(stringTriple->string[2], triple->object);
						varLitStringHash.insert(triple->object, stringTriple->string[2]);
					}
				}






				QByteArray defaultURIBytes = QString("x").toUtf8();

				rasqal_query_s* squery = (rasqal_query_s*)mRasqalQuery;
				raptor_sequence* queryTripleSeq = squery->triples;

				rasqal_graph_pattern_s *gps = (rasqal_graph_pattern_s*)gp;
				int triplesStartColumn = gps->start_column;
				int triplesEndColumn = gps->end_column;
				int triplesCount = triplesEndColumn - triplesStartColumn + 1;

				int minUsedTriplesStartColumn = gps->end_column;
				int maxUsedTriplesEndColumn = gps->start_column;

				QSet<QString> variableSet = varStringList.toSet();
				QSet<QString> usedVariableSet;
				QSet<QString> remainVariableSet = variableSet;

				for (const QString& varString : varStringList) {
					if (!usedVariableSet.contains(varString)) {
						remainVariableSet.remove(varString);
						QSet<QString> connectedVariableSet;
						QList<CRDFStringTriple*> connectedTripleStringList;
						for (cint64 i = 0; i < triplesCount; ++i) {
							connectedTripleStringList.append(nullptr);
						}
						cint64 connectedVarCount = get_connected_triples_and_variables(varString, variableSet, tripleStringList, connectedVariableSet, connectedTripleStringList);
						usedVariableSet += connectedVariableSet;

						QSet<cint64> usedtriplePosSet;
						QList<CRDFStringTriple> convertingStringTriplesList;
						for (cint64 i = 0; i < triplesCount; ++i) {
							CRDFStringTriple* tripleString = connectedTripleStringList.at(i);
							if (tripleString) {
								usedtriplePosSet.insert(i);
								CRDFStringTriple convTripleString(*tripleString);
								convertingStringTriplesList.append(convTripleString);
							}
						}
						//if (connectedVariableSet.count() == variableSet.count()) {
						//	// last variable, i.e., last query, also integrate fact entailment checking triples, e.g., A(a)
						//	for (CRDFStringTriple* tripleString : tripleStringList) {
						//		if (tripleString) {
						//			CRDFStringTriple convTripleString(*tripleString);
						//			convertingStringTriplesList.append(convTripleString);
						//		}
						//	}
						//}

						QSet<QString> distinguishedVariableSet;
						QStringList distinguishedVariableStringList;
						QStringList subQueryVarStringList;
						for (QString varString : varStringList) {
							if (connectedVariableSet.contains(varString)) {
								subQueryVarStringList.append(varString);

								QString nameOnlyReducedVarString = varString;
								if (nameOnlyReducedVarString.startsWith("$") || nameOnlyReducedVarString.startsWith("?")) {
									nameOnlyReducedVarString = nameOnlyReducedVarString.mid(1);

									cint64 count1 = 0;
									cint64 count2 = 0;
									if (!mQueryContainsSelectAll && mConfReduceBGPSubQueryAnswerVariables) {
										QString pattern = "(" + QRegularExpression::escape("?") + "|" + QRegularExpression::escape("$") + ")" + QRegularExpression::escape(nameOnlyReducedVarString) + "(?!\\w)";
										count1 = mQueryText.count(QRegularExpression(pattern));
									}
									cint64 bgpCount = varStringUsageCountHash.value("?" + nameOnlyReducedVarString) + varStringUsageCountHash.value("$" + nameOnlyReducedVarString);
									cint64 totalCount = count1 + count2;
									if (mQueryContainsSelectAll || mConfReduceBGPSubQueryAnswerVariables && totalCount > bgpCount) {
										distinguishedVariableSet.insert(varString);
										distinguishedVariableStringList.append(varString);
									}

								}
							}
						}


						CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData* bgpData = new CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData();
						bgpData->bgp = gp;
						bgpData->tripleStartPos = gps->end_column;
						bgpData->tripleEndPos = gps->start_column;

						CBGPSubQueryGenerationData* subQueryGenData = createSubQueryGenerationData(mRasqalQuery, bgpData);
						subQueryGenData->connectedTripleStringList = connectedTripleStringList;
						subQueryGenData->convertingStringTriplesList = convertingStringTriplesList;
						subQueryGenData->usedtriplePosSet = usedtriplePosSet;
						subQueryGenData->connectedVariableSet = connectedVariableSet;
						subQueryGenData->subQueryVarStringList = subQueryVarStringList;
						subQueryGenData->distinguishedVariableSet = distinguishedVariableSet;
						subQueryGenData->distinguishedVariableStringList = distinguishedVariableStringList;


						cint64 subQueryId = subQueryGenData->bgpData->subQueryId;


						QStringList::const_iterator varIt = distinguishedVariableStringList.constBegin(), varItEnd = distinguishedVariableStringList.constEnd();
						QSet<QString> replacedVarLitStringSet;
						if (varIt != varItEnd) {
							QString varString = *varIt;

							bool startTriplePos = true;
							for (int i = 0; i < triplesCount && (varIt != varItEnd || !remainVariableSet.isEmpty()); ++i) {
								if (connectedTripleStringList.at(i)) {
									int triplePos = triplesStartColumn + i;
									rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(queryTripleSeq, triplePos);
									if (varIt != varItEnd) {
										minUsedTriplesStartColumn = qMin(minUsedTriplesStartColumn, triplePos);
										maxUsedTriplesEndColumn = qMax(maxUsedTriplesEndColumn, triplePos);
									}
									if (varIt != varItEnd || !remainVariableSet.isEmpty()) {
										bgpData->tripleStartPos = qMin(bgpData->tripleStartPos, triplePos);
										bgpData->tripleEndPos = qMax(bgpData->tripleEndPos, triplePos);
									}


									cint64 encodedSubQueryId = subQueryId + 1;
									if (startTriplePos) {
										encodedSubQueryId = -encodedSubQueryId;
										startTriplePos = false;
									}
									triple->origin = rasqal_new_integer_literal(mRasqalWorld, RASQAL_LITERAL_INTEGER, encodedSubQueryId);


									//triple->origin = (rasqal_literal*)encodedSubQueryId;

									rasqal_literal* lastVarLit = varStringLitHash.value(varString);

									if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->subject)) {
										QString varLitString = varLitStringHash.value(triple->subject);
										if (!replacedVarLitStringSet.contains(varLitString) && varLitString == varString) {
											replacedVarLitStringSet.insert(varLitString);
											triple->subject = lastVarLit;
											if (varIt != varItEnd) {
												++varIt;
												if (varIt != varItEnd) {
													varString = *varIt;
													lastVarLit = varStringLitHash.value(varString);
												}
											}
										} else {
											triple->subject = rasqal_new_uri_literal(mRasqalWorld, raptor_new_uri(mRaptorWorld, (const unsigned char*)defaultURIBytes.constData()));
										}
									}
									if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->object)) {
										QString varLitString = varLitStringHash.value(triple->object);
										if (!replacedVarLitStringSet.contains(varLitString) && varLitString == varString) {
											triple->object = lastVarLit;
											replacedVarLitStringSet.insert(varLitString);
											if (varIt != varItEnd) {
												++varIt;
												if (varIt != varItEnd) {
													varString = *varIt;
													lastVarLit = varStringLitHash.value(varString);
												}
											}
										} else {
											triple->object = rasqal_new_uri_literal(mRasqalWorld, raptor_new_uri(mRaptorWorld, (const unsigned char*)defaultURIBytes.constData()));
										}
									}


								}
							}
						} else {
							bool hasTriple = false;
							bool startTriplePos = true;
							for (int i = 0; i < triplesCount && (!hasTriple || !remainVariableSet.isEmpty()); ++i) {
								if (connectedTripleStringList.at(i)) {
									int triplePos = triplesStartColumn + i;
									minUsedTriplesStartColumn = qMin(minUsedTriplesStartColumn, triplePos);
									maxUsedTriplesEndColumn = qMax(maxUsedTriplesEndColumn, triplePos);

									bgpData->tripleStartPos = qMin(bgpData->tripleStartPos, triplePos);
									bgpData->tripleEndPos = qMax(bgpData->tripleEndPos, triplePos);

									rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(queryTripleSeq, triplePos);

									cint64 encodedSubQueryId = subQueryId + 1;
									if (startTriplePos) {
										encodedSubQueryId = -encodedSubQueryId;
										startTriplePos = false;
									}
									triple->origin = rasqal_new_integer_literal(mRasqalWorld, RASQAL_LITERAL_INTEGER, encodedSubQueryId);

									if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->subject)) {
										hasTriple = true;
										if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->object)) {
											triple->object = rasqal_new_uri_literal(mRasqalWorld, raptor_new_uri(mRaptorWorld, (const unsigned char*)defaultURIBytes.constData()));
										}
									} else if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->object)) {
										hasTriple = true;
									}

								}
							}

						}


					}
				}



				// separate query for fact entailment checking triples, e.g., A(a)
				QList<CRDFStringTriple> convertingStringTriplesList;
				QSet<cint64> usedtriplePosSet;
				cint64 i = 0;
				for (CRDFStringTriple* tripleString : tripleStringList) {
					if (tripleString) {
						usedtriplePosSet.insert(i);
						CRDFStringTriple convTripleString(*tripleString);
						convertingStringTriplesList.append(convTripleString);
					}
					++i;
				}
				if (!convertingStringTriplesList.isEmpty()) {
					QStringList emptyVarList;
					bool canExtendBeginning = minUsedTriplesStartColumn > gps->start_column;



					CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData* bgpData = new CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData();
					bgpData->bgp = gp;
					bgpData->tripleStartPos = gps->end_column;
					bgpData->tripleEndPos = gps->start_column;

					CBGPSubQueryGenerationData* subQueryGenData = createSubQueryGenerationData(mRasqalQuery, bgpData);
					subQueryGenData->connectedTripleStringList = tripleStringList;
					subQueryGenData->convertingStringTriplesList = convertingStringTriplesList;
					subQueryGenData->usedtriplePosSet = usedtriplePosSet;

					cint64 subQueryId = subQueryGenData->bgpData->subQueryId;

					bool startTriplePos = true;
					for (int i = 0; i < triplesCount; ++i) {
						int triplePos = triplesStartColumn + i;
						if (triplePos >= minUsedTriplesStartColumn - 1 && canExtendBeginning || triplePos <= maxUsedTriplesEndColumn + 1 && !canExtendBeginning) {
							minUsedTriplesStartColumn = qMin(minUsedTriplesStartColumn, triplePos);
							maxUsedTriplesEndColumn = qMax(maxUsedTriplesEndColumn, triplePos);

							bgpData->tripleStartPos = qMin(bgpData->tripleStartPos, triplePos);
							bgpData->tripleEndPos = qMax(bgpData->tripleEndPos, triplePos);


							rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(queryTripleSeq, triplePos);
							cint64 encodedSubQueryId = subQueryId + 1;
							if (startTriplePos) {
								encodedSubQueryId = -encodedSubQueryId;
								startTriplePos = false;
							}
							triple->origin = rasqal_new_integer_literal(mRasqalWorld, RASQAL_LITERAL_INTEGER, encodedSubQueryId);

						}
					}
				}


				if (mConfMinimizeBGPTriples) {
					if (maxUsedTriplesEndColumn < minUsedTriplesStartColumn) {
						gps->end_column = gps->start_column;
					} else {
						gps->start_column = minUsedTriplesStartColumn;
						gps->end_column = maxUsedTriplesEndColumn;
					}
				}

			}

			return true;
		}



		bool CSPARQLRedlandRasqalReasoningBGPQueryParser::identifyBGPBasedSubQueries(rasqal_graph_pattern* gp) {
			bool subQueryGenerated = false;
			bool kbRelevantTriples = false;
			if (mKnowledgeBaseRelevantBGPsSet.contains(gp) && rasqal_graph_pattern_get_operator(gp) == RASQAL_GRAPH_PATTERN_OPERATOR_BASIC) {
				kbRelevantTriples = true;
			}
			if (kbRelevantTriples) {
				subQueryGenerated |= identifyBGPBasedSubQueryTriples(gp);
			}

			raptor_sequence* subGrapPatternSeq = rasqal_graph_pattern_get_sub_graph_pattern_sequence(gp);
			if (subGrapPatternSeq) {
				int subGrapPatternSeqSize = raptor_sequence_size(subGrapPatternSeq);
				int subBGPsSubQueryIdentificationCount = 0;
				for (int i = 0; i < subGrapPatternSeqSize; ++i) {
					rasqal_graph_pattern* subGP = (rasqal_graph_pattern*)raptor_sequence_get_at(subGrapPatternSeq, i);
					if (identifyBGPBasedSubQueries(subGP)) {
						++subBGPsSubQueryIdentificationCount;
					}
				}

				if (mConfSubQueriesDependencyIdentification && rasqal_graph_pattern_get_operator(gp) == RASQAL_GRAPH_PATTERN_OPERATOR_GROUP && subBGPsSubQueryIdentificationCount >= 1 && subGrapPatternSeqSize >= 2) {

					for (int i = 1; i < subGrapPatternSeqSize; ++i) {
						rasqal_graph_pattern* subGP = (rasqal_graph_pattern*)raptor_sequence_get_at(subGrapPatternSeq, i);
						QList<rasqal_graph_pattern*> subQueryBGPs;
						if (rasqal_graph_pattern_get_operator(subGP) == RASQAL_GRAPH_PATTERN_OPERATOR_OPTIONAL) {
							raptor_sequence* optSubGrapPatternSeq = rasqal_graph_pattern_get_sub_graph_pattern_sequence(subGP);
							if (optSubGrapPatternSeq) {
								int optSubGrapPatternSeqSize = raptor_sequence_size(optSubGrapPatternSeq);
								if (optSubGrapPatternSeqSize == 1) {
									rasqal_graph_pattern* optSubGP = (rasqal_graph_pattern*)raptor_sequence_get_at(optSubGrapPatternSeq, 0);
									subQueryBGPs.append(optSubGP);
								}
							}
						} else if (rasqal_graph_pattern_get_operator(subGP) == RASQAL_GRAPH_PATTERN_OPERATOR_UNION) {
							raptor_sequence* optSubGrapPatternSeq = rasqal_graph_pattern_get_sub_graph_pattern_sequence(gp);
							if (optSubGrapPatternSeq) {
								int optSubGrapPatternSeqSize = raptor_sequence_size(optSubGrapPatternSeq);
								if (optSubGrapPatternSeqSize >= 1) {
									for (int j = 0; j < optSubGrapPatternSeqSize; ++j) {
										rasqal_graph_pattern* optSubGP = (rasqal_graph_pattern*)raptor_sequence_get_at(optSubGrapPatternSeq, j);
										subQueryBGPs.append(optSubGP);
									}
								}
							}
						} else if (rasqal_graph_pattern_get_operator(subGP) == RASQAL_GRAPH_PATTERN_OPERATOR_BASIC) {
							subQueryBGPs.append(subGP);
						}


						for (rasqal_graph_pattern* sgp : subQueryBGPs) {
							QList<CBGPSubQueryGenerationData*> subQueryBGPDataList = mBGPSubQueryIdDataHash.values(sgp);
							for (CBGPSubQueryGenerationData* subQueryBGPData : subQueryBGPDataList) {


								for (int j = i - 1; j >= 0; --j) {
									rasqal_graph_pattern* prevSubQueryBGP = (rasqal_graph_pattern*)raptor_sequence_get_at(subGrapPatternSeq, j);
									QList<CBGPSubQueryGenerationData*> prevSubQueryBGPDataList = mBGPSubQueryIdDataHash.values(prevSubQueryBGP);
									for (CBGPSubQueryGenerationData* prevSubQueryBGPData : prevSubQueryBGPDataList) {

										bool commonVars = subQueryBGPData->distinguishedVariableSet.intersects(prevSubQueryBGPData->distinguishedVariableSet);
										if (commonVars) {
											subQueryBGPData->bgpData->dependentSubQueryIds.prepend(prevSubQueryBGPData->bgpData->subQueryId);
										}
									}
								}

							}
						}
					}

				}


				if (mConfSubQueriesFollowingRestrictionsIntegration && rasqal_graph_pattern_get_operator(gp) == RASQAL_GRAPH_PATTERN_OPERATOR_GROUP) {
					for (int i = 0; i < subGrapPatternSeqSize - 1; ++i) {
						rasqal_graph_pattern* subGP = (rasqal_graph_pattern*)raptor_sequence_get_at(subGrapPatternSeq, i);
						QList<CBGPSubQueryGenerationData*> subQueryBGPDataList = mBGPSubQueryIdDataHash.values(subGP);

						for (int j = i + 1; j < subGrapPatternSeqSize; ++j) {
							rasqal_graph_pattern* followingSubGP = (rasqal_graph_pattern*)raptor_sequence_get_at(subGrapPatternSeq, j);
							QList<CBGPSubQueryGenerationData*> followingSubQueryBGPDataList = mBGPSubQueryIdDataHash.values(followingSubGP);


							for (CBGPSubQueryGenerationData* subQueryBGPData : subQueryBGPDataList) {

								for (CBGPSubQueryGenerationData* followingSubQueryBGPData : followingSubQueryBGPDataList) {

									bool commonVars = subQueryBGPData->distinguishedVariableSet.intersects(followingSubQueryBGPData->distinguishedVariableSet);
									commonVars |= subQueryBGPData->additionalIntegratedVariableSet.intersects(followingSubQueryBGPData->distinguishedVariableSet);
									if (commonVars) {
										integrateFollowingSubQueryRestrictions(subQueryBGPData, followingSubQueryBGPData);
									}

								}
							}
						}

					}

				}


			}

			return subQueryGenerated;
		}


		bool CSPARQLRedlandRasqalReasoningBGPQueryParser::integrateFollowingSubQueryRestrictions(CBGPSubQueryGenerationData* subQueryBGPData, CBGPSubQueryGenerationData* followingSubQueryBGPData) {
			subQueryBGPData->additionalIntegratingStringTriplesList.append(followingSubQueryBGPData->convertingStringTriplesList);

			for (QString varString : followingSubQueryBGPData->connectedVariableSet) {
				if (!subQueryBGPData->connectedVariableSet.contains(varString)) {
					subQueryBGPData->additionalIntegratedVariableSet.insert(varString);
				}
			}
			return true;
		}





		bool CSPARQLRedlandRasqalReasoningBGPQueryParser::generateBGPBasedSubQueries(const QStringList& projectedVarList) {
			if (mConfWriteSubQueryTriples) {
				QFile file("./Debugging/Answering/RASQAL-Sub-Query-Triples.txt");
				if (file.open(QIODevice::WriteOnly)) {
					file.write(QString("Sub-Query Count: %1 \r\n").arg(mIdSubQueryIdDataHash.count()).toUtf8());
					file.close();
				}
			}

			for (QHash<cint64, CBGPSubQueryGenerationData*>::const_iterator it = mIdSubQueryIdDataHash.constBegin(), itEnd = mIdSubQueryIdDataHash.constEnd(); it != itEnd; ++it) {
				CBGPSubQueryGenerationData* subQueryGenData = it.value();

				CRedlandRasqalBGPSubQueryData* bgpData = subQueryGenData->bgpData;
				QSet<CExpressionVariable*> varExpSet;
				QList<CRDFStringTriple> stringTriplesList = subQueryGenData->convertingStringTriplesList;
				stringTriplesList.append(subQueryGenData->additionalIntegratingStringTriplesList);

				QList<CAxiomExpression*> axiomList = getTripleOWLAxioms(&stringTriplesList, true, &varExpSet);

				if (mConfWriteSubQueryTriples) {
					QFile file("./Debugging/Answering/RASQAL-Sub-Query-Triples.txt");
					if (file.open(QIODevice::Append)) {
						file.write(QString("\r\n\r\nSub-Query %1 \r\n").arg(subQueryGenData->bgpData->subQueryId).toUtf8());
						for (CRDFStringTriple stringTriples : stringTriplesList) {
							file.write(stringTriples.string[0].toUtf8());
							file.write("   ");
							file.write(stringTriples.string[1].toUtf8());
							file.write("   ");
							file.write(stringTriples.string[2].toUtf8());
							file.write(" .\r\n");
						}
						file.close();
					}
				}


				bool allIndiVars = true;
				bool allClassVars = true;
				bool allObjPropVars = true;
				bool allDataPropVars = true;

				bool hasIndiVars = false;
				bool hasClassVars = false;
				bool hasObjPropVars = false;
				bool hasDataPropVars = false;

				for (CExpressionVariable* var : varExpSet) {
					if (dynamic_cast<CClassVariableExpression*>(var)) {
						allIndiVars = false;
						allObjPropVars = false;
						allDataPropVars = false;
						hasClassVars = true;
					} else if (dynamic_cast<CIndividualVariableExpression*>(var)) {
						allClassVars = false;
						allObjPropVars = false;
						allDataPropVars = false;
						hasIndiVars = true;
					} else if (dynamic_cast<CObjectPropertyVariableExpression*>(var)) {
						allIndiVars = false;
						allClassVars = false;
						allDataPropVars = false;
						hasObjPropVars = true;
					} else if (dynamic_cast<CDataPropertyVariableExpression*>(var)) {
						allIndiVars = false;
						allClassVars = false;
						allObjPropVars = false;
						hasDataPropVars = true;
					}
				}


				QHash<QString, CExpressionVariable*> disVarNameExpHash;
				QList<CExpressionVariable*> ignoreCardVarExpList;

				for (CExpressionVariable* var : varExpSet) {
					if (subQueryGenData->distinguishedVariableSet.contains("?" + var->getName()) || subQueryGenData->distinguishedVariableSet.contains("$" + var->getName())) {
						disVarNameExpHash.insert("?" + var->getName(), var);
					} else if (subQueryGenData->additionalIntegratedVariableSet.contains("?" + var->getName()) || subQueryGenData->additionalIntegratedVariableSet.contains("$" + var->getName())) {
						ignoreCardVarExpList.append(var);
					}
				}

				QList<CExpressionVariable*> disVarList;
				for (QStringList::iterator varIt = subQueryGenData->distinguishedVariableStringList.begin(); varIt != subQueryGenData->distinguishedVariableStringList.end(); ++varIt) {
					QString varString(*varIt);
					if (disVarNameExpHash.contains(varString)) {
						disVarList.append(disVarNameExpHash.value(varString));
					}
				}

				QList<CExpressionVariable*> sortedDisVarList;
				sortedDisVarList.reserve(disVarList.size());
				QHash<QString, cint64> projVarPosHash;
				cint64 nextProjVarPos = 0;
				for (CExpressionVariable* disVar : disVarList) {
					projVarPosHash.insert("?" + disVar->getName(),-1);
					sortedDisVarList.append(nullptr);
				}
				for (const QString& projVarString : projectedVarList) {
					if (projVarPosHash.contains(projVarString)) {
						projVarPosHash.insert(projVarString, nextProjVarPos++);
					}
				}
				for (CExpressionVariable* disVar : disVarList) {
					if (projVarPosHash.contains("?" + disVar->getName())) {
						cint64 pos = projVarPosHash.value("?" + disVar->getName());
						if (pos >= 0) {
							sortedDisVarList[pos] = disVar;
						}
					}
				}
				for (CExpressionVariable* disVar : disVarList) {
					if (projVarPosHash.value("?" + disVar->getName(), -1) < 0) {
						sortedDisVarList[nextProjVarPos++] = disVar;
					}
				}


				cint64 limit = -1;
				cint64 offset = 0;
				if (mSingleBGPResultsDirectlyUsable && mConfPropagateBGPSubQueryDistinct) {
					cint64 queryOffset = rasqal_query_get_offset(mRasqalQuery);
					cint64 queryLimit = rasqal_query_get_limit(mRasqalQuery);
					limit = queryLimit;
					offset = queryOffset;
					bgpData->completelyHandledResultsTransfer = true;
					rasqal_query_set_offset(mRasqalQuery, 0);
					if (queryLimit != -1) {
						rasqal_query_set_limit(mRasqalQuery, 0);
					}
				}



				if (hasObjPropVars && allObjPropVars) {
					if (!sortedDisVarList.isEmpty()) {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-SELECT-Object-Properties-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternObjectPropertiesSelectQuery(axiomList, sortedDisVarList, limit, offset, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					} else {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-ASK-Object-Properties-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternObjectPropertiesAskQuery(axiomList, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					}
				} else if (hasDataPropVars && allDataPropVars) {
					if (!sortedDisVarList.isEmpty()) {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-SELECT-Data-Properties-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternDataPropertiesSelectQuery(axiomList, sortedDisVarList, limit, offset, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					} else {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-ASK-Data-Properties-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternDataPropertiesAskQuery(axiomList, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					}
				} else if (hasClassVars && allClassVars) {
					if (!sortedDisVarList.isEmpty()) {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-SELECT-Classes-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternClassSelectQuery(axiomList, sortedDisVarList, limit, offset, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					} else {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-ASK-Classes-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternClassAskQuery(axiomList, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					}
				} else if (hasIndiVars && allIndiVars || varExpSet.isEmpty()) {

					CEXPRESSIONLIST<COrderingTermExpression*> orderingList;
					QList<CFilteringTermExpression*> filteringList;

					bool distinctModifier = mConfForceDistinctSubQueries;
					if (!distinctModifier) {
						if (mDistinctSubBGPsSet.contains(bgpData->bgp)) {
							distinctModifier = true;
						}
					}


					if (!sortedDisVarList.isEmpty()) {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-SELECT-Individuals-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternIndividualSelectQuery(axiomList, sortedDisVarList, ignoreCardVarExpList, distinctModifier, limit, offset, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					} else {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-ASK-Individuals-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternIndividualAskQuery(axiomList, filteringList, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					}
				} else if (hasIndiVars && (hasObjPropVars || hasDataPropVars || hasClassVars) || varExpSet.isEmpty()) {

					CEXPRESSIONLIST<COrderingTermExpression*> orderingList;
					QList<CFilteringTermExpression*> filteringList;

					bool distinctModifier = mConfForceDistinctSubQueries;
					if (!distinctModifier) {
						if (mDistinctSubBGPsSet.contains(bgpData->bgp)) {
							distinctModifier = true;
						}
					}


					if (!sortedDisVarList.isEmpty()) {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-SELECT-Individuals-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternIndividualMixedSelectQuery(axiomList, sortedDisVarList, ignoreCardVarExpList, distinctModifier, limit, offset, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					} else {
						QString subQueryName = QString("Redland-Rasqal-BGP-Reasoning-ASK-Individuals-Sub-Query-%1").arg(subQueryGenData->bgpData->subQueryId);
						CQuerySPARQLBasicGraphPatternExpression* sparqlQuery = mQueryBuilder->getSPARQLBasicGraphPatternIndividualAskQuery(axiomList, filteringList, subQueryName);
						mBGPSubQueryNameHash.insert(subQueryName, bgpData);
					}
				}


			}
			return true;
		}









		bool rasqal_expression_is_aggregate(rasqal_expression* e) {
			if (e->op == RASQAL_EXPR_COUNT ||
				e->op == RASQAL_EXPR_SUM ||
				e->op == RASQAL_EXPR_AVG ||
				e->op == RASQAL_EXPR_MIN ||
				e->op == RASQAL_EXPR_MAX ||
				e->op == RASQAL_EXPR_SAMPLE ||
				e->op == RASQAL_EXPR_GROUP_CONCAT)
				return true;

			if (e->op != RASQAL_EXPR_FUNCTION)
				return false;

			return (e->flags & RASQAL_EXPR_FLAG_AGGREGATE) != 0;
		}

		int rasqal_algebra_extract_aggregate_expression_visit(void *user_data, rasqal_expression *e) {

			/* If an aggregate expression, return found */
			if (rasqal_expression_is_aggregate(e)) {
				return 1;
			}

			return 0;
		}




		bool hasProjectionAggregate(rasqal_projection* projection) {
			if (projection) {
				raptor_sequence* seq = projection->variables;
				int seqSize = raptor_sequence_size(seq);
				bool hasAggregate = false;
				for (int i = 0; i < seqSize; ++i) {
					rasqal_variable* v = (rasqal_variable*)raptor_sequence_get_at(seq, i);
					rasqal_expression* expr = v->expression;

					if (!expr)
						continue;

					if (rasqal_expression_visit(expr, rasqal_algebra_extract_aggregate_expression_visit, &hasAggregate)) {
						return true;
					}
				}
			}
			return false;
		}




		bool hasProjectionNonVariableExpression(rasqal_projection* projection) {
			if (projection) {
				raptor_sequence* seq = projection->variables;
				int seqSize = raptor_sequence_size(seq);
				bool hasAggregate = false;
				for (int i = 0; i < seqSize; ++i) {
					rasqal_variable* v = (rasqal_variable*)raptor_sequence_get_at(seq, i);
					rasqal_expression* expr = v->expression;

					if (expr) {
						return true;
					}

				}
			}
			return false;
		}



		bool CSPARQLRedlandRasqalReasoningBGPQueryParser::identifySubBGPsDistinctComputable(rasqal_graph_pattern* basicGrapPattern, QSet<rasqal_graph_pattern*>& distinctSubBGPsSet) {

			if (hasProjectionAggregate(((rasqal_graph_pattern_s*)basicGrapPattern)->projection)) {
				return false;
			}

			rasqal_graph_pattern_operator op = rasqal_graph_pattern_get_operator(basicGrapPattern);
			if (op != RASQAL_GRAPH_PATTERN_OPERATOR_GROUP && op != RASQAL_GRAPH_PATTERN_OPERATOR_BASIC && op != RASQAL_GRAPH_PATTERN_OPERATOR_OPTIONAL && op != RASQAL_GRAPH_PATTERN_OPERATOR_UNION) {
				return false;
			}

			rasqal_expression* filterExp = rasqal_graph_pattern_get_filter_expression(basicGrapPattern);
			if (filterExp) {
				return false;
			}

			distinctSubBGPsSet.insert(basicGrapPattern);
			raptor_sequence* subBGPSequence = rasqal_graph_pattern_get_sub_graph_pattern_sequence(basicGrapPattern);
			if (subBGPSequence) {
				int seqSize = raptor_sequence_size(subBGPSequence);
				for (int i = 0; i < seqSize; ++i) {
					rasqal_graph_pattern* subBGP = (rasqal_graph_pattern*)raptor_sequence_get_at(subBGPSequence, i);
					identifySubBGPsDistinctComputable(subBGP, distinctSubBGPsSet);
				}
			}
			return true;
		}





		bool CSPARQLRedlandRasqalReasoningBGPQueryParser::identifyHandlingKnowledgeBaseRelevantBGPs(rasqal_graph_pattern* grapPattern, bool rootPattern, QSet<rasqal_graph_pattern*>& kbRelevantBGPsSet, const QString& kbName, bool defaultKB) {

			rasqal_graph_pattern_operator op = rasqal_graph_pattern_get_operator(grapPattern);
			if (rootPattern && op != RASQAL_GRAPH_PATTERN_OPERATOR_GRAPH && defaultKB) {
				kbRelevantBGPsSet.insert(grapPattern);
			}
			if (op == RASQAL_GRAPH_PATTERN_OPERATOR_GRAPH) {
				rasqal_literal* graphLit = rasqal_graph_pattern_get_origin(grapPattern);
				const unsigned char* graphUrl = rasqal_literal_as_string(graphLit);
				QString graphUrlString = QString::fromUtf8((const char*)graphUrl);
				if (graphUrlString == kbName) {
					kbRelevantBGPsSet.insert(grapPattern);
				}

			}

			raptor_sequence* subBGPSequence = rasqal_graph_pattern_get_sub_graph_pattern_sequence(grapPattern);
			if (subBGPSequence) {
				int seqSize = raptor_sequence_size(subBGPSequence);
				for (int i = 0; i < seqSize; ++i) {
					rasqal_graph_pattern* subBGP = (rasqal_graph_pattern*)raptor_sequence_get_at(subBGPSequence, i);

					rasqal_graph_pattern_operator subOp = rasqal_graph_pattern_get_operator(subBGP);
					if (subOp != RASQAL_GRAPH_PATTERN_OPERATOR_GRAPH && kbRelevantBGPsSet.contains(grapPattern)) {
						kbRelevantBGPsSet.insert(subBGP);
					}
					identifyHandlingKnowledgeBaseRelevantBGPs(subBGP, false, kbRelevantBGPsSet, kbName, defaultKB);
				}
			}
			return true;
		}






		QHash<QString, CSPARQLRedlandRasqalReasoningBGPQueryParser::CRedlandRasqalBGPSubQueryData*> CSPARQLRedlandRasqalReasoningBGPQueryParser::parseBGPReasoningSubQueries(librdf_world* world, librdf_query* redlandQuery, rasqal_query* rasqalQuery, const QString& queryText, const QString& knowledgeBase, bool defaultKnowledgeBase, cint64& nextSubQueryId) {
			QList<CQuerySPARQLBasicGraphPatternExpression*> sparqlQueryList;

			mQueryText = queryText;
			mRedlandWorld = world;
			mRedlandQuery = redlandQuery;
			mRasqalQuery = rasqalQuery;
			mRasqalWorld = librdf_world_get_rasqal(world);
			mRaptorWorld = rasqal_world_get_raptor(mRasqalWorld);
			mNextSubQueryId = nextSubQueryId;

			rasqal_graph_pattern * basicGrapPattern = rasqal_query_get_query_graph_pattern(rasqalQuery);

			if (mConfWriteQueryStructure) {
				raptor_iostream * raptorIoStream = raptor_new_iostream_to_filename(mRaptorWorld, "./Debugging/Answering/raptor-query.txt");
				rasqal_query_write(raptorIoStream, rasqalQuery, NULL, NULL);
				raptor_iostream_write_end(raptorIoStream);
				raptor_free_iostream(raptorIoStream);


				FILE* fh = fopen("./Debugging/Answering/raptor-query-graph-pattern.txt", "w");
				rasqal_graph_pattern_print(basicGrapPattern, fh);
				fclose(fh);
			}


			bool queryDistinct = rasqal_query_get_distinct(rasqalQuery);
			if (queryDistinct && mConfPropagateBGPSubQueryDistinct && !hasProjectionAggregate(((rasqal_query_s*)rasqalQuery)->projection)) {
				identifySubBGPsDistinctComputable(basicGrapPattern, mDistinctSubBGPsSet);
			}

			QStringList selectProjVarList;
			raptor_sequence* projectVarSeq = rasqal_query_get_bound_variable_sequence(rasqalQuery);
			if (projectVarSeq) {
				int seqSize = raptor_sequence_size(projectVarSeq);
				for (int i = 0; i < seqSize; ++i) {
					rasqal_variable* v = (rasqal_variable*)raptor_sequence_get_at(projectVarSeq, i);

					QString	varString = getRasqalVariableString(rasqalQuery, v);
					if (!varString.isEmpty()) {
						selectProjVarList.append(varString);
					}
				}
			}


			mQueryContainsSelectAll = queryText.contains("*");

			mSingleBGPResultsDirectlyUsable = mConfAllowResultStreamingFromSingleSubQuery;
			mSingleBGPResultsDirectlyUsable = checkSingleBGPResultsDirectUsable(mSingleBGPResultsDirectlyUsable, basicGrapPattern, rasqalQuery);

			identifyHandlingKnowledgeBaseRelevantBGPs(basicGrapPattern, true, mKnowledgeBaseRelevantBGPsSet, knowledgeBase, defaultKnowledgeBase);

			identifyBGPBasedSubQueries(basicGrapPattern);

			generateBGPBasedSubQueries(selectProjVarList);

			if (mConfWriteQueryStructure) {
				raptor_iostream * raptorIoStream = raptor_new_iostream_to_filename(mRaptorWorld, "./Debugging/Answering/raptor-query.txt");
				rasqal_query_write(raptorIoStream, rasqalQuery, NULL, NULL);
				raptor_iostream_write_end(raptorIoStream);
				raptor_free_iostream(raptorIoStream);

				FILE* fh = fopen("./Debugging/Answering/raptor-query-graph-pattern.txt", "w");
				rasqal_graph_pattern_print(basicGrapPattern, fh);
				fclose(fh);
			}

			nextSubQueryId = mNextSubQueryId;

			return mBGPSubQueryNameHash;
		}





		bool CSPARQLRedlandRasqalReasoningBGPQueryParser::checkSingleBGPResultsDirectUsable(bool singleBGPResultsDirectlyUsable, rasqal_graph_pattern * basicGrapPattern, rasqal_query* rasqalQuery) {
			if (singleBGPResultsDirectlyUsable && rasqal_graph_pattern_get_operator(basicGrapPattern) != RASQAL_GRAPH_PATTERN_OPERATOR_BASIC) {
				singleBGPResultsDirectlyUsable = false;
			}
			if (singleBGPResultsDirectlyUsable && rasqal_graph_pattern_get_filter_expression(basicGrapPattern)) {
				singleBGPResultsDirectlyUsable = false;
			}
			if (singleBGPResultsDirectlyUsable && ((rasqal_graph_pattern_s*)basicGrapPattern)->projection) {
				singleBGPResultsDirectlyUsable = false;
			}
			if (singleBGPResultsDirectlyUsable && ((rasqal_graph_pattern_s*)basicGrapPattern)->modifier) {
				singleBGPResultsDirectlyUsable = false;
			}
			if (singleBGPResultsDirectlyUsable && hasProjectionNonVariableExpression(((rasqal_query_s*)rasqalQuery)->projection)) {
				singleBGPResultsDirectlyUsable = false;
			}
			if (singleBGPResultsDirectlyUsable && rasqal_query_get_order_conditions_sequence(rasqalQuery)) {
				singleBGPResultsDirectlyUsable = false;
			}
			if (singleBGPResultsDirectlyUsable) {
				rasqal_query_verb verb = rasqal_query_get_verb(rasqalQuery);
				if (verb != RASQAL_QUERY_VERB_SELECT && verb != RASQAL_QUERY_VERB_ASK) {
					singleBGPResultsDirectlyUsable = false;
				}
			}
			if (singleBGPResultsDirectlyUsable && rasqal_query_get_group_conditions_sequence(rasqalQuery)) {
				singleBGPResultsDirectlyUsable = false;
			}
			if (singleBGPResultsDirectlyUsable && rasqal_query_get_having_conditions_sequence(rasqalQuery)) {
				singleBGPResultsDirectlyUsable = false;
			}
			return singleBGPResultsDirectlyUsable;
		}

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_REDLAND_INTEGRATION
