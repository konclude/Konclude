/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CRoleBackwardPilingPropagationHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRoleBackwardPilingPropagationHash::CRoleBackwardPilingPropagationHash(CProcessContext* context) : mContext(context),mRoleBackPropDataHash(context) {
				}


				CRoleBackwardPilingPropagationHash* CRoleBackwardPilingPropagationHash::initRoleBackwardPilingPropagationHash() {
					mRoleBackPropDataHash.clear();
					return this;
				}

				CRoleBackwardPilingPropagationHash* CRoleBackwardPilingPropagationHash::copyRoleBackwardPilingPropagationHash(CRoleBackwardPilingPropagationHash* copyRoleBackPilPropHash, cint64 maxCopyPilingDepth) {
					CMemoryAllocationManager* memMan = mContext->getUsedMemoryAllocationManager();
					mRoleBackPropDataHash = copyRoleBackPilPropHash->mRoleBackPropDataHash;
					cint64 maxPilDepth = maxCopyPilingDepth;
					//mRoleBackPropDataHash.detachRestrictedCopy(&maxPilDepth);
					mRoleBackPropDataHash.detach();
					for (CPROCESSHASH<CRole*,CRoleBackwardPilingPropagationHashData>::iterator it = mRoleBackPropDataHash.begin(), itEnd = mRoleBackPropDataHash.end(); it != itEnd; ++it) {
						CRoleBackwardPilingPropagationHashData& data = it.value();
						CBackwardPilingPropagationReapplyDescriptor* reapplyLinkerIt = data.mReapplyLinker;
						data.mReapplyLinker = nullptr;
						CBackwardPilingPropagationReapplyDescriptor* lastReapplyLinker = nullptr;
						while (reapplyLinkerIt) {
							CBackwardPilingPropagationReapplyDescriptor* reapplyLinker = reapplyLinkerIt;
							if (reapplyLinker->getPilingDepth() <= maxPilDepth) {
								// copy reapply linker
								CBackwardPilingPropagationReapplyDescriptor* copyReapplyLinker = CObjectAllocator<CBackwardPilingPropagationReapplyDescriptor>::allocateAndConstruct(memMan);
								copyReapplyLinker->initBackwardPropagationReapplyDescriptor(reapplyLinker->getReapplyConceptPilingDescriptor(),0);
								if (lastReapplyLinker) {
									lastReapplyLinker->setNext(copyReapplyLinker);
								} else {
									data.mReapplyLinker = copyReapplyLinker;
								}
								lastReapplyLinker = copyReapplyLinker;
							}
							reapplyLinkerIt = reapplyLinkerIt->getNext();
						}
					}
					return this;
				}


				CBackwardPilingPropagationReapplyDescriptor* CRoleBackwardPilingPropagationHash::addBackwardPropagationLink(CRole* role, CBackwardPilingPropagationLink* link) {
					CRoleBackwardPilingPropagationHashData& backPropData = mRoleBackPropDataHash[role];
					if (backPropData.mLinkLinker) {
						if (backPropData.mLinkLinker->getSourceIndividual() == link->getSourceIndividual()) {
							if (backPropData.mLinkLinker->getSourcePilingDepth() <= link->getSourcePilingDepth() && backPropData.mLinkLinker->getPilingDepth() <= link->getPilingDepth()) {
								return nullptr;
							}
						}
						backPropData.mMaxLinkPilingDepth = qMax(link->getPilingDepth(),backPropData.mMaxLinkPilingDepth);
					} else {
						backPropData.mMaxLinkPilingDepth = link->getPilingDepth();
					}
					backPropData.mLinkLinker = link->append(backPropData.mLinkLinker);
					if (link->getPilingDepth() >= backPropData.mMinReapplyPilingDepth) {
						return backPropData.mReapplyLinker;
					} else {
						return nullptr;
					}
				}


				CBackwardPilingPropagationLink* CRoleBackwardPilingPropagationHash::addBackwardPropagationConceptDescriptor(CRole* role, CBackwardPilingPropagationReapplyDescriptor* reapplyConDes) {
					CRoleBackwardPilingPropagationHashData& backPropData = mRoleBackPropDataHash[role];
					if (backPropData.mReapplyLinker) {
						backPropData.mMinReapplyPilingDepth = qMin(backPropData.mMinReapplyPilingDepth,reapplyConDes->getPilingDepth());
					} else {
						backPropData.mMinReapplyPilingDepth = reapplyConDes->getPilingDepth();
					}
					backPropData.mReapplyLinker = reapplyConDes->append(backPropData.mReapplyLinker);
					if (backPropData.mMaxLinkPilingDepth >= reapplyConDes->getPilingDepth()) {
						return backPropData.mLinkLinker;
					} else {
						return nullptr;
					}
				}


				CPROCESSHASH<CRole*,CRoleBackwardPilingPropagationHashData>* CRoleBackwardPilingPropagationHash::getRoleBackwardPropagationDataHash() {
					return &mRoleBackPropDataHash;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
