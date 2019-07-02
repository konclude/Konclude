/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGSTATISTICGATHERING_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGSTATISTICGATHERING_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessingStatistics.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

#define STATDATAINC(STATDESC,processingStatisticGatheringData) \
			if (processingStatisticGatheringData) { \
				processingStatisticGatheringData->incProcessingStatistic(CProcessingStatistics::stat##STATDESC.getStatisticGatheringIndex()); \
			}
#define STATDATAINCM(STATDESC,incCount,processingStatisticGatheringData) \
			if (processingStatisticGatheringData) { \
				processingStatisticGatheringData->incProcessingStatistic(CProcessingStatistics::stat##STATDESC.getStatisticGatheringIndex(),incCount); \
			}

				/*! 
				 *
				 *		\class		CProcessingStatisticGathering
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CProcessingStatisticGathering {
					// public methods
					public:
						//! Constructor
						CProcessingStatisticGathering();

						//! Destructor
						virtual ~CProcessingStatisticGathering();

						inline CProcessingStatisticGathering* incProcessingStatistic(cint64 index);
						inline CProcessingStatisticGathering* incProcessingStatistic(cint64 index, cint64 incCount);

						CProcessingStatisticGathering* initProcessingStatisticsGathering(CProcessingStatisticGathering* refStat);

						CProcessingStatisticGathering* addGatheredProcessingStatisticsTo(CProcessingStatisticGathering* stat);
						cint64 getGatheredProcessingStatistic(cint64 index);

						cint64* getProcessingStatisticsVector();


					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mStatVec[CProcessingStatistics::statisticVectorSize];

					// private methods
					private:

					// private variables
					private:

				};


				CProcessingStatisticGathering* CProcessingStatisticGathering::incProcessingStatistic(cint64 index) {
					if (index >= 0 && index < CProcessingStatistics::statisticVectorSize) {
						++mStatVec[index];
					}
					return this;
				}

				CProcessingStatisticGathering* CProcessingStatisticGathering::incProcessingStatistic(cint64 index, cint64 incCount) {
					if (index >= 0 && index < CProcessingStatistics::statisticVectorSize) {
						mStatVec[index] += incCount;
					}
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGSTATISTICGATHERING_H
