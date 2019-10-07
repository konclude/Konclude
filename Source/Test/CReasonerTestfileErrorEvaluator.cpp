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

#include "CReasonerTestfileErrorEvaluator.h"


namespace Konclude {

	namespace Test {



		CReasonerTestfileErrorEvaluator::CReasonerTestfileErrorEvaluator() : CLogIdentifier("::Konclude::Test::ReasonerTestfileErrorEvaluator",this) {
		}


		CReasonerTestfileErrorEvaluator::~CReasonerTestfileErrorEvaluator() {
		}


		CReasonerTestfileEvaluator *CReasonerTestfileErrorEvaluator::evaluateTestfile(CReasonerTestfileTest *test, const QByteArray &resultingByteArray) {
			QDomDocument document;
			if (document.setContent(resultingByteArray,true)) {
				QDomElement documentEl(document.documentElement());
				checkTestResponseErrors(test->getTestResults(),&documentEl);
			} else {
				test->getTestResults()->addTestResult(false,false,false,false,0);
			}
			return this;
		}


		CReasonerTestfileErrorEvaluator *CReasonerTestfileErrorEvaluator::checkTestResponseErrors(CReasonerTestfileTestResult *testResult, QDomElement *node) {

			QDomElement documentElement(*node);

			QString docElemName = documentElement.tagName();
			if (docElemName == "ox:ResponseMessage" || docElemName == "ResponseMessage") {
				QDomElement responseElement = documentElement.firstChildElement();
				
				double totalResponseTime = 0;
				bool responseError = false;
				bool wrongResult = false;

				while (!responseElement.isNull()) {
					bool unknownElement = false;
					bool errorElement = false;
					bool hasResponseTime = false;
					qint64 responseTime = 0;
					QString respElemName = responseElement.tagName();
					if (respElemName == "ox:KB" || respElemName == "KB") {						
					} else if (respElemName == "ox:OK" || respElemName == "OK") {
					} else if (respElemName == "ox:Error" || respElemName == "Error") {
						QDomElement errResult = responseElement.firstChildElement("BooleanResponse");
						if (errResult.isNull()) {
							errResult = responseElement.firstChildElement("ox:BooleanResponse");
						}
						if (errResult.isNull()) {
							errorElement = true;
						} else {
							wrongResult = true;
							QString boolRespString = errResult.attribute("result");
							LOG(WARNING,getLogDomain(),logTr("WRONG result of tested reasoner is '%1'.").arg(boolRespString),this);
						}
					} else if (respElemName == "ox:BooleanResponse" || respElemName == "BooleanResponse") {
						QString respTimeString = responseElement.attribute("response-time","");
						if (!respTimeString.isEmpty()) {
							bool convertResponseTimeSuccess = false;
							responseTime = respTimeString.toLongLong(&convertResponseTimeSuccess);
							if (convertResponseTimeSuccess) {
								hasResponseTime = true;
							}
						}
						QString boolRespString = responseElement.attribute("result");
						LOG(INFO,getLogDomain(),logTr("Reasoner calculated correct result: '%1'.").arg(boolRespString),this);

					} else if (respElemName == "#text") {
						// ignoring text nodes
					} else if (respElemName == "#comment") {
						// ignoring comment nodes
					} else {
						unknownElement = true;
					}

					if (unknownElement || errorElement) {
						responseError = true;
					} 
					if (hasResponseTime) {
						totalResponseTime += responseTime;
					}
					responseElement = responseElement.nextSiblingElement();
				}

				testResult->addTestResult(false,false,responseError,!wrongResult,totalResponseTime);
			}
			return this;

		}


	}; // end namespace Test

}; // end namespace Konclude
