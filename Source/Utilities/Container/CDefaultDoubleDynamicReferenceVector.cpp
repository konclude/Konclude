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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CDEFAULTDOUBLEDYNAMICREFERENCEVECTOR_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CDEFAULTDOUBLEDYNAMICREFERENCEVECTOR_CPP


#include "CDefaultDoubleDynamicReferenceVector.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T, class S, cint64 bucket2Size>
			CDefaultDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::CDefaultDoubleDynamicReferenceVectorBase(CContext* context) : CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>(context) {
				CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::clear();
			}


			template<class T, class S, cint64 bucket2Size>
			CDefaultDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::~CDefaultDoubleDynamicReferenceVectorBase() {
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDefaultDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getData(cint64 index) {
				return CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getData(index,nullptr);
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDefaultDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getData(cint64 index, bool* localFlag) {
				return CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getData(index,nullptr,localFlag);
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDefaultDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getLocalData(cint64 index) {
				return CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getLocalData(index,nullptr);
			}

			template<class T, class S, cint64 bucket2Size>
			const T CDefaultDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getReferenceData(cint64 index) {
				return CDoubleDynamicReferenceVectorBase<T,S,bucket2Size>::getReferenceData(index,nullptr);
			}



		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CDEFAULTDOUBLEDYNAMICREFERENCEVECTOR_CPP
