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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHEONTOLOGYDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHEONTOLOGYDATA_H

// Libraries includes
#include <QAtomicInteger>
#include <QList>


// Namespace includes
#include "CacheSettings.h"
#include "COccurrenceStatisticsCacheOntologyDataVector.h"
#include "COccurrenceStatisticsConceptData.h"
#include "COccurrenceStatisticsRoleData.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		COccurrenceStatisticsCacheOntologyData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceStatisticsCacheOntologyData {
					// public methods
					public:
						//! Constructor
						COccurrenceStatisticsCacheOntologyData();

						~COccurrenceStatisticsCacheOntologyData();

						cint64 getUsageCount();
						COccurrenceStatisticsCacheOntologyData* incUsageCount();
						COccurrenceStatisticsCacheOntologyData* decUsageCount();


						COccurrenceStatisticsRoleData getAccummulatedRoleDataOccurrenceStatistics(cint64 id);
						COccurrenceStatisticsConceptData getAccummulatedConceptDataOccurrenceStatistics(cint64 id);


						COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>* getWriteableConceptDataVector(cint64 conCount);
						COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>* getWriteableRoleDataVector(cint64 roleCount);


						COccurrenceStatisticsCacheOntologyData* releaseWrittenConceptDataVector(COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>* vec);
						COccurrenceStatisticsCacheOntologyData* releaseWrittenRoleDataVector(COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>* vec);


					// protected methods
					protected:

					// protected variables
					protected:
						QAtomicInteger<cint64> mUsageCounter;

						CXLinker<COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>*>* mConceptDataVecLinker;
						CXLinker<COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>*>* mRoleDataVecLinker;

						QList<COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>*> mFreeConceptDataVecList;
						QList<COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>*> mFreeRoleDataVecList;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHEONTOLOGYDATA_H
