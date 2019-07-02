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

#include "CReapplyQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CReapplyQueue::CReapplyQueue() {
					mStaticReapplyDesLinker = nullptr;
					mDynamicReapplyDesLinker = nullptr;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT
					mDesStaticCount = 0;
					mDesDynamicCount = 0;
#endif
				}



				CReapplyQueue* CReapplyQueue::initReapplyQueue(CReapplyQueue* prevReapplyQueue) {
					if (prevReapplyQueue) {
						mStaticReapplyDesLinker = prevReapplyQueue->mStaticReapplyDesLinker;
						mDynamicReapplyDesLinker = prevReapplyQueue->mDynamicReapplyDesLinker;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT
						mDesDynamicCount = prevReapplyQueue->mDesDynamicCount;
						mDesStaticCount = prevReapplyQueue->mDesStaticCount;
#endif
					} else {
						mStaticReapplyDesLinker = nullptr;
						mDynamicReapplyDesLinker = nullptr;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT
						mDesStaticCount = 0;
						mDesDynamicCount = 0;
#endif
					}
					return this;
				}

#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT
				qint64 CReapplyQueue::getReapplyCount() {
					return mDesStaticCount + mDesDynamicCount;
				}
#endif

				bool CReapplyQueue::isEmpty() {
					return !mDynamicReapplyDesLinker && !mStaticReapplyDesLinker;
				}

				bool CReapplyQueue::hasConceptDescriptor(CConceptDescriptor* conceptDescriptor) {
					CReapplyConceptDescriptor* desLinker = mStaticReapplyDesLinker;
					while (desLinker) {
						if (desLinker->hasConceptDescriptor(conceptDescriptor)) {
							return true;
						}
						desLinker = desLinker->getNext();
					}
					desLinker = mDynamicReapplyDesLinker;
					while (desLinker) {
						if (desLinker->hasConceptDescriptor(conceptDescriptor)) {
							return true;
						}
						desLinker = desLinker->getNext();
					}
					return false;
				}


				CReapplyQueue* CReapplyQueue::addReapplyConceptDescriptor(CReapplyConceptDescriptor *conProDes) {
					if (conProDes) {
						if (conProDes->isStaticDescriptor()) {
							mStaticReapplyDesLinker = conProDes->append(mStaticReapplyDesLinker);
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT
							++mDesStaticCount;
#endif
						} else {
							mDynamicReapplyDesLinker = conProDes->append(mDynamicReapplyDesLinker);
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT
							++mDesDynamicCount;
#endif
						}
					}
					return this;
				}


				CReapplyQueueIterator CReapplyQueue::getIterator(bool clearDynamicReapplyQueue) {
					CReapplyQueueIterator it(mStaticReapplyDesLinker,mDynamicReapplyDesLinker);
					if (clearDynamicReapplyQueue) {
						mDynamicReapplyDesLinker = nullptr;
#ifdef KONCLUDE_EMPLOY_REASONER_KERNEL_PROCESS_REAPPLY_QUEUE_COUNT
						mDesDynamicCount = 0;
#endif
					}
					return it;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
