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

#ifndef KONCLUDE_REASONER_ANSWERER_CREDLANDSTOREDTRIPLESINDEXINGBASEBATCHLINKERVECTORDATA_CPP
#define KONCLUDE_REASONER_ANSWERER_CREDLANDSTOREDTRIPLESINDEXINGBASEBATCHLINKERVECTORDATA_CPP


#include "CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData.h"

namespace Konclude {

	namespace Reasoner {

		namespace Triples {

			template<class T>
			CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>::CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData(CMemoryAllocationManager* reallocationMemMan) : CMemoryPoolReallocationIncreasingContext(reallocationMemMan, 100, 500000, 2) {
				mBatchLinker = nullptr;
				mLastBatchLinker = nullptr;
			}


			template<class T>
			bool CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>::hasMoreBatchLinker() {
				return mBatchLinker != nullptr;
			}



			template<class T>
			CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>* CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>::mergeBatchLinkers(CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>* vecData) {
				if (vecData && vecData->mBatchLinker) {
					vecData->mLastBatchLinker->setNext(mBatchLinker);
					mBatchLinker = vecData->mBatchLinker;
				}
				return this;
			}

			template<class T>
			CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>* CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>::addBatchLinker(T* linker) {
				if (linker) {
					mBatchLinker = linker->append(mBatchLinker);
				}
				if (!mLastBatchLinker && mBatchLinker) {
					mLastBatchLinker = mBatchLinker->getLastListLink();
				}
				return this;
			}

			template<class T>
			T* CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>::takeNextBatchLinker() {
				T* tmpLinker = mBatchLinker;
				if (mBatchLinker) {
					mBatchLinker = mBatchLinker->getNext();
					if (!mBatchLinker) {
						mLastBatchLinker = nullptr;
					}
				}
				if (tmpLinker) {
					tmpLinker->clearNext();
				}
				return tmpLinker;
			}

			template<class T>
			T* CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>::getNextBatchLinker() const {
				T* tmpLinker = mBatchLinker;
				return tmpLinker;
			}


			template<class T>
			T* CRedlandStoredTriplesIndexingBaseBatchLinkerVectorData<T>::createBatchLinker() {
				T* linker = CObjectAllocator<T>::allocateAndConstruct(&mMemMan);
				return linker;
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CREDLANDSTOREDTRIPLESINDEXINGBASEBATCHLINKERVECTORDATA_CPP

