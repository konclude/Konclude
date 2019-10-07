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

#ifndef KONCLUDE_SCHEDULER_CTASKPROCESSINGSTATISTICS_H
#define KONCLUDE_SCHEDULER_CTASKPROCESSINGSTATISTICS_H

// Libraries includes
#include <QVector>


// Namespace includes
#include "SchedulerSettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Scheduler {


#ifdef KONCLUDE_SCHEDULER_TASK_PROCESSING_STATISTICS
	#define INCTASKPROCESSINGSTAT(a) a
#else
	#define INCTASKPROCESSINGSTAT(a)
#endif

		/*! 
		 *
		 *		\class		CTaskProcessingStatistics
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskProcessingStatistics {
			// public methods
			public:
				//! Constructor
				CTaskProcessingStatistics();

				//! Destructor
				virtual ~CTaskProcessingStatistics();

				CTaskProcessingStatistics* clear();


				CTaskProcessingStatistics* incStatisticTasksProcessedCount(cint64 incCount = 1);
				CTaskProcessingStatistics* incStatisticTasksAddedCount(cint64 incCount = 1);
				CTaskProcessingStatistics* incStatisticTasksCreatedCount(cint64 incCount = 1);
				CTaskProcessingStatistics* incStatisticTasksCompletedCount(cint64 incCount = 1);
				CTaskProcessingStatistics* incStatisticTasksUpdatedCount(cint64 incCount = 1);
				CTaskProcessingStatistics* incStatisticEventsProcessedCount(cint64 incCount = 1);
				CTaskProcessingStatistics* incStatisticTasksRequestedCount(cint64 incCount = 1);
				CTaskProcessingStatistics* incStatisticThreadsBlockedCount(cint64 incCount = 1);
				CTaskProcessingStatistics* incStatisticCallbacksExecutedCount(cint64 incCount = 1);

				cint64 getStatisticTasksProcessedCount();
				cint64 getStatisticTasksAddedCount();
				cint64 getStatisticTasksCreatedCount();
				cint64 getStatisticTasksCompletedCount();
				cint64 getStatisticTasksUpdatedCount();
				cint64 getStatisticEventsProcessedCount();
				cint64 getStatisticTasksRequestedCount();
				cint64 getStatisticThreadsBlockedCount();
				cint64 getStatisticCallbacksExecutedCount();


				CTaskProcessingStatistics* incStatisticTasksCreatedDepthCount(cint64 taskDepth, cint64 incCount = 1);
				CTaskProcessingStatistics* incStatisticTasksProcessedDepthCount(cint64 taskDepth, cint64 incCount = 1);
				
				cint64 getStatisticTasksCreatedDepthCount(cint64 taskDepth);
				cint64 getStatisticTasksProcessedDepthCount(cint64 taskDepth);

				QVector<cint64>* getStatisticTasksCreatedDepthCountVector();
				QVector<cint64>* getStatisticTasksProcessedDepthCountVector();


				const static cint64 VECCOUNTERSIZE = 10000;

			// protected methods
			protected:

			// protected variables
			protected:
				cint64 mStatTasksProcessed;
				cint64 mStatTasksAdded;
				cint64 mStatTasksCreated;
				cint64 mStatTasksCompleted;
				cint64 mStatTasksUpdated;
				cint64 mStatEventsProcessed;
				cint64 mStatTasksRequested;
				cint64 mStatThreadBlocked;
				cint64 mStatCallbacksExecuted;


				QVector<cint64> mTaskCreatedDepthCountVec;
				QVector<cint64> mTaskProcessedDepthCountVec;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKPROCESSINGSTATISTICS_H
