/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CReasonerEvaluationRequestResponse.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationRequestResponse::CReasonerEvaluationRequestResponse() {
				mResponseTime = 0;
				mTimedOut = false;
				mResponseError = false;
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

			CReasonerEvaluationRequestResponse* CReasonerEvaluationRequestResponse::setRequestNode(const QDomElement& reqNode) {
				mRequestNode = reqNode;
				return this;
			}

			CReasonerEvaluationRequestResponse* CReasonerEvaluationRequestResponse::setResponseNode(const QDomElement& resNode) {
				mResponseNode = resNode;
				return this;
			}

			CReasonerEvaluationRequestResponse* CReasonerEvaluationRequestResponse::setModifiedNode(const QDomElement& resNode) {
				mModifiedNode = resNode;
				return this;
			}


			cint64 CReasonerEvaluationRequestResponse::getResponseTime() {
				return mResponseTime;
			}

			QDomElement CReasonerEvaluationRequestResponse::getRequestNode() {
				return mRequestNode;
			}

			QDomElement CReasonerEvaluationRequestResponse::getResponseNode() {
				return mResponseNode;
			}

			QDomElement CReasonerEvaluationRequestResponse::getModifiedNode() {
				return mModifiedNode;
			}

			bool CReasonerEvaluationRequestResponse::isTimedOut() {
				return mTimedOut;
			}

			bool CReasonerEvaluationRequestResponse::hasResponseError() {
				return mResponseError;
			}
		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
