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

#ifndef KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHY_CPP
#define KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHY_CPP

#include "CParentChildPredecessorHierarchy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {



			template<class T, class N, class E>
			CParentChildPredecessorHierarchy<T,N,E>::CParentChildPredecessorHierarchy() {
				topNode = nullptr;
				bottomNode = nullptr;
			}

			template<class T, class N, class E>
			CParentChildPredecessorHierarchy<T,N,E>::CParentChildPredecessorHierarchy(N* topHierarchyNode, N* bottomHierarchyNode) {
				topNode = topHierarchyNode;
				bottomNode = bottomHierarchyNode;
				nodeContainer.append(topNode);
				nodeContainer.append(bottomNode);

				bottomNode->makeParent(topNode);
			}

			template<class T, class N, class E>
			CParentChildPredecessorHierarchy<T,N,E>::CParentChildPredecessorHierarchy(const E& topElement, const E& bottomElement) {
				topNode = createHierarchyNode(topElement);
				bottomNode = createHierarchyNode(bottomElement);

				bottomNode->makeParent(topNode);

			}


			template<class T, class N, class E>
			T* CParentChildPredecessorHierarchy<T,N,E>::initHierarchy(const E& topElement, const E& bottomElement) {
				qDeleteAll(nodeContainer);
				nodeContainer.clear();
				elmNodeHash.clear();
				topNode = createHierarchyNode(topElement);
				bottomNode = createHierarchyNode(bottomElement);

				bottomNode->makeParent(topNode);
				return (T*)this;
			}


			template<class T, class N, class E>
			CParentChildPredecessorHierarchy<T,N,E>::~CParentChildPredecessorHierarchy() {
				qDeleteAll(nodeContainer);
			}


			template<class T, class N, class E>
			N* CParentChildPredecessorHierarchy<T,N,E>::createHierarchyNode(const E& element) {
				N* node = new N(element);
				nodeContainer.append(node);
				elmNodeHash.insert(element,node);
				return node;
			}




			template<class T, class N, class E>
			N* CParentChildPredecessorHierarchy<T,N,E>::getTopHierarchyNode() {
				return topNode;
			}

			template<class T, class N, class E>
			N* CParentChildPredecessorHierarchy<T,N,E>::getBottomHierarchyNode() {
				return bottomNode;
			}


			template<class T, class N, class E>
			N* CParentChildPredecessorHierarchy<T,N,E>::getHierarchyNode(const E& element) {
				return elmNodeHash.value(element,nullptr);
			}


			template<class T, class N, class E>
			N* CParentChildPredecessorHierarchy<T,N,E>::getHierarchyNode(const E& element, bool create) {
				N* node = getHierarchyNode(element);
				if (!node) {
					node = createHierarchyNode(element);
				}
				return node;
			}


			template<class T, class N, class E>
			bool CParentChildPredecessorHierarchy<T,N,E>::hasHierarchyNode(const E& element) {
				return elmNodeHash.contains(element);
			}



			template<class T, class N, class E>
			bool CParentChildPredecessorHierarchy<T,N,E>::isNodeSubsumption(N* subsumerNode, N* subsumedNode) {
				if (!subsumerNode || !subsumedNode) {
					return false;
				}
				if (subsumerNode == subsumedNode) {
					return true;
				}
				return subsumerNode->hasPredecessorNode(subsumedNode);
			}

			template<class T, class N, class E>
			bool CParentChildPredecessorHierarchy<T,N,E>::isNodeEquivalence(N* node1, N* node2) {
				if (!node1 || !node2) {
					return false;
				}
				if (node1 == node2) {
					return true;
				}
				return node1->hasPredecessorNode(node2) && node2->hasPredecessorNode(node1);
			}


			template<class T, class N, class E>
			T* CParentChildPredecessorHierarchy<T,N,E>::updateNodeEquivalentElements(N* node) {
				E topElement = topNode->getOneEquivalentElement();
				E bottomElement = bottomNode->getOneEquivalentElement();
				if (node) {
					QList<E>* equivalentElementList = node->getEquivalentElementList();
					for (typename QList<E>::const_iterator it = equivalentElementList->constBegin(), itEnd = equivalentElementList->constEnd(); it != itEnd; ++it) {
						E element(*it);
						if (element == topElement) {
							topNode = node;
						}
						if (element == bottomElement) {
							bottomNode = node;
						}
						elmNodeHash.insert(element,node);
					}
				}
				return (T*)this;
			}


			template<class T, class N, class E>
			bool CParentChildPredecessorHierarchy<T,N,E>::isNodeSatisfiable(N* node) {
				return node != bottomNode && !node->hasPredecessorNode(bottomNode);
			}

			template<class T, class N, class E>
			bool CParentChildPredecessorHierarchy<T,N,E>::isNodeNotSatisfiable(N* node) {
				return node = bottomNode;
			}


			template<class T, class N, class E>
			bool CParentChildPredecessorHierarchy<T,N,E>::isTopHierarchyNode(N* node) {
				return node && topNode == node;
			}

			template<class T, class N, class E>
			bool CParentChildPredecessorHierarchy<T,N,E>::isBottomHierarchyNode(N* node) {
				return node && bottomNode == node;
			}



			template<class T, class N, class E>
			bool CParentChildPredecessorHierarchy<T,N,E>::repairInsertion(N* node, N* insertParentNode) {
				QSet<N*>* nodeChildSet = insertParentNode->getChildNodeSet();
				for (typename QSet<N*>::iterator it = nodeChildSet->begin(), itEnd = nodeChildSet->end(); it != itEnd; ++it) {
					N* childNode = *it;
					if (childNode != node && childNode->hasPredecessorNode(node)) {
						// now indirect predecessor
						childNode->removeParent(insertParentNode);
						it = nodeChildSet->erase(it);
						if (it == itEnd) {
							break;
						}
					}
				}
				QSet<N*>* nodeParentSet = node->getParentNodeSet();
				for (typename QSet<N*>::iterator it = nodeParentSet->begin(), itEnd = nodeParentSet->end(); it != itEnd; ++it) {
					N* parentNode = *it;
					if (parentNode != insertParentNode && insertParentNode->hasPredecessorNode(parentNode)) {
						// now indirect predecessor
						parentNode->removeChild(node);
						it = nodeParentSet->erase(it);
						if (it == itEnd) {
							break;
						}
					}
				}
				return true;
			}



			template<class T, class N, class E>
			bool CParentChildPredecessorHierarchy<T,N,E>::repairedParentNodeInsertion(N* node, N* insertParentNode) {
				if (!node->hasPredecessorNode(insertParentNode)) {
					node->makeParent(insertParentNode);

					repairInsertion(node,insertParentNode);

					N* updatePredNode = insertParentNode;
					QSet<N*>* updatePredSet = updatePredNode->getPredecessorNodeSet();
					for (typename QSet<N*>::const_iterator predIt = updatePredSet->constBegin(), predItEnd = updatePredSet->constEnd(); predIt != predItEnd; ++predIt) {
						updatePredNode = *predIt;						
						QList<N*> updateList(node->getChildNodeList());
						while (!updateList.isEmpty()) {
							N* updateNode = updateList.takeFirst();
							if (!updateNode->hasPredecessorNode(updatePredNode)) {
								updateNode->makePredecessor(updatePredNode);
								QSet<N*>* updateNodeChildSet = updateNode->getChildNodeSet();
								for (typename QSet<N*>::const_iterator it = updateNodeChildSet->constBegin(), itEnd = updateNodeChildSet->constEnd(); it != itEnd; ++it) {
									updateList.append(*it);
								}
							}
						}
						updatePredNode = nullptr;
					}
				}
				return false;
			}


			template<class T, class N, class E>
			T* CParentChildPredecessorHierarchy<T,N,E>::setNodeSatisfiable(N* node) {
				KONCLUDE_ASSERT_X(node != bottomNode,"hierarchy satisfiable","bottom element can not be satisfiable");
				bottomNode->makeParent(node);
				node->makeParent(topNode);
				repairInsertion(bottomNode,node);
				repairInsertion(node,topNode);
				return (T*)this;
			}

			template<class T, class N, class E>
			T* CParentChildPredecessorHierarchy<T,N,E>::setNodeUnsatisfiable(N* node) {
				QList<N*> updateList;
				updateList.append(node);
				while (!updateList.isEmpty()) {
					N* updateNode = updateList.takeFirst();
					QList<E>* elementList = updateNode->getEquivalentElementList();
					for (typename QList<E>::const_iterator it = elementList->constBegin(), itEnd = elementList->constEnd(); it != itEnd; ++it) {
						E element = *it;
						bottomNode->addEquivalentElement(element);
						elmNodeHash.insert(element,bottomNode);
					}
					QSet<N*>* updateNodeChildSet = updateNode->getChildNodeSet();
					for (typename QSet<N*>::const_iterator it = updateNodeChildSet->constBegin(), itEnd = updateNodeChildSet->constEnd(); it != itEnd; ++it) {
						updateList.append(*it);
					}
				}
				QSet<N*>* nodeParentSet = node->getParentNodeSet();
				for (typename QSet<N*>::iterator it = nodeParentSet->begin(), itEnd = nodeParentSet->end(); it != itEnd; ++it) {
					N* parentNode = *it;
					it = nodeParentSet->erase(it);
					node->removeParent(parentNode);
					repairedParentNodeInsertion(bottomNode,parentNode);
					if (it == itEnd) {
						break;
					}
				}

				return (T*)this;
			}

			template<class T, class N, class E>
			T* CParentChildPredecessorHierarchy<T,N,E>::setNodeSubsumption(N* subsumerNode, N* subsumedNode) {
				if (subsumerNode != subsumedNode) {
					if (subsumedNode == bottomNode) {
						// unsatisfiable
						setNodeUnsatisfiable(subsumerNode);
					}
					if (subsumerNode->hasPredecessorNode(subsumedNode)) {
						// make equivalent
						N* mergeIntoNode = subsumedNode;
						N* mergingNode = subsumerNode;
						if (subsumerNode == bottomNode || subsumerNode == topNode) {
							mergeIntoNode = subsumerNode;
							mergingNode = subsumedNode;
						}

						QList<E>* elementList = mergingNode->getEquivalentElementList();
						for (typename QList<E>::const_iterator it = elementList->constBegin(), itEnd = elementList->constEnd(); it != itEnd; ++it) {
							E element = *it;
							mergeIntoNode->addEquivalentElement(element);
							elmNodeHash.insert(element,mergeIntoNode);
						}

						QList<N*> nodeChildList(mergingNode->getChildNodeList());
						QList<N*> updateList(nodeChildList);
						QList<N*> nodeParentList(mergingNode->getParentNodeList());
						for (typename QList<N*>::const_iterator it = nodeChildList.constBegin(), itEnd = nodeChildList.constEnd(); it != itEnd; ++it) {
							N* parentNode = *it;
							parentNode->removeChild(mergingNode);
							mergingNode->removeParent(parentNode);
						}
						for (typename QList<N*>::const_iterator it = nodeChildList.constBegin(), itEnd = nodeChildList.constEnd(); it != itEnd; ++it) {
							N* childNode = *it;
							childNode->removeParent(mergingNode);
							mergingNode->removeChild(childNode);
						}
						while (!updateList.isEmpty()) {
							N* updateNode = updateList.takeFirst();
							if (updateNode->hasPredecessorNode(mergingNode)) {
								updateNode->removePredecessor(mergingNode);
								QSet<N*>* updateNodeChildSet = updateNode->getChildNodeSet();
								for (typename QSet<N*>::const_iterator it = updateNodeChildSet->constBegin(), itEnd = updateNodeChildSet->constEnd(); it != itEnd; ++it) {
									updateList.append(*it);
								}
							}
						}
						for (typename QList<N*>::const_iterator it = nodeChildList.constBegin(), itEnd = nodeChildList.constEnd(); it != itEnd; ++it) {
							N* parentNode = *it;
							setNodeSubsumption(mergeIntoNode,parentNode);
						}
						for (typename QList<N*>::const_iterator it = nodeChildList.constBegin(), itEnd = nodeChildList.constEnd(); it != itEnd; ++it) {
							N* childNode = *it;
							setNodeSubsumption(childNode,mergeIntoNode);
						}

					} else {
						repairedParentNodeInsertion(subsumerNode,subsumedNode);
					}
				}
				return (T*)this;
			}

			template<class T, class N, class E>
			T* CParentChildPredecessorHierarchy<T,N,E>::setNodeEquivalence(N* eqNode1, N* eqNode2) {
				if (eqNode1 != eqNode2) {
					setNodeSubsumption(eqNode1,eqNode2);
					setNodeSubsumption(eqNode2,eqNode1);
				}
				return (T*)this;
			}


			template<class T, class N, class E>
			CHierarchyDepthTraversalIterator<N> CParentChildPredecessorHierarchy<T,N,E>::getDepthTraversalIterator(bool forceUnique) {
				return CHierarchyDepthTraversalIterator<N>(topNode,bottomNode,forceUnique);
			}


		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHY_CPP
