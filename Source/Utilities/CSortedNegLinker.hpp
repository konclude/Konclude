/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_UTILITIES_CSortedNegLinker_H
#define KONCLUDE_UTILITIES_CSortedNegLinker_H

// Libraries includes


// Namespace includes
#include <QLinkedListIterator>
#include "CAllocationObject.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Utilities {


		template<class T> class CSortedNegLinker;



		/*! 
		 *
		 *		\class		CSortedNegLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Spezialised Pointer Version to avoid comparison between addresses
		 *
		 */
		template<class T>
		class CSortedNegLinker<T*> : public CAllocationObject {
			// public methods
			public:
				//! Constructor
				CSortedNegLinker();
				CSortedNegLinker(T* dataElement, bool isNegated = false, CSortedNegLinker<T*> *nextLink = 0);

				//! Destructor
				virtual ~CSortedNegLinker();


				CSortedNegLinker<T*> *init(T* dataElement, bool isNegated = false, CSortedNegLinker<T*> *nextLink = 0);

				bool hasNext();

				CSortedNegLinker<T*> *insertSortedNextSorted(CSortedNegLinker<T*> *nextLink);
				CSortedNegLinker<T*> *insertNextSorted(CSortedNegLinker<T*> *nextLink);

				
				CSortedNegLinker<T*> *insertNext(CSortedNegLinker<T*> *nextLink);
				CSortedNegLinker<T*> *getNext();

				CSortedNegLinker<T*> *setNext(CSortedNegLinker<T*> *nextLink);

				T* getData();
				CSortedNegLinker<T*> *setData(T* dataElement);

				bool isNegated();
				CSortedNegLinker<T*> *setNegated(bool isNegated);


				CSortedNegLinker<T*> *append(CSortedNegLinker<T*> *appendingList);

				CSortedNegLinker<T*> *removeNext();

				CSortedNegLinker<T*> *getLastListLink();

				qint64 getCount();

				bool resortLinkerData();


			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CSortedNegLinker<T*> *next;
				T* data;
				bool negated;

		};


		template<class T>
		CSortedNegLinker<T*>::CSortedNegLinker() {
			next = 0;
			negated = false;
		}

		template<class T>
		CSortedNegLinker<T*>::CSortedNegLinker(T* dataElement, bool isNegated, CSortedNegLinker<T*> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
		}

		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::init(T* dataElement, bool isNegated, CSortedNegLinker<T*> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
			return this;
		}


		template<class T>
		CSortedNegLinker<T*>::~CSortedNegLinker() {
		}


		template<class T>
		bool CSortedNegLinker<T*>::resortLinkerData() {
			bool hasResorted = false;
			if (next) {
				bool nextHasResorted = true;
				while (nextHasResorted) {
					nextHasResorted = next->resortLinkerData();
				}
				if (next->getData() > data) {
					T *tmpData = data;
					bool tmpNeg = negated;
					data = next->getData();
					negated = next->isNegated();
					next->setData(tmpData);
					next->setNegated(tmpNeg);
					hasResorted = true;
				}
			}
			return hasResorted;
		}


		template<class T>
		bool CSortedNegLinker<T*>::hasNext() {
			return next != 0;
		}

		
		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::insertNext(CSortedNegLinker<T*> *nextLink) {
			if (nextLink) {
				CSortedNegLinker<T*> *tmpNext = next;
				next = nextLink;
				if (tmpNext) {
					nextLink->append(tmpNext);
				}
			}
			return this;
		}


		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::insertNextSorted(CSortedNegLinker<T*> *nextLink) {
			CSortedNegLinker<T*> *firstLinker = this;
			while (nextLink) {

				CSortedNegLinker<T*> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
				} else {
					nextLink = 0;
				}
				nIns->setNext(0);

				CSortedNegLinker<T*> *list = firstLinker;
				CSortedNegLinker<T*> *lastList = 0;
				bool inserted = false;
				T* dataIns = nIns->getData();
				while (list) {
					T* dataList = list->getData();
					if (*dataIns <= *dataList) {
						if (lastList == 0){
							firstLinker = nIns->insertNext(firstLinker);
						} else {
							lastList->insertNext(nIns);
						}
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
			return firstLinker;
		}



		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::insertSortedNextSorted(CSortedNegLinker<T*> *nextLink) {
			// items from nextLink are already sorted!
			CSortedNegLinker<T*> *firstLinker = this;

			CSortedNegLinker<T*> *list = firstLinker;
			CSortedNegLinker<T*> *lastList = 0;

			while (nextLink) {

				CSortedNegLinker<T*> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
				} else {
					nextLink = 0;
				}
				nIns->setNext(0);

				bool inserted = false;
				T* dataIns = nIns->getData();
				while (list) {
					T* dataList = list->getData();
					if (*dataIns <= *dataList) {
						if (lastList == 0){
							firstLinker = nIns->insertNext(firstLinker);
							list = firstLinker->next;
							lastList = firstLinker;
						} else {
							lastList->insertNext(nIns);
							lastList = lastList->next;
							// list is already on the correct position
						}
						inserted = true;
						break;
					}
					lastList = list;
					list = list->next;
				}
				if (!inserted) {
					lastList->insertNext(nIns);
					lastList = nIns;
				}
			}
			return firstLinker;
		}




		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::setNext(CSortedNegLinker<T*> *nextLink) {
			next = nextLink;
			return this;
		}



		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::getNext() {
			return next;
		}



		template<class T>
		T* CSortedNegLinker<T*>::getData() {
			return data;
		}



		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::setData(T* dataElement) {
			data = dataElement;
			return this;
		}

		template<class T>
		bool CSortedNegLinker<T*>::isNegated() {
			return negated;
		}

		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::setNegated(bool isNegated) {
			negated = isNegated;
			return this;
		}



		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::append(CSortedNegLinker<T*> *appendingList) {
			CSortedNegLinker<T*> *last = getLastListLink();
			last->insertNext(appendingList);
			return this;
		}

		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::removeNext() {
			CSortedNegLinker<T*> *tmpNext = next;
			if (next) {
				CSortedNegLinker<T*> *tmpNextNext = next->next;
				next->next = 0;
				setNext(tmpNextNext);
			}
			return tmpNext;
		}




		template<class T>
		CSortedNegLinker<T*> *CSortedNegLinker<T*>::getLastListLink() {
			CSortedNegLinker<T*> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}



		template<class T>
		qint64 CSortedNegLinker<T*>::getCount() {
			qint64 count = 0;
			CSortedNegLinker<T*> *linkIt = this;
			while (linkIt) {
				++count;
				linkIt = linkIt->getNext();
			}
			return count;
		}





		/*! 
		 *
		 *		\class		CSortedNegLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CSortedNegLinker : public CAllocationObject {
			// public methods
			public:
				//! Constructor
				CSortedNegLinker();
				CSortedNegLinker(T dataElement, bool isNegated = false, CSortedNegLinker<T> *nextLink = 0);

				//! Destructor
				virtual ~CSortedNegLinker();


				CSortedNegLinker<T> *init(T dataElement, bool isNegated = false, CSortedNegLinker<T> *nextLink = 0);

				bool hasNext();

				CSortedNegLinker<T> *insertSortedNextSorted(CSortedNegLinker<T> *nextLink);
				CSortedNegLinker<T> *insertNextSorted(CSortedNegLinker<T> *nextLink);

				
				CSortedNegLinker<T> *insertNext(CSortedNegLinker<T> *nextLink);
				CSortedNegLinker<T> *getNext();

				CSortedNegLinker<T> *setNext(CSortedNegLinker<T> *nextLink);

				T getData();
				CSortedNegLinker<T> *setData(T dataElement);

				bool isNegated();
				CSortedNegLinker<T> *setNegated(bool isNegated);


				CSortedNegLinker<T> *append(CSortedNegLinker<T> *appendingList);

				CSortedNegLinker<T> *removeNext();

				CSortedNegLinker<T> *getLastListLink();

				qint64 getCount();

				bool resortLinkerData();


			// protected methods
			protected:

			// protected variables
			protected:
				CSortedNegLinker<T> *next;
				T data;
				bool negated;

			// private methods
			private:

			// private variables
			private:

		};


		template<class T>
		CSortedNegLinker<T>::CSortedNegLinker() {
			next = 0;
			negated = false;
		}

		template<class T>
		CSortedNegLinker<T>::CSortedNegLinker(T dataElement, bool isNegated, CSortedNegLinker<T> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
		}

		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::init(T dataElement, bool isNegated, CSortedNegLinker<T> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
			negated = isNegated;
			return this;
		}


		template<class T>
		CSortedNegLinker<T>::~CSortedNegLinker() {
		}


		template<class T>
		bool CSortedNegLinker<T>::resortLinkerData() {
			bool hasResorted = false;
			if (next) {
				bool nextHasResorted = true;
				while (nextHasResorted) {
					nextHasResorted = next->resortLinkerData();
				}
				if (next->getData() > data) {
					T tmpData = data;
					bool tmpNeg = negated;
					data = next->getData();
					negated = next->isNegated();
					next->setData(tmpData);
					next->setNegated(tmpNeg);
					hasResorted = true;
				}
			}
			return hasResorted;
		}


		template<class T>
		bool CSortedNegLinker<T>::hasNext() {
			return next != 0;
		}

		
		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::insertNext(CSortedNegLinker<T> *nextLink) {
			if (nextLink) {
				CSortedNegLinker<T> *tmpNext = next;
				next = nextLink;
				if (tmpNext) {
					nextLink->append(tmpNext);
				}
			}
			return this;
		}


		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::insertNextSorted(CSortedNegLinker<T> *nextLink) {
			CSortedNegLinker<T> *firstLinker = this;
			while (nextLink) {

				CSortedNegLinker<T> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
				} else {
					nextLink = 0;
				}
				nIns->setNext(0);

				CSortedNegLinker<T> *list = firstLinker;
				CSortedNegLinker<T> *lastList = 0;
				bool inserted = false;
				T dataIns = nIns->getData();
				while (list) {
					T dataList = list->getData();
					if (dataIns <= dataList) {
						if (lastList == 0){
							firstLinker = nIns->insertNext(firstLinker);
						} else {
							lastList->insertNext(nIns);
						}
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
			return firstLinker;
		}



		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::insertSortedNextSorted(CSortedNegLinker<T> *nextLink) {
			// items from nextLink are already sorted!
			CSortedNegLinker<T> *firstLinker = this;

			CSortedNegLinker<T> *list = firstLinker;
			CSortedNegLinker<T> *lastList = 0;

			while (nextLink) {

				CSortedNegLinker<T> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
				} else {
					nextLink = 0;
				}
				nIns->setNext(0);

				bool inserted = false;
				T dataIns = nIns->getData();
				while (list) {
					T dataList = list->getData();
					if (dataIns <= dataList) {
						if (lastList == 0){
							firstLinker = nIns->insertNext(firstLinker);
							list = firstLinker->next;
							lastList = firstLinker;
						} else {
							lastList->insertNext(nIns);
							lastList = lastList->next;
							// list is already on the correct position
						}
						inserted = true;
						break;
					}
					lastList = list;
					list = list->next;
				}
				if (!inserted) {
					lastList->insertNext(nIns);
					lastList = nIns;
				}
			}
			return firstLinker;
		}




		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::setNext(CSortedNegLinker<T> *nextLink) {
			next = nextLink;
			return this;
		}



		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::getNext() {
			return next;
		}



		template<class T>
		T CSortedNegLinker<T>::getData() {
			return data;
		}



		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::setData(T dataElement) {
			data = dataElement;
			return this;
		}

		template<class T>
		bool CSortedNegLinker<T>::isNegated() {
			return negated;
		}

		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::setNegated(bool isNegated) {
			negated = isNegated;
			return this;
		}



		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::append(CSortedNegLinker<T> *appendingList) {
			CSortedNegLinker<T> *last = getLastListLink();
			last->insertNext(appendingList);
			return this;
		}

		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::removeNext() {
			CSortedNegLinker<T> *tmpNext = next;
			if (next) {
				CSortedNegLinker<T> *tmpNextNext = next->next;
				next->next = 0;
				setNext(tmpNextNext);
			}
			return tmpNext;
		}




		template<class T>
		CSortedNegLinker<T> *CSortedNegLinker<T>::getLastListLink() {
			CSortedNegLinker<T> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}


		template<class T>
		qint64 CSortedNegLinker<T>::getCount() {
			qint64 count = 0;
			CSortedNegLinker<T> *linkIt = this;
			while (linkIt) {
				++count;
				linkIt = linkIt->getNext();
			}
			return count;
		}

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CSortedNegLinker_H
