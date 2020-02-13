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

#include "COptimizedComplexCompositionOntologyAnsweringItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexCompositionOntologyAnsweringItem::COptimizedComplexCompositionOntologyAnsweringItem(CConcreteOntology* ontology, CConfigurationBase* configuration) : COntologyAnsweringItem(ontology,configuration) {
				mCalcStatCollStrings = new CAnsweringCalculationStatisticsCollectionStrings();
				mDiffStoredStatCollStrings = nullptr;

				mConfCollectProcessStatistics = CConfigDataReader::readConfigBoolean(configuration, "Konclude.Calculation.Answering.CollectProcessStatistics", false);
				mConfLogProcessingProgressStatistics = CConfigDataReader::readConfigBoolean(configuration, "Konclude.Answering.LogCompositionQueriesProcessingProgressStatistics", false);
				mConfLogProcessingFinishedStatistics = CConfigDataReader::readConfigBoolean(configuration, "Konclude.Answering.LogCompositionQueriesProcessingFinishedStatistics", false);
				mCurrentlyAnsweringQueryCount = 0;

			}

			COptimizedComplexCompositionOntologyAnsweringItem::~COptimizedComplexCompositionOntologyAnsweringItem() {
				delete mDiffStoredStatCollStrings;
			}



			CAnsweringStatisticsCollectionStrings* COptimizedComplexCompositionOntologyAnsweringItem::getDifferenceStoredAnsweringStatisticsCollectionStrings() {
				return mDiffStoredStatCollStrings;
			}

			COptimizedComplexCompositionOntologyAnsweringItem* COptimizedComplexCompositionOntologyAnsweringItem::setDifferenceStoredAnsweringStatisticsCollectionStrings(CAnsweringStatisticsCollectionStrings* stats) {
				if (mDiffStoredStatCollStrings) {
					delete mDiffStoredStatCollStrings;
				}
				mDiffStoredStatCollStrings = stats;
				return this;
			}


			CAnsweringCalculationStatisticsCollectionStrings* COptimizedComplexCompositionOntologyAnsweringItem::getAnsweringStatisticsCollectionStrings() {
				return mCalcStatCollStrings;
			}


			CAnsweringCalculationStatisticsCollection* COptimizedComplexCompositionOntologyAnsweringItem::getCalculationStatisticsCollection() {
				CAnsweringCalculationStatisticsCollection* statColl = nullptr;
				if (!mReuseStatCollList.isEmpty()) {
					statColl = mReuseStatCollList.takeFirst();
				}
				if (!statColl && mCalcStatCollStrings) {
					statColl = mCalcStatCollStrings->createCalculationStatisticsCollection();
				}
				return statColl;
			}

			COptimizedComplexCompositionOntologyAnsweringItem* COptimizedComplexCompositionOntologyAnsweringItem::reuseCalculationStatisticsCollection(CAnsweringCalculationStatisticsCollection* statColl) {
				mReuseStatCollList.append(statColl);
				return this;
			}



			bool COptimizedComplexCompositionOntologyAnsweringItem::isProcessStatisticsCollectionConfigured() {
				return mConfCollectProcessStatistics;
			}



			bool COptimizedComplexCompositionOntologyAnsweringItem::isLogProcessingProgressStatisticsConfigured() {
				return mConfLogProcessingProgressStatistics;
			}


			bool COptimizedComplexCompositionOntologyAnsweringItem::isLogProcessingFinishedStatisticsConfigured() {
				return mConfLogProcessingFinishedStatistics;
			}

			CAnsweringHandlingStatistics* COptimizedComplexCompositionOntologyAnsweringItem::getAnsweringHandlingStatistics() {
				return &mAnsweringHandlingStatistics;
			}

			QList<CComplexQueryCompositionProcessingData*>* COptimizedComplexCompositionOntologyAnsweringItem::getScheduledQueriesProcessingDataList() {
				return &mScheduledQueriesProcDataList;
			}

			COptimizedComplexCompositionOntologyAnsweringItem* COptimizedComplexCompositionOntologyAnsweringItem::addScheduledQueriesProcessingData(CComplexQueryCompositionProcessingData* procData) {
				mScheduledQueriesProcDataList.append(procData);
				return this;
			}

			COptimizedComplexCompositionOntologyAnsweringItem* COptimizedComplexCompositionOntologyAnsweringItem::decCurrentlyAnsweringQueryCount() {
				mCurrentlyAnsweringQueryCount--;
				return this;
			}

			COptimizedComplexCompositionOntologyAnsweringItem* COptimizedComplexCompositionOntologyAnsweringItem::incCurrentlyAnsweringQueryCount() {
				mCurrentlyAnsweringQueryCount++;
				return this;
			}

			bool COptimizedComplexCompositionOntologyAnsweringItem::isCurrentlyAnsweringQueryCount() {
				return mCurrentlyAnsweringQueryCount;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
