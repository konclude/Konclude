/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEVECTOR_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEVECTOR_CPP


#include "CDynamicReferenceVector.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T, class S, cint64 bucket2Size>
			CDynamicReferenceVectorBase<T,S,bucket2Size>::CDynamicReferenceVectorBase(CContext* context) : mContext(context) {
				clear();
			}


			template<class T, class S, cint64 bucket2Size>
			CDynamicReferenceVectorBase<T,S,bucket2Size>::~CDynamicReferenceVectorBase() {
			}


			template<class T, class S, cint64 bucket2Size>
			const T CDynamicReferenceVectorBase<T,S,bucket2Size>::getData(cint64 index, const T& defaultValue) {
				if (mRootBucket && index >= 0 && index < mMaxIndex) {
					return mRootBucket->getData(index,defaultValue);
				}
				return defaultValue;
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDynamicReferenceVectorBase<T,S,bucket2Size>::getData(cint64 index, const T& defaultValue, bool* localFlag) {
				if (mRootBucket && index >= 0 && index < mMaxIndex) {
					return mRootBucket->getData(index,defaultValue,localFlag);
				}
				return defaultValue;
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDynamicReferenceVectorBase<T,S,bucket2Size>::getLocalData(cint64 index, const T& defaultValue) {
				if (mRootBucket && index >= 0 && index < mMaxIndex) {
					return mRootBucket->getLocalData(index,defaultValue);
				}
				return defaultValue;
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDynamicReferenceVectorBase<T,S,bucket2Size>::getReferenceData(cint64 index, const T& defaultValue) {
				if (mRootBucket && index >= 0 && index < mMaxIndex) {
					return mRootBucket->getReferenceData(index,defaultValue);
				}
				return defaultValue;
			}


			template<class T, class S, cint64 bucket2Size>
			S* CDynamicReferenceVectorBase<T,S,bucket2Size>::setLocalData(cint64 index, const T& data) {
				mMaxItem = qMax(index,mMaxItem);
				if (!mRootBucket) {
					mRootBucket = CObjectParameterizingAllocator< CDynamicReferenceBucketBase<T,bucket2Size>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
					mRootBucket->initBucket(0,nullptr);
					mRootBucketDepth = 0;
					mMaxIndex = 2<<bucket2Size;
				}
				while (index >= mMaxIndex) {
					CDynamicReferenceBucketBase<T,bucket2Size>* newRootBucket = CObjectParameterizingAllocator< CDynamicReferenceBucketBase<T,bucket2Size>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
					mRootBucketDepth += bucket2Size+1;
					newRootBucket->initBucket(mRootBucketDepth,nullptr);
					newRootBucket->installBucket(0,mRootBucket);
					mRootBucket = newRootBucket;
					if (mMaxIndex == 0) {
						mMaxIndex = 2<<bucket2Size;
					} else {
						mMaxIndex *= 2<<bucket2Size;
					}
				}
				mRootBucket->setLocalData(index,data);
				return (S*)this;
			}

			template<class T, class S, cint64 bucket2Size>
			S* CDynamicReferenceVectorBase<T,S,bucket2Size>::setData(cint64 index, const T& data) {
				setLocalData(index,data);
				return (S*)this;
			} 



			template<class T, class S, cint64 bucket2Size>
			bool CDynamicReferenceVectorBase<T,S,bucket2Size>::hasLocalData(cint64 index) {
				if (mRootBucket && index >= 0 && index < mMaxIndex) {
					return mRootBucket->hasLocalData(index);
				}
				return false;
			}

			template<class T, class S, cint64 bucket2Size>
			bool CDynamicReferenceVectorBase<T,S,bucket2Size>::hasData(cint64 index) {
				if (mRootBucket && index >= 0 && index < mMaxIndex) {
					return mRootBucket->hasData(index);
				}
				return false;
			}

			template<class T, class S, cint64 bucket2Size>
			bool CDynamicReferenceVectorBase<T,S,bucket2Size>::hasReferencedData(cint64 index) {
				if (mRootBucket && index >= 0 && index < mMaxIndex) {
					return mRootBucket->hasReferencedData(index);
				}
				return false;
			}


			template<class T, class S, cint64 bucket2Size>
			S* CDynamicReferenceVectorBase<T,S,bucket2Size>::clear() {
				mMaxIndex = 0;
				mRootBucketDepth = 0;
				mMaxItem = -1;
				mRootBucket = nullptr;
				mReferenceVector = nullptr;
				mReferencedMaxItem = -1;
				return (S*)this;
			}

			template<class T, class S, cint64 bucket2Size>
			S* CDynamicReferenceVectorBase<T,S,bucket2Size>::referenceVector(CDynamicReferenceVectorBase<T,S,bucket2Size>* refVector) {
				clear();
				mReferenceVector = refVector;
				if (mReferenceVector && mReferenceVector->mMaxIndex > 0) {
					mRootBucket = CObjectParameterizingAllocator< CDynamicReferenceBucketBase<T,bucket2Size>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
					mRootBucket->initBucket(refVector->mRootBucketDepth,mReferenceVector->mRootBucket);
					mRootBucketDepth = refVector->mRootBucketDepth;
					mMaxIndex = refVector->mMaxIndex;
					mMaxItem = refVector->mMaxItem;
					mReferencedMaxItem = refVector->mMaxItem;
				}
				return (S*)this;
			}


			template<class T, class S, cint64 bucket2Size>
			cint64 CDynamicReferenceVectorBase<T,S,bucket2Size>::getItemCount() {
				return mMaxItem+1;
			}

			template<class T, class S, cint64 bucket2Size>
			cint64 CDynamicReferenceVectorBase<T,S,bucket2Size>::getItemMaxIndex() {
				return mMaxItem;
			}

			template<class T, class S, cint64 bucket2Size>
			cint64 CDynamicReferenceVectorBase<T,S,bucket2Size>::getReferencedItemCount() {
				return mReferencedMaxItem+1;
			}

			template<class T, class S, cint64 bucket2Size>
			cint64 CDynamicReferenceVectorBase<T,S,bucket2Size>::getReferencedItemMaxIndex() {
				return mReferencedMaxItem;
			}

		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEVECTOR_CPP
