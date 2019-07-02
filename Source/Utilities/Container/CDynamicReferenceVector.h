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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEVECTOR_H
#define KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEVECTOR_H

// Namespace includes
#include "ContainerSettings.h"

// Library includes

// Other includes
#include "CDynamicReferenceBucket.h"

// Logger includes


namespace Konclude {

	using namespace Context;

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CDynamicReferenceVectorBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class S, cint64 bucket2Size = 5>
			class CDynamicReferenceVectorBase {
				// public methods
				public:
					//! Constructor
					inline CDynamicReferenceVectorBase(CContext* context = nullptr);

					//! Destructor
					inline ~CDynamicReferenceVectorBase();

					inline const T getData(cint64 index, const T& defaultValue);
					inline const T getData(cint64 index, const T& defaultValue, bool* localFlag);
					inline const T getLocalData(cint64 index, const T& defaultValue);
					inline const T getReferenceData(cint64 index, const T& defaultValue);

					inline S* setLocalData(cint64 index, const T& data);
					inline S* setData(cint64 index, const T& data);

					inline bool hasLocalData(cint64 index);
					inline bool hasData(cint64 index);
					inline bool hasReferencedData(cint64 index);

					inline S* clear();
					inline S* referenceVector(CDynamicReferenceVectorBase<T,S,bucket2Size>* refVector);

					inline cint64 getItemCount();
					inline cint64 getItemMaxIndex();

					inline cint64 getReferencedItemCount();
					inline cint64 getReferencedItemMaxIndex();

				// protected methods
				protected:

				// protected variables
				protected:
					CContext* mContext;
					cint64 mMaxIndex;
					cint64 mRootBucketDepth;
					cint64 mMaxItem;
					cint64 mReferencedMaxItem;

					CDynamicReferenceVectorBase<T,S,bucket2Size>* mReferenceVector;

					CDynamicReferenceBucketBase<T,bucket2Size>* mRootBucket;

				// private methods
				private:

				// private variables
				private:
			};



			/*! 
			 *
			 *		\class		CDynamicReferenceVector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, cint64 bucket2Size = 5>
			class CDynamicReferenceVector : public CDynamicReferenceVectorBase<T,CDynamicReferenceVector<T,bucket2Size>,bucket2Size> {
				// public methods
				public:
					// Constructor
					inline CDynamicReferenceVector(CContext* context = nullptr) : CDynamicReferenceVectorBase<T,CDynamicReferenceVector<T,bucket2Size>,bucket2Size>(context) {};

					//! Destructor
					inline ~CDynamicReferenceVector() {};
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CDynamicReferenceVector.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CDYNAMICREFERENCEVECTOR_H
