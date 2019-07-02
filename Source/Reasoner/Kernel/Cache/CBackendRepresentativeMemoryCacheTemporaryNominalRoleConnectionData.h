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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYNOMINALROLECONNECTIONDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYNOMINALROLECONNECTIONDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelCacheItem.h"
#include "CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker.h"
#include "CBackendRepresentativeMemoryCacheTemporaryLabelReference.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"
#include "Reasoner/Ontology/CIndividualReference.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {


				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData : public CLinkerBase<CRole*,CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData();

						CBackendRepresentativeMemoryCacheTemporaryNominalRoleConnectionData* initNominalRoleConnectionData(CRole* connectionRole, bool inversedConnection, CIndividualReference connectedIndi);


						CRole* getConnectionRole();
						CIndividualReference getConnectedIndividual();
						bool isInversedConnection();

					// protected methods
					protected:

					// protected variables
					protected:
						CIndividualReference mConnectedIndividual;
						bool mInversedConnection;

					// private methods
					private:

					// private variables
					private:


				};


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYNOMINALROLECONNECTIONDATA_H
