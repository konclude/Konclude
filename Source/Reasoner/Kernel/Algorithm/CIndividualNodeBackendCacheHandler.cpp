/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CIndividualNodeBackendCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CIndividualNodeBackendCacheHandler::CIndividualNodeBackendCacheHandler(CBackendRepresentativeMemoryCacheReader* backAssCacheReader, CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter) {
					mAssBackCacheReader = backAssCacheReader;
					mAssBackCacheWriter = backAssCacheWriter;

					mStatCreatedTempLabels = 0;
					mStatReusedTempLabels = 0;
					mStatReusedCachedLabels = 0;
					mStatReusedHashedIndiLabels = 0;
					mStatReusedHashedSignatureLabels = 0;

					mTmpContext = nullptr;
					mMemAllocMan = nullptr;
					mWriteData = nullptr;
				}





				bool CIndividualNodeBackendCacheHandler::prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (!mMemAllocMan) {
						CTaskMemoryPoolAllocationManager satCacheMemManCreaterMemMan(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&satCacheMemManCreaterMemMan);
						mMemAllocMan->initTaskMemoryPoolAllocationManager(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mMemAllocMan,mMemAllocMan);
						return true;
					}
					return false;
				}

				bool CIndividualNodeBackendCacheHandler::commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						CBackendRepresentativeMemoryCacheLabelAssociationWriteData* commitWriteData = nullptr;
						CBackendRepresentativeMemoryCacheLabelAssociationWriteData* writeDataIt = mWriteData;
						while (writeDataIt) {
							CBackendRepresentativeMemoryCacheLabelAssociationWriteData* tmpWriteData = writeDataIt;
							writeDataIt = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)writeDataIt->getNext();
							tmpWriteData->clearNext();
							if (commitWriteData) {
								commitWriteData = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)tmpWriteData->append(commitWriteData);
							} else {
								commitWriteData = tmpWriteData;
							}
						}
						mAssBackCacheWriter->writeCachedData(commitWriteData,mMemPoolCon.takeMemoryPools());
						mWriteData = nullptr;
						mMemAllocMan = nullptr;
						mTmpContext = nullptr;
						return true;
					}
					return false;
				}



				bool CIndividualNodeBackendCacheHandler::addCacheMessages(CBackendRepresentativeMemoryCacheLabelAssociationWriteData* backAssWriteData, CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						mWriteData = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)backAssWriteData->append(mWriteData);
					} else {
						mWriteData = backAssWriteData;
					}
					return true;
				}






				bool CIndividualNodeBackendCacheHandler::findAssociationBackendCardinality(CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
					if (!conSet) {
						return false;
					}
					return false;
				}



				bool CIndividualNodeBackendCacheHandler::findAssociationBackendLabel(CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
					if (!conSet) {
						return false;
					}

					QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>* hashedRefPair = nullptr;
					if (indiNode->getNominalIndividual() == nullptr) {
						QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>& refPair = (*mIndiNodeRefLabelHash)[indiNode];
						if (refPair.first) {
							tmpAssWriteDataLinker->setReferredLabelData(refPair.first);
							++mStatReusedHashedIndiLabels;
							return true;
						} else if (refPair.second) {
							tmpAssWriteDataLinker->setReferredTemporaryLabelData(refPair.second);
							++mStatReusedHashedIndiLabels;
							return true;
						}
						hashedRefPair = &refPair;
					}

					CIndividual* nominalIndi = indiNode->getNominalIndividual();
					CConcept* nomIndiConcept = nullptr;
					if (nominalIndi) {
						nomIndiConcept = nominalIndi->getIndividualNominalConcept();
					}

					CConceptSetSignature conSetSignature;
					for (CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
						CConcept* concept = conDesIt->getConcept();
						bool negation = conDesIt->isNegated();
						if (concept != nomIndiConcept) {
							conSetSignature.addConceptSignature(concept,negation);						
						}
					}
					cint64 conSetSigValue = conSetSignature.getSignatureValue();
					cint64 conCount = conSet->getConceptCount();
					if (nomIndiConcept) {
						--conCount;
					}

					
					CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getLabelCacheEntry(conSetSigValue,conCount,conSet->getAddingSortedConceptDescriptionLinker(),nomIndiConcept);
					if (cachedLabelItem) {
						++mStatReusedCachedLabels;
						if (hashedRefPair) {
							hashedRefPair->first = cachedLabelItem;
						}
						tmpAssWriteDataLinker->setReferredLabelData(cachedLabelItem);
						return true;
					}


					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*mSignatureTmpRefLabelHash)[conSetSigValue];
					if (prevTempRefLabels) {
						for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
							if (prevTempRefLabelIt->getDeterministicCacheValueCount() == conCount) {
								bool compatibleConcepts = true;
								for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getDeterministicCacheValueLinker(); labelValueLinkerIt && compatibleConcepts; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
									CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
									CConcept* concept = (CConcept*)cacheValue.getIdentification();
									bool negation = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
									if (!conSet->hasConcept(concept,negation)) {
										compatibleConcepts = false;
									}
								}
								if (compatibleConcepts) {
									++mStatReusedHashedSignatureLabels;
									++mStatReusedTempLabels;
									if (hashedRefPair) {
										hashedRefPair->second = prevTempRefLabelIt;
									}
									tmpAssWriteDataLinker->setReferredTemporaryLabelData(prevTempRefLabelIt);
									return true;
								}
							}
						}
					}


					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
					newTempRefLabel->initLabelWriteData(conSetSigValue);
					newTempRefLabel->setCompletelySaturated(false);
					newTempRefLabel->setCompletelyHandled(true);
					for (CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
						CConcept* concept = conDesIt->getConcept();
						bool negation = conDesIt->isNegated();
						CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
						labelValueLinker->initLabelValueLinker(getCacheValue(concept,negation));
						newTempRefLabel->appendDeterministicCacheValueLinker(labelValueLinker);
					}
					mNewTmpLabelList->append(newTempRefLabel);
					++mStatCreatedTempLabels;

					if (hashedRefPair) {
						hashedRefPair->second = newTempRefLabel;
					}
					tmpAssWriteDataLinker->setReferredTemporaryLabelData(newTempRefLabel);
					prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
					return true;
				}


				const CCacheValue CIndividualNodeBackendCacheHandler::getCacheValue(CConcept* concept, bool negation) {
					return mAssBackCacheReader->getCacheValue(concept,negation);
				}





				CBackendRepresentativeMemoryCacheIndividualAssociationData* CIndividualNodeBackendCacheHandler::getIndividualAssociationData(CIndividual* individual) {
					return mAssBackCacheReader->getIndividualAssociation(individual);
				}



				bool CIndividualNodeBackendCacheHandler::hasConceptIndividualAssociation(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext) {
					CBackendRepresentativeMemoryLabelCacheItem* backendLabelItem = associationData->getBackendLabelCacheEntry();
					if (backendLabelItem) {
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* valueHash = backendLabelItem->getDeterministicTagCacheValueHash(false);
						if (valueHash) {
							CBackendRepresentativeMemoryLabelValueLinker* valueLinker = valueHash->value(concept->getConceptTag());
							if (valueLinker && valueLinker->getCacheValue() == getCacheValue(concept,negation)) {
								return true;
							}
						}
					}
					return false;
				}



				bool CIndividualNodeBackendCacheHandler::tryAssociateNodesWithBackendCache(CIndividualProcessNodeLinker* indiNodeLinker, CCalculationAlgorithmContext* calcAlgContext) {

					mIndiNodeRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< CIndividualProcessNode*,QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*> >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());

					mSignatureTmpRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());

					mNewTmpLabelList = CObjectParameterizingAllocator< CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());

					prepareCacheMessages(calcAlgContext);

					CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker = nullptr;

					for (CIndividualProcessNodeLinker* indiNodeLinkerIt = indiNodeLinker; indiNodeLinkerIt; indiNodeLinkerIt = indiNodeLinkerIt->getNext()) {
						CIndividualProcessNode* indiNode = indiNodeLinkerIt->getProcessingIndividual();
						CIndividual* nominalIndividual = indiNode->getNominalIndividual();
						if (nominalIndividual) {

							CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* newAssWriteDataLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
							newAssWriteDataLinker->initAccociationWriteData(nominalIndividual);

							findAssociationBackendLabel(indiNode,newAssWriteDataLinker,calcAlgContext);

							tmpAssWriteDataLinker = newAssWriteDataLinker->append(tmpAssWriteDataLinker);
						}
					}

					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker = nullptr;
					for (CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >::const_iterator it = mNewTmpLabelList->constBegin(), itEnd = mNewTmpLabelList->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTmpLabelLinker = (*it);
						newTmpLabelLinker->clearNext();
						tempLabelWriteDataLinker = newTmpLabelLinker->append(tempLabelWriteDataLinker);
					}

					CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker = nullptr;


					CBackendRepresentativeMemoryCacheLabelAssociationWriteData* writeData = CObjectAllocator< CBackendRepresentativeMemoryCacheLabelAssociationWriteData >::allocateAndConstruct(mMemAllocMan);
					writeData->initWriteData(tmpAssWriteDataLinker,tempLabelWriteDataLinker,tempCardWriteDataLinker);
					addCacheMessages(writeData,calcAlgContext);
					commitCacheMessages(calcAlgContext);

					return false;
				}










			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
