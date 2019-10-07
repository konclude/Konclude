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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CREUSECOMPLETIONGRAPHCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CREUSECOMPLETIONGRAPHCACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"

#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCacheReader.h"
#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCacheWriter.h"
#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCacheEntry.h"
#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCacheEntryWriteData.h"
#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCacheEntryExpandWriteData.h"

#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Cache;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CReuseCompletionGraphCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReuseCompletionGraphCacheHandler {
					// public methods
					public:
						//! Constructor
						CReuseCompletionGraphCacheHandler(CReuseCompletionGraphCacheReader* reuseCompGraphCacheReader, CReuseCompletionGraphCacheWriter* reuseCompGraphCacheWriter);

						//! Destructor
						virtual ~CReuseCompletionGraphCacheHandler();

						CReuseCompletionGraphCacheEntry* getReuseCompletionGraphEntry(CIndividualProcessNode* indiProcNode, bool* minimalCompletionGraph, bool* deterministicConnection, CCalculationAlgorithmContext* calcAlgContext);

						bool commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:
						bool prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext);
						bool addCacheMessages(CReuseCompletionGraphCacheEntryWriteData* expandWriteData, CCalculationAlgorithmContext* calcAlgContext);

					// protected variables
					protected:
						CReuseCompletionGraphCacheReader* mReuseCompGraphCacheReader;
						CReuseCompletionGraphCacheWriter* mReuseCompGraphCacheWriter;


						CReuseCompletionGraphCacheEntryWriteData* mWriteData;
						CTaskMemoryPoolAllocationManager* mMemAllocMan;
						CMemoryPoolContainer mMemPoolCon;
						CContextBase* mTmpContext;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CREUSECOMPLETIONGRAPHCACHEHANDLER_H
