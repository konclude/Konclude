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

#include "CSignatureSatisfiableExpanderCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSignatureSatisfiableExpanderCacheWriter::CSignatureSatisfiableExpanderCacheWriter(CSignatureSatisfiableExpanderCache* cache) : mCache(cache) {
				}


				CSignatureSatisfiableExpanderCacheWriter* CSignatureSatisfiableExpanderCacheWriter::writeCachedData(CSignatureSatisfiableExpanderCacheEntryWriteData* writeData, CMemoryPool* memoryPools) {
					mCache->writeCachedData(writeData,memoryPools);
					return this;
				}


				CSignatureSatisfiableExpanderCacheWriter* CSignatureSatisfiableExpanderCacheWriter::writeExpandCached(cint64 prevSignature, cint64 newSignature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGHASH<cint64,cint64>* depHash, CMemoryPool* memoryPools) {
					mCache->writeExpandCached(prevSignature,newSignature,cacheValueList,depHash,memoryPools);
					return this;
				}

				CSignatureSatisfiableExpanderCacheWriter* CSignatureSatisfiableExpanderCacheWriter::writeSatisfiableBranchCached(cint64 signature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGLIST<CCacheValue>* branchedList, CMemoryPool* memoryPools) {
					mCache->writeSatisfiableBranchCached(signature,cacheValueList,branchedList,memoryPools);
					return this;
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
