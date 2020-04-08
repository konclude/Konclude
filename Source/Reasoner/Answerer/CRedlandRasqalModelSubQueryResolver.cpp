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

#include "CRedlandRasqalModelSubQueryResolver.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CRedlandRasqalModelSubQueryResolver::CRedlandRasqalModelSubQueryResolver(QVector<CRedlandRasqalSubQueryProcessingData*>* subQueryProcDataVec, librdf_world* world, librdf_query* redlandQuery, rasqal_query* rasqalQuery, QHash<QString, TDependentOntologyPair>* queryKbNameDepOntPair) {
				mSubQueryProcDataVec = subQueryProcDataVec;
				mWorld = world;
				mRedlandQuery = redlandQuery;
				mRasqalQuery = rasqalQuery;
				mResolvingModel = nullptr;
				mQueryKbNameDepOntPairHash = queryKbNameDepOntPair;


				raptor_sequence* tripleSeq = rasqal_query_get_triple_sequence(mRasqalQuery);
				int tripleSeqSize = raptor_sequence_size(tripleSeq);
				mTriplesContextLiteralVector.resize(tripleSeqSize);
				for (int i = 0; i < tripleSeqSize; ++i) {
					rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(tripleSeq, i);
					rasqal_literal* lit = rasqal_triple_get_origin(triple);
					mTriplesContextLiteralVector[i] = lit;
				}
				mTriplesContextLiteralsReset = false;

			}


			CRedlandRasqalModelSubQueryResolver::~CRedlandRasqalModelSubQueryResolver() {
				releaseRedlandResolvingModel();
			}



			/* A Model Factory */
			struct librdf_model_factory_s {
				char* name;
				char* label;

				/* the rest of this structure is populated by the
				model-specific register function */
				size_t context_length;

				/* init the factory */
				void(*init)(void);

				/* terminate the factory */
				void(*terminate)(void);

				/* create a new model */
				int(*create)(librdf_model* model, librdf_storage* storage, librdf_hash* options);

				/* copy a model */
				/* clone is assumed to do leave the new model in the same state
				* after an init() method on an existing model - i.e ready to
				* use but closed.
				*/
				librdf_model* (*clone)(librdf_model* new_model);

				/* destroy model */
				void(*destroy)(librdf_model* model);

				/* return the number of statements in the model for model */
				int(*size)(librdf_model* model);

				/* add a statement to the model from the given model */
				int(*add_statement)(librdf_model* model, librdf_statement* statement);

				/* add a statement to the model from the given model */
				int(*add_statements)(librdf_model* model, librdf_stream* statement_stream);

				/* remove a statement from the model  */
				int(*remove_statement)(librdf_model* model, librdf_statement* statement);

				/* check if statement in model  */
				int(*contains_statement)(librdf_model* model, librdf_statement* statement);
				/* check for [node, property, ?] */
				int(*has_arc_in)(librdf_model *model, librdf_node *node, librdf_node *property);
				/* check for [?, property, node] */
				int(*has_arc_out)(librdf_model *model, librdf_node *node, librdf_node *property);


				/* serialise the model in model  */
				librdf_stream* (*serialise)(librdf_model* model);

				/* serialise the results of a query */
				librdf_stream* (*find_statements)(librdf_model* model, librdf_statement* statement);
				librdf_stream* (*find_statements_with_options)(librdf_model* model, librdf_statement* statement, librdf_node* context_node, librdf_hash* options);

				/* return a list of Nodes marching given arc, target */
				librdf_iterator* (*get_sources)(librdf_model* model, librdf_node *arc, librdf_node *target);

				/* return a list of Nodes marching given source, target */
				librdf_iterator* (*get_arcs)(librdf_model* model, librdf_node *source, librdf_node *target);

				/* return a list of Nodes marching given source, target */
				librdf_iterator* (*get_targets)(librdf_model* model, librdf_node *source, librdf_node *target);

				/* return list of properties to/from a node */
				librdf_iterator* (*get_arcs_in)(librdf_model *model, librdf_node *node);
				librdf_iterator* (*get_arcs_out)(librdf_model *model, librdf_node *node);

				/* add a statement to the model from the context */
				int(*context_add_statement)(librdf_model* model, librdf_node* context, librdf_statement *statement);

				/* remove a statement from the context  */
				int(*context_remove_statement)(librdf_model* model, librdf_node* context, librdf_statement *statement);

				/* list statements in a context  */
				librdf_stream* (*context_serialize)(librdf_model* model, librdf_node* context);

				/* query the model */
				librdf_query_results* (*query_execute)(librdf_model* model, librdf_query* query);

				/* sync the model to the storage - OPTIONAL */
				int(*sync)(librdf_model* model);

				/* add a statement from the context - OPTIONAL (librdf_model will
				* implement using context_add_statement if missing)
				*/
				int(*context_add_statements)(librdf_model* model, librdf_node* context, librdf_stream *stream);

				/* remove a statement from the context - OPTIONAL (librdf_model will
				* implement using context_remove_statement if missing)
				*/
				int(*context_remove_statements)(librdf_model* model, librdf_node* context);

				/* get the single storage for this model if there is one - OPTIONAL */
				librdf_storage* (*get_storage)(librdf_model* model);

				/* search for statement in a context - OPTIONAL (rdf_model will do
				* it using find_statements if missing)
				*/
				librdf_stream* (*find_statements_in_context)(librdf_model* model, librdf_statement* statement, librdf_node* context_node);

				/* return an iterator of context nodes in the store - OPTIONAL
				* (returning NULL)
				*/
				librdf_iterator* (*get_contexts)(librdf_model* model);

				/* features - OPTIONAL */
				librdf_node* (*get_feature)(librdf_model* model, librdf_uri* feature);
				int(*set_feature)(librdf_model* model, librdf_uri* feature, librdf_node* value);

				/* transactions - OPTIONAL */
				int(*transaction_start)(librdf_model* model);
				int(*transaction_start_with_handle)(librdf_model* model, void* handle);
				int(*transaction_commit)(librdf_model* model);
				int(*transaction_rollback)(librdf_model* model);
				void* (*transaction_get_handle)(librdf_model* model);


				/* check if statement in model  */
				int(*contains_statement_with_context)(librdf_model* model, librdf_statement* statement, librdf_node* context_node);


			};


			struct librdf_model_s {
				librdf_world *world;

				/* usage: usage count of this instance
				*
				* Used by other redland classes such as iterator, stream
				* via librdf_model_add_reference librdf_model_remove_reference
				* The usage count of model after construction is 1.
				*/
				int usage;

				/* sub_models: list of sub librdf_model* */
				librdf_list*     sub_models;

				/* supports_contexts : does the storage model support redland contexts? */
				int supports_contexts;

				/* context : model implementation user data */
				void *context;

				struct librdf_model_factory_s* factory;
			};




			/**
			* librdf_storage_factory:
			* @version: Interface version.  Only version 1 is defined.
			* @name: Name (ID) of this storage, e.g. "megastore"
			* @label: Label of this storage, e.g. "Megastore Storage"
			* @init: Create a new storage.
			*   This method should create the required instance data and store it with
			*   librdf_storage_set_instance() so it can be used in other methods.
			* @clone: Copy a storage.
			*   This is assumed to leave the new storage in the same state as an existing
			*   storage after an init() method - i.e ready to use but closed.
			* @terminate: Destroy a storage.
			*   This method is responsible for freeing all memory allocated in the init method.
			* @open: Make storage be associated with model
			* @close: Close storage/model context
			* @size: Return the number of statements in the storage for model
			* @add_statement: Add a statement to the storage from the given model. OPTIONAL
			* @add_statements: Add a statement to the storage from the given model. OPTIONAL
			* @remove_statement: Remove a statement from the storage. OPTIONAL
			* @contains_statement: Check if statement is in storage
			* @has_arc_in: Check for [node, property, ?]
			* @has_arc_out: Check for [?, property, node]
			* @serialise: Serialise the model in storage
			* @find_statements: Return a stream of triples matching a triple pattern
			* @find_statements_with_options: Return a stream of triples matching a triple pattern with some options.  OPTIONAL
			* @find_sources: Return a list of Nodes marching given arc, target
			* @find_arcs: Return a list of Nodes marching given source, target
			* @find_targets: Return a list of Nodes marching given source, target
			* @get_arcs_in:  Return list of properties to a node (i.e. with node as the object)
			* @get_arcs_out: Return list of properties from a node (i.e. with node as the subject)
			* @context_add_statement: Add a statement to the storage from the context.
			*    NOTE: If context is NULL, this MUST be equivalent to @add_statement. OPTIONAL.
			* @context_remove_statement: Remove a statement from a context.
			*    NOTE: if context is NULL, this MUST be equivalent to remove_statement. OPTIONAL.
			* @context_serialise: Serialise statements in a context. OPTIONAL
			* @sync: Synchronise to underlying storage. OPTIONAL
			* @context_add_statements: Add statements to a context. storage core will do this using context_add_statement if missing.
			*    NOTE: If context is NULL, this MUST be equivalent to add_statements. OPTIONAL
			* @context_remove_statements: Remove statements from a context. storage core will do this using context_remove_statement if missing). OPTIONAL
			* @find_statements_in_context: Search for statement in a context. storage core will do this using find_statements if missing. OPTIONAL
			* @get_contexts: Return an iterator of context nodes. Returns NULL if contexts not supported. OPTIONAL
			* @get_feature: Get a feature. OPTIONAL
			* @set_feature: Set a feature. OPTIONAL
			* @transaction_start: Begin a transaction. OPTIONAL
			* @transaction_start_with_handle: Begin a transaction with opaque data handle. OPTIONAL
			* @transaction_commit: Commit a transaction. OPTIONAL
			* @transaction_rollback: Rollback a transaction. OPTIONAL
			* @transaction_get_handle: Get opaque data handle passed to transaction_start_with_handle. OPTIONAL
			*
			* A Storage Factory
			*/
			struct librdf_storage_factory_s {
				/* Interface version */
				int version;

				/* Name (ID) of this storage */
				char* name;

				/* Label of this storage */
				char* label;

				/* The rest of this structure is populated by the storage-specific
				* register function
				*/

				/* Create a new storage. */
				int(*init)(librdf_storage* storage, const char *name, librdf_hash* options);

				/* Copy a storage. */
				int(*clone)(librdf_storage* new_storage, librdf_storage* old_storage);

				/* Destroy a storage. */
				void(*terminate)(librdf_storage* storage);

				/* Make storage be associated with model */
				int(*open)(librdf_storage* storage, librdf_model* model);

				/* Close storage/model context */
				int(*close)(librdf_storage* storage);

				/* Return the number of statements in the storage for model */
				int(*size)(librdf_storage* storage);

				/* Add a statement to the storage from the given model */
				int(*add_statement)(librdf_storage* storage, librdf_statement* statement);

				/* Add a statement to the storage from the given model */
				int(*add_statements)(librdf_storage* storage, librdf_stream* statement_stream);

				/* Remove a statement from the storage */
				int(*remove_statement)(librdf_storage* storage, librdf_statement* statement);

				/* Check if statement is in storage */
				int(*contains_statement)(librdf_storage* storage, librdf_statement* statement);

				/* Check for [node, property, ?] */
				int(*has_arc_in)(librdf_storage *storage, librdf_node *node, librdf_node *property);

				/* Check for [?, property, node] */
				int(*has_arc_out)(librdf_storage *storage, librdf_node *node, librdf_node *property);

				/* Serialise the model in storage */
				librdf_stream* (*serialise)(librdf_storage* storage);

				/* Return a stream of triples matching a triple pattern */
				librdf_stream* (*find_statements)(librdf_storage* storage, librdf_statement* statement);

				/* Return a stream of triples matching a triple pattern with some options. */
				librdf_stream* (*find_statements_with_options)(librdf_storage* storage, librdf_statement* statement, librdf_node* context_node, librdf_hash* options);

				/* Return a list of Nodes marching given arc, target */
				librdf_iterator* (*find_sources)(librdf_storage* storage, librdf_node *arc, librdf_node *target);

				/* Return a list of Nodes marching given source, target */
				librdf_iterator* (*find_arcs)(librdf_storage* storage, librdf_node *src, librdf_node *target);

				/* Return a list of Nodes marching given source, target */
				librdf_iterator* (*find_targets)(librdf_storage* storage, librdf_node *src, librdf_node *target);

				/** Return list of properties to a node (i.e. with node as the object) */
				librdf_iterator* (*get_arcs_in)(librdf_storage *storage, librdf_node *node);

				/* Return list of properties from a node (i.e. with node as the subject) */
				librdf_iterator* (*get_arcs_out)(librdf_storage *storage, librdf_node *node);

				/* Add a statement to the storage from the context */
				int(*context_add_statement)(librdf_storage* storage, librdf_node* context, librdf_statement *statement);

				/* Remove a statement from a context */
				int(*context_remove_statement)(librdf_storage* storage, librdf_node* context, librdf_statement *statement);

				/* Serialise statements in a context */
				librdf_stream* (*context_serialise)(librdf_storage* storage, librdf_node* context);

				/* Synchronise to underlying storage */
				int(*sync)(librdf_storage* storage);

				/* Add statements to a context */
				int(*context_add_statements)(librdf_storage* storage, librdf_node* context, librdf_stream *stream);

				/* Remove statements from a context */
				int(*context_remove_statements)(librdf_storage* storage, librdf_node* context);

				/* Search for statement in a context */
				librdf_stream* (*find_statements_in_context)(librdf_storage* storage,
					librdf_statement* statement, librdf_node* context_node);

				/* Return an iterator of context nodes */
				librdf_iterator* (*get_contexts)(librdf_storage* storage);

				/* Get a feature */
				librdf_node* (*get_feature)(librdf_storage* storaage, librdf_uri* feature);

				/* Set a feature */
				int(*set_feature)(librdf_storage* storage, librdf_uri* feature, librdf_node* value);

				/* Begin a transaction */
				int(*transaction_start)(librdf_storage* storage);

				/* Begin a transaction with opaque data handle */
				int(*transaction_start_with_handle)(librdf_storage* storage, void* handle);

				/* Commit a transaction */
				int(*transaction_commit)(librdf_storage* storage);

				/* Rollback a transaction */
				int(*transaction_rollback)(librdf_storage* storage);

				/* Get opaque data handle passed to transaction_start_with_handle */
				void* (*transaction_get_handle)(librdf_storage* storage);

				/** Storage engine supports querying - OPTIONAL */
				int(*supports_query)(librdf_storage* storage, librdf_query *query);

				/** Storage engine returns query results - OPTIONAL */
				librdf_query_results* (*query_execute)(librdf_storage* storage, librdf_query *query);
			};


			/** A storage object */
			struct librdf_storage_s {
				librdf_world *world;

				/* usage count of this instance
				* Used by other redland classes such as model, iterator, stream
				* via  librdf_storage_add_reference librdf_storage_remove_reference
				* The usage count of storage after construction is 1.
				*/
				int usage;

				librdf_model *model;
				void *instance;
				int index_contexts;
				struct librdf_storage_factory_s* factory;
			};

			typedef struct {
				librdf_storage *storage;
			} librdf_model_storage_context;


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
				//rasqal_projection* projection;
				void* projection;

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


			static librdf_node* rasqal_literal_to_redland_node(librdf_world *world, rasqal_literal* l) {
				rasqal_literal_type type;

				if (!l)
					return NULL;

				/* FIXME: Workaround for Issue #0000519
				* http://bugs.librdf.org/mantis/view.php?id=519
				*
				* Remove this 'if' when RASQAL_MIN_VERSION is 0.9.30 or larger
				*/
				if (l->type == RASQAL_LITERAL_INTEGER_SUBTYPE)
					type = RASQAL_LITERAL_STRING;
				else
					type = rasqal_literal_get_rdf_term_type(l);

				if (type == RASQAL_LITERAL_URI)
					return librdf_new_node_from_uri(world, (librdf_uri*)l->value.uri);
				else if (type == RASQAL_LITERAL_STRING)
					return librdf_new_node_from_typed_literal(world,
					(unsigned char*)l->string,
						l->language,
						(librdf_uri*)l->datatype);
				else if (type == RASQAL_LITERAL_BLANK)
					return librdf_new_node_from_blank_identifier(world,
					(unsigned char*)l->string);

				return NULL;
			}























































#define LIBRDF_MALLOC(type, size) (type)malloc(size)
#define LIBRDF_CALLOC(type, size, count) (type)calloc(size, count)
#define LIBRDF_FREE(type, ptr)   free(ptr)



			rasqal_literal*
				redland_node_to_rasqal_literal(librdf_world* world, librdf_node *node) {
				rasqal_literal* l;
				rasqal_world* rasqal_world_ptr = librdf_world_get_rasqal(world);

				if (librdf_node_is_resource(node)) {
					raptor_uri* uri = (raptor_uri*)librdf_new_uri_from_uri(librdf_node_get_uri(node));
					l = rasqal_new_uri_literal(rasqal_world_ptr, uri); /* transfer uri ownership to literal */
				} else if (librdf_node_is_literal(node)) {
					unsigned char *string;
					librdf_uri *uri;
					unsigned char *new_string;
					char *new_language = NULL;
					raptor_uri *new_datatype = NULL;
					size_t len;

					string = librdf_node_get_literal_value_as_counted_string(node, &len);
					new_string = (unsigned char*)rasqal_alloc_memory(len + 1);
					if (!new_string)
						return NULL;
					strcpy((char*)new_string, (const char*)string);

					string = (unsigned char*)librdf_node_get_literal_value_language(node);
					if (string) {
						new_language = (char*)rasqal_alloc_memory(strlen((const char*)string) + 1);
						if (!new_language) {
							rasqal_free_memory((void*)new_string);
							return NULL;
						}
						strcpy((char*)new_language, (const char*)string);
					}
					uri = librdf_node_get_literal_value_datatype_uri(node);
					if (uri)
						new_datatype = (raptor_uri*)librdf_new_uri_from_uri(uri);
					/* transfer new_string,new_language,new_datatype ownership to literal */
					l = rasqal_new_string_literal(rasqal_world_ptr, (const unsigned char*)new_string, new_language, new_datatype, NULL);
				} else {
					unsigned char *blank = librdf_node_get_blank_identifier(node);
					unsigned char *new_blank;
					if (!blank)
						return NULL;
					new_blank = (unsigned char*)rasqal_alloc_memory(strlen((const char*)blank) + 1);
					if (!new_blank)
						return NULL;
					strcpy((char*)new_blank, (const char*)blank);
					/* transfer new_blank ownership to literal */
					l = rasqal_new_simple_literal(rasqal_world_ptr, RASQAL_LITERAL_BLANK, (const unsigned char*)new_blank);
				}

				return l;
			}


			/** A query object */
			struct librdf_query_s {
				librdf_world *world;
				int usage;
				void *context;
				void* factory;

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


			/* prototypes for local functions */
			static int rasqal_redland_init_triples_match(rasqal_triples_match* rtm, rasqal_triples_source *rts, void *user_data, rasqal_triple_meta *m, rasqal_triple *t);
			static int rasqal_redland_triple_present(rasqal_triples_source *rts, void *user_data, rasqal_triple *t);
			static void rasqal_redland_free_triples_source(void *user_data);






			typedef struct {
				librdf_world *world;
				librdf_query *query;
				librdf_model *model;
			} rasqal_redland_triples_source_user_data;



			static int
				rasqal_redland_new_triples_source(rasqal_query* rdf_query,
					void *factory_user_data,
					void *user_data,
					rasqal_triples_source *rts) {
				librdf_world *world = (librdf_world*)factory_user_data;
				rasqal_redland_triples_source_user_data* rtsc = (rasqal_redland_triples_source_user_data*)user_data;
				rasqal_world* rasqal_world_ptr = librdf_world_get_rasqal(world);
				raptor_world* raptor_world_ptr = rasqal_world_get_raptor(rasqal_world_ptr);

				raptor_sequence *seq;
				librdf_query_rasqal_context *context;
				librdf_iterator* cit;

				rtsc->world = world;
				rtsc->query = (librdf_query*)rasqal_query_get_user_data(rdf_query);
				context = (librdf_query_rasqal_context*)((librdf_query_s*)rtsc->query)->context;
				rtsc->model = context->model;

				seq = rasqal_query_get_data_graph_sequence(rdf_query);

				/* FIXME: queries with data graphs in them (such as FROM in SPARQL)
				* are deleted, so that there are no unexpected data loads
				*/
				if (seq) {
					while (raptor_sequence_size(seq)) {
						rasqal_data_graph* dg = (rasqal_data_graph*)raptor_sequence_pop(seq);
						rasqal_free_data_graph(dg);
					}
				}

				if (librdf_model_supports_contexts(rtsc->model)) {
					/* Add all contexts (named graphs) to the query so Rasqal can bind them */
					cit = librdf_model_get_contexts(rtsc->model);
					while (!librdf_iterator_end(cit)) {
						librdf_node* node = (librdf_node*)librdf_iterator_get_object(cit);
						librdf_uri* uri;
						raptor_uri* source_uri;
						rasqal_data_graph* dg;

						uri = librdf_node_get_uri(node);
						source_uri = (raptor_uri*)raptor_new_uri(raptor_world_ptr,
							librdf_uri_as_string(uri));

						dg = rasqal_new_data_graph_from_uri(rasqal_world_ptr,
							source_uri, source_uri,
							RASQAL_DATA_GRAPH_NAMED,
							NULL, NULL, NULL);
						rasqal_query_add_data_graph(rdf_query, dg);

						raptor_free_uri(source_uri);
						librdf_iterator_next(cit);
					}
					librdf_free_iterator(cit);
				}

#ifdef RASQAL_TRIPLES_SOURCE_MIN_VERSION
				rts->version = 1;
#endif

				rts->init_triples_match = rasqal_redland_init_triples_match;
				rts->triple_present = rasqal_redland_triple_present;
				rts->free_triples_source = rasqal_redland_free_triples_source;

				return 0;
			}



			/**
			* librdf_model_contains_statement:
			* @model: the model object
			* @statement: the statement
			*
			* Check for a statement in the model.
			*
			* It must be a complete statement - all of subject, predicate,
			* object parts must be present and a legal RDF triple.  Use
			* librdf_model_find_statements to search for partial statement
			* matches.
			*
			* WARNING: librdf_model_contains_statement may not work correctly
			* with stores using contexts.  In this case, a search using
			* librdf_model_find_statements for a non-empty list will
			* return the correct result.
			*
			* Return value: non 0 if the model contains the statement (>0 if the statement is illegal)
			**/
			int librdf_model_contains_statement_with_context(librdf_model* model, librdf_statement* statement, librdf_node* context) {
				//if (!librdf_statement_is_complete(statement))
				//	return 1;

				return ((librdf_model_factory_s*)((librdf_model_s*)model)->factory)->contains_statement_with_context(model, statement, context) ? -1 : 0;
			}



			static int
				rasqal_redland_triple_present(rasqal_triples_source *rts, void *user_data,
					rasqal_triple *t) {
				rasqal_redland_triples_source_user_data* rtsc = (rasqal_redland_triples_source_user_data*)user_data;
				librdf_node* nodes[4];
				librdf_statement *s;
				int rc;

				/* ASSUMPTION: all the parts of the triple are not variables */
				/* FIXME: and no error checks */
				//nodes[0] = rasqal_literal_to_redland_node(rtsc->world, t->subject);
				//nodes[1] = rasqal_literal_to_redland_node(rtsc->world, t->predicate);
				//nodes[2] = rasqal_literal_to_redland_node(rtsc->world, t->object);
				nodes[0] = NULL;
				nodes[1] = NULL;
				nodes[2] = NULL;
				if (t->origin != NULL) {
					//nodes[3] = rasqal_literal_to_redland_node(rtsc->world, t->origin);
					//nodes[3] = (librdf_node*)t->origin;
					nodes[3] = (librdf_node*)t->origin;
					//nodes[3] = (librdf_node*)t->origin;
				} else {
					nodes[3] = NULL;
				}

				//s = librdf_new_statement_from_nodes(rtsc->world, nodes[0], nodes[1], nodes[2]);
				s = NULL;

				//rc = librdf_model_contains_statement(rtsc->model, s);
				rc = librdf_model_contains_statement_with_context(rtsc->model, s, nodes[3]);
				//librdf_free_statement(s);
				return rc;
			}



			static void
				rasqal_redland_free_triples_source(void *user_data) {
				/* rasqal_redland_triples_source_user_data* rtsc=(rasqal_redland_triples_source_user_data*)user_data; */
			}


#ifdef RASQAL_TRIPLES_SOURCE_FACTORY_MIN_VERSION
#define RTS_RET int
#else
#define RTS_RET void
#endif

			static RTS_RET
				rasqal_redland_register_triples_source_factory(rasqal_triples_source_factory *factory) {
#ifdef RASQAL_TRIPLES_SOURCE_FACTORY_MIN_VERSION
				factory->version = 1;
#endif
				factory->user_data_size = sizeof(rasqal_redland_triples_source_user_data);
				factory->new_triples_source = rasqal_redland_new_triples_source;

#ifdef RASQAL_TRIPLES_SOURCE_FACTORY_MIN_VERSION
				return 0;
#endif
			}



			struct librdf_stream_s {
				librdf_world *world;
				void *context;
				int is_finished; /* 1 when have no more statements */
				int is_updated; /* 1 when we know there is a current item */
				int is_updating; /* 1 when are in the middle of update process */

								 /* Used when mapping */
				librdf_statement *current;
				librdf_list *map_list; /* non-empty means there is a list of maps */

				int(*is_end_method)(void*);
				int(*next_method)(void*);
				void* (*get_method)(void*, int); /* flags: type of get */
				void(*finished_method)(void*);
			};

			typedef struct {
				librdf_node* nodes[3];
				librdf_node* origin;
				/* query statement, made from the nodes above (even when exact) */
				librdf_statement *qstatement;
				librdf_stream *stream;
			} rasqal_redland_triples_match_context;


			static rasqal_triple_parts
				rasqal_redland_bind_match(struct rasqal_triples_match_s* rtm,
					void *user_data,
					rasqal_variable* bindings[4],
					rasqal_triple_parts parts) {
				rasqal_redland_triples_match_context* rtmc = (rasqal_redland_triples_match_context*)rtm->user_data;
				rasqal_literal* l;
				librdf_statement* statement;
				rasqal_triple_parts result = (rasqal_triple_parts)0;
				librdf_world *world = ((librdf_stream_s*)rtmc->stream)->world;

				statement = librdf_stream_get_object(rtmc->stream);
				if (!statement)
					return (rasqal_triple_parts)0;

#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
				LIBRDF_DEBUG1("  matched statement ");
				librdf_statement_print(statement, stderr);
				fputc('\n', stderr);
#endif

				/* set 1 or 2 variable values from the fields of statement */

				if (bindings[0] && (parts & RASQAL_TRIPLE_SUBJECT)) {
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
					LIBRDF_DEBUG1("binding subject to variable\n");
#endif

#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
					l = redland_node_to_rasqal_literal(world,
						librdf_statement_get_subject(statement));
#else
					l = (rasqal_literal*)librdf_statement_get_subject(statement);
					statement->subject = NULL;
#endif // KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING


					rasqal_variable_set_value(bindings[0], l);
					result = RASQAL_TRIPLE_SUBJECT;
				}

				if (bindings[1] && (parts & RASQAL_TRIPLE_PREDICATE)) {
					if (
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
						false &&
#endif // KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
						bindings[0] == bindings[1]) {
						/* check matching(?x, ?x, ...) / subject=predicate */
						if (!librdf_node_equals(librdf_statement_get_subject(statement),
							librdf_statement_get_predicate(statement)))
							return (rasqal_triple_parts)0;
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
						LIBRDF_DEBUG1("subject and predicate values match\n");
#endif
					} else {
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
						LIBRDF_DEBUG1("binding predicate to variable\n");
#endif

#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
						l = redland_node_to_rasqal_literal(world,
							librdf_statement_get_predicate(statement));
#else
						l = (rasqal_literal*)librdf_statement_get_predicate(statement);
						statement->predicate = NULL;
#endif // KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING


						rasqal_variable_set_value(bindings[1], l);
						result = (rasqal_triple_parts)(result | RASQAL_TRIPLE_PREDICATE);
					}
				}

				if (bindings[2] && (parts & RASQAL_TRIPLE_OBJECT)) {
					int bind = 1;

					if (
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
						false &&
#endif // KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
						bindings[0] == bindings[2]) {
						/* check matching (?x, ..., ?x) / subject=object */
						if (!librdf_node_equals(librdf_statement_get_subject(statement),
							librdf_statement_get_object(statement)))
							return (rasqal_triple_parts)0;
						bind = 0;
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
						LIBRDF_DEBUG1("subject and object values match\n");
#endif
					}
					if (
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
						false &&
#endif // KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
						(bindings[1] == bindings[2]) &&
						!(bindings[0] == bindings[1])) {
						/* check matching (..., ?x, ?x) / predicate=object
						* Don't do this if matching (?x, ?x, ...) / subject=predicate
						* was already done since that would mean the match was (?x, ?x, ?x)
						*/
						if (!librdf_node_equals(librdf_statement_get_predicate(statement),
							librdf_statement_get_object(statement)))
							return (rasqal_triple_parts)0;
						bind = 0;
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
						LIBRDF_DEBUG1("predicate and object values match\n");
#endif
					}

					if (bind) {
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
						LIBRDF_DEBUG1("binding object to variable\n");
#endif

#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
						l = redland_node_to_rasqal_literal(world,
							librdf_statement_get_object(statement));
#else
						l = (rasqal_literal*)librdf_statement_get_object(statement);
						statement->object = NULL;
#endif // KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING

						rasqal_variable_set_value(bindings[2], l);
						result = (rasqal_triple_parts)(result | RASQAL_TRIPLE_OBJECT);
					}
				}

				/* Contexts */
				if (
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
					false &&
#endif // KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
					bindings[3] && (parts & RASQAL_TRIPLE_ORIGIN)) {
					int bind = 1;
					librdf_node* context_node = librdf_stream_get_context2(rtmc->stream);

					if (bindings[0] == bindings[3]) {
						/* check matching (?x, ..., ...) in context ?x */
						if (!librdf_node_equals(librdf_statement_get_subject(statement),
							context_node))
							return (rasqal_triple_parts)0;
						bind = 0;
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
						LIBRDF_DEBUG1("subject and context values match\n");
#endif
					}

					if (bindings[1] == bindings[3]) {
						/* check matching (..., ?x,  ...) in context ?x */
						if (!librdf_node_equals(librdf_statement_get_predicate(statement),
							context_node))
							return (rasqal_triple_parts)0;
						bind = 0;
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
						LIBRDF_DEBUG1("predicate and context values match\n");
#endif
					}

					if (bindings[2] == bindings[3]) {
						/* check matching (..., ..., ?x) in context ?x */
						if (!librdf_node_equals(librdf_statement_get_object(statement),
							context_node))
							return (rasqal_triple_parts)0;
						bind = 0;
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
						LIBRDF_DEBUG1("object and context values match\n");
#endif
					}

					if (bind) {
#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
						LIBRDF_DEBUG1("binding origin to variable\n");
#endif
						if (context_node)
							l = redland_node_to_rasqal_literal(world, context_node);
						else
							l = NULL;
						rasqal_variable_set_value(bindings[3], l);
						result = (rasqal_triple_parts)(result | RASQAL_TRIPLE_ORIGIN);
					}
				}


#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
				if (statement->subject) {
					rasqal_free_literal((rasqal_literal*)statement->subject);
					statement->subject = NULL;
				}
				if (statement->predicate) {
					rasqal_free_literal((rasqal_literal*)statement->predicate);
					statement->predicate = NULL;
				}
				if (statement->object) {
					rasqal_free_literal((rasqal_literal*)statement->object);
					statement->object = NULL;
				}
#endif // KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING

				return parts;
			}


			static void
				rasqal_redland_next_match(struct rasqal_triples_match_s* rtm,
					void *user_data) {
				rasqal_redland_triples_match_context* rtmc = (rasqal_redland_triples_match_context*)rtm->user_data;

				librdf_stream_next(rtmc->stream);
			}

			static int
				rasqal_redland_is_end(struct rasqal_triples_match_s* rtm,
					void *user_data) {
				rasqal_redland_triples_match_context* rtmc = (rasqal_redland_triples_match_context*)rtm->user_data;

				return librdf_stream_end(rtmc->stream);
			}


			static void
				rasqal_redland_finish_triples_match(struct rasqal_triples_match_s* rtm,
					void *user_data) {
				rasqal_redland_triples_match_context* rtmc = (rasqal_redland_triples_match_context*)rtm->user_data;

				if (rtmc) {
					if (rtmc->stream) {
						librdf_free_stream(rtmc->stream);
						rtmc->stream = NULL;
					}
					if (rtmc->qstatement) {
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
						if (rtmc->qstatement->subject) {
							rasqal_free_literal((rasqal_literal*)rtmc->qstatement->subject);
							rtmc->qstatement->subject = NULL;
						}
						if (rtmc->qstatement->predicate) {
							rasqal_free_literal((rasqal_literal*)rtmc->qstatement->predicate);
							rtmc->qstatement->predicate = NULL;
						}
						if (rtmc->qstatement->object) {
							rasqal_free_literal((rasqal_literal*)rtmc->qstatement->object);
							rtmc->qstatement->object = NULL;
						}
#endif // KONCLUDE_REDLAND_RASQAL_SUBQUERY_DIRECT_RASQAL_LITERAL_RESOLVING
						librdf_free_statement(rtmc->qstatement);
						rtmc->qstatement = NULL;
					}
					LIBRDF_FREE(rasqal_redland_triples_match_context, rtmc);
				}
			}


			static int
				rasqal_redland_init_triples_match(rasqal_triples_match* rtm,
					rasqal_triples_source *rts, void *user_data,
					rasqal_triple_meta *m, rasqal_triple *t) {
				rasqal_redland_triples_source_user_data* rtsc = (rasqal_redland_triples_source_user_data*)user_data;
				rasqal_redland_triples_match_context* rtmc;
				rasqal_variable* var;

				rtm->bind_match = rasqal_redland_bind_match;
				rtm->next_match = rasqal_redland_next_match;
				rtm->is_end = rasqal_redland_is_end;
				rtm->finish = rasqal_redland_finish_triples_match;

				rtmc = LIBRDF_CALLOC(rasqal_redland_triples_match_context*, 1, sizeof(*rtmc));
				if (!rtmc)
					return 1;

				rtm->user_data = rtmc;


				/* at least one of the triple terms is a variable and we need to
				* do a triplesMatching() aka librdf_model_find_statements
				*
				* redland find_statements will do the right thing and internally
				* pick the most efficient, indexed way to get the answer.
				*/

				if ((var = rasqal_literal_as_variable(t->subject))) {
					if (var->value) {
						//rtmc->nodes[0] = rasqal_literal_to_redland_node(rtsc->world, var->value);
						rtmc->nodes[0] = NULL;
						var = NULL;
					} else {
						rtmc->nodes[0] = NULL;
					}
				} else {
					//rtmc->nodes[0] = rasqal_literal_to_redland_node(rtsc->world, t->subject);
					rtmc->nodes[0] = NULL;
				}

				m->bindings[0] = var;


				if ((var = rasqal_literal_as_variable(t->predicate))) {
					if (var->value) {
						//rtmc->nodes[1] = rasqal_literal_to_redland_node(rtsc->world, var->value);
						rtmc->nodes[1] = NULL;
						var = NULL;
					} else {
						rtmc->nodes[1] = NULL;
					}
				} else {
					//rtmc->nodes[1] = rasqal_literal_to_redland_node(rtsc->world, t->predicate);
					rtmc->nodes[1] = NULL;
				}

				m->bindings[1] = var;


				if ((var = rasqal_literal_as_variable(t->object))) {
					if (var->value) {
						//rtmc->nodes[2] = rasqal_literal_to_redland_node(rtsc->world, var->value);
						rtmc->nodes[2] = NULL;
						var = NULL;
					} else {
						rtmc->nodes[2] = NULL;
					}
				} else {
					//rtmc->nodes[2] = rasqal_literal_to_redland_node(rtsc->world, t->object);
					rtmc->nodes[2] = NULL;
				}

				m->bindings[2] = var;


				if (t->origin != NULL) {
					//if ((var = rasqal_literal_as_variable(t->origin))) {
					//	if (var->value) {
					//		rtmc->origin = rasqal_literal_to_redland_node(rtsc->world, var->value);
					//	}
					//} else {
					//	rtmc->origin = rasqal_literal_to_redland_node(rtsc->world, t->origin);
					rtmc->origin = (librdf_node*)t->origin;
						//rtmc->origin = (librdf_node*)t->origin;
					//}
					//m->bindings[3] = var;
				}

				rtmc->qstatement = NULL;
				//rtmc->qstatement = librdf_new_statement_from_nodes(rtsc->world,
				//	rtmc->nodes[0],
				//	rtmc->nodes[1],
				//	rtmc->nodes[2]);
				//if (!rtmc->qstatement) {
				//	return 1;
				//}

#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
				//LIBRDF_DEBUG1("query statement: ");
				//librdf_statement_print(rtmc->qstatement, stderr);
				//if (rtmc->origin) {
				//	fput(" with context node: ", stderr);
				//	librdf_node_print(rtmc->origin, stderr);
				//}
				//fputc('\n', stderr);
#endif

				if (rtmc->origin) {
					rtmc->stream = ((librdf_model_factory_s*)((librdf_model_s*)rtsc->model)->factory)->find_statements_in_context(rtsc->model, rtmc->qstatement, rtmc->origin);
				} else {
					rtmc->stream = ((librdf_model_factory_s*)((librdf_model_s*)rtsc->model)->factory)->find_statements(rtsc->model, rtmc->qstatement);
				}

				if (!rtmc->stream)
					return 1;

#if defined(LIBRDF_DEBUG) && LIBRDF_DEBUG > 1
				LIBRDF_DEBUG1("rasqal_init_triples_match done\n");
#endif

				return 0;
			}



















































































































			rasqal_literal* create_literal_from_binding(CRedlandRasqalSubQueryProcessingData* subQueryProcData, CVariableBindingResult* varBin) {
				rasqal_literal* l = nullptr;
				librdf_world* world = subQueryProcData->getRedlandWorld();
				rasqal_world* rasqal_world_ptr = librdf_world_get_rasqal(world);
				if (varBin) {
					if (varBin->isNamedIndividualBindingType()) {
						QByteArray bytes = varBin->getNamedIndividualBindingString().toUtf8();
						raptor_uri* uri = (raptor_uri*)librdf_new_uri(world, (const unsigned char*)bytes.constData());
						l = rasqal_new_uri_literal(rasqal_world_ptr, uri); /* transfer uri ownership to literal */
					} else if (varBin->isAnonymousIndividualBindingType()) {
					} else if (varBin->isLiteralBindingType()) {
						QPair<QString, QString> litTypePair = varBin->getLiteralDatatypeDatavalueBindingStringPair();
						QByteArray datatypeBytes = litTypePair.first.toUtf8();
						QByteArray literalBytes = litTypePair.second.toUtf8();

						//librdf_uri* datatypeUri = librdf_new_uri(world, (const unsigned char*)datatypeBytes.constData());
						raptor_uri* datatypeUri = raptor_new_uri(librdf_world_get_raptor(world), (const unsigned char*)datatypeBytes.constData());
						//uri = librdf_node_get_literal_value_datatype_uri(node);
						//new_datatype = (raptor_uri*)librdf_new_uri_from_uri(uri);
						//node = librdf_new_node_from_typed_literal(world, (const unsigned char*)literalBytes.constData(), NULL, datatypeUri);

						char *new_language = NULL;
						unsigned char* new_string = (unsigned char*)rasqal_alloc_memory(literalBytes.length() + 1);
						strcpy((char*)new_string, (const char*)literalBytes.constData());

						l = rasqal_new_string_literal(rasqal_world_ptr, (const unsigned char*)new_string, new_language, datatypeUri, NULL);
					}
				}
				return l;
			}






			librdf_node* create_node_from_binding(CRedlandRasqalSubQueryProcessingData* subQueryProcData, CVariableBindingResult* varBin) {
				librdf_node* node = nullptr;
				librdf_world* world = subQueryProcData->getRedlandWorld();
				rasqal_world* rasqal_world_ptr = librdf_world_get_rasqal(world);

#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
				if (varBin) {
					if (varBin->isNamedIndividualBindingType()) {
						QByteArray bytes = varBin->getNamedIndividualBindingString().toUtf8();
						librdf_uri *uri = librdf_new_uri(world, (const unsigned char*)bytes.constData());
						node = librdf_new_node_from_uri(world, uri);
					} else if (varBin->isAnonymousIndividualBindingType()) {
					} else if (varBin->isLiteralBindingType()) {
						QPair<QString, QString> litTypePair = varBin->getLiteralDatatypeDatavalueBindingStringPair();
						QByteArray datatypeBytes = litTypePair.first.toUtf8();
						QByteArray literalBytes = litTypePair.second.toUtf8();
						librdf_uri* datatypeUri = librdf_new_uri(world, (const unsigned char*)datatypeBytes.constData());
						node = librdf_new_node_from_typed_literal(world, (const unsigned char*)literalBytes.constData(), NULL, datatypeUri);
					}
				} else {
					node = librdf_new_node_from_node(subQueryProcData->getDefaultReturningNode());
				}


#else 
				rasqal_literal* l = create_literal_from_binding(subQueryProcData, varBin);
				node = (librdf_node*)l;
#endif

				return node;
			}

			librdf_node* create_node_from_binding(CRedlandRasqalSubQueryProcessingData* subQueryProcData, QVector<CVariableBindingResult*>* bindingsVec, CRedlandRasqalSubQueryTripleResolvingData* resolvingData, cint64 pos) {
				librdf_node* node = nullptr;
				librdf_world* world = subQueryProcData->getRedlandWorld();
				if (!resolvingData->getVariableBound(pos)) {
					cint64 bindingPos = resolvingData->getBindingsVectorResolvingPosition(pos);
					if (bindingPos >= 0) {
						if (bindingPos < bindingsVec->size()) {
							CVariableBindingResult* varBin = (*bindingsVec)[bindingPos];
							node = create_node_from_binding(subQueryProcData, varBin);
						} else {
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
							node = librdf_new_node_from_node(subQueryProcData->getDefaultReturningNode());
#else 
							node = (librdf_node*)rasqal_new_literal_from_literal((rasqal_literal*)subQueryProcData->getDefaultReturningNode());
#endif
						}
					} else {
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
						node = librdf_new_node_from_node(subQueryProcData->getDefaultReturningNode());
#else 
						node = NULL;
#endif
					}
				} else {
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
					node = librdf_new_node_from_node(resolvingData->getBoundNode(pos));
#else 
					//node = (librdf_node*)rasqal_new_literal_from_literal(resolvingData->getBoundLiteral(pos));
					node = NULL;
#endif
				}
				return node;
			}


			librdf_statement* create_statement_from_bindings(CRedlandRasqalSubQueryProcessingData* subQueryProcData, QVector<CVariableBindingResult*>* bindingsVec, CRedlandRasqalSubQueryTripleResolvingData* resolvingData) {
				librdf_world* world = subQueryProcData->getRedlandWorld();
				librdf_node* subjectNode = create_node_from_binding(subQueryProcData, bindingsVec, resolvingData, 0);
				librdf_node* predicateNode = create_node_from_binding(subQueryProcData, bindingsVec, resolvingData, 1);
				librdf_node* objectNode = create_node_from_binding(subQueryProcData, bindingsVec, resolvingData, 2);
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
				librdf_statement* statement = librdf_new_statement_from_nodes(world, subjectNode, predicateNode, objectNode);
#else 
				librdf_statement* statement = resolvingData->getNextTransferringStatement();
				if (statement->subject) {
					rasqal_free_literal((rasqal_literal*)statement->subject);
					statement->subject = NULL;
				}
				if (statement->predicate) {
					rasqal_free_literal((rasqal_literal*)statement->predicate);
					statement->predicate = NULL;
				}
				if (statement->object) {
					rasqal_free_literal((rasqal_literal*)statement->object);
					statement->object = NULL;
				}
				statement->subject = subjectNode;
				statement->predicate = predicateNode;
				statement->object = objectNode;
#endif
				return statement;
			}


			void load_next_bindings(CRedlandRasqalSubQueryProcessingData* subQueryProcData) {
				*subQueryProcData->getCurrentBindingsVector() = *subQueryProcData->getNextBindingsVector();
				CVariableBindingsResultIterator* varBindIt = subQueryProcData->getVariableBindingsIterator();
				for (cint64 i = 0; i < subQueryProcData->getNextBindingsVector()->count(); ++i) {
					(*subQueryProcData->getNextBindingsVector())[i] = nullptr;
				}
				for (cint64 i = 0; i < subQueryProcData->getTriplesResolvingDataVector()->count(); ++i) {
					(*subQueryProcData->getTriplesResolvingDataVector())[i]->setResolved(false);
				}
				if (varBindIt) {
					cint64 nextPos = 0;
					while (varBindIt->hasNext()) {
						CVariableBindingResult* varBin = varBindIt->getNext(true);
						(*subQueryProcData->getNextBindingsVector())[nextPos++] = varBin;
					}
				}

			}

			
			int is_end_method_stream_next_answer_resolving(void* context) {
				CRedlandRasqalSubQueryProcessingData* subQueryProcData = (CRedlandRasqalSubQueryProcessingData*)context;
				subQueryProcData->setNextReturningPosition(subQueryProcData->getFirstTripleReturningBinding() + 1);
				CVariableBindingsResultIterator* varBindIt = subQueryProcData->getVariableBindingsIterator();
				if (!varBindIt) {
					return 1;
				}
				return 0;
			}

			int next_method_stream_next_answer_resolving(void* context) {
				CRedlandRasqalSubQueryProcessingData* subQueryProcData = (CRedlandRasqalSubQueryProcessingData*)context;
				subQueryProcData->setNextReturningPosition(subQueryProcData->getFirstTripleReturningBinding() + 1);
				CVariableBindingsAnswersResultIterator* answersIt = subQueryProcData->getAnswersIterator();
				CVariableBindingsResultIterator* varBindIt = subQueryProcData->getVariableBindingsIterator();
				if (answersIt) {
					CVariableBindingsAnswerResult* varBindAnswer = answersIt->getNext(true);
					if (varBindAnswer) {
						varBindIt = varBindAnswer->getVariableBindingsIterator(varBindIt);
						subQueryProcData->setVariableBindingsIterator(varBindIt);
						if (!answersIt->hasNext()) {
							subQueryProcData->setAnswersIterator(nullptr);
							delete answersIt;
						}
					} else {
						delete varBindIt;
						varBindIt = nullptr;
						subQueryProcData->setVariableBindingsIterator(varBindIt);
					}
				} else {
					delete varBindIt;
					varBindIt = nullptr;
					subQueryProcData->setVariableBindingsIterator(varBindIt);
				}
				subQueryProcData->incVisitedResultCountStatistics();
				load_next_bindings(subQueryProcData);
				if (!varBindIt) {
					return 1;
				}
				return 0;
			}
			
			void* get_method_stream_next_answer_resolving(void* context, int flags) {
				librdf_node* node = nullptr;

				CRedlandRasqalSubQueryProcessingData* subQueryProcData = (CRedlandRasqalSubQueryProcessingData*)context;
				QVector<CVariableBindingResult*>* nextBindingsVec = subQueryProcData->getNextBindingsVector();
				librdf_world* world = subQueryProcData->getRedlandWorld();
				subQueryProcData->setNextReturningPosition(subQueryProcData->getFirstTripleReturningBinding() + 1);

				CRedlandRasqalSubQueryTripleResolvingData* resolvingData = (*subQueryProcData->getTriplesResolvingDataVector())[subQueryProcData->getFirstTripleReturningBinding()];

				librdf_statement* statement = create_statement_from_bindings(subQueryProcData, nextBindingsVec, resolvingData);
				return (void*)statement;
			}
			
			void finished_method_stream_next_answer_resolving(void* context) {
			}









			int is_end_method_stream_next_binding_resolving(void* context) {

				CRedlandRasqalSubQueryTripleResolvingData* bindingResolvingData = (CRedlandRasqalSubQueryTripleResolvingData*)context;
				CRedlandRasqalSubQueryProcessingData* subQueryProcData = (CRedlandRasqalSubQueryProcessingData*)bindingResolvingData->getSubQueryProcessingData();

				if (!subQueryProcData->getBaseAnswerResult() || subQueryProcData->getBaseAnswerResult()->getResultCount() <= 0) {
					return 1;
				}

				if (bindingResolvingData->isResolved()) {
					return 1;
				}
				return 0;
			}

			int next_method_stream_next_binding_resolving(void* context) {
				CRedlandRasqalSubQueryTripleResolvingData* bindingResolvingData = (CRedlandRasqalSubQueryTripleResolvingData*)context;
				bindingResolvingData->setResolved(true);
				return 1;
			}

			void* get_method_stream_next_binding_resolving(void* context, int flags) {
				CRedlandRasqalSubQueryTripleResolvingData* bindingResolvingData = (CRedlandRasqalSubQueryTripleResolvingData*)context;
				CRedlandRasqalSubQueryProcessingData* subQueryProcData = (CRedlandRasqalSubQueryProcessingData*)bindingResolvingData->getSubQueryProcessingData();
				QVector<CVariableBindingResult*>* currentBindingsVec = subQueryProcData->getCurrentBindingsVector();

				librdf_statement* statement = create_statement_from_bindings(subQueryProcData, currentBindingsVec, bindingResolvingData);

				return (void*)statement;
			}

			void finished_method_stream_next_binding_resolving(void* context) {
			}




















			int is_end_method_stream_next_boolean_result(void* context) {

				CRedlandRasqalSubQueryTripleResolvingData* bindingResolvingData = (CRedlandRasqalSubQueryTripleResolvingData*)context;
				CRedlandRasqalSubQueryProcessingData* subQueryProcData = (CRedlandRasqalSubQueryProcessingData*)bindingResolvingData->getSubQueryProcessingData();


				if (!subQueryProcData->getBooleanQueryResult()) {
					return 1;
				}

				if (bindingResolvingData->isResolved()) {
					return 1;
				}
				return 0;
			}

			int next_method_stream_next_boolean_result(void* context) {
				CRedlandRasqalSubQueryTripleResolvingData* bindingResolvingData = (CRedlandRasqalSubQueryTripleResolvingData*)context;
				bindingResolvingData->setResolved(true);
				return 1;
			}

			void* get_method_stream_next_boolean_result(void* context, int flags) {
				CRedlandRasqalSubQueryTripleResolvingData* bindingResolvingData = (CRedlandRasqalSubQueryTripleResolvingData*)context;
				CRedlandRasqalSubQueryProcessingData* subQueryProcData = (CRedlandRasqalSubQueryProcessingData*)bindingResolvingData->getSubQueryProcessingData();
				QVector<CVariableBindingResult*>* currentBindingsVec = subQueryProcData->getCurrentBindingsVector();

				librdf_statement* statement = create_statement_from_bindings(subQueryProcData, currentBindingsVec, bindingResolvingData);

				return (void*)statement;
			}

			void finished_method_stream_next_boolean_result(void* context) {
			}













			CRedlandRasqalSubQueryTripleResolvingData* get_triple_resolving_data(CRedlandRasqalSubQueryProcessingData* subQueryProcData, librdf_statement* statement, cint64 triplePos) {
				QVector<CRedlandRasqalSubQueryTripleResolvingData*>* triplesResolvingDataVec = subQueryProcData->getTriplesResolvingDataVector();
				return (*triplesResolvingDataVec)[triplePos];
			}


			bool update_triple_bound_variables_resolving_data(CRedlandRasqalSubQueryProcessingData* subQueryProcData, rasqal_triple* triple, cint64 triplePos, QVector<rasqal_literal*>& varBoundVec) {
				bool requiresBindingReturning = false;
				QVector<CRedlandRasqalSubQueryTripleResolvingData*>* triplesResolvingDataVec = subQueryProcData->getTriplesResolvingDataVector();
				librdf_world* world = subQueryProcData->getRedlandWorld();
				CRedlandRasqalSubQueryTripleResolvingData* tripleResolveData = (*triplesResolvingDataVec)[triplePos];

				if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->subject)) {
					rasqal_variable * var = rasqal_literal_as_variable(triple->subject);
					tripleResolveData->setSubjectVariable(var);
					tripleResolveData->setSubjectVariableBound(var->value != nullptr);
					cint64 bindingPos = tripleResolveData->getSubjectBindingsVectorResolvingPosition();
					if (var->value && !varBoundVec[bindingPos]) {
						rasqal_literal* lit = rasqal_new_literal_from_literal(var->value);
						varBoundVec[bindingPos] = lit;
						tripleResolveData->setSubjectBoundLiteral(lit);
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
						librdf_node* boundNode = rasqal_literal_to_redland_node(world, var->value);
						tripleResolveData->setSubjectBoundNode(boundNode);
#endif
					} else {
						requiresBindingReturning = true;
						tripleResolveData->setSubjectBoundNode(nullptr);
					}
				} else {
					tripleResolveData->setSubjectVariable(nullptr);
				}

				if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->predicate)) {
					rasqal_variable * var = rasqal_literal_as_variable(triple->predicate);
					tripleResolveData->setPredicateVariable(var);
					tripleResolveData->setPredicateVariableBound(var->value != nullptr);
					cint64 bindingPos = tripleResolveData->getPredicateBindingsVectorResolvingPosition();
					if (var->value && !varBoundVec[bindingPos]) {
						rasqal_literal* lit = rasqal_new_literal_from_literal(var->value);
						varBoundVec[bindingPos] = lit;
						tripleResolveData->setPredicateBoundLiteral(lit);
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
						librdf_node* boundNode = rasqal_literal_to_redland_node(world, var->value);
						tripleResolveData->setPredicateBoundNode(boundNode);
#endif
					} else {
						requiresBindingReturning = true;
						tripleResolveData->setPredicateBoundNode(nullptr);
					}
				} else {
					tripleResolveData->setPredicateVariable(nullptr);
				}

				if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->object)) {
					rasqal_variable * var = rasqal_literal_as_variable(triple->object);
					tripleResolveData->setObjectVariable(var);
					tripleResolveData->setObjectVariableBound(var->value != nullptr);
					cint64 bindingPos = tripleResolveData->getObjectBindingsVectorResolvingPosition();
					if (var->value && !varBoundVec[bindingPos]) {
						rasqal_literal* lit = rasqal_new_literal_from_literal(var->value);
						varBoundVec[bindingPos] = lit;
						tripleResolveData->setObjectBoundLiteral(lit);
#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
						librdf_node* boundNode = rasqal_literal_to_redland_node(world, var->value);
						tripleResolveData->setObjectBoundNode(boundNode);
#endif
					} else {
						requiresBindingReturning = true;
						tripleResolveData->setObjectBoundNode(nullptr);
					}
				} else {
					tripleResolveData->setObjectVariable(nullptr);
				}

				return requiresBindingReturning;
			}



			CRedlandRasqalSubQueryTripleResolvingData* create_triple_resolving_data(CRedlandRasqalSubQueryProcessingData* subQueryProcData, rasqal_triple* triple, cint64 triplePos) {

				QVector<CRedlandRasqalSubQueryTripleResolvingData*>* triplesResolvingDataVec = subQueryProcData->getTriplesResolvingDataVector();
				if (triplesResolvingDataVec->count() <= triplePos) {

					cint64 nextVariableBindingPos = subQueryProcData->getNextBindingsVariablePosition();
					QStringList* varList = subQueryProcData->getTriplesRemainingBindingVariableList();

					cint64 tripleVariableCount = 0;

					cint64 subjectBindingPos = -1;
					cint64 predicateBindingPos = -1;
					cint64 objectBindingPos = -1;

					if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->subject)) {
						subjectBindingPos = nextVariableBindingPos;
						if (!varList->isEmpty()) {
							varList->removeFirst();
							if (!varList->isEmpty()) {
								nextVariableBindingPos++;
								++tripleVariableCount;
								subQueryProcData->incNextBindingsVariablePosition();
							}
						}
					}


					if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->predicate)) {
						predicateBindingPos = nextVariableBindingPos;
						if (!varList->isEmpty()) {
							varList->removeFirst();
							if (!varList->isEmpty()) {
								nextVariableBindingPos++;
								++tripleVariableCount;
								subQueryProcData->incNextBindingsVariablePosition();
							}
						}
					}
					if (RASQAL_LITERAL_VARIABLE == rasqal_literal_get_type(triple->object)) {
						objectBindingPos = nextVariableBindingPos;
						if (!varList->isEmpty()) {
							varList->removeFirst();
							if (!varList->isEmpty()) {
								nextVariableBindingPos++;
								++tripleVariableCount;
								subQueryProcData->incNextBindingsVariablePosition();
							}
						}
					}

					CRedlandRasqalSubQueryTripleResolvingData* newTripleResolvingData = new CRedlandRasqalSubQueryTripleResolvingData(subQueryProcData, triplePos, subjectBindingPos, predicateBindingPos, objectBindingPos);

					if (triplesResolvingDataVec->size() <= triplePos) {
						triplesResolvingDataVec->resize(triplePos + 1);
					}
					(*triplesResolvingDataVec)[triplePos] = newTripleResolvingData;
				}

				return (*triplesResolvingDataVec)[triplePos];
			}






			CRedlandRasqalSubQueryProcessingData* get_sub_query_processing_data(librdf_model* model, librdf_node* context_node) {
				librdf_model_storage_context* storageContext = (librdf_model_storage_context*)((librdf_model_s*)model)->context;
				CRedlandRasqalModelSubQueryResolver* resolver = (CRedlandRasqalModelSubQueryResolver*)((librdf_storage_s*)storageContext->storage)->instance;
				librdf_world* world = resolver->getRedlandWorld();

				if (!resolver->isTriplesContextLiteralsReset()) {
					QVector<rasqal_literal*>* triplesContextLiteralVector = resolver->getTriplesContextLiteralVector();
					rasqal_query* rasqalQuery = resolver->getRasqalQuery();
					raptor_sequence* tripleSeq = rasqal_query_get_triple_sequence(rasqalQuery);
					int tripleSeqSize = raptor_sequence_size(tripleSeq);
					for (int i = 0; i < tripleSeqSize; ++i) {
						rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(tripleSeq, i);
						rasqal_literal* tripleContextLit = rasqal_triple_get_origin(triple);
						if (tripleContextLit != (*triplesContextLiteralVector)[i]) {
							rasqal_triple_set_origin(triple, (*triplesContextLiteralVector)[i]);
						}
					}


					resolver->setTriplesContextLiteralsReset(true);
				}

				QVector<CRedlandRasqalSubQueryProcessingData*>* subQueryProcDataVec = resolver->getSubQueryProcessingDataVector();

				bool firstTriple = false;
				cint64 subQueryId = -1;

				if (context_node) {
					rasqal_literal* rasqContNode = (rasqal_literal*)context_node;
					const unsigned char* chrString = rasqal_literal_as_string(rasqContNode);
					int i = 0;

					if (chrString[0] == '-') {
						++i;
						firstTriple = true;
					}
					subQueryId = 0;
					while (true) {
						subQueryId += (chrString[i] - '0');
						if (chrString[i + 1] != 0) {
							++i;
							subQueryId *= 10;
						} else {
							break;
						}
					}
					subQueryId = subQueryId - 1;
				}

				//if (context_node && librdf_node_is_literal(context_node)) {
				//	unsigned char* chrString = librdf_node_get_literal_value(context_node);
				//	int i = 0;

				//	if (chrString[0] == '-') {
				//		++i;
				//		firstTriple = true;
				//	}
				//	subQueryId = 0;
				//	while (true) {
				//		subQueryId += (chrString[i] - '0');
				//		if (chrString[i + 1] != 0) {
				//			++i;
				//			subQueryId *= 10;
				//		} else {
				//			break;
				//		}
				//	}
				//	subQueryId = subQueryId - 1;
				//}

				//if (context_node != NULL) {
				//	subQueryId = (cint64)subQueryId;
				//	if (subQueryId < 0) {
				//		firstTriple = true;
				//		subQueryId = -subQueryId;
				//	}
				//	subQueryId = subQueryId - 1;
				//}


				CRedlandRasqalSubQueryProcessingData* subQueryProcData = subQueryProcDataVec->at(subQueryId);
				if (firstTriple) {
					subQueryProcData->setNextReturningPosition(0);
				}
				return subQueryProcData;
			}


			void initialize_update_resolving(CRedlandRasqalSubQueryProcessingData* subQueryProcData) {
				rasqal_graph_pattern* subqueryGraphPattern = subQueryProcData->getSubqueryGraphPattern();
				rasqal_graph_pattern_s *gps = (rasqal_graph_pattern_s*)subqueryGraphPattern;
				int triplesStartColumn = subQueryProcData->getGPTripleStartPos();
				int triplesEndColumn = subQueryProcData->getGPTriplesEndPos();

				librdf_world* world = subQueryProcData->getRedlandWorld();
				rasqal_world* rasqal_world_ptr = librdf_world_get_rasqal(world);

				if (!subQueryProcData->isInitialized()) {
					if (subQueryProcData->isBooleanSubQuery()) {
						QVector<rasqal_literal*> varBoundVec(1);
						rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(gps->triples, 0);
						create_triple_resolving_data(subQueryProcData, triple, 0);

						QByteArray bytes = QString("x").toUtf8();
						raptor_uri* uri = (raptor_uri*)librdf_new_uri(world, (const unsigned char*)bytes.constData());

#ifdef KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING
						librdf_node* defaultNode = librdf_new_node_from_uri(world, uri);
						subQueryProcData->setDefaultReturningNode(defaultNode);
#else
						librdf_node* defaultNode = (librdf_node*)rasqal_new_uri_literal(rasqal_world_ptr, uri);
						subQueryProcData->setDefaultReturningNode(defaultNode);
#endif //KONCLUDE_REDLAND_RASQAL_SUBQUERY_LIBRDF_NODE_RESOLVING

					} else {
						CQueryResult* queryResult = subQueryProcData->getSubQuery()->getQueryResult();
						CVariableBindingsAnswersResult* varIndBindsResult = dynamic_cast<CVariableBindingsAnswersResult*>(queryResult);
						if (varIndBindsResult) {
							const QStringList& varList = varIndBindsResult->getVariableNames();
							*subQueryProcData->getTriplesRemainingBindingVariableList() = varList;
							subQueryProcData->getCurrentBindingsVector()->resize(varList.size());
							subQueryProcData->getNextBindingsVector()->resize(varList.size());
							subQueryProcData->setBindingsCount(varList.size());
							subQueryProcData->getBindingPositionVariableBoundRedlandNodeVector()->resize(varList.size());

							QByteArray bytes = QString("x").toUtf8();
							librdf_uri *uri = librdf_new_uri(world, (const unsigned char*)bytes.constData());
							librdf_node* defaultNode = librdf_new_node_from_uri(world, uri);
							subQueryProcData->setDefaultReturningNode(defaultNode);

							cint64 triplePos = 0;
							for (int i = triplesStartColumn; i <= triplesEndColumn; ++i) {
								rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(gps->triples, i);
								create_triple_resolving_data(subQueryProcData, triple, triplePos++);
							}
							subQueryProcData->setTotalResultCountStatistics(varIndBindsResult->getResultCount());
						}
					}
					subQueryProcData->setInitialized(true);
				}

				if (!subQueryProcData->isBooleanSubQuery()) {
					// analyze which variables are already bound
					cint64 triplePos = 0;
					QVector<rasqal_literal*> varBoundVec(subQueryProcData->getBindingsCount());
					QVector<rasqal_literal*>* bindPosVarBoundNodeVec = subQueryProcData->getBindingPositionVariableBoundRedlandNodeVector();
					int triplePosFIrstBindingReturning = -1;
					for (int i = triplesStartColumn; i <= triplesEndColumn; ++i) {
						rasqal_triple* triple = (rasqal_triple*)raptor_sequence_get_at(gps->triples, i);
						if (update_triple_bound_variables_resolving_data(subQueryProcData, triple, triplePos++, varBoundVec)) {
							if (triplePosFIrstBindingReturning == -1) {
								triplePosFIrstBindingReturning = i - triplesStartColumn;
							}
						}
					}
					subQueryProcData->setFirstTripleReturningBinding(triplePosFIrstBindingReturning);
					bool hasBoundVariables = false;
					for (int i = 0; i < subQueryProcData->getBindingsCount(); ++i) {
						rasqal_literal*& prevNode = (*bindPosVarBoundNodeVec)[i];
						if (prevNode) {
							rasqal_free_literal(prevNode);
						}
						prevNode = varBoundVec[i];
						if (prevNode) {
							hasBoundVariables = true;
						}
					}
					subQueryProcData->setBoundVariables(hasBoundVariables);


					CVariableBindingsAnswersResult* baseAnswerResult = nullptr;
					if (!hasBoundVariables) {
						CQueryResult* queryResult = subQueryProcData->getSubQuery()->getQueryResult();
						baseAnswerResult = dynamic_cast<CVariableBindingsAnswersResult*>(queryResult);
					} else {
						subQueryProcData->incResultOrderingCountStatistics();
						CRedlandRasqalSubQueryBoundNodeResolvingHashData* bindingsPosResolveData = subQueryProcData->getBoundResolvingDataBase();
						for (cint64 i = 0; i < subQueryProcData->getBindingsCount() && bindingsPosResolveData->getResolvedVariableBindingsResult() && bindingsPosResolveData->getResolvedVariableBindingsResult()->getResultCount() > 0; ++i) {
							rasqal_literal* boundLiteral = (*bindPosVarBoundNodeVec)[i];
							if (boundLiteral) {
								CRedlandRasqalSubQueryBindingPositionResolvingHash* bindingsPosResolveHash = bindingsPosResolveData->getBindingPositionResolvingHash();
								CRedlandRasqalSubQueryBoundNodeResolvingHash* boundNodeResolvHash = bindingsPosResolveHash->getBindingPositionResolvingData(i)->getBoundNodeResolvingHash();
								if (!boundNodeResolvHash->isResolved()) {
									//QString resHashString;
									CVariableBindingsAnswersResult* varIndBindsResult = boundNodeResolvHash->getResolvingVariableBindingsResult();
									// resolve
									CVariableBindingsAnswersResultIterator* answersIt = varIndBindsResult->getVariableBindingsAnswersIterator();
									while (answersIt->hasNext()) {
										CVariableBindingsAnswerResult* varBindAnswer = answersIt->getNext();
										CVariableBindingsListAnswerResult* listVarBindAnswer = dynamic_cast<CVariableBindingsListAnswerResult*>(varBindAnswer);
										if (listVarBindAnswer && i < listVarBindAnswer->getResultCount()) {
											CVariableBindingResult* varBindRes = listVarBindAnswer->getResult(i);
											rasqal_literal* literal = create_literal_from_binding(subQueryProcData, varBindRes);
											//resHashString += QString("%1\r\n").arg(QString::fromUtf8((const char*)rasqal_literal_as_string(literal)));
											boundNodeResolvHash->getBoundNodeResolvingData(literal)->getResolvedVariableBindingsResult()->addResultVariableBindings(varBindAnswer);
										}
									}
									boundNodeResolvHash->setResolved(true);
									//if (subQueryProcData->getSubQueryId() == 3) {
									//	cout << "\r\n resolving " << rasqal_literal_as_string(boundLiteral) << " \r\n";
									//	bool debug = true;
									//}

								}
								bindingsPosResolveData = boundNodeResolvHash->getBoundNodeResolvingData(boundLiteral);
							}
						}
						baseAnswerResult = bindingsPosResolveData->getResolvedVariableBindingsResult();
						subQueryProcData->setLastBoundResolvingData(bindingsPosResolveData);
					}
					subQueryProcData->setBaseAnswerResult(baseAnswerResult);



					CVariableBindingsAnswersResultIterator* answersIt = subQueryProcData->getAnswersIterator();
					CVariableBindingsResultIterator* varBindIt = subQueryProcData->getVariableBindingsIterator();

					if (answersIt) {
						delete answersIt;
						answersIt = nullptr;
					}
					if (varBindIt) {
						delete varBindIt;
						varBindIt = nullptr;
					}

					CVariableBindingsAnswersResult* varIndBindsResult = baseAnswerResult;

					if (varIndBindsResult) {
						answersIt = varIndBindsResult->getVariableBindingsAnswersIterator();
						if (answersIt && answersIt->hasNext()) {
							CVariableBindingsAnswerResult* varBindAnswer = answersIt->getNext(true);
							varBindIt = varBindAnswer->getVariableBindingsIterator(varBindIt);
						} else {
							delete answersIt;
							answersIt = nullptr;
						}
					}
					subQueryProcData->setAnswersIterator(answersIt);
					subQueryProcData->setVariableBindingsIterator(varBindIt);
					subQueryProcData->setCurrentVariableBinding(nullptr);

					load_next_bindings(subQueryProcData);
				}
			}


			librdf_stream* find_statements_context_resolving(librdf_model* model, librdf_statement* statement, librdf_node* context_node) {
				librdf_model_storage_context* storageContext = (librdf_model_storage_context*)((librdf_model_s*)model)->context;				
				CRedlandRasqalModelSubQueryResolver* resolver = (CRedlandRasqalModelSubQueryResolver*)((librdf_storage_s*)storageContext->storage)->instance;
				librdf_world* world = resolver->getRedlandWorld();

				CRedlandRasqalSubQueryProcessingData* subQueryProcData = get_sub_query_processing_data(model, context_node);

				cint64 nextPos = subQueryProcData->getNextReturningPosition();
				subQueryProcData->setNextReturningPosition(nextPos + 1);


				if (nextPos == 0) {
					initialize_update_resolving(subQueryProcData);
				}
				if (subQueryProcData->isBooleanSubQuery()) {
					CRedlandRasqalSubQueryTripleResolvingData* tripleResolvData = get_triple_resolving_data(subQueryProcData, statement, 0);
					librdf_stream * stream = librdf_new_stream(world, tripleResolvData, is_end_method_stream_next_boolean_result, next_method_stream_next_boolean_result, get_method_stream_next_boolean_result, finished_method_stream_next_boolean_result);
					return stream;

				} else if (nextPos == subQueryProcData->getFirstTripleReturningBinding()) {
					librdf_stream * stream = librdf_new_stream(world, subQueryProcData, is_end_method_stream_next_answer_resolving, next_method_stream_next_answer_resolving, get_method_stream_next_answer_resolving, finished_method_stream_next_answer_resolving);
					return stream;
				} else {
					CRedlandRasqalSubQueryTripleResolvingData* tripleResolvData = get_triple_resolving_data(subQueryProcData, statement, nextPos);
					librdf_stream * stream = librdf_new_stream(world, tripleResolvData, is_end_method_stream_next_binding_resolving, next_method_stream_next_binding_resolving, get_method_stream_next_binding_resolving, finished_method_stream_next_binding_resolving);
					return stream;
				}
			}

			librdf_stream* find_statements_resolving(librdf_model* model, librdf_statement* statement) {
				return NULL;
			}


			int contains_statement_with_context_resolving(librdf_model* model, librdf_statement* statement, librdf_node* context_node) {

				librdf_model_storage_context* storageContext = (librdf_model_storage_context*)((librdf_model_s*)model)->context;
				CRedlandRasqalModelSubQueryResolver* resolver = (CRedlandRasqalModelSubQueryResolver*)((librdf_storage_s*)storageContext->storage)->instance;
				librdf_world* world = resolver->getRedlandWorld();

				CRedlandRasqalSubQueryProcessingData* subQueryProcData = get_sub_query_processing_data(model, context_node);

				cint64 nextPos = subQueryProcData->getNextReturningPosition();
				subQueryProcData->setNextReturningPosition(nextPos + 1);
				if (nextPos == 0) {
					initialize_update_resolving(subQueryProcData);
				}

				if (subQueryProcData->isBooleanSubQuery()) {
					if (subQueryProcData->getBooleanQueryResult()) {
						return 1;
					} else {
						return 0;
					}
				}

				if (!subQueryProcData->getBaseAnswerResult() || subQueryProcData->getBaseAnswerResult()->getResultCount() <= 0) {
					return 0;
				}

				return 1;
			}

			int contains_statement_resolving(librdf_model* model, librdf_statement* statement) {
				return -1;
			}


			void destroy_resolving(librdf_model* model) {
			}


			librdf_world* CRedlandRasqalModelSubQueryResolver::getRedlandWorld() {
				return mWorld;
			}


			void CRedlandRasqalModelSubQueryResolver::releaseRedlandResolvingModel() {
				if (mResolvingModel) {
					for (CRedlandRasqalSubQueryProcessingData* subQueryProcData : *mSubQueryProcDataVec) {
						subQueryProcData->deleteBoundResolvingData();
					}
					librdf_model_s* tmpModel = (librdf_model_s*)mResolvingModel;
					delete tmpModel->factory;
					delete ((librdf_storage_s*)((librdf_model_storage_context*)tmpModel->context)->storage)->factory;
					delete ((librdf_model_storage_context*)tmpModel->context)->storage;
					delete tmpModel->context;
					delete mResolvingModel;
					mResolvingModel = nullptr;
				}
			}



			void rasqal_log_handler_error_notifier_resolving(void *user_data, raptor_log_message *message) {
				QString** rasqalParsingErrorString = (QString**)user_data;
				if (message->level <= 50 && *rasqalParsingErrorString == nullptr) {
					*rasqalParsingErrorString = new QString(QString("%1").arg(message->text));
				}
			}



			int get_contexts_resolving_iterator_is_end(void* data) {
				CRedlandRasqalModelSubQueryResolver* resolver = (CRedlandRasqalModelSubQueryResolver*)data;
				QHash<QString, TDependentOntologyPair>::const_iterator it = resolver->getQueryKnowledgeBaseNameDependentOntologyPairHashIterator();
				QHash<QString, TDependentOntologyPair>::const_iterator itEnd = resolver->getQueryKnowledgeBaseNameDependentOntologyPairHash()->constEnd();
				if (it != itEnd) {
					return 0;
				}
				return 1;
			}

			int get_contexts_resolving_iterator_next(void* data) {
				CRedlandRasqalModelSubQueryResolver* resolver = (CRedlandRasqalModelSubQueryResolver*)data;
				QHash<QString, TDependentOntologyPair>::const_iterator it = resolver->getQueryKnowledgeBaseNameDependentOntologyPairHashIterator();
				QHash<QString, TDependentOntologyPair>::const_iterator itEnd = resolver->getQueryKnowledgeBaseNameDependentOntologyPairHash()->constEnd();
				if (it != itEnd) {
					resolver->setQueryKnowledgeBaseNameDependentOntologyPairHashIterator(++it);
					return 0;
				}
				return 1;
			}

			void* get_contexts_resolving_iterator_get(void* data, int flags) {
				CRedlandRasqalModelSubQueryResolver* resolver = (CRedlandRasqalModelSubQueryResolver*)data;
				librdf_world* world = resolver->getRedlandWorld();
				QHash<QString, TDependentOntologyPair>::const_iterator it = resolver->getQueryKnowledgeBaseNameDependentOntologyPairHashIterator();
				QHash<QString, TDependentOntologyPair>::const_iterator itEnd = resolver->getQueryKnowledgeBaseNameDependentOntologyPairHash()->constEnd();
				if (it != itEnd) {
					QString kbName = it.key();
					QByteArray kbNameData = kbName.toUtf8();
					librdf_node* node = librdf_new_node_from_uri_string(world, (const unsigned char*)kbNameData.constData());
					return node;
				}
				return NULL;
			}

			void get_contexts_resolving_iterator_finished(void* data) {
			}

			librdf_iterator* get_contexts_resolving(librdf_model* model) {
				librdf_model_storage_context* storageContext = (librdf_model_storage_context*)((librdf_model_s*)model)->context;
				CRedlandRasqalModelSubQueryResolver* resolver = (CRedlandRasqalModelSubQueryResolver*)((librdf_storage_s*)storageContext->storage)->instance;
				librdf_world* world = resolver->getRedlandWorld();
				resolver->setQueryKnowledgeBaseNameDependentOntologyPairHashIterator(resolver->getQueryKnowledgeBaseNameDependentOntologyPairHash()->constBegin());

				librdf_iterator * iterator = librdf_new_iterator(world, resolver, &get_contexts_resolving_iterator_is_end, &get_contexts_resolving_iterator_next, &get_contexts_resolving_iterator_get, &get_contexts_resolving_iterator_finished);

				return iterator;
			}



			librdf_model* CRedlandRasqalModelSubQueryResolver::getRedlandResolvingModel() {
				if (!mResolvingModel) {

					librdf_storage* indexedStorage = librdf_new_storage(mWorld, "hashes", NULL, "hash-type='memory',index-predicates='yes'");
					librdf_model* indexedModel = librdf_new_model(mWorld, indexedStorage, NULL);


					rasqal_world* rasqal_world_ptr = librdf_world_get_rasqal(mWorld);
					rasqal_world_set_log_handler(rasqal_world_ptr, &mRasqalErrorString, &rasqal_log_handler_error_notifier_resolving);

					rasqal_set_triples_source_factory(rasqal_world_ptr, rasqal_redland_register_triples_source_factory, mWorld);


					librdf_model_s* tmpModel = new librdf_model_s();
					tmpModel->factory = new librdf_model_factory_s();
					tmpModel->supports_contexts = 1;
					//tmpModel->factory->supports_query = &supports_query_resolving;
					tmpModel->factory->find_statements = &find_statements_resolving;
					tmpModel->factory->find_statements_in_context = &find_statements_context_resolving;
					tmpModel->factory->query_execute = ((librdf_model_s*)indexedModel)->factory->query_execute;
					tmpModel->factory->contains_statement_with_context = &contains_statement_with_context_resolving;
					tmpModel->factory->contains_statement = &contains_statement_resolving;
					tmpModel->factory->destroy = &destroy_resolving;
					tmpModel->factory->get_contexts = &get_contexts_resolving;
					tmpModel->usage = 1;


					librdf_model_storage_context* modelStorageContext = new librdf_model_storage_context();
					librdf_storage_s* storage = new librdf_storage_s();
					librdf_storage_factory_s* storageFactory = new librdf_storage_factory_s();
					storageFactory->supports_query = nullptr;
					storage->factory = storageFactory;
					modelStorageContext->storage = (librdf_storage*)storage;
					storage->instance = this;

					tmpModel->context = modelStorageContext;

					mResolvingModel = (librdf_model*)tmpModel;

					librdf_free_model(indexedModel);
					librdf_free_storage(indexedStorage);
				}
				return mResolvingModel;
			}


			QVector<CRedlandRasqalSubQueryProcessingData*>* CRedlandRasqalModelSubQueryResolver::getSubQueryProcessingDataVector() {
				return mSubQueryProcDataVec;
			}


			QString CRedlandRasqalModelSubQueryResolver::getIterationStatusString() {
				QString statString = QString("Redland Rasqal query processing");
				for (CRedlandRasqalSubQueryProcessingData* subQueryProcData : *mSubQueryProcDataVec) {
					QString subQueryStatString = QString(", visited %2 of %3 results of SubQuery-%1 (with %4 reorderings/lookups)").arg(subQueryProcData->getSubQueryId()).arg(subQueryProcData->getVisitedResultCountStatistics()).arg(subQueryProcData->getTotalResultCountStatistics()).arg(subQueryProcData->getResultOrderingCountStatistics());
					statString += subQueryStatString;
				}
				return statString;
			}


			QHash<QString, TDependentOntologyPair>* CRedlandRasqalModelSubQueryResolver::getQueryKnowledgeBaseNameDependentOntologyPairHash() {
				return mQueryKbNameDepOntPairHash;
			}

			QHash<QString, TDependentOntologyPair>::const_iterator CRedlandRasqalModelSubQueryResolver::getQueryKnowledgeBaseNameDependentOntologyPairHashIterator() {
				return mQueryKbNameDepOntPairHashIt;
			}

			CRedlandRasqalModelSubQueryResolver* CRedlandRasqalModelSubQueryResolver::setQueryKnowledgeBaseNameDependentOntologyPairHashIterator(QHash<QString, TDependentOntologyPair>::const_iterator it) {
				mQueryKbNameDepOntPairHashIt = it;
				return this;
			}

			QVector<rasqal_literal*>* CRedlandRasqalModelSubQueryResolver::getTriplesContextLiteralVector() {
				return &mTriplesContextLiteralVector;
			}

			bool CRedlandRasqalModelSubQueryResolver::isTriplesContextLiteralsReset() {
				return mTriplesContextLiteralsReset;
			}

			CRedlandRasqalModelSubQueryResolver* CRedlandRasqalModelSubQueryResolver::setTriplesContextLiteralsReset(bool reset) {
				mTriplesContextLiteralsReset = reset;
				return this;
			}

			rasqal_query* CRedlandRasqalModelSubQueryResolver::getRasqalQuery() {
				return mRasqalQuery;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // KONCLUDE_REDLAND_INTEGRATION
