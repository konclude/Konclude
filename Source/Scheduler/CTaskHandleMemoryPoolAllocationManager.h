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

#ifndef KONCLUDE_SCHEDULER_CTASKHANDLEMEMORYPOOLALLOCATIONMANAGER_H
#define KONCLUDE_SCHEDULER_CTASKHANDLEMEMORYPOOLALLOCATIONMANAGER_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskHandleMemoryAllocationManager.h"


// Other includes
#include "Utilities/Memory/CMemoryPoolAllocationManager.h"
#include "Utilities/Memory/CMemoryPoolProvider.h"
#include "Utilities/Memory/CConsiderateMemoryPoolProvider.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskHandleMemoryPoolAllocationManager
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskHandleMemoryPoolAllocationManager : public CMemoryPoolAllocationManager, virtual public CTaskHandleMemoryAllocationManager {
			// public methods
			public:
				//! Constructor
				CTaskHandleMemoryPoolAllocationManager(CConsiderateMemoryPoolProvider* memoryPoolProvider);

				//! Destructor
				virtual ~CTaskHandleMemoryPoolAllocationManager();

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

			// protected variables
			protected:
				CConsiderateMemoryPoolProvider* mConsideratedMemPoolProvider;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKHANDLEMEMORYPOOLALLOCATIONMANAGER_H
