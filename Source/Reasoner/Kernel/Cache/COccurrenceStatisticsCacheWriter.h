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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHEWRITER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHEWRITER_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "COccurrenceStatisticsCache.h"
#include "COccurrenceStatisticsCacheData.h"
#include "COccurrenceStatisticsCacheWriteData.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {

				/*!
				*
				*		\class		COccurrenceStatisticsCacheWriter
				*		\author		Andreas Steigmiller
				*		\version	0.1
				*		\brief		TODO
				*
				*/
				class COccurrenceStatisticsCacheWriter {
					// public methods
					public:
						//! Constructor
						COccurrenceStatisticsCacheWriter(COccurrenceStatisticsCache* cache, COccurrenceStatisticsCacheData* data);


						COccurrenceStatisticsCacheWriter* writeCachedData(COccurrenceStatisticsCacheWriteData* writeData, CMemoryPool* memoryPools);


						COccurrenceStatisticsCacheWriter* incConceptInstanceOccurrencceStatistics(CConcreteOntology* ontology, cint64 conceptId, cint64 deterministicCount, cint64 nondeterministicCount, cint64 individualCount, cint64 existentialCount);
						COccurrenceStatisticsCacheWriter* incRoleInstanceOccurrencceStatistics(CConcreteOntology* ontology, cint64 roleId, cint64 deterministicCount, cint64 nondeterministicCount, cint64 individualCount, cint64 existentialCount, cint64 outgoingCount, cint64 incomingCount);

					// protected methods
					protected:
						COccurrenceStatisticsCacheWriter* loadOntologyDataVectors(CConcreteOntology* ontology);


					// protected variables
					protected:
						COccurrenceStatisticsCache* mCache;
						COccurrenceStatisticsCacheData* mData;

						cint64 mOntologyTag;
						COccurrenceStatisticsCacheOntologyData* mOntologyData;
						COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>* mConceptDataVector;
						COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>* mRoleDataVector;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHEWRITER_H
