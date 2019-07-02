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

#include "CNonDeterministicDependencyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {


					CNonDeterministicDependencyNode::CNonDeterministicDependencyNode(CProcessContext* processContext) : CDependencyNode(processContext),mClashTrackPoint(this) {
					}

					CNonDeterministicDependencyNode* CNonDeterministicDependencyNode::initNonDeterministicDependencyNode(DEPENDENCNODEYTYPE depType, CBranchTreeNode* branchNode, CConceptDescriptor* conceptDescriptor) {
						initDependencyNode(depType,conceptDescriptor);
						mClashTrackPoint.setClashedOrIrelevantBranch(true);
						mBranchTrackPoints = &mClashTrackPoint;
						mDependencyClashes = nullptr;
						mBranchNode = branchNode;
						mBranchTag = 0;
						mClosedTrackPoint = nullptr;
						mClosingTrackPoint = nullptr;
						return this;
					}

					CNonDeterministicDependencyNode* CNonDeterministicDependencyNode::initNonDeterministicDependencyNode(DEPENDENCNODEYTYPE depType, CBranchTreeNode* branchNode, CIndividualProcessNode* individualNode, CConceptDescriptor* conceptDescriptor) {
						initDependencyNode(depType,individualNode,conceptDescriptor);
						mClashTrackPoint.setClashedOrIrelevantBranch(true);
						mBranchTrackPoints = &mClashTrackPoint;
						mDependencyClashes = nullptr;
						mBranchNode = branchNode;
						mBranchTag = 0;
						mClosedTrackPoint = nullptr;
						mClosingTrackPoint = nullptr;
						return this;
					}

					bool CNonDeterministicDependencyNode::isDeterministiDependencyNode() {
						return false;
					}


					CDependencyTrackPoint* CNonDeterministicDependencyNode::getContinueDependencyTrackPoint() {
						return &mClashTrackPoint;
					}


					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyNode::getDependencyTrackPointBranch() {
						CNonDeterministicDependencyTrackPoint* nonDetTrackPoint = CObjectParameterizingAllocator< CNonDeterministicDependencyTrackPoint,CNonDeterministicDependencyNode* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),this);
						mBranchTrackPoints = nonDetTrackPoint->append(mBranchTrackPoints);
						updateDependencyTrackPointBranchingTag(nonDetTrackPoint,mBranchTag);
						return nonDetTrackPoint;
					}


					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyNode::getBranchTrackPoints() {
						return mBranchTrackPoints;
					}


					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyNode::getClashTrackPoint() {
						return &mClashTrackPoint;
					}



					CClashedDependencyDescriptor* CNonDeterministicDependencyNode::getDependencyClashes() {
						return mDependencyClashes;
					}

					bool CNonDeterministicDependencyNode::hasDependencyClashes() {
						return mDependencyClashes != nullptr;
					}

					CNonDeterministicDependencyNode* CNonDeterministicDependencyNode::setDependencyClash(CClashedDependencyDescriptor* clash) {
						mDependencyClashes = clash;
						return this;
					}


					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyNode::addBranchClashes(CClashedDependencyDescriptor* clash) {
						mClashTrackPoint.addClashes(clash);
						return &mClashTrackPoint;
					}


					bool CNonDeterministicDependencyNode::updateBranchingTags() {
						mBranchTag = getDependedBranchingLevel();
						bool changed = false;
						CNonDeterministicDependencyTrackPoint* branchTrackPointsIt = mBranchTrackPoints;
						while (branchTrackPointsIt) {
							changed |= updateDependencyTrackPointBranchingTag(branchTrackPointsIt,mBranchTag);
							branchTrackPointsIt = branchTrackPointsIt->getNext();
						}
						return changed;
					}



					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyNode::getClosingDependencyTrackPoint() {
						return mClosingTrackPoint;
					}

					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyNode::getClosedDependencyTrackPoint() {
						return mClosedTrackPoint;
					}


					CNonDeterministicDependencyNode* CNonDeterministicDependencyNode::setClosingDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
						mClosingTrackPoint = depTrackPoint;
						return this;
					}

					CNonDeterministicDependencyNode* CNonDeterministicDependencyNode::setClosedDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
						mClosedTrackPoint = depTrackPoint;
						return this;
					}

					bool CNonDeterministicDependencyNode::hasClosingDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
						return mClosingTrackPoint == depTrackPoint;
					}

					bool CNonDeterministicDependencyNode::hasClosedDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
						return mClosedTrackPoint == depTrackPoint;
					}

					bool CNonDeterministicDependencyNode::hasClosingDependencyTrackPoint() {
						return mClosingTrackPoint != nullptr;
					}

					bool CNonDeterministicDependencyNode::hasClosedDependencyTrackPoint() {
						return mClosedTrackPoint != nullptr;
					}

					cint64 CNonDeterministicDependencyNode::getOpenedDependencyTrackingPointsCount() {
						cint64 opDepTrackPointCount = 0;
						CNonDeterministicDependencyTrackPoint* branchTrackPointsIt = mBranchTrackPoints;
						while (branchTrackPointsIt) {
							if (!branchTrackPointsIt->isClashedOrIrelevantBranch()) {
								++opDepTrackPointCount;
							}
							branchTrackPointsIt = branchTrackPointsIt->getNext();
						}
						return opDepTrackPointCount;
					}

					bool CNonDeterministicDependencyNode::hasMultipleOpenedDependencyTrackingPoints() {
						cint64 opDepTrackPointCount = 0;
						CNonDeterministicDependencyTrackPoint* branchTrackPointsIt = mBranchTrackPoints;
						while (branchTrackPointsIt) {
							if (!branchTrackPointsIt->isClashedOrIrelevantBranch()) {
								++opDepTrackPointCount;
								if (opDepTrackPointCount >= 2) {
									return true;
								}
							}
							branchTrackPointsIt = branchTrackPointsIt->getNext();
						}
						return false;
					}


					bool CNonDeterministicDependencyNode::hasOtherOpenedDependencyTrackingPoints(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
						CNonDeterministicDependencyTrackPoint* branchTrackPointsIt = mBranchTrackPoints;
						while (branchTrackPointsIt) {
							if (branchTrackPointsIt != depTrackPoint && !branchTrackPointsIt->isClashedOrIrelevantBranch()) {
								return true;
							}
							branchTrackPointsIt = branchTrackPointsIt->getNext();
						}
						return false;
					}


					CBranchTreeNode* CNonDeterministicDependencyNode::getBranchNode() {
						return mBranchNode;
					}



				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
