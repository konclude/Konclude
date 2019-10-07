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

#include "CLinkedNeighbourRoleAssertionSaturationHash.h"
#include "CIndividualSaturationProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CLinkedNeighbourRoleAssertionSaturationHash::CLinkedNeighbourRoleAssertionSaturationHash(CProcessContext* context) : mContext(context),mNeighbourRoleAssertionDataHash(context) {
				}


				CLinkedNeighbourRoleAssertionSaturationHash* CLinkedNeighbourRoleAssertionSaturationHash::initNeighbourRoleAssertionHash() {
					mNeighbourRoleAssertionDataHash.clear();
					return this;
				}

				CLinkedNeighbourRoleAssertionSaturationHash* CLinkedNeighbourRoleAssertionSaturationHash::copyNeighbourRoleAssertionHash(CLinkedNeighbourRoleAssertionSaturationHash* copyNeighbourRoleAssertionHash) {
					mNeighbourRoleAssertionDataHash = copyNeighbourRoleAssertionHash->mNeighbourRoleAssertionDataHash;
					return this;
				}



				CLinkedNeighbourRoleAssertionSaturationHash* CLinkedNeighbourRoleAssertionSaturationHash::addNeighbourRoleAssertion(cint64 sourceIndiId, cint64 targetIndiId, CRole* assertedRole, bool roleInversed, CIndividualSaturationProcessNode* connectedSatNode) {
					CLinkedNeighbourRoleAssertionSaturationNodeLinker* linker = CObjectAllocator<CLinkedNeighbourRoleAssertionSaturationNodeLinker>::allocateAndConstruct(mContext->getMemoryAllocationManager());
					linker->initLinkedNeighbourRoleAssertionLinker(assertedRole, roleInversed, connectedSatNode);
					CLinkedNeighbourRoleAssertionSaturationData* neighbourSatData = getNeighbourRoleAssertionData(targetIndiId, true);
					neighbourSatData->addLinkedRoleAssertedLinker(linker);
					return this;
				}


				CLinkedNeighbourRoleAssertionSaturationData* CLinkedNeighbourRoleAssertionSaturationHash::getNeighbourRoleAssertionData(cint64 neighbourIndiId, bool createIfNotExists) {
					if (createIfNotExists) {
						CLinkedNeighbourRoleAssertionSaturationData*& data = mNeighbourRoleAssertionDataHash[neighbourIndiId];
						if (!data) {
							data = CObjectAllocator<CLinkedNeighbourRoleAssertionSaturationData>::allocateAndConstruct(mContext->getMemoryAllocationManager());
						}
						return data;
					} else {
						return mNeighbourRoleAssertionDataHash.value(neighbourIndiId);
					}
				}


				CLinkedNeighbourRoleAssertionSaturationNodeLinker* CLinkedNeighbourRoleAssertionSaturationHash::getNeighbourRoleAssertionLinker(cint64 neighbourIndiId) {
					CLinkedNeighbourRoleAssertionSaturationData* neighbourSatData = getNeighbourRoleAssertionData(neighbourIndiId, false);
					if (neighbourSatData) {
						return neighbourSatData->getLinkedRoleAssertedLinker();
					}
					return nullptr;
				}


				CPROCESSHASH<cint64, CLinkedNeighbourRoleAssertionSaturationData*>* CLinkedNeighbourRoleAssertionSaturationHash::getNeighbourRoleAssertionDataHash() {
					return &mNeighbourRoleAssertionDataHash;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
