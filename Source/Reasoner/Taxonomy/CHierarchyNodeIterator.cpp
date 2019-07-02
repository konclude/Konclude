/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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
#ifndef KONCLUDE_REASONER_TAXONOMY_CHIERARCHYNODEITERATOR_CPP
#define KONCLUDE_REASONER_TAXONOMY_CHIERARCHYNODEITERATOR_CPP

#include "CHierarchyNodeIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class N>
			CHierarchyNodeIterator<N>::CHierarchyNodeIterator(QList<N*>* nodeList, bool backwardIterating) : mListIterator(*nodeList) {
				mNodeList = nodeList;
				mBackwardIterating = backwardIterating;
				if (mBackwardIterating) {
					mListIterator.toBack();
				}
			}

			template<class N>
			bool CHierarchyNodeIterator<N>::hasNextHierarchyNode() {
				if (mNodeList && (!mBackwardIterating && mListIterator.hasNext()) || (mBackwardIterating && mListIterator.hasPrevious())) {
					return true;
				}
				return false;
			}

			template<class N>
			N* CHierarchyNodeIterator<N>::nextHierarchyNode(bool moveNext) {
				N* node = nullptr;
				if (!mBackwardIterating) {
					if (mNodeList && mListIterator.hasNext()) {
						node = mListIterator.peekNext();
						if (moveNext) {
							mListIterator.next();
						}
					}
				} else {
					if (mNodeList && mListIterator.hasPrevious()) {
						node = mListIterator.peekPrevious();
						if (moveNext) {
							mListIterator.previous();
						}
					}
				}
				return node;
			}


		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CHIERARCHYNODEITERATOR_CPP
