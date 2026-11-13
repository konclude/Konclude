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

#include "CParallelSaturationProcessContext.h"
#include "Scheduler/CTaskMemoryPoolAllocationManager.h"
#include "Utilities/Memory/CMemoryAllocator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConsiderateMemoryPoolProvider* CParallelSaturationProcessContext::getParallelSaturationMemoryPoolProvider() {
					static CNewAllocationMemoryPoolProvider* provider = new CNewAllocationMemoryPoolProvider();
					return provider;
				}


				CParallelSaturationProcessContext* CParallelSaturationProcessContext::createParallelSaturationProcessContext() {
					CConsiderateMemoryPoolProvider* provider = getParallelSaturationMemoryPoolProvider();
					CMemoryPoolContainer* bootstrapContainer = new CMemoryPoolContainer();
					void* memManAdd = CTaskMemoryPoolAllocationManager::allocateMemoryToContainer(sizeof(CProcessMemoryPoolAllocationManager),bootstrapContainer,provider);
					CProcessMemoryPoolAllocationManager* memMan = new (memManAdd) CProcessMemoryPoolAllocationManager(bootstrapContainer,provider);
					void* contextAdd = CMemoryAllocator::allocateMemory(memMan,sizeof(CParallelSaturationProcessContext));
					return new (contextAdd) CParallelSaturationProcessContext(memMan,bootstrapContainer);
				}


				CParallelSaturationProcessContext::CParallelSaturationProcessContext(CProcessMemoryPoolAllocationManager* memMan, CMemoryPoolContainer* bootstrapContainer) {
					mBootstrapContainer = bootstrapContainer;
					mMemMan = memMan;
					mUsedMemMan = mMemMan;
					mProcessTagger = CObjectAllocator<CProcessTagger>::allocateAndConstruct(mMemMan);
					mUsedProcessTagger = mProcessTagger;
#ifndef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
					mProcStatGath = CObjectAllocator<CProcessingStatisticGathering>::allocateAndConstruct(mMemMan);
					mUsedProcessStatGath = mProcStatGath;
#else
					mProcStatGath = nullptr;
					mUsedProcessStatGath = nullptr;
#endif
				}


				CParallelSaturationProcessContext* CParallelSaturationProcessContext::attachToContainer(CMemoryPoolContainer* container) {
					if (mBootstrapContainer) {
						container->appendMemoryPool(mBootstrapContainer->takeMemoryPools());
						mMemMan->initTaskMemoryPoolAllocationManager(container,getParallelSaturationMemoryPoolProvider());
						delete mBootstrapContainer;
						mBootstrapContainer = nullptr;
					}
					return this;
				}


				CProcessTagger* CParallelSaturationProcessContext::getProcessTagger() {
					return mProcessTagger;
				}

				CProcessingStatisticGathering* CParallelSaturationProcessContext::getProcessingStatisticsGathering() {
					return mProcStatGath;
				}

				CMemoryAllocationManager* CParallelSaturationProcessContext::getMemoryAllocationManager() {
					return mMemMan;
				}

				CTaskContext* CParallelSaturationProcessContext::updateContext(CTaskHandleContext* handleContext) {
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
