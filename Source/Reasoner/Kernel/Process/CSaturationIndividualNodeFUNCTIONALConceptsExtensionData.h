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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONINDIVIDUALNODEFUNCTIONALCONCEPTSEXTENSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONINDIVIDUALNODEFUNCTIONALCONCEPTSEXTENSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CRoleSaturationProcessLinker.h"
#include "CSaturationSuccessorFUNCTIONALConceptExtensionData.h"
#include "CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CSaturationIndividualNodeFUNCTIONALConceptsExtensionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationIndividualNodeFUNCTIONALConceptsExtensionData {
					// public methods
					public:
						//! Constructor
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData(CProcessContext* processContext);

						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* initFUNCTIONALConceptsExtensionData(CIndividualSaturationProcessNode* indiProcessNode);

						bool isSuccessorExtensionInitialized();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* setSuccessorExtensionInitialized(bool initialized);

						CRoleSaturationProcessLinker* getLinkedSuccessorAddedRoleProcessLinker();
						CRoleSaturationProcessLinker* takeLinkedSuccessorAddedRoleProcessLinker();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* addLinkedSuccessorAddedRoleProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker);
						bool hasLinkedSuccessorAddedProcessLinkerForRole(CRole* role);

						CRoleSaturationProcessLinker* getFunctionalityAddedRoleProcessLinker();
						CRoleSaturationProcessLinker* takeFunctionalityAddedRoleProcessLinker();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* addFunctionalityAddedRoleProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker);
						bool hasFunctionalityAddedProcessLinkerForRole(CRole* role);



						CRoleSaturationProcessLinker* getCopyingInitializingRoleProcessLinker();
						CRoleSaturationProcessLinker* takeCopyingInitializingRoleProcessLinker();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* addCopyingInitializingRoleProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker);
						bool hasCopyingInitializingProcessLinkerForRole(CRole* role);



						bool isExtensionProcessingQueued();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* setExtensionProcessingQueued(bool queued);


						CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash* getLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash();
						CSaturationSuccessorFUNCTIONALConceptExtensionData* getFUNCTIONALConceptsExtensionData(CRole* role, bool create = true);


						bool hasExtensionProcessData();
						CSaturationSuccessorFUNCTIONALConceptExtensionData* takeNextExtensionProcessData();
						CSaturationSuccessorFUNCTIONALConceptExtensionData* getExtensionProcessDataLinker();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* addExtensionProcessData(CSaturationSuccessorFUNCTIONALConceptExtensionData* processData);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						bool mSuccessorExtensionInitialized;
						bool mExtensionProcessingQueued;
						CRoleSaturationProcessLinker* mLinkedSuccessorAddedRoleProcessLinker;
						CRoleSaturationProcessLinker* mFunctionalityAddedRoleProcessLinker;
						CRoleSaturationProcessLinker* mCopyingInitializingRoleProcessLinker;
						CIndividualSaturationProcessNode* mIndiProcessNode;
						CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash mLinkedSuccRoleFUNCTIONALConceptExtHash;
						CSaturationSuccessorFUNCTIONALConceptExtensionData* mExtensionProcessLinker;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONINDIVIDUALNODEFUNCTIONALCONCEPTSEXTENSIONDATA_H
