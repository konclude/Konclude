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
#ifndef KONCLUDE_REASONER_TAXONOMY_CHIERARCHYDEPTHTRAVERSALITERATOR_CPP
#define KONCLUDE_REASONER_TAXONOMY_CHIERARCHYDEPTHTRAVERSALITERATOR_CPP


#include "CHierarchyDepthTraversalIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class N>
			CHierarchyDepthTraversalIterator<N>::CHierarchyDepthTraversalIterator() : mCurrentIterator(QSet<N*>()) {
				mTopNode = nullptr;
				mBottomNode = nullptr;
				mUnigueVisit = false;
				mCurrentNode = nullptr;
			}


			template<class N>
			CHierarchyDepthTraversalIterator<N>::CHierarchyDepthTraversalIterator(N* topNode, N* bottomNode, bool forceUniqueVisit) : mCurrentIterator(*topNode->getChildNodeSet()) {
				mTopNode = topNode;
				mBottomNode = bottomNode;
				mUnigueVisit = forceUniqueVisit;
				mCurrentNode = topNode;
				mUniqueNodeSet.insert(mBottomNode);
			}

			template<class N>
			bool CHierarchyDepthTraversalIterator<N>::hasNextHierarchyNode() {
				if (mCurrentIterator.hasNext()) {
					QSetIterator<N*> tmpIterator(mCurrentIterator);
					while (tmpIterator.hasNext()) {
						N* nextNode = tmpIterator.next();
						if (!mUniqueNodeSet.contains(nextNode)) {
							return true;
						}
					}
				}
				for (typename QList< QSetIterator<N*> >::const_iterator it = mParentNodeIteratorList.constBegin(), itEnd = mParentNodeIteratorList.constEnd(); it != itEnd; ++it) {
					QSetIterator<N*> prevIt(*it);
					QSetIterator<N*> tmpIterator(prevIt);
					while (tmpIterator.hasNext()) {
						N* nextNode = tmpIterator.next();
						if (!mUniqueNodeSet.contains(nextNode)) {
							return true;
						}
					}
				}
				return false;
			}

			template<class N>
			N* CHierarchyDepthTraversalIterator<N>::nextHierarchyNode(bool moveNext) {
				N* node = nullptr;
				if (mCurrentIterator.hasNext()) {
					node = mCurrentIterator.peekNext();
					while (node && mUniqueNodeSet.contains(node)) {
						mCurrentIterator.next();
						if (mCurrentIterator.hasNext()) {
							node = mCurrentIterator.peekNext();
						} else {
							node = nullptr;
						}
					}
					if (moveNext && node) {
						mCurrentIterator.next();
						mParentNodeList.append(mCurrentNode);
						mParentNodeIteratorList.append(mCurrentIterator);
						mCurrentNode = node;
						mCurrentIterator = QSetIterator<N*>(*node->getChildNodeSet());
					}
				}
				if (!node) {
					QListIterator<N*> lastNodeIt(mParentNodeList);
					lastNodeIt.toBack();
					QListIterator< QSetIterator<N*> > lastIteratorIt(mParentNodeIteratorList);
					lastIteratorIt.toBack();

					while (!node && lastNodeIt.hasPrevious()) {
						N* lastNode = lastNodeIt.previous();
						QSetIterator<N*> lastIterator = lastIteratorIt.previous();
						if (lastIterator.hasNext()) {
							node = lastIterator.peekNext();
							while (node && mUniqueNodeSet.contains(node)) {
								lastIterator.next();
								if (lastIterator.hasNext()) {
									node = lastIterator.peekNext();
								} else {
									node = nullptr;
								}
							}
						}
						if (moveNext && node) {
							mClosedNodeBranchesList.append(mCurrentNode);
							QListIterator<N*> lastNodeRemIt(mParentNodeList);
							lastNodeRemIt.toBack();
							while (lastNodeRemIt.previous() != lastNode) {
								mClosedNodeBranchesList.append(mParentNodeList.takeLast());
								mParentNodeIteratorList.removeLast();
							}
							mParentNodeList.removeLast();
							mParentNodeIteratorList.removeLast();
							lastIterator.next();
							mParentNodeList.append(lastNode);
							mParentNodeIteratorList.append(lastIterator);
							mCurrentNode = node;
							mCurrentIterator = QSetIterator<N*>(*node->getChildNodeSet());
						}
					}
				}
				if (node && mUnigueVisit) {
					mUniqueNodeSet.insert(node);
				}

				return node;
			}

			template<class N>
			CHierarchyNodeIterator<N> CHierarchyDepthTraversalIterator<N>::getTraversedParentNodeIterator() {
				return CHierarchyNodeIterator<N>(&mParentNodeList,true);
			}

			template<class N>
			CHierarchyNodeIterator<N> CHierarchyDepthTraversalIterator<N>::getClosedTraversedNodeIterator() {
				return CHierarchyNodeIterator<N>(&mClosedNodeBranchesList,true);
			}

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CHIERARCHYDEPTHTRAVERSALITERATOR_CPP
