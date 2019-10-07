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

#include "CRedlandStoredTriplesIndividualAssertionConvertionIndexer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {



			CRedlandStoredTriplesIndividualAssertionConvertionIndexer::CRedlandStoredTriplesIndividualAssertionConvertionIndexer() {
			}

			CRedlandStoredTriplesIndividualAssertionConvertionIndexer::~CRedlandStoredTriplesIndividualAssertionConvertionIndexer() {
			}


			bool CRedlandStoredTriplesIndividualAssertionConvertionIndexer::indexABoxIndividuals(CConcreteOntology* updateConcreteOntology, COntologyTriplesData* ontologyTripleData) {
				
				mPreprocessContext = new CPreProcessContextBase(updateConcreteOntology, updateConcreteOntology->getConfiguration());

				mAnonymousOntologyIdentifier = updateConcreteOntology->getOntologyName() + ":";
				if (!mAnonymousOntologyIdentifier.startsWith("_:")) {
					mAnonymousOntologyIdentifier = "_:" + mAnonymousOntologyIdentifier;
				}

				bool newTriplesMapped = false;
				for (CTriplesData* tripleData : *ontologyTripleData->getAllTriplesData()) {
					CRedlandStoredTriplesData* redlandTriplesData = dynamic_cast<CRedlandStoredTriplesData*>(tripleData);

					librdf_world* world = redlandTriplesData->getRedlandWorld();
					mNextIndiId = 0;
					mDatatypeHashInitialized = false;
					mMemMan = updateConcreteOntology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();


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
						mNextIndiId = qMax(maxIndividualId, mNextIndiId);
						for (CIndividual* indi : *activeIndiSet) {
							maxIndividualId = qMax(indi->getIndividualID(), maxIndividualId);
							QByteArray indiCharData = CIRIName::getRecentIRIName(indi->getIndividualNameLinker()).toUtf8();
							librdf_node* indiNode = librdf_new_node_from_uri_string(world, (const unsigned char*)indiCharData.constData());

							CIndividualNodeData* indiData = new CIndividualNodeData(indiNode, indi);
							mIndividualNodeDataHash.insert(CRedlandNodeHasher(indiNode), indiData);
							mNextIndiId = qMax(indi->getIndividualID(), mNextIndiId);
						}
						mNextIndiId++;
					}


					cint64 conceptAssertionCount = 0;
					cint64 objectPropertyAssertionCount = 0;
					cint64 dataPropertyAssertionCount = 0;

					CXLinker<librdf_statement*>* statementLinker = redlandTriplesData->getRedlandStatementLinker();
					if (statementLinker) {


					//librdf_stream* stream = librdf_model_as_stream(redlandTriplesData->getRedlandIndexedModel());
					//if (stream) {
						CIndividualNodeData* lastIndiData = nullptr;
						librdf_node* lastSubjectNode = nullptr;

						//while (!librdf_stream_end(stream)) {

						while (statementLinker) {

							//librdf_statement* statement = librdf_stream_get_object(stream);
							librdf_statement* statement = statementLinker->getData();
							librdf_node* predicateNode = librdf_statement_get_predicate(statement);
							librdf_node* objectNode = librdf_statement_get_object(statement);
							librdf_node* subjectNode = librdf_statement_get_subject(statement);

							if (librdf_node_equals(predicateNode, rdfTypePredicate)) {
								if (librdf_node_equals(objectNode, namedIndividualObject)) {
									lastIndiData = getIndividualData(subjectNode, lastIndiData, lastSubjectNode, updateConcreteOntology);
									lastIndiData->mIndividual->setAnonymousIndividual(false);
								} else {
									CConceptNodeData* conceptData = mConceptNodeDataHash.value(CRedlandNodeHasher(objectNode));
									if (conceptData) {
										lastIndiData = getIndividualData(subjectNode, lastIndiData, lastSubjectNode, updateConcreteOntology);
										CConceptAssertionLinker* conceptAssertionLinker = CObjectAllocator<CConceptAssertionLinker>::allocateAndConstruct(mMemMan);
										conceptAssertionLinker->initNegLinker(conceptData->mConcept, false);
										lastIndiData->mIndividual->addAssertionConceptLinker(conceptAssertionLinker);
										conceptAssertionCount++;
									}
									if (lastIndiData && librdf_node_is_resource(subjectNode)) {
										lastIndiData->mIndividual->setAnonymousIndividual(false);
									}
								}
							} else {
								CRoleNodeData* roleData = mRoleNodeDataHash.value(CRedlandNodeHasher(predicateNode));
								if (roleData) {
									lastIndiData = getIndividualData(subjectNode, lastIndiData, lastSubjectNode, updateConcreteOntology);
									if (librdf_node_is_resource(subjectNode)) {
										lastIndiData->mIndividual->setAnonymousIndividual(false);
									}
									if (roleData->mDataRole) {
										if (librdf_node_is_literal(objectNode)) {
											const char* literalValue = (const char*)librdf_node_get_literal_value(objectNode);
											librdf_uri* datatypeUri = librdf_node_get_literal_value_datatype_uri(objectNode);
											CDatatype* datatype = getDatatypeFromDatatypeUri(datatypeUri, updateConcreteOntology, redlandTriplesData);

											CDataLiteral* dataLiteral = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(mMemMan, updateConcreteOntology->getDataBoxes()->getBoxContext());
											dataLiteral->initDataLiteral(QString::fromUtf8(literalValue), datatype);

											createDataLiteralValue(dataLiteral, updateConcreteOntology);

											CDataAssertionLinker* dataAssertionLinker = CObjectAllocator<CDataAssertionLinker>::allocateAndConstruct(mMemMan);
											dataAssertionLinker->initDataAssertionLinker(roleData->mRole, dataLiteral);

											lastIndiData->mIndividual->addAssertionDataLinker(dataAssertionLinker);
											dataPropertyAssertionCount++;
										}
									} else {
										if (!librdf_node_is_literal(objectNode)) {
											CIndividualNodeData* targetIndiNode = getIndividualData(objectNode, updateConcreteOntology);
											if (librdf_node_is_resource(objectNode)) {
												targetIndiNode->mIndividual->setAnonymousIndividual(false);
											}
											CRoleAssertionLinker* roleAssertionLinker = CObjectAllocator<CRoleAssertionLinker>::allocateAndConstruct(mMemMan);
											roleAssertionLinker->initRoleAssertionLinker(roleData->mRole, targetIndiNode->mIndividual);
											lastIndiData->mIndividual->addAssertionRoleLinker(roleAssertionLinker);
											CReverseRoleAssertionLinker* reverseRoleAssertionLinker = CObjectAllocator<CReverseRoleAssertionLinker>::allocateAndConstruct(mMemMan);
											reverseRoleAssertionLinker->initReverseRoleAssertionLinker(roleAssertionLinker, lastIndiData->mIndividual);
											targetIndiNode->mIndividual->addReverseAssertionRoleLinker(reverseRoleAssertionLinker);
											objectPropertyAssertionCount++;
										}
									}
								}
							}

							//librdf_stream_next(stream);
							statementLinker = statementLinker->getNext();
						}
					}


					updateConcreteOntology->getABox()->setNextIndividualId(mNextIndiId);


					LOG(INFO, "::Konclude::Reasoner::TripleEncodedAssertionsIndexer", QString("Identified %1 concept, %2 object property, and %3 data property assertions for overall %4 individuals").arg(conceptAssertionCount).arg(objectPropertyAssertionCount).arg(dataPropertyAssertionCount).arg(mNextIndiId), this);

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
					//QString modelString = mIndividualAssertionIndexCache->getRepresentativeCacheModelString();
					//QFile modelFile("./Debugging/RepresentativeCache/assertion-index-data.txt");
					//if (modelFile.open(QIODevice::WriteOnly)) {
					//	modelFile.write(modelString.toUtf8());
					//}
#endif

				}

				delete mPreprocessContext;

				return newTriplesMapped;
			}


			CDataLiteralValue* CRedlandStoredTriplesIndividualAssertionConvertionIndexer::createDataLiteralValue(CDataLiteral* dataLiteral, CConcreteOntology* ontology) {
				CDataLiteralValue* dataLiteralValue = mDataLiteralNormalizer.createPreprocessedDataLiteralValue(dataLiteral, mPreprocessContext, ontology->getDataBoxes()->getBoxContext());
				return dataLiteralValue;
			}



			CRedlandStoredTriplesIndividualAssertionConvertionIndexer::CIndividualNodeData* CRedlandStoredTriplesIndividualAssertionConvertionIndexer::getIndividualData(librdf_node* indiNode, CConcreteOntology* ontology) {
				CIndividualNodeData* indiNodeData = mIndividualNodeDataHash.value(indiNode);
				if (!indiNodeData) {
					librdf_node* nodeCopy = librdf_new_node_from_node(indiNode);
					CIndividual* nextIndi = CObjectAllocator<CIndividual>::allocateAndConstruct(mMemMan);
					nextIndi->initIndividual(mNextIndiId++);
					nextIndi->setAnonymousIndividual(true);
					indiNodeData = new CIndividualNodeData(nodeCopy, nextIndi);
					mIndividualNodeDataHash.insert(nodeCopy, indiNodeData);


					const char* indiNameCharPointer = nullptr;
					bool blankNode = false;
					if (!librdf_node_is_blank(indiNode)) {
						librdf_uri* uri = librdf_node_get_uri(indiNode);
						indiNameCharPointer = (const char*)librdf_uri_as_string(uri);
					} else {
						indiNameCharPointer = (const char*)librdf_node_get_blank_identifier(indiNode);
						blankNode = true;
					}

					CIRIName* newName = CObjectAllocator<CIRIName>::allocateAndConstruct(mMemMan);
					QString indiName = QString::fromUtf8(indiNameCharPointer);
					if (blankNode) {
						if (!indiName.startsWith("_:")) {
							indiName = mAnonymousOntologyIdentifier + indiName;
						} else {
							indiName = mAnonymousOntologyIdentifier + indiName.mid(2);
						}
					}
					newName->init(indiName);
					CLinker<CName*>* newNameLinker = CObjectAllocator< CLinker<CName*> >::allocateAndConstruct(mMemMan);
					newNameLinker->init(newName);
					nextIndi->addNameLinker(newNameLinker);

					CIndividualProcessData* indiProcData = new CIndividualProcessData();
					indiProcData->initIndividualProcessExtensionData();
					nextIndi->setIndividualData(indiProcData);

					ontology->getABox()->getIndividualVector(true)->setData(nextIndi->getIndividualID(), nextIndi);
					ontology->getABox()->getActiveIndividualSet(true)->insert(nextIndi);

					ontology->getStringMapping()->getIndividualNameIndividualMappingHash()->insert(indiName, nextIndi);
				}
				return indiNodeData;
			}


			CRedlandStoredTriplesIndividualAssertionConvertionIndexer::CIndividualNodeData* CRedlandStoredTriplesIndividualAssertionConvertionIndexer::getIndividualData(librdf_node* subjectNode, CIndividualNodeData*& lastIndiData, librdf_node*& lastSubjectNode, CConcreteOntology* ontology) {
				if (!lastIndiData || !librdf_node_equals(lastSubjectNode, subjectNode)) {
					CIndividualNodeData* indiNodeData = getIndividualData(subjectNode, ontology);
					lastIndiData = indiNodeData;
					if (lastSubjectNode) {
						librdf_free_node(lastSubjectNode);
					}
					lastSubjectNode = librdf_new_node_from_node(subjectNode);
				}
				return lastIndiData;
			}



			CDatatype* CRedlandStoredTriplesIndividualAssertionConvertionIndexer::getDatatypeFromDatatypeUri(librdf_uri* uri, CConcreteOntology* ontology, CRedlandStoredTriplesData* ontologyTripleData) {
				CDatatype* datatype = nullptr;
				if (!mDatatypeHashInitialized) {
					CBUILDHASH<CDatatype*, CDatatypeExpression*>* datatypeDatatypeExpHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getDatatypeDatatypeExpessionHash();
					for (CBUILDHASH<CDatatype*, CDatatypeExpression*>::const_iterator it = datatypeDatatypeExpHash->constBegin(), itEnd = datatypeDatatypeExpHash->constEnd(); it != itEnd; ++it) {
						CDatatype* datatype = it.key();
						if (datatype->getDatatypeType() == CDatatype::DT_STRING) {
							mDefaultDatatype = datatype;
						}
						QByteArray datatypeCharData = datatype->getDatatypeIRI().toUtf8();
						librdf_uri* datatypeUri = librdf_new_uri(ontologyTripleData->getRedlandWorld(), (const unsigned char*)datatypeCharData.constData());
						mDatatypeIriDatatypeHash.insert(CRedlandUriHasher(datatypeUri), datatype);
					}
					mDatatypeHashInitialized = true;
				}
				if (uri) {
					datatype = mDatatypeIriDatatypeHash.value(CRedlandUriHasher(uri));
				} else {
					datatype = mDefaultDatatype;
				}
				return datatype;
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
