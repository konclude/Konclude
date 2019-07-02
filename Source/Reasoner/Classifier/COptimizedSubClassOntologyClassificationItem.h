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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSONTOLOGYCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSONTOLOGYCLASSIFICATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "COntologyClassificationItem.h"
#include "COptimizedSubClassSatisfiableTestingItem.h"


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
			 *		\class		COptimizedSubClassOntologyClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedSubClassOntologyClassificationItem : public COntologyClassificationItem {
				// public methods
				public:
					//! Constructor
					COptimizedSubClassOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics = 0);

					//! Destructor
					virtual ~COptimizedSubClassOntologyClassificationItem();

					QHash<CConcept*,COptimizedSubClassSatisfiableTestingItem*>* getConceptSatisfiableTestItemHash();
					QList<COptimizedSubClassSatisfiableTestingItem*>* getConceptSatisfiableTestItemContainer();
					COptimizedSubClassSatisfiableTestingItem* getTopConceptSatisfiableTestItem();
					COptimizedSubClassSatisfiableTestingItem* getBottomConceptSatisfiableTestItem();

					QList<COptimizedSubClassSatisfiableTestingItem*>* getNextSatisfiableTestingItemList();
					QSet<COptimizedSubClassSatisfiableTestingItem*>* getNextCandidateSatisfiableTestingItemSet();
					QSet<COptimizedSubClassSatisfiableTestingItem*>* getRemainingCandidateSatisfiableTestingItemSet();


					COptimizedSubClassOntologyClassificationItem* initTopBottomSatisfiableTestingItems(COptimizedSubClassSatisfiableTestingItem* topItem, COptimizedSubClassSatisfiableTestingItem* bottomItem);


					COptimizedSubClassSatisfiableTestingItem* getConceptSatisfiableTestItem(CConcept* satTestConcept, bool create = false);


					cint64 getRemainingSatisfiableTestsCount();
					cint64 getRunningSatisfiableTestsCount();

					COptimizedSubClassOntologyClassificationItem* incRemainingSatisfiableTestsCount(cint64 incCount = 1);
					COptimizedSubClassOntologyClassificationItem* incRunningSatisfiableTestsCount(cint64 incCount = 1);
					COptimizedSubClassOntologyClassificationItem* decRemainingSatisfiableTestsCount(cint64 decCount = 1);
					COptimizedSubClassOntologyClassificationItem* decRunningSatisfiableTestsCount(cint64 decCount = 1);

					bool hasAllSatisfiableTestsCompleted();
					bool hasRemainingSatisfiableTests();


					QList<COptimizedSubClassSatisfiableTestingItem*>* getSatisfiableConceptItemList();
					COptimizedSubClassOntologyClassificationItem* addSatisfiableConceptItem(COptimizedSubClassSatisfiableTestingItem* item);

					QHash<CConcept*,CConceptReferenceLinking*>* getConceptReferenceLinkingDataHash();

				// protected methods
				protected:

				// protected variables
				protected:
					QHash<CConcept*,COptimizedSubClassSatisfiableTestingItem*> mConceptSatItemHash;
					QList<COptimizedSubClassSatisfiableTestingItem*> mSatTestItemContainer;
					COptimizedSubClassSatisfiableTestingItem* mTopSatTestItem;
					COptimizedSubClassSatisfiableTestingItem* mBottomSatTestItem;

					QList<COptimizedSubClassSatisfiableTestingItem*> mNextItemList;
					QSet<COptimizedSubClassSatisfiableTestingItem*> mNextCandItemSet;
					QSet<COptimizedSubClassSatisfiableTestingItem*> mRemainingCandItemSet;
					QHash<CConcept*,CConceptReferenceLinking*> mConceptRefLinkData;

					cint64 mRemainingSatTests;
					cint64 mRunningSatTests;

					QList<COptimizedSubClassSatisfiableTestingItem*> mSatisfiableItemList;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDSUBCLASSONTOLOGYCLASSIFICATIONITEM_H
