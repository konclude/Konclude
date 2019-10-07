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

#include "CComplexQueryProcessingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CComplexQueryProcessingData::CComplexQueryProcessingData(CComplexAnsweringQuery* query, CCallbackData* callback) {
				mQuery = query;
				mCallback = callback;
				mWaitingComputationStepsCount = 0;
				mConceptItem = nullptr;
				mUnsatisfiable = false;
				mWaitingEntailmentComputationsCount = 0;
				mBuildingVariableItemCount = 0;
				mVariableBuildingItem = nullptr;
				mFinishingHandler = nullptr;
				mOffsetSkippedMappingCount = 0;
				mQueryProcessingInitializedAnsweringStatistics = nullptr;
				mComputationError = false;
			}


			CComplexQueryProcessingData::~CComplexQueryProcessingData() {
				if (mQueryProcessingInitializedAnsweringStatistics) {
					delete mQueryProcessingInitializedAnsweringStatistics;
				}
			}

			CComplexAnsweringQuery* CComplexQueryProcessingData::getQuery() {
				return mQuery;
			}

			CCallbackData* CComplexQueryProcessingData::getCallback() {
				return mCallback;
			}


			bool CComplexQueryProcessingData::isWaitingComputationStep() {
				return mWaitingComputationStepsCount > 0;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setComputationStepFinished(CComplexConceptStepComputationProcess* computationStep) {
				mWaitingComputationStepsCount--;
				return this;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setComputationStepWaiting(CComplexConceptStepComputationProcess* computationStep) {
				mWaitingComputationStepsCount++;
				return this;
			}

			COptimizedComplexConceptItem* CComplexQueryProcessingData::getConceptItem() {
				return mConceptItem;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setConceptItem(COptimizedComplexConceptItem* conceptItem) {
				mConceptItem = conceptItem;
				return this;
			}

			bool CComplexQueryProcessingData::isUnsatisfiable() {
				return mUnsatisfiable;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setUnsatisfiable(bool unsat) {
				mUnsatisfiable = unsat;
				return this;
			}


			bool CComplexQueryProcessingData::isComputationError() {
				return mComputationError;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setComputationError(bool error) {
				mComputationError = error;
				return this;
			}



			bool CComplexQueryProcessingData::hasEntailmentComputation() {
				return mWaitingEntailmentComputationsCount > 0;
			}

			bool CComplexQueryProcessingData::hasComputation() {
				return hasEntailmentComputation() || isWaitingComputationStep() || isBuildingVariableItem();
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::incEntailmentComputation(cint64 count) {
				mWaitingEntailmentComputationsCount += count;
				return this;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::decEntailmentComputation(cint64 count) {
				mWaitingEntailmentComputationsCount -= count;
				return this;
			}


			bool CComplexQueryProcessingData::isBuildingVariableItem() {
				return mBuildingVariableItemCount > 0;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::incBuildingVariableItem(cint64 count) {
				mBuildingVariableItemCount += count;
				return this;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::decBuildingVariableItem(cint64 count) {
				mBuildingVariableItemCount -= count;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* CComplexQueryProcessingData::getVariableBuildingItem() {
				return mVariableBuildingItem;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setVariableBuildingItem(COptimizedComplexBuildingVariableCompositionsItem* variableBuildingItem) {
				mVariableBuildingItem = variableBuildingItem;
				return this;
			}


			bool CComplexQueryProcessingData::isProcessingStarted() {
				return mProcessingStarted;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setProcessingStarted(bool started) {
				mProcessingStarted = started;
				mProcessingTimer.start();
				return this;
			}


			bool CComplexQueryProcessingData::isProcessingFinished() {
				return mProcessingFinished;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setProcessingFinished(bool finished) {
				mProcessingFinished = finished;
				mProcessingTime = mProcessingTimer.elapsed();
				return this;
			}

			qint64 CComplexQueryProcessingData::getProcessingTime() {
				return mProcessingTime;
			}

			CAbstractComplexQueryFinishingHandler* CComplexQueryProcessingData::getFinishingHandler() {
				return mFinishingHandler;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setFinishingHandler(CAbstractComplexQueryFinishingHandler* finishingHandler) {
				mFinishingHandler = finishingHandler;
				return this;
			}


			cint64 CComplexQueryProcessingData::getOffsetSkippedMappingCount() {
				return mOffsetSkippedMappingCount;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setOffsetSkippedMappingCount(cint64 skippedMappingCount) {
				mOffsetSkippedMappingCount = skippedMappingCount;
				return this;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::incOffsetSkippedMappingCount(cint64 incSkippedMappingCount) {
				mOffsetSkippedMappingCount += incSkippedMappingCount;
				return this;
			}

			CAnsweringHandlingStatistics* CComplexQueryProcessingData::getQueryProcessingInitializedAnsweringStatistics() {
				return mQueryProcessingInitializedAnsweringStatistics;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setQueryProcessingInitializedAnsweringStatistics(CAnsweringHandlingStatistics* stats) {
				if (mQueryProcessingInitializedAnsweringStatistics) {
					delete mQueryProcessingInitializedAnsweringStatistics;
				}
				mQueryProcessingInitializedAnsweringStatistics = stats;
				return this;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
