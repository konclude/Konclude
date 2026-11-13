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

// Libraries includes
#include <QString>
#include <QStringList>
#include <QList>

// Namespace includes
#include "CConformanceChannel.h"
#include "CConformanceTask.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceTestCase
			 *		\brief		A test case read from a *.kt.owl.xml control ontology.
			 *
			 *					The task kind and the expected answer are stored as typed
			 *					values; the literal text of the expected result is kept only
			 *					until the task kind is known, because it can only be
			 *					interpreted in the vocabulary of its task.
			 */
			class CConformanceTestCase {
				// public methods
				public:
					CConformanceTestCase();

				// public variables
				public:
					QString mId;                             // IRI/name of the test individual
					CConformanceTask::TYPE mTask;            // task kind, valid only when mHasTask is true
					bool mHasTask;                           // whether the ontology declared a task
					CConformanceChannel::TYPE mChannel;      // transport the test runs on (file or live server)
					bool mHasChannel;                        // whether the ontology declared kt:onChannel
					QString mInterface;                      // report label derived from mChannel
					QString mOntologyFile;                   // absolute path to the input ontology / request file
					QString mDatasetFile;                    // absolute path to the ontology a SPARQL test loads
					QStringList mExpectedResultTexts;        // expected result literals as read from the ontology
					QList<CConformanceTask::VERDICT> mExpectedVerdicts; // typed expectations, one per answer
					CConformanceTask::VERDICT mExpectedVerdict; // first expectation, used in reports
					QString mExpectedFile;                   // absolute path to an expected OWL/XML result (classification or realization)
					QString mTestClass;                      // class IRI for satisfiability (as declared in the ontology)
					QString mTestIndividual;                 // individual IRI for a restricted realization (optional)
					QString mControlFile;                    // originating *.kt.owl.xml (for messages)
					QString mSuiteId;                        // suite the control ontology declares/derives
					bool mKnownFailure;                      // marked via kt:knownFailure or kt:flaky
					QStringList mKnownIssueIds;              // issues the test is linked to
			};


			inline CConformanceTestCase::CConformanceTestCase()
					: mTask(CConformanceTask::TYPE::CONSISTENCY), mHasTask(false),
					  mChannel(CConformanceChannel::TYPE::COMMAND_LINE), mHasChannel(false),
					  mExpectedVerdict(CConformanceTask::VERDICT::UNSET), mKnownFailure(false) {
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTCASE_H
