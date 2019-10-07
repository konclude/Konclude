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

#include "CReasonerEvaluationEvaluatedCallbackContextData.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationEvaluatedCallbackContextData::CReasonerEvaluationEvaluatedCallbackContextData(bool failed) {
				mEvaluationRunned = false;
				mTestTimeout = 0;
				mTestCount = 0;
				mEvaluationFailed = failed;
			}

			CReasonerEvaluationEvaluatedCallbackContextData::CReasonerEvaluationEvaluatedCallbackContextData(bool runned, const QString& reasonerNameString, const QString& reasonerOutputDirString, cint64 testTimeout, cint64 testCount) {
				mEvaluationRunned = runned;
				mReasonerNameString = reasonerNameString;
				mReasonerOutputDirectoryString = reasonerOutputDirString;
				mTestTimeout = testTimeout;
				mTestCount = testCount;
				mEvaluationFailed = false;
			}

			bool CReasonerEvaluationEvaluatedCallbackContextData::getEvaluationRunned() {
				return mEvaluationRunned;
			}

			bool CReasonerEvaluationEvaluatedCallbackContextData::getEvaluationFailed() {
				return mEvaluationFailed;
			}

			CReasonerEvaluationEvaluatedCallbackContextData* CReasonerEvaluationEvaluatedCallbackContextData::getEvaluationRunned(bool runned) {
				mEvaluationRunned = runned;
				return this;
			}

			QString CReasonerEvaluationEvaluatedCallbackContextData::getReasonerNameString() {
				return mReasonerNameString;
			}

			QString CReasonerEvaluationEvaluatedCallbackContextData::getReasonerOutputDirectoryString() {
				return mReasonerOutputDirectoryString;
			}

			cint64 CReasonerEvaluationEvaluatedCallbackContextData::getTestTimeout() {
				return mTestTimeout;
			}

			cint64 CReasonerEvaluationEvaluatedCallbackContextData::getTestCount() {
				return mTestCount;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
