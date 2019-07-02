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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSSATURATIONSATISFIABLETESTINGITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSSATURATIONSATISFIABLETESTINGITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CConceptSubsumerObserver.h"
#include "COptimizedConceptSaturationSatisfiableTestingItem.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptReferenceLinking.h"
#include "Reasoner/Ontology/CConceptSatisfiableReferenceLinkingData.h"
#include "Reasoner/Taxonomy/CHierarchyNode.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COptimizedClassSaturationSatisfiableTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedClassSaturationSatisfiableTestingItem : public COptimizedConceptSaturationSatisfiableTestingItem, public CConceptSubsumerObserver {
				// public methods
				public:
					//! Constructor
					COptimizedClassSaturationSatisfiableTestingItem();

					virtual bool isClassSatisfiableTestingItem();

					COptimizedClassSaturationSatisfiableTestingItem* initSatisfiableTestingItem(CConcept* satTestConcept);

					CHierarchyNode* getSatisfiableConceptHierarchyNode();
					COptimizedClassSaturationSatisfiableTestingItem* setSatisfiableConceptHierarchyNode(CHierarchyNode* hierNode);

					QSet<COptimizedClassSaturationSatisfiableTestingItem*>* getSubsumingConceptItemSet();
					QList<COptimizedClassSaturationSatisfiableTestingItem*>* getSubsumingConceptItemList();
					cint64 getSubsumingConceptItemCount() const;

					QList<COptimizedConceptSaturationSatisfiableTestingItem*>* getSuccessorItemList();
					cint64 getUnprocessedPredecessorItemCount();

					bool hasOnlyProcessedPredecessorItems();
					COptimizedClassSaturationSatisfiableTestingItem* decUnprocessedPredecessorItems(cint64 decCount = 1);
					COptimizedClassSaturationSatisfiableTestingItem* incUnprocessedPredecessorItems(cint64 incCount = 1);

					COptimizedClassSaturationSatisfiableTestingItem* addSuccessorSatisfiableTestItem(COptimizedConceptSaturationSatisfiableTestingItem* succItem);

					bool isSatisfiableTestOrdered();
					bool isSatisfiableTested();
					bool getSatisfiableTestedResult();

					COptimizedClassSaturationSatisfiableTestingItem* setSatisfiableTestOrdered(bool satTestOrdered);
					COptimizedClassSaturationSatisfiableTestingItem* setSatisfiableTested(bool satTested);
					COptimizedClassSaturationSatisfiableTestingItem* setSatisfiableTestedResult(bool satTestedResult);


					COptimizedClassSaturationSatisfiableTestingItem* addSubsumingConceptItem(COptimizedClassSaturationSatisfiableTestingItem* subsumingItem);

					CConceptSubsumerObserver* tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept);


					QList<COptimizedClassSaturationSatisfiableTestingItem*>* sortSubsumingConceptItemList();

					bool isEquivalentItem();
					COptimizedClassSaturationSatisfiableTestingItem* setEquivalentItem(bool equivalent);

					bool isPredecessorItem();
					COptimizedClassSaturationSatisfiableTestingItem* setPredecessorItem(bool isPredecessorOfOneItem);



				// protected methods
				protected:

				// protected variables
				protected:
					CHierarchyNode* mConSatHierNode;

					QSet<COptimizedClassSaturationSatisfiableTestingItem*> mSubsumingConceptItemSet;
					QList<COptimizedClassSaturationSatisfiableTestingItem*> mSubsumingConceptItemList;

					QList<COptimizedConceptSaturationSatisfiableTestingItem*> mSuccList;
					cint64 mPredCounter;

					bool mSatTestOrdered;
					bool mTestedSat;
					bool mSatTestedResult;

					bool mEquiItem;
					bool mPredOfItem;

					//QString mSubsumingConceptString;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCLASSSATURATIONSATISFIABLETESTINGITEM_H
