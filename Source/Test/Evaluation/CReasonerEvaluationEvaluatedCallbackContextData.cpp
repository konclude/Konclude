/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
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
