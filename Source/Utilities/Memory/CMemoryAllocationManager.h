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

#ifndef KONCLUDE_UTILITIES_MEMORY_CMEMORYALLOCATIONMANAGER_H
#define KONCLUDE_UTILITIES_MEMORY_CMEMORYALLOCATIONMANAGER_H

// Library includes


// Namespace includes
#include "MemorySettings.h"

// Other includes
#include <iostream>

// Logger includes
#include "Logger/CLogger.h"

using namespace std;

namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CMemoryAllocationManager
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CMemoryAllocationManager {
				// public methods
				public:
					//! Constructor
					CMemoryAllocationManager();

					//! Destructor
					virtual ~CMemoryAllocationManager();

					virtual void* allocate(size_t size) = 0;
					virtual CMemoryAllocationManager* release(void* address) = 0;

					virtual void* allocateAligned(size_t size, int alignment) = 0;

					virtual cint64 getOptimizedMoreAllocationSize(cint64 size);


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

#endif // KONCLUDE_UTILITIES_MEMORY_CMEMORYALLOCATIONMANAGER_H
