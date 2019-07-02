/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCRITICALSATURATIONCONCEPTTYPEQUEUES_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCRITICALSATURATIONCONCEPTTYPEQUEUES_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CCriticalSaturationConceptQueue.h"
#include "CIndividualSaturationProcessNodeLinker.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CCriticalSaturationConceptTypeQueues
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCriticalSaturationConceptTypeQueues {
					// public methods
					public:
						//! Constructor
						CCriticalSaturationConceptTypeQueues(CProcessContext* context = nullptr);


						enum CRITICALSATURATIONCONCEPTQUEUETYPE { CCT_FORALL = 0, CCT_ATMOST = 1, CCT_DISJUNCTION = 2, CCT_EQCANDIDATE = 3, CCT_VALUE = 4, CCT_NOMINAL = 5 };


						CCriticalSaturationConceptTypeQueues* initCriticalSaturationConceptQueues(CIndividualSaturationProcessNode* indiNode);

						bool hasCriticalSaturationConceptsQueued();

						CCriticalSaturationConceptQueue* getCriticalSaturationConceptQueue(CRITICALSATURATIONCONCEPTQUEUETYPE type, bool create);

						bool isProcessNodeQueued();
						CCriticalSaturationConceptTypeQueues* setProcessNodeQueued(bool queued);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mContext;
						CIndividualSaturationProcessNode* mIndiNode;
						CCriticalSaturationConceptQueue** mQueueVec;
						bool mQueued;

						const static cint64 mConceptTypeCount = 6;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCRITICALSATURATIONCONCEPTTYPEQUEUES_H
