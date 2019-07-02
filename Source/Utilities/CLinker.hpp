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

#ifndef KONCLUDE_UTILITIES_CLINKER_H
#define KONCLUDE_UTILITIES_CLINKER_H

// Libraries includes
#include <QLinkedListIterator>


// Namespace includes
#include "CAllocationObject.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Utilities {


		/*! 
		 *
		 *		\class		CLinker
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		template<class T>
		class CLinker : public CAllocationObject {
			// public methods
			public:
				//! Constructor
				CLinker();
				CLinker(T dataElement, CLinker<T> *nextLink = 0);

				//! Destructor
				virtual ~CLinker();


				CLinker<T> *init(T dataElement, CLinker<T> *nextLink = 0);

				bool hasNext();

				
				CLinker<T> *insertNext(CLinker<T> *nextLink);
				CLinker<T> *getNext();

				CLinker<T> *setNext(CLinker<T> *nextLink);

				T getData();
				CLinker<T> *setData(T dataElement);


				CLinker<T> *append(CLinker<T> *appendingList);

				CLinker<T> *removeNext();

				CLinker<T> *getLastListLink();


			// protected methods
			protected:

			// private methods
			private:

			// private variables
			private:
				CLinker<T> *next;
				T data;

		};


		template<class T>
		CLinker<T>::CLinker() {
			next = 0;
		}

		template<class T>
		CLinker<T>::CLinker(T dataElement, CLinker<T> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
		}

		template<class T>
		CLinker<T> *CLinker<T>::init(T dataElement, CLinker<T> *nextLink) {
			next = 0;
			setNext(nextLink);
			data = dataElement;
			return this;
		}


		template<class T>
		CLinker<T>::~CLinker() {
		}


		template<class T>
		bool CLinker<T>::hasNext() {
			return next != 0;
		}

		
		template<class T>
		CLinker<T> *CLinker<T>::insertNext(CLinker<T> *nextLink) {
			if (nextLink) {
				CLinker<T> *tmpNext = next;
				next = nextLink;
				if (tmpNext) {
					nextLink->append(tmpNext);
				}
			}
			return this;
		}



		template<class T>
		CLinker<T> *CLinker<T>::setNext(CLinker<T> *nextLink) {
			next = nextLink;
			return this;
		}



		template<class T>
		CLinker<T> *CLinker<T>::getNext() {
			return next;
		}



		template<class T>
		T CLinker<T>::getData() {
			return data;
		}



		template<class T>
		CLinker<T> *CLinker<T>::setData(T dataElement) {
			data = dataElement;
			return this;
		}



		template<class T>
		CLinker<T> *CLinker<T>::append(CLinker<T> *appendingList) {
			CLinker<T> *last = getLastListLink();
			last->insertNext(appendingList);
			return this;
		}

		template<class T>
		CLinker<T> *CLinker<T>::removeNext() {
			CLinker<T> *tmpNext = next;
			if (next) {
				CLinker<T> *tmpNextNext = next->next;
				next->next = 0;
				setNext(tmpNextNext);
			}
			return tmpNext;
		}


		template<class T>
		CLinker<T> *CLinker<T>::getLastListLink() {
			CLinker<T> *lastListLink = this;
			while (lastListLink->hasNext()) {
				lastListLink = lastListLink->getNext();
			}
			return lastListLink;
		}



	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CLINKER_H
