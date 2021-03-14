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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMCARDINALITYEXTENSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMCARDINALITYEXTENSIONDATA_H

// Libraries includes
#include <QMap>

// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelCacheItemExtensionData.h"
#include "CBackendRepresentativeMemoryLabelCacheItemCardinalityData.h"


// Other includes
#include "Utilities/Memory/CObjectParameterizingAllocator.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {



				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryLabelCacheItemTagResolvingExtensionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData : public CBackendRepresentativeMemoryLabelCacheItemExtensionData {
					// public methods
					public:

						//! Constructor
						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData(CContext* context);

						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* initCardinalityExtensionData();


						CBackendRepresentativeMemoryLabelCacheItemCardinalityData* getRoleCardinalityData(cint64 roleTag);
						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* setRoleCardinalityData(cint64 roleTag, CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardinalityData);

						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>* getRoleCardinalityDataHash();

						// protected methods
					protected:

						// protected variables
					protected:
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>* mRoleCardinalityDataHash;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMCARDINALITYEXTENSIONDATA_H
