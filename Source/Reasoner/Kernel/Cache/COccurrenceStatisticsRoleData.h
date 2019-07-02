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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSROLEDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSROLEDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "COccurrenceStatisticsData.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		COccurrenceStatisticsRoleData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceStatisticsRoleData : public COccurrenceStatisticsData {
					// public methods
					public:
						//! Constructor
						COccurrenceStatisticsRoleData();

						~COccurrenceStatisticsRoleData();

						cint64 getOutgoingNodeInstanceOccurrencesCount();
						cint64 getIncomingNodeInstanceOccurrencesCount();

						COccurrenceStatisticsRoleData* incOutgoingNodeInstanceOccurrencesCount(cint64 incCount = 1);
						COccurrenceStatisticsRoleData* incIncomingNodeInstanceOccurrencesCount(cint64 incCount = 1);

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mOutgoingNodeInstanceOccurrencesCount;
						cint64 mIncomingNodeInstanceOccurrencesCount;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSROLEDATA_H
