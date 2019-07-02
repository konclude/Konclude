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

#include "CTaskAdditionalAllocationEvent.h"


namespace Konclude {

	namespace Scheduler {

		namespace Events {


			CTaskAdditionalAllocationEvent::CTaskAdditionalAllocationEvent(CMemoryPool* memoryPool) 
					: CEvent(EVENTTYPEID,memoryPool) {
				
				mAddMemoryPools = nullptr;
				mTask = nullptr;
			}


			CTaskAdditionalAllocationEvent::~CTaskAdditionalAllocationEvent() {
			}


			CTaskAdditionalAllocationEvent* CTaskAdditionalAllocationEvent::setTask(CTask* task) {
				mTask = task;
				return this;
			}

			CTask* CTaskAdditionalAllocationEvent::getTask() {
				return mTask;
			}

			CMemoryPool* CTaskAdditionalAllocationEvent::getAdditionalAllocatedMemoryPools() {
				return mAddMemoryPools;
			}

			CTaskAdditionalAllocationEvent* CTaskAdditionalAllocationEvent::setAdditionalAllocatedMemoryPools(CMemoryPool* addMemoryPools) {
				mAddMemoryPools = addMemoryPools;
				return this;
			}



		}; // end namespace Events

	}; // end namespace Scheduler

}; // end namespace Konclude
