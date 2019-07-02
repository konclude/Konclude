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

#ifndef KONCLUDE_REASONER_CONSISTISER_CSATURATIONCONCEPTDATAITEM_H
#define KONCLUDE_REASONER_CONSISTISER_CSATURATIONCONCEPTDATAITEM_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"
#include "CSaturationConceptReferenceLinking.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConceptReferenceLinking.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Consistiser {



			/*! 
			 *
			 *		\class		CSaturationConceptDataItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSaturationConceptDataItem : public CSaturationConceptReferenceLinking {
				// public methods
				public:
					//! Constructor
					CSaturationConceptDataItem();


					CSaturationConceptDataItem* initConceptSaturationTestingItem(CConcept* satTestConcept, bool negated, CRole* role);
					CConcept* getSaturationConcept();
					CRole* getSaturationRoleRanges();
					bool getSaturationNegation();


					CConceptReferenceLinking* getConceptReferenceLinking();
					CSaturationConceptDataItem* setConceptReferenceLinking(CConceptReferenceLinking* refLinking);


					QList<CSaturationConceptDataItem*>* getExistReferenceConceptItemList();
					CSaturationConceptDataItem* addExistReferenceConceptItemList(CSaturationConceptDataItem* existItem);


					bool isOrderingQueued();
					CSaturationConceptDataItem* setOrderingQueued(bool queued);

					bool hasIndirectSuccessorsItems();
					CSaturationConceptDataItem* setIndirectSuccessorsItems(bool hasSuccessors);

					bool hasMultipleDirectPredecessorsItems();
					CSaturationConceptDataItem* setMultipleDirectPredecessorsItems(bool multiblePredItems);


					// for saturation algorithm
					enum SATURATIONITEMREFERENCESPECIALMODE { NONESATURATIONMODE, SATURATIONCOPYMODE, SATURATIONSUBSTITUTEMODE };


					bool isInvalidSpecialItemReference();
					CSaturationConceptDataItem* setInvalidSpecialItemReference(bool invalid);

					CSaturationConceptDataItem* getSpecialItemReference();
					CSaturationConceptDataItem* setSpecialItemReference(CSaturationConceptDataItem* reference);
					SATURATIONITEMREFERENCESPECIALMODE getSpecialReferenceMode();
					CSaturationConceptDataItem* setSpecialItemReferenceMode(SATURATIONITEMREFERENCESPECIALMODE mode);


					bool isItemProcessingQueued();
					CSaturationConceptDataItem* setItemProcessingQueued(bool queued);
					bool isItemProcessingMarked();
					CSaturationConceptDataItem* setItemProcessingMarked(bool marked);

				// protected methods
				protected:

				// protected variables
				protected:
					CConcept* mConceptSat;
					CRole* mRoleRanges;
					bool mNegation;

					QList<CSaturationConceptDataItem*> mExistReferenceItemList;

					bool mOrderingQueued;
					bool mSucessorsItems;
					bool mMultiplePredItems;
					bool mInvalidSpecialItemReference;
					bool mProcessingQueued;
					bool mProcessingMarked;

					CConceptReferenceLinking* mConRefLinking;

					CSaturationConceptDataItem* mSpecialItemReference;
					SATURATIONITEMREFERENCESPECIALMODE mSpecialReferenceMode;

					//QString mSubsumingConceptString;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CSATURATIONCONCEPTDATAITEM_H
