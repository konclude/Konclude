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

#ifndef KONCLUDE_REASONER_KERNEL_CALCULATION_CCONCURRENTTASKCALCULATIONENVIRONMENT_H
#define KONCLUDE_REASONER_KERNEL_CALCULATION_CCONCURRENTTASKCALCULATIONENVIRONMENT_H

// Libraries includes


// Namespace includes
#include "CCalculationEnviroment.h"

// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTaskJobCallbackExecuter.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTaskStatusPropagator.h"

#include "Scheduler/CSingleThreadTaskProcessorUnit.h"
#include "Scheduler/CTaskProcessorSchedulerThread.h"
#include "Scheduler/CTaskProcessorCompletorThread.h"
#include "Scheduler/CTaskProcessorThread.h"

#include "Utilities/Memory/CCentralizedAllocationLimitation.h"



// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Scheduler;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			using namespace Task;

			namespace Calculation {


				/*! 
				 *
				 *		\class		CConcurrentTaskCalculationEnvironment
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CConcurrentTaskCalculationEnvironment : public CCalculationEnviroment {
					// public methods
					public:
						//! Constructor
						CConcurrentTaskCalculationEnvironment();

						//! Destructor
						virtual ~CConcurrentTaskCalculationEnvironment();


						CConcurrentTaskCalculationEnvironment* initSingleTaskProcessor(CSingleThreadTaskProcessorUnit* processUnit);
						CConcurrentTaskCalculationEnvironment* initMultiTaskProcessor(CTaskProcessorSchedulerThread* schedulerUnit, CTaskProcessorCompletorThread* completorUnit);
						CConcurrentTaskCalculationEnvironment* appendMultiTaskProcessor(CTaskProcessorThread* processorUnit);
						CConcurrentTaskCalculationEnvironment* initCallbackExecuter(CSatisfiableCalculationTaskJobCallbackExecuter* callbackExecuter);
						CConcurrentTaskCalculationEnvironment* initStatusPropagator(CSatisfiableCalculationTaskStatusPropagator* statusPropagator);

						CSingleThreadTaskProcessorUnit* getSingleTaskProcessorUnit();
						CTaskEventHandlerBasedScheduler* getSchedulerTaskProcessorUnit();
						CTaskEventHandlerBasedCompletor* getCompletorTaskProcessorUnit();
						QList<CTaskEventHandlerBasedProcessor*> getTaskProcessorUnitList();

						CSatisfiableCalculationTaskJobCallbackExecuter* getSatisfiableCalculationTaskJobCallbackExecuter();
						CSatisfiableCalculationTaskStatusPropagator* getSatisfiableCalculationTaskStatusPropagator();

						CCentralizedAllocationLimitation* getAllocationLimitation();
						CConcurrentTaskCalculationEnvironment* setAllocationLimitation(CCentralizedAllocationLimitation* allocationLimitation);


						cint64 getCalculationComputionTime();
						cint64 getCalculationBlockingTime();
						cint64 getCalculationMemoryConsumption();
						cint64 getCalculationMemoryReserved();

						cint64 getCalculationStatisticTasksProcessedCount();
						cint64 getCalculationStatisticTasksAddedCount();
						cint64 getCalculationStatisticTasksCreatedCount();
						cint64 getCalculationStatisticTasksCompletedCount();
						cint64 getCalculationStatisticTasksUpdatedCount();
						cint64 getCalculationStatisticEventsProcessedCount();
						cint64 getCalculationStatisticTasksRequestedCount();
						cint64 getCalculationStatisticThreadsBlockedCount();


						double getCalculationApproximatedRemainingTasksCount();



					// protected methods
					protected:

					// protected variables
					protected:
						CSingleThreadTaskProcessorUnit* mProcessUnit;
						CTaskEventHandlerBasedScheduler* mSchedulerUnit;
						CTaskEventHandlerBasedCompletor* mCompletorUnit;
						QList<CTaskProcessorThreadBase*> mThreadUnitList;
						QList<CTaskEventHandlerBasedProcessor*> mProcessorUnitList;
						CSatisfiableCalculationTaskJobCallbackExecuter* mCallbackExecuter;
						CSatisfiableCalculationTaskStatusPropagator* mStatusPropagator;

						CCentralizedAllocationLimitation* mAllocationLimitation;

						QVector<cint64> mTaskCreatedDepthCountVec;
						QVector<cint64> mTaskProcessedDepthCountVec;
						QVector<double> mTaskRelativeIncreaseTaskPerDepthVec;
						QVector<double> mTaskTotalIncreaseTaskPerDepthVec;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Calculation

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CALCULATION_CCONCURRENTTASKCALCULATIONENVIRONMENT_H
