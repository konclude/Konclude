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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CREUSECOMPLETIONGRAPHCACHEREADER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CREUSECOMPLETIONGRAPHCACHEREADER_H

// Libraries includes
#include <QAtomicPointer>
#include <QVector>

// Namespace includes
#include "CacheSettings.h"
#include "CReuseCompletionGraphCacheSlotItem.h"
#include "CReuseCompletionGraphCacheEntry.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Ontology/CConcept.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {


				typedef QPair<CConcept*,bool> TConceptNegPair;


				/*! 
				 *
				 *		\class		CReuseCompletionGraphCacheReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReuseCompletionGraphCacheReader : public CLinkerBase<CReuseCompletionGraphCacheReader*,CReuseCompletionGraphCacheReader> {
					// public methods
					public:
						//! Constructor
						CReuseCompletionGraphCacheReader();


						CReuseCompletionGraphCacheReader* updateSlot(CReuseCompletionGraphCacheSlotItem* updatedSlot);

						CReuseCompletionGraphCacheEntry* getCacheEntry(CConcept* concept, bool* minimalCompletionGraph, bool* deterministicConnection);


					// protected methods
					protected:
						bool hasUpdatedSlotItem();
						bool switchToUpdatedSlotItem();

						CXLinker<TConceptNegPair>* getConceptTestLinker();
						CReuseCompletionGraphCacheReader* releaseConceptTestLinker(CXLinker<TConceptNegPair>* linker);

						bool addTestingConcepts(CSortedNegLinker<CConcept*>* conLinker, bool negate, cint64 addingLevel, CCACHINGSET<CConcept*>* testedConceptSet);
						bool addTestingConcept(CConcept* concept, bool negated, cint64 addingLevel, CCACHINGSET<CConcept*>* testedConceptSet);

					// protected variables
					protected:
						static const cint64 mTestConceptLinkerVecSize = 4;


						CReuseCompletionGraphCacheContext mContext;
						CReuseCompletionGraphCacheSlotItem* mCurrentSlot;
						QAtomicPointer<CReuseCompletionGraphCacheSlotItem> mUpdatedSlot;

						class CCacheEntryVotingItem {
							public:
								CCacheEntryVotingItem() {
								}

								CCacheEntryVotingItem* reset() {
									mIncompatibleCount = 0;
									mEntailedCount = 0;
									mMissingCount = 0;
									mTmpReferenced = false;
									mMinimalFound = false;
									mEntry = nullptr;
									mMinConnectionLevel = mTestConceptLinkerVecSize;
									return this;
								}

								cint64 mIncompatibleCount;
								cint64 mEntailedCount;
								cint64 mMissingCount;
								bool mTmpReferenced;
								bool mMinimalFound;
								cint64 mMinConnectionLevel;
								CReuseCompletionGraphCacheEntry* mEntry;
						};

						
						CXLinker<TConceptNegPair>* mFreeLinker;
						QVector<CCacheEntryVotingItem> mEntryVotingVec;


						CXLinker<TConceptNegPair>* mTestConceptLinkerVec[mTestConceptLinkerVecSize];
						cint64 mTestConceptLinkerCount;
						cint64 mTestConceptLinkerLevel;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CREUSECOMPLETIONGRAPHCACHEREADER_H
