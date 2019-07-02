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

#ifndef KONCLUDE_TEST_CREASONERTESTFILETESTRESULT_H
#define KONCLUDE_TEST_CREASONERTESTFILETESTRESULT_H

// Libraries includes


// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Test {


		/*! 
		 *
		 *		\class		CReasonerTestfileTestResult
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CReasonerTestfileTestResult {
			// public methods
			public:
				//! Constructor
				CReasonerTestfileTestResult();

				//! Destructor
				virtual ~CReasonerTestfileTestResult();

				virtual double getAverageCorrectResponseTime();
				virtual qint64 getCorrectResponseCount();
				virtual qint64 getCrashCount();
				virtual qint64 getErrorCount();
				virtual qint64 getTotalTestCount();
				virtual qint64 getTimeoutCount();
				virtual qint64 getWrongResponseCount();
				virtual qint64 getFailedCount();

				virtual CReasonerTestfileTestResult *addTestResult(bool crashed, bool timeout, bool error, bool correctResponse, double responseTime);
				virtual CReasonerTestfileTestResult *addFailedResult();

			// protected methods
			protected:

			// protected variables
			protected:
				double avgResponseTime;
				qint64 corrRespCount;
				qint64 totTestCount;
				qint64 crashCount;
				qint64 errorCount;
				qint64 timeoutCount;
				qint64 wrongRespCount;
				qint64 failedCount;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CREASONERTESTFILETESTRESULT_H
