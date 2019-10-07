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

#include "CComplexConceptStepComputationProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CComplexConceptStepComputationProcess::CComplexConceptStepComputationProcess(ComplexConceptStepComputationProcessType computationType) {
				mComputationType = computationType;
				mFinished = false;
				mStarted = false;
				mRequired = false;
				mQueued = false;
				mProcessing = false;

				mCurrentlyRunningComputationCount = 0;
				mFinishedComputationCount = 0;
				mDerivedComputationCount = 0;
			}


			CComplexConceptStepComputationProcess::~CComplexConceptStepComputationProcess() {
			}


			ComplexConceptStepComputationProcessType CComplexConceptStepComputationProcess::getComputationType() {
				return mComputationType;
			}


			bool CComplexConceptStepComputationProcess::isComputationProcessFinished() {
				return mFinished;
			}

			bool CComplexConceptStepComputationProcess::isComputationProcessStarted() {
				return mStarted;
			}

			bool CComplexConceptStepComputationProcess::isComputationProcessQueued() {
				return mQueued;
			}

			bool CComplexConceptStepComputationProcess::isComputationProcessProcessing() {
				return mProcessing;
			}

			CComplexConceptStepComputationProcess* CComplexConceptStepComputationProcess::setComputationProcessStarted(bool started) {
				mStarted = started;
				return this;
			}

			CComplexConceptStepComputationProcess* CComplexConceptStepComputationProcess::setComputationProcessQueued(bool queued) {
				mQueued = queued;
				return this;
			}

			CComplexConceptStepComputationProcess* CComplexConceptStepComputationProcess::setComputationProcessFinished(bool finished) {
				mFinished = finished;
				return this;
			}

			CComplexConceptStepComputationProcess* CComplexConceptStepComputationProcess::setComputationProcessRequired(bool required) {
				mRequired = required;
				return this;
			}

			CComplexConceptStepComputationProcess* CComplexConceptStepComputationProcess::setComputationProcessProcessing(bool processing) {
				mProcessing = processing;
				return this;
			}

			bool CComplexConceptStepComputationProcess::isComputationProcessRequired() {
				return mRequired && !mFinished;
			}


			CComplexConceptStepComputationProcess* CComplexConceptStepComputationProcess::incCurrentlyRunningComputationCount(cint64 incCount) {
				mCurrentlyRunningComputationCount += incCount;
				return this;
			}

			CComplexConceptStepComputationProcess* CComplexConceptStepComputationProcess::decCurrentlyRunningComputationCount(cint64 decCount) {
				mCurrentlyRunningComputationCount -= decCount;
				return this;
			}

			CComplexConceptStepComputationProcess* CComplexConceptStepComputationProcess::incFinishedComputationCount(cint64 incCount) {
				mFinishedComputationCount += incCount;
				return this;
			}

			CComplexConceptStepComputationProcess* CComplexConceptStepComputationProcess::incDerivedComputationCount(cint64 incCount) {
				mDerivedComputationCount += incCount;
				return this;
			}


			cint64 CComplexConceptStepComputationProcess::getCurrentlyRunningComputationCount() {
				return mCurrentlyRunningComputationCount;
			}

			cint64 CComplexConceptStepComputationProcess::getFinishedComputationCount() {
				return mFinishedComputationCount;
			}


			cint64 CComplexConceptStepComputationProcess::getDerivedComputationCount() {
				return mDerivedComputationCount;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
