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

#include "CReasonerEvaluationTerminationResult.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationTerminationResult::CReasonerEvaluationTerminationResult() {
				mInTimeTerminated = false;
				mTerminationForced = false;
				mProcessError = false;
				mMaxMemoryUsage = 0;
			}

			CReasonerEvaluationTerminationResult* CReasonerEvaluationTerminationResult::initResult(bool processError, bool inTimeTerminated, bool forcedTermination, const QString& errorText, const QString& stnOutText, const QString& errOutText, cint64 maxMemoryUsage) {
				mProcessError = processError;
				mInTimeTerminated = inTimeTerminated;
				mTerminationForced = forcedTermination;
				mStandardOutputText = stnOutText;
				mErrorOutputText = errOutText;
				mErrorText = errorText;
				mMaxMemoryUsage = maxMemoryUsage;
				return this;
			}


			bool CReasonerEvaluationTerminationResult::isInTimeTerminated() {
				return mInTimeTerminated;
			}

			bool CReasonerEvaluationTerminationResult::isTerminationForced() {
				return mTerminationForced;
			}

			bool CReasonerEvaluationTerminationResult::hasProcessError() {
				return mProcessError;
			}

			QString CReasonerEvaluationTerminationResult::getStandardOutputText() {
				return mStandardOutputText;
			}

			QString CReasonerEvaluationTerminationResult::getErrorOutputText() {
				return mErrorOutputText;
			}

			QString CReasonerEvaluationTerminationResult::getErrorText() {
				return mErrorText;
			}


			cint64 CReasonerEvaluationTerminationResult::getMaxMemoryUsage() {
				return mMaxMemoryUsage;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
