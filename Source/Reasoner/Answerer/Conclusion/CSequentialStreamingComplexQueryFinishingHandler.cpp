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

#include "CSequentialStreamingComplexQueryFinishingHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Conclusion {

				CSequentialStreamingComplexQueryFinishingHandler::CSequentialStreamingComplexQueryFinishingHandler(CComplexQueryProcessingData* queryProcessingData, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, bool confExtendedLogging, const QString& logDomain, cint64 confMaximumBatchMappingsComputationSize, cint64 confFirstBatchMappingsComputationSize, double confBatchMappingsComputationSizeIncreasingFactor, bool confContinueMappingsCompuationWhenResultsNotWriteable, cint64 confMappingsComputationUnlimitedInterpretationSize)
				: CAbstractStreamingComplexQueryFinishingHandler(queryProcessingData, ontoAnsweringItem, confExtendedLogging, logDomain, confMaximumBatchMappingsComputationSize, confFirstBatchMappingsComputationSize, confBatchMappingsComputationSizeIncreasingFactor, confContinueMappingsCompuationWhenResultsNotWriteable, confMappingsComputationUnlimitedInterpretationSize) {
				}


				CSequentialStreamingComplexQueryFinishingHandler::~CSequentialStreamingComplexQueryFinishingHandler() {
				}





				void CSequentialStreamingComplexQueryFinishingHandler::processUpdatedBindingsCardinalityLinkers() {
					while ((mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() < mCompAssIndVarQuery->getResultLimitIncludingOffset() || mCompAssIndVarQuery->getResultLimitIncludingOffset() == -1) && (!mLastVarItemProcessingDep->isBatchProcessed() || mLastVarItemProcessingDep->loadNextBatch())) {

						while ((mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() <= mCompAssIndVarQuery->getResultLimitIncludingOffset() || mCompAssIndVarQuery->getResultLimitIncludingOffset() == -1) && mLastVarItemProcessingDep->getBatchCurrentBindingsCardinalityLinker(false)) {
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = mLastVarItemProcessingDep->getBatchCurrentBindingsCardinalityLinker(true);
							COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
							COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

							handleBindingCardinalityLinker(bindings, cardinalites);
						}

						while ((mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() < mCompAssIndVarQuery->getResultLimitIncludingOffset() || mCompAssIndVarQuery->getResultLimitIncludingOffset() == -1) && mLastVarItemProcessingDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
							COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = mLastVarItemProcessingDep->getBatchCurrentUpdatedCardinalityLinker(true);
							if (!mDistinct) {
								COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
								COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
								COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

								COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
								tmpDiffCardinalites.substractCardinalities(prevCardinalites);

								handleBindingCardinalityLinker(bindings, &tmpDiffCardinalites);
							}
						}
					}
				}



			}; // end namespace Conclusion

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
