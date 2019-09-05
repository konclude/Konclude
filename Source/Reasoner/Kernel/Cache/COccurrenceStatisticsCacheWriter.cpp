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

#include "COccurrenceStatisticsCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceStatisticsCacheWriter::COccurrenceStatisticsCacheWriter(COccurrenceStatisticsCache* cache, COccurrenceStatisticsCacheData* data) : mCache(cache), mData(data) {
					mOntologyTag = -1;
					mOntologyData = nullptr;
				}


				COccurrenceStatisticsCacheWriter* COccurrenceStatisticsCacheWriter::writeCachedData(COccurrenceStatisticsCacheWriteData* writeData, CMemoryPool* memoryPools) {
					mCache->writeCachedData(writeData,memoryPools);
					return this;
				}


				COccurrenceStatisticsCacheWriter* COccurrenceStatisticsCacheWriter::loadOntologyDataVectors(CConcreteOntology* ontology) {
					cint64 ontologyId = ontology->getOntologyID();
					if (ontologyId != mOntologyTag) {
						mData->getReadWriteLock()->lockForWrite();
						if (mOntologyData) {
							mOntologyData->releaseWrittenConceptDataVector(mConceptDataVector);
							mOntologyData->releaseWrittenRoleDataVector(mRoleDataVector);
							mOntologyData->decUsageCount();
						}
						mOntologyData = mData->getOntologyData(ontologyId, true);
						mOntologyData->incUsageCount();
						mConceptDataVector = mOntologyData->getWriteableConceptDataVector(ontology->getTBox()->getConceptCount());
						mRoleDataVector = mOntologyData->getWriteableRoleDataVector(ontology->getRBox()->getRoleCount());
						mOntologyTag = ontologyId;
						mData->incUpdateId();
						mData->getReadWriteLock()->unlock();
					}
					return this;
				}


				COccurrenceStatisticsCacheWriter* COccurrenceStatisticsCacheWriter::incConceptInstanceOccurrencceStatistics(CConcreteOntology* ontology, cint64 conceptId, cint64 deterministicCount, cint64 nondeterministicCount, cint64 individualCount, cint64 existentialCount) {
					loadOntologyDataVectors(ontology);
					COccurrenceStatisticsConceptData* conceptData = mConceptDataVector->getOccurrenceStatisticsData(conceptId);
					if (conceptData) {
						if (deterministicCount != 0) {
							conceptData->incDeterministicInstanceOccurrencesCount(deterministicCount);
						}
						if (nondeterministicCount != 0) {
							conceptData->incNonDeterministicInstanceOccurrencesCount(nondeterministicCount);
						}
						if (existentialCount != 0) {
							conceptData->incExistentialInstanceOccurrencesCount(existentialCount);
						}
						if (individualCount != 0) {
							conceptData->incIndividualInstanceOccurrencesCount(individualCount);
						}
					}
					return this;
				}

				COccurrenceStatisticsCacheWriter* COccurrenceStatisticsCacheWriter::incRoleInstanceOccurrencceStatistics(CConcreteOntology* ontology, cint64 roleId, cint64 deterministicCount, cint64 nondeterministicCount, cint64 individualCount, cint64 existentialCount, cint64 outgoingCount, cint64 incomingCount) {
					loadOntologyDataVectors(ontology);
					COccurrenceStatisticsRoleData* roleData = mRoleDataVector->getOccurrenceStatisticsData(roleId);
					if (roleData) {
						if (deterministicCount != 0) {
							roleData->incDeterministicInstanceOccurrencesCount(deterministicCount);
						}
						if (nondeterministicCount != 0) {
							roleData->incNonDeterministicInstanceOccurrencesCount(nondeterministicCount);
						}
						if (existentialCount != 0) {
							roleData->incExistentialInstanceOccurrencesCount(existentialCount);
						}
						if (individualCount != 0) {
							roleData->incIndividualInstanceOccurrencesCount(individualCount);
						}
						if (outgoingCount != 0) {
							roleData->incOutgoingNodeInstanceOccurrencesCount(outgoingCount);
						}
						if (incomingCount != 0) {
							roleData->incIncomingNodeInstanceOccurrencesCount(incomingCount);
						}
					}
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
