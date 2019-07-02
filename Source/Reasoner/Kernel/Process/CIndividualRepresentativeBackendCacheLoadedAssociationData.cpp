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

#include "CIndividualRepresentativeBackendCacheLoadedAssociationData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualRepresentativeBackendCacheLoadedAssociationData::CIndividualRepresentativeBackendCacheLoadedAssociationData() {
					mIndividualIntegrated = false;
					mIndirectlyConnectedIndividualsIntegrated = false;
					mLoadedIndiAssData = nullptr;
					mLoadedNomConnData = nullptr;
				}


				bool CIndividualRepresentativeBackendCacheLoadedAssociationData::isIndividualIntegrated() {
					return mIndividualIntegrated;
				}

				CIndividualRepresentativeBackendCacheLoadedAssociationData* CIndividualRepresentativeBackendCacheLoadedAssociationData::setIndividualIntegrated(bool integrated) {
					mIndividualIntegrated = integrated;
					return this;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CIndividualRepresentativeBackendCacheLoadedAssociationData::getLoadedIndividualAssociationData() {
					return mLoadedIndiAssData;
				}

				CIndividualRepresentativeBackendCacheLoadedAssociationData* CIndividualRepresentativeBackendCacheLoadedAssociationData::setLoadedIndividualAssociationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssoData) {
					mLoadedIndiAssData = indiAssoData;
					return this;
				}

				bool CIndividualRepresentativeBackendCacheLoadedAssociationData::hasIndirectlyConnectedIndividualsIntegration() {
					return mIndirectlyConnectedIndividualsIntegrated;
				}

				CIndividualRepresentativeBackendCacheLoadedAssociationData* CIndividualRepresentativeBackendCacheLoadedAssociationData::setIndirectlyConnectedIndividualsIntegration(bool integrated) {
					mIndirectlyConnectedIndividualsIntegrated = integrated;
					return this;
				}

				CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* CIndividualRepresentativeBackendCacheLoadedAssociationData::getLoadedNominalIndirectConnectionData() {
					return mLoadedNomConnData;
				}

				CIndividualRepresentativeBackendCacheLoadedAssociationData* CIndividualRepresentativeBackendCacheLoadedAssociationData::setLoadedNominalIndirectConnectionData(CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* nomConnData) {
					mLoadedNomConnData = nomConnData;
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
