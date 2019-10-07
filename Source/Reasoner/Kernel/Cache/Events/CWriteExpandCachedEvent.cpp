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

#include "CWriteExpandCachedEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				namespace Events {


					CWriteExpandCachedEvent::CWriteExpandCachedEvent(cint64 prevSignature, cint64 newSignature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGHASH<cint64,cint64>* depHash, CMemoryPool* memoryPools) 
							: CCustomEvent(EVENTTYPE),mPrevSignature(prevSignature),mNewSignature(newSignature),mCacheValueList(cacheValueList),mDepHash(depHash),mMemoryPools(memoryPools) {
					}


					cint64 CWriteExpandCachedEvent::getPrevSignature() {
						return mPrevSignature;
					}

					cint64 CWriteExpandCachedEvent::getNewSignature() {
						return mNewSignature;
					}

					CCACHINGLIST<CCacheValue>* CWriteExpandCachedEvent::getCacheValueList() {
						return mCacheValueList;
					}

					CCACHINGHASH<cint64,cint64>* CWriteExpandCachedEvent::getDepHash() {
						return mDepHash;
					}

					CMemoryPool* CWriteExpandCachedEvent::getMemoryPools() {
						return mMemoryPools;
					}


				}; // end namespace Events

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
