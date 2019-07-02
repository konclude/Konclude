/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CTaskProcessingStatistics.h"


namespace Konclude {

	namespace Scheduler {



		CTaskProcessingStatistics::CTaskProcessingStatistics() {
			mTaskCreatedDepthCountVec.resize(VECCOUNTERSIZE);
			mTaskProcessedDepthCountVec.resize(VECCOUNTERSIZE);
			clear();
		}

		CTaskProcessingStatistics::~CTaskProcessingStatistics() {
		}


		CTaskProcessingStatistics* CTaskProcessingStatistics::clear() {
			mStatTasksProcessed = 0;
			mStatTasksAdded = 0;
			mStatTasksCreated = 0;
			mStatTasksCompleted = 0;
			mStatTasksUpdated = 0;
			mStatEventsProcessed = 0;
			mStatTasksRequested = 0;
			mStatThreadBlocked = 0;
			mStatCallbacksExecuted = 0;
			for (cint64 i = 0; i < VECCOUNTERSIZE; ++i) {
				mTaskCreatedDepthCountVec[i] = 0;
				mTaskProcessedDepthCountVec[i] = 0;
			}
			return this;
		}


		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticTasksProcessedCount(cint64 incCount) {
			mStatTasksProcessed += incCount;
			return this;
		}

		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticTasksAddedCount(cint64 incCount) {
			mStatTasksAdded += incCount;
			return this;
		}

		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticTasksCreatedCount(cint64 incCount) {
			mStatTasksCreated += incCount;
			return this;
		}

		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticTasksCompletedCount(cint64 incCount) {
			mStatTasksCompleted += incCount;
			return this;
		}

		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticTasksUpdatedCount(cint64 incCount) {
			mStatTasksUpdated += incCount;
			return this;
		}

		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticEventsProcessedCount(cint64 incCount) {
			mStatEventsProcessed += incCount;
			return this;
		}


		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticTasksRequestedCount(cint64 incCount) {
			mStatTasksRequested += incCount;
			return this;
		}

		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticThreadsBlockedCount(cint64 incCount) {
			mStatThreadBlocked += incCount;
			return this;
		}

		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticCallbacksExecutedCount(cint64 incCount) {
			mStatCallbacksExecuted += incCount;
			return this;
		}


		cint64 CTaskProcessingStatistics::getStatisticTasksProcessedCount() {
			return mStatTasksProcessed;
		}

		cint64 CTaskProcessingStatistics::getStatisticTasksAddedCount() {
			return mStatTasksAdded;
		}

		cint64 CTaskProcessingStatistics::getStatisticTasksCreatedCount() {
			return mStatTasksCreated;
		}

		cint64 CTaskProcessingStatistics::getStatisticTasksCompletedCount() {
			return mStatTasksCompleted;
		}

		cint64 CTaskProcessingStatistics::getStatisticTasksUpdatedCount() {
			return mStatTasksUpdated;
		}

		cint64 CTaskProcessingStatistics::getStatisticEventsProcessedCount() {
			return mStatEventsProcessed;
		}

		cint64 CTaskProcessingStatistics::getStatisticTasksRequestedCount() {
			return mStatTasksRequested;
		}

		cint64 CTaskProcessingStatistics::getStatisticThreadsBlockedCount() {
			return mStatThreadBlocked;
		}


		cint64 CTaskProcessingStatistics::getStatisticCallbacksExecutedCount() {
			return mStatCallbacksExecuted;
		}


		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticTasksCreatedDepthCount(cint64 taskDepth, cint64 incCount) {
			if (taskDepth >= 0 && taskDepth < VECCOUNTERSIZE) {
				mTaskCreatedDepthCountVec[taskDepth] += incCount;
			}
			return this;
		}

		CTaskProcessingStatistics* CTaskProcessingStatistics::incStatisticTasksProcessedDepthCount(cint64 taskDepth, cint64 incCount) {
			if (taskDepth >= 0 && taskDepth < VECCOUNTERSIZE) {
				mTaskProcessedDepthCountVec[taskDepth] += incCount;
			}
			return this;
		}

		cint64 CTaskProcessingStatistics::getStatisticTasksCreatedDepthCount(cint64 taskDepth) {
			if (taskDepth >= 0 && taskDepth < VECCOUNTERSIZE) {
				return mTaskCreatedDepthCountVec[taskDepth];
			} else {
				return 0;
			}
		}

		cint64 CTaskProcessingStatistics::getStatisticTasksProcessedDepthCount(cint64 taskDepth) {
			if (taskDepth >= 0 && taskDepth < VECCOUNTERSIZE) {
				return mTaskProcessedDepthCountVec[taskDepth];
			} else {
				return 0;
			}
		}


		QVector<cint64>* CTaskProcessingStatistics::getStatisticTasksCreatedDepthCountVector() {
			return &mTaskCreatedDepthCountVec;
		}

		QVector<cint64>* CTaskProcessingStatistics::getStatisticTasksProcessedDepthCountVector() {
			return &mTaskProcessedDepthCountVec;
		}


	}; // end namespace Scheduler

}; // end namespace Konclude
