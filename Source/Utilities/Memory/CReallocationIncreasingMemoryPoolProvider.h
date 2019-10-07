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

#ifndef KONCLUDE_UTILITIES_MEMORY_CREALLOCATIONINCREASINGMEMORYPOOLPROVIDER_H
#define KONCLUDE_UTILITIES_MEMORY_CREALLOCATIONINCREASINGMEMORYPOOLPROVIDER_H

// Library includes
#include <qglobal.h>

// Namespace includes
#include "MemorySettings.h"
#include "CConsiderateMemoryPoolProvider.h"
#include "CMemoryAllocationManager.h"
#include "CObjectAllocator.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CNewReallocationIncreasingMemoryPoolProvider
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReallocationIncreasingMemoryPoolProvider : public CConsiderateMemoryPoolProvider {
				// public methods
				public:
					//! Constructor
					CReallocationIncreasingMemoryPoolProvider(CMemoryAllocationManager* reallocatingMemMan, cint64 defaultPoolSize = DEFAULTMEMORYPOOLSIZE, cint64 maxPoolSize = -1, double increasingFactor = 2.);

					//! Destructor
					virtual ~CReallocationIncreasingMemoryPoolProvider();

					virtual CMemoryPool* acquireMemoryPool(cint64 minPoolSize);
					virtual CMemoryPoolProvider* releaseMemoryPool(CMemoryPool* memoryPool);

					virtual CMemoryPool* acquireMemoryPoolConsiderated(cint64 minPoolSize);

					cint64 getAllocatedReleaseDifferencePoolCount();
					cint64 getAllocatedReleaseDifferencePoolSize();


				// protected methods
				protected:

				// protected variables
				protected:
					CMemoryAllocationManager* mReallocatingMemMan;

					cint64 mDefaultPoolSize;

					cint64 mStatAllocatedPoolSize;
					cint64 mStatAllocatedPoolCount;
					cint64 mStatReleasedPoolSize;
					cint64 mStatReleasedPoolCount;

					cint64 mStatDiffPoolSize;
					cint64 mStatDiffPoolCount;

					cint64 mMaxPoolSize;
					double mIncreasingFactor;


				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CREALLOCATIONINCREASINGMEMORYPOOLPROVIDER_H
