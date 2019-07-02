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

#ifndef KONCLUDE_TEST_CHASHMODIFICATIONTHREAD_H
#define KONCLUDE_TEST_CHASHMODIFICATIONTHREAD_H

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
		 *		\class		CHashModificationThread
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CHashModificationThread : public QThread {
			// public methods
			public:
				//! Constructor
				CHashModificationThread(CQtHash<cint64,cint64>** qtOriginalHashes, cint64 hashesCount, cint64 modificationCountPerStep, bool extendBaseHashes);
				CHashModificationThread(CQtManagedRestrictedModificationHash<cint64,cint64>** qtModifiedHashes, cint64 hashesCount, CMemoryPoolProvider* memoryPoolProvider, cint64 modificationCountPerStep, bool extendBaseHashes);

				//! Destructor
				virtual ~CHashModificationThread();

				virtual void startModifications();
				virtual void stopModifications();

				cint64 getModificationStepsCount();


			// protected methods
			protected:
				virtual void run();

			// protected variables
			protected:
				cint64 mModificationSteps;
				CQtHash<cint64,cint64>** mQtOriginalHashes;
				CQtManagedRestrictedModificationHash<cint64,cint64>** mQtModifiedHashes;
				CMemoryPoolProvider* mMemoryPoolProvider;
				bool mUseModifiedHashes;
				cint64 mHashesCount;
				CMemoryPoolAllocationManager* mMemoryPoolAllocationMan;
				CContextBase* mContextBase;
				bool mCanceled;
				cint64 mHashModSize;
				cint64 mModificationCountPerStep;
				bool mExtendBaseHashes;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

using namespace Konclude::Test;

#endif // KONCLUDE_TEST_CHASHMODIFICATIONTHREAD_H
