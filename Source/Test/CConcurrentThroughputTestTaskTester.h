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

#ifndef KONCLUDE_TEST_CCONCURRENTTHROUGHPUTTESTTASKTESTER_H
#define KONCLUDE_TEST_CCONCURRENTTHROUGHPUTTESTTASKTESTER_H

// Libraries includes
#include <QTime>
#include <QList>
#include <qmath.h>

// Namespace includes
#include "CThroughputTestTask.h"
#include "CThroughputTestTaskHandleAlgorithm.h"

// Other includes
#include "Concurrent/Callback/CCallbackData.h"

#include "Scheduler/CSingleThreadTaskProcessorUnit.h"
#include "Scheduler/CTaskEventCommunicator.h"

#include "Scheduler/CTaskProcessorSchedulerThread.h"
#include "Scheduler/CTaskProcessorCompletorThread.h"
#include "Scheduler/CTaskProcessorThread.h"


#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Scheduler;
	using namespace Utilities::Memory;
	using namespace Concurrent::Callback;

	namespace Test {


		/*! 
		 *
		 *		\class		CConcurrentThroughputTestTaskTester
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConcurrentThroughputTestTaskTester : public CCallbackData {
			// public methods
			public:
				//! Constructor
				CConcurrentThroughputTestTaskTester();

				//! Destructor
				virtual ~CConcurrentThroughputTestTaskTester();


				void generateTestingStructure(cint64 maxBranchDepth, cint64 branchingFactor);
				void startTesting();
				void stopTesting();

				cint64 getTaskProcessedCount();
				void destroyTestingStructure();

				virtual void doCallback();




			// protected methods
			protected:

			// protected variables
			protected:
				CSingleThreadTaskProcessorUnit* mProcessUnit;
				CTaskProcessorSchedulerThread* mSchedulerUnit;
				CTaskProcessorCompletorThread* mCompletorUnit;
				CThroughputTestTaskHandleAlgorithm* mHandleAlg;
				QList<CTaskProcessorThread*> mProcessorUnitList;

				cint64 mMaxBranchDepth;
				cint64 mBranchingFactor;
				cint64 mTotalTestTasks;

				cint64 mThreadTestCount;
				cint64 mThreadMaxTestCount;
				cint64 mScaleReferenceTime;

				QTime mMeasurementTimer;
				cint64 mLastEventProcessed;
				cint64 mLastTaskProcessed;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CCONCURRENTTHROUGHPUTTESTTASKTESTER_H
