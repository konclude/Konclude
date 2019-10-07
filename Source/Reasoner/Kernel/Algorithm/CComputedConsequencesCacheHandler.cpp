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

#include "CComputedConsequencesCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CComputedConsequencesCacheHandler::CComputedConsequencesCacheHandler(CComputedConsequencesCacheReader* satCacheReader, CComputedConsequencesCacheWriter* satCacheWriter) {
					mSatCacheReader = satCacheReader;
					mSatCacheWriter = satCacheWriter;

					mTmpContext = nullptr;
					mMemAllocMan = nullptr;
					mWriteData = nullptr;
				}





				bool CComputedConsequencesCacheHandler::canCacheTypeConcept(CIndividual* individual, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext) {
					CConcreteOntology* ontology = calcAlgContext->getProcessingDataBox()->getOntology();
					CConsistence* consistence = ontology->getConsistence();
					CBOXHASH<CConcept*,CConcept*>* eqConCandHash = ontology->getTBox()->getEquivalentConceptCandidateHash(false);
					if (consistence && concept->getTerminology()) {
						CConsistenceData* consData = consistence->getConsistenceModelData();
						if (consData) {
							CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
							if (consTaskData) {
								CSatisfiableCalculationTask* detSatCalcTask = consTaskData->getDeterministicSatisfiableTask();
								if (detSatCalcTask) {
									CIndividualProcessNodeVector* detSatProcNodeVec = detSatCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
									CIndividualProcessNode* detIndiNode = detSatProcNodeVec->getData(-individual->getIndividualID());
									while (detIndiNode->getIndividualNodeID() != detIndiNode->getMergedIntoIndividualNodeID()) {
										detIndiNode = detSatProcNodeVec->getData(detIndiNode->getMergedIntoIndividualNodeID());
									}
									CReapplyConceptLabelSet* detConSet = detIndiNode->getReapplyConceptLabelSet(false);
									if (detConSet && !detConSet->containsConcept(concept,negation)) {
										//if (concept->getTerminology() == ontology) {
										//	return true;
										//}
										CSatisfiableCalculationTask* compGraphCachedCalcTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
										CIndividualProcessNodeVector* compGraphCachedProcNodeVec = compGraphCachedCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
										CIndividualProcessNode* compGraphCachedIndiNode = compGraphCachedProcNodeVec->getData(-individual->getIndividualID());
										while (compGraphCachedIndiNode->getIndividualNodeID() != compGraphCachedIndiNode->getMergedIntoIndividualNodeID()) {
											compGraphCachedIndiNode = detSatProcNodeVec->getData(compGraphCachedIndiNode->getIndividualNodeID());
										}
										CReapplyConceptLabelSet* compGraphCachedConSet = compGraphCachedIndiNode->getReapplyConceptLabelSet(false);
										if (compGraphCachedConSet && compGraphCachedConSet->containsConcept(concept,negation)) {
											return true;
										}
										if (!negation && eqConCandHash) {
											CConcept* candidateConcept = eqConCandHash->value(concept);
											if (compGraphCachedConSet && candidateConcept && compGraphCachedConSet->containsConcept(candidateConcept,false)) {
												return true;
											}

										}
									}
								}
							}
						}
					}
					return false;
				}



				bool CComputedConsequencesCacheHandler::tryCacheTypeConcept(CIndividual* individual, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext) {
					CComputedConsequencesTypesCacheEntry* typeCacheEntry = mSatCacheReader->getTypesCacheEntry(individual);

					if ((!typeCacheEntry || !typeCacheEntry->hasConcept(concept,negation)) && canCacheTypeConcept(individual,concept,negation,calcAlgContext)) {
						// try caching
						prepareCacheMessages(calcAlgContext);
						CTaskMemoryPoolAllocationManager* satCacheMemMan = mMemAllocMan;
						CContextBase* tmpContext = mTmpContext;

						CComputedConsequencesCacheWriteTypesData* unsatWriteData = CObjectAllocator< CComputedConsequencesCacheWriteTypesData >::allocateAndConstruct(satCacheMemMan);
						unsatWriteData->initTypesCacheWriteData(individual,concept,negation);
						addCacheMessages(unsatWriteData,calcAlgContext);
						return true;
					}

					return false;				
				}


				CSortedNegLinker<CConcept*>* CComputedConsequencesCacheHandler::getCachedTypesConceptLinker(CIndividual* individual, CCalculationAlgorithmContext* calcAlgContext) {
					CSortedNegLinker<CConcept*>* typeConceptLinker = nullptr;
					CComputedConsequencesTypesCacheEntry* typeCacheEntry = mSatCacheReader->getTypesCacheEntry(individual);
					if (typeCacheEntry) {
						typeConceptLinker = typeCacheEntry->getConceptLinker();
					}
					return typeConceptLinker;
				}




				bool CComputedConsequencesCacheHandler::prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (!mMemAllocMan) {
						CTaskMemoryPoolAllocationManager satCacheMemManCreaterMemMan(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&satCacheMemManCreaterMemMan);
						mMemAllocMan->initTaskMemoryPoolAllocationManager(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mMemAllocMan,mMemAllocMan);
						return true;
					}
					return false;
				}

				bool CComputedConsequencesCacheHandler::commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						CComputedConsequencesCacheWriteData* commitWriteData = nullptr;
						CComputedConsequencesCacheWriteData* writeDataIt = mWriteData;
						while (writeDataIt) {
							CComputedConsequencesCacheWriteData* tmpWriteData = writeDataIt;
							writeDataIt = (CComputedConsequencesCacheWriteData*)writeDataIt->getNext();
							tmpWriteData->clearNext();
							if (commitWriteData) {
								commitWriteData = (CComputedConsequencesCacheWriteData*)tmpWriteData->append(commitWriteData);
							} else {
								commitWriteData = tmpWriteData;
							}
						}
						mSatCacheWriter->writeCacheData(commitWriteData,mMemPoolCon.takeMemoryPools());
						mWriteData = nullptr;
						mMemAllocMan = nullptr;
						mTmpContext = nullptr;
						return true;
					}
					return false;
				}



				bool CComputedConsequencesCacheHandler::addCacheMessages(CComputedConsequencesCacheWriteData* expandWriteData, CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						mWriteData = (CComputedConsequencesCacheWriteData*)expandWriteData->append(mWriteData);
					} else {
						mWriteData = expandWriteData;
					}
					commitCacheMessages(calcAlgContext);
					return true;
				}





			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
