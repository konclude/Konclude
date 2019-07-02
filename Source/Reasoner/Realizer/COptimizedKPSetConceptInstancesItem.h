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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANCESITEM_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANCESITEM_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetConceptInstantiatedItem.h"


// Other includes
#include "Reasoner/Taxonomy/CHierarchyNode.h"

#include "Reasoner/Realization/CConceptInstantiatedItem.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Taxonomy;
		using namespace Realization;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedKPSetConceptInstancesItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetConceptInstancesItem : public CConceptInstantiatedItem {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetConceptInstancesItem();

					COptimizedKPSetConceptInstancesItem* initInstancesItem(CHierarchyNode* hierNode);

					COptimizedKPSetConceptInstancesItem* setInitialUnprocessedSuccessorItemCount();

					CHierarchyNode* getHierarchyNode();
					COptimizedKPSetConceptInstancesItem* setHierarchyNode(CHierarchyNode* hierNode);

					QSet<COptimizedKPSetConceptInstantiatedItem*>* getKnownInstancesSet();
					QSet<COptimizedKPSetConceptInstantiatedItem*>* getPossibleInstancesSet();

					QList<COptimizedKPSetConceptInstancesItem*>* getParentItemList();
					QList<COptimizedKPSetConceptInstancesItem*>* getSuccessorItemList();

					bool hasKnownInstance(COptimizedKPSetConceptInstantiatedItem* item);
					bool hasPossibleInstance(COptimizedKPSetConceptInstantiatedItem* item);

					COptimizedKPSetConceptInstancesItem* addKnownInstance(COptimizedKPSetConceptInstantiatedItem* item);
					COptimizedKPSetConceptInstancesItem* removeKnownInstance(COptimizedKPSetConceptInstantiatedItem* item);
					COptimizedKPSetConceptInstancesItem* addPossibleInstance(COptimizedKPSetConceptInstantiatedItem* item);

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
					COptimizedKPSetConceptInstantiatedItem* takeNextTestingPossibleInstance();


				// protected methods
				protected:

				// protected variables
				protected:
					CHierarchyNode* mHierNode;
					QSet<COptimizedKPSetConceptInstantiatedItem*> mKnownInstancesSet;
					QSet<COptimizedKPSetConceptInstantiatedItem*> mPossibleInstancesSet;

					QList<COptimizedKPSetConceptInstancesItem*> mParentItemList;
					QList<COptimizedKPSetConceptInstancesItem*> mSuccessorItemList;

					cint64 mUnprocessedSuccItemCount;
					cint64 mTestingPossInstanceCount;
					bool mAllSuccProcessedFlag;
					bool mToProcessFlag;
					bool mProcessingQueuedFlag;
					bool mSelfSuccsCompletedFlag;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDKPSETCONCEPTINSTANCESITEM_H
