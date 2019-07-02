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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATURATIONNODEEXPANSIONCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATURATIONNODEEXPANSIONCACHEHANDLER_H

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
#include "Reasoner/Kernel/Process/CIndividualNodeSatisfiableExpandingCacheStoringData.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptReferenceLinking.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Consistiser/CSaturationConceptDataItem.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"

#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCacheReader.h"
#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCacheWriter.h"
#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCacheEntry.h"
#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCacheWriteData.h"
#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCacheUnsatisfiabilityWriteData.h"
#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCacheExpansionWriteData.h"

#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Consistiser;

		namespace Kernel {

			using namespace Process;
			using namespace Dependency;
			using namespace Cache;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CSaturationNodeExpansionCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationNodeExpansionCacheHandler {
					// public methods
					public:
						//! Constructor
						CSaturationNodeExpansionCacheHandler(CSaturationNodeAssociatedExpansionCacheReader* satCacheReader, CSaturationNodeAssociatedExpansionCacheWriter* satCacheWriter);

						CIndividualSaturationProcessNode* getSaturationIndividualNodeForConcept(CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext);

						bool cacheUnsatisfiableConcept(CConcept* concept, CCalculationAlgorithmContext* calcAlgContext);
						bool isConceptUnsatisfiableCached(CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext);


						bool isNodeSatisfiableCached(CIndividualProcessNode* individualProcessNode, CSaturationNodeAssociatedConceptExpansion*& expansion, CCalculationAlgorithmContext* calcAlgContext);

						bool tryNodeSatisfiableCaching(CIndividualProcessNode* individualProcessNode, CCalculationAlgorithmContext* calcAlgContext);
						
						bool getCachedDeterministicExpansion(CIndividualSaturationProcessNode* saturationNode, CSaturationNodeAssociatedDeterministicConceptExpansion*& expansion, CCalculationAlgorithmContext* calcAlgContext);

						bool commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:
						bool prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext);
						bool addCacheMessages(CSaturationNodeAssociatedExpansionCacheWriteData* expandWriteData, CCalculationAlgorithmContext* calcAlgContext);

						CCacheValue getCacheValueForConcept(CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext);
						CCacheValue getCacheValueForConcept(CConceptDescriptor* conDes, CCalculationAlgorithmContext* calcAlgContext);

						bool testNodeMatchingExpansion(CSaturationNodeAssociatedConceptExpansion* expansion, CSaturationNodeAssociatedConceptExpansion* alternativeExpansion, CReapplyConceptLabelSet* conSet, CIndividualNodeSaturationBlockingData* satBlockData, CCalculationAlgorithmContext* calcAlgContext);


						bool isDeterministicallyDependingOnSaturationConcept(CIndividualProcessNode* individualNode, CDependencyTrackPoint* depTrackPoint, CConceptDescriptor* saturationConceptDescriptor, CCalculationAlgorithmContext* calcAlgContext);

						bool testNodeCachingPossible(CIndividualProcessNode* individualProcessNode, bool& onlyIfCompletelyDeterministic, bool& onlyAllNondeterministic, CSaturationNodeAssociatedExpansionCacheEntry*& cacheEntry, CCalculationAlgorithmContext* calcAlgContext);


					// protected variables
					protected:
						CSaturationNodeAssociatedExpansionCacheReader* mSatCacheReader;
						CSaturationNodeAssociatedExpansionCacheWriter* mSatCacheWriter;

						CSaturationNodeAssociatedExpansionCacheWriteData* mWriteData;
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

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATURATIONNODEEXPANSIONCACHEHANDLER_H
