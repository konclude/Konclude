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

#include "CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker() : CLinkerBase<CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker*, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker>(this) {
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::initAccociationWriteData(CIndividual* individual, UPDATE_TYPE labelUpdateType, UPDATE_TYPE linksUpdateType) {
					initAccociationWriteData(individual->getIndividualID(), labelUpdateType, linksUpdateType);
					mIndividual = individual;
					return this;
				}


				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::initAccociationWriteData(cint64 indiID, UPDATE_TYPE labelUpdateType, UPDATE_TYPE linksUpdateType) {
					initCachingStatusFlags(CBackendRepresentativeMemoryCachingFlags::FLAG_COMPLETELY_HANDLED | CBackendRepresentativeMemoryCachingFlags::FLAG_COMPLETELY_SATURATED);
					mLabelUpdateType = labelUpdateType;
					mLinksUpdateType = linksUpdateType;
					mIndividual = nullptr;
					mIndividualID = indiID;
					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
						mReferredLabelData[i] = nullptr;
						mReferredTmpLabelData[i] = nullptr;
					}
					mReferredTmpCardData = nullptr;
					mReferredCardData = nullptr;
					mRoleSetNeighbourUpdateDataLinker = nullptr;
					mAssociationUpdateId = 0;
					mIntegratedIndirectlyConnectedIndividualsChangeId = 0;
					mRepresentativeSameIndiId = mIndividualID;
					mDeterministicSameIndiId = mIndividualID;
					mIndirectlyConnectedNominalIndividual = false;
					mIndirectlyConnectedIndividualIntegration = false;
					mRequireSameAsNeighboursCompletion = false;
					mScheduledIndividual = false;
					return this;
				}


				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setReferredLabel(cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryLabelReference& labelRef) {
					if (labelRef.getReferredLabelData()) {
						setReferredLabelData(labelType, labelRef.getReferredLabelData());
					} else {
						setReferredTemporaryLabelData(labelType, labelRef.getReferredTemporaryLabelData());
					}
					return this;
				}


				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setReferredTemporaryLabelData(cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* referredTmpLabelData) {
					if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL && referredTmpLabelData && referredTmpLabelData->getLabelType() != CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL) {
						bool debug = true;
					}
					mReferredTmpLabelData[labelType] = referredTmpLabelData;
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setReferredLabelData(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* referredLabelData) {
					if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL && referredLabelData && referredLabelData->getLabelType() != CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL) {
						bool debug = true;
					}
					mReferredLabelData[labelType] = referredLabelData;
					return this;
				}


				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setReferredTemporaryCardinalityData(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* referredTmpCardData) {
					mReferredTmpCardData = referredTmpCardData;
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setReferredCardinalityData(CBackendRepresentativeMemoryCardinalityCacheItem* referredCardData) {
					mReferredCardData = referredCardData;
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getReferredTemporaryCardinalityData() {
					return mReferredTmpCardData;
				}


				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getReferredCardinalityData() {
					return mReferredCardData;
				}



				CIndividual* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getIndividual() {
					return mIndividual;
				}

				cint64 CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getIndividualID() {
					return mIndividualID;
				}


				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getReferredTemporaryLabelData(cint64 labelType) {
					return mReferredTmpLabelData[labelType];
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getReferredLabelData(cint64 labelType) {
					return mReferredLabelData[labelType];
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::UPDATE_TYPE CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getLabelUpdateType() {
					return mLabelUpdateType;
				}


				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::UPDATE_TYPE CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getLinksUpdateType() {
					return mLinksUpdateType;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setLinksUpdateType(UPDATE_TYPE updateType) {
					mLinksUpdateType = updateType;
					return this;
				}


				CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getRoleSetNeighbourUpdateDataLinker() {
					return mRoleSetNeighbourUpdateDataLinker;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setRoleSetNeighbourUpdateDataLinker(CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinker) {
					mRoleSetNeighbourUpdateDataLinker = roleSetNeighbourUpdateDataLinker;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getUsedAssociationUpdateId() {
					return mAssociationUpdateId;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setUsedAssociationUpdateId(cint64 id) {
					mAssociationUpdateId = id;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getIntegratedIndirectlyConnectedIndividualsChangeId() {
					return mIntegratedIndirectlyConnectedIndividualsChangeId;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setIntegratedIndirectlyConnectedIndividualsChangeId(cint64 integratedChangeId) {
					mIntegratedIndirectlyConnectedIndividualsChangeId = integratedChangeId;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::isIndirectlyConnectedNominalIndividual() {
					return mIndirectlyConnectedNominalIndividual;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setIndirectlyConnectedNominalIndividual(bool indirectlyConnected) {
					mIndirectlyConnectedNominalIndividual = indirectlyConnected;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::hasIndirectlyConnectedIndividualIntegration() {
					return mIndirectlyConnectedIndividualIntegration;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setIndirectlyConnectedIndividualIntegration(bool indirectlyConnectedIndividualIntegration) {
					mIndirectlyConnectedIndividualIntegration = indirectlyConnectedIndividualIntegration;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getRepresentativeSameIndividualId() {
					return mRepresentativeSameIndiId;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setRepresentativeSameIndividualId(cint64 indiId) {
					mRepresentativeSameIndiId = indiId;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getDeterministicSameIndividualId() {
					return mDeterministicSameIndiId;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setDeterministicSameIndividualId(cint64 indiId) {
					mDeterministicSameIndiId = indiId;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::requireSameAsNeighboursCompletion() {
					return mRequireSameAsNeighboursCompletion;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setRequireSameAsNeighboursCompletion(bool requireCompletion) {
					mRequireSameAsNeighboursCompletion = requireCompletion;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::isScheduledIndividual() {
					return mScheduledIndividual;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setScheduledIndividual(bool scheduled) {
					mScheduledIndividual = scheduled;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
