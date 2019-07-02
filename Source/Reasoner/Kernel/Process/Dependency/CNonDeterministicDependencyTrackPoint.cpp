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

#include "CNonDeterministicDependencyTrackPoint.h"
#include "CBranchTreeNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				namespace Dependency {


					CNonDeterministicDependencyTrackPoint::CNonDeterministicDependencyTrackPoint(CDependencyNode* depNode) : CDependencyTrackPoint(depNode),CLinkerBase<CNonDeterministicDependencyTrackPoint*,CNonDeterministicDependencyTrackPoint>(this) {
						mBranchNode = nullptr;
						mClashes = nullptr;
						mClashedIrelevant = false;
					}


					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyTrackPoint::initBranch(CBranchTreeNode* branchNode) {
						mBranchNode = branchNode;
						addMaximumBranchingTagCandidate(mBranchNode->getBranchingLevel());
						return this;
					}


					CClashedDependencyDescriptor* CNonDeterministicDependencyTrackPoint::getClashes() {
						return mClashes;
					}

					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyTrackPoint::addClashes(CClashedDependencyDescriptor* clashes, bool setClashed) {
						if (clashes) {
							mClashes = clashes->append(mClashes);
							mClashedIrelevant |= setClashed;
						}
						return this;
					}

					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyTrackPoint::setClashes(CClashedDependencyDescriptor* clashes, bool setClashed) {
						mClashes = clashes;
						mClashedIrelevant |= setClashed;
						return this;
					}

					CBranchTreeNode* CNonDeterministicDependencyTrackPoint::getBranchNode() {
						return mBranchNode;
					}

					bool CNonDeterministicDependencyTrackPoint::isClashedOrIrelevantBranch() {
						return mClashedIrelevant;
					}


					CNonDeterministicDependencyTrackPoint* CNonDeterministicDependencyTrackPoint::setClashedOrIrelevantBranch(bool clashedOrIrelevant) {
						mClashedIrelevant = clashedOrIrelevant;
						return this;
					}



				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
