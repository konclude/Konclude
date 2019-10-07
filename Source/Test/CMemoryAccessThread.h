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

#ifndef KONCLUDE_TEST_CMEMORYACCESSTHREAD_H
#define KONCLUDE_TEST_CMEMORYACCESSTHREAD_H

// Libraries includes
#include <QThread>


// Namespace includes


// Other includes
#include "Utilities/Container/CQtHash.h"
#include "Utilities/Container/CQtManagedRestrictedModificationHash.h"

#include "Utilities/Memory/CObjectAllocator.h"
#include "Utilities/Memory/CMemoryPoolAllocationManager.h"

#include "Utilities/UtilitiesSettings.h"

#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;
	using namespace Utilities;
	using namespace Utilities::Container;
	using namespace Utilities::Memory;

	namespace Test {


		/*! 
		 *
		 *		\class		CMemoryAccessThread
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CMemoryAccessThread : public QThread {
			// public methods
			public:
				//! Constructor
				CMemoryAccessThread(cint64 readWriteRatio, int* accessArray, cint64 arraySize, bool atomicOperations, cint64 minWriteIndex, cint64 maxWriteIndex, CMemoryPoolProvider* memoryPoolProvider);

				//! Destructor
				virtual ~CMemoryAccessThread();

				virtual void startModifications();
				virtual void stopModifications();

				cint64 getModificationStepsCount();


			// protected methods
			protected:
				virtual void run();

			// protected variables
			protected:
				int* mMemoryAccessArray;
				cint64 mArraySize;

				CMemoryPoolProvider* mMemoryPoolProvider;
				CMemoryPoolAllocationManager* mMemoryPoolAllocationMan;
				CContextBase* mContextBase;
				
				bool mCanceled;
				cint64 mReadWriteRatio;
				cint64 mModificationSteps;

				bool mAtomicIncrements;

				bool mLocalWrite;
				cint64 mLocalWriteStep;
				cint64 mLocalWriteIndex;

				cint64 mMinWriteIndex;
				cint64 mMaxWriteIndex;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

using namespace Konclude::Test;

#endif // KONCLUDE_TEST_CMEMORYACCESSTHREAD_H
