/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_UTILITIES_MEMORY_CTEMPMEMORYPOOLCONTAINERALLOCATIONMANAGER_H
#define KONCLUDE_UTILITIES_MEMORY_CTEMPMEMORYPOOLCONTAINERALLOCATIONMANAGER_H

// Libraries includes


// Namespace includes
#include "MemorySettings.h"
#include "CMemoryAllocationManager.h"
#include "CMemoryPoolContainer.h"
#include "CMemoryPoolProvider.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Utilities {

		namespace Memory {

			/*! 
			 *
			 *		\class		CTempMemoryPoolContainerAllocationManager
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTempMemoryPoolContainerAllocationManager : public CMemoryAllocationManager {
				// public methods
				public:
					//! Constructor
					CTempMemoryPoolContainerAllocationManager(CMemoryPoolContainer* memoryPoolContainer, CMemoryPoolProvider* memoryPoolProvider);
					CTempMemoryPoolContainerAllocationManager(CMemoryPoolProvider* memoryPoolProvider);
					CTempMemoryPoolContainerAllocationManager();

					//! Destructor
					virtual ~CTempMemoryPoolContainerAllocationManager();


					CTempMemoryPoolContainerAllocationManager* initTempMemoryPoolContainerAllocationManager(CMemoryPoolContainer* memoryPoolContainer, CMemoryPoolProvider* memoryPoolProvider);

					virtual CMemoryAllocationManager* release(void* address);

					virtual void* allocate(size_t size);

					virtual void* allocateAligned(size_t size, int alignment);

					virtual cint64 getOptimizedMoreAllocationSize(cint64 size);

					static void* allocateMemoryToContainer(size_t size, CMemoryPoolContainer* memoryPoolContainer, CMemoryPoolProvider* memoryPoolProvider);

					CMemoryPool* getMemoryPools();
					CMemoryPool* takeMemoryPools();

				// protected methods
				protected:
					char* getMemoryBlock(cint64 memoryBlockSize);
					char* getMemoryBlock(cint64 memoryBlockSize, cint64 alignment);

				// protected variables
				protected:
					CMemoryPoolProvider* mMemoryPoolProvider;
					CMemoryPoolContainer* mMemoryPoolContainer;

					CMemoryPoolContainer mTmpMemoryPoolContainer;


					cint64 mStatPoolAllocCount;
					cint64 mStatAllocPoolSize;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CTEMPMEMORYPOOLCONTAINERALLOCATIONMANAGER_H
