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

#include "CRedlandStoredTriplesSimpleABoxAssertionsAccessor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			CRedlandStoredTriplesSimpleABoxAssertionsAccessor::CRedlandStoredTriplesSimpleABoxAssertionsAccessor(CConcreteOntology* ontology, CRedlandStoredTriplesIndividualAssertionIndexCache* individualAssertionIndexCache, CRedlandStoredTriplesData* tripleData) {
				mTripleData = tripleData;
				mIndividualAssertionIndexCache = individualAssertionIndexCache;
				mOntology = ontology;

				mAnonymousOntologyIdentifier = mOntology->getOntologyName() + ":";
				if (!mAnonymousOntologyIdentifier.startsWith("_:")) {
					mAnonymousOntologyIdentifier = "_:" + mAnonymousOntologyIdentifier;
				}
			}

			CRedlandStoredTriplesSimpleABoxAssertionsAccessor::~CRedlandStoredTriplesSimpleABoxAssertionsAccessor() {
			}



			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::hasIndividualId(const QString& indiName) {
				cint64 indiId = mIndividualAssertionIndexCache->getIndividualIdFromIndividualName(indiName);
				return indiId >= 0;
			}


			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::visitIndividualId(const QString& indiName, function<bool(cint64 indiId)> visitFunc) {
				cint64 indiId = mIndividualAssertionIndexCache->getIndividualIdFromIndividualName(indiName);
				if (indiId >= 0) {
					visitFunc(indiId);
					return true;
				}
				return false;
			}

			QString CRedlandStoredTriplesSimpleABoxAssertionsAccessor::getAnonymousResolvedIndividualName(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData) {
				QString indiName = indiCacheData->getIndividualName();
				if (indiCacheData->isAnonymous()) {
					if (indiName.startsWith("_:")) {
						indiName = indiName.mid(2);
					}
					return mAnonymousOntologyIdentifier + indiName;
				}
				return indiName;
			}


			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::visitIndividualName(cint64 individualId, COntologyTriplesIndividualNamesVisitor* visitor) {
				CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData = mIndividualAssertionIndexCache->getIndividualAssertionIndexCacheData(individualId);
				if (indiCacheData) {
					visitor->visitIndividualName(getAnonymousResolvedIndividualName(indiCacheData), this);
					return true;
				}
				return false;
			}


			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::visitIndividualName(cint64 individualId, function<bool(const QString& indiName)> visitFunc) {
				CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData = mIndividualAssertionIndexCache->getIndividualAssertionIndexCacheData(individualId);
				if (indiCacheData) {
					visitFunc(getAnonymousResolvedIndividualName(indiCacheData));
					return true;
				}
				return false;
			}


			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::visitIndividualAnonymity(cint64 individualId, function<bool(bool anonymous)> visitFunc) {
				CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData = mIndividualAssertionIndexCache->getIndividualAssertionIndexCacheData(individualId);
				if (indiCacheData) {
					visitFunc(indiCacheData->isAnonymous());
					return true;
				}
				return false;
			}




			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::visitIndividualAssertions(cint64 individualId, COntologyTriplesIndividualAssertionsVisitor* visitor, bool visitConceptAssertions, bool visitRoleAssertions, bool visitReverseRoleAssertions, bool visitDataAssertions) {
				CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData = mIndividualAssertionIndexCache->getIndividualAssertionIndexCacheData(individualId);
				if (indiCacheData) {

					librdf_node* indiNode = librdf_new_node_from_node(indiCacheData->getIndividualNode());

					bool continueVisiting = true;

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
					continueVisiting &= visitor->visitIndividualName(indiCacheData->getIndividualName(), this);
#endif

					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* repIndiAssData = indiCacheData->getCurrentRepresentativeIndividualAssertionsIndexData();
					if (repIndiAssData) {
						CRedlandStoredTriplesRepresentativeTagTypeConceptsSet* repConSet = repIndiAssData->getRepresentativeTypeConceptsSet();
						if (visitConceptAssertions && repConSet && repConSet->getTagElementCount() > 0) {
							for (QMap<cint64, CConcept*>::const_iterator it = repConSet->getTagDataMap()->constBegin(), itEnd = repConSet->getTagDataMap()->constEnd(); continueVisiting && it != itEnd; ++it) {
								CConcept* concept = it.value();
								continueVisiting &= visitor->visitConceptAssertion(concept, this);
							}
						}

						CRedlandStoredTriplesRepresentativeTagRolesSet* repOutRoleSet = repIndiAssData->getRepresentativeOutgoingObjectRolesSet();
						if (visitRoleAssertions && repOutRoleSet && repOutRoleSet->getTagElementCount() > 0) {
							for (QMap<cint64, CRole*>::const_iterator it = repOutRoleSet->getTagDataMap()->constBegin(), itEnd = repOutRoleSet->getTagDataMap()->constEnd(); continueVisiting && it != itEnd; ++it) {
								CRole* role = it.value();

								librdf_model* model = mTripleData->getRedlandIndexedModel();
								librdf_statement *partial_statement = librdf_new_statement(mTripleData->getRedlandWorld());

								librdf_statement_set_predicate(partial_statement, librdf_new_node_from_node(getRolePredicateNode(role)));
								librdf_statement_set_subject(partial_statement, librdf_new_node_from_node(indiNode));

								librdf_stream* stream = librdf_model_find_statements(model, partial_statement);
								while (!librdf_stream_end(stream) && continueVisiting) {
									librdf_statement* statement = librdf_stream_get_object(stream);
									librdf_node* objectNode = librdf_statement_get_object(statement);

									CRedlandStoredTriplesIndividualAssertionIndexCacheData* otherIndiCacheData = mIndividualAssertionIndexCache->getIndividualAssertionIndexCacheData(objectNode, false);
									cint64 otherIndiId = otherIndiCacheData->getIndividualId();
									continueVisiting &= visitor->visitRoleAssertion(role, otherIndiId, this);
									librdf_stream_next(stream);
								}
								librdf_free_stream(stream);
								librdf_free_statement(partial_statement);
							}
						}


						CRedlandStoredTriplesRepresentativeTagRolesSet* repIncRoleSet = repIndiAssData->getRepresentativeIncomingObjectRolesSet();
						if (visitReverseRoleAssertions && repIncRoleSet && repIncRoleSet->getTagElementCount() > 0) {
							for (QMap<cint64, CRole*>::const_iterator it = repIncRoleSet->getTagDataMap()->constBegin(), itEnd = repIncRoleSet->getTagDataMap()->constEnd(); continueVisiting && it != itEnd; ++it) {
								CRole* role = it.value();

								librdf_model* model = mTripleData->getRedlandIndexedModel();
								librdf_statement *partial_statement = librdf_new_statement(mTripleData->getRedlandWorld());

								librdf_statement_set_predicate(partial_statement, librdf_new_node_from_node(getRolePredicateNode(role)));
								librdf_statement_set_object(partial_statement, librdf_new_node_from_node(indiNode));

								librdf_stream* stream = librdf_model_find_statements(model, partial_statement);
								while (!librdf_stream_end(stream) && continueVisiting) {
									librdf_statement* statement = librdf_stream_get_object(stream);
									librdf_node* subjectNode = librdf_statement_get_subject(statement);

									cint64 otherIndiId = mIndividualAssertionIndexCache->getIndividualIdFromIndividualNode(subjectNode);
									continueVisiting &= visitor->visitReverseRoleAssertion(role, otherIndiId, this);
									librdf_stream_next(stream);
								}
								librdf_free_stream(stream);
								librdf_free_statement(partial_statement);
							}
						}



						CRedlandStoredTriplesRepresentativeTagRolesSet* repDataRoleSet = repIndiAssData->getRepresentativeOutgoingDataRolesSet();
						if (visitDataAssertions && repDataRoleSet && repDataRoleSet->getTagElementCount() > 0) {


							for (QMap<cint64, CRole*>::const_iterator it = repDataRoleSet->getTagDataMap()->constBegin(), itEnd = repDataRoleSet->getTagDataMap()->constEnd(); continueVisiting && it != itEnd; ++it) {
								CRole* role = it.value();

								librdf_model* model = mTripleData->getRedlandIndexedModel();
								librdf_statement *partial_statement = librdf_new_statement(mTripleData->getRedlandWorld());

								librdf_statement_set_predicate(partial_statement, librdf_new_node_from_node(getRolePredicateNode(role)));
								librdf_statement_set_subject(partial_statement, librdf_new_node_from_node(indiNode));

								librdf_stream* stream = librdf_model_find_statements(model, partial_statement);
								while (!librdf_stream_end(stream) && continueVisiting) {
									librdf_statement* statement = librdf_stream_get_object(stream);
									librdf_node* objectNode = librdf_statement_get_object(statement);

									const char* literalValue = (const char*)librdf_node_get_literal_value(objectNode);
									librdf_uri* datatypeUri = librdf_node_get_literal_value_datatype_uri(objectNode);
									CDatatype* datatype = getDatatypeFromDatatypeUri(datatypeUri);

									CDataLiteral dataLiteral(nullptr);
									dataLiteral.initDataLiteral(QString::fromUtf8(literalValue), datatype);

									continueVisiting &= visitor->visitDataAssertion(role, &dataLiteral, this);
									librdf_stream_next(stream);
								}
								librdf_free_stream(stream);
								librdf_free_statement(partial_statement);
							}
						}

					}
					librdf_free_node(indiNode);
					return true;
				}
				return false;
			}


			librdf_node* CRedlandStoredTriplesSimpleABoxAssertionsAccessor::getRolePredicateNode(CRole* role) {
				librdf_node*& predicateNode = mRolePredicateNodeHash[role];
				if (!predicateNode) {
					QByteArray predicateCharData = CIRIName::getRecentIRIName(role->getNameLinker()).toUtf8();
					predicateNode = librdf_new_node_from_uri_string(mTripleData->getRedlandWorld(), (const unsigned char*)predicateCharData.constData());
				}
				return predicateNode;
			}


			CDatatype* CRedlandStoredTriplesSimpleABoxAssertionsAccessor::getDatatypeFromDatatypeUri(librdf_uri* uri) {
				CDatatype* datatype = nullptr;
				initializeDatatypeMaps();
				if (uri) {
					datatype = mDatatypeIriDatatypeHash.value(CRedlandUriHasher(uri));
				} else {
					datatype = mDefaultDatatype;
				}
				return datatype;
			}



			librdf_uri* CRedlandStoredTriplesSimpleABoxAssertionsAccessor::getDatatypeUriFromDatatype(CDatatype* datatype) {
				librdf_uri* datatypeUri = nullptr;
				initializeDatatypeMaps();
				if (datatype) {
					datatypeUri = mDatatypeDatatypeIriHash.value(datatype);
				} else {
					datatypeUri = mDefaultDatatypeUri;
				}
				return datatypeUri;
			}

			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::initializeDatatypeMaps() {
				if (!mDatatypeHashInitialized) {
					CBUILDHASH<CDatatype*, CDatatypeExpression*>* datatypeDatatypeExpHash = mOntology->getDataBoxes()->getExpressionDataBoxMapping()->getDatatypeDatatypeExpessionHash();
					for (CBUILDHASH<CDatatype*, CDatatypeExpression*>::const_iterator it = datatypeDatatypeExpHash->constBegin(), itEnd = datatypeDatatypeExpHash->constEnd(); it != itEnd; ++it) {
						CDatatype* datatype = it.key();

						QByteArray datatypeCharData = datatype->getDatatypeIRI().toUtf8();
						librdf_uri* datatypeUri = librdf_new_uri(mTripleData->getRedlandWorld(), (const unsigned char*)datatypeCharData.constData());
						mDatatypeIriDatatypeHash.insert(CRedlandUriHasher(datatypeUri), datatype);

						mDatatypeDatatypeIriHash.insert(datatype, datatypeUri);

						if (datatype->getDatatypeType() == CDatatype::DT_STRING) {
							mDefaultDatatype = datatype;
							mDefaultDatatypeUri = datatypeUri;
						}
					}
					mDatatypeHashInitialized = true;
					return true;
				}
				return false;
			}



			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::visitAllAssertedConcepts(COntologyTriplesAssertedTypesVisitor* visitor) {
				bool visited = false;
				for (CConcept* concept : *mIndividualAssertionIndexCache->getAssertedConceptSet()) {
					visited = true;
					if (!visitor->visitAssertedConcept(concept, this)) {
						return visited;
					}
				}
				return visited;
			}

			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::visitAllAssertedObjectRoles(COntologyTriplesAssertedTypesVisitor* visitor) {
				bool visited = false;
				for (CRole* role : *mIndividualAssertionIndexCache->getAssertedObjectRoleSet()) {
					visited = true;
					if (!visitor->visitAssertedObjectRole(role, this)) {
						return visited;
					}
				}
				return visited;
			}

			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::visitAllAssertedDataRoles(COntologyTriplesAssertedTypesVisitor* visitor) {
				bool visited = false;
				for (CRole* role : *mIndividualAssertionIndexCache->getAssertedDataRoleSet()) {
					visited = true;
					if (!visitor->visitAssertedDataRole(role, this)) {
						return visited;
					}
				}
				return visited;
			}


			cint64 CRedlandStoredTriplesSimpleABoxAssertionsAccessor::getMaxIndexedIndividualId() {
				return mIndividualAssertionIndexCache->getNextIndividualId() - 1;
			}

			cint64 CRedlandStoredTriplesSimpleABoxAssertionsAccessor::getIndexedIndividualCount() {
				return mIndividualAssertionIndexCache->getNextIndividualId();
			}


			bool CRedlandStoredTriplesSimpleABoxAssertionsAccessor::visitDataValueIndividuals(CRole* dataRole, CDataLiteral* dataLiteral, function<bool(cint64 indiId)> visitFunc) {

				bool continueVisiting = true;

				librdf_model* model = mTripleData->getRedlandIndexedModel();
				librdf_statement *partial_statement = librdf_new_statement(mTripleData->getRedlandWorld());

				librdf_statement_set_predicate(partial_statement, librdf_new_node_from_node(getRolePredicateNode(dataRole)));

				QByteArray dataLiteralCharData = dataLiteral->getLexicalDataLiteralValueString().toUtf8();

				librdf_uri* datatypeUri = getDatatypeUriFromDatatype(dataLiteral->getDatatype());
				if (datatypeUri == mDefaultDatatypeUri) {
					datatypeUri = nullptr;
				}

				librdf_statement_set_object(partial_statement, librdf_new_node_from_typed_literal(mTripleData->getRedlandWorld(), (const unsigned char*)dataLiteralCharData.constData(), nullptr, datatypeUri));

				librdf_stream* stream = librdf_model_find_statements(model, partial_statement);
				while (!librdf_stream_end(stream) && continueVisiting) {
					librdf_statement* statement = librdf_stream_get_object(stream);
					librdf_node* subjectNode = librdf_statement_get_subject(statement);

					cint64 indiId = mIndividualAssertionIndexCache->getIndividualIdFromIndividualNode(subjectNode);

					continueVisiting &= visitFunc(indiId);
					librdf_stream_next(stream);
				}
				librdf_free_stream(stream);
				librdf_free_statement(partial_statement);
				return true;

			}



		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
