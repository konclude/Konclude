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

#include "CReasonerTestfileTestResult.h"


namespace Konclude {

	namespace Test {



		CReasonerTestfileTestResult::CReasonerTestfileTestResult() {
			avgResponseTime = 0;
			corrRespCount = 0;
			totTestCount = 0;
			crashCount = 0;
			timeoutCount = 0;
			wrongRespCount = 0;
			failedCount = 0;
			errorCount = 0;
		}


		CReasonerTestfileTestResult::~CReasonerTestfileTestResult() {
		}

		double CReasonerTestfileTestResult::getAverageCorrectResponseTime() {
			return avgResponseTime;
		}

		qint64 CReasonerTestfileTestResult::getCorrectResponseCount() {
			return corrRespCount;
		}

		qint64 CReasonerTestfileTestResult::getWrongResponseCount() {
			return wrongRespCount;
		}

		qint64 CReasonerTestfileTestResult::getFailedCount() {
			return failedCount;
		}

		qint64 CReasonerTestfileTestResult::getCrashCount() {
			return crashCount;
		}

		qint64 CReasonerTestfileTestResult::getErrorCount() {
			return errorCount;
		}

		qint64 CReasonerTestfileTestResult::getTimeoutCount() {
			return timeoutCount;
		}

		qint64 CReasonerTestfileTestResult::getTotalTestCount() {
			return totTestCount;
		}


		CReasonerTestfileTestResult *CReasonerTestfileTestResult::addFailedResult() {
			totTestCount++;
			failedCount++;
			return this;
		}


		CReasonerTestfileTestResult *CReasonerTestfileTestResult::addTestResult(bool crashed, bool timeout, bool error, bool correctResponse, double responseTime) {
			totTestCount++;
			if (crashed) {
				crashCount++;
				if (timeout) {
					timeoutCount++;
				}
			} else if (timeout) {
				timeoutCount++;
			} else if (error) {
				errorCount++;
			} else {
				if (correctResponse) {
					corrRespCount++;
					avgResponseTime = avgResponseTime + (1./corrRespCount) * (responseTime - avgResponseTime);
				}
			}
			return this;
		}




	}; // end namespace Test

}; // end namespace Konclude
