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

#include "CReasonerEvaluationTestsuite.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationTestsuite::CReasonerEvaluationTestsuite() {
			}

			CReasonerEvaluationTestsuite* CReasonerEvaluationTestsuite::initEvaluationTestsuite(const QString& globalConfig, const QString& reasonerConfig, const QString& testDirectory, const QString& evaluationDirectory, const QString& plattform, CReasonerEvaluationFiltering* evalFiltering) {
				mGlobalConfig = globalConfig;
				mReasonerConfig = reasonerConfig;
				mTestsDirectory = testDirectory;
				mEvaluationDirectory = evaluationDirectory;
				mPlatform = plattform;
				mFiltering = evalFiltering;
				return this;
			}

			QString CReasonerEvaluationTestsuite::getGlobalConfigString() {
				return mGlobalConfig;
			}

			QString CReasonerEvaluationTestsuite::getReasonerConfigString() {
				return mReasonerConfig;
			}

			QString CReasonerEvaluationTestsuite::getTestsDirectoryString() {
				return mTestsDirectory;
			}

			QString CReasonerEvaluationTestsuite::getEvaluationDirectoryString() {
				return mEvaluationDirectory;
			}

			QString CReasonerEvaluationTestsuite::getPlatformString() {
				return mPlatform;
			}

			CReasonerEvaluationFiltering* CReasonerEvaluationTestsuite::getEvaluationFiltering() {
				return mFiltering;
			}

			CReasonerEvaluationTestsuite* CReasonerEvaluationTestsuite::setGlobalConfigString(const QString& string) {
				mGlobalConfig = string;
				return this;
			}

			CReasonerEvaluationTestsuite* CReasonerEvaluationTestsuite::setReasonerConfigString(const QString& string) {
				mReasonerConfig = string;
				return this;
			}

			CReasonerEvaluationTestsuite* CReasonerEvaluationTestsuite::setTestsDirectoryString(const QString& string) {
				mTestsDirectory = string;
				return this;
			}

			CReasonerEvaluationTestsuite* CReasonerEvaluationTestsuite::setEvaluationDirectoryString(const QString& string) {
				mEvaluationDirectory = string;
				return this;
			}

			CReasonerEvaluationTestsuite* CReasonerEvaluationTestsuite::setPlatformString(const QString& string) {
				mPlatform = string;
				return this;
			}

			CReasonerEvaluationTestsuite* CReasonerEvaluationTestsuite::setEvaluationFiltering(CReasonerEvaluationFiltering* evalFiltering) {
				mFiltering = evalFiltering;
				return this;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
