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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONPROCESSNODEEXTENSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONPROCESSNODEEXTENSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CSaturationDisjunctCommonConceptExtractionData.h"
#include "CLinkedRoleSaturationSuccessorHash.h"
#include "CCriticalSaturationConceptTypeQueues.h"
#include "CSaturationIndividualNodeSuccessorExtensionData.h"
#include "CSaturationIndividualNodeNominalHandlingData.h"
#include "CCriticalPredecessorRoleCardinalityHash.h"
#include "CSaturationIndividualNodeDatatypeData.h"
#include "CSaturationATMOSTSuccessorMergingData.h"
#include "CLinkedNeighbourRoleAssertionSaturationHash.h"
#include "CLinkedDataValueAssertionSaturationData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualSaturationProcessNodeExtensionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualSaturationProcessNodeExtensionData {
					// public methods
					public:
						//! Constructor
						CIndividualSaturationProcessNodeExtensionData(CProcessContext* processContext = nullptr);
						
						CIndividualSaturationProcessNodeExtensionData* initIndividualExtensionData(CIndividualSaturationProcessNode* indiNode);

						CSaturationDisjunctCommonConceptExtractionData* getDisjunctCommonConceptExtractionData(bool create = true);
						CLinkedRoleSaturationSuccessorHash* getLinkedRoleSuccessorHash(bool create = true);
						CLinkedNeighbourRoleAssertionSaturationHash* getLinkedNeighbourRoleAssertionHash(bool create = true);
						CLinkedDataValueAssertionSaturationData* getLinkedDataValueAssertionData(bool create = true);
						
						CCriticalSaturationConceptTypeQueues* getCriticalConceptTypeQueues(bool create = true);
						
						CSaturationIndividualNodeSuccessorExtensionData* getSuccessorExtensionData(bool create = true);

						CSaturationIndividualNodeNominalHandlingData* getNominalHandlingData(bool create = true);

						CCriticalPredecessorRoleCardinalityHash* getCriticalPredecessorRoleCardinalityHash(bool create = true);

						CSaturationIndividualNodeDatatypeData* getAppliedDatatypeData(bool create = true);

						CSaturationATMOSTSuccessorMergingData* getATMOSTSuccessorMergingData(bool create = true);

						CSaturationSuccessorRoleAssertionLinker* getRoleAssertionLinker();
						CIndividualSaturationProcessNodeExtensionData* addRoleAssertionLinker(CSaturationSuccessorRoleAssertionLinker* roleAssertionLinker);
						CIndividualSaturationProcessNodeExtensionData* addRoleAssertion(CIndividualSaturationProcessNode* destinationNode, CRole* role, bool roleNegation);


					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CMemoryAllocationManager* mMemAllocMan;

						CIndividualSaturationProcessNode* mIndiNode;
						CSaturationDisjunctCommonConceptExtractionData* mDisComConExtData;
						CLinkedRoleSaturationSuccessorHash* mLinkedRoleSuccHash;
						CCriticalSaturationConceptTypeQueues* mCriticalConceptTypeQueues;
						CSaturationIndividualNodeSuccessorExtensionData* mSuccessorExtensionData;
						CSaturationIndividualNodeNominalHandlingData* mNominalHandlingData;
						CSaturationATMOSTSuccessorMergingData* mATMOSTSuccessorMergingData;

						CCriticalPredecessorRoleCardinalityHash* mCriticalPredRoleCardHash;

						CSaturationSuccessorRoleAssertionLinker* mRoleAssertionLinker;

						CSaturationIndividualNodeDatatypeData* mAppliedDatatypeData;

						CLinkedNeighbourRoleAssertionSaturationHash* mLinkedNeighbourRoleAssertionHash;
						CLinkedDataValueAssertionSaturationData* mLinkedDataValueAssertionData;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONPROCESSNODEEXTENSIONDATA_H
