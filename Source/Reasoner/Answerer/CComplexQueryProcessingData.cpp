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

#include "CComplexQueryProcessingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CComplexQueryProcessingData::CComplexQueryProcessingData(CComplexAnsweringQuery* query, CCallbackData* callback) {
				mComplexAnsweringQuery = query;
				mCallback = callback;
				mUnsatisfiable = false;
				mQueryProcessingInitializedAnsweringStatistics = nullptr;
			}


			CComplexQueryProcessingData::~CComplexQueryProcessingData() {
				if (mQueryProcessingInitializedAnsweringStatistics) {
					delete mQueryProcessingInitializedAnsweringStatistics;
				}
			}

			CComplexAnsweringQuery* CComplexQueryProcessingData::getComplexAnsweringQuery() {
				return mComplexAnsweringQuery;
			}

			CCallbackData* CComplexQueryProcessingData::getCallback() {
				return mCallback;
			}



			bool CComplexQueryProcessingData::isUnsatisfiable() {
				return mUnsatisfiable;
			}

			CComplexQueryProcessingData* CComplexQueryProcessingData::setUnsatisfiable(bool unsat) {
				mUnsatisfiable = unsat;
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


			CComplexQueryProcessingData* CComplexQueryProcessingData::setSubQueryProcessingFinished(bool finished) {
				mSubQueryProcessingTime = mProcessingTimer.elapsed();
				mSubQueryProcessingFinished = true;
				return this;
			}

			bool CComplexQueryProcessingData::isSubQueryProcessingFinished() {
				return mSubQueryProcessingFinished;
			}


			qint64 CComplexQueryProcessingData::getTotalProcessingTime() {
				return mProcessingTime;
			}


			qint64 CComplexQueryProcessingData::getSubQueryProcessingTime() {
				return mSubQueryProcessingTime;
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
