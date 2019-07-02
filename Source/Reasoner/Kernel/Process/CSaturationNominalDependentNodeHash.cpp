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

#include "CSaturationNominalDependentNodeHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationNominalDependentNodeHash::CSaturationNominalDependentNodeHash(CProcessContext* processContext) : mNominalDependentNodeDataHash(processContext),mProcessContext(processContext) {
				}

				CSaturationNominalDependentNodeHash* CSaturationNominalDependentNodeHash::initNominalDependentNodeHash(CSaturationNominalDependentNodeHash* nominalDependentHash) {
					if (nominalDependentHash) {
						mNominalDependentNodeDataHash = nominalDependentHash->mNominalDependentNodeDataHash;
					} else {
						mNominalDependentNodeDataHash.clear();
					}
					return this;
				}

				CSaturationNominalDependentNodeData* CSaturationNominalDependentNodeHash::getNominalDependentNodeData(cint64 nominalID) {
					CSaturationNominalDependentNodeData* nominalDepNodeData = nullptr;
					const CSaturationNominalDependentNodeHashData& hashData = mNominalDependentNodeDataHash.value(nominalID);
					nominalDepNodeData = hashData.mNominalDependentNodeData;
					return nominalDepNodeData;
				}


				CSaturationNominalDependentNodeHash* CSaturationNominalDependentNodeHash::addNominalDependentNodeData(cint64 nominalID, CSaturationNominalDependentNodeData* dependentNodeData) {
					CSaturationNominalDependentNodeHashData& hashData = mNominalDependentNodeDataHash[nominalID];
					hashData.mNominalDependentNodeData = dependentNodeData->append(hashData.mNominalDependentNodeData);
					return this;
				}


				CSaturationNominalDependentNodeHash* CSaturationNominalDependentNodeHash::addNominalDependentNode(cint64 nominalID, CIndividualSaturationProcessNode* dependentNode, CSaturationNominalDependentNodeData::NOMINALCONNECTIONTYPE connectionType) {
					CSaturationNominalDependentNodeHashData& hashData = mNominalDependentNodeDataHash[nominalID];
					CSaturationNominalDependentNodeData* dependentNodeData = CObjectParameterizingAllocator<CSaturationNominalDependentNodeData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					dependentNodeData->initNominalDependentNodeData(dependentNode,connectionType);
					hashData.mNominalDependentNodeData = dependentNodeData->append(hashData.mNominalDependentNodeData);
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
