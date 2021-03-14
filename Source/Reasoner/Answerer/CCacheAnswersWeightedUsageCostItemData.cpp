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

#include "CCacheAnswersWeightedUsageCostItemData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CCacheAnswersWeightedUsageCostItemData::CCacheAnswersWeightedUsageCostItemData() {
				mComplexConceptItemSet = nullptr;
				mComplexVariableItemSet = nullptr;
			}


			CCacheAnswersWeightedUsageCostItemData::~CCacheAnswersWeightedUsageCostItemData() {
				if (mComplexConceptItemSet) {
					delete mComplexConceptItemSet;
				}
				if (mComplexVariableItemSet) {
					delete mComplexVariableItemSet;
				}
			}

			CCacheAnswersWeightedUsageCostItemData::CCacheAnswersWeightedUsageCostItemData(COptimizedComplexConceptItem* complexConceptItem) {
				getComplexConceptItemSet()->insert(complexConceptItem);
				mComplexVariableItemSet = nullptr;
			}

			CCacheAnswersWeightedUsageCostItemData::CCacheAnswersWeightedUsageCostItemData(COptimizedComplexVariableCompositionItem* complexVariableItem) {
				mComplexConceptItemSet = nullptr;
				getComplexVariableItemSet()->insert(complexVariableItem);

			}


			QSet<COptimizedComplexConceptItem*>* CCacheAnswersWeightedUsageCostItemData::getComplexConceptItemSet(bool createIfNotExists) {
				if (!mComplexConceptItemSet && createIfNotExists) {
					mComplexConceptItemSet = new QSet<COptimizedComplexConceptItem *>();
				}
				return mComplexConceptItemSet;
			}

			QSet<COptimizedComplexVariableCompositionItem*>* CCacheAnswersWeightedUsageCostItemData::getComplexVariableItemSet(bool createIfNotExists) {
				if (!mComplexVariableItemSet && createIfNotExists) {
					mComplexVariableItemSet = new QSet<COptimizedComplexVariableCompositionItem *>();
				}
				return mComplexVariableItemSet;
			}

			CCacheAnswersWeightedUsageCostItemData* CCacheAnswersWeightedUsageCostItemData::addItem(COptimizedComplexConceptItem* complexConceptItem) {
				getComplexConceptItemSet()->insert(complexConceptItem);
				return this;
			}

			CCacheAnswersWeightedUsageCostItemData* CCacheAnswersWeightedUsageCostItemData::addItem(COptimizedComplexVariableCompositionItem* complexVariableItem) {
				getComplexVariableItemSet()->insert(complexVariableItem);
				return this;
			}

			bool CCacheAnswersWeightedUsageCostItemData::isEmpty() {
				if (mComplexConceptItemSet && !mComplexConceptItemSet->isEmpty()) {
					return false;
				}
				if (mComplexVariableItemSet && !mComplexVariableItemSet->isEmpty()) {
					return false;
				}
				return true;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
