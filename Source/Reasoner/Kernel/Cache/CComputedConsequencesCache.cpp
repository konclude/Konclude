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

#include "CComputedConsequencesCache.h"
#include "CComputedConsequencesCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CComputedConsequencesCache::CComputedConsequencesCache(CConfiguration* config, QString threadIdentifierName, CWatchDog *watchDogThread) : CThread(threadIdentifierName,watchDogThread) {


					mReaderLinker = nullptr;

					CMemoryAllocationManager* memMan = mContext.getMemoryAllocationManager();

					mConfAllowedNonDetExpansionCount = 1;
					if (config) {
						mConfAllowedNonDetExpansionCount = CConfigDataReader::readConfigInteger(config,"Konclude.Calculation.Optimization.SaturationExpansionSatisfiabilityCacheCount",1);
					}

					startThread(QThread::HighestPriority);
				}


				CComputedConsequencesCache::~CComputedConsequencesCache() {
				}



				CComputedConsequencesCache* CComputedConsequencesCache::writeCacheData(CComputedConsequencesCacheWriteData* writeData, CMemoryPool* memoryPools) {
					postEvent(new CWriteComputedConcequencesCacheEntryEvent(writeData,memoryPools));
					return this;
				}



				CCacheStatistics* CComputedConsequencesCache::getCacheStatistics() {
					return &mCacheStat;
				}



				CComputedConsequencesCache* CComputedConsequencesCache::installWriteCacheData(CComputedConsequencesCacheWriteData* writeData, CComputedConsequencesCacheContext* context) {
					CComputedConsequencesCacheWriteData* writeDataLinker = writeData;
					while (writeDataLinker) {
						if (writeDataLinker->getWriteDataType() == CComputedConsequencesCacheWriteData::CCWT_TYPE) {
							CComputedConsequencesCacheWriteTypesData* cccwtd = (CComputedConsequencesCacheWriteTypesData*)writeDataLinker;
							addTypesExpansionData(cccwtd,context);
						}
						writeDataLinker = (CComputedConsequencesCacheWriteData*)writeDataLinker->getNext();
					}
					return this;
				}



				CComputedConsequencesCache* CComputedConsequencesCache::addTypesExpansionData(CComputedConsequencesCacheWriteTypesData* cccwtd, CComputedConsequencesCacheContext* context) {
					CIndividual* individual = cccwtd->getIndividual();
					CConcept* conceptType = cccwtd->getConcept();
					bool conceptNegation = cccwtd->getNegation();
					CComputedConsequencesTypesCacheEntry* cacheEntry = getComputedTypesCacheEntryForNode(individual,context,true);
					if (cacheEntry) {
						CSortedNegLinker<CConcept*>* conceptLinker = context->createConceptLinker();
						conceptLinker->init(conceptType,conceptNegation);
						cacheEntry->addConceptLinker(conceptLinker);
					}
					return this;
				}





				CComputedConsequencesTypesCacheEntry* CComputedConsequencesCache::getComputedTypesCacheEntryForNode(CIndividual* individual, CComputedConsequencesCacheContext* context, bool create) {
					CIndividualProcessData* indProData = (CIndividualProcessData*)individual->getIndividualData();
					CComputedConsequencesTypesCacheEntry* cacheEntry = nullptr;
					if (indProData) {
						cacheEntry = (CComputedConsequencesTypesCacheEntry*)indProData->getComputedConsequencesCachingData();
					}
					if (indProData && !cacheEntry && create) {
						cacheEntry = CObjectParameterizingAllocator<CComputedConsequencesTypesCacheEntry,CComputedConsequencesCacheContext*>::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
						cacheEntry->initCacheEntry(individual);
						indProData->setComputedConsequencesCachingData(cacheEntry);
					}
					return cacheEntry;
				}





				CComputedConsequencesCacheReader* CComputedConsequencesCache::createCacheReader() {
					CComputedConsequencesCacheReader* readerLinker = new CComputedConsequencesCacheReader();
					mReaderSyncMutex.lock();
					mReaderLinker = readerLinker->append(mReaderLinker);
					mReaderSyncMutex.unlock();
					return readerLinker;
				}


				CComputedConsequencesCacheWriter* CComputedConsequencesCache::createCacheWriter() {
					return new CComputedConsequencesCacheWriter(this);
				}




				bool CComputedConsequencesCache::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CThread::processCustomsEvents(type,event)) {
						return true;
					} else if (type == EVENTWRITECOMPUTEDCONSEQUENCESCACHEDATAENTRY) {
						CWriteComputedConcequencesCacheEntryEvent* wscde = (CWriteComputedConcequencesCacheEntryEvent*)event;
						CMemoryPool* memoryPools = wscde->getMemoryPools();
						CComputedConsequencesCacheWriteData* writeData = wscde->getWriteData();
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
