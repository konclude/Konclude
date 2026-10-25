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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTSUITE_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTSUITE_H

// Libraries includes
#include <QString>


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceTestSuite
			 *		\brief		A named group of test cases.
			 *
			 *					A suite is declared explicitly in the control ontology of the
			 *					directory it represents with ClassAssertion(kt:TestSuite, <suite>),
			 *					optionally annotated with kt:suiteName and kt:suiteDirectory.
			 *					Without a declaration the runner derives the suite from the
			 *					directory name, so existing control ontologies keep working.
			 *
			 *					Every test of that ontology belongs to the suite; a test may state
			 *					it, or point at another suite, with the object property
			 *					kt:memberOfSuite.
			 *
			 *					Run configurations (kt:usesTestRunConfig) and reasoner
			 *					configurations (kt:usesTestSystemConfig) assigned to a suite apply to all
			 *					tests of that suite unless a test adds its own assignment.
			 */
			class CConformanceTestSuite {
				// public methods
				public:
					CConformanceTestSuite();

					//! Label used in reports, without whitespace (e.g.
					//! "DL-corner-cases"). Falls back to the local part of mId.
					QString getDisplayName() const;

				// public variables
				public:
					QString mId;				//!< individual IRI of the suite (kt:suite-...)
					QString mName;				//!< optional human-readable name (kt:suiteName)
					QString mDirectory;			//!< directory relative to the input root
			};


			inline CConformanceTestSuite::CConformanceTestSuite() {
			}


			inline QString CConformanceTestSuite::getDisplayName() const {
				QString name = mName;
				if (name.isEmpty()) {
					int colonIndex = mId.lastIndexOf(':');
					name = colonIndex >= 0 ? mId.mid(colonIndex + 1) : mId;
				}
				name.replace(' ', '-');
				return name;
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTSUITE_H
