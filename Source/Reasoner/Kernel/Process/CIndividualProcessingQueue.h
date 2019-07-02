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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALPROCESSINGQUEUE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALPROCESSINGQUEUE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualProcessNodeDescriptor.h"
#include "CIndividualProcessNode.h"
#include "CIndividualProcessNodePriority.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualProcessingQueue
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualProcessingQueue {
					// public methods
					public:
						//! Constructor
						CIndividualProcessingQueue(CProcessContext* processContext);

						//! Destructor
						virtual ~CIndividualProcessingQueue();

						CIndividualProcessingQueue* initProcessingQueue(CIndividualProcessingQueue* processingQueue);


						CIndividualProcessNodeDescriptor *takeNextProcessIndividualDescriptor();
						CIndividualProcessNodeDescriptor *getNextProcessIndividualDescriptor();
						CIndividualProcessingQueue *insertIndiviudalProcessDescriptor(CIndividualProcessNodeDescriptor *indiProDes);

						bool hasHigherPriorityIndividual(CIndividualProcessNodePriority priority);
						bool needsIndiviudalInsertion(CIndividualProcessNode *individual, CIndividualProcessNodePriority priority);
						bool isEmpty();
						bool hasIndividualProcessDescriptor();
						bool isIndividualQueued(CIndividualProcessNode *individual);

						CPROCESSMAP<CIndividualProcessNodePriority,CIndividualProcessNodeDescriptor*> mPriorityIndiDesMap;
						CPROCESSHASH<cint64,CIndividualProcessNodePriority> mIndiDesPriorityHash;

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;

						cint64 mIndiProDesCount;
						bool mHasMaxIndiPriority;
						CIndividualProcessNodePriority mMaxIndiPriority;
						CIndividualProcessNode* mLastCheckIndi;
						CIndividualProcessNodePriority mLastCheckIndiPriority;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALPROCESSINGQUEUE_H
