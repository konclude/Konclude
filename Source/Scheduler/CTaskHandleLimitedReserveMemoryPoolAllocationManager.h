/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_SCHEDULER_CTASKHANDLELIMITEDRESERVEMEMORYPOOLALLOCATIONMANAGER_H
#define KONCLUDE_SCHEDULER_CTASKHANDLELIMITEDRESERVEMEMORYPOOLALLOCATIONMANAGER_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskHandleMemoryAllocationManager.h"


// Other includes
#include "Utilities/Memory/CLimitedReserveMemoryPoolAllocationManager.h"
#include "Utilities/Memory/CConsiderateMemoryPoolProvider.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskHandleLimitedReserveMemoryPoolAllocationManager
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskHandleLimitedReserveMemoryPoolAllocationManager : public CLimitedReserveMemoryPoolAllocationManager, virtual public CTaskHandleMemoryAllocationManager {
			// public methods
			public:
				//! Constructor
				CTaskHandleLimitedReserveMemoryPoolAllocationManager(CConsiderateMemoryPoolProvider* memoryPoolProvider, cint64 freeReserveLimit = 20, cint64 freeReleaseBlockSize = 5);

				//! Destructor
				virtual ~CTaskHandleLimitedReserveMemoryPoolAllocationManager();

				virtual void* allocate(size_t size);
				virtual CMemoryAllocationManager* release(void* address);

				virtual void* allocateAligned(size_t size, int alignment);

				virtual CMemoryTemporaryAllocationManager* releaseAllMemory();

				virtual CMemoryPool* acqireTemporaryMemoryPool(size_t size);
				virtual CMemoryTemporaryAllocationManager* releaseTemporaryMemoryPool(CMemoryPool* memoryPool);

				virtual cint64 getOptimizedMoreAllocationSize(cint64 size);

				virtual CMemoryTemporaryAllocationManager* releaseMemoryPoolContainer(CMemoryPoolContainer* memoryPoolContainer);

				virtual CMemoryPoolProvider* releaseMemoryPool(CMemoryPool* memoryPool);
				virtual CMemoryPool* acquireMemoryPool(cint64 minPoolSize);

				virtual CMemoryPool* acquireMemoryPoolConsiderated(cint64 minPoolSize);

			// protected methods
			protected:
				CConsiderateMemoryPoolProvider* mConsideratedMemPoolProvider;

			// protected variables
			protected:

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKHANDLELIMITEDRESERVEMEMORYPOOLALLOCATIONMANAGER_H
