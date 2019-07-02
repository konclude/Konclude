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

#include "CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher::CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher() {
					mConceptSetLabel = nullptr;
					mNeighbourRoleCombinationLabel = nullptr;
					mDetCombinedDataRoleSetLabel = nullptr;
					mNonDetCombinedDataRoleSetLabel = nullptr;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher::CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher(CBackendRepresentativeMemoryLabelCacheItem* conceptSetLabel, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleCombinationLabel, CBackendRepresentativeMemoryLabelCacheItem* detCombinedDataRoleSetLabel, CBackendRepresentativeMemoryLabelCacheItem* nonDetCombinedDataRoleSetLabel) {
					mConceptSetLabel = conceptSetLabel;
					mNeighbourRoleCombinationLabel = neighbourRoleCombinationLabel;
					mDetCombinedDataRoleSetLabel = detCombinedDataRoleSetLabel;
					mNonDetCombinedDataRoleSetLabel = nonDetCombinedDataRoleSetLabel;
					computeHashValue();
				}


				cint64 CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher::getHashValue() const {
					return mHashValue;
				}


				void CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher::computeHashValue() {
					mHashValue = 0;
					mHashValue += (cint64)mConceptSetLabel;
					mHashValue += (cint64)mNeighbourRoleCombinationLabel;
					mHashValue += (cint64)mDetCombinedDataRoleSetLabel;
					mHashValue += (cint64)mNonDetCombinedDataRoleSetLabel;
				}


				bool CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher::operator==(const CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher& hasher) const {
					return mConceptSetLabel == hasher.mConceptSetLabel && mNeighbourRoleCombinationLabel == hasher.mNeighbourRoleCombinationLabel && mDetCombinedDataRoleSetLabel == hasher.mDetCombinedDataRoleSetLabel && mNonDetCombinedDataRoleSetLabel == hasher.mNonDetCombinedDataRoleSetLabel;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
