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

#include "CProcessContextBase.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CProcessContextBase::CProcessContextBase(CMemoryPoolContainer* memoryPoolContainer, CTaskHandleContext* handleContext) {
					CTaskHandleMemoryAllocationManager* tempMemMan = handleContext->getTaskHandleMemoryAllocationManager();
					void* memManAdd = CProcessMemoryPoolAllocationManager::allocateMemoryToContainer(sizeof(CProcessMemoryPoolAllocationManager),memoryPoolContainer,tempMemMan);
					mMemMan = new (memManAdd) CProcessMemoryPoolAllocationManager(memoryPoolContainer,tempMemMan);
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


				CProcessContextBase::~CProcessContextBase() {
				}

				CProcessContextBase* CProcessContextBase::referenceProcessContext(CProcessContextBase* reference) {
					if (reference) {
						mProcessTagger->initProcessTagger(reference->mProcessTagger);
					}
					return this;
				}


				CProcessTagger* CProcessContextBase::getProcessTagger() {
					return mProcessTagger;
				}

				CProcessingStatisticGathering* CProcessContextBase::getProcessingStatisticsGathering() {
					return mProcStatGath;
				}


				CTaskContext* CProcessContextBase::updateContext(CTaskHandleContext* handleContext) {
					CTaskHandleMemoryAllocationManager* tempMemMan = handleContext->getTaskHandleMemoryAllocationManager();
					mMemMan->updateMemoryProvider(tempMemMan);
					return this;
				}

				CMemoryAllocationManager* CProcessContextBase::getMemoryAllocationManager() {
					return mMemMan;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
