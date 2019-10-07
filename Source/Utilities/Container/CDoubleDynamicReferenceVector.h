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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CDOUBLEDYNAMICREFERENCEVECTOR_H
#define KONCLUDE_UTILITIES_CONTAINER_CDOUBLEDYNAMICREFERENCEVECTOR_H

// Namespace includes
#include "ContainerSettings.h"

// Library includes

// Other includes
#include "CDynamicReferenceVector.h"

// Logger includes


namespace Konclude {

	using namespace Context;

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CDoubleDynamicReferenceVectorBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class S, cint64 bucket2Size = 5>
			class CDoubleDynamicReferenceVectorBase {
				// public methods
				public:
					//! Constructor
					inline CDoubleDynamicReferenceVectorBase(CContext* context = nullptr);

					//! Destructor
					inline ~CDoubleDynamicReferenceVectorBase();

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
					inline S* referenceVector(CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>* refVector);

					inline cint64 getItemCount();
					inline cint64 getItemMaxIndex();
					inline cint64 getItemMinIndex();

					inline cint64 getReferencedItemCount();
					inline cint64 getReferencedItemMaxIndex();
					inline cint64 getReferencedItemMinIndex();

				// protected methods
				protected:

				// protected variables
				protected:
					CDynamicReferenceVectorBase<T,S,bucket2Size> mFirstVector,mSecondVector;


				// private methods
				private:

				// private variables
				private:
			};



			/*! 
			 *
			 *		\class		CDoubleDynamicReferenceVector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, cint64 bucket2Size = 5>
			class CDoubleDynamicReferenceVector : public CDoubleDynamicReferenceVectorBase<T,CDoubleDynamicReferenceVector<T,bucket2Size>,bucket2Size> {
				// public methods
				public:
					// Constructor
					inline CDoubleDynamicReferenceVector(CContext* context = nullptr) : CDoubleDynamicReferenceVectorBase<T,CDoubleDynamicReferenceVector<T,bucket2Size>,bucket2Size>(context) {};

					//! Destructor
					inline ~CDoubleDynamicReferenceVector() {};
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CDoubleDynamicReferenceVector.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CDOUBLEDYNAMICREFERENCEVECTOR_H
