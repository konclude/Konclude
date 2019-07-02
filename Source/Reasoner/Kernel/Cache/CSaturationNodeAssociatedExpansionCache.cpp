/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CSaturationNodeAssociatedExpansionCache.h"
#include "CSaturationNodeAssociatedExpansionCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSaturationNodeAssociatedExpansionCache::CSaturationNodeAssociatedExpansionCache(CConfiguration* config, QString threadIdentifierName, CWatchDog *watchDogThread) : CThread(threadIdentifierName,watchDogThread) {


					mReaderLinker = nullptr;

					CMemoryAllocationManager* memMan = mContext.getMemoryAllocationManager();
					mSaturationNodeCacheUpdate = new CSaturationNodeCacheUpdater(&mContext);

					mConfAllowedNonDetExpansionCount = 1;
					if (config) {
						mConfAllowedNonDetExpansionCount = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Optimization.SaturationExpansionSatisfiabilityCacheCount",1);
					}

					startThread(QThread::HighestPriority);
				}


				CSaturationNodeAssociatedExpansionCache::~CSaturationNodeAssociatedExpansionCache() {
					delete mSaturationNodeCacheUpdate;
				}



				CSaturationNodeAssociatedExpansionCache* CSaturationNodeAssociatedExpansionCache::writeCacheData(CSaturationNodeAssociatedExpansionCacheWriteData* writeData, CMemoryPool* memoryPools) {
					postEvent(new CWriteSaturationCacheDataEvent(writeData,memoryPools));
					return this;
				}



				CCacheStatistics* CSaturationNodeAssociatedExpansionCache::getCacheStatistics() {
					return &mCacheStat;
				}



				CSaturationNodeAssociatedExpansionCache* CSaturationNodeAssociatedExpansionCache::installWriteCacheData(CSaturationNodeAssociatedExpansionCacheWriteData* writeData, CSaturationNodeAssociatedExpansionCacheContext* context) {
					CSaturationNodeAssociatedExpansionCacheWriteData* writeDataLinker = writeData;
					while (writeDataLinker) {
						if (writeDataLinker->getWriteDataType() == CSaturationNodeAssociatedExpansionCacheWriteData::SNAECWT_UNSAT) {
							CSaturationNodeAssociatedExpansionCacheUnsatisfiabilityWriteData* snaecuwd = (CSaturationNodeAssociatedExpansionCacheUnsatisfiabilityWriteData*)writeDataLinker;
							CIndividualSaturationProcessNode* node = snaecuwd->getUnsatisfiableSaturationIndividualNode();
							propagateUnsatisfibility(node,context);							
						} else if (writeDataLinker->getWriteDataType() == CSaturationNodeAssociatedExpansionCacheWriteData::SNAECWT_EXPAND) {
							CSaturationNodeAssociatedExpansionCacheExpansionWriteData* snaecewd = (CSaturationNodeAssociatedExpansionCacheExpansionWriteData*)writeDataLinker;
							addNodeExpansionData(snaecewd,context);
						}
						writeDataLinker = (CSaturationNodeAssociatedExpansionCacheWriteData*)writeDataLinker->getNext();
					}
					return this;
				}



				CSaturationNodeAssociatedExpansionCache* CSaturationNodeAssociatedExpansionCache::addNodeExpansionData(CSaturationNodeAssociatedExpansionCacheExpansionWriteData* snaecewd, CSaturationNodeAssociatedExpansionCacheContext* context) {
					CIndividualSaturationProcessNode* node = snaecewd->getSaturationIndividualNode();
					CSaturationNodeAssociatedExpansionCacheEntry* cacheEntry = getCacheEntryForNode(node,context,true);
					if (cacheEntry) {

						if (snaecewd->isDeterministicExpansion()) {
							if (!cacheEntry->hasDeterministicConceptExpansion()) {
								CSaturationNodeAssociatedDeterministicConceptExpansion* detExpansion = CObjectParameterizingAllocator< CSaturationNodeAssociatedDeterministicConceptExpansion,CSaturationNodeAssociatedExpansionCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
								detExpansion->initDeterministicConceptExpansion();

								fillExpansionData(detExpansion,snaecewd,context);

								detExpansion->setNonDeterministicExpansionRequired(snaecewd->requiresNondeterministicExpansion());
								cacheEntry->setDeterministicConceptExpansion(detExpansion);

							} else {
								CSaturationNodeAssociatedDeterministicConceptExpansion* detExpansion = extendDeterministicExpansionData(cacheEntry->getDeterministicConceptExpansion(),snaecewd,context);
								detExpansion->setNonDeterministicExpansionRequired(snaecewd->requiresNondeterministicExpansion());
								cacheEntry->setDeterministicConceptExpansion(detExpansion);

							}
						} else {
							if (cacheEntry->areMoreNondeterministicExpansionAllowed()) {
								cacheEntry->decRemainingAllowedNondeterministicExpansionCount();

								CSaturationNodeAssociatedNondeterministicConceptExpansion* nondetExpansion = CObjectParameterizingAllocator< CSaturationNodeAssociatedNondeterministicConceptExpansion,CSaturationNodeAssociatedExpansionCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
								nondetExpansion->initNondeterministicConceptExpansion();

								fillExpansionData(nondetExpansion,snaecewd,context);

								cacheEntry->addNondeterministicConceptExpansion(nondetExpansion);
							}
						}
					}
					return this;
				}



				CSaturationNodeAssociatedDeterministicConceptExpansion* CSaturationNodeAssociatedExpansionCache::extendDeterministicExpansionData(CSaturationNodeAssociatedConceptExpansion* prevConceptExpansion, CSaturationNodeAssociatedExpansionCacheExpansionWriteData* snaecewd, CSaturationNodeAssociatedExpansionCacheContext* context) {
					CSaturationNodeAssociatedDeterministicConceptExpansion* newDetExpansion = nullptr;
					for (CSaturationNodeAssociatedConceptLinker* conceptExpansionLinkerIt = snaecewd->getExpansionConceptLinker(); conceptExpansionLinkerIt; conceptExpansionLinkerIt = conceptExpansionLinkerIt->getNext()) {
						CCacheValue* cacheValue = conceptExpansionLinkerIt->getCacheValue();

						if (!prevConceptExpansion->hasConceptExpansionLinker(cacheValue)) {
							if (!newDetExpansion) {
								newDetExpansion = CObjectParameterizingAllocator< CSaturationNodeAssociatedDeterministicConceptExpansion,CSaturationNodeAssociatedExpansionCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
								newDetExpansion->initDeterministicConceptExpansion();
							}
							CSaturationNodeAssociatedConceptLinker* conceptExpansionLinker = CObjectAllocator< CSaturationNodeAssociatedConceptLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
							conceptExpansionLinker->initConceptLinker(*cacheValue);
							newDetExpansion->addConceptExpansionLinker(conceptExpansionLinker);
						}
					}
					if (!newDetExpansion && !snaecewd->requiresNondeterministicExpansion()) {
						newDetExpansion = CObjectParameterizingAllocator< CSaturationNodeAssociatedDeterministicConceptExpansion,CSaturationNodeAssociatedExpansionCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
						newDetExpansion->initDeterministicConceptExpansion();
					}

					if (newDetExpansion) {
						CSaturationNodeAssociatedDependentNominalSet* nominalDependentSet = snaecewd->getDependentNominalSet();
						if (nominalDependentSet && !nominalDependentSet->isEmpty()) {
							for (CSaturationNodeAssociatedDependentNominalSet::const_iterator it = nominalDependentSet->constBegin(), itEnd = nominalDependentSet->constEnd(); it != itEnd; ++it) {
								cint64 nominalID = *it;
								newDetExpansion->getDependentNominalSet(true)->insert(nominalID);
							}
						}
						newDetExpansion->setHasTightCardinalityRestriction(snaecewd->hasTightAtMostRestriction());
						newDetExpansion->setConceptSetSignature(snaecewd->getConceptSetSignature());
						newDetExpansion->setTotalConceptCount(snaecewd->getTotalConceptCount());
					}

					return newDetExpansion;
				}



				CSaturationNodeAssociatedExpansionCache* CSaturationNodeAssociatedExpansionCache::fillExpansionData(CSaturationNodeAssociatedConceptExpansion* conceptExpansion, CSaturationNodeAssociatedExpansionCacheExpansionWriteData* snaecewd, CSaturationNodeAssociatedExpansionCacheContext* context) {
					for (CSaturationNodeAssociatedConceptLinker* conceptExpansionLinkerIt = snaecewd->getExpansionConceptLinker(); conceptExpansionLinkerIt; conceptExpansionLinkerIt = conceptExpansionLinkerIt->getNext()) {
						CCacheValue* cacheValue = conceptExpansionLinkerIt->getCacheValue();
						CSaturationNodeAssociatedConceptLinker* conceptExpansionLinker = CObjectAllocator< CSaturationNodeAssociatedConceptLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
						conceptExpansionLinker->initConceptLinker(*cacheValue);
						conceptExpansion->addConceptExpansionLinker(conceptExpansionLinker);
					}

					CSaturationNodeAssociatedDependentNominalSet* nominalDependentSet = snaecewd->getDependentNominalSet();
					if (nominalDependentSet && !nominalDependentSet->isEmpty()) {
						for (CSaturationNodeAssociatedDependentNominalSet::const_iterator it = nominalDependentSet->constBegin(), itEnd = nominalDependentSet->constEnd(); it != itEnd; ++it) {
							cint64 nominalID = *it;
							conceptExpansion->getDependentNominalSet(true)->insert(nominalID);
						}
					}

					conceptExpansion->setHasTightCardinalityRestriction(snaecewd->hasTightAtMostRestriction());
					conceptExpansion->setConceptSetSignature(snaecewd->getConceptSetSignature());
					conceptExpansion->setTotalConceptCount(snaecewd->getTotalConceptCount());
					
					return this;
				}





				CSaturationNodeAssociatedExpansionCacheEntry* CSaturationNodeAssociatedExpansionCache::getCacheEntryForNode(CIndividualSaturationProcessNode* node, CSaturationNodeAssociatedExpansionCacheContext* context, bool create) {
					CSaturationNodeAssociatedExpansionCacheEntry* cacheEntry = (CSaturationNodeAssociatedExpansionCacheEntry*)node->getCacheExpansionData();
					if (!cacheEntry) {
						cacheEntry = CObjectParameterizingAllocator< CSaturationNodeAssociatedExpansionCacheEntry,CSaturationNodeAssociatedExpansionCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
						cacheEntry->initCacheEntry(node,mConfAllowedNonDetExpansionCount);
						node->setCacheExpansionData(cacheEntry);
					}
					return cacheEntry;
				}




				CSaturationNodeAssociatedExpansionCache* CSaturationNodeAssociatedExpansionCache::propagateUnsatisfibility(CIndividualSaturationProcessNode* node, CSaturationNodeAssociatedExpansionCacheContext* context) {
					mSaturationNodeCacheUpdate->propagateUnsatisfibility(node,context);
					return this;
				}



				CSaturationNodeAssociatedExpansionCacheReader* CSaturationNodeAssociatedExpansionCache::createCacheReader() {
					CSaturationNodeAssociatedExpansionCacheReader* readerLinker = new CSaturationNodeAssociatedExpansionCacheReader();
					mReaderSyncMutex.lock();
					mReaderLinker = readerLinker->append(mReaderLinker);
					mReaderSyncMutex.unlock();
					return readerLinker;
				}


				CSaturationNodeAssociatedExpansionCacheWriter* CSaturationNodeAssociatedExpansionCache::createCacheWriter() {
					return new CSaturationNodeAssociatedExpansionCacheWriter(this);
				}




				bool CSaturationNodeAssociatedExpansionCache::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CThread::processCustomsEvents(type,event)) {
						return true;
					} else if (type == EVENTWRITESATURATIONCACHEDATAENTRY) {
						CWriteSaturationCacheDataEvent* wscde = (CWriteSaturationCacheDataEvent*)event;
						CMemoryPool* memoryPools = wscde->getMemoryPools();
						CSaturationNodeAssociatedExpansionCacheWriteData* writeData = wscde->getWriteData();
						installWriteCacheData(writeData,&mContext);
						mContext.getMemoryPoolAllocationManager()->releaseTemporaryMemoryPools(memoryPools);
						return true;

					}
					return false;
				}




			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
