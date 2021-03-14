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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CBACKENDNEIGHBOUREXPANSIONCONTROLLINGDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CBACKENDNEIGHBOUREXPANSIONCONTROLLINGDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"


// Other includes
#include "Reasoner/Kernel/Process/Dependency/CREUSEBACKENDEXPANSIONMODESDependencyNode.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				using namespace Dependency;

				/*! 
				 *
				 *		\class		CBackendNeighbourExpansionControllingData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendNeighbourExpansionControllingData {
					// public methods
					public:
						//! Constructor
						CBackendNeighbourExpansionControllingData(CProcessContext* processContext);

						CBackendNeighbourExpansionControllingData* initExpansionControllingData(CBackendNeighbourExpansionControllingData* contData);

						cint64 getExpandedNeighbourLinkCount();
						CBackendNeighbourExpansionControllingData* incExpandedNeighbourLinkCount(cint64 count = 1);


						CREUSEBACKENDEXPANSIONMODESDependencyNode* getReuseModesDependencyNode();
						CBackendNeighbourExpansionControllingData* setReuseModesDependencyNode(CREUSEBACKENDEXPANSIONMODESDependencyNode* depNode);

						bool hasExpansionReusingMode();

						bool isFixedReuseExpansionMode();
						CBackendNeighbourExpansionControllingData* setFixedReuseExpansionMode(bool active);

						bool isPrioritizedReuseExpansionMode();
						CBackendNeighbourExpansionControllingData* setPrioritizedReuseExpansionMode(bool active);

						CDependencyTrackPoint* getReuseContinuingDependencyTrackPoint();
						CBackendNeighbourExpansionControllingData* setReuseContinuingDependencyTrackPoint(CDependencyTrackPoint* depTrackPoint);

						CXLinker<CIndividualProcessNode*>* getLastBackendExpandedEnsuringExistingIndividualLinksLinker();
						CBackendNeighbourExpansionControllingData* setLastBackendExpandedEnsuringExistingIndividualLinksLinker(CXLinker<CIndividualProcessNode*>* indiLinker);


						CXLinker<CIndividualProcessNode*>* getCutBackendNeighbourExpansionIndividualLinker();
						CBackendNeighbourExpansionControllingData* addCutBackendNeighbourExpansionIndividualLinker(CXLinker<CIndividualProcessNode*>* indiLinker);

						CXLinker<CIndividualProcessNode*>* getLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker();
						CBackendNeighbourExpansionControllingData* setLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker(CXLinker<CIndividualProcessNode*>* indiLinker);

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mExpandedNeighbourLinkCount;
						CREUSEBACKENDEXPANSIONMODESDependencyNode* mReuseModesDepNode;
						bool mFixedReuseExpansionMode;
						bool mPrioritizedReuseExpansionMode;
						CDependencyTrackPoint* mReuseContinuingDependencyTrackPoint;

						CXLinker<CIndividualProcessNode*>* mLastBackendExpandedEnsuringExistingIndividualLinksLinker;
						CXLinker<CIndividualProcessNode*>* mCutBackendNeighbourExpansionIndividualLinker;
						CXLinker<CIndividualProcessNode*>* mLastCutBackendNeighbourExpansionEnsuringExistingIndividualLinksLinker;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CBACKENDNEIGHBOUREXPANSIONCONTROLLINGDATA_H
