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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTESTSUITE_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTESTSUITE_H

// Libraries includes


// Namespace includes
#include "CReasonerEvaluationFiltering.h"


// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationTestsuite
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationTestsuite {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationTestsuite();

					CReasonerEvaluationTestsuite* initEvaluationTestsuite(const QString& globalConfig, const QString& reasonerConfig, const QString& testDirectory, const QString& evaluationDirectory, const QString& plattform, CReasonerEvaluationFiltering* evalFiltering);

					QString getGlobalConfigString();
					QString getReasonerConfigString();
					QString getTestsDirectoryString();
					QString getEvaluationDirectoryString();
					QString getPlatformString();

					CReasonerEvaluationFiltering* getEvaluationFiltering();

					CReasonerEvaluationTestsuite* setGlobalConfigString(const QString& string);
					CReasonerEvaluationTestsuite* setReasonerConfigString(const QString& string);
					CReasonerEvaluationTestsuite* setTestsDirectoryString(const QString& string);
					CReasonerEvaluationTestsuite* setEvaluationDirectoryString(const QString& string);
					CReasonerEvaluationTestsuite* setPlatformString(const QString& string);

					CReasonerEvaluationTestsuite* setEvaluationFiltering(CReasonerEvaluationFiltering* evalFiltering);

				// protected methods
				protected:

				// protected variables
				protected:
					QString mGlobalConfig;
					QString mReasonerConfig;
					QString mTestsDirectory;
					QString mEvaluationDirectory;
					QString mPlatform;

					CReasonerEvaluationFiltering* mFiltering;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTESTSUITE_H
