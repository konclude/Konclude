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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSTESTINGITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSTESTINGITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "COptimizedKPSetClassPossibleSubsumptionMap.h"
#include "CClassificationSatisfiablePossibleSubsumptionCalculationConceptReferenceLinking.h"
#include "CClassificationClassPseudoModel.h"
#include "CConceptSubsumerObserver.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
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
			 *		\class		COptimizedKPSetClassTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetClassTestingItem : public CClassificationSatisfiablePossibleSubsumptionCalculationConceptReferenceLinking, public CConceptSubsumerObserver {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetClassTestingItem();
					~COptimizedKPSetClassTestingItem();


					COptimizedKPSetClassTestingItem* initSatisfiableTestingItem(CConcept* satTestConcept);
					CConcept* getTestingConcept();

					CHierarchyNode* getSatisfiableConceptHierarchyNode();
					COptimizedKPSetClassTestingItem* setSatisfiableConceptHierarchyNode(CHierarchyNode* hierNode);

					QSet<COptimizedKPSetClassTestingItem*>* getSubsumingConceptItemSet();
					QList<COptimizedKPSetClassTestingItem*>* getSubsumingConceptItemList();
					cint64 getSubsumingConceptItemCount() const;
					bool hasSubsumerConceptItem(COptimizedKPSetClassTestingItem* item);

					QList<COptimizedKPSetClassTestingItem*>* getSuccessorItemList();
					cint64 getUnprocessedPredecessorItemCount();

					bool hasOnlyProcessedPredecessorItems();
					COptimizedKPSetClassTestingItem* decUnprocessedPredecessorItems(cint64 decCount = 1);
					COptimizedKPSetClassTestingItem* incUnprocessedPredecessorItems(cint64 incCount = 1);
					COptimizedKPSetClassTestingItem* setUnprocessedPredecessorItems(cint64 predCount);

					COptimizedKPSetClassTestingItem* addSuccessorSatisfiableTestItem(COptimizedKPSetClassTestingItem* succItem);

					bool isSatisfiableTestOrdered();
					bool isSatisfiableTested();
					bool isResultUnsatisfiableDerivated();
					bool isResultSatisfiableDerivated();
					bool getSatisfiableTestedResult();

					COptimizedKPSetClassTestingItem* setSatisfiableTestOrdered(bool satTestOrdered);
					COptimizedKPSetClassTestingItem* setSatisfiableTested(bool satTested);
					COptimizedKPSetClassTestingItem* setResultUnsatisfiableDerivated(bool unsatDerivated);
					COptimizedKPSetClassTestingItem* setResultSatisfiableDerivated(bool satDerivated);
					COptimizedKPSetClassTestingItem* setSatisfiableTestedResult(bool satTestedResult);


					COptimizedKPSetClassTestingItem* addSubsumingConceptItem(COptimizedKPSetClassTestingItem* subsumingItem);
					CConceptSubsumerObserver* tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept);


					QList<COptimizedKPSetClassTestingItem*>* sortSubsumingConceptItemList();

					bool isEquivalentItem();
					COptimizedKPSetClassTestingItem* setEquivalentItem(bool equivalent);

					bool isPredecessorItem();
					COptimizedKPSetClassTestingItem* setPredecessorItem(bool isPredecessorOfOneItem);

					bool isMoreConceptClassificationInformationRequired();

					COptimizedKPSetClassPossibleSubsumptionMap* getPossibleSubsumptionMap(bool create = false);


					QSet<COptimizedKPSetClassTestingItem*>* getUpPropagationItemSet();
					QSet<COptimizedKPSetClassTestingItem*>* getDownPropagationItemSet();
					COptimizedKPSetClassTestingItem* addUpPropagationItem(COptimizedKPSetClassTestingItem* item);
					COptimizedKPSetClassTestingItem* addDownPropagationItem(COptimizedKPSetClassTestingItem* item);
					bool isPropagationConnected();
					COptimizedKPSetClassTestingItem* setPropagationConnected(bool connected);


					virtual CClassPossibleSubsumptionMap* getClassPossibleSubsumptionMap();
					bool isPossibleSubsumptionMapInitialized();
					COptimizedKPSetClassTestingItem* setPossibleSubsumptionMapInitialized(bool initialized);

					CClassificationClassPseudoModel* getClassPseudoModel();
					bool isClassPseudoModelInitalized();
					COptimizedKPSetClassTestingItem* setClassPseudoModelInitalized(bool initialized);



					QSet<COptimizedKPSetClassTestingItem*>* getPossibleSubsumedSet(bool create);
					QList<COptimizedKPSetClassTestingItem*>* getPossibleSubsumedList();
					COptimizedKPSetClassTestingItem* setPossibleSubsumedList(QList<COptimizedKPSetClassTestingItem*>* possSubsumedList);
					bool hasRemainingPossibleSubsumedItems();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mConceptSat;
					CHierarchyNode* mConSatHierNode;

					QSet<COptimizedKPSetClassTestingItem*> mSubsumingConceptItemSet;
					QList<COptimizedKPSetClassTestingItem*> mSubsumingConceptItemList;

					QList<COptimizedKPSetClassTestingItem*> mSuccList;
					cint64 mPredCounter;

					// true, if the satisfiability testing for this concept has started
					bool mSatTestOrdered;
					// true, if the satisfiability testing has finished
					bool mTestedSat;
					// true, if for a subsumer concept the unsatisfiability has been discovered
					bool mUnsatDerivated;
					// true, if for the concept the satisfiability has been extracted from a completion graph
					bool mSatDerivated;
					// true of false if the concept is satisfiable respectively unsatisfiable
					bool mSatTestedResult;

					bool mPropagationConnected;
					QSet<COptimizedKPSetClassTestingItem*> mUpPropagationItemSet;
					QSet<COptimizedKPSetClassTestingItem*> mDownPropagationItemSet;


					QSet<COptimizedKPSetClassTestingItem*>* mPossibleSubsumedSet;
					QList<COptimizedKPSetClassTestingItem*>* mPossibleSubsumedList;


					bool mEquiItem;
					bool mPredOfItem;

					COptimizedKPSetClassPossibleSubsumptionMap* mPossibleSubsumptionMap;
					bool mPossSubsumMapInitialized;

					//QString mSubsumingConceptString;
					CClassificationClassPseudoModel mPseudoModel;
					bool mPseudoModelInitialized;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSTESTINGITEM_H
