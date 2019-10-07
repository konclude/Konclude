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

#ifndef KONCLUDE_TEST_CCONCURRENTHASHMODIFICATIONTESTER_H
#define KONCLUDE_TEST_CCONCURRENTHASHMODIFICATIONTESTER_H

// Libraries includes


// Namespace includes
#include "CHashModificationThread.h"


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
		 *		\class		CConcurrentHashModificationTester
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConcurrentHashModificationTester {
			// public methods
			public:
				//! Constructor
				CConcurrentHashModificationTester();

				//! Destructor
				virtual ~CConcurrentHashModificationTester();


				void generateTestingStructure(cint64 modificationThreadsCount, cint64 modificationCountPerStep, bool useModifiedHashes, bool extendBaseHashes);
				void startHashModificationTest();
				void stopHashModificationTest();

				cint64 getModificationStepsCount();
				void destroyTestingStructure();



			// protected methods
			protected:

			// protected variables
			protected:
				CList<CHashModificationThread*> mModThreadList;
				CList<CQtHash<cint64,cint64>*> mHashContainer1;
				CList<CQtManagedRestrictedModificationHash<cint64,cint64>*> mHashContainer2;

				CQtHash<cint64,cint64>** mQtOriginalHashes;
				CQtManagedRestrictedModificationHash<cint64,cint64>** mQtModifiedHashes;
				CMemoryPoolProvider* mMemoryPoolProvider;
				bool mUseModifiedHashes;
				cint64 mHashesCount;
				cint64 mModificationThreadsCount;
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

#endif // KONCLUDE_TEST_CCONCURRENTHASHMODIFICATIONTESTER_H
