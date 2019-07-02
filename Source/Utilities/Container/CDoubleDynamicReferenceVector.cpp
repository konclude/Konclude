/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_UTILITIES_CONTAINER_CDOUBLEDYNAMICREFERENCEVECTOR_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CDOUBLEDYNAMICREFERENCEVECTOR_CPP


#include "CDoubleDynamicReferenceVector.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T, class S, cint64 bucket2Size>
			CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::CDoubleDynamicReferenceVectorBase(CContext* context) : mFirstVector(context),mSecondVector(context) {
				clear();
			}


			template<class T, class S, cint64 bucket2Size>
			CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::~CDoubleDynamicReferenceVectorBase() {
			}


			template<class T, class S, cint64 bucket2Size>
			const T CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getData(cint64 index, const T& defaultValue) {
				if (index <= 0) {
					return mFirstVector.getData(-(index),defaultValue);
				} else {
					return mSecondVector.getData(index,defaultValue);
				}
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getData(cint64 index, const T& defaultValue, bool* localFlag) {
				if (index <= 0) {
					return mFirstVector.getData(-(index),defaultValue,localFlag);
				} else {
					return mSecondVector.getData(index,defaultValue,localFlag);
				}
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getLocalData(cint64 index, const T& defaultValue) {
				if (index <= 0) {
					return mFirstVector.getLocalData(-(index),defaultValue);
				} else {
					return mSecondVector.getLocalData(index,defaultValue);
				}
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getReferenceData(cint64 index, const T& defaultValue) {
				if (index <= 0) {
					return mFirstVector.getReferenceData(-(index),defaultValue);
				} else {
					return mSecondVector.getReferenceData(index,defaultValue);
				}
			}


			template<class T, class S, cint64 bucket2Size>
			S* CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::setLocalData(cint64 index, const T& data) {
				if (index <= 0) {
					mFirstVector.setLocalData(-(index),data);
				} else {
					mSecondVector.setLocalData(index,data);
				}
				return (S*)this;
			}

			template<class T, class S, cint64 bucket2Size>
			S* CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::setData(cint64 index, const T& data) {
				if (index <= 0) {
					mFirstVector.setData(-(index),data);
				} else {
					mSecondVector.setData(index,data);
				}
				return (S*)this;
			} 



			template<class T, class S, cint64 bucket2Size>
			bool CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::hasLocalData(cint64 index) {
				if (index <= 0) {
					return mFirstVector.hasLocalData(-(index));
				} else {
					return mSecondVector.hasLocalData(index);
				}
			}

			template<class T, class S, cint64 bucket2Size>
			bool CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::hasData(cint64 index) {
				if (index <= 0) {
					return mFirstVector.hasData(-(index));
				} else {
					return mSecondVector.hasData(index);
				}
			}

			template<class T, class S, cint64 bucket2Size>
			bool CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::hasReferencedData(cint64 index) {
				if (index <= 0) {
					return mFirstVector.hasReferencedData(-(index));
				} else {
					return mSecondVector.hasReferencedData(index);
				}
			}


			template<class T, class S, cint64 bucket2Size>
			S* CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::clear() {
				mFirstVector.clear();
				mSecondVector.clear();
				return (S*)this;
			}

			template<class T, class S, cint64 bucket2Size>
			S* CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::referenceVector(CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>* refVector) {
				clear();
				mFirstVector.referenceVector(&refVector->mFirstVector);
				mSecondVector.referenceVector(&refVector->mSecondVector);
				return (S*)this;
			}


			template<class T, class S, cint64 bucket2Size>
			cint64 CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getItemCount() {
				return mFirstVector.getItemCount()+mSecondVector.getItemCount();
			}

			template<class T, class S, cint64 bucket2Size>
			cint64 CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getItemMaxIndex() {
				return mSecondVector.getItemMaxIndex();
			}

			template<class T, class S, cint64 bucket2Size>
			cint64 CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getItemMinIndex() {
				return -mFirstVector.getItemMaxIndex();
			}

			template<class T, class S, cint64 bucket2Size>
			cint64 CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getReferencedItemCount() {
				return mFirstVector.getReferencedItemCount()+mSecondVector.getReferencedItemCount();
			}

			template<class T, class S, cint64 bucket2Size>
			cint64 CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getReferencedItemMaxIndex() {
				return mSecondVector.getItemCount();
			}

			template<class T, class S, cint64 bucket2Size>
			cint64 CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getReferencedItemMinIndex() {
				return -mFirstVector.getItemCount();
			}

		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CDOUBLEDYNAMICREFERENCEVECTOR_CPP
