/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CSatisfiableCalculationJobsQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CSatisfiableCalculationJobsQuery::CSatisfiableCalculationJobsQuery() {
				mJobCalculationError = false;
				mQueryError = false;
				mJobCalcedNegative = false;
				mJobCalcedCount = 0;
				mJobTotalCount = 0;
				mQueryResult = nullptr;
				mQueryStats = nullptr;
				mConfig = nullptr;
			}


			CSatisfiableCalculationJobsQuery::~CSatisfiableCalculationJobsQuery() {
				qDeleteAll(mJobDataList);
				delete mQueryResult;
				delete mQueryStats;
				delete mConfig;
			}


			CSatisfiableCalculationJobsQuery* CSatisfiableCalculationJobsQuery::setCalculationConfiguration(CCalculationConfigurationExtension *calculationConfiguration) {
				mConfig = calculationConfiguration;
				return this;
			}

			CCalculationConfigurationExtension* CSatisfiableCalculationJobsQuery::getCalculationConfiguration() {
				return mConfig;
			}

			CQUERYLIST<CCalculationJob*> CSatisfiableCalculationJobsQuery::getJobList() {
				return *((CQUERYLIST<CCalculationJob*>*)&mJobDataList);
			}

			CQUERYLIST<CSatisfiableCalculationJob*> CSatisfiableCalculationJobsQuery::getSatisfiableCalculationJobList() {
				return mJobDataList;
			}

			QString CSatisfiableCalculationJobsQuery::getQueryName() {
				return mQueryName;
			}

			CSatisfiableCalculationJobsQuery* CSatisfiableCalculationJobsQuery::setQueryName(const QString& queryName) {
				mQueryName = queryName;
				return this;
			}

			QString CSatisfiableCalculationJobsQuery::getQueryString() {
				return mQueryString;
			}

			bool CSatisfiableCalculationJobsQuery::hasAnswer() {
				return mJobCalcedNegative || mJobCalcedCount >= mJobTotalCount;
			}

			bool CSatisfiableCalculationJobsQuery::hasError() {
				return mQueryError || CQuery::hasError();
			}

			bool CSatisfiableCalculationJobsQuery::getResult(bool queryPositive) {
				if (queryPositive) {
					return mJobCalcedNegative ? false : mJobCalcedCount>= mJobTotalCount;
				} else {
					return mJobCalcedNegative;
				}
			}


			CQueryResult* CSatisfiableCalculationJobsQuery::getQueryResult() {
				if (!mQueryResult) {					
					if (hasAnswer()) {
						mQueryResult = new CBooleanQueryResult(getResult());
					}
				}
				return mQueryResult;
			}



			QString CSatisfiableCalculationJobsQuery::getAnswerString() {
				if (hasAnswer()) {
					if (getResult() == true) {
						return mPositiveAnswerString;
					} else {
						return mNegativeAnswerString;
					}
				}
				return mNoAnswerString;
			}


			CQuery* CSatisfiableCalculationJobsQuery::addErrorString(const QString& errorString) {
				CQuery::addErrorString(errorString);
				mQueryError = true;
				return this;
			}



			CSatisfiableCalculationJobsQuery* CSatisfiableCalculationJobsQuery::setQueryString(const QString& text) {
				mQueryString = text;
				return this;
			}

			CSatisfiableCalculationJobsQuery* CSatisfiableCalculationJobsQuery::setAnswerString(const QString& positiveText, const QString& negativeText, const QString& noAnswerText) {
				mPositiveAnswerString = positiveText;
				mNegativeAnswerString = negativeText;
				mNoAnswerString = noAnswerText;
				return this;
			}

			bool CSatisfiableCalculationJobsQuery::canCancelRemainingJobs() {
				return mJobCalcedNegative;
			}


			CSatisfiableCalculationJobsQuery* CSatisfiableCalculationJobsQuery::addTestSatisfiableCalculationJob(CSatisfiableCalculationJob* satCalculationJob, bool mustBeSatisfiableForPositive) {
				mJobDataList.append(satCalculationJob);
				mJobTotalCount++;
				CSatisfiableCalculationJobQueryData* queryData = new CSatisfiableCalculationJobQueryData(this,mustBeSatisfiableForPositive);
				satCalculationJob->setQueryData(queryData);
				return this;
			}

			bool CSatisfiableCalculationJobsQuery::interpreteJobSatisfiableCalculation(CSatisfiableCalculationJob* satCalculationJob, bool satisfiable, bool calculationError, QString* newAnserString) {
				bool queryFinished = false;
				CSatisfiableCalculationJobQueryData* queryData = (CSatisfiableCalculationJobQueryData*)satCalculationJob->getQueryData();
				if (calculationError) {
					mQueryError = true;
					queryData->setJobCalculationError(true);
				} else {
					if (!queryData->isJobCalculated()) {
						mJobCalcedCount++;
					}
					queryData->setJobCalculated(true);
					queryData->setJobCalculationSatisfiable(satisfiable);
					if (queryData->needsJobSatisfiabilityForPositiveQueryAnswer() != satisfiable) {
						mJobCalcedNegative = true;
					}
				}
				if (hasAnswer() || hasError()) {
					queryFinished = true;
					if (newAnserString) {
						*newAnserString = getAnswerString();
					}
				}
				return queryFinished;
			}

			CSatisfiableCalculationJobsQuery* CSatisfiableCalculationJobsQuery::setQueryStatistics(CQueryStatistics* stats) {
				mQueryStats = stats;
				return this;
			}

			CQueryStatistics* CSatisfiableCalculationJobsQuery::getQueryStatistics() {
				return mQueryStats;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
