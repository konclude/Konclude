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

#ifndef KONCLUDE_TEST_CREASONERTESTFILETEST_H
#define KONCLUDE_TEST_CREASONERTESTFILETEST_H

// Libraries includes


// Namespace includes
#include "CReasonerTestfileTestResult.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Test {


		/*! 
		 *
		 *		\class		CReasonerTestfileTest
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CReasonerTestfileTest {
			// public methods
			public:
				//! Constructor
				CReasonerTestfileTest();

				//! Destructor
				virtual ~CReasonerTestfileTest();

				virtual QString getTestRequestFile();
				virtual CReasonerTestfileTest *setTestRequestFile(const QString &requestFile);
				virtual bool hasTestRequestFile();

				virtual QString getTestExpectedResponseFile();
				virtual CReasonerTestfileTest *setTestExpectedResponseFile(const QString &responseFile);
				virtual bool hasTestExpectedResponseFile();

				virtual qint64 getTestLoopCount();
				virtual CReasonerTestfileTest *setTestLoopCount(qint64 loopCount);

				virtual CReasonerTestfileTestResult *getTestResults();

				virtual QString getWorkingDirectory();
				virtual CReasonerTestfileTest *setWorkingDirectory(const QString &workingDir);
				virtual bool hasWorkingDirectory();

				virtual qint64 getTimeout();
				virtual CReasonerTestfileTest *setTimeout(qint64 testTimeout);


			// protected methods
			protected:

			// protected variables
			protected:
				QString testRequestFile;
				QString expectedResponseFile;
				qint64 testExeCount;
				QString workDir;
				qint64 timeout;

				CReasonerTestfileTestResult *testResult;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CREASONERTESTFILETEST_H
