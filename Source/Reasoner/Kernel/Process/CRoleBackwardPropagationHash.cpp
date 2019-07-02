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

#include "CRoleBackwardPropagationHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRoleBackwardPropagationHash::CRoleBackwardPropagationHash(CProcessContext* context) : mContext(context),mRoleBackPropDataHash(context) {
				}

				CBackwardPropagationReapplyDescriptor* CRoleBackwardPropagationHash::addBackwardPropagationLink(CRole* role, CBackwardPropagationLink* link) {
					CRoleBackwardPropagationHashData& backPropData = mRoleBackPropDataHash[role];
					if (backPropData.mLinkLinker) {
						if (backPropData.mLinkLinker->getSourceIndividual() == link->getSourceIndividual()) {
							return nullptr;
						}
					}
					backPropData.mLinkLinker = link->append(backPropData.mLinkLinker);
					return backPropData.mReapplyLinker;
				}


				CBackwardPropagationLink* CRoleBackwardPropagationHash::addBackwardPropagationConceptDescriptor(CRole* role, CBackwardPropagationReapplyDescriptor* reapplyConDes) {
					CRoleBackwardPropagationHashData& backPropData = mRoleBackPropDataHash[role];
					backPropData.mReapplyLinker = reapplyConDes->append(backPropData.mReapplyLinker);
					return backPropData.mLinkLinker;
				}


				CPROCESSHASH<CRole*,CRoleBackwardPropagationHashData>* CRoleBackwardPropagationHash::getRoleBackwardPropagationDataHash() {
					return &mRoleBackPropDataHash;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
