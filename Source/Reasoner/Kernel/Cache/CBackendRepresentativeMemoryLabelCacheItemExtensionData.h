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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMEXTENSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMEXTENSIONDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryCacheContext.h"


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
				 *		\class		CBackendRepresentativeMemoryLabelCacheItemExtensionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryLabelCacheItemExtensionData : public CLinkerBase<cint64,CBackendRepresentativeMemoryLabelCacheItemExtensionData> {
					// public methods
					public:

						enum LABEL_CACHE_ITEM_EXTENSION_TYPE {
							INDIVIDUAL_ASSOCIATION_MAP = 0,
							INDIVIDUAL_NEIGHBOUR_ARRAY_INDEX = 1,
							TAG_RESOLVING_HASH = 2,
							CARDINALITY_HASH = 2, // can use the same index as TAG_RESOLVING_HASH since they should never be used toghether on the same label item type
						};

						static const cint64 LABEL_CACHE_ITEM_EXTENSION_TYPE_COUNT = 3;


						//! Constructor
						CBackendRepresentativeMemoryLabelCacheItemExtensionData(CBackendRepresentativeMemoryCacheContext* context);


						LABEL_CACHE_ITEM_EXTENSION_TYPE getExtensionType();

					// protected methods
					protected:

					// protected variables
					protected:
						CBackendRepresentativeMemoryCacheContext* mContext;

						LABEL_CACHE_ITEM_EXTENSION_TYPE mCacheItemExtensionType;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMEXTENSIONDATA_H
