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

#include "CSatisfiableCalculationTaskStatusPropagator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				CSatisfiableCalculationTaskStatusPropagator::CSatisfiableCalculationTaskStatusPropagator() {
				}


				CSatisfiableCalculationTaskStatusPropagator::~CSatisfiableCalculationTaskStatusPropagator() {
				}


				bool CSatisfiableCalculationTaskStatusPropagator::updateTaskStatus(CTask* task, bool& moreDownPropagation, bool& moreUpPropagation) {
					CSatisfiableCalculationTask* satCalcTask = (CSatisfiableCalculationTask*)task;
					CTaskStatus* taskStatus = satCalcTask->getTaskStatus();


					if (taskStatus->isProcessable() || taskStatus->isError()) {

						bool cancelTasks = false;


						CBooleanTaskResult* taskResult = satCalcTask->getSatisfiableCalculationTaskResult();
						if (taskResult->hasResult() && !taskResult->getResultValue()) {
							cancelTasks = true;
						}

						CSatisfiableCalculationTask* parentTask = (CSatisfiableCalculationTask*)satCalcTask->getParentTask();
						if (parentTask && !cancelTasks) {
							CBooleanTaskResult* parentTaskResult = parentTask->getSatisfiableCalculationTaskResult();
							CTaskStatus* parentTaskStatus = parentTask->getTaskStatus();
							if (parentTaskStatus->isError() || parentTaskStatus->isCanceled() || (parentTaskResult->hasResult() && parentTaskResult->getResultValue())) {
								cancelTasks = true;
							}
						}


						if (cancelTasks) {
							taskStatus->setCanceled(true);
							moreDownPropagation = true;
							return true;
						} else {

							if (taskStatus->isError()) {
								if (parentTask) {
									parentTask->getTaskStatus()->setError(true,taskStatus->getErrorCode());
								}
								moreDownPropagation = true;
								return true;

							} else if (taskResult->hasResult() && taskResult->getResultValue()) {
								taskStatus->setFinished(true);

								if (parentTask) {
									CBooleanTaskResult* parentTaskResult = parentTask->getSatisfiableCalculationTaskResult();
									if (!parentTaskResult->hasResult()) {
										parentTaskResult->installResult(true);
										moreUpPropagation = true;
									}
								}

								moreDownPropagation = true;
								return true;
							}
						}
					}
					return false;
				}

				bool CSatisfiableCalculationTaskStatusPropagator::completeTaskStatus(CTask* task, bool& moreUpPropagation) {
					CSatisfiableCalculationTask* satCalcTask = (CSatisfiableCalculationTask*)task;
					CBooleanTaskResult* taskResult = satCalcTask->getSatisfiableCalculationTaskResult();
					CTaskStatus* taskStatus = satCalcTask->getTaskStatus();
					if (taskStatus->isProcessable() || taskStatus->isError()) {
						if (taskStatus->isError()) {
							CTask* parentTask = satCalcTask->getParentTask();
							if (parentTask && !parentTask->getTaskStatus()->isError()) {
								parentTask->getTaskStatus()->setError(true,taskStatus->getErrorCode());
								moreUpPropagation = true;
								return true;
							}
						} else if (taskResult->hasResult()) {
							taskStatus->setFinished(true);
							if (taskResult->getResultValue()) {
								CSatisfiableCalculationTask* parentTask = (CSatisfiableCalculationTask*)satCalcTask->getParentTask();
								if (parentTask) {
									CBooleanTaskResult* parentTaskResult = parentTask->getSatisfiableCalculationTaskResult();
									if (!parentTaskResult->hasResult()) {
										parentTaskResult->installResult(true);
										moreUpPropagation = true;
									}
								}
							}
						} else {
							if (satCalcTask == satCalcTask->getRootTask()) {
								bool bug = true;
							}
							taskResult->installResult(false);
							taskStatus->setFinished(true);
						}
						return true;
					}
					return false;
				}



			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
