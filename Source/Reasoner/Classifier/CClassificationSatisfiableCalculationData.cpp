/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CClassificationSatisfiableCalculationData.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassificationSatisfiableCalculationData::CClassificationSatisfiableCalculationData() {
				mSatCalcedTask = nullptr;
				mSatCalcedJob = nullptr;
				mPrevData = nullptr;
				mDependendCalculationCount = 0;
			}


			bool CClassificationSatisfiableCalculationData::hasSatisfiableCalculationTask() {
				return mSatCalcedTask != nullptr;
			}

			CSatisfiableCalculationTask* CClassificationSatisfiableCalculationData::getSatisfiableCalculationTask() {
				return mSatCalcedTask;
			}

			CClassificationSatisfiableCalculationData* CClassificationSatisfiableCalculationData::setSatisfiableCalculationTask(CSatisfiableCalculationTask* task) {
				mSatCalcedTask = task;
				mSatCalcedJob = mSatCalcedTask;
				return this;
			}


			bool CClassificationSatisfiableCalculationData::hasSatisfiableCalculationJobInstantiation() {
				return mSatCalcedJob != nullptr;
			}



			bool CClassificationSatisfiableCalculationData::clearSatisfiableCalculationJobInstantiation() {
				bool cleared = false;
				if (mSatCalcedTask) {		
					CNewAllocationMemoryPoolProvider tmpProv;
					tmpProv.releaseMemoryPool(mSatCalcedTask->getMemoryPools());
					mSatCalcedTask = nullptr;
					mSatCalcedJob = nullptr;
					cleared = true;
				}
				return cleared;
			}

			CSatisfiableCalculationJobInstantiation* CClassificationSatisfiableCalculationData::getSatisfiableCalculationJobInstantiation() {
				return mSatCalcedJob;
			}


			CTaskPreyingListner* CClassificationSatisfiableCalculationData::tellPreyedSatisfialbeTask(CSatisfiableCalculationTask* task) {
				task->getTaskStatus()->setMemoryReleaseable(false);
				setSatisfiableCalculationTask(task);
				return this;
			}


			CClassificationSatisfiableCalculationData* CClassificationSatisfiableCalculationData::getPreviousCalculationData() {
				return mPrevData;
			}

			CClassificationSatisfiableCalculationData* CClassificationSatisfiableCalculationData::setPreviousCalculationData(CClassificationSatisfiableCalculationData* data) {
				mPrevData = data;
				return this;
			}

			cint64 CClassificationSatisfiableCalculationData::getDependendCalculationCount() {
				return mDependendCalculationCount;
			}

			CClassificationSatisfiableCalculationData* CClassificationSatisfiableCalculationData::incDependendCalculationCount(cint64 incCount) {
				mDependendCalculationCount += incCount;
				return this;
			}

			bool CClassificationSatisfiableCalculationData::decDependendCalculationCount(cint64 decCount) {
				mDependendCalculationCount -= decCount;
				return mDependendCalculationCount <= 0;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
