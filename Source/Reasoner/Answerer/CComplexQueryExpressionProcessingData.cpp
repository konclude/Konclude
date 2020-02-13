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

#include "CComplexQueryExpressionProcessingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CComplexQueryExpressionProcessingData::CComplexQueryExpressionProcessingData(CComplexAnsweringExpressionQuery* query, CCallbackData* callback) : CComplexQueryProcessingData(query, callback) {
				mQuery = query;
				mWaitingComputationStepsCount = 0;
				mConceptItem = nullptr;
				mWaitingEntailmentComputationsCount = 0;
				mBuildingVariableItemCount = 0;
				mVariableBuildingItem = nullptr;
				mFinishingHandler = nullptr;
				mOffsetSkippedMappingCount = 0;
				mQueryProcessingInitializedAnsweringStatistics = nullptr;
				mComputationError = false;
				mExtendibleData = nullptr;
			}


			CComplexQueryExpressionProcessingData::~CComplexQueryExpressionProcessingData() {
			}

			CComplexAnsweringExpressionQuery* CComplexQueryExpressionProcessingData::getQuery() {
				return mQuery;
			}


			bool CComplexQueryExpressionProcessingData::isWaitingComputationStep() {
				return mWaitingComputationStepsCount > 0;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::setComputationStepFinished(CComplexConceptStepComputationProcess* computationStep) {
				mWaitingComputationStepsCount--;
				return this;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::setComputationStepWaiting(CComplexConceptStepComputationProcess* computationStep) {
				mWaitingComputationStepsCount++;
				return this;
			}

			COptimizedComplexConceptItem* CComplexQueryExpressionProcessingData::getConceptItem() {
				return mConceptItem;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::setConceptItem(COptimizedComplexConceptItem* conceptItem) {
				mConceptItem = conceptItem;
				return this;
			}


			bool CComplexQueryExpressionProcessingData::isComputationError() {
				return mComputationError;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::setComputationError(bool error) {
				mComputationError = error;
				return this;
			}



			bool CComplexQueryExpressionProcessingData::hasEntailmentComputation() {
				return mWaitingEntailmentComputationsCount > 0;
			}

			bool CComplexQueryExpressionProcessingData::hasComputation() {
				return hasEntailmentComputation() || isWaitingComputationStep() || isBuildingVariableItem();
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::incEntailmentComputation(cint64 count) {
				mWaitingEntailmentComputationsCount += count;
				return this;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::decEntailmentComputation(cint64 count) {
				mWaitingEntailmentComputationsCount -= count;
				return this;
			}


			bool CComplexQueryExpressionProcessingData::isBuildingVariableItem() {
				return mBuildingVariableItemCount > 0;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::incBuildingVariableItem(cint64 count) {
				mBuildingVariableItemCount += count;
				return this;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::decBuildingVariableItem(cint64 count) {
				mBuildingVariableItemCount -= count;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* CComplexQueryExpressionProcessingData::getVariableBuildingItem() {
				return mVariableBuildingItem;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::setVariableBuildingItem(COptimizedComplexBuildingVariableCompositionsItem* variableBuildingItem) {
				mVariableBuildingItem = variableBuildingItem;
				return this;
			}



			CAbstractComplexQueryFinishingHandler* CComplexQueryExpressionProcessingData::getFinishingHandler() {
				return mFinishingHandler;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::setFinishingHandler(CAbstractComplexQueryFinishingHandler* finishingHandler) {
				mFinishingHandler = finishingHandler;
				return this;
			}


			cint64 CComplexQueryExpressionProcessingData::getOffsetSkippedMappingCount() {
				return mOffsetSkippedMappingCount;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::setOffsetSkippedMappingCount(cint64 skippedMappingCount) {
				mOffsetSkippedMappingCount = skippedMappingCount;
				return this;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::incOffsetSkippedMappingCount(cint64 incSkippedMappingCount) {
				mOffsetSkippedMappingCount += incSkippedMappingCount;
				return this;
			}

			CComplexQueryExtendibleData* CComplexQueryExpressionProcessingData::getQueryExtendibleData() {
				return mExtendibleData;
			}

			CComplexQueryExpressionProcessingData* CComplexQueryExpressionProcessingData::setQueryExtendibleData(CComplexQueryExtendibleData* extData) {
				mExtendibleData = extData;
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
