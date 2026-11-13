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

#include "CSaturationSubsumptionDecider.h"
#include "Reasoner/Ontology/CRoleChain.h"

#include "Reasoner/Consistiser/CSaturationConceptReferenceLinking.h"
#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNodeStatusFlags.h"
#include "Reasoner/Kernel/Process/CLinkedRoleSaturationSuccessorData.h"
#include "Reasoner/Kernel/Process/CSaturationSuccessorData.h"
#include "Reasoner/Ontology/CRBox.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CSaturationSubsumptionDecider::CSaturationSubsumptionDecider(CConcreteOntology* ontology) {
				mCompletedSuccessorDecision = true;
				mOntology = ontology;
				mSatCalcTask = nullptr;
				mSubsumedCount = 0;
				mNotSubsumedCount = 0;
				mUndecidedCount = 0;
				mFiredImplicationCount = 0;
				mFastDecisionCount = 0;
				mClosureDecisionCount = 0;
				mCompletionConcept = nullptr;
				mRootSubClassConcept = nullptr;
				mRootBaseNode = nullptr;
				mCompletionRequestCount = 0;
				mExistentialMemoQueryConcept = nullptr;
				mCompletedDecisionCount = 0;
				// the saturation of the ontology, as CPrecomputedSaturationSubsumerExtractor takes it
				CPrecomputation* precomputation = mOntology->getPrecomputation();
				if (precomputation) {
					CSaturationTaskData* saturationTaskData = (CSaturationTaskData*)precomputation->getSaturationModelData();
					if (saturationTaskData) {
						mSatCalcTask = saturationTaskData->getSaturationTask();
					}
				}
			}


			CSaturationSubsumptionDecider::~CSaturationSubsumptionDecider() {
				qDeleteAll(mConjunctionMergeBaseHash);
				qDeleteAll(mCompletedLabelHash);
			}


			CConcept* CSaturationSubsumptionDecider::getCompletionConcept() {
				return mCompletionConcept;
			}


			QList<CConcept*> CSaturationSubsumptionDecider::getHierarchyAncestors(CConcept* concept) {
				QList<CConcept*> ancestors;
				CClassification* classification = mOntology->getClassification();
				CClassConceptClassification* classClassification = classification ? classification->getClassConceptClassification() : nullptr;
				CTaxonomy* taxonomy = classClassification ? classClassification->getClassConceptTaxonomy() : nullptr;
				CHierarchyNode* node = taxonomy ? taxonomy->getHierarchyNode(concept, false) : nullptr;
				if (node) {
					for (CConcept* equivalent : *node->getEquivalentConceptList()) {
						if (equivalent != concept) {
							ancestors.append(equivalent);
						}
					}
					for (QSet<CHierarchyNode*>::const_iterator it = node->getPredecessorNodeSet()->constBegin(), itEnd = node->getPredecessorNodeSet()->constEnd(); it != itEnd; ++it) {
						CHierarchyNode* ancestorNode(*it);
						if (ancestorNode != node && ancestorNode != taxonomy->getTopHierarchyNode()) {
							for (CConcept* ancestor : *ancestorNode->getEquivalentConceptList()) {
								ancestors.append(ancestor);
							}
						}
					}
				}
				return ancestors;
			}


			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideNamedSubsumptionFromHierarchy(CConcept* subClassConcept, CConcept* superClassConcept) {
				CClassification* classification = mOntology->getClassification();
				CClassConceptClassification* classClassification = classification ? classification->getClassConceptClassification() : nullptr;
				CTaxonomy* taxonomy = classClassification ? classClassification->getClassConceptTaxonomy() : nullptr;
				if (!taxonomy) {
					return UNDECIDED;
				}
				CHierarchyNode* subNode = taxonomy->getHierarchyNode(subClassConcept, false);
				CHierarchyNode* superNode = taxonomy->getHierarchyNode(superClassConcept, false);
				if (!subNode || !superNode) {
					return UNDECIDED;
				}
				if (subNode == superNode || superNode == taxonomy->getTopHierarchyNode() || subNode->getPredecessorNodeSet()->contains(superNode)) {
					return SUBSUMED;
				}
				return NOT_SUBSUMED;
			}


			void CSaturationSubsumptionDecider::setCompletedLabel(CConcept* concept, const QVector<CCompletedLabelEntry>& entries) {
				CCompletedLabel* label = mCompletedLabelHash.value(concept, nullptr);
				if (!label) {
					label = new CCompletedLabel();
					mCompletedLabelHash.insert(concept, label);
				}
				label->mConcept = concept;
				label->mEntries = entries;
				label->mPolarityHash.clear();
				label->mSuccessorRoleList.clear();
				label->mSuccessorRoleTagSet.clear();
				label->mDataRoleTagSet.clear();
				label->mHasNondeterministic = false;
				label->mUnsatisfiable = false;
				label->mFailed = false;
				QSet<CRole*> roleSet;
				for (const CCompletedLabelEntry& entry : entries) {
					quint8 bits = entry.mNegated ? 2 : 1;
					if (entry.mDeterministic) {
						bits |= entry.mNegated ? 8 : 4;
					} else {
						label->mHasNondeterministic = true;
					}
					label->mPolarityHash[entry.mConcept] |= bits;
					cint64 opCode = entry.mConcept->getOperatorCode();
					cint64 absCode = opCode < 0 ? -opCode : opCode;
					bool existential = (absCode == 5 || absCode == 11) && ((opCode < 0) != entry.mNegated);
					if (existential || (!entry.mNegated && opCode == CCATLEAST)) {
						CRole* role = entry.mConcept->getRole();
						if (role && !roleSet.contains(role)) {
							roleSet.insert(role);
							if (role->isDataRole()) {
								collectRoleTagsWithSuperRoles(role, label->mDataRoleTagSet);
							} else {
								label->mSuccessorRoleList.append(role);
								collectRoleTagsWithSuperRoles(role, label->mSuccessorRoleTagSet);
							}
						}
					}
				}
			}


			void CSaturationSubsumptionDecider::setCompletedLabelUnsatisfiable(CConcept* concept) {
				CCompletedLabel* label = mCompletedLabelHash.value(concept, nullptr);
				if (!label) {
					label = new CCompletedLabel();
					mCompletedLabelHash.insert(concept, label);
				}
				label->mUnsatisfiable = true;
			}


			void CSaturationSubsumptionDecider::setCompletionFailed(CConcept* concept) {
				CCompletedLabel* label = mCompletedLabelHash.value(concept, nullptr);
				if (!label) {
					label = new CCompletedLabel();
					mCompletedLabelHash.insert(concept, label);
				}
				label->mFailed = true;
			}


			bool CSaturationSubsumptionDecider::hasCompletedLabel(CConcept* concept) {
				return mCompletedLabelHash.contains(concept);
			}


			void CSaturationSubsumptionDecider::collectRoleTagsWithSuperRoles(CRole* role, QSet<cint64>& tagSet) {
				tagSet.insert(role->getRoleTag());
				for (CSortedNegLinker<CRole*>* superIt = role->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
					if (!superIt->isNegated()) {
						tagSet.insert(superIt->getData()->getRoleTag());
					}
				}
			}


			bool CSaturationSubsumptionDecider::hasSuccessorRoleConflict(const QList<CRole*>& successorRoleList, const QSet<cint64>& dataRoleTagSet, const CMergeState& state) {
				// successors under a functional role on both sides would be merged, and what two parts
				// say about the values of a data role is left to the tableau
				for (CRole* role : successorRoleList) {
					if (role->isFunctional() && state.hasSuccessorRole(role)) {
						mUndecidedMergeConceptCodeCounts[-2000] = mUndecidedMergeConceptCodeCounts.value(-2000, 0) + 1;
						return true;
					}
					for (CSortedNegLinker<CRole*>* superIt = role->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
						if (!superIt->isNegated() && superIt->getData()->isFunctional() && state.hasSuccessorRole(superIt->getData())) {
							mUndecidedMergeConceptCodeCounts[-2000] = mUndecidedMergeConceptCodeCounts.value(-2000, 0) + 1;
							return true;
						}
					}
				}
				for (cint64 dataRoleTag : dataRoleTagSet) {
					if (state.hasDataRoleTag(dataRoleTag)) {
						mUndecidedMergeConceptCodeCounts[-2400] = mUndecidedMergeConceptCodeCounts.value(-2400, 0) + 1;
						return true;
					}
				}
				return false;
			}


			bool CSaturationSubsumptionDecider::hasSaturation() {
				return mSatCalcTask != nullptr;
			}


			cint64 CSaturationSubsumptionDecider::getSubsumedCount() {
				return mSubsumedCount;
			}

			cint64 CSaturationSubsumptionDecider::getNotSubsumedCount() {
				return mNotSubsumedCount;
			}

			cint64 CSaturationSubsumptionDecider::getUndecidedCount() {
				return mUndecidedCount;
			}

			QHash<cint64,cint64>* CSaturationSubsumptionDecider::getUndecidedMergeConceptCodeCounts() {
				return &mUndecidedMergeConceptCodeCounts;
			}

			cint64 CSaturationSubsumptionDecider::getFiredImplicationCount() {
				return mFiredImplicationCount;
			}

			cint64 CSaturationSubsumptionDecider::getCompletedLabelDecisiveCount() {
				cint64 count = 0;
				for (const CQueryCompletionStatistics& statistics : mQueryCompletionStatisticsHash) {
					if (statistics.mReliableDecisiveCount > 0) {
						count += statistics.mCompletedDecisiveCount;
					}
				}
				return count;
			}

			cint64 CSaturationSubsumptionDecider::getCompletedLabelUndecidedCount() {
				cint64 count = 0;
				for (const CQueryCompletionStatistics& statistics : mQueryCompletionStatisticsHash) {
					if (statistics.mReliableDecisiveCount > 0) {
						count += statistics.mCompletedUndecidedCount;
					}
				}
				return count;
			}

			cint64 CSaturationSubsumptionDecider::getFastDecisionCount() {
				return mFastDecisionCount;
			}

			cint64 CSaturationSubsumptionDecider::getClosureDecisionCount() {
				return mClosureDecisionCount;
			}



			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideSubClass(CConcept* subClassConcept, CConcept* queryConcept, bool queryNegation) {
				Verdict verdict = UNDECIDED;
				mCompletionConcept = nullptr;
				mRootSubClassConcept = subClassConcept;
				mRootBaseNode = nullptr;
				if (queryConcept != mExistentialMemoQueryConcept) {
					// the remembered decisions are about the fillers of one query
					mExistentialMemoHash.clear();
					mExistentialMemoQueryConcept = queryConcept;
				}
				if (mSatCalcTask && subClassConcept && queryConcept && mOntology->getDataBoxes()->getTBox()->getBottomConcept() == subClassConcept) {
					// the bottom node is a sub class candidate below every leaf of the search; it is subsumed by
					// everything, and it has no saturation of its own, so without this it would be sent to the
					// label completion, whose test conjoins the class with all its hierarchy ancestors: for the
					// bottom node that is every class of the ontology, a single tableau test of 20 minutes and
					// 7 GB on SNOMED CT International (248 186 ancestors) and a frozen Mac on the SDD (582 633)
					verdict = SUBSUMED;
				} else if (mSatCalcTask && subClassConcept && queryConcept) {
					bool unsatisfiable = false;
					bool reliable = false;
					CIndividualSaturationProcessNode* node = getSaturationNode(subClassConcept, &unsatisfiable, &reliable);
					mRootBaseNode = node;
					if (!node || (!unsatisfiable && !reliable)) {
						// no usable saturation: the class's completed label stands in, or has to be obtained first
						CCompletedLabel* label = mCompletedLabelHash.value(subClassConcept, nullptr);
						if (label) {
							if (label->mUnsatisfiable) {
								verdict = SUBSUMED;
							} else if (!label->mFailed) {
								verdict = decideEntailedCompleted(label, queryConcept, queryNegation, 0);
								CQueryCompletionStatistics& statistics = mQueryCompletionStatisticsHash[QPair<CConcept*,bool>(queryConcept, queryNegation)];
								if (verdict == SUBSUMED || verdict == NOT_SUBSUMED) {
									++statistics.mCompletedDecisiveCount;
								} else {
									++statistics.mCompletedUndecidedCount;
								}
							}
						} else {
							mCompletionConcept = subClassConcept;
							verdict = NEEDS_COMPLETION;
						}
						node = nullptr;
					}
					if (!node && verdict == UNDECIDED) {
						// the reasons besides label concepts are counted under negative keys, printed halved
						// by the handler: -100 no saturation node, -300 insufficient, -400 unprocessed, -500
						// critical, -600 not completed, -1100 cardinality or nominal flags, -1500 data value,
						// -1600 nominal integrated, -1700 no label, -1000 functional role, -2500 and -3000
						// universal restrictions reaching successors and self loops (-5300 those of a completed
						// label's successors satisfied by the saturations of its existential restrictions, -6510 one of those
						// restrictions' fillers without a saturation), -3500 derived successor,
						// and for the query side -4600 no node, -4700 unreliable, -4800 flags, -4900 data
						// value or nominal, -5000 no label; for an existential restriction of the query -5600 no
						// label, -5700 a successor with a range or a qualified or cardinality restriction,
						// -5800 a successor's filler undecided, -5900 the node unreliable, -6000 a role whose
						// successors the saturation may not materialise, -6100 some successor undecided, -6200
						// no saturation of a successor's filler
						mUndecidedMergeConceptCodeCounts[-200] = mUndecidedMergeConceptCodeCounts.value(-200, 0) + 1;
					}
					if (node) {
						if (unsatisfiable) {
							// an unsatisfiable class is subsumed by everything
							verdict = SUBSUMED;
						} else {
							verdict = decideEntailed(node, reliable, queryConcept, queryNegation, 0);
							if (verdict == SUBSUMED || verdict == NOT_SUBSUMED) {
								++mQueryCompletionStatisticsHash[QPair<CConcept*,bool>(queryConcept, queryNegation)].mReliableDecisiveCount;
							}
						}
					}
				}
				if (verdict == SUBSUMED) {
					++mSubsumedCount;
				} else if (verdict == NOT_SUBSUMED) {
					++mNotSubsumedCount;
				} else if (verdict == NEEDS_COMPLETION) {
					++mCompletionRequestCount;
				} else {
					++mUndecidedCount;
				}
				return verdict;
			}



			CIndividualSaturationProcessNode* CSaturationSubsumptionDecider::getSaturationNode(CConcept* concept, bool* unsatisfiable, bool* reliable, bool negated) {
				CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
				if (!conProData) {
					return nullptr;
				}
				CConceptSaturationReferenceLinkingData* conSatRefLinking = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
				if (!conSatRefLinking) {
					return nullptr;
				}
				// a negated concept has a saturation of its own if the saturation needed one, an existential
				// restriction for instance, which is stored as a negated universal one
				CSaturationConceptReferenceLinking* satConRefLinking = (CSaturationConceptReferenceLinking*)conSatRefLinking->getConceptSaturationReferenceLinkingData(negated);
				if (!satConRefLinking) {
					return nullptr;
				}
				CIndividualSaturationProcessNode* baseNode = (CIndividualSaturationProcessNode*)satConRefLinking->getIndividualProcessNodeForConcept();
				if (!baseNode) {
					return nullptr;
				}
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CIndividualSaturationProcessNodeStatusFlags* baseFlags = baseNode->getIndirectStatusFlags();
				CIndividualSaturationProcessNodeStatusFlags* repFlags = repNode->getIndirectStatusFlags();
				if (unsatisfiable) {
					*unsatisfiable = baseFlags->hasClashedFlag() || repFlags->hasClashedFlag();
				}
				if (reliable) {
					*reliable = isReliable(baseNode);
				}
				return baseNode;
			}


			CIndividualSaturationProcessNode* CSaturationSubsumptionDecider::getRepresentativeNode(CIndividualSaturationProcessNode* baseNode) {
				CIndividualSaturationProcessNode* node = baseNode;
				while (node->hasSubstituteIndividualNode()) {
					node = node->getSubstituteIndividualNode();
				}
				return node;
			}


			bool CSaturationSubsumptionDecider::isReliable(CIndividualSaturationProcessNode* baseNode) {
				// the flags the classifier consults before it trusts a saturation, plus the critical one
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CIndividualSaturationProcessNodeStatusFlags* baseFlags = baseNode->getIndirectStatusFlags();
				CIndividualSaturationProcessNodeStatusFlags* repFlags = repNode->getIndirectStatusFlags();
				if (baseFlags->hasInsufficientFlag() || repFlags->hasInsufficientFlag()) {
					mUndecidedMergeConceptCodeCounts[-600] = mUndecidedMergeConceptCodeCounts.value(-600, 0) + 1;
					return false;
				}
				if (baseFlags->hasUnprocessedFlag() || repFlags->hasUnprocessedFlag()) {
					mUndecidedMergeConceptCodeCounts[-800] = mUndecidedMergeConceptCodeCounts.value(-800, 0) + 1;
					return false;
				}
				if (baseFlags->hasCriticalFlag() || repFlags->hasCriticalFlag()) {
					mUndecidedMergeConceptCodeCounts[-1000] = mUndecidedMergeConceptCodeCounts.value(-1000, 0) + 1;
					return false;
				}
				if (!repNode->isCompleted()) {
					mUndecidedMergeConceptCodeCounts[-1200] = mUndecidedMergeConceptCodeCounts.value(-1200, 0) + 1;
					return false;
				}
				if (repFlags->hasCardinalityRestrictedFlag() || repFlags->hasCardinalityProplematicFlag() || repFlags->hasNominalConnectionFlag()) {
					// the saturation does not derive what a cardinality or a nominal implies, the tableau does
					mUndecidedMergeConceptCodeCounts[-2200] = mUndecidedMergeConceptCodeCounts.value(-2200, 0) + 1;
					return false;
				}
				return true;
			}


			bool CSaturationSubsumptionDecider::hasLabelConcept(CIndividualSaturationProcessNode* baseNode, CConcept* concept, bool negated) {
				CIndividualSaturationProcessNode* node = baseNode;
				while (node->hasSubstituteIndividualNode()) {
					if (node != baseNode && !negated) {
						// a substitute stands for a concept that subsumes the base one, see the classifier's extractor
						CSaturationConceptDataItem* satConDataItem = (CSaturationConceptDataItem*)node->getSaturationConceptReferenceLinking();
						if (satConDataItem && !satConDataItem->getSaturationNegation() && !satConDataItem->getSaturationRoleRanges()) {
							if (satConDataItem->getSaturationConcept() == concept) {
								return true;
							}
						}
					}
					node = node->getSubstituteIndividualNode();
				}
				CReapplyConceptSaturationLabelSet* labelSet = node->getReapplyConceptSaturationLabelSet(false);
				if (labelSet && labelSet->hasConcept(concept, negated)) {
					return true;
				}
				return false;
			}


			bool CSaturationSubsumptionDecider::isSuccessorCreatingRole(CRole* role) {
				// a role whose successors the saturation may not have materialised, because they are
				// implied by chains or transitivity that the preprocessing compiled away
				if (role->isDataRole()) {
					return false;
				}
				if (role->isComplexRole() || role->isTransitive() || role->hasRoleChainSuperSharing() || role->hasRoleChainSubSharing()) {
					return false;
				}
				return true;
			}



			bool CSaturationSubsumptionDecider::isSubRoleOf(CRole* subRole, CRole* superRole) {
				// by tag, the query side's roles are those of the answering ontology
				if (subRole == superRole || subRole->getRoleTag() == superRole->getRoleTag()) {
					return true;
				}
				for (CSortedNegLinker<CRole*>* superIt = subRole->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
					if (!superIt->isNegated() && superIt->getData()->getRoleTag() == superRole->getRoleTag()) {
						return true;
					}
				}
				return false;
			}


			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideExistentialFromLabel(CIndividualSaturationProcessNode* baseNode, CRole* role, CConcept* filler, bool fillerNegation, cint64 depth) {
				// the verdict depends on the node and the restriction only, and the search visits the same nodes
				// again and again, as ancestors of the candidates and as successors, so it is remembered; the
				// saturation does not change while the answering thread asks
				CExistentialMemoKey memoKey(QPair<CIndividualSaturationProcessNode*,CConcept*>(baseNode, filler), role->getRoleTag() * 2 + (fillerNegation ? 1 : 0));
				QHash<CExistentialMemoKey,Verdict>::const_iterator memoIt = mExistentialMemoHash.constFind(memoKey);
				if (memoIt != mExistentialMemoHash.constEnd()) {
					return memoIt.value();
				}
				Verdict verdict = decideExistentialFromLabelUncached(baseNode, role, filler, fillerNegation, depth);
				if (verdict != NEEDS_COMPLETION) {
					mExistentialMemoHash.insert(memoKey, verdict);
				}
				return verdict;
			}


			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideExistentialFromLabelUncached(CIndividualSaturationProcessNode* baseNode, CRole* role, CConcept* filler, bool fillerNegation, cint64 depth) {
				// every existential restriction of the label over the role or a sub role is a successor of the
				// node's instances: the filler is entailed if one of them satisfies it, and not entailed if none
				// can, which needs every one of them decided; a successor's range concepts, qualified and
				// cardinality restrictions and a filler without a saturation are left undecided
				CReapplyConceptSaturationLabelSet* labelSet = getRepresentativeNode(baseNode)->getReapplyConceptSaturationLabelSet(false);
				if (!labelSet) {
					mUndecidedMergeConceptCodeCounts[-5600] = mUndecidedMergeConceptCodeCounts.value(-5600, 0) + 1;
					return UNDECIDED;
				}
				bool undecided = false;
				for (CConceptSaturationDescriptor* conDesIt = labelSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
					CConcept* labelConcept = conDesIt->getConcept();
					bool labelNegated = conDesIt->isNegated();
					cint64 opCode = labelConcept->getOperatorCode();
					CRole* labelRole = labelConcept->getRole();
					if (!labelRole || labelRole->isDataRole()) {
						continue;
					}
					bool existential = (!labelNegated && opCode == CCSOME) || (labelNegated && opCode == CCALL);
					bool otherSuccessor = (!labelNegated && (opCode == CCAQSOME || opCode == CCATLEAST)) || (labelNegated && (opCode == CCAQALL || opCode == CCATMOST));
					if (!existential && !otherSuccessor) {
						continue;
					}
					if (!isSubRoleOf(labelRole, role)) {
						continue;
					}
					CSortedNegLinker<CConcept*>* labelFillerLinker = labelConcept->getOperandList();
					if (otherSuccessor || !labelFillerLinker || labelRole->getDomainRangeConceptList(true)) {
						mUndecidedMergeConceptCodeCounts[-5700] = mUndecidedMergeConceptCodeCounts.value(-5700, 0) + 1;
						undecided = true;
						continue;
					}
					CConcept* successorConcept = labelFillerLinker->getData();
					bool successorNegation = labelNegated ? !labelFillerLinker->isNegated() : labelFillerLinker->isNegated();
					Verdict verdict = UNDECIDED;
					if (successorConcept->hasClassName() && !successorNegation && filler->hasClassName() && !fillerNegation) {
						// the class hierarchy is complete for named classes, the saturation is not always
						verdict = decideNamedSubsumptionFromHierarchy(successorConcept, filler);
					}
					if (verdict == UNDECIDED) {
						bool unsatisfiable = false;
						bool reliable = false;
						CIndividualSaturationProcessNode* successorNode = getSaturationNode(successorConcept, &unsatisfiable, &reliable, successorNegation);
						if (successorNode && unsatisfiable) {
							verdict = SUBSUMED;
						} else if (successorNode) {
							verdict = decideEntailed(successorNode, reliable, filler, fillerNegation, depth + 1);
						} else {
							mUndecidedMergeConceptCodeCounts[-6200] = mUndecidedMergeConceptCodeCounts.value(-6200, 0) + 1;
						}
					}
					if (verdict == SUBSUMED || verdict == NEEDS_COMPLETION) {
						return verdict;
					} else if (verdict == UNDECIDED) {
						mUndecidedMergeConceptCodeCounts[-5800] = mUndecidedMergeConceptCodeCounts.value(-5800, 0) + 1;
						undecided = true;
					}
				}
				return undecided ? UNDECIDED : NOT_SUBSUMED;
			}


			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideEntailedExistential(CIndividualSaturationProcessNode* baseNode, bool reliable, CConcept* concept, bool conceptNegated, CRole* role, CConcept* filler, bool fillerNegation, cint64 depth) {
				// whether every instance of the node's concept has a successor under the role that satisfies the
				// filler: the concept itself in the label, or a linked successor that satisfies the filler
				if (hasLabelConcept(baseNode, concept, conceptNegated)) {
					return SUBSUMED;
				}
				if (!role || role->isDataRole()) {
					return UNDECIDED;
				}
				// why an existential restriction stays undecided is counted under -5600 and up, see decideSubClass
				bool undecided = false;
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CLinkedRoleSaturationSuccessorHash* succHash = repNode->getLinkedRoleSuccessorHash(false);
				// the saturation does not link every successor of a node, the existential restrictions of its
				// label stand for them, so the label is searched as well as the linked successors (issue #19)
				Verdict labelVerdict = decideExistentialFromLabel(baseNode, role, filler, fillerNegation, depth);
				if (labelVerdict == SUBSUMED || labelVerdict == NEEDS_COMPLETION) {
					return labelVerdict;
				} else if (labelVerdict == UNDECIDED) {
					undecided = true;
				}
				if (baseNode == mRootBaseNode && mRootSubClassConcept && !undecided) {
					// a label the saturation left without a named subsumer also lacks that subsumer's
					// existential restrictions, so those of the class's ancestors in the hierarchy, which is
					// complete for named classes, are looked at as well
					for (CConcept* ancestor : getHierarchyAncestors(mRootSubClassConcept)) {
						bool ancestorUnsatisfiable = false;
						bool ancestorReliable = false;
						CIndividualSaturationProcessNode* ancestorNode = getSaturationNode(ancestor, &ancestorUnsatisfiable, &ancestorReliable);
						if (!ancestorNode || ancestorNode == baseNode) {
							continue;
						}
						Verdict ancestorVerdict = decideExistentialFromLabel(ancestorNode, role, filler, fillerNegation, depth);
						if (ancestorVerdict == SUBSUMED || ancestorVerdict == NEEDS_COMPLETION) {
							return ancestorVerdict;
						} else if (ancestorVerdict == UNDECIDED || !ancestorReliable) {
							undecided = true;
							break;
						}
					}
				}
				if (succHash && succHash->hasLinkedRoleSuccessorData(role)) {
					// the successors are linked under every super role, so the role itself suffices;
					// looked up without the getter, whose subscript access modifies the node's hash
					// from this thread while the tableau tasks read it
					CLinkedRoleSaturationSuccessorData* roleSuccData = succHash->getLinkedRoleSuccessorHash()->value(role);
					CPROCESSMAP<cint64,CSaturationSuccessorData*>* succMap = roleSuccData ? roleSuccData->getSuccessorNodeDataMap(false) : nullptr;
					if (succMap) {
						for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succMap->constBegin(), itEnd = succMap->constEnd(); it != itEnd; ++it) {
							CSaturationSuccessorData* succData = it.value();
							if (succData->mActiveCount <= 0) {
								continue;
							}
							if (succData->mVALUENominalConnection || !succData->mSuccIndiNode) {
								mUndecidedMergeConceptCodeCounts[-5700] = mUndecidedMergeConceptCodeCounts.value(-5700, 0) + 1;
								undecided = true;
								continue;
							}
							CIndividualSaturationProcessNode* succNode = succData->mSuccIndiNode;
							Verdict succVerdict = decideEntailed(succNode, isReliable(succNode), filler, fillerNegation, depth + 1);
							if (succVerdict == SUBSUMED || succVerdict == NEEDS_COMPLETION) {
								return succVerdict;
							} else if (succVerdict == UNDECIDED) {
								mUndecidedMergeConceptCodeCounts[-5800] = mUndecidedMergeConceptCodeCounts.value(-5800, 0) + 1;
								undecided = true;
							}
						}
					}
				}
				if (undecided || !reliable || !isSuccessorCreatingRole(role)) {
					cint64 code = undecided ? -6100 : (!reliable ? -5900 : -6000);
					mUndecidedMergeConceptCodeCounts[code] = mUndecidedMergeConceptCodeCounts.value(code, 0) + 1;
					return UNDECIDED;
				}
				return NOT_SUBSUMED;
			}


			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideEntailed(CIndividualSaturationProcessNode* baseNode, bool reliable, CConcept* concept, bool negated, cint64 depth) {
				if (depth > 64) {
					return UNDECIDED;
				}
				cint64 opCode = concept->getOperatorCode();
				CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();

				if (opCode == CCNOT && opLinker) {
					return decideEntailed(baseNode, reliable, opLinker->getData(), !(opLinker->isNegated() ^ negated), depth + 1);
				}
				if (negated && opCode == CCALL && opLinker) {
					// the ontology stores an existential restriction as a negated universal one, not r only not D
					// for r some D, and a query of the form r some D arrives in that form (issue #19)
					return decideEntailedExistential(baseNode, reliable, concept, true, concept->getRole(), opLinker->getData(), !opLinker->isNegated(), depth);
				}

				if (!negated) {
					// every instance of the node's concept has to satisfy the concept
					if (opCode == CCTOP) {
						return SUBSUMED;
					}
					if (opCode == CCBOTTOM) {
						return reliable ? NOT_SUBSUMED : UNDECIDED;
					}
					if (concept->hasClassName()) {
						if (hasLabelConcept(baseNode, concept, false)) {
							return SUBSUMED;
						}
						if (baseNode == mRootBaseNode && mRootSubClassConcept) {
							// the class hierarchy is complete for named classes, the saturation is not always,
							// a functional role can imply a subsumer the saturation does not derive
							Verdict hierarchyVerdict = decideNamedSubsumptionFromHierarchy(mRootSubClassConcept, concept);
							if (hierarchyVerdict != UNDECIDED) {
								return hierarchyVerdict;
							}
						}
						// a complete saturation contains every named subsumer, which is how the classifier
						// derives the class hierarchy from it
						return reliable ? NOT_SUBSUMED : UNDECIDED;
					}
					if (opCode == CCAND) {
						bool undecided = false;
						for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
							Verdict opVerdict = decideEntailed(baseNode, reliable, opIt->getData(), opIt->isNegated(), depth + 1);
							if (opVerdict == NOT_SUBSUMED || opVerdict == NEEDS_COMPLETION) {
								return opVerdict;
							} else if (opVerdict == UNDECIDED) {
								undecided = true;
							}
						}
						return undecided ? UNDECIDED : SUBSUMED;
					}
					if (opCode == CCOR) {
						for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
							Verdict opVerdict = decideEntailed(baseNode, reliable, opIt->getData(), opIt->isNegated(), depth + 1);
							if (opVerdict == SUBSUMED || opVerdict == NEEDS_COMPLETION) {
								return opVerdict;
							}
						}
						// a disjunction can be entailed without any disjunct being entailed
						return UNDECIDED;
					}
					if (opCode == CCSOME && opLinker) {
						return decideEntailedExistential(baseNode, reliable, concept, false, concept->getRole(), opLinker->getData(), opLinker->isNegated(), depth);
					}
					// universal and cardinality restrictions, nominals, self restrictions and data ranges are
					// only recognised when the label carries the very concept
					if (hasLabelConcept(baseNode, concept, false)) {
						return SUBSUMED;
					}
					return UNDECIDED;

				} else {
					// no instance of the node's concept may satisfy the concept, i.e. their conjunction is
					// unsatisfiable
					if (opCode == CCTOP) {
						return reliable ? NOT_SUBSUMED : UNDECIDED;
					}
					if (opCode == CCBOTTOM) {
						return SUBSUMED;
					}
					if (opCode == CCOR) {
						// the negation of a disjunction is the conjunction of the negated disjuncts
						bool undecided = false;
						for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
							Verdict opVerdict = decideEntailed(baseNode, reliable, opIt->getData(), !opIt->isNegated(), depth + 1);
							if (opVerdict == NOT_SUBSUMED || opVerdict == NEEDS_COMPLETION) {
								return opVerdict;
							} else if (opVerdict == UNDECIDED) {
								undecided = true;
							}
						}
						return undecided ? UNDECIDED : SUBSUMED;
					}
					if (hasLabelConcept(baseNode, concept, true)) {
						return SUBSUMED;
					}
					if (concept->hasClassName() || opCode == CCAND) {
						// subsumed as soon as one conjunct is excluded on its own
						if (opCode == CCAND) {
							for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
								Verdict opVerdict = decideEntailed(baseNode, reliable, opIt->getData(), !opIt->isNegated(), depth + 1);
								if (opVerdict == SUBSUMED || opVerdict == NEEDS_COMPLETION) {
									return opVerdict;
								}
							}
						}
						// otherwise the node's concept and the conjunction have to be shown compatible
						QList<CConcept*> conjuncts;
						if (collectNamedConjuncts(concept, false, conjuncts, 0)) {
							return decideConjunctionSatisfiable(baseNode, reliable, conjuncts);
						}
					}
					return UNDECIDED;
				}
			}



			bool CSaturationSubsumptionDecider::collectNamedConjuncts(CConcept* concept, bool negated, QList<CConcept*>& conjuncts, cint64 depth) {
				if (negated || depth > 64) {
					return false;
				}
				if (concept->hasClassName()) {
					conjuncts.append(concept);
					return true;
				}
				if (concept->getOperatorCode() == CCAND) {
					for (CSortedNegLinker<CConcept*>* opIt = concept->getOperandList(); opIt; opIt = opIt->getNext()) {
						if (!collectNamedConjuncts(opIt->getData(), opIt->isNegated(), conjuncts, depth + 1)) {
							return false;
						}
					}
					return true;
				}
				return false;
			}



			CSaturationSubsumptionDecider::LabelConceptKind CSaturationSubsumptionDecider::getLabelConceptKind(CConcept* concept, bool negated) {
				// what a concept of a saturated label can do when the label is merged with another
				// one, following the rules the saturation applies to each operator:
				//  - a named class, the top concept, a conjunction, an existential restriction, an at
				//    least restriction or the self loop adds nothing at the merged node beyond what its
				//    own part already derived, its successors stay separate, so it can only clash with
				//    its complement;
				//  - an implication fires when all its triggers are present, which the other part
				//    may supply;
				//  - a universal restriction, including the automaton transitions of the role chains,
				//    propagates into the successors of the other parts;
				//  - everything else, disjunctions, choices, cardinalities, nominals, data, branch
				//    triggers, is left to the tableau.
				// The operator codes of the basic constructors come in pairs whose sign the negation
				// flag flips, an existential restriction is stored as a negated universal one, so the
				// effective operator is taken for those.
				if (concept->hasClassName()) {
					return LCK_SAFE;
				}
				cint64 opCode = concept->getOperatorCode();
				cint64 absCode = opCode < 0 ? -opCode : opCode;
				bool paired = absCode == 1 || absCode == 3 || absCode == 4 || absCode == 5 || absCode == 11;
				cint64 effCode = (paired && negated) ? -opCode : opCode;
				bool effNegated = paired ? false : negated;
				if (effCode == CCTOP || effCode == CCBOTTOM) {
					return LCK_SAFE;
				}
				if (!effNegated) {
					if (effCode == CCATOM || effCode == CCSUB || effCode == CCEQ) {
						return LCK_SAFE;
					}
					if (effCode == CCAND || effCode == CCIMPLTRIG || effCode == CCAQAND || effCode == CCIMPLAQAND || effCode == CCAQCHOOCE) {
						return LCK_CONJUNCTION;
					}
					if (effCode == CCSOME || effCode == CCAQSOME || effCode == CCATLEAST || effCode == CCSELF) {
						return LCK_SAFE;
					}
					if (effCode == CCIMPL) {
						return LCK_IMPLICATION;
					}
					if (effCode == CCEQCAND) {
						return LCK_CANDIDATE;
					}
					if (effCode == CCALL || effCode == CCAQALL || effCode == CCIMPLALL || effCode == CCIMPLAQALL) {
						return LCK_UNIVERSAL;
					}
				} else {
					if (effCode == CCATOM || effCode == CCSUB || effCode == CCEQ) {
						return LCK_SAFE;
					}
				}
				return LCK_UNSAFE;
			}


			bool CSaturationSubsumptionDecider::isSelfLoopOnRole(CConcept* selfConcept, CRole* role) {
				// whether a self restriction makes the node its own successor under the role, directly
				// or through a sub role
				CRole* selfRole = selfConcept->getRole();
				if (!selfRole) {
					return false;
				}
				if (selfRole == role || selfRole->getRoleTag() == role->getRoleTag()) {
					return true;
				}
				for (CSortedNegLinker<CRole*>* superIt = selfRole->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
					if (!superIt->isNegated() && (superIt->getData() == role || superIt->getData()->getRoleTag() == role->getRoleTag())) {
						return true;
					}
				}
				return false;
			}


			bool CSaturationSubsumptionDecider::hasSuccessorWithoutConcept(CIndividualSaturationProcessNode* repNode, CRole* role, CConcept* concept, bool negated, cint64 depth) {
				// whether the node has an active successor under the role that lacks the concept, which
				// a universal restriction of another part would then add
				CLinkedRoleSaturationSuccessorHash* succHash = repNode->getLinkedRoleSuccessorHash(false);
				if (!succHash || !succHash->hasLinkedRoleSuccessorData(role)) {
					return false;
				}
				// looked up without the getter, whose subscript access modifies the node's hash from
				// this thread while the tableau tasks read it
				CLinkedRoleSaturationSuccessorData* roleSuccData = succHash->getLinkedRoleSuccessorHash()->value(role);
				CPROCESSMAP<cint64,CSaturationSuccessorData*>* succMap = roleSuccData ? roleSuccData->getSuccessorNodeDataMap(false) : nullptr;
				if (!succMap) {
					return false;
				}
				for (CPROCESSMAP<cint64,CSaturationSuccessorData*>::const_iterator it = succMap->constBegin(), itEnd = succMap->constEnd(); it != itEnd; ++it) {
					CSaturationSuccessorData* succData = it.value();
					if (succData->mActiveCount <= 0) {
						continue;
					}
					if (succData->mVALUENominalConnection || !succData->mSuccIndiNode) {
						return true;
					}
					if (!saturationNodeSatisfies(succData->mSuccIndiNode, concept, negated, depth)) {
						return true;
					}
				}
				return false;
			}


			bool CSaturationSubsumptionDecider::saturationNodeSatisfies(CIndividualSaturationProcessNode* succNode, CConcept* concept, bool negated, cint64 depth) {
				// the concept is in the successor's saturated label, or, down to the depth, it would add nothing
				// there: a conjunction all of whose conjuncts it holds, a disjunction one of whose disjuncts it
				// holds, or a plain universal restriction over a role the successor has no self loop on, whose
				// filler every successor under the role holds already, in particular when there is none (the
				// remodelled SNOMED CT closures are of the form ∀group.∀r.C, and most of them reach a group
				// successor that has no r successor at all); named classes, existential restrictions and the rest
				// are not derived here
				if (hasLabelConcept(succNode, concept, negated)) {
					return true;
				}
				if (depth <= 0 || concept->hasClassName()) {
					return false;
				}
				cint64 opCode = concept->getOperatorCode();
				if ((opCode == CCAND && !negated) || (opCode == CCOR && negated)) {
					for (CSortedNegLinker<CConcept*>* opIt = concept->getOperandList(); opIt; opIt = opIt->getNext()) {
						if (!saturationNodeSatisfies(succNode, opIt->getData(), opIt->isNegated() ^ negated, depth)) {
							return false;
						}
					}
					return true;
				}
				if ((opCode == CCOR && !negated) || (opCode == CCAND && negated)) {
					for (CSortedNegLinker<CConcept*>* opIt = concept->getOperandList(); opIt; opIt = opIt->getNext()) {
						if (saturationNodeSatisfies(succNode, opIt->getData(), opIt->isNegated() ^ negated, depth)) {
							return true;
						}
					}
					return false;
				}
				if ((opCode == CCALL && !negated) || (opCode == CCSOME && negated)) {
					CRole* role = concept->getRole();
					if (!role || role->isDataRole() || !isSuccessorCreatingRole(role)) {
						return false;
					}
					CIndividualSaturationProcessNode* repSucc = getRepresentativeNode(succNode);
					if (!isUsableSuccessorSaturation(succNode)) {
						return false;
					}
					CReapplyConceptSaturationLabelSet* labelSet = repSucc->getReapplyConceptSaturationLabelSet(false);
					if (labelSet) {
						for (CConceptSaturationDescriptor* conDesIt = labelSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
							if (!conDesIt->isNegated() && conDesIt->getConcept()->getOperatorCode() == CCSELF && isSelfLoopOnRole(conDesIt->getConcept(), role)) {
								return false;
							}
							cint64 labelOpCode = conDesIt->getConcept()->getOperatorCode();
							if ((labelOpCode == CCOR && !conDesIt->isNegated()) || (labelOpCode == CCAND && conDesIt->isNegated())) {
								// a choice the saturation did not make: the model's successor may hold a disjunct with
								// successors of its own
								return false;
							}
						}
					}
					for (CSortedNegLinker<CConcept*>* opIt = concept->getOperandList(); opIt; opIt = opIt->getNext()) {
						if (hasSuccessorWithoutConcept(repSucc, role, opIt->getData(), opIt->isNegated() ^ negated, depth - 1)) {
							return false;
						}
					}
					return true;
				}
				return false;
			}


			bool CSaturationSubsumptionDecider::isUsableSuccessorSaturation(CIndividualSaturationProcessNode* node) {
				// a saturation whose successors stand for those of a node of the completion model: processed and
				// completed, not insufficient (the saturation gave up on completing the label, so existential
				// restrictions may be missing), not clashed, and without a nominal (whose successors come from the
				// ABox). The critical flag is allowed, it marks disjunctions and universal restrictions propagated
				// into the successors, which add no successor (the disjunctions are refused at the successor
				// itself by the caller). The cardinality flags are allowed as well: a restriction keeps the
				// saturation from merging successors the tableau would merge, which leaves it with at least the
				// successors of the model, each with at most the model's label, so what is read from it (no
				// successor under a role, or a concept at every successor) holds in the model; and a restriction
				// at its limit (the problematic flag) is not disturbed, since the caller admits only fillers that
				// add no successor.
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(node);
				CIndividualSaturationProcessNodeStatusFlags* baseFlags = node->getIndirectStatusFlags();
				CIndividualSaturationProcessNodeStatusFlags* repFlags = repNode->getIndirectStatusFlags();
				if (baseFlags->hasInsufficientFlag() || repFlags->hasInsufficientFlag() || baseFlags->hasUnprocessedFlag() || repFlags->hasUnprocessedFlag() || baseFlags->hasClashedFlag() || repFlags->hasClashedFlag()) {
					return false;
				}
				if (!repNode->isCompleted()) {
					return false;
				}
				if (repFlags->hasNominalConnectionFlag()) {
					return false;
				}
				return true;
			}


			bool CSaturationSubsumptionDecider::isSuccessorNeutralFiller(CConcept* concept, bool negated) {
				if (concept->hasClassName()) {
					// a named class brings its axioms, which may have existential restrictions
					return negated;
				}
				cint64 opCode = concept->getOperatorCode();
				if (opCode == CCTOP || opCode == CCBOTTOM) {
					return true;
				}
				if ((opCode == CCALL && !negated) || (opCode == CCSOME && negated)) {
					return true;
				}
				if (opCode == CCAND || opCode == CCOR) {
					for (CSortedNegLinker<CConcept*>* opIt = concept->getOperandList(); opIt; opIt = opIt->getNext()) {
						if (!isSuccessorNeutralFiller(opIt->getData(), opIt->isNegated() ^ negated)) {
							return false;
						}
					}
					return true;
				}
				return false;
			}


			bool CSaturationSubsumptionDecider::completedLabelSuccessorsSatisfyUniversal(CCompletedLabel* label, const CMergeState* base, CRole* role, CConcept* universalConcept, bool negated) {
				// A completed label is the root of a model of its class, its successors are those of its
				// existential restrictions, and the successor of ∃r.F holds at least what the saturation derives
				// for F, whether the tableau expanded it or left it to the saturation. So a universal restriction
				// of the base that reaches these successors is satisfied if, for every existential restriction of
				// the label under the role, the saturation of its filler satisfies the universal restriction's
				// filler (saturationNodeSatisfies: present, or a plain nested universal restriction over a role the
				// filler has no successor under, which asks for a saturation whose successors stand for the
				// model's, isUsableSuccessorSaturation), provided that nothing reaching the successors can add
				// successors to them, every universal restriction of the base and of the label over a role above
				// one of the existential restrictions' roles having a filler that adds none
				// (isSuccessorNeutralFiller), and that no nested restriction leads back to the root over an inverse
				// role, which the saturation does not model. The successors are the fillers' own saturation nodes,
				// not the successors of the class's saturation node: a node the saturation gave up on
				// (insufficient) lists none of them although its label holds the existential restrictions, and a
				// restriction would then count as satisfied by nothing (the OWL API scenario's primitive P2).
				cint64 opCode = universalConcept->getOperatorCode();
				if (!((opCode == CCALL && !negated) || (opCode == CCSOME && negated))) {
					return false;
				}
				if (!role || role->isDataRole() || !isSuccessorCreatingRole(role)) {
					return false;
				}
				// the existential restrictions of the label under the role, the saturation nodes of their fillers, and the universal restrictions of the label
				QList<CRole*> existentialRoleList;
				QList<CIndividualSaturationProcessNode*> successorNodeList;
				QList<QPair<CConcept*,bool> > universalList;
				for (const CCompletedLabelEntry& entry : label->mEntries) {
					CConcept* entryConcept = entry.mConcept;
					cint64 entryOpCode = entryConcept->getOperatorCode();
					cint64 absCode = entryOpCode < 0 ? -entryOpCode : entryOpCode;
					bool existential = absCode == 5 && ((entryOpCode < 0) != entry.mNegated);
					bool automatonExistential = absCode == 11 && ((entryOpCode < 0) != entry.mNegated);
					bool atLeast = !entry.mNegated && entryOpCode == CCATLEAST;
					bool universal = (absCode == 5 || absCode == 11) && ((entryOpCode < 0) == entry.mNegated);
					CRole* entryRole = entryConcept->getRole();
					if (!entryRole) {
						continue;
					}
					if (universal) {
						universalList.append(QPair<CConcept*,bool>(entryConcept, entry.mNegated));
					} else if ((existential || automatonExistential || atLeast) && isSubRoleOf(entryRole, role)) {
						if (automatonExistential || atLeast || entryRole->isDataRole()) {
							// successors a saturation node does not stand for one by one
							return false;
						}
						for (CSortedNegLinker<CConcept*>* opIt = entryConcept->getOperandList(); opIt; opIt = opIt->getNext()) {
							bool unsatisfiable = false;
							CIndividualSaturationProcessNode* fillerNode = getSaturationNode(opIt->getData(), &unsatisfiable, nullptr, opIt->isNegated() ^ entry.mNegated);
							if (!fillerNode || unsatisfiable) {
								// no saturation standing for the model's successor
								mUndecidedMergeConceptCodeCounts[-13020] = mUndecidedMergeConceptCodeCounts.value(-13020, 0) + 1;
								return false;
							}
							successorNodeList.append(fillerNode);
						}
						existentialRoleList.append(entryRole);
					}
				}
				if (existentialRoleList.isEmpty()) {
					return false;
				}
				// nothing else reaching the successors may add successors to them
				for (const CMergeUniversal& baseUniversal : base->mUniversalList) {
					universalList.append(QPair<CConcept*,bool>(baseUniversal.mConcept, baseUniversal.mNegated));
				}
				for (const QPair<CConcept*,bool>& reaching : universalList) {
					CRole* reachingRole = reaching.first->getRole();
					if (!reachingRole) {
						return false;
					}
					bool reaches = false;
					for (CRole* existentialRole : existentialRoleList) {
						if (isSubRoleOf(existentialRole, reachingRole)) {
							reaches = true;
						}
					}
					if (reaches) {
						for (CSortedNegLinker<CConcept*>* opIt = reaching.first->getOperandList(); opIt; opIt = opIt->getNext()) {
							if (!isSuccessorNeutralFiller(opIt->getData(), opIt->isNegated() ^ reaching.second)) {
								return false;
							}
						}
					}
				}
				for (CSortedNegLinker<CConcept*>* opIt = universalConcept->getOperandList(); opIt; opIt = opIt->getNext()) {
					CConcept* filler = opIt->getData();
					bool fillerNegated = opIt->isNegated() ^ negated;
					cint64 fillerOpCode = filler->getOperatorCode();
					if ((fillerOpCode == CCALL && !fillerNegated) || (fillerOpCode == CCSOME && fillerNegated)) {
						// a nested restriction over the inverse of a role leading to a successor reaches the root
						CRole* nestedRole = filler->getRole();
						CRole* nestedInverseRole = nestedRole ? nestedRole->getInverseRole() : nullptr;
						for (CRole* existentialRole : existentialRoleList) {
							if (nestedInverseRole && isSubRoleOf(existentialRole, nestedInverseRole)) {
								return false;
							}
						}
					}
					for (CIndividualSaturationProcessNode* successorNode : successorNodeList) {
						if (!saturationNodeSatisfies(successorNode, filler, fillerNegated, 1)) {
							return false;
						}
					}
				}
				return true;
			}


			void CSaturationSubsumptionDecider::setCompletedSuccessorDecision(bool enabled) {
				mCompletedSuccessorDecision = enabled;
			}



			bool CSaturationSubsumptionDecider::addMergeLabelConcept(CMergeState& state, CConcept* concept, bool negated, bool derived) {
				// adds a concept to the merged label and everything the saturation would derive from it
				// at the merged node; false if the merge cannot be decided
				if (negated ? state.hasNegative(concept) : state.hasPositive(concept)) {
					return true;
				}
				if (negated) {
					state.mNegativeSet.insert(concept);
				} else {
					state.mPositiveSet.insert(concept);
				}
				if (state.hasPositive(concept) && state.hasNegative(concept)) {
					state.mClashed = true;
					return true;
				}
				if (!negated && concept->hasClassName()) {
					if (derived) {
						// a named class derived at the merged node brings its own saturation along
						return addMergeNamedClass(state, concept);
					}
					return true;
				}
				LabelConceptKind kind = getLabelConceptKind(concept, negated);
				cint64 opCode = concept->getOperatorCode();
				if (kind == LCK_UNSAFE && ((opCode == CCOR && !negated) || (opCode == CCAND && negated))) {
					// a disjunction one of whose disjuncts the merged label holds already is satisfied and
					// adds nothing; only a disjunction that still needs a choice leaves the merge to the
					// tableau (issue #33)
					for (CSortedNegLinker<CConcept*>* opIt = concept->getOperandList(); opIt; opIt = opIt->getNext()) {
						bool disjunctNegated = opIt->isNegated() ^ negated;
						if (disjunctNegated ? state.hasNegative(opIt->getData()) : state.hasPositive(opIt->getData())) {
							mUndecidedMergeConceptCodeCounts[-13004] = mUndecidedMergeConceptCodeCounts.value(-13004, 0) + 1;
							return true;
						}
					}
				}
				if (kind == LCK_UNSAFE) {
					cint64 codeKey = opCode * 2 + (negated ? 1 : 0);
					mUndecidedMergeConceptCodeCounts[codeKey] = mUndecidedMergeConceptCodeCounts.value(codeKey, 0) + 1;
					return false;
				}
				if (kind == LCK_CONJUNCTION) {
					if (derived) {
						for (CSortedNegLinker<CConcept*>* opIt = concept->getOperandList(); opIt; opIt = opIt->getNext()) {
							if (opCode == CCAQCHOOCE && opIt->isNegated()) {
								// the choose rule adds the operands of the descriptor's polarity only
								continue;
							}
							if (!addMergeLabelConcept(state, opIt->getData(), opIt->isNegated(), true)) {
								return false;
							}
						}
					}
					// the operands of a conjunction of a part are in that part's label already
					return true;
				}
				if (kind == LCK_IMPLICATION) {
					state.mImplicationList.append(concept);
					return true;
				}
				if (kind == LCK_CANDIDATE) {
					CSortedNegLinker<CConcept*>* candidateLinker = concept->getOperandList();
					if (candidateLinker && !candidateLinker->isNegated() && candidateLinker->getData()->hasClassName()) {
						state.mCandidateList.append(candidateLinker->getData());
						return true;
					}
					mUndecidedMergeConceptCodeCounts[-9000] = mUndecidedMergeConceptCodeCounts.value(-9000, 0) + 1;
					return false;
				}
				if (kind == LCK_UNIVERSAL) {
					state.mUniversalList.append(CMergeUniversal(concept, negated, derived ? nullptr : state.mCurrentPartNode));
					return true;
				}
				if (!negated && opCode == CCSELF) {
					state.mSelfConceptSet.insert(concept);
					return true;
				}
				if (derived) {
					cint64 absCode = opCode < 0 ? -opCode : opCode;
					bool existential = (absCode == 5 || absCode == 11) && ((opCode < 0) != negated);
					if (existential || (!negated && opCode == CCATLEAST)) {
						// a derived successor: everything a universal restriction or a cardinality could
						// do with it is left to the tableau, and the domain of its role joins the node
						CRole* role = concept->getRole();
						if (!role || opCode == CCATLEAST) {
							mUndecidedMergeConceptCodeCounts[-7000] = mUndecidedMergeConceptCodeCounts.value(-7000, 0) + 1;
							return false;
						}
						state.mDerivedSuccessorRoleSet.insert(role);
						for (CSortedNegLinker<CConcept*>* domIt = role->getDomainConceptList(); domIt; domIt = domIt->getNext()) {
							if (!addMergeLabelConcept(state, domIt->getData(), domIt->isNegated(), true)) {
								return false;
							}
						}
					}
				}
				return true;
			}


			bool CSaturationSubsumptionDecider::addMergeNamedClass(CMergeState& state, CConcept* concept) {
				// adds the saturation of a named class to the merged label, its successors become
				// successors of the merged node
				bool unsatisfiable = false;
				bool reliable = false;
				CIndividualSaturationProcessNode* baseNode = getSaturationNode(concept, &unsatisfiable, &reliable);
				if (baseNode && unsatisfiable) {
					state.mClashed = true;
					return true;
				}
				if (!baseNode || !reliable) {
					// no usable saturation: the class's completed label stands in, or has to be obtained first
					CCompletedLabel* label = mCompletedLabelHash.value(concept, nullptr);
					if (!label) {
						mUndecidedMergeConceptCodeCounts[baseNode ? -9400 : -9200] = mUndecidedMergeConceptCodeCounts.value(baseNode ? -9400 : -9200, 0) + 1;
						mCompletionConcept = concept;
						return false;
					}
					if (label->mUnsatisfiable) {
						state.mClashed = true;
						return true;
					}
					if (label->mFailed) {
						return false;
					}
					if (!extendCompletedLabel(label) && mCompletionConcept) {
						return false;
					}
					return addCompletedLabelToMergeState(state, label);
				}
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				if (state.hasPartNode(repNode)) {
					return true;
				}
				state.mPartNodeSet.insert(repNode);
				state.mPartNodeList.append(repNode);
				CIndividualSaturationProcessNode* previousPartNode = state.mCurrentPartNode;
				state.mCurrentPartNode = repNode;

				CIndividualSaturationProcessNodeStatusFlags* flags = repNode->getIndirectStatusFlags();
				if (flags->hasCardinalityRestrictedFlag() || flags->hasCardinalityProplematicFlag() || flags->hasNominalConnectionFlag()) {
					mUndecidedMergeConceptCodeCounts[-9600] = mUndecidedMergeConceptCodeCounts.value(-9600, 0) + 1;
					return false;
				}
				if (repNode->hasDataValueApplied() || repNode->hasNominalIntegrated()) {
					mUndecidedMergeConceptCodeCounts[-9800] = mUndecidedMergeConceptCodeCounts.value(-9800, 0) + 1;
					return false;
				}
				CIndividualSaturationProcessNode* node = baseNode;
				while (node->hasSubstituteIndividualNode()) {
					if (node != baseNode) {
						CSaturationConceptDataItem* satConDataItem = (CSaturationConceptDataItem*)node->getSaturationConceptReferenceLinking();
						if (satConDataItem && !satConDataItem->getSaturationNegation() && !satConDataItem->getSaturationRoleRanges()) {
							if (!addMergeLabelConcept(state, satConDataItem->getSaturationConcept(), false, false)) {
								return false;
							}
						}
					}
					node = node->getSubstituteIndividualNode();
				}
				CReapplyConceptSaturationLabelSet* labelSet = repNode->getReapplyConceptSaturationLabelSet(false);
				if (!labelSet) {
					mUndecidedMergeConceptCodeCounts[-10000] = mUndecidedMergeConceptCodeCounts.value(-10000, 0) + 1;
					return false;
				}
				for (CConceptSaturationDescriptor* conDesIt = labelSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
					if (!addMergeLabelConcept(state, conDesIt->getConcept(), conDesIt->isNegated(), false)) {
						return false;
					}
				}
				// successors under a functional role would be merged with those of the other parts
				CLinkedRoleSaturationSuccessorHash* succHash = repNode->getLinkedRoleSuccessorHash(false);
				if (succHash) {
					CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* roleSuccHash = succHash->getLinkedRoleSuccessorHash();
					for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator it = roleSuccHash->constBegin(), itEnd = roleSuccHash->constEnd(); it != itEnd; ++it) {
						CRole* role = it.key();
						CLinkedRoleSaturationSuccessorData* roleSuccData = it.value();
						if (roleSuccData && roleSuccData->getSuccessorCount() > 0) {
							if (role->isFunctional() && state.hasSuccessorRole(role)) {
								mUndecidedMergeConceptCodeCounts[-2000] = mUndecidedMergeConceptCodeCounts.value(-2000, 0) + 1;
								return false;
							}
							if (role->isDataRole()) {
								// what two parts say about the values of a data role is left to the tableau
								if (state.hasDataRoleTag(role->getRoleTag())) {
									mUndecidedMergeConceptCodeCounts[-2400] = mUndecidedMergeConceptCodeCounts.value(-2400, 0) + 1;
									return false;
								}
								state.mDataRoleTagSet.insert(role->getRoleTag());
							}
							state.mSuccessorRoleSet.insert(role);
						}
					}
				}
				state.mCurrentPartNode = previousPartNode;
				return true;
			}



			bool CSaturationSubsumptionDecider::closeMergeStateWithCandidates(CMergeState& state) {
				bool added = true;
				while (added && !state.mClashed) {
					added = false;
					QList<CConcept*> candidates;
					for (const CMergeState* part = &state; part; part = part->mBase) {
						candidates.append(part->mCandidateList);
					}
					for (CConcept* candidate : candidates) {
						if (state.mAddedCandidateSet.contains(candidate) || state.hasPositive(candidate)) {
							continue;
						}
						state.mAddedCandidateSet.insert(candidate);
						// derived, so that the class brings its saturation along
						if (!addMergeLabelConcept(state, candidate, false, true)) {
							return false;
						}
						added = true;
					}
					if (added && !closeMergeState(state)) {
						return false;
					}
				}
				return true;
			}


			bool CSaturationSubsumptionDecider::closeMergeState(CMergeState& state) {
				// fires the implications, the base's and the state's, until none fires any more; the
				// first operand is the implied concept, the ones after it are the triggers, which the
				// saturation waits for in order, see updateImplicationReapplyConceptSaturationDescriptor.
				// Of the base's implications only those waiting for a trigger this state supplies are
				// looked at, found through the base's trigger hash, so that a merge does not walk the
				// whole implication list of the query's side
				bool changed = !state.mClashed;
				bool firstRound = true;
				while (changed && !state.mClashed) {
					changed = false;
					QSet<CConcept*> baseImplicationSet;
					if (state.mBase && !state.mBase->mTriggerImplicationHash.isEmpty()) {
						const QHash<CConcept*,QList<CConcept*> >& triggerHash = state.mBase->mTriggerImplicationHash;
						struct Collector {
							const QHash<CConcept*,QList<CConcept*> >* mHash;
							QSet<CConcept*>* mSet;
							void consider(CConcept* trigger) {
								QHash<CConcept*,QList<CConcept*> >::const_iterator it = mHash->constFind(trigger);
								if (it != mHash->constEnd()) {
									for (CConcept* implConcept : it.value()) {
										mSet->insert(implConcept);
									}
								}
							}
						} collector = { &triggerHash, &baseImplicationSet };
						if (firstRound) {
							if (state.mOverlayLabelSet) {
								for (CConceptSaturationDescriptor* conDesIt = state.mOverlayLabelSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
									if (!conDesIt->isNegated()) {
										collector.consider(conDesIt->getConcept());
									}
								}
								for (CConcept* substitute : state.mOverlaySubstituteArray) {
									collector.consider(substitute);
								}
							}
							if (state.mOverlayEntries) {
								for (const CCompletedLabelEntry& entry : *state.mOverlayEntries) {
									if (!entry.mNegated) {
										collector.consider(entry.mConcept);
									}
								}
							}
						}
						for (QSet<CConcept*>::const_iterator it = state.mPositiveSet.constBegin(), itEnd = state.mPositiveSet.constEnd(); it != itEnd; ++it) {
							collector.consider(*it);
						}
					}
					for (cint64 listIndex = 0; listIndex < 2 && !state.mClashed; ++listIndex) {
						QList<CConcept*> implList;
						if (listIndex == 0) {
							implList = baseImplicationSet.toList();
						} else {
							implList = state.mImplicationList;
						}
						for (cint64 i = 0; i < implList.size() && !state.mClashed; ++i) {
							CConcept* implConcept = implList.at(i);
							CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
							if (!impliedLinker) {
								continue;
							}
							bool triggered = true;
							for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt; trigIt = trigIt->getNext()) {
								// a trigger counts when its concept is in the label positively, whatever the flag
								// of its operand linker says, see insertConceptReapplicationReturnTriggered
								if (!state.hasPositive(trigIt->getData())) {
									triggered = false;
									break;
								}
							}
							if (triggered) {
								bool concluded = impliedLinker->isNegated() ? state.hasNegative(impliedLinker->getData()) : state.hasPositive(impliedLinker->getData());
								if (!concluded) {
									++mFiredImplicationCount;
									if (!addMergeLabelConcept(state, impliedLinker->getData(), impliedLinker->isNegated(), true)) {
										return false;
									}
									changed = true;
								}
							}
						}
					}
					firstRound = false;
				}
				return true;
			}


			const CSaturationSubsumptionDecider::CMergeState* CSaturationSubsumptionDecider::getConjunctionMergeBase(const QList<CConcept*>& conjuncts) {
				quint64 key = 1469598103934665603ULL;
				for (CConcept* conjunct : conjuncts) {
					key = (key ^ (quint64)conjunct->getConceptTag()) * 1099511628211ULL;
				}
				QHash<quint64,CMergeState*>::const_iterator found = mConjunctionMergeBaseHash.constFind(key);
				while (found != mConjunctionMergeBaseHash.constEnd() && found.key() == key) {
					CMergeState* candidate = found.value();
					bool same = candidate->mConjunctTagList.size() == conjuncts.size();
					for (cint64 i = 0; same && i < conjuncts.size(); ++i) {
						same = candidate->mConjunctTagList.at(i) == conjuncts.at(i)->getConceptTag();
					}
					if (same) {
						return candidate->mClashed || !candidate->mImplicationList.isEmpty() || !candidate->mPositiveSet.isEmpty() ? candidate : nullptr;
					}
					++found;
				}
				if (mConjunctionMergeBaseHash.size() > 4096) {
					// the sides of the queries asked so far, enough for a session of an editor
					qDeleteAll(mConjunctionMergeBaseHash);
					mConjunctionMergeBaseHash.clear();
				}
				CMergeState* base = new CMergeState();
				for (CConcept* conjunct : conjuncts) {
					base->mConjunctTagList.append(conjunct->getConceptTag());
				}
				bool decided = true;
				for (CConcept* conjunct : conjuncts) {
					if (!addMergeNamedClass(*base, conjunct)) {
						decided = false;
						break;
					}
				}
				if (decided && !base->mClashed) {
					decided = closeMergeState(*base);
				}
				if (decided && !base->mClashed && !base->mCandidateList.isEmpty()) {
					// the equivalence candidates the closure brought along are resolved once here, as
					// optional classes, instead of in every merge with the base: what they add rests on
					// a choice, so the base then proves no clash any more
					CMergeState candidateState(base);
					if (closeMergeStateWithCandidates(candidateState) && !candidateState.mClashed) {
						base->mPositiveSet.unite(candidateState.mPositiveSet);
						base->mNegativeSet.unite(candidateState.mNegativeSet);
						base->mImplicationList.append(candidateState.mImplicationList);
						base->mUniversalList.append(candidateState.mUniversalList);
						base->mSelfConceptSet.unite(candidateState.mSelfConceptSet);
						base->mSuccessorRoleSet.unite(candidateState.mSuccessorRoleSet);
						base->mDerivedSuccessorRoleSet.unite(candidateState.mDerivedSuccessorRoleSet);
						base->mPartNodeList.append(candidateState.mPartNodeList);
						base->mPartNodeSet.unite(candidateState.mPartNodeSet);
						base->mCompletedSuccessorRoleTagSet.unite(candidateState.mCompletedSuccessorRoleTagSet);
						base->mDataRoleTagSet.unite(candidateState.mDataRoleTagSet);
						base->mCandidateList.clear();
						base->mHasNondeterministic = true;
					} else if (mCompletionConcept) {
						decided = false;
					}
				}
				if (decided && !base->mClashed) {
					// the implications that wait for something the base lacks, by the first such trigger
					for (CConcept* implConcept : base->mImplicationList) {
						CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
						if (!impliedLinker) {
							continue;
						}
						for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt; trigIt = trigIt->getNext()) {
							if (!base->hasPositive(trigIt->getData())) {
								base->mTriggerImplicationHash[trigIt->getData()].append(implConcept);
							}
						}
					}
				}
				if (!decided && mCompletionConcept) {
					// the base is built again once the label of the class is completed
					delete base;
					return nullptr;
				}
				if (!decided) {
					// an undecidable base is kept as an empty one, so that it is not built again
					delete base;
					base = new CMergeState();
					for (CConcept* conjunct : conjuncts) {
						base->mConjunctTagList.append(conjunct->getConceptTag());
					}
				}
				mConjunctionMergeBaseHash.insertMulti(key, base);
				return decided ? base : nullptr;
			}



			CSaturationSubsumptionDecider::FastVerdict CSaturationSubsumptionDecider::decideConjunctionSatisfiableFast(CIndividualSaturationProcessNode* baseNode, const CMergeState* base) {
				// The node's label is scanned once and nothing is copied: a concept that the base holds
				// in the other polarity is a clash, a construct the tableau has to handle leaves the
				// merge undecided, and an implication that the union would fire hands over to the
				// closure. What remains is checked for universal restrictions reaching successors of the
				// other side and for functional roles with successors on both sides.
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CIndividualSaturationProcessNodeStatusFlags* flags = repNode->getIndirectStatusFlags();
				if (flags->hasCardinalityRestrictedFlag() || flags->hasCardinalityProplematicFlag() || flags->hasNominalConnectionFlag()) {
					mUndecidedMergeConceptCodeCounts[-2200] = mUndecidedMergeConceptCodeCounts.value(-2200, 0) + 1;
					return FAST_UNDECIDED;
				}
				if (repNode->hasDataValueApplied()) {
					mUndecidedMergeConceptCodeCounts[-3000] = mUndecidedMergeConceptCodeCounts.value(-3000, 0) + 1;
					return FAST_UNDECIDED;
				}
				if (repNode->hasNominalIntegrated()) {
					mUndecidedMergeConceptCodeCounts[-3200] = mUndecidedMergeConceptCodeCounts.value(-3200, 0) + 1;
					return FAST_UNDECIDED;
				}
				CReapplyConceptSaturationLabelSet* labelSet = repNode->getReapplyConceptSaturationLabelSet(false);
				if (!labelSet) {
					mUndecidedMergeConceptCodeCounts[-3400] = mUndecidedMergeConceptCodeCounts.value(-3400, 0) + 1;
					return FAST_UNDECIDED;
				}
				QVarLengthArray<CConcept*,4> substituteArray;
				CIndividualSaturationProcessNode* node = baseNode;
				while (node->hasSubstituteIndividualNode()) {
					if (node != baseNode) {
						CSaturationConceptDataItem* satConDataItem = (CSaturationConceptDataItem*)node->getSaturationConceptReferenceLinking();
						if (satConDataItem && !satConDataItem->getSaturationNegation() && !satConDataItem->getSaturationRoleRanges()) {
							CConcept* substitute = satConDataItem->getSaturationConcept();
							if (base->hasNegative(substitute)) {
								return FAST_SUBSUMED;
							}
							if (base->mTriggerImplicationHash.contains(substitute)) {
								return FAST_FALLBACK;
							}
							substituteArray.append(substitute);
						}
					}
					node = node->getSubstituteIndividualNode();
				}
				// present in the union of the node and the base
				struct Presence {
					const CMergeState* mBase;
					CReapplyConceptSaturationLabelSet* mLabelSet;
					QVarLengthArray<CConcept*,4>* mSubstitutes;
					bool has(CConcept* concept, bool negated) const {
						if (negated ? mBase->hasNegative(concept) : mBase->hasPositive(concept)) {
							return true;
						}
						if (mLabelSet->hasConcept(concept, negated)) {
							return true;
						}
						if (!negated) {
							for (CConcept* substitute : *mSubstitutes) {
								if (substitute == concept) {
									return true;
								}
							}
						}
						return false;
					}
				} presence = { base, labelSet, &substituteArray };

				QVarLengthArray<CConcept*,16> implicationArray;
				QVarLengthArray<QPair<CConcept*,bool>,16> universalArray;
				QVarLengthArray<CConcept*,4> selfArray;
				bool candidateSeen = !base->mCandidateList.isEmpty();
				for (CConceptSaturationDescriptor* conDesIt = labelSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
					CConcept* concept = conDesIt->getConcept();
					bool negated = conDesIt->isNegated();
					if (negated ? base->hasPositive(concept) : base->hasNegative(concept)) {
						return FAST_SUBSUMED;
					}
					LabelConceptKind kind = getLabelConceptKind(concept, negated);
					if (kind == LCK_CANDIDATE) {
						candidateSeen = true;
					}
					if (kind == LCK_UNSAFE) {
						cint64 codeKey = concept->getOperatorCode() * 2 + (negated ? 1 : 0);
						mUndecidedMergeConceptCodeCounts[codeKey] = mUndecidedMergeConceptCodeCounts.value(codeKey, 0) + 1;
						return FAST_UNDECIDED;
					}
					if (kind == LCK_IMPLICATION) {
						implicationArray.append(concept);
					} else if (kind == LCK_UNIVERSAL) {
						universalArray.append(QPair<CConcept*,bool>(concept, negated));
					} else if (!negated && concept->getOperatorCode() == CCSELF) {
						selfArray.append(concept);
					}
					if (!negated && base->mTriggerImplicationHash.contains(concept)) {
						// the node supplies a trigger the base waited for
						for (CConcept* implConcept : base->mTriggerImplicationHash.value(concept)) {
							CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
							bool triggered = true;
							for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt && triggered; trigIt = trigIt->getNext()) {
								triggered = presence.has(trigIt->getData(), false);
							}
							if (triggered && !presence.has(impliedLinker->getData(), impliedLinker->isNegated())) {
								mUndecidedMergeConceptCodeCounts[-9600] = mUndecidedMergeConceptCodeCounts.value(-9600, 0) + 1;
								return FAST_FALLBACK;
							}
						}
					}
				}
				// an implication of the node that the base completes
				for (CConcept* implConcept : implicationArray) {
					CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
					if (!impliedLinker) {
						continue;
					}
					bool triggered = true;
					for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt && triggered; trigIt = trigIt->getNext()) {
						triggered = presence.has(trigIt->getData(), false);
					}
					if (triggered && !presence.has(impliedLinker->getData(), impliedLinker->isNegated())) {
						mUndecidedMergeConceptCodeCounts[-9800] = mUndecidedMergeConceptCodeCounts.value(-9800, 0) + 1;
						return FAST_FALLBACK;
					}
				}
				// successors under a functional role on both sides would be merged
				CLinkedRoleSaturationSuccessorHash* succHash = repNode->getLinkedRoleSuccessorHash(false);
				if (succHash) {
					CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* roleSuccHash = succHash->getLinkedRoleSuccessorHash();
					for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator it = roleSuccHash->constBegin(), itEnd = roleSuccHash->constEnd(); it != itEnd; ++it) {
						CRole* role = it.key();
						CLinkedRoleSaturationSuccessorData* roleSuccData = it.value();
						if (roleSuccData && roleSuccData->getSuccessorCount() > 0 && role->isFunctional() && base->hasSuccessorRole(role)) {
							mUndecidedMergeConceptCodeCounts[-2000] = mUndecidedMergeConceptCodeCounts.value(-2000, 0) + 1;
							return FAST_UNDECIDED;
						}
						if (roleSuccData && roleSuccData->getSuccessorCount() > 0 && role->isDataRole() && base->hasDataRoleTag(role->getRoleTag())) {
							mUndecidedMergeConceptCodeCounts[-2400] = mUndecidedMergeConceptCodeCounts.value(-2400, 0) + 1;
							return FAST_UNDECIDED;
						}
					}
				}
				// the universal restrictions of the base reach the node's successors and self loops
				for (const CMergeUniversal& universal : base->mUniversalList) {
					CRole* role = universal.mConcept->getRole();
					CSortedNegLinker<CConcept*>* opLinker = universal.mConcept->getOperandList();
					if (!role || !opLinker) {
						return FAST_UNDECIDED;
					}
					for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
						bool propagatedNegation = opIt->isNegated() ^ universal.mNegated;
						if (hasSuccessorWithoutConcept(repNode, role, opIt->getData(), propagatedNegation)) {
							mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
							return FAST_UNDECIDED;
						}
						for (CConcept* selfConcept : selfArray) {
							if (isSelfLoopOnRole(selfConcept, role) && !presence.has(opIt->getData(), propagatedNegation)) {
								mUndecidedMergeConceptCodeCounts[-6000] = mUndecidedMergeConceptCodeCounts.value(-6000, 0) + 1;
								return FAST_UNDECIDED;
							}
						}
					}
				}
				// and the node's universal restrictions reach the successors and self loops of the base
				for (const QPair<CConcept*,bool>& universal : universalArray) {
					CRole* role = universal.first->getRole();
					CSortedNegLinker<CConcept*>* opLinker = universal.first->getOperandList();
					if (!role || !opLinker) {
						return FAST_UNDECIDED;
					}
					if (base->hasCompletedSuccessorRoleTag(role->getRoleTag())) {
						// a successor of a completed part, whose label is not at hand
						mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
						return FAST_UNDECIDED;
					}
					for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
						bool propagatedNegation = opIt->isNegated() ^ universal.second;
						for (CIndividualSaturationProcessNode* partNode : base->mPartNodeList) {
							if (hasSuccessorWithoutConcept(partNode, role, opIt->getData(), propagatedNegation)) {
								mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
								return FAST_UNDECIDED;
							}
						}
						for (CConcept* selfConcept : base->mSelfConceptSet) {
							if (isSelfLoopOnRole(selfConcept, role) && !presence.has(opIt->getData(), propagatedNegation)) {
								mUndecidedMergeConceptCodeCounts[-6000] = mUndecidedMergeConceptCodeCounts.value(-6000, 0) + 1;
								return FAST_UNDECIDED;
							}
						}
					}
				}
				if (candidateSeen) {
					// the equivalence candidates have to be added before the merge is called satisfiable
					mUndecidedMergeConceptCodeCounts[-9900] = mUndecidedMergeConceptCodeCounts.value(-9900, 0) + 1;
					return FAST_FALLBACK;
				}
				return FAST_NOT_SUBSUMED;
			}



			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideConjunctionSatisfiable(CIndividualSaturationProcessNode* baseNode, bool reliable, const QList<CConcept*>& conjuncts) {
				if (!reliable) {
					return UNDECIDED;
				}
				const CMergeState* base = getConjunctionMergeBase(conjuncts);
				if (!base) {
					return mCompletionConcept ? NEEDS_COMPLETION : UNDECIDED;
				}
				if (base->mClashed) {
					// the conjunction has no instance on its own, so nothing has a common instance with it,
					// unless the clash rests on a choice of a completed label
					return base->hasNondeterministic() ? UNDECIDED : SUBSUMED;
				}
				FastVerdict fastVerdict = decideConjunctionSatisfiableFast(baseNode, base);
				if (fastVerdict != FAST_FALLBACK) {
					++mFastDecisionCount;
					if (fastVerdict == FAST_SUBSUMED && base->hasNondeterministic()) {
						return UNDECIDED;
					}
					return fastVerdict == FAST_SUBSUMED ? SUBSUMED : fastVerdict == FAST_NOT_SUBSUMED ? NOT_SUBSUMED : UNDECIDED;
				}
				++mClosureDecisionCount;
				Verdict closureVerdict = decideConjunctionSatisfiableClosure(baseNode, base);
				if (closureVerdict == UNDECIDED && mCompletionConcept) {
					return NEEDS_COMPLETION;
				}
				return closureVerdict;
			}



			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideConjunctionSatisfiableClosure(CIndividualSaturationProcessNode* baseNode, const CMergeState* base) {
				// The merged label starts as the union of the saturated labels of the parts, and is
				// closed under the rules the saturation applies at a node: a conjunction adds its
				// operands, an implication whose triggers are all present adds its conclusion, and a
				// named class that is derived brings its saturation along. The parts' successors are
				// the successors of the merged node. The closure clashes if a concept ends up positive
				// and negated, which is how an absorbed disjointness surfaces; otherwise the closure
				// with the successors is a model unless a universal restriction reaches a successor of
				// another part that lacks its operand, or a construct that only the tableau handles is
				// involved, in which case nothing is decided.
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CSaturationConceptDataItem* satConDataItem = (CSaturationConceptDataItem*)repNode->getSaturationConceptReferenceLinking();
				CConcept* baseConcept = satConDataItem ? satConDataItem->getSaturationConcept() : nullptr;
				if (!baseConcept || !baseConcept->hasClassName()) {
					// the node of the sub class candidate is always that of a named class
					return UNDECIDED;
				}
				CMergeState state(base);
				bool decided = overlayNodeIntoMergeState(state, baseNode);
				if (decided && !state.mClashed) {
					decided = closeMergeState(state);
				}
				if (state.mClashed) {
					// everything in the closure is entailed for the conjunction, so a clash is a proof,
					// unless a part came from a completed label with an entry that rests on a choice
					return state.hasNondeterministic() ? UNDECIDED : SUBSUMED;
				}
				if (!decided) {
					return UNDECIDED;
				}
				if (!closeMergeStateWithCandidates(state)) {
					return UNDECIDED;
				}
				if (state.mClashed) {
					// a clash with a candidate added proves nothing
					mUndecidedMergeConceptCodeCounts[-9200] = mUndecidedMergeConceptCodeCounts.value(-9200, 0) + 1;
					return UNDECIDED;
				}
				// a universal restriction reaches the successors of the other parts, and the derived ones
				QList<CIndividualSaturationProcessNode*> partNodeList = base->mPartNodeList + state.mPartNodeList;
				QSet<CConcept*> selfConceptSet = base->mSelfConceptSet + state.mSelfConceptSet;
				for (cint64 listIndex = 0; listIndex < 2; ++listIndex) {
					const QList<CMergeUniversal>& universalList = listIndex == 0 ? base->mUniversalList : state.mUniversalList;
					for (const CMergeUniversal& universal : universalList) {
						CRole* role = universal.mConcept->getRole();
						CSortedNegLinker<CConcept*>* opLinker = universal.mConcept->getOperandList();
						if (!role || !opLinker) {
							mUndecidedMergeConceptCodeCounts[-4000] = mUndecidedMergeConceptCodeCounts.value(-4000, 0) + 1;
							return UNDECIDED;
						}
						for (CRole* derivedRole : state.mDerivedSuccessorRoleSet) {
							if (derivedRole == role || derivedRole->getRoleTag() == role->getRoleTag()) {
								mUndecidedMergeConceptCodeCounts[-8000] = mUndecidedMergeConceptCodeCounts.value(-8000, 0) + 1;
								return UNDECIDED;
							}
							for (CSortedNegLinker<CRole*>* superIt = derivedRole->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
								if (!superIt->isNegated() && (superIt->getData() == role || superIt->getData()->getRoleTag() == role->getRoleTag())) {
									mUndecidedMergeConceptCodeCounts[-8000] = mUndecidedMergeConceptCodeCounts.value(-8000, 0) + 1;
									return UNDECIDED;
								}
							}
						}
						if (listIndex == 1 && base->hasCompletedSuccessorRoleTag(role->getRoleTag()) || listIndex == 0 && state.mCompletedSuccessorRoleTagSet.contains(role->getRoleTag())) {
							// a successor of a completed part, whose label is not at hand
							mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
							return UNDECIDED;
						}
						for (CIndividualSaturationProcessNode* partNode : partNodeList) {
							if (partNode == universal.mSourceNode) {
								continue;
							}
							if (listIndex == 0 && base->hasPartNode(partNode)) {
								// the base is consistent with its own successors already
								continue;
							}
							for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
								// a negated existential restriction propagates the negated filler
								bool propagatedNegation = opIt->isNegated() ^ universal.mNegated;
								if (hasSuccessorWithoutConcept(partNode, role, opIt->getData(), propagatedNegation)) {
									mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
									return UNDECIDED;
								}
							}
						}
						// a self loop makes the merged node its own successor
						for (CConcept* selfConcept : selfConceptSet) {
							if (isSelfLoopOnRole(selfConcept, role)) {
								for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
									bool propagatedNegation = opIt->isNegated() ^ universal.mNegated;
									bool present = propagatedNegation ? state.hasNegative(opIt->getData()) : state.hasPositive(opIt->getData());
									if (!present) {
										mUndecidedMergeConceptCodeCounts[-6000] = mUndecidedMergeConceptCodeCounts.value(-6000, 0) + 1;
										return UNDECIDED;
									}
								}
							}
						}
					}
				}
				return NOT_SUBSUMED;
			}




			bool CSaturationSubsumptionDecider::extendCompletedLabel(CCompletedLabel* label) {
				if (label->mExtended) {
					return true;
				}
				if (label->mExtensionFailed || label->mUnsatisfiable || label->mFailed || label->mExtending) {
					// an extension under way, of a label whose candidates lead back to it, is used as it is
					return false;
				}
				label->mExtending = true;
				CConcept* previousCompletionConcept = mCompletionConcept;
				mCompletionConcept = nullptr;
				CMergeState state;
				bool added = addCompletedLabelToMergeState(state, label);
				bool closed = added && !state.mClashed && closeMergeState(state) && !state.mClashed;
				QSet<CConcept*> ownPositiveSet = state.mPositiveSet;
				QSet<CConcept*> ownNegativeSet = state.mNegativeSet;
				bool candidatesClosed = closed && closeMergeStateWithCandidates(state);
				bool extended = candidatesClosed && !state.mClashed;
				label->mExtending = false;
				if (!extended) {
					if (mCompletionConcept) {
						// a candidate needs its label first, the extension is tried again after
						mUndecidedMergeConceptCodeCounts[-10800] = mUndecidedMergeConceptCodeCounts.value(-10800, 0) + 1;
						return false;
					}
					cint64 reasonKey = !added ? -10200 : !closed ? -10400 : !candidatesClosed ? -10600 : -10000;
					mUndecidedMergeConceptCodeCounts[reasonKey] = mUndecidedMergeConceptCodeCounts.value(reasonKey, 0) + 1;
					mCompletionConcept = previousCompletionConcept;
					label->mExtensionFailed = true;
					return false;
				}
				mUndecidedMergeConceptCodeCounts[-11000] = mUndecidedMergeConceptCodeCounts.value(-11000, 0) + 1;
				mCompletionConcept = previousCompletionConcept;
				// the entries: the original ones as they were, without the candidate markers; what the
				// closure of the label alone derived as deterministic as the label; what came with the
				// candidates as resting on a choice
				bool labelDeterministic = !label->mHasNondeterministic;
				QVector<CCompletedLabelEntry> entries;
				QHash<CConcept*,quint8> seen;
				for (const CCompletedLabelEntry& entry : label->mEntries) {
					if (getLabelConceptKind(entry.mConcept, entry.mNegated) == LCK_CANDIDATE) {
						continue;
					}
					entries.append(entry);
					seen[entry.mConcept] |= entry.mNegated ? 2 : 1;
				}
				for (cint64 polarity = 0; polarity < 2; ++polarity) {
					bool negated = polarity == 1;
					const QSet<CConcept*>& conceptSet = negated ? state.mNegativeSet : state.mPositiveSet;
					const QSet<CConcept*>& ownSet = negated ? ownNegativeSet : ownPositiveSet;
					for (QSet<CConcept*>::const_iterator it = conceptSet.constBegin(), itEnd = conceptSet.constEnd(); it != itEnd; ++it) {
						CConcept* concept(*it);
						if ((seen.value(concept, 0) & (negated ? 2 : 1)) != 0 || getLabelConceptKind(concept, negated) == LCK_CANDIDATE) {
							continue;
						}
						entries.append(CCompletedLabelEntry(concept, negated, labelDeterministic && ownSet.contains(concept)));
						seen[concept] |= negated ? 2 : 1;
					}
				}
				QSet<CRole*> roleSet;
				for (CRole* role : label->mSuccessorRoleList) {
					roleSet.insert(role);
				}
				roleSet.unite(state.mSuccessorRoleSet);
				roleSet.unite(state.mDerivedSuccessorRoleSet);
				QSet<cint64> dataRoleTagSet = label->mDataRoleTagSet;
				dataRoleTagSet.unite(state.mDataRoleTagSet);
				setCompletedLabel(label->mConcept, entries);
				for (QSet<CRole*>::const_iterator it = roleSet.constBegin(), itEnd = roleSet.constEnd(); it != itEnd; ++it) {
					CRole* role(*it);
					if (role->isDataRole()) {
						collectRoleTagsWithSuperRoles(role, label->mDataRoleTagSet);
					} else if (!label->mSuccessorRoleList.contains(role)) {
						label->mSuccessorRoleList.append(role);
						collectRoleTagsWithSuperRoles(role, label->mSuccessorRoleTagSet);
					}
				}
				label->mDataRoleTagSet.unite(dataRoleTagSet);
				label->mExtended = true;
				return true;
			}


			bool CSaturationSubsumptionDecider::addCompletedLabelToMergeState(CMergeState& state, CCompletedLabel* label) {
				// the label already holds every consequence, so nothing is derived from its named
				// classes; its successors are known by their roles only
				if (label->mHasNondeterministic) {
					state.mHasNondeterministic = true;
				}
				CIndividualSaturationProcessNode* previousPartNode = state.mCurrentPartNode;
				state.mCurrentPartNode = nullptr;
				for (const CCompletedLabelEntry& entry : label->mEntries) {
					cint64 entryOpCode = entry.mConcept->getOperatorCode();
					if ((entryOpCode == CCOR && !entry.mNegated) || (entryOpCode == CCAND && entry.mNegated)) {
						// a disjunction is an unresolved choice in a saturation label, but a completed label
						// is the root label of a model, which holds the disjunct it chose as an entry of its
						// own, so the disjunction is satisfied and adds nothing (issue #33)
						mUndecidedMergeConceptCodeCounts[-13000] = mUndecidedMergeConceptCodeCounts.value(-13000, 0) + 1;
						continue;
					}
					if (!addMergeLabelConcept(state, entry.mConcept, entry.mNegated, false)) {
						state.mCurrentPartNode = previousPartNode;
						return false;
					}
				}
				state.mCurrentPartNode = previousPartNode;
				if (hasSuccessorRoleConflict(label->mSuccessorRoleList, label->mDataRoleTagSet, state)) {
					return false;
				}
				for (CRole* role : label->mSuccessorRoleList) {
					state.mSuccessorRoleSet.insert(role);
					for (CSortedNegLinker<CRole*>* superIt = role->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
						if (!superIt->isNegated()) {
							state.mSuccessorRoleSet.insert(superIt->getData());
						}
					}
				}
				state.mCompletedSuccessorRoleTagSet.unite(label->mSuccessorRoleTagSet);
				state.mDataRoleTagSet.unite(label->mDataRoleTagSet);
				return true;
			}



			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideEntailedCompleted(CCompletedLabel* label, CConcept* concept, bool negated, cint64 depth) {
				// as decideEntailed, over a completed label: an entry that rests on a choice is present in
				// one model, so it neither proves the entailment nor the absence of it
				if (depth > 64) {
					return UNDECIDED;
				}
				cint64 opCode = concept->getOperatorCode();
				CSortedNegLinker<CConcept*>* opLinker = concept->getOperandList();
				if (opCode == CCNOT && opLinker) {
					return decideEntailedCompleted(label, opLinker->getData(), !(opLinker->isNegated() ^ negated), depth + 1);
				}
				cint64 absCode = opCode < 0 ? -opCode : opCode;
				bool existential = (absCode == 5 || absCode == 11) && ((opCode < 0) != negated);
				if (existential) {
					// the completed label is the root label of a model, whose successors come from the
					// existential and cardinality restrictions among its entries; a model in which the root
					// has no successor over the role, nor a self loop on it, refutes the existential
					// restriction (issue #33)
					CRole* role = concept->getRole();
					// over a role that chains or transitivity lead to, a successor may exist without a
					// restriction among the entries, so the model cannot be read this way
					bool materialised = role && !role->isComplexRole() && !role->isTransitive() && !role->hasRoleChainSuperSharing() && !role->hasRoleChainSubSharing();
					if (role && materialised && !role->isDataRole() && !label->mSuccessorRoleTagSet.contains(role->getRoleTag())) {
						bool selfLoop = false;
						for (const CCompletedLabelEntry& entry : label->mEntries) {
							if (!entry.mNegated && entry.mConcept->getOperatorCode() == CCSELF && isSelfLoopOnRole(entry.mConcept, role)) {
								selfLoop = true;
								break;
							}
						}
						if (!selfLoop) {
							mUndecidedMergeConceptCodeCounts[-13008] = mUndecidedMergeConceptCodeCounts.value(-13008, 0) + 1;
							return NOT_SUBSUMED;
						}
					}
					mUndecidedMergeConceptCodeCounts[-13010] = mUndecidedMergeConceptCodeCounts.value(-13010, 0) + 1;
					return UNDECIDED;
				}
				if (opCode == CCAQCHOOCE && !negated && concept->getRole()) {
					// an existential restriction over a role with chains, transformed into the automaton's
					// choose concept: the model refutes it when its root has no successor over any role a
					// word of the automaton can start with, the role itself, its sub roles, and the first
					// roles of the chains leading to it or to its sub roles, each closed under the same
					// rule; the root of a completion has no predecessors, so no word starts there with an
					// inverse role (issue #19)
					CRole* role = concept->getRole();
					QSet<CRole*> firstRoleSet;
					bool decidable = !role->isDataRole();
					QHash<CRole*,QPair<bool,QSet<CRole*> > >::const_iterator cachedIt = mAutomatonFirstRoleHash.constFind(role);
					bool cached = cachedIt != mAutomatonFirstRoleHash.constEnd();
					if (cached) {
						decidable = cachedIt.value().first;
						firstRoleSet = cachedIt.value().second;
					}
					QList<CRole*> workList;
					if (!cached) {
						workList.append(role);
						firstRoleSet.insert(role);
					}
					while (!workList.isEmpty() && decidable) {
						CRole* currentRole = workList.takeFirst();
						// the role and its sub roles, found over the roles' super role lists, since a role
						// does not list its sub roles
						QList<CRole*> producingRoleList;
						producingRoleList.append(currentRole);
						CRoleVector* roleVector = mOntology->getRBox()->getRoleVector(false);
						cint64 roleCount = roleVector ? roleVector->getItemCount() : 0;
						for (cint64 roleIndex = 0; roleIndex < roleCount; ++roleIndex) {
							CRole* candidateRole = roleVector->getData(roleIndex);
							if (candidateRole && candidateRole != currentRole) {
								for (CSortedNegLinker<CRole*>* superIt = candidateRole->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
									if (!superIt->isNegated() && superIt->getData() == currentRole) {
										producingRoleList.append(candidateRole);
										break;
									}
								}
							}
						}
						for (CRole* producingRole : producingRoleList) {
							for (CXLinker<CRoleChain*>* chainIt = producingRole->getRoleChainSuperSharingLinker(); chainIt && decidable; chainIt = chainIt->getNext()) {
								CXLinker<CRole*>* chainRoleLinker = chainIt->getData()->getRoleChainLinker();
								if (!chainRoleLinker) {
									continue;
								}
								CRole* firstRole = chainRoleLinker->getData();
								if (decidable && !firstRoleSet.contains(firstRole)) {
									firstRoleSet.insert(firstRole);
									workList.append(firstRole);
								}
							}
						}
					}
					if (!cached) {
						mAutomatonFirstRoleHash.insert(role, QPair<bool,QSet<CRole*> >(decidable, firstRoleSet));
					}
					if (decidable) {
						bool reachable = false;
						for (CRole* firstRole : firstRoleSet) {
							if (label->mSuccessorRoleTagSet.contains(firstRole->getRoleTag())) {
								reachable = true;
								break;
							}
							for (const CCompletedLabelEntry& entry : label->mEntries) {
								if (!entry.mNegated && entry.mConcept->getOperatorCode() == CCSELF && isSelfLoopOnRole(entry.mConcept, firstRole)) {
									reachable = true;
									break;
								}
							}
							if (reachable) {
								break;
							}
						}
						if (!reachable) {
							mUndecidedMergeConceptCodeCounts[-13040] = mUndecidedMergeConceptCodeCounts.value(-13040, 0) + 1;
							return NOT_SUBSUMED;
						}
					}
					mUndecidedMergeConceptCodeCounts[-13042] = mUndecidedMergeConceptCodeCounts.value(-13042, 0) + 1;
					return UNDECIDED;
				}
				if (!negated) {
					if (opCode == CCTOP) {
						return SUBSUMED;
					}
					if (opCode == CCBOTTOM) {
						return NOT_SUBSUMED;
					}
					if (concept->hasClassName()) {
						if (label->hasDeterministicEntry(concept, false)) {
							return SUBSUMED;
						}
						// a model need not name every subsumer at its root, the class hierarchy does
						if (mRootSubClassConcept) {
							Verdict hierarchyVerdict = decideNamedSubsumptionFromHierarchy(mRootSubClassConcept, concept);
							if (hierarchyVerdict != UNDECIDED) {
								return hierarchyVerdict;
							}
						}
						return UNDECIDED;
					}
					if (opCode == CCAND) {
						bool undecided = false;
						for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
							Verdict opVerdict = decideEntailedCompleted(label, opIt->getData(), opIt->isNegated(), depth + 1);
							if (opVerdict == NOT_SUBSUMED || opVerdict == NEEDS_COMPLETION) {
								return opVerdict;
							} else if (opVerdict == UNDECIDED) {
								undecided = true;
							}
						}
						return undecided ? UNDECIDED : SUBSUMED;
					}
					if (opCode == CCOR) {
						for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
							if (decideEntailedCompleted(label, opIt->getData(), opIt->isNegated(), depth + 1) == SUBSUMED) {
								return SUBSUMED;
							}
						}
						return UNDECIDED;
					}
					if (label->hasDeterministicEntry(concept, false)) {
						return SUBSUMED;
					}
					return UNDECIDED;
				} else {
					if (opCode == CCTOP) {
						return NOT_SUBSUMED;
					}
					if (opCode == CCBOTTOM) {
						return SUBSUMED;
					}
					if (opCode == CCOR) {
						bool undecided = false;
						for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
							Verdict opVerdict = decideEntailedCompleted(label, opIt->getData(), !opIt->isNegated(), depth + 1);
							if (opVerdict == NOT_SUBSUMED || opVerdict == NEEDS_COMPLETION) {
								return opVerdict;
							} else if (opVerdict == UNDECIDED) {
								undecided = true;
							}
						}
						return undecided ? UNDECIDED : SUBSUMED;
					}
					if (label->hasDeterministicEntry(concept, true)) {
						return SUBSUMED;
					}
					if (concept->hasClassName() || opCode == CCAND) {
						if (opCode == CCAND) {
							for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
								Verdict opVerdict = decideEntailedCompleted(label, opIt->getData(), !opIt->isNegated(), depth + 1);
								if (opVerdict == SUBSUMED || opVerdict == NEEDS_COMPLETION) {
									return opVerdict;
								}
							}
						}
						QList<CConcept*> conjuncts;
						if (collectNamedConjuncts(concept, false, conjuncts, 0)) {
							const CMergeState* base = getConjunctionMergeBase(conjuncts);
							if (!base) {
								return mCompletionConcept ? NEEDS_COMPLETION : UNDECIDED;
							}
							if (base->mClashed) {
								return base->hasNondeterministic() ? UNDECIDED : SUBSUMED;
							}
							Verdict labelVerdict = decideCompletedLabelSatisfiable(label, base);
							if (labelVerdict == UNDECIDED && mCompletionConcept) {
								return NEEDS_COMPLETION;
							}
							return labelVerdict;
						}
					}
					return UNDECIDED;
				}
			}


			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideCompletedLabelSatisfiable(CCompletedLabel* label, const CMergeState* base) {
				// the fast scan over a completed label: a clash proves the subsumption only when the
				// entry and the query's side do not rest on a choice, and a successor of the label is
				// known by its role only, so a universal restriction that could reach it leaves the
				// merge to the tableau
				++mCompletedDecisionCount;
				if (!extendCompletedLabel(label) && mCompletionConcept) {
					return UNDECIDED;
				}
				bool nondeterministic = base->hasNondeterministic();
				bool candidateSeen = !base->mCandidateList.isEmpty();
				QVarLengthArray<CConcept*,16> implicationArray;
				QVarLengthArray<QPair<CConcept*,bool>,16> universalArray;
				QVarLengthArray<CConcept*,4> selfArray;
				struct Presence {
					const CMergeState* mBase;
					const CCompletedLabel* mLabel;
					bool has(CConcept* concept, bool negated) const {
						return (negated ? mBase->hasNegative(concept) : mBase->hasPositive(concept)) || mLabel->hasEntry(concept, negated);
					}
				} presence = { base, label };
				for (const CCompletedLabelEntry& entry : label->mEntries) {
					CConcept* concept = entry.mConcept;
					bool negated = entry.mNegated;
					if (negated ? base->hasPositive(concept) : base->hasNegative(concept)) {
						return (nondeterministic || !entry.mDeterministic) ? UNDECIDED : SUBSUMED;
					}
					cint64 entryOpCode = concept->getOperatorCode();
					if ((entryOpCode == CCOR && !negated) || (entryOpCode == CCAND && negated)) {
						// satisfied by the disjunct the model chose, which is an entry of its own (issue #33)
						mUndecidedMergeConceptCodeCounts[-13002] = mUndecidedMergeConceptCodeCounts.value(-13002, 0) + 1;
						continue;
					}
					LabelConceptKind kind = getLabelConceptKind(concept, negated);
					if (kind == LCK_UNSAFE) {
						cint64 codeKey = concept->getOperatorCode() * 2 + (negated ? 1 : 0);
						mUndecidedMergeConceptCodeCounts[codeKey] = mUndecidedMergeConceptCodeCounts.value(codeKey, 0) + 1;
						return UNDECIDED;
					}
					if (kind == LCK_CANDIDATE) {
						candidateSeen = true;
					}
					if (kind == LCK_IMPLICATION) {
						implicationArray.append(concept);
					} else if (kind == LCK_UNIVERSAL) {
						universalArray.append(QPair<CConcept*,bool>(concept, negated));
					} else if (!negated && concept->getOperatorCode() == CCSELF) {
						selfArray.append(concept);
					}
					if (!negated && base->mTriggerImplicationHash.contains(concept)) {
						for (CConcept* implConcept : base->mTriggerImplicationHash.value(concept)) {
							CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
							bool triggered = true;
							for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt && triggered; trigIt = trigIt->getNext()) {
								triggered = presence.has(trigIt->getData(), false);
							}
							if (triggered && !presence.has(impliedLinker->getData(), impliedLinker->isNegated())) {
								return decideCompletedLabelSatisfiableClosure(label, base);
							}
						}
					}
				}
				for (CConcept* implConcept : implicationArray) {
					CSortedNegLinker<CConcept*>* impliedLinker = implConcept->getOperandList();
					if (!impliedLinker) {
						continue;
					}
					bool triggered = true;
					for (CSortedNegLinker<CConcept*>* trigIt = impliedLinker->getNext(); trigIt && triggered; trigIt = trigIt->getNext()) {
						triggered = presence.has(trigIt->getData(), false);
					}
					if (triggered && !presence.has(impliedLinker->getData(), impliedLinker->isNegated())) {
						return decideCompletedLabelSatisfiableClosure(label, base);
					}
				}
				if (hasSuccessorRoleConflict(label->mSuccessorRoleList, label->mDataRoleTagSet, *base)) {
					return UNDECIDED;
				}
				// the universal restrictions of the base reach the label's successors and self loops
				for (const CMergeUniversal& universal : base->mUniversalList) {
					CRole* role = universal.mConcept->getRole();
					CSortedNegLinker<CConcept*>* opLinker = universal.mConcept->getOperandList();
					if (!role || !opLinker) {
						return UNDECIDED;
					}
					if (label->mSuccessorRoleTagSet.contains(role->getRoleTag())) {
						if (!mCompletedSuccessorDecision || !completedLabelSuccessorsSatisfyUniversal(label, base, role, universal.mConcept, universal.mNegated)) {
							mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
							return UNDECIDED;
						}
						// satisfied by the saturations of the label's existential restrictions
						mUndecidedMergeConceptCodeCounts[-10600] = mUndecidedMergeConceptCodeCounts.value(-10600, 0) + 1;
					}
					for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
						bool propagatedNegation = opIt->isNegated() ^ universal.mNegated;
						for (CConcept* selfConcept : selfArray) {
							if (isSelfLoopOnRole(selfConcept, role) && !presence.has(opIt->getData(), propagatedNegation)) {
								mUndecidedMergeConceptCodeCounts[-6000] = mUndecidedMergeConceptCodeCounts.value(-6000, 0) + 1;
								return UNDECIDED;
							}
						}
					}
				}
				// and the label's universal restrictions reach the successors and self loops of the base
				for (const QPair<CConcept*,bool>& universal : universalArray) {
					CRole* role = universal.first->getRole();
					CSortedNegLinker<CConcept*>* opLinker = universal.first->getOperandList();
					if (!role || !opLinker) {
						return UNDECIDED;
					}
					if (base->hasCompletedSuccessorRoleTag(role->getRoleTag())) {
						mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
						return UNDECIDED;
					}
					for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
						bool propagatedNegation = opIt->isNegated() ^ universal.second;
						for (CIndividualSaturationProcessNode* partNode : base->mPartNodeList) {
							if (hasSuccessorWithoutConcept(partNode, role, opIt->getData(), propagatedNegation)) {
								mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
								return UNDECIDED;
							}
						}
						for (CConcept* selfConcept : base->mSelfConceptSet) {
							if (isSelfLoopOnRole(selfConcept, role) && !presence.has(opIt->getData(), propagatedNegation)) {
								mUndecidedMergeConceptCodeCounts[-6000] = mUndecidedMergeConceptCodeCounts.value(-6000, 0) + 1;
								return UNDECIDED;
							}
						}
					}
				}
				if (candidateSeen) {
					return decideCompletedLabelSatisfiableClosure(label, base);
				}
				return NOT_SUBSUMED;
			}


			CSaturationSubsumptionDecider::Verdict CSaturationSubsumptionDecider::decideCompletedLabelSatisfiableClosure(CCompletedLabel* label, const CMergeState* base) {
				// the closure with a completed label as the node's part
				++mClosureDecisionCount;
				CMergeState state(base);
				if (!overlayCompletedLabelIntoMergeState(state, label)) {
					return UNDECIDED;
				}
				if (!closeMergeState(state)) {
					return UNDECIDED;
				}
				if (state.mClashed) {
					return state.hasNondeterministic() ? UNDECIDED : SUBSUMED;
				}
				if (!closeMergeStateWithCandidates(state)) {
					return UNDECIDED;
				}
				if (state.mClashed) {
					mUndecidedMergeConceptCodeCounts[-9200] = mUndecidedMergeConceptCodeCounts.value(-9200, 0) + 1;
					return UNDECIDED;
				}
				QSet<CConcept*> selfConceptSet = base->mSelfConceptSet + state.mSelfConceptSet;
				for (cint64 listIndex = 0; listIndex < 2; ++listIndex) {
					const QList<CMergeUniversal>& universalList = listIndex == 0 ? base->mUniversalList : state.mUniversalList;
					for (const CMergeUniversal& universal : universalList) {
						CRole* role = universal.mConcept->getRole();
						CSortedNegLinker<CConcept*>* opLinker = universal.mConcept->getOperandList();
						if (!role || !opLinker) {
							return UNDECIDED;
						}
						cint64 roleTag = role->getRoleTag();
						// a successor known by its role only, or one derived in the closure
						if (listIndex == 0 && (state.mCompletedSuccessorRoleTagSet.contains(roleTag)) || listIndex == 1 && (base->hasCompletedSuccessorRoleTag(roleTag) || state.mCompletedSuccessorRoleTagSet.contains(roleTag))) {
							mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
							return UNDECIDED;
						}
						for (CRole* derivedRole : state.mDerivedSuccessorRoleSet) {
							QSet<cint64> derivedTagSet;
							collectRoleTagsWithSuperRoles(derivedRole, derivedTagSet);
							if (derivedTagSet.contains(roleTag)) {
								mUndecidedMergeConceptCodeCounts[-8000] = mUndecidedMergeConceptCodeCounts.value(-8000, 0) + 1;
								return UNDECIDED;
							}
						}
						if (listIndex == 1) {
							for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
								bool propagatedNegation = opIt->isNegated() ^ universal.mNegated;
								for (CIndividualSaturationProcessNode* partNode : base->mPartNodeList) {
									if (hasSuccessorWithoutConcept(partNode, role, opIt->getData(), propagatedNegation)) {
										mUndecidedMergeConceptCodeCounts[-5000] = mUndecidedMergeConceptCodeCounts.value(-5000, 0) + 1;
										return UNDECIDED;
									}
								}
							}
						}
						for (CConcept* selfConcept : selfConceptSet) {
							if (isSelfLoopOnRole(selfConcept, role)) {
								for (CSortedNegLinker<CConcept*>* opIt = opLinker; opIt; opIt = opIt->getNext()) {
									bool propagatedNegation = opIt->isNegated() ^ universal.mNegated;
									bool present = propagatedNegation ? state.hasNegative(opIt->getData()) : state.hasPositive(opIt->getData());
									if (!present) {
										mUndecidedMergeConceptCodeCounts[-6000] = mUndecidedMergeConceptCodeCounts.value(-6000, 0) + 1;
										return UNDECIDED;
									}
								}
							}
						}
					}
				}
				return NOT_SUBSUMED;
			}


			cint64 CSaturationSubsumptionDecider::getCompletionRequestCount() {
				return mCompletionRequestCount;
			}


			cint64 CSaturationSubsumptionDecider::getCompletedDecisionCount() {
				return mCompletedDecisionCount;
			}



			bool CSaturationSubsumptionDecider::overlayNodeIntoMergeState(CMergeState& state, CIndividualSaturationProcessNode* baseNode) {
				CIndividualSaturationProcessNode* repNode = getRepresentativeNode(baseNode);
				CIndividualSaturationProcessNodeStatusFlags* flags = repNode->getIndirectStatusFlags();
				if (flags->hasCardinalityRestrictedFlag() || flags->hasCardinalityProplematicFlag() || flags->hasNominalConnectionFlag()) {
					return false;
				}
				if (repNode->hasDataValueApplied() || repNode->hasNominalIntegrated()) {
					return false;
				}
				CReapplyConceptSaturationLabelSet* labelSet = repNode->getReapplyConceptSaturationLabelSet(false);
				if (!labelSet) {
					return false;
				}
				if (state.hasPartNode(repNode)) {
					return true;
				}
				state.mPartNodeSet.insert(repNode);
				state.mPartNodeList.append(repNode);
				state.mOverlayLabelSet = labelSet;
				CIndividualSaturationProcessNode* node = baseNode;
				while (node->hasSubstituteIndividualNode()) {
					if (node != baseNode) {
						CSaturationConceptDataItem* satConDataItem = (CSaturationConceptDataItem*)node->getSaturationConceptReferenceLinking();
						if (satConDataItem && !satConDataItem->getSaturationNegation() && !satConDataItem->getSaturationRoleRanges()) {
							CConcept* substitute = satConDataItem->getSaturationConcept();
							if (state.hasNegativeBesidesOverlay(substitute)) {
								state.mClashed = true;
								return true;
							}
							state.mOverlaySubstituteArray.append(substitute);
						}
					}
					node = node->getSubstituteIndividualNode();
				}
				for (CConceptSaturationDescriptor* conDesIt = labelSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
					CConcept* concept = conDesIt->getConcept();
					bool negated = conDesIt->isNegated();
					if (negated ? state.hasPositiveBesidesOverlay(concept) : state.hasNegativeBesidesOverlay(concept)) {
						state.mClashed = true;
						return true;
					}
					LabelConceptKind kind = getLabelConceptKind(concept, negated);
					cint64 opCode = concept->getOperatorCode();
					if (kind == LCK_UNSAFE) {
						cint64 codeKey = opCode * 2 + (negated ? 1 : 0);
						mUndecidedMergeConceptCodeCounts[codeKey] = mUndecidedMergeConceptCodeCounts.value(codeKey, 0) + 1;
						return false;
					}
					if (kind == LCK_IMPLICATION) {
						state.mImplicationList.append(concept);
					} else if (kind == LCK_UNIVERSAL) {
						state.mUniversalList.append(CMergeUniversal(concept, negated, repNode));
					} else if (kind == LCK_CANDIDATE) {
						CSortedNegLinker<CConcept*>* candidateLinker = concept->getOperandList();
						if (!candidateLinker || candidateLinker->isNegated() || !candidateLinker->getData()->hasClassName()) {
							mUndecidedMergeConceptCodeCounts[-9000] = mUndecidedMergeConceptCodeCounts.value(-9000, 0) + 1;
							return false;
						}
						state.mCandidateList.append(candidateLinker->getData());
					} else if (!negated && opCode == CCSELF) {
						state.mSelfConceptSet.insert(concept);
					}
				}
				// successors under a functional role would be merged with those of the other parts
				CLinkedRoleSaturationSuccessorHash* succHash = repNode->getLinkedRoleSuccessorHash(false);
				if (succHash) {
					CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* roleSuccHash = succHash->getLinkedRoleSuccessorHash();
					for (CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>::const_iterator it = roleSuccHash->constBegin(), itEnd = roleSuccHash->constEnd(); it != itEnd; ++it) {
						CRole* role = it.key();
						CLinkedRoleSaturationSuccessorData* roleSuccData = it.value();
						if (roleSuccData && roleSuccData->getSuccessorCount() > 0) {
							if (role->isFunctional() && state.hasSuccessorRole(role)) {
								mUndecidedMergeConceptCodeCounts[-2000] = mUndecidedMergeConceptCodeCounts.value(-2000, 0) + 1;
								return false;
							}
							if (role->isDataRole()) {
								if (state.hasDataRoleTag(role->getRoleTag())) {
									mUndecidedMergeConceptCodeCounts[-2400] = mUndecidedMergeConceptCodeCounts.value(-2400, 0) + 1;
									return false;
								}
								state.mDataRoleTagSet.insert(role->getRoleTag());
							}
							state.mSuccessorRoleSet.insert(role);
						}
					}
				}
				return true;
			}


			bool CSaturationSubsumptionDecider::overlayCompletedLabelIntoMergeState(CMergeState& state, CCompletedLabel* label) {
				if (label->mHasNondeterministic) {
					state.mHasNondeterministic = true;
				}
				state.mOverlayPolarityHash = &label->mPolarityHash;
				state.mOverlayEntries = &label->mEntries;
				for (const CCompletedLabelEntry& entry : label->mEntries) {
					CConcept* concept = entry.mConcept;
					bool negated = entry.mNegated;
					if (negated ? state.hasPositiveBesidesOverlay(concept) : state.hasNegativeBesidesOverlay(concept)) {
						state.mClashed = true;
						return true;
					}
					LabelConceptKind kind = getLabelConceptKind(concept, negated);
					cint64 opCode = concept->getOperatorCode();
					if ((opCode == CCOR && !negated) || (opCode == CCAND && negated)) {
						// satisfied by the disjunct the model chose, which is an entry of its own (issue #33)
						mUndecidedMergeConceptCodeCounts[-13006] = mUndecidedMergeConceptCodeCounts.value(-13006, 0) + 1;
						continue;
					}
					if (kind == LCK_UNSAFE) {
						cint64 codeKey = opCode * 2 + (negated ? 1 : 0);
						mUndecidedMergeConceptCodeCounts[codeKey] = mUndecidedMergeConceptCodeCounts.value(codeKey, 0) + 1;
						return false;
					}
					if (kind == LCK_IMPLICATION) {
						state.mImplicationList.append(concept);
					} else if (kind == LCK_UNIVERSAL) {
						state.mUniversalList.append(CMergeUniversal(concept, negated, nullptr));
					} else if (kind == LCK_CANDIDATE) {
						CSortedNegLinker<CConcept*>* candidateLinker = concept->getOperandList();
						if (!candidateLinker || candidateLinker->isNegated() || !candidateLinker->getData()->hasClassName()) {
							mUndecidedMergeConceptCodeCounts[-9000] = mUndecidedMergeConceptCodeCounts.value(-9000, 0) + 1;
							return false;
						}
						state.mCandidateList.append(candidateLinker->getData());
					} else if (!negated && opCode == CCSELF) {
						state.mSelfConceptSet.insert(concept);
					}
				}
				if (hasSuccessorRoleConflict(label->mSuccessorRoleList, label->mDataRoleTagSet, state)) {
					return false;
				}
				for (CRole* role : label->mSuccessorRoleList) {
					state.mSuccessorRoleSet.insert(role);
					for (CSortedNegLinker<CRole*>* superIt = role->getIndirectSuperRoleList(); superIt; superIt = superIt->getNext()) {
						if (!superIt->isNegated()) {
							state.mSuccessorRoleSet.insert(superIt->getData());
						}
					}
				}
				state.mCompletedSuccessorRoleTagSet.unite(label->mSuccessorRoleTagSet);
				state.mDataRoleTagSet.unite(label->mDataRoleTagSet);
				return true;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
