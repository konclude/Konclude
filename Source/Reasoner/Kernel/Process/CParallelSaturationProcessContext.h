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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPARALLELSATURATIONPROCESSCONTEXT_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPARALLELSATURATIONPROCESSCONTEXT_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CProcessMemoryPoolAllocationManager.h"
#include "CProcessTagger.h"
#include "CProcessingStatisticGathering.h"


// Other includes
#include "Utilities/Memory/CMemoryPoolContainer.h"
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CObjectAllocator.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*!
				 *
				 *		\class		CParallelSaturationProcessContext
				 *		\author		Konclude Developer Team
				 *		\version	0.1
				 *		\brief		The process context of a helper thread of the parallel saturation: its own memory pools
				 *					(from a provider of its own, so no two threads allocate from the same manager), tagger and
				 *					statistics. The pools are taken over by the saturation task when the helper is done, so
				 *					everything the helper allocated into the saturation nodes lives as long as the task.
				 *
				 */
				class CParallelSaturationProcessContext : public CProcessContext {
					// public methods
					public:
						//! Creates the context, its allocation manager, tagger and statistics inside the first pool the
						//! manager allocates, so that all of them move with the pools (see attachToContainer).
						static CParallelSaturationProcessContext* createParallelSaturationProcessContext();

						virtual CProcessTagger* getProcessTagger();
						virtual CProcessingStatisticGathering* getProcessingStatisticsGathering();
						virtual CMemoryAllocationManager* getMemoryAllocationManager();
						virtual CTaskContext* updateContext(CTaskHandleContext* handleContext);

						//! Hands the pools allocated so far to the container (the saturation task) and lets the manager
						//! allocate into that container from now on; the context is then owned by the pools and never freed.
						CParallelSaturationProcessContext* attachToContainer(CMemoryPoolContainer* container);

						//! The pool provider of all parallel saturation threads: it only wraps new and the pool recycler.
						static CConsiderateMemoryPoolProvider* getParallelSaturationMemoryPoolProvider();

					// protected methods
					protected:
						CParallelSaturationProcessContext(CProcessMemoryPoolAllocationManager* memMan, CMemoryPoolContainer* bootstrapContainer);

					// protected variables
					protected:
						CMemoryPoolContainer* mBootstrapContainer;
						CProcessMemoryPoolAllocationManager* mMemMan;
						CProcessTagger* mProcessTagger;
						CProcessingStatisticGathering* mProcStatGath;

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPARALLELSATURATIONPROCESSCONTEXT_H
