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

#include "CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData::CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData() {
				}


				CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData::~CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData() {
				}


				CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData* CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData::initUnsatisfiableOccurenceCacheRetrievalData(CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData* prevData) {
					if (prevData) {
						mLastRetCachingTag = prevData->mLastRetCachingTag;
						mLastRetConceptDes = prevData->mLastRetConceptDes;
					} else {
						mLastRetCachingTag = 0;
						mLastRetConceptDes = nullptr;
					}
					return this;
				}


				cint64 CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData::getLastRetrievalCachingTag() {
					return mLastRetCachingTag;
				}

				CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData* CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData::setLastRetrievalCachingTag(cint64 tag) {
					mLastRetCachingTag = tag;
					return this;
				}

				CConceptDescriptor* CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData::getLastRetrievalConceptDescriptor() {
					return mLastRetConceptDes;
				}

				CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData* CIndividualNodeUnsatisfiableOccurenceCacheRetrievalData::setLastRetrievalConceptDescriptor(CConceptDescriptor* conDes) {
					mLastRetConceptDes = conDes;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
