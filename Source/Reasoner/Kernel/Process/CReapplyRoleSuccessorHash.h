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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYROLESUCCESSORHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYROLESUCCESSORHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualLinkEdge.h"
#include "CRoleSuccessorLinkIterator.h"
#include "CReapplyQueue.h"
#include "CProcessContext.h"
#include "CReapplyRoleSuccessorData.h"
#include "CRoleSuccessorIterator.h"


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
				 *		\class		CReapplyRoleSuccessorHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReapplyRoleSuccessorHash {
					// public methods
					public:
						//! Constructor
						CReapplyRoleSuccessorHash(CProcessContext* context = nullptr);

						CReapplyRoleSuccessorHash* initRoleSuccessorHash(CReapplyRoleSuccessorHash* prevRoleSuccHash);

						CReapplyRoleSuccessorHash* insertRoleSuccessorLink(CRole* role, CIndividualLinkEdge* link, CReapplyQueueIterator* reapplyQueueIterator = nullptr);
						CReapplyRoleSuccessorHash* removeRoleSuccessorLink(CRole* role, CIndividualLinkEdge* link);
						CReapplyRoleSuccessorHash* removeRoleSuccessorLink(CRole* role, cint64 sourceIndiID, cint64 destinationIndiID);

						cint64 getRoleSuccessorCount(CRole* role);


						CRoleSuccessorLinkIterator getRoleSuccessorLinkIterator(CRole* role);
						CRoleSuccessorLinkIterator getRoleSuccessorLinkIterator(CRole* role, cint64* linkCount);
						CRoleSuccessorLinkIterator getRoleSuccessorLinkIterator(CRole* role, cint64* linkCount, CIndividualLinkEdge*& lastLink);
						CRoleSuccessorLinkIterator getRoleSuccessorHistoryLinkIterator(CRole* role, CIndividualLinkEdge* lastLink);
						CRoleSuccessorLinkIterator getRoleSuccessorHistoryLinkIterator(CRole* role, CIndividualLinkEdge* lastLink, cint64* linkCount);
						bool hasRoleSuccessor(CRole* role);

						bool hasRoleSuccessorToIndividual(CRole* role, cint64 sourceIndiID, cint64 destinationIndiID, bool locateable);
						CIndividualLinkEdge* getRoleSuccessorToIndividualLink(CRole* role, cint64 sourceIndiID, cint64 destinationIndiID, bool locateable);


						CReapplyQueue* getRoleReapplyQueue(CRole* role, bool create = false);
						bool containsRoleReapplyQueue(CRole* role);

						CReapplyQueueIterator getRoleReapplyIterator(CRole* role, bool clearDynamicReapplyQueue = true);

						CRoleSuccessorIterator getRoleIterator();

					// protected methods
					protected:
						cint64 getCoupledIndividualID(CIndividualLinkEdge* link);
						cint64 getCoupledIndividualID(cint64 indi1ID, cint64 indi2ID);

					// protected variables
					protected:

						CProcessContext* mContext;
						CPROCESSHASH<CRole*,CReapplyRoleSuccessorData> mRoleSuccessorDataHash;
						cint64 mLinkCount;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYROLESUCCESSORHASH_H
