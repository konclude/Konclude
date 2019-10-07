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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHEWRITEDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHEWRITEDATA_H

// Libraries includes
#include <QList>

// Namespace includes
#include "CacheSettings.h"
#include "CCacheEntryWriteData.h"
#include "CCacheValue.h"

// Other includes
#include "Utilities/Memory/CMemoryPool.h"
#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				/*! 
				 *
				 *		\class		CSaturationNodeAssociatedExpansionCacheWriteData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationNodeAssociatedExpansionCacheWriteData : public CCacheEntryWriteData {
					// public methods
					public:
						//! Constructor
						CSaturationNodeAssociatedExpansionCacheWriteData();

						enum SATURATIONNODEASSOCIATEDEXPANSIONCACHEWRITEDATATYPE { SNAECWT_UNSAT = 1, SNAECWT_EXPAND = 2 };

						SATURATIONNODEASSOCIATEDEXPANSIONCACHEWRITEDATATYPE getWriteDataType();

					// protected methods
					protected:

					// protected variables
					protected:
						SATURATIONNODEASSOCIATEDEXPANSIONCACHEWRITEDATATYPE mWriteDataType;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHEWRITEDATA_H
