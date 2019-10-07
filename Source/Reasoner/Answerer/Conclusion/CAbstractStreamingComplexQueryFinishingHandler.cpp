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

#include "CAbstractStreamingComplexQueryFinishingHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Conclusion {

				CAbstractStreamingComplexQueryFinishingHandler::CAbstractStreamingComplexQueryFinishingHandler(CComplexQueryProcessingData* queryProcessingData, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, bool confExtendedLogging, const QString& logDomain, cint64 confMaximumBatchMappingsComputationSize, cint64 confFirstBatchMappingsComputationSize, double confBatchMappingsComputationSizeIncreasingFactor, bool confContinueMappingsCompuationWhenResultsNotWriteable, cint64 confMappingsComputationUnlimitedInterpretationSize) {
					mQueryProcessingData = queryProcessingData;
					mOntoAnsweringItem = ontoAnsweringItem;
					mConfExtendedLogging = confExtendedLogging;
					mLogDomain = logDomain;
					mConfMaximumBatchMappingsComputationSize = confMaximumBatchMappingsComputationSize;

					mConfFirstBatchMappingsComputationSize = confFirstBatchMappingsComputationSize;
					mNextMappingRequestingCountLimit = mConfFirstBatchMappingsComputationSize;
					mConfBatchMappingsComputationSizeIncreasingFactor = confBatchMappingsComputationSizeIncreasingFactor;
					mNextMappingRequestingCountLimitIncreasingFactor = mConfBatchMappingsComputationSizeIncreasingFactor;
					mConfContinueMappingsCompuationWhenResultsNotWriteable = confContinueMappingsCompuationWhenResultsNotWriteable;
					mConfMappingsComputationUnlimitedInterpretationSize = confMappingsComputationUnlimitedInterpretationSize;
				}


				CAbstractStreamingComplexQueryFinishingHandler::~CAbstractStreamingComplexQueryFinishingHandler() {
					if (mResultGenerator) {
						delete mResultGenerator;
					}
				}



				bool CAbstractStreamingComplexQueryFinishingHandler::init() {
					mVarBuildItem = mQueryProcessingData->getVariableBuildingItem();
					mCompAssIndVarQuery = (CComplexAssertionsIndividualVariablesAnsweringQuery*)mQueryProcessingData->getQuery();
					mConceptItem = mQueryProcessingData->getConceptItem();
					cint64 nextId = 0;
					for (CExpressionVariable* varExp : *mCompAssIndVarQuery->getDistinguishedVariableExpressions()) {
						mDisVarIdHash.insert(varExp, nextId++);
						mVarList.append(varExp->getName());
					}

					CVariableBindingFilteringAnswerMapping* lastFilteringAnsweringMapping = nullptr;
					for (CVariableBindingFiltering* filteringLinker = mCompAssIndVarQuery->getResultFilteringLinker(); filteringLinker; filteringLinker = filteringLinker->getNext()) {
						CVariableBindingFilteringAnswerMapping* tmpFilteringAnsweringMapping = filteringLinker->createFitleringAnswerMapping(mDisVarIdHash);
						if (!mFilteringAnsweringMapping) {
							lastFilteringAnsweringMapping = mFilteringAnsweringMapping = tmpFilteringAnsweringMapping;
						} else {
							lastFilteringAnsweringMapping->append(tmpFilteringAnsweringMapping);
							lastFilteringAnsweringMapping = tmpFilteringAnsweringMapping;
						}
					}

					cint64 variablesCount = 1;
					if (mVarBuildItem) {
						variablesCount = mVarBuildItem->getAllVariableSet()->count();
					}
					mDistinct = mCompAssIndVarQuery->isDistinctRequired();


					mBindsAnswersResult = nullptr;
					mExistBindsAnswersStreamingResult = dynamic_cast<CVariableBindingsAnswersStreamingResult*>(mCompAssIndVarQuery->getQueryResult());
					if (mExistBindsAnswersStreamingResult) {
						mAnswersWriteable &= mExistBindsAnswersStreamingResult->initResult(mVarList);
						mBindsAnswersResult = mExistBindsAnswersStreamingResult;
					} else {
						if (mCompAssIndVarQuery->getResultOrderingLinker()) {
							cint64 orderingVarCount = mCompAssIndVarQuery->getResultOrderingLinker()->getCount();
							bool mappingDistinct = mDistinct && mVarList.count() != variablesCount;
							CVariableBindingsAnswerResultMapOrdering* ordering = new CVariableBindingsAnswerResultMapOrdering(orderingVarCount);
							ordering->setDistinct(mappingDistinct);
							cint64 orderingVarNum = 0;
							for (CVariableBindingOrdering* orderingIt = mCompAssIndVarQuery->getResultOrderingLinker(); orderingIt; orderingIt = orderingIt->getNext()) {
								CExpressionVariable* orderingVarExp = orderingIt->getOrderingVariableExpression();
								CVariableBindingOrdering::ORDERING_TYPE orderingType = orderingIt->getOrderingType();
								cint64 orderingVarId = mDisVarIdHash.value(orderingVarExp);
								ordering->setAscending(orderingVarNum, orderingType == CVariableBindingOrdering::ASC);
								ordering->setVariableIndex(orderingVarNum, orderingVarId);
								++orderingVarNum;
							}
							mBindsAnswersResult = new CVariableBindingsAnswersOrderedMapResult(mVarList, ordering);
						} else {
							if (mDistinct) {
								mBindsAnswersResult = new CVariableBindingsAnswersSetResult(mVarList);
							} else {
								mBindsAnswersResult = new CVariableBindingsAnswersListResult(mVarList);
							}
						}
					}



					if (!mQueryProcessingData->isUnsatisfiable()) {
						if (mVarBuildItem && mVarBuildItem->isSatisfiable()) {
							CExpressionVariable* lastVarExp = mVarBuildItem->getLastHandledVariableExpression();
							if (!lastVarExp) {
								CVariableBindingsListAnswerResult* bindAnsRes = new CVariableBindingsListAnswerResult();
								for (CExpressionVariable* varExp : *mCompAssIndVarQuery->getDistinguishedVariableExpressions()) {
									QString varName = varExp->getName();
									QString anonymousBindingName = varName.replace("?", "").replace("$", "") + "_1";
									CVariableBindingStringResult* bindingResult = new CVariableBindingStringResult(anonymousBindingName, CVariableBindingResult::VBTANONYMOUSINDIVIDUAL);
									bindAnsRes->addResultVariableBinding(bindingResult);
								}
								mResultWriter.addVariableBindingAnswerToResult(mBindsAnswersResult, bindAnsRes, mFilteringAnsweringMapping, mCompAssIndVarQuery);
							} else {
								mLastVarItem = mVarBuildItem->getVariableLastCompositionItem(lastVarExp);
								COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping = mVarBuildItem->getVariableItemIndexMapping(lastVarExp).value(mLastVarItem);
								QHash<CExpressionVariable*, cint64> varIdxHash;

								QSet<CExpressionVariable*> distinguishedAnswerVariableSet(mCompAssIndVarQuery->getDistinguishedVariableExpressions()->toSet());
								for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = itemIndexMapping->constBegin(), itEnd = itemIndexMapping->constEnd(); it != itEnd; ++it) {
									cint64 varIdx = it.key();
									CExpressionVariable* varExp = it.value();
									if (distinguishedAnswerVariableSet.contains(varExp)) {
										varIdxHash.insert(varExp, varIdx);
									} else {
										mNonAnswerVariableMappingIndexList.append(varIdx);
									}
								}

								for (CExpressionVariable* varExp : *mCompAssIndVarQuery->getDistinguishedVariableExpressions()) {
									cint64 varIdx = varIdxHash.value(varExp, -1);
									if (varIdx >= 0) {
										mAnswerDistinguishedVariableMappingIndexList.append(varIdx);
									}
								}


								mSameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();

								mAnswerVarCount = mAnswerDistinguishedVariableMappingIndexList.size();


								mBindingResultVectorArray = new CVariableBindingResultVector*[mAnswerVarCount];
								for (cint64 i = 0; i < mAnswerVarCount; ++i) {
									mBindingResultVectorArray[i] = new CVariableBindingResultVector();
									mBindingResultVectorArray[i]->mVectorPos = i;
								}


								mIndiNameResolver = mOntoAnsweringItem->getOntology()->getIndividualNameResolver();
								mLastVarItemMapping = mLastVarItem->getVariableMapping();

								mLastVarItemProcessingDep = new COptimizedComplexVariableCompositionItemDependence(mLastVarItem);

								mBindResListAnswer = new CVariableBindingsListAnswerResult(mAnswerVarCount);

							}
						} else if (mConceptItem) {
							mSameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
							mConceptRealization = mOntoAnsweringItem->getOntology()->getRealization()->getConceptRealization();
							mInstanceItemSet = mConceptItem->getKnownInstanceItems();

						}
					}


					mInitialized = true;
					return true;
				}



				COptimizedComplexVariableCompositionItem* CAbstractStreamingComplexQueryFinishingHandler::getResultUsingLastVariableCompositionItem() {
					return mLastVarItem;
				}


				bool CAbstractStreamingComplexQueryFinishingHandler::finishQueryProcessing() {

					if (mLastVarItemMapping) {
						delete mBindResListAnswer;
						delete[] mBindingResultVectorArray;
						// TODO: delete all bindings in vector


						if (mConfExtendedLogging) {
							//LOG(INFO, mLogDomain, logTr("Last item cardinality over all variable mappings: %1").arg(mLastItemAllBindingsCardinality), this);
							cint64 mappingCount = mLastVarItemMapping->getBindingCount();
							LOG(INFO, mLogDomain, logTr("%2 mappings extended to %1 answers with average cardinality of %3 (maximum %4) and, in average, %5 (maximum %6) replicated answers due to same individuals.")
								.arg(mBindsAnswersResult->getResultCount()).arg(mappingCount)
								.arg(QString::number(mImplicitTotalCardinality / (double)mappingCount)).arg(mImplicitMaximalCardinality)
								.arg(QString::number(mExplicitTotalCardinality / (double)mappingCount)).arg(mExplicitMaximalCardinality)
								, this);
						}
					}

					LOG(INFO, mLogDomain, logTr("Determined %1 answers for complex ABox query with %2 answer variables (%3).").arg(mBindsAnswersResult->getResultCount()).arg(mBindsAnswersResult->getVariableNames().count()).arg(mBindsAnswersResult->getVariableNames().join(", ")), this);


					if (!mExistBindsAnswersStreamingResult) {
						mCompAssIndVarQuery->setQueryResult(mBindsAnswersResult);
					}
					for (CVariableBindingFilteringAnswerMapping* filteringAnsweringMappingIt = mFilteringAnsweringMapping; filteringAnsweringMappingIt; ) {
						CVariableBindingFilteringAnswerMapping* tmpFilteringAnsweringMapping = filteringAnsweringMappingIt;
						filteringAnsweringMappingIt = filteringAnsweringMappingIt->getNext();
						delete tmpFilteringAnsweringMapping;
					}


					return true;
				}



				bool CAbstractStreamingComplexQueryFinishingHandler::processQueryResults() {

					if (!mQueryProcessingData->isUnsatisfiable()) {


						function<cint64(cint64)> determineRemainingMappingsFunc = [&](cint64 processedMappings)->cint64 {
							cint64 currentAnswerCount = mBindsAnswersResult->getResultCount() + mQueryProcessingData->getOffsetSkippedMappingCount();
							cint64 requiredAnswerCount = mCompAssIndVarQuery->getResultLimitIncludingOffset();
							cint64 remainingRequiredAnswerCount = requiredAnswerCount - currentAnswerCount;
							cint64 processedMappingCount = processedMappings;
							cint64 requiredMappingsCount = 0;
							if (requiredAnswerCount == -1) {
								requiredMappingsCount = -1;
							} else {
								double mappingAnswerFactor = (double)currentAnswerCount / (double)processedMappingCount;
								requiredMappingsCount = (remainingRequiredAnswerCount / mappingAnswerFactor) + 1;
								requiredMappingsCount += processedMappingCount;
							}

							if (requiredMappingsCount == -1) {
								requiredMappingsCount = mNextMappingRequestingCountLimit;
								if (mConfMappingsComputationUnlimitedInterpretationSize >= 0 && requiredMappingsCount > mConfMappingsComputationUnlimitedInterpretationSize) {
									requiredMappingsCount = -1;
								}
							} else {
								requiredMappingsCount = qMin((cint64)mNextMappingRequestingCountLimit, requiredMappingsCount);
							}
							mNextMappingRequestingCountLimit *= mNextMappingRequestingCountLimitIncreasingFactor;
							if (mNextMappingRequestingCountLimit > mConfMaximumBatchMappingsComputationSize) {
								mNextMappingRequestingCountLimit = mConfMaximumBatchMappingsComputationSize;
							}
							return requiredMappingsCount;
						};

						++mComputationBatchNr;

						if (mVarBuildItem && mVarBuildItem->isSatisfiable()) {
							if (mLastVarItemProcessingDep) {


								processUpdatedBindingsCardinalityLinkers();



								bool moreComputationPossible = !mLastVarItem->isVariableMappingsComputed() && !mLastVarItemProcessingDep->isProcessingFinished() && (mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() < mCompAssIndVarQuery->getResultLimitIncludingOffset() || mCompAssIndVarQuery->getResultLimitIncludingOffset() == -1);
								bool resultsFurtherUsage = true;
								if (mExistBindsAnswersStreamingResult && moreComputationPossible) {
									resultsFurtherUsage = mExistBindsAnswersStreamingResult->flushResults();
									if (!mAnswersWriteable) {
										// count answers
										resultsFurtherUsage = true;
									}
								}


								bool dependentItemRescheduled = false;
								if (moreComputationPossible) {

									cint64 requiredMappingsCount = determineRemainingMappingsFunc(mLastVarItemProcessingDep->getTotalProcessedBindingsCardinalityLinkerCount());

									//dependentItemRescheduled |= rescheduleVariableCompositionItemComputation(mVarBuildItem, nullptr, mLastVarItem, requiredMappingsCount);

									bool continueComputation = true;
									if (!resultsFurtherUsage && !mConfContinueMappingsCompuationWhenResultsNotWriteable) {
										continueComputation = false;
										LOG(WARN, mLogDomain, logTr("Processing batch %1 resulted in %2 answers from %3 mappings, but no further computation batch scheduled since answers seem no longer writeable/sendable and it has not been configured to continue.").arg(mComputationBatchNr).arg(mBindsAnswersResult->getResultCount()).arg(mLastVarItemMapping->getBindingCount()), this);
									}


									if (continueComputation) {
										dependentItemRescheduled = true;
										if (requiredMappingsCount != -1) {
											mLastVarItem->setVariableMappingsComputationRequirement(qMax(mLastVarItem->getVariableMappingsComputationRequirement(), requiredMappingsCount));
										} else {
											mLastVarItem->setVariableMappingsComputationRequirement(requiredMappingsCount);
										}

										QString nextRequestingString = "all";
										if (requiredMappingsCount != -1) {
											nextRequestingString = QString("%1").arg(requiredMappingsCount);
										}
										LOG(INFO, mLogDomain, logTr("Processing batch %1 resulted in %2 answers from %3 mappings, scheduling next batch with %4 requested mappings.").arg(mComputationBatchNr).arg(mBindsAnswersResult->getResultCount()).arg(mLastVarItemMapping->getBindingCount()).arg(nextRequestingString), this);

										if (!mLastVarItem->isComputationQueued()) {
											mLastVarItem->setComputationQueued(true);
											mVarBuildItem->addComputeVariableMappingItem(mLastVarItem);
										}
									}

								}

								return !dependentItemRescheduled;


							}
						} else if (mConceptItem) {

							if (!mResultGenerator) {
								mResultGenerator = new CInstanceBindingAddingVisitor(mBindsAnswersResult, mFilteringAnsweringMapping, mCompAssIndVarQuery, mOntoAnsweringItem->getOntology()->getIndividualNameResolver(), mQueryProcessingData, &mResultWriter);
							}
							if (mInstanceItemSet) {
								while ((mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() < mCompAssIndVarQuery->getResultLimitIncludingOffset() || mCompAssIndVarQuery->getResultLimitIncludingOffset() == -1) && mInstanceItemLastProcessingLinker != mInstanceItemLastProcessedLinker || mInstanceItemLastRetrievedLinker != mInstanceItemSet->getAddedRealizationIndividualInstanceItemReferenceLinker()) {
									while ((mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() < mCompAssIndVarQuery->getResultLimitIncludingOffset() || mCompAssIndVarQuery->getResultLimitIncludingOffset() == -1) && mInstanceItemLastProcessingLinker != mInstanceItemLastProcessedLinker) {
										++mInstanceItemLinkerProcessedCount;
										mSameRealization->visitSameIndividuals(mInstanceItemLastProcessingLinker->getRealizationIndividualInstanceItemReference(), mResultGenerator);
										mInstanceItemLastProcessingLinker = mInstanceItemLastProcessingLinker->getNext();
									}
									if (mInstanceItemLastRetrievedLinker != mInstanceItemSet->getAddedRealizationIndividualInstanceItemReferenceLinker()) {
										mInstanceItemLastProcessedLinker = mInstanceItemLastRetrievedLinker;
										mInstanceItemLastRetrievedLinker = mInstanceItemLastProcessingLinker = mInstanceItemSet->getAddedRealizationIndividualInstanceItemReferenceLinker();
									}
								}


								bool moreInstanceComputationScheduled = false;
								// TODO: fix computed all check and scheduling
								// TODO: eliminate duplicated code for determining remaining mappings
								// TODO: request only one instance for concept item in the beginning


								CComplexConceptStepInstanceComputationProcess* instanceCompStep = mConceptItem->getComputationProcess()->getInstancesComputationProcess(true);
								bool moreComputationPossible = !instanceCompStep->hasAllInstancesComputed() && (mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() < mCompAssIndVarQuery->getResultLimitIncludingOffset() || mCompAssIndVarQuery->getResultLimitIncludingOffset() == -1);
								bool resultsFurtherUsage = true;
								if (mExistBindsAnswersStreamingResult && moreComputationPossible) {
									resultsFurtherUsage = mExistBindsAnswersStreamingResult->flushResults();
									if (!mAnswersWriteable) {
										// count answers
										resultsFurtherUsage = true;
									}
								}


								if (!instanceCompStep->hasAllInstancesComputed() && (mQueryProcessingData->getOffsetSkippedMappingCount() + mBindsAnswersResult->getResultCount() < mCompAssIndVarQuery->getResultLimitIncludingOffset() || mCompAssIndVarQuery->getResultLimitIncludingOffset() == -1)) {

									cint64 requiredMappingsCount = determineRemainingMappingsFunc(mInstanceItemLinkerProcessedCount);

									bool continueComputation = true;
									if (!resultsFurtherUsage && !mConfContinueMappingsCompuationWhenResultsNotWriteable) {
										continueComputation = false;
										LOG(WARN, mLogDomain, logTr("Processing batch %1 resulted in %2 answers from %3 instances, but no further computation batch scheduled since answers seem no longer writeable/sendable and it has not been configured to continue.").arg(mComputationBatchNr).arg(mBindsAnswersResult->getResultCount()).arg(mInstanceItemLinkerProcessedCount), this);
									}


									if (continueComputation) {

										moreInstanceComputationScheduled = true;
										CComputedItemDataNotificationLinker* notLinker = mOntoAnsweringItem->createComputedItemDataNotificationLinker();
										notLinker->setQueryProcessingData(mQueryProcessingData);

										if (requiredMappingsCount >= 0) {
											instanceCompStep->addComputedInstancesCountRequirement(requiredMappingsCount, notLinker);
										} else {
											instanceCompStep->setAllInstanceComputationRequired(true);
											instanceCompStep->addNotificationLinker(notLinker);
										}

										mQueryProcessingData->setComputationStepWaiting(instanceCompStep);

										QString nextRequestingString = "all";
										if (requiredMappingsCount != -1) {
											nextRequestingString = QString("%1").arg(requiredMappingsCount);
										}
										LOG(INFO, mLogDomain, logTr("Processing batch %1 resulted in %2 answers from %3 instances, scheduling next batch with %4 requested instances.").arg(mComputationBatchNr).arg(mBindsAnswersResult->getResultCount()).arg(mInstanceItemLinkerProcessedCount).arg(nextRequestingString), this);

										mConceptItem->setQueuedProcessStep(nullptr);
										if (!mConceptItem->hasQueuedProcessStep() && !instanceCompStep->isComputationProcessQueued() && !instanceCompStep->isComputationProcessProcessing()) {
											mConceptItem->setQueuedProcessStep(instanceCompStep);
											instanceCompStep->setComputationProcessQueued(true);
											COptimizedComplexConceptStepAnsweringItem* processingStep = mOntoAnsweringItem->getConceptProcessingStepItem(instanceCompStep->getComputationType());
											processingStep->addQueuedConceptItem(mConceptItem);
										}
									}


								}

								return !moreInstanceComputationScheduled;


								//for (COptimizedComplexConceptInstanziatedIndividualItemLinker* itemLinker = instanceItemSet->getAddedRealizationIndividualInstanceItemReferenceLinker(); itemLinker; itemLinker = itemLinker->getNext()) {
								//	mSameRealization->visitSameIndividuals(itemLinker->getRealizationIndividualInstanceItemReference(), &resultGenerator);
								//}
							}
							//for (auto node : *conceptItem->getDirectSubClassNodeSet()) {
							//	conceptRealization->visitInstances(node->getOneEquivalentConcept(), false, &resultGenerator);
							//}

						}

					}


					return true;
				}




				void CAbstractStreamingComplexQueryFinishingHandler::processUpdatedBindingsCardinalityLinkers() {
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

				void CAbstractStreamingComplexQueryFinishingHandler::handleBindingCardinalityLinker(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalities) {
					handleBindingCardinalityLinker(bindings, cardinalities, mBindsAnswersResult, mBindResListAnswer, mBindingResultVectorArray, mAnswerNr, mImplicitMaximalCardinality, mImplicitTotalCardinality, mExplicitMaximalCardinality, mExplicitTotalCardinality);
				}





				void CAbstractStreamingComplexQueryFinishingHandler::handleBindingCardinalityLinker(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalities, 
							CVariableBindingsAnswersResult*& useBindsAnswersResult, CVariableBindingsListAnswerResult*& useBindResListAnswer, CAbstractStreamingComplexQueryFinishingHandler::CVariableBindingResultVector**& useBindingResultVectorArray,
							cint64& useAnswerNr, cint64& useImplicitMaximalCardinality, cint64& useImplicitTotalCardinality, cint64& useExplicitMaximalCardinality, cint64& useExplicitTotalCardinality) {

					useAnswerNr++;

					for (cint64 i = 0; i < mAnswerVarCount; ++i) {
						useBindingResultVectorArray[i]->mValidBindingCount = 0;
						useBindingResultVectorArray[i]->mCurrentBindingIdx = 0;
						useBindingResultVectorArray[i]->mNextIteratingVector = nullptr;
					}

					cint64 cardinality = 1;
					if (!mDistinct) {
						if (cardinalities) {
							cardinality *= cardinalities->getSameIndividualsSeparatlyConsideredCardinality();
						}
						// determine additional cardinality of binding result due to same individuals bound to non-distinguished variables
						for (cint64 varIdx : mNonAnswerVariableMappingIndexList) {
							COptimizedComplexVariableIndividualMappings::VARIABLE_TYPE bindingType = mLastVarItemMapping->getBindingMapping(varIdx);
							if (bindingType == COptimizedComplexVariableIndividualMappings::INDIVIDUAL_VARIABLE) {
								CInstanceBindingIndividualCountingVisitor visitor;
								mSameRealization->visitSameIndividuals(bindings->getBinding(varIdx).reference, &visitor);
								if (visitor.individualCount > 1) {
									cardinality *= visitor.individualCount;
								}
							}
						}
					}

					if (!cardinalities) {
						mLastItemAllBindingsCardinality += 1;
					} else {
						mLastItemAllBindingsCardinality += cardinalities->getSameIndividualsSeparatlyConsideredCardinality();
					}

					useImplicitMaximalCardinality = qMax(useImplicitMaximalCardinality, cardinality);
					useImplicitTotalCardinality += cardinality;

					cint64 vectorIdx = 0;
					for (cint64 varIdx : mAnswerDistinguishedVariableMappingIndexList) {
						COptimizedComplexVariableIndividualMappings::VARIABLE_TYPE bindingType = mLastVarItemMapping->getBindingMapping(varIdx);
						if (bindingType == COptimizedComplexVariableIndividualMappings::INDIVIDUAL_VARIABLE) {

							CInstanceBindingIndividualLambdaCallingVisitor visitor([&](const CIndividualReference& indiRef) -> bool {
								QString indiName = mIndiNameResolver->getIndividualName(indiRef);
								if (!indiName.isEmpty()) {
									CVariableBindingResultVector* bindingResultVector = useBindingResultVectorArray[vectorIdx];
									if (bindingResultVector->size() <= bindingResultVector->mValidBindingCount) {
										bindingResultVector->resize(bindingResultVector->mValidBindingCount + 5);
									}
									CVariableBindingResult*& binding = (*bindingResultVector)[bindingResultVector->mValidBindingCount++];
									if (!binding) {
										binding = mResultWriter.createVariableBindingResult(indiRef, mCompAssIndVarQuery, indiName);
									} else {
										binding->initVariableBinding(indiRef, indiName);
									}
								}
								return true;
							});
							mSameRealization->visitSameIndividuals(bindings->getBinding(varIdx).reference, &visitor);
							vectorIdx++;

						} else if (bindingType == COptimizedComplexVariableIndividualMappings::DATA_LITERAL_VARIABLE) {

							TIndividualInstanceItemDataBinding& varBinding = bindings->getBinding(varIdx);
							CDataLiteral* dataLiteral = (CDataLiteral*)varBinding.pointer;

							CVariableBindingResultVector* bindingResultVector = useBindingResultVectorArray[vectorIdx];
							if (bindingResultVector->size() <= bindingResultVector->mValidBindingCount) {
								bindingResultVector->resize(bindingResultVector->mValidBindingCount + 5);
							}
							CVariableBindingResult*& binding = (*bindingResultVector)[bindingResultVector->mValidBindingCount++];
							if (!binding) {
								binding = mResultWriter.createVariableBindingResult(dataLiteral, mCompAssIndVarQuery);
							} else {
								binding->initVariableBinding(dataLiteral);
							}
							vectorIdx++;

						}
					}


					CVariableBindingResultVector* firstIteratingBindingVec = nullptr;
					CVariableBindingResultVector* lastIteratingBindingVec = nullptr;
					for (cint64 i = 0; i < mAnswerVarCount; ++i) {
						CVariableBindingResultVector* bindingVec = useBindingResultVectorArray[i];
						if (bindingVec->mValidBindingCount > 1) {
							if (lastIteratingBindingVec) {
								lastIteratingBindingVec->mNextIteratingVector = bindingVec;
							}
							if (firstIteratingBindingVec == nullptr) {
								firstIteratingBindingVec = bindingVec;
							}
							lastIteratingBindingVec = bindingVec;
						}
						useBindResListAnswer->setResult(i, (*bindingVec)[bindingVec->mCurrentBindingIdx]);
					}

					mResultWriter.addReusedVariableBindingAnswerToResultConsideringOffsetLimit(useBindsAnswersResult, useBindResListAnswer, mFilteringAnsweringMapping, mCompAssIndVarQuery, mQueryProcessingData, cardinality);

					cint64 explicitCardinality = 1;

					bool isValidBindingCombination = true;
					while ((useBindsAnswersResult->getResultCount() <= mCompAssIndVarQuery->getResultLimitIncludingOffset() || mCompAssIndVarQuery->getResultLimitIncludingOffset() == -1) && isValidBindingCombination && firstIteratingBindingVec) {

						firstIteratingBindingVec->mCurrentBindingIdx++;

						if (firstIteratingBindingVec->mCurrentBindingIdx >= firstIteratingBindingVec->mValidBindingCount) {
							firstIteratingBindingVec->mCurrentBindingIdx = 0;
							CVariableBindingResultVector* overflowIteratingBindingVec = firstIteratingBindingVec->mNextIteratingVector;
							if (!overflowIteratingBindingVec) {
								isValidBindingCombination = false;
							}
							while (overflowIteratingBindingVec) {
								overflowIteratingBindingVec->mCurrentBindingIdx++;
								CVariableBindingResultVector* nextOverflowIteratingBindingVec = nullptr;
								if (overflowIteratingBindingVec->mCurrentBindingIdx >= overflowIteratingBindingVec->mValidBindingCount) {
									overflowIteratingBindingVec->mCurrentBindingIdx = 0;
									nextOverflowIteratingBindingVec = overflowIteratingBindingVec->mNextIteratingVector;
									if (!nextOverflowIteratingBindingVec) {
										isValidBindingCombination = false;
									}
								}
								if (isValidBindingCombination) {
									useBindResListAnswer->setResult(overflowIteratingBindingVec->mVectorPos, (*overflowIteratingBindingVec)[overflowIteratingBindingVec->mCurrentBindingIdx]);
								}
								overflowIteratingBindingVec = nextOverflowIteratingBindingVec;
							}
						}
						if (isValidBindingCombination) {
							useBindResListAnswer->setResult(firstIteratingBindingVec->mVectorPos, (*firstIteratingBindingVec)[firstIteratingBindingVec->mCurrentBindingIdx]);

							mResultWriter.addReusedVariableBindingAnswerToResultConsideringOffsetLimit(useBindsAnswersResult, useBindResListAnswer, mFilteringAnsweringMapping, mCompAssIndVarQuery, mQueryProcessingData, cardinality);
							explicitCardinality++;
						}

					}

					useExplicitMaximalCardinality = qMax(useExplicitMaximalCardinality, explicitCardinality);
					useExplicitTotalCardinality += explicitCardinality;

				};






			}; // end namespace Conclusion

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
