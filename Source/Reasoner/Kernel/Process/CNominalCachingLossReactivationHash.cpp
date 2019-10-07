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

#include "CNominalCachingLossReactivationHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CNominalCachingLossReactivationHash::CNominalCachingLossReactivationHash(CProcessContext* processContext) : mNominalReactivationDataHash(processContext),mProcessContext(processContext) {
				}

				CNominalCachingLossReactivationHash* CNominalCachingLossReactivationHash::initNominalDependentNodeHash(CNominalCachingLossReactivationHash* nominalDependentHash) {
					if (nominalDependentHash) {
						mNominalReactivationDataHash = nominalDependentHash->mNominalReactivationDataHash;
					} else {
						mNominalReactivationDataHash.clear();
					}
					return this;
				}


				CNominalCachingLossReactivationData* CNominalCachingLossReactivationHash::getNominalCachingLossReactivationData(CIndividualProcessNode* nominalIndiNode, bool create) {
					return getNominalCachingLossReactivationData(nominalIndiNode->getIndividualNodeID(),create);
				}


				CNominalCachingLossReactivationData* CNominalCachingLossReactivationHash::getNominalCachingLossReactivationData(cint64 nominalID, bool create) {
					CNominalCachingLossReactivationData* nominalNodeReactData = nullptr;
					if (!create) {
						const CNominalCachingLossReactivationHashData& hashData = mNominalReactivationDataHash.value(nominalID);
						nominalNodeReactData = hashData.mPrevReactivationData;
					} else {
						CNominalCachingLossReactivationHashData& hashData = mNominalReactivationDataHash[nominalID];	
						nominalNodeReactData = hashData.mReactivationData;
						if (!nominalNodeReactData) {
							nominalNodeReactData = CObjectParameterizingAllocator< CNominalCachingLossReactivationData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							nominalNodeReactData->initNominalCachingLossReactivationData(nominalID,hashData.mPrevReactivationData);
							hashData.mReactivationData = nominalNodeReactData;
							hashData.mPrevReactivationData = nominalNodeReactData;
						}
					}
					return nominalNodeReactData;
				}




			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
