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
