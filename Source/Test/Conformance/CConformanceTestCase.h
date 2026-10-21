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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTCASE_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTCASE_H

#include <QString>

namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceTestCase
			 *		\brief		A test case read from a *.kt.owl.xml control ontology.
			 *					Supported tasks: consistency, satisfiability, owllink-boolean.
			 */
			class CConformanceTestCase {
				public:
					CConformanceTestCase() : mKnownFailure(false), mTimeoutSeconds(0) {}

					QString mId;                 // IRI/name of the test individual
					QString mTask;               // "consistency" | "satisfiability" | "owllink-boolean" | "classification"
					QString mInterface;          // interface the test runs on ("CommandLine" | "OWLlink" | ...)
					QString mOntologyFile;       // absolute path to the input ontology / request file
					QString mExpectedResult;     // consistent/inconsistent | satisfiable/unsatisfiable | true/false
					QString mExpectedFile;       // absolute path to an expected OWL/XML classification result (task "classification")
					QString mTestClass;          // class IRI for satisfiability (exactly as declared in the ontology)
					QString mControlFile;        // originating *.kt.owl.xml (for messages)
					bool mKnownFailure;          // marked via kt:knownFailure or kt:flaky
					qint64 mTimeoutSeconds;      // 0 => runner default
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTCASE_H
