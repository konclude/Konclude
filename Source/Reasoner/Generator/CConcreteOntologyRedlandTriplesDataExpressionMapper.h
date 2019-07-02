/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYREDLANDTRIPLESDATAEXPRESSIONMAPPER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYREDLANDTRIPLESDATAEXPRESSIONMAPPER_H


// Libraries includes
#include <QHash>
#include <QStack>


// Namespace includes
#include "CConcreteOntologyTriplesExpressionMapper.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Triples/CRedlandStoredTriplesData.h"
#include "Reasoner/Triples/CRedlandNodeHasher.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Utilities::Container;
	using namespace Parser;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Triples;

		namespace Generator {

			/*! 
			 *
			 *		\class		CConcreteOntologyRedlandTriplesDataExpressionMapper
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyRedlandTriplesDataExpressionMapper : public CConcreteOntologyTriplesExpressionMapper {
				// public methods
				public:
					CConcreteOntologyRedlandTriplesDataExpressionMapper(COntologyBuilder* ontologyBuilder);
					virtual ~CConcreteOntologyRedlandTriplesDataExpressionMapper();


					virtual bool mapTriples(CConcreteOntology* updateConcreteOntology, COntologyTriplesData* ontologyTripleData);




				// protected functions
				protected:


					
					class CRedlandNodeProcessingData {
						public:
							CRedlandNodeProcessingData(librdf_node* redlandNode, CBuildExpression* expression) {
								mRedlandNode = redlandNode;
								mExpression = expression;
							}

						public:
							librdf_node* mRedlandNode = nullptr;
							CBuildExpression* mExpression = nullptr;
							QList<CRedlandNodeProcessingData*>* mDependendRedlandNodeProcessingDataList = nullptr;
							function<CBuildExpression*(const QList<CBuildExpression*>&)> mExpressionBuildFunc;
							bool mBuilt = false;
							bool mProcessed = false;
							bool mStacked = false;
					};

					
					class CRedlandTripleStream : public CLinkerBase<CRedlandTripleStream*, CRedlandTripleStream> {
						public:
							CRedlandTripleStream* init(librdf_stream* stream) {
								mStream = stream;
								mCurrentStatement = nullptr;
								return this;
							}

							CRedlandTripleStream* clear() {
								librdf_free_stream(mStream);
								mStream = nullptr;
								mCurrentStatement = nullptr;
								return this;
							}

							bool atEnd() {
								return !mCurrentStatement && librdf_stream_end(mStream);
							}

							librdf_statement* getStatement() {
								if (!mCurrentStatement) {
									mCurrentStatement = librdf_stream_get_object(mStream);
								}
								return mCurrentStatement;
							}

							CRedlandTripleStream* moveNext() {
								mCurrentStatement = nullptr;
								librdf_stream_next(mStream);
								return this;
							}

							CRedlandTripleStream* forEach(function<void(librdf_statement*)> func) {
								while (!atEnd()) {
									librdf_statement* statement = getStatement();
									func(statement);
									moveNext();
								}
								return this;
							}

						protected:
							librdf_statement* mCurrentStatement;
							librdf_stream* mStream;

					};


					class CRedlandNodeStream : public CLinkerBase<CRedlandNodeStream*, CRedlandNodeStream> {
						public:
							CRedlandNodeStream* init(CRedlandTripleStream* stream, int nodePos) {
								mNodePos = nodePos;
								mCurrentNode = nullptr;
								mStreamWrapper = stream;
								return this;
							}

							CRedlandNodeStream* clear(function<void(CRedlandTripleStream*)> streamFreeFunc) {
								if (mStreamWrapper) {
									streamFreeFunc(mStreamWrapper);
									mStreamWrapper = nullptr;
								}
								mCurrentNode = nullptr;
								mNodePos = 0;
								return this;
							}

							librdf_node* getNode() {
								librdf_statement* statement = mStreamWrapper->getStatement();
								if (!mCurrentNode) {
									if (mNodePos == 0) {
										mCurrentNode = librdf_statement_get_subject(statement);
									}
									else if (mNodePos == 1) {
										mCurrentNode = librdf_statement_get_predicate(statement);
									}
									else {
										mCurrentNode = librdf_statement_get_object(statement);
									}
									return mCurrentNode;
								}
								return nullptr;
							}

							CRedlandNodeStream* moveNext() {
								mCurrentNode = nullptr;
								mStreamWrapper->moveNext();
								return this;
							}

							bool atEnd() {
								return !mCurrentNode && mStreamWrapper->atEnd();
							}

							CRedlandNodeStream* forEach(function<void(librdf_node*)> func) {
								while (!atEnd()) {
									librdf_node* node = getNode();
									func(node);
									moveNext();
								}
								return this;
							}


						protected:
							CRedlandTripleStream* mStreamWrapper;
							librdf_node* mCurrentNode;
							int mNodePos;

					};


					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildComplexABoxAxioms();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildSimpleABoxAxioms();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectBetweenIndividualsBasedAxiomExpression(CRedlandNodeProcessingData* nextProcessingData, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunc, librdf_statement* filteringStatement, QList<CAxiomExpression*>& axiomList);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectBetweenIndividualsBasedAxiomExpression(function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunc, librdf_statement* filteringStatement, QList<CAxiomExpression*>& axiomList);


					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectBuildExpressionsFromProcessingDataList(QList<CRedlandNodeProcessingData*>& processingDataList, QList<CBuildExpression*>& expressionList);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildDeclarations();

					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildDatatypeBasedAxioms();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildClassBasedAxioms();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildObjectPropertyBasedAxioms();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildDataPropertyBasedAxioms();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildAxioms(QList<CRedlandNodeProcessingData*>* nodeHandlingList, function<void(CRedlandNodeProcessingData*, QList<CAxiomExpression*>&)> dependenciesCollectionFunction);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectDatatypeBasedAxiomExpressions(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectDatatypeBasedAxiomExpressionsBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions);
					
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectClassBasedAxiomExpressions(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectClassBasedAxiomExpressionsBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectObjectPropertyBasedAxiomExpressions(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectObjectPropertyBasedAxiomExpressionsBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval(QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval(function<CRedlandNodeStream*()> nodeStreamFunc, function<CAxiomExpression*(const QList<CBuildExpression*>& expressions)> buildFunc, QList<CAxiomExpression*>& axiomExpressions);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectDataPropertyBasedAxiomExpressions(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectDataPropertyBasedAxiomExpressionsBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectDataPropertyTypesAxiomExpressionsBySuccessorRetrieval(QList<CAxiomExpression*>& axiomExpressions);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildSeparateNodeBasedAxioms();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildAxiomExpressionForMemberRDFNodeList(librdf_node* node, librdf_statement* partialFilteringStatementForOWLMembers, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildAxiomExpressionForMemberRDFNodeListWithNewHandling(librdf_node* node, librdf_statement* partialFilteringStatementForOWLMembers, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildAxiomExpressionForObjectORDataPropertyMemberRDFNodeList(librdf_node* node, librdf_statement* partialFilteringStatementForOWLMembers, function<CAxiomExpression*(const QList<CBuildExpression*>&, bool hasDataProperties, bool hasObjectProperties)> buildFunction, QList<CAxiomExpression*>& axiomExpressions);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildAxiomExpressionFromNode(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& expressionList, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildAxiomExpressionFromRDFNodeList(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& expressionList, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildAxiomExpressionFromRDFNodeListWithNewHandling(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CBuildExpression*>& expressionList, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions);



					CConcreteOntologyRedlandTriplesDataExpressionMapper* handleDeclaration(librdf_node* node, function<CBuildExpression*(const QString &uri)> buildEntityFunc, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>& nodeHandlingList);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* handleClassExpressionRestriction(librdf_node* node);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectClassExpressionDependencies(CRedlandNodeProcessingData* processingData);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectClassExpressionDependenciesBySuccessorRetrieval(CRedlandNodeProcessingData* processingData);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectDataRangeExpressionDependencies(CRedlandNodeProcessingData* processingData);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectDataRangeExpressionDependenciesBySuccessorRetrieval(CRedlandNodeProcessingData* processingData);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectDataFacetExpressionBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, librdf_node* node, librdf_node* facetNode, const char* facetUri);

					

					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildExpressions(QList<CRedlandNodeProcessingData*>* nodeHandlingList, function<void(CRedlandNodeProcessingData*)> dependenciesCollectionFunction);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildObjectPropertyExpressions();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildDataRangeExpressions();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* buildClassExpressions();


					CRedlandNodeProcessingData* createLiteralValueProcessingData(librdf_node* node);
					CRedlandNodeProcessingData* createNamedIndividualProcessingData(librdf_node* node);

					QList<librdf_node*>* getOWLListElementNodesList(librdf_node* listNode, QList<librdf_node*>* extendingList = nullptr);


					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectBuildExpressionsFromNode(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& existingList);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectBuildExpressionsFromRDFNodeList(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& existingList);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectBuildExpressionsFromNodeStream(CRedlandNodeStream* nodeStream, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& existingList);
				
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectBuildExpressionsFromNodeWithNewHandling(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CBuildExpression*>& existingList);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectBuildExpressionsFromNodeStreamWithNewHandling(CRedlandNodeStream* nodeStream, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CBuildExpression*>& existingList);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* collectBuildExpressionsFromRDFNodeListWithNewHandling(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CBuildExpression*>& existingList);


					QList<CRedlandNodeProcessingData*>* getNodeProcessingDataListFromNode(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>* existingList = nullptr);
					QList<CRedlandNodeProcessingData*>* getNodeProcessingDataListFromNode(librdf_node* node, function<CRedlandNodeProcessingData*(librdf_node*)> resolvingFunc, QList<CRedlandNodeProcessingData*>* existingList);
					QList<CRedlandNodeProcessingData*>* getNodeProcessingDataListFromNodeWithNewHandling(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CRedlandNodeProcessingData*>* existingList);

					QList<CRedlandNodeProcessingData*>* getRDFListElementNodesAsProcessingDataList(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>* existingList = nullptr);
					QList<CRedlandNodeProcessingData*>* getNodeProcessingDataListFromNodeStream(CRedlandNodeStream* nodeStream, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>* existingList = nullptr);

					QList<CRedlandNodeProcessingData*>* getRDFListElementNodesAsProcessingDataListWithNewHandling(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CRedlandNodeProcessingData*>* existingList = nullptr);
					QList<CRedlandNodeProcessingData*>* getNodeProcessingDataListFromNodeStreamWithNewHandling(CRedlandNodeStream* nodeStream, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CRedlandNodeProcessingData*>* existingList = nullptr);

					QList<CRedlandNodeProcessingData*>* getNodeProcessingDataListFromNodeStream(CRedlandNodeStream* nodeStream, function<CRedlandNodeProcessingData*(librdf_node*)> resolvingFunc, QList<CRedlandNodeProcessingData*>* existingList = nullptr);

					CRedlandNodeStream* getOWLObjectInverseOfPredecessorNodesStream(librdf_node* currentNode);


					CRedlandNodeStream* getOWLIntersectionOfSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLUnionOfSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLComplementOfSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLOneOfSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLDatatypeComplementOfSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLOnDatatypeSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLWithRestrictionSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLOnPropertySuccessorNodesStream(librdf_node* currentNode);

					CRedlandNodeStream* getOWLSomeValuesFromSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLAllValuesFromSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLHasValueSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLHasSelfSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLMinCardinalitySuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLMaxCardinalitySuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLCardinalitySuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLMinQualifiedCardinalitySuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLMaxQualifiedCardinalitySuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLQualifiedCardinalitySuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLOnDataRangeSuccessorNodesStream(librdf_node* currentNode);
					CRedlandNodeStream* getOWLOnClassSuccessorNodesStream(librdf_node* currentNode);





					CRedlandNodeStream* getOWLClassInstanceNodesStream();
					CRedlandNodeStream* getOWLObjectPropertyInstanceNodesStream();
					CRedlandNodeStream* getOWLDatatypePropertyInstanceNodesStream();
					CRedlandNodeStream* getRDFSDatatypeInstanceNodesStream();
					CRedlandNodeStream* getOWLNamedIndividualInstanceNodesStream();
					CRedlandNodeStream* getOWLRestrictionInstanceNodesStream();


					CRedlandNodeStream* getOWLAllDisjointClassesInstanceNodesStream();
					CRedlandNodeStream* getOWLAllDisjointPropertiesInstanceNodesStream();
					CRedlandNodeStream* getOWLAllDifferentInstanceNodesStream();
					CRedlandNodeStream* getOWLAllNegativePropertyAssertionInstanceNodesStream();


					CRedlandNodeStream* getOWLFunctionalPropertyInstanceNodesStream();
					CRedlandNodeStream* getOWLInverseFunctionalPropertyInstanceNodesStream();
					CRedlandNodeStream* getOWLReflexivePropertyInstanceNodesStream();
					CRedlandNodeStream* getOWLIrreflexivePropertyInstanceNodesStream();
					CRedlandNodeStream* getOWLSymmetricPropertyInstanceNodesStream();
					CRedlandNodeStream* getOWLAsymmetricPropertyInstanceNodesStream();
					CRedlandNodeStream* getOWLTransetivePropertyInstanceNodesStream();



					librdf_statement* getAdaptedPartialFilteringStatement(librdf_node* subjectFilterNode, librdf_node* predicateFilterNode, librdf_node* objectFilterNode, librdf_statement*& partialFilteringStatement);


					CRedlandTripleStream* getPartialStatementFilteredTripleStream(librdf_statement* partialFilteringStatement);
					librdf_statement* initPartialFilteringStatement(const char* subjectFilter, const char* predicateFilter, const char* objectFilter, librdf_statement*& partialFilteringStatement);
					librdf_node* initPredicateIndexMapping(const char* predicateUri, cint64 predicateIndex, QHash<cint64, librdf_node*>* predicateIndexUriNodeHash, QHash<CRedlandNodeHasher, cint64>* predicateNodeIndexHash);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* initTripleDataProcessing(CRedlandStoredTriplesData* redlandTriplesData);

					CRedlandTripleStream* createRedlandTripleStreamWrapper();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* releaseRedlandTripleStreamWrapper(CRedlandTripleStream* streamWrapper);

					CRedlandNodeStream* createRedlandNodeStreamWrapper();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* releaseRedlandNodeStreamWrapper(CRedlandNodeStream* streamWrapper);

					bool collectClassExpressionOnPropertyDependenciesReturnPresence(CRedlandNodeProcessingData* processingData, bool* dataPropertyFlag, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>** nodeDataHash = nullptr);
					QList<CRedlandNodeProcessingData*>* getNodeProcessingDataListFromObjectORDataPropertyNode(librdf_node* node, bool* dataPropertyFlag = nullptr, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>** nodeDataHash = nullptr, QList<CRedlandNodeProcessingData*>* existingList = nullptr);

					bool checkHasBooleanTrueLiteralValue(librdf_node* node);
					cint64 getNonNegativeNumberFromLiteralValue(librdf_node* node);

					CConcreteOntologyRedlandTriplesDataExpressionMapper* clearTripleDataProcessing();
					CConcreteOntologyRedlandTriplesDataExpressionMapper* clearIndexUriHash(QHash<cint64, librdf_node*>& indexUriNodeHash);
					CConcreteOntologyRedlandTriplesDataExpressionMapper* clearPartialFilteringStatement(librdf_statement*& partialFilteringStatement);

					librdf_node* initDatatypeMapping(CRedlandStoredTriplesData* redlandTriplesData, const char* datatypeIri);

				// protected variables
				protected:
					CRedlandTripleStream* mFreeTripleStreamWrappers = nullptr;
					CRedlandNodeStream* mFreeNodeStreamWrappers = nullptr;

					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfOwlClass = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfOwlRestriction = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfOwlObjectProperty = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfOwlDataProperty = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfRDFSDatatype = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfOwlNamedIndividual = nullptr;

					librdf_statement* mPartialFilteringStatementForRDFFirstSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForRDFRestSuccessors = nullptr;

					librdf_statement* mPartialFilteringStatementForOWLComplementOfSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLOneOfSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLDatatypeComplementOfSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLOnDatatypeSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLWithRestrictionSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLFacets = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLFacetSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLOnPropertySuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLSomeValuesFromSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLAllValuesFromSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLHasValueSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLHasSelfSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLMinCardinalitySuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLMaxCardinalitySuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLCardinalitySuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLMinQualifiedCardinalitySuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLMaxQualifiedCardinalitySuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLQualifiedCardinalitySuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLOnDataRangeSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLOnClassSuccessors = nullptr;

					librdf_statement* mPartialFilteringStatementForOWLObjectInverseOfPredecessors = nullptr;

					librdf_statement* mPartialFilteringStatementForOWLIntersectionOfSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLUnionOfSuccessors = nullptr;

					librdf_statement* mPartialFilteringStatementForOWLDataRanges = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLClasses = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLAxioms = nullptr;



					librdf_statement* mPartialFilteringStatementForOWLMembers = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLDistinctMembers = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfAllDisjointClasses = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfAllDisjointProperties = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfAllDifferent = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfNegativePropertyAssertion = nullptr;


					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfFunctionalProperty = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfInverseFunctionalProperty = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfReflexiveProperty = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfIrreflexiveProperty = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOSymmetricProperty = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfAsymmetricProperty = nullptr;
					librdf_statement* mPartialFilteringStatementForAllRDFTypeOfTransetiveProperty = nullptr;




					librdf_statement* mPartialFilteringStatementForOWLSourceIndividualSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLTargetIndividualSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLTargetValueSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLAssertionPropertySuccessors = nullptr;

					librdf_statement* mPartialFilteringStatementForOWLAllRDFTypePredecessors = nullptr;

					librdf_statement* mPartialFilteringStatementForOWLPropertyAssertions = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLDifferentIndividuals = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLSameIndividuals = nullptr;






					librdf_statement* mPartialFilteringStatementForRDFSSubPropertySuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLPropertyChainAxiomSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLEquivalentPropertySuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLPropertyDisjointWithSuccessors = nullptr;

					librdf_statement* mPartialFilteringStatementForRDFSRangeSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForRDFSDomainSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLInverseOfSuccessors = nullptr;


					librdf_statement* mPartialFilteringStatementForRDFSSubClassOfSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLEquivalentClassesSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLDisjointClassesSuccessors = nullptr;
					librdf_statement* mPartialFilteringStatementForOWLDisjointUnionSuccessors = nullptr;






					librdf_node* mRDFNilNode = nullptr;
					librdf_uri* mBooleanDatatypeUri = nullptr;


					CRedlandStoredTriplesData* mRedlandTriplesData;

					QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*> mClassNodeIdentifierDataHash;
					QList<CRedlandNodeProcessingData*> mClassNodeHandlingList;

					QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*> mObjectPropertyNodeIdentifierDataHash;
					QList<CRedlandNodeProcessingData*> mObjectPropertyNodeHandlingList;

					QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*> mDataPropertyNodeIdentifierDataHash;
					QList<CRedlandNodeProcessingData*> mDataPropertyNodeHandlingList;

					QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*> mDatatypeNodeIdentifierDataHash;
					QList<CRedlandNodeProcessingData*> mDatatypeNodeHandlingList;

					QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*> mNamedIndividualNodeIdentifierDataHash;
					QList<CRedlandNodeProcessingData*> mNamedIndividualNodeHandlingList;

					QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*> mDataLiteralNodeIdentifierDataHash;
					QList<CRedlandNodeProcessingData*> mDataFacetRestrictionHandlingContainerList;

					QHash<CRedlandNodeHasher, cint64> mDataRangePredicateIndexHash;
					QHash<cint64, librdf_node*> mDataRangePredicateIndexUriNodeHash;

					QHash<CRedlandNodeHasher, cint64> mClassPredicateIndexHash;
					QHash<cint64, librdf_node*> mClassPredicateIndexUriNodeHash;

					QHash<CRedlandNodeHasher, cint64> mClassBasedAxiomPredicateIndexHash;
					QHash<cint64, librdf_node*> mClassBasedAxiomPredicateIndexUriNodeHash;

					QHash<CRedlandNodeHasher, cint64> mDatatypeBasedAxiomPredicateIndexHash;
					QHash<cint64, librdf_node*> mDatatypeBasedAxiomPredicateIndexUriNodeHash;

					QHash<CRedlandNodeHasher, cint64> mPropertyBasedAxiomPredicateIndexHash;
					QHash<cint64, librdf_node*> mPropertyBasedAxiomPredicateIndexUriNodeHash;

					QHash<CRedlandNodeHasher, cint64> mPropertyBasedAxiomObjectIndexHash;
					QHash<cint64, librdf_node*> mPropertyBasedAxiomObjectIndexUriNodeHash;


					QHash<const char*, librdf_node*> mFacetUriNodeHash;


					static const cint64 mOWLIntersectionOfPredicateIndex = 0;
					static const cint64 mOWLUnionOfPredicateIndex = 1;
					static const cint64 mOWLDatatypeComplementOfPredicateIndex = 2;
					static const cint64 mOWLOneOfPredicateIndex = 3;
					static const cint64 mOWLOnDatatypePredicateIndex = 4;
					static const cint64 mOWLWithRestrictionPredicateIndex = 5;

					static const cint64 mOWLComplementOfPredicateIndex = 6;
					static const cint64 mOWLOnPropertyPredicateIndex = 7;
					static const cint64 mOWLSomeValuesFromPredicateIndex = 8;
					static const cint64 mOWLAllValuesFromPredicateIndex = 9;
					static const cint64 mOWLHasValuePredicateIndex = 10;
					static const cint64 mOWLHasSelfPredicateIndex = 11;
					static const cint64 mOWLOnClassPredicateIndex = 12;
					static const cint64 mOWLMinQualifiedCardinalityPredicateIndex = 13;
					static const cint64 mOWLMaxQualifiedCardinalityPredicateIndex = 14;
					static const cint64 mOWLQualifiedCardinalityPredicateIndex = 15;

					static const cint64 mOWLMinCardinalityPredicateIndex = 16;
					static const cint64 mOWLMaxCardinalityPredicateIndex = 17;
					static const cint64 mOWLCardinalityPredicateIndex = 18;
					static const cint64 mOWLOnDataRangePredicateIndex = 19;



					static const cint64 mOWLClassAxiomSubClassOfPredicateIndex = 1;
					static const cint64 mOWLClassAxiomEquvialentClassPredicateIndex = 2;
					static const cint64 mOWLClassAxiomDisjointWithPredicateIndex = 3;
					static const cint64 mOWLClassAxiomDisjointUnionOfPredicateIndex = 4;

					static const cint64 mOWLPredicateBasedAxiomSubPropertyOfPredicateIndex = 1;
					static const cint64 mOWLPredicateBasedAxiomPropertyChainAxiomPredicateIndex = 2;
					static const cint64 mOWLPredicateBasedAxiomEquivalentPropertyPredicateIndex = 3;
					static const cint64 mOWLPredicateBasedAxiomPropertyDisjointWithPredicateIndex = 4;
					static const cint64 mOWLPredicateBasedAxiomDomainPredicateIndex = 5;
					static const cint64 mOWLPredicateBasedAxiomRangePredicateIndex = 6;
					static const cint64 mOWLPredicateBasedAxiomInverseOfPredicateIndex = 7;
					static const cint64 mRDFPredicateBasedAxiomTypeOfPredicateIndex = 8;

					static const cint64 mOWLPredicateBasedAxiomTypeFunctionalPropertyObjectIndex = 1;
					static const cint64 mOWLPredicateBasedAxiomTypeInverseFunctionalPropertyObjectIndex = 2;
					static const cint64 mOWLPredicateBasedAxiomTypeReflexivePropertyObjectIndex = 3;
					static const cint64 mOWLPredicateBasedAxiomTypeIrreflexivePropertyObjectIndex = 4;
					static const cint64 mOWLPredicateBasedAxiomTypeSymmetricPropertyObjectIndex = 5;
					static const cint64 mOWLPredicateBasedAxiomTypeAsymmetricPropertyObjectIndex = 6;
					static const cint64 mOWLPredicateBasedAxiomTypeTransitivePropertyObjectIndex = 7;


					static const cint64 mOWLPredicateMaxIndex = 20;
					static const cint64 mOWLDataRangePredicateMaxIndex = 6;
					static const cint64 mOWLClassPredicateMaxIndex = 20;



					bool mConfSuccessorRetrieval = true;
					bool mConfExtractSimpleABoxAssertions = false;


			};







		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYREDLANDTRIPLESDATAEXPRESSIONMAPPER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
