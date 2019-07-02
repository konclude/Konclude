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

#ifndef KONCLUDE_UTILITIES_CSORTEDLINKER_H
#define KONCLUDE_UTILITIES_CSORTEDLINKER_H

// Libraries includes
#include <QLinkedListIterator>


// Namespace includes
#include "CAllocationObject.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Utilities {


		template<class T> class CSortedLinker;



		/*! 
		 *
		 *		\class		CSortedLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Spezialised Pointer Version to avoid comparison between addresses
		 *
		 */
		template<class T>
		class CSortedLinker<T*> : public CAllocationObject {
			// public methods
			public:
				//! Constructor
				CSortedLinker();
				CSortedLinker(T* dataElement, CSortedLinker<T*> *nextLink = 0);

				//! Destructor
				virtual ~CSortedLinker();


				CSortedLinker<T*> *init(T* dataElement, CSortedLinker<T*> *nextLink = 0);

				bool hasNext();

				CSortedLinker<T*> *insertSortedNextSorted(CSortedLinker<T*> *nextLink);
				CSortedLinker<T*> *insertNextSorted(CSortedLinker<T*> *nextLink);

				
				CSortedLinker<T*> *insertNext(CSortedLinker<T*> *nextLink);
				CSortedLinker<T*> *getNext();

				CSortedLinker<T*> *setNext(CSortedLinker<T*> *nextLink);

				T* getData();
				CSortedLinker<T*> *setData(T* dataElement);


				CSortedLinker<T*> *append(CSortedLinker<T*> *appendingList);

				CSortedLinker<T*> *removeNext();

				CSortedLinker<T*> *removeOne(T* dataElement);
				CSortedLinker<T*> *removeAll(T* dataElement);


				CSortedLinker<T*> *getLastListLink();


			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CSortedLinker<T*> *next;
				T* data;

		};


		template<class T>
		CSortedLinker<T*>::CSortedLinker() {
			next = 0;
		}

		template<class T>
		CSortedLinker<T*>::CSortedLinker(T* dataElement, CSortedLinker<T*> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
		}

		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::init(T* dataElement, CSortedLinker<T*> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
			return this;
		}


		template<class T>
		CSortedLinker<T*>::~CSortedLinker() {
		}


		template<class T>
		bool CSortedLinker<T*>::hasNext() {
			return next != 0;
		}

		
		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::insertNext(CSortedLinker<T*> *nextLink) {
			if (nextLink) {
				CSortedLinker<T*> *tmpNext = next;
				next = nextLink;
				if (tmpNext) {
					nextLink->append(tmpNext);
				}
			}
			return this;
		}


		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::insertNextSorted(CSortedLinker<T*> *nextLink) {
			CSortedLinker<T*> *firstLinker = this;
			while (nextLink) {

				CSortedLinker<T*> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
				} else {
					nextLink = 0;
				}
				nIns->setNext(0);

				CSortedLinker<T*> *list = firstLinker;
				CSortedLinker<T*> *lastList = 0;
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
		CSortedLinker<T*> *CSortedLinker<T*>::insertSortedNextSorted(CSortedLinker<T*> *nextLink) {
			// items from nextLink are already sorted!
			CSortedLinker<T*> *firstLinker = this;

			CSortedLinker<T*> *list = firstLinker;
			CSortedLinker<T*> *lastList = 0;

			while (nextLink) {

				CSortedLinker<T*> *nIns = nextLink;

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
				}
			}
			return firstLinker;
		}




		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::setNext(CSortedLinker<T*> *nextLink) {
			next = nextLink;
			return this;
		}



		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::getNext() {
			return next;
		}



		template<class T>
		T* CSortedLinker<T*>::getData() {
			return data;
		}



		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::setData(T* dataElement) {
			data = dataElement;
			return this;
		}



		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::append(CSortedLinker<T*> *appendingList) {
			CSortedLinker<T*> *last = getLastListLink();
			last->insertNext(appendingList);
			return this;
		}

		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::removeNext() {
			CSortedLinker<T*> *tmpNext = next;
			if (next) {
				CSortedLinker<T*> *tmpNextNext = next->next;
				next->next = 0;
				setNext(tmpNextNext);
			}
			return tmpNext;
		}


		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::removeOne(T* dataElement) {
			CSortedLinker<T*> *loc = this;
			if (loc->getData() == dataElement) {
				loc = loc->getNext();
				return loc;
			} else {
				while (loc) {
					CSortedLinker<T*> *nxt = loc->getNext(); 
					if (nxt) {
						if (nxt->getData() == dataElement) {
							loc->setNext(nxt->getNext());
							return this;
						}
					}
					loc = loc->getNext();
				}
			}
			return this;
		}


		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::removeAll(T* dataElement) {
			CSortedLinker<T*> *loc = this;
			if (loc->getData() == dataElement) {
				loc = loc->getNext();
				return loc;
			} else {
				while (loc) {
					CSortedLinker<T*> *nxt = loc->getNext(); 
					if (nxt) {
						if (nxt->getData() == dataElement) {
							loc->setNext(nxt->getNext());
						}
					}
					loc = loc->getNext();
				}
			}
			return this;
		}


		template<class T>
		CSortedLinker<T*> *CSortedLinker<T*>::getLastListLink() {
			CSortedLinker<T*> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}







		/*! 
		 *
		 *		\class		CSortedLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CSortedLinker : public CAllocationObject {
			// public methods
			public:
				//! Constructor
				CSortedLinker();
				CSortedLinker(T dataElement, CSortedLinker<T> *nextLink = 0);

				//! Destructor
				virtual ~CSortedLinker();


				CSortedLinker<T> *init(T dataElement, CSortedLinker<T> *nextLink = 0);

				bool hasNext();

				CSortedLinker<T> *insertSortedNextSorted(CSortedLinker<T> *nextLink);
				CSortedLinker<T> *insertNextSorted(CSortedLinker<T> *nextLink);

				
				CSortedLinker<T> *insertNext(CSortedLinker<T> *nextLink);
				CSortedLinker<T> *getNext();

				CSortedLinker<T> *setNext(CSortedLinker<T> *nextLink);

				T getData();
				CSortedLinker<T> *setData(T dataElement);


				CSortedLinker<T> *append(CSortedLinker<T> *appendingList);

				CSortedLinker<T> *removeNext();

				CSortedLinker<T> *getLastListLink();

				CSortedLinker<T> *removeOne(T dataElement);
				CSortedLinker<T> *removeAll(T dataElement);

			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CSortedLinker<T> *next;
				T data;

		};


		template<class T>
		CSortedLinker<T>::CSortedLinker() {
			next = 0;
		}

		template<class T>
		CSortedLinker<T>::CSortedLinker(T dataElement, CSortedLinker<T> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
		}

		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::init(T dataElement, CSortedLinker<T> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
			return this;
		}


		template<class T>
		CSortedLinker<T>::~CSortedLinker() {
		}


		template<class T>
		bool CSortedLinker<T>::hasNext() {
			return next != 0;
		}

		
		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::insertNext(CSortedLinker<T> *nextLink) {
			if (nextLink) {
				CSortedLinker<T> *tmpNext = next;
				next = nextLink;
				if (tmpNext) {
					nextLink->append(tmpNext);
				}
			}
			return this;
		}


		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::insertNextSorted(CSortedLinker<T> *nextLink) {
			CSortedLinker<T> *firstLinker = this;
			while (nextLink) {

				CSortedLinker<T> *nIns = nextLink;

				if (nextLink->hasNext()) {
					nextLink = nextLink->getNext();
				} else {
					nextLink = 0;
				}
				nIns->setNext(0);

				CSortedLinker<T> *list = firstLinker;
				CSortedLinker<T> *lastList = 0;
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
		CSortedLinker<T> *CSortedLinker<T>::insertSortedNextSorted(CSortedLinker<T> *nextLink) {
			// items from nextLink are already sorted!
			CSortedLinker<T> *firstLinker = this;

			CSortedLinker<T> *list = firstLinker;
			CSortedLinker<T> *lastList = 0;

			while (nextLink) {

				CSortedLinker<T> *nIns = nextLink;

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
				}
			}
			return firstLinker;
		}




		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::setNext(CSortedLinker<T> *nextLink) {
			next = nextLink;
			return this;
		}



		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::getNext() {
			return next;
		}



		template<class T>
		T CSortedLinker<T>::getData() {
			return data;
		}



		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::setData(T dataElement) {
			data = dataElement;
			return this;
		}



		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::append(CSortedLinker<T> *appendingList) {
			CSortedLinker<T> *last = getLastListLink();
			last->insertNext(appendingList);
			return this;
		}

		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::removeNext() {
			CSortedLinker<T> *tmpNext = next;
			if (next) {
				CSortedLinker<T> *tmpNextNext = next->next;
				next->next = 0;
				setNext(tmpNextNext);
			}
			return tmpNext;
		}


		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::removeOne(T dataElement) {
			CSortedLinker<T> *loc = this;
			if (loc->getData() == dataElement) {
				loc = loc->getNext();
				return loc;
			} else {
				while (loc) {
					CSortedLinker<T> *nxt = loc->getNext(); 
					if (nxt) {
						if (nxt->getData() == dataElement) {
							loc->setNext(nxt->getNext());
							return this;
						}
					}
					loc = loc->getNext();
				}
			}
			return this;
		}


		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::removeAll(T dataElement) {
			CSortedLinker<T> *loc = this;
			if (loc->getData() == dataElement) {
				loc = loc->getNext();
				return loc;
			} else {
				while (loc) {
					CSortedLinker<T> *nxt = loc->getNext(); 
					if (nxt) {
						if (nxt->getData() == dataElement) {
							loc->setNext(nxt->getNext());
						}
					}
					loc = loc->getNext();
				}
			}
			return this;
		}


		template<class T>
		CSortedLinker<T> *CSortedLinker<T>::getLastListLink() {
			CSortedLinker<T> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}



	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CSortedLinker_H
