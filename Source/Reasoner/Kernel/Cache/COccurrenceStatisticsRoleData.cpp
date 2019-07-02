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

#include "COccurrenceStatisticsRoleData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceStatisticsRoleData::COccurrenceStatisticsRoleData() {
					mOutgoingNodeInstanceOccurrencesCount = 0;
					mIncomingNodeInstanceOccurrencesCount = 0;
				}

				COccurrenceStatisticsRoleData::~COccurrenceStatisticsRoleData() {
				}

				cint64 COccurrenceStatisticsRoleData::getOutgoingNodeInstanceOccurrencesCount() {
					return mOutgoingNodeInstanceOccurrencesCount;
				}

				cint64 COccurrenceStatisticsRoleData::getIncomingNodeInstanceOccurrencesCount() {
					return mIncomingNodeInstanceOccurrencesCount;
				}

				COccurrenceStatisticsRoleData* COccurrenceStatisticsRoleData::incOutgoingNodeInstanceOccurrencesCount(cint64 incCount) {
					mOutgoingNodeInstanceOccurrencesCount += incCount;
					return this;
				}

				COccurrenceStatisticsRoleData* COccurrenceStatisticsRoleData::incIncomingNodeInstanceOccurrencesCount(cint64 incCount) {
					mIncomingNodeInstanceOccurrencesCount += incCount;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
