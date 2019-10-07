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

#include "CIndividualSaturationProcessNodeExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualSaturationProcessNodeExtensionData::CIndividualSaturationProcessNodeExtensionData(CProcessContext* processContext) : mProcessContext(processContext) {
					mMemAllocMan = mProcessContext->getMemoryAllocationManager();
				}

				CIndividualSaturationProcessNodeExtensionData* CIndividualSaturationProcessNodeExtensionData::initIndividualExtensionData(CIndividualSaturationProcessNode* indiNode) {
					mIndiNode = indiNode;
					mDisComConExtData = nullptr;
					mLinkedRoleSuccHash = nullptr;
					mLinkedNeighbourRoleAssertionHash = nullptr;
					mLinkedDataValueAssertionData = nullptr;
					mCriticalConceptTypeQueues = nullptr;
					mSuccessorExtensionData = nullptr;
					mNominalHandlingData = nullptr;
					mCriticalPredRoleCardHash = nullptr;
					mRoleAssertionLinker = nullptr;
					mAppliedDatatypeData = nullptr;
					mATMOSTSuccessorMergingData = nullptr;
					return this;
				}

				CSaturationDisjunctCommonConceptExtractionData* CIndividualSaturationProcessNodeExtensionData::getDisjunctCommonConceptExtractionData(bool create) {
					if (!mDisComConExtData && create) {
						mDisComConExtData = CObjectParameterizingAllocator< CSaturationDisjunctCommonConceptExtractionData,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mDisComConExtData->initExtractionData(mIndiNode);
					}
					return mDisComConExtData;
				}


				CLinkedRoleSaturationSuccessorHash* CIndividualSaturationProcessNodeExtensionData::getLinkedRoleSuccessorHash(bool create) {
					if (create && !mLinkedRoleSuccHash) {
						mLinkedRoleSuccHash = CObjectParameterizingAllocator< CLinkedRoleSaturationSuccessorHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mLinkedRoleSuccHash->initRoleSuccessorHash();
					}
					return mLinkedRoleSuccHash;
				}


				CLinkedNeighbourRoleAssertionSaturationHash* CIndividualSaturationProcessNodeExtensionData::getLinkedNeighbourRoleAssertionHash(bool create) {
					if (create && !mLinkedNeighbourRoleAssertionHash) {
						mLinkedNeighbourRoleAssertionHash = CObjectParameterizingAllocator< CLinkedNeighbourRoleAssertionSaturationHash, CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan, mProcessContext);
						mLinkedNeighbourRoleAssertionHash->initNeighbourRoleAssertionHash();
					}
					return mLinkedNeighbourRoleAssertionHash;
				}



				CLinkedDataValueAssertionSaturationData* CIndividualSaturationProcessNodeExtensionData::getLinkedDataValueAssertionData(bool create) {
					if (create && !mLinkedDataValueAssertionData) {
						mLinkedDataValueAssertionData = CObjectParameterizingAllocator< CLinkedDataValueAssertionSaturationData, CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan, mProcessContext);
						mLinkedDataValueAssertionData->initDataValueAssertionData();
					}
					return mLinkedDataValueAssertionData;
				}

				CCriticalSaturationConceptTypeQueues* CIndividualSaturationProcessNodeExtensionData::getCriticalConceptTypeQueues(bool create) {
					if (!mCriticalConceptTypeQueues && create) {
						mCriticalConceptTypeQueues = CObjectParameterizingAllocator< CCriticalSaturationConceptTypeQueues,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mCriticalConceptTypeQueues->initCriticalSaturationConceptQueues(mIndiNode);
					}
					return mCriticalConceptTypeQueues;
				}


				CSaturationIndividualNodeSuccessorExtensionData* CIndividualSaturationProcessNodeExtensionData::getSuccessorExtensionData(bool create) {
					if (!mSuccessorExtensionData && create) {
						mSuccessorExtensionData = CObjectParameterizingAllocator< CSaturationIndividualNodeSuccessorExtensionData,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mSuccessorExtensionData->initExtensionData(mIndiNode);
					}
					return mSuccessorExtensionData;
				}


				CSaturationIndividualNodeNominalHandlingData* CIndividualSaturationProcessNodeExtensionData::getNominalHandlingData(bool create) {
					if (!mNominalHandlingData && create) {
						mNominalHandlingData = CObjectParameterizingAllocator< CSaturationIndividualNodeNominalHandlingData,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mNominalHandlingData->initNominalHandlingData(mIndiNode);
					}
					return mNominalHandlingData;
				}


				CCriticalPredecessorRoleCardinalityHash* CIndividualSaturationProcessNodeExtensionData::getCriticalPredecessorRoleCardinalityHash(bool create) {
					if (!mCriticalPredRoleCardHash && create) {
						mCriticalPredRoleCardHash = CObjectParameterizingAllocator< CCriticalPredecessorRoleCardinalityHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mCriticalPredRoleCardHash->initCriticalPredecessorRoleCardinalityHash();
					}
					return mCriticalPredRoleCardHash;
				}

				CSaturationIndividualNodeDatatypeData* CIndividualSaturationProcessNodeExtensionData::getAppliedDatatypeData(bool create) {
					if (!mAppliedDatatypeData && create) {
						mAppliedDatatypeData = CObjectParameterizingAllocator< CSaturationIndividualNodeDatatypeData,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mAppliedDatatypeData->initExtensionData(mIndiNode);
					}
					return mAppliedDatatypeData;
				}

				CSaturationATMOSTSuccessorMergingData* CIndividualSaturationProcessNodeExtensionData::getATMOSTSuccessorMergingData(bool create) {
					if (!mATMOSTSuccessorMergingData && create) {
						mATMOSTSuccessorMergingData = CObjectParameterizingAllocator< CSaturationATMOSTSuccessorMergingData,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mATMOSTSuccessorMergingData->initSuccessorMergingData(mIndiNode);
					}
					return mATMOSTSuccessorMergingData;
				}

				CSaturationSuccessorRoleAssertionLinker* CIndividualSaturationProcessNodeExtensionData::getRoleAssertionLinker() {
					return mRoleAssertionLinker;
				}


				CIndividualSaturationProcessNodeExtensionData* CIndividualSaturationProcessNodeExtensionData::addRoleAssertionLinker(CSaturationSuccessorRoleAssertionLinker* roleAssertionLinker) {
					if (roleAssertionLinker) {
						mRoleAssertionLinker = roleAssertionLinker->append(mRoleAssertionLinker);
					}
					return this;
				}

				CIndividualSaturationProcessNodeExtensionData* CIndividualSaturationProcessNodeExtensionData::addRoleAssertion(CIndividualSaturationProcessNode* destinationNode, CRole* role, bool roleNegation) {
					CSaturationSuccessorRoleAssertionLinker* roleAssertionLinker = CObjectAllocator<CSaturationSuccessorRoleAssertionLinker>::allocateAndConstruct(mMemAllocMan);
					roleAssertionLinker->initSaturationSuccessorRoleAssertionLinker(destinationNode,role,roleNegation);
					addRoleAssertionLinker(roleAssertionLinker);
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
