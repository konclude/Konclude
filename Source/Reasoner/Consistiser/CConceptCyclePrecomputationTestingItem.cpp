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

#include "CConceptCyclePrecomputationTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CConceptCyclePrecomputationTestingItem::CConceptCyclePrecomputationTestingItem(CConceptCycleData* conceptCycleData, COntologyPrecomputationItem* preCompItem) : CPrecomputationTestingItem(preCompItem),mTaskPreyingAdapter(this) {
				mConceptCycleData = conceptCycleData;
			}

			CPrecomputationTestingItem::PRECOMPUTATIONTYPE CConceptCyclePrecomputationTestingItem::getPrecomputationTestingType() {
				return CONCEPTCYCLEPRECOMPUTATIONTYPE;
			}

			CTaskPreyingAdapter* CConceptCyclePrecomputationTestingItem::getTaskPreyingAdapter() {
				return &mTaskPreyingAdapter;
			}

			CTaskPreyingListner* CConceptCyclePrecomputationTestingItem::tellPreyedSatisfialbeTask(CSatisfiableCalculationTask* task) {
				if (mSatCalcTaskPointer.testAndSetOrdered(nullptr,task)) {
					CSatisfiableCalculationTask* taskIt = task;
					while (taskIt) {
						taskIt->getTaskStatus()->setMemoryReleaseable(false);
						taskIt = (CSatisfiableCalculationTask*)taskIt->getParentTask();
					}
				}
				return this;
			}

			CConceptCycleData* CConceptCyclePrecomputationTestingItem::getConceptCycleData() {
				return mConceptCycleData;
			}

			CSatisfiableCalculationJobInstantiation* CConceptCyclePrecomputationTestingItem::getJobInstantiation() {
                return mSatCalcTaskPointer.fetchAndAddRelaxed(0);
			}

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
