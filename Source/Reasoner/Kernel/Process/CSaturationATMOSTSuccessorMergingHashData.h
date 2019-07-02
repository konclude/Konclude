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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONATMOSTSUCCESSORMERGINGHASHDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONATMOSTSUCCESSORMERGINGHASHDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"

// Logger includes
#include "Logger/CLogger.h"

#include "Utilities/Container/CNegLinker.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CSaturationATMOSTSuccessorMergingHashData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationATMOSTSuccessorMergingHashData {
					// public methods
					public:
						//! Constructor
						CSaturationATMOSTSuccessorMergingHashData();


						bool mInitialized;
						bool mQueued;
						CIndividualSaturationSuccessorLinkDataLinker* mSuccessorLinkMergingLinker;
						CIndividualSaturationProcessNode* mLastSuccessorNode;
						CXNegLinker<CRole*>* mLastSuccessorCreationRoleLinker;

						cint64 mFoundCardinality;
						cint64 mMergeableCardinality;
						cint64 mMinCardinality;


					// protected methods
					protected:

					// protected variables
					protected:


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONATMOSTSUCCESSORMERGINGHASHDATA_H
