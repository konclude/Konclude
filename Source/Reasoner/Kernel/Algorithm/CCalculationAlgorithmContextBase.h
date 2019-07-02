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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONALGORITHMCONTEXTBASE_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONALGORITHMCONTEXTBASE_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"


// Other includes
#include "Scheduler/CTaskContext.h"

#include "Reasoner/Kernel/Process/CProcessContext.h"

#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Scheduler;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Strategy;
			using namespace Task;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CCalculationAlgorithmContextBase
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationAlgorithmContextBase : public CCalculationAlgorithmContext {
					// public methods
					public:
						//! Constructor
						CCalculationAlgorithmContextBase();

						//! Destructor
						virtual ~CCalculationAlgorithmContextBase();

						CCalculationAlgorithmContextBase* initCalculationAlgorithmContext(CTaskProcessorContext* taskHandleContext);

						CCalculationAlgorithmContextBase* initCalculationAlgorithmContext(CTaskProcessorContext* taskHandleContext, CConceptProcessingPriorityStrategy* conceptPriorityStrategy, 
									CIndividualProcessingPriorityStrategy* individualPriorityStrategy, CTaskProcessingPriorityStrategy* taskPriorityStrategy, CUnsatisfiableCacheRetrievalStrategy* unsatCacheRetrievalStrategy, 
									CIndividualNodeManager* indiNodeManager, CClashDescriptorFactory* clashDesFactory, CDependencyFactory* depFactory, CUnsatisfiableCacheHandler* unsatCacheHandler, CSatisfiableExpanderCacheHandler* satExpCacheHandler);
						CCalculationAlgorithmContextBase* initTaskProcessContext(CProcessContext* processContext, CSatisfiableCalculationTask* satCalcTask);

						virtual CProcessTagger* getProcessTagger();
						virtual CProcessContext* getProcessContext();
						virtual CMemoryAllocationManager* getProcessTaskMemoryAllocationManager();
						virtual CMemoryAllocationManager* getMemoryAllocationManager();
						virtual CTaskHandleMemoryAllocationManager* getTemporaryMemoryAllocationManager();
						virtual CConceptProcessingPriorityStrategy* getConceptPriorityStrategy();
						virtual CIndividualProcessingPriorityStrategy* getIndividualPriorityStrategy();
						virtual CProcessingDataBox* getProcessingDataBox();
						virtual CSatisfiableCalculationTask* getSatisfiableCalculationTask();
						virtual CTaskProcessorContext* getTaskProcessorContext();
						virtual CTaskProcessingPriorityStrategy* getTaskPriorityStrategy();
						virtual CProcessingStatisticGathering* getProcessingStatisticsGathering();
						virtual CBranchTreeNode* getBranchTreeNode();
						virtual CBranchTreeNode* getNewBranchTreeNode();
						virtual CDependencyNode* getBaseDependencyNode();
						virtual CUnsatisfiableCacheHandler* getUnsatisfiableCacheHandler();
						virtual CIndividualNodeManager* getIndividualNodeManager();
						virtual CClashDescriptorFactory* getClashDescriptorFactory();
						virtual CUnsatisfiableCacheRetrievalStrategy* getUnsatisfiableCacheRetrievalStrategy();
						virtual CDependencyFactory* getDependencyFactory();
						virtual CSatisfiableExpanderCacheHandler* getSatisfiableExpanderCacheHandler();


					// protected methods
					protected:

					// protected variables
					protected:
						CTaskHandleMemoryAllocationManager* mTempMemMan;
						CMemoryAllocationManager* mPrTaskMemMan;
						CProcessTagger* mProcessTagger;
						CProcessContext* mProcessContext;
						CConceptProcessingPriorityStrategy* mConceptPriorityStrategy;
						CIndividualProcessingPriorityStrategy* mIndividualPriorityStrategy;
						CTaskProcessingPriorityStrategy* mTaskPriorityStrategy;
						CProcessingDataBox* mProcessingDataBox;
						CSatisfiableCalculationTask* mSatCalcTask;
						CTaskProcessorContext* mTaskProcessorContext;
						CProcessingStatisticGathering* mProcStatGath;
						CBranchTreeNode* mBranchTreeNode;
						CDependencyNode* mBaseDepNode;
						CIndividualNodeManager* mIndiNodeManager;
						CUnsatisfiableCacheHandler* mUnsatCacheHandler;
						CClashDescriptorFactory* mClashDescriptorFactory;
						CUnsatisfiableCacheRetrievalStrategy* mUnsatCachRetStrategy;
						CDependencyFactory* mDepFactory;
						CSatisfiableExpanderCacheHandler* mSatExpCacheHandler;


					// private methods
					private:

					// private variables
					private:

				};


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONALGORITHMCONTEXTBASE_H
