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

#ifndef KONCLUDE_UTILITIES_MEMORY_COBJECTMEMORYPOOLALLOCATOR_H
#define KONCLUDE_UTILITIES_MEMORY_COBJECTMEMORYPOOLALLOCATOR_H

// Library includes


// Namespace includes
#include "MemorySettings.h"
#include "CMemoryPoolProvider.h"
#include "CMemoryPool.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace std;

namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CObjectMemoryPoolAllocator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CObjectMemoryPoolAllocator {
				// public methods
				public:
					//! Constructor
					CObjectMemoryPoolAllocator() {
					}

					//! Destructor
					virtual ~CObjectMemoryPoolAllocator() {
					}

					static inline T* allocateAndConstructWithMemroyPool(CMemoryPoolProvider* memoryPoolProvider = nullptr) {
						T* object = 0;
						CMemoryPool* memoryPool = nullptr;
						if (memoryPoolProvider) {
							memoryPool = memoryPoolProvider->acquireMemoryPool(sizeof(T));
						} else {
							memoryPool = new CMemoryPool();
							cint64 memorySize = qMax((cint64)sizeof(T),DEFAULTMEMORYPOOLSIZE);
							char* memoryPoolMemory = new char[memorySize];
							memoryPool->setMemoryBlockData(memoryPoolMemory,memorySize);
						}
						if (memoryPool) {
							cint64 memorySize = (cint64)sizeof(T);
							object = (T*)(memoryPool->getMemoryBlockPointer());
							memoryPool->incMemoryBlockPointer(sizeof(T));
							new (object) T(memoryPool);
						}
						return object;
					}


					static inline T* allocateAndConstructWithMemroyPool(CMemoryPool* memoryPool) {
						T* object = 0;
						if (memoryPool) {
							cint64 memorySize = (cint64)sizeof(T);
							object = (T*)(memoryPool->getMemoryBlockPointer());
							memoryPool->incMemoryBlockPointer(sizeof(T));
							new (object) T(memoryPool);
						}
						return object;
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

#include "CObjectMemoryPoolAllocator.cpp"

#endif // KONCLUDE_UTILITIES_MEMORY_COBJECTMEMORYPOOLALLOCATOR_H
