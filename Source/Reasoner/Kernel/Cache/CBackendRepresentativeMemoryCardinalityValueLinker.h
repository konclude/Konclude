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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCARDINALITYVALUELINKER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCARDINALITYVALUELINKER_H

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
				 *		\class		CBackendRepresentativeMemoryCardinalityValueLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCardinalityValueLinker : public CLinkerBase<cint64,CBackendRepresentativeMemoryCardinalityValueLinker> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCardinalityValueLinker();

						CBackendRepresentativeMemoryCardinalityValueLinker* initCardinalityValueLinker(cint64 roleTag, cint64 existentialMaxUsedCardinality, cint64 minimalRestrictingCardinality);

						cint64 getRoleTag();
						cint64 getExistentialMaxUsedCardinality();
						cint64 getMinimalRestrictingCardinality();

						CBackendRepresentativeMemoryCardinalityValueLinker* updateExistentialMaxUsedCardinality(cint64 existentialMaxUsedCardinality);
						CBackendRepresentativeMemoryCardinalityValueLinker* updateMinimalRestrictingCardinality(cint64 minimalRestrictingCardinality);

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mExistentialMaxUsedCardinality;
						cint64 mMinimalRestrictingCardinality;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCARDINALITYVALUELINKER_H
