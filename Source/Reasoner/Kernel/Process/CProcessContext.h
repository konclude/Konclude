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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSCONTEXT_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSCONTEXT_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessTagger.h"
#include "CProcessMemoryPoolAllocationManager.h"
#include "CProcessingStatisticGathering.h"


// Other includes
#include "Scheduler/CTaskContext.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Scheduler;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CProcessContext
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CProcessContext : public CTaskContext {
					// public methods
					public:
						//! Constructor
						CProcessContext();

						//! Destructor
						virtual ~CProcessContext();

						virtual CProcessTagger* getProcessTagger() = 0;
						virtual CProcessingStatisticGathering* getProcessingStatisticsGathering() = 0;


						inline CProcessTagger* getUsedProcessTagger();
						inline CMemoryAllocationManager* getUsedMemoryAllocationManager();
						inline CProcessingStatisticGathering* getUsedProcessingStatisticsGathering();



					// protected methods
					protected:

					// protected variables
					protected:
						CProcessMemoryPoolAllocationManager* mUsedMemMan;
						CProcessTagger* mUsedProcessTagger;
						CProcessingStatisticGathering* mUsedProcessStatGath;


					// private methods
					private:

					// private variables
					private:

				};


				CProcessTagger* CProcessContext::getUsedProcessTagger() {
					return mUsedProcessTagger;
				}
				
				CMemoryAllocationManager* CProcessContext::getUsedMemoryAllocationManager() {
					return mUsedMemMan;
				}

				CProcessingStatisticGathering* CProcessContext::getUsedProcessingStatisticsGathering() {
					return mUsedProcessStatGath;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSCONTEXT_H
