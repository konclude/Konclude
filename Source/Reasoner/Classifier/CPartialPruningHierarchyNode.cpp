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

#include "CPartialPruningHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CPartialPruningHierarchyNode::CPartialPruningHierarchyNode(CConcept *representiveConcept) : CHierarchyNode(representiveConcept) {
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
			}


			CPartialPruningHierarchyNode::~CPartialPruningHierarchyNode() {
			}


			CHierarchyNode *CPartialPruningHierarchyNode::repairParentNodeInsertion(CHierarchyNode *replacedParentNodeCandidate, CClassifierStatistics *statistics) {
				// called for the subsumer node
				QList<CHierarchyNode *> parentNodes(parentNodeSet.values());
				for (QList<CHierarchyNode *>::const_iterator it = parentNodes.constBegin(), itEnd = parentNodes.constEnd(); it != itEnd; ++it) {
					CHierarchyNode *parentNode = *it;
					if (parentNode != replacedParentNodeCandidate) {
						if (replacedParentNodeCandidate->hasPredecessorNode(parentNode)) {
							// the previous parent node is not longer a direct parent
							removeParent(parentNode);
							parentNode->removeChild(this);
						}
					}
				}
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CPartialPruningHierarchyNode::repairChildsParentNodeReplacement(CHierarchyNode *replacedParentNodeCandidate, CClassifierStatistics *statistics) {
				// called for the subsumed node
				QList<CHierarchyNode *> childNodes(childNodeSet.values());
				for (QList<CHierarchyNode *>::const_iterator it = childNodes.constBegin(), itEnd = childNodes.constEnd(); it != itEnd; ++it) {
					CHierarchyNode *childNode = *it;
					if (childNode->hasPredecessorNode(replacedParentNodeCandidate)) {
						removeChild(childNode);
						childNode->removeParent(this);
					}
				}
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}


			CHierarchyNode *CPartialPruningHierarchyNode::mergeToParentNode(CHierarchyNode *mergeParentDirection, CClassifierStatistics *statistics) {

				QSet<CHierarchyNode*> mergeNodesSet;
				QList<CHierarchyNode*> potMergeNodesList(getParentNodeList());

				while (!potMergeNodesList.isEmpty()) {
					CHierarchyNode* potMergeNode = potMergeNodesList.takeFirst();
					if (potMergeNode->hasPredecessorNode(mergeParentDirection) || potMergeNode == mergeParentDirection) {
						if (!mergeNodesSet.contains(potMergeNode)) {
							mergeNodesSet.insert(potMergeNode);
							if (potMergeNode != mergeParentDirection) {
								potMergeNodesList.append(potMergeNode->getParentNodeList());
							}
						}
					}
				}

				foreach (CHierarchyNode* mergeNode, mergeNodesSet) {
					foreach (CConcept* concept, *mergeNode->getEquivalentConceptList()) {
						addEquivalentConcept(concept);
					}
					foreach (CHierarchyNode* parentNode, *mergeNode->getParentNodeSet()) {
						if (!hasParentNode(parentNode) && !mergeNodesSet.contains(parentNode)) {
							makeParent(parentNode);
						}
						mergeNode->removeParent(parentNode);
						parentNode->removeChild(mergeNode);
					}
					foreach (CHierarchyNode* childNode, *mergeNode->getChildNodeSet()) {
						if (!hasChildNode(childNode) && !mergeNodesSet.contains(childNode) && childNode != this) {
							makeChild(childNode);
						}
						mergeNode->removeChild(childNode);
						childNode->removeParent(mergeNode);
					}

					foreach (CHierarchyNode* predNode, *mergeNode->getPredecessorNodeSet()) {
						if (!hasPredecessorNode(predNode) && !mergeNodesSet.contains(predNode)) {
							makePredecessor(predNode);
						}
						mergeNode->removePredecessor(predNode);
						predNode->removeSuccessor(mergeNode);
					}
					foreach (CHierarchyNode* succNode, *mergeNode->getSuccessorNodeSet()) {
						if (!hasSuccessorNode(succNode) && !mergeNodesSet.contains(succNode) && succNode != this) {
							makeSuccessor(succNode);
						}
						mergeNode->removeSuccessor(succNode);
						succNode->removePredecessor(mergeNode);
					}
					foreach (CHierarchyNode* incPredNode, *mergeNode->getIncompatiblePredecessorNodeSet()) {
						if (!hasIncompatiblePredecessorNode(incPredNode)) {
							makeIncompatiblePredecessor(incPredNode);
						}
						mergeNode->removeIncompatiblePredecessorNode(incPredNode);
						incPredNode->removeIncompatibleSuccessorNode(mergeNode);
					}
					foreach (CHierarchyNode* incSuccNode, *mergeNode->getIncompatibleSuccessorNodeSet()) {
						if (!hasIncompatibleSuccessorNode(incSuccNode)) {
							makeIncompatibleSuccessor(incSuccNode);
						}
						mergeNode->removeIncompatibleSuccessorNode(incSuccNode);
						incSuccNode->removeIncompatiblePredecessorNode(mergeNode);
					}

					foreach (CHierarchyNode* disjNode, *mergeNode->getDisjointNodeSet()) {
						if (!hasDisjointNode(disjNode)) {
							makeDisjoint(disjNode);
						}
						mergeNode->removeDisjointNode(disjNode);
						disjNode->removeDisjointNode(mergeNode);
					}
				}

				foreach (CHierarchyNode* parNode1, parentNodeSet) {
					foreach (CHierarchyNode* parNode2, parentNodeSet) {
						if (parNode1 != parNode2 && parNode2->hasPredecessorNode(parNode1)) {
							removeParent(parNode1);
							parNode1->removeChild(this);
						}
					}
				}

				foreach (CHierarchyNode* childNode1, childNodeSet) {
					foreach (CHierarchyNode* childNode2, childNodeSet) {
						if (childNode1 != childNode2 && childNode1->hasPredecessorNode(childNode2)) {
							removeChild(childNode1);
							childNode1->removeParent(this);
						}
					}
				}


				//QSet<CHierarchyNode *> parentMergeNodesSet(parentNodeSet);
				//QList<CHierarchyNode *> parentMergeNodesList(parentMergeNodesSet.values());
				//parentMergeNodesSet.insert(this);
				//bool mergedIntoDirection = false;
				//while (!parentMergeNodesList.isEmpty()) {
				//	CHierarchyNode *parentNode = parentMergeNodesList.takeFirst();

				//	if (parentNode == this) {
				//		bool bug = true;
				//	}

				//	if (parentNode == mergeParentDirection || parentNode->hasPredecessorNode(mergeParentDirection)) {
				//		// has to merge
				//		qint64 addParentEqConCount = parentNode->getEquivalentConceptCount();
				//		qint64 thisPrevEqConCount = eqConCount;

				//		foreach (CConcept *con, *parentNode->getEquivalentConceptList()) {
				//			eqConCount++;
				//			eqConList.append(con);
				//		}
				//		if (statistics && parentNode != mergeParentDirection) {
				//			statistics->incDerivatedTestedSubsumptionCount(addParentEqConCount*thisPrevEqConCount);
				//		}

				//		if (statistics) {
				//			foreach (CHierarchyNode *precNode, predNodeSet) {
				//				if (!parentNode->hasPredecessorNode(precNode) && parentNode != mergeParentDirection) {
				//					statistics->incDerivatedTestedSubsumptionCount(addParentEqConCount*precNode->getEquivalentConceptCount());
				//				}
				//			}
				//			foreach (CHierarchyNode *succNode, succNodeSet) {
				//				if (!parentNode->hasSuccessorNode(succNode)) {
				//					statistics->incDerivatedTestedSubsumptionCount(addParentEqConCount*succNode->getEquivalentConceptCount());
				//				}
				//			}
				//			foreach (CHierarchyNode *incPrecNode, incompPredNodeSet) {
				//				if (!parentNode->hasIncompatiblePredecessorNode(incPrecNode)) {
				//					statistics->incPrunedTestedSubsumptionCount(addParentEqConCount*incPrecNode->getEquivalentConceptCount());
				//				}
				//			}
				//			foreach (CHierarchyNode *incSuccNode, incompSuccNodeSet) {
				//				if (!parentNode->hasIncompatibleSuccessorNode(incSuccNode)) {
				//					statistics->incPrunedTestedSubsumptionCount(addParentEqConCount*incSuccNode->getEquivalentConceptCount());
				//				}
				//			}
				//		}

				//		//((CPartialPruningHierarchyNode *)parentNode)->repairChildsParentNodeReplacement(this,statistics);


				//		QList<CHierarchyNode *> parentList(parentNode->getParentNodeList());
				//		foreach (CHierarchyNode *parentParentNode, parentList) {
				//			parentParentNode->removeChild(parentNode);
				//			parentNode->removeParent(parentParentNode);
				//			if (parentNodeSet.isEmpty()) {
				//				bool bug = true;
				//			}
				//			if (!hasPredecessorNode(parentParentNode)) {
				//				if (statistics && parentParentNode != mergeParentDirection) {
				//					statistics->incDerivatedTestedSubsumptionCount(thisPrevEqConCount*parentParentNode->getEquivalentConceptCount());
				//				}
				//			}
				//			bool oldParPredecessor = false;
				//			foreach (CHierarchyNode* oldParNode, parentNodeSet) {
				//				if (oldParNode == parentParentNode && oldParNode->hasPredecessorNode(parentParentNode)) {
				//					oldParPredecessor = true;
				//				}
				//			}
				//			if (!oldParPredecessor) {
				//				makeParent(parentParentNode);
				//				if (!parentMergeNodesSet.contains(parentParentNode)) {
				//					parentMergeNodesSet.insert(parentParentNode);
				//					parentMergeNodesList.append(parentParentNode);
				//				}
				//			}
				//		}

				//		QList<CHierarchyNode *> childList(parentNode->getChildNodeList());
				//		foreach (CHierarchyNode *parentChildNode, childList) {
				//			parentChildNode->removeParent(parentNode);
				//			parentNode->removeChild(parentChildNode);
				//			if (parentNodeSet.isEmpty()) {
				//				bool bug = true;
				//			}
				//			if (parentChildNode != this) {
				//				if (!hasSuccessorNode(parentChildNode)) {
				//					if (statistics) {
				//						statistics->incDerivatedTestedSubsumptionCount(thisPrevEqConCount*parentChildNode->getEquivalentConceptCount());
				//					}
				//					makeChild(parentChildNode);
				//				}
				//			}
				//		}

				//		QList<CHierarchyNode *> predList(parentNode->getPredecessorNodeList());
				//		foreach (CHierarchyNode *parentPredeccessorNode, predList) {
				//			parentPredeccessorNode->removeSuccessor(parentNode);
				//			parentNode->removePredecessor(parentPredeccessorNode);
				//			if (!hasPredecessorNode(parentPredeccessorNode)) {
				//				if (statistics && parentPredeccessorNode != mergeParentDirection) {
				//					statistics->incDerivatedTestedSubsumptionCount(thisPrevEqConCount*parentPredeccessorNode->getEquivalentConceptCount());
				//				}
				//				makePredecessor(parentPredeccessorNode);
				//			}
				//		}

				//		QSet<CHierarchyNode *> *disjointSet(parentNode->getDisjointNodeSet());
				//		foreach (CHierarchyNode *disjointNode, *disjointSet) {
				//			disjointNode->removeDisjointNode(parentNode);
				//			parentNode->removeDisjointNode(disjointNode);
				//			if (!hasDisjointNode(disjointNode)) {
				//				makeDisjoint(disjointNode);
				//				if (!hasIncompatiblePredecessorNode(disjointNode)) {
				//					if (statistics) {
				//						statistics->incPrunedTestedSubsumptionCount(thisPrevEqConCount*disjointNode->getEquivalentConceptCount());
				//					}
				//					addIncompatiblePredecessorNode(disjointNode);
				//					disjointNode->addIncompatibleSuccessorNode(this);
				//				}
				//				if (!hasIncompatibleSuccessorNode(disjointNode)) {
				//					if (statistics) {
				//						statistics->incPrunedTestedSubsumptionCount(thisPrevEqConCount*disjointNode->getEquivalentConceptCount());
				//					}
				//					addIncompatibleSuccessorNode(disjointNode);
				//					disjointNode->addIncompatiblePredecessorNode(this);
				//				}
				//			}
				//		}

				//		QList<CHierarchyNode *> succList(parentNode->getSuccessorNodeList());
				//		foreach (CHierarchyNode *parentSuccessorNode, succList) {
				//			parentSuccessorNode->removePredecessor(parentNode);
				//			parentNode->removeSuccessor(parentSuccessorNode);
				//			if (parentSuccessorNode != this) {
				//				if (!hasSuccessorNode(parentSuccessorNode)) {
				//					if (statistics) {
				//						statistics->incDerivatedTestedSubsumptionCount(thisPrevEqConCount*parentSuccessorNode->getEquivalentConceptCount());
				//					}
				//					makeSuccessor(parentSuccessorNode);
				//				}
				//			}
				//		}

				//		QSet<CHierarchyNode *> *incPredList(parentNode->getIncompatiblePredecessorNodeSet());
				//		foreach (CHierarchyNode *parentIncompatiblePredNode, *incPredList) {
				//			parentIncompatiblePredNode->removeIncompatibleSuccessorNode(parentNode);
				//			parentNode->removeIncompatiblePredecessorNode(parentIncompatiblePredNode);
				//			if (!hasIncompatiblePredecessorNode(parentIncompatiblePredNode)) {
				//				if (statistics) {
				//					statistics->incPrunedTestedSubsumptionCount(thisPrevEqConCount*parentIncompatiblePredNode->getEquivalentConceptCount());
				//				}
				//				addIncompatiblePredecessorNode(parentIncompatiblePredNode);
				//				parentIncompatiblePredNode->addIncompatibleSuccessorNode(this);
				//			}
				//		}
				//		QSet<CHierarchyNode *> *incSuccList(parentNode->getIncompatibleSuccessorNodeSet());
				//		foreach (CHierarchyNode *parentIncompatibleSuccNode, *incSuccList) {
				//			parentIncompatibleSuccNode->removeIncompatiblePredecessorNode(parentNode);
				//			parentNode->removeIncompatibleSuccessorNode(parentIncompatibleSuccNode);
				//			if (!hasIncompatibleSuccessorNode(parentIncompatibleSuccNode)) {
				//				if (statistics) {
				//					statistics->incPrunedTestedSubsumptionCount(thisPrevEqConCount*parentIncompatibleSuccNode->getEquivalentConceptCount());
				//				}
				//				addIncompatibleSuccessorNode(parentIncompatibleSuccNode);
				//				parentIncompatibleSuccNode->addIncompatiblePredecessorNode(this);
				//			}
				//		}


				//		foreach (CHierarchyNode* parNode1, parentNodeSet) {
				//			foreach (CHierarchyNode* parNode2, parentNodeSet) {
				//				if (parNode1 != parNode2 && parNode2->hasPredecessorNode(parNode1)) {
				//					removeParent(parNode1);
				//					parNode1->removeChild(this);

				//					if (parentNodeSet.isEmpty()) {
				//						bool bug = true;
				//					}

				//				}
				//			}
				//		}

				//		foreach (CHierarchyNode* childNode1, childNodeSet) {
				//			foreach (CHierarchyNode* childNode2, childNodeSet) {
				//				if (childNode1 != childNode2 && childNode1->hasPredecessorNode(childNode2)) {
				//					removeChild(childNode1);
				//					childNode1->removeParent(this);

				//					if (childNode1->getParentNodeSet()->isEmpty()) {
				//						bool bug = true;
				//					}

				//				}
				//			}
				//		}


				//	} else {
				//		//repairParentNodeInsertion(parentNode,statistics);

				//		//((CPartialPruningHierarchyNode *)parentNode)->repairChildsParentNodeReplacement(this,statistics);

				//		if (parentNodeSet.isEmpty()) {
				//			bool bug = true;
				//		}

				//	}
				//}
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CPartialPruningHierarchyNode::propagateTransitiveRelations(CClassifierStatistics *statistics) {
				CHierarchyNode *updateNode = this;
				QSet<CHierarchyNode *> *updateChildNodeSet = getChildNodeSet();
				QSet<CHierarchyNode *> *updateParentNodeSet = getParentNodeSet();
				QSet<CHierarchyNode *> *updatePredNodeSet = getPredecessorNodeSet();
				QSet<CHierarchyNode *> *updateSuccNodeSet = getSuccessorNodeSet();
				QSet<CHierarchyNode *> *updateDisjointSet = getDisjointNodeSet();

				for (QSet<CHierarchyNode *>::const_iterator itPred = updatePredNodeSet->constBegin(), itPredEnd = updatePredNodeSet->constEnd(); itPred != itPredEnd; ++itPred) {
					CHierarchyNode *predNode = (*itPred);
					QList<CHierarchyNode *> updateList;
					// add missing predecessor nodes to children
					updateList += getChildNodeList();
					while (!updateList.isEmpty()) {
						CHierarchyNode *updateNode = updateList.takeFirst();
						if (!updateNode->hasPredecessorNode(predNode)) {
							if (statistics) {
								statistics->incDerivatedTestedSubsumptionCount(updateNode->getEquivalentConceptCount()*predNode->getEquivalentConceptCount());
							}
							updateNode->makePredecessor(predNode);
							updateList += updateNode->getChildNodeList();
						}
					}
				}


				for (QSet<CHierarchyNode *>::const_iterator itDisj = updateDisjointSet->constBegin(), itDisjEnd = updateDisjointSet->constEnd(); itDisj != itDisjEnd; ++itDisj) {
					CHierarchyNode *disjNode = (*itDisj);
					QList<CHierarchyNode *> updateList;
					// add missing disjoint nodes to children
					updateList += getChildNodeList();
					while (!updateList.isEmpty()) {
						CHierarchyNode *updateNode = updateList.takeFirst();
						if (!updateNode->hasDisjointNode(disjNode)) {
							updateNode->makeDisjoint(disjNode);

							if (!updateNode->hasIncompatiblePredecessorNode(disjNode)) {
								if (statistics) {
									statistics->incPrunedTestedSubsumptionCount(updateNode->getEquivalentConceptCount()*disjNode->getEquivalentConceptCount());
								}
								updateNode->makeIncompatiblePredecessor(disjNode);
							}
							if (!updateNode->hasIncompatibleSuccessorNode(disjNode)) {
								if (statistics) {
									statistics->incPrunedTestedSubsumptionCount(updateNode->getEquivalentConceptCount()*disjNode->getEquivalentConceptCount());
								}
								updateNode->makeIncompatibleSuccessor(disjNode);
							}
							updateList += updateNode->getChildNodeList();
						}
					}
				}


				for (QSet<CHierarchyNode *>::const_iterator itSucc = updateSuccNodeSet->constBegin(), itSuccEnd = updateSuccNodeSet->constEnd(); itSucc != itSuccEnd; ++itSucc) {
					CHierarchyNode *succNode = (*itSucc);
					QList<CHierarchyNode *> updateList;
					// add missing successor nodes to parents
					updateList += getParentNodeList();
					while (!updateList.isEmpty()) {
						CHierarchyNode *updateNode = updateList.takeFirst();
						if (!updateNode->hasSuccessorNode(succNode)) {
							if (statistics) {
								statistics->incDerivatedTestedSubsumptionCount(updateNode->getEquivalentConceptCount()*succNode->getEquivalentConceptCount());
							}
							updateNode->makeSuccessor(succNode);
							updateList += updateNode->getParentNodeList();
						}
					}
				}

				QSet<CHierarchyNode *> *incompSuccNodeSet = getIncompatibleSuccessorNodeSet();
				for (QSet<CHierarchyNode *>::const_iterator itIncomp = incompSuccNodeSet->constBegin(), itIncompEnd = incompSuccNodeSet->constEnd(); itIncomp != itIncompEnd; ++itIncomp) {
					CHierarchyNode *incompNode = (*itIncomp);
					QList<CHierarchyNode *> updateList;
					// propagate incompatible successor node relations from current node to all children
					updateList += getChildNodeList();
					while (!updateList.isEmpty()) {
						CHierarchyNode *updateNode = updateList.takeFirst();
						if (!updateNode->hasIncompatibleSuccessorNode(incompNode)) {
							if (statistics) {
								statistics->incPrunedTestedSubsumptionCount(updateNode->getEquivalentConceptCount()*incompNode->getEquivalentConceptCount());
							}
							updateNode->addIncompatibleSuccessorNode(incompNode);
							incompNode->addIncompatiblePredecessorNode(updateNode);
							QSet<CHierarchyNode *> *updateChildNodeSet = updateNode->getChildNodeSet();
							for (QSet<CHierarchyNode *>::const_iterator itUpChild = updateChildNodeSet->constBegin(), itUpChildEnd = updateChildNodeSet->constEnd(); itUpChild != itUpChildEnd; ++itUpChild) {
								CHierarchyNode *updateChildNode = (*itUpChild);
								updateList.append(updateChildNode);
							}
						}
					}
				}


				QSet<CHierarchyNode *> *incompPredNodeSet = getIncompatiblePredecessorNodeSet();
				for (QSet<CHierarchyNode *>::const_iterator itIncomp = incompPredNodeSet->constBegin(), itIncompEnd = incompPredNodeSet->constEnd(); itIncomp != itIncompEnd; ++itIncomp) {
					CHierarchyNode *incompNode = (*itIncomp);
					QList<CHierarchyNode *> updateList;
					// propagate incompatible predecessor node relations from current node to all parents
					updateList += getParentNodeList();
					while (!updateList.isEmpty()) {
						CHierarchyNode *updateNode = updateList.takeFirst();
						if (!updateNode->hasIncompatiblePredecessorNode(incompNode)) {
							if (statistics) {
								statistics->incPrunedTestedSubsumptionCount(updateNode->getEquivalentConceptCount()*incompNode->getEquivalentConceptCount());
							}
							updateNode->addIncompatiblePredecessorNode(incompNode);
							incompNode->addIncompatibleSuccessorNode(updateNode);
							QSet<CHierarchyNode *> *updateParentNodeSet = updateNode->getParentNodeSet();
							for (QSet<CHierarchyNode *>::const_iterator itUpParent = updateParentNodeSet->constBegin(), itUpParentEnd = updateParentNodeSet->constEnd(); itUpParent != itUpParentEnd; ++itUpParent) {
								CHierarchyNode *updateParentNode = (*itUpParent);
								updateList.append(updateParentNode);
							}
						}
					}
				}


				//// derivate new incompatible equivalences to parent nodes
				//for (QSet<CHierarchyNode *>::const_iterator itParent = updateParentNodeSet->constBegin(), itParentEnd = updateParentNodeSet->constEnd(); itParent != itParentEnd; ++itParent) {
				//	CHierarchyNode *parentNode = (*itParent);
				//	if (!hasIncompatibleSuccessorNode(parentNode)) {
				//		for (QSet<CHierarchyNode *>::const_iterator itUpChild = parentNode->getChildNodeSet()->constBegin(), itUpChildEnd = parentNode->getChildNodeSet()->constEnd(); itUpChild != itUpChildEnd; ++itUpChild) {
				//			CHierarchyNode *parentChildNode = (*itUpChild);
				//			if (hasIncompatibleSuccessorNode(parentChildNode) || hasIncompatiblePredecessorNode(parentChildNode)) {
				//				if (statistics) {
				//					statistics->incDerivatedTestedSubsumptionCount(getEquivalentConceptCount()*parentNode->getEquivalentConceptCount());
				//				}
				//				makeIncompatiblePredecessor(parentNode);
				//			}
				//		}
				//	}
				//}


#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}



			CHierarchyNode *CPartialPruningHierarchyNode::repairChildNodes(CClassifierStatistics *statistics) {
				foreach (CHierarchyNode* childNode1, childNodeSet) {
					foreach (CHierarchyNode* childNode2, childNodeSet) {
						if (childNode1 != childNode2 && childNode1->hasPredecessorNode(childNode2)) {
							removeChild(childNode1);
							childNode1->removeParent(this);
						}
					}
				}
				return this;
			}


			CHierarchyNode *CPartialPruningHierarchyNode::repairParentNodes(CClassifierStatistics *statistics) {
				foreach (CHierarchyNode* parNode1, parentNodeSet) {
					foreach (CHierarchyNode* parNode2, parentNodeSet) {
						if (parNode1 != parNode2 && parNode2->hasPredecessorNode(parNode1)) {
							removeParent(parNode1);
							parNode1->removeChild(this);
						}
					}
				}
				return this;
			}



			CHierarchyNode *CPartialPruningHierarchyNode::propagateTransitivePredecessorRelation(CHierarchyNode *transAddPredNode, CClassifierStatistics *statistics) {
				QList<CHierarchyNode *> updateList;
				updateList.append(this);
				while (!updateList.isEmpty()) {
					CHierarchyNode *updateNode = updateList.takeFirst();
					QSet<CHierarchyNode *> *updateChildNodeSet = updateNode->getChildNodeSet();
					QSet<CHierarchyNode *> *updatePredNodeSet = updateNode->getPredecessorNodeSet();
					for (QSet<CHierarchyNode *>::const_iterator itChild = updateChildNodeSet->constBegin(), itChildEnd = updateChildNodeSet->constEnd(); itChild != itChildEnd; ++itChild) {
						CHierarchyNode *childNode = (*itChild);
						if (!childNode->hasPredecessorNode(transAddPredNode)) {
							if (statistics) {
								statistics->incDerivatedTestedSubsumptionCount(childNode->getEquivalentConceptCount()*transAddPredNode->getEquivalentConceptCount());
							}
							childNode->makePredecessor(transAddPredNode);
							updateList.append(childNode);
						}
					}
				}
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}



			CHierarchyNode *CPartialPruningHierarchyNode::pruneTransitiveRelations(CHierarchyNode *initIncompNode, CClassifierStatistics *statistics) {
				for (QSet<CHierarchyNode *>::const_iterator parentIt = initIncompNode->getParentNodeSet()->constBegin(), parentItEnd = initIncompNode->getParentNodeSet()->constEnd(); parentIt != parentItEnd; ++parentIt) {
					CHierarchyNode *incompNodeParent = (*parentIt);
					if (!incompNodeParent->hasIncompatiblePredecessorNode(initIncompNode)) {
						if (hasPredecessorNode(incompNodeParent)) {
							if (statistics) {
								statistics->incPrunedTestedSubsumptionCount(incompNodeParent->getEquivalentConceptCount()*initIncompNode->getEquivalentConceptCount());
							}
							incompNodeParent->makeIncompatiblePredecessor(initIncompNode);
						}
					}
				}
				QList<CHierarchyNode *> incompChildNodeList;
				QSet<CHierarchyNode *> incompNodeSet;
				incompChildNodeList.append(initIncompNode);
				incompNodeSet.insert(initIncompNode);
				while (!incompChildNodeList.isEmpty()) {
					CHierarchyNode *nextIncompNode = incompChildNodeList.takeFirst();
					QSet<CHierarchyNode *> *incNodeSet = nextIncompNode->getChildNodeSet();
					for (QSet<CHierarchyNode *>::const_iterator childIt = incNodeSet->constBegin(), childItEnd = incNodeSet->constEnd(); childIt != childItEnd; ++childIt) {
						CHierarchyNode *childIncompNode = (*childIt);
						if (!hasIncompatiblePredecessorNode(childIncompNode)) {
							if (!incompNodeSet.contains(childIncompNode)) {
								incompNodeSet.insert(childIncompNode);
								incompChildNodeList.append(childIncompNode);
							}
						}
					}
				}
				QList<CHierarchyNode *> predList;
				for (QSet<CHierarchyNode *>::const_iterator incompIt = incompNodeSet.constBegin(), incompItEnd = incompNodeSet.constEnd(); incompIt != incompItEnd; ++incompIt) {
					CHierarchyNode *incompNode = (*incompIt);
					// set incompatible to all predecessors
					predList.append(this);
					while (!predList.isEmpty()) {
						CHierarchyNode *predNode = predList.takeFirst();
						if (!predNode->hasIncompatiblePredecessorNode(incompNode)) {
							if (statistics) {
								statistics->incPrunedTestedSubsumptionCount(predNode->getEquivalentConceptCount()*incompNode->getEquivalentConceptCount());
							}
							predNode->addIncompatiblePredecessorNode(incompNode);
							incompNode->addIncompatibleSuccessorNode(predNode);
							QSet<CHierarchyNode *> *parentNodeSet = predNode->getParentNodeSet();
							for (QSet<CHierarchyNode *>::const_iterator parentIt = parentNodeSet->constBegin(), parentItEnd = parentNodeSet->constEnd(); parentIt != parentItEnd; ++parentIt) {
								CHierarchyNode *parentNode = (*parentIt);
								predList.append(parentNode);
							}
						}
					}
				}

#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
