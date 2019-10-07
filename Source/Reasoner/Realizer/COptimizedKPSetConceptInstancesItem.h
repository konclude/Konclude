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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANCESITEM_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANCESITEM_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetIndividualItem.h"
#include "COntologyRealizingDynamicRequirmentProcessingContainer.h"
#include "CPossibleInstancesIndividualsMergingData.h"


// Other includes
#include "Reasoner/Taxonomy/CHierarchyNode.h"

#include "Reasoner/Realization/CConceptInstantiatedItem.h"

#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryLabelCacheItem.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Taxonomy;
		using namespace Realization;
		using namespace Kernel::Cache;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetConceptInstancesItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetConceptInstancesItem : public CConceptInstantiatedItem, public COntologyRealizingDynamicRequirmentProcessingContainer {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetConceptInstancesItem();

					COptimizedKPSetConceptInstancesItem* initInstancesItem(CHierarchyNode* hierNode);

					COptimizedKPSetConceptInstancesItem* setInitialUnprocessedSuccessorItemCount();

					CHierarchyNode* getHierarchyNode();
					COptimizedKPSetConceptInstancesItem* setHierarchyNode(CHierarchyNode* hierNode);

					QMap<cint64, COptimizedKPSetIndividualItem*>* getKnownInstancesMap();
					QMap<cint64, COptimizedKPSetIndividualItem*>* getPossibleInstancesMap();

					QList<COptimizedKPSetConceptInstancesItem*>* getParentItemList();
					QList<COptimizedKPSetConceptInstancesItem*>* getSuccessorItemList();

					bool hasKnownInstance(COptimizedKPSetIndividualItem* item);
					bool hasPossibleInstance(COptimizedKPSetIndividualItem* item);

					COptimizedKPSetConceptInstancesItem* addKnownInstance(COptimizedKPSetIndividualItem* item);
					COptimizedKPSetConceptInstancesItem* removeKnownInstance(COptimizedKPSetIndividualItem* item);
					COptimizedKPSetConceptInstancesItem* addPossibleInstance(COptimizedKPSetIndividualItem* item);

					COptimizedKPSetConceptInstancesItem* addParentItem(COptimizedKPSetConceptInstancesItem* item);
					COptimizedKPSetConceptInstancesItem* addSuccessorItem(COptimizedKPSetConceptInstancesItem* item);

					cint64 getUnprocessedSuccessorItemCount();
					COptimizedKPSetConceptInstancesItem* setUnprocessedSuccessorItemCount(cint64 unpSuccCount);
					COptimizedKPSetConceptInstancesItem* incUnprocessedSuccessorItemCount(cint64 incCount = 1);
					COptimizedKPSetConceptInstancesItem* decUnprocessedSuccessorItemCount(cint64 decCount = 1);

					cint64 getTestingPossibleInstancesCount();
					bool hasTestingPossibleInstances();
					COptimizedKPSetConceptInstancesItem* setTestingPossibleInstancesCount(cint64 testPossIntCount);
					COptimizedKPSetConceptInstancesItem* incTestingPossibleInstancesCount(cint64 incCount = 1);
					COptimizedKPSetConceptInstancesItem* decTestingPossibleInstancesCount(cint64 decCount = 1);

					bool hasAllSuccessorProcessedFlag();
					COptimizedKPSetConceptInstancesItem* setAllSuccessorProcessedFlag(bool allSuccProcessed);

					bool hasToProcessPossibleInstancesFlag();
					COptimizedKPSetConceptInstancesItem* setToProcessPossibleInstancesFlag(bool toProcess);

					bool hasPossibleInstancesProcessingQueuedFlag();
					COptimizedKPSetConceptInstancesItem* setPossibleInstancesProcessingQueuedFlag(bool processingQueued);

					bool hasSelfSuccessorProcessedFlag();
					COptimizedKPSetConceptInstancesItem* setSelfSuccessorProcessedFlag(bool selfSuccProcessed);

					bool hasPossibleInstances();
					COptimizedKPSetIndividualItem* takeNextTestingPossibleInstance();
					COptimizedKPSetConceptInstancesItem* removeTestingPossibleInstance(COptimizedKPSetIndividualItem* possInstance);
					QMap<cint64, COptimizedKPSetIndividualItem*>* getPrefferedPossibleInstanceTestingSet();


					COptimizedKPSetConceptInstancesItem* setConceptInstancesTestingFinished(bool finished);
					bool isConceptInstancesTestingFinished();

					QHash<COptimizedKPSetIndividualItem*, CPossibleInstancesIndividualsMergingLinker*>* getIndividualItemPossibleInstanceMergingLinkerHash(bool create = false);
					QList<CPossibleInstancesIndividualsMergingData*>* getPossibleInstanceMergingDataContainer(bool create = false);
					COptimizedKPSetConceptInstancesItem* clearPossibleInstanceMergingData();

					QMap<cint64, COptimizedKPSetIndividualItem*>* getModelMergingCheckedSet(bool create = false);


					COptimizedKPSetConceptInstancesItem* addKnownInstancesLabelCacheItem(CBackendRepresentativeMemoryLabelCacheItem* item);
					COptimizedKPSetConceptInstancesItem* addKnownMostSpecificInstancesLabelCacheItem(CBackendRepresentativeMemoryLabelCacheItem* item);
					COptimizedKPSetConceptInstancesItem* addPossibleInstancesLabelCacheItem(CBackendRepresentativeMemoryLabelCacheItem* item);
					COptimizedKPSetConceptInstancesItem* addPossibleMostSpecificInstancesLabelCacheItem(CBackendRepresentativeMemoryLabelCacheItem* item);


					QList<CBackendRepresentativeMemoryLabelCacheItem*>* getKnownInstancesLabelCacheItemList();
					QList<CBackendRepresentativeMemoryLabelCacheItem*>* getKnownMostSpecificInstancesLabelCacheItemList();
					QList<CBackendRepresentativeMemoryLabelCacheItem*>* getPossibleInstancesLabelCacheItemList();
					QList<CBackendRepresentativeMemoryLabelCacheItem*>* getPossibleMostSpecificInstancesLabelCacheItemList();


				// protected methods
				protected:

				// protected variables
				protected:
					CHierarchyNode* mHierNode;
					QMap<cint64, COptimizedKPSetIndividualItem*> mKnownInstancesMap;
					QMap<cint64, COptimizedKPSetIndividualItem*> mPossibleInstancesMap;

					QList<COptimizedKPSetConceptInstancesItem*> mParentItemList;
					QList<COptimizedKPSetConceptInstancesItem*> mSuccessorItemList;

					cint64 mUnprocessedSuccItemCount;
					cint64 mTestingPossInstanceCount;
					bool mAllSuccProcessedFlag;
					bool mToProcessFlag;
					bool mProcessingQueuedFlag;
					bool mSelfSuccsCompletedFlag;

					bool mConceptInstancesTestingFinished;



					QHash<COptimizedKPSetIndividualItem*, CPossibleInstancesIndividualsMergingLinker*>* mIndividualItemPossibleInstanceMergingLinkerHash;
					QList<CPossibleInstancesIndividualsMergingData*>* mPossibleInstanceMergingDataContainer;
					QMap<cint64, COptimizedKPSetIndividualItem*>* mPrefferedPossibleInstancesSet;

					QMap<cint64, COptimizedKPSetIndividualItem*>* mModelMergingCheckedSet;




					QList<CBackendRepresentativeMemoryLabelCacheItem*> mKnownInstancesLabelCacheItemList;
					QList<CBackendRepresentativeMemoryLabelCacheItem*> mKnownMostSpecificInstancesLabelCacheItemList;
					QList<CBackendRepresentativeMemoryLabelCacheItem*> mPossibleInstancesLabelCacheItemList;
					QList<CBackendRepresentativeMemoryLabelCacheItem*> mPossibleMostSpecificInstancesLabelCacheItemList;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANCESITEM_H
