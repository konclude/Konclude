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
