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

#include "CBackendNeighbourExpansionControllingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CBackendNeighbourExpansionControllingData::CBackendNeighbourExpansionControllingData(CProcessContext* processContext) {
				}


				CBackendNeighbourExpansionControllingData* CBackendNeighbourExpansionControllingData::initExpansionControllingData(CBackendNeighbourExpansionControllingData* contData) {
					if (contData) {
						mExpandedNeighbourLinkCount = contData->mExpandedNeighbourLinkCount;
						mReuseModesDepNode = contData->mReuseModesDepNode;
						mReuseContinuingDependencyTrackPoint = contData->mReuseContinuingDependencyTrackPoint;
						mPrioritizedReuseExpansionMode = contData->mPrioritizedReuseExpansionMode;
						mFixedReuseExpansionMode = contData->mFixedReuseExpansionMode;
						mLastBackendExpandedEnsuringExistingIndividualLinksLinker = contData->mLastBackendExpandedEnsuringExistingIndividualLinksLinker;
						mCutBackendNeighbourExpansionIndividualLinker = contData->mCutBackendNeighbourExpansionIndividualLinker;
						mLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker = contData->mLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker;
					} else {
						mExpandedNeighbourLinkCount = 0;
						mReuseModesDepNode = nullptr;
						mReuseContinuingDependencyTrackPoint = nullptr;
						mPrioritizedReuseExpansionMode = false;
						mFixedReuseExpansionMode = false;
						mLastBackendExpandedEnsuringExistingIndividualLinksLinker = nullptr;
						mCutBackendNeighbourExpansionIndividualLinker = nullptr;
						mLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker = nullptr;
					}
					return this;
				}


				cint64 CBackendNeighbourExpansionControllingData::getExpandedNeighbourLinkCount() {
					return mExpandedNeighbourLinkCount;
				}

				CBackendNeighbourExpansionControllingData* CBackendNeighbourExpansionControllingData::incExpandedNeighbourLinkCount(cint64 count) {
					mExpandedNeighbourLinkCount += count;
					return this;
				}

				CREUSEBACKENDEXPANSIONMODESDependencyNode* CBackendNeighbourExpansionControllingData::getReuseModesDependencyNode() {
					return mReuseModesDepNode;
				}

				CBackendNeighbourExpansionControllingData* CBackendNeighbourExpansionControllingData::setReuseModesDependencyNode(CREUSEBACKENDEXPANSIONMODESDependencyNode* depNode) {
					mReuseModesDepNode = depNode;
					return this;
				}


				bool CBackendNeighbourExpansionControllingData::hasExpansionReusingMode() {
					return isFixedReuseExpansionMode() || isPrioritizedReuseExpansionMode();
				}

				bool CBackendNeighbourExpansionControllingData::isFixedReuseExpansionMode() {
					return mFixedReuseExpansionMode;
				}

				CBackendNeighbourExpansionControllingData* CBackendNeighbourExpansionControllingData::setFixedReuseExpansionMode(bool active) {
					mFixedReuseExpansionMode = active;
					return this;
				}

				bool CBackendNeighbourExpansionControllingData::isPrioritizedReuseExpansionMode() {
					return mPrioritizedReuseExpansionMode;
				}

				CBackendNeighbourExpansionControllingData* CBackendNeighbourExpansionControllingData::setPrioritizedReuseExpansionMode(bool active) {
					mPrioritizedReuseExpansionMode = active;
					return this;
				}

				CDependencyTrackPoint* CBackendNeighbourExpansionControllingData::getReuseContinuingDependencyTrackPoint() {
					return mReuseContinuingDependencyTrackPoint;
				}

				CBackendNeighbourExpansionControllingData* CBackendNeighbourExpansionControllingData::setReuseContinuingDependencyTrackPoint(CDependencyTrackPoint* depTrackPoint) {
					mReuseContinuingDependencyTrackPoint = depTrackPoint;
					return this;
				}


				CXLinker<CIndividualProcessNode*>* CBackendNeighbourExpansionControllingData::getLastBackendExpandedEnsuringExistingIndividualLinksLinker() {
					return mLastBackendExpandedEnsuringExistingIndividualLinksLinker;
				}

				CBackendNeighbourExpansionControllingData* CBackendNeighbourExpansionControllingData::setLastBackendExpandedEnsuringExistingIndividualLinksLinker(CXLinker<CIndividualProcessNode*>* indiLinker) {
					mLastBackendExpandedEnsuringExistingIndividualLinksLinker = indiLinker;
					return this;
				}



				CXLinker<CIndividualProcessNode*>* CBackendNeighbourExpansionControllingData::getCutBackendNeighbourExpansionIndividualLinker() {
					return mCutBackendNeighbourExpansionIndividualLinker;
				}

				CBackendNeighbourExpansionControllingData* CBackendNeighbourExpansionControllingData::addCutBackendNeighbourExpansionIndividualLinker(CXLinker<CIndividualProcessNode*>* indiLinker) {
					mCutBackendNeighbourExpansionIndividualLinker = indiLinker->append(mCutBackendNeighbourExpansionIndividualLinker);
					return this;
				}

				CXLinker<CIndividualProcessNode*>* CBackendNeighbourExpansionControllingData::getLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker() {
					return mLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker;
				}

				CBackendNeighbourExpansionControllingData* CBackendNeighbourExpansionControllingData::setLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker(CXLinker<CIndividualProcessNode*>* indiLinker) {
					mLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker = indiLinker;
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
