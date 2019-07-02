/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYCARDINALITYWRITEDATALINKER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYCARDINALITYWRITEDATALINKER_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryCardinalityValueLinker.h"


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
				 *		\class		CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker : public CLinkerBase<CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker*,CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker();

						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* initLabelWriteData(cint64 signature);

						cint64 getSignature();
						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* setSignature(cint64 signature);


						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* appendCardinalityCacheValueLinker(CBackendRepresentativeMemoryCardinalityValueLinker* linker);
						CBackendRepresentativeMemoryCardinalityValueLinker* getCardinalityCacheValueLinker();

						cint64 getCardinalityCacheValueCount();


						void* getTemporaryData();
						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* setTemporaryData(void* tmpData);
						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* clearTemporaryData();


					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mSignature;
						CBackendRepresentativeMemoryCardinalityValueLinker* mCardValueLinker;
						cint64 mCardValueCount;

						void* mTmpData;


					// private methods
					private:

					// private variables
					private:


				};


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYCARDINALITYWRITEDATALINKER_H
