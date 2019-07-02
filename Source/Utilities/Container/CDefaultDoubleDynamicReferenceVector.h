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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CDEFAULTDOUBLEDYNAMICREFERENCEVECTOR_H
#define KONCLUDE_UTILITIES_CONTAINER_CDEFAULTDOUBLEDYNAMICREFERENCEVECTOR_H

// Namespace includes
#include "ContainerSettings.h"

// Library includes

// Other includes
#include "CDoubleDynamicReferenceVector.h"

// Logger includes


namespace Konclude {

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CDefaultDoubleDynamicReferenceVectorBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class S, cint64 bucket2Size = 5>
			class CDefaultDoubleDynamicReferenceVectorBase : public CDoubleDynamicReferenceVectorBase<T,S,bucket2Size> {
				// public methods
				public:
					//! Constructor
					inline CDefaultDoubleDynamicReferenceVectorBase(CContext* context = nullptr);

					//! Destructor
					inline ~CDefaultDoubleDynamicReferenceVectorBase();

					inline const T getData(cint64 index);
					inline const T getData(cint64 index, bool* localFlag);
					inline const T getLocalData(cint64 index);
					inline const T getReferenceData(cint64 index);



				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:
			};



			/*! 
			 *
			 *		\class		CDefaultDoubleDynamicReferenceVector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, cint64 bucket2Size = 5>
			class CDefaultDoubleDynamicReferenceVector : public CDefaultDoubleDynamicReferenceVectorBase<T,CDefaultDoubleDynamicReferenceVector<T,bucket2Size>,bucket2Size> {
				// public methods
				public:
					// Constructor
					inline CDefaultDoubleDynamicReferenceVector(CContext* context = nullptr) : CDefaultDoubleDynamicReferenceVectorBase<T,CDefaultDoubleDynamicReferenceVector<T,bucket2Size>,bucket2Size>(context) {};

					//! Destructor
					inline ~CDefaultDoubleDynamicReferenceVector() {};
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CDefaultDoubleDynamicReferenceVector.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CDEFAULTDOUBLEDYNAMICREFERENCEVECTOR_H
