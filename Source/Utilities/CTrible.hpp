/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_UTILITIES_CTRIBLE_H
#define KONCLUDE_UTILITIES_CTRIBLE_H

// Libraries includes
#include <QPair>

// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Utilities {

		/*!
		 *
		 *		\class		CTrible
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CTrible : public QPair<QPair<T,T>,T> {

			// public Functions
			public:
				//! Constructor
				CTrible(const T &first, const T &second, const T &third);

				T getFirst();
				T getSecond();
				T getThird();

				CTrible<T> *setFirst(const T &first);
				CTrible<T> *setSecond(const T &second);
				CTrible<T> *setThird(const T &third);

				CTrible<T> *set(const T &first, const T &second, const T &third);

			// private Functions
			private:

			// private Variables
			private:
		
		};


		template<class T>
		CTrible<T>::CTrible(const T &first, const T &second, const T &third) {
			this->first.first = first;
			this->first.second = second;
			this->second = third;
		}

		template<class T>
		CTrible<T> *CTrible<T>::setFirst(const T &first) {
			this->first.first = first;
		}

		template<class T>
		CTrible<T> *CTrible<T>::setSecond(const T &second) {
			this->first.second = second;
		}

		template<class T>
		CTrible<T> *CTrible<T>::setThird(const T &third) {
			this->second = third;
		}

		template<class T>
		CTrible<T> *CTrible<T>::set(const T &first, const T &second, const T &third) {
			this->first.first = first;
			this->first.second = second;
			this->second = third;
			return this;
		}


		template<class T>
		T CTrible<T>::getFirst() {
			return this->first.first;
		}

		template<class T>
		T CTrible<T>::getSecond() {
			return this->first.second;
		}

		template<class T>
		T CTrible<T>::getThird() {
			return this->second;
		}


	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CTRIBLE_H
