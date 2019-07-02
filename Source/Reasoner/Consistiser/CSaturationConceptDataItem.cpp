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

#include "CSaturationConceptDataItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CSaturationConceptDataItem::CSaturationConceptDataItem() {
			}


			CSaturationConceptDataItem* CSaturationConceptDataItem::initConceptSaturationTestingItem(CConcept* satTestConcept, bool negated, CRole* role) {
				mConceptSat = satTestConcept;
				mNegation = negated;
				mConRefLinking = nullptr;
				mSpecialItemReference = nullptr;
				mSpecialReferenceMode = NONESATURATIONMODE;
				mOrderingQueued = false;
				mSucessorsItems = false;
				mMultiplePredItems = false;
				mRoleRanges = role;
				mInvalidSpecialItemReference = false;
				mProcessingQueued = false;
				mProcessingMarked = false;
				return this;
			}

			CConcept* CSaturationConceptDataItem::getSaturationConcept() {
				return mConceptSat;
			}

			CRole* CSaturationConceptDataItem::getSaturationRoleRanges() {
				return mRoleRanges;
			}

			bool CSaturationConceptDataItem::getSaturationNegation() {
				return mNegation;
			}

			CConceptReferenceLinking* CSaturationConceptDataItem::getConceptReferenceLinking() {
				return mConRefLinking;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::setConceptReferenceLinking(CConceptReferenceLinking* refLinking) {
				mConRefLinking = refLinking;
				return this;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::getSpecialItemReference() {
				return mSpecialItemReference;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::setSpecialItemReference(CSaturationConceptDataItem* reference) {
				mSpecialItemReference = reference;
				return this;
			}

			CSaturationConceptDataItem::SATURATIONITEMREFERENCESPECIALMODE CSaturationConceptDataItem::getSpecialReferenceMode() {
				return mSpecialReferenceMode;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::setSpecialItemReferenceMode(CSaturationConceptDataItem::SATURATIONITEMREFERENCESPECIALMODE mode) {
				mSpecialReferenceMode = mode;
				return this;
			}

			bool CSaturationConceptDataItem::isOrderingQueued() {
				return mOrderingQueued;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::setOrderingQueued(bool queued) {
				mOrderingQueued = queued;
				return this;
			}

			bool CSaturationConceptDataItem::hasIndirectSuccessorsItems() {
				return mSucessorsItems;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::setIndirectSuccessorsItems(bool hasSuccessors) {
				mSucessorsItems = hasSuccessors;
				return this;
			}

			QList<CSaturationConceptDataItem*>* CSaturationConceptDataItem::getExistReferenceConceptItemList() {
				return &mExistReferenceItemList;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::addExistReferenceConceptItemList(CSaturationConceptDataItem* existItem) {
				mExistReferenceItemList.append(existItem);
				return this;
			}

			bool CSaturationConceptDataItem::hasMultipleDirectPredecessorsItems() {
				return mMultiplePredItems;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::setMultipleDirectPredecessorsItems(bool multiblePredItems) {
				mMultiplePredItems = multiblePredItems;
				return this;
			}

			bool CSaturationConceptDataItem::isInvalidSpecialItemReference() {
				return mInvalidSpecialItemReference;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::setInvalidSpecialItemReference(bool invalid) {
				mInvalidSpecialItemReference = invalid;
				return this;
			}

			bool CSaturationConceptDataItem::isItemProcessingQueued() {
				return mProcessingQueued;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::setItemProcessingQueued(bool queued) {
				mProcessingQueued = queued;
				return this;
			}


			bool CSaturationConceptDataItem::isItemProcessingMarked() {
				return mProcessingMarked;
			}

			CSaturationConceptDataItem* CSaturationConceptDataItem::setItemProcessingMarked(bool marked) {
				mProcessingMarked = marked;
				return this;
			}

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
