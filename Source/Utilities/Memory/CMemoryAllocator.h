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

#ifndef KONCLUDE_UTILITIES_MEMORY_CMEMORYALLOCATOR_H
#define KONCLUDE_UTILITIES_MEMORY_CMEMORYALLOCATOR_H

// Library includes
#include <QtGlobal>

// Namespace includes
#include "MemorySettings.h"
#include "CMemoryAllocationManager.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace std;

namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CMemoryAllocator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CMemoryAllocator {
				// public methods
				public:
					//! Constructor
					CMemoryAllocator() {
					}

					//! Destructor
					virtual ~CMemoryAllocator() {
					}

					static inline void* allocateMemory(CMemoryAllocationManager* memoryAllocatorManager, size_t memorySize) {
						void* memAddr = 0;
						if (memoryAllocatorManager) {
							memAddr = memoryAllocatorManager->allocate(memorySize);
						} else {
                            memAddr = qMallocAligned(memorySize,0);
						}
						return memAddr;
					}

					static inline void* reallocateMemory(void* memAddr, CMemoryAllocationManager* memoryAllocatorManager, size_t prevMemorySize, size_t newMemorySize) {
						void* newMemAddr = 0;
						if (memoryAllocatorManager) {
							newMemAddr = memoryAllocatorManager->allocate(newMemorySize);
							::memcpy(newMemAddr,memAddr,prevMemorySize);
							memoryAllocatorManager->release(memAddr);
						} else {
                            newMemAddr = qReallocAligned(memAddr,newMemorySize,prevMemorySize,0);
						}
						return newMemAddr;
					}


					static inline void* allocateAlignedMemory(CMemoryAllocationManager* memoryAllocatorManager, size_t memorySize, cint alignment) {
						void* memAddr = 0;
						if (memoryAllocatorManager) {
							memAddr = memoryAllocatorManager->allocateAligned(memorySize,alignment);
						} else {
							memAddr = qMallocAligned(memorySize,alignment);
						}
						return memAddr;
					}

					static inline void releaseMemory(void* memAddr, CMemoryAllocationManager* memoryAllocatorManager) {
						if (memoryAllocatorManager) {
							memoryAllocatorManager->release(memAddr);
						} else {
                            qFreeAligned(memAddr);
						}
					}

					static inline void releaseAlignedMemory(void* memAddr, CMemoryAllocationManager* memoryAllocatorManager) {
						if (memoryAllocatorManager) {
							memoryAllocatorManager->release(memAddr);
						} else {
							qFreeAligned(memAddr);
						}
					}
				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude


#endif // KONCLUDE_UTILITIES_MEMORY_CMEMORYALLOCATOR_H
