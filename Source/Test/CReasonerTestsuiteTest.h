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

#ifndef KONCLUDE_TEST_CREASONERTESTSUITETEST_H
#define KONCLUDE_TEST_CREASONERTESTSUITETEST_H

// Libraries includes


// Namespace includes
#include "CReasonerTestfileTest.h"
#include "CReasonerTestsuiteTestResult.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Test {


		/*! 
		 *
		 *		\class		CReasonerTestsuiteTest
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CReasonerTestsuiteTest {
			// public methods
			public:
				//! Constructor
				CReasonerTestsuiteTest();

				//! Destructor
				virtual ~CReasonerTestsuiteTest();

				virtual qint64 getDefaultTestLoopCount();
				virtual CReasonerTestsuiteTest *setDefaultTestLoopCount(qint64 loopCount);

				virtual QString getDefaultWorkingDirectory();
				virtual CReasonerTestsuiteTest *setDefaultWorkingDirectory(const QString &workingDir);
				virtual bool hasDefaultWorkingDirectory();

				virtual CReasonerTestfileTest *createTestfileTest();
				virtual QLinkedList<CReasonerTestfileTest *> getTestfileTestList();

				virtual qint64 getDefaultTimeout();
				virtual CReasonerTestsuiteTest *setDefaultTimeout(qint64 defTimeout);

				virtual CReasonerTestsuiteTestResult *getTestsuiteResult();

			// protected methods
			protected:

			// protected variables
			protected:
				qint64 testExeCount;
				QString workDir;
				qint64 timeout;

				QLinkedList<CReasonerTestfileTest *> testList;

				CReasonerTestsuiteTestResult testsuiteResult;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CREASONERTESTSUITETEST_H
