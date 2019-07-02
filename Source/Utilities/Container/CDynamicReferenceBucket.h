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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEBUCKET_H
#define KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEBUCKET_H

// Namespace includes
#include "ContainerSettings.h"

// Library includes

// Other includes
#include "Context/CContext.h"

// Logger includes


namespace Konclude {

	using namespace Context;

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CDynamicReferenceBucketBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, cint64 bucket2Size = 5>
			class CDynamicReferenceBucketBase { 
				// public methods
				public:
					//! Constructor
					inline CDynamicReferenceBucketBase(CContext* context = nullptr);

					//! Destructor
					inline ~CDynamicReferenceBucketBase();

					inline void initBucket(cint64 bucketDepth, CDynamicReferenceBucketBase<T,bucket2Size>* referenceBucket = nullptr);

					inline const T getData(cint64 index, const T& defaultValue);
					inline const T getData(cint64 index, const T& defaultValue, bool* localFlag);
					inline const T getLocalData(cint64 index, const T& defaultValue);
					inline const T getReferenceData(cint64 index, const T& defaultValue);

					inline void setLocalData(cint64 index, const T& data);
					inline void setData(cint64 index, const T& data);

					inline bool hasLocalData(cint64 index);
					inline bool hasReferencedData(cint64 index);
					inline bool hasData(cint64 index);

					inline void installBucket(cint64 index, CDynamicReferenceBucketBase<T,bucket2Size>* bucket);

				// protected variables
				protected:
					union CBucketDataPair {
						T mData;
						CDynamicReferenceBucketBase<T,bucket2Size>* mBucket;
					};

					struct CReferenceBucketDataPair {
						bool mReferenced;
						bool mUninitialized;
						CBucketDataPair mBucketData;
					};

				// protected methods
				protected:
					inline cint64 getBucketIndex(cint64 index);
					inline cint64 getBucketIndex(cint64 index, CDynamicReferenceBucketBase<T,bucket2Size>* bucket);

					inline CDynamicReferenceBucketBase<T,bucket2Size>* getRelevantReferenceBucket(CDynamicReferenceBucketBase<T,bucket2Size>* bucket);
					inline CDynamicReferenceBucketBase<T,bucket2Size>* getUpdatedRelevantReferenceBucket(CDynamicReferenceBucketBase<T,bucket2Size>* bucket);


					inline CReferenceBucketDataPair* createBucketVector(CDynamicReferenceBucketBase<T,bucket2Size>* bucket, bool baseVector) {
						CReferenceBucketDataPair* refVector = nullptr;
						if (bucket) {
							refVector = bucket->mVec;
						}
						CReferenceBucketDataPair* vector = CObjectAllocator<CReferenceBucketDataPair>::allocateArray(mMemMan,BUCKETSIZE);
						for (cint64 ind = 0; ind < BUCKETSIZE; ++ind) {
							CReferenceBucketDataPair* refBucketDataPair = &vector[ind];
							if (refVector) {
								if (baseVector) {
									refBucketDataPair->mReferenced = true;
									refBucketDataPair->mUninitialized = refVector[ind].mUninitialized;
									refBucketDataPair->mBucketData.mData = refVector[ind].mBucketData.mData;
								} else {
									refBucketDataPair->mReferenced = true;
									refBucketDataPair->mUninitialized = false;
									refBucketDataPair->mBucketData.mBucket = refVector[ind].mBucketData.mBucket;
								}
							} else {
								refBucketDataPair->mReferenced = false;
								refBucketDataPair->mUninitialized = true;
								refBucketDataPair->mBucketData.mBucket = nullptr;
							}
						}
						return vector;
					}

					inline CDynamicReferenceBucketBase<T,bucket2Size>* createBucket(cint64 bucketDepth, CReferenceBucketDataPair* bucketPos);

				// protected variables
				protected:
					CDynamicReferenceBucketBase<T,bucket2Size>* mReferenceBucket;
					CReferenceBucketDataPair* mVec;
					cint64 mBucketDepth;

					CContext* mContext;
					CMemoryAllocationManager* mMemMan;

					static const cint64 BUCKETSIZE = 2<<bucket2Size;
					static const cint64 BUCKETINDEXMASK = BUCKETSIZE-1;

				// private methods
				private:

				// private variables
				private:
			};

			class CTestDynamicReferenceBucketBase : public CDynamicReferenceBucketBase<cint64> {
			};



		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CDynamicReferenceBucket.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEBUCKET_H
