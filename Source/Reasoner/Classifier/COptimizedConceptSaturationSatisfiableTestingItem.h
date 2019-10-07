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

#ifndef KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCONCEPTSATURATIONSATISFIABLETESTINGITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCONCEPTSATURATIONSATISFIABLETESTINGITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassificationConceptReferenceLinking.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConceptReferenceLinking.h"
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
			 *		\class		COptimizedConceptSaturationSatisfiableTestingItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedConceptSaturationSatisfiableTestingItem : public CClassificationConceptReferenceLinking {
				// public methods
				public:
					//! Constructor
					COptimizedConceptSaturationSatisfiableTestingItem();


					COptimizedConceptSaturationSatisfiableTestingItem* initConceptSatisfiableTestingItem(CConcept* satTestConcept);
					CConcept* getSatisfiableConcept();

					virtual bool isClassSatisfiableTestingItem();

					CConceptReferenceLinking* getConceptReferenceLinking();
					COptimizedConceptSaturationSatisfiableTestingItem* setConceptReferenceLinking(CConceptReferenceLinking* refLinking);


					QList<COptimizedConceptSaturationSatisfiableTestingItem*>* getExistReferenceConceptItemList();
					COptimizedConceptSaturationSatisfiableTestingItem* addExistReferenceConceptItemList(COptimizedConceptSaturationSatisfiableTestingItem* existItem);


					bool isOrderingQueued();
					COptimizedConceptSaturationSatisfiableTestingItem* setOrderingQueued(bool queued);

					bool hasIndirectSuccessorsItems();
					COptimizedConceptSaturationSatisfiableTestingItem* setIndirectSuccessorsItems(bool hasSuccessors);

					bool hasPilingReferenceItem();
					COptimizedConceptSaturationSatisfiableTestingItem* setPilingReferenceItem(bool piledReference);

					bool hasMultipleDirectPredecessorsItems();
					COptimizedConceptSaturationSatisfiableTestingItem* setMultipleDirectPredecessorsItems(bool multiblePredItems);


					// for piling saturation algorithm
					enum SATURATIONITEMREFERENCESPECIALMODE { NONESATURATIONMODE, SATURATIONCOPYMODE, SATURATIONPILINGMODE, SATURATIONSUBSTITUTEMODE };

					COptimizedConceptSaturationSatisfiableTestingItem* getSpecialItemReference();
					COptimizedConceptSaturationSatisfiableTestingItem* setSpecialItemReference(COptimizedConceptSaturationSatisfiableTestingItem* reference);
					SATURATIONITEMREFERENCESPECIALMODE getSpecialReferenceMode();
					COptimizedConceptSaturationSatisfiableTestingItem* setSpecialItemReferenceMode(SATURATIONITEMREFERENCESPECIALMODE mode);


				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mConceptSat;

					QList<COptimizedConceptSaturationSatisfiableTestingItem*> mExistReferenceItemList;

					bool mOrderingQueued;
					bool mPilingReference;
					bool mSucessorsItems;
					bool mMultiplePredItems;

					CConceptReferenceLinking* mConRefLinking;

					COptimizedConceptSaturationSatisfiableTestingItem* mSpecialItemReference;
					SATURATIONITEMREFERENCESPECIALMODE mSpecialReferenceMode;

					//QString mSubsumingConceptString;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_COPTIMIZEDCONCEPTSATURATIONSATISFIABLETESTINGITEM_H
