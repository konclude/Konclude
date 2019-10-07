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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CDEFAULTDYNAMICREFERENCEVECTOR_H
#define KONCLUDE_UTILITIES_CONTAINER_CDEFAULTDYNAMICREFERENCEVECTOR_H

// Namespace includes
#include "ContainerSettings.h"

// Library includes

// Other includes
#include "CDynamicReferenceVector.h"

// Logger includes


namespace Konclude {

	namespace Utilities {

		namespace Container {


			/*! 
			 *
			 *		\class		CDefaultDynamicReferenceVectorBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class S, cint64 bucket2Size = 5>
			class CDefaultDynamicReferenceVectorBase : public CDynamicReferenceVectorBase<T,S,bucket2Size> {
				// public methods
				public:
					//! Constructor
					inline CDefaultDynamicReferenceVectorBase(CContext* context = nullptr);

					//! Destructor
					inline ~CDefaultDynamicReferenceVectorBase();

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
			 *		\class		CDefaultDynamicReferenceVector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, cint64 bucket2Size = 5>
			class CDefaultDynamicReferenceVector : public CDefaultDynamicReferenceVectorBase<T,CDefaultDynamicReferenceVector<T,bucket2Size>,bucket2Size> {
				// public methods
				public:
					// Constructor
					inline CDefaultDynamicReferenceVector(CContext* context = nullptr) : CDefaultDynamicReferenceVectorBase<T,CDefaultDynamicReferenceVector<T,bucket2Size>,bucket2Size>(context) {};

					//! Destructor
					inline ~CDefaultDynamicReferenceVector() {};
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CDefaultDynamicReferenceVector.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CDEFAULTDYNAMICREFERENCEVECTOR_H
