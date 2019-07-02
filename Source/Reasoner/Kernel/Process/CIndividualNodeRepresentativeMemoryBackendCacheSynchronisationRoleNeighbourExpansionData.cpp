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

#include "CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData::CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData() {
					mNeighboursPossiblyInfluenced = false;
					mPossiblyNondeterministicallyInstantiated = false;
					mIndiIdLinker = nullptr;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData::areNeighboursPossiblyInfluenced() {
					return mNeighboursPossiblyInfluenced;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData::setNeighboursPossiblyInfluenced(bool possiblyInfluenced) {
					mNeighboursPossiblyInfluenced = possiblyInfluenced;
					return this;
				}


				CXLinker<cint64>* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData::getIndividualIdLinker() {
					return mIndiIdLinker;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData::setIndividualIdLinker(CXLinker<cint64>* indiIdLinker) {
					mIndiIdLinker = indiIdLinker;
					return this;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData::addIndividualIdLinker(CXLinker<cint64>* indiIdLinker) {
					mIndiIdLinker = indiIdLinker->append(mIndiIdLinker);
					return this;
				}


				bool CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData::isPossiblyNondeterministicallyInstantiated() {
					return mPossiblyNondeterministicallyInstantiated;
				}

				CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData* CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData::setPossiblyNondeterministicallyInstantiated(bool possiblyNondeterministicallyInstantiated) {
					mPossiblyNondeterministicallyInstantiated = possiblyNondeterministicallyInstantiated;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
