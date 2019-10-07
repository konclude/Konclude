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

#include "CComplexConceptStepInstanceComputationProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CComplexConceptStepInstanceComputationProcess::CComplexConceptStepInstanceComputationProcess() : CComplexConceptStepComputationProcess(COMPUTATIONTYPEINSTANCES) {
				mMaximalRequiredInstancesCount = 0;
				mComputedInstancesCount = 0;
				mRequiredAllInstances = false;
				mAllInstancesComputed = false;
				mExpectedInstancesCount = 0;
			}


			CComplexConceptStepInstanceComputationProcess::~CComplexConceptStepInstanceComputationProcess() {
			}



			bool CComplexConceptStepInstanceComputationProcess::requiresAdditionalInstanceComputation() {
				if (mMaximalRequiredInstancesCount > mComputedInstancesCount) {
					return true;
				}
				return false;
			}

			cint64 CComplexConceptStepInstanceComputationProcess::getComputedInstancesCount() {
				return mComputedInstancesCount;
			}

			cint64 CComplexConceptStepInstanceComputationProcess::getRequiredInstancesCount() {
				return mMaximalRequiredInstancesCount;
			}

			cint64 CComplexConceptStepInstanceComputationProcess::getAdditionalRequiredComputedInstancesCount() {
				return mMaximalRequiredInstancesCount - mComputedInstancesCount;
			}


			CComplexConceptStepInstanceComputationProcess* CComplexConceptStepInstanceComputationProcess::updateComputedInstancesCount(cint64 instancesCount, function<void(CComputedItemDataNotificationLinker* notificationLinker)> waitingHandlerFunction) {
				mComputedInstancesCount = instancesCount;
				while (!mCountDataMap.isEmpty() && mCountDataMap.begin().key() <= instancesCount) {
					CRequirementWaitingDependencyData& waitDepData = mCountDataMap.begin().value();
					CComputedItemDataNotificationLinker* notificationLinker = waitDepData.takeNotificationLinker();
					if (notificationLinker) {
						waitingHandlerFunction(notificationLinker);
					}
					mCountDataMap.erase(mCountDataMap.begin());
				}
				return this;
			}



			CComplexConceptStepInstanceComputationProcess* CComplexConceptStepInstanceComputationProcess::updateAllInstancesComputed(function<void(CComputedItemDataNotificationLinker* notificationLinker)> waitingHandlerFunction) {
				mAllInstancesComputed = true;
				while (!mCountDataMap.isEmpty()) {
					CRequirementWaitingDependencyData& waitDepData = mCountDataMap.begin().value();
					CComputedItemDataNotificationLinker* notificationLinker = waitDepData.takeNotificationLinker();
					if (notificationLinker) {
						waitingHandlerFunction(notificationLinker);
					}
					mCountDataMap.erase(mCountDataMap.begin());
				}
				return this;
			}



			CComplexConceptStepInstanceComputationProcess* CComplexConceptStepInstanceComputationProcess::addComputedInstancesCountRequirement(cint64 count, CComputedItemDataNotificationLinker* notificationLinker) {
				CRequirementWaitingDependencyData& depData = mCountDataMap[count];
				if (notificationLinker) {
					depData.addNotificationLinker(notificationLinker);
				}
				mMaximalRequiredInstancesCount = qMax(mMaximalRequiredInstancesCount, count);
				return this;
			}




			bool CComplexConceptStepInstanceComputationProcess::requiresAllInstanceComputation() {
				return mRequiredAllInstances;
			}

			CComplexConceptStepInstanceComputationProcess* CComplexConceptStepInstanceComputationProcess::setAllInstanceComputationRequired(bool allInstancesRequired) {
				mRequiredAllInstances = allInstancesRequired;
				return this;
			}


			bool CComplexConceptStepInstanceComputationProcess::hasAllInstancesComputed() {
				return mAllInstancesComputed;
			}

			CComplexConceptStepInstanceComputationProcess* CComplexConceptStepInstanceComputationProcess::setAllInstancesComputed(bool allInstancesComputed) {
				mAllInstancesComputed = allInstancesComputed;
				return this;
			}


			double CComplexConceptStepInstanceComputationProcess::getExpectedInstancesCount() {
				return mExpectedInstancesCount;
			}

			CComplexConceptStepInstanceComputationProcess* CComplexConceptStepInstanceComputationProcess::setExpectedInstancesCount(double expectedCount) {
				mExpectedInstancesCount = expectedCount;
				return this;
			}

			
		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
