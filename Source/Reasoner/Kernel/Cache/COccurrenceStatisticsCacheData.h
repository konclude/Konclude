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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHEDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHEDATA_H

// Libraries includes
#include <QReadWriteLock>


// Namespace includes
#include "CacheSettings.h"
#include "COccurrenceStatisticsCacheOntologyData.h"
#include "COccurrenceStatisticsCacheContext.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		COccurrenceStatisticsCacheData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceStatisticsCacheData {
					// public methods
					public:
						//! Constructor
						COccurrenceStatisticsCacheData();

						~COccurrenceStatisticsCacheData();


						QReadWriteLock* getReadWriteLock();

						cint64 getUpdateId();
						COccurrenceStatisticsCacheData* incUpdateId();


						COccurrenceStatisticsCacheOntologyData* getOntologyData(cint64 ontologyId, bool createIfNotExists = false);

					// protected methods
					protected:

					// protected variables
					protected:
						QReadWriteLock mReadWriteLock;
						cint64 mUpdateId;

						QHash<cint64, COccurrenceStatisticsCacheOntologyData*> mOntologyDataHash;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHEDATA_H
