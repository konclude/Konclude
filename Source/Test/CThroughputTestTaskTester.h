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

#ifndef KONCLUDE_TEST_CTHROUGHPUTTESTTASKTESTER_H
#define KONCLUDE_TEST_CTHROUGHPUTTESTTASKTESTER_H

// Libraries includes


// Namespace includes
#include "CThroughputTestTask.h"
#include "CThroughputTestTaskHandleAlgorithm.h"

// Other includes
#include "Scheduler/CSingleThreadTaskProcessorUnit.h"
#include "Scheduler/CTaskEventCommunicator.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Scheduler;
	using namespace Utilities::Memory;

	namespace Test {


		/*! 
		 *
		 *		\class		CThroughputTestTaskTester
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CThroughputTestTaskTester {
			// public methods
			public:
				//! Constructor
				CThroughputTestTaskTester();

				//! Destructor
				virtual ~CThroughputTestTaskTester();


				void generateTestingStructure(cint64 maxBranchDepth, cint64 branchingFactor);
				void startTesting();
				void stopTesting();

				cint64 getTaskProcessedCount();
				void destroyTestingStructure();



			// protected methods
			protected:

			// protected variables
			protected:
				CSingleThreadTaskProcessorUnit* mProcessUnit;
				CThroughputTestTaskHandleAlgorithm* handleAlg;

				cint64 mMaxBranchDepth;
				cint64 mBranchingFactor;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CTHROUGHPUTTESTTASKTESTER_H
