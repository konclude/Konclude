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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONALGORITHMCONTEXT_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONALGORITHMCONTEXT_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"


// Other includes
#include "Scheduler/CTaskContext.h"
#include "Scheduler/CTaskProcessorContext.h"

#include "Reasoner/Kernel/Process/CProcessContext.h"
#include "Reasoner/Kernel/Process/CProcessingDataBox.h"
#include "Reasoner/Kernel/Process/CProcessingStatisticGathering.h"

#include "Reasoner/Kernel/Strategy/CConceptProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CIndividualProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CTaskProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CUnsatisfiableCacheRetrievalStrategy.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"

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


#ifdef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
#define STATINC(STATDESC,calcAlgContext)
#define STATINCM(STATDESC,incCount,calcAlgContext)
#else
#define STATINC(STATDESC,calcAlgContext) STATDATAINC(STATDESC,calcAlgContext->getUsedProcessingStatisticsGathering())
#define STATINCM(STATDESC,incCount,calcAlgContext) STATDATAINCM(STATDESC,incCount,calcAlgContext->getUsedProcessingStatisticsGathering())
#endif


				/*! 
				 *
				 *		\class		CCalculationAlgorithmContext
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationAlgorithmContext : public CContext {
					// public methods
					public:
						//! Constructor
						CCalculationAlgorithmContext();

						//! Destructor
						virtual ~CCalculationAlgorithmContext();

						virtual CProcessTagger* getProcessTagger() = 0;
						virtual CProcessContext* getProcessContext() = 0;
						virtual CMemoryAllocationManager* getProcessTaskMemoryAllocationManager() = 0;
						virtual CTaskHandleMemoryAllocationManager* getTemporaryMemoryAllocationManager() = 0;
						virtual CConceptProcessingPriorityStrategy* getConceptPriorityStrategy() = 0;
						virtual CIndividualProcessingPriorityStrategy* getIndividualPriorityStrategy() = 0;
						virtual CProcessingDataBox* getProcessingDataBox() = 0;
						virtual CSatisfiableCalculationTask* getSatisfiableCalculationTask() = 0;
						virtual CTaskProcessorContext* getTaskProcessorContext() = 0;
						virtual CTaskProcessingPriorityStrategy* getTaskPriorityStrategy() = 0;
						virtual CProcessingStatisticGathering* getProcessingStatisticsGathering() = 0;
						virtual CBranchTreeNode* getBranchTreeNode() = 0;
						virtual CBranchTreeNode* getNewBranchTreeNode() = 0;
						virtual CDependencyNode* getBaseDependencyNode() = 0;
						virtual CUnsatisfiableCacheHandler* getUnsatisfiableCacheHandler() = 0;
						virtual CIndividualNodeManager* getIndividualNodeManager() = 0;
						virtual CClashDescriptorFactory* getClashDescriptorFactory() = 0;
						virtual CUnsatisfiableCacheRetrievalStrategy* getUnsatisfiableCacheRetrievalStrategy() = 0;
						virtual CDependencyFactory* getDependencyFactory() = 0;
						virtual CSatisfiableExpanderCacheHandler* getSatisfiableExpanderCacheHandler() = 0;


						inline CProcessTagger* getUsedProcessTagger();
						inline CMemoryAllocationManager* getUsedProcessTaskMemoryAllocationManager();
						inline CTaskHandleMemoryAllocationManager* getUsedTemporaryMemoryAllocationManager();
						inline CProcessContext* getUsedProcessContext();
						inline CConceptProcessingPriorityStrategy* getUsedConceptPriorityStrategy();
						inline CIndividualProcessingPriorityStrategy* getUsedIndividualPriorityStrategy();
						inline CProcessingDataBox* getUsedProcessingDataBox();
						inline CSatisfiableCalculationTask* getUsedSatisfiableCalculationTask();
						inline CTaskProcessorContext* getUsedTaskProcessorContext();
						inline CTaskProcessingPriorityStrategy* getUsedTaskPriorityStrategy();
						inline CProcessingStatisticGathering* getUsedProcessingStatisticsGathering();
						inline CBranchTreeNode* getUsedBranchTreeNode();
						inline CDependencyNode* getUsedBaseDependencyNode();
						inline CUnsatisfiableCacheHandler* getUsedUnsatisfiableCacheHandler();
						inline CIndividualNodeManager* getUsedIndividualNodeManager();
						inline CClashDescriptorFactory* getUsedClashDescriptorFactory();
						inline CUnsatisfiableCacheRetrievalStrategy* getUsedUnsatisfiableCacheRetrievalStrategy();
						inline CDependencyFactory* getUsedDependencyFactory();
						inline CSatisfiableExpanderCacheHandler* getUsedSatisfiableExpanderCacheHandler();

						inline bool setMinModificationAncestorDepthCandidate(cint64 ancDepthCandidate);
						inline bool setMinModificationIndividualIDCandidate(cint64 indiIDCandidate);
						inline bool setMinModificationIndividualCandidate(CIndividualProcessNode* indiNode);


						bool isMinModificationUpdated();
						CCalculationAlgorithmContext* setMinModificationAncestorDepth(cint64 ancDepth);
						cint64 getMinModificationAncestorDepth();
						CCalculationAlgorithmContext* setMinModificationIndividualID(cint64 indiID);
						cint64 getMinModificationIndividualID();
						CCalculationAlgorithmContext* setMinModificationIndividual(CIndividualProcessNode* indiNode);

						cint64 getMaxCompletionGraphCachedIndividualNodeID();
						cint64 getCompletionGraphCachedLocalizationTag();
						bool hasCompletionGraphCachedIndividualNodes();

						CIndividualProcessNode* getCurrentIndividualNode();
						CCalculationAlgorithmContext* setCurrentIndividualNode(CIndividualProcessNode* indiNode);

					// protected methods
					protected:

					// protected variables
					protected:
						CTaskHandleMemoryAllocationManager* mUsedTempMemMan;
						CMemoryAllocationManager* mUsedPrTaskMemMan;
						CProcessTagger* mUsedProcessTagger;
						CProcessContext* mUsedProcessContext;
						CConceptProcessingPriorityStrategy* mUsedConceptPriorityStrategy;
						CIndividualProcessingPriorityStrategy* mUsedIndividualPriorityStrategy;
						CProcessingDataBox* mUsedProcessingDataBox;
						CSatisfiableCalculationTask* mUsedSatCalcTask;
						CTaskProcessorContext* mUsedTaskProcessorContext;
						CTaskProcessingPriorityStrategy* mUsedTaskPriorityStrategy;
						CProcessingStatisticGathering* mUsedProcessStatGath;
						CBranchTreeNode* mUsedBranchTreeNode;
						CDependencyNode* mUsedBaseDepNode;
						cint64 mMinModificationAncestorDepth;
						cint64 mMinModificationIndividualID;
						bool mMinModificationUpdated;
						CUnsatisfiableCacheHandler* mUsedUnsatCacheHandler;
						CIndividualNodeManager* mUsedIndividualNodeManager;
						CClashDescriptorFactory* mUsedClashDescriptorFactory;
						CUnsatisfiableCacheRetrievalStrategy* mUsedUnsatCachRetStrategy;
						CDependencyFactory* mUsedDepFactory;
						CSatisfiableExpanderCacheHandler* mUsedSatExpCacheHandler;
						cint64 mMaxCompletionGraphCachedIndiNodeID;
						CIndividualProcessNode* mCurrentIndiNode;
						cint64 mCompletionGraphCachedLocalizationTag;


					// private methods
					private:

					// private variables
					private:

				};


				CProcessTagger* CCalculationAlgorithmContext::getUsedProcessTagger() {
					return mUsedProcessTagger;
				}
				
				CProcessContext* CCalculationAlgorithmContext::getUsedProcessContext() {
					return mUsedProcessContext;
				}

				CMemoryAllocationManager* CCalculationAlgorithmContext::getUsedProcessTaskMemoryAllocationManager() {
					return mUsedPrTaskMemMan;
				}

				CTaskHandleMemoryAllocationManager* CCalculationAlgorithmContext::getUsedTemporaryMemoryAllocationManager() {
					return mUsedTempMemMan;
				}

				CConceptProcessingPriorityStrategy* CCalculationAlgorithmContext::getUsedConceptPriorityStrategy() {
					return mUsedConceptPriorityStrategy;
				}

				CIndividualProcessingPriorityStrategy* CCalculationAlgorithmContext::getUsedIndividualPriorityStrategy() {
					return mUsedIndividualPriorityStrategy;
				}

				CProcessingDataBox* CCalculationAlgorithmContext::getUsedProcessingDataBox() {
					return mUsedProcessingDataBox;
				}

				CSatisfiableCalculationTask* CCalculationAlgorithmContext::getUsedSatisfiableCalculationTask() {
					return mUsedSatCalcTask;
				}

				CTaskProcessorContext* CCalculationAlgorithmContext::getUsedTaskProcessorContext() {
					return mUsedTaskProcessorContext;
				}

				CTaskProcessingPriorityStrategy* CCalculationAlgorithmContext::getUsedTaskPriorityStrategy() {
					return mUsedTaskPriorityStrategy;
				}

				CProcessingStatisticGathering* CCalculationAlgorithmContext::getUsedProcessingStatisticsGathering() {
					return mUsedProcessStatGath;
				}

				CBranchTreeNode* CCalculationAlgorithmContext::getUsedBranchTreeNode() {
					return mUsedBranchTreeNode;
				}

				CDependencyNode* CCalculationAlgorithmContext::getUsedBaseDependencyNode() {
					return mUsedBaseDepNode;
				}


				bool CCalculationAlgorithmContext::setMinModificationAncestorDepthCandidate(cint64 ancDepthCandidate) {
					bool updated = ancDepthCandidate < mMinModificationIndividualID;
					mMinModificationIndividualID = qMin(ancDepthCandidate,mMinModificationIndividualID);
					return updated;
				}

				bool CCalculationAlgorithmContext::setMinModificationIndividualIDCandidate(cint64 indiIDCandidate) {
					bool updated = indiIDCandidate < mMinModificationIndividualID;
					mMinModificationIndividualID = qMin(indiIDCandidate,mMinModificationIndividualID);
					return updated;
				}

				bool CCalculationAlgorithmContext::setMinModificationIndividualCandidate(CIndividualProcessNode* indiNode) {
					return setMinModificationIndividualIDCandidate(indiNode->getIndividualID()) || setMinModificationAncestorDepthCandidate(indiNode->getIndividualAncestorDepth());
				}

				CUnsatisfiableCacheHandler* CCalculationAlgorithmContext::getUsedUnsatisfiableCacheHandler() {
					return mUsedUnsatCacheHandler;
				}

				CIndividualNodeManager* CCalculationAlgorithmContext::getUsedIndividualNodeManager() {
					return mUsedIndividualNodeManager;
				}
				
				CClashDescriptorFactory* CCalculationAlgorithmContext::getUsedClashDescriptorFactory() {
					return mUsedClashDescriptorFactory;
				}

				CUnsatisfiableCacheRetrievalStrategy* CCalculationAlgorithmContext::getUsedUnsatisfiableCacheRetrievalStrategy() {
					return mUsedUnsatCachRetStrategy;
				}

				CDependencyFactory* CCalculationAlgorithmContext::getUsedDependencyFactory() {
					return mUsedDepFactory;
				}

				CSatisfiableExpanderCacheHandler* CCalculationAlgorithmContext::getUsedSatisfiableExpanderCacheHandler() {
					return mUsedSatExpCacheHandler;
				}

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONALGORITHMCONTEXT_H
