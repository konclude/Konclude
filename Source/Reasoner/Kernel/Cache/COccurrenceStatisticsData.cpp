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

#include "COccurrenceStatisticsData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceStatisticsData::COccurrenceStatisticsData() {
					mDeterministicInstanceOccurrencesCount = 0;
					mNonDeterministicInstanceOccurrencesCount = 0;
					mIndividualInstanceOccurrencesCount = 0;
					mExistentialInstanceOccurrencesCount = 0;
				}


				COccurrenceStatisticsData::~COccurrenceStatisticsData() {
				}

				cint64 COccurrenceStatisticsData::getDeterministicInstanceOccurrencesCount() {
					return mDeterministicInstanceOccurrencesCount;
				}

				cint64 COccurrenceStatisticsData::getNonDeterministicInstanceOccurrencesCount() {
					return mNonDeterministicInstanceOccurrencesCount;
				}

				cint64 COccurrenceStatisticsData::getIndividualInstanceOccurrencesCount() {
					return mIndividualInstanceOccurrencesCount;
				}

				cint64 COccurrenceStatisticsData::getExistentialInstanceOccurrencesCount() {
					return mExistentialInstanceOccurrencesCount;
				}


				COccurrenceStatisticsData* COccurrenceStatisticsData::incDeterministicInstanceOccurrencesCount(cint64 incCount) {
					mDeterministicInstanceOccurrencesCount += incCount;
					return this;
				}

				COccurrenceStatisticsData* COccurrenceStatisticsData::incNonDeterministicInstanceOccurrencesCount(cint64 incCount) {
					mNonDeterministicInstanceOccurrencesCount += incCount;
					return this;
				}

				COccurrenceStatisticsData* COccurrenceStatisticsData::incIndividualInstanceOccurrencesCount(cint64 incCount) {
					mIndividualInstanceOccurrencesCount += incCount;
					return this;
				}

				COccurrenceStatisticsData* COccurrenceStatisticsData::incExistentialInstanceOccurrencesCount(cint64 incCount) {
					mExistentialInstanceOccurrencesCount += incCount;
					return this;
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
