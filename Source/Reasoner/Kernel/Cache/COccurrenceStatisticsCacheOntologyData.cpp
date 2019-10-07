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

#include "COccurrenceStatisticsCacheOntologyData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceStatisticsCacheOntologyData::COccurrenceStatisticsCacheOntologyData() {
					mUsageCounter.store(0);
					mConceptDataVecLinker = nullptr;
					mRoleDataVecLinker = nullptr;
				}


				COccurrenceStatisticsCacheOntologyData::~COccurrenceStatisticsCacheOntologyData() {
					CXLinker < COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>* >* conceptDataVecLinkerIt = mConceptDataVecLinker;
					while (conceptDataVecLinkerIt) {
						CXLinker < COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>* >* tmpConceptDataVecLinkerIt = conceptDataVecLinkerIt;
						conceptDataVecLinkerIt = conceptDataVecLinkerIt->getNext();
						delete tmpConceptDataVecLinkerIt->getData();
						delete tmpConceptDataVecLinkerIt;
					}

					CXLinker < COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>* >* roleDataVecLinkerIt = mRoleDataVecLinker;
					while (roleDataVecLinkerIt) {
						CXLinker < COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>* >* tmpRoleDataVecLinkerIt = roleDataVecLinkerIt;
						roleDataVecLinkerIt = roleDataVecLinkerIt->getNext();
						delete tmpRoleDataVecLinkerIt->getData();
						delete tmpRoleDataVecLinkerIt;
					}

				}

				cint64 COccurrenceStatisticsCacheOntologyData::getUsageCount() {
					return mUsageCounter;
				}

				COccurrenceStatisticsCacheOntologyData* COccurrenceStatisticsCacheOntologyData::incUsageCount() {
					mUsageCounter.ref();
					return this;
				}

				COccurrenceStatisticsCacheOntologyData* COccurrenceStatisticsCacheOntologyData::decUsageCount() {
					mUsageCounter.deref();
					return this;
				}

				COccurrenceStatisticsRoleData COccurrenceStatisticsCacheOntologyData::getAccummulatedRoleDataOccurrenceStatistics(cint64 id) {
					COccurrenceStatisticsRoleData accData;
					for (CXLinker < COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>* >* roleDataVecLinkerIt = mRoleDataVecLinker; roleDataVecLinkerIt; roleDataVecLinkerIt = roleDataVecLinkerIt->getNext()) {
						COccurrenceStatisticsRoleData* data = roleDataVecLinkerIt->getData()->getOccurrenceStatisticsData(id);
						if (data) {
							accData.incDeterministicInstanceOccurrencesCount(data->getDeterministicInstanceOccurrencesCount());
							accData.incNonDeterministicInstanceOccurrencesCount(data->getNonDeterministicInstanceOccurrencesCount());
							accData.incExistentialInstanceOccurrencesCount(data->getExistentialInstanceOccurrencesCount());
							accData.incIndividualInstanceOccurrencesCount(data->getIndividualInstanceOccurrencesCount());
							accData.incIncomingNodeInstanceOccurrencesCount(data->getIncomingNodeInstanceOccurrencesCount());
							accData.incOutgoingNodeInstanceOccurrencesCount(data->getOutgoingNodeInstanceOccurrencesCount());
						}
					}
					return accData;
				}

				COccurrenceStatisticsConceptData COccurrenceStatisticsCacheOntologyData::getAccummulatedConceptDataOccurrenceStatistics(cint64 id) {
					COccurrenceStatisticsConceptData accData;
					for (CXLinker < COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>* >* conceptDataVecLinkerIt = mConceptDataVecLinker; conceptDataVecLinkerIt; conceptDataVecLinkerIt = conceptDataVecLinkerIt->getNext()) {
						COccurrenceStatisticsConceptData* data = conceptDataVecLinkerIt->getData()->getOccurrenceStatisticsData(id);
						if (data) {
							accData.incDeterministicInstanceOccurrencesCount(data->getDeterministicInstanceOccurrencesCount());
							accData.incNonDeterministicInstanceOccurrencesCount(data->getNonDeterministicInstanceOccurrencesCount());
							accData.incExistentialInstanceOccurrencesCount(data->getExistentialInstanceOccurrencesCount());
							accData.incIndividualInstanceOccurrencesCount(data->getIndividualInstanceOccurrencesCount());
						}
					}
					return accData;
				}

				COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>* COccurrenceStatisticsCacheOntologyData::getWriteableConceptDataVector(cint64 conCount) {
					COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>* vec = nullptr;
					if (!mFreeConceptDataVecList.isEmpty()) {
						vec = mFreeConceptDataVecList.takeLast();
					}
					if (!vec) {
						vec = new COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>(conCount);
						CXLinker<COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>*>* linker = new CXLinker<COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData> *>(vec);
						linker->initLinker(vec, nullptr);
						mConceptDataVecLinker = linker->append(mConceptDataVecLinker);
					}
					return vec;
				}


				COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>* COccurrenceStatisticsCacheOntologyData::getWriteableRoleDataVector(cint64 roleCount) {
					COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>* vec = nullptr;
					if (!mFreeRoleDataVecList.isEmpty()) {
						vec = mFreeRoleDataVecList.takeLast();
					}
					if (!vec) {
						vec = new COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>(roleCount);
						CXLinker<COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>*>* linker = new CXLinker<COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData> *>();
						linker->initLinker(vec, nullptr);
						mRoleDataVecLinker = linker->append(mRoleDataVecLinker);
					}
					return vec;
				}

				COccurrenceStatisticsCacheOntologyData* COccurrenceStatisticsCacheOntologyData::releaseWrittenConceptDataVector(COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsConceptData>* vec) {
					mFreeConceptDataVecList.append(vec);
					return this;
				}

				COccurrenceStatisticsCacheOntologyData* COccurrenceStatisticsCacheOntologyData::releaseWrittenRoleDataVector(COccurrenceStatisticsCacheOntologyDataVector<COccurrenceStatisticsRoleData>* vec) {
					mFreeRoleDataVecList.append(vec);
					return this;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
