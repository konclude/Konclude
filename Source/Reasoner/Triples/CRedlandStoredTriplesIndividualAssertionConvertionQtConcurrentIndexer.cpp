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

#include "CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {



			CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer() {
				mIndiResolvingKeyCount = 211;
				mIndiResolvingHashVec.resize(mIndiResolvingKeyCount);

			}

			CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::~CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer() {
			}
			
			cint64 CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::getCStringHashValue(const char* p) {
				cint64 result = 0;
				const cint64 prime = 31;
				for (cint64 i = 0; p[i] != 0; ++i) {
					result = p[i] + (result * prime);
				}
				return result;
			}

			cint64 CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::getNodeHashValue(librdf_node* redlandNode) {
				cint64 hashValue = 0;
				if (librdf_node_is_literal(redlandNode)) {
					hashValue = getCStringHashValue((const char*)librdf_node_get_literal_value(redlandNode));
				} else if (!librdf_node_is_blank(redlandNode)) {
					librdf_uri* uri = librdf_node_get_uri(redlandNode);
					hashValue = getCStringHashValue((const char*)librdf_uri_as_string(uri));
				} else {
					hashValue = getCStringHashValue((const char*)librdf_node_get_blank_identifier(redlandNode));
				}
				return hashValue;
			}


			CIndividual* CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::getIndividual(librdf_node* indiNode, CRedlandStoredTriplesIndividualResolvingHashMemoryManaged* indiResolveHash, CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>* keyMappedVec) {
				CIndividual*& indi = indiResolveHash->getIndividual(indiNode);
				if (!indi) {
					CRedlandStoredTriplesIndividualPreProcessMemoryManagingContext* indiDataMemAllocManager = indiResolveHash->getIndividualDataMemoryManager();
					indi = CObjectAllocator<CIndividual>::allocateAndConstruct(indiDataMemAllocManager->getMemoryAllocationManager());
					indi->initIndividual(0);

					const char* indiNameCharPointer = nullptr;
					bool blankNode = false;
					if (!librdf_node_is_blank(indiNode)) {
						librdf_uri* uri = librdf_node_get_uri(indiNode);
						indiNameCharPointer = (const char*)librdf_uri_as_string(uri);
					} else {
						indiNameCharPointer = (const char*)librdf_node_get_blank_identifier(indiNode);
						blankNode = true;
					}

					CIRIName* newName = CObjectAllocator<CIRIName>::allocateAndConstruct(indiDataMemAllocManager->getMemoryAllocationManager());
					QString indiName = QString::fromUtf8(indiNameCharPointer);
					if (blankNode) {
						if (!indiName.startsWith("_:")) {
							indiName = mAnonymousOntologyIdentifier + indiName;
						} else {
							indiName = mAnonymousOntologyIdentifier + indiName.mid(2);
						}
					}
					newName->init(indiName);
					CLinker<CName*>* newNameLinker = CObjectAllocator< CLinker<CName*> >::allocateAndConstruct(indiDataMemAllocManager->getMemoryAllocationManager());
					newNameLinker->init(newName);
					indi->addNameLinker(newNameLinker);

					keyMappedVec->addCreationIndvidual(indi);
				}
				return indi;
			}












			bool CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::indexABoxIndividuals(CConcreteOntology* updateConcreteOntology, COntologyTriplesData* ontologyTripleData) {

				mIndexingTime.start();

				mUupdatingOntology = updateConcreteOntology;
				mPreprocessContext = new CPreProcessContextBase(updateConcreteOntology, updateConcreteOntology->getConfiguration());

				mAnonymousOntologyIdentifier = updateConcreteOntology->getOntologyName() + ":";
				if (!mAnonymousOntologyIdentifier.startsWith("_:")) {
					mAnonymousOntologyIdentifier = "_:" + mAnonymousOntologyIdentifier;
				}
				mNextIndiId = 0;
				mDatatypeHashInitialized = false;

				bool newTriplesMapped = false;





				mTripleDeleteMapFunc = [&](CRedlandStoredTriplesData*& redlandTriplesData) {
					if (redlandTriplesData) {

						librdf_world* world = redlandTriplesData->getRedlandWorld();
						mMemMan = updateConcreteOntology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();


						CXLinker<librdf_statement*>* statementLinker = redlandTriplesData->getRedlandStatementLinker();
						if (statementLinker) {

							while (statementLinker) {
								librdf_statement* statement = statementLinker->getData();
								librdf_free_statement(statement);
								CXLinker<librdf_statement*>* tmpStatementLinker = statementLinker;
								statementLinker = statementLinker->getNext();
								delete tmpStatementLinker;
							}
							redlandTriplesData->setRedlandStatementLinker(nullptr);
						} else {
							librdf_stream* stream = librdf_model_as_stream(redlandTriplesData->getRedlandIndexedModel());
							if (stream) {
								while (!librdf_stream_end(stream)) {
									librdf_statement* statement = librdf_stream_get_object(stream);
									librdf_model_remove_statement(redlandTriplesData->getRedlandIndexedModel(), statement);
									librdf_free_statement(statement);
									librdf_stream_next(stream);
								}
								librdf_free_stream(stream);
							}

						}

					}
				};

				mIndiKeyReduceFunc = [&](CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>& vec, const CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>& reduceVec) -> void {

					CBoxContext* boxContext = updateConcreteOntology->getDataBoxes()->getBoxContext();
					if (vec.size() < reduceVec.size()) {
						vec.resize(reduceVec.size());
					}
					for (cint64 i = 0; i < vec.count(); ++i) {
						CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<CRedlandStoredTriplesIndexingBaseResolvingLinker>*& vecData = vec[i];
						CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<CRedlandStoredTriplesIndexingBaseResolvingLinker>* const & reduceVecData = reduceVec[i];
						if (reduceVecData) {
							if (!vecData) {
								vecData = reduceVecData;
							} else {
								vecData->mergeBatchLinkers(reduceVecData);
							}
						}

					}
					for (CXLinker<CIndividual*>* indiLinkerIt = reduceVec.getIndvidualCreationLinker(); indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
						CIndividual* indi = indiLinkerIt->getData();
						indi->setIndividualID(mNextIndiId++);

						updateConcreteOntology->getABox()->getIndividualVector(true)->setData(indi->getIndividualID(), indi);
						updateConcreteOntology->getABox()->getActiveIndividualSet(true)->insert(indi);

						updateConcreteOntology->getStringMapping()->getIndividualNameIndividualMappingHash()->insert(CIRIName::getRecentIRIName(indi->getNameLinker()), indi);
					}
					vec.appendMemoryPool(reduceVec.getMemoryPools());
				};


				

				cint64 procCount = QThreadPool::globalInstance()->maxThreadCount();
				QList<CTriplesData*>* triplesDataList = ontologyTripleData->getAllTriplesData();

				if (!triplesDataList->isEmpty()) {
					CTriplesData* tripleData = triplesDataList->first();

					CRedlandStoredTriplesData* redlandTriplesData = dynamic_cast<CRedlandStoredTriplesData*>(tripleData);

					librdf_world* world = redlandTriplesData->getRedlandWorld();
					mMemMan = updateConcreteOntology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();


					librdf_node* rdfTypePredicate = librdf_new_node_from_uri_string(world, (const unsigned char*)PREFIX_RDF_TYPE);
					librdf_node* namedIndividualObject = librdf_new_node_from_uri_string(world, (const unsigned char*)PREFIX_OWL_NAMED_INDIVIDUAL);


					if (!mExistingEntitiesInitialized) {
						mExistingEntitiesInitialized = true;
						mRoleNodeDataHash.clear();
						QSet<CRole*>* activeRoleSet = updateConcreteOntology->getRBox()->getActivePropertyRoleSet(false);
						if (activeRoleSet) {
							for (CRole* role : *activeRoleSet) {
								QByteArray roleCharData = CIRIName::getRecentIRIName(role->getPropertyNameLinker()).toUtf8();
								librdf_node* roleNode = librdf_new_node_from_uri_string(world, (const unsigned char*)roleCharData.constData());
								mRoleNodeDataHash.insert(CRedlandNodeHasher(roleNode), new CRoleNodeData(roleNode, role, role->isDataRole()));
							}
						}


						mConceptNodeDataHash.clear();
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


						mIndividualNodeDataHash.clear();
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
					}

				}

				if (triplesDataList->count() < procCount * 2) {
					for (CTriplesData* tripleData : *ontologyTripleData->getAllTriplesData()) {
						CRedlandStoredTriplesData* redlandTriplesData = dynamic_cast<CRedlandStoredTriplesData*>(tripleData);
						librdf_world* world = redlandTriplesData->getRedlandWorld();
						librdf_node* rdfTypePredicate = librdf_new_node_from_uri_string(world, (const unsigned char*)PREFIX_RDF_TYPE);
						librdf_node* namedIndividualObject = librdf_new_node_from_uri_string(world, (const unsigned char*)PREFIX_OWL_NAMED_INDIVIDUAL);

						indexTriples(redlandTriplesData, rdfTypePredicate, namedIndividualObject, updateConcreteOntology);

					}

					bool deleteIndexedTriplesData = CConfigDataReader::readConfigBoolean(updateConcreteOntology->getConfiguration(), "Konclude.Calculation.Preprocessing.TripleEncodedAssertionsIndexing.DeleteTriplesDataAfterIndexing", false);
					if (deleteIndexedTriplesData) {
						QVector<CRedlandStoredTriplesData*> triplesDataVec(triplesDataList->count());
						cint64 tripleDataId = 0;
						for (CTriplesData* tripleData : *triplesDataList) {
							CRedlandStoredTriplesData* redlandTriplesData = (CRedlandStoredTriplesData*)tripleData;
							ensureDatatypeHashInitialized(updateConcreteOntology, redlandTriplesData);
							triplesDataVec[tripleDataId++] = redlandTriplesData;
						}
						QtConcurrent::blockingMap(triplesDataVec, mTripleDeleteMapFunc);
					}

				} else {
					indexTripleData(triplesDataList, updateConcreteOntology);
				}

				for (cint64 i = 0; i < mIndiResolvingKeyCount; ++i) {
					CRedlandStoredTriplesIndividualResolvingHashMemoryManaged*& indiResolveHash = mIndiResolvingHashVec[i];
					if (indiResolveHash) {
						mConceptAssertionCount += indiResolveHash->getConceptAssertionCount();
						mObjectPropertyAssertionCount += indiResolveHash->getObjectPropertyAssertionCount();
						mDataPropertyAssertionCount += indiResolveHash->getDataPropertyAssertionCount();
						delete indiResolveHash;
						indiResolveHash = nullptr;
					}
				}


				LOG(INFO, "::Konclude::Reasoner::TripleEncodedAssertionsIndexer", QString("Identified %1 concept, %2 object property, and %3 data property assertions for overall %4 individuals in %5 ms.").arg(mConceptAssertionCount).arg(mObjectPropertyAssertionCount).arg(mDataPropertyAssertionCount).arg(mNextIndiId).arg(mIndexingTime.elapsed()), this);
		
				delete mPreprocessContext;

				return newTriplesMapped;
			}








			void CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::procssIndividualIndexing(CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>& mappedIndi1HashKeyVec, CRedlandStoredTriplesData* redlandTriplesData, CConcreteOntology* updateConcreteOntology) {


				function<CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>(const CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<CRedlandStoredTriplesIndexingBaseResolvingLinker>* const & batchData)> individual2ResolveKeyMapFunc =
					[&](const CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<CRedlandStoredTriplesIndexingBaseResolvingLinker>* const & batchData) ->CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> {

					CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> keyMappedVec(mIndiResolvingKeyCount);
					if (batchData) {

						CRedlandStoredTriplesIndexingBaseResolvingLinker* resolveLinker = batchData->getNextBatchLinker();
						while (resolveLinker) {

							cint64 index = qAbs(resolveLinker->mIndi1HashValue % mIndiResolvingKeyCount);
							CRedlandStoredTriplesIndividualResolvingHashMemoryManaged*& indiResolveHash = mIndiResolvingHashVec[index];
							if (!indiResolveHash) {
								indiResolveHash = new CRedlandStoredTriplesIndividualResolvingHashMemoryManaged(mUupdatingOntology, mUupdatingOntology->getConfiguration(), mAnonymousOntologyIdentifier);
							}

							CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_TYPE assertionType = (CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_TYPE)resolveLinker->getData();

							librdf_statement* statement = resolveLinker->mStatement;
							librdf_node* predicateNode = librdf_statement_get_predicate(statement);
							librdf_node* objectNode = librdf_statement_get_object(statement);
							librdf_node* subjectNode = librdf_statement_get_subject(statement);

							resolveLinker->mIndi1 = getIndividual(subjectNode, indiResolveHash, &keyMappedVec);


							if (assertionType == CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_CONCEPT) {
								CConceptAssertionLinker* conAssLinker = indiResolveHash->createAssertionConceptLinker();
								conAssLinker->initNegLinker(resolveLinker->mConcept, false);

								resolveLinker->mIndi1->addAssertionConceptLinker(conAssLinker);

								resolveLinker = resolveLinker->getNext();
							} else if (assertionType == CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_DATA) {
								CRedlandStoredTriplesIndividualPreProcessMemoryManagingContext* indiMemManContext = indiResolveHash->getIndividualDataMemoryManager();
								CDataAssertionLinker* dataAssLinker = indiResolveHash->createAssertionDataLinker();

								const char* literalValue = (const char*)librdf_node_get_literal_value(objectNode);
								librdf_uri* datatypeUri = librdf_node_get_literal_value_datatype_uri(objectNode);
								CDatatype* datatype = getDatatypeFromDatatypeUri(datatypeUri, mUupdatingOntology, redlandTriplesData);

								CDataLiteral* dataLiteral = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(indiMemManContext->getMemoryAllocationManager(), indiMemManContext);
								CDataLiteralNormalizerPreProcess* dataLiteralNormalizer = indiResolveHash->getIndividualDataLiteralNormalizer();

								dataLiteral->initDataLiteral(QString::fromUtf8(literalValue), datatype);
								dataLiteralNormalizer->createPreprocessedDataLiteralValue(dataLiteral, indiMemManContext, indiMemManContext);
								dataAssLinker->initDataAssertionLinker(resolveLinker->mRole, dataLiteral);

								resolveLinker->mIndi1->addAssertionDataLinker(dataAssLinker);
								resolveLinker = resolveLinker->getNext();
							} else if (assertionType == CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_ROLE) {
								cint64 nextIndex = qAbs(resolveLinker->mIndi2HashValue % mIndiResolvingKeyCount);
								CRoleAssertionLinker* roleAssLinker = indiResolveHash->createAssertionRoleLinker();
								resolveLinker->mRoleAssLinker = roleAssLinker;
								resolveLinker->mIndi1->addAssertionRoleLinker(roleAssLinker);
								CRedlandStoredTriplesIndexingBaseResolvingLinker* tmpResolveLinker = resolveLinker;
								resolveLinker = resolveLinker->getNext();
								tmpResolveLinker->clearNext();
								keyMappedVec.addBatchLinker(nextIndex, tmpResolveLinker);
							} else {
								resolveLinker = resolveLinker->getNext();
							}

						}
					}
					return keyMappedVec;
				};


				function<CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>(const CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<CRedlandStoredTriplesIndexingBaseResolvingLinker>* const & batchData)> individual3ResolveKeyMapFunc =
					[&](const CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<CRedlandStoredTriplesIndexingBaseResolvingLinker>* const & batchData) ->CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> {

					CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> keyMappedVec(mIndiResolvingKeyCount);
					if (batchData) {

						CRedlandStoredTriplesIndexingBaseResolvingLinker* resolveLinker = batchData->getNextBatchLinker();
						while (resolveLinker) {

							cint64 index = qAbs(resolveLinker->mIndi2HashValue % mIndiResolvingKeyCount);
							CRedlandStoredTriplesIndividualResolvingHashMemoryManaged*& indiResolveHash = mIndiResolvingHashVec[index];
							if (!indiResolveHash) {
								indiResolveHash = new CRedlandStoredTriplesIndividualResolvingHashMemoryManaged(mUupdatingOntology, mUupdatingOntology->getConfiguration(), mAnonymousOntologyIdentifier);
							}

							librdf_statement* statement = resolveLinker->mStatement;
							librdf_node* objectNode = librdf_statement_get_object(statement);
							resolveLinker->mIndi2 = getIndividual(objectNode, indiResolveHash, &keyMappedVec);

							CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_TYPE assertionType = (CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_TYPE)resolveLinker->getData();
							if (assertionType == CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_ROLE) {
								CRoleAssertionLinker* roleAssLinker = resolveLinker->mRoleAssLinker;
								roleAssLinker->initRoleAssertionLinker(resolveLinker->mRole, resolveLinker->mIndi2);
								CReverseRoleAssertionLinker* reverseRoleAssLinker = indiResolveHash->createReverseAssertionRoleLinker();
								reverseRoleAssLinker->initReverseRoleAssertionLinker(roleAssLinker, resolveLinker->mIndi1);
								resolveLinker->mIndi2->addReverseAssertionRoleLinker(reverseRoleAssLinker);
							}

							resolveLinker = resolveLinker->getNext();
						}
					}
					return keyMappedVec;
				};


				

				CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> mappedIndi2HashKeyVec = QtConcurrent::blockingMappedReduced<CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>>(mappedIndi1HashKeyVec, individual2ResolveKeyMapFunc, mIndiKeyReduceFunc);
				CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> mappedIndi3HashKeyVec = QtConcurrent::blockingMappedReduced<CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>>(mappedIndi2HashKeyVec, individual3ResolveKeyMapFunc, mIndiKeyReduceFunc);

				CMemoryPool* mappedIndi1HashKeyVecMemoryPools = mappedIndi1HashKeyVec.takeMemoryPools();
				CMemoryPool* mappedIndi2HashKeyVecMemoryPools = mappedIndi2HashKeyVec.takeMemoryPools();
				CMemoryPool* mappedIndi3HashKeyVecMemoryPools = mappedIndi3HashKeyVec.takeMemoryPools();
				//QtConcurrent::run(QThreadPool::globalInstance(), [=] {
				clearMemoryPools(mappedIndi1HashKeyVecMemoryPools);
				//});
				//QtConcurrent::run(QThreadPool::globalInstance(), [=] {
				clearMemoryPools(mappedIndi2HashKeyVecMemoryPools);
				//});
				//QtConcurrent::run(QThreadPool::globalInstance(), [=] {
				clearMemoryPools(mappedIndi3HashKeyVecMemoryPools);
				//});
			}




			void CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::clearMemoryPools(CMemoryPool* memPools) {
				while (memPools) {
					CMemoryPool* tmpMemPool = memPools;
					memPools = memPools->getNextMemoryPool();
					delete[] tmpMemPool->getMemoryBlockData();
					delete tmpMemPool;
				}
			}



			void CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::indexTripleData(QList<CTriplesData*>* triplesDataList, CConcreteOntology* updateConcreteOntology) {
				QVector<CRedlandStoredTriplesData*> triplesDataVec(triplesDataList->count());
				cint64 tripleDataId = 0;
				for (CTriplesData* tripleData : *triplesDataList) {
					CRedlandStoredTriplesData* redlandTriplesData = (CRedlandStoredTriplesData*)tripleData;
					ensureDatatypeHashInitialized(updateConcreteOntology, redlandTriplesData);
					triplesDataVec[tripleDataId++] = redlandTriplesData;
				}

				indexHashExistingIndividuals(updateConcreteOntology);


				function<CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>(CRedlandStoredTriplesData* redlandTriplesData)> individual1ResolveKeyMapFunc =
					[&](CRedlandStoredTriplesData* redlandTriplesData) ->CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> {

					CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> keyMappedVec(mIndiResolvingKeyCount);
					if (redlandTriplesData) {


						librdf_world* world = redlandTriplesData->getRedlandWorld();
						mMemMan = updateConcreteOntology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();


						librdf_node* rdfTypePredicate = librdf_new_node_from_uri_string(world, (const unsigned char*)PREFIX_RDF_TYPE);
						librdf_node* namedIndividualObject = librdf_new_node_from_uri_string(world, (const unsigned char*)PREFIX_OWL_NAMED_INDIVIDUAL);

						CXLinker<librdf_statement*>* statementLinker = redlandTriplesData->getRedlandStatementLinker();
						if (statementLinker) {

							while (statementLinker) {
								librdf_statement* statement = statementLinker->getData();
								handleStatementMapping(statement, keyMappedVec, rdfTypePredicate, namedIndividualObject);
								statementLinker = statementLinker->getNext();
							}
						} else {
							librdf_stream* stream = librdf_model_as_stream(redlandTriplesData->getRedlandIndexedModel());
							if (stream) {
								while (!librdf_stream_end(stream)) {
									librdf_statement* statement = librdf_stream_get_object(stream);
									handleStatementMapping(statement, keyMappedVec, rdfTypePredicate, namedIndividualObject);
									librdf_stream_next(stream);
								}
								librdf_free_stream(stream);
							}

						}

					}
					return keyMappedVec;
				};



				CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> mappedIndi1HashKeyVec = QtConcurrent::blockingMappedReduced<CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>>(triplesDataVec, individual1ResolveKeyMapFunc, mIndiKeyReduceFunc);

				procssIndividualIndexing(mappedIndi1HashKeyVec, nullptr, updateConcreteOntology);






				bool deleteIndexedTriplesData = CConfigDataReader::readConfigBoolean(updateConcreteOntology->getConfiguration(), "Konclude.Calculation.Preprocessing.TripleEncodedAssertionsIndexing.DeleteTriplesDataAfterIndexing", false);
				if (deleteIndexedTriplesData) {
					QtConcurrent::blockingMap(triplesDataVec, mTripleDeleteMapFunc);
				}







				updateConcreteOntology->getABox()->setNextIndividualId(mNextIndiId);
			}






			void CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::indexTriples(CRedlandStoredTriplesData* redlandTriplesData, librdf_node* rdfTypePredicate, librdf_node* namedIndividualObject, CConcreteOntology* updateConcreteOntology) {
				CIndividualNodeData* lastIndiData = nullptr;
				librdf_node* lastSubjectNode = nullptr;

				cint64 statementBatchLimit = 1000;
				cint64 statementBatchCountLimit = 1000;

				CXLinker<librdf_statement*>* statementLinker = redlandTriplesData->getRedlandStatementLinker();
				if (statementLinker) {
					ensureDatatypeHashInitialized(updateConcreteOntology, redlandTriplesData);


					indexHashExistingIndividuals(updateConcreteOntology);



					CXLinker<librdf_statement*>* statementLinkerIt = statementLinker;
					while (statementLinkerIt) {

						CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseBatchLinker> statementBatchingLinkerVec(mIndiResolvingKeyCount);
						cint64 statementBatchSize = 0;
						cint64 statementBatchCount = 0;
						while (statementLinkerIt && statementBatchCount < statementBatchCountLimit) {
							CXLinker<librdf_statement*>* statementBeginLinker = statementLinkerIt;
							CXLinker<librdf_statement*>* statementEndLinker = statementLinkerIt;
							statementBatchSize = 0;
							while (statementEndLinker && statementBatchSize < statementBatchLimit) {
								statementEndLinker = statementEndLinker->getNext();
								++statementBatchSize;
							}
							statementLinkerIt = statementEndLinker;
							CRedlandStoredTriplesIndexingBaseBatchLinker* linker = statementBatchingLinkerVec.createBatchLinker();
							linker->initBatchLinker(statementBeginLinker, statementEndLinker, statementBatchSize);
							statementBatchingLinkerVec.addBatchLinker(statementBatchingLinkerVec.getNextIndex(true), linker);
							statementBatchCount++;
						}








						function<CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>(const CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<CRedlandStoredTriplesIndexingBaseBatchLinker>* const & batchData)> individual1ResolveKeyMapFunc = 
							[&](const CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<CRedlandStoredTriplesIndexingBaseBatchLinker>* const & batchData) ->CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> {

							CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> keyMappedVec(mIndiResolvingKeyCount);
							if (batchData) {
								CRedlandStoredTriplesIndexingBaseBatchLinker* batchLinker = batchData->getNextBatchLinker();
								while (batchLinker) {

									CXLinker<librdf_statement*>* statementLinker = batchLinker->getNextStatementLinker();
									while (statementLinker) {
										librdf_statement* statement = statementLinker->getData();

										handleStatementMapping(statement, keyMappedVec, rdfTypePredicate, namedIndividualObject);

										statementLinker = batchLinker->getNextStatementLinker();
									}

									batchLinker = batchLinker->getNext();
								}
							}
							return keyMappedVec;
						};


						CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> mappedIndi1HashKeyVec = QtConcurrent::blockingMappedReduced<CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker>>(statementBatchingLinkerVec, individual1ResolveKeyMapFunc, mIndiKeyReduceFunc);

						procssIndividualIndexing(mappedIndi1HashKeyVec, redlandTriplesData, updateConcreteOntology);

						CMemoryPool* statementBatchingLinkerVecMemoryPools = statementBatchingLinkerVec.takeMemoryPools();
						//QtConcurrent::run(QThreadPool::globalInstance(), [=] {
						clearMemoryPools(statementBatchingLinkerVecMemoryPools);
						//});

					}


					//CBoxContext* boxContext = updateConcreteOntology->getDataBoxes()->getBoxContext();
					//for (cint64 i = 0; i < mIndiResolvingKeyCount; ++i) {
					//	CRedlandStoredTriplesIndividualResolvingHashMemoryManaged* indiResolveHash = mIndiResolvingHashVec[i];
					//	if (indiResolveHash) {
					//		CMemoryPool* memPools = indiResolveHash->takeIndividualDataMemoryPools();
					//		boxContext->addUsedMemoryPools(memPools);
					//		for (CQtManagedRestrictedModificationHash<CRedlandNodeHasher, CIndividual*>::const_iterator it = indiResolveHash->constBegin(), itEnd = indiResolveHash->constEnd(); it != itEnd; ++it) {
					//			CIndividual* indi = it.value();
					//			indi->setIndividualID(mNextIndiId++);
					//			updateConcreteOntology->getABox()->getIndividualVector(true)->setData(indi->getIndividualID(), indi);
					//			updateConcreteOntology->getABox()->getActiveIndividualSet(true)->insert(indi);
					//			updateConcreteOntology->getStringMapping()->getIndividualNameIndividualMappingHash()->insert(CIRIName::getRecentIRIName(indi->getNameLinker()), indi);
					//		}
					//	}
					//}


					//while (statementLinker) {

					//	librdf_statement* statement = statementLinker->getData();


					//	identifyIndividuals(statement, rdfTypePredicate, namedIndividualObject, lastIndiData, lastSubjectNode, updateConcreteOntology, conceptAssertionCount, redlandTriplesData, dataPropertyAssertionCount, objectPropertyAssertionCount);


					//	statementLinker = statementLinker->getNext();
					//}
				} else {
					librdf_stream* stream = librdf_model_as_stream(redlandTriplesData->getRedlandIndexedModel());
					if (stream) {
						while (!librdf_stream_end(stream)) {
							librdf_statement* statement = librdf_stream_get_object(stream);
							identifyIndividuals(statement, rdfTypePredicate, namedIndividualObject, lastIndiData, lastSubjectNode, updateConcreteOntology, redlandTriplesData);
							librdf_stream_next(stream);
						}
						librdf_free_stream(stream);
					}

				}


				updateConcreteOntology->getABox()->setNextIndividualId(mNextIndiId);

			}

			void CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::handleStatementMapping(librdf_statement* statement, CRedlandStoredTriplesIndexingBaseBatchLinkerVector<CRedlandStoredTriplesIndexingBaseResolvingLinker> &keyMappedVec, librdf_node* rdfTypePredicate, librdf_node* namedIndividualObject) {
				librdf_node* predicateNode = librdf_statement_get_predicate(statement);
				librdf_node* objectNode = librdf_statement_get_object(statement);
				librdf_node* subjectNode = librdf_statement_get_subject(statement);

				CRedlandStoredTriplesIndexingBaseResolvingLinker* resolvingLinker = keyMappedVec.createBatchLinker();
				resolvingLinker->setData(CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_NONE);
				resolvingLinker->mStatement = statement;

				resolvingLinker->mIndi1HashValue = getNodeHashValue(subjectNode);
				cint64 index = qAbs(resolvingLinker->mIndi1HashValue % mIndiResolvingKeyCount);

				if (librdf_node_equals(predicateNode, rdfTypePredicate)) {
					if (librdf_node_equals(objectNode, namedIndividualObject)) {
						resolvingLinker->setData(CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_INDIVIDUAL);
						keyMappedVec.addBatchLinker(index, resolvingLinker);
					} else {
						resolvingLinker->setData(CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_CONCEPT);
						CConceptNodeData* conceptData = mConceptNodeDataHash.value(CRedlandNodeHasher(objectNode));
						if (conceptData) {
							resolvingLinker->mConcept = conceptData->mConcept;
							keyMappedVec.addBatchLinker(index, resolvingLinker);
						}
					}
				} else {
					CRoleNodeData* roleData = mRoleNodeDataHash.value(CRedlandNodeHasher(predicateNode));
					if (roleData) {
						resolvingLinker->mRole = roleData->mRole;
						if (roleData->mDataRole) {
							if (librdf_node_is_literal(objectNode)) {
								resolvingLinker->setData(CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_DATA);
								keyMappedVec.addBatchLinker(index, resolvingLinker);
							}
						} else {
							if (!librdf_node_is_literal(objectNode)) {
								resolvingLinker->setData(CRedlandStoredTriplesIndexingBaseResolvingLinker::ASSERTION_ROLE);
								keyMappedVec.addBatchLinker(index, resolvingLinker);
								resolvingLinker->mIndi2HashValue = getNodeHashValue(objectNode);
							}
						}
					}
				}
			}

			void CRedlandStoredTriplesIndividualAssertionConvertionQtConcurrentIndexer::indexHashExistingIndividuals(CConcreteOntology* updateConcreteOntology) {
				QSet<CIndividual*>* activeIndiSet = updateConcreteOntology->getABox()->getActiveIndividualSet(false);
				if (!mExistingIndividualsHashed) {
					mExistingIndividualsHashed = true;
					if (activeIndiSet && !activeIndiSet->isEmpty()) {

						for (QHash<CRedlandNodeHasher, CIndividualNodeData*>::const_iterator it = mIndividualNodeDataHash.constBegin(), itEnd = mIndividualNodeDataHash.constEnd(); it != itEnd; ++it) {
							cint64 hashValue = it.key().getNodeHashValue();
							cint64 index = qAbs(hashValue % mIndiResolvingKeyCount);
							CRedlandStoredTriplesIndividualResolvingHashMemoryManaged*& indiResolveHash = mIndiResolvingHashVec[index];
							if (!indiResolveHash) {
								indiResolveHash = new CRedlandStoredTriplesIndividualResolvingHashMemoryManaged(mUupdatingOntology, mUupdatingOntology->getConfiguration(), mAnonymousOntologyIdentifier);
							}
							indiResolveHash->insertIndividual(it.key(), it.value()->mIndividual);
						}
					}
				}
			}

		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
