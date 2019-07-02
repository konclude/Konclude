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

#ifndef KONCLUDE_UTILITIES_CREVERSESORT_H
#define KONCLUDE_UTILITIES_CREVERSESORT_H

// Libraries includes
#include <QPair>

// Namespace includes


// Other includes



// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Utilities {




		template<class T> class CReverseSort;


		/*! 
		*
		*		\class		CReverseSort
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		Spezialised Pointer Version to avoid comparison between addresses
		*
		*/
		template<class T>
		class CReverseSort<T*> {
			// public Functions
			public:
				//! Constructor
				CReverseSort();
				CReverseSort(T *&sortObject);
				//! Destructor
				virtual ~CReverseSort();

				T*& getData();

				bool operator==(const CReverseSort<T*> &comparisonObject);
				bool operator<=(const CReverseSort<T*> &comparisonObject);
				bool operator>=(const CReverseSort<T*> &comparisonObject);

			// private Functions
			private:
				T *object;

			// private Variables
			private:

		};


		template<class T>
		CReverseSort<T*>::CReverseSort() {
		}

		template<class T>
		CReverseSort<T*>::CReverseSort(T *&sortObject) {
			object = sortObject;
		}

		template<class T>
		T*& CReverseSort<T*>::getData() {
			return object;
		}

		template<class T>
		bool CReverseSort<T*>::operator==(const CReverseSort<T*> &comparisonObject) {
			return *object == *comparisonObject;
		}


		template<class T>
		bool CReverseSort<T*>::operator<=(const CReverseSort<T*> &comparisonObject) {
			return *object >= *comparisonObject;
		}

		template<class T>
		bool CReverseSort<T*>::operator>=(const CReverseSort<T*> &comparisonObject) {
			return *object <= *comparisonObject;
		}







		/*!
		 *
		 *		\class		CReverseSort
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CReverseSort {

			// public Functions
			public:
				//! Constructor
				CReverseSort();
				CReverseSort(T &sortObject);
				//! Destructor
				virtual ~CReverseSort();

				T& getData();


				bool operator==(const CReverseSort<T> &comparisonObject);
				bool operator<=(const CReverseSort<T> &comparisonObject);
				bool operator>=(const CReverseSort<T> &comparisonObject);

			// private Functions
			private:
				T object;

			// private Variables
			private:
		
		};


		template<class T>
		CReverseSort<T>::CReverseSort() {
		}

		template<class T>
		CReverseSort<T>::CReverseSort(T &sortObject) {
			object = sortObject;
		}


		template<class T>
		T& CReverseSort<T>::getData() {
			return object;
		}


		template<class T>
		bool CReverseSort<T>::operator==(const CReverseSort<T> &comparisonObject) {
			return object == comparisonObject;
		}


		template<class T>
		bool CReverseSort<T>::operator<=(const CReverseSort<T> &comparisonObject) {
			return object >= comparisonObject;
		}

		template<class T>
		bool CReverseSort<T>::operator>=(const CReverseSort<T> &comparisonObject) {
			return object <= comparisonObject;
		}

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CREVERSESORT_H
