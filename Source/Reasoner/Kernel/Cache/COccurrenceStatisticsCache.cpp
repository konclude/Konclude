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

#include "COccurrenceStatisticsCache.h"
#include "COccurrenceStatisticsCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceStatisticsCache::COccurrenceStatisticsCache(CConfiguration* config, QString threadIdentifierName, CWatchDog *watchDogThread) : CThread(threadIdentifierName, watchDogThread) {
					mWriteDataCount = 0;

					mConfig = config;

					mCacheData = new COccurrenceStatisticsCacheData();

					
					startThread(QThread::HighestPriority);
				}


				COccurrenceStatisticsCache::~COccurrenceStatisticsCache() {
				}



				CCacheStatistics* COccurrenceStatisticsCache::getCacheStatistics() {
					return &mCacheStat;
				}



				COccurrenceStatisticsCacheReader* COccurrenceStatisticsCache::createCacheReader() {
					COccurrenceStatisticsCacheReader* reader = new COccurrenceStatisticsCacheReader(mCacheData);
					return reader;
				}




				COccurrenceStatisticsCacheWriter* COccurrenceStatisticsCache::createCacheWriter() {
					COccurrenceStatisticsCacheWriter* writer = new COccurrenceStatisticsCacheWriter(this, mCacheData);
					return writer;
				}



				COccurrenceStatisticsCache* COccurrenceStatisticsCache::writeCachedData(COccurrenceStatisticsCacheWriteData* writeData, CMemoryPool* memoryPools) {
					postEvent(new CWriteCachedDataEvent(writeData, memoryPools));
					return this;
				}




				bool COccurrenceStatisticsCache::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CThread::processCustomsEvents(type, event)) {
						return true;

					} else if (type == EVENTWRITECACHEDDATAENTRY) {
						CWriteCachedDataEvent* wcde = (CWriteCachedDataEvent*)event;
						CMemoryPool* memoryPools = wcde->getMemoryPools();


						mContext.releaseTemporaryMemoryPools(memoryPools);
						mCacheStat.setMemoryConsumption(mContext.getMemoryConsumption());

						return true;
					}
					return false;
				}




			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
