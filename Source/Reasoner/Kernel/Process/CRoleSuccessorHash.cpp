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

#include "CRoleSuccessorHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRoleSuccessorHash::CRoleSuccessorHash(CProcessContext* context) : mContext(context),mRoleSuccessorDataHash(context) {
				}

				CRoleSuccessorHash::~CRoleSuccessorHash() {
				}

				CRoleSuccessorHash* CRoleSuccessorHash::initRoleSuccessorHash(CRoleSuccessorHash* prevRoleSuccHash) {
					if (prevRoleSuccHash) {
						mRoleSuccessorDataHash = prevRoleSuccHash->mRoleSuccessorDataHash;
					} else {
						mRoleSuccessorDataHash.clear();
					}
					return this;
				}


				CRoleSuccessorHash* CRoleSuccessorHash::insertRoleSuccessorLink(CRole* role, CIndividualLinkEdge* link) {
					CRoleSuccessorData& roleSuccData = mRoleSuccessorDataHash[role];
					if (roleSuccData.mLinkSet) {
						if (!roleSuccData.mLocatedLinkSet) {
							CPROCESSHASH<cint64,CIndividualLinkEdge*>* newLinkSet = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CIndividualLinkEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							newLinkSet->init(roleSuccData.mLinkSet,mContext);
							roleSuccData.mLinkSet = newLinkSet;
							roleSuccData.mLocatedLinkSet = true;
						}
						roleSuccData.mLinkSet->insert(link->getCoupledIndividualID(),link);
					} else {
						roleSuccData.mLinkLinker = link->append(roleSuccData.mLinkLinker);
					}
					return this;
				}


				CRoleSuccessorHash* CRoleSuccessorHash::removeRoleSuccessorLink(CRole* role, CIndividualLinkEdge* link) {
					CRoleSuccessorData& roleSuccData = mRoleSuccessorDataHash[role];
					if (roleSuccData.mLinkSet) {
						if (!roleSuccData.mLocatedLinkSet) {
							CPROCESSHASH<cint64,CIndividualLinkEdge*>* newLinkSet = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CIndividualLinkEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							newLinkSet->init(roleSuccData.mLinkSet,mContext);
							roleSuccData.mLinkSet = newLinkSet;
							roleSuccData.mLocatedLinkSet = true;
						}
						roleSuccData.mLinkSet->remove(link->getCoupledIndividualID());
					} else {
						if (roleSuccData.mLinkLinker) {
							// replace by set
							CPROCESSHASH<cint64,CIndividualLinkEdge*>* newLinkSet = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CIndividualLinkEdge*>,CContext* >::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mContext),mContext);
							roleSuccData.mLinkSet = newLinkSet;
							roleSuccData.mLocatedLinkSet = true;

							CIndividualLinkEdge* linkIt = roleSuccData.mLinkLinker;
							while (linkIt) {
								if (linkIt != link) {
									newLinkSet->insert(linkIt->getCoupledIndividualID(),linkIt);
								}
								linkIt = linkIt->getNext();
							}
						}
					}
					return this;
				}


				CRoleSuccessorLinkIterator CRoleSuccessorHash::getRoleSuccessorLinkIterator(CRole* role) {
					CRoleSuccessorData* roleSuccData = nullptr;
					if (mRoleSuccessorDataHash.tryGetValuePointer(role,roleSuccData)) {
						if (roleSuccData->mLinkSet) {
							return CRoleSuccessorLinkIterator((roleSuccData->mLinkSet)->begin(),(roleSuccData->mLinkSet)->end());
						} else {
							return CRoleSuccessorLinkIterator(roleSuccData->mLinkLinker);
						}
					}
					return CRoleSuccessorLinkIterator(nullptr);
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
