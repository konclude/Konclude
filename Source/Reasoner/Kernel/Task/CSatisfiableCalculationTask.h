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

#ifndef KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLECALCULATIONTASK_H
#define KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLECALCULATIONTASK_H

// Libraries includes


// Namespace includes
#include "CCalculationConfigurationExtension.h"
#include "CCalculationStatisticsCollector.h"
#include "CSatisfiableSubsumptionIdentifierAdapter.h"
#include "CTaskPreyingAdapter.h"
#include "CSatisfiableCalculationJobInstantiation.h"
#include "CSatisfiableTaskClassificationMessageAdapter.h"


// Other includes
#include "Scheduler/CTask.h"

#include "Reasoner/Kernel/Process/CProcessContextBase.h"
#include "Reasoner/Kernel/Process/CProcessingDataBox.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Scheduler;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;

			namespace Task {


				/*! 
				 *
				 *		\class		CSatisfiableCalculationTask
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableCalculationTask : public CTask, public CSatisfiableCalculationJobInstantiation {
					// public methods
					public:
						//! Constructor
						CSatisfiableCalculationTask(CMemoryPool* memoryPool);

						//! Destructor
						virtual ~CSatisfiableCalculationTask();

						virtual CTaskContext* getTaskContext(CTaskHandleContext* context);
						CProcessContext* getProcessContext(CTaskHandleContext* context);

						CProcessingDataBox* getProcessingDataBox();

						virtual CTask* makeTaskReference(CTask* dependedTask, CTaskHandleContext* handlerContext);
						virtual CTask* initTask(CTask* parentTask, CTaskHandleContext* context);

						virtual CSatisfiableCalculationTask* initSatisfiableCalculationTask(CConcreteOntology* ontology, CCalculationConfigurationExtension* calculationConfig, CCalculationStatisticsCollector* calcStatCollector, CTaskHandleContext* context);
						virtual CSatisfiableCalculationTask* initBranchDependedSatisfiableCalculationTask(CSatisfiableCalculationTask* dependedTask, CTaskHandleContext* context);
						virtual CSatisfiableCalculationTask* initUndependedSatisfiableCalculationTask(CSatisfiableCalculationTask* baseTask, CCalculationConfigurationExtension* calculationConfig, CCalculationStatisticsCollector* calcStatCollector, CTaskHandleContext* context);

						CBooleanTaskResult* getSatisfiableCalculationTaskResult();

						CCalculationConfigurationExtension* getCalculationConfiguration();

						virtual CTask* completeTask();

						CSatisfiableCalculationTask* setConsistenceAdapter(CTaskPreyingAdapter* consistenceAdapter);
						CTaskPreyingAdapter* getConsistenceAdapter();


						CSatisfiableCalculationTask* setSatisfiableSubsumptionIdentifierAdapter(CSatisfiableSubsumptionIdentifierAdapter* satSubIdObserver);
						CSatisfiableSubsumptionIdentifierAdapter* getSatisfiableSubsumptionIdentifierAdapter();

						CSatisfiableCalculationTask* setClassificationMessageAdapter(CSatisfiableTaskClassificationMessageAdapter* classMessAdapter);
						CSatisfiableTaskClassificationMessageAdapter* getClassificationMessageAdapter();


						bool isCalculationTableauCompletionTask();
						bool isCalculationTableauSaturationTask();

						CSatisfiableCalculationTask* setCalculationTaskType(cint64 taskType);
						cint64 getCalculationTaskType();


						static const cint64 CALCULATIONTABLEAUCOMPLETIONTASK = 0;
						static const cint64 CALCULATIONTABLEAUDEFAULTSATURATIONTASK = 1;
						static const cint64 CALCULATIONTABLEAUPILINGSATURATIONTASK = 2;
						static const cint64 CALCULATIONTABLEAUAPPROXIMATEDSATURATIONTASK = 3;

					// protected methods
					protected:
						virtual CTaskResult* createTaskResult(CTaskHandleContext* handlerContext);
						virtual CTaskStatus* createTaskStatus(CTaskHandleContext* handlerContext);

					// protected variables
					protected:
						CBooleanTaskResult mBoolTaskResult;
						CTaskStatus mDefaultTaskResult;

						CProcessContextBase* mProcessContext;
						CProcessingDataBox* mProcessingDataBox;

						CCalculationConfigurationExtension* mCalculationConfig;
						CCalculationStatisticsCollector* mCalcStatColl;
						CTaskPreyingAdapter* mConsAdapter;
						CSatisfiableSubsumptionIdentifierAdapter* mSatSubIdAdapter;
						CSatisfiableTaskClassificationMessageAdapter* mClassMessAdapter;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_TASK_CSATISFIABLECALCULATIONTASK_H
