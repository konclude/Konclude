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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALDELAYEDBACKENDINITIALIZATIONPROCESSINGQUEUE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALDELAYEDBACKENDINITIALIZATIONPROCESSINGQUEUE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualProcessNode.h"
#include "CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualDelayedBackendInitializationProcessingQueue
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualDelayedBackendInitializationProcessingQueue {
					// public methods
					public:
						//! Constructor
						CIndividualDelayedBackendInitializationProcessingQueue(CProcessContext* processContext);

						CIndividualDelayedBackendInitializationProcessingQueue* initProcessingQueue(CIndividualDelayedBackendInitializationProcessingQueue* processingQueue);

						CXLinker<CIndividualProcessNode*>* getProcessIndividualNodeLinker();

						CIndividualProcessNode* takeNextProcessIndividualNode();
						CIndividualProcessNode* getNextProcessIndividualNode();
						bool addIndividualNodeQueuingLinker(CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* indiNodeLinker);

						bool isEmpty();
						bool isRoot();

						CIndividualDelayedBackendInitializationProcessingQueue* insertIndiviudalProcessNode(CIndividualProcessNode* indiProNode);

					// protected methods
					protected:
						bool moveIndividualNodeLinkersToLocalQueue();

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CMemoryAllocationManager* mMemMan;

						CIndividualDelayedBackendInitializationProcessingQueue* mRootQueue;


						cint64 mIndiProDesCount;

						QAtomicPointer<CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker>* mIndiNodeQueuingLinkerPointer;
						CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* mLastTakenIndiNodeQueuingLinker;
						CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* mLastIndiNodeQueuingLinker;

						CXLinker<CIndividualProcessNode*>* mQueuedIndiNodeLinker;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALDELAYEDBACKENDINITIALIZATIONPROCESSINGQUEUE_H
