/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CBooleanTrueResultCancelTaskStatusPropagator.h"


namespace Konclude {

	namespace Scheduler {



		CBooleanTrueResultCancelTaskStatusPropagator::CBooleanTrueResultCancelTaskStatusPropagator() {
		}

		CBooleanTrueResultCancelTaskStatusPropagator::~CBooleanTrueResultCancelTaskStatusPropagator() {
		}


		bool CBooleanTrueResultCancelTaskStatusPropagator::updateTaskStatus(CTask* task, bool& moreDownPropagation, bool& moreUpPropagation) {
			CTaskStatus* taskStatus = task->getTaskStatus();
			CTaskResult* taskResult = task->getTaskResult();
			CBooleanTaskResult* booleanTaskResult = dynamic_cast<CBooleanTaskResult*>(taskResult);
			bool statusUpdated = false;
			bool validChildTasks = false;
			CXNegLinker<CTask*>* taskRefLinkerIt = task->getReferencedTaskLinker();
			while (taskRefLinkerIt) {
				if (taskRefLinkerIt->isNegated()) {
					validChildTasks = true;
					break;
				}
				taskRefLinkerIt = taskRefLinkerIt->getNext();
			}
			if (taskStatus->isCanceled() && validChildTasks) {
				statusUpdated = true;
				moreDownPropagation = true;
			}
			if (booleanTaskResult) {
				if (booleanTaskResult->hasResult() && validChildTasks) {
					statusUpdated = true;
					moreDownPropagation = true;
				}
				if (booleanTaskResult->hasResultValue(true)) {
					if (!taskStatus->isCanceled()) {
						taskStatus->setCanceled(true);
					}
					CTask* parentTask = task->getParentTask();
					CTaskResult* parentTaskResult = parentTask->getTaskResult();
					CTaskStatus* parentTaskStatus = parentTask->getTaskStatus();
					if (parentTask && !parentTaskStatus->isCanceled()) {
						parentTaskStatus->setCanceled(true);
						CBooleanTaskResult* parentBooleanTaskResult = dynamic_cast<CBooleanTaskResult*>(parentTaskResult);
						parentBooleanTaskResult->installResult(true);
						statusUpdated = true;
						moreUpPropagation = true;
					}
				}
			}
			return statusUpdated;
		}


		bool CBooleanTrueResultCancelTaskStatusPropagator::completeTaskStatus(CTask* task, bool& moreUpPropagation) {
			CTaskStatus* taskStatus = task->getTaskStatus();
			CTaskResult* taskResult = task->getTaskResult();
			CBooleanTaskResult* booleanTaskResult = dynamic_cast<CBooleanTaskResult*>(taskResult);
			bool statusUpdated = false;

			if (booleanTaskResult) {
				if (!booleanTaskResult->hasResult()) {
					booleanTaskResult->installResult(false);
					statusUpdated = true;
				}
				if (booleanTaskResult->hasResultValue(true)) {
					if (!taskStatus->isCanceled()) {
						taskStatus->setCanceled(true);
					}
					CTask* parentTask = task->getParentTask();
					CTaskResult* parentTaskResult = parentTask->getTaskResult();
					CTaskStatus* parentTaskStatus = parentTask->getTaskStatus();
					if (parentTask && !parentTaskStatus->isCanceled()) {
						parentTaskStatus->setCanceled(true);
						CBooleanTaskResult* parentBooleanTaskResult = dynamic_cast<CBooleanTaskResult*>(parentTaskResult);
						parentBooleanTaskResult->installResult(true);
						statusUpdated = true;
						moreUpPropagation = true;
					}
				}
			}
			return statusUpdated;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude
