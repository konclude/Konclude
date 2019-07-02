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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CBACKWARDPILINGPROPAGATIONLINK_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CBACKWARDPILINGPROPAGATIONLINK_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CPilingDepth.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Utilities/Container/CLinker.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CBackwardPilingPropagationLink
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackwardPilingPropagationLink : public CLinkerBase<CBackwardPilingPropagationLink*,CBackwardPilingPropagationLink>, public CPilingDepth {
					// public methods
					public:
						//! Constructor
						CBackwardPilingPropagationLink();


						CBackwardPilingPropagationLink* initBackwardPropagationLink(CIndividualPilingProcessNode* sourceIndividual, cint64 sourcePilingDepth, CRole* role, cint64 pilingDepth);

						CRole* getLinkRole();
						CBackwardPilingPropagationLink* setLinkRole(CRole* role);

						CIndividualPilingProcessNode* getSourceIndividual();

						cint64 getSourcePilingDepth();
						CBackwardPilingPropagationLink* setSourcePilingDepth(cint64 pilingDepth);

					// protected methods
					protected:

					// protected variables
					protected:
						CRole* mRole;
						CIndividualPilingProcessNode* mSourceIndividual;
						cint64 mSourcePilingDepth;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CBACKWARDPILINGPROPAGATIONLINK_H
