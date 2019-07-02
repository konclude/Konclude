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

#include "CComplexConceptStepInstanceComputationProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CComplexConceptStepInstanceComputationProcess::CComplexConceptStepInstanceComputationProcess() : CComplexConceptStepComputationProcess(COMPUTATIONTYPEINSTANCES) {
				mMaximalRequiredInstancesCount = 0;
				mComputedInstancesCount = 0;
				mRequiredAllInstances = false;
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


			CComplexConceptStepInstanceComputationProcess* CComplexConceptStepInstanceComputationProcess::updateComputedInstancesCount(cint64 instancesCount, function<void(CXLinker<CComplexQueryProcessingData*>* waitingQueryProcessingLinker, CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* waitingBuildingVariableItemProcessingLinker)> waitingHandlerFunction) {
				mComputedInstancesCount = instancesCount;
				while (!mCountDataMap.isEmpty() && mCountDataMap.begin().key() <= instancesCount) {
					CRequirementWaitingDependencyData& waitDepData = mCountDataMap.begin().value();
					CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* buildingVariableItemProcessingLinker = waitDepData.takeBuildingVariableItemProcessingLinker();
					CXLinker<CComplexQueryProcessingData*>* queryProcessingLinker = waitDepData.takeQueryProcessingLinker();
					if (buildingVariableItemProcessingLinker || queryProcessingLinker) {
						waitingHandlerFunction(queryProcessingLinker, buildingVariableItemProcessingLinker);
					}
					mCountDataMap.erase(mCountDataMap.begin());
				}
				return this;
			}



			CComplexConceptStepInstanceComputationProcess* CComplexConceptStepInstanceComputationProcess::updateAllInstancesComputed(function<void(CXLinker<CComplexQueryProcessingData*>* waitingQueryProcessingLinker, CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* waitingBuildingVariableItemProcessingLinker)> waitingHandlerFunction) {
				while (!mCountDataMap.isEmpty()) {
					CRequirementWaitingDependencyData& waitDepData = mCountDataMap.begin().value();
					CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* buildingVariableItemProcessingLinker = waitDepData.takeBuildingVariableItemProcessingLinker();
					CXLinker<CComplexQueryProcessingData*>* queryProcessingLinker = waitDepData.takeQueryProcessingLinker();
					if (buildingVariableItemProcessingLinker || queryProcessingLinker) {
						waitingHandlerFunction(queryProcessingLinker, buildingVariableItemProcessingLinker);
					}
					mCountDataMap.erase(mCountDataMap.begin());
				}
				return this;
			}



			CComplexConceptStepInstanceComputationProcess* CComplexConceptStepInstanceComputationProcess::addComputedInstancesCountRequirement(cint64 count, CXLinker<CComplexQueryProcessingData*>* waitingQueryProcessingLinker, CXLinker<COptimizedComplexBuildingVariableCompositionsItem*>* waitingBuildingVariableItemProcessingLinker) {
				CRequirementWaitingDependencyData& depData = mCountDataMap[count];
				if (waitingQueryProcessingLinker) {
					depData.addQueryProcessingLinker(waitingQueryProcessingLinker);
				}
				if (waitingBuildingVariableItemProcessingLinker) {
					depData.addBuildingVariableItemProcessingLinker(waitingBuildingVariableItemProcessingLinker);
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


			
		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
