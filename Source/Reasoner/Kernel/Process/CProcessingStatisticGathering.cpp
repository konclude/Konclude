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

#include "CProcessingStatisticGathering.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CProcessingStatisticGathering::CProcessingStatisticGathering() {
					initProcessingStatisticsGathering(nullptr);
				}


				CProcessingStatisticGathering::~CProcessingStatisticGathering() {
				}



				CProcessingStatisticGathering* CProcessingStatisticGathering::initProcessingStatisticsGathering(CProcessingStatisticGathering* refStat) {
					if (refStat) {
						for (cint64 inx = 0; inx < CProcessingStatistics::statisticVectorSize; ++inx) {
							mStatVec[inx] += refStat->mStatVec[inx];
						}
					} else {
						for (cint64 inx = 0; inx < CProcessingStatistics::statisticVectorSize; ++inx) {
							mStatVec[inx] = 0;
						}
					}
					return this;
				}


				CProcessingStatisticGathering* CProcessingStatisticGathering::addGatheredProcessingStatisticsTo(CProcessingStatisticGathering* stat) {
					if (stat) {
						for (cint64 inx = 0; inx < CProcessingStatistics::statisticVectorSize; ++inx) {
							stat->mStatVec[inx] += mStatVec[inx];
						}
					}
					return this;
				}


				cint64* CProcessingStatisticGathering::getProcessingStatisticsVector() {
					return mStatVec;
				}

				cint64 CProcessingStatisticGathering::getGatheredProcessingStatistic(cint64 index) {
					if (index >= 0 && index < CProcessingStatistics::statisticVectorSize) {
						return mStatVec[index];
					}
					return 0;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
