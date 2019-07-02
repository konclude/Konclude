/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSONTOLOGYCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSONTOLOGYCLASSIFICATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "COntologyClassificationItem.h"
#include "COptimizedKPSetClassTestingItem.h"


// Other includes
#include "Reasoner/Ontology/CConceptReferenceLinking.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Classifier {


			/*! 
			 *
			 *		\class		COptimizedKPSetClassOntologyClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetClassOntologyClassificationItem : public COntologyClassificationItem {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetClassOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics = 0);

					//! Destructor
					virtual ~COptimizedKPSetClassOntologyClassificationItem();

					QHash<CConcept*,COptimizedKPSetClassTestingItem*>* getConceptSatisfiableTestItemHash();
					QList<COptimizedKPSetClassTestingItem*>* getConceptSatisfiableTestItemContainer();
					COptimizedKPSetClassTestingItem* getTopConceptSatisfiableTestItem();
					COptimizedKPSetClassTestingItem* getBottomConceptSatisfiableTestItem();

					QList<COptimizedKPSetClassTestingItem*>* getNextSatisfiableTestingItemList();
					QSet<COptimizedKPSetClassTestingItem*>* getNextCandidateSatisfiableTestingItemSet();
					QSet<COptimizedKPSetClassTestingItem*>* getRemainingCandidateSatisfiableTestingItemSet();

					QList<COptimizedKPSetClassTestingItem*>* getNextPossibleSubsumptionTestingItemList();
					QSet<COptimizedKPSetClassTestingItem*>* getCurrentPossibleSubsumptionTestingItemSet();


					COptimizedKPSetClassOntologyClassificationItem* initTopBottomSatisfiableTestingItems(COptimizedKPSetClassTestingItem* topItem, COptimizedKPSetClassTestingItem* bottomItem);


					COptimizedKPSetClassTestingItem* getConceptSatisfiableTestItem(CConcept* satTestConcept, bool create = false);


					cint64 getRemainingSatisfiableTestsCount();
					cint64 getRunningSatisfiableTestsCount();

					COptimizedKPSetClassOntologyClassificationItem* incRemainingSatisfiableTestsCount(cint64 incCount = 1);
					COptimizedKPSetClassOntologyClassificationItem* incRunningSatisfiableTestsCount(cint64 incCount = 1);
					COptimizedKPSetClassOntologyClassificationItem* decRemainingSatisfiableTestsCount(cint64 decCount = 1);
					COptimizedKPSetClassOntologyClassificationItem* decRunningSatisfiableTestsCount(cint64 decCount = 1);

					bool hasAllSatisfiableTestsCompleted();
					bool hasRemainingSatisfiableTests();


					QList<COptimizedKPSetClassTestingItem*>* getSatisfiableConceptItemList();
					COptimizedKPSetClassOntologyClassificationItem* addSatisfiableConceptItem(COptimizedKPSetClassTestingItem* item);

					QHash<CConcept*,CConceptReferenceLinking*>* getConceptReferenceLinkingDataHash();

					bool hasSatisfiableTestingPhaseFinished();
					bool hasPossibleSubsumptionTestingPhaseFinished();

					COptimizedKPSetClassOntologyClassificationItem* setSatisfiableTestingPhaseFinished(bool finished);
					COptimizedKPSetClassOntologyClassificationItem* setPossibleSubsumptionTestingPhaseFinished(bool finished);

					cint64 hasRemainingPossibleSubsumptionTests();

					cint64 getRemainingPossibleSubsumptionTestsCount();
					COptimizedKPSetClassOntologyClassificationItem* setRemainingPossibleSubsumptionTestsCount(cint64 testCount);
					COptimizedKPSetClassOntologyClassificationItem* incRemainingPossibleSubsumptionTestsCount(cint64 incCount = 1);
					COptimizedKPSetClassOntologyClassificationItem* decRemainingPossibleSubsumptionTestsCount(cint64 decCount = 1);

					COptimizedKPSetClassOntologyClassificationItem* incRunningPossibleSubsumptionTestsCount(cint64 incCount = 1);
					COptimizedKPSetClassOntologyClassificationItem* decRunningPossibleSubsumptionTestsCount(cint64 decCount = 1);

					QSet<COptimizedKPSetClassTestingItem*>* getRemainingPossibleSubsumptionClassTestingSet();
					cint64 getRemainingPossibleSubsumptionClassTestingCount();

					QSet<CConcept*>* getEquivaltentConceptNonCandidateSet();
					CBOXHASH<CConcept*,CConcept*>* getEquivalentConceptCandidateHash();

					COptimizedKPSetClassOntologyClassificationItem* addMemoryPools(CMemoryPool* memoryPools);
					CMemoryPool* takeMemoryPools();

					cint64 getCalculatedPossibleSubsumerCount();
					cint64 getCalculatedTruePossibleSubsumerCount();
					cint64 getCalculatedFalsePossibleSubsumerCount();

					COptimizedKPSetClassOntologyClassificationItem* setCalculatedPossibleSubsumerCount(cint64 subsumCount);
					COptimizedKPSetClassOntologyClassificationItem* setCalculatedTruePossibleSubsumerCount(cint64 subsumCount);
					COptimizedKPSetClassOntologyClassificationItem* setCalculatedFalsePossibleSubsumerCount(cint64 subsumCount);

					COptimizedKPSetClassOntologyClassificationItem* incCalculatedPossibleSubsumerCount(cint64 incSubsumCount = 1);
					COptimizedKPSetClassOntologyClassificationItem* incCalculatedTruePossibleSubsumerCount(cint64 incSubsumCount = 1);
					COptimizedKPSetClassOntologyClassificationItem* incCalculatedFalsePossibleSubsumerCount(cint64 incSubsumCount = 1);


					cint64 getPossibleSubsumerCount();
					cint64 getTruePossibleSubsumerCount();
					cint64 getFalsePossibleSubsumerCount();

					COptimizedKPSetClassOntologyClassificationItem* setPossibleSubsumerCount(cint64 subsumCount);
					COptimizedKPSetClassOntologyClassificationItem* setTruePossibleSubsumerCount(cint64 subsumCount);
					COptimizedKPSetClassOntologyClassificationItem* setFalsePossibleSubsumerCount(cint64 subsumCount);

					COptimizedKPSetClassOntologyClassificationItem* incPossibleSubsumerCount(cint64 incSubsumCount = 1);
					COptimizedKPSetClassOntologyClassificationItem* incTruePossibleSubsumerCount(cint64 incSubsumCount = 1);
					COptimizedKPSetClassOntologyClassificationItem* incFalsePossibleSubsumerCount(cint64 incSubsumCount = 1);

				// protected methods
				protected:

				// protected variables
				protected:
					QHash<CConcept*,COptimizedKPSetClassTestingItem*> mConceptSatItemHash;
					QList<COptimizedKPSetClassTestingItem*> mSatTestItemContainer;
					COptimizedKPSetClassTestingItem* mTopSatTestItem;
					COptimizedKPSetClassTestingItem* mBottomSatTestItem;

					QList<COptimizedKPSetClassTestingItem*> mNextItemList;
					QSet<COptimizedKPSetClassTestingItem*> mNextCandItemSet;
					QSet<COptimizedKPSetClassTestingItem*> mRemainingCandItemSet;

					QList<COptimizedKPSetClassTestingItem*> mNextPossSubsumItemList;
					QSet<COptimizedKPSetClassTestingItem*> mCurrentPossSubsumItemSet;

					QHash<CConcept*,CConceptReferenceLinking*> mConceptRefLinkData;

					cint64 mRemainingSatTests;
					cint64 mRunningSatTests;

					bool mSatisfiableTestingPhaseFinished;
					bool mPossibleSubsumptionTestingPhaseFinished;

					cint64 mRemainingPossibleSubsumptionTests;
					cint64 mRunningPossibleSubsumptionTests;

					cint64 mCalculatedPossibleSubsumCount;
					cint64 mCalculatedTruePossibleSubsumCount;
					cint64 mCalculatedFalsePossibleSubsumCount;

					cint64 mPossibleSubsumCount;
					cint64 mTruePossibleSubsumCount;
					cint64 mFalsePossibleSubsumCount;

					QSet<COptimizedKPSetClassTestingItem*> mRemPossClassTestingSet;

					QList<COptimizedKPSetClassTestingItem*> mSatisfiableItemList;

					QSet<CConcept*> mEquivConNonCandidateSet;
					CBOXHASH<CConcept*,CConcept*> mEquivConCandidateHash;

					CMemoryPoolContainer mMemoryPoolContainer;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETCLASSONTOLOGYCLASSIFICATIONITEM_H
