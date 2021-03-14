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

#include "CBackendNeighbourExpansionQueueDataLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CBackendNeighbourExpansionQueueDataLinker::CBackendNeighbourExpansionQueueDataLinker(CProcessContext* processContext) {
					setData(this);
				}


				CBackendNeighbourExpansionQueueDataLinker* CBackendNeighbourExpansionQueueDataLinker::initQueueData(CBackendNeighbourExpansionQueueDataLinker* queueData) {
					if (queueData) {
						mRole = queueData->mRole;
						mConcept = queueData->mConcept;
						mConNegation = queueData->mConNegation;
						mConceptNondeterministic = queueData->mConceptNondeterministic;
						mBaseIndiNode = queueData->mBaseIndiNode;
						mBackendSyncDataIndiNode = queueData->mBackendSyncDataIndiNode;
						mLastExpandedNeighbourId = queueData->mLastExpandedNeighbourId;
						mNeighbourVisitingCursor = queueData->mNeighbourVisitingCursor;
						mNeighbourArrayId = queueData->mNeighbourArrayId;
						mForceExpansion = queueData->mForceExpansion;
						mPropagationCutExpansion = queueData->mPropagationCutExpansion;
						mLabel = queueData->mLabel;
						mMissingNondeterministicExpansionPropagation = queueData->mMissingNondeterministicExpansionPropagation;
					} else {
						mRole = nullptr;
						mConcept = nullptr;
						mConNegation = false;
						mConceptNondeterministic = false;
						mBaseIndiNode = nullptr;
						mBackendSyncDataIndiNode = nullptr;
						mLastExpandedNeighbourId = -1;
						mNeighbourVisitingCursor = 0;
						mNeighbourArrayId = -1;
						mForceExpansion = false;
						mPropagationCutExpansion = false;
						mLabel = nullptr;
						mMissingNondeterministicExpansionPropagation = false;
					}
					return this;
				}

				CBackendNeighbourExpansionQueueDataLinker* CBackendNeighbourExpansionQueueDataLinker::initQueueData(cint64 neighbourArrayId, CRole* role, CConcept* concept, bool conNegation, bool nondeterministic, bool forceExpansion, CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* backendSyncDataIndiNode, cint64 lastExpandedNeighbourId, cint64 neighbourVisitingCoursor, bool propagationCutExpansion, bool missingNondeterministicExpansionPropagation) {
					mRole = role;
					mConcept = concept;
					mConNegation = conNegation;
					mConceptNondeterministic = nondeterministic;
					mBaseIndiNode = baseIndiNode;
					mBackendSyncDataIndiNode = backendSyncDataIndiNode;
					mLastExpandedNeighbourId = lastExpandedNeighbourId;
					mNeighbourVisitingCursor = neighbourVisitingCoursor;
					mNeighbourArrayId = neighbourArrayId;
					mForceExpansion = forceExpansion;
					mLabel = nullptr;
					mPropagationCutExpansion = propagationCutExpansion;
					mMissingNondeterministicExpansionPropagation = missingNondeterministicExpansionPropagation;
					return this;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendNeighbourExpansionQueueDataLinker::getExpandingLabel() {
					return mLabel;
				}

				CBackendNeighbourExpansionQueueDataLinker* CBackendNeighbourExpansionQueueDataLinker::setExpandingLabel(CBackendRepresentativeMemoryLabelCacheItem* label) {
					mLabel = label;
					return this;
				}


				CBackendNeighbourExpansionQueueDataLinker* CBackendNeighbourExpansionQueueDataLinker::updateNeighbourExpansionCursor(cint64 lastExpandedNeighbourId, cint64 neighbourVisitingCoursor) {
					mLastExpandedNeighbourId = lastExpandedNeighbourId;
					mNeighbourVisitingCursor = neighbourVisitingCoursor;
					return this;
				}

				cint64 CBackendNeighbourExpansionQueueDataLinker::getNeighbourArrayId() {
					return mNeighbourArrayId;
				}

				bool CBackendNeighbourExpansionQueueDataLinker::isForceExpansion() {
					return mForceExpansion;
				}

				bool CBackendNeighbourExpansionQueueDataLinker::isPropagationCutExpansion() {
					return mPropagationCutExpansion;
				}


				CRole* CBackendNeighbourExpansionQueueDataLinker::getRole() {
					return mRole;
				}

				CConcept* CBackendNeighbourExpansionQueueDataLinker::getConcept() {
					return mConcept;
				}

				bool CBackendNeighbourExpansionQueueDataLinker::getConceptNegation() {
					return mConNegation;
				}

				bool CBackendNeighbourExpansionQueueDataLinker::getConceptNondeterministic() {
					return mConceptNondeterministic;
				}

				CIndividualProcessNode* CBackendNeighbourExpansionQueueDataLinker::getBaseIndividualNode() {
					return mBaseIndiNode;
				}

				CIndividualProcessNode* CBackendNeighbourExpansionQueueDataLinker::getBackendSyncDataIndividualNode() {
					return mBackendSyncDataIndiNode;
				}

				cint64 CBackendNeighbourExpansionQueueDataLinker::getLastExpandedNeighbourId() {
					return mLastExpandedNeighbourId;
				}

				cint64 CBackendNeighbourExpansionQueueDataLinker::getNeighbourVisitingCursor() {
					return mNeighbourVisitingCursor;
				}


				bool CBackendNeighbourExpansionQueueDataLinker::isMissingNondeterministicExpansionPropagation() {
					return mMissingNondeterministicExpansionPropagation;
				}

				CBackendNeighbourExpansionQueueDataLinker* CBackendNeighbourExpansionQueueDataLinker::setMissingNondeterministicExpansionPropagation(bool missingProp) {
					mMissingNondeterministicExpansionPropagation = missingProp;
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
