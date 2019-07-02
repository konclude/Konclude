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

#ifndef KONCLUDE_UTILITIES_CDBLNEGLINKER_H
#define KONCLUDE_UTILITIES_CDBLNEGLINKER_H

// Libraries includes


// Namespace includes
#include <QLinkedListIterator>

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Utilities {




		/*! 
		 *
		 *		\class		CDblNegLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CDblNegLinker {
			// public methods
			public:
				//! Constructor
				CDblNegLinker();
				CDblNegLinker(T dataElement, bool isNegated = false, CDblNegLinker<T> *nextLink = 0);

				//! Destructor
				virtual ~CDblNegLinker();


				CDblNegLinker<T> *init(T dataElement, bool isNegated = false, CDblNegLinker<T> *nextLink = 0);

				bool hasNext();
				bool hasPrev();

				
				CDblNegLinker<T> *insertNext(CDblNegLinker<T> *nextLink);
				CDblNegLinker<T> *getNext();

				CDblNegLinker<T> *insertPrev(CDblNegLinker<T> *prevLink);
				CDblNegLinker<T> *getPrev();

				CDblNegLinker<T> *setNext(CDblNegLinker<T> *prevLink);
				CDblNegLinker<T> *setPrev(CDblNegLinker<T> *prevLink);

				T getData();
				CDblNegLinker<T> *setData(T dataElement);

				bool isNegated();
				CDblNegLinker<T> *setNegated(bool isNegated);


				CDblNegLinker<T> *prepend(CDblNegLinker<T> *prependingList);
				CDblNegLinker<T> *append(CDblNegLinker<T> *appendingList);

				CDblNegLinker<T> *removeNext();
				CDblNegLinker<T> *removePrev();

				CDblNegLinker<T> *getLastListLink();
				CDblNegLinker<T> *getFirstListLink();


			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CDblNegLinker<T> *next;
				CDblNegLinker<T> *prev;
				T data;
				bool negated;

		};


		template<class T>
		CDblNegLinker<T>::CDblNegLinker() {
			next = 0;
			prev = 0;
			negated = false;
		}

		template<class T>
		CDblNegLinker<T>::CDblNegLinker(T dataElement, bool isNegated, CDblNegLinker<T> *nextLink) {
			next = 0;
			prev = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
		}

		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::init(T dataElement, bool isNegated, CDblNegLinker<T> *nextLink) {
			next = 0;
			prev = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
			return this;
		}


		template<class T>
		CDblNegLinker<T>::~CDblNegLinker() {
		}


		template<class T>
		bool CDblNegLinker<T>::hasNext() {
			return next != 0;
		}

		template<class T>
		bool CDblNegLinker<T>::hasPrev() {
			return prev != 0;
		}
		
		
		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::insertNext(CDblNegLinker<T> *nextLink) {
			if (nextLink) {
				CDblNegLinker<T> *tmpNext = next;
				next = nextLink;
				nextLink->prev = this;
				if (tmpNext) {
					nextLink->append(tmpNext);
				}
			}
			return this;
		}

		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::insertPrev(CDblNegLinker<T> *prevLink) {
			if (prevLink) {
				CDblNegLinker<T> *pLlast = prevLink->getLastListLink();
				CDblNegLinker<T> *tmpPrev = prev;
				prev = pLlast;
				pLlast->next = this;
				if (tmpPrev) {
					tmpPrev->next = 0;
					prevLink->prepend(tmpPrev);
				}
				return prevLink;
			} else {
				return this;
			}
		}


		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::setNext(CDblNegLinker<T> *nextLink) {
			next = nextLink;
			if (nextLink) {
				nextLink->prev = this;
			}
			return this;
		}


		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::setPrev(CDblNegLinker<T> *prevLink) {
			if (prevLink) {
				CDblNegLinker<T> *pLlast = prevLink->getLastListLink();
				pLlast->next = this;
				prev = pLlast;
				return prevLink;
			} else {
				prev = 0;
				return this;
			}
		}

		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::getNext() {
			return next;
		}

		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::getPrev() {
			return prev;
		}


		template<class T>
		T CDblNegLinker<T>::getData() {
			return data;
		}



		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::setData(T dataElement) {
			data = dataElment;
			return this;
		}

		template<class T>
		bool CDblNegLinker<T>::isNegated() {
			return negated;
		}

		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::setNegated(bool isNegated) {
			negated = isNegated;
			return this;
		}


		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::prepend(CDblNegLinker<T> *prependingList) {
			CDblNegLinker<T> *last = prependingList->getLastListLink();
			last->insertNext(this);
			return prependingList;
		}

		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::append(CDblNegLinker<T> *appendingList) {
			CDblNegLinker<T> *last = getLastListLink();
			last->insertNext(appendingList);
			return this;
		}

		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::removeNext() {
			CDblNegLinker<T> *next = getNext();
			if (next) {
				next->next = 0;
				next->prev = 0;
				setNext(next->getNext());
			}
			return next;
		}

		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::removePrev() {
			CDblNegLinker<T> *prev = getPrev();
			if (prev) {
				prev->next = 0;
				prev->prev = 0;
				setPrev(prev->getPrev());
			}
			return prev;
		}


		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::getLastListLink() {
			CDblNegLinker<T> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}

		template<class T>
		CDblNegLinker<T> *CDblNegLinker<T>::getFirstListLink() {
			CDblNegLinker<T> *firstListLink = this;
			while (firstListLink->hasPrev()) {
				firstListLink = firstListLink->getPrev();
			}
			return firstListLink;
		}



	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CDBLNEGLINKER_H
