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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYNOMINALINDIRECTCONNECTIONDATALINKER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYNOMINALINDIRECTCONNECTIONDATALINKER_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"

// Other includes
#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionExtensionDataLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker : public CLinkerBase<CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker*,CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker();

						CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* initNominalIndirectConnectionData(cint64 indiID);

						CXLinker<cint64>* getIndirectlyConnectedIndividualIdLinker();
						CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* setIndirectlyConnectedIndividualIdLinker(CXLinker<cint64>* indirectlyConnectedIndividualIdLinker);
						CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* addIndirectlyConnectedIndividualIdLinker(CXLinker<cint64>* indirectlyConnectedIndividualIdLinker);


						cint64 getIndividualID();

						cint64 getLastChangeIntegrationId();
						CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* setLastChangeIntegrationId(cint64 integrationID);


					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mNominalIndiID;
						cint64 mLastIntegrationID;


						CXLinker<cint64>* mIndirectlyConnectedIndividualIdLinker;


						cint64 mAssociationUpdateId;

					// private methods
					private:

					// private variables
					private:


				};


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYNOMINALINDIRECTCONNECTIONDATALINKER_H
