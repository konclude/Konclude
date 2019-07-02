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

#include "CTrackedClashedDescriptor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CTrackedClashedDescriptor::CTrackedClashedDescriptor() {
				}


				CTrackedClashedDescriptor* CTrackedClashedDescriptor::initTrackedClashedDescriptor(CIndividualProcessNode* indiNode, CConceptDescriptor* conDes, CVariableBindingPath* varBindPath, CDependencyTrackPoint* depTrackPoint) {
					initClashedDependencyDescriptor(depTrackPoint);
					mErrorFlag = false;
					mIndiNodeID = -1;
					mIndiNodeLevel = -1;
					mNominalIndiFlag = false;
					mConceptDescriptor = nullptr;
					mVarBindPath = nullptr;
					mDetermisticFlag = false;
					mIndepenentFlag = false;
					mBranchingLevelTag = -1;
					mProcessingTag = -1;
					if (indiNode) {
						mIndiNodeID = indiNode->getIndividualNodeID();
						mIndiNodeLevel = indiNode->getIndividualNominalLevelOrAncestorDepth();
						mNominalIndiFlag = indiNode->isNominalIndividualNode();
					} else {
						mErrorFlag = true;
					}
					mConceptDescriptor = conDes;
					mVarBindPath = varBindPath;
					if (depTrackPoint) {
						mDetermisticFlag = depTrackPoint->isPointingToDeterministicDependencyNode();
						mIndepenentFlag = depTrackPoint->isPointingToIndependentDependencyNode();
						mProcessingTag = depTrackPoint->getProcessingTag();
						mBranchingLevelTag = depTrackPoint->getBranchingTag();
						if (mBranchingLevelTag <= -1) {
							mErrorFlag = true;
						}
					} else {
						mErrorFlag = true;
					}
					return this;
				}

				CTrackedClashedDescriptor* CTrackedClashedDescriptor::initTrackedClashedDescriptor(CTrackedClashedDescriptor* trackedClashDes) {
					mErrorFlag = trackedClashDes->mErrorFlag;
					mIndiNodeID = trackedClashDes->mIndiNodeID;
					mIndiNodeLevel = trackedClashDes->mIndiNodeLevel;
					mNominalIndiFlag = trackedClashDes->mNominalIndiFlag;
					mConceptDescriptor = trackedClashDes->mConceptDescriptor;
					mDetermisticFlag = trackedClashDes->mDetermisticFlag;
					mIndepenentFlag = trackedClashDes->mIndepenentFlag;
					mBranchingLevelTag = trackedClashDes->mBranchingLevelTag;
					mProcessingTag = trackedClashDes->mProcessingTag;
					mVarBindPath = trackedClashDes->mVarBindPath;
					initClashedDependencyDescriptor(trackedClashDes->mDependencyTrackPoint);
					return this;
				}


				CTrackedClashedDescriptor* CTrackedClashedDescriptor::initTrackedClashedDescriptor(CTrackedClashedDescriptor* trackedClashDesForIndiNode, CConceptDescriptor* conDes, CVariableBindingPath* varBindPath, CDependencyTrackPoint* depTrackPoint) {
					mErrorFlag = trackedClashDesForIndiNode->mErrorFlag;
					mIndiNodeID = trackedClashDesForIndiNode->mIndiNodeID;
					mIndiNodeLevel = trackedClashDesForIndiNode->mIndiNodeLevel;
					mNominalIndiFlag = trackedClashDesForIndiNode->mNominalIndiFlag;
					mVarBindPath = varBindPath;
					initClashedDependencyDescriptor(depTrackPoint);
					mConceptDescriptor = conDes;
					if (depTrackPoint) {
						mDetermisticFlag = depTrackPoint->isPointingToDeterministicDependencyNode();
						mIndepenentFlag = depTrackPoint->isPointingToIndependentDependencyNode();
						mProcessingTag = depTrackPoint->getProcessingTag();
						mBranchingLevelTag = depTrackPoint->getBranchingTag();
						if (mBranchingLevelTag <= -1) {
							mErrorFlag = true;
						}
					} else {
						mErrorFlag = true;
					}
					return this;
				}

				CTrackedClashedDescriptor* CTrackedClashedDescriptor::initTrackedClashedDescriptor(CTrackedClashedDescriptor* trackedClashDesForIndiNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint) {
					initTrackedClashedDescriptor(trackedClashDesForIndiNode,conDes,trackedClashDesForIndiNode->getVariableBindingPath(),depTrackPoint);
					return this;
				}


				bool CTrackedClashedDescriptor::isAppropriatedIndividualNominal() {
					return mNominalIndiFlag;
				}

				cint64 CTrackedClashedDescriptor::getAppropriatedIndividualID() {
					return mIndiNodeID;
				}

				cint64 CTrackedClashedDescriptor::getAppropriatedIndividualLevel() {
					return mIndiNodeLevel;
				}


				cint64 CTrackedClashedDescriptor::getBranchingLevelTag() {
					return mBranchingLevelTag;
				}

				cint64 CTrackedClashedDescriptor::getProcessingTag() {
					return mProcessingTag;
				}


				bool CTrackedClashedDescriptor::isPointingToDeterministicDependencyNode() {
					return mDetermisticFlag;
				}

				bool CTrackedClashedDescriptor::isPointingToNonDeterministicDependencyNode() {
					return !mDetermisticFlag;
				}


				bool CTrackedClashedDescriptor::isPointingToIndependentDependencyNode() {
					return mIndepenentFlag;
				}


				bool CTrackedClashedDescriptor::isTrackingError() {
					return mErrorFlag;
				}


				CConceptDescriptor* CTrackedClashedDescriptor::getConceptDescriptor() {
					return mConceptDescriptor;
				}

				CTrackedClashedDescriptor* CTrackedClashedDescriptor::setConceptDescriptor(CConceptDescriptor* conDes) {
					mConceptDescriptor = conDes;
					return this;
				}


				CVariableBindingPath* CTrackedClashedDescriptor::getVariableBindingPath() {
					return mVarBindPath;
				}

				bool CTrackedClashedDescriptor::candidateTracking(bool* trackingError, bool* nominalOccured, cint64* maxBranchLevel, cint64* minIndividualLevel) {
					bool usingCandidate = false;
					if (mErrorFlag) {
						if (trackingError && !*trackingError) {
							*trackingError = true;
							usingCandidate = true;
						} 
					} else {
						if (maxBranchLevel && *maxBranchLevel < mBranchingLevelTag) {
							*maxBranchLevel = mBranchingLevelTag;
							usingCandidate = true;
						}
						if (minIndividualLevel && *minIndividualLevel > mIndiNodeLevel) {
							*minIndividualLevel = mIndiNodeLevel;
							usingCandidate = true;
						}
						if (nominalOccured && mNominalIndiFlag && !*nominalOccured) {
							*nominalOccured = mNominalIndiFlag;
							usingCandidate = true;
						}
					}
					return usingCandidate;
				}


				bool CTrackedClashedDescriptor::isProcessedAfter(cint64 processingTag) {
					return mProcessingTag > processingTag;
				}

				CTrackedClashedDescriptor* CTrackedClashedDescriptor::append(CTrackedClashedDescriptor* trackedClashedDescriptor) {
					return (CTrackedClashedDescriptor*)CClashedDependencyDescriptor::append(trackedClashedDescriptor);
				}

				CTrackedClashedDescriptor* CTrackedClashedDescriptor::getNextDescriptor() {
					return (CTrackedClashedDescriptor*)getNext();
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
