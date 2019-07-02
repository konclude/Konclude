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

#ifndef KONCLUDE_UTILITIES_CNEGLINKER_H
#define KONCLUDE_UTILITIES_CNEGLINKER_H

// Libraries includes


// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Utilities {


		/*! 
		 *
		 *		\class		CNegLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CNegLinker {
			// public methods
			public:
				//! Constructor
				CNegLinker();
				CNegLinker(T dataElement, bool isNegated = false, CNegLinker<T> *nextLink = 0);

				//! Destructor
				virtual ~CNegLinker();


				CNegLinker<T> *init(T dataElement, bool isNegated = false, CNegLinker<T> *nextLink = 0);

				bool hasNext();
				
				CNegLinker<T> *setNext(CNegLinker<T> *nextLink);
				CNegLinker<T> *getNext();

				T getData();
				CNegLinker<T> *setData(T dataElement);

				bool isNegated();
				CNegLinker<T> *setNegated(bool isNegated);

				CNegLinker<T> *prepend(CNegLinker<T> *prependingList);
				CNegLinker<T> *append(CNegLinker<T> *appendingList);

				CNegLinker<T> *removeNext();

				CNegLinker<T> *getLastListLink();


			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CNegLinker<T> *next;
				T data;
				bool negated;

		};


		template<class T>
		CNegLinker<T>::CNegLinker() {
			next = 0;
			negated = false;
		}

		template<class T>
		CNegLinker<T>::CNegLinker(T dataElement, bool isNegated, CNegLinker<T> *nextLink) {
			next = nextLink;
			negated = isNegated;
			data = dataElement;
		}

		template<class T>
		CNegLinker<T> *CNegLinker<T>::init(T dataElement, bool isNegated, CNegLinker<T> *nextLink) {
			next = nextLink;
			negated = isNegated;
			data = dataElement;
		}


		template<class T>
		CNegLinker<T>::~CNegLinker() {
		}


		template<class T>
		bool CNegLinker<T>::hasNext() {
			return next != 0;
		}

		
		template<class T>
		CNegLinker<T> *CNegLinker<T>::setNext(CNegLinker<T> *nextLink) {
			next = nextLink;
			return this;
		}

		template<class T>
		CNegLinker<T> *CNegLinker<T>::getNext() {
			return next;
		}


		template<class T>
		T CNegLinker<T>::getData() {
			return data;
		}

		template<class T>
		CNegLinker<T> *CNegLinker<T>::setData(T dataElement) {
			data = dataElement;
			return this;
		}


		template<class T>
		bool CNegLinker<T>::isNegated() {
			return negated;
		}

		template<class T>
		CNegLinker<T> *CNegLinker<T>::setNegated(bool isNegated) {
			negated = isNegated;
			return this;
		}


		template<class T>
		CNegLinker<T> *CNegLinker<T>::prepend(CNegLinker<T> *prependingList) {
			CNegLinker<T> *last = prependingList->getLastListLink();
			last->setNext(this);
			return prependingList;
		}

		template<class T>
		CNegLinker<T> *CNegLinker<T>::append(CNegLinker<T> *appendingList) {
			CNegLinker<T> *last = getLastListLink();
			last->setNext(appendingList);
			return this;
		}

		template<class T>
		CNegLinker<T> *CNegLinker<T>::removeNext() {
			CNegLinker<T> *tmpNext = next;
			if (next) {
				CNegLinker<T> *tmpNextNext = next->next;
				next->next = 0;
				setNext(tmpNextNext);
			}
			return tmpNext;
		}

		template<class T>
		CNegLinker<T> *CNegLinker<T>::getLastListLink() {
			CNegLinker<T> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}



	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CNEGLINKER_H
