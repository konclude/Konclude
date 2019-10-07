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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALREACTIVATIONPROCESSINGQUEUE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALREACTIVATIONPROCESSINGQUEUE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualProcessNode.h"
#include "CIndividualDepthPriority.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualReactivationProcessingQueue
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualReactivationProcessingQueue {
					// public methods
					public:
						//! Constructor
						CIndividualReactivationProcessingQueue(CProcessContext* processContext);

						CIndividualReactivationProcessingQueue* initProcessingQueue(CIndividualReactivationProcessingQueue* processingQueue);


						bool takeNextReactivationIndividual(CIndividualProcessNode*& indiNode, bool& forceReactivation);
						bool getNextReactivationIndividual(CIndividualProcessNode*& indiNode, bool& forceReactivation);
						bool insertReactivationIndiviudal(CIndividualProcessNode* individual, bool forceReactivation);

						bool isEmpty();
						cint64 getQueuedIndividualCount();
						bool hasQueuedIndividuals();
						bool hasQueuedIndividual(CIndividualProcessNode* individual);


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;

						class CIndividualForceReactivationData {
							public:
								CIndividualForceReactivationData(CIndividualProcessNode* indiNode = nullptr, bool forceReactivation = false) {
									mForceReactivation = forceReactivation;
									mIndiNode = indiNode;
								}
								bool mForceReactivation;
								CIndividualProcessNode* mIndiNode;
						};

						CPROCESSMAP<CIndividualDepthPriority,CIndividualForceReactivationData> mPriorityIndiReactivationMap;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALREACTIVATIONPROCESSINGQUEUE_H
