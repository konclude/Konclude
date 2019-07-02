/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
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
					return getNominalCachingLossReactivationData(nominalIndiNode->getIndividualID(),create);
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
