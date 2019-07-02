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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSSATISFIABLETESTINGITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSSATISFIABLETESTINGITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CConceptSubsumerObserver.h"
#include "CClassificationSatisfiableCalculationConceptReferenceLinking.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSatisfiableReferenceLinkingData.h"

#include "Reasoner/Consistiser/CIndividualDependenceTrackingMarker.h"

#include "Reasoner/Taxonomy/CHierarchyNode.h"

#include "Reasoner/Kernel/Cache/CCacheEntry.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;
		using namespace Consistiser;
		using namespace Kernel::Cache;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COptimizedSubClassSatisfiableTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedSubClassSatisfiableTestingItem : public CClassificationSatisfiableCalculationConceptReferenceLinking, public CConceptSubsumerObserver, public CIndividualDependenceTrackingMarker {
				// public methods
				public:
					//! Constructor
					COptimizedSubClassSatisfiableTestingItem();


					COptimizedSubClassSatisfiableTestingItem* initSatisfiableTestingItem(CConcept* satTestConcept);
					CConcept* getSatisfiableConcept();

					CHierarchyNode* getSatisfiableConceptHierarchyNode();
					COptimizedSubClassSatisfiableTestingItem* setSatisfiableConceptHierarchyNode(CHierarchyNode* hierNode);

					QSet<COptimizedSubClassSatisfiableTestingItem*>* getSubsumingConceptItemSet();
					QList<COptimizedSubClassSatisfiableTestingItem*>* getSubsumingConceptItemList();
					cint64 getSubsumingConceptItemCount() const;

					QList<COptimizedSubClassSatisfiableTestingItem*>* getSuccessorItemList();
					cint64 getUnprocessedPredecessorItemCount();

					bool hasOnlyProcessedPredecessorItems();
					COptimizedSubClassSatisfiableTestingItem* decUnprocessedPredecessorItems(cint64 decCount = 1);
					COptimizedSubClassSatisfiableTestingItem* incUnprocessedPredecessorItems(cint64 incCount = 1);
					COptimizedSubClassSatisfiableTestingItem* setUnprocessedPredecessorItems(cint64 count);

					COptimizedSubClassSatisfiableTestingItem* addSuccessorSatisfiableTestItem(COptimizedSubClassSatisfiableTestingItem* succItem);

					bool isSatisfiableTestOrdered();
					bool isSatisfiableTested();
					bool isResultUnsatisfiableDerivated();
					bool isResultSatisfiableDerivated();
					bool getSatisfiableTestedResult();

					COptimizedSubClassSatisfiableTestingItem* setSatisfiableTestOrdered(bool satTestOrdered);
					COptimizedSubClassSatisfiableTestingItem* setSatisfiableTested(bool satTested);
					COptimizedSubClassSatisfiableTestingItem* setResultUnsatisfiableDerivated(bool unsatDerivated);
					COptimizedSubClassSatisfiableTestingItem* setResultSatisfiableDerivated(bool satDerivated);
					COptimizedSubClassSatisfiableTestingItem* setSatisfiableTestedResult(bool satTestedResult);


					COptimizedSubClassSatisfiableTestingItem* addSubsumingConceptItem(COptimizedSubClassSatisfiableTestingItem* subsumingItem);
					CConceptSubsumerObserver* tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept);


					QList<COptimizedSubClassSatisfiableTestingItem*>* sortSubsumingConceptItemList();

					bool isEquivalentItem();
					COptimizedSubClassSatisfiableTestingItem* setEquivalentItem(bool equivalent);

					bool isPredecessorItem();
					COptimizedSubClassSatisfiableTestingItem* setPredecessorItem(bool isPredecessorOfOneItem);

					bool isMoreConceptClassificationInformationRequired();



					CCacheEntry* getFastSatisfiabilityTestedSaturationCacheEntry();
					COptimizedSubClassSatisfiableTestingItem* setFastSatisfiabilityTestedSaturationCacheEntry(CCacheEntry* cacheEntry);

					bool hasSuccessfullyFastSatisfiabilityTested();
					COptimizedSubClassSatisfiableTestingItem* setSuccessfullyFastSatisfiabilityTested(bool successfullyTested);

					virtual CIndividualDependenceTrackingMarker* setIndividualDependenceTracked();
					bool hasIndividualDependenceTracked();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mConceptSat;
					CHierarchyNode* mConSatHierNode;

					QSet<COptimizedSubClassSatisfiableTestingItem*> mSubsumingConceptItemSet;
					QList<COptimizedSubClassSatisfiableTestingItem*> mSubsumingConceptItemList;

					QList<COptimizedSubClassSatisfiableTestingItem*> mSuccList;
					cint64 mPredCounter;

					bool mSatTestOrdered;
					bool mTestedSat;
					bool mUnsatDerivated;
					bool mSatDerivated;
					bool mSatTestedResult;

					bool mEquiItem;
					bool mPredOfItem;

					//QString mSubsumingConceptString;

					CCacheEntry* mFastSatCacheEntry;
					bool mSuccFastSatTested;

					bool mIndiDepTracked;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSSATISFIABLETESTINGITEM_H
