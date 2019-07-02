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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CSORTEDLINKER_H
#define KONCLUDE_UTILITIES_CONTAINER_CSORTEDLINKER_H

// Namespace includes
#include "ContainerSettings.h"
#include "CLinker.h"

// Library includes

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T> class CLinkerOperatorCompareFunctor;

			template<class T>
			class CLinkerOperatorCompareFunctor<T*> {
				public:
					static inline bool sortedLinkerDataCompare(T* const & beforeData1, T* const & data2) {
						return *beforeData1 <= *data2;
					}
			};

			template<class T>
			class CLinkerOperatorCompareFunctor {
				public:
					static inline bool sortedLinkerDataCompare(const T& beforeData1, const T& data2) {
						return beforeData1 <= data2;
					}
			};


			/*! 
			 *
			 *		\class		CSortedLinkerBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class S, class C = CLinkerOperatorCompareFunctor<T> >
			class CSortedLinkerBase : public CLinkerBase<T,S> {
				// public methods
				public:
					// Constructor
					inline CSortedLinkerBase();
					inline CSortedLinkerBase(const T& dataElement, CSortedLinkerBase<T,S,C>* nextLink = nullptr);

					//! Destructor
					inline ~CSortedLinkerBase();


					S* insertSortedNextSorted(CSortedLinkerBase<T,S,C>* nextLink);
					S* insertNextSorted(CSortedLinkerBase<T,S,C>* nextLink);


				// protected methods
				protected:
					bool isSortedBeforeCompare(const T& checkData, const T& linkedData) const;

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:
			};


			/*! 
			*
			*		\class		CSortedLinker
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			template<class T, class C = CLinkerOperatorCompareFunctor<T> >
			class CXSortedLinker : public CSortedLinkerBase<T,CXSortedLinker<T>,C> {
				// public methods
				public:
					// Constructor
					inline CXSortedLinker() : CSortedLinkerBase<T,CXSortedLinker<T>,C>() {};
					inline CXSortedLinker(const T& dataElement, CXSortedLinker<T,C>* nextLink = nullptr) : CSortedLinkerBase<T,CXSortedLinker<T>,C>(dataElement,nextLink) {};

					//! Destructor
					inline ~CXSortedLinker() {};
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CSortedLinker.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CSORTEDLINKER_H
