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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CNEGLINKER_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CNEGLINKER_CPP


#include "CNegLinker.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T, class S>
			CNegLinkerBase<T,S>::CNegLinkerBase() : CLinkerBase<T,S>() {
			}

			template<class T, class S>
			CNegLinkerBase<T,S>::CNegLinkerBase(const T& dataElement, bool negation, CNegLinkerBase<T,S>* nextLink) : CLinkerBase<T,S>(dataElement,nextLink),CNegator(negation) {				
			}

			template<class T, class S>
			CNegLinkerBase<T,S>::~CNegLinkerBase() {
			}

			template<class T, class S>
			S* CNegLinkerBase<T,S>::initNegLinker(const T& dataElement, bool negation, CNegLinkerBase<T,S>* nextLink) {
				CLinkerBase<T,S>::initLinker(dataElement,nextLink);
				CNegator::initNeg(negation);
				return (S*)this;
			}



		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CNEGLINKER_CPP
