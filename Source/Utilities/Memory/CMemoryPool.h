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

#ifndef KONCLUDE_UTILITIES_MEMORY_CMEMORYPOOL_H
#define KONCLUDE_UTILITIES_MEMORY_CMEMORYPOOL_H

// Library includes


// Namespace includes
#include "MemorySettings.h"

// Other includes
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		using namespace Container;

		namespace Memory {


			/*! 
			 *
			 *		\class		CMemoryPool
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CMemoryPool : public CLinkerBase<CMemoryPool*,CMemoryPool> {
				// public methods
				public:
					//! Constructor
					CMemoryPool();

					//! Destructor
					virtual ~CMemoryPool();

					char* getMemoryBlockData();
					cint64 getMemoryBlockSize();
					char* getMemoryBlockPointer();
					char* getMemoryBlockEnd();

					CMemoryPool* setMemoryBlockData(char* memoryBlock, cint64 memoryBlockSize, bool memoryBlockMapped = false);
					//! whether the block consists of pages mapped from the system rather than an array from the allocator
					bool isMemoryBlockMapped();
					/*!
					 *	Obtains a block for a pool: pages mapped from the system where the platform allows
					 *	it, since pool blocks released to the allocator stay in its retained regions and the
					 *	process grows with every reasoner it destroys (issue #45); an array otherwise.
					 */
					static char* allocateMemoryBlock(cint64 memoryBlockSize, bool& memoryBlockMapped);
					/*!
					 *	The size a mapped block of the given size really occupies: whole pages (16 KiB on
					 *	Apple silicon, 4 KiB on Linux). A pool sized this way uses the pages it is charged for;
					 *	the default pool size of 50 000 bytes alone leaves the fourth page of each block
					 *	three quarters empty on macOS. The size itself where blocks are not mapped.
					 */
					static cint64 mappedBlockSize(cint64 memoryBlockSize);
					//! releases a block the way it was obtained
					static void releaseMemoryBlock(char* memoryBlock, cint64 memoryBlockSize, bool memoryBlockMapped);
					//! releases the block of the pool, not the pool
					static void releaseMemoryBlockData(CMemoryPool* memoryPool);
					CMemoryPool* setMemoryBlockPointer(char* memoryBlockPointer);
					CMemoryPool* incMemoryBlockPointer(cint64 pointerInc);

					CMemoryPool* resetMemoryBlockPointer();

					CMemoryPool* getNextMemoryPool();
					CMemoryPool* setNextMemoryPool(CMemoryPool* nextMemoryPool);

				// protected methods
				protected:

				// protected variables
				protected:
					char* mMemoryBlockBegin;
					char* mMemoryBlockEnd;
					cint64 mMemoryBlockSize;
					char* mMemoryBlockPointer;
					bool mMemoryBlockMapped;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CMEMORYPOOL_H
