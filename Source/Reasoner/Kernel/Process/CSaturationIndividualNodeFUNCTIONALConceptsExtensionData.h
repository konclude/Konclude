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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONINDIVIDUALNODEFUNCTIONALCONCEPTSEXTENSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CSATURATIONINDIVIDUALNODEFUNCTIONALCONCEPTSEXTENSIONDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CRoleSaturationProcessLinker.h"
#include "CConceptSaturationProcessLinker.h"
#include "CSaturationSuccessorFUNCTIONALConceptExtensionData.h"
#include "CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash.h"
#include "CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash.h"
#include "CSaturationPredecessorFUNCTIONALConceptExtensionData.h"

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
						CSaturationSuccessorFUNCTIONALConceptExtensionData* getSuccessorFUNCTIONALConceptsExtensionData(CRole* role, bool create = true);


						bool hasSuccessorExtensionProcessData();
						CSaturationSuccessorFUNCTIONALConceptExtensionData* takeNextSuccessorExtensionProcessData();
						CSaturationSuccessorFUNCTIONALConceptExtensionData* getSuccessorExtensionProcessDataLinker();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* addSuccessorExtensionProcessData(CSaturationSuccessorFUNCTIONALConceptExtensionData* processData);






						CRoleSaturationProcessLinker* getLinkedPredecessorAddedRoleProcessLinker();
						CRoleSaturationProcessLinker* takeLinkedPredecessorAddedRoleProcessLinker();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* addLinkedPredecessorAddedRoleProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker);
						bool hasLinkedPredecessorAddedProcessLinkerForRole(CRole* role);


						CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash* getPredecessorRoleFUNCTIONALConceptsExtensionHash();
						CSaturationPredecessorFUNCTIONALConceptExtensionData* getPredecessorFUNCTIONALConceptsExtensionData(CRole* role, bool create = true);


						bool hasPredecessorExtensionProcessData();
						CSaturationPredecessorFUNCTIONALConceptExtensionData* takeNextPredecessorExtensionProcessData();
						CSaturationPredecessorFUNCTIONALConceptExtensionData* getPredecessorExtensionProcessDataLinker();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* addPredecessorExtensionProcessData(CSaturationPredecessorFUNCTIONALConceptExtensionData* processData);




						CPROCESSHASH<CIndividualSaturationProcessNode*,CRole*>* getForwardingPredecessorMergedHash(bool create = true);
						bool hasIndividualNodeForwardingPredecessorMerged(CIndividualSaturationProcessNode* indiNode, CRole* role);
						bool hasIndividualNodeForwardingPredecessorMerged(CIndividualSaturationProcessNode* indiNode);
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* setIndividualNodeForwardingPredecessorMerged(CIndividualSaturationProcessNode* indiNode, CRole* role);






						CConceptSaturationProcessLinker* getQualifiedFunctionalAtmostConceptProcessLinker();
						CConceptSaturationProcessLinker* takeQualifiedFunctionalAtmostConceptProcessLinker();
						CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* addQualifiedFunctionalAtmostConceptProcessLinker(CConceptSaturationProcessLinker* conceptProcessLinker);
						bool hasQualifiedFunctionalAtmostConceptProcessLinkerForConcept(CConceptSaturationDescriptor* conDes);


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
						CConceptSaturationProcessLinker* mQualFuncAtmostConProcessLinker;
						CIndividualSaturationProcessNode* mIndiProcessNode;

						CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash mLinkedSuccRoleFUNCTIONALConceptExtHash;
						CSaturationSuccessorFUNCTIONALConceptExtensionData* mSuccessorExtensionProcessLinker;

						CRoleSaturationProcessLinker* mLinkedPredecessorAddedRoleProcessLinker;
						CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash mLinkedPredRoleFUNCTIONALConceptExtHash;
						CSaturationPredecessorFUNCTIONALConceptExtensionData* mPredecessorExtensionProcessLinker;

						CPROCESSHASH<CIndividualSaturationProcessNode*,CRole*>* mForwardingPredMergedHash;

						CPROCESSSET<CConceptSaturationDescriptor*>* mQualifiedFunctionalAtmostConceptProcessSet;

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
