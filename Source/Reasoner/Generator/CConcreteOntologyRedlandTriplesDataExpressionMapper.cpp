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

#include "CConcreteOntologyRedlandTriplesDataExpressionMapper.h"


namespace Konclude {

	namespace Reasoner {

		namespace Generator {


			int CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData::nextId = 0;


			CConcreteOntologyRedlandTriplesDataExpressionMapper::CConcreteOntologyRedlandTriplesDataExpressionMapper(COntologyBuilder* ontologyBuilder) : CConcreteOntologyTriplesExpressionMapper(ontologyBuilder) {

				mConfSuccessorRetrieval = true;
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::~CConcreteOntologyRedlandTriplesDataExpressionMapper() {
			}


			bool CConcreteOntologyRedlandTriplesDataExpressionMapper::mapTriples(CConcreteOntology* updateConcreteOntology, COntologyTriplesData* ontologyTripleData) {

				bool newTriplesMapped = false;
				for (CTriplesData* tripleData : *ontologyTripleData->getAllTriplesData()) {
					CRedlandStoredTriplesData* redlandTriplesData = dynamic_cast<CRedlandStoredTriplesData*>(tripleData);

					librdf_model* model = redlandTriplesData->getRedlandIndexedModel();
					librdf_statement *partial_statement = librdf_new_statement(redlandTriplesData->getRedlandWorld());

					librdf_stream* stream = librdf_model_find_statements(model, partial_statement);
					if (!stream) {
						return false;
					} else {
						newTriplesMapped = true;

						initTripleDataProcessing(redlandTriplesData);

						buildDeclarations();

						buildObjectPropertyExpressions();
						buildDataRangeExpressions();
						buildClassExpressions();

						buildClassBasedAxioms();
						buildObjectPropertyBasedAxioms();
						buildDataPropertyBasedAxioms();
						buildDatatypeBasedAxioms();

						buildSeparateNodeBasedAxioms();

						buildComplexABoxAxioms();

						if (mConfExtractSimpleABoxAssertions) {
							buildSimpleABoxAxioms();
						}

						// do clean up
						clearTripleDataProcessing();

					}
				}

				return newTriplesMapped;
			}




			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildSimpleABoxAxioms() {

				QList<CAxiomExpression*> axiomExpressionList;

				for (CRedlandNodeProcessingData* nextProcessingData : mClassNodeHandlingList) {
					if (nextProcessingData->mExpression) {
						if (librdf_node_is_resource(nextProcessingData->mRedlandNode)) {
							QList<CRedlandNodeProcessingData*>* processingDataList = getNodeProcessingDataListFromNodeStreamWithNewHandling(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nullptr, nullptr, nextProcessingData->mRedlandNode, mPartialFilteringStatementForOWLAllRDFTypePredecessors)), 0), mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { 
								return createNamedIndividualProcessingData(node); 
							});
							if (processingDataList && !processingDataList->isEmpty()) {
								for (CRedlandNodeProcessingData* processingData : *processingDataList) {
									CAxiomExpression* axiomExpression = mOntologyBuilder->getClassAssertion(processingData->mExpression, nextProcessingData->mExpression);
									if (axiomExpression) {
										axiomExpressionList.append(axiomExpression);
									}
								}
							}
							if (processingDataList) {
								delete processingDataList;
							}
						}
					}
				}


				for (CRedlandNodeProcessingData* nextProcessingData : mObjectPropertyNodeHandlingList) {
					if (nextProcessingData->mExpression) {
						collectBetweenIndividualsBasedAxiomExpression(nextProcessingData, [&](const QList<CBuildExpression*>& expressionList)->CAxiomExpression* { return mOntologyBuilder->getObjectPropertyAssertion(expressionList); }, mPartialFilteringStatementForOWLPropertyAssertions, axiomExpressionList);
					}
				}

				collectBetweenIndividualsBasedAxiomExpression([&](const QList<CBuildExpression*>& expressionList)->CAxiomExpression* { return mOntologyBuilder->getDifferentIndividuals(expressionList); }, mPartialFilteringStatementForOWLDifferentIndividuals, axiomExpressionList);
				collectBetweenIndividualsBasedAxiomExpression([&](const QList<CBuildExpression*>& expressionList)->CAxiomExpression* { return mOntologyBuilder->getSameIndividual(expressionList); }, mPartialFilteringStatementForOWLSameIndividuals, axiomExpressionList);


				for (CRedlandNodeProcessingData* nextProcessingData : mDataPropertyNodeHandlingList) {
					if (nextProcessingData->mExpression) {

						releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nullptr, nextProcessingData->mRedlandNode, nullptr, mPartialFilteringStatementForOWLPropertyAssertions))->forEach([&](librdf_statement* statement) {
							librdf_node* subject = librdf_statement_get_subject(statement);
							librdf_node* object = librdf_statement_get_object(statement);

							if (librdf_node_is_literal(object)) {
								QList<CRedlandNodeProcessingData*>* processingDataList = nullptr;
								// TODO: individuals may be anonymous individuals
								processingDataList = getNodeProcessingDataListFromNodeWithNewHandling(subject, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, processingDataList);
								processingDataList = getNodeProcessingDataListFromNodeWithNewHandling(object, mDataLiteralNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createLiteralValueProcessingData(node); }, processingDataList);
								if (processingDataList) {
									processingDataList->append(nextProcessingData);
									QList<CBuildExpression*> expressionList;
									collectBuildExpressionsFromProcessingDataList(*processingDataList, expressionList);
									CAxiomExpression* axiomExpression = mOntologyBuilder->getDataPropertyAssertion(expressionList);
									if (axiomExpression) {
										axiomExpressionList.append(axiomExpression);
									}
									delete processingDataList;
								}
							}
						}));

					}
				}

				for (CAxiomExpression* axiomExpression : axiomExpressionList) {
					mOntologyBuilder->tellOntologyAxiom(axiomExpression);
				}

				return this;
			}




			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectBetweenIndividualsBasedAxiomExpression(CRedlandNodeProcessingData* nextProcessingData, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunc, librdf_statement* filteringStatement, QList<CAxiomExpression*>& axiomList) {
				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nullptr, nextProcessingData->mRedlandNode, nullptr, filteringStatement))->forEach([&](librdf_statement* statement) {
					librdf_node* subject = librdf_statement_get_subject(statement);
					librdf_node* object = librdf_statement_get_object(statement);

					QList<CRedlandNodeProcessingData*>* processingDataList = nullptr;
					// TODO: individuals may be anonymous individuals
					processingDataList = getNodeProcessingDataListFromNodeWithNewHandling(subject, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, processingDataList);
					processingDataList = getNodeProcessingDataListFromNodeWithNewHandling(object, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, processingDataList);
					if (processingDataList) {
						processingDataList->append(nextProcessingData);
						QList<CBuildExpression*> expressionList;
						collectBuildExpressionsFromProcessingDataList(*processingDataList, expressionList);
						CAxiomExpression* axiomExpression = buildFunc(expressionList);
						if (axiomExpression) {
							axiomList.append(axiomExpression);
						}
						delete processingDataList;
					}
				}));

				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectBetweenIndividualsBasedAxiomExpression(function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunc, librdf_statement* filteringStatement, QList<CAxiomExpression*>& axiomList) {
				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nullptr, nullptr, nullptr, filteringStatement))->forEach([&](librdf_statement* statement) {
					librdf_node* subject = librdf_statement_get_subject(statement);
					librdf_node* object = librdf_statement_get_object(statement);

					QList<CRedlandNodeProcessingData*>* processingDataList = nullptr;
					// TODO: individuals may be anonymous individuals
					processingDataList = getNodeProcessingDataListFromNodeWithNewHandling(subject, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, processingDataList);
					processingDataList = getNodeProcessingDataListFromNodeWithNewHandling(object, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, processingDataList);
					if (processingDataList) {
						QList<CBuildExpression*> expressionList;
						collectBuildExpressionsFromProcessingDataList(*processingDataList, expressionList);
						CAxiomExpression* axiomExpression = buildFunc(expressionList);
						if (axiomExpression) {
							axiomList.append(axiomExpression);
						}
						delete processingDataList;
					}
				}));

				return this;
			}







			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildComplexABoxAxioms() {

				QList<CAxiomExpression*> axiomExpressionList;

				for (CRedlandNodeProcessingData* nextProcessingData : mClassNodeHandlingList) {
					if (nextProcessingData->mExpression) {
						if (librdf_node_is_blank(nextProcessingData->mRedlandNode)) {
							QList<CRedlandNodeProcessingData*>* processingDataList = getNodeProcessingDataListFromNodeStreamWithNewHandling(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nullptr, nullptr, nextProcessingData->mRedlandNode, mPartialFilteringStatementForOWLAllRDFTypePredecessors)), 0), mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); });
							if (processingDataList && !processingDataList->isEmpty()) {
								for (CRedlandNodeProcessingData* processingData : *processingDataList) {
									QList<CBuildExpression*> expressionList;
									expressionList.append(nextProcessingData->mExpression);
									expressionList.append(processingData->mExpression);
									collectBuildExpressionsFromProcessingDataList(*processingDataList, expressionList);
									CAxiomExpression* axiomExpression = mOntologyBuilder->getClassAssertion(expressionList);
									if (axiomExpression) {
										axiomExpressionList.append(axiomExpression);
									}
									delete processingDataList;
								}
							}

						}
					}
				}
				// complex data range assertions
				for (CRedlandNodeProcessingData* nextProcessingData : mDataPropertyNodeHandlingList) {
					if (nextProcessingData->mExpression) {

						releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nullptr, nextProcessingData->mRedlandNode, nullptr, mPartialFilteringStatementForOWLPropertyAssertions))->forEach([&](librdf_statement* statement) {
							librdf_node* subject = librdf_statement_get_subject(statement);
							librdf_node* object = librdf_statement_get_object(statement);

							if (!librdf_node_is_literal(object)) {
								QList<CRedlandNodeProcessingData*>* processingDataList = nullptr;
								// TODO: individuals may be anonymous individuals
								processingDataList = getNodeProcessingDataListFromNodeWithNewHandling(subject, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, processingDataList);
								processingDataList = getNodeProcessingDataListFromNode(object, mDatatypeNodeIdentifierDataHash, processingDataList);
								if (processingDataList) {
									processingDataList->append(nextProcessingData);
									QList<CBuildExpression*> expressionList;
									collectBuildExpressionsFromProcessingDataList(*processingDataList, expressionList);
									CAxiomExpression* axiomExpression = mOntologyBuilder->getDataPropertyAssertion(expressionList);
									if (axiomExpression) {
										axiomExpressionList.append(axiomExpression);
									}
									delete processingDataList;
								}
							}
						}));

					}
				}


				for (CAxiomExpression* axiomExpression : axiomExpressionList) {
					mOntologyBuilder->tellOntologyAxiom(axiomExpression);
				}

				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildDeclarations() {


				releaseRedlandNodeStreamWrapper(getOWLClassInstanceNodesStream()->forEach([&](librdf_node* node) {
					handleDeclaration(node, [&](const QString &uri)->CBuildExpression* { 
						return mOntologyBuilder->getClass(uri); 
					}, mClassNodeIdentifierDataHash, mClassNodeHandlingList);
				}));

				releaseRedlandNodeStreamWrapper(getOWLRestrictionInstanceNodesStream()->forEach([&](librdf_node* node) {
					handleClassExpressionRestriction(node);
				}));

				releaseRedlandNodeStreamWrapper(getOWLObjectPropertyInstanceNodesStream()->forEach([&](librdf_node* node) {
					handleDeclaration(node, [&](const QString &uri)->CBuildExpression* { return mOntologyBuilder->getObjectProberty(uri); }, mObjectPropertyNodeIdentifierDataHash, mObjectPropertyNodeHandlingList);
				}));

				releaseRedlandNodeStreamWrapper(getOWLDatatypePropertyInstanceNodesStream()->forEach([&](librdf_node* node) {
					handleDeclaration(node, [&](const QString &uri)->CBuildExpression* { return mOntologyBuilder->getDataProberty(uri); }, mDataPropertyNodeIdentifierDataHash, mDataPropertyNodeHandlingList);
				}));

				releaseRedlandNodeStreamWrapper(getRDFSDatatypeInstanceNodesStream()->forEach([&](librdf_node* node) {
					handleDeclaration(node, [&](const QString &uri)->CBuildExpression* { return mOntologyBuilder->getDatatype(uri); }, mDatatypeNodeIdentifierDataHash, mDatatypeNodeHandlingList);
				}));

				if (mConfExtractSimpleABoxAssertions) {
					releaseRedlandNodeStreamWrapper(getOWLNamedIndividualInstanceNodesStream()->forEach([&](librdf_node* node) {
						handleDeclaration(node, [&](const QString &uri)->CBuildExpression* { return mOntologyBuilder->getNamedIndividual(uri); }, mNamedIndividualNodeIdentifierDataHash, mNamedIndividualNodeHandlingList);
					}));
				}
				return this;
			}




			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildSeparateNodeBasedAxioms() {
				QList<CAxiomExpression*> axiomExpressionList;
				releaseRedlandNodeStreamWrapper(getOWLAllDisjointClassesInstanceNodesStream()->forEach([&](librdf_node* node) {
					bool processed = false;
					if (!processed) {
						buildAxiomExpressionForMemberRDFNodeList(node, mPartialFilteringStatementForOWLMembers, mClassNodeIdentifierDataHash, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { 
							CAxiomExpression* axiomExp = mOntologyBuilder->getDisjointClasses(expressions);
							processed |= axiomExp != nullptr;
							return axiomExp;
						}, axiomExpressionList);
					}
				}));

				releaseRedlandNodeStreamWrapper(getOWLAllDisjointPropertiesInstanceNodesStream()->forEach([&](librdf_node* node) {
					bool processed = false;
					if (!processed) {
						buildAxiomExpressionForObjectORDataPropertyMemberRDFNodeList(node, mPartialFilteringStatementForOWLMembers, [&](const QList<CBuildExpression*>& expressions, bool hasDataProperties, bool hasObjectProperties)->CAxiomExpression* { 
							CAxiomExpression* axiomExp = nullptr;
							if (!hasDataProperties && hasObjectProperties) {
								axiomExp = mOntologyBuilder->getDisjointObjectProperties(expressions);
							} else if (hasDataProperties && !hasObjectProperties) {
								axiomExp = mOntologyBuilder->getDisjointDataProperties(expressions);
							}
							processed |= axiomExp != nullptr;
							return axiomExp;
						}, axiomExpressionList);
					}
				}));

				releaseRedlandNodeStreamWrapper(getOWLAllDifferentInstanceNodesStream()->forEach([&](librdf_node* node) {
					bool processed = false;
					if (!processed) {
						buildAxiomExpressionForMemberRDFNodeListWithNewHandling(node, mPartialFilteringStatementForOWLMembers, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* {
							if (expressions.size() > 0) {
								CAxiomExpression* axiomExp = mOntologyBuilder->getDifferentIndividuals(expressions);
								processed |= axiomExp != nullptr;
								return axiomExp;
							}
							return nullptr;
						}, axiomExpressionList);
					}
					if (!processed) {
						buildAxiomExpressionForMemberRDFNodeListWithNewHandling(node, mPartialFilteringStatementForOWLDistinctMembers, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* {
							if (expressions.size() > 0) {
								CAxiomExpression* axiomExp = mOntologyBuilder->getDifferentIndividuals(expressions);
								processed |= axiomExp != nullptr;
								return axiomExp;
							}
							return nullptr;
						}, axiomExpressionList);
					}
				}));

				releaseRedlandNodeStreamWrapper(getOWLAllNegativePropertyAssertionInstanceNodesStream()->forEach([&](librdf_node* node) {
					bool processed = false;
					if (!processed) {
						QList<CRedlandNodeProcessingData*>* sourcendividualProcessingDataList = getNodeProcessingDataListFromNodeStreamWithNewHandling(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(node, nullptr, nullptr, mPartialFilteringStatementForOWLSourceIndividualSuccessors)), 2), mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); });
						if (sourcendividualProcessingDataList) {

							if (!processed) {
								QList<CRedlandNodeProcessingData*>* targetValueLiteralProcessingDataList = getNodeProcessingDataListFromNodeStreamWithNewHandling(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(node, nullptr, nullptr, mPartialFilteringStatementForOWLTargetValueSuccessors)), 2), mDataLiteralNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createLiteralValueProcessingData(node); });
								if (targetValueLiteralProcessingDataList) {

									QList<CRedlandNodeProcessingData*>* propertyProcessingDataList = getNodeProcessingDataListFromNodeStream(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(node, nullptr, nullptr, mPartialFilteringStatementForOWLAssertionPropertySuccessors)), 2), mObjectPropertyNodeIdentifierDataHash);
									if (propertyProcessingDataList) {
										QList<CBuildExpression*> expressionList;
										collectBuildExpressionsFromProcessingDataList(*sourcendividualProcessingDataList, expressionList);
										collectBuildExpressionsFromProcessingDataList(*targetValueLiteralProcessingDataList, expressionList);
										collectBuildExpressionsFromProcessingDataList(*propertyProcessingDataList, expressionList);
										CAxiomExpression* axiomExpression = mOntologyBuilder->getNegativeObjectPropertyAssertion(expressionList);
										if (axiomExpression) {
											processed = true;
											axiomExpressionList.append(axiomExpression);
										}
										delete propertyProcessingDataList;
									}

									delete targetValueLiteralProcessingDataList;
								}
							}


							if (!processed) {
								QList<CRedlandNodeProcessingData*>* targetIndividualProcessingDataList = getNodeProcessingDataListFromNodeStreamWithNewHandling(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(node, nullptr, nullptr, mPartialFilteringStatementForOWLTargetIndividualSuccessors)), 2), mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); });
								if (targetIndividualProcessingDataList) {
									QList<CRedlandNodeProcessingData*>* propertyProcessingDataList = getNodeProcessingDataListFromNodeStream(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(node, nullptr, nullptr, mPartialFilteringStatementForOWLAssertionPropertySuccessors)), 2), mDataPropertyNodeIdentifierDataHash);
									if (propertyProcessingDataList) {
										QList<CBuildExpression*> expressionList;
										collectBuildExpressionsFromProcessingDataList(*sourcendividualProcessingDataList, expressionList);
										collectBuildExpressionsFromProcessingDataList(*targetIndividualProcessingDataList, expressionList);
										collectBuildExpressionsFromProcessingDataList(*propertyProcessingDataList, expressionList);
										CAxiomExpression* axiomExpression = mOntologyBuilder->getNegativeDataPropertyAssertion(expressionList);
										if (axiomExpression) {
											processed = true;
											axiomExpressionList.append(axiomExpression);
										}
										delete propertyProcessingDataList;
									}

									delete targetIndividualProcessingDataList;
								}
							}
							delete sourcendividualProcessingDataList;
						}
					}

				}));
				for (CAxiomExpression* axiomExpression : axiomExpressionList) {
					mOntologyBuilder->tellOntologyAxiom(axiomExpression);
				}
				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectBuildExpressionsFromProcessingDataList(QList<CRedlandNodeProcessingData*>& processingDataList, QList<CBuildExpression*>& expressionList) {
				for (CRedlandNodeProcessingData* processingData : processingDataList) {
					if (processingData->mExpression) {
						expressionList.append(processingData->mExpression);
					}
				}
				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildAxiomExpressionForMemberRDFNodeList(librdf_node* node, librdf_statement* partialFilteringStatementForOWLMembers, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions) {
				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(node, nullptr, nullptr, partialFilteringStatementForOWLMembers))->forEach([&](librdf_statement* statement) {
					librdf_node* object = librdf_statement_get_object(statement);

					QList<CBuildExpression*> expressionList;
					buildAxiomExpressionFromRDFNodeList(object, nodeIdentifierDataHash, expressionList, buildFunction, axiomExpressions);

				}));
				return this;
			}




			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildAxiomExpressionForMemberRDFNodeListWithNewHandling(librdf_node* node, librdf_statement* partialFilteringStatementForOWLMembers, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions) {
				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(node, nullptr, nullptr, partialFilteringStatementForOWLMembers))->forEach([&](librdf_statement* statement) {
					librdf_node* object = librdf_statement_get_object(statement);

					QList<CBuildExpression*> expressionList;
					buildAxiomExpressionFromRDFNodeListWithNewHandling(object, nodeIdentifierDataHash, newHandlingFunc, expressionList, buildFunction, axiomExpressions);

				}));
				return this;
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildAxiomExpressionForObjectORDataPropertyMemberRDFNodeList(librdf_node* node, librdf_statement* partialFilteringStatementForOWLMembers, function<CAxiomExpression*(const QList<CBuildExpression*>&, bool hasDataProperties, bool hasObjectProperties)> buildFunction, QList<CAxiomExpression*>& axiomExpressions) {
				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(node, nullptr, nullptr, partialFilteringStatementForOWLMembers))->forEach([&](librdf_statement* statement) {
					librdf_node* object = librdf_statement_get_object(statement);

					bool dataPropertyNodeUsed = false;
					bool objectPropertyNodeUsed = false;

					QList<CBuildExpression*> expressionList;
					QList<librdf_node*>* nodeList = nullptr;
					nodeList = getOWLListElementNodesList(object, nodeList);
					if (nodeList) {
						for (librdf_node* listElementNode : *nodeList) {
							CRedlandNodeHasher hasher(listElementNode);
							CRedlandNodeProcessingData* processingData = mObjectPropertyNodeIdentifierDataHash.value(hasher);
							if (!processingData) {
								processingData = mDataPropertyNodeIdentifierDataHash.value(hasher);
								if (processingData) {
									dataPropertyNodeUsed = true;
								}
							} else {
								objectPropertyNodeUsed = true;
							}

							if (processingData && processingData->mExpression) {
								expressionList.append(processingData->mExpression);
							}
						}

						CAxiomExpression* axiomExp = buildFunction(expressionList, dataPropertyNodeUsed, objectPropertyNodeUsed);
						if (axiomExp) {
							axiomExpressions.append(axiomExp);
						}

						delete nodeList;
					}

				}));
				return this;
			}







			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildDatatypeBasedAxioms() {
				if (mConfSuccessorRetrieval) {
					buildAxioms(&mDatatypeNodeHandlingList, [&](CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) { collectDatatypeBasedAxiomExpressionsBySuccessorRetrieval(processingData, axiomExpressions); });
				} else {
					buildAxioms(&mDatatypeNodeHandlingList, [&](CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) { collectDatatypeBasedAxiomExpressions(processingData, axiomExpressions); });
				}
				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildClassBasedAxioms() {
				if (mConfSuccessorRetrieval) {
					buildAxioms(&mClassNodeHandlingList, [&](CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) { collectClassBasedAxiomExpressionsBySuccessorRetrieval(processingData, axiomExpressions); });
				} else {
					buildAxioms(&mClassNodeHandlingList, [&](CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) { collectClassBasedAxiomExpressions(processingData, axiomExpressions); });
				}
				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildObjectPropertyBasedAxioms() {
				if (mConfSuccessorRetrieval) {
					buildAxioms(&mObjectPropertyNodeHandlingList, [&](CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) { collectObjectPropertyBasedAxiomExpressionsBySuccessorRetrieval(processingData, axiomExpressions); });
					QList<CAxiomExpression*> axiomExpressions;
					collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval(axiomExpressions);
					for (CAxiomExpression* axiomExpression : axiomExpressions) {
						mOntologyBuilder->tellOntologyAxiom(axiomExpression);
					}
				} else {
					buildAxioms(&mObjectPropertyNodeHandlingList, [&](CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) { collectObjectPropertyBasedAxiomExpressions(processingData, axiomExpressions); });
				}
				return this;
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildDataPropertyBasedAxioms() {
				if (mConfSuccessorRetrieval) {
					buildAxioms(&mDataPropertyNodeHandlingList, [&](CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) { collectDataPropertyBasedAxiomExpressionsBySuccessorRetrieval(processingData, axiomExpressions); });
					QList<CAxiomExpression*> axiomExpressions;
					collectDataPropertyTypesAxiomExpressionsBySuccessorRetrieval(axiomExpressions);
					for (CAxiomExpression* axiomExpression : axiomExpressions) {
						mOntologyBuilder->tellOntologyAxiom(axiomExpression);
					}

				} else {
					buildAxioms(&mDataPropertyNodeHandlingList, [&](CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) { collectDataPropertyBasedAxiomExpressions(processingData, axiomExpressions); });
				}
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildAxioms(QList<CRedlandNodeProcessingData*>* nodeHandlingList, function<void(CRedlandNodeProcessingData*, QList<CAxiomExpression*>&)> dependenciesCollectionFunction) {

				QList<CAxiomExpression*> axiomExpressionList;

				for (CRedlandNodeProcessingData* nextProcessingData : *nodeHandlingList) {
					if (nextProcessingData->mExpression) {
						dependenciesCollectionFunction(nextProcessingData, axiomExpressionList);
					}
				}

				for (CAxiomExpression* axiomExpression : axiomExpressionList) {
					mOntologyBuilder->tellOntologyAxiom(axiomExpression);
				}
				return this;
			}





			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectClassBasedAxiomExpressions(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) {
				if (processingData->mExpression) {

					// TODO: if there are too many successor triples, then use the other method, which directly filters by the corresponding predicates
					// create/use forEachAsLong/forEachLimited
					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLAxioms))->forEach([&](librdf_statement* statement) {
						librdf_node* predicate = librdf_statement_get_predicate(statement);
						librdf_node* object = librdf_statement_get_object(statement);
						cint64 index = mClassBasedAxiomPredicateIndexHash.value(CRedlandNodeHasher(predicate), -1);
						if (index >= 0) {

							QList<CBuildExpression*> expressionList;
							expressionList.append(processingData->mExpression);

							switch (index) {
								case mOWLClassAxiomSubClassOfPredicateIndex: 
									buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getSubClassOf(expressions); }, axiomExpressions);
									break; 
								case mOWLClassAxiomEquvialentClassPredicateIndex:
									buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getEquivalentClasses(expressions); }, axiomExpressions);
									break;
								case mOWLClassAxiomDisjointWithPredicateIndex:
									buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDisjointClasses(expressions); }, axiomExpressions);
									break;
								case mOWLClassAxiomDisjointUnionOfPredicateIndex:
									buildAxiomExpressionFromRDFNodeList(object, mClassNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDisjointUnion(expressions); }, axiomExpressions);
									break;
								default:
									break;
							}

						}
					}));

				}
				return this;
			}




			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectClassBasedAxiomExpressionsBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) {

				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForRDFSSubClassOfSuccessors))->forEach([&](librdf_statement* statement) {
					librdf_node* object = librdf_statement_get_object(statement);
					buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getSubClassOf(expressions); }, axiomExpressions);
				}));

				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLEquivalentClassesSuccessors))->forEach([&](librdf_statement* statement) {
					librdf_node* object = librdf_statement_get_object(statement);
					buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getEquivalentClasses(expressions); }, axiomExpressions);
				}));

				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLDisjointClassesSuccessors))->forEach([&](librdf_statement* statement) {
					librdf_node* object = librdf_statement_get_object(statement);
					buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDisjointClasses(expressions); }, axiomExpressions);
				}));

				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLDisjointUnionSuccessors))->forEach([&](librdf_statement* statement) {
					librdf_node* object = librdf_statement_get_object(statement);
					buildAxiomExpressionFromRDFNodeList(object, mClassNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDisjointUnion(expressions); }, axiomExpressions);
				}));

				return this;
			}





			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectDatatypeBasedAxiomExpressions(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) {
				if (processingData->mExpression) {

					// TODO: if there are too many successor triples, then use the other method, which directly filters by the corresponding predicates
					// create/use forEachAsLong/forEachLimited
					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLAxioms))->forEach([&](librdf_statement* statement) {
						librdf_node* predicate = librdf_statement_get_predicate(statement);
						librdf_node* object = librdf_statement_get_object(statement);
						cint64 index = mDatatypeBasedAxiomPredicateIndexHash.value(CRedlandNodeHasher(predicate), -1);
						if (index >= 0) {

							QList<CBuildExpression*> expressionList;
							expressionList.append(processingData->mExpression);

							switch (index) {
								case mOWLClassAxiomEquvialentClassPredicateIndex:
									// TODO: build datatype definition axiom (may not yet available)
									break; 
								default:
									break;
							}

						}
					}));

				}
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectDatatypeBasedAxiomExpressionsBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) {
				// TODO: build datatype definition axiom (may not yet available)
				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectObjectPropertyBasedAxiomExpressions(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) {
				if (processingData->mExpression) {

					// TODO: if there are too many successor triples, then use the other method, which directly filters by the corresponding predicates
					// create/use forEachAsLong/forEachLimited
					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLAxioms))->forEach([&](librdf_statement* statement) {
						librdf_node* predicate = librdf_statement_get_predicate(statement);
						librdf_node* object = librdf_statement_get_object(statement);
						cint64 predicateIndex = mPropertyBasedAxiomPredicateIndexHash.value(CRedlandNodeHasher(predicate), -1);
						if (predicateIndex >= 0) {

							QList<CBuildExpression*> expressionList;
							expressionList.append(processingData->mExpression);

							switch (predicateIndex) {
							case mOWLPredicateBasedAxiomSubPropertyOfPredicateIndex:
								buildAxiomExpressionFromNode(object, mObjectPropertyNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getSubObjectPropertyOf(expressions); }, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomPropertyChainAxiomPredicateIndex:
								buildAxiomExpressionFromRDFNodeList(object, mObjectPropertyNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* {
									QList<CBuildExpression*> resortedExpressionList(expressions);
									if (resortedExpressionList.size() > 0) {
										CBuildExpression* superProperty = resortedExpressionList.takeFirst();
										resortedExpressionList.append(superProperty);
									}
									return mOntologyBuilder->getSubObjectPropertyOf(resortedExpressionList);
								}, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomEquivalentPropertyPredicateIndex:
								buildAxiomExpressionFromNode(object, mObjectPropertyNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getEquivalentObjectProperties(expressions); }, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomPropertyDisjointWithPredicateIndex:
								buildAxiomExpressionFromNode(object, mObjectPropertyNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDisjointObjectProperties(expressions); }, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomDomainPredicateIndex:
								buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getObjectPropertyDomainExpression(expressions); }, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomRangePredicateIndex:
								buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getObjectPropertyRangeExpression(expressions); }, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomInverseOfPredicateIndex:
								buildAxiomExpressionFromNode(object, mObjectPropertyNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getInverseObjectProperties(expressions); }, axiomExpressions);
								break;
							case mRDFPredicateBasedAxiomTypeOfPredicateIndex:
								cint64 objectIndex = mPropertyBasedAxiomObjectIndexHash.value(CRedlandNodeHasher(object), -1);
								if (objectIndex >= 0) {
									switch (objectIndex) {
									case mOWLPredicateBasedAxiomTypeFunctionalPropertyObjectIndex:
										axiomExpressions.append(mOntologyBuilder->getFunctionalObjectProperty(processingData->mExpression));
										break;
									case mOWLPredicateBasedAxiomTypeInverseFunctionalPropertyObjectIndex:
										axiomExpressions.append(mOntologyBuilder->getInverseFunctionalObjectProperty(processingData->mExpression));
										break;
									case mOWLPredicateBasedAxiomTypeReflexivePropertyObjectIndex:
										axiomExpressions.append(mOntologyBuilder->getReflexiveObjectProperty(processingData->mExpression));
										break;
									case mOWLPredicateBasedAxiomTypeIrreflexivePropertyObjectIndex:
										axiomExpressions.append(mOntologyBuilder->getIrreflexiveObjectProperty(processingData->mExpression));
										break;
									case mOWLPredicateBasedAxiomTypeSymmetricPropertyObjectIndex:
										axiomExpressions.append(mOntologyBuilder->getSymmetricObjectProperty(processingData->mExpression));
										break;
									case mOWLPredicateBasedAxiomTypeAsymmetricPropertyObjectIndex:
										axiomExpressions.append(mOntologyBuilder->getAsymmetricObjectProperty(processingData->mExpression));
										break;
									case mOWLPredicateBasedAxiomTypeTransitivePropertyObjectIndex:
										axiomExpressions.append(mOntologyBuilder->getTransetiveObjectProperty(processingData->mExpression));
										break;
									default:
										break;
									}
								}
								break;
							}

						}
					}));

				}
				return this;
			}





			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectDataPropertyTypesAxiomExpressionsBySuccessorRetrieval(QList<CAxiomExpression*>& axiomExpressions) {
				releaseRedlandNodeStreamWrapper(getOWLFunctionalPropertyInstanceNodesStream()->forEach([&](librdf_node* node) {
					QList<CRedlandNodeProcessingData*>* dataProcessingDataList = getNodeProcessingDataListFromNode(node, mDataPropertyNodeIdentifierDataHash);
					if (dataProcessingDataList) {
						for (CRedlandNodeProcessingData* processingData : *dataProcessingDataList) {
							axiomExpressions.append(mOntologyBuilder->getFunctionalDataProperty(QList<CBuildExpression*>() << processingData->mExpression));
						}
						delete dataProcessingDataList;
					}
				}));
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval(QList<CAxiomExpression*>& axiomExpressions) {

				collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval([&]()->CRedlandNodeStream* { return getOWLFunctionalPropertyInstanceNodesStream(); }, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getFunctionalObjectProperty(expressions); }, axiomExpressions);
				collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval([&]()->CRedlandNodeStream* { return getOWLInverseFunctionalPropertyInstanceNodesStream(); }, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getInverseFunctionalObjectProperty(expressions); }, axiomExpressions);
				collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval([&]()->CRedlandNodeStream* { return getOWLReflexivePropertyInstanceNodesStream(); }, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getReflexiveObjectProperty(expressions); }, axiomExpressions);
				collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval([&]()->CRedlandNodeStream* { return getOWLIrreflexivePropertyInstanceNodesStream(); }, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getIrreflexiveObjectProperty(expressions); }, axiomExpressions);
				collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval([&]()->CRedlandNodeStream* { return getOWLSymmetricPropertyInstanceNodesStream(); }, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getSymmetricObjectProperty(expressions); }, axiomExpressions);
				collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval([&]()->CRedlandNodeStream* { return getOWLAsymmetricPropertyInstanceNodesStream(); }, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getAsymmetricObjectProperty(expressions); }, axiomExpressions);
				collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval([&]()->CRedlandNodeStream* { return getOWLTransetivePropertyInstanceNodesStream(); }, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getTransetiveObjectProperty(expressions); }, axiomExpressions);

				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectObjectPropertyTypesAxiomExpressionsBySuccessorRetrieval(function<CRedlandNodeStream*()> nodeStreamFunc, function<CAxiomExpression*(const QList<CBuildExpression*>& expressions)> buildFunc,  QList<CAxiomExpression*>& axiomExpressions) {
				releaseRedlandNodeStreamWrapper(nodeStreamFunc()->forEach([&](librdf_node* node) {
					QList<CRedlandNodeProcessingData*>* objectProcessingDataList = getNodeProcessingDataListFromNode(node, mObjectPropertyNodeIdentifierDataHash);
					if (objectProcessingDataList) {
						for (CRedlandNodeProcessingData* processingData : *objectProcessingDataList) {
							axiomExpressions.append(buildFunc(QList<CBuildExpression*>() << processingData->mExpression));
						}
						delete objectProcessingDataList;
					}
				}));
				return this;
			}






			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectObjectPropertyBasedAxiomExpressionsBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) {
				if (processingData->mExpression) {

					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForRDFSSubPropertySuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mObjectPropertyNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getSubObjectPropertyOf(expressions); }, axiomExpressions);
					}));

					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLPropertyChainAxiomSuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromRDFNodeList(object, mObjectPropertyNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* {
							QList<CBuildExpression*> resortedExpressionList(expressions);
							if (!resortedExpressionList.isEmpty()) {
								CBuildExpression* superProperty = resortedExpressionList.takeFirst();
								resortedExpressionList.append(superProperty);
							}
							return mOntologyBuilder->getSubObjectPropertyOf(resortedExpressionList);
						}, axiomExpressions);
					}));


					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLEquivalentPropertySuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mObjectPropertyNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getEquivalentObjectProperties(expressions); }, axiomExpressions);
					}));


					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLPropertyDisjointWithSuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mObjectPropertyNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDisjointObjectProperties(expressions); }, axiomExpressions);
					}));


					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForRDFSDomainSuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getObjectPropertyDomainExpression(expressions); }, axiomExpressions);
					}));


					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForRDFSRangeSuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getObjectPropertyRangeExpression(expressions); }, axiomExpressions);
					}));


					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLInverseOfSuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mObjectPropertyNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getInverseObjectProperties(expressions); }, axiomExpressions);
					}));
				}
				return this;
			}




			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectDataPropertyBasedAxiomExpressions(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) {
				if (processingData->mExpression) {

					// TODO: if there are too many successor triples, then use the other method, which directly filters by the corresponding predicates
					// create/use forEachAsLong/forEachLimited
					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLAxioms))->forEach([&](librdf_statement* statement) {
						librdf_node* predicate = librdf_statement_get_predicate(statement);
						librdf_node* object = librdf_statement_get_object(statement);
						cint64 predicateIndex = mPropertyBasedAxiomPredicateIndexHash.value(CRedlandNodeHasher(predicate), -1);
						if (predicateIndex >= 0) {

							QList<CBuildExpression*> expressionList;
							expressionList.append(processingData->mExpression);

							switch (predicateIndex) {
							case mOWLPredicateBasedAxiomSubPropertyOfPredicateIndex:
								buildAxiomExpressionFromNode(object, mDataPropertyNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getSubDataPropertyOf(expressions); }, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomEquivalentPropertyPredicateIndex:
								buildAxiomExpressionFromNode(object, mDataPropertyNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getEquivalentDataProperties(expressions); }, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomPropertyDisjointWithPredicateIndex:
								buildAxiomExpressionFromNode(object, mDataPropertyNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDisjointDataProperties(expressions); }, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomDomainPredicateIndex:
								buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDataPropertyDomainExpression(expressions); }, axiomExpressions);
								break;
							case mOWLPredicateBasedAxiomRangePredicateIndex:
								buildAxiomExpressionFromNode(object, mDatatypeNodeIdentifierDataHash, expressionList, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDataPropertyRangeExpression(expressions); }, axiomExpressions);
								break;
							case mRDFPredicateBasedAxiomTypeOfPredicateIndex:
								cint64 objectIndex = mPropertyBasedAxiomObjectIndexHash.value(CRedlandNodeHasher(object), -1);
								if (objectIndex >= 0) {
									switch (objectIndex) {
									case mOWLPredicateBasedAxiomTypeFunctionalPropertyObjectIndex:
										axiomExpressions.append(mOntologyBuilder->getFunctionalDataProperty(processingData->mExpression));
										break;
									}
								}
								break;
							}

						}
					}));

				}
				return this;
			}






			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectDataPropertyBasedAxiomExpressionsBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, QList<CAxiomExpression*>& axiomExpressions) {
				if (processingData->mExpression) {

					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForRDFSSubPropertySuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mDataPropertyNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getSubDataPropertyOf(expressions); }, axiomExpressions);
					}));


					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLEquivalentPropertySuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mDataPropertyNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getEquivalentDataProperties(expressions); }, axiomExpressions);
					}));


					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLPropertyDisjointWithSuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mDataPropertyNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDisjointDataProperties(expressions); }, axiomExpressions);
					}));


					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForRDFSDomainSuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mClassNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { return mOntologyBuilder->getDataPropertyDomainExpression(expressions); }, axiomExpressions);
					}));


					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForRDFSRangeSuccessors))->forEach([&](librdf_statement* statement) {
						librdf_node* object = librdf_statement_get_object(statement);
						buildAxiomExpressionFromNode(object, mDatatypeNodeIdentifierDataHash, QList<CBuildExpression*>() << processingData->mExpression, [&](const QList<CBuildExpression*>& expressions)->CAxiomExpression* { 
							return mOntologyBuilder->getDataPropertyRangeExpression(expressions); 
						}, axiomExpressions);
					}));


				}
				return this;
			}







			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildAxiomExpressionFromNode(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& expressionList, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions) {
				cint64 prevSize = expressionList.size();
				collectBuildExpressionsFromNode(node, nodeIdentifierDataHash, expressionList);
				CAxiomExpression* axiomExpression = buildFunction(expressionList);
				if (axiomExpression) {
					axiomExpressions.append(axiomExpression);
				}
				while (expressionList.size() > prevSize) {
					expressionList.removeLast();
				}
				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildAxiomExpressionFromRDFNodeList(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& expressionList, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions) {
				cint64 prevSize = expressionList.size();
				collectBuildExpressionsFromRDFNodeList(listNode, nodeIdentifierDataHash, expressionList);
				CAxiomExpression* axiomExpression = buildFunction(expressionList);
				if (axiomExpression) {
					axiomExpressions.append(axiomExpression);
				}
				while (expressionList.size() > prevSize) {
					expressionList.removeLast();
				}
				return this;
			}





			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildAxiomExpressionFromRDFNodeListWithNewHandling(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CBuildExpression*>& expressionList, function<CAxiomExpression*(const QList<CBuildExpression*>&)> buildFunction, QList<CAxiomExpression*>& axiomExpressions) {
				cint64 prevSize = expressionList.size();
				collectBuildExpressionsFromRDFNodeListWithNewHandling(listNode, nodeIdentifierDataHash, newHandlingFunc, expressionList);
				CAxiomExpression* axiomExpression = buildFunction(expressionList);
				if (axiomExpression) {
					axiomExpressions.append(axiomExpression);
				}
				while (expressionList.size() > prevSize) {
					expressionList.removeLast();
				}
				return this;
			}











			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildObjectPropertyExpressions() {
				for (CRedlandNodeProcessingData* nextProcessingData : mObjectPropertyNodeHandlingList) {

					nextProcessingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStreamWithNewHandling(getOWLObjectInverseOfPredecessorNodesStream(nextProcessingData->mRedlandNode), mObjectPropertyNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* {
						CRedlandNodeProcessingData* processingData = nullptr;
						CBuildExpression* expression = nullptr;
						if (librdf_node_is_blank(node)) {
							expression = mOntologyBuilder->getInverseObjectPropertyOf(nextProcessingData->mExpression);
							processingData = new CRedlandNodeProcessingData(node, expression);
						}
						return processingData;
					});

					if (nextProcessingData->mDependendRedlandNodeProcessingDataList) {
						delete nextProcessingData->mDependendRedlandNodeProcessingDataList;
						nextProcessingData->mDependendRedlandNodeProcessingDataList = nullptr;
					}

				}
				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildDataRangeExpressions() {
				if (mConfSuccessorRetrieval) {
					buildExpressions(&mDatatypeNodeHandlingList, [&](CRedlandNodeProcessingData* processingData) { collectDataRangeExpressionDependenciesBySuccessorRetrieval(processingData);  });
				} else {
					buildExpressions(&mDatatypeNodeHandlingList, [&](CRedlandNodeProcessingData* processingData) { collectDataRangeExpressionDependencies(processingData);  });
				}
				return this;
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildClassExpressions() {
				if (mConfSuccessorRetrieval) {
					buildExpressions(&mClassNodeHandlingList, [&](CRedlandNodeProcessingData* processingData) { collectClassExpressionDependenciesBySuccessorRetrieval(processingData);  });
				} else {
					buildExpressions(&mClassNodeHandlingList, [&](CRedlandNodeProcessingData* processingData) { collectClassExpressionDependencies(processingData);  });
				}
				return this;
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::buildExpressions(QList<CRedlandNodeProcessingData*>* nodeHandlingList, function<void(CRedlandNodeProcessingData*)> dependenciesCollectionFunction) {

				QStack<CRedlandNodeProcessingData*> processingStack;
				QList<CBuildExpression*> dependentBuildExpressionList;

				for (CRedlandNodeProcessingData* nextProcessingData : *nodeHandlingList) {
					if (!nextProcessingData->mStacked && !nextProcessingData->mExpression) {
						nextProcessingData->mStacked = true;

						processingStack.push(nextProcessingData);
						while (!processingStack.isEmpty()) {
							CRedlandNodeProcessingData* processingData = processingStack.top();

							bool buildingPossible = false;
							if (!processingData->mExpression && !processingData->mProcessed) {
								dependenciesCollectionFunction(processingData);

								bool stackedProcessing = false;
								if (processingData->mDependendRedlandNodeProcessingDataList) {
									for (CRedlandNodeProcessingData* dependentProcessingData : *processingData->mDependendRedlandNodeProcessingDataList) {
										if (!dependentProcessingData->mExpression && !dependentProcessingData->mStacked) {
											processingStack.push(dependentProcessingData);
											dependentProcessingData->mStacked = true;
											stackedProcessing = true;
										}
									}
									if (!stackedProcessing) {
										buildingPossible = true;
									}
								} else {
									// probably an error, not clear how to build expression
									//TODO: print warning/error message
									buildingPossible = true;
								}

								processingData->mProcessed = true;
							} else {
								buildingPossible = true;
							}


							if (buildingPossible) {
								processingStack.pop();
								if (processingData->mExpressionBuildFunc) {
									dependentBuildExpressionList.clear();
									if (processingData->mDependendRedlandNodeProcessingDataList) {
										for (CRedlandNodeProcessingData* dependentProcessingData : *processingData->mDependendRedlandNodeProcessingDataList) {
											if (dependentProcessingData->mExpression) {
												dependentBuildExpressionList.append(dependentProcessingData->mExpression);
											}
										}
										//delete processingData->mDependendRedlandNodeProcessingDataList;
									}
									//processingData->mDependendRedlandNodeProcessingDataList = nullptr;
									processingData->mExpression = processingData->mExpressionBuildFunc(dependentBuildExpressionList);
									processingData->mBuilt = true;
									processingData->mDependendRedlandNodeProcessingDataList = nullptr;
								}
							}
						}
					}
				}
				return this;
			}













			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectDataRangeExpressionDependencies(CRedlandNodeProcessingData* processingData) {
				if (!processingData->mExpression) {

					librdf_node* dataRangePredicateObjectArray[mOWLDataRangePredicateMaxIndex];
					for (cint64 i = 0; i < mOWLDataRangePredicateMaxIndex; ++i) {
						dataRangePredicateObjectArray[i] = nullptr;
					}


					// TODO: if there are too many successor triples, then use the other method, which directly filters by the corresponding predicates
					// create/use forEachAsLong/forEachLimited
					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLDataRanges))->forEach([&](librdf_statement* statement) {
						librdf_node* predicate = librdf_statement_get_predicate(statement);
						librdf_node* object = librdf_statement_get_object(statement);
						cint64 index = mDataRangePredicateIndexHash.value(CRedlandNodeHasher(predicate), -1);
						if (index >= 0 && !dataRangePredicateObjectArray[index]) {
							dataRangePredicateObjectArray[index] = librdf_new_node_from_node(object);
						}
					}));



					// data intersection of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && dataRangePredicateObjectArray[mOWLIntersectionOfPredicateIndex]) {
						librdf_node* listNode = dataRangePredicateObjectArray[mOWLIntersectionOfPredicateIndex];
						processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mDatatypeNodeIdentifierDataHash, processingData->mDependendRedlandNodeProcessingDataList);
						processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataIntersectionOf(expressions); };
					}

					// data union of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && dataRangePredicateObjectArray[mOWLUnionOfPredicateIndex]) {
						librdf_node* listNode = dataRangePredicateObjectArray[mOWLUnionOfPredicateIndex];
						processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mDatatypeNodeIdentifierDataHash, processingData->mDependendRedlandNodeProcessingDataList);
						processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataUnionOf(expressions); };
					}


					// data complement of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && dataRangePredicateObjectArray[mOWLDatatypeComplementOfPredicateIndex]) {
						processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNode(dataRangePredicateObjectArray[mOWLDatatypeComplementOfPredicateIndex], mDatatypeNodeIdentifierDataHash);
						if (processingData->mDependendRedlandNodeProcessingDataList) {
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataComplementOf(expressions); };
						}
					}


					// data one of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && dataRangePredicateObjectArray[mOWLOneOfPredicateIndex]) {
						librdf_node* listNode = dataRangePredicateObjectArray[mOWLOneOfPredicateIndex];
						processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataListWithNewHandling(listNode, mDataLiteralNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createLiteralValueProcessingData(node); });
						processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataOneOf(expressions); };
					}


					// datatype restriction
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && dataRangePredicateObjectArray[mOWLOnDatatypePredicateIndex] && dataRangePredicateObjectArray[mOWLWithRestrictionPredicateIndex]) {
						processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNode(dataRangePredicateObjectArray[mOWLOnDatatypePredicateIndex], mDatatypeNodeIdentifierDataHash);
						if (processingData->mDependendRedlandNodeProcessingDataList) {
							QList<librdf_node*>* facetNodeList = getOWLListElementNodesList(dataRangePredicateObjectArray[mOWLWithRestrictionPredicateIndex]);

							if (facetNodeList) {
								for (librdf_node* facetNode : *facetNodeList) {
									releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(facetNode, nullptr, nullptr, mPartialFilteringStatementForOWLFacets))->forEach([&](librdf_statement* statement) {
										librdf_node* objectNode = librdf_statement_get_object(statement);
										librdf_node* predicateNode = librdf_statement_get_predicate(statement);
										if (librdf_node_is_literal(objectNode) && librdf_node_is_resource(predicateNode)) {
											CDataLiteralExpression* datLitExp = (CDataLiteralExpression*)createLiteralValueProcessingData(objectNode)->mExpression;
											const char* facetUri = (const char*)librdf_uri_as_string(librdf_node_get_uri(predicateNode));
											CDataFacetExpression* dataFacetExp = mOntologyBuilder->getDataFacet(QString::fromUtf8(facetUri));
											CDataFacetRestrictionExpression* dataFacetRestExp = mOntologyBuilder->getDataFacetRestriction(datLitExp, dataFacetExp);
											CRedlandNodeProcessingData* tmpProcessignData = new CRedlandNodeProcessingData(facetNode, dataFacetRestExp);
											processingData->mDependendRedlandNodeProcessingDataList->append(tmpProcessignData);
											mDataFacetRestrictionHandlingContainerList.append(tmpProcessignData);
										}
									}));

								}
								delete facetNodeList;
							}

							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDatatypeRestriction(expressions); };
						}
					}


					for (cint64 i = 0; i < mOWLDataRangePredicateMaxIndex; ++i) {
						if (dataRangePredicateObjectArray[i]) {
							librdf_free_node(dataRangePredicateObjectArray[i]);
						}
					}

				}
				return this;
			}





			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectDataRangeExpressionDependenciesBySuccessorRetrieval(CRedlandNodeProcessingData* processingData) {
				if (!processingData->mExpression) {

					// data intersection of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						releaseRedlandNodeStreamWrapper(getOWLIntersectionOfSuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* listNode) {
							processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mDatatypeNodeIdentifierDataHash, processingData->mDependendRedlandNodeProcessingDataList);
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataIntersectionOf(expressions); };
						}));
					}

					// data union of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						releaseRedlandNodeStreamWrapper(getOWLUnionOfSuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* listNode) {
							processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mDatatypeNodeIdentifierDataHash, processingData->mDependendRedlandNodeProcessingDataList);
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataUnionOf(expressions); };
						}));
					}


					// data complement of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStream(getOWLDatatypeComplementOfSuccessorNodesStream(processingData->mRedlandNode), mDatatypeNodeIdentifierDataHash);
						if (processingData->mDependendRedlandNodeProcessingDataList) {
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataComplementOf(expressions); };
						}
					}


					// data one of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						releaseRedlandNodeStreamWrapper(getOWLOneOfSuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* listNode) {
							processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataListWithNewHandling(listNode, mDataLiteralNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createLiteralValueProcessingData(node); });
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataOneOf(expressions); };
						}));
					}

					// datatype restriction
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStream(getOWLOnDatatypeSuccessorNodesStream(processingData->mRedlandNode), mDatatypeNodeIdentifierDataHash);
						if (processingData->mDependendRedlandNodeProcessingDataList) {
							QList<librdf_node*>* facetNodeList = nullptr;
							releaseRedlandNodeStreamWrapper(getOWLWithRestrictionSuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* node) {
								facetNodeList = getOWLListElementNodesList(node, facetNodeList);
							}));

							if (facetNodeList) {
								for (librdf_node* facetNode : *facetNodeList) {
									for (QHash<const char*, librdf_node*>::const_iterator it = mFacetUriNodeHash.constBegin(), itEnd = mFacetUriNodeHash.constEnd(); it != itEnd; ++it) {
										const char* facetUri = it.key();
										librdf_node* facetUriNode = it.value();
										collectDataFacetExpressionBySuccessorRetrieval(processingData, facetNode, facetUriNode, facetUri);
									}
								}
								delete facetNodeList;
							}

							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDatatypeRestriction(expressions); };
						}
					}

				}
				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectDataFacetExpressionBySuccessorRetrieval(CRedlandNodeProcessingData* processingData, librdf_node* node, librdf_node* facetNode, const char* facetUri) {
				releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(node, facetNode, nullptr, mPartialFilteringStatementForOWLFacetSuccessors))->forEach([&](librdf_statement* statement) {
					librdf_node* objectNode = librdf_statement_get_object(statement);
					if (librdf_node_is_literal(objectNode)) {
						CDataLiteralExpression* datLitExp = (CDataLiteralExpression*)createLiteralValueProcessingData(objectNode)->mExpression;
						CDataFacetExpression* dataFacetExp = mOntologyBuilder->getDataFacet(QString::fromUtf8(facetUri));
						CDataFacetRestrictionExpression* dataFacetRestExp = mOntologyBuilder->getDataFacetRestriction(datLitExp, dataFacetExp);
						CRedlandNodeProcessingData* tmpProcessignData = new CRedlandNodeProcessingData(node, dataFacetRestExp);
						processingData->mDependendRedlandNodeProcessingDataList->append(tmpProcessignData);
						mDataFacetRestrictionHandlingContainerList.append(tmpProcessignData);
					}
				}));

				return this;
			}







			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectClassExpressionDependencies(CRedlandNodeProcessingData* processingData) {
				if (!processingData->mExpression) {

					librdf_node* classPredicateObjectArray[mOWLPredicateMaxIndex];
					for (cint64 i = 0; i < mOWLPredicateMaxIndex; ++i) {
						classPredicateObjectArray[i] = nullptr;
					}



					// TODO: if there are too many successor triples that do not match, then use the other method, which directly filters by the corresponding predicates
					// create/use forEachAsLong/forEachLimited
					releaseRedlandTripleStreamWrapper(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(processingData->mRedlandNode, nullptr, nullptr, mPartialFilteringStatementForOWLClasses))->forEach([&](librdf_statement* statement) {
						librdf_node* predicate = librdf_statement_get_predicate(statement);
						librdf_node* object = librdf_statement_get_object(statement);
						cint64 index = mClassPredicateIndexHash.value(CRedlandNodeHasher(predicate), -1);
						if (index >= 0 && !classPredicateObjectArray[index]) {
							classPredicateObjectArray[index] = librdf_new_node_from_node(object);
						}
					}));


					// object intersection of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && classPredicateObjectArray[mOWLIntersectionOfPredicateIndex]) {
						librdf_node* listNode = classPredicateObjectArray[mOWLIntersectionOfPredicateIndex];
						processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mClassNodeIdentifierDataHash, processingData->mDependendRedlandNodeProcessingDataList);
						processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectIntersectionOf(expressions); };
					}

					// object union of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && classPredicateObjectArray[mOWLUnionOfPredicateIndex]) {
						librdf_node* listNode = classPredicateObjectArray[mOWLUnionOfPredicateIndex];
						processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mClassNodeIdentifierDataHash, processingData->mDependendRedlandNodeProcessingDataList);
						processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectUnionOf(expressions); };
					}

					// object complement of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && classPredicateObjectArray[mOWLComplementOfPredicateIndex]) {
						processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNode(classPredicateObjectArray[mOWLComplementOfPredicateIndex], mClassNodeIdentifierDataHash);
						if (processingData->mDependendRedlandNodeProcessingDataList) {
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectComplementOf(expressions); };
						}
					}

					// object one of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && classPredicateObjectArray[mOWLOneOfPredicateIndex]) {
						librdf_node* listNode = classPredicateObjectArray[mOWLOneOfPredicateIndex];
						processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataListWithNewHandling(listNode, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); });
						processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectOneOf(expressions); };
					}





					// restrictions
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList && classPredicateObjectArray[mOWLOnPropertyPredicateIndex]) {

						QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>* nodeDataHash = nullptr;
						bool dataPropertyNodeUsed = false;
						processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromObjectORDataPropertyNode(classPredicateObjectArray[mOWLOnPropertyPredicateIndex], &dataPropertyNodeUsed, &nodeDataHash, processingData->mDependendRedlandNodeProcessingDataList);


						if (processingData->mDependendRedlandNodeProcessingDataList) {
							// object/data some value from
							if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLSomeValuesFromPredicateIndex]) {
								processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNode(classPredicateObjectArray[mOWLSomeValuesFromPredicateIndex], *nodeDataHash, processingData->mDependendRedlandNodeProcessingDataList);
								if (dataPropertyNodeUsed) {
									processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataSomeValuesFrom(expressions); };
								} else {
									processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectSomeValuesFrom(expressions); };
								}
							}

							// object/data some value from
							if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLAllValuesFromPredicateIndex]) {
								processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNode(classPredicateObjectArray[mOWLAllValuesFromPredicateIndex], *nodeDataHash, processingData->mDependendRedlandNodeProcessingDataList);
								if (dataPropertyNodeUsed) {
									processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataAllValuesFrom(expressions); };
								} else {
									processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectAllValuesFrom(expressions); };
								}
							}

							// object/data has value
							if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLHasValuePredicateIndex]) {
								if (!dataPropertyNodeUsed) {
									processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeWithNewHandling(classPredicateObjectArray[mOWLHasValuePredicateIndex], mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, processingData->mDependendRedlandNodeProcessingDataList);
									processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectHasValue(expressions); };
								} else {
									processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeWithNewHandling(classPredicateObjectArray[mOWLHasValuePredicateIndex], mDataLiteralNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createLiteralValueProcessingData(node); }, processingData->mDependendRedlandNodeProcessingDataList);
									processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataHasValue(expressions); };
								}
							}

							// object has self
							if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLHasSelfPredicateIndex]) {
								if (!dataPropertyNodeUsed) {
									if (checkHasBooleanTrueLiteralValue(classPredicateObjectArray[mOWLHasSelfPredicateIndex])) {
										processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectHasSelf(expressions); };
									}
								}
							}



							// qualified cardinality restrictions
							if (!processingData->mExpressionBuildFunc && (classPredicateObjectArray[mOWLOnClassPredicateIndex] || classPredicateObjectArray[mOWLOnDataRangePredicateIndex])) {

								cint64 prevDepSize = processingData->mDependendRedlandNodeProcessingDataList->size();
								if (!dataPropertyNodeUsed) {
									processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNode(classPredicateObjectArray[mOWLOnClassPredicateIndex], mClassNodeIdentifierDataHash, processingData->mDependendRedlandNodeProcessingDataList);
								} else {
									processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNode(classPredicateObjectArray[mOWLOnDataRangePredicateIndex], mDatatypeNodeIdentifierDataHash, processingData->mDependendRedlandNodeProcessingDataList);
								}
								if (processingData->mDependendRedlandNodeProcessingDataList->size() > prevDepSize) {

									if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLMinQualifiedCardinalityPredicateIndex]) {
										cint64 minCardinality = getNonNegativeNumberFromLiteralValue(classPredicateObjectArray[mOWLMinQualifiedCardinalityPredicateIndex]);
										if (minCardinality >= 0) {
											if (!dataPropertyNodeUsed) {
												processingData->mExpressionBuildFunc = [minCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectMinCardinality(expressions, minCardinality); };
											} else {
												processingData->mExpressionBuildFunc = [minCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataMinCardinality(expressions, minCardinality); };
											}
										}
									}

									if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLMaxQualifiedCardinalityPredicateIndex]) {
										cint64 maxCardinality = getNonNegativeNumberFromLiteralValue(classPredicateObjectArray[mOWLMaxQualifiedCardinalityPredicateIndex]);
										if (maxCardinality >= 0) {
											if (!dataPropertyNodeUsed) {
												processingData->mExpressionBuildFunc = [maxCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectMaxCardinality(expressions, maxCardinality); };
											} else {
												processingData->mExpressionBuildFunc = [maxCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataMaxCardinality(expressions, maxCardinality); };
											}
										}
									}

									if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLQualifiedCardinalityPredicateIndex]) {
										cint64 cardinality = getNonNegativeNumberFromLiteralValue(classPredicateObjectArray[mOWLQualifiedCardinalityPredicateIndex]);
										if (cardinality >= 0) {
											if (!dataPropertyNodeUsed) {
												processingData->mExpressionBuildFunc = [cardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectExactlyCardinality(expressions, cardinality); };
											} else {
												processingData->mExpressionBuildFunc = [cardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataExactCardinality(expressions, cardinality); };
											}
										}
									}
								}
								if (!processingData->mExpressionBuildFunc) {
									while (processingData->mDependendRedlandNodeProcessingDataList->size() > prevDepSize) {
										processingData->mDependendRedlandNodeProcessingDataList->removeLast();
									}
								}
							}

							// unqualified cardinality restrictions
							if (!processingData->mExpressionBuildFunc && !dataPropertyNodeUsed) {
								if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLMinCardinalityPredicateIndex]) {
									cint64 minCardinality = getNonNegativeNumberFromLiteralValue(classPredicateObjectArray[mOWLMinCardinalityPredicateIndex]);
									if (minCardinality >= 0) {
										if (!dataPropertyNodeUsed) {
											processingData->mExpressionBuildFunc = [minCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectMinCardinality(expressions, minCardinality); };
										} else {
											processingData->mExpressionBuildFunc = [minCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataMinCardinality(expressions, minCardinality); };
										}
									}
								}

								if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLMaxCardinalityPredicateIndex]) {
									cint64 maxCardinality = getNonNegativeNumberFromLiteralValue(classPredicateObjectArray[mOWLMaxCardinalityPredicateIndex]);
									if (maxCardinality >= 0) {
										if (!dataPropertyNodeUsed) {
											processingData->mExpressionBuildFunc = [maxCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectMaxCardinality(expressions, maxCardinality); };
										} else {
											processingData->mExpressionBuildFunc = [maxCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataMaxCardinality(expressions, maxCardinality); };
										}
									}
								}

								if (!processingData->mExpressionBuildFunc && classPredicateObjectArray[mOWLCardinalityPredicateIndex]) {
									cint64 cardinality = getNonNegativeNumberFromLiteralValue(classPredicateObjectArray[mOWLCardinalityPredicateIndex]);
									if (cardinality >= 0) {
										if (!dataPropertyNodeUsed) {
											processingData->mExpressionBuildFunc = [cardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectExactlyCardinality(expressions, cardinality); };
										} else {
											processingData->mExpressionBuildFunc = [cardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataExactCardinality(expressions, cardinality); };
										}
									}
								}
							}

						}
						if (!processingData->mExpressionBuildFunc) {
							delete processingData->mDependendRedlandNodeProcessingDataList;
							processingData->mDependendRedlandNodeProcessingDataList = nullptr;
						}

					}



				}
				return this;
			}








			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectClassExpressionDependenciesBySuccessorRetrieval(CRedlandNodeProcessingData* processingData) {
				if (!processingData->mExpression) {

					// object intersection of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						releaseRedlandNodeStreamWrapper(getOWLIntersectionOfSuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* listNode) {
							processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mClassNodeIdentifierDataHash);
							if (processingData->mDependendRedlandNodeProcessingDataList->isEmpty()) {
								processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mClassNodeIdentifierDataHash);
								processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mClassNodeIdentifierDataHash);
								processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mClassNodeIdentifierDataHash);

							}
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectIntersectionOf(expressions); };
						}));
					}

					// object union of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						releaseRedlandNodeStreamWrapper(getOWLUnionOfSuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* listNode) {
							processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataList(listNode, mClassNodeIdentifierDataHash);
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectUnionOf(expressions); };
						}));
					}

					// object complement of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStream(getOWLComplementOfSuccessorNodesStream(processingData->mRedlandNode), mClassNodeIdentifierDataHash);
						if (processingData->mDependendRedlandNodeProcessingDataList) {
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectComplementOf(expressions); };
						}
					}

					// object one of
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						releaseRedlandNodeStreamWrapper(getOWLOneOfSuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* listNode) {
							processingData->mDependendRedlandNodeProcessingDataList = getRDFListElementNodesAsProcessingDataListWithNewHandling(listNode, mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); });
							processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectOneOf(expressions); };
						}));
					}




					// restriction
					if (!processingData->mExpressionBuildFunc && !processingData->mDependendRedlandNodeProcessingDataList) {
						QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>* nodeDataHash = nullptr;
						bool dataPropertyNodeUsed = false;
						bool onPropertyPresent = collectClassExpressionOnPropertyDependenciesReturnPresence(processingData, &dataPropertyNodeUsed, &nodeDataHash);

						if (onPropertyPresent && processingData->mDependendRedlandNodeProcessingDataList) {
							cint64 onPropertyDepDataCount = processingData->mDependendRedlandNodeProcessingDataList->size();

							// object/data some value from
							if (!processingData->mExpressionBuildFunc) {
								processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStream(getOWLSomeValuesFromSuccessorNodesStream(processingData->mRedlandNode), *nodeDataHash, processingData->mDependendRedlandNodeProcessingDataList);
								if (processingData->mDependendRedlandNodeProcessingDataList->size() > onPropertyDepDataCount) {
									if (dataPropertyNodeUsed) {
										processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataSomeValuesFrom(expressions); };
									} else {
										processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectSomeValuesFrom(expressions); };
									}
								}
							}

							// object/data add value from
							if (!processingData->mExpressionBuildFunc) {
								processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStream(getOWLAllValuesFromSuccessorNodesStream(processingData->mRedlandNode), *nodeDataHash, processingData->mDependendRedlandNodeProcessingDataList);
								if (processingData->mDependendRedlandNodeProcessingDataList->size() > onPropertyDepDataCount) {
									if (dataPropertyNodeUsed) {
										processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataAllValuesFrom(expressions); };
									} else {
										processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectAllValuesFrom(expressions); };
									}
								}
							}

							// object/data has value
							if (!processingData->mExpressionBuildFunc) {
								if (dataPropertyNodeUsed) {
									processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStreamWithNewHandling(getOWLHasValueSuccessorNodesStream(processingData->mRedlandNode), mDataLiteralNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createLiteralValueProcessingData(node); }, processingData->mDependendRedlandNodeProcessingDataList);
								} else {
									processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStreamWithNewHandling(getOWLHasValueSuccessorNodesStream(processingData->mRedlandNode), mNamedIndividualNodeIdentifierDataHash, [&](librdf_node* node)->CRedlandNodeProcessingData* { return createNamedIndividualProcessingData(node); }, processingData->mDependendRedlandNodeProcessingDataList);
								}
								if (processingData->mDependendRedlandNodeProcessingDataList->size() > onPropertyDepDataCount) {
									if (dataPropertyNodeUsed) {
										processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataHasValue(expressions); };
									} else {
										processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectHasValue(expressions); };
									}
								}
							}


							// object has self
							if (!processingData->mExpressionBuildFunc && !dataPropertyNodeUsed) {
								releaseRedlandNodeStreamWrapper(getOWLHasSelfSuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* node) {
									if (!processingData->mExpressionBuildFunc && checkHasBooleanTrueLiteralValue(node)) {
										processingData->mExpressionBuildFunc = [&](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectHasSelf(expressions); };
									}
								}));
							}


							// cardinalities
							if (!processingData->mExpressionBuildFunc) {
								if (!dataPropertyNodeUsed) {
									processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStream(getOWLOnClassSuccessorNodesStream(processingData->mRedlandNode), *nodeDataHash, processingData->mDependendRedlandNodeProcessingDataList);
								} else {
									processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStream(getOWLOnDataRangeSuccessorNodesStream(processingData->mRedlandNode), *nodeDataHash, processingData->mDependendRedlandNodeProcessingDataList);
								}
								cint64 onClassOrDataRangeDepDataCount = processingData->mDependendRedlandNodeProcessingDataList->size();

								if (!processingData->mExpressionBuildFunc && onClassOrDataRangeDepDataCount > onPropertyDepDataCount) {
									// qualified cardinality restrictions

									if (!processingData->mExpressionBuildFunc) {
										cint64 minCardinality = -1;
										releaseRedlandNodeStreamWrapper(getOWLMinQualifiedCardinalitySuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* node) {
											minCardinality = getNonNegativeNumberFromLiteralValue(node);
										}));

										if (minCardinality >= 0) {
											if (!dataPropertyNodeUsed) {
												processingData->mExpressionBuildFunc = [minCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectMinCardinality(expressions, minCardinality); };
											} else {
												processingData->mExpressionBuildFunc = [minCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataMinCardinality(expressions, minCardinality); };
											}
										}
									}


									if (!processingData->mExpressionBuildFunc) {
										cint64 maxCardinality = -1;
										releaseRedlandNodeStreamWrapper(getOWLMaxQualifiedCardinalitySuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* node) {
											maxCardinality = getNonNegativeNumberFromLiteralValue(node);
										}));

										if (maxCardinality >= 0) {
											if (!dataPropertyNodeUsed) {
												processingData->mExpressionBuildFunc = [maxCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectMaxCardinality(expressions, maxCardinality); };
											} else {
												processingData->mExpressionBuildFunc = [maxCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataMaxCardinality(expressions, maxCardinality); };
											}
										}
									}

									if (!processingData->mExpressionBuildFunc) {
										cint64 cardinality = -1;
										releaseRedlandNodeStreamWrapper(getOWLQualifiedCardinalitySuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* node) {
											cardinality = getNonNegativeNumberFromLiteralValue(node);
										}));

										if (!dataPropertyNodeUsed) {
											processingData->mExpressionBuildFunc = [cardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectExactlyCardinality(expressions, cardinality); };
										} else {
											processingData->mExpressionBuildFunc = [cardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataExactCardinality(expressions, cardinality); };
										}
									}


									if (!processingData->mExpressionBuildFunc) {
										while (processingData->mDependendRedlandNodeProcessingDataList->size() > onPropertyDepDataCount) {
											processingData->mDependendRedlandNodeProcessingDataList->removeLast();
										}
									}

								}



								if (!processingData->mExpressionBuildFunc) {
									// unqualified cardinality restriction

									if (!processingData->mExpressionBuildFunc) {
										cint64 minCardinality = -1;
										releaseRedlandNodeStreamWrapper(getOWLMinCardinalitySuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* node) {
											minCardinality = getNonNegativeNumberFromLiteralValue(node);
										}));

										if (minCardinality >= 0) {
											if (!dataPropertyNodeUsed) {
												processingData->mExpressionBuildFunc = [minCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectMinCardinality(expressions, minCardinality); };
											} else {
												processingData->mExpressionBuildFunc = [minCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataMinCardinality(expressions, minCardinality); };
											}
										}
									}

									if (!processingData->mExpressionBuildFunc) {
										cint64 maxCardinality = -1;
										releaseRedlandNodeStreamWrapper(getOWLMaxCardinalitySuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* node) {
											maxCardinality = getNonNegativeNumberFromLiteralValue(node);
										}));

										if (maxCardinality >= 0) {
											if (!dataPropertyNodeUsed) {
												processingData->mExpressionBuildFunc = [maxCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectMaxCardinality(expressions, maxCardinality); };
											} else {
												processingData->mExpressionBuildFunc = [maxCardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataMaxCardinality(expressions, maxCardinality); };
											}
										}
									}


									if (!processingData->mExpressionBuildFunc) {
										cint64 cardinality = -1;
										releaseRedlandNodeStreamWrapper(getOWLCardinalitySuccessorNodesStream(processingData->mRedlandNode)->forEach([&](librdf_node* node) {
											cardinality = getNonNegativeNumberFromLiteralValue(node);
										}));

										if (cardinality >= 0) {
											if (!dataPropertyNodeUsed) {
												processingData->mExpressionBuildFunc = [cardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getObjectExactlyCardinality(expressions, cardinality); };
											} else {
												processingData->mExpressionBuildFunc = [cardinality, this](const QList<CBuildExpression*>& expressions)->CBuildExpression* { return mOntologyBuilder->getDataExactCardinality(expressions, cardinality); };
											}
										}
									}

								}

								
							} // end cardinality

							
							if (!processingData->mExpressionBuildFunc) {
								delete processingData->mDependendRedlandNodeProcessingDataList;
								processingData->mDependendRedlandNodeProcessingDataList = nullptr;
							}
						}

					} // end restriction


				}
				return this;
			}










			bool CConcreteOntologyRedlandTriplesDataExpressionMapper::checkHasBooleanTrueLiteralValue(librdf_node* node) {
				if (librdf_node_is_literal(node)) {
					const char* lexicalDataValueString = (const char*)librdf_node_get_literal_value(node);
					QString literalValueString = QString::fromUtf8(lexicalDataValueString);
					literalValueString = literalValueString.toUpper();
					if (literalValueString.compare("TRUE") != 0) {
						return false;
					}

					const char* lexicalDataValueLanguageString = librdf_node_get_literal_value_language(node);
					if (lexicalDataValueLanguageString) {
						return false;
					}

					librdf_uri* datatypeUri = librdf_node_get_literal_value_datatype_uri(node);
					if (datatypeUri) {
						if (librdf_uri_equals(mBooleanDatatypeUri, datatypeUri) == 0) {
							return false;
						}
					} else {
						return false;
					}
					return true;
				}
				return false;
			}




			cint64 CConcreteOntologyRedlandTriplesDataExpressionMapper::getNonNegativeNumberFromLiteralValue(librdf_node* node) {
				cint64 nonNegativeNumber = -1;
				if (librdf_node_is_literal(node)) {
					const char* lexicalDataValueString = (const char*)librdf_node_get_literal_value(node);
					QString literalValueString = QString::fromUtf8(lexicalDataValueString);
					bool parsingSuccess = false;
					nonNegativeNumber = literalValueString.toLongLong(&parsingSuccess);
					if (!parsingSuccess) {
						nonNegativeNumber = -1;
					}
				}
				return nonNegativeNumber;
			}



			QList<CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getNodeProcessingDataListFromObjectORDataPropertyNode(librdf_node* node, bool* dataPropertyFlag, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>** nodeDataHash, QList<CRedlandNodeProcessingData*>* extendingList) {
				QList<CRedlandNodeProcessingData*>* nodesProcessingData = extendingList;
				bool dataPropertyNodeUsed = false;
				CRedlandNodeHasher hasher(node);
				CRedlandNodeProcessingData* processingData = mObjectPropertyNodeIdentifierDataHash.value(hasher);
				if (!processingData) {
					processingData = mDataPropertyNodeIdentifierDataHash.value(hasher);
					if (processingData) {
						dataPropertyNodeUsed = true;
					}
				}
				if (processingData) {
					if (!nodesProcessingData) {
						nodesProcessingData = new QList<CRedlandNodeProcessingData *>();
					}
					nodesProcessingData->append(processingData);
				}

				if (dataPropertyFlag) {
					*dataPropertyFlag = false;
					if (dataPropertyNodeUsed) {
						*dataPropertyFlag = true;
					}
				}

				if (nodeDataHash) {
					*nodeDataHash = &mClassNodeIdentifierDataHash;
					if (dataPropertyNodeUsed) {
						*nodeDataHash = &mDatatypeNodeIdentifierDataHash;
					}
				}
				return nodesProcessingData;
			}


			bool CConcreteOntologyRedlandTriplesDataExpressionMapper::collectClassExpressionOnPropertyDependenciesReturnPresence(CRedlandNodeProcessingData* processingData, bool* dataPropertyFlag, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>** nodeDataHash) {
				bool onPropertyPresent = false;
				bool dataPropertyNodeUsed = false;
				processingData->mDependendRedlandNodeProcessingDataList = getNodeProcessingDataListFromNodeStream(getOWLOnPropertySuccessorNodesStream(processingData->mRedlandNode), [&](librdf_node* node)->CRedlandNodeProcessingData* {
					CRedlandNodeHasher hasher(node);
					CRedlandNodeProcessingData* processingData = mObjectPropertyNodeIdentifierDataHash.value(hasher);
					if (!processingData) {
						processingData = mDataPropertyNodeIdentifierDataHash.value(hasher);
						if (processingData) {
							dataPropertyNodeUsed = true;
						}
					}
					if (processingData) {
						onPropertyPresent = true;
					}
					return processingData;
				}, processingData->mDependendRedlandNodeProcessingDataList);


				if (dataPropertyFlag) {
					*dataPropertyFlag = dataPropertyNodeUsed;
				}

				if (nodeDataHash) {
					*nodeDataHash = &mClassNodeIdentifierDataHash;
					if (dataPropertyNodeUsed) {
						*nodeDataHash = &mDatatypeNodeIdentifierDataHash;
					}
				}
				return onPropertyPresent;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData* CConcreteOntologyRedlandTriplesDataExpressionMapper::createNamedIndividualProcessingData(librdf_node* node) {
				CRedlandNodeProcessingData* processingData = nullptr;
				CBuildExpression* expression = nullptr;
				if (!librdf_node_is_blank(node)) {
					librdf_uri* uri = librdf_node_get_uri(node);
					const char* uriString = (const char*)librdf_uri_as_string(uri);
					expression = mOntologyBuilder->getNamedIndividual(QString::fromUtf8(uriString));
					processingData = new CRedlandNodeProcessingData(node, expression);
				}
				return processingData;
			}




			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData* CConcreteOntologyRedlandTriplesDataExpressionMapper::createLiteralValueProcessingData(librdf_node* node) {
				CRedlandNodeProcessingData* processingData = nullptr;
				if (librdf_node_is_literal(node)) {
					const char* lexicalDataValueString = (const char*)librdf_node_get_literal_value(node);
					QString literalValueString = QString::fromUtf8(lexicalDataValueString);
					const char* lexicalDataValueLanguageString = librdf_node_get_literal_value_language(node);
					if (lexicalDataValueLanguageString) {
						literalValueString += QString("@%1").arg(QString::fromUtf8(lexicalDataValueLanguageString));
					}

					librdf_uri* datatypeUri = librdf_node_get_literal_value_datatype_uri(node);
					const char* datatypeUriString = nullptr;
					if (datatypeUri) {
						datatypeUriString = (const char*)librdf_uri_as_string(datatypeUri);
					}
					if (!datatypeUriString) {
						datatypeUriString = PREFIX_XML_PLAINLITERAL_DATATYPE;
					}
					CDataLexicalValueExpression* dataLexValueExp = mOntologyBuilder->getDataLexicalValue(literalValueString);
					CDatatypeExpression* datatypeExp = mOntologyBuilder->getDatatype(QString::fromUtf8(datatypeUriString));
					CDataLiteralExpression* dataLitExp = mOntologyBuilder->getDataLiteral(dataLexValueExp, datatypeExp);
					processingData = new CRedlandNodeProcessingData(node, dataLitExp);
				}
				return processingData;
			}






			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectBuildExpressionsFromNode(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& existingList) {
				CRedlandNodeProcessingData* processingData = nodeIdentifierDataHash.value(CRedlandNodeHasher(node));
				if (processingData && processingData->mExpression) {
					existingList.append(processingData->mExpression);
				}
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectBuildExpressionsFromNodeStream(CRedlandNodeStream* nodeStream, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& existingList) {
				nodeStream->forEach([&](librdf_node* node) {
					collectBuildExpressionsFromNode(node, nodeIdentifierDataHash, existingList);
				});
				releaseRedlandNodeStreamWrapper(nodeStream);
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectBuildExpressionsFromRDFNodeList(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CBuildExpression*>& existingList) {
				QList<librdf_node*> listNodes;
				listNodes.append(librdf_new_node_from_node(listNode));
				while (!listNodes.isEmpty()) {
					librdf_node* nextListNode = listNodes.takeFirst();
					if (!librdf_node_equals(nextListNode, mRDFNilNode)) {

						collectBuildExpressionsFromNodeStream(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFFirstSuccessors)), 2), nodeIdentifierDataHash, existingList);

						releaseRedlandNodeStreamWrapper(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFRestSuccessors)), 2)->forEach([&](librdf_node* node) {
							listNodes.append(librdf_new_node_from_node(node));
						}));
					}
					librdf_free_node(nextListNode);
				}
				return this;
			}







			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectBuildExpressionsFromNodeWithNewHandling(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CBuildExpression*>& existingList) {
				CRedlandNodeProcessingData* processingData = nodeIdentifierDataHash.value(CRedlandNodeHasher(node));
				if (!processingData) {
					librdf_node* nodeCopy = librdf_new_node_from_node(node);
					processingData = newHandlingFunc(nodeCopy);
					if (processingData) {
						nodeIdentifierDataHash[CRedlandNodeHasher(nodeCopy)] = processingData;
					} else {
						librdf_free_node(nodeCopy);
					}
				}
				if (processingData && processingData->mExpression) {
					existingList.append(processingData->mExpression);
				}
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectBuildExpressionsFromNodeStreamWithNewHandling(CRedlandNodeStream* nodeStream, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CBuildExpression*>& existingList) {
				nodeStream->forEach([&](librdf_node* node) {
					collectBuildExpressionsFromNodeWithNewHandling(node, nodeIdentifierDataHash, newHandlingFunc, existingList);
				});
				releaseRedlandNodeStreamWrapper(nodeStream);
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::collectBuildExpressionsFromRDFNodeListWithNewHandling(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CBuildExpression*>& existingList) {
				QList<librdf_node*> listNodes;
				listNodes.append(librdf_new_node_from_node(listNode));
				while (!listNodes.isEmpty()) {
					librdf_node* nextListNode = listNodes.takeFirst();
					if (!librdf_node_equals(nextListNode, mRDFNilNode)) {

						collectBuildExpressionsFromNodeStreamWithNewHandling(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFFirstSuccessors)), 2), nodeIdentifierDataHash, newHandlingFunc, existingList);

						releaseRedlandNodeStreamWrapper(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFRestSuccessors)), 2)->forEach([&](librdf_node* node) {
							listNodes.append(librdf_new_node_from_node(node));
						}));
					}
					librdf_free_node(nextListNode);
				}
				return this;
			}













			QList<CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getNodeProcessingDataListFromNode(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>* existingList) {
				QList<CRedlandNodeProcessingData*>* nodesProcessingData = existingList;
				CRedlandNodeProcessingData* processingData = nodeIdentifierDataHash.value(CRedlandNodeHasher(node));
				if (processingData) {
					if (!nodesProcessingData) {
						nodesProcessingData = new QList<CRedlandNodeProcessingData*>();
					}
					nodesProcessingData->append(processingData);
				}
				return nodesProcessingData;
			}



			QList<CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getNodeProcessingDataListFromNodeStream(CRedlandNodeStream* nodeStream, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>* existingList) {
				QList<CRedlandNodeProcessingData*>* nodesProcessingData = existingList;
				nodeStream->forEach([&](librdf_node* node) {
					nodesProcessingData = getNodeProcessingDataListFromNode(node, nodeIdentifierDataHash, nodesProcessingData);
				});
				releaseRedlandNodeStreamWrapper(nodeStream);
				return nodesProcessingData;
			}

			QList<CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getRDFListElementNodesAsProcessingDataList(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>* existingList) {
				QList<CRedlandNodeProcessingData*>* elementNodesProcessingData = existingList;
				QList<librdf_node*> listNodes;
				listNodes.append(librdf_new_node_from_node(listNode));
				while (!listNodes.isEmpty()) {
					librdf_node* nextListNode = listNodes.takeFirst();
					if (!librdf_node_equals(nextListNode, mRDFNilNode)) {

						elementNodesProcessingData = getNodeProcessingDataListFromNodeStream(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFFirstSuccessors)), 2), nodeIdentifierDataHash, elementNodesProcessingData);
						
						releaseRedlandNodeStreamWrapper(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFRestSuccessors)), 2)->forEach([&](librdf_node* node) {
							listNodes.append(librdf_new_node_from_node(node));
						}));

					}
					librdf_free_node(nextListNode);
				}
				return elementNodesProcessingData;
			}







			QList<CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getNodeProcessingDataListFromNode(librdf_node* node, function<CRedlandNodeProcessingData*(librdf_node*)> resolvingFunc, QList<CRedlandNodeProcessingData*>* existingList) {
				QList<CRedlandNodeProcessingData*>* nodesProcessingData = existingList;
				CRedlandNodeProcessingData* processingData = resolvingFunc(node);
				if (processingData) {
					if (!nodesProcessingData) {
						nodesProcessingData = new QList<CRedlandNodeProcessingData*>();
					}
					nodesProcessingData->append(processingData);
				}
				return nodesProcessingData;
			}

			QList<CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getNodeProcessingDataListFromNodeStream(CRedlandNodeStream* nodeStream, function<CRedlandNodeProcessingData*(librdf_node*)> resolvingFunc, QList<CRedlandNodeProcessingData*>* existingList) {
				QList<CRedlandNodeProcessingData*>* nodesProcessingData = existingList;
				nodeStream->forEach([&](librdf_node* node) {
					nodesProcessingData = getNodeProcessingDataListFromNode(node, resolvingFunc, nodesProcessingData);
				});
				releaseRedlandNodeStreamWrapper(nodeStream);
				return nodesProcessingData;
			}




			QList<CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getNodeProcessingDataListFromNodeWithNewHandling(librdf_node* node, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CRedlandNodeProcessingData*>* existingList) {
				QList<CRedlandNodeProcessingData*>* nodesProcessingData = existingList;
				CRedlandNodeProcessingData* processingData = nodeIdentifierDataHash.value(CRedlandNodeHasher(node));
				if (!processingData) {
					librdf_node* nodeCopy = librdf_new_node_from_node(node);
					processingData = newHandlingFunc(nodeCopy);
					if (processingData) {
						nodeIdentifierDataHash[CRedlandNodeHasher(nodeCopy)] = processingData;
					} else {
						librdf_free_node(nodeCopy);
					}
				}
				if (processingData) {
					if (!nodesProcessingData) {
						nodesProcessingData = new QList<CRedlandNodeProcessingData*>();
					}
					nodesProcessingData->append(processingData);
				}
				return nodesProcessingData;
			}

			QList<CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getNodeProcessingDataListFromNodeStreamWithNewHandling(CRedlandNodeStream* nodeStream, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CRedlandNodeProcessingData*>* existingList) {
				QList<CRedlandNodeProcessingData*>* nodesProcessingData = existingList;
				nodeStream->forEach([&](librdf_node* node) {
					nodesProcessingData = getNodeProcessingDataListFromNodeWithNewHandling(node, nodeIdentifierDataHash, newHandlingFunc, nodesProcessingData);
				});
				releaseRedlandNodeStreamWrapper(nodeStream);
				return nodesProcessingData;
			}

			QList<CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeProcessingData*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getRDFListElementNodesAsProcessingDataListWithNewHandling(librdf_node* listNode, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, function<CRedlandNodeProcessingData*(librdf_node* node)> newHandlingFunc, QList<CRedlandNodeProcessingData*>* existingList) {
				QList<CRedlandNodeProcessingData*>* elementNodesProcessingData = existingList;
				QList<librdf_node*> listNodes;
				listNodes.append(librdf_new_node_from_node(listNode));
				while (!listNodes.isEmpty()) {
					librdf_node* nextListNode = listNodes.takeFirst();
					if (!librdf_node_equals(nextListNode, mRDFNilNode)) {

						elementNodesProcessingData = getNodeProcessingDataListFromNodeStreamWithNewHandling(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFFirstSuccessors)), 2), nodeIdentifierDataHash, newHandlingFunc, elementNodesProcessingData);

						releaseRedlandNodeStreamWrapper(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFRestSuccessors)), 2)->forEach([&](librdf_node* node) {
							listNodes.append(librdf_new_node_from_node(node));
						}));

					}
					librdf_free_node(nextListNode);
				}
				return elementNodesProcessingData;
			}






			QList<librdf_node*>* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLListElementNodesList(librdf_node* listNode, QList<librdf_node*>* extendingList) {
				QList<librdf_node*>* elementNodes = extendingList;
				QList<librdf_node*> listNodes;
				listNodes.append(librdf_new_node_from_node(listNode));
				while (!listNodes.isEmpty()) {
					librdf_node* nextListNode = listNodes.takeFirst();
					if (!librdf_node_equals(nextListNode, mRDFNilNode)) {

						releaseRedlandNodeStreamWrapper(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFFirstSuccessors)), 2)->forEach([&](librdf_node* node) {
							if (!elementNodes) {
								elementNodes = new QList<librdf_node*>();
							}
							elementNodes->append(librdf_new_node_from_node(node));
						}));


						releaseRedlandNodeStreamWrapper(createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nextListNode, nullptr, nullptr, mPartialFilteringStatementForRDFRestSuccessors)), 2)->forEach([&](librdf_node* node) {
							listNodes.append(librdf_new_node_from_node(node));
						}));

					}
					librdf_free_node(nextListNode);
				}
				return elementNodes;
			}





			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::handleDeclaration(librdf_node* node, function<CBuildExpression*(const QString &uri)> buildEntityFunc, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>& nodeHandlingList) {
				bool anonym = true;
				CBuildExpression* expression = nullptr;
				librdf_node* nodeCopy = librdf_new_node_from_node(node);
				if (!librdf_node_is_blank(nodeCopy)) {					
					anonym = false;
					librdf_uri* uri = librdf_node_get_uri(nodeCopy);
					const char* uriString = (const char*)librdf_uri_as_string(uri);
					expression = buildEntityFunc(QString::fromUtf8(uriString));
					CAxiomExpression* declExpression = mOntologyBuilder->getDeclaration(expression);
					mOntologyBuilder->tellOntologyAxiom(declExpression);
				} else {
					expression = expression;
				}
				CRedlandNodeProcessingData*& processingData = nodeIdentifierDataHash[CRedlandNodeHasher(nodeCopy)];
				if (!processingData) {
					processingData = new CRedlandNodeProcessingData(nodeCopy, expression);
					nodeHandlingList.append(processingData);
				} else {
					librdf_free_node(nodeCopy);
				}
				return this;
			}





			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::addBaseNodeExpression(const char* uri, CBuildExpression* expr, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>& nodeHandlingList, CRedlandStoredTriplesData* redlandTriplesData) {
				librdf_node* node = librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)uri);
				addBaseNodeExpression(node, expr, nodeIdentifierDataHash, nodeHandlingList);
				librdf_free_node(node);
				return this;
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::addBaseNodeExpression(librdf_node* node, CBuildExpression* expression, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>& nodeHandlingList) {
				librdf_node* nodeCopy = librdf_new_node_from_node(node);
				CRedlandNodeProcessingData*& processingData = nodeIdentifierDataHash[CRedlandNodeHasher(nodeCopy)];
				if (!processingData) {
					processingData = new CRedlandNodeProcessingData(nodeCopy, expression);
					nodeHandlingList.append(processingData);
				}
				return this;
			}





			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::handleClassExpressionRestriction(librdf_node* node) {
				bool anonym = true;
				CBuildExpression* expression = nullptr;
				librdf_node* nodeCopy = librdf_new_node_from_node(node);
				CRedlandNodeProcessingData*& processingData = mClassNodeIdentifierDataHash[CRedlandNodeHasher(nodeCopy)];
				if (!processingData) {
					processingData = new CRedlandNodeProcessingData(nodeCopy, expression);
					mClassNodeHandlingList.append(processingData);
				} else {
					librdf_free_node(nodeCopy);
				}
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandTripleStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getPartialStatementFilteredTripleStream(librdf_statement* partialFilteringStatement) {
				librdf_stream* stream = librdf_model_find_statements(mRedlandTriplesData->getRedlandIndexedModel(), partialFilteringStatement);
				return createRedlandTripleStreamWrapper()->init(stream);
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::initTripleDataProcessing(CRedlandStoredTriplesData* redlandTriplesData) {
				mRedlandTriplesData = redlandTriplesData;

				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_CLASS, mPartialFilteringStatementForAllRDFTypeOfOwlClass);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_RESTRICTION, mPartialFilteringStatementForAllRDFTypeOfOwlRestriction);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_OBJECT_PROPERTY, mPartialFilteringStatementForAllRDFTypeOfOwlObjectProperty);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_DATATYPE_PROPERTY, mPartialFilteringStatementForAllRDFTypeOfOwlDataProperty);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_RDFS_DATATYPE, mPartialFilteringStatementForAllRDFTypeOfRDFSDatatype);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_NAMED_INDIVIDUAL, mPartialFilteringStatementForAllRDFTypeOfOwlNamedIndividual);

				initPartialFilteringStatement(nullptr, PREFIX_OWL_INVERSE_OF, nullptr, mPartialFilteringStatementForOWLObjectInverseOfPredecessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_INTERSECTION_OF, nullptr, mPartialFilteringStatementForOWLIntersectionOfSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_UNION_OF, nullptr, mPartialFilteringStatementForOWLUnionOfSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_FIRST, nullptr, mPartialFilteringStatementForRDFFirstSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_REST, nullptr, mPartialFilteringStatementForRDFRestSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_COMPLEMENT_OF, nullptr, mPartialFilteringStatementForOWLComplementOfSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_ONE_OF, nullptr, mPartialFilteringStatementForOWLOneOfSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_DATATYPE_COMPLEMENT_OF, nullptr, mPartialFilteringStatementForOWLDatatypeComplementOfSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_ON_DATATYPE, nullptr, mPartialFilteringStatementForOWLOnDatatypeSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_WITH_RESTRICTION, nullptr, mPartialFilteringStatementForOWLWithRestrictionSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_ON_PROPERTY, nullptr, mPartialFilteringStatementForOWLOnPropertySuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_SOME_VALUES_FROM, nullptr, mPartialFilteringStatementForOWLSomeValuesFromSuccessors);


				initPartialFilteringStatement(nullptr, PREFIX_OWL_ALL_VALUES_FROM, nullptr, mPartialFilteringStatementForOWLAllValuesFromSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_HAS_VALUE, nullptr, mPartialFilteringStatementForOWLHasValueSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_HAS_SELF, nullptr, mPartialFilteringStatementForOWLHasSelfSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_MIN_CARDINALITY, nullptr, mPartialFilteringStatementForOWLMinCardinalitySuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_MAX_CARDINALITY, nullptr, mPartialFilteringStatementForOWLMaxCardinalitySuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_CARDINALITY, nullptr, mPartialFilteringStatementForOWLCardinalitySuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_MIN_QUALIFIED_CARDINALITY, nullptr, mPartialFilteringStatementForOWLMinQualifiedCardinalitySuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_MAX_QUALIFIED_CARDINALITY, nullptr, mPartialFilteringStatementForOWLMaxQualifiedCardinalitySuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_QUALIFIED_CARDINALITY, nullptr, mPartialFilteringStatementForOWLQualifiedCardinalitySuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_ON_DATA_RANGE, nullptr, mPartialFilteringStatementForOWLOnDataRangeSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_ON_CLASS, nullptr, mPartialFilteringStatementForOWLOnClassSuccessors);




				initPartialFilteringStatement(nullptr, PREFIX_RDFS_SUBPROPERTY_OF, nullptr, mPartialFilteringStatementForRDFSSubPropertySuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_PROPERTY_CHAIN_AXIOM, nullptr, mPartialFilteringStatementForOWLPropertyChainAxiomSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_EQUIVALENT_PROPERTY, nullptr, mPartialFilteringStatementForOWLEquivalentPropertySuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_PROPERTY_DISJOINT_WITH, nullptr, mPartialFilteringStatementForOWLPropertyDisjointWithSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_RDFS_RANGE, nullptr, mPartialFilteringStatementForRDFSRangeSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_RDFS_DOMAIN, nullptr, mPartialFilteringStatementForRDFSDomainSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_INVERSE_OF, nullptr, mPartialFilteringStatementForOWLInverseOfSuccessors);


				initPartialFilteringStatement(nullptr, PREFIX_RDFS_SUBCLASS_OF, nullptr, mPartialFilteringStatementForRDFSSubClassOfSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_EQUIVALENT_CLASS, nullptr, mPartialFilteringStatementForOWLEquivalentClassesSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_DISJOINT_WITH, nullptr, mPartialFilteringStatementForOWLDisjointClassesSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_DISJOINT_UNION_OF, nullptr, mPartialFilteringStatementForOWLDisjointUnionSuccessors);





				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_ALL_DISJOINT_CLASSES, mPartialFilteringStatementForAllRDFTypeOfAllDisjointClasses);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_ALL_DISJOINT_PROPERTIES, mPartialFilteringStatementForAllRDFTypeOfAllDisjointProperties);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_ALL_DIFFERENT, mPartialFilteringStatementForAllRDFTypeOfAllDifferent);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_ALL_NEGATIVE_PROPERTY_ASSERTION, mPartialFilteringStatementForAllRDFTypeOfNegativePropertyAssertion);



				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_FUNCTIONAL_PROPERTY, mPartialFilteringStatementForAllRDFTypeOfFunctionalProperty);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_INVERSE_FUNCTIONAL_PROPERTY, mPartialFilteringStatementForAllRDFTypeOfInverseFunctionalProperty);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_REFLEXIVE_PROPERTY, mPartialFilteringStatementForAllRDFTypeOfReflexiveProperty);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_IRREFLEXIVE_PROPERTY, mPartialFilteringStatementForAllRDFTypeOfIrreflexiveProperty);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_SYMMETRIC_PROPERTY, mPartialFilteringStatementForAllRDFTypeOSymmetricProperty);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_ASYMMETRIC_PROPERTY, mPartialFilteringStatementForAllRDFTypeOfAsymmetricProperty);
				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, PREFIX_OWL_TRANSITIVE_PROPERTY, mPartialFilteringStatementForAllRDFTypeOfTransetiveProperty);


				initPartialFilteringStatement(nullptr, PREFIX_OWL_MEMBERS, nullptr, mPartialFilteringStatementForOWLMembers);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_DISTINCT_MEMBERS, nullptr, mPartialFilteringStatementForOWLDistinctMembers);

				initPartialFilteringStatement(nullptr, PREFIX_OWL_SOURCE_INDIVIDUAL, nullptr, mPartialFilteringStatementForOWLSourceIndividualSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_ASSERTION_PROPERTY, nullptr, mPartialFilteringStatementForOWLTargetIndividualSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_TARGET_INDIVIDUAL, nullptr, mPartialFilteringStatementForOWLTargetValueSuccessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_TARGET_VALUE, nullptr, mPartialFilteringStatementForOWLAssertionPropertySuccessors);


				initPartialFilteringStatement(nullptr, PREFIX_RDF_TYPE, nullptr, mPartialFilteringStatementForOWLAllRDFTypePredecessors);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_DIFFERENT_FROM, nullptr, mPartialFilteringStatementForOWLDifferentIndividuals);
				initPartialFilteringStatement(nullptr, PREFIX_OWL_SAME_AS, nullptr, mPartialFilteringStatementForOWLSameIndividuals);


				initPartialFilteringStatement(nullptr, nullptr, nullptr, mPartialFilteringStatementForOWLPropertyAssertions);
				initPartialFilteringStatement(nullptr, nullptr, nullptr, mPartialFilteringStatementForOWLDataRanges);
				initPartialFilteringStatement(nullptr, nullptr, nullptr, mPartialFilteringStatementForOWLClasses);
				initPartialFilteringStatement(nullptr, nullptr, nullptr, mPartialFilteringStatementForOWLFacets);
				initPartialFilteringStatement(nullptr, nullptr, nullptr, mPartialFilteringStatementForOWLAxioms);
				initPartialFilteringStatement(nullptr, nullptr, nullptr, mPartialFilteringStatementForOWLFacetSuccessors);







				initPredicateIndexMapping(PREFIX_OWL_INTERSECTION_OF, mOWLIntersectionOfPredicateIndex, &mDataRangePredicateIndexUriNodeHash, &mDataRangePredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_UNION_OF, mOWLUnionOfPredicateIndex, &mDataRangePredicateIndexUriNodeHash, &mDataRangePredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_DATATYPE_COMPLEMENT_OF, mOWLDatatypeComplementOfPredicateIndex, &mDataRangePredicateIndexUriNodeHash, &mDataRangePredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_ONE_OF, mOWLOneOfPredicateIndex, &mDataRangePredicateIndexUriNodeHash, &mDataRangePredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_ON_DATATYPE, mOWLOnDatatypePredicateIndex, &mDataRangePredicateIndexUriNodeHash, &mDataRangePredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_WITH_RESTRICTION, mOWLWithRestrictionPredicateIndex, &mDataRangePredicateIndexUriNodeHash, &mDataRangePredicateIndexHash);


				initPredicateIndexMapping(PREFIX_OWL_INTERSECTION_OF, mOWLIntersectionOfPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_UNION_OF, mOWLUnionOfPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_ONE_OF, mOWLOneOfPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_COMPLEMENT_OF, mOWLComplementOfPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_ON_PROPERTY, mOWLOnPropertyPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_SOME_VALUES_FROM, mOWLSomeValuesFromPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_ALL_VALUES_FROM, mOWLAllValuesFromPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_HAS_VALUE, mOWLHasValuePredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_HAS_SELF, mOWLHasSelfPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_ON_CLASS, mOWLOnClassPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_MIN_QUALIFIED_CARDINALITY, mOWLMinQualifiedCardinalityPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_MAX_QUALIFIED_CARDINALITY, mOWLMaxQualifiedCardinalityPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_QUALIFIED_CARDINALITY, mOWLQualifiedCardinalityPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_MIN_CARDINALITY, mOWLMinCardinalityPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_MAX_CARDINALITY, mOWLMaxCardinalityPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_CARDINALITY, mOWLCardinalityPredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_ON_DATA_RANGE, mOWLOnDataRangePredicateIndex, &mClassPredicateIndexUriNodeHash, &mClassPredicateIndexHash);




				initPredicateIndexMapping(PREFIX_RDFS_SUBCLASS_OF, mOWLClassAxiomSubClassOfPredicateIndex, &mClassBasedAxiomPredicateIndexUriNodeHash, &mClassBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_EQUIVALENT_CLASS, mOWLClassAxiomEquvialentClassPredicateIndex, &mClassBasedAxiomPredicateIndexUriNodeHash, &mClassBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_DISJOINT_WITH, mOWLClassAxiomDisjointWithPredicateIndex, &mClassBasedAxiomPredicateIndexUriNodeHash, &mClassBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_DISJOINT_UNION_OF, mOWLClassAxiomDisjointUnionOfPredicateIndex, &mClassBasedAxiomPredicateIndexUriNodeHash, &mClassBasedAxiomPredicateIndexHash);

				initPredicateIndexMapping(PREFIX_RDFS_SUBPROPERTY_OF, mOWLPredicateBasedAxiomSubPropertyOfPredicateIndex, &mPropertyBasedAxiomPredicateIndexUriNodeHash, &mPropertyBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_PROPERTY_CHAIN_AXIOM, mOWLPredicateBasedAxiomPropertyChainAxiomPredicateIndex, &mPropertyBasedAxiomPredicateIndexUriNodeHash, &mPropertyBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_EQUIVALENT_PROPERTY, mOWLPredicateBasedAxiomEquivalentPropertyPredicateIndex, &mPropertyBasedAxiomPredicateIndexUriNodeHash, &mPropertyBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_PROPERTY_DISJOINT_WITH, mOWLPredicateBasedAxiomPropertyDisjointWithPredicateIndex, &mPropertyBasedAxiomPredicateIndexUriNodeHash, &mPropertyBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_RDFS_DOMAIN, mOWLPredicateBasedAxiomDomainPredicateIndex, &mPropertyBasedAxiomPredicateIndexUriNodeHash, &mPropertyBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_RDFS_RANGE, mOWLPredicateBasedAxiomRangePredicateIndex, &mPropertyBasedAxiomPredicateIndexUriNodeHash, &mPropertyBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_INVERSE_OF, mOWLPredicateBasedAxiomInverseOfPredicateIndex, &mPropertyBasedAxiomPredicateIndexUriNodeHash, &mPropertyBasedAxiomPredicateIndexHash);
				initPredicateIndexMapping(PREFIX_RDF_TYPE, mRDFPredicateBasedAxiomTypeOfPredicateIndex, &mPropertyBasedAxiomPredicateIndexUriNodeHash, &mPropertyBasedAxiomPredicateIndexHash);


				initPredicateIndexMapping(PREFIX_OWL_FUNCTIONAL_PROPERTY, mOWLPredicateBasedAxiomTypeFunctionalPropertyObjectIndex, &mPropertyBasedAxiomObjectIndexUriNodeHash, &mPropertyBasedAxiomObjectIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_INVERSE_FUNCTIONAL_PROPERTY, mOWLPredicateBasedAxiomTypeInverseFunctionalPropertyObjectIndex, &mPropertyBasedAxiomObjectIndexUriNodeHash, &mPropertyBasedAxiomObjectIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_REFLEXIVE_PROPERTY, mOWLPredicateBasedAxiomTypeReflexivePropertyObjectIndex, &mPropertyBasedAxiomObjectIndexUriNodeHash, &mPropertyBasedAxiomObjectIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_IRREFLEXIVE_PROPERTY, mOWLPredicateBasedAxiomTypeIrreflexivePropertyObjectIndex, &mPropertyBasedAxiomObjectIndexUriNodeHash, &mPropertyBasedAxiomObjectIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_SYMMETRIC_PROPERTY, mOWLPredicateBasedAxiomTypeSymmetricPropertyObjectIndex, &mPropertyBasedAxiomObjectIndexUriNodeHash, &mPropertyBasedAxiomObjectIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_ASYMMETRIC_PROPERTY, mOWLPredicateBasedAxiomTypeAsymmetricPropertyObjectIndex, &mPropertyBasedAxiomObjectIndexUriNodeHash, &mPropertyBasedAxiomObjectIndexHash);
				initPredicateIndexMapping(PREFIX_OWL_TRANSITIVE_PROPERTY, mOWLPredicateBasedAxiomTypeTransitivePropertyObjectIndex, &mPropertyBasedAxiomObjectIndexUriNodeHash, &mPropertyBasedAxiomObjectIndexHash);


				mFacetUriNodeHash.insert(PREFIX_MIN_INCLUSIVE_FACET, librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_MIN_INCLUSIVE_FACET));
				mFacetUriNodeHash.insert(PREFIX_MIN_EXCLUSIVE_FACET, librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_MIN_EXCLUSIVE_FACET));
				mFacetUriNodeHash.insert(PREFIX_MAX_INCLUSIVE_FACET, librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_MAX_INCLUSIVE_FACET));
				mFacetUriNodeHash.insert(PREFIX_MAX_EXCLUSIVE_FACET, librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_MAX_EXCLUSIVE_FACET));
				mFacetUriNodeHash.insert(PREFIX_MAX_LENGTH_FACET, librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_MAX_LENGTH_FACET));
				mFacetUriNodeHash.insert(PREFIX_MIN_LENGTH_FACET, librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_MIN_LENGTH_FACET));
				mFacetUriNodeHash.insert(PREFIX_LENGTH_FACET, librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_LENGTH_FACET));
				mFacetUriNodeHash.insert(PREFIX_PATTERN_FACET, librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_PATTERN_FACET));
				mFacetUriNodeHash.insert(PREFIX_LANGUAGE_RANGE_FACET, librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_LANGUAGE_RANGE_FACET));

				mRDFNilNode = librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_RDF_NIL);


				mBooleanDatatypeUri = librdf_new_uri(redlandTriplesData->getRedlandWorld(), (const unsigned char*)PREFIX_XML_BOOLEAN_DATATYPE);


				initDatatypeMapping(redlandTriplesData, PREFIX_OWL_TOP_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_OWL_BOTTOM_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_OWL_REAL_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_OWL_RATIONAL_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_INTEGER_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_DECIMAL_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_NONNEGATIVEINTEGER_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_POSITIVEINTEGER_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_NONPOSITIVEINTEGER_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_NEGATIVEINTEGER_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_LONG_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_INT_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_SHORT_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_BYTE_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_UNSIGNEDLONG_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_UNSIGNEDINT_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_UNSIGNEDSHORT_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_UNSIGNEDBYTE_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_PLAINLITERAL_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_STRING_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_NORMALIZEDSTRING_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_TOKEN_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_NAME_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_NCNAME_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_NMTOKEN_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_LANGUAGE_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_BOOLEAN_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_DOUBLE_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_FLOAT_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_IRI_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_BINARYHEX_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_BINARYBASE64_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_XML_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_DATETIME_DATATYPE);
				initDatatypeMapping(redlandTriplesData, PREFIX_XML_DATETIMESTAMP_DATATYPE);



				addBaseNodeExpression(PREFIX_OWL_THING, mOntologyBuilder->getTopClass(), mClassNodeIdentifierDataHash, mClassNodeHandlingList, redlandTriplesData);
				addBaseNodeExpression(PREFIX_OWL_NOTHING, mOntologyBuilder->getBottomClass(), mClassNodeIdentifierDataHash, mClassNodeHandlingList, redlandTriplesData);
				addBaseNodeExpression(PREFIX_OWL_TOPOBJECTPROPERTY, mOntologyBuilder->getTopObjectProberty(), mObjectPropertyNodeIdentifierDataHash, mObjectPropertyNodeHandlingList, redlandTriplesData);
				addBaseNodeExpression(PREFIX_OWL_BOTTOMOBJECTPROPERTY, mOntologyBuilder->getBottomObjectProberty(), mObjectPropertyNodeIdentifierDataHash, mObjectPropertyNodeHandlingList, redlandTriplesData);
				addBaseNodeExpression(PREFIX_OWL_TOPDATAPROPERTY, mOntologyBuilder->getTopObjectProberty(), mDataPropertyNodeIdentifierDataHash, mDataPropertyNodeHandlingList, redlandTriplesData);
				addBaseNodeExpression(PREFIX_OWL_BOTTOMDATAPROPERTY, mOntologyBuilder->getTopObjectProberty(), mDataPropertyNodeIdentifierDataHash, mDataPropertyNodeHandlingList, redlandTriplesData);
				addBaseNodeExpression(PREFIX_OWL_TOP_DATATYPE, mOntologyBuilder->getTopDataRange(), mDatatypeNodeIdentifierDataHash, mDatatypeNodeHandlingList, redlandTriplesData);
				addBaseNodeExpression(PREFIX_OWL_BOTTOM_DATATYPE, mOntologyBuilder->getBottomDataRange(), mDatatypeNodeIdentifierDataHash, mDatatypeNodeHandlingList, redlandTriplesData);

				return this;
			}




			librdf_node* CConcreteOntologyRedlandTriplesDataExpressionMapper::initDatatypeMapping(CRedlandStoredTriplesData* redlandTriplesData, const char* datatypeIri) {
				librdf_node* datatypeNode = librdf_new_node_from_uri_string(redlandTriplesData->getRedlandWorld(), (const unsigned char*)datatypeIri);
				CRedlandNodeProcessingData* datatypeProcessingData = new CRedlandNodeProcessingData(datatypeNode, mOntologyBuilder->getDatatype(datatypeIri));
				mDatatypeNodeIdentifierDataHash.insert(datatypeNode, datatypeProcessingData);
				return datatypeNode;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::clearTripleDataProcessing() {


				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfOwlClass);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfOwlRestriction);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfOwlObjectProperty);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfOwlDataProperty);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfRDFSDatatype);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfOwlNamedIndividual);

				clearPartialFilteringStatement(mPartialFilteringStatementForRDFFirstSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForRDFRestSuccessors);

				clearPartialFilteringStatement(mPartialFilteringStatementForOWLComplementOfSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLOneOfSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLDatatypeComplementOfSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLOnDatatypeSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLWithRestrictionSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLFacets);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLFacetSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLOnPropertySuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLSomeValuesFromSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLAllValuesFromSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLHasValueSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLHasSelfSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLMinCardinalitySuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLMaxCardinalitySuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLCardinalitySuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLMinQualifiedCardinalitySuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLMaxQualifiedCardinalitySuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLQualifiedCardinalitySuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLOnDataRangeSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLOnClassSuccessors);

				clearPartialFilteringStatement(mPartialFilteringStatementForOWLObjectInverseOfPredecessors);

				clearPartialFilteringStatement(mPartialFilteringStatementForOWLIntersectionOfSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLUnionOfSuccessors);

				clearPartialFilteringStatement(mPartialFilteringStatementForOWLDataRanges);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLClasses);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLAxioms);

				clearPartialFilteringStatement(mPartialFilteringStatementForRDFSSubPropertySuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLPropertyChainAxiomSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLEquivalentPropertySuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLPropertyDisjointWithSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForRDFSRangeSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForRDFSDomainSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLInverseOfSuccessors);

				clearPartialFilteringStatement(mPartialFilteringStatementForRDFSSubClassOfSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLEquivalentClassesSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLDisjointClassesSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLDisjointUnionSuccessors);

				clearPartialFilteringStatement(mPartialFilteringStatementForOWLMembers);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLDistinctMembers);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfAllDisjointClasses);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfAllDisjointProperties);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfAllDifferent);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfNegativePropertyAssertion);


				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfFunctionalProperty);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfInverseFunctionalProperty);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfReflexiveProperty);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfIrreflexiveProperty);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOSymmetricProperty);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfAsymmetricProperty);
				clearPartialFilteringStatement(mPartialFilteringStatementForAllRDFTypeOfTransetiveProperty);


				clearPartialFilteringStatement(mPartialFilteringStatementForOWLSourceIndividualSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLTargetIndividualSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLTargetValueSuccessors);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLAssertionPropertySuccessors);

				clearPartialFilteringStatement(mPartialFilteringStatementForOWLAllRDFTypePredecessors);

				clearPartialFilteringStatement(mPartialFilteringStatementForOWLPropertyAssertions);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLDifferentIndividuals);
				clearPartialFilteringStatement(mPartialFilteringStatementForOWLSameIndividuals);


				for (QHash<const char*, librdf_node*>::const_iterator it = mFacetUriNodeHash.constBegin(), itEnd = mFacetUriNodeHash.constEnd(); it != itEnd; ++it) {
					librdf_free_node(it.value());
				}
				mFacetUriNodeHash.clear();

				mClassNodeIdentifierDataHash.clear();
				mClassNodeHandlingList.clear();
				mObjectPropertyNodeIdentifierDataHash.clear();
				mObjectPropertyNodeHandlingList.clear();
				mDataPropertyNodeIdentifierDataHash.clear();
				mDataPropertyNodeHandlingList.clear();
				mDatatypeNodeIdentifierDataHash.clear();
				mDataLiteralNodeIdentifierDataHash.clear();
				mDatatypeNodeHandlingList.clear();
				mNamedIndividualNodeIdentifierDataHash.clear();
				mNamedIndividualNodeHandlingList.clear();

				mDataFacetRestrictionHandlingContainerList.clear();

				if (mBooleanDatatypeUri) {
					librdf_free_uri(mBooleanDatatypeUri);
					mBooleanDatatypeUri = nullptr;
				}
				if (mRDFNilNode) {
					librdf_free_node(mRDFNilNode);
					mRDFNilNode = nullptr;
				}

				mClassPredicateIndexHash.clear();
				clearIndexUriHash(mClassPredicateIndexUriNodeHash);

				mDataRangePredicateIndexHash.clear();
				clearIndexUriHash(mDataRangePredicateIndexUriNodeHash);

				mClassBasedAxiomPredicateIndexHash.clear();
				clearIndexUriHash(mClassBasedAxiomPredicateIndexUriNodeHash);

				mDatatypeBasedAxiomPredicateIndexHash.clear();
				clearIndexUriHash(mDatatypeBasedAxiomPredicateIndexUriNodeHash);

				mPropertyBasedAxiomPredicateIndexHash.clear();
				clearIndexUriHash(mPropertyBasedAxiomPredicateIndexUriNodeHash);

				mPropertyBasedAxiomObjectIndexHash.clear();
				clearIndexUriHash(mPropertyBasedAxiomObjectIndexUriNodeHash);
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::clearPartialFilteringStatement(librdf_statement*& partialFilteringStatement) {
				if (partialFilteringStatement) {
					librdf_free_statement(partialFilteringStatement);
					partialFilteringStatement = nullptr;
				}
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::clearIndexUriHash(QHash<cint64, librdf_node*>& indexUriNodeHash) {
				for (QHash<cint64, librdf_node*>::const_iterator it = indexUriNodeHash.constBegin(), itEnd = indexUriNodeHash.constEnd(); it != itEnd; ++it) {
					librdf_node* node = it.value();
					librdf_free_node(node);
				}
				indexUriNodeHash.clear();
				return this;
			}




			librdf_node* CConcreteOntologyRedlandTriplesDataExpressionMapper::initPredicateIndexMapping(const char* predicateUriChar, cint64 predicateIndex, QHash<cint64, librdf_node*>* predicateIndexUriNodeHash, QHash<CRedlandNodeHasher, cint64>* predicateNodeIndexHash) {
				librdf_uri* predicateUri = librdf_new_uri(mRedlandTriplesData->getRedlandWorld(), (const unsigned char*)predicateUriChar);
				librdf_node* predicateNode = librdf_new_node_from_uri(mRedlandTriplesData->getRedlandWorld(), predicateUri);
				predicateIndexUriNodeHash->insert(predicateIndex, predicateNode);
				predicateNodeIndexHash->insert(CRedlandNodeHasher(predicateNode), predicateIndex);
				return predicateNode;
			}



			librdf_statement* CConcreteOntologyRedlandTriplesDataExpressionMapper::initPartialFilteringStatement(const char* subjectFilterUri, const char* predicateFilterUri, const char* objectFilterUri, librdf_statement*& partialFilteringStatement) {
				if (partialFilteringStatement) {
					librdf_free_statement(partialFilteringStatement);
					partialFilteringStatement = nullptr;
				}
				librdf_statement* partial_statement = librdf_new_statement(mRedlandTriplesData->getRedlandWorld());
				librdf_node* subject = nullptr;
				if (subjectFilterUri) {
					subject = librdf_new_node_from_uri_string(mRedlandTriplesData->getRedlandWorld(), (const unsigned char*)subjectFilterUri);
					librdf_statement_set_subject(partial_statement, subject);
				}
				librdf_node* predicate = nullptr;
				if (predicateFilterUri) {
					predicate = librdf_new_node_from_uri_string(mRedlandTriplesData->getRedlandWorld(), (const unsigned char*)predicateFilterUri);
					librdf_statement_set_predicate(partial_statement, predicate);
				}
				librdf_node* object = nullptr;
				if (objectFilterUri) {
					object = librdf_new_node_from_uri_string(mRedlandTriplesData->getRedlandWorld(), (const unsigned char*)objectFilterUri);
					librdf_statement_set_object(partial_statement, object);
				}
				partialFilteringStatement = partial_statement;
				return partial_statement;
			}


			librdf_statement* CConcreteOntologyRedlandTriplesDataExpressionMapper::getAdaptedPartialFilteringStatement(librdf_node* subjectFilterNode, librdf_node* predicateFilterNode, librdf_node* objectFilterNode, librdf_statement*& partialFilteringStatement) {
				librdf_node* subject = nullptr;
				if (subjectFilterNode) {
					subject = librdf_new_node_from_node(subjectFilterNode);
					librdf_statement_set_subject(partialFilteringStatement, subject);
				}
				librdf_node* predicate = nullptr;
				if (predicateFilterNode) {
					predicate = librdf_new_node_from_node(predicateFilterNode);
					librdf_statement_set_predicate(partialFilteringStatement, predicate);
				}
				librdf_node* object = nullptr;
				if (objectFilterNode) {
					object = librdf_new_node_from_node(objectFilterNode);
					librdf_statement_set_object(partialFilteringStatement, object);
				}
				return partialFilteringStatement;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLIntersectionOfSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLIntersectionOfSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLUnionOfSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLUnionOfSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLComplementOfSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLComplementOfSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLOneOfSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLOneOfSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLDatatypeComplementOfSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLDatatypeComplementOfSuccessors)), 2);
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLOnDatatypeSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLOnDatatypeSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLWithRestrictionSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLWithRestrictionSuccessors)), 2);
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLOnPropertySuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLOnPropertySuccessors)), 2);
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLSomeValuesFromSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLSomeValuesFromSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLAllValuesFromSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLAllValuesFromSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLHasValueSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLHasValueSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLHasSelfSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLHasSelfSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLMinCardinalitySuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLMinCardinalitySuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLMaxCardinalitySuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLMaxCardinalitySuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLCardinalitySuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLCardinalitySuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLMinQualifiedCardinalitySuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLMinQualifiedCardinalitySuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLMaxQualifiedCardinalitySuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLMaxQualifiedCardinalitySuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLQualifiedCardinalitySuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLQualifiedCardinalitySuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLOnDataRangeSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLOnDataRangeSuccessors)), 2);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLOnClassSuccessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(currentNode, nullptr, nullptr, mPartialFilteringStatementForOWLOnClassSuccessors)), 2);
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLObjectInverseOfPredecessorNodesStream(librdf_node* currentNode) {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(getAdaptedPartialFilteringStatement(nullptr, nullptr, currentNode, mPartialFilteringStatementForOWLObjectInverseOfPredecessors)), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLClassInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfOwlClass), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLRestrictionInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfOwlRestriction), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLObjectPropertyInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfOwlObjectProperty), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLDatatypePropertyInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfOwlDataProperty), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getRDFSDatatypeInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfRDFSDatatype), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLNamedIndividualInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfOwlNamedIndividual), 0);
			}



			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLAllDisjointClassesInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfAllDisjointClasses), 0);
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLAllDisjointPropertiesInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfAllDisjointProperties), 0);
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLAllDifferentInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfAllDifferent), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLAllNegativePropertyAssertionInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfNegativePropertyAssertion), 0);
			}




			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLFunctionalPropertyInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfFunctionalProperty), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLInverseFunctionalPropertyInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfInverseFunctionalProperty), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLReflexivePropertyInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfReflexiveProperty), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLIrreflexivePropertyInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfIrreflexiveProperty), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLSymmetricPropertyInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOSymmetricProperty), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLAsymmetricPropertyInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfAsymmetricProperty), 0);
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::getOWLTransetivePropertyInstanceNodesStream() {
				return createRedlandNodeStreamWrapper()->init(getPartialStatementFilteredTripleStream(mPartialFilteringStatementForAllRDFTypeOfTransetiveProperty), 0);
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandTripleStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::createRedlandTripleStreamWrapper() {
				CRedlandTripleStream* nextWrapper = mFreeTripleStreamWrappers;
				if (mFreeTripleStreamWrappers) {
					mFreeTripleStreamWrappers = mFreeTripleStreamWrappers->getNext();
					nextWrapper->clearNext();
				} else {
					nextWrapper = new CRedlandTripleStream();
				}
				return nextWrapper;
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::releaseRedlandTripleStreamWrapper(CRedlandTripleStream* streamWrapper) {
				mFreeTripleStreamWrappers = streamWrapper->clear()->append(mFreeTripleStreamWrappers);
				return this;
			}


			CConcreteOntologyRedlandTriplesDataExpressionMapper::CRedlandNodeStream* CConcreteOntologyRedlandTriplesDataExpressionMapper::createRedlandNodeStreamWrapper() {
				CRedlandNodeStream* nextWrapper = mFreeNodeStreamWrappers;
				if (mFreeNodeStreamWrappers) {
					mFreeNodeStreamWrappers = mFreeNodeStreamWrappers->getNext();
					nextWrapper->clearNext();
				} else {
					nextWrapper = new CRedlandNodeStream();
				}
				return nextWrapper;
			}

			CConcreteOntologyRedlandTriplesDataExpressionMapper* CConcreteOntologyRedlandTriplesDataExpressionMapper::releaseRedlandNodeStreamWrapper(CRedlandNodeStream* streamWrapper) {
				mFreeNodeStreamWrappers = streamWrapper->clear([this](CRedlandTripleStream* streamWrapper) { releaseRedlandTripleStreamWrapper(streamWrapper); })->append(mFreeNodeStreamWrappers);
				return this;
			}


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
