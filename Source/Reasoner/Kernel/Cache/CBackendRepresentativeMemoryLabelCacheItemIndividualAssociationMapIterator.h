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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMINDIVIDUALASSOCIATIONMAPITERATOR_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMINDIVIDUALASSOCIATIONMAPITERATOR_H

// Libraries includes
#include <QMap>

// Namespace includes
#include "CacheSettings.h"


// Other includes
#include "Utilities/Memory/CObjectParameterizingAllocator.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {




				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator {
					// public methods
					public:
						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator();

						//! Constructor
						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator(QMap<cint64, DummyValue>::const_iterator baseIndiAssoMapItBegin, QMap<cint64, DummyValue>::const_iterator baseIndiAssoMapItEnd, 
							QMap<cint64, DummyValue>::const_iterator sameIndiMergedAssoMapItBegin, QMap<cint64, DummyValue>::const_iterator sameIndiMergedAssoMapItEnd, bool iterateAscending);


						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator* moveNext();
						bool atEnd();

						cint64 currentAssociatedIndividualId();
						bool currentAssociatedIndividualSameMerged();


					// protected methods
					protected:

					// protected variables
					protected:
						bool mIterateAscending;

						bool mHasCurrentIndiId;
						bool mCurrentIndiSameMerged;
						cint64 mCurrentIndiId;

						QMap<cint64, DummyValue>::const_iterator mBaseIndiAssoMapIt;
						QMap<cint64, DummyValue>::const_iterator mBaseIndiAssoMapItBegin;
						QMap<cint64, DummyValue>::const_iterator mBaseIndiAssoMapItEnd;
						
						QMap<cint64, DummyValue>::const_iterator mSameIndiMergedAssoMapIt;
						QMap<cint64, DummyValue>::const_iterator mSameIndiMergedAssoMapItBegin;
						QMap<cint64, DummyValue>::const_iterator mSameIndiMergedAssoMapItEnd;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMINDIVIDUALASSOCIATIONMAPITERATOR_H
