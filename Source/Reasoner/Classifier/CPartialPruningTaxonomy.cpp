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

#include "CPartialPruningTaxonomy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {



			CPartialPruningTaxonomy::CPartialPruningTaxonomy(CConcept *topConcept, CConcept *bottomConcept) : CTaxonomy(new CPartialPruningHierarchyNode(topConcept),new CPartialPruningHierarchyNode(bottomConcept)) {
				activeNodeSet.insert(bottomNode);
				confPruneRelations = true;
				confDerivateRelations = true;
				statistics = 0;
				testCompStatistics = 0;
			}


			CPartialPruningTaxonomy::~CPartialPruningTaxonomy() {
				if (statistics) {
					delete statistics;
				}
			}

			CPartialPruningTaxonomy *CPartialPruningTaxonomy::createStatistics(CClassifierStatistics *nextClassificationStatistics) {
				testCompStatistics = nextClassificationStatistics;
				if (!statistics) {
					statistics = new CClassifierStatistics(nextClassificationStatistics);
				}
				return this;
			}

			CPartialPruningTaxonomy *CPartialPruningTaxonomy::readConfig(CConfigurationBase *config) {
				confPruneRelations = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Classification.PruneSubsumptionRelations",false);
				confDerivateRelations = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Classification.DerivateSubsumptionRelations",false);
				return this;
			}


			CTaxonomy *CPartialPruningTaxonomy::setConceptSatisfiable(CConcept *concept, bool satisfiable) {
				if (satisfiable) {
					CHierarchyNode *conNode = getHierarchyNode(concept);
					if (conNode != topNode && !conNode->hasParentNode(topNode)) {
						activeNodeSet.insert(conNode);
						topNode->makeChild(conNode);
						conNode->makeChild(bottomNode);
						((CPartialPruningHierarchyNode *)topNode)->repairChildNodes(statistics);
						((CPartialPruningHierarchyNode *)conNode)->repairParentNodes(statistics);
						//((CPartialPruningHierarchyNode *)topNode)->repairChildsParentNodeReplacement(conNode,statistics);
						//((CPartialPruningHierarchyNode *)conNode)->repairParentNodeInsertion(topNode,statistics);
						conNode->addIncompatiblePredecessorNode(bottomNode);
						bottomNode->addIncompatibleSuccessorNode(conNode);
						if (statistics) {
							// inc statistics to top concepts
							statistics->incDerivatedTestedSubsumptionCount(topNode->getEquivalentConceptCount()*conNode->getEquivalentConceptCount());
							// inc statistics to bottom concepts
							statistics->incDerivatedTestedSubsumptionCount(bottomNode->getEquivalentConceptCount()*conNode->getEquivalentConceptCount());
							statistics->incPrunedTestedSubsumptionCount(bottomNode->getEquivalentConceptCount()*conNode->getEquivalentConceptCount());
						}
						if (confDerivateRelations) {
							((CPartialPruningHierarchyNode *)conNode)->propagateTransitiveRelations(statistics);
						}
					} else if (conNode == topNode) {
						activeNodeSet.insert(topNode);
						topNode->addIncompatiblePredecessorNode(bottomNode);
						bottomNode->addIncompatibleSuccessorNode(topNode);
						if (statistics) {
							statistics->incDerivatedTestedSubsumptionCount(bottomNode->getEquivalentConceptCount()*topNode->getEquivalentConceptCount());
							statistics->incPrunedTestedSubsumptionCount(bottomNode->getEquivalentConceptCount()*topNode->getEquivalentConceptCount());
						}
						if (confDerivateRelations) {
							((CPartialPruningHierarchyNode *)conNode)->propagateTransitiveRelations(statistics);
						}
					}
				} else {
					CHierarchyNode *conNode = getHierarchyNode(concept,false);
					activeNodeSet.remove(conNode);
					if (conNode == topNode) {
						topNode = bottomNode;
					}
					bottomNode->addEquivalentConcept(concept);
					conNodeHash.insert(concept,bottomNode);
				}
#ifdef DEBUGTESTTAXONOMYCONSITENCE
				testTaxonomyConsistence();
#endif
				return this;
			}



			CTaxonomy *CPartialPruningTaxonomy::setConceptSubsumption(CConcept *subsumerConcept, CConcept *subsumedConcept) {
				CHierarchyNode *subsumedNode = getHierarchyNode(subsumedConcept);
				CHierarchyNode *subsumerNode = getHierarchyNode(subsumerConcept);
				if (subsumedNode != subsumerNode && !subsumedNode->hasSuccessorNode(subsumerNode)) {
					bool makeEquivalent = subsumerNode->hasSuccessorNode(subsumedNode);
					if (makeEquivalent) {
						//taxString = getConceptHierarchyString();

						((CPartialPruningHierarchyNode *)subsumedNode)->mergeToParentNode(subsumerNode,statistics);
						QList<CConcept *> *eqList = subsumedNode->getEquivalentConceptList();
						foreach (CConcept *concept, *eqList) {
							CHierarchyNode *prevNode = conNodeHash.value(concept);
							if (prevNode != subsumedNode) {
								setInactiveNode(prevNode);
								conNodeHash.insert(concept,subsumedNode);
							}
						}

						//QString taxString2 = getConceptHierarchyString();

#ifdef DEBUGTESTTAXONOMYCONSITENCE
						testTaxonomyConsistence();
#endif
					} else {
						subsumedNode->makeChild(subsumerNode);
						// for all common predecessors and successors repair quasi ordering relation
						((CPartialPruningHierarchyNode *)subsumedNode)->repairChildNodes(statistics);
						((CPartialPruningHierarchyNode *)subsumedNode)->repairParentNodes(statistics);
						((CPartialPruningHierarchyNode *)subsumerNode)->repairParentNodes(statistics);
						((CPartialPruningHierarchyNode *)subsumerNode)->repairChildNodes(statistics);
						//((CPartialPruningHierarchyNode *)subsumedNode)->repairChildsParentNodeReplacement(subsumerNode,statistics);
						//((CPartialPruningHierarchyNode *)subsumerNode)->repairParentNodeInsertion(subsumedNode,statistics);
					}
					if (confDerivateRelations) {
						((CPartialPruningHierarchyNode *)subsumerNode)->propagateTransitiveRelations(statistics);
						((CPartialPruningHierarchyNode *)subsumedNode)->propagateTransitiveRelations(statistics);
					}
				}
#ifdef DEBUGTESTTAXONOMYCONSITENCE
				testTaxonomyConsistence();
#endif
				return this;
			}


			CTaxonomy *CPartialPruningTaxonomy::setConceptNotSubsumption(CConcept *notSubsumerConcept, CConcept *notSubsumedConcept) {
				CHierarchyNode *notSubsumedNode = getHierarchyNode(notSubsumedConcept);
				CHierarchyNode *notSubsumerNode = getHierarchyNode(notSubsumerConcept);

				if (statistics) {
					statistics->incPrunedTestedSubsumptionCount(-notSubsumedNode->getEquivalentConceptCount()*notSubsumerNode->getEquivalentConceptCount());
				}

				if (confPruneRelations) {
					((CPartialPruningHierarchyNode *)notSubsumerNode)->pruneTransitiveRelations(notSubsumedNode,statistics);
				}

				if (confDerivateRelations) {
					((CPartialPruningHierarchyNode *)notSubsumerNode)->propagateTransitiveRelations(statistics);
					((CPartialPruningHierarchyNode *)notSubsumedNode)->propagateTransitiveRelations(statistics);
				}

#ifdef DEBUGTESTTAXONOMYCONSITENCE
				testTaxonomyConsistence();
#endif

				return this;
			}


			CTaxonomy *CPartialPruningTaxonomy::setConceptDisjoints(CConcept *disjointConcept1, CConcept *disjointConcept2) {
				CHierarchyNode *disjNode1 = getHierarchyNode(disjointConcept1);
				CHierarchyNode *disjNode2 = getHierarchyNode(disjointConcept2);


				if (!disjNode1->hasIncompatiblePredecessorNode(disjNode2)) {
					if (confPruneRelations) {
						((CPartialPruningHierarchyNode *)disjNode1)->pruneTransitiveRelations(disjNode2,statistics);
					}
					if (confDerivateRelations) {
						((CPartialPruningHierarchyNode *)disjNode1)->propagateTransitiveRelations(statistics);
						((CPartialPruningHierarchyNode *)disjNode2)->propagateTransitiveRelations(statistics);
					}

				}
				if (!disjNode2->hasIncompatiblePredecessorNode(disjNode1)) {
					if (confPruneRelations) {
						((CPartialPruningHierarchyNode *)disjNode2)->pruneTransitiveRelations(disjNode1,statistics);
					}
					if (confDerivateRelations) {
						((CPartialPruningHierarchyNode *)disjNode2)->propagateTransitiveRelations(statistics);
						((CPartialPruningHierarchyNode *)disjNode1)->propagateTransitiveRelations(statistics);
					}
				}

				disjNode2->makeDisjoint(disjNode1);
				if (confDerivateRelations) {
					((CPartialPruningHierarchyNode *)disjNode2)->propagateTransitiveRelations(statistics);
					((CPartialPruningHierarchyNode *)disjNode1)->propagateTransitiveRelations(statistics);
				}

#ifdef DEBUGTESTTAXONOMYCONSITENCE
				testTaxonomyConsistence();
#endif

				return this;
			}



			CHierarchyNode *CPartialPruningTaxonomy::createHierarchyNode(CConcept *concept) {
				return new CPartialPruningHierarchyNode(concept);
			}





			void CPartialPruningTaxonomy::testTaxonomyConsistence() {
//#ifdef DEBUGTESTTAXONOMYCONSITENCE
//				taxString = getConceptHierarchyString();
//#endif
				//for (QList<CHierarchyNode *>::iterator it = nodeContainer.begin(); it != nodeContainer.end(); ++it) {
				//	CHierarchyNode* node = *it;
				//	if (node->getParentNodeSet()->isEmpty() && node->isActive() && node->getOneEquivalentConcept()->getConceptTag() != 0 && node->getOneEquivalentConcept()->getConceptTag() != 1) {
				//		bool bug = true;
				//	}
				//}

				//for (QHash<CConcept *,CHierarchyNode *>::iterator conIt = conNodeHash.begin(), conItEnd = conNodeHash.end(); conIt != conItEnd; ++conIt) {
				//	CConcept* con = conIt.key();
				//	cint count = 0;
				//	CHierarchyNode* firstNode = nullptr;
				//	for (QList<CHierarchyNode *>::iterator it = nodeContainer.begin(); it != nodeContainer.end(); ++it) {
				//		CHierarchyNode* node = *it;
				//		if (node->isActive() && node->getEquivalentConceptList()->count(con) > 0) {
				//			count += node->getEquivalentConceptList()->count(con);
				//			if (count > 1) {
				//				bool bug = true;
				//			}
				//			firstNode = node;
				//		}
				//	}
				//}


				//if (testCompStatistics) {
				//	testSubsumptionStatistics(testCompStatistics->getTestedSubsumptionCount());
				//}

				//for (QHash<CConcept *,CHierarchyNode *>::const_iterator it = conNodeHash.constBegin(), itEnd = conNodeHash.constEnd(); it != itEnd; ++it) {
				//	CHierarchyNode *node = *it;

					//if (node->getEquivalentConceptsNodeString() == "#Existential" || node->getEquivalentConceptsNodeString() == "#EliminateExistential2") {
					//	foreach (CHierarchyNode *otherNode, *node->getIncompatiblePredecessorNodeSet()) {
					//		if (otherNode->getEquivalentConceptsNodeString() == "#Existential" || otherNode->getEquivalentConceptsNodeString() == "#EliminateExistential2") {
					//			bool bug = true;
					//		}
					//	}
					//	foreach (CHierarchyNode *otherNode, *node->getSuccessorNodeSet()) {
					//		if (otherNode->getEquivalentConceptsNodeString() == "#Existential" || otherNode->getEquivalentConceptsNodeString() == "#EliminateExistential2") {
					//			bool bug = true;
					//		}
					//	}
					//}

				//	if (isActiveNode(node)) {
				//		bool consistence = true;
				//		foreach (CHierarchyNode *otherNode, node->getPredecessorNodeList()) {
				//			if (isActiveNode(otherNode) && node != bottomNode) {
				//				if (node->hasIncompatiblePredecessorNode(otherNode)) {
				//					consistence = false;
				//				}
				//			}
				//		}
				//		foreach (CHierarchyNode *otherNode, node->getSuccessorNodeList()) {
				//			if (otherNode != bottomNode && node->hasIncompatibleSuccessorNode(otherNode) && isActiveNode(otherNode)) {
				//				consistence = false;
				//			}
				//		}
				//		foreach (CHierarchyNode *otherNode, node->getChildNodeList()) {
				//			if (!otherNode->hasParentNode(node) && isActiveNode(otherNode)) {
				//				consistence = false;
				//			}
				//			if (!otherNode->hasPredecessorNode(node) && isActiveNode(otherNode)) {
				//				consistence = false;
				//			}
				//		}
				//		foreach (CHierarchyNode *otherNode, node->getParentNodeList()) {
				//			if (!otherNode->hasChildNode(node) && isActiveNode(otherNode)) {
				//				consistence = false;
				//			}
				//			if (!otherNode->hasSuccessorNode(node) && isActiveNode(otherNode)) {
				//				consistence = false;
				//			}
				//		}
				//		foreach (CHierarchyNode *otherNode, node->getPredecessorNodeList()) {
				//			if (!otherNode->hasSuccessorNode(node) && isActiveNode(otherNode)) {
				//				consistence = false;
				//			}
				//		}
				//		foreach (CHierarchyNode *otherNode, node->getSuccessorNodeList()) {
				//			if (!otherNode->hasPredecessorNode(node) && isActiveNode(otherNode)) {
				//				consistence = false;
				//			}
				//		}
				//		foreach (CHierarchyNode *otherNode, *node->getIncompatiblePredecessorNodeSet()) {
				//			if (!otherNode->hasIncompatibleSuccessorNode(node) && isActiveNode(otherNode) && node != bottomNode) {
				//				consistence = false;
				//			}
				//		}
				//		foreach (CHierarchyNode *otherNode, *node->getIncompatibleSuccessorNodeSet()) {
				//			if (!otherNode->hasIncompatiblePredecessorNode(node) && isActiveNode(otherNode)) {
				//				consistence = false;
				//			}
				//		}
				//		if (!consistence) {
				//			LOG(ERROR,"::Konclude::Taxonomy::PartialPruningTaxonomy",logTr("Taxonomy inconsistence detected."),this);

				//		}
				//	}
				//}
			}



			void CPartialPruningTaxonomy::testSubsumptionStatistics(qint64 subsumCount) {
				qint64 testSubSumCount = 0;

				QSet<CHierarchyNode *> checkedSet;

				for (QHash<CConcept *,CHierarchyNode *>::const_iterator it = conNodeHash.constBegin(), itEnd = conNodeHash.constEnd(); it != itEnd; ++it) {
					CHierarchyNode *node = *it;
					qint64 thisNodeSubSumCount = 0;
					if (isActiveNode(node) && !checkedSet.contains(node)) {
						checkedSet.insert(node);
						if (node->getEquivalentConceptCount() > 1) {
							thisNodeSubSumCount += node->getEquivalentConceptCount();
						}
						foreach (CHierarchyNode *otherNode, node->getPredecessorNodeList()) {
							if (isActiveNode(otherNode)) {
								thisNodeSubSumCount += node->getEquivalentConceptCount() * otherNode->getEquivalentConceptCount();
							}
						}
						foreach (CHierarchyNode *otherNode, *node->getIncompatiblePredecessorNodeSet()) {
							if (isActiveNode(otherNode)) {
								thisNodeSubSumCount += node->getEquivalentConceptCount() * otherNode->getEquivalentConceptCount();
							}
						}
					}
					testSubSumCount += thisNodeSubSumCount;
				}		
				if (subsumCount != testSubSumCount) {
					bool bug = true;
				}
			}



			CTaxonomy *CPartialPruningTaxonomy::completeUnknownAsNonSubsumptions() {
				foreach (CHierarchyNode *updateNode, bottomNode->getParentNodeList()) {
					QSet<CHierarchyNode *> *parentNodeSet = updateNode->getParentNodeSet();
					for (QSet<CHierarchyNode *>::const_iterator it1 = parentNodeSet->constBegin(), it1End = parentNodeSet->constEnd(); it1 != it1End; ++it1) {
						CHierarchyNode *parentNode(*it1);
						if (!updateNode->hasIncompatibleSuccessorNode(parentNode)) {
							setConceptNotSubsumption(parentNode->getOneEquivalentConcept(),updateNode->getOneEquivalentConcept());
						}
						QSet<CHierarchyNode *> *predNodeSet = updateNode->getPredecessorNodeSet();
						for (QSet<CHierarchyNode *>::const_iterator it1 = predNodeSet->constBegin(), it1End = predNodeSet->constEnd(); it1 != it1End; ++it1) {
							CHierarchyNode *predNode(*it1);
							QSet<CHierarchyNode *> *predChildNodeSet = predNode->getChildNodeSet();
							for (QSet<CHierarchyNode *>::const_iterator it2 = predChildNodeSet->constBegin(), it2End = predChildNodeSet->constEnd(); it2 != it2End; ++it2) {
								CHierarchyNode *predChildNode(*it2);
								for (QSet<CHierarchyNode *>::const_iterator it3 = predChildNodeSet->constBegin(), it3End = predChildNodeSet->constEnd(); it3 != it3End; ++it3) {
									CHierarchyNode *otherPredChildNode(*it3);
									if (predChildNode != otherPredChildNode) {
										if (!predChildNode->hasIncompatiblePredecessorNode(otherPredChildNode)) {
											setConceptNotSubsumption(predChildNode->getOneEquivalentConcept(),otherPredChildNode->getOneEquivalentConcept());
										}
									}
								}
								if (predChildNode != updateNode) {
									if (!updateNode->hasPredecessorNode(predChildNode) && !updateNode->hasIncompatiblePredecessorNode(predChildNode)) {
										setConceptNotSubsumption(updateNode->getOneEquivalentConcept(),predChildNode->getOneEquivalentConcept());
									}
									if (!predChildNode->hasIncompatibleSuccessorNode(predNode)) {
										setConceptNotSubsumption(predNode->getOneEquivalentConcept(),predChildNode->getOneEquivalentConcept());
									}
								}
							}
						}
					}
				}
				return this;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
