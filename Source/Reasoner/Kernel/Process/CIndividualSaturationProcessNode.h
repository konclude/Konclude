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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONPROCESSNODE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONPROCESSNODE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CIndividualProcessNodeReference.h"
#include "CRoleBackwardSaturationPropagationHash.h"
#include "CLinkedRoleSaturationSuccessorHash.h"
#include "CIndividualSaturationProcessNode.h"
#include "CIndividualSaturationProcessNodeLinker.h"
#include "CConceptSaturationProcessLinker.h"
#include "CReapplyConceptSaturationLabelSet.h"
#include "CIndividualSaturationProcessNodeStatusFlags.h"
#include "CSaturationDisjunctCommonConceptExtractionData.h"
#include "CIndividualSaturationProcessNodeExtensionData.h"
#include "CSaturationIndividualNodeNominalHandlingData.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CRole.h"
#include "Reasoner/Ontology/CIndividual.h"
#include "Reasoner/Ontology/CExtendedConceptReferenceLinkingData.h"


#include "Utilities/Memory/CObjectParameterizingAllocator.h"

#include "Utilities/Container/CSortedNegLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {


				/*! 
				 *
				 *		\class		CIndividualSaturationProcessNode
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualSaturationProcessNode : public CIndividualProcessNodeReference {
					// public methods
					public:
						//! Constructor
						CIndividualSaturationProcessNode(CProcessContext* processContext = nullptr);

						CIndividualSaturationProcessNode* initIndividualSaturationProcessNode(cint64 individualID, CExtendedConceptReferenceLinkingData* conSatRefLinkData);
						CIndividualSaturationProcessNode* initRootIndividualSaturationProcessNode();
						CIndividualSaturationProcessNode* initCopingIndividualSaturationProcessNode(CIndividualSaturationProcessNode* indiNode);
						CIndividualSaturationProcessNode* initSubstituitingIndividualSaturationProcessNode(CIndividualSaturationProcessNode* indiNode);

						CExtendedConceptReferenceLinkingData* getSaturationConceptReferenceLinking();

						CReapplyConceptSaturationLabelSet* getReapplyConceptSaturationLabelSet(bool create = true);
						
						CIndividualSaturationProcessNodeExtensionData* getIndividualExtensionData(bool create = true);
						CSaturationDisjunctCommonConceptExtractionData* getDisjunctCommonConceptExtractionData(bool create = true);
						CLinkedRoleSaturationSuccessorHash* getLinkedRoleSuccessorHash(bool create = true);
						CCriticalSaturationConceptTypeQueues* getCriticalConceptTypeQueues(bool create = true);
						CSaturationIndividualNodeSuccessorExtensionData* getSuccessorExtensionData(bool create = true);
						CSaturationIndividualNodeNominalHandlingData* getNominalHandlingData(bool create = true);

						CRoleBackwardSaturationPropagationHash* getRoleBackwardPropagationHash(bool create = true);
						CIndividualSaturationProcessNodeLinker* getIndividualSaturationProcessNodeLinker();
						CIndividualSaturationProcessNode* setIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* processNodeLinker);

						CConceptSaturationProcessLinker* getConceptSaturationProcessLinker();
						CConceptSaturationProcessLinker* takeConceptSaturationProcessLinker();
						CIndividualSaturationProcessNode* setConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conProcessLinker);
						CIndividualSaturationProcessNode* addConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conProcessLinker);
						CIndividualSaturationProcessNode* clearConceptSaturationProcessLinker();

						bool getRequiredBackwardPropagation();
						CIndividualSaturationProcessNode* setRequiredBackwardPropagation(bool requiredBackProp);

						bool hasSubstituteIndividualNode();
						CIndividualSaturationProcessNode* getSubstituteIndividualNode();
						CIndividualSaturationProcessNode* setSubstituteIndividualNode(CIndividualSaturationProcessNode* indiNode);
						
						bool hasCopyIndividualNode();
						CIndividualSaturationProcessNode* getCopyIndividualNode();
						CIndividualSaturationProcessNode* setCopyIndividualNode(CIndividualSaturationProcessNode* indiNode);

						bool hasCopyDependingIndividualNodeLinker();
						CXLinker<CIndividualSaturationProcessNode*>* getCopyDependingIndividualNodeLinker();
						CIndividualSaturationProcessNode* setCopyDependingIndividualNodeLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker);
						CIndividualSaturationProcessNode* addCopyDependingIndividualNodeLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker);

						bool hasDependingSaturationIndividualNode();
						CIndividualSaturationProcessNode* getDependingSaturationIndividualNode();
						CIndividualSaturationProcessNode* setDependingSaturationIndividualNode(CIndividualSaturationProcessNode* indiNode);

						bool hasDirectSaturationIndividualNode();
						CIndividualSaturationProcessNode* getDirectSaturationIndividualNode();
						CIndividualSaturationProcessNode* setDirectSaturationIndividualNode(CIndividualSaturationProcessNode* indiNode);

						cint64 getIndividualID();
						CIndividualSaturationProcessNode* setIndividualID(cint64 indiID);

						bool isInitialized();
						CIndividualSaturationProcessNode* setInitialized(bool initialized);

						bool isCompleted();
						CIndividualSaturationProcessNode* setCompleted(bool completed);


						CBackwardSaturationPropagationLink* getInitializingBackwardPropagationLinks();
						CIndividualSaturationProcessNode* setInitializingBackwardPropagationLinks(CBackwardSaturationPropagationLink* backwardPropLinks);
						CIndividualSaturationProcessNode* addInitializingBackwardPropagationLinks(CBackwardSaturationPropagationLink* backwardPropLinks);

						CIndividualSaturationProcessNode* getReferenceIndividualSaturationProcessNode();
						cint64 getReferenceMode();

						CIndividualSaturationProcessNode* setReferenceIndividualSaturationProcessNode(CIndividualSaturationProcessNode* refNode);
						CIndividualSaturationProcessNode* setReferenceMode(cint64 refMode);

						CIndividualSaturationProcessNodeStatusFlags* getDirectStatusFlags();
						CIndividualSaturationProcessNodeStatusFlags* getIndirectStatusFlags();

						bool hasClashedConceptSaturationDescriptorLinker();
						CConceptSaturationDescriptor* getClashedConceptSaturationDescriptorLinker();
						CIndividualSaturationProcessNode* addClashedConceptSaturationDescriptorLinker(CConceptSaturationDescriptor* clashConSatDes);
					
						
						bool hasNonInverseConnectedIndividualNodeLinker();
						CXLinker<CIndividualSaturationProcessNode*>* getNonInverseConnectedIndividualNodeLinker();
						CIndividualSaturationProcessNode* setNonInverseConnectedIndividualNodeLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker);
						CIndividualSaturationProcessNode* addNonInverseConnectedIndividualNodeLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker);


						CSuccessorConnectedNominalSet* getSuccessorConnectedNominalSet(bool create = true);

						bool hasNominalIntegrated();
						CIndividualSaturationProcessNode* setNominalIntegrated(bool integrated);

						bool hasMultipleCardinalityAncestorNodesLinker();
						CXLinker<CIndividualSaturationProcessNode*>* getMultipleCardinalityAncestorNodesLinker();
						CIndividualSaturationProcessNode* setMultipleCardinalityAncestorNodesLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker);
						CIndividualSaturationProcessNode* addMultipleCardinalityAncestorNodesLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker);



						bool isIndividualSaturationCompletionNodeLinkerQueued();
						CIndividualSaturationProcessNodeLinker* getIndividualSaturationCompletionNodeLinker(bool create = true);
						CIndividualSaturationProcessNode* setIndividualSaturationCompletionNodeLinker(CIndividualSaturationProcessNodeLinker* processNodeLinker);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CMemoryAllocationManager* mMemAllocMan;

						CExtendedConceptReferenceLinkingData* mSaturationLinkRefData;
						CRoleBackwardSaturationPropagationHash* mRoleBackPropHash;
						CReapplyConceptSaturationLabelSet* mReapplyConSatLabelSet;
						CIndividualSaturationProcessNodeExtensionData* mIndiExtensionData;
						CIndividualSaturationProcessNodeLinker* mIndiProcessLinker;
						CIndividualSaturationProcessNodeLinker* mIndiCompletionLinker;
						CConceptSaturationProcessLinker* mConceptSaturationProcessLinker;
						CIndividualSaturationProcessNode* mSubstituteIndiNode;
						CIndividualSaturationProcessNode* mCopyIndiNode;
						CIndividualSaturationProcessNodeStatusFlags mDirectStatusFlags;
						CIndividualSaturationProcessNodeStatusFlags mIndirectStatusFlags;
						bool mRequiredBackProp;
						bool mIntegratedNominal;
						CConceptSaturationDescriptor* mClashedConSatDesLinker;

						CXLinker<CIndividualSaturationProcessNode*>* mDependingIndiNodeLinker;

						CXLinker<CIndividualSaturationProcessNode*>* mNonInverseConnectedIndiNodeLinker;
						CXLinker<CIndividualSaturationProcessNode*>* mMultipleCardinalityAncestorNodesLinker;

						CIndividualSaturationProcessNode* mDepSaturationIndiNode;
						CIndividualSaturationProcessNode* mDirectSaturationIndiNode;


						cint64 mIndiID;
						CBackwardSaturationPropagationLink* mInitBackwardPropLinks;


						CIndividualSaturationProcessNode* mReferenceIndiNode;
						cint64 mReferenceMode;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALSATURATIONPROCESSNODE_H
