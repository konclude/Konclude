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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		COccurrenceStatisticsData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceStatisticsData {
					// public methods
					public:
						//! Constructor
						COccurrenceStatisticsData();

						~COccurrenceStatisticsData();


						cint64 getDeterministicInstanceOccurrencesCount();
						cint64 getNonDeterministicInstanceOccurrencesCount();

						cint64 getIndividualInstanceOccurrencesCount();
						cint64 getExistentialInstanceOccurrencesCount();



						COccurrenceStatisticsData* incDeterministicInstanceOccurrencesCount(cint64 incCount = 1);
						COccurrenceStatisticsData* incNonDeterministicInstanceOccurrencesCount(cint64 incCount = 1);

						COccurrenceStatisticsData* incIndividualInstanceOccurrencesCount(cint64 incCount = 1);
						COccurrenceStatisticsData* incExistentialInstanceOccurrencesCount(cint64 incCount = 1);



					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mDeterministicInstanceOccurrencesCount;
						cint64 mNonDeterministicInstanceOccurrencesCount;

						cint64 mIndividualInstanceOccurrencesCount;
						cint64 mExistentialInstanceOccurrencesCount;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSDATA_H
