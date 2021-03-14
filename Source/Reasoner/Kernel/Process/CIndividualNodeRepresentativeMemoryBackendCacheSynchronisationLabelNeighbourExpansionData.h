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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEREPRESENTATIVEMEMORYBACKENDCACHESYNCHRONISATIONLABELNEIGHBOUREXPANSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEREPRESENTATIVEMEMORYBACKENDCACHESYNCHRONISATIONLABELNEIGHBOUREXPANSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CBackendNeighbourExpansionQueueDataLinker.h"


// Other includes
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryLabelCacheItem.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Cache;

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData {
					// public methods
					public:
						//! Constructor
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData();


						bool isNeighbourLabelDelayedRepresentativeExpansion();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* setNeighbourLabelDelayedRepresentativeExpansion(bool delayedRepresentativeExpansion = true);


						CBackendRepresentativeMemoryLabelCacheItem* getConceptSetLabel();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* setConceptSetLabel(CBackendRepresentativeMemoryLabelCacheItem* roleSetLabel);



						cint64 getRepresentativeExpandedIndividual();
						cint64 getNeighbourExpansionArrayId();
						cint64 getNextLabelNeighbourExpansionIteratorCursor();
						bool hasAllLabelNeighbourExpanded();
						bool hasAllLabelNeighbourExpansionScheduled();

						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* setRepresentativeExpandedIndividual(cint64 expandedIndi);
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* setNeighbourExpansionArrayId(cint64 arrayId);
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* setNextLabelNeighbourExpansionIteratorCursor(cint64 cursor);
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* setAllLabelNeighbourExpanded(bool allExpanded);
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* setAllLabelNeighbourExpansionScheduled(bool allExpansionScheduled);

						CIndividualProcessNode* getExpandingIndividiaulNode();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* setExpandingIndividiaulNode(CIndividualProcessNode* expIndiNode);

						CBackendNeighbourExpansionQueueDataLinker* getExpandingQueueData();
						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* setExpandingQueueData(CBackendNeighbourExpansionQueueDataLinker* expQueueData);


					// protected methods
					protected:

					// protected variables
					protected:
						bool mDelayedRepresentativeExpansion;
						CBackendRepresentativeMemoryLabelCacheItem* mConceptSetLabel;

						cint64 mRepresentativeExpandedIndi;
						cint64 mNextLabelNeighbourExpansionIteratorCursor;
						cint64 mNeighbourExpansionArrayId;
						bool mAllLabelNeighbourExpanded;
						bool mAllLabelNeighbourExpansionScheduled;

						CIndividualProcessNode* mIndiNode;

						CBackendNeighbourExpansionQueueDataLinker* mExpDataLinker;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODEREPRESENTATIVEMEMORYBACKENDCACHESYNCHRONISATIONLABELNEIGHBOUREXPANSIONDATA_H
