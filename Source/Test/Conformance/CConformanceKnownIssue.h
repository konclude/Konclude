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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEKNOWNISSUE_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEKNOWNISSUE_H

// Libraries includes
#include <QString>


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceKnownIssue
			 *		\brief		Metadata of one issue declared in known-issues.owl.xml.
			 *
			 *					The issue id is the individual iri, the type is the
			 *					specific kt:ReasonerBug / kt:UnsupportedFeature /
			 *					kt:PerformanceIssue / kt:CorpusLimitation class, and the
			 *					description is the kt:description literal.
			 */
			class CConformanceKnownIssue {
				// public variables
				public:
					QString mId;
					QString mType;
					QString mDescription;
					QString mStatus;
					QString mTracker;

				// public methods
				public:
					CConformanceKnownIssue() {
					}
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEKNOWNISSUE_H
