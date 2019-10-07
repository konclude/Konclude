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

#include "CPrecomputatorStatistics.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CPrecomputatorStatistics::CPrecomputatorStatistics(CPrecomputatorStatistics *nextPrecompuationStatistics) {
				resetValues();
				nextCompStat = nextPrecompuationStatistics;
			}


			qint64 CPrecomputatorStatistics::getTotalSaturationTaskCount() {
				return mTotalSaturationTasks;
			}

			CPrecomputatorStatistics *CPrecomputatorStatistics::incTotalSaturationTaskCount(qint64 incCount) {
				mTotalSaturationTasks += incCount;
				if (nextCompStat) {
					nextCompStat->incTotalSaturationTaskCount(incCount);
				}
				return this;
			}

			qint64 CPrecomputatorStatistics::getIndividualSaturationTaskCount() {
				return mIndividualSaturationTasks;
			}

			CPrecomputatorStatistics *CPrecomputatorStatistics::incIndividualSaturationTaskCount(qint64 incCount) {
				mIndividualSaturationTasks += incCount;
				if (nextCompStat) {
					nextCompStat->incIndividualSaturationTaskCount(incCount);
				}
				return this;
			}

			qint64 CPrecomputatorStatistics::getIndividualPrecomputationTaskCount() {
				return mIndividualPrecomputationTasks;
			}

			CPrecomputatorStatistics *CPrecomputatorStatistics::incIndividualPrecomputationTaskCount(qint64 incCount) {
				mIndividualPrecomputationTasks += incCount;
				if (nextCompStat) {
					nextCompStat->incIndividualPrecomputationTaskCount(incCount);
				}
				return this;
			}



			CPrecomputatorStatistics *CPrecomputatorStatistics::resetValues() {
				mTotalSaturationTasks = 0;
				mIndividualSaturationTasks = 0;
				mIndividualPrecomputationTasks = 0;
				return this;
			}


			CPrecomputatorStatistics *CPrecomputatorStatistics::appendStatistics(CPrecomputatorStatistics *statistics) {
				mTotalSaturationTasks += statistics->getTotalSaturationTaskCount();
				mIndividualSaturationTasks += statistics->getIndividualSaturationTaskCount();
				mIndividualPrecomputationTasks += statistics->getIndividualPrecomputationTaskCount();
				return this;
			}


		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
