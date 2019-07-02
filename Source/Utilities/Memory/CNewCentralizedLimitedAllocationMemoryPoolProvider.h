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

#ifndef KONCLUDE_UTILITIES_MEMORY_CNEWCENTRALIZEDLIMITEDALLOCATIONMEMORYPOOLPROVIDER_H
#define KONCLUDE_UTILITIES_MEMORY_CNEWCENTRALIZEDLIMITEDALLOCATIONMEMORYPOOLPROVIDER_H

// Library includes
#include <qglobal.h>

// Namespace includes
#include "MemorySettings.h"
#include "CMemoryPoolProvider.h"
#include "CAllocationLimitator.h"
#include "CMemoryAllocationException.h"
#include "CConsiderateMemoryPoolProvider.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CNewCentralizedLimitedAllocationMemoryPoolProvider
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNewCentralizedLimitedAllocationMemoryPoolProvider : public CConsiderateMemoryPoolProvider {
				// public methods
				public:
					//! Constructor
					CNewCentralizedLimitedAllocationMemoryPoolProvider(CAllocationLimitator* allocLimitator, cint64 defaultPoolSize = DEFAULTMEMORYPOOLSIZE);

					//! Destructor
					virtual ~CNewCentralizedLimitedAllocationMemoryPoolProvider();

					virtual CMemoryPool* acquireMemoryPool(cint64 minPoolSize);
					virtual CMemoryPoolProvider* releaseMemoryPool(CMemoryPool* memoryPool);

					virtual CMemoryPool* acquireMemoryPoolConsiderated(cint64 minPoolSize);


					cint64 getAllocatedReleaseDifferencePoolCount();
					cint64 getAllocatedReleaseDifferencePoolSize();


				// protected methods
				protected:

				// protected variables
				protected:
					CAllocationLimitator* mAllocLimitator;

					cint64 mDefaultPoolSize;

					cint64 mStatAllocatedPoolSize;
					cint64 mStatAllocatedPoolCount;
					cint64 mStatReleasedPoolSize;
					cint64 mStatReleasedPoolCount;

					cint64 mStatDiffPoolSize;
					cint64 mStatDiffPoolCount;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CNEWCENTRALIZEDLIMITEDALLOCATIONMEMORYPOOLPROVIDER_H
