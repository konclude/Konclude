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

#ifndef KONCLUDE_UTILITIES_MEMORY_CLIMITEDRESERVEMEMORYPOOLALLOCATIONMANAGER_H
#define KONCLUDE_UTILITIES_MEMORY_CLIMITEDRESERVEMEMORYPOOLALLOCATIONMANAGER_H

// Library includes


// Namespace includes
#include "MemorySettings.h"
#include "CMemoryPoolAllocationManager.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CLimitedReserveMemoryPoolAllocationManager
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CLimitedReserveMemoryPoolAllocationManager : public CMemoryPoolAllocationManager {
				// public methods
				public:
					//! Constructor
					CLimitedReserveMemoryPoolAllocationManager(CMemoryPoolProvider* memoryPoolProvider, cint64 freeReserveLimit = 20, cint64 freeReleaseBlockSize = 5);

					//! Destructor
					virtual ~CLimitedReserveMemoryPoolAllocationManager();

					virtual CMemoryTemporaryAllocationManager* releaseTemporaryMemoryPool(CMemoryPool* memoryPool);


				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mFreeReserveLimit;
					cint64 mFreeReleaseBlockSize;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CLIMITEDRESERVEMEMORYPOOLALLOCATIONMANAGER_H
