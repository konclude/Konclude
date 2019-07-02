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

#include "CTrackedClashedDependencyLine.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CTrackedClashedDependencyLine::CTrackedClashedDependencyLine(CPROCESSINGSET<CTrackedClashedDescriptorHasher>* clashedSet) {
					mFreeTrackedClashedDescriptors = nullptr;
					mClashedSet = clashedSet;
				}


				CTrackedClashedDependencyLine* CTrackedClashedDependencyLine::initTrackedClashedDependencyLine(bool exactIndiNodeTracking, cint64 individualNodeTrackLevel, cint64 branchingLevel) {
					mExactIndividualTracking = exactIndiNodeTracking;
					mIndividualTrackLevel = individualNodeTrackLevel;
					mBranchingLevel = branchingLevel;

					mLevelTrackedClashes = nullptr;
					mLevelTrackedBranchingClashes = nullptr;
					mPrevLevelsTrackedClashes = nullptr;
					mPrevLevelsTrackedNonDetClashes = nullptr;
					mPrevLevelsTrackedNonDetBranchingClashes = nullptr;
					mIndependentTrackedClashes = nullptr;
					return this;
				}

				CTrackedClashedDependencyLine* CTrackedClashedDependencyLine::sortInTrackedClashedDescriptors(CTrackedClashedDescriptor* clashedDes, bool forceInsertion) {
					CTrackedClashedDescriptor* clashedDesIt = clashedDes;
					while (clashedDesIt) {
						CTrackedClashedDescriptor* clashedDesTmp = clashedDesIt;
						clashedDesIt = clashedDesIt->getNextDescriptor();
						clashedDesTmp->clearNext();
						CTrackedClashedDescriptorHasher desHasher(clashedDesTmp);
						bool clashDesInsertion = forceInsertion;
						if (!mClashedSet->contains(desHasher)) {
							mClashedSet->insert(desHasher);
							clashDesInsertion = true;
						}
						if (clashDesInsertion) {
							if (clashedDesTmp->isPointingToIndependentDependencyNode()) {
								mIndependentTrackedClashes = clashedDesTmp->append(mIndependentTrackedClashes);
							} else {
								if (clashedDesTmp->getAppropriatedIndividualLevel() > mIndividualTrackLevel) {
									// previous individual node level
									if (clashedDesTmp->isPointingToNonDeterministicDependencyNode()) {
										if (clashedDesTmp->getBranchingLevelTag() == mBranchingLevel) {
											mPrevLevelsTrackedNonDetBranchingClashes = clashedDesTmp->append(mPrevLevelsTrackedNonDetBranchingClashes);
										} else {
											mPrevLevelsTrackedNonDetClashes = clashedDesTmp->append(mPrevLevelsTrackedNonDetClashes);
										}
									} else {
										// deterministic, branching level does not matter
										mPrevLevelsTrackedClashes = clashedDesTmp->append(mPrevLevelsTrackedClashes);
									}
								} else {
									// current individual node level
									if (clashedDesTmp->getBranchingLevelTag() == mBranchingLevel) {
										mLevelTrackedBranchingClashes = clashedDesTmp->append(mLevelTrackedBranchingClashes);
									} else {
										mLevelTrackedClashes = clashedDesTmp->append(mLevelTrackedClashes);
									}
								}
							}
						} else {
							addFreeTrackedClashedDescriptor(clashedDesTmp);
						}
					}
					return this;
				}



				CTrackedClashedDependencyLine* CTrackedClashedDependencyLine::moveToNextIndividualNodeLevel(cint64 newLevel) {
					mIndividualTrackLevel = newLevel;
					//if (mLevelTrackedClashes) {
					//	mPrevLevelsTrackedClashes = mLevelTrackedClashes->append(mPrevLevelsTrackedClashes);
					//}
					CTrackedClashedDescriptor* levelTrackedClashes = mLevelTrackedClashes;
					mLevelTrackedClashes = nullptr;
					sortInTrackedClashedDescriptors(levelTrackedClashes,true);

					CTrackedClashedDescriptor* levelTrackedBranchingClashes = mLevelTrackedBranchingClashes;
					mLevelTrackedBranchingClashes = nullptr;
					sortInTrackedClashedDescriptors(levelTrackedBranchingClashes,true);
					return this;
				}



				bool CTrackedClashedDependencyLine::hasIndependentTrackedClashedDescriptors() {
					return mIndependentTrackedClashes != nullptr;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::takeNextIndependentTrackedClashedDescriptor() {
					CTrackedClashedDescriptor* clashedDesTmp = mIndependentTrackedClashes;
					if (mIndependentTrackedClashes) {
						mIndependentTrackedClashes = mIndependentTrackedClashes->getNextDescriptor();
						clashedDesTmp->clearNext();
					}
					return clashedDesTmp;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::getIndependentTrackedClashedDescriptors() {
					return mIndependentTrackedClashes;
				}

				bool CTrackedClashedDependencyLine::hasLevelTrackedClashedDescriptors() {
					return mLevelTrackedClashes != nullptr;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::takeNextLevelTrackedClashedDescriptor() {
					CTrackedClashedDescriptor* clashedDesTmp = mLevelTrackedClashes;
					if (mLevelTrackedClashes) {
						mLevelTrackedClashes = mLevelTrackedClashes->getNextDescriptor();
						clashedDesTmp->clearNext();
					}
					return clashedDesTmp;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::getLevelTrackedClashedDescriptors() {
					return mLevelTrackedClashes;
				}


				bool CTrackedClashedDependencyLine::hasLevelTrackedBranchingClashedDescriptors() {
					return mLevelTrackedBranchingClashes != nullptr;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::takeNextLevelTrackedBranchingClashedDescriptor() {
					CTrackedClashedDescriptor* clashedDesTmp = mLevelTrackedBranchingClashes;
					if (mLevelTrackedBranchingClashes) {
						mLevelTrackedBranchingClashes = mLevelTrackedBranchingClashes->getNextDescriptor();
						clashedDesTmp->clearNext();
					}
					return clashedDesTmp;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::getLevelTrackedBranchingClashedDescriptors() {
					return mLevelTrackedBranchingClashes;
				}


				bool CTrackedClashedDependencyLine::hasPerviousLevelTrackedClashedDescriptors() {
					return mPrevLevelsTrackedClashes != nullptr;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::takeNextPerviousLevelTrackedClashedDescriptor() {
					CTrackedClashedDescriptor* clashedDesTmp = mPrevLevelsTrackedClashes;
					if (mPrevLevelsTrackedClashes) {
						mPrevLevelsTrackedClashes = mPrevLevelsTrackedClashes->getNextDescriptor();
						clashedDesTmp->clearNext();
					}
					return clashedDesTmp;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::getPerviousLevelTrackedClashedDescriptors() {
					return mPrevLevelsTrackedClashes;
				}


				bool CTrackedClashedDependencyLine::hasPerviousLevelTrackedNonDeterministicClashedDescriptors() {
					return mPrevLevelsTrackedNonDetClashes != nullptr;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::takeNextPerviousLevelTrackedNonDeterministicClashedDescriptor() {
					CTrackedClashedDescriptor* clashedDesTmp = mPrevLevelsTrackedNonDetClashes;
					if (mPrevLevelsTrackedNonDetClashes) {
						mPrevLevelsTrackedNonDetClashes = mPrevLevelsTrackedNonDetClashes->getNextDescriptor();
						clashedDesTmp->clearNext();
					}
					return clashedDesTmp;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::getPerviousLevelTrackedNonDeterministicClashedDescriptors() {
					return mPrevLevelsTrackedNonDetClashes;
				}



				bool CTrackedClashedDependencyLine::hasPerviousLevelTrackedNonDeterministicBranchingClashedDescriptors() {
					return mPrevLevelsTrackedNonDetBranchingClashes != nullptr;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::takeNextPerviousLevelTrackedNonDeterministicBranchingClashedDescriptor() {
					CTrackedClashedDescriptor* clashedDesTmp = mPrevLevelsTrackedNonDetBranchingClashes;
					if (mPrevLevelsTrackedNonDetBranchingClashes) {
						mPrevLevelsTrackedNonDetBranchingClashes = mPrevLevelsTrackedNonDetBranchingClashes->getNextDescriptor();
						clashedDesTmp->clearNext();
					}
					return clashedDesTmp;
				}

				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::getPerviousLevelTrackedNonDeterministicBranchingClashedDescriptors() {
					return mPrevLevelsTrackedNonDetBranchingClashes;
				}



				bool CTrackedClashedDependencyLine::hasMoreTrackedClashedList() {
					return mLevelTrackedClashes || mLevelTrackedBranchingClashes || mPrevLevelsTrackedClashes || mPrevLevelsTrackedNonDetClashes || mPrevLevelsTrackedNonDetBranchingClashes || mIndependentTrackedClashes;
				}


				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::takeNextTrackedClashedList() {
					CTrackedClashedDescriptor* clashes = nullptr;
					if (mLevelTrackedClashes) {
						clashes = mLevelTrackedClashes;
						mLevelTrackedClashes = nullptr;
					} else if (mLevelTrackedBranchingClashes) {
						clashes = mLevelTrackedBranchingClashes;
						mLevelTrackedBranchingClashes = nullptr;
					} else if (mPrevLevelsTrackedClashes) {
						clashes = mPrevLevelsTrackedClashes;
						mPrevLevelsTrackedClashes = nullptr;
					} else if (mPrevLevelsTrackedNonDetClashes) {
						clashes = mPrevLevelsTrackedNonDetClashes;
						mPrevLevelsTrackedNonDetClashes = nullptr;
					} else if (mPrevLevelsTrackedNonDetBranchingClashes) {
						clashes = mPrevLevelsTrackedNonDetBranchingClashes;
						mPrevLevelsTrackedNonDetBranchingClashes = nullptr;
					} else if (mIndependentTrackedClashes) {
						clashes = mIndependentTrackedClashes;
						mIndependentTrackedClashes = nullptr;
					}
					return clashes;
				}


				CTrackedClashedDescriptor* CTrackedClashedDependencyLine::takeNextFreeTrackedClashedDescriptor() {
					CTrackedClashedDescriptor* clashedDesTmp = mFreeTrackedClashedDescriptors;
					if (mFreeTrackedClashedDescriptors) {
						mFreeTrackedClashedDescriptors = mFreeTrackedClashedDescriptors->getNextDescriptor();
						clashedDesTmp->clearNext();
					}
					return clashedDesTmp;
				}

				CTrackedClashedDependencyLine* CTrackedClashedDependencyLine::addFreeTrackedClashedDescriptor(CTrackedClashedDescriptor* clashDes) {
					if (clashDes) {
						mFreeTrackedClashedDescriptors = clashDes->append(mFreeTrackedClashedDescriptors);
					}
					return this;
				}


				CPROCESSINGSET<CTrackedClashedDescriptorHasher>* CTrackedClashedDependencyLine::getTrackedClashedDescriptorSet() {
					return mClashedSet;
				}


				cint64 CTrackedClashedDependencyLine::getBranchingLevel() {
					return mBranchingLevel;
				}

				cint64 CTrackedClashedDependencyLine::getIndividualNodeLevel() {
					return mIndividualTrackLevel;
				}

				bool CTrackedClashedDependencyLine::hasOnlyIndependentTrackedClashedDescriptorsRemaining() {
					return mIndependentTrackedClashes && !mLevelTrackedClashes && !mLevelTrackedBranchingClashes && !mPrevLevelsTrackedClashes && !mPrevLevelsTrackedNonDetClashes && !mPrevLevelsTrackedNonDetBranchingClashes;
				}


				bool CTrackedClashedDependencyLine::hasOnlyCurrentIndividualNodeLevelClashesDescriptors() {
					return !mPrevLevelsTrackedClashes && !mPrevLevelsTrackedNonDetClashes && !mPrevLevelsTrackedNonDetBranchingClashes;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
