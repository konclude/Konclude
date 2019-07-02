/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_UTILITIES_MEMORY_CMEMORYPOOLALLOCATIONMANAGER_H
#define KONCLUDE_UTILITIES_MEMORY_CMEMORYPOOLALLOCATIONMANAGER_H

// Library includes


// Namespace includes
#include "MemorySettings.h"
#include "CMemoryPoolProvider.h"
#include "CMemoryPoolContainer.h"
#include "CMemoryTemporaryAllocationManager.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CMemoryPoolAllocationManager
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CMemoryPoolAllocationManager : public CMemoryTemporaryAllocationManager {
				// public methods
				public:
					//! Constructor
					CMemoryPoolAllocationManager(CMemoryPoolProvider* memoryPoolProvider);

					//! Destructor
					virtual ~CMemoryPoolAllocationManager();

					virtual void* allocate(size_t size);
					virtual CMemoryAllocationManager* release(void* address);

					virtual void* allocateAligned(size_t size, int alignment);

					virtual CMemoryTemporaryAllocationManager* releaseAllMemory();

					virtual CMemoryPool* acqireTemporaryMemoryPool(size_t size);
					virtual CMemoryTemporaryAllocationManager* releaseTemporaryMemoryPool(CMemoryPool* memoryPool);

					virtual cint64 getOptimizedMoreAllocationSize(cint64 size);

					virtual CMemoryTemporaryAllocationManager* releaseMemoryPoolContainer(CMemoryPoolContainer* memoryPoolContainer);

					virtual cint64 getFreeMemoryPoolCount();

				// protected methods
				protected:
					char* getMemoryBlock(cint64 memoryBlockSize);
					char* getMemoryBlock(cint64 memoryBlockSize, cint64 alignment);

				// protected variables
				protected:
					CMemoryPoolProvider* mMemoryPoolProvider;

					CMemoryPool* mFreeMemoryPoolList;
					CMemoryPool* mUsedMemoryPoolList;
					CMemoryPool* mUsingMemoryPool;

					cint64 mFreeMemoryPoolCount;
					cint64 mStatMemToUsingCount;
					cint64 mStatMemToFreeCount;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CMEMORYPOOLALLOCATIONMANAGER_H
