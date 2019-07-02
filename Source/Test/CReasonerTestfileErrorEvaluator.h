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

#ifndef KONCLUDE_TEST_CREASONERTESTFILEERROREVALUATOR_H
#define KONCLUDE_TEST_CREASONERTESTFILEERROREVALUATOR_H

// Libraries includes
#include <QDomDocument>

// Namespace includes
#include "CReasonerTestfileTest.h"
#include "CReasonerTestfileTestResult.h"
#include "CReasonerTestfileEvaluator.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Logger;

	namespace Test {


		/*! 
		 *
		 *		\class		CReasonerTestfileErrorEvaluator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CReasonerTestfileErrorEvaluator : public CReasonerTestfileEvaluator, public CLogIdentifier {

			// public methods
			public:
				//! Constructor
				CReasonerTestfileErrorEvaluator();

				//! Destructor
				virtual ~CReasonerTestfileErrorEvaluator();


				virtual CReasonerTestfileEvaluator *evaluateTestfile(CReasonerTestfileTest *test, const QByteArray &resultingByteArray);


			// protected methods
			protected:
				virtual CReasonerTestfileErrorEvaluator *checkTestResponseErrors(CReasonerTestfileTestResult *testResult, QDomElement *node);


			// protected variables
			protected:


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CREASONERTESTFILEERROREVALUATOR_H
