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

#ifndef KONCLUDE_UTILITIES_CONTAINER_CSORTEDNEGLINKER_CPP
#define KONCLUDE_UTILITIES_CONTAINER_CSORTEDNEGLINKER_CPP


#include "CSortedNegLinker.h"


namespace Konclude {

	namespace Utilities {

		namespace Container {

			template<class T, class S, class C>
			CSortedNegLinkerBase<T,S,C>::CSortedNegLinkerBase() : CNegLinkerBase<T,S>() {
			}

			template<class T, class S, class C>
			CSortedNegLinkerBase<T,S,C>::CSortedNegLinkerBase(const T& dataElement, bool negation, CSortedNegLinkerBase<T,S,C>* nextLink) : CNegLinkerBase<T,S>(dataElement,negation,nextLink) {				
			}

			template<class T, class S, class C>
			CSortedNegLinkerBase<T,S,C>::~CSortedNegLinkerBase() {
			}


			template<class T, class S, class C>
			S* CSortedNegLinkerBase<T,S,C>::insertNextSorted(CSortedNegLinkerBase<T,S,C>* nextLink) {
				CSortedNegLinkerBase<T,S,C>* firstLinker = this;
				while (nextLink) {

					CSortedNegLinkerBase<T,S,C>* nIns = nextLink;

					nextLink = nextLink->getNext();
					nIns->setNext(nullptr);

					CSortedNegLinkerBase<T,S,C>* list = firstLinker;
					CSortedNegLinkerBase<T,S,C>* lastList = nullptr;
					bool inserted = false;
					const T& dataIns = nIns->getData();
					bool dataInsNegated = nIns->isNegated();
					while (list) {
						const T& dataList = list->getData();
						bool dataListNegated = list->isNegated();
						// compare items
						if (isSortedBeforeCompare(dataIns,dataInsNegated,dataList,dataListNegated)) {
							if (lastList == nullptr){
								firstLinker = nIns->insertNext(firstLinker);
							} else {
								lastList->insertNext(nIns);
							}
							inserted = true;
							break;
						}
						lastList = list;
						list = (CSortedNegLinkerBase<T,S,C>*)list->next;
					}
					if (!inserted) {
						lastList->insertNext(nIns);
					}
				}
				return (S*)firstLinker;
			}



			template<class T, class S, class C>
			S* CSortedNegLinkerBase<T,S,C>::insertSortedNextSorted(CSortedNegLinkerBase<T,S,C>* nextLink) {
				// items from nextLink are already sorted!
				CSortedNegLinkerBase<T,S,C>* firstLinker = this;

				CSortedNegLinkerBase<T,S,C>* list = firstLinker;
				CSortedNegLinkerBase<T,S,C>* lastList = nullptr;

				while (nextLink) {

					CSortedNegLinkerBase<T,S,C>* nIns = nextLink;

					nextLink = nextLink->getNext();
					nIns->setNext(nullptr);

					bool inserted = false;
					const T& dataIns = nIns->getData();
					bool dataInsNegated = nIns->isNegated();
					while (list) {
						const T& dataList = list->getData();
						bool dataListNegated = list->isNegated();
						if (isSortedBeforeCompare(dataIns,dataInsNegated,dataList,dataListNegated)) {
							if (lastList == 0){
								firstLinker = nIns->insertNext(firstLinker);
								list = (CSortedNegLinkerBase<T,S,C>*)firstLinker->next;
								lastList = firstLinker;
							} else {
								lastList->insertNext(nIns);
								lastList = (CSortedNegLinkerBase<T,S,C>*)lastList->next;
								// list is already on the correct position
							}
							inserted = true;
							break;
						}
						lastList = list;
						list = (CSortedNegLinkerBase<T,S,C>*)list->next;
					}
					if (!inserted) {
						lastList->insertNext(nIns);
						lastList = nIns;
					}
				}
				return (S*)firstLinker;
			}


			template<class T, class S, class C>
			bool CSortedNegLinkerBase<T,S,C>::isSortedBeforeCompare(const T& checkData, const bool& checkNegation, const T& linkedData, const bool& linkedNegation) const {
				return C::sortedNegLinkerDataCompare(checkData,checkNegation,linkedData,linkedNegation);
			}


		}; // end namespace Container

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_CONTAINER_CSORTEDNEGLINKER_CPP
