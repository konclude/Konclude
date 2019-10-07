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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CSORTEDLINKER_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CSORTEDLINKER_CPP


#include "CSortedLinker.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T, class S, class C>
			CSortedLinkerBase<T,S,C>::CSortedLinkerBase() : CLinkerBase<T,S>() {
			}

			template<class T, class S, class C>
			CSortedLinkerBase<T,S,C>::CSortedLinkerBase(const T& dataElement, CSortedLinkerBase<T,S,C>* nextLink) : CLinkerBase<T,S>(dataElement,nextLink) {				
			}

			template<class T, class S, class C>
			CSortedLinkerBase<T,S,C>::~CSortedLinkerBase() {
			}


			template<class T, class S, class C>
			S* CSortedLinkerBase<T,S,C>::insertNextSorted(CSortedLinkerBase<T,S,C>* nextLink) {
				CSortedLinkerBase<T,S,C>* firstLinker = this;
				while (nextLink) {

					CSortedLinkerBase<T,S,C>* nIns = nextLink;

					nextLink = nextLink->getNext();
					nIns->setNext(nullptr);

					CSortedLinkerBase<T,S,C>* list = firstLinker;
					CSortedLinkerBase<T,S,C>* lastList = nullptr;
					bool inserted = false;
					const T& dataIns = nIns->getData();
					while (list) {
						const T& dataList = list->getData();
						// compare items
						if (isSortedBeforeCompare(dataIns,dataList)) {
							if (lastList == nullptr){
								firstLinker = nIns->insertNext(firstLinker);
							} else {
								lastList->insertNext(nIns);
							}
							inserted = true;
							break;
						}
						lastList = list;
						list = (CSortedLinkerBase<T,S,C>*)list->next;
					}
					if (!inserted) {
						lastList->insertNext(nIns);
					}
				}
				return (S*)firstLinker;
			}



			template<class T, class S, class C>
			S* CSortedLinkerBase<T,S,C>::insertSortedNextSorted(CSortedLinkerBase<T,S,C>* nextLink) {
				// items from nextLink are already sorted!
				CSortedLinkerBase<T,S,C>* firstLinker = this;

				CSortedLinkerBase<T,S,C>* list = firstLinker;
				CSortedLinkerBase<T,S,C>* lastList = nullptr;

				while (nextLink) {

					CSortedLinkerBase<T,S,C>* nIns = nextLink;

					nextLink = nextLink->getNext();
					nIns->setNext(nullptr);

					bool inserted = false;
					const T& dataIns = nIns->getData();
					while (list) {
						const T& dataList = list->getData();
						if (isSortedBeforeCompare(dataIns,dataList)) {
							if (lastList == 0){
								firstLinker = nIns->insertNext(firstLinker);
								list = (CSortedLinkerBase<T,S,C>*)firstLinker->next;
								lastList = firstLinker;
							} else {
								lastList->insertNext(nIns);
								lastList = (CSortedLinkerBase<T,S,C>*)lastList->next;
								// list is already on the correct position
							}
							inserted = true;
							break;
						}
						lastList = list;
						list = (CSortedLinkerBase<T,S,C>*)list->next;
					}
					if (!inserted) {
						lastList->insertNext(nIns);
						lastList = nIns;
					}
				}
				return (S*)firstLinker;
			}


			template<class T, class S, class C>
			bool CSortedLinkerBase<T,S,C>::isSortedBeforeCompare(const T& checkData, const T& linkedData) const {
				return C::sortedLinkerDataCompare(checkData,linkedData);
			}


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CSORTEDLINKER_CPP
