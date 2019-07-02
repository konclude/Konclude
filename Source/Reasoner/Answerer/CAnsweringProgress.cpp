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

#include "CAnsweringProgress.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringProgress::CAnsweringProgress() {
				resetValues();
			}

			qint64 CAnsweringProgress::getTestedInstances() {
				return mTestedInstances;
			}

			qint64 CAnsweringProgress::getTotalInstances() {
				return mTotalInstances;
			}

			double CAnsweringProgress::getRemainingMilliSeconds() {
				return mRemainingMilliSeconds;
			}

			double CAnsweringProgress::getProgessPercent() {
				return mProgessPercent;
			}


			CAnsweringProgress *CAnsweringProgress::setTestedInstances(qint64 testedInstances) {
				mTestedInstances = testedInstances;
				return this;
			}

			CAnsweringProgress *CAnsweringProgress::setTotalInstances(qint64 totalInstances) {
				mTotalInstances = totalInstances;
				return this;
			}

			CAnsweringProgress *CAnsweringProgress::setRemainingMilliSeconds(double remainingMilliSeconds) {
				mRemainingMilliSeconds = remainingMilliSeconds;
				return this;
			}

			CAnsweringProgress *CAnsweringProgress::setProgessPercent(double progessPercent) {
				mProgessPercent = progessPercent;
				return this;
			}

			CAnsweringProgress *CAnsweringProgress::resetValues() {
				mTestedInstances = 0;
				mTotalInstances = 0;
				mRemainingMilliSeconds = 0.;
				mProgessPercent = 0.;
				return this;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
