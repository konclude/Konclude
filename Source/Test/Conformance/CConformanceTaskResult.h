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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASKRESULT_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASKRESULT_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CConformanceTask.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceTaskResult
			 *		\brief		Outcome of one execution of a reasoner task: whether the
			 *					execution completed, the typed verdict it produced, whether
			 *					that verdict matches the expectation, and a human-readable
			 *					explanation when it did not.
			 *
			 *					The explanation is diagnostic text for the log and the
			 *					result ontology; it is never parsed to derive a verdict.
			 */
			class CConformanceTaskResult {
				// public types
				public:
					//! State of the execution, independent of the answer it produced.
					enum class STATUS {
						COMPLETED,				//!< The reasoner finished; mVerdict holds the answer.
						TIMEOUT,				//!< The reasoner was killed after the time limit.
						EXECUTION_ERROR			//!< The reasoner could not be run or wrote no answer.
					};

				// public methods
				public:
					CConformanceTaskResult();

					//! True when both results describe the same observation, i.e. the same
					//! status and the same verdict. Used to detect flaky repetitions.
					bool isSameObservationAs(const CConformanceTaskResult& other) const;

					//! True when the verdict equals the expectation.
					bool isMatched() const;

					//! Short human-readable answer for reports, e.g. "satisfiable",
					//! "timed out after 60 s" or the execution error message.
					QString describe(CConformanceTask::TYPE taskType) const;

				// public variables
				public:
					STATUS mStatus;
					CConformanceTask::VERDICT mVerdict;
					bool mMatched;
					QString mExplanation;
			};


			inline CConformanceTaskResult::CConformanceTaskResult()
					: mStatus(STATUS::EXECUTION_ERROR), mVerdict(CConformanceTask::VERDICT::UNSET), mMatched(false) {
			}


			inline bool CConformanceTaskResult::isSameObservationAs(const CConformanceTaskResult& other) const {
				return mStatus == other.mStatus && mVerdict == other.mVerdict;
			}


			inline bool CConformanceTaskResult::isMatched() const {
				return mMatched;
			}


			inline QString CConformanceTaskResult::describe(CConformanceTask::TYPE taskType) const {
				if (mStatus == STATUS::TIMEOUT) {
					return mExplanation.isEmpty() ? QString("timed out") : mExplanation;
				}
				if (mStatus == STATUS::EXECUTION_ERROR) {
					return mExplanation.isEmpty() ? QString("execution error") : mExplanation;
				}
				QString verdictText = CConformanceTask::verdictToString(taskType, mVerdict);
				return verdictText.isEmpty() ? QString("no verdict") : verdictText;
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETASKRESULT_H
