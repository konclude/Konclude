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

#include "CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData(CProcessContext* context) {
					mContext = context;
				}


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::~CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData() {
				}



				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::initSynchronisationData(CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* synchData) {
					mRoleNeighbourExpansionDataHash = nullptr;
					mNeighbourExpansionDataHash = nullptr;
					mNeighbourLabelExpansionDataHashArray = nullptr;
					mIntegratedMergedIndividualIdSet = nullptr;
					mRoleRepNeighbourCountHash = nullptr;
					mRemainingIndirectCompatibilityMergedIndividualCheckingSet = nullptr;
					if (synchData) {
						mLastCriticalNeighbourTestedConDes = synchData->mLastCriticalNeighbourTestedConDes;
						mLastSynchronizationTestedConDes = synchData->mLastSynchronizationTestedConDes;
						mLastIndirectConnectedIndiTestedConDes = synchData->mLastIndirectConnectedIndiTestedConDes;
						mLastSynchedConDes = synchData->mLastSynchedConDes;
						mLastNeighbourInfluenceTestedConDes = synchData->mLastNeighbourInfluenceTestedConDes;
						mNeighbourInfluenceTestingCriticalCardinalityReset = synchData->mNeighbourInfluenceTestingCriticalCardinalityReset;
						mLastMergedIntoIndividualTestingCriticalCardinalityReset = synchData->mLastMergedIntoIndividualTestingCriticalCardinalityReset;
						mAssociationData = synchData->mAssociationData;
						mSynchron = synchData->mSynchron;
						mCriticalNeighbour = synchData->mCriticalNeighbour;
						mDeterministicSameIndividualMerged = synchData->mDeterministicSameIndividualMerged;
						mDeterministicDifferentIndividualDifferentiated = synchData->mDeterministicDifferentIndividualDifferentiated;
						mPrioritziedPropagationLinksEstablished = synchData->mPrioritziedPropagationLinksEstablished;
						mCriticalIndirectConnectedIndis = synchData->mCriticalIndirectConnectedIndis;
						mNomainlIndirectConnectionIndividualExpanded = synchData->mNomainlIndirectConnectionIndividualExpanded;
						mLastCriticalNeighbourLinkEdge = synchData->mLastCriticalNeighbourLinkEdge;
						mCriticalCardinalityInitiallyChecked = synchData->mCriticalCardinalityInitiallyChecked;
						mLastNewNeighbourLinkEdge = synchData->mLastNewNeighbourLinkEdge;
						mIndirectConnectedIndiTestedLinkEdge = synchData->mIndirectConnectedIndiTestedLinkEdge;
						mCriticalCardinality = synchData->mCriticalCardinality;
						mLastMergedIndiCount = synchData->mLastMergedIndiCount;
						mLastProcessedMergedIndividualLinker = synchData->mLastProcessedMergedIndividualLinker;
						mConceptSetLabelProcessedIndiNodeLinker = synchData->mConceptSetLabelProcessedIndiNodeLinker;
						mNonConceptSetRelatedProcessing = synchData->mNonConceptSetRelatedProcessing;
						mNonConceptSetNeighbourLabelRelatedProcessing = synchData->mNonConceptSetNeighbourLabelRelatedProcessing;
						mNeighbourLabelRepresentativeExpansionLinker = synchData->mNeighbourLabelRepresentativeExpansionLinker;
						mNeighbourLabelRepresentativeExpansion = synchData->mNeighbourLabelRepresentativeExpansion;
						mBackendConceptSetInitialized = synchData->mBackendConceptSetInitialized;
						mBackendConceptSetInitializationQueued = synchData->mBackendConceptSetInitializationQueued;
						mBackendConceptSetInitializationRequired = synchData->mBackendConceptSetInitializationRequired;
						mBackendConceptSetInitializationDelayingRegistered = synchData->mBackendConceptSetInitializationDelayingRegistered;
						if (synchData->mRoleNeighbourExpansionDataHash) {
							getRoleNeighbourExpansionDataHash()->init(synchData->mRoleNeighbourExpansionDataHash);
						}
						if (synchData->mNeighbourExpansionDataHash) {
							getNeighbourExpansionDataHash()->init(synchData->mNeighbourExpansionDataHash);
						}
						if (synchData->mNeighbourLabelExpansionDataHashArray) {
							cint64 arraySize = mAssociationData->getRoleSetNeighbourArray()->getIndexData()->getArraySize();
							for (cint64 i = 0; i < arraySize; ++i) {
								CPROCESSHASH< CBackendRepresentativeMemoryLabelCacheItem*, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData >* hash = synchData->mNeighbourLabelExpansionDataHashArray[i];
								getNeighbourLabelExpansionDataHash(i, true)->init(hash);
							}
						}
						if (synchData->mIntegratedMergedIndividualIdSet) {
							getIntegratedMergedIndividualIdSet()->init(synchData->mIntegratedMergedIndividualIdSet);
						}
						if (synchData->mRemainingIndirectCompatibilityMergedIndividualCheckingSet) {
							getRemainingIndirectCompatibilityMergedIndividualCheckingSet()->init(synchData->mRemainingIndirectCompatibilityMergedIndividualCheckingSet);
						}
						if (synchData->mRoleRepNeighbourCountHash) {
							getRoleRepresentativeNeighbourCountHash(true)->init(synchData->mRoleRepNeighbourCountHash);
						}
						mMergedIndividualsAllNeighbourExpanded = synchData->mMergedIndividualsAllNeighbourExpanded;
						mAllNeighbourExpansion = synchData->mAllNeighbourExpansion;
						mAllNeighbourForcedExpansion = synchData->mAllNeighbourForcedExpansion;
						mAllNeighbourExpansionScheduled = synchData->mAllNeighbourExpansionScheduled;
						mAllNeighbourForcedExpansionScheduled = synchData->mAllNeighbourForcedExpansionScheduled;
						mMergedIndirectlyConnectedNominalIndividuals = synchData->mMergedIndirectlyConnectedNominalIndividuals;
						mMergedIndiNodeLinker = synchData->mMergedIndiNodeLinker;
						mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker = synchData->mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker;
						mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker = synchData->mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker;
						mLastSynchronizedConceptsTestedMergedNodeLinker = synchData->mLastSynchronizedConceptsTestedMergedNodeLinker;
						mLastCriticalNeighboursTestedMergedNodeLinker = synchData->mLastCriticalNeighboursTestedMergedNodeLinker;
						mLastDirectExpansionHandledMergedNodeLinker = synchData->mLastDirectExpansionHandledMergedNodeLinker;
						mLastInferringExpansionHandledMergedNodeLinker = synchData->mLastInferringExpansionHandledMergedNodeLinker;
						mLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker = synchData->mLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker;
						mLastReuseExpansionHandledMergedNodeLinker = synchData->mLastReuseExpansionHandledMergedNodeLinker;
						mLastIndirectCompatibilityExpansionHandledMergedNodeLinker = synchData->mLastIndirectCompatibilityExpansionHandledMergedNodeLinker;
						mBackendExpansionReuseDependencyTrackPoint = synchData->mBackendExpansionReuseDependencyTrackPoint;
						mReuseNonDeterministicSameIndividualMerged = synchData->mReuseNonDeterministicSameIndividualMerged;
						mReuseNonDeterministicDifferentIndividualStated = synchData->mReuseNonDeterministicDifferentIndividualStated;
						mReuseNonDeterministicConceptsAdded = synchData->mReuseNonDeterministicConceptsAdded;
						mUseBackendNeighbourExpansion = synchData->mUseBackendNeighbourExpansion;
						mPrevBackendNeighbourExpansion = synchData->mUseBackendNeighbourExpansion;
						mBackendNeighbourExpansionQueue = nullptr;
						mNewlyMergedIndividuals = synchData->mNewlyMergedIndividuals;
						mNewlyMergedAllNeighbourExpansion = synchData->mNewlyMergedAllNeighbourExpansion;
						mNewlyMergedInferringNeighbourExpansion = synchData->mNewlyMergedInferringNeighbourExpansion;
						mNonDeterministicallyMergedIndividuals = synchData->mNonDeterministicallyMergedIndividuals;
						mScheduledIndividual = synchData->mScheduledIndividual;
					} else {
						mLastIndirectConnectedIndiTestedConDes = nullptr;
						mLastSynchronizationTestedConDes = nullptr;
						mLastCriticalNeighbourTestedConDes = nullptr;
						mLastSynchedConDes = nullptr;
						mLastNeighbourInfluenceTestedConDes = nullptr;
						mNeighbourInfluenceTestingCriticalCardinalityReset = false;
						mLastMergedIntoIndividualTestingCriticalCardinalityReset = -1;
						mAssociationData = nullptr;
						mLastCriticalNeighbourLinkEdge = nullptr;
						mCriticalCardinalityInitiallyChecked = false;
						mLastNewNeighbourLinkEdge = nullptr;
						mIndirectConnectedIndiTestedLinkEdge = nullptr;
						mSynchron = false;
						mCriticalNeighbour = false;
						mDeterministicSameIndividualMerged = false;
						mDeterministicDifferentIndividualDifferentiated = false;
						mPrioritziedPropagationLinksEstablished = false;
						mCriticalIndirectConnectedIndis = false;
						mNomainlIndirectConnectionIndividualExpanded = false;
						mCriticalCardinality = false;
						mRoleNeighbourExpansionDataHash = nullptr;
						mNeighbourExpansionDataHash = nullptr;
						mNeighbourLabelExpansionDataHashArray = nullptr;
						mConceptSetLabelProcessedIndiNodeLinker = nullptr;
						mLastProcessedMergedIndividualLinker = nullptr;
						mLastMergedIndiCount = 0;
						mNonConceptSetRelatedProcessing = false;
						mNonConceptSetNeighbourLabelRelatedProcessing = false;
						mNeighbourLabelRepresentativeExpansionLinker = nullptr;
						mNeighbourLabelRepresentativeExpansion = false;
						mBackendConceptSetInitialized = false;
						mBackendConceptSetInitializationQueued = false;
						mBackendConceptSetInitializationRequired = false;
						mBackendConceptSetInitializationDelayingRegistered = false;
						mMergedIndividualsAllNeighbourExpanded = false;
						mAllNeighbourExpansion = false;
						mAllNeighbourForcedExpansion = false;
						mAllNeighbourExpansionScheduled = false;
						mAllNeighbourForcedExpansionScheduled = false;
						mMergedIndirectlyConnectedNominalIndividuals = false;
						mMergedIndiNodeLinker = nullptr;
						mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker = nullptr;
						mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker = nullptr;
						mLastSynchronizedConceptsTestedMergedNodeLinker = nullptr;
						mRoleRepNeighbourCountHash = nullptr;
						mLastCriticalNeighboursTestedMergedNodeLinker = nullptr;
						mLastDirectExpansionHandledMergedNodeLinker = nullptr;
						mLastInferringExpansionHandledMergedNodeLinker = nullptr;
						mLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker = nullptr;
						mLastReuseExpansionHandledMergedNodeLinker = nullptr;
						mLastIndirectCompatibilityExpansionHandledMergedNodeLinker = nullptr;
						mBackendExpansionReuseDependencyTrackPoint = nullptr;
						mReuseNonDeterministicSameIndividualMerged = false;
						mReuseNonDeterministicDifferentIndividualStated = false;
						mReuseNonDeterministicConceptsAdded = false;
						mBackendNeighbourExpansionQueue = nullptr;
						mUseBackendNeighbourExpansion = nullptr;
						mPrevBackendNeighbourExpansion = nullptr;
						mNewlyMergedIndividuals = false;
						mNewlyMergedAllNeighbourExpansion = false;
						mNewlyMergedInferringNeighbourExpansion = false;
						mNonDeterministicallyMergedIndividuals = false;
						mScheduledIndividual = false;
					}
					return this;
				}


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::initSynchronisationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData) {
					mLastIndirectConnectedIndiTestedConDes = nullptr;
					mLastCriticalNeighbourTestedConDes = nullptr;
					mLastSynchronizationTestedConDes = nullptr;
					mLastSynchedConDes = nullptr;
					mLastNeighbourInfluenceTestedConDes = nullptr;
					mNeighbourInfluenceTestingCriticalCardinalityReset = false;
					mLastMergedIntoIndividualTestingCriticalCardinalityReset = -1;
					mCriticalCardinalityInitiallyChecked = false;
					mLastCriticalNeighbourLinkEdge = nullptr;
					mLastNewNeighbourLinkEdge = nullptr;
					mIndirectConnectedIndiTestedLinkEdge = nullptr;
					mAssociationData = associationData;
					mSynchron = false;
					mCriticalNeighbour = false;
					mDeterministicSameIndividualMerged = false;
					mDeterministicDifferentIndividualDifferentiated = false;
					mPrioritziedPropagationLinksEstablished = false;
					mCriticalIndirectConnectedIndis = false;
					mNomainlIndirectConnectionIndividualExpanded = false;
					mCriticalCardinality = false;
					mRoleNeighbourExpansionDataHash = nullptr;
					mNeighbourExpansionDataHash = nullptr;
					mNeighbourLabelExpansionDataHashArray = nullptr;
					mLastProcessedMergedIndividualLinker = nullptr;
					mLastMergedIndiCount = 0;
					mConceptSetLabelProcessedIndiNodeLinker = nullptr;
					mNonConceptSetRelatedProcessing = false;
					mNonConceptSetNeighbourLabelRelatedProcessing = false;
					mNeighbourLabelRepresentativeExpansionLinker = nullptr;
					mNeighbourLabelRepresentativeExpansion = false;
					mBackendConceptSetInitialized = false;
					mBackendConceptSetInitializationQueued = false;
					mBackendConceptSetInitializationRequired = false;
					mBackendConceptSetInitializationDelayingRegistered = false;
					mIntegratedMergedIndividualIdSet = nullptr;
					mMergedIndividualsAllNeighbourExpanded = false;
					mAllNeighbourExpansion = false;
					mAllNeighbourForcedExpansion = false;
					mAllNeighbourExpansionScheduled = false;
					mAllNeighbourForcedExpansionScheduled = false;
					mMergedIndirectlyConnectedNominalIndividuals = false;
					mMergedIndiNodeLinker = nullptr;
					mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker = nullptr;
					mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker = nullptr;
					mLastSynchronizedConceptsTestedMergedNodeLinker = nullptr;
					mRoleRepNeighbourCountHash = nullptr;
					mLastCriticalNeighboursTestedMergedNodeLinker = nullptr;
					mLastDirectExpansionHandledMergedNodeLinker = nullptr;
					mLastInferringExpansionHandledMergedNodeLinker = nullptr;
					mLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker = nullptr;
					mLastReuseExpansionHandledMergedNodeLinker = nullptr;
					mLastIndirectCompatibilityExpansionHandledMergedNodeLinker = nullptr;
					mRemainingIndirectCompatibilityMergedIndividualCheckingSet = nullptr;
					mBackendExpansionReuseDependencyTrackPoint = nullptr;
					mReuseNonDeterministicSameIndividualMerged = false;
					mReuseNonDeterministicDifferentIndividualStated = false;
					mReuseNonDeterministicConceptsAdded = false;
					mBackendNeighbourExpansionQueue = nullptr;
					mUseBackendNeighbourExpansion = nullptr;
					mPrevBackendNeighbourExpansion = nullptr;
					mNewlyMergedIndividuals = false;
					mNewlyMergedAllNeighbourExpansion = false;
					mNewlyMergedInferringNeighbourExpansion = false;
					mNonDeterministicallyMergedIndividuals = false;
					mScheduledIndividual = false;
					return this;
				}



				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNewlyMergedIndividuals() {
					return mNewlyMergedIndividuals;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setNewlyMergedIndividuals(bool newlyMerged) {
					mNewlyMergedIndividuals = newlyMerged;
					return this;
				}



				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNonDeterministicallyMergedIndividuals() {
					return mNonDeterministicallyMergedIndividuals;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setNonDeterministicallyMergedIndividuals(bool nonDeterministicallyMerged) {
					mNonDeterministicallyMergedIndividuals = nonDeterministicallyMerged;
					return this;
				}




				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNewlyMergedAllNeighbourExpansion() {
					return mNewlyMergedAllNeighbourExpansion;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setNewlyMergedAllNeighbourExpansion(bool allExpanded) {
					mNewlyMergedAllNeighbourExpansion = allExpanded;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNewlyMergedInferringNeighbourExpansion() {
					return mNewlyMergedInferringNeighbourExpansion;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setNewlyMergedInferringNeighbourExpansion(bool inferringExpanded) {
					mNewlyMergedInferringNeighbourExpansion = inferringExpanded;
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



				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasCriticalCardinalityInitiallyChecked() {
					return mCriticalCardinalityInitiallyChecked;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setCriticalCardinalityInitiallyChecked(bool checked) {
					mCriticalCardinalityInitiallyChecked = checked;
					return this;
				}




				CIndividualLinkEdge* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastNewNeighbourLinkEdge() {
					return mLastNewNeighbourLinkEdge;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastNewNeighbourLinkEdge(CIndividualLinkEdge* linkEdge) {
					mLastNewNeighbourLinkEdge = linkEdge;
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

				CPROCESSHASH< QPair<CRole*, bool>, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData >* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getRoleNeighbourExpansionDataHash(bool createIfNotExists) {
					if (!mRoleNeighbourExpansionDataHash && createIfNotExists) {
						mRoleNeighbourExpansionDataHash = CObjectParameterizingAllocator< CPROCESSHASH< QPair<CRole*, bool>, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData >, CProcessContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(), mContext);
					}
					return mRoleNeighbourExpansionDataHash;
				}


				CPROCESSHASH< cint64, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationNeighbourExpansionData >* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getNeighbourExpansionDataHash(bool createIfNotExists) {
					if (!mNeighbourExpansionDataHash && createIfNotExists) {
						mNeighbourExpansionDataHash = CObjectParameterizingAllocator< CPROCESSHASH< cint64, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationNeighbourExpansionData >, CProcessContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(), mContext);
					}
					return mNeighbourExpansionDataHash;
				}



				CPROCESSHASH< CBackendRepresentativeMemoryLabelCacheItem*, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData >* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getNeighbourLabelExpansionDataHash(cint64 arrayId, bool createIfNotExists) {
					if (!mNeighbourLabelExpansionDataHashArray) {
						cint64 arraySize = 0; 
						arraySize = mAssociationData->getRoleSetNeighbourArray()->getIndexData()->getArraySize();
						mNeighbourLabelExpansionDataHashArray = CObjectAllocator< CPROCESSHASH< CBackendRepresentativeMemoryLabelCacheItem*, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData >* >::allocateAndConstructArray(mContext->getUsedMemoryAllocationManager(), arraySize);
					}
					CPROCESSHASH< CBackendRepresentativeMemoryLabelCacheItem*, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData >*& hash = mNeighbourLabelExpansionDataHashArray[arrayId];
					if (!hash) {
						hash = CObjectParameterizingAllocator< CPROCESSHASH< CBackendRepresentativeMemoryLabelCacheItem*, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData >, CProcessContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(), mContext);
					}
					return hash;
				}




				CPROCESSHASH< CRole*, cint64 >* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getRoleRepresentativeNeighbourCountHash(bool create) {
					if (!mRoleRepNeighbourCountHash && create) {
						mRoleRepNeighbourCountHash = CObjectParameterizingAllocator< CPROCESSHASH< CRole*, cint64 >, CProcessContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(), mContext);
					}
					return mRoleRepNeighbourCountHash;
				}



				CConceptDescriptor* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastNeighbourInfluenceTestedConceptDescriptor() {
					return mLastNeighbourInfluenceTestedConDes;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastNeighbourInfluenceTestedConceptDescriptor(CConceptDescriptor* conDes) {
					mLastNeighbourInfluenceTestedConDes = conDes;
					return this;
				}





				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNeighbourInfluenceTestingCriticalCardinalityReset() {
					return mNeighbourInfluenceTestingCriticalCardinalityReset;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNeighbourInfluenceTestingCriticalCardinalityReset(bool reset) {
					mNeighbourInfluenceTestingCriticalCardinalityReset = reset;
					return this;
				}


				cint64 CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastMergedIntoIndividualTestingCriticalCardinalityReset() {
					return mLastMergedIntoIndividualTestingCriticalCardinalityReset;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastMergedIntoIndividualTestingCriticalCardinalityReset(cint64 indiId) {
					mLastMergedIntoIndividualTestingCriticalCardinalityReset = indiId;
					return this;
				}





				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::isCriticalIndirectConnectionIndividualExpansionBlocking() {
					return mCriticalIndirectConnectedIndis;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setCriticalIndirectConnectionIndividualExpansionBlocking(bool critical) {
					mCriticalIndirectConnectedIndis = critical;
					return this;
				}




				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNominalIndirectConnectionIndividualExpanded() {
					return mNomainlIndirectConnectionIndividualExpanded;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setNominalIndirectConnectionIndividualExpanded(bool expanded) {
					mNomainlIndirectConnectionIndividualExpanded = expanded;
					return this;
				}






				CIndividualLinkEdge* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastIndirectConnectedIndividualExpansionTestedLinkEdge() {
					return mIndirectConnectedIndiTestedLinkEdge;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastIndirectConnectedIndividualExpansionTestedLinkEdge(CIndividualLinkEdge* linkEdge) {
					mIndirectConnectedIndiTestedLinkEdge = linkEdge;
					return this;
				}


				CConceptDescriptor* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastIndirectConnectedIndividualExpansionTestedConceptDescriptor() {
					return mLastIndirectConnectedIndiTestedConDes;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastIndirectConnectedIndividualExpansionTestedConceptDescriptor(CConceptDescriptor* conDes) {
					mLastIndirectConnectedIndiTestedConDes = conDes;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasDeterministicSameIndividualMerged() {
					return mDeterministicSameIndividualMerged;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setDeterministicSameIndividualMerged(bool merged) {
					mDeterministicSameIndividualMerged = merged;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasDeterministicDifferentIndividualDifferentiated() {
					return mDeterministicDifferentIndividualDifferentiated;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setDeterministicDifferentIndividualDifferentiated(bool differentiated) {
					mDeterministicDifferentIndividualDifferentiated = differentiated;
					return this;
				}



				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasPrioritziedPropagationLinksEstablished() {
					return mPrioritziedPropagationLinksEstablished;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setPrioritziedPropagationLinksEstablished(bool established) {
					mPrioritziedPropagationLinksEstablished = established;
					return this;
				}



				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getMergedIndividualNodeLinker() {
					return mMergedIndiNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::addMergedIndividualNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mMergedIndiNodeLinker = linker->append(mMergedIndiNodeLinker);
					return this;
				}





				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker() {
					return mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker = linker;
					return this;
				}



				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker() {
					return mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker = linker;
					return this;
				}






				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastSynchronizedConceptsTestedMergedNodeLinker() {
					return mLastSynchronizedConceptsTestedMergedNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastSynchronizedConceptsTestedMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mLastSynchronizedConceptsTestedMergedNodeLinker = linker;
					return this;
				}


				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastCriticalNeighboursTestedMergedNodeLinker() {
					return mLastCriticalNeighboursTestedMergedNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastCriticalNeighboursTestedMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mLastCriticalNeighboursTestedMergedNodeLinker = linker;
					return this;
				}



				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastDirectExpansionHandledMergedNodeLinker() {
					return mLastDirectExpansionHandledMergedNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastDirectExpansionHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mLastDirectExpansionHandledMergedNodeLinker = linker;
					return this;
				}





				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastInferringExpansionHandledMergedNodeLinker() {
					return mLastInferringExpansionHandledMergedNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastInferringExpansionHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mLastInferringExpansionHandledMergedNodeLinker = linker;
					return this;
				}



				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker() {
					return mLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mLastNewlyMergedExpansionRequiringCheckingMergedNodeLinker = linker;
					return this;
				}





				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastIndirectCompatibilityExpansionHandledMergedNodeLinker() {
					return mLastIndirectCompatibilityExpansionHandledMergedNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastIndirectCompatibilityExpansionHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mLastIndirectCompatibilityExpansionHandledMergedNodeLinker = linker;
					return this;
				}





				CPROCESSSET<cint64>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getIntegratedMergedIndividualIdSet(bool createIfNotExists) {
					if (!mIntegratedMergedIndividualIdSet && createIfNotExists) {
						mIntegratedMergedIndividualIdSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>, CProcessContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(), mContext);
					}
					return mIntegratedMergedIndividualIdSet;
				}



				CPROCESSSET<cint64>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getRemainingIndirectCompatibilityMergedIndividualCheckingSet(bool createIfNotExists) {
					if (!mRemainingIndirectCompatibilityMergedIndividualCheckingSet && createIfNotExists) {
						mRemainingIndirectCompatibilityMergedIndividualCheckingSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>, CProcessContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(), mContext);
					}
					return mRemainingIndirectCompatibilityMergedIndividualCheckingSet;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasMergedIndividualsAllNeighbourExpanded() {
					return mMergedIndividualsAllNeighbourExpanded;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setMergedIndividualsAllNeighbourExpanded(bool expanded) {
					mMergedIndividualsAllNeighbourExpanded = expanded;
					return this;
				}



				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasMergedIndirectlyConnectedNominalIndividuals() {
					return mMergedIndirectlyConnectedNominalIndividuals;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setMergedIndirectlyConnectedNominalIndividuals(bool connection) {
					mMergedIndirectlyConnectedNominalIndividuals = connection;
					return this;
				}



				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasAllNeighbourExpansion() {
					return mAllNeighbourExpansion;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setAllNeighbourExpansion(bool expanded) {
					mAllNeighbourExpansion = expanded;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasAllNeighbourForcedExpansion() {
					return mAllNeighbourForcedExpansion;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setAllNeighbourForcedExpansion(bool expanded) {
					mAllNeighbourForcedExpansion = expanded;
					return this;
				}







				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasAllNeighbourExpansionScheduled() {
					return mAllNeighbourExpansionScheduled;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setAllNeighbourExpansionScheduled(bool expansionScheduled) {
					mAllNeighbourExpansionScheduled = expansionScheduled;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasAllNeighbourForcedExpansionScheduled() {
					return mAllNeighbourForcedExpansionScheduled;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setAllNeighbourForcedExpansionScheduled(bool expansionScheduled) {
					mAllNeighbourForcedExpansionScheduled = expansionScheduled;
					return this;
				}







				cint64 CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastMergedIndividualCount() {
					return mLastMergedIndiCount;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastMergedIndividualCount(cint64 count) {
					mLastMergedIndiCount = count;
					return this;
				}


				CXLinker<cint64>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastProcessedMergedIndividualLinker() {
					return mLastProcessedMergedIndividualLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastProcessedMergedIndividualLinker(CXLinker<cint64>* indiLinker) {
					mLastProcessedMergedIndividualLinker = indiLinker;
					return this;
				}



				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getConceptSetLabelProcessedNodeLinker() {
					return mConceptSetLabelProcessedIndiNodeLinker;
				}


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setConceptSetLabelProcessedNodeLinker(CXLinker<CIndividualProcessNode*>* nodeLinker) {
					mConceptSetLabelProcessedIndiNodeLinker = nodeLinker;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNonConceptSetBackendLabelRelatedProcessing() {
					return mNonConceptSetRelatedProcessing;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setNonConceptSetBackendLabelRelatedProcessing(bool nonConceptSetRelatedProcessing) {
					mNonConceptSetRelatedProcessing = nonConceptSetRelatedProcessing;
					return this;
				}



				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNonConceptSetBackendNeighbourLabelRelatedProcessing() {
					return mNonConceptSetNeighbourLabelRelatedProcessing;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setNonConceptSetBackendNeighbourLabelRelatedProcessing(bool nonConceptSetRelatedProcessing) {
					mNonConceptSetNeighbourLabelRelatedProcessing = nonConceptSetRelatedProcessing;
					return this;
				}




				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNeighbourLabelRepresentativeExpansionInstalled() {
					return mNeighbourLabelRepresentativeExpansionLinker != nullptr;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::installNeighbourLabelRepresentativeExpansion(CXLinker<CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData*>* linker) {
					mNeighbourLabelRepresentativeExpansionLinker = linker->append(mNeighbourLabelRepresentativeExpansionLinker);
					return this;
				}

				CXLinker<CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getNeighbourLabelRepresentativeExpansionLinker() {
					return mNeighbourLabelRepresentativeExpansionLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::clearNeighbourLabelRepresentativeExpansionLinker() {
					mNeighbourLabelRepresentativeExpansionLinker = nullptr;
					return this;
				}





				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasNeighbourLabelRepresentativeExpansion() {
					return mNeighbourLabelRepresentativeExpansion;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setNeighbourLabelRepresentativeExpansion(bool repExp) {
					mNeighbourLabelRepresentativeExpansion = repExp;
					return this;
				}








				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::isBackendConceptSetInitialized() {
					return mBackendConceptSetInitialized;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setBackendConceptSetInitialized(bool initialized) {
					mBackendConceptSetInitialized = initialized;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::isBackendConceptSetInitializationQueued() {
					return mBackendConceptSetInitializationQueued;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setBackendConceptSetInitializationQueued(bool queued) {
					mBackendConceptSetInitializationQueued = queued;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::isBackendConceptSetInitializationRequired() {
					return mBackendConceptSetInitializationRequired;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setBackendConceptSetInitializationRequired(bool initRequired) {
					mBackendConceptSetInitializationRequired = initRequired;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::isBackendConceptSetInitializationDelayingRegistered() {
					return mBackendConceptSetInitializationDelayingRegistered;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setBackendConceptSetInitializationDelayingRegistered(bool registered) {
					mBackendConceptSetInitializationDelayingRegistered = registered;
					return this;
				}

				CNonDeterministicDependencyTrackPoint* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getBackendExpansionReuseDependencyTrackPoint() {
					return mBackendExpansionReuseDependencyTrackPoint;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setBackendExpansionReuseDependencyTrackPoint(CNonDeterministicDependencyTrackPoint* depTrackPoint) {
					mBackendExpansionReuseDependencyTrackPoint = depTrackPoint;
					return this;
				}




				CXLinker<CIndividualProcessNode*>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getLastReuseExpansionHandledMergedNodeLinker() {
					return mLastReuseExpansionHandledMergedNodeLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setLastReuseExpansionHandledMergedNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mLastReuseExpansionHandledMergedNodeLinker = linker;
					return this;
				}

				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasReuseNonDeterministicSameIndividualMerged() {
					return mReuseNonDeterministicSameIndividualMerged;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setReuseNonDeterministicSameIndividualMerged(bool merged) {
					mReuseNonDeterministicSameIndividualMerged = merged;
					return this;
				}

				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasReuseNonDeterministicDifferentIndividualStated() {
					return mReuseNonDeterministicDifferentIndividualStated;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setReuseNonDeterministicDifferentIndividualStated(bool stated) {
					mReuseNonDeterministicDifferentIndividualStated = stated;
					return this;
				}

				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasReuseNonDeterministicConceptsAdded() {
					return mReuseNonDeterministicConceptsAdded;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setReuseNonDeterministicConceptsAdded(bool added) {
					mReuseNonDeterministicConceptsAdded = added;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::isScheduledIndividual() {
					return mScheduledIndividual;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setScheduledIndividual(bool scheduled) {
					mScheduledIndividual = scheduled;
					return this;
				}


				CBackendNeighbourExpansionQueue* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::getBackendNeighbourExpansionQueue(bool create) {
					if (!mBackendNeighbourExpansionQueue && create) {
						mBackendNeighbourExpansionQueue = CObjectParameterizingAllocator< CBackendNeighbourExpansionQueue, CProcessContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(), mContext);
						mBackendNeighbourExpansionQueue->initBackendNeighbourExpansionQueue(mPrevBackendNeighbourExpansion);
						mUseBackendNeighbourExpansion = mBackendNeighbourExpansionQueue;
					}
					return mUseBackendNeighbourExpansion;
				}


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::clearBackendNeighbourExpansionQueue() {
					mBackendNeighbourExpansionQueue = nullptr;
					mUseBackendNeighbourExpansion = nullptr;
					mPrevBackendNeighbourExpansion = nullptr;
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
