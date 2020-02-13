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

#include "COptimizedComplexCompositionAnsweringHandler.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			



			COptimizedComplexCompositionAnsweringHandler::COptimizedComplexCompositionAnsweringHandler(COptimizedComplexCompositionOntologyAnsweringItem* ontoAnsweringItem) : CAnsweringHandler(), CLogDomain("::Konclude::Reasoner::Kernel::ComplexComposedQueryAnswerer") {
				mOntoAnsweringItem = ontoAnsweringItem;


			}


			COptimizedComplexCompositionAnsweringHandler::~COptimizedComplexCompositionAnsweringHandler() {
			}

			bool COptimizedComplexCompositionAnsweringHandler::canAnsweringComplexQuery(bool composedQuery) {
				return composedQuery;
			}

			bool COptimizedComplexCompositionAnsweringHandler::canAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery) {
				if (dynamic_cast<CComplexAnsweringCompositionQuery*>(complexAnsweringQuery)) {
					return true;
				}
				return false;
			}

			bool COptimizedComplexCompositionAnsweringHandler::addAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback) {
				mPendingQueryProcessingList.append(new CComplexQueryCompositionProcessingData((CComplexAnsweringCompositionQuery*)complexAnsweringQuery, callback));
				return true;
			}



			bool COptimizedComplexCompositionAnsweringHandler::initializeProcessing(CAnswererContext* answererContext) {
				while (!mPendingQueryProcessingList.isEmpty()) {
					CComplexQueryCompositionProcessingData* queryData = mPendingQueryProcessingList.takeFirst();

					CAnsweringHandlingStatistics* stats = mOntoAnsweringItem->getAnsweringHandlingStatistics();
					if (stats) {
						CAnsweringHandlingStatistics* initStats = new CAnsweringHandlingStatistics();
						*initStats = *stats;
						queryData->setQueryProcessingInitializedAnsweringStatistics(initStats);
					}

					initializeQueryProcessing(queryData,answererContext);
				}
				return true;
			}



			bool COptimizedComplexCompositionAnsweringHandler::prepareProcessing(CAnswererContext* answererContext) {
				return true;
			}



			COptimizedComplexCompositionAnsweringHandler* COptimizedComplexCompositionAnsweringHandler::initializeQueryProcessing(CComplexQueryCompositionProcessingData* queryData, CAnswererContext* answererContext) {
				queryData->getQuery()->addErrorString("Processing of complex composition query not supported.");
				CCallbackData* callback = queryData->getCallback();
				if (callback) {
					callback->doCallback();
				}
				delete queryData;
				return this;
			}




			bool COptimizedComplexCompositionAnsweringHandler::continueCalculationCreation(CAnswererContext* answererContext) {
				bool processing = false;

				return processing;
			}




			bool COptimizedComplexCompositionAnsweringHandler::processCalculationMessage(CAnsweringMessageData* message, CAnswererContext* answererContext) {
				while (message) {
					CAnsweringMessageData::ANSWERINGMESSAGEDATA messageType = message->getAnsweringMessageDataType();
					if (messageType == CAnsweringMessageData::CALCULATIONCOMPLETED) {
						CAnsweringMessageDataCalculationCompleted* calcCompMessage = (CAnsweringMessageDataCalculationCompleted*)message;

						bool errorOcurred = false;
						if (calcCompMessage->hasCalculationError()) {
							LOG(ERROR, getDomain(), logTr("Error in computation, answer computation for ontology '%1' failed.").arg(mOntoAnsweringItem->getOntology()->getTerminologyName()), this);
							errorOcurred = true;
						}

						CAnsweringMessageDataCalculationCompleted::CALCULATIONTYPE calcType = calcCompMessage->getCalculationType();
						
						CAnsweringCalculationStatisticsCollection* statColl = calcCompMessage->getStatisticsCollection();
						if (statColl) {
							mOntoAnsweringItem->reuseCalculationStatisticsCollection(statColl);
						}

					} else if (messageType == CAnsweringMessageData::CALCULATIONADAPTER) {
						CAnsweringMessageDataCalculationAdapter* calcAdapMessage = (CAnsweringMessageDataCalculationAdapter*)message;
						CAnsweringMessageDataCalculationAdapter::CALCULATIONADAPTERTYPE calcType = calcAdapMessage->getCalculationAdapterType();
						
					}
					message = message->getNext();
				}
				return false;
			}



			bool COptimizedComplexCompositionAnsweringHandler::processRequirementMessage(CAnsweringMessageData* message, CAnswererContext* answererContext) {
				while (message) {
					CAnsweringMessageData::ANSWERINGMESSAGEDATA messageType = message->getAnsweringMessageDataType();
					if (messageType == CAnsweringMessageData::REQUIREMENTCOMPLETED) {
					}
					message = message->getNext();
				}
				return false;				
			}



			

			bool COptimizedComplexCompositionAnsweringHandler::isAnsweringFinished(CAnswererContext* answererContext) {
				return true;
			}





			CAnsweringProgress* COptimizedComplexCompositionAnsweringHandler::getAnsweringProgress() {


				if (mTotalPossibleInstancesCount != mTestedPossibleInstancesCount) {
					cint64 tested = mTestedPossibleInstancesCount;
					cint64 total = mTotalPossibleInstancesCount;
					cint64 elapsedTime = mAnsweringStartTime.elapsed();
					double remainingMilliSeconds = 0.;
					double progessPercent = 0.;
					if (elapsedTime <= 0 && total > 0) {
						remainingMilliSeconds = 0.;
					} else {
						remainingMilliSeconds = ((double)elapsedTime / (double)tested) * (total - tested);
					}
					if (total <= 0) {
						progessPercent = 0.;
					} else {
						progessPercent = ((double)tested / (double)total) * 100.;
					}
					mAnsweringProgress.setProgessPercent(progessPercent);
					mAnsweringProgress.setRemainingMilliSeconds(remainingMilliSeconds);
					mAnsweringProgress.setTestedInstances(tested);
					mAnsweringProgress.setTotalInstances(total);
				} else {
					mAnsweringProgress.setProgessPercent(0.);
					mAnsweringProgress.setRemainingMilliSeconds(0.);
					mAnsweringProgress.setTestedInstances(mTestedPossibleInstancesCount);
					mAnsweringProgress.setTotalInstances(mTotalPossibleInstancesCount);
				}
				return &mAnsweringProgress;
			}




			CAnsweringHandler* COptimizedComplexCompositionAnsweringHandler::storeAnsweringStatistics() {
				CAnsweringStatisticsCollectionStrings* stats = new CAnsweringStatisticsCollectionStrings();
				addAnsweringStatistics(stats, false);
				mOntoAnsweringItem->setDifferenceStoredAnsweringStatisticsCollectionStrings(stats);
				return this;
			}



			CAnsweringHandler* COptimizedComplexCompositionAnsweringHandler::addAnsweringStatistics(CAnsweringStatistics* stats, bool lastStoredDiff) {
				CAnsweringCalculationStatisticsCollectionStrings* calcStatsColl = mOntoAnsweringItem->getAnsweringStatisticsCollectionStrings();
				CAnsweringStatisticsCollectionStrings* diffStats = mOntoAnsweringItem->getDifferenceStoredAnsweringStatisticsCollectionStrings();
				if (calcStatsColl) {
					for (const QString& statName : calcStatsColl->getStatisticsNameStringList()) {
						cint64 value = calcStatsColl->getStatisticIntegerValue(statName);
						if (lastStoredDiff && diffStats) {
							value = value - diffStats->getStatisticIntegerValue(statName);
						}
						stats->addProcessingStatistics(statName, value);
					}
				}

				CAnsweringStatistics* tmpStats = stats;
				if (lastStoredDiff && diffStats) {
					tmpStats = new CAnsweringStatisticsCollectionStrings();
				}
				CAnsweringHandlingStatistics* handlingStats = mOntoAnsweringItem->getAnsweringHandlingStatistics();
				if (handlingStats) {
					stats->addProcessingStatistics("answering-processed-query-count", handlingStats->getTotalAnsweredQueriesCount());
				}
				if (lastStoredDiff && diffStats) {
					for (const QString& statName : tmpStats->getStatisticsNameStringList()) {
						cint64 value = tmpStats->getStatisticIntegerValue(statName);
						value = value - diffStats->getStatisticIntegerValue(statName);
						stats->addProcessingStatistics(statName, value);
					}
				}

				return this;
			}










		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
