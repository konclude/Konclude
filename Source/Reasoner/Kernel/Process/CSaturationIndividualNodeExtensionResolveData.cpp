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

#include "CSaturationIndividualNodeExtensionResolveData.h"
#include "CIndividualSaturationProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationIndividualNodeExtensionResolveData::CSaturationIndividualNodeExtensionResolveData(CProcessContext* processContext) {
					mProcessContext = processContext;
					mExtensionResolveHash = nullptr;
					mIndiNode = nullptr;
					mIndiID = 0;
				}

				CSaturationIndividualNodeExtensionResolveData* CSaturationIndividualNodeExtensionResolveData::initExtensionResolveData(CIndividualSaturationProcessNode* indiProcessNode) {
					mIndiNode = indiProcessNode;
					mIndiID = indiProcessNode->getIndividualID();
					mExtensionResolveHash = nullptr;
					return this;
				}


				CSaturationIndividualNodeExtensionResolveData* CSaturationIndividualNodeExtensionResolveData::initExtensionResolveData(cint64 indiID) {
					mIndiNode = nullptr;
					mIndiID = indiID;
					mExtensionResolveHash = nullptr;
					return this;
				}


				CSaturationIndividualNodeExtensionResolveHash* CSaturationIndividualNodeExtensionResolveData::getIndividualNodeExtensionResolveHash(bool create) {
					if (!mExtensionResolveHash && create) {
						mExtensionResolveHash = CObjectParameterizingAllocator< CSaturationIndividualNodeExtensionResolveHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					}
					return mExtensionResolveHash;
				}

				CSaturationIndividualNodeExtensionResolveData* CSaturationIndividualNodeExtensionResolveData::setProcessingIndividualNode(CIndividualSaturationProcessNode* indiProcessNode) {
					mIndiNode = indiProcessNode;
					return this;
				}

				CIndividualSaturationProcessNode* CSaturationIndividualNodeExtensionResolveData::getProcessingIndividualNode() {
					return mIndiNode;
				}

				cint64 CSaturationIndividualNodeExtensionResolveData::getProcessingIndividualNodeID() {
					return mIndiID;
				}

				bool CSaturationIndividualNodeExtensionResolveData::hasProcessingIndividualNode() {
					return mIndiNode != nullptr;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
