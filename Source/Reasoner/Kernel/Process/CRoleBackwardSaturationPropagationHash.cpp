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

#include "CRoleBackwardSaturationPropagationHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRoleBackwardSaturationPropagationHash::CRoleBackwardSaturationPropagationHash(CProcessContext* context) : mContext(context),mRoleBackPropDataHash(context) {
				}


				CRoleBackwardSaturationPropagationHash* CRoleBackwardSaturationPropagationHash::initRoleBackwardSaturationPropagationHash() {
					mRoleBackPropDataHash.clear();
					mSelfConnected = false;
					return this;
				}

				CRoleBackwardSaturationPropagationHash* CRoleBackwardSaturationPropagationHash::copyRoleBackwardSaturationPropagationHash(CRoleBackwardSaturationPropagationHash* copyRoleBackPilPropHash, CIndividualSaturationProcessNode* newIndiNode) {
					CMemoryAllocationManager* memMan = mContext->getUsedMemoryAllocationManager();
					mRoleBackPropDataHash = copyRoleBackPilPropHash->mRoleBackPropDataHash;
					mRoleBackPropDataHash.detach();
					mSelfConnected = copyRoleBackPilPropHash->mSelfConnected;
					if (mSelfConnected) {
						// create a connection to itself for all corresponding roles
						for (CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>::iterator it = mRoleBackPropDataHash.begin(), itEnd = mRoleBackPropDataHash.end(); it != itEnd; ++it) {
							CRole* role = it.key();
							CRoleBackwardSaturationPropagationHashData& hashData(it.value());
							if (hashData.mSelfConnected) {
								CBackwardSaturationPropagationLink* selfBackLink = CObjectAllocator<CBackwardSaturationPropagationLink>::allocateAndConstruct(memMan);
								selfBackLink->initBackwardPropagationLink(newIndiNode,role);
								hashData.mLinkLinker = selfBackLink->append(hashData.mLinkLinker);
							}
						}
					}
					return this;
				}


				CBackwardSaturationPropagationReapplyDescriptor* CRoleBackwardSaturationPropagationHash::getBackwardPropagationBackwardPropagationConceptDescriptor(CRole* role) {
					const CRoleBackwardSaturationPropagationHashData backPropData = mRoleBackPropDataHash.value(role);
					return backPropData.mReapplyLinker;
				}


				CBackwardSaturationPropagationReapplyDescriptor* CRoleBackwardSaturationPropagationHash::addBackwardPropagationLink(CRole* role, CBackwardSaturationPropagationLink* link) {
					CRoleBackwardSaturationPropagationHashData& backPropData = mRoleBackPropDataHash[role];
					if (backPropData.mLinkLinker) {
						if (backPropData.mLinkLinker->getSourceIndividual() == link->getSourceIndividual()) {
							return nullptr;
						}
					}
					backPropData.mLinkLinker = link->append(backPropData.mLinkLinker);
					return backPropData.mReapplyLinker;
				}


				CBackwardSaturationPropagationReapplyDescriptor* CRoleBackwardSaturationPropagationHash::addSelfConnectedBackwardPropagationLink(CRole* role, CBackwardSaturationPropagationLink* link) {
					mSelfConnected = true;
					CRoleBackwardSaturationPropagationHashData& backPropData = mRoleBackPropDataHash[role];
					if (backPropData.mLinkLinker) {
						if (backPropData.mLinkLinker->getSourceIndividual() == link->getSourceIndividual()) {
							return nullptr;
						}
					}
					backPropData.mLinkLinker = link->append(backPropData.mLinkLinker);
					backPropData.mSelfConnected = true;
					return backPropData.mReapplyLinker;
				}


				CRoleBackwardSaturationPropagationHashData& CRoleBackwardSaturationPropagationHash::addBackwardPropagationConceptDescriptor(CRole* role, CBackwardSaturationPropagationReapplyDescriptor* reapplyConDes) {
					CRoleBackwardSaturationPropagationHashData& backPropData = mRoleBackPropDataHash[role];
					backPropData.mReapplyLinker = reapplyConDes->append(backPropData.mReapplyLinker);
					return backPropData;
				}


				CPROCESSHASH<CRole*,CRoleBackwardSaturationPropagationHashData>* CRoleBackwardSaturationPropagationHash::getRoleBackwardPropagationDataHash() {
					return &mRoleBackPropDataHash;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
