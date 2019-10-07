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
					mIntegratedMergedIndividualIdSet = nullptr;
					mRemainingIndirectCompatibilityMergedIndividualCheckingSet = nullptr;
					if (synchData) {
						mLastCriticalNeighbourTestedConDes = synchData->mLastCriticalNeighbourTestedConDes;
						mLastSynchronizationTestedConDes = synchData->mLastSynchronizationTestedConDes;
						mLastIndirectConnectedIndiTestedConDes = synchData->mLastIndirectConnectedIndiTestedConDes;
						mLastSynchedConDes = synchData->mLastSynchedConDes;
						mLastNeighbourInfluenceTestedConDes = synchData->mLastNeighbourInfluenceTestedConDes;
						mNeighbourInfluenceTestingCriticalCardinalityReset = synchData->mNeighbourInfluenceTestingCriticalCardinalityReset;
						mAssociationData = synchData->mAssociationData;
						mSynchron = synchData->mSynchron;
						mCriticalNeighbour = synchData->mCriticalNeighbour;
						mDeterministicSameIndividualMerged = synchData->mDeterministicSameIndividualMerged;
						mDeterministicDifferentIndividualDifferentiated = synchData->mDeterministicDifferentIndividualDifferentiated;
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
						if (synchData->mIntegratedMergedIndividualIdSet) {
							getIntegratedMergedIndividualIdSet()->init(synchData->mIntegratedMergedIndividualIdSet);
						}
						if (synchData->mRemainingIndirectCompatibilityMergedIndividualCheckingSet) {
							getRemainingIndirectCompatibilityMergedIndividualCheckingSet()->init(synchData->mRemainingIndirectCompatibilityMergedIndividualCheckingSet);
						}
						mMergedIndividualsAllNeighbourExpanded = synchData->mMergedIndividualsAllNeighbourExpanded;
						mAllNeighbourExpansion = synchData->mAllNeighbourExpansion;
						mAllNeighbourForcedExpansion = synchData->mAllNeighbourForcedExpansion;
						mMergedIndirectlyConnectedNominalIndividuals = synchData->mMergedIndirectlyConnectedNominalIndividuals;
						mMergedIndiNodeLinker = synchData->mMergedIndiNodeLinker;
						mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker = synchData->mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker;
						mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker = synchData->mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker;
						mLastSynchronizedConceptsTestedMergedNodeLinker = synchData->mLastSynchronizedConceptsTestedMergedNodeLinker;
						mLastCriticalNeighboursTestedMergedNodeLinker = synchData->mLastCriticalNeighboursTestedMergedNodeLinker;
						mLastDirectExpansionHandledMergedNodeLinker = synchData->mLastDirectExpansionHandledMergedNodeLinker;
						mLastIndirectCompatibilityExpansionHandledMergedNodeLinker = synchData->mLastIndirectCompatibilityExpansionHandledMergedNodeLinker;
					} else {
						mLastIndirectConnectedIndiTestedConDes = nullptr;
						mLastSynchronizationTestedConDes = nullptr;
						mLastCriticalNeighbourTestedConDes = nullptr;
						mLastSynchedConDes = nullptr;
						mLastNeighbourInfluenceTestedConDes = nullptr;
						mNeighbourInfluenceTestingCriticalCardinalityReset = false;
						mAssociationData = nullptr;
						mLastCriticalNeighbourLinkEdge = nullptr;
						mCriticalCardinalityInitiallyChecked = false;
						mLastNewNeighbourLinkEdge = nullptr;
						mIndirectConnectedIndiTestedLinkEdge = nullptr;
						mSynchron = false;
						mCriticalNeighbour = false;
						mDeterministicSameIndividualMerged = false;
						mDeterministicDifferentIndividualDifferentiated = false;
						mCriticalIndirectConnectedIndis = false;
						mNomainlIndirectConnectionIndividualExpanded = false;
						mCriticalCardinality = false;
						mRoleNeighbourExpansionDataHash = nullptr;
						mNeighbourExpansionDataHash = nullptr;
						mConceptSetLabelProcessedIndiNodeLinker = nullptr;
						mLastProcessedMergedIndividualLinker = nullptr;
						mLastMergedIndiCount = 0;
						mNonConceptSetRelatedProcessing = false;
						mBackendConceptSetInitialized = false;
						mBackendConceptSetInitializationQueued = false;
						mBackendConceptSetInitializationRequired = false;
						mBackendConceptSetInitializationDelayingRegistered = false;
						mMergedIndividualsAllNeighbourExpanded = false;
						mAllNeighbourExpansion = false;
						mAllNeighbourForcedExpansion = false;
						mMergedIndirectlyConnectedNominalIndividuals = false;
						mMergedIndiNodeLinker = nullptr;
						mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker = nullptr;
						mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker = nullptr;
						mLastSynchronizedConceptsTestedMergedNodeLinker = nullptr;
						mLastCriticalNeighboursTestedMergedNodeLinker = nullptr;
						mLastDirectExpansionHandledMergedNodeLinker = nullptr;
						mLastIndirectCompatibilityExpansionHandledMergedNodeLinker = nullptr;
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
					mCriticalCardinalityInitiallyChecked = false;
					mLastCriticalNeighbourLinkEdge = nullptr;
					mLastNewNeighbourLinkEdge = nullptr;
					mIndirectConnectedIndiTestedLinkEdge = nullptr;
					mAssociationData = associationData;
					mSynchron = false;
					mCriticalNeighbour = false;
					mDeterministicSameIndividualMerged = false;
					mDeterministicDifferentIndividualDifferentiated = false;
					mCriticalIndirectConnectedIndis = false;
					mNomainlIndirectConnectionIndividualExpanded = false;
					mCriticalCardinality = false;
					mRoleNeighbourExpansionDataHash = nullptr;
					mNeighbourExpansionDataHash = nullptr;
					mLastProcessedMergedIndividualLinker = nullptr;
					mLastMergedIndiCount = 0;
					mConceptSetLabelProcessedIndiNodeLinker = nullptr;
					mNonConceptSetRelatedProcessing = false;
					mBackendConceptSetInitialized = false;
					mBackendConceptSetInitializationQueued = false;
					mBackendConceptSetInitializationRequired = false;
					mBackendConceptSetInitializationDelayingRegistered = false;
					mIntegratedMergedIndividualIdSet = nullptr;
					mMergedIndividualsAllNeighbourExpanded = false;
					mAllNeighbourExpansion = false;
					mAllNeighbourForcedExpansion = false;
					mMergedIndirectlyConnectedNominalIndividuals = false;
					mMergedIndiNodeLinker = nullptr;
					mLastIndirectlyConnectedNominalIndividualsHandledMergedNodeLinker = nullptr;
					mLastIndirectlyConnectedNominalIndividualsTestedMergedNodeLinker = nullptr;
					mLastSynchronizedConceptsTestedMergedNodeLinker = nullptr;
					mLastCriticalNeighboursTestedMergedNodeLinker = nullptr;
					mLastDirectExpansionHandledMergedNodeLinker = nullptr;
					mLastIndirectCompatibilityExpansionHandledMergedNodeLinker = nullptr;
					mRemainingIndirectCompatibilityMergedIndividualCheckingSet = nullptr;
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



				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasAllNeighbourExpansion() {
					return mAllNeighbourExpansion;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setAllNeighbourExpansion(bool expanded) {
					mAllNeighbourExpansion = expanded;
					return this;
				}



				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasMergedIndirectlyConnectedNominalIndividuals() {
					return mMergedIndirectlyConnectedNominalIndividuals;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setMergedIndirectlyConnectedNominalIndividuals(bool connection) {
					mMergedIndirectlyConnectedNominalIndividuals = connection;
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::hasAllNeighbourForcedExpansion() {
					return mAllNeighbourForcedExpansion;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData::setAllNeighbourForcedExpansion(bool expanded) {
					mAllNeighbourForcedExpansion = expanded;
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

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
