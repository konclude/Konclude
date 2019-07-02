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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALPILINGPROCESSNODE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALPILINGPROCESSNODE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CIndividualProcessNodeReference.h"
#include "CRoleBackwardPilingPropagationHash.h"
#include "CLinkedRolePilingSuccessorHash.h"
#include "CIndividualPilingProcessNode.h"
#include "CIndividualPilingProcessNodeLinker.h"
#include "CConceptPilingProcessLinker.h"
#include "CReapplyConceptPilingLabelSet.h"
#include "CIndividualNodeExpressivityViolationFlags.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CRole.h"
#include "Reasoner/Ontology/CIndividual.h"


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
				 *		\class		CIndividualPilingProcessNode
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualPilingProcessNode : public CIndividualProcessNodeReference, public CPilingDepth {
					// public methods
					public:
						//! Constructor
						CIndividualPilingProcessNode(CProcessContext* processContext = nullptr);

						CIndividualPilingProcessNode* initIndividualPilingProcessNode(cint64 individualID = 0);
						CIndividualPilingProcessNode* initRootIndividualPilingProcessNode();
						CIndividualPilingProcessNode* initPilingIndividualPilingProcessNode(CIndividualPilingProcessNode* indiNode);
						CIndividualPilingProcessNode* initCopingIndividualPilingProcessNode(CIndividualPilingProcessNode* indiNode, cint64 copyPilingDepth);

						bool canPilingIndividualPilingProcessNode();


						CReapplyConceptPilingLabelSet* getReapplyConceptPilingLabelSet(bool create = true);

						CRoleBackwardPilingPropagationHash* getRoleBackwardPropagationHash(bool create = true);
						CLinkedRolePilingSuccessorHash* getLinkedRoleSuccessorHash(bool create = true);
						CIndividualPilingProcessNodeLinker* getIndividualPilingProcessNodeLinker();
						CIndividualPilingProcessNode* setIndividualPilingProcessNodeLinker(CIndividualPilingProcessNodeLinker* processNodeLinker);

						CConceptPilingProcessLinker* getConceptPilingProcessLinker();
						CConceptPilingProcessLinker* takeConceptPilingProcessLinker();
						CIndividualPilingProcessNode* setConceptPilingProcessLinker(CConceptPilingProcessLinker* conProcessLinker);
						CIndividualPilingProcessNode* addConceptPilingProcessLinker(CConceptPilingProcessLinker* conProcessLinker);
						CIndividualPilingProcessNode* clearConceptPilingProcessLinker();

						bool getRequiredBackwardPropagation();
						CIndividualPilingProcessNode* setRequiredBackwardPropagation(bool requiredBackProp);

						bool hasSubstituteIndividualNode();
						CIndividualPilingProcessNode* getSubstituteIndividualNode();
						CIndividualPilingProcessNode* setSubstituteIndividualNode(CIndividualPilingProcessNode* indiNode);
						
						bool hasCopyIndividualNode();
						CIndividualPilingProcessNode* getCopyIndividualNode();
						CIndividualPilingProcessNode* setCopyIndividualNode(CIndividualPilingProcessNode* indiNode);

						bool hasCopyDependingIndividualNodeLinker();
						CXLinker<CIndividualPilingProcessNode*>* getCopyDependingIndividualNodeLinker();
						CIndividualPilingProcessNode* setCopyDependingIndividualNodeLinker(CXLinker<CIndividualPilingProcessNode*>* indiLinker);
						CIndividualPilingProcessNode* addCopyDependingIndividualNodeLinker(CXLinker<CIndividualPilingProcessNode*>* indiLinker);

						bool hasDependingPilingIndividualNode();
						CIndividualPilingProcessNode* getDependingPilingIndividualNode();
						CIndividualPilingProcessNode* setDependingPilingIndividualNode(CIndividualPilingProcessNode* indiNode);

						bool hasDirectPilingIndividualNode();
						CIndividualPilingProcessNode* getDirectPilingIndividualNode();
						CIndividualPilingProcessNode* setDirectPilingIndividualNode(CIndividualPilingProcessNode* indiNode);

						bool hasRootPilingIndividualNode();
						CIndividualPilingProcessNode* getRootPilingIndividualNode();
						CIndividualPilingProcessNode* setRootPilingIndividualNode(CIndividualPilingProcessNode* indiNode);

						cint64 getCopyDependingPilingDepth();
						CIndividualPilingProcessNode* setCopyDependingPilingDepth(cint64 copyDepPilingDepth);

						cint64 getMaxRequiredBackwardPropagationPilingDepth();
						CIndividualPilingProcessNode* setMaxRequiredBackwardPropagationPilingDepth(cint64 maxBackPropPilingDepth);

						cint64 getIndividualID();
						CIndividualPilingProcessNode* setIndividualID(cint64 indiID);

						bool hasInitializingConcepts();
						CIndividualPilingProcessNode* clearInitializingConcepts();
						CXSortedNegLinker<CConcept*>* getInitializingConceptLinker();
						CIndividualPilingProcessNode* setInitializingConceptLinker(CXSortedNegLinker<CConcept*>* initializingConceptLinker);

						bool hasAssertionConcepts();
						CIndividualPilingProcessNode* clearAssertionConcepts();
						CXNegLinker<CConcept*>* getAssertionConceptLinker();
						CIndividualPilingProcessNode* setAssertionConceptLinker(CXNegLinker<CConcept*>* assertionConceptLinker);

						bool isInitialized();
						CIndividualPilingProcessNode* setInitialized(bool initialized);

						CBackwardPilingPropagationLink* getInitializingBackwardPropagationLinks();
						CIndividualPilingProcessNode* setInitializingBackwardPropagationLinks(CBackwardPilingPropagationLink* backwardPropLinks);
						CIndividualPilingProcessNode* addInitializingBackwardPropagationLinks(CBackwardPilingPropagationLink* backwardPropLinks);

						bool hasInitializedConcepts();
						CXSortedNegLinker<CConcept*>* getInitializedConceptLinker();
						CIndividualPilingProcessNode* setInitializedConceptLinker(CXSortedNegLinker<CConcept*>* initializedConceptLinker);

						CIndividualPilingProcessNode* getReferenceIndividualPilingProcessNode();
						cint64 getReferenceMode();

						CIndividualPilingProcessNode* setReferenceIndividualPilingProcessNode(CIndividualPilingProcessNode* refNode);
						CIndividualPilingProcessNode* setReferenceMode(cint64 refMode);


						CIndividualNodeExpressivityViolationFlags* getDirectViolationFlags(bool create = true);
						CIndividualNodeExpressivityViolationFlags* getIndirectViolationFlags(bool create = true);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;
						CMemoryAllocationManager* mMemAllocMan;

						CRoleBackwardPilingPropagationHash* mRoleBackPropHash;
						CLinkedRolePilingSuccessorHash* mLinkedRoleSuccHash;
						CReapplyConceptPilingLabelSet* mReapplyConPilLabelSet;
						CIndividualPilingProcessNodeLinker* mIndiProcessLinker;
						CConceptPilingProcessLinker* mConceptPilingProcessLinker;
						CIndividualPilingProcessNode* mSubstituteIndiNode;
						CIndividualPilingProcessNode* mCopyIndiNode;
						bool mRequiredBackProp;

						cint64 mDependingPilingDepth;
						CXLinker<CIndividualPilingProcessNode*>* mDependingIndiNodeLinker;

						CIndividualPilingProcessNode* mDepPilingIndiNode;
						CIndividualPilingProcessNode* mDirectPilingIndiNode;
						CIndividualPilingProcessNode* mRootPilingIndiNode;
						cint64 mMaxRequiredBackPropPilingDepth;

						CXSortedNegLinker<CConcept*>* mInitializingConceptLinker;
						CXNegLinker<CConcept*>* mAssertionConceptLinker;
						cint64 mIndiID;
						bool mInitialized;
						CBackwardPilingPropagationLink* mInitBackwardPropLinks;
						CXSortedNegLinker<CConcept*>* mInitializedConceptLinker;


						CIndividualPilingProcessNode* mReferenceIndiNode;
						cint64 mReferenceMode;

						CIndividualNodeExpressivityViolationFlags* mDirectViolationFlags;
						CIndividualNodeExpressivityViolationFlags* mIndirectViolationFlags;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALPILINGPROCESSNODE_H
