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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEOUTCOME_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEOUTCOME_H

// Libraries includes
#include <QString>
#include <QList>


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceOutcome
			 *		\brief		Classification of an executed conformance test case and the
			 *					textual form used by the console report.
			 *
			 *					This class is the single source of truth for the outcome
			 *					vocabulary; reports and the result ontology render outcomes
			 *					through it instead of carrying their own string tables.
			 */
			class CConformanceOutcome {
				// public types
				public:
					enum class TYPE {
						PASS,					//!< The observed answer equals the expected answer.
						KNOWN,					//!< The test fails/times out, but is a known issue.
						FAIL,					//!< The observed answer differs from the expected answer.
						FIXED,					//!< A known issue now succeeds and has to be cleaned up.
						TIMEOUT,				//!< The reasoner did not finish within the time limit.
						FLAKY					//!< Repeated runs produced different answers.
					};

				// public methods
				public:
					//! All outcomes, in the order used by the reports.
					static QList<TYPE> allTypes();

					//! Short upper-case identifier (e.g. "PASS"), also used in the
					//! result ontology and in the console report prefix.
					static QString typeToString(TYPE type);

					//! Human-readable description with the identifier, e.g.
					//! "passed (PASS)" or "known issues now fixed (FIXED)".
					static QString typeToDescription(TYPE type);

					//! True when the outcome turns a test run red.
					static bool typeIsFailing(TYPE type);
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEOUTCOME_H
