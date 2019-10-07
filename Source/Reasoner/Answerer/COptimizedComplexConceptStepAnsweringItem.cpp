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

#include "COptimizedComplexConceptStepAnsweringItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexConceptStepAnsweringItem::COptimizedComplexConceptStepAnsweringItem() {
			}

			COptimizedComplexConceptStepAnsweringItem::~COptimizedComplexConceptStepAnsweringItem() {
			}
			

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptStepAnsweringItem::addQueuedConceptItem(COptimizedComplexConceptItem* item) {
				if (!mConceptItemQueuedSet.contains(item)) {
					mConceptItemQueuedSet.insert(item);
					mConceptItemQueuedList.append(item);
				}
				return this;
			}

			COptimizedComplexConceptItem* COptimizedComplexConceptStepAnsweringItem::takeNextQueuedConceptItem() {
				COptimizedComplexConceptItem* item = nullptr;
				if (!mConceptItemQueuedList.isEmpty()) {
					item = mConceptItemQueuedList.takeFirst();
					mConceptItemQueuedSet.remove(item);
				}
				return item;
			}


			COptimizedComplexConceptItem* COptimizedComplexConceptStepAnsweringItem::getNextQueuedConceptItem() {
				COptimizedComplexConceptItem* item = nullptr;
				if (!mConceptItemQueuedList.isEmpty()) {
					item = mConceptItemQueuedList.first();
				}
				return item;
			}

			QList<COptimizedComplexConceptItem*>* COptimizedComplexConceptStepAnsweringItem::getConceptItemQueuedList() {
				return &mConceptItemQueuedList;
			}

			QSet<COptimizedComplexConceptItem*>* COptimizedComplexConceptStepAnsweringItem::getConceptItemQueuedSet() {
				return &mConceptItemQueuedSet;
			}


			QSet<COptimizedComplexConceptItem*>* COptimizedComplexConceptStepAnsweringItem::getConceptItemProcessingSet() {
				return &mConceptItemProcessingSet;
			}


			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptStepAnsweringItem::addProcessingConceptItem(COptimizedComplexConceptItem* item) {
				mConceptItemProcessingSet.insert(item);
				return this;
			}

			COptimizedComplexConceptStepAnsweringItem* COptimizedComplexConceptStepAnsweringItem::removeProcessingConceptItem(COptimizedComplexConceptItem* item) {
				mConceptItemProcessingSet.remove(item);
				return this;
			}

			bool COptimizedComplexConceptStepAnsweringItem::hasConceptItemsQueued() {
				return !mConceptItemQueuedSet.isEmpty();
			}

			bool COptimizedComplexConceptStepAnsweringItem::hasConceptItemsProcessing() {
				return !mConceptItemProcessingSet.isEmpty();
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
