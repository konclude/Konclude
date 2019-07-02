/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CBranchingMergingProcessingRestrictionSpecification.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBranchingMergingProcessingRestrictionSpecification::CBranchingMergingProcessingRestrictionSpecification(CProcessContext* processContext) {
					mProcessContext = processContext;
					mDistinctMergedNodesSet = nullptr;
					mMergingNodesLinker = nullptr;
					mNominalMergingNodesLinker = nullptr;
					mMergingInitNodesLinker = nullptr;
					mOnlyPosQualifyNodesLinker = nullptr;
					mOnlyNegQualifyNodesLinker = nullptr;
					mBothQualifyNodesLinker = nullptr;
					mLastDistinctMergedNodesSet = nullptr;
					mIndiLink = nullptr;
					mRemainingLinkerMergingCandidateIndiNodeCount = 0;
					mRemainingValidMergingCandidateIndiNodeCount = 0;
					mDistinctSetFixed = false;
					mHasMergingInitCandidates = false;
					mRemainingNominalCreationCount = 0;
					mAddedBlockablePredMergingNodeCandidate = false;
					mAddedBlockablePredDepTrackPoint = nullptr;
					mDependencyTrackPoint = nullptr;
					mMergingDependencyNode = nullptr;
					mInitMergingNodesClashes = nullptr;
					mMultipleInitMergingNodesClashes = nullptr;

					mDistinctSetNodeRelocated = false;

					//mList.clear();
					//mList.append(QString(""));
					//mList.append(QString(""));
					//mList.append(QString(""));
				}



				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::initBranchingMergingProcessingRestriction(CBranchingMergingProcessingRestrictionSpecification* prevRest) {
					CProcessingRestrictionSpecification::initProcessingRestriction(prevRest);
					if (prevRest) {
						mDistinctMergedNodesSet = nullptr;
						mLastDistinctMergedNodesSet = nullptr;
						if (prevRest->mLastDistinctMergedNodesSet) {
							mLastDistinctMergedNodesSet = prevRest->mLastDistinctMergedNodesSet;
						}
						mNominalMergingNodesLinker = prevRest->mNominalMergingNodesLinker;
						mMergingNodesLinker = prevRest->mMergingNodesLinker;
						mMergingInitNodesLinker = prevRest->mMergingInitNodesLinker;
						mOnlyPosQualifyNodesLinker = prevRest->mOnlyPosQualifyNodesLinker;
						mOnlyNegQualifyNodesLinker = prevRest->mOnlyNegQualifyNodesLinker;
						mBothQualifyNodesLinker = prevRest->mBothQualifyNodesLinker;
						mIndiLink = prevRest->mIndiLink;
						mRemainingLinkerMergingCandidateIndiNodeCount = prevRest->mRemainingLinkerMergingCandidateIndiNodeCount;
						mRemainingValidMergingCandidateIndiNodeCount = prevRest->mRemainingValidMergingCandidateIndiNodeCount;
						mDistinctSetFixed = prevRest->mDistinctSetFixed;
						mHasMergingInitCandidates = prevRest->mHasMergingInitCandidates;
						mRemainingNominalCreationCount = prevRest->mRemainingNominalCreationCount;
						mAddedBlockablePredMergingNodeCandidate = prevRest->mAddedBlockablePredMergingNodeCandidate;
						mAddedBlockablePredDepTrackPoint = prevRest->mAddedBlockablePredDepTrackPoint;
						mDependencyTrackPoint = prevRest->mDependencyTrackPoint;
						mMergingDependencyNode = prevRest->mMergingDependencyNode;
						mInitMergingNodesClashes = prevRest->mInitMergingNodesClashes;
						mMultipleInitMergingNodesClashes = prevRest->mMultipleInitMergingNodesClashes;
						mDistinctSetNodeRelocated = prevRest->mDistinctSetNodeRelocated;

						//mList = prevRest->mList;
					} else {
						mDistinctSetNodeRelocated = false;
						mDistinctMergedNodesSet = nullptr;
						mMergingNodesLinker = nullptr;
						mNominalMergingNodesLinker = nullptr;
						mMergingInitNodesLinker = nullptr;
						mOnlyPosQualifyNodesLinker = nullptr;
						mOnlyNegQualifyNodesLinker = nullptr;
						mBothQualifyNodesLinker = nullptr;
						mIndiLink = nullptr;
						mRemainingLinkerMergingCandidateIndiNodeCount = 0;
						mRemainingValidMergingCandidateIndiNodeCount = 0;
						mDistinctSetFixed = false;
						mHasMergingInitCandidates = false;
						mRemainingNominalCreationCount = 0;
						mAddedBlockablePredMergingNodeCandidate = false;
						mAddedBlockablePredDepTrackPoint = nullptr;
						mDependencyTrackPoint = nullptr;
						mMergingDependencyNode = nullptr;
						mInitMergingNodesClashes = nullptr;
						mMultipleInitMergingNodesClashes = nullptr;
						
					}
					return this;
				}



				CPROCESSSET<cint64>* CBranchingMergingProcessingRestrictionSpecification::getDistinctMergedNodesSet(bool create) {
					if (!mDistinctMergedNodesSet && create) {
						createLocalizedDistinctMergedNodeSet();
					}
					return mLastDistinctMergedNodesSet;
				}


				CPROCESSSET<cint64>* CBranchingMergingProcessingRestrictionSpecification::createLocalizedDistinctMergedNodeSet() {
					if (!mDistinctMergedNodesSet) {
						mDistinctMergedNodesSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>,CContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mLastDistinctMergedNodesSet) {
							*mDistinctMergedNodesSet = *mLastDistinctMergedNodesSet;
						}
						mLastDistinctMergedNodesSet = mDistinctMergedNodesSet;
					}
					return mLastDistinctMergedNodesSet;
				}



				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::addDistinctMergedNode(cint64 mergedIndiNode) {
					createLocalizedDistinctMergedNodeSet()->insert(mergedIndiNode);
					return this;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::removeDistinctMergedNode(cint64 mergedIndiNode) {
					createLocalizedDistinctMergedNodeSet()->remove(mergedIndiNode);
					return this;
				}

				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingProcessingRestrictionSpecification::getMergingCandidateNodeLinker() {
					return mMergingNodesLinker;
				}

				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingProcessingRestrictionSpecification::takeNextMergingCandidateNodeLinker() {
					CBranchingMergingIndividualNodeCandidateLinker* tmpMergingNodeLinker = nullptr;
					tmpMergingNodeLinker = takeNextMergingInitializationCandidateNodeLinker();
					if (!tmpMergingNodeLinker) {
						tmpMergingNodeLinker = mNominalMergingNodesLinker;
						if (mNominalMergingNodesLinker) {
							--mRemainingLinkerMergingCandidateIndiNodeCount;
							--mRemainingValidMergingCandidateIndiNodeCount;
							mNominalMergingNodesLinker = mNominalMergingNodesLinker->getNext();
						}
						if (!tmpMergingNodeLinker) {
							tmpMergingNodeLinker = mMergingNodesLinker;
							if (mMergingNodesLinker) {
								--mRemainingLinkerMergingCandidateIndiNodeCount;
								--mRemainingValidMergingCandidateIndiNodeCount;
								mMergingNodesLinker = mMergingNodesLinker->getNext();
							}
						}
					}
					return tmpMergingNodeLinker;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::addMergingCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker) {
					while (linker) {
						CBranchingMergingIndividualNodeCandidateLinker* linkerIt = linker;
						linker = linker->getNext();
						linkerIt->clearNext();
						if (!mAddedBlockablePredMergingNodeCandidate && linkerIt->isCandidateBlockableAndCreator()) {
							mAddedBlockablePredDepTrackPoint = linkerIt->getMergingIndividualLink()->getDependencyTrackPoint();
							mAddedBlockablePredMergingNodeCandidate = true;
						}
						++mRemainingLinkerMergingCandidateIndiNodeCount;
						++mRemainingValidMergingCandidateIndiNodeCount;
						if (linkerIt->getMergingIndividualNodeCandidate()->isNominalIndividual()) {
							mNominalMergingNodesLinker = linkerIt->append(mNominalMergingNodesLinker);
						} else {
							mMergingNodesLinker = linkerIt->append(mMergingNodesLinker);
						}
					}
					return this;
				}


				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingProcessingRestrictionSpecification::getMergingInitializationCandidateNodeLinker() {
					return mMergingInitNodesLinker;
				}

				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingProcessingRestrictionSpecification::takeNextMergingInitializationCandidateNodeLinker() {
					CBranchingMergingIndividualNodeCandidateLinker* tmpMergingNodeLinker = nullptr;
					tmpMergingNodeLinker = mMergingInitNodesLinker;
					if (mMergingInitNodesLinker) {
						--mRemainingLinkerMergingCandidateIndiNodeCount;
						--mRemainingValidMergingCandidateIndiNodeCount;
						mMergingInitNodesLinker = mMergingInitNodesLinker->getNext();
					}
					return tmpMergingNodeLinker;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::addMergingInitializationCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker) {
					if (linker) {
						mHasMergingInitCandidates = true;
						CBranchingMergingIndividualNodeCandidateLinker* linkerIt = linker;
						while (linkerIt) {
							if (!mAddedBlockablePredMergingNodeCandidate && linkerIt->isCandidateBlockableAndCreator()) {
								mAddedBlockablePredDepTrackPoint = linkerIt->getMergingIndividualLink()->getDependencyTrackPoint();
								mAddedBlockablePredMergingNodeCandidate = true;
							}
							++mRemainingLinkerMergingCandidateIndiNodeCount;
							++mRemainingValidMergingCandidateIndiNodeCount;
							linkerIt = linkerIt->getNext();
						}
						mMergingInitNodesLinker = linker->append(mMergingInitNodesLinker);
					}
					return this;
				}



				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingProcessingRestrictionSpecification::getOnlyPosQualifyCandidateNodeLinker() {
					return mOnlyPosQualifyNodesLinker;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::addOnlyPosQualifyCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker) {
					if (linker) {
						mOnlyPosQualifyNodesLinker = linker->append(mOnlyPosQualifyNodesLinker);
					}
					return this;
				}


				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::clearOnlyPosQualifyCandidateNodeLinker() {
					mOnlyPosQualifyNodesLinker = nullptr;
					return this;
				}

				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingProcessingRestrictionSpecification::getOnlyNegQualifyCandidateNodeLinker() {
					return mOnlyNegQualifyNodesLinker;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::addOnlyNegQualifyCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker) {
					if (linker) {
						mOnlyNegQualifyNodesLinker = linker->append(mOnlyNegQualifyNodesLinker);
					}
					return this;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::clearOnlyNegQualifyCandidateNodeLinker() {
					mOnlyNegQualifyNodesLinker = nullptr;
					return this;
				}

				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingProcessingRestrictionSpecification::getBothQualifyCandidateNodeLinker() {
					return mBothQualifyNodesLinker;
				}


				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::addBothQualifyCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker) {
					if (linker) {
						mBothQualifyNodesLinker = linker->append(mBothQualifyNodesLinker);
					}
					return this;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::setBothQualifyCandidateNodeLinker(CBranchingMergingIndividualNodeCandidateLinker* linker) {
					mBothQualifyNodesLinker = linker;
					return this;
				}

				cint64 CBranchingMergingProcessingRestrictionSpecification::getRemainingLinkerMergingCandidateIndividualNodeCount() {
					return mRemainingLinkerMergingCandidateIndiNodeCount;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::setRemainingLinkerMergingCandidateIndividualNodeCount(cint64 remainingCanidateCount) {
					mRemainingLinkerMergingCandidateIndiNodeCount = remainingCanidateCount;
					return this;
				}

				CIndividualLinkEdge* CBranchingMergingProcessingRestrictionSpecification::getLastIndividualLink() {
					return mIndiLink;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::setLastIndividualLink(CIndividualLinkEdge* indiLink) {
					mIndiLink = indiLink;
					return this;
				}


				cint64 CBranchingMergingProcessingRestrictionSpecification::getRemainingValidMergingCandidateIndividualNodeCount() {
					return mRemainingValidMergingCandidateIndiNodeCount;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::setRemainingValidMergingCandidateIndividualNodeCount(cint64 remainingCanidateCount) {
					mRemainingValidMergingCandidateIndiNodeCount = remainingCanidateCount;
					return this;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::incRemainingValidMergingCandidateIndividualNodeCount() {
					++mRemainingValidMergingCandidateIndiNodeCount;
					return this;
				}


				bool CBranchingMergingProcessingRestrictionSpecification::hasValidRemainingMergingCandidates() {
					return mRemainingValidMergingCandidateIndiNodeCount == mRemainingLinkerMergingCandidateIndiNodeCount;
				}


				bool CBranchingMergingProcessingRestrictionSpecification::hasRemainingMergingCandidates() {
					return mNominalMergingNodesLinker != nullptr || mMergingNodesLinker != nullptr || mMergingInitNodesLinker != nullptr;
				}

				bool CBranchingMergingProcessingRestrictionSpecification::hasMergingInitializationCandidates() {
					return mHasMergingInitCandidates;
				}

				bool CBranchingMergingProcessingRestrictionSpecification::hasRemainingMergingInitializationCandidates() {
					return mMergingInitNodesLinker != nullptr;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::setDistinctSetFixed(bool fixed) {
					mDistinctSetFixed = fixed;
					return this;
				}

				bool CBranchingMergingProcessingRestrictionSpecification::isDistinctSetFixed() {
					return mDistinctSetFixed;
				}


				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::setRemainingNominalCreationCount(cint64 nomCount) {
					mRemainingNominalCreationCount = nomCount;
					return this;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::decRemainingNominalCreationCount(cint64 decCount) {
					mRemainingNominalCreationCount -= decCount;
					return this;
				}


				cint64 CBranchingMergingProcessingRestrictionSpecification::getRemainingNominalCreationCount() {
					return mRemainingNominalCreationCount;
				}

				bool CBranchingMergingProcessingRestrictionSpecification::hasAddedBlockablePredecessorMergingNodeCandidate() {
					return mAddedBlockablePredMergingNodeCandidate;
				}

				CDependencyTrackPoint* CBranchingMergingProcessingRestrictionSpecification::getAddedBlockablePredecessorDependencyTrackPoint() {
					return mAddedBlockablePredDepTrackPoint;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::initMergingDependencyNode(CNonDeterministicDependencyNode* depNode) {
					mMergingDependencyNode = depNode;
					return this;
				}

				CNonDeterministicDependencyNode* CBranchingMergingProcessingRestrictionSpecification::getMergingDependencyNode() {
					return mMergingDependencyNode;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::setMergingNodesInitializationClashesDescriptors(CClashedDependencyDescriptor* clashes) {
					mInitMergingNodesClashes = clashes;
					return this;
				}

				CClashedDependencyDescriptor* CBranchingMergingProcessingRestrictionSpecification::getMergingNodesInitializationClashesDescriptors() {
					return mInitMergingNodesClashes;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::setMultipleMergingNodesInitializationClashesDescriptors(CClashedDependencyDescriptor* clashes) {
					mMultipleInitMergingNodesClashes = clashes;
					return this;
				}

				CClashedDependencyDescriptor* CBranchingMergingProcessingRestrictionSpecification::getMultipleMergingNodesInitializationClashesDescriptors() {
					return mMultipleInitMergingNodesClashes;
				}


				bool CBranchingMergingProcessingRestrictionSpecification::isDistinctSetNodeRelocated() {
					return mDistinctSetNodeRelocated;
				}

				CBranchingMergingProcessingRestrictionSpecification* CBranchingMergingProcessingRestrictionSpecification::setDistinctSetNodeRelocated(bool distinctSetNodeRelocated) {
					mDistinctSetNodeRelocated = distinctSetNodeRelocated;
					return this;
				}


				//bool CBranchingMergingProcessingRestrictionSpecification::addIndividualToContainer(cint64 container, cint64 individual) {
				//	QString insertString;
				//	--container;
				//	if (mList.size() > container) {
				//		insertString = mList.value(container);
				//		insertString += QString(" %1").arg(individual);
				//		mList.replace(container,insertString);
				//	} else {
				//		insertString += QString("%1").arg(individual);
				//		mList.insert(container,insertString);
				//	}
				//	return true;
				//}

				//void CBranchingMergingProcessingRestrictionSpecification::printIndividualContainer() {
				//	cout<<"\n"<<mList.join("\n").toLocal8Bit().data()<<endl;
				//}



			}; // end namespace Kernel

		}; // end namespace Process

	}; // end namespace Reasoner

}; // end namespace Konclude
