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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEBUCKET_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEBUCKET_CPP


#include "CDynamicReferenceBucket.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T, cint64 bucket2Size>
			CDynamicReferenceBucketBase<T,bucket2Size>::CDynamicReferenceBucketBase(CContext* context) : mContext(context) {
				mMemMan = CContext::getMemoryAllocationManager(mContext);
			}


			template<class T, cint64 bucket2Size>
			CDynamicReferenceBucketBase<T,bucket2Size>::~CDynamicReferenceBucketBase() {
			}


			template<class T, cint64 bucket2Size>
			CDynamicReferenceBucketBase<T,bucket2Size>* CDynamicReferenceBucketBase<T,bucket2Size>::getRelevantReferenceBucket(CDynamicReferenceBucketBase<T,bucket2Size>* bucket) {
				CDynamicReferenceBucketBase<T,bucket2Size>* relevantBucket = bucket;
				while (relevantBucket->mVec == nullptr) {
					relevantBucket = relevantBucket->mReferenceBucket;
				}
				return relevantBucket;
			}


			template<class T, cint64 bucket2Size>
			CDynamicReferenceBucketBase<T,bucket2Size>* CDynamicReferenceBucketBase<T,bucket2Size>::getUpdatedRelevantReferenceBucket(CDynamicReferenceBucketBase<T,bucket2Size>* bucket) {
				CDynamicReferenceBucketBase<T,bucket2Size>* relevantBucket = bucket;
				if (bucket) {
					while (relevantBucket && relevantBucket->mVec == nullptr) {
						relevantBucket = relevantBucket->mReferenceBucket;
					}
					bucket->mReferenceBucket = relevantBucket;
				}
				return relevantBucket;
			}

			
			template<class T, cint64 bucket2Size>
			void CDynamicReferenceBucketBase<T,bucket2Size>::installBucket(cint64 index, CDynamicReferenceBucketBase<T,bucket2Size>* bucket) {
				cint64 bucketIndex = getBucketIndex(index,this);
				if (mVec == nullptr) {
					bool isBaseBucket = mBucketDepth == 0;
					CDynamicReferenceBucketBase<T,bucket2Size>* refBucket = nullptr;
					refBucket = getUpdatedRelevantReferenceBucket(this);
					mVec = createBucketVector(refBucket,isBaseBucket);
				}
				CReferenceBucketDataPair* referenceBucketPair = &mVec[bucketIndex];
				referenceBucketPair->mReferenced = false;
				referenceBucketPair->mUninitialized = false;
				referenceBucketPair->mBucketData.mBucket = bucket;
			}


			template<class T, cint64 bucket2Size>
			bool CDynamicReferenceBucketBase<T,bucket2Size>::hasLocalData(cint64 index) {
				CDynamicReferenceBucketBase<T,bucket2Size>* nextBucket = this;
				while (nextBucket) {
					if (nextBucket->mVec == nullptr) {
						return false;
					}
					cint64 bucketIndex = getBucketIndex(index,nextBucket);
					CReferenceBucketDataPair* referenceBucketPair = &nextBucket->mVec[bucketIndex];
					if (referenceBucketPair->mReferenced) {
						return false;
					}
					bool isBaseBucket = nextBucket->mBucketDepth == 0;
					if (isBaseBucket) {
						return !referenceBucketPair->mUninitialized;
					}
					nextBucket = referenceBucketPair->mBucketData.mBucket;
				}
				return false;
			}

			template<class T, cint64 bucket2Size>
			bool CDynamicReferenceBucketBase<T,bucket2Size>::hasReferencedData(cint64 index) {
				CDynamicReferenceBucketBase<T,bucket2Size>* nextBucket = this;
				bool isReferenced = false;
				while (nextBucket) {
					if (nextBucket->mVec == nullptr) {
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						if (nextBucket == nullptr) {
							return false;
						}
						isReferenced = true;
					}
					cint64 bucketIndex = getBucketIndex(index,nextBucket);
					CReferenceBucketDataPair* referenceBucketPair = &nextBucket->mVec[bucketIndex];
					bool isBaseBucket = nextBucket->mBucketDepth == 0;
					if (isBaseBucket) {
						isReferenced |= referenceBucketPair->mReferenced;
						if (isReferenced) {
							return !referenceBucketPair->mUninitialized;
						} else {
							return false;
						}
					}
					if (referenceBucketPair->mReferenced) {
						// bucket is referenced
						isReferenced = true;						
						// update reference
						nextBucket = referenceBucketPair->mBucketData.mBucket;
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						referenceBucketPair->mBucketData.mBucket = nextBucket;
					} else {
						nextBucket = referenceBucketPair->mBucketData.mBucket;
					}
				}
				return false;
			}



			template<class T, cint64 bucket2Size>
			const T CDynamicReferenceBucketBase<T,bucket2Size>::getData(cint64 index, const T& defaultValue, bool* localFlag) {
				CDynamicReferenceBucketBase<T,bucket2Size>* nextBucket = this;
				bool isReferenced = false;
				while (nextBucket) {
					if (nextBucket->mVec == nullptr) {
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						if (nextBucket == nullptr) {
							return defaultValue;
						}
						isReferenced = true;
					}
					cint64 bucketIndex = getBucketIndex(index,nextBucket);
					CReferenceBucketDataPair* referenceBucketPair = &nextBucket->mVec[bucketIndex];
					bool isBaseBucket = nextBucket->mBucketDepth == 0;
					if (isBaseBucket) {
						isReferenced |= referenceBucketPair->mReferenced;
						if (referenceBucketPair->mUninitialized) {
							return defaultValue;
						} else {
							if (localFlag) {
								*localFlag = !isReferenced;
							}
							return referenceBucketPair->mBucketData.mData;
						}
					}
					if (referenceBucketPair->mReferenced) {
						// bucket is referenced
						isReferenced = true;						
						// update reference
						nextBucket = referenceBucketPair->mBucketData.mBucket;
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						referenceBucketPair->mBucketData.mBucket = nextBucket;
					} else {
						nextBucket = referenceBucketPair->mBucketData.mBucket;
					}
				}
				return defaultValue;
			}



			template<class T, cint64 bucket2Size>
			bool CDynamicReferenceBucketBase<T,bucket2Size>::hasData(cint64 index) {
				CDynamicReferenceBucketBase<T,bucket2Size>* nextBucket = this;
				while (nextBucket) {
					if (nextBucket->mVec == nullptr) {
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						if (nextBucket == nullptr) {
							return false;
						}
					}
					cint64 bucketIndex = getBucketIndex(index,nextBucket);
					CReferenceBucketDataPair* referenceBucketPair = &nextBucket->mVec[bucketIndex];
					bool isBaseBucket = nextBucket->mBucketDepth == 0;
					if (isBaseBucket) {
						return !referenceBucketPair->mUninitialized;
					}
					if (referenceBucketPair->mReferenced) {
						// bucket is referenced
						// update reference
						nextBucket = referenceBucketPair->mBucketData.mBucket;
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						referenceBucketPair->mBucketData.mBucket = nextBucket;
					} else {
						nextBucket = referenceBucketPair->mBucketData.mBucket;
					}
				}
				return false;
			}

			template<class T, cint64 bucket2Size>
			const T CDynamicReferenceBucketBase<T,bucket2Size>::getData(cint64 index, const T& defaultValue) {
				CDynamicReferenceBucketBase<T,bucket2Size>* nextBucket = this;
				while (nextBucket) {
					if (nextBucket->mVec == nullptr) {
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						if (nextBucket == nullptr) {
							return defaultValue;
						}
					}
					cint64 bucketIndex = getBucketIndex(index,nextBucket);
					CReferenceBucketDataPair* referenceBucketPair = &nextBucket->mVec[bucketIndex];
					bool isBaseBucket = nextBucket->mBucketDepth == 0;
					if (isBaseBucket) {
						if (referenceBucketPair->mUninitialized) {
							return defaultValue;
						} else {
							return referenceBucketPair->mBucketData.mData;
						}
					}
					if (referenceBucketPair->mReferenced) {
						// bucket is referenced
						// update reference
						nextBucket = referenceBucketPair->mBucketData.mBucket;
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						referenceBucketPair->mBucketData.mBucket = nextBucket;
					} else {
						nextBucket = referenceBucketPair->mBucketData.mBucket;
					}
				}
				return defaultValue;
			}


			template<class T, cint64 bucket2Size>
			cint64 CDynamicReferenceBucketBase<T,bucket2Size>::getBucketIndex(cint64 index) {
				cint64 bucketIndex = (index & (BUCKETINDEXMASK << mBucketDepth)) >> mBucketDepth;
				return bucketIndex;
			}


			template<class T, cint64 bucket2Size>
			cint64 CDynamicReferenceBucketBase<T,bucket2Size>::getBucketIndex(cint64 index, CDynamicReferenceBucketBase<T,bucket2Size>* bucket) {
				cint64 bucketDepth = bucket->mBucketDepth;
				cint64 bucketIndex = (index & (BUCKETINDEXMASK << bucketDepth)) >> bucketDepth;
				return bucketIndex;
			}


			template<class T, cint64 bucket2Size>
			const T CDynamicReferenceBucketBase<T,bucket2Size>::getLocalData(cint64 index, const T& defaultValue) {
				CDynamicReferenceBucketBase<T,bucket2Size>* nextBucket = this;
				while (nextBucket) {
					if (nextBucket->mVec == nullptr) {
						return defaultValue;
					}
					cint64 bucketIndex = getBucketIndex(index,nextBucket);
					CReferenceBucketDataPair* referenceBucketPair = &nextBucket->mVec[bucketIndex];
					if (referenceBucketPair->mReferenced) {
						return defaultValue;
					}
					bool isBaseBucket = nextBucket->mBucketDepth == 0;
					if (isBaseBucket) {
						if (!referenceBucketPair->mUninitialized) {
							return referenceBucketPair->mBucketData.mData;
						} else {
							return defaultValue;
						}
					}
					nextBucket = referenceBucketPair->mBucketData.mBucket;
				}
				return defaultValue;
			}

			template<class T, cint64 bucket2Size>
			const T CDynamicReferenceBucketBase<T,bucket2Size>::getReferenceData(cint64 index, const T& defaultValue) {
				CDynamicReferenceBucketBase<T,bucket2Size>* nextBucket = this;
				bool isReferenced = false;
				while (nextBucket) {
					if (!isReferenced && nextBucket->mReferenceBucket) {
						nextBucket = nextBucket->mReferenceBucket;
						isReferenced = true;
					}
					if (nextBucket->mVec == nullptr) {
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						if (nextBucket == nullptr) {
							return defaultValue;
						}
						isReferenced = true;
					}
					cint64 bucketIndex = getBucketIndex(index,nextBucket);
					CReferenceBucketDataPair* referenceBucketPair = &nextBucket->mVec[bucketIndex];
					bool isBaseBucket = nextBucket->mBucketDepth == 0;
					if (isBaseBucket) {
						isReferenced |= referenceBucketPair->mReferenced;
						if (isReferenced && !referenceBucketPair->mUninitialized) {
							return referenceBucketPair->mBucketData.mData;
						} else {
							return defaultValue;
						}
					}
					if (referenceBucketPair->mReferenced) {
						// bucket is referenced
						isReferenced = true;						
						// update reference
						nextBucket = referenceBucketPair->mBucketData.mBucket;
						nextBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						referenceBucketPair->mBucketData.mBucket = nextBucket;
					} else {
						nextBucket = referenceBucketPair->mBucketData.mBucket;
					}
				}
				return defaultValue;
			}


			template<class T, cint64 bucket2Size>
			void CDynamicReferenceBucketBase<T,bucket2Size>::setLocalData(cint64 index, const T& data) {
				CDynamicReferenceBucketBase<T,bucket2Size>* nextBucket = this;
				while (nextBucket) {
					bool isBaseBucket = nextBucket->mBucketDepth == 0;
					if (nextBucket->mVec == nullptr) {
						CDynamicReferenceBucketBase<T,bucket2Size>* refBucket = nullptr;
						refBucket = getUpdatedRelevantReferenceBucket(nextBucket);
						nextBucket->mVec = createBucketVector(refBucket,isBaseBucket);
					}
					cint64 bucketIndex = getBucketIndex(index,nextBucket);
					CReferenceBucketDataPair* referenceBucketPair = &nextBucket->mVec[bucketIndex];
					if (isBaseBucket) {
						referenceBucketPair->mBucketData.mData = data;
						referenceBucketPair->mReferenced = false;
						referenceBucketPair->mUninitialized = false;
						return;
					}
					if (referenceBucketPair->mReferenced || !referenceBucketPair->mBucketData.mBucket) {
						nextBucket = createBucket(nextBucket->mBucketDepth-(bucket2Size+1),referenceBucketPair);
					}
					CDynamicReferenceBucketBase<T,bucket2Size>* possNextBucket = referenceBucketPair->mBucketData.mBucket;
					if (!possNextBucket) {
						possNextBucket = createBucket(nextBucket->mBucketDepth-bucket2Size,referenceBucketPair);
					}
					nextBucket = possNextBucket;   
				}
			}

			template<class T, cint64 bucket2Size>
			void CDynamicReferenceBucketBase<T,bucket2Size>::setData(cint64 index, const T& data) {
				setLocalData(index,data);
			}


			template<class T, cint64 bucket2Size>
			void CDynamicReferenceBucketBase<T,bucket2Size>::initBucket(cint64 bucketDepth, CDynamicReferenceBucketBase<T,bucket2Size>* referenceBucket) {
				mBucketDepth = bucketDepth;
				mReferenceBucket = referenceBucket;
				mVec = nullptr;
			}


			template<class T, cint64 bucket2Size>
			CDynamicReferenceBucketBase<T,bucket2Size>* CDynamicReferenceBucketBase<T,bucket2Size>::createBucket(cint64 bucketDepth, CReferenceBucketDataPair* bucketPos) {
				CDynamicReferenceBucketBase<T,bucket2Size>* refBucket = nullptr;
				if (bucketPos && bucketPos->mReferenced) {
					refBucket = bucketPos->mBucketData.mBucket;
				}
				CDynamicReferenceBucketBase<T,bucket2Size>* bucket = CObjectParameterizingAllocator< CDynamicReferenceBucketBase<T,bucket2Size>,CContext* >::allocateAndConstructAndParameterize(mMemMan,mContext);
				bucket->initBucket(bucketDepth,refBucket);
				bucketPos->mReferenced = false;
				bucketPos->mUninitialized = false;
				bucketPos->mBucketData.mBucket = bucket;
				return bucket;
			}





		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEBUCKET_CPP
