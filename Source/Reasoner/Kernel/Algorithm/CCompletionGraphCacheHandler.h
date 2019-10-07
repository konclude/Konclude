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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCOMPLETIONGRAPHCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCOMPLETIONGRAPHCACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CClashDescriptorFactory.h"
#include "CTrackedClashedDescriptor.h"
#include "CCalculationAlgorithmContextBase.h"
#include "CIndividualNodeBackendCacheHandler.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CClashedDependencyDescriptor.h"
#include "Reasoner/Kernel/Process/CIndividualNodeSatisfiableExpandingCacheStoringData.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"

#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"

#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Dependency;
			using namespace Task;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CCompletionGraphCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCompletionGraphCacheHandler {
					// public methods
					public:
						//! Constructor
						CCompletionGraphCacheHandler(CIndividualNodeBackendCacheHandler* backendCacheHandler);

						//! Destructor
						virtual ~CCompletionGraphCacheHandler();

						bool isIndividualNodeCompletionGraphConsistenceBlocked(CIndividualProcessNode* individualNode, bool& conceptSetExtended, CCalculationAlgorithmContext* calcAlgContext);
						bool isIndividualNodeCompletionGraphConsistencePresentNonBlocked(CIndividualProcessNode* individualNode, CCalculationAlgorithmContext* calcAlgContext);

						bool getReactivationIndividuals(CIndividualProcessNode* individualNode, CIndividualReactivationProcessingQueue* reactProcQueue, CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:
						bool testCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext);
							

						bool testConceptLabelExtended(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext);

						bool testConceptLabelSubSetCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext);
						bool testDistinctSubSetCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext);
						bool testMergingSubSetCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext);
						bool testBindingSubSetCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext);

						bool testCriticalConcept(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CConceptDescriptor* conDes, bool containedFlag, CCalculationAlgorithmContext* calcAlgContext);
						bool testCriticalB2AutomateTransitionOperands(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CConcept* concept, bool conNegation, CCalculationAlgorithmContext* calcAlgContext);

						bool checkPropagatedConceptsMissing(CConcept* concept, bool conNegation, CIndividualProcessNode* succIndi, CConceptOperator* conOperator, CIndividualProcessNode* compGraphCachedIndiNode);

						bool loadConsistenceModelData(CCalculationAlgorithmContext* calcAlgContext);


						bool visitMergedIndividualsBackendSynchronisationData(CIndividualProcessNode* indiNode, function<bool(CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* mergedDataIndiNode)> visitFunc, CCalculationAlgorithmContext* calcAlgContext);


					// protected variables
					protected:
						CConcreteOntology* mLastOntology;
						bool mLastLoadedCachedCompletionGraph;
						bool mPreviousConsistencyCachedCompletionGraph;
						CSatisfiableCalculationTask* mCalcTask;
						CSatisfiableCalculationTask* mDetSatCalcTask;
						CSatisfiableCalculationTask* mCompGraphCachedCalcTask;
						CIndividualProcessNodeVector* mIndiProcNodeVec;
						CIndividualProcessNodeVector* mDetSatProcNodeVec;
						CIndividualProcessNodeVector* mCompGraphCachedProcNodeVec;
						cint64 mDetLocalizationTag;
						bool mIncrementalExpansionCaching;
						cint64 mCurrentIncrementalExpansionID;
						CConfiguration* mLastConfig;


						CIndividualNodeBackendCacheHandler* mBackendCacheHandler;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCOMPLETIONGRAPHCACHEHANDLER_H
