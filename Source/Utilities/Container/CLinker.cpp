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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CLINKER_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CLINKER_CPP


#include "CLinker.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T, class S>
			CLinkerBase<T,S>::CLinkerBase() {
				next = 0;
			}

			template<class T, class S>
			CLinkerBase<T,S>::CLinkerBase(const T& dataElement, CLinkerBase<T,S>* nextLink) {
				next = 0;
				setNext(nextLink);
				data = dataElement;
			}

			template<class T, class S>
			CLinkerBase<T,S>::~CLinkerBase() {
			}

			template<class T, class S>
			S* CLinkerBase<T,S>::initLinker(const T& dataElement, CLinkerBase<T,S>* nextLink) {
				next = 0;
				setNext(nextLink);
				data = dataElement;
				return (S*)this;
			}

			template<class T, class S>
			bool CLinkerBase<T,S>::hasNext() const {
				return next != 0;
			}

			template<class T, class S>
			S* CLinkerBase<T,S>::insertNext(CLinkerBase<T,S>* nextLink) {
				if (nextLink) {
					CLinkerBase<T,S> *tmpNext = next;
					next = nextLink;
					if (tmpNext) {
						nextLink->append(tmpNext);
					}
				}
				return (S*)this;
			}

			template<class T, class S>
			inline S* CLinkerBase<T,S>::clearNext() {
				next = nullptr;
				return (S*)this;
			}


			template<class T, class S>
			S* CLinkerBase<T,S>::getNext() const {
				return (S*)next;
			}

			template<class T, class S>
			S* CLinkerBase<T,S>::setNext(CLinkerBase<T,S>* nextLink) {
				next = nextLink;
				return (S*)this;
			}

			template<class T, class S>
			T& CLinkerBase<T,S>::getData() {
				return data;
			}

			template<class T, class S>
			S* CLinkerBase<T,S>::setData(const T& dataElement) {
				data = dataElement;
				return (S*)this;
			}

			template<class T, class S>
			S* CLinkerBase<T,S>::append(CLinkerBase<T,S>* appendingList) {
				CLinkerBase<T,S>* last = getLastListLink();
				last->setNext(appendingList);
				return (S*)this;
			}

			template<class T, class S>
			S* CLinkerBase<T,S>::getRemovedNext() {
				CLinkerBase<T,S>* tmpNext = next;
				if (next) {
					CLinkerBase<T,S>* tmpNextNext = next->next;
					next->next = 0;
					setNext(tmpNextNext);
				}
				return (S*)tmpNext;
			}

			template<class T, class S>
			S* CLinkerBase<T,S>::getLastListLink() const {
				const CLinkerBase<T,S>* lastListLink = this;
				while (lastListLink->hasNext()) {
					lastListLink = lastListLink->getNext();
				}
				return (S*)lastListLink;
			}

			template<class T, class S>
			cint64 CLinkerBase<T,S>::getCount() const {
				int linkerCount = 0;
				const CLinkerBase<T,S>* itemLinker = this;
				while (itemLinker != nullptr) {
					++linkerCount;
					itemLinker = itemLinker->getNext();
				}
				return linkerCount;
			}

			template<class T, class S>
			cint64 CLinkerBase<T,S>::size() const {
				return getCount();
			}

			template<class T, class S>
			bool CLinkerBase<T,S>::contains(const T& dataElement) const {
				CLinkerBase<T,S>* itemLinker = this;
				while (itemLinker != nullptr) {
					if (itemLinker->getData() == dataElement) {
						return true;
					}
					itemLinker = itemLinker->getNext();
				}
				return false;
			}


			template<class T, class S>
			S* CLinkerBase<T,S>::removeOne(const T& dataElement) {
				CLinkerBase<T,S>* loc = this;
				if (loc->getData() == dataElement) {
					loc = loc->getNext();
					return (S*)loc;
				} else {
					while (loc) {
						CLinkerBase<T,S>* nxt = loc->getNext(); 
						if (nxt) {
							if (nxt->getData() == dataElement) {
								loc->setNext(nxt->getNext());
								return (S*)this;
							}
						}
						loc = loc->getNext();
					}
				}
				return (S*)this;
			}


			template<class T, class S>
			S* CLinkerBase<T,S>::removeAll(const T& dataElement) {
				CLinkerBase<T,S>* first = this;
				CLinkerBase<T,S>* loc = this;
				while (loc->getData() == dataElement) {
					loc = loc->getNext();
					first = loc;
				} 
				while (loc) {
					CLinkerBase<T,S>* nxt = loc->getNext(); 
					if (nxt) {
						if (nxt->getData() == dataElement) {
							loc->setNext(nxt->getNext());
						}
					}
					loc = loc->getNext();
				}
				return (S*)this;
			}


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CLINKER_CPP
