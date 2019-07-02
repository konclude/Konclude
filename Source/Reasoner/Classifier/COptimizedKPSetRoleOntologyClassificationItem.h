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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETROLEONTOLOGYCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETROLEONTOLOGYCLASSIFICATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "COntologyPropertyRoleClassificationItem.h"
#include "COptimizedKPSetRoleTestingItem.h"


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
			 *		\class		COptimizedKPSetRoleOntologyClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedKPSetRoleOntologyClassificationItem : public COntologyPropertyRoleClassificationItem {
				// public methods
				public:
					//! Constructor
					COptimizedKPSetRoleOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics = 0);

					//! Destructor
					virtual ~COptimizedKPSetRoleOntologyClassificationItem();

					QHash<CRole*,COptimizedKPSetRoleTestingItem*>* getRoleSatisfiableTestItemHash();
					QList<COptimizedKPSetRoleTestingItem*>* getRoleSatisfiableTestItemList();
					COptimizedKPSetRoleTestingItem* getTopRoleSatisfiableTestItem();
					COptimizedKPSetRoleTestingItem* getBottomRoleSatisfiableTestItem();


					COptimizedKPSetRoleOntologyClassificationItem* setTopRoleSatisfiableTestItem(COptimizedKPSetRoleTestingItem* item);
					COptimizedKPSetRoleOntologyClassificationItem* setBottomRoleSatisfiableTestItem(COptimizedKPSetRoleTestingItem* item);


					QList<COptimizedKPSetRoleTestingItem*>* getNextSatisfiableTestingItemList();
					QSet<COptimizedKPSetRoleTestingItem*>* getNextCandidateSatisfiableTestingItemSet();
					QSet<COptimizedKPSetRoleTestingItem*>* getRemainingCandidateSatisfiableTestingItemSet();

					QList<COptimizedKPSetRoleTestingItem*>* getNextPossibleSubsumptionTestingItemList();
					QSet<COptimizedKPSetRoleTestingItem*>* getCurrentPossibleSubsumptionTestingItemSet();


					COptimizedKPSetRoleOntologyClassificationItem* initTopBottomSatisfiableTestingItems(COptimizedKPSetRoleTestingItem* topItem, COptimizedKPSetRoleTestingItem* bottomItem);


					COptimizedKPSetRoleTestingItem* getRoleSatisfiableTestItem(CRole* satTestRole, bool create = false);


					cint64 getRemainingSatisfiableTestsCount();
					cint64 getRunningSatisfiableTestsCount();

					COptimizedKPSetRoleOntologyClassificationItem* incRemainingSatisfiableTestsCount(cint64 incCount = 1);
					COptimizedKPSetRoleOntologyClassificationItem* incRunningSatisfiableTestsCount(cint64 incCount = 1);
					COptimizedKPSetRoleOntologyClassificationItem* decRemainingSatisfiableTestsCount(cint64 decCount = 1);
					COptimizedKPSetRoleOntologyClassificationItem* decRunningSatisfiableTestsCount(cint64 decCount = 1);

					bool hasAllSatisfiableTestsCompleted();
					bool hasRemainingSatisfiableTests();


					QList<COptimizedKPSetRoleTestingItem*>* getSatisfiableRoleItemList();
					COptimizedKPSetRoleOntologyClassificationItem* addSatisfiableRoleItem(COptimizedKPSetRoleTestingItem* item);

					QHash<CConcept*,CClassificationSatisfiableCalculationConceptReferenceLinking*>* getConceptReferenceLinkingDataHash();

					bool hasSatisfiableTestingPhaseFinished();
					bool hasPossibleSubsumptionTestingPhaseFinished();

					COptimizedKPSetRoleOntologyClassificationItem* setSatisfiableTestingPhaseFinished(bool finished);
					COptimizedKPSetRoleOntologyClassificationItem* setPossibleSubsumptionTestingPhaseFinished(bool finished);

					cint64 hasRemainingPossibleSubsumptionTests();

					cint64 getRemainingPossibleSubsumptionTestsCount();
					COptimizedKPSetRoleOntologyClassificationItem* setRemainingPossibleSubsumptionTestsCount(cint64 testCount);
					COptimizedKPSetRoleOntologyClassificationItem* incRemainingPossibleSubsumptionTestsCount(cint64 incCount = 1);
					COptimizedKPSetRoleOntologyClassificationItem* decRemainingPossibleSubsumptionTestsCount(cint64 decCount = 1);

					COptimizedKPSetRoleOntologyClassificationItem* incRunningPossibleSubsumptionTestsCount(cint64 incCount = 1);
					COptimizedKPSetRoleOntologyClassificationItem* decRunningPossibleSubsumptionTestsCount(cint64 decCount = 1);

					QSet<COptimizedKPSetRoleTestingItem*>* getRemainingPossibleSubsumptionTestingSet();
					cint64 getRemainingPossibleSubsumptionTestingCount();


					COptimizedKPSetRoleOntologyClassificationItem* addMemoryPools(CMemoryPool* memoryPools);
					CMemoryPool* takeMemoryPools();

					cint64 getCalculatedPossibleSubsumerCount();
					cint64 getCalculatedTruePossibleSubsumerCount();
					cint64 getCalculatedFalsePossibleSubsumerCount();

					COptimizedKPSetRoleOntologyClassificationItem* setCalculatedPossibleSubsumerCount(cint64 subsumCount);
					COptimizedKPSetRoleOntologyClassificationItem* setCalculatedTruePossibleSubsumerCount(cint64 subsumCount);
					COptimizedKPSetRoleOntologyClassificationItem* setCalculatedFalsePossibleSubsumerCount(cint64 subsumCount);

					COptimizedKPSetRoleOntologyClassificationItem* incCalculatedPossibleSubsumerCount(cint64 incSubsumCount = 1);
					COptimizedKPSetRoleOntologyClassificationItem* incCalculatedTruePossibleSubsumerCount(cint64 incSubsumCount = 1);
					COptimizedKPSetRoleOntologyClassificationItem* incCalculatedFalsePossibleSubsumerCount(cint64 incSubsumCount = 1);


					cint64 getPossibleSubsumerCount();
					cint64 getTruePossibleSubsumerCount();
					cint64 getFalsePossibleSubsumerCount();

					COptimizedKPSetRoleOntologyClassificationItem* setPossibleSubsumerCount(cint64 subsumCount);
					COptimizedKPSetRoleOntologyClassificationItem* setTruePossibleSubsumerCount(cint64 subsumCount);
					COptimizedKPSetRoleOntologyClassificationItem* setFalsePossibleSubsumerCount(cint64 subsumCount);

					COptimizedKPSetRoleOntologyClassificationItem* incPossibleSubsumerCount(cint64 incSubsumCount = 1);
					COptimizedKPSetRoleOntologyClassificationItem* incTruePossibleSubsumerCount(cint64 incSubsumCount = 1);
					COptimizedKPSetRoleOntologyClassificationItem* incFalsePossibleSubsumerCount(cint64 incSubsumCount = 1);
					



					CConcreteOntology* getTemporaryRoleClassificationOntology();
					COptimizedKPSetRoleOntologyClassificationItem* setTemporaryRoleClassificationOntology(CConcreteOntology* ont);


					QHash<CConcept*,COptimizedKPSetRoleTestingItem*>* getMarkerConceptInstancesItemHash();




					CConcept* getTemporaryAllPropagationConcept();
					COptimizedKPSetRoleOntologyClassificationItem* setTemporaryAllPropagationConcept(CConcept* concept);


					CIndividual* getTemporaryPropagationIndividual();
					CIndividual* getTemporaryMarkerIndividual();
					COptimizedKPSetRoleOntologyClassificationItem* setTemporaryPropagationIndividual(CIndividual* indi);
					COptimizedKPSetRoleOntologyClassificationItem* setTemporaryMarkerIndividual(CIndividual* indi);


				// protected methods
				protected:

				// protected variables
				protected:
					QHash<CRole*,COptimizedKPSetRoleTestingItem*> mRoleSatItemHash;
					QList<COptimizedKPSetRoleTestingItem*> mSatTestItemContainer;
					COptimizedKPSetRoleTestingItem* mTopSatTestItem;
					COptimizedKPSetRoleTestingItem* mBottomSatTestItem;

					QList<COptimizedKPSetRoleTestingItem*> mNextItemList;
					QSet<COptimizedKPSetRoleTestingItem*> mNextCandItemSet;
					QSet<COptimizedKPSetRoleTestingItem*> mRemainingCandItemSet;

					QList<COptimizedKPSetRoleTestingItem*> mNextPossSubsumItemList;
					QSet<COptimizedKPSetRoleTestingItem*> mCurrentPossSubsumItemSet;

					QHash<CConcept*,CClassificationSatisfiableCalculationConceptReferenceLinking*> mConceptRefLinkData;

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

					QSet<COptimizedKPSetRoleTestingItem*> mRemPossClassTestingSet;

					QList<COptimizedKPSetRoleTestingItem*> mSatisfiableItemList;

					CMemoryPoolContainer mMemoryPoolContainer;

					CConcreteOntology* mTmpRoleClassificationOnt;
					QHash<CConcept*,COptimizedKPSetRoleTestingItem*> mMarkerConRolInsItemHash;

					CConcept* mTmpAllPropConcept;
					CIndividual* mTmpIndiPropagation;
					CIndividual* mTmpIndiMarker;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDKPSETROLEONTOLOGYCLASSIFICATIONITEM_H
