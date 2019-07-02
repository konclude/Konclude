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

#include "CCriticalSaturationConceptTypeQueues.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CCriticalSaturationConceptTypeQueues::CCriticalSaturationConceptTypeQueues(CProcessContext* context) : mContext(context) {
					mQueueVec = nullptr;
				}


				CCriticalSaturationConceptTypeQueues* CCriticalSaturationConceptTypeQueues::initCriticalSaturationConceptQueues(CIndividualSaturationProcessNode* indiNode) {
					mIndiNode = indiNode;
					mQueued = false;;
					CMemoryAllocationManager* memMan = mContext->getUsedMemoryAllocationManager();
					mQueueVec = CObjectAllocator< CCriticalSaturationConceptQueue* >::allocateAndConstructArray(memMan,mConceptTypeCount);
					for (cint64 i = 0; i < mConceptTypeCount; ++i) {
						mQueueVec[i] = nullptr;
					}
					return this;
				}


				bool CCriticalSaturationConceptTypeQueues::hasCriticalSaturationConceptsQueued() {
					for (cint64 i = 0; i < mConceptTypeCount; ++i) {
						CCriticalSaturationConceptQueue*& queue = mQueueVec[i];
						if (queue && queue->hasCriticalConceptDescriptorLinker()) {
							return true;
						}
					}
					return false;
				}

				CCriticalSaturationConceptQueue* CCriticalSaturationConceptTypeQueues::getCriticalSaturationConceptQueue(CRITICALSATURATIONCONCEPTQUEUETYPE type, bool create) {
					CCriticalSaturationConceptQueue*& queue = mQueueVec[(cint64)type];
					if (!queue && create) {
						CMemoryAllocationManager* memMan = mContext->getUsedMemoryAllocationManager();
						queue = CObjectAllocator< CCriticalSaturationConceptQueue >::allocateAndConstruct(memMan);
						queue->initCriticalSaturationConceptQueue(mIndiNode);
					}
					return queue;
				}


				bool CCriticalSaturationConceptTypeQueues::isProcessNodeQueued() {
					return mQueued;
				}

				CCriticalSaturationConceptTypeQueues* CCriticalSaturationConceptTypeQueues::setProcessNodeQueued(bool queued) {
					mQueued = queued;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
