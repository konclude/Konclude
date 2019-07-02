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

#ifndef KONCLUDE_SCHEDULER_CTASKMEMORYPOOLALLOCATIONMANAGER_H
#define KONCLUDE_SCHEDULER_CTASKMEMORYPOOLALLOCATIONMANAGER_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskMemoryAllocationManager.h"


// Other includes
#include "Utilities/Memory/CMemoryPoolContainer.h"
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
		 *		\class		CTaskMemoryPoolAllocationManager
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskMemoryPoolAllocationManager : public CTaskMemoryAllocationManager {
			// public methods
			public:
				//! Constructor
				CTaskMemoryPoolAllocationManager(CMemoryPoolContainer* memoryPoolContainer, CConsiderateMemoryPoolProvider* memoryPoolProvider);
				CTaskMemoryPoolAllocationManager();

				//! Destructor
				virtual ~CTaskMemoryPoolAllocationManager();


				CTaskMemoryPoolAllocationManager* initTaskMemoryPoolAllocationManager(CMemoryPoolContainer* memoryPoolContainer, CConsiderateMemoryPoolProvider* memoryPoolProvider);


				virtual CTaskMemoryAllocationManager* updateMemoryProvider(CConsiderateMemoryPoolProvider* memoryPoolProvider);

				virtual CMemoryAllocationManager* release(void* address);

				virtual void* allocate(size_t size);

				virtual void* allocateAligned(size_t size, int alignment);

				virtual cint64 getOptimizedMoreAllocationSize(cint64 size);

				static void* allocateMemoryToContainer(size_t size, CMemoryPoolContainer* memoryPoolContainer, CMemoryPoolProvider* memoryPoolProvider);


			// protected methods
			protected:
				char* getMemoryBlock(cint64 memoryBlockSize);
				char* getMemoryBlock(cint64 memoryBlockSize, cint64 alignment);

			// protected variables
			protected:
				CConsiderateMemoryPoolProvider* mMemoryPoolProvider;
				CMemoryPoolContainer* mMemoryPoolContainer;

				cint64 mStatPoolAllocCount;
				cint64 mStatAllocPoolSize;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKMEMORYPOOLALLOCATIONMANAGER_H
