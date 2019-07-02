/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CRealizationProgress.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CRealizationProgress::CRealizationProgress() {
				resetValues();
			}


			qint64 CRealizationProgress::getRealizationCount() {
				return mRealizationCount;
			}

			qint64 CRealizationProgress::getTestedClasses() {
				return mTestedClasses;
			}

			qint64 CRealizationProgress::getTotalClasses() {
				return mTotalClasses;
			}

			double CRealizationProgress::getRemainingMilliSeconds() {
				return mRemainingMilliSeconds;
			}

			double CRealizationProgress::getProgessPercent() {
				return mProgessPercent;
			}

			CRealizationProgress *CRealizationProgress::setRealizationCount(qint64 classificationCount) {
				mRealizationCount = classificationCount;
				return this;
			}

			CRealizationProgress *CRealizationProgress::setTestedClasses(qint64 testedClasses) {
				mTestedClasses = testedClasses;
				return this;
			}

			CRealizationProgress *CRealizationProgress::setTotalClasses(qint64 totalClasses) {
				mTotalClasses = totalClasses;
				return this;
			}

			CRealizationProgress *CRealizationProgress::setRemainingMilliSeconds(double remainingMilliSeconds) {
				mRemainingMilliSeconds = remainingMilliSeconds;
				return this;
			}

			CRealizationProgress *CRealizationProgress::setProgessPercent(double progessPercent) {
				mProgessPercent = progessPercent;
				return this;
			}

			CRealizationProgress *CRealizationProgress::resetValues() {
				mRealizationCount = 0;
				mTestedClasses = 0;
				mTotalClasses = 0;
				mRemainingMilliSeconds = 0.;
				mProgessPercent = 0.;
				return this;
			}



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
