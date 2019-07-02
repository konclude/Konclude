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

#include "COptimizedConceptSaturationSatisfiableTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedConceptSaturationSatisfiableTestingItem::COptimizedConceptSaturationSatisfiableTestingItem() {
			}


			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::initConceptSatisfiableTestingItem(CConcept* satTestConcept) {
				mConceptSat = satTestConcept;
				mConRefLinking = nullptr;
				mSpecialItemReference = nullptr;
				mSpecialReferenceMode = NONESATURATIONMODE;
				mOrderingQueued = false;
				mSucessorsItems = false;
				mPilingReference = false;
				mMultiplePredItems = false;
				return this;
			}

			CConcept* COptimizedConceptSaturationSatisfiableTestingItem::getSatisfiableConcept() {
				return mConceptSat;
			}


			bool COptimizedConceptSaturationSatisfiableTestingItem::isClassSatisfiableTestingItem() {
				return false;
			}


			CConceptReferenceLinking* COptimizedConceptSaturationSatisfiableTestingItem::getConceptReferenceLinking() {
				return mConRefLinking;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::setConceptReferenceLinking(CConceptReferenceLinking* refLinking) {
				mConRefLinking = refLinking;
				return this;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::getSpecialItemReference() {
				return mSpecialItemReference;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::setSpecialItemReference(COptimizedConceptSaturationSatisfiableTestingItem* reference) {
				mSpecialItemReference = reference;
				return this;
			}

			COptimizedConceptSaturationSatisfiableTestingItem::SATURATIONITEMREFERENCESPECIALMODE COptimizedConceptSaturationSatisfiableTestingItem::getSpecialReferenceMode() {
				return mSpecialReferenceMode;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::setSpecialItemReferenceMode(COptimizedConceptSaturationSatisfiableTestingItem::SATURATIONITEMREFERENCESPECIALMODE mode) {
				mSpecialReferenceMode = mode;
				return this;
			}

			bool COptimizedConceptSaturationSatisfiableTestingItem::isOrderingQueued() {
				return mOrderingQueued;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::setOrderingQueued(bool queued) {
				mOrderingQueued = queued;
				return this;
			}

			bool COptimizedConceptSaturationSatisfiableTestingItem::hasIndirectSuccessorsItems() {
				return mSucessorsItems;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::setIndirectSuccessorsItems(bool hasSuccessors) {
				mSucessorsItems = hasSuccessors;
				return this;
			}

			bool COptimizedConceptSaturationSatisfiableTestingItem::hasPilingReferenceItem() {
				return mPilingReference;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::setPilingReferenceItem(bool piledReference) {
				mPilingReference = piledReference;
				return this;
			}



			QList<COptimizedConceptSaturationSatisfiableTestingItem*>* COptimizedConceptSaturationSatisfiableTestingItem::getExistReferenceConceptItemList() {
				return &mExistReferenceItemList;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::addExistReferenceConceptItemList(COptimizedConceptSaturationSatisfiableTestingItem* existItem) {
				mExistReferenceItemList.append(existItem);
				return this;
			}

			bool COptimizedConceptSaturationSatisfiableTestingItem::hasMultipleDirectPredecessorsItems() {
				return mMultiplePredItems;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedConceptSaturationSatisfiableTestingItem::setMultipleDirectPredecessorsItems(bool multiblePredItems) {
				mMultiplePredItems = multiblePredItems;
				return this;
			}

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
