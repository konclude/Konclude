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

#ifndef KONCLUDE_UTILITIES_MEMORY_COBJECTALLOCATOR_H
#define KONCLUDE_UTILITIES_MEMORY_COBJECTALLOCATOR_H

// Library includes


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
			 *		\class		CObjectAllocator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CObjectAllocator {
				// public methods
				public:
					//! Constructor
					CObjectAllocator() {
					}

					//! Destructor
					virtual ~CObjectAllocator() {
					}

					static inline T* allocateAndConstruct(CMemoryAllocationManager* memoryAllocatorManager) {
						T* object = 0;
						if (memoryAllocatorManager) {
							void* memAddr = memoryAllocatorManager->allocate(sizeof(T));
							object = (T*)memAddr;
							new (object) T();
						} else {
							object = new T();
						}
						return object;
					}

					static inline T* allocate(CMemoryAllocationManager* memoryAllocatorManager) {
						T* object = 0;
						if (memoryAllocatorManager) {
							void* memAddr = memoryAllocatorManager->allocate(sizeof(T));
							object = (T*)memAddr;
						} else {
							object = new T();
						}
						return object;
					}

					static inline T* allocateMemory(CMemoryAllocationManager* memoryAllocatorManager) {
						T* object = 0;
						if (memoryAllocatorManager) {
							void* memAddr = memoryAllocatorManager->allocate(sizeof(T));
							object = (T*)memAddr;
						} else {
							object = new T();
						}
						return object;
					}

					static inline T* allocateAndConstructArray(CMemoryAllocationManager* memoryAllocatorManager, cint arrayCount) {
						T* objectArray = 0;
						if (memoryAllocatorManager) {
							void* memAddr = memoryAllocatorManager->allocate(sizeof(T)*arrayCount);
							objectArray = (T*)memAddr;
							T* nextObject = objectArray;
							for (cint idx = 0; idx < arrayCount; ++idx) {
								new (nextObject) T();
								nextObject += 1;
							}
						} else {
							objectArray = new T[arrayCount];
							T* nextObject = objectArray;
							for (cint idx = 0; idx < arrayCount; ++idx) {
								new (nextObject) T();
								nextObject += 1;
							}
						}
						return objectArray;
					}

					static inline T* allocateArray(CMemoryAllocationManager* memoryAllocatorManager, cint arrayCount) {
						T* objectArray = 0;
						if (memoryAllocatorManager) {
							void* memAddr = memoryAllocatorManager->allocate(sizeof(T)*arrayCount);
							objectArray = (T*)memAddr;
						} else {
							objectArray = new T[arrayCount];
						}
						return objectArray;
					}

					static inline void release(T* object, CMemoryAllocationManager* memoryAllocatorManager) {
						if (object) {
							if (memoryAllocatorManager) {
								memoryAllocatorManager->release(object);
							} else {
								::operator delete (object);
							}
						}
					}

					static inline void destroyAndRelease(T* object, CMemoryAllocationManager* memoryAllocatorManager) {
						if (object) {
							if (memoryAllocatorManager) {
								object->T::~T();
								memoryAllocatorManager->release(object);
							} else {
								object->~T();
								::operator delete (object);
							}
						}
					}

					static inline void destroyAndReleaseArray(T* object, CMemoryAllocationManager* memoryAllocatorManager) {
						// TODO: destroy object in array somehow
						if (object) {
							if (memoryAllocatorManager) {
								memoryAllocatorManager->release(object);
							} else {
								::operator delete [] (object);
							}
						}
					}

					static inline void releaseArray(T* object, CMemoryAllocationManager* memoryAllocatorManager) {
						if (object) {
							if (memoryAllocatorManager) {
								memoryAllocatorManager->release(object);
							} else {
								::operator delete [] (object);
							}
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

			template<class T>
			inline void COADestroyAndRelease(T* object, CMemoryAllocationManager* memoryAllocatorManager) {
				CObjectAllocator<T>::destroyAndRelease(object,memoryAllocatorManager);
			}


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CObjectAllocator.cpp"

#endif // KONCLUDE_UTILITIES_MEMORY_COBJECTALLOCATOR_H
