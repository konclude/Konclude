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

#include "CCacheTaggingPool.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CCacheTaggingPool::CCacheTaggingPool(cint64 poolSize) {
					mTagPool = nullptr;
					mPoolIndex = 0;
					mPoolSize = poolSize;
					mPoolSizeMask = mPoolSize-1;
					mNextPoolStartTag = 0;

					qsrand(QTime::currentTime().msec());
				}


				CCacheTaggingPool::~CCacheTaggingPool() {
					delete mTagPool;
				}

				cint64 CCacheTaggingPool::takeNextTag() {
					cint64 nextTag = 0;
					if (!hasMoreTags()) {
						createNextTaggingPool();
					}
					nextTag = mTagPool[mPoolIndex++];
					return nextTag;
				}

				bool CCacheTaggingPool::hasMoreTags() {
					return mTagPool && mPoolIndex < mPoolSize;
				}

				CCacheTaggingPool* CCacheTaggingPool::createNextTaggingPool() {
					if (!mTagPool) {
						mTagPool = new cint64[mPoolSize];
					}
					for (cint64 i = 0; i < mPoolSize; ++i) {
						mTagPool[i] = i+mNextPoolStartTag;
					}
					mNextPoolStartTag += mPoolSize;
					// build random permutation using Fisher-Yates algorithm
					for (cint64 i = mPoolSize-1; i > 0; --i) {
						cint64 randomIndex = qrand() % (i+1);
						cint64 tmpVal = mTagPool[randomIndex];
						mTagPool[randomIndex] = mTagPool[i];
						mTagPool[i] = tmpVal;
					}
					mPoolIndex = 0;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
