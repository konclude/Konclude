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

#include "CQtConcurrentStreamingComplexQueryFinishingHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Conclusion {

				CQtConcurrentStreamingComplexQueryFinishingHandler::CQtConcurrentStreamingComplexQueryFinishingHandler(CComplexQueryProcessingData* queryProcessingData, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, bool confExtendedLogging, const QString& logDomain, cint64 confMaximumBatchMappingsComputationSize, cint64 confFirstBatchMappingsComputationSize, double confBatchMappingsComputationSizeIncreasingFactor, bool confContinueMappingsCompuationWhenResultsNotWriteable, cint64 confMappingsComputationUnlimitedInterpretationSize)
						: CSequentialStreamingComplexQueryFinishingHandler(queryProcessingData, ontoAnsweringItem, confExtendedLogging, logDomain, confMaximumBatchMappingsComputationSize, confFirstBatchMappingsComputationSize, confBatchMappingsComputationSizeIncreasingFactor, confContinueMappingsCompuationWhenResultsNotWriteable, confMappingsComputationUnlimitedInterpretationSize) {

					mAllowConcurrentAnswerGeneration = true;
				}


				CQtConcurrentStreamingComplexQueryFinishingHandler::~CQtConcurrentStreamingComplexQueryFinishingHandler() {
				}





				void CQtConcurrentStreamingComplexQueryFinishingHandler::processUpdatedBindingsCardinalityLinkers() {

					if (!mAllowConcurrentAnswerGeneration || mCompAssIndVarQuery->getResultLimitIncludingOffset() > 0 || mFilteringAnsweringMapping || !mExistBindsAnswersStreamingResult) {
						mAllowConcurrentAnswerGeneration = false;
						CSequentialStreamingComplexQueryFinishingHandler::processUpdatedBindingsCardinalityLinkers();
					}

					if (!mAnswersWriteable && !mSameRealization->hasPotentiallySameIndividuals() && (mLastVarItemProcessingDep->getDependentItem()->getVariableMapping()->getMaximumCardinalitySameIndividualsSeparatelyConsidered() <= 1 || mDistinct)) {
						//TODO: max cardinality must be 1
						cint64 currentBindingCount = mLastVarItemProcessingDep->getDependentItem()->getVariableMapping()->getBindingCount();
						cint64 writtenResultCount = mExistBindsAnswersStreamingResult->getResultCount();
						mResultWriter.addReusedVariableBindingAnswerToResultConsideringOffsetLimit(mExistBindsAnswersStreamingResult, nullptr, mFilteringAnsweringMapping, mCompAssIndVarQuery, mQueryProcessingData, currentBindingCount - writtenResultCount);
					} else {

						while (!mLastVarItemProcessingDep->isBatchProcessed() || mLastVarItemProcessingDep->loadNextBatchProvidedBatch()) {



							cint64 bindingCardLinkerCount = 0;
							QVector<COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker*> bindingsCardLinkerBatchVec;
							while (!mLastVarItemProcessingDep->isBatchProcessed(false) || mLastVarItemProcessingDep->loadNextBatchProvidedBatch(false)) {

								while (mLastVarItemProcessingDep->getBatchProvidedBindingCardinalityBatchLinker(false)) {
									COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* bindingBatchLinker = mLastVarItemProcessingDep->getBatchProvidedBindingCardinalityBatchLinker(true);
									mLastVarItemProcessingDep->setBatchProvidedBindingCardinalityLinkersProcessed(bindingBatchLinker);
									bindingCardLinkerCount += bindingBatchLinker->getLinkerCount();
									bindingsCardLinkerBatchVec.append(bindingBatchLinker);
								}

							}
							if (!bindingsCardLinkerBatchVec.isEmpty()) {
								handleBindingCardinalityLinkerBatches(bindingsCardLinkerBatchVec, bindingCardLinkerCount);
							}




							while (mLastVarItemProcessingDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
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
				}



				void CQtConcurrentStreamingComplexQueryFinishingHandler::handleBindingCardinalityLinkerBatches(const QVector<COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker*>& bindingsCardLinkerBatchVec, cint64 bindingCardLinkerCount) {

					class CAnswerGenerationData {
					public:
						cint64 useAnswerNr = 0;
						cint64 useImplicitMaximalCardinality = 0;
						cint64 useImplicitTotalCardinality = 0;
						cint64 useExplicitMaximalCardinality = 0;
						cint64 useExplicitTotalCardinality = 0;
						CVariableBindingsAnswersStreamingResult* concurrentStreamBindsAnswersResult = nullptr;
						CVariableBindingsAnswersStreamingResult* existBindsAnswersStreamingResult = nullptr;

						CAnswerGenerationData* addData(const CAnswerGenerationData& genData) {
							useAnswerNr += genData.useAnswerNr;
							useImplicitMaximalCardinality = qMax(genData.useImplicitMaximalCardinality, useImplicitMaximalCardinality);
							useImplicitTotalCardinality += genData.useImplicitTotalCardinality;
							useExplicitMaximalCardinality = qMax(genData.useExplicitMaximalCardinality, useExplicitMaximalCardinality);
							useExplicitTotalCardinality += genData.useExplicitTotalCardinality;
							if (genData.existBindsAnswersStreamingResult) {
								genData.existBindsAnswersStreamingResult->releaseConcurrentlyWriteableStreamingResults(genData.concurrentStreamBindsAnswersResult);
							}
							return this;
						}
					};

					function<CAnswerGenerationData(COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* const & batchData)> bindingBatchesAnswerGenerationFunc = [&](COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* const & batchData) -> CAnswerGenerationData {

						CVariableBindingResultVector** useBindingResultVectorArray = new CVariableBindingResultVector*[mAnswerVarCount];
						for (cint64 i = 0; i < mAnswerVarCount; ++i) {
							useBindingResultVectorArray[i] = new CVariableBindingResultVector();
							useBindingResultVectorArray[i]->mVectorPos = i;
						}

						COptimizedComplexVariableIndividualBindingsCardinalityLinker* statBindCardLinker = batchData->getStartBindingsCardinalityLinker();
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* endBindCardLinker = batchData->getEndBindingsCardinalityLinker();

						CVariableBindingsAnswersStreamingResult* concurrentStreamBindsAnswersResult = mExistBindsAnswersStreamingResult->getConcurrentlyWriteableStreamingResults();
						CVariableBindingsAnswersResult* useBindsAnswersResult = concurrentStreamBindsAnswersResult;
						CVariableBindingsListAnswerResult* useBindResListAnswer = new CVariableBindingsListAnswerResult(mAnswerVarCount);
						cint64 useAnswerNr = 0;
						cint64 useImplicitMaximalCardinality = 0;
						cint64 useImplicitTotalCardinality = 0;
						cint64 useExplicitMaximalCardinality = 0;
						cint64 useExplicitTotalCardinality = 0;


						for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindCardLinkerIt = statBindCardLinker; bindCardLinkerIt; ) {
							COptimizedComplexVariableIndividualBindings* bindings = bindCardLinkerIt->getBindings();
							COptimizedComplexVariableIndividualBindingsCardinality* cardinalities = bindCardLinkerIt->getInitialCardinalities();
							
							handleBindingCardinalityLinker(bindings, cardinalities, useBindsAnswersResult, useBindResListAnswer, useBindingResultVectorArray, useAnswerNr, useImplicitMaximalCardinality, useImplicitTotalCardinality, useExplicitMaximalCardinality, useExplicitTotalCardinality);

							if (bindCardLinkerIt == endBindCardLinker) {
								bindCardLinkerIt = nullptr;
							} else {
								bindCardLinkerIt = bindCardLinkerIt->getNext();
							}
						}


						delete useBindingResultVectorArray;
						delete useBindResListAnswer;

						CAnswerGenerationData anserGenData;
						anserGenData.useAnswerNr = useAnswerNr;
						anserGenData.concurrentStreamBindsAnswersResult = concurrentStreamBindsAnswersResult;
						anserGenData.existBindsAnswersStreamingResult = mExistBindsAnswersStreamingResult;
						anserGenData.useImplicitMaximalCardinality = useImplicitMaximalCardinality;
						anserGenData.useImplicitTotalCardinality = useImplicitTotalCardinality;
						anserGenData.useExplicitMaximalCardinality = useExplicitMaximalCardinality;
						anserGenData.useExplicitTotalCardinality = useExplicitTotalCardinality;
						return anserGenData;

					};


					function<void(CAnswerGenerationData& genData, const CAnswerGenerationData& redGenData)> bindingBatchesAnswerReductionFunc = [&](CAnswerGenerationData& genData, const CAnswerGenerationData& redGenData) -> void {
						genData.addData(redGenData);
					};

					mExistBindsAnswersStreamingResult->flushResults();

					//QtConcurrent::blockingMapped(bindingsCardLinkerBatchVec, bindingBatchesAnswerGenerationFunc);
					//CAnswerGenerationData genData;
					CAnswerGenerationData genData = QtConcurrent::blockingMappedReduced<CAnswerGenerationData>(bindingsCardLinkerBatchVec, bindingBatchesAnswerGenerationFunc, bindingBatchesAnswerReductionFunc);

					mExplicitMaximalCardinality += genData.useExplicitMaximalCardinality;
					mExplicitTotalCardinality += genData.useExplicitTotalCardinality;
					mImplicitMaximalCardinality += genData.useImplicitMaximalCardinality;
					mImplicitTotalCardinality += genData.useImplicitTotalCardinality;
					mAnswerNr += genData.useAnswerNr;
					if (mConfExtendedLogging) {
						LOG(INFO, mLogDomain, logTr("Finished concurrent answer generation and serialization from %1 batches with %2 mappings in average.").arg(bindingsCardLinkerBatchVec.size()).arg(bindingCardLinkerCount / bindingsCardLinkerBatchVec.size()), this);
					}
				}


			}; // end namespace Conclusion

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
