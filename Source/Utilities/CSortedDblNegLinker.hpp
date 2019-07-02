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

#ifndef KONCLUDE_UTILITIES_CSORTEDDBLNEGLINKER_H
#define KONCLUDE_UTILITIES_CSORTEDDBLNEGLINKER_H

// Libraries includes


// Namespace includes
#include <QLinkedListIterator>

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Utilities {






		template<class T> class CSortedDblNegLinker;




		/*! 
		 *
		 *		\class		CSortedDblNegLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Spezialised Pointer Version to avoid comparison between addresses
		 *
		 */
		template<class T>
		class CSortedDblNegLinker<T*> {
			// public methods
			public:
				//! Constructor
				CSortedDblNegLinker();
				CSortedDblNegLinker(T* dataElement, bool isNegated = false, CSortedDblNegLinker<T*> *nextLink = 0);

				//! Destructor
				virtual ~CSortedDblNegLinker();


				CSortedDblNegLinker<T*> *init(T* dataElement, bool isNegated = false, CSortedDblNegLinker<T*> *nextLink = 0);

				bool hasNext();
				bool hasPrev();

				CSortedDblNegLinker<T*> *insertSortedNextSorted(CSortedDblNegLinker<T*> *nextLink);
				CSortedDblNegLinker<T*> *insertNextSorted(CSortedDblNegLinker<T*> *nextLink);

				
				CSortedDblNegLinker<T*> *insertNext(CSortedDblNegLinker<T*> *nextLink);
				CSortedDblNegLinker<T*> *getNext();

				CSortedDblNegLinker<T*> *insertPrev(CSortedDblNegLinker<T*> *prevLink);
				CSortedDblNegLinker<T*> *getPrev();

				CSortedDblNegLinker<T*> *setNext(CSortedDblNegLinker<T*> *prevLink);
				CSortedDblNegLinker<T*> *setPrev(CSortedDblNegLinker<T*> *prevLink);

				T* getData();
				CSortedDblNegLinker<T*> *setData(T* dataElement);

				bool isNegated();
				CSortedDblNegLinker<T*> *setNegated(bool isNegated);


				CSortedDblNegLinker<T*> *prepend(CSortedDblNegLinker<T*> *prependingList);
				CSortedDblNegLinker<T*> *append(CSortedDblNegLinker<T*> *appendingList);

				CSortedDblNegLinker<T*> *removeNext();
				CSortedDblNegLinker<T*> *removePrev();

				CSortedDblNegLinker<T*> *getLastListLink();
				CSortedDblNegLinker<T*> *getFirstListLink();


			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CSortedDblNegLinker<T*> *next;
				CSortedDblNegLinker<T*> *prev;
				T* data;
				bool negated;

		};


		template<class T>
		CSortedDblNegLinker<T*>::CSortedDblNegLinker() {
			next = 0;
			prev = 0;
			negated = false;
		}

		template<class T>
		CSortedDblNegLinker<T*>::CSortedDblNegLinker(T* dataElement, bool isNegated, CSortedDblNegLinker<T*> *nextLink) {
			next = 0;
			prev = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
		}

		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::init(T* dataElement, bool isNegated, CSortedDblNegLinker<T*> *nextLink) {
			next = 0;
			prev = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
			return this;
		}


		template<class T>
		CSortedDblNegLinker<T*>::~CSortedDblNegLinker() {
		}


		template<class T>
		bool CSortedDblNegLinker<T*>::hasNext() {
			return next != 0;
		}

		template<class T>
		bool CSortedDblNegLinker<T*>::hasPrev() {
			return prev != 0;
		}
		
		
		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::insertNext(CSortedDblNegLinker<T*> *nextLink) {
			if (nextLink) {
				CSortedDblNegLinker<T*> *tmpNext = next;
				next = nextLink;
				nextLink->prev = this;
				if (tmpNext) {
					nextLink->append(tmpNext);
				}
			}
			return this;
		}


		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::insertNextSorted(CSortedDblNegLinker<T*> *nextLink) {
			while (nextLink) {

				CSortedDblNegLinker<T*> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
					nextLink->removePrev();
				} else {
					nextLink = 0;
				}

				CSortedDblNegLinker<T*> *list = this;
				CSortedDblNegLinker<T*> *lastList = list;
				bool inserted = false;
				T* dataIns = nIns->getData();
				while (list) {
					T* dataList = list->getData();
					if (*dataIns <= *dataList) {
						list->insertPrev(nIns);
						inserted = true;
						break;
					}
					lastList = list;
					list = list->next;
				}
				if (!inserted) {
					lastList->insertNext(nIns);
				}
			}
			return getFirstListLink();
		}



		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::insertSortedNextSorted(CSortedDblNegLinker<T*> *nextLink) {
			// items from nextLink are already sorted!
			CSortedDblNegLinker<T*> *list = this;

			while (nextLink) {
				CSortedDblNegLinker<T*> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
					nextLink->removePrev();
				} else {
					nextLink = 0;
				}

				CSortedDblNegLinker<T*> *lastList = list;
				bool inserted = false;
				T* dataIns = nIns->getData();
				while (list) {
					T* dataList = list->getData();
					if (*dataIns <= *dataList) {
						list = list->insertPrev(nIns);
						inserted = true;
						break;
					}
					lastList = list;
					list = list->next;
				}
				if (!inserted) {
					lastList->insertNext(nIns);
					list = lastList;
				}
			}
			return getFirstListLink();
		}



		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::insertPrev(CSortedDblNegLinker<T*> *prevLink) {
			if (prevLink) {
				CSortedDblNegLinker<T*> *pLlast = prevLink->getLastListLink();
				CSortedDblNegLinker<T*> *tmpPrev = prev;
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
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::setNext(CSortedDblNegLinker<T*> *nextLink) {
			next = nextLink;
			if (nextLink) {
				nextLink->prev = this;
			}
			return this;
		}


		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::setPrev(CSortedDblNegLinker<T*> *prevLink) {
			if (prevLink) {
				CSortedDblNegLinker<T*> *pLlast = prevLink->getLastListLink();
				pLlast->next = this;
				prev = pLlast;
				return prevLink;
			} else {
				prev = 0;
				return this;
			}
		}

		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::getNext() {
			return next;
		}

		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::getPrev() {
			return prev;
		}


		template<class T>
		T* CSortedDblNegLinker<T*>::getData() {
			return data;
		}



		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::setData(T* dataElement) {
			data = dataElement;
			return this;
		}

		template<class T>
		bool CSortedDblNegLinker<T*>::isNegated() {
			return negated;
		}

		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::setNegated(bool isNegated) {
			negated = isNegated;
			return this;
		}


		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::prepend(CSortedDblNegLinker<T*> *prependingList) {
			CSortedDblNegLinker<T*> *last = prependingList->getLastListLink();
			last->insertNext(this);
			return prependingList;
		}

		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::append(CSortedDblNegLinker<T*> *appendingList) {
			CSortedDblNegLinker<T*> *last = getLastListLink();
			last->insertNext(appendingList);
			return this;
		}

		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::removeNext() {
			CSortedDblNegLinker<T*> *next = getNext();
			if (next) {
				next->next = 0;
				next->prev = 0;
				setNext(next->getNext());
			}
			return next;
		}

		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::removePrev() {
			CSortedDblNegLinker<T*> *prev = getPrev();
			if (prev) {
				prev->next = 0;
				prev->prev = 0;
				setPrev(prev->getPrev());
			}
			return prev;
		}


		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::getLastListLink() {
			CSortedDblNegLinker<T*> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}

		template<class T>
		CSortedDblNegLinker<T*> *CSortedDblNegLinker<T*>::getFirstListLink() {
			CSortedDblNegLinker<T*> *firstListLink = this;
			while (firstListLink->hasPrev()) {
				firstListLink = firstListLink->getPrev();
			}
			return firstListLink;
		}





		/*! 
		 *
		 *		\class		CSortedDblNegLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CSortedDblNegLinker {
			// public methods
			public:
				//! Constructor
				CSortedDblNegLinker();
				CSortedDblNegLinker(T dataElement, bool isNegated = false, CSortedDblNegLinker<T> *nextLink = 0);

				//! Destructor
				virtual ~CSortedDblNegLinker();


				CSortedDblNegLinker<T> *init(T dataElement, bool isNegated = false, CSortedDblNegLinker<T> *nextLink = 0);

				bool hasNext();
				bool hasPrev();

				CSortedDblNegLinker<T> *insertSortedNextSorted(CSortedDblNegLinker<T> *nextLink);
				CSortedDblNegLinker<T> *insertNextSorted(CSortedDblNegLinker<T> *nextLink);

				
				CSortedDblNegLinker<T> *insertNext(CSortedDblNegLinker<T> *nextLink);
				CSortedDblNegLinker<T> *getNext();

				CSortedDblNegLinker<T> *insertPrev(CSortedDblNegLinker<T> *prevLink);
				CSortedDblNegLinker<T> *getPrev();

				CSortedDblNegLinker<T> *setNext(CSortedDblNegLinker<T> *prevLink);
				CSortedDblNegLinker<T> *setPrev(CSortedDblNegLinker<T> *prevLink);

				T getData();
				CSortedDblNegLinker<T> *setData(T dataElement);

				bool isNegated();
				CSortedDblNegLinker<T> *setNegated(bool isNegated);


				CSortedDblNegLinker<T> *prepend(CSortedDblNegLinker<T> *prependingList);
				CSortedDblNegLinker<T> *append(CSortedDblNegLinker<T> *appendingList);

				CSortedDblNegLinker<T> *removeNext();
				CSortedDblNegLinker<T> *removePrev();

				CSortedDblNegLinker<T> *getLastListLink();
				CSortedDblNegLinker<T> *getFirstListLink();


			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CSortedDblNegLinker<T> *next;
				CSortedDblNegLinker<T> *prev;
				T data;
				bool negated;

		};


		template<class T>
		CSortedDblNegLinker<T>::CSortedDblNegLinker() {
			next = 0;
			prev = 0;
			negated = false;
		}

		template<class T>
		CSortedDblNegLinker<T>::CSortedDblNegLinker(T dataElement, bool isNegated, CSortedDblNegLinker<T> *nextLink) {
			next = 0;
			prev = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
		}

		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::init(T dataElement, bool isNegated, CSortedDblNegLinker<T> *nextLink) {
			next = 0;
			prev = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
			return this;
		}


		template<class T>
		CSortedDblNegLinker<T>::~CSortedDblNegLinker() {
		}


		template<class T>
		bool CSortedDblNegLinker<T>::hasNext() {
			return next != 0;
		}

		template<class T>
		bool CSortedDblNegLinker<T>::hasPrev() {
			return prev != 0;
		}
		
		
		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::insertNext(CSortedDblNegLinker<T> *nextLink) {
			if (nextLink) {
				CSortedDblNegLinker<T> *tmpNext = next;
				next = nextLink;
				nextLink->prev = this;
				if (tmpNext) {
					nextLink->append(tmpNext);
				}
			}
			return this;
		}


		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::insertNextSorted(CSortedDblNegLinker<T> *nextLink) {
			while (nextLink) {

				CSortedDblNegLinker<T> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
					nextLink->removePrev();
				} else {
					nextLink = 0;
				}

				CSortedDblNegLinker<T> *list = this;
				CSortedDblNegLinker<T> *lastList = list;
				bool inserted = false;
				T dataIns = nIns->getData();
				while (list) {
					T dataList = list->getData();
					if (dataIns <= dataList) {
						list->insertPrev(nIns);
						inserted = true;
						break;
					}
					lastList = list;
					list = list->next;
				}
				if (!inserted) {
					lastList->insertNext(nIns);
				}
			}
			return getFirstListLink();
		}



		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::insertSortedNextSorted(CSortedDblNegLinker<T> *nextLink) {
			// items from nextLink are already sorted!
			CSortedDblNegLinker<T> *list = this;

			while (nextLink) {
				CSortedDblNegLinker<T> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
					nextLink->removePrev();
				} else {
					nextLink = 0;
				}

				CSortedDblNegLinker<T> *lastList = list;
				bool inserted = false;
				T dataIns = nIns->getData();
				while (list) {
					T dataList = list->getData();
					if (dataIns <= dataList) {
						list = list->insertPrev(nIns);
						inserted = true;
						break;
					}
					lastList = list;
					list = list->next;
				}
				if (!inserted) {
					lastList->insertNext(nIns);
					list = lastList;
				}
			}
			return getFirstListLink();
		}



		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::insertPrev(CSortedDblNegLinker<T> *prevLink) {
			if (prevLink) {
				CSortedDblNegLinker<T> *pLlast = prevLink->getLastListLink();
				CSortedDblNegLinker<T> *tmpPrev = prev;
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
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::setNext(CSortedDblNegLinker<T> *nextLink) {
			next = nextLink;
			if (nextLink) {
				nextLink->prev = this;
			}
			return this;
		}


		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::setPrev(CSortedDblNegLinker<T> *prevLink) {
			if (prevLink) {
				CSortedDblNegLinker<T> *pLlast = prevLink->getLastListLink();
				pLlast->next = this;
				prev = pLlast;
				return prevLink;
			} else {
				prev = 0;
				return this;
			}
		}

		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::getNext() {
			return next;
		}

		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::getPrev() {
			return prev;
		}


		template<class T>
		T CSortedDblNegLinker<T>::getData() {
			return data;
		}



		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::setData(T dataElement) {
			data = dataElement;
			return this;
		}

		template<class T>
		bool CSortedDblNegLinker<T>::isNegated() {
			return negated;
		}

		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::setNegated(bool isNegated) {
			negated = isNegated;
			return this;
		}


		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::prepend(CSortedDblNegLinker<T> *prependingList) {
			CSortedDblNegLinker<T> *last = prependingList->getLastListLink();
			last->insertNext(this);
			return prependingList;
		}

		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::append(CSortedDblNegLinker<T> *appendingList) {
			CSortedDblNegLinker<T> *last = getLastListLink();
			last->insertNext(appendingList);
			return this;
		}

		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::removeNext() {
			CSortedDblNegLinker<T> *next = getNext();
			if (next) {
				next->next = 0;
				next->prev = 0;
				setNext(next->getNext());
			}
			return next;
		}

		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::removePrev() {
			CSortedDblNegLinker<T> *prev = getPrev();
			if (prev) {
				prev->next = 0;
				prev->prev = 0;
				setPrev(prev->getPrev());
			}
			return prev;
		}


		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::getLastListLink() {
			CSortedDblNegLinker<T> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}

		template<class T>
		CSortedDblNegLinker<T> *CSortedDblNegLinker<T>::getFirstListLink() {
			CSortedDblNegLinker<T> *firstListLink = this;
			while (firstListLink->hasPrev()) {
				firstListLink = firstListLink->getPrev();
			}
			return firstListLink;
		}



	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CSORTEDDBLNEGLINKER_H
