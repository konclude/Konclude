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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSUCCESSORROLEHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSUCCESSORROLEHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualLinkEdge.h"
#include "CSuccessorRoleIterator.h"
#include "CProcessContext.h"
#include "CSuccessorIterator.h"

// Other includes

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
				 *		\class		CSuccessorRoleHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSuccessorRoleHash {
					// public methods
					public:
						//! Constructor
						CSuccessorRoleHash(CProcessContext* context = nullptr);

						CSuccessorRoleHash* initSuccessorRoleHash(CSuccessorRoleHash* prevRoleSuccHash);

						CSuccessorRoleHash* insertSuccessorRoleLink(cint64 indi, CIndividualLinkEdge* link);
						CSuccessorRoleHash* removeSuccessor(cint64 indi);

						CSuccessorRoleIterator getSuccessorRoleIterator(cint64 indi);
						bool hasSuccessorIndividualNode(cint64 indi);
						CSuccessorIterator getSuccessorIterator();


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mContext;
						CPROCESSHASH<cint64, CIndividualLinkEdge*>* mSuccessorLinkHash;
						CPROCESSHASH<cint64, CIndividualLinkEdge*>* mPrevSuccessorLinkHash;
						bool mPrevValidatingRequired;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CSUCCESSORROLEHASH_H
