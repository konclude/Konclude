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
