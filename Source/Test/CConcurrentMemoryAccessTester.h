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

#ifndef KONCLUDE_TEST_CConcurrentMemoryAccessTester_H
#define KONCLUDE_TEST_CConcurrentMemoryAccessTester_H

// Libraries includes


// Namespace includes
#include "CMemoryAccessThread.h"


// Other includes
#include "Utilities/Container/CList.h"

#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;
	using namespace Utilities::Memory;

	namespace Test {


		/*! 
		 *
		 *		\class		CConcurrentMemoryAccessTester
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConcurrentMemoryAccessTester {
			// public methods
			public:
				//! Constructor
				CConcurrentMemoryAccessTester();

				//! Destructor
				virtual ~CConcurrentMemoryAccessTester();


				void generateTestingStructure(cint64 arraySize, cint64 readWriteRatio, bool atomicOperations, cint64 threadCount);
				void startHashModificationTest();
				void stopHashModificationTest();

				cint64 getModificationStepsCount();
				void destroyTestingStructure();



			// protected methods
			protected:

			// protected variables
			protected:
				CList<CMemoryAccessThread*> mModThreadList;
				int* mMemoryArray;
				cint64 mArraySize;
				cint64 mReadWriteRatio;
				bool mAtomicOperations;
				cint64 mThreadCount;
				bool mSharedWrite;

				CMemoryPoolProvider* mMemoryPoolProvider;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

using namespace Konclude::Test;

#endif // KONCLUDE_TEST_CConcurrentMemoryAccessTester_H
