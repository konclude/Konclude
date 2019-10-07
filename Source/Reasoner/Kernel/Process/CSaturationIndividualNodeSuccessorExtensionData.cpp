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

#include "CSaturationIndividualNodeSuccessorExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationIndividualNodeSuccessorExtensionData::CSaturationIndividualNodeSuccessorExtensionData(CProcessContext* processContext) {
					mProcessContext = processContext;
				}

				CSaturationIndividualNodeSuccessorExtensionData* CSaturationIndividualNodeSuccessorExtensionData::initExtensionData(CIndividualSaturationProcessNode* indiProcessNode) {
					mExtensionResolveData = nullptr;
					mIndiProcessNode = indiProcessNode;
					mALLConceptsExtensionData = nullptr;
					mFUNCTIONALConceptsExtensionData = nullptr;
					mAncSuccMergeResolveData = nullptr;
					mExtensionProcessingQueued = false;
					return this;
				}

				CSaturationIndividualNodeExtensionResolveData* CSaturationIndividualNodeSuccessorExtensionData::getExtensionResolveData() {
					return mExtensionResolveData;
				}


				CSaturationIndividualNodeSuccessorExtensionData* CSaturationIndividualNodeSuccessorExtensionData::setExtensionResolveData(CSaturationIndividualNodeExtensionResolveData* resolveData) {
					mExtensionResolveData = resolveData;
					return this;
				}


				bool CSaturationIndividualNodeSuccessorExtensionData::isExtensionProcessingQueued() {
					return mExtensionProcessingQueued;
				}

				CSaturationIndividualNodeSuccessorExtensionData* CSaturationIndividualNodeSuccessorExtensionData::setExtensionProcessingQueued(bool queued) {
					mExtensionProcessingQueued = queued;
					return this;
				}



				CSaturationIndividualNodeExtensionResolveData* CSaturationIndividualNodeSuccessorExtensionData::getAncestorSuccessorMergeResolveData(bool create) {
					if (!mAncSuccMergeResolveData && create) {
						mAncSuccMergeResolveData = CObjectParameterizingAllocator< CSaturationIndividualNodeExtensionResolveData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mAncSuccMergeResolveData->initExtensionResolveData(mIndiProcessNode);
					}
					return mAncSuccMergeResolveData;
				}


				CSaturationIndividualNodeExtensionResolveData* CSaturationIndividualNodeSuccessorExtensionData::getBaseExtensionResolveData(bool create) {
					if (!mExtensionResolveData && create) {
						mExtensionResolveData = CObjectParameterizingAllocator< CSaturationIndividualNodeExtensionResolveData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mExtensionResolveData->initExtensionResolveData(mIndiProcessNode);
					}
					return mExtensionResolveData;
				}

				CSaturationIndividualNodeALLConceptsExtensionData* CSaturationIndividualNodeSuccessorExtensionData::getALLConceptsExtensionData(bool create) {
					if (!mALLConceptsExtensionData && create) {
						mALLConceptsExtensionData = CObjectParameterizingAllocator< CSaturationIndividualNodeALLConceptsExtensionData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mALLConceptsExtensionData->initALLConceptsExtensionData(mIndiProcessNode);
					}
					return mALLConceptsExtensionData;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeSuccessorExtensionData::getFUNCTIONALConceptsExtensionData(bool create) {
					if (!mFUNCTIONALConceptsExtensionData && create) {
						mFUNCTIONALConceptsExtensionData = CObjectParameterizingAllocator< CSaturationIndividualNodeFUNCTIONALConceptsExtensionData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mFUNCTIONALConceptsExtensionData->initFUNCTIONALConceptsExtensionData(mIndiProcessNode);
					}
					return mFUNCTIONALConceptsExtensionData;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
