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

#include "CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData() {
				}


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::~CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData() {
				}



				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::initSynchronisationData(CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* synchData) {
					if (synchData) {
						mLastCriticalNeighbourTestedConDes = synchData->mLastCriticalNeighbourTestedConDes;
						mLastSynchronizationTestedConDes = synchData->mLastSynchronizationTestedConDes;
						mLastSynchedConDes = synchData->mLastSynchedConDes;
						mAssociationData = synchData->mAssociationData;
						mSynchron = synchData->mSynchron;
						mCriticalNeighbour = synchData->mCriticalNeighbour;
						mLastCriticalNeighbourLinkEdge = synchData->mLastCriticalNeighbourLinkEdge;
						mCriticalCardinality = synchData->mCriticalCardinality;
					} else {
						mLastSynchronizationTestedConDes = nullptr;
						mLastCriticalNeighbourTestedConDes = nullptr;
						mLastSynchedConDes = nullptr;
						mAssociationData = nullptr;
						mLastCriticalNeighbourLinkEdge = nullptr;
						mSynchron = false;
						mCriticalNeighbour = false;
						mCriticalCardinality = false;
					}
					return this;
				}


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::initSynchronisationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData) {
					mLastCriticalNeighbourTestedConDes = nullptr;
					mLastSynchronizationTestedConDes = nullptr;
					mLastSynchedConDes = nullptr;
					mLastCriticalNeighbourLinkEdge = nullptr;
					mAssociationData = associationData;
					mSynchron = true;
					mCriticalNeighbour = false;
					mCriticalCardinality = false;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::isCriticalCardinalityExpansionBlocking() {
					return mCriticalCardinality;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setCriticalCardinalityExpansionBlocking(bool critical) {
					mCriticalCardinality = critical;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::isCriticalNeighbourExpansionBlocking() {
					return mCriticalNeighbour;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setCriticalNeighbourExpansionBlocking(bool critical) {
					mCriticalNeighbour = critical;
					return this;
				}


				CConceptDescriptor* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastSynchronizationTestedConceptDescriptor() {
					return mLastSynchronizationTestedConDes;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastSynchronizationTestedConceptDescriptor(CConceptDescriptor* conDes) {
					mLastSynchronizationTestedConDes = conDes;
					return this;
				}



				CIndividualLinkEdge* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastCriticalCardinalityLinkEdge() {
					return mLastCriticalNeighbourLinkEdge;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastCriticalCardinalityLinkEdge(CIndividualLinkEdge* linkEdge) {
					mLastCriticalNeighbourLinkEdge = linkEdge;
					return this;
				}


				CConceptDescriptor* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastCriticalNeighbourExpansionTestedConceptDescriptor() {
					return mLastCriticalNeighbourTestedConDes;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastCriticalNeighbourExpansionTestedConceptDescriptor(CConceptDescriptor* conDes) {
					mLastCriticalNeighbourTestedConDes = conDes;
					return this;
				}



				CConceptDescriptor* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastSynchedConceptDescriptor() {
					return mLastSynchedConDes;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastSynchedConceptDescriptor(CConceptDescriptor* conDes) {
					mLastSynchedConDes = conDes;
					return this;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getAssocitaionData() {
					return mAssociationData;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setAssociationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData) {
					mAssociationData = associationData;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::isBackendCacheSynchron() {
					return mSynchron;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setBackendCacheSynchron(bool synchron) {
					mSynchron = synchron;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
