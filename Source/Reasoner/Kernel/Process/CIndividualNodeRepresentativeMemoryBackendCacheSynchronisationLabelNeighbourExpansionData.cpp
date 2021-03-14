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

#include "CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData() {
					mDelayedRepresentativeExpansion = false;
					mConceptSetLabel = nullptr;
					mNeighbourExpansionArrayId = -1;
					mRepresentativeExpandedIndi = -1;
					mNextLabelNeighbourExpansionIteratorCursor = -1;
					mAllLabelNeighbourExpanded = false;
					mAllLabelNeighbourExpansionScheduled = false;
					mIndiNode = nullptr;
					mExpDataLinker = nullptr;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::isNeighbourLabelDelayedRepresentativeExpansion() {
					return mDelayedRepresentativeExpansion;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::setNeighbourLabelDelayedRepresentativeExpansion(bool delayedRepresentativeExpansion) {
					mDelayedRepresentativeExpansion = delayedRepresentativeExpansion;
					return this;
				}


				CBackendRepresentativeMemoryLabelCacheItem* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::getConceptSetLabel() {
					return mConceptSetLabel;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::setConceptSetLabel(CBackendRepresentativeMemoryLabelCacheItem* roleSetLabel) {
					mConceptSetLabel = roleSetLabel;
					return this;
				}


				cint64 CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::getRepresentativeExpandedIndividual() {
					return mRepresentativeExpandedIndi;
				}

				cint64 CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::getNeighbourExpansionArrayId() {
					return mNeighbourExpansionArrayId;
				}

				cint64 CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::getNextLabelNeighbourExpansionIteratorCursor() {
					return mNextLabelNeighbourExpansionIteratorCursor;
				}

				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::hasAllLabelNeighbourExpanded() {
					return mAllLabelNeighbourExpanded;
				}
				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::hasAllLabelNeighbourExpansionScheduled() {
					return mAllLabelNeighbourExpansionScheduled;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::setRepresentativeExpandedIndividual(cint64 expandedIndi) {
					mRepresentativeExpandedIndi = expandedIndi;
					return this;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::setNextLabelNeighbourExpansionIteratorCursor(cint64 cursor) {
					mNextLabelNeighbourExpansionIteratorCursor = cursor;
					return this;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::setNeighbourExpansionArrayId(cint64 arrayId) {
					mNeighbourExpansionArrayId = arrayId;
					return this;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::setAllLabelNeighbourExpanded(bool allExpanded) {
					mAllLabelNeighbourExpanded = allExpanded;
					return this;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::setAllLabelNeighbourExpansionScheduled(bool allExpansionScheduled) {
					mAllLabelNeighbourExpansionScheduled = allExpansionScheduled;
					return this;
				}


				CIndividualProcessNode* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::getExpandingIndividiaulNode() {
					return mIndiNode;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::setExpandingIndividiaulNode(CIndividualProcessNode* expIndiNode) {
					mIndiNode = expIndiNode;
					return this;
				}

				CBackendNeighbourExpansionQueueDataLinker* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::getExpandingQueueData() {
					return mExpDataLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData::setExpandingQueueData(CBackendNeighbourExpansionQueueDataLinker* expQueueData) {
					mExpDataLinker = expQueueData;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
