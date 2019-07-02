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

#include "CIndividualNodeSaturationBlockingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeSaturationBlockingData::CIndividualNodeSaturationBlockingData() {
				}


				CIndividualNodeSaturationBlockingData* CIndividualNodeSaturationBlockingData::initSaturationBlockingData(cint64 cachedConceptCount, CConceptDescriptor* lastConfConDes, CIndividualSaturationProcessNode* saturationNode) {
					mSaturationBlockedConceptCount = cachedConceptCount;
					mLastConfConDes = lastConfConDes;
					mSaturationNode = saturationNode;
					return this;
				}

				CIndividualNodeSaturationBlockingData* CIndividualNodeSaturationBlockingData::setSaturationBlockedConceptCount(cint64 cachedCount) {
					mSaturationBlockedConceptCount = cachedCount;
					return this;
				}

				cint64 CIndividualNodeSaturationBlockingData::getSaturationBlockedConceptCount() {
					return mSaturationBlockedConceptCount;
				}

				CConceptDescriptor* CIndividualNodeSaturationBlockingData::getLastConfirmedConceptDescriptior() {
					return mLastConfConDes;
				}


				CIndividualSaturationProcessNode* CIndividualNodeSaturationBlockingData::getSaturationIndividualNode() {
					return mSaturationNode;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
