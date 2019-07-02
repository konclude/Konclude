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

#include "CReasonerEvaluationRequestResponse.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {

			CReasonerEvaluationRequestResponse::CReasonerEvaluationRequestResponse() {
				mResponseTime = 0;
				mTimedOut = false;
				mResponseError = false;
				mEvaluationError = false;
			}

			CReasonerEvaluationRequestResponse* CReasonerEvaluationRequestResponse::setResponseTime(cint64 responseTime) {
				mResponseTime = responseTime;
				return this;
			}

			CReasonerEvaluationRequestResponse* CReasonerEvaluationRequestResponse::setTimedOut(bool timedOut) {
				mTimedOut = timedOut;
				return this;
			}

			CReasonerEvaluationRequestResponse* CReasonerEvaluationRequestResponse::setResponseError(bool responseError) {
				mResponseError = responseError;
				return this;
			}

			CReasonerEvaluationRequestResponse* CReasonerEvaluationRequestResponse::setEvaluationError(bool evaluationError) {
				mEvaluationError = evaluationError;
				return this;
			}


			CReasonerEvaluationRequestResponse* CReasonerEvaluationRequestResponse::setEvaluationErrorString(const QString& errorString) {
				mEvaluationErrorString = errorString;
				return this;
			}


			cint64 CReasonerEvaluationRequestResponse::getResponseTime() {
				return mResponseTime;
			}

			bool CReasonerEvaluationRequestResponse::isTimedOut() {
				return mTimedOut;
			}

			bool CReasonerEvaluationRequestResponse::hasResponseError() {
				return mResponseError;
			}

			bool CReasonerEvaluationRequestResponse::hasEvaluationError() {
				return mEvaluationError;
			}

			QString CReasonerEvaluationRequestResponse::getEvaluationErrorString() {
				return mEvaluationErrorString;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
