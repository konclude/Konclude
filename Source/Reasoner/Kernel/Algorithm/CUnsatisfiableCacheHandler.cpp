/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CUnsatisfiableCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CUnsatisfiableCacheHandler::CUnsatisfiableCacheHandler(COccurrenceUnsatisfiableCacheReader* occurUnsatCacheReader, COccurrenceUnsatisfiableCacheWriter* occurUnsatCacheWriter) {
					mOccurUnsatCacheReader = occurUnsatCacheReader;
					mOccurUnsatCacheWriter = occurUnsatCacheWriter;
					mConfConceptDataUnsatisfiablePrecheck = true;
				}


				CUnsatisfiableCacheHandler::~CUnsatisfiableCacheHandler() {
				}


				bool CUnsatisfiableCacheHandler::isIndividualNodeUnsatisfiableCached(CIndividualProcessNode* individualNode, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(UNSATCACHERETRIEVALCOUNT,calcAlgContext);
					bool unsatisfiableChecked = false;
					CConceptDescriptor* lastConDes = nullptr;
					cint64 lastRetCachingTag = 0;

					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					CConceptDescriptor* conDesLinker = conSet->getAddingSortedConceptDescriptionLinker();
					cint64 currentCachingTag = mOccurUnsatCacheReader->getCurrentCachingTag();

					CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData* unsatRetData = (CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData*)individualNode->getIndividualUnsatisfiableCacheRetrievalData(false);
					if (unsatRetData) {
						lastRetCachingTag = unsatRetData->getLastRetrievalCachingTag();
						lastConDes = unsatRetData->getLastRetrievalConceptDescriptor();
						// nothing has changed
						if (lastRetCachingTag >= currentCachingTag && lastConDes == conDesLinker) {
							return false;
						}
					}

					if (lastConDes != conDesLinker) {
						lastRetCachingTag = 0;
					}

					if (mConfConceptDataUnsatisfiablePrecheck) {
						STATINC(UNSATCACHERETRIEVALPRECHECKCOUNT,calcAlgContext);
						cint64 minMaxCachedTag = CINT64_MAX;
						cint64 maxMinCachedTag = CINT64_MIN;
						cint64 minUnsatCachedSize = CINT64_MAX;
						cint64 possCachedCount = 0;
						cint64 maxLastCachingTag = 0;
						CConceptDescriptor* conDesIt = conDesLinker;
						while (conDesIt && lastConDes != conDesIt) {
							CConcept* concept = conDesIt->getConcept();
							bool conNeg = conDesIt->getNegation();
							CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
							if (conProcData) {
								CUnsatisfiableCachingTags* cachingTags = conProcData->getUnsatisfiableCachingTags(conNeg);
								if (cachingTags) {
									if (cachingTags->candidateTags(minMaxCachedTag,maxMinCachedTag,minUnsatCachedSize,lastRetCachingTag+1)) {
										++possCachedCount;
										lastConDes = nullptr;
									}
								}
							}
							conDesIt = conDesIt->getNext();
						}
						if (possCachedCount < minUnsatCachedSize) {
							STATINC(UNSATCACHERETRIEVALPRECHECKDIRECTFAILEDCOUNT,calcAlgContext);
							unsatisfiableChecked = true;
						} else if (minMaxCachedTag == maxMinCachedTag) {
							// try to get unsatisfiable concepts
							cint64 unsatLineCount = CINT64_MAX;
							cint64 foundUnsatLineCount = 0;
							CConceptDescriptor* conDesIt = conDesLinker;
							while (conDesIt && lastConDes != conDesIt && foundUnsatLineCount <= unsatLineCount) {
								CConcept* concept = conDesIt->getConcept();
								bool conNeg = conDesIt->getNegation();
								CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
								if (conProcData) {
									CUnsatisfiableCachingTags* cachingTags = conProcData->getUnsatisfiableCachingTags(conNeg);
									if (cachingTags) {
										if (cachingTags->hasCandidateTags(minMaxCachedTag,maxMinCachedTag,lastRetCachingTag+1)) {
											++foundUnsatLineCount;
											cachingTags->candidateMinUnsatisfiableSize(unsatLineCount,maxMinCachedTag);
										}
									}
								}
								conDesIt = conDesIt->getNext();
							}

							if (foundUnsatLineCount == unsatLineCount) {
								// found unsatisfiable concepts, try to extract clashes
								cint64 clashLinkerGenCount = 0;
								CClashedDependencyDescriptor* clashedDepDesLinker = nullptr;
								CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(false,true,false);
								while (conSetIt.hasNext()) {
									CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
									CDependencyTrackPoint* depTrackPoint = conSetIt.getDependencyTrackPoint();
									CConcept* concept = conDes->getConcept();
									bool conNeg = conDes->getNegation();

									CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
									if (conProcData) {
										CUnsatisfiableCachingTags* cachingTags = conProcData->getUnsatisfiableCachingTags(conNeg);
										if (cachingTags) {
											if (cachingTags->hasCandidateTags(minMaxCachedTag,maxMinCachedTag,lastRetCachingTag+1)) {
												++clashLinkerGenCount;
												clashedDepDesLinker = calcAlgContext->getUsedClashDescriptorFactory()->createClashedConceptDescriptor(clashedDepDesLinker,individualNode,conDes,depTrackPoint,calcAlgContext);
												if (clashLinkerGenCount == unsatLineCount) {
													clashDescriptors = clashedDepDesLinker->append(clashDescriptors);
													STATINC(UNSATCACHERETRIEVALPRECHECKSUCCESSCOUNT,calcAlgContext);
													return true;
												}
											}
										}
									}
									conSetIt.moveNext();
								}
								STATINC(UNSATCACHERETRIEVALPRECHECKINSUFFICIENTCOUNT,calcAlgContext);
							} else {
								STATINC(UNSATCACHERETRIEVALPRECHECKINSUFFICIENTCOUNT,calcAlgContext);
							}
						}
					}

					CMemoryAllocationManager* memMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();

					if (!unsatisfiableChecked) {
						STATINC(UNSATCACHERETRIEVALHASHCOUNT,calcAlgContext);
						CXLinker<CCacheValue*>* cacheTestLinker = nullptr;
						CXLinker<CCacheValue*>* lastCacheTestLinker = nullptr;
						// build concept linkers for cache testing
						CReapplyConceptLabelSetIterator conSetIt = conSet->getConceptLabelSetIterator(true);
						while (conSetIt.hasNext()) {
							CConceptDescriptor* conDes = conSetIt.getConceptDescriptor();
							CConcept* concept = conDes->getConcept();
							bool conNeg = conDes->getNegation();
							CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
							if (conProcData) {
								CUnsatisfiableCachingTags* cachingTags = conProcData->getUnsatisfiableCachingTags(conNeg);
								if (cachingTags) {
									CCacheValue* cacheValue = CObjectAllocator<CCacheValue>::allocateAndConstruct(memMan);
									if (!conNeg) {
										cacheValue->initCacheValue(concept->getConceptTag(),(cint64)concept,CCacheValue::CACHEVALTAGANDCONCEPT);
									} else {
										cacheValue->initCacheValue(concept->getConceptTag(),(cint64)concept,CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT);
									}
									CXLinker<CCacheValue*>* nextCacheTestLinker = CObjectAllocator< CXLinker<CCacheValue*> >::allocateAndConstruct(memMan);
									nextCacheTestLinker->initLinker(cacheValue);
									if (!lastCacheTestLinker) {
										cacheTestLinker = nextCacheTestLinker;
										lastCacheTestLinker = nextCacheTestLinker;
									} else {
										lastCacheTestLinker->append(nextCacheTestLinker);
										lastCacheTestLinker = nextCacheTestLinker;
									}
								}
							}
							conSetIt.moveNext();
						}

						if (cacheTestLinker) {
							CXLinker<CCacheValue*>* unsatLinker = mOccurUnsatCacheReader->getUnsatisfiableItems(cacheTestLinker,memMan);
							if (!unsatLinker) {
								// not unsatisfiable
								unsatisfiableChecked = true;
								STATINC(UNSATCACHERETRIEVALHASHFAILEDCOUNT,calcAlgContext);
							} else {
								// found unsatisfiable cached concepts, extract clashes
								CXLinker<CCacheValue*>* unsatLinkerIt = unsatLinker;
								while (unsatLinkerIt) {
									CCacheValue* cacheValue = unsatLinkerIt->getData();
									CConcept* concept = (CConcept*)cacheValue->getIdentification();
									CConceptDescriptor* conDes = nullptr;
									CDependencyTrackPoint* depTrackPoint = nullptr;
									conSet->getConceptDescriptor(concept,conDes,depTrackPoint);

									clashDescriptors = calcAlgContext->getUsedClashDescriptorFactory()->createClashedConceptDescriptor(clashDescriptors,individualNode,conDes,depTrackPoint,calcAlgContext);

									unsatLinkerIt = unsatLinkerIt->getNext();
									STATINC(UNSATCACHERETRIEVALHASHSUCCESSCOUNT,calcAlgContext);
								}
								return true;
							}
						}
						return false;
					}

					if (unsatisfiableChecked) {

						CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData* locUnsatRetData = (CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData*)individualNode->getIndividualUnsatisfiableCacheRetrievalData(true);
						if (!locUnsatRetData) {
							CMemoryAllocationManager* taskMemMan = calcAlgContext->getUsedProcessTaskMemoryAllocationManager();
							locUnsatRetData = CObjectAllocator<CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData>::allocateAndConstruct(taskMemMan);
							individualNode->setIndividualUnsatisfiableCacheRetrievalData(locUnsatRetData);
						}
						locUnsatRetData->setLastRetrievalCachingTag(currentCachingTag);
						locUnsatRetData->setLastRetrievalConceptDescriptor(conDesLinker);
					}

					return false;
				}



				bool CUnsatisfiableCacheHandler::areClashedDescriptorsLastCached(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContext* calcAlgContext) {
					CTrackedClashedDescriptor* trackedClashedDesIt = trackedClashedDes;
					for (QList<CCacheValue>::const_iterator it = mUnsatItemList.constBegin(), itEnd = mUnsatItemList.constEnd(); it != itEnd; ++it) {
						if (!trackedClashedDesIt) {
							return false;
						}
						CCacheValue cachedValueItem(*it);

						CConceptDescriptor* conDes = trackedClashedDesIt->getConceptDescriptor();
						CConcept* concept = conDes->getConcept();
						bool conNeg = conDes->getNegation();

						if (conNeg) {
							if (cachedValueItem.getCacheValueIdentifier() != CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT) {
								return false;
							}
						} else {
							if (cachedValueItem.getCacheValueIdentifier() != CCacheValue::CACHEVALTAGANDCONCEPT) {
								return false;
							}
						}
						if ((CConcept*)cachedValueItem.getIdentification() != concept) {
							return false;
						}

						trackedClashedDesIt = trackedClashedDesIt->getNextDescriptor();

					}
					if (trackedClashedDesIt) {
						return false;
					}
					return true;
				}


				bool CUnsatisfiableCacheHandler::areClashedDescriptorsAlreadyCached(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(UNSATCACHEWRITINGALREADYCACHEDTESTCOUNT,calcAlgContext);

					if (areClashedDescriptorsLastCached(trackedClashedDes,calcAlgContext)) {
						return true;
					}

					CTrackedClashedDescriptor* trackedClashedDesIt = trackedClashedDes;
					while (trackedClashedDesIt) {
						CConceptDescriptor* conDes = trackedClashedDesIt->getConceptDescriptor();
						CConcept* concept = conDes->getConcept();
						bool conNeg = conDes->getNegation();
						CConceptProcessData* conProcData = (CConceptProcessData*)concept->getConceptData();
						bool clashDesCached = false;
						if (conProcData) {
							CUnsatisfiableCachingTags* cachingTags = conProcData->getUnsatisfiableCachingTags(conNeg);
							if (cachingTags) {
								if (cachingTags->getLastCachingTag() > 0) {
									clashDesCached = true;
								}
							}
						}
						if (!clashDesCached) {
							STATINC(UNSATCACHEWRITINGALREADYCACHEDTESTPRECHECKFAILEDCOUNT,calcAlgContext);
							return false;
						}
						trackedClashedDesIt = trackedClashedDesIt->getNextDescriptor();
					}
					CMemoryAllocationManager* memMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
					CXLinker<CCacheValue*>* cacheTestLinker = nullptr;
					CXLinker<CCacheValue*>* lastCacheTestLinker = nullptr;
					trackedClashedDesIt = trackedClashedDes;
					CCacheValue* lastCacheValue = nullptr;
					while (trackedClashedDesIt) {
						CConceptDescriptor* conDes = trackedClashedDesIt->getConceptDescriptor();
						CConcept* concept = conDes->getConcept();
						bool conNeg = conDes->getNegation();

						CCacheValue* cacheValue = CObjectAllocator<CCacheValue>::allocateAndConstruct(memMan);
						if (!conNeg) {
							cacheValue->initCacheValue(concept->getConceptTag(),(cint64)concept,CCacheValue::CACHEVALTAGANDCONCEPT);
						} else {
							cacheValue->initCacheValue(concept->getConceptTag(),(cint64)concept,CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT);
						}
						if (!lastCacheValue || *lastCacheValue != *cacheValue) {
							CXLinker<CCacheValue*>* nextCacheTestLinker = CObjectAllocator< CXLinker<CCacheValue*> >::allocateAndConstruct(memMan);
							nextCacheTestLinker->initLinker(cacheValue);
							if (!lastCacheTestLinker) {
								cacheTestLinker = nextCacheTestLinker;
								lastCacheTestLinker = nextCacheTestLinker;
							} else {
								lastCacheTestLinker->append(nextCacheTestLinker);
								lastCacheTestLinker = nextCacheTestLinker;
							}
							lastCacheValue = cacheValue;
						}
						trackedClashedDesIt = trackedClashedDesIt->getNextDescriptor();
					}
					CXLinker<CCacheValue*>* unsatLinker = mOccurUnsatCacheReader->getUnsatisfiableItems(cacheTestLinker,memMan);
					if (unsatLinker) {
						STATINC(UNSATCACHEWRITINGALREADYCACHEDTESTHASHFAILEDCOUNT,calcAlgContext);
					}
					return unsatLinker != nullptr;
				}


				bool CUnsatisfiableCacheHandler::writeUnsatisfiableClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContext* calcAlgContext) {
					STATINC(UNSATCACHEWRITINGTRYINGCOUNT,calcAlgContext);
					if (!areClashedDescriptorsAlreadyCached(trackedClashedDes,calcAlgContext)) {
						mUnsatItemList.clear();
						CTrackedClashedDescriptor* trackedClashedDesIt = trackedClashedDes;
						while (trackedClashedDesIt) {
							CConceptDescriptor* conDes = trackedClashedDesIt->getConceptDescriptor();
							if (conDes->isNegated()) {
								mUnsatItemList.append(CCacheValue(conDes->getConceptTag(),(cint64)conDes->getConcept(),CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT));
							} else {
								mUnsatItemList.append(CCacheValue(conDes->getConceptTag(),(cint64)conDes->getConcept(),CCacheValue::CACHEVALTAGANDCONCEPT));
							}
							trackedClashedDesIt = trackedClashedDesIt->getNextDescriptor();
						}
						mOccurUnsatCacheWriter->setUnsatisfiable(mUnsatItemList);
						STATINC(UNSATCACHEWRITINGCOUNT,calcAlgContext);
						return true;
					} else {
						STATINC(UNSATCACHEWRITINGALREADYCACHEDCOUNT,calcAlgContext);
					}
					return false;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
