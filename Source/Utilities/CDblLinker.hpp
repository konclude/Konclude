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

#ifndef KONCLUDE_UTILITIES_CDBLLINKER_H
#define KONCLUDE_UTILITIES_CDBLLINKER_H

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
		 *		\class		CDblLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CDblLinker {
			// public methods
			public:
				//! Constructor
				CDblLinker();
				CDblLinker(T dataElement, CDblLinker<T> *nextLink = 0);

				//! Destructor
				virtual ~CDblLinker();


				CDblLinker<T> *init(T dataElement, CDblLinker<T> *nextLink = 0);

				bool hasNext();
				bool hasPrev();

				
				CDblLinker<T> *insertNext(CDblLinker<T> *nextLink);
				CDblLinker<T> *getNext();

				CDblLinker<T> *insertPrev(CDblLinker<T> *prevLink);
				CDblLinker<T> *getPrev();

				CDblLinker<T> *setNext(CDblLinker<T> *prevLink);
				CDblLinker<T> *setPrev(CDblLinker<T> *prevLink);

				T getData();
				CDblLinker<T> *setData(T dataElement);


				CDblLinker<T> *prepend(CDblLinker<T> *prependingList);
				CDblLinker<T> *append(CDblLinker<T> *appendingList);

				CDblLinker<T> *removeNext();
				CDblLinker<T> *removePrev();

				CDblLinker<T> *getLastListLink();
				CDblLinker<T> *getFirstListLink();


			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CDblLinker<T> *next;
				CDblLinker<T> *prev;
				T data;

		};


		template<class T>
		CDblLinker<T>::CDblLinker() {
			next = 0;
			prev = 0;
		}

		template<class T>
		CDblLinker<T>::CDblLinker(T dataElement, CDblLinker<T> *nextLink) {
			next = 0;
			prev = 0;
			setNext(nextLink);
			data = dataElement;
		}

		template<class T>
		CDblLinker<T> *CDblLinker<T>::init(T dataElement, CDblLinker<T> *nextLink) {
			next = 0;
			prev = 0;
			setNext(nextLink);
			data = dataElement;
			return this;
		}


		template<class T>
		CDblLinker<T>::~CDblLinker() {
		}


		template<class T>
		bool CDblLinker<T>::hasNext() {
			return next != 0;
		}

		template<class T>
		bool CDblLinker<T>::hasPrev() {
			return prev != 0;
		}
		
		
		template<class T>
		CDblLinker<T> *CDblLinker<T>::insertNext(CDblLinker<T> *nextLink) {
			if (nextLink) {
				CDblLinker<T> *tmpNext = next;
				next = nextLink;
				nextLink->prev = this;
				if (tmpNext) {
					nextLink->append(tmpNext);
				}
			}
			return this;
		}

		template<class T>
		CDblLinker<T> *CDblLinker<T>::insertPrev(CDblLinker<T> *prevLink) {
			if (prevLink) {
				CDblLinker<T> *pLlast = prevLink->getLastListLink();
				CDblLinker<T> *tmpPrev = prev;
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
		CDblLinker<T> *CDblLinker<T>::setNext(CDblLinker<T> *nextLink) {
			next = nextLink;
			if (nextLink) {
				nextLink->prev = this;
			}
			return this;
		}


		template<class T>
		CDblLinker<T> *CDblLinker<T>::setPrev(CDblLinker<T> *prevLink) {
			if (prevLink) {
				CDblLinker<T> *pLlast = prevLink->getLastListLink();
				pLlast->next = this;
				prev = pLlast;
				return prevLink;
			} else {
				prev = 0;
				return this;
			}
		}

		template<class T>
		CDblLinker<T> *CDblLinker<T>::getNext() {
			return next;
		}

		template<class T>
		CDblLinker<T> *CDblLinker<T>::getPrev() {
			return prev;
		}


		template<class T>
		T CDblLinker<T>::getData() {
			return data;
		}



		template<class T>
		CDblLinker<T> *CDblLinker<T>::setData(T dataElement) {
			data = dataElement;
			return this;
		}


		template<class T>
		CDblLinker<T> *CDblLinker<T>::prepend(CDblLinker<T> *prependingList) {
			CDblLinker<T> *last = prependingList->getLastListLink();
			last->insertNext(this);
			return prependingList;
		}

		template<class T>
		CDblLinker<T> *CDblLinker<T>::append(CDblLinker<T> *appendingList) {
			CDblLinker<T> *last = getLastListLink();
			last->insertNext(appendingList);
			return this;
		}

		template<class T>
		CDblLinker<T> *CDblLinker<T>::removeNext() {
			CDblLinker<T> *next = getNext();
			if (next) {
				next->next = 0;
				next->prev = 0;
				setNext(next->getNext());
			}
			return next;
		}

		template<class T>
		CDblLinker<T> *CDblLinker<T>::removePrev() {
			CDblLinker<T> *prev = getPrev();
			if (prev) {
				prev->next = 0;
				prev->prev = 0;
				setPrev(prev->getPrev());
			}
			return prev;
		}


		template<class T>
		CDblLinker<T> *CDblLinker<T>::getLastListLink() {
			CDblLinker<T> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}

		template<class T>
		CDblLinker<T> *CDblLinker<T>::getFirstListLink() {
			CDblLinker<T> *firstListLink = this;
			while (firstListLink->hasPrev()) {
				firstListLink = firstListLink->getPrev();
			}
			return firstListLink;
		}



	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CDBLLINKER_H
