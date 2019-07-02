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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLEEXPANDERCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLEEXPANDERCACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CClashDescriptorFactory.h"
#include "CTrackedClashedDescriptor.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CClashedDependencyDescriptor.h"
#include "Reasoner/Kernel/Process/CIndividualNodeSatisfiableExpandingCacheStoringData.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"

#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheReader.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheWriter.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheEntry.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheEntryWriteData.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheEntryExpandWriteData.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData.h"

#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Dependency;
			using namespace Cache;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CSatisfiableExpanderCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableExpanderCacheHandler {
					// public methods
					public:
						//! Constructor
						CSatisfiableExpanderCacheHandler(CSignatureSatisfiableExpanderCacheReader* satCacheReader, CSignatureSatisfiableExpanderCacheWriter* satCacheWriter);

						//! Destructor
						virtual ~CSatisfiableExpanderCacheHandler();

						typedef QPair<cint64,CDependencyTrackPoint*> CDepthDependencyPair;


						bool isIndividualNodeExpandCached(CIndividualProcessNode* individualNode, bool* satisfiable, CSignatureSatisfiableExpanderCacheEntry** entry, CCalculationAlgorithmContext* calcAlgContext);
						bool compareIndividualNodeCompatibility(CIndividualProcessNode* individualNode, CSignatureSatisfiableExpanderCacheEntry* cachedEntry, CCalculationAlgorithmContext* calcAlgContext);

						bool cacheIndividualNodeExpansion(CIndividualProcessNode* individualNode, CCalculationAlgorithmContext* calcAlgContext);
						bool cacheIndividualNodeExpansion(CIndividualProcessNode* individualNode, CIndividualNodeSatisfiableExpandingCacheStoringData* satStorData, CConceptDescriptor* lastAddedConDes, CConceptDescriptor* lastCachedConceptDes, cint64 lastSignature, cint64 lastCachedSignature, CCalculationAlgorithmContext* calcAlgContext);


						bool hasDependencyToAncestor(CIndividualProcessNode* individualNode, CDependencyTrackPoint* depTrackPoint, bool* branchedFlag, CCalculationAlgorithmContext* calcAlgContext);
						bool simpleDependencyTracking(CIndividualProcessNode* individualNode, cint64 conTag, CDependencyTrackPoint* depTrackPoint, CCACHINGHASH<cint64,cint64>* depHash, CPROCESSINGSET<cint64>* notBranchConceptsSet, bool* branchedFlag, CCalculationAlgorithmContext* calcAlgContext);
						bool complexDependencyTracking(CIndividualProcessNode* individualNode, cint64 conTag, CPROCESSINGSET<CDepthDependencyPair>* depSet, CPROCESSINGLIST<CDepthDependencyPair>* depList, CCACHINGHASH<cint64,cint64>* depHash, CPROCESSINGSET<cint64>* notBranchConceptsSet, bool* branchedFlag, CCalculationAlgorithmContext* calcAlgContext);

						bool cacheIndividualNodeSatisfiable(CIndividualProcessNode* individualNode, CCalculationAlgorithmContext* calcAlgContext);

						bool isConceptRelevantForSatisfiableBranch(CIndividualProcessNode* individualNode, CConceptDescriptor* conDes, CCalculationAlgorithmContext* calcAlgContext);
						bool isAutomatConceptRelevantForSatisfiableBranch(CIndividualProcessNode* individualNode, CConcept* concept, bool conNegation, CCalculationAlgorithmContext* calcAlgContext);

						bool isCardinalityRestrictionCriticalForSatisfiable(CIndividualProcessNode* individualNode, CConceptDescriptor* conDes, CCalculationAlgorithmContext* calcAlgContext);
						bool hasInverseSubRole(CRole* role, CCalculationAlgorithmContext* calcAlgContext);
						bool hasInverseSuperRole(CRole* role, CCalculationAlgorithmContext* calcAlgContext);
					
						bool commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:
						bool prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext);
						bool addCacheMessages(CSignatureSatisfiableExpanderCacheEntryWriteData* expandWriteData, CCalculationAlgorithmContext* calcAlgContext);

					// protected variables
					protected:
						CSignatureSatisfiableExpanderCacheReader* mSatCacheReader;
						CSignatureSatisfiableExpanderCacheWriter* mSatCacheWriter;

						CReapplyConceptLabelSet* mTestConSet;

						CSignatureSatisfiableExpanderCacheEntryWriteData* mWriteData;
						CTaskMemoryPoolAllocationManager* mMemAllocMan;
						CMemoryPoolContainer mMemPoolCon;
						CContextBase* mTmpContext;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLEEXPANDERCACHEHANDLER_H
