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

#include "CIndividualNodeIncrementalExpansionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeIncrementalExpansionData::CIndividualNodeIncrementalExpansionData(CProcessContext* processContext) : mProcessContext(processContext) {
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::initIncrementalExpansionData(CIndividualNodeIncrementalExpansionData* prevData) {
					if (prevData) {
						mDirectlyChanged = prevData->mDirectlyChanged;
						mDirectlyChangedNeighbourConnNode = prevData->mDirectlyChangedNeighbourConnNode;
						mCompletionGraphCompatible = prevData->mCompletionGraphCompatible;
						mLastCompatibleCheckedConDes = prevData->mLastCompatibleCheckedConDes;
						mLastCompatibleCheckedLink = prevData->mLastCompatibleCheckedLink;
						mPrevCompGraphCorrIndiNode = prevData->mPrevCompGraphCorrIndiNode;
						mPrevCompGraphCorrIndiNodeLoaded = prevData->mPrevCompGraphCorrIndiNodeLoaded;
						mUseNeighbourPropagatedDirectlyChangedList = prevData->mUseNeighbourPropagatedDirectlyChangedList;
						mLocNeighbourPropagatedDirectlyChangedList = nullptr;
						mUseIncrementalExpansionList = prevData->mUseIncrementalExpansionList;
						mLocIncrementalExpansionList = nullptr;
						mIncremetnalExpansionListInitialized = prevData->mIncremetnalExpansionListInitialized;
						mExpansionPriority = prevData->mExpansionPriority;
					} else {
						mDirectlyChanged = false;
						mDirectlyChangedNeighbourConnNode = nullptr;
						mCompletionGraphCompatible = false;
						mPrevCompGraphCorrIndiNodeLoaded = false;
						mLastCompatibleCheckedConDes = nullptr;
						mLastCompatibleCheckedLink = nullptr;
						mPrevCompGraphCorrIndiNode = nullptr;
						mLocNeighbourPropagatedDirectlyChangedList = nullptr;
						mUseNeighbourPropagatedDirectlyChangedList = nullptr;
						mLocIncrementalExpansionList = nullptr;
						mUseIncrementalExpansionList = nullptr;
						mIncremetnalExpansionListInitialized = false;
						mExpansionPriority = 0.;
					}
					return this;
				}

				CIndividualProcessNode* CIndividualNodeIncrementalExpansionData::getDirectlyChangedNeighbourConnectionNode() {
					return mDirectlyChangedNeighbourConnNode;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setDirectlyChangedNeighbourConnectionNode(CIndividualProcessNode* node) {
					mDirectlyChangedNeighbourConnNode = node;
					return this;
				}

				bool CIndividualNodeIncrementalExpansionData::hasDirectlyChangedNeighbourConnection() {
					return mDirectlyChangedNeighbourConnNode != nullptr;
				}

				bool CIndividualNodeIncrementalExpansionData::isCompatibilityChanged() {
					return mDirectlyChanged && hasDirectlyChangedNeighbourConnection();
				}

				bool CIndividualNodeIncrementalExpansionData::isDirectlyChanged() {
					return mDirectlyChanged;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setDirectlyChanged(bool directlyChanged) {
					mDirectlyChanged = directlyChanged;
					return this;
				}


				bool CIndividualNodeIncrementalExpansionData::isPreviousCompletionGraphCompatible() {
					return mCompletionGraphCompatible;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setPreviousCompletionGraphCompatible(bool compatible) {
					mCompletionGraphCompatible = compatible;
					return this;
				}


				CConceptDescriptor* CIndividualNodeIncrementalExpansionData::getLastCompatibleCheckedConceptDescriptor() {
					return mLastCompatibleCheckedConDes;
				}

				CLinkEdge* CIndividualNodeIncrementalExpansionData::getLastCompatibleCheckedLink() {
					return mLastCompatibleCheckedLink;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setLastCompatibleCheckedConceptDescriptor(CConceptDescriptor* conDes) {
					mLastCompatibleCheckedConDes = conDes;
					return this;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setLastCompatibleCheckedLink(CLinkEdge* link) {
					mLastCompatibleCheckedLink = link;
					return this;
				}

				CIndividualProcessNode* CIndividualNodeIncrementalExpansionData::getPreviousCompletionGraphCorrespondenceIndividualNode() {
					return mPrevCompGraphCorrIndiNode;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setPreviousCompletionGraphCorrespondenceIndividualNode(CIndividualProcessNode* node) {
					mPrevCompGraphCorrIndiNode = node;
					return this;
				}

				bool CIndividualNodeIncrementalExpansionData::isPreviousCompletionGraphCorrespondenceIndividualNodeLoaded() {
					return mPrevCompGraphCorrIndiNodeLoaded;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setPreviousCompletionGraphCorrespondenceIndividualNodeLoaded(bool loaded) {
					mPrevCompGraphCorrIndiNodeLoaded = loaded;
					return this;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::addNeighbourPropagatedDirectlyChanged(CIndividualProcessNode* indiNode) {
					getNeighbourPropagatedDirectlyChangedList(true)->append(indiNode);
					return this;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::clearNeighbourPropagatedDirectlyChangedList() {
					if (mUseNeighbourPropagatedDirectlyChangedList && !mUseNeighbourPropagatedDirectlyChangedList->isEmpty()) {
						getNeighbourPropagatedDirectlyChangedList(true)->clear();
					}
					return this;
				}

				bool CIndividualNodeIncrementalExpansionData::hasNeighbourPropagatedDirectlyChanged() {
					if (mUseNeighbourPropagatedDirectlyChangedList && !mUseNeighbourPropagatedDirectlyChangedList->isEmpty()) {
						return true;
					}
					return false;
				}


				CPROCESSLIST<CIndividualProcessNode*>* CIndividualNodeIncrementalExpansionData::getNeighbourPropagatedDirectlyChangedList(bool create) {
					if (!mLocNeighbourPropagatedDirectlyChangedList && create) {
						mLocNeighbourPropagatedDirectlyChangedList = CObjectParameterizingAllocator< CPROCESSLIST<CIndividualProcessNode*>,CContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseNeighbourPropagatedDirectlyChangedList) {
							*mLocNeighbourPropagatedDirectlyChangedList = *mUseNeighbourPropagatedDirectlyChangedList;
						}
						mUseNeighbourPropagatedDirectlyChangedList = mLocNeighbourPropagatedDirectlyChangedList;
					}
					return mUseNeighbourPropagatedDirectlyChangedList;
				}


				CPROCESSLIST<CIndividual*>* CIndividualNodeIncrementalExpansionData::getIncrementalExpansionList(bool create) {
					if (!mLocIncrementalExpansionList && create) {
						mLocIncrementalExpansionList = CObjectParameterizingAllocator< CPROCESSLIST<CIndividual*>,CContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (mUseIncrementalExpansionList) {
							*mLocIncrementalExpansionList = *mUseIncrementalExpansionList;
						}
						mUseIncrementalExpansionList = mLocIncrementalExpansionList;
					}
					return mUseIncrementalExpansionList;
				}

				bool CIndividualNodeIncrementalExpansionData::isIncremetnalExpansionListInitialized() {
					return mIncremetnalExpansionListInitialized;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setIncremetnalExpansionListInitialized(bool initialized) {
					mIncremetnalExpansionListInitialized = initialized;
					return this;
				}

				bool CIndividualNodeIncrementalExpansionData::requiresFurtherIncrementalExpansion() {
					if (mUseIncrementalExpansionList && !mUseIncrementalExpansionList->isEmpty()) {
						return true;
					}
					return false;
				}


				double CIndividualNodeIncrementalExpansionData::getExpansionPriority() {
					return mExpansionPriority;
				}

				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setExpansionPriority(double priority) {
					mExpansionPriority = priority;
					return this;
				}


				CIndividualNodeIncrementalExpansionData* CIndividualNodeIncrementalExpansionData::setExpansionID(cint64 id) {
					mExpansionPriority = (cint64)id;
					return this;
				}

				double CIndividualNodeIncrementalExpansionData::getNextIncrementalExpansionPriority() {
					return mExpansionPriority + 1.0;
				}

				CIndividual* CIndividualNodeIncrementalExpansionData::takeNextIncrementalExpansionIndividual() {
					if (mUseIncrementalExpansionList && !mUseIncrementalExpansionList->isEmpty()) {
						return mUseIncrementalExpansionList->takeFirst();
					}
					return nullptr;
				}

				CIndividual* CIndividualNodeIncrementalExpansionData::getNextIncrementalExpansionIndividual() {
					if (mUseIncrementalExpansionList && !mUseIncrementalExpansionList->isEmpty()) {
						return mUseIncrementalExpansionList->first();
					}
					return nullptr;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
