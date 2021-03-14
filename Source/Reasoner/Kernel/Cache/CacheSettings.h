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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CACHESETTINGS_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CACHESETTINGS_H

// Libraries includes
#include <QEvent>

// Namespace includes


// Other includes
#include "Reasoner/Ontology/OntologySettings.h"

#include "Utilities/UtilitiesSettings.h"
#include "Utilities/Container/CQtManagedRestrictedModificationHash.h"
#include "Utilities/Container/CQtManagedRestrictedModificationList.h"
#include "Utilities/Container/CQtManagedRestrictedModificationMap.h"
#include "Utilities/Container/CQtManagedRestrictedModificationSet.h"

// Logger includes


namespace Konclude {

	using namespace Utilities;
	using namespace Container;

	namespace Reasoner {

		using namespace Ontology;

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

				class CSaturationNodeAssociatedExpansionCacheEntry;
				class CSaturationNodeAssociatedExpansionCacheWriter;
				class CSaturationNodeAssociatedExpansionCacheReader;
				class CSaturationNodeAssociatedExpansionCache;
				class CSaturationNodeCacheUpdater;

				class CComputedConsequencesCache;
				class CComputedConsequencesCacheEntry;
				class CComputedConsequencesCacheReader;
				class CComputedConsequencesCacheWriter;
				class CComputedConsequencesCacheContext;

				class CBackendRepresentativeMemoryCacheWriter;
				class CBackendRepresentativeMemoryCacheReader;
				class CBackendRepresentativeMemoryCache;
				class CBackendRepresentativeMemoryLabelSignatureResolveCacheItem;
				class CBackendRepresentativeMemoryLabelCacheItem;
				class CBackendRepresentativeMemoryCacheIndividualAssociationData;

				class COccurrenceStatisticsCache;
				class COccurrenceStatisticsCacheReader;
				class COccurrenceStatisticsCacheWriter;
				class COccurrenceStatisticsCacheData;

				struct DummyValue {
				};


				// events

				const QEvent::Type EVENTWRITEUNSATISFIABLECACHEENTRY				= (QEvent::Type)2000;
				const QEvent::Type EVENTWRITESATISFIABLECACHEENTRY					= (QEvent::Type)2001;
				const QEvent::Type EVENTWRITEEXPANDCACHEDENTRY						= (QEvent::Type)2002;
				const QEvent::Type EVENTWRITESATISFIABLEBRANCHCACHEDENTRY			= (QEvent::Type)2003;
				const QEvent::Type EVENTWRITECACHEDDATAENTRY						= (QEvent::Type)2004;
				const QEvent::Type EVENTWRITESATURATIONCACHEDATAENTRY				= (QEvent::Type)2005;
				const QEvent::Type EVENTWRITECOMPUTEDCONSEQUENCESCACHEDATAENTRY		= (QEvent::Type)2006;
				const QEvent::Type EVENTWRITEBACKENDASSOCIATIONENTRY				= (QEvent::Type)2007;
				const QEvent::Type EVENTRETRIEVEINCOMPLETELYASSOCIATIONCACHED		= (QEvent::Type)2008;
				const QEvent::Type EVENTINITIALIZEINDIVIDUALSASSOCIATIONSCACHE		= (QEvent::Type)2009;
				const QEvent::Type EVENTREPORTMAXIMUMHANDLEDRECOMPUTATIONID			= (QEvent::Type)2010;


				// defines

#define CCACHINGHASH CQtManagedRestrictedModificationHash
#define CCACHINGLIST CQtManagedRestrictedModificationList
#define CCACHINGSET CQtManagedRestrictedModificationSet
#define CCACHINGMAP CQtManagedRestrictedModificationMap


#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
#define KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
#endif


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // end KONCLUDE_REASONER_KERNEL_CACHE_CACHESETTINGS_H