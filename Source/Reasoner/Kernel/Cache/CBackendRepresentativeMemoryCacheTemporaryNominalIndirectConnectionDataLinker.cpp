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

#include "CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker::CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker() : CLinkerBase<CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker*, CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker>(this) {
				}



				CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker::initNominalIndirectConnectionData(cint64 indiID) {
					mLastIntegrationID = 0;
					mNominalIndiID = indiID;
					mIndirectlyConnectedIndividualIdLinker = nullptr;
					return this;
				}

				CXLinker<cint64>* CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker::getIndirectlyConnectedIndividualIdLinker() {
					return mIndirectlyConnectedIndividualIdLinker;
				}

				CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker::setIndirectlyConnectedIndividualIdLinker(CXLinker<cint64>* indirectlyConnectedIndividualIdLinker) {
					mIndirectlyConnectedIndividualIdLinker = indirectlyConnectedIndividualIdLinker;
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker::addIndirectlyConnectedIndividualIdLinker(CXLinker<cint64>* indirectlyConnectedIndividualIdLinker) {
					mIndirectlyConnectedIndividualIdLinker = indirectlyConnectedIndividualIdLinker->append(mIndirectlyConnectedIndividualIdLinker);
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker::getIndividualID() {
					return mNominalIndiID;
				}


				cint64 CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker::getLastChangeIntegrationId() {
					return mLastIntegrationID;
				}

				CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker::setLastChangeIntegrationId(cint64 id) {
					mLastIntegrationID = id;
					return this;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
