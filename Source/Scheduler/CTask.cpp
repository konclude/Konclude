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

#include "CTask.h"


namespace Konclude {

	namespace Scheduler {



		CTask::CTask(CMemoryPool* memoryPool) : CSortedLinkerBase<CTask*,CTask,CTask>(this,nullptr), CMemoryPoolContainer(memoryPool) {
		}

		CTask::~CTask() {
		}

		CTask* CTask::setTaskID(cint64 id) {
			mTaskID = id;
			return this;
		}

		cint64 CTask::getTaskID() const {
			return mTaskID;
		}

		CTask* CTask::setTaskDepth(cint64 taskDepth) {
			mTaskDepth = taskDepth;
			return this;
		}

		cint64 CTask::getTaskDepth() const {
			return mTaskDepth;
		}


		CTask* CTask::setTaskType(cint64 taskType) {
			mTaskType = taskType;
			return this;
		}

		cint64 CTask::getTaskType() const {
			return mTaskType;
		}


		double CTask::getTaskPriority() const {
			return mTaskPriority;
		}

		CTask* CTask::setTaskPriority(double taskPriority) {
			mTaskPriority = taskPriority;
			return this;
		}

		CTask* CTask::getParentTask() const {
			return mParentTask;
		}

		CTask* CTask::getRootTask() const {
			return mRootTask;
		}


		CTask* CTask::setParentTask(CTask* parentTask) {
			mParentTask = parentTask;
			return this;
		}

		CTask* CTask::setRootTask(CTask* rootTask) {
			mRootTask = rootTask;
			return this;
		}


		CTaskResult* CTask::getTaskResult() {
			return mTaskResult;
		}

		CTaskStatus* CTask::getTaskStatus() {
			return mTaskStatus;
		}


		CTaskContext* CTask::getTaskContext(CTaskHandleContext* context) {
			if (!mTaskContext) {
				void* memManAdd = CTaskMemoryPoolAllocationManager::allocateMemoryToContainer(sizeof(CTaskContextBase),this,context->getTaskHandleMemoryAllocationManager());
				mTaskContext = new (memManAdd) CTaskContextBase(this,context);
			} else {
				mTaskContext->updateContext(context);
			}
			return mTaskContext;
		}


		CTask* CTask::decActiveReferenceCount(cint64 decCount) {
			mActiveTaskReferenceCount -= decCount;
			return this;
		}

		CTask* CTask::incActiveReferenceCount(cint64 incCount) {
			mActiveTaskReferenceCount += incCount;
			return this;
		}

		cint64 CTask::getActiveReferenceCount() const {
			return mActiveTaskReferenceCount;
		}

		bool CTask::hasActiveReferencedTask() const {
			return mActiveTaskReferenceCount > 0;
		}


		CTask* CTask::decDependedStatusUpdatesCount(cint64 decCount) {
			mDependedStatusUpdatesCount -= decCount;
			return this;
		}

		CTask* CTask::incDependedStatusUpdatesCount(cint64 incCount) {
			mDependedStatusUpdatesCount += incCount;
			return this;
		}

		cint64 CTask::getDependedStatusUpdatesCount() const {
			return mDependedStatusUpdatesCount;
		}

		bool CTask::hasDependedStatusUpdates() const {
			return mDependedStatusUpdatesCount > 0;
		}

		bool CTask::hasDependedStatusUpdatesOrActiveReferencedTasks() const {
			return hasDependedStatusUpdates() || hasActiveReferencedTask();
		}


		CTask* CTask::makeTaskReference(CTask* dependedTask, CTaskHandleContext* handlerContext) {
			dependedTask->setParentTask(this);
			dependedTask->setRootTask(mRootTask);
			dependedTask->setTaskDepth(mTaskDepth+1);
			mActiveTaskReferenceCount++;
			CTaskContext* taskContext = getTaskContext(handlerContext);
			CXNegLinker<CTask*>* childNegLinker = CObjectAllocator<CXNegLinker<CTask*> >::allocateAndConstruct(taskContext->getMemoryAllocationManager());
			mReferencedTaskLinker = childNegLinker->initNegLinker(dependedTask,true,mReferencedTaskLinker);
			dependedTask->setCompletionNegatorConnector(childNegLinker);
			return this;
		}


		CTask* CTask::initTask(CTask* parentTask, CTaskHandleContext* handlerContext) {		
			initLinker(this,nullptr);
			mTaskStatus = nullptr;
			mTaskResult = nullptr;
			mRootTask = this;
			mParentTask = nullptr;
			mTaskContext = nullptr;
			mCompletionNegator = nullptr;
			mReferencedTaskLinker = nullptr;
			mCallbackLinker = nullptr;
			mTaskOwner = nullptr;
			mTaskID = 0;
			mTaskPriority = 0.;
			mActiveTaskReferenceCount = 0;
			mDependedStatusUpdatesCount = 0;
			mTaskDepth = 0;
			mCompletionRequested = false;
			mTaskRelevant = false;
			mTaskDispensMarked = false;
			mTaskType = 0;

			mTaskStatus = createTaskStatus(handlerContext);
			mTaskResult = createTaskResult(handlerContext);

			if (parentTask) {
				parentTask->makeTaskReference(this,handlerContext);
				mRootTask = parentTask->getRootTask();
			}
			return this;
		}


		CXNegLinker<CTask*>* CTask::getReferencedTaskLinker() {
			return mReferencedTaskLinker;
		}

		CTask* CTask::setReferencedTaskLinker(CXNegLinker<CTask*>* referencedTaskLinker) {
			mReferencedTaskLinker = referencedTaskLinker;
			return this;
		}

		CTask* CTask::appendReferencedTaskLinker(CXNegLinker<CTask*>* referencedTaskLinker) {
			mReferencedTaskLinker = referencedTaskLinker->getLastListLink()->setNext(mReferencedTaskLinker);
			return this;
		}


		CTask* CTask::setCompletionNegatorConnector(CNegator* negator) {
			mCompletionNegator = negator;
			return this;
		}

		CTask* CTask::completeTask() {
			if (mCompletionNegator) {
				mCompletionNegator->setNegation(false);
			}
			return this;
		}


		CCallbackData* CTask::getCallbackLinker() {
			return mCallbackLinker;
		}

		CTask* CTask::setCallbackLinker(CCallbackData* callback) {
			mCallbackLinker = callback;
			return this;
		}

		CTask* CTask::addCallbackLinker(CCallbackData* callback) {
			mCallbackLinker = callback->getLastListLink()->setNext(mCallbackLinker);
			return this;
		}


		CTaskOwner* CTask::getTaskOwner() {
			return mTaskOwner;
		}

		CTask* CTask::setTaskOwner(CTaskOwner* taskOwner) {
			mTaskOwner = taskOwner;
			return this;
		}

		CTask* CTask::clearTaskOwner() {
			mTaskOwner = nullptr;
			return this;
		}

		bool CTask::hasTaskOwner(CTaskOwner* taskOwner) {
			return mTaskOwner == taskOwner;
		}

		bool CTask::hasTaskOwner() {
			return mTaskOwner;
		}

		bool CTask::hasNoTaskOwnerAndNoParentTask() {
			return mTaskOwner == nullptr && mParentTask == nullptr;
		}


		CTask* CTask::setCompletionRequested(bool requested) {
			mCompletionRequested = requested;
			return this;
		}

		bool CTask::getCompletionRequested() {
			return mCompletionRequested;
		}

		bool CTask::isCompletionRequested() {
			return mCompletionRequested;
		}


		bool CTask::isTaskRelevant() {
			return mTaskRelevant;
		}

		CTask* CTask::setTaskRelevant(bool relevant) {
			mTaskRelevant = relevant;
			return this;
		}


		bool CTask::isTaskDispenseMarked() {
			return mTaskDispensMarked;
		}

		CTask* CTask::setTaskDispenseMarked(bool dispenseable) {
			mTaskDispensMarked = dispenseable;
			return this;
		}



		bool CTask::clearUninitializedReferenceTasks() {
			bool clearedRefTasks = false;
			CXNegLinker<CTask*>* refTaskIt = mReferencedTaskLinker;
			while (refTaskIt) {
				if (refTaskIt->isNegated() && refTaskIt->getData()->getTaskStatus()->isTaskStateUNINITIALIZED()) {
					refTaskIt->setNegation(false);
					--mActiveTaskReferenceCount;
					clearedRefTasks = true;
				}
				refTaskIt = refTaskIt->getNext();
			}
			return clearedRefTasks;
		}


	}; // end namespace Scheduler

}; // end namespace Konclude
