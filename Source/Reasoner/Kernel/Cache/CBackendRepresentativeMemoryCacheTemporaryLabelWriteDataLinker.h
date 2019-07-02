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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYLABELWRITEDATALINKER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYLABELWRITEDATALINKER_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelValueLinker.h"


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
				 *		\class		CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker : public CLinkerBase<CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker();

						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* initLabelWriteData(cint64 signature);

						cint64 getSignature();
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* setSignature(cint64 signature);


						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* appendDeterministicCacheValueLinker(CBackendRepresentativeMemoryLabelValueLinker* linker);
						CBackendRepresentativeMemoryLabelValueLinker* getDeterministicCacheValueLinker();

						cint64 getDeterministicCacheValueCount();

						bool isCompletelyHandled();
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* setCompletelyHandled(bool completelyHandled);

						bool isCompletelySaturated();
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* setCompletelySaturated(bool completelySaturated);

						void* getTemporaryData();
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* setTemporaryData(void* tmpData);
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* clearTemporaryData();


					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mSignature;
						CBackendRepresentativeMemoryLabelValueLinker* mDetValueLinker;
						cint64 mDetValueCount;

						bool mCompletelyHandled;
						bool mCompletelySaturated;

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

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYLABELWRITEDATALINKER_H
