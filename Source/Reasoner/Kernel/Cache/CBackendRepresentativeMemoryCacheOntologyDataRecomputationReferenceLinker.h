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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEONTOLOGYDATARECOMPUTATIONREFERENCELINKER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEONTOLOGYDATARECOMPUTATIONREFERENCELINKER_H

// Libraries includes
#include <QAtomicInteger>

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
				 *		\class		CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker : public CLinkerBase<CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker*, CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker(cint64 ontologyDataUpdateId);

						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* updateUsedRecomputationId(cint64 recomputationId);

						cint64 getMinUsedRecomputationId();
						cint64 getMaxUsedRecomputationId();
						cint64 getOntologyDataUpdateId();
						bool isOntologyDataActive();
						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* setOntologyDataInactive();

						bool isNextOntologyDataAllInactive();
						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* setNextOntologyDataAllInactive();


					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mOntologyDataUpdateId;
						bool mOntologyDataActive;
						bool mNextOntologyDataAllInactive;

						QAtomicInteger<cint64> mMinUsedRecomputationId;
						QAtomicInteger<cint64> mMaxUsedRecomputationId;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEONTOLOGYDATARECOMPUTATIONREFERENCELINKER_H
