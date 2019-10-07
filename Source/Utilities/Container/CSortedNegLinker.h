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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CSORTEDNEGLINKER_H
#define KONCLUDE_UTILITIES_CONTAINER_CSORTEDNEGLINKER_H

// Namespace includes
#include "ContainerSettings.h"
#include "CSortedLinker.h"
#include "CNegLinker.h"

// Library includes

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T> class CNegLinkerOperatorCompareFunctor;

			template<class T>
			class CNegLinkerOperatorCompareFunctor<T*> {
			public:
				static inline bool sortedNegLinkerDataCompare(T* const & beforeData1, const bool& negation1, T* const & data2, const bool& negation2) {
					return *beforeData1 <= *data2;
				}
			};

			template<class T>
			class CNegLinkerOperatorCompareFunctor {
			public:
				static inline bool sortedNegLinkerDataCompare(const T& beforeData1, const bool& negation1, const T& data2, const bool& negation2) {
					return beforeData1 <= data2;
				}
			};


			/*! 
			 *
			 *		\class		CSortedNegLinkerBase
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class S, class C = CNegLinkerOperatorCompareFunctor<T> >
			class CSortedNegLinkerBase : public CNegLinkerBase<T,S> {
				// public methods
				public:
					// Constructor
					inline CSortedNegLinkerBase();
					inline CSortedNegLinkerBase(const T& dataElement, bool negation = false, CSortedNegLinkerBase<T,S,C>* nextLink = nullptr);

					//! Destructor
					inline ~CSortedNegLinkerBase();


					S* insertSortedNextSorted(CSortedNegLinkerBase<T,S,C>* nextLink);
					S* insertNextSorted(CSortedNegLinkerBase<T,S,C>* nextLink);


				// protected methods
				protected:
					bool isSortedBeforeCompare(const T& checkData, const bool& checkNegation, const T& linkedData, const bool& linkedNegation) const;

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:
			};


			/*! 
			*
			*		\class		CSortedNegLinker
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			template<class T, class C = CNegLinkerOperatorCompareFunctor<T> >
			class CXSortedNegLinker : public CSortedNegLinkerBase<T,CXSortedNegLinker<T>,C> {
				// public methods
				public:
					// Constructor
					inline CXSortedNegLinker() : CSortedNegLinkerBase<T,CXSortedNegLinker<T>,C>() {};
					inline CXSortedNegLinker(const T& dataElement, bool negation = false, CXSortedNegLinker<T,C>* nextLink = nullptr) : CSortedNegLinkerBase<T,CXSortedNegLinker<T>,C>(dataElement,negation,nextLink) {};

					//! Destructor
					inline ~CXSortedNegLinker() {};
			};


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CSortedNegLinker.cpp"

#endif // KONCLUDE_UTILITIES_CONTAINER_CSORTEDNEGLINKER_H
