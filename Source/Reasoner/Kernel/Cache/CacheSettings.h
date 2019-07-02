/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CACHESETTINGS_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CACHESETTINGS_H

// Libraries includes
#include <QEvent>

// Namespace includes


// Other includes
#include "Utilities/UtilitiesSettings.h"
#include "Utilities/Container/CQtManagedRestrictedModificationHash.h"
#include "Utilities/Container/CQtManagedRestrictedModificationList.h"
#include "Utilities/Container/CQtManagedRestrictedModificationSet.h"

// Logger includes


namespace Konclude {

	using namespace Utilities;
	using namespace Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				/*! 
				 *
				 *		\file		CacheSettings
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */


				// forward declarations

				class COccurrenceUnsatisfiableCacheReader;
				class COccurrenceUnsatisfiableCache;
				class COccurrenceUnsatisfiableCacheUpdateSlotItem;
				class COccurrenceUnsatisfiableCacheEntry;
				class COccurrenceUnsatisfiableCacheEntriesHash;
				class COccurrenceUnsatisfiableCacheWriter;

				class CSignatureSatisfiableExpanderCacheWriter;
				class CSignatureSatisfiableExpanderCacheReader;
				class CSignatureSatisfiableExpanderCache;
				class CSignatureSatisfiableExpanderCacheEntry;

				class CReuseCompletionGraphCacheEntry;
				class CReuseCompletionGraphCacheWriter;
				class CReuseCompletionGraphCacheReader;
				class CReuseCompletionGraphCache;
				class CReuseCompletionGraphCacheContext;

				// events

				const QEvent::Type EVENTWRITEUNSATISFIABLECACHEENTRY		= (QEvent::Type)2000;
				const QEvent::Type EVENTWRITESATISFIABLECACHEENTRY			= (QEvent::Type)2001;
				const QEvent::Type EVENTWRITEEXPANDCACHEDENTRY				= (QEvent::Type)2002;
				const QEvent::Type EVENTWRITESATISFIABLEBRANCHCACHEDENTRY	= (QEvent::Type)2003;
				const QEvent::Type EVENTWRITECACHEDDATAENTRY				= (QEvent::Type)2004;


				// defines

#define CCACHINGHASH CQtManagedRestrictedModificationHash
#define CCACHINGLIST CQtManagedRestrictedModificationList
#define CCACHINGSET CQtManagedRestrictedModificationSet


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_KERNEL_CACHE_CACHESETTINGS_H