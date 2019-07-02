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

#include "COccurrenceStatisticsCacheReader.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceStatisticsCacheReader::COccurrenceStatisticsCacheReader(COccurrenceStatisticsCacheData* data) {
					mData = data;
					mOntologyUpdateId = 0;
					mOntologyTag = -1;
					mOntologyData = nullptr;
				}



				COccurrenceStatisticsRoleData COccurrenceStatisticsCacheReader::getAccummulatedRoleDataOccurrenceStatistics(cint64 ontologyId, cint64 roleId) {
					COccurrenceStatisticsRoleData data;
					loadOntologyDataVectors(ontologyId);
					if (mOntologyData) {
						data = mOntologyData->getAccummulatedRoleDataOccurrenceStatistics(roleId);
					}
					return data;
				}

				COccurrenceStatisticsConceptData COccurrenceStatisticsCacheReader::getAccummulatedConceptDataOccurrenceStatistics(cint64 ontologyId, cint64 conceptId) {
					COccurrenceStatisticsConceptData data;
					loadOntologyDataVectors(ontologyId);
					if (mOntologyData) {
						data = mOntologyData->getAccummulatedConceptDataOccurrenceStatistics(conceptId);
					}
					return data;
				}



				COccurrenceStatisticsCacheReader* COccurrenceStatisticsCacheReader::loadOntologyDataVectors(cint64 ontologyId) {
					if (ontologyId != mOntologyTag || !mOntologyData && mOntologyUpdateId != mData->getUpdateId()) {
						mData->getReadWriteLock()->lockForRead();
						if (mOntologyData) {
							mOntologyData->decUsageCount();
						}
						mOntologyData = mData->getOntologyData(ontologyId, false);
						mOntologyData->incUsageCount();
						mOntologyTag = ontologyId;
						mOntologyUpdateId = mData->getUpdateId();
						mData->getReadWriteLock()->unlock();
					}
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
