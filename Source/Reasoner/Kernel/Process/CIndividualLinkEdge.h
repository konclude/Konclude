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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALLINKEDGE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALLINKEDGE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CLinkEdge.h"


// Other includes
#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualLinkEdge
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualLinkEdge : public CLinkEdge, public CLinkerBase<CIndividualLinkEdge*,CIndividualLinkEdge> {
					// public methods
					public:
						//! Constructor
						CIndividualLinkEdge(CProcessContext* processContext = nullptr);

						CIndividualLinkEdge* initIndividualLinkEdge(CIndividualLinkEdge* link);
						CIndividualLinkEdge* initIndividualLinkEdge(CIndividualProcessNode* creatorIndividual, CIndividualProcessNode* sourceIndividual, CIndividualProcessNode* destinationIndividual, CRole* role, CDependencyTrackPoint* depTrackPoint);

						CIndividualProcessNode* getCreatorIndividual();
						cint64 getCreatorIndividualID();
						bool isCreatorIndividual(CIndividualProcessNode* indi);
						bool isCreatorIndividualID(CIndividualProcessNode* indi);
						bool isCreatorIndividualID(cint64 indiID);

					// protected methods
					protected:

					// protected variables
					protected:
						CIndividualProcessNode* mCreatorIndividual;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALLINKEDGE_H
