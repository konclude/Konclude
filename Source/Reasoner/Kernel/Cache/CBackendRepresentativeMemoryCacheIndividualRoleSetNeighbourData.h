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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEINDIVIDUALROLESETNEIGHBOURDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEINDIVIDUALROLESETNEIGHBOURDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData();

						bool visitNeighbourIndividualIds(function<bool(cint64 id)> visitFunc);
						bool visitNeighbourIndividualIdsFromCursor(function<bool(cint64 id, cint64 cursor)> visitFunc, cint64 cursor = 0);

						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* getIndividualIdLinker();
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData* setIndividualIdLinker(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinker);
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData* addIndividualIdLinker(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinker);
						cint64 getIndividualCount();

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mCount;
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* mIndiIdLinker;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEINDIVIDUALROLESETNEIGHBOURDATA_H
