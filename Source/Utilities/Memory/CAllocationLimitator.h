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

#ifndef KONCLUDE_UTILITIES_MEMORY_CALLOCATIONLIMITATOR_H
#define KONCLUDE_UTILITIES_MEMORY_CALLOCATIONLIMITATOR_H

// Library includes
#include <qglobal.h>

// Namespace includes
#include "MemorySettings.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			/*! 
			 *
			 *		\class		CAllocationLimitator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAllocationLimitator {
				// public methods
				public:
					//! Constructor
					CAllocationLimitator();

					//! Destructor
					virtual ~CAllocationLimitator();


					virtual cint64 getMemoryAllocatedSize() = 0;
					virtual cint64 getMaximumMemoryAllocationSize() = 0;
					virtual cint64 getRemainingMemoryAllocationSize() = 0;

					virtual bool addAllocatingMemorySize(cint64 allocatedMemorySize) = 0;
					virtual CAllocationLimitator* addReleasedMemorySize(cint64 allocatedMemorySize) = 0;


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

#endif // KONCLUDE_UTILITIES_MEMORY_CALLOCATIONLIMITATOR_H
