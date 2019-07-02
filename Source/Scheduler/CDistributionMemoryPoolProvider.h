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

#ifndef KONCLUDE_SCHEDULER_CDISTRIBUTIONMEMORYPOOLPROVIDER_H
#define KONCLUDE_SCHEDULER_CDISTRIBUTIONMEMORYPOOLPROVIDER_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CDistributionMemoryPoolReleaser.h"

// Other includes
#include "Utilities/Memory/CMemoryPoolProvider.h"

#include "Utilities/Container/CSortedLinker.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CDistributionMemoryPoolProvider
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CDistributionMemoryPoolProvider : public CSortedLinkerBase<CDistributionMemoryPoolProvider*,CDistributionMemoryPoolProvider,CDistributionMemoryPoolProvider>, public CMemoryPoolProvider {
			// public methods
			public:
				//! Constructor
				CDistributionMemoryPoolProvider(CMemoryPoolProvider* alternateMemoryPoolProvider, CDistributionMemoryPoolReleaser* distributionMemoryReleaser);

				//! Destructor
				virtual ~CDistributionMemoryPoolProvider();


				virtual CMemoryPool* acquireMemoryPool(cint64 minPoolSize);
				virtual CDistributionMemoryPoolProvider* releaseMemoryPool(CMemoryPool* memoryPool);

				virtual CDistributionMemoryPoolProvider* appendDistributedMemoryPools(CMemoryPool* memoryPools);
				virtual bool checkAppendableDistributedMemoryPools(CMemoryPool* memoryPools);

				virtual double getMissingMemoryPoolRatio();
				virtual cint64 getMissingMemoryPoolCount();
				virtual cint64 getMemoryPoolSupplyCount();
				virtual CDistributionMemoryPoolProvider* updateMissingMemoryPoolRatio();
				virtual CDistributionMemoryPoolProvider* updatedRoundAcquiredMemoryPoolCounts();

				static inline bool sortedLinkerDataCompare(CDistributionMemoryPoolProvider* const & beforeData1, CDistributionMemoryPoolProvider* const & data2) {
					return beforeData1->getMissingMemoryPoolRatio() < data2->getMissingMemoryPoolRatio();
				}


				cint64 getAllocatedReleaseDifferencePoolCount();
				cint64 getAllocatedReleaseDifferencePoolSize();

			// protected methods
			protected:
				bool pickDistributedMemoryPools();

			// protected variables
			protected:
				CMemoryPoolProvider* mAlternateMemoryPoolProvider;
				CDistributionMemoryPoolReleaser* mDistributionMemoryReleaser;
				CMemoryPool* mAvailablePoolLinker;
				CMemoryPool* mDistributedPoolLinker;
				CMemoryPool* mLastPickedDistributedPoolLinker;

				cint64 mDistributedPoolCount;
				cint64 mPickedPoolCount;
				cint64 mAvailablePoolCount;
				cint64 mAquiredPoolCount;
				cint64 mLastAcquiredPoolCount;
				cint64 mLastAcquiredPoolDiff;

				double mLastSavedMissRatio;


				double mLastNewDiffRatio;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CDISTRIBUTIONMEMORYPOOLPROVIDER_H
