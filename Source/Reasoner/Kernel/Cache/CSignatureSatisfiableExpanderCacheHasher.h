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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEHASHER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEHASHER_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "CSignatureSatisfiableExpanderCacheContext.h"
#include "CExpanderCacheValueLinker.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CSignatureSatisfiableExpanderCacheHasher
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSignatureSatisfiableExpanderCacheHasher {
					// public methods
					public:
						//! Constructor
						CSignatureSatisfiableExpanderCacheHasher(CCACHINGSET<CCacheValue>* cacheValueSet);
						CSignatureSatisfiableExpanderCacheHasher(CExpanderCacheValueLinker* cacheValueLinker, cint64 count);
						CSignatureSatisfiableExpanderCacheHasher(const CSignatureSatisfiableExpanderCacheHasher& hasher);

						cint64 getHashValue() const;

						bool operator==(const CSignatureSatisfiableExpanderCacheHasher& hasher) const;

					// protected methods
					protected:
						void extendHashValue(CCacheValue* cacheValue);
						void calculateHashValue(CCACHINGSET<CCacheValue>* cacheValueSet);
						void calculateHashValue(CExpanderCacheValueLinker* cacheValueLinker, cint64 count);

						bool hasEqualCacheValues(CExpanderCacheValueLinker* cacheValueLinker, CExpanderCacheValueLinker* cacheValueLinker2, cint64 count) const;
						bool hasEqualCacheValues(CCACHINGSET<CCacheValue>* cacheValueSet, CCACHINGSET<CCacheValue>* cacheValueSet2) const;
						bool hasEqualCacheValues(CExpanderCacheValueLinker* cacheValueLinker, CCACHINGSET<CCacheValue>* cacheValueSet, cint64 count) const;

					// protected variables
					protected:
						cint64 mHashValue;
						cint64 mCacheValueCount;
						CExpanderCacheValueLinker* mCacheValueLinker;
						CCACHINGSET<CCacheValue>* mCacheValueSet;



					// private methods
					private:

					// private variables
					private:

				};


				inline uint qHash(const CSignatureSatisfiableExpanderCacheHasher& hasher) {
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

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHEHASHER_H
