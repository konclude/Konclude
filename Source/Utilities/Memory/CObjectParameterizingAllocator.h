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

#ifndef KONCLUDE_UTILITIES_MEMORY_COBJECTPARAMETERIZINGALLOCATOR_H
#define KONCLUDE_UTILITIES_MEMORY_COBJECTPARAMETERIZINGALLOCATOR_H

// Library includes


// Namespace includes
#include "MemorySettings.h"
#include "CMemoryAllocationManager.h"
#include "CObjectAllocator.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace std;

namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CObjectParameterizingAllocator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class P>
			class CObjectParameterizingAllocator : public CObjectAllocator<T> {
				// public methods
				public:
					//! Constructor
					CObjectParameterizingAllocator() {
					}

					//! Destructor
					virtual ~CObjectParameterizingAllocator() {
					}

					static inline T* allocateAndConstructAndParameterize(CMemoryAllocationManager* memoryAllocatorManager, P parameter) {
						T* object = 0;
						if (memoryAllocatorManager) {
							void* memAddr = memoryAllocatorManager->allocate(sizeof(T));
							object = (T*)memAddr;
							new (object) T(parameter);
						} else {
							object = new T(parameter);
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



			template<class T>
			inline void COPADestroyAndRelease(T* object, CMemoryAllocationManager* memoryAllocatorManager) {
				COADestroyAndRelease(object,memoryAllocatorManager);
			}




		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#include "CObjectParameterizingAllocator.cpp"

#endif // KONCLUDE_UTILITIES_MEMORY_COBJECTPARAMETERIZINGALLOCATOR_H
