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

#include "CIndividualMergingHashData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualMergingHashData::CIndividualMergingHashData() {
					mMergedIndi = false;
				}



				CIndividualMergingHashData* CIndividualMergingHashData::initIndividualMergingHashData(CIndividualMergingHashData* indiMergingHashData) {
					if (indiMergingHashData) {
						setDependencyTrackPoint(indiMergingHashData->getDependencyTrackPoint());
						mReapplyQueue.initReapplyQueue(indiMergingHashData->getReapplyQueue());
						mMergedIndi = indiMergingHashData->mMergedIndi;
					} else {
						setDependencyTrackPoint(nullptr);
						mReapplyQueue.initReapplyQueue(nullptr);
						mMergedIndi = false;
					}
					return this;
				}

				CCondensedReapplyQueue* CIndividualMergingHashData::getReapplyQueue() {
					return &mReapplyQueue;
				}


				bool CIndividualMergingHashData::isMergedWithIndividual() const {
					return mMergedIndi;
				}

				CIndividualMergingHashData* CIndividualMergingHashData::setMergedWithIndividual(bool merged) {
					mMergedIndi = merged;
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
