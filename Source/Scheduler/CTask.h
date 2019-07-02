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

#ifndef KONCLUDE_SCHEDULER_CTASK_H
#define KONCLUDE_SCHEDULER_CTASK_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskStatus.h"
#include "CTaskResult.h"
#include "CBooleanTaskResult.h"
#include "CTaskHandleContext.h"
#include "CTaskContext.h"
#include "CTaskContextBase.h"
#include "CTaskOwner.h"


// Other includes
#include "Utilities/Container/CLinker.h"
#include "Utilities/Container/CSortedLinker.h"
#include "Utilities/Container/CNegLinker.h"
#include "Utilities/Memory/CMemoryPool.h"
#include "Utilities/Memory/CMemoryPoolContainer.h"
#include "Utilities/Memory/CDeletionContainer.h"
#include "Utilities/Memory/CObjectAllocator.h"


#include "Concurrent/Callback/CCallbackData.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;
	using namespace Utilities::Memory;
	using namespace Concurrent::Callback;

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTask
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTask : public CSortedLinkerBase<CTask*,CTask,CTask>, public CMemoryPoolContainer, public CDeletionContainer {
			// public methods
			public:
				//! Constructor
				CTask(CMemoryPool* memoryPool = nullptr);

				//! Destructor
				virtual ~CTask();

				CTask* setTaskID(cint64 id);
				cint64 getTaskID() const;

				CTask* setTaskDepth(cint64 taskDepth);
				cint64 getTaskDepth() const;

				CTask* setTaskType(cint64 taskType);
				cint64 getTaskType() const;

				double getTaskPriority() const;
				CTask* setTaskPriority(double taskPriority);

				CTask* getParentTask() const;
				CTask* getRootTask() const;

				CTask* setParentTask(CTask* parentTask);
				CTask* setRootTask(CTask* rootTask);

				CTaskResult* getTaskResult();
				CTaskStatus* getTaskStatus();

				virtual CTaskContext* getTaskContext(CTaskHandleContext* context);

				virtual CTask* makeTaskReference(CTask* dependedTask, CTaskHandleContext* handlerContext);

				virtual CTask* initTask(CTask* parentTask, CTaskHandleContext* context);

				virtual CTask* completeTask();

				CTask* incActiveReferenceCount(cint64 incCount = 1);
				CTask* decActiveReferenceCount(cint64 decCount = 1);
				cint64 getActiveReferenceCount() const;
				bool hasActiveReferencedTask() const;

				CTask* incDependedStatusUpdatesCount(cint64 incCount = 1);
				CTask* decDependedStatusUpdatesCount(cint64 decCount = 1);
				cint64 getDependedStatusUpdatesCount() const;
				bool hasDependedStatusUpdates() const;

				bool hasDependedStatusUpdatesOrActiveReferencedTasks() const;

				CXNegLinker<CTask*>* getReferencedTaskLinker();
				CTask* setReferencedTaskLinker(CXNegLinker<CTask*>* referencedTaskLinker);
				CTask* appendReferencedTaskLinker(CXNegLinker<CTask*>* referencedTaskLinker);

				CCallbackData* getCallbackLinker();
				CTask* setCallbackLinker(CCallbackData* callback);
				CTask* addCallbackLinker(CCallbackData* callback);


				CTaskOwner* getTaskOwner();
				CTask* setTaskOwner(CTaskOwner* taskOwner);
				CTask* clearTaskOwner();
				bool hasTaskOwner(CTaskOwner* taskOwner);
				bool hasTaskOwner();

				bool hasNoTaskOwnerAndNoParentTask();


				bool clearUninitializedReferenceTasks();


				static inline bool sortedLinkerDataCompare(CTask* const & beforeData1, CTask* const & data2) {
					return beforeData1->getTaskPriority() >= data2->getTaskPriority();
				}

				CTask* setCompletionRequested(bool requested);
				bool getCompletionRequested();
				bool isCompletionRequested();

				bool isTaskRelevant();
				CTask* setTaskRelevant(bool relevant);

				bool isTaskDispenseMarked();
				CTask* setTaskDispenseMarked(bool dispenseable);


			// protected methods
			protected:
				CTask* setCompletionNegatorConnector(CNegator* negator);

				virtual CTaskStatus* createTaskStatus(CTaskHandleContext* handlerContext) = 0;
				virtual CTaskResult* createTaskResult(CTaskHandleContext* handlerContext) = 0;

			// protected variables
			protected:
				cint64 mTaskID;
				cint64 mTaskType;
				double mTaskPriority;
				bool mTaskRelevant;
				bool mTaskDispensMarked;
				CTaskStatus* mTaskStatus;
				CTaskResult* mTaskResult;

				CTaskContext* mTaskContext;
				CTaskOwner* mTaskOwner;

				CTask* mParentTask;
				CTask* mRootTask;
				cint64 mActiveTaskReferenceCount;
				cint64 mDependedStatusUpdatesCount;
				cint64 mTaskDepth;

				CXNegLinker<CTask*>* mReferencedTaskLinker;
				CNegator* mCompletionNegator;
				CCallbackData* mCallbackLinker;

				bool mCompletionRequested;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASK_H
