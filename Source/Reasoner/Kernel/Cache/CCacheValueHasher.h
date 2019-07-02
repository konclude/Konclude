/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CCACHEVALUEHASHER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CCACHEVALUEHASHER_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CCacheValueHasher
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCacheValueHasher {
					// public methods
					public:
						//! Constructor
						CCacheValueHasher(CCacheValue* cacheValue);

						cint64 getHashValue() const;

						bool operator==(const CCacheValueHasher& hasher) const;

					// protected methods
					protected:

					// protected variables
					protected:
						CCacheValue* mCacheValue;



					// private methods
					private:

					// private variables
					private:

				};


				inline uint qHash(const CCacheValueHasher& hasher) {
					cint64 key = (cint64)hasher.getHashValue();
					if (sizeof(cint64) > sizeof(uint)) {
						return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
					} else {
						return uint(key);
					}
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CCACHEVALUEHASHER_H
