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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CROLEBACKWARDPILINGPROPAGATIONHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CROLEBACKWARDPILINGPROPAGATIONHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CBackwardPilingPropagationReapplyDescriptor.h"
#include "CBackwardPilingPropagationLink.h"
#include "CRoleBackwardPilingPropagationHashData.h"
#include "CProcessContext.h"


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
				 *		\class		CRoleBackwardPilingPropagationHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CRoleBackwardPilingPropagationHash {
					// public methods
					public:
						//! Constructor
						CRoleBackwardPilingPropagationHash(CProcessContext* context = nullptr);


						CRoleBackwardPilingPropagationHash* initRoleBackwardPilingPropagationHash();
						CRoleBackwardPilingPropagationHash* copyRoleBackwardPilingPropagationHash(CRoleBackwardPilingPropagationHash* copyRoleBackPilPropHash, cint64 maxCopyPilingDepth);


						CBackwardPilingPropagationReapplyDescriptor* addBackwardPropagationLink(CRole* role, CBackwardPilingPropagationLink* link);
						CBackwardPilingPropagationLink* addBackwardPropagationConceptDescriptor(CRole* role, CBackwardPilingPropagationReapplyDescriptor* reapplyConDes);

						CPROCESSHASH<CRole*,CRoleBackwardPilingPropagationHashData>* getRoleBackwardPropagationDataHash();


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mContext;
						CPROCESSHASH<CRole*,CRoleBackwardPilingPropagationHashData> mRoleBackPropDataHash;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CROLEBACKWARDPILINGPROPAGATIONHASH_H
