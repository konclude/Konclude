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

#include "CCondensedReapplyQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CCondensedReapplyQueue::CCondensedReapplyQueue() {
					mDynamicPosNegReapplyDesLinker = nullptr;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_CONDENSED_REAPPLY_QUEUE_COUNT
					mDynamicPosNegCount = 0;
#endif
				}


				CCondensedReapplyQueue* CCondensedReapplyQueue::initReapplyQueue(CCondensedReapplyQueue* prevReapplyQueue) {
					if (prevReapplyQueue) {
						mDynamicPosNegReapplyDesLinker = prevReapplyQueue->mDynamicPosNegReapplyDesLinker;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_CONDENSED_REAPPLY_QUEUE_COUNT
						mDynamicPosNegCount = prevReapplyQueue->mDynamicPosNegCount;
#endif
					} else {
						mDynamicPosNegReapplyDesLinker = nullptr;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_CONDENSED_REAPPLY_QUEUE_COUNT
						mDynamicPosNegCount = 0;
#endif
					}
					return this;
				}

#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_CONDENSED_REAPPLY_QUEUE_COUNT
				qint64 CCondensedReapplyQueue::getReapplyCount() {
					return mDynamicPosNegCount;
				}
#endif

				bool CCondensedReapplyQueue::isEmpty() const {
					return !mDynamicPosNegReapplyDesLinker;
				}


				bool CCondensedReapplyQueue::hasConceptDescriptor(CConceptDescriptor* conceptDescriptor) {
					CCondensedReapplyConceptDescriptor* desLinker = mDynamicPosNegReapplyDesLinker;
					while (desLinker) {
						if (desLinker->hasConceptDescriptor(conceptDescriptor)) {
							return true;
						}
						desLinker = desLinker->getNext();
					}
					return false;
				}


				CCondensedReapplyQueue* CCondensedReapplyQueue::addReapplyConceptDescriptor(CCondensedReapplyConceptDescriptor *conProDes) {
					if (conProDes) {
						mDynamicPosNegReapplyDesLinker = (CCondensedReapplyConceptDescriptor*)conProDes->append(mDynamicPosNegReapplyDesLinker);
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_CONDENSED_REAPPLY_QUEUE_COUNT
						++mDynamicPosNegCount;
#endif
					}
					return this;
				}

				CCondensedReapplyQueueIterator CCondensedReapplyQueue::getIterator(bool onlyPositiveDescriptors, bool clearDynamicReapplyQueue) {
					return getIterator(onlyPositiveDescriptors == true,onlyPositiveDescriptors == false,clearDynamicReapplyQueue);
				}

				CCondensedReapplyQueueIterator CCondensedReapplyQueue::getIterator(bool positiveDescriptors, bool negativeDescriptors, bool clearDynamicReapplyQueue) {
					CCondensedReapplyQueueIterator it(mDynamicPosNegReapplyDesLinker,positiveDescriptors,negativeDescriptors);
					if (clearDynamicReapplyQueue) {
						mDynamicPosNegReapplyDesLinker = nullptr;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_CONDENSED_REAPPLY_QUEUE_COUNT
						mDynamicPosNegCount = 0;
#endif
					}
					return it;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
