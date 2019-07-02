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

#include "CIndividualPilingProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {




				CIndividualPilingProcessNode::CIndividualPilingProcessNode(CProcessContext* processContext) : mProcessContext(processContext) {
					mMemAllocMan = mProcessContext->getMemoryAllocationManager();
				}


				CIndividualPilingProcessNode* CIndividualPilingProcessNode::initIndividualPilingProcessNode(cint64 individualID) {
					mRoleBackPropHash = nullptr;
					mLinkedRoleSuccHash = nullptr;
					mReapplyConPilLabelSet = nullptr;
					mIndiProcessLinker = nullptr;
					mConceptPilingProcessLinker = nullptr;
					mSubstituteIndiNode = nullptr;
					mCopyIndiNode = nullptr;
					mRequiredBackProp = false;
					mPilingDepth = 0;
					mDependingPilingDepth = 0;
					mMaxRequiredBackPropPilingDepth = 0;

					mDependingIndiNodeLinker = nullptr;
					mDepPilingIndiNode = nullptr;
					mDirectPilingIndiNode = nullptr;
					mRootPilingIndiNode = this;

					mIndiID = individualID;
					mInitializingConceptLinker = nullptr;
					mAssertionConceptLinker = nullptr;
					mInitBackwardPropLinks = nullptr;
					mInitialized = false;
					mInitializedConceptLinker = nullptr;
					mReferenceIndiNode = nullptr;
					mReferenceMode = 0;
					mDirectViolationFlags = nullptr;
					mIndirectViolationFlags = nullptr;
					return this;
				}


				CIndividualPilingProcessNode* CIndividualPilingProcessNode::initRootIndividualPilingProcessNode() {
					return this;
				}


				CIndividualPilingProcessNode* CIndividualPilingProcessNode::initPilingIndividualPilingProcessNode(CIndividualPilingProcessNode* indiNode) {
					setPilingDepth(indiNode->getPilingDepth()+1);
					setDirectPilingIndividualNode(indiNode);
					setRootPilingIndividualNode(indiNode->getRootPilingIndividualNode());
					indiNode->setDependingPilingIndividualNode(this);
					mDependingPilingDepth = 0;
					mMaxRequiredBackPropPilingDepth = 0;

					mReapplyConPilLabelSet = indiNode->mReapplyConPilLabelSet;
					mRoleBackPropHash = indiNode->mRoleBackPropHash;
					mLinkedRoleSuccHash = indiNode->mLinkedRoleSuccHash;

					mDirectViolationFlags = indiNode->mDirectViolationFlags;
					mIndirectViolationFlags = indiNode->mIndirectViolationFlags;
					return this;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::initCopingIndividualPilingProcessNode(CIndividualPilingProcessNode* indiNode, cint64 copyPilingDepth) {
					mPilingDepth = 0;
					mMaxRequiredBackPropPilingDepth = 0;

					if (indiNode->getRoleBackwardPropagationHash(false)) {
						getRoleBackwardPropagationHash(true)->copyRoleBackwardPilingPropagationHash(indiNode->getRoleBackwardPropagationHash(false),copyPilingDepth);
					}
					if (indiNode->getReapplyConceptPilingLabelSet(false)) {
						getReapplyConceptPilingLabelSet(true)->copyReapplyConceptPilingLabelSet(indiNode->getReapplyConceptPilingLabelSet(false),copyPilingDepth);
					}
					if (indiNode->getLinkedRoleSuccessorHash(false)) {
						getLinkedRoleSuccessorHash(true)->copyRoleSuccessorHash(indiNode->getLinkedRoleSuccessorHash(false),copyPilingDepth);
					}


					CXLinker<CIndividualPilingProcessNode*>* depCopyLinker = CObjectAllocator< CXLinker<CIndividualPilingProcessNode*> >::allocateAndConstruct(mMemAllocMan);
					depCopyLinker->initLinker(this);
					indiNode->addCopyDependingIndividualNodeLinker(depCopyLinker);
					mDependingPilingDepth = copyPilingDepth;
					mDirectViolationFlags = indiNode->mDirectViolationFlags;
					mIndirectViolationFlags = indiNode->mIndirectViolationFlags;
					return this;
				}


				bool CIndividualPilingProcessNode::canPilingIndividualPilingProcessNode() {
					return mDepPilingIndiNode != nullptr;
				}


				CReapplyConceptPilingLabelSet* CIndividualPilingProcessNode::getReapplyConceptPilingLabelSet(bool create) {
					if (create && !mReapplyConPilLabelSet) {
						mReapplyConPilLabelSet = CObjectParameterizingAllocator< CReapplyConceptPilingLabelSet,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mReapplyConPilLabelSet->initReapplyConceptPilingLabelSet();
					}
					return mReapplyConPilLabelSet;
				}

				CRoleBackwardPilingPropagationHash* CIndividualPilingProcessNode::getRoleBackwardPropagationHash(bool create) {
					if (create && !mRoleBackPropHash) {
						mRoleBackPropHash = CObjectParameterizingAllocator< CRoleBackwardPilingPropagationHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mRoleBackPropHash->initRoleBackwardPilingPropagationHash();
					}
					return mRoleBackPropHash;
				}

				CLinkedRolePilingSuccessorHash* CIndividualPilingProcessNode::getLinkedRoleSuccessorHash(bool create) {
					if (create && !mLinkedRoleSuccHash) {
						mLinkedRoleSuccHash = CObjectParameterizingAllocator< CLinkedRolePilingSuccessorHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mLinkedRoleSuccHash->initRoleSuccessorHash();
					}
					return mLinkedRoleSuccHash;
				}

				CIndividualPilingProcessNodeLinker* CIndividualPilingProcessNode::getIndividualPilingProcessNodeLinker() {
					return mIndiProcessLinker;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setIndividualPilingProcessNodeLinker(CIndividualPilingProcessNodeLinker* processNodeLinker) {
					mIndiProcessLinker = processNodeLinker;
					return this;
				}

				CConceptPilingProcessLinker* CIndividualPilingProcessNode::getConceptPilingProcessLinker() {
					return mConceptPilingProcessLinker;
				}

				CConceptPilingProcessLinker* CIndividualPilingProcessNode::takeConceptPilingProcessLinker() {
					CConceptPilingProcessLinker* conProcLinker = mConceptPilingProcessLinker;
					if (mConceptPilingProcessLinker) {
						mConceptPilingProcessLinker = mConceptPilingProcessLinker->getNext();
					}
					return conProcLinker;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setConceptPilingProcessLinker(CConceptPilingProcessLinker* conProcessLinker) {
					mConceptPilingProcessLinker = conProcessLinker;
					return this;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::addConceptPilingProcessLinker(CConceptPilingProcessLinker* conProcessLinker) {
					mConceptPilingProcessLinker = conProcessLinker->append(mConceptPilingProcessLinker);
					return this;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::clearConceptPilingProcessLinker() {
					mConceptPilingProcessLinker = nullptr;
					return this;
				}

				bool CIndividualPilingProcessNode::getRequiredBackwardPropagation() {
					return mRequiredBackProp;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setRequiredBackwardPropagation(bool requiredBackProp) {
					mRequiredBackProp = requiredBackProp;
					return this;
				}

				bool CIndividualPilingProcessNode::hasSubstituteIndividualNode() {
					return mSubstituteIndiNode != nullptr;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::getSubstituteIndividualNode() {
					return mSubstituteIndiNode;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setSubstituteIndividualNode(CIndividualPilingProcessNode* indiNode) {
					mSubstituteIndiNode = indiNode;
					return this;
				}

				bool CIndividualPilingProcessNode::hasCopyIndividualNode() {
					return mCopyIndiNode != nullptr;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::getCopyIndividualNode() {
					return mCopyIndiNode;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setCopyIndividualNode(CIndividualPilingProcessNode* indiNode) {
					mCopyIndiNode = indiNode;
					return this;
				}

				bool CIndividualPilingProcessNode::hasCopyDependingIndividualNodeLinker() {
					return mDependingIndiNodeLinker != nullptr;
				}

				CXLinker<CIndividualPilingProcessNode*>* CIndividualPilingProcessNode::getCopyDependingIndividualNodeLinker() {
					return mDependingIndiNodeLinker;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setCopyDependingIndividualNodeLinker(CXLinker<CIndividualPilingProcessNode*>* indiLinker) {
					mDependingIndiNodeLinker = indiLinker;
					return this;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::addCopyDependingIndividualNodeLinker(CXLinker<CIndividualPilingProcessNode*>* indiLinker) {
					if (indiLinker) {
						mDependingIndiNodeLinker = indiLinker->append(mDependingIndiNodeLinker);
					}
					return this;
				}

				bool CIndividualPilingProcessNode::hasDependingPilingIndividualNode() {
					return mDepPilingIndiNode != nullptr;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::getDependingPilingIndividualNode() {
					return mDepPilingIndiNode;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setDependingPilingIndividualNode(CIndividualPilingProcessNode* indiNode) {
					mDepPilingIndiNode = indiNode;
					return this;
				}

				bool CIndividualPilingProcessNode::hasDirectPilingIndividualNode() {
					return mDirectPilingIndiNode != nullptr;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::getDirectPilingIndividualNode() {
					return mDirectPilingIndiNode;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setDirectPilingIndividualNode(CIndividualPilingProcessNode* indiNode) {
					mDirectPilingIndiNode = indiNode;
					return this;
				}

				bool CIndividualPilingProcessNode::hasRootPilingIndividualNode() {
					return mRootPilingIndiNode != nullptr;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::getRootPilingIndividualNode() {
					return mRootPilingIndiNode;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setRootPilingIndividualNode(CIndividualPilingProcessNode* indiNode) {
					mRootPilingIndiNode = indiNode;
					return this;
				}

				cint64 CIndividualPilingProcessNode::getCopyDependingPilingDepth() {
					return mDependingPilingDepth;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setCopyDependingPilingDepth(cint64 copyDepPilingDepth) {
					mDependingPilingDepth = copyDepPilingDepth;
					return this;
				}

				cint64 CIndividualPilingProcessNode::getMaxRequiredBackwardPropagationPilingDepth() {
					return mMaxRequiredBackPropPilingDepth;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setMaxRequiredBackwardPropagationPilingDepth(cint64 maxBackPropPilingDepth) {
					mMaxRequiredBackPropPilingDepth = maxBackPropPilingDepth;
					return this;
				}

				bool CIndividualPilingProcessNode::hasInitializingConcepts() {
					return mInitializingConceptLinker != nullptr;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::clearInitializingConcepts() {
					mInitializingConceptLinker = nullptr;
					return this;
				}


				CXSortedNegLinker<CConcept*>* CIndividualPilingProcessNode::getInitializingConceptLinker() {
					return mInitializingConceptLinker;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setInitializingConceptLinker(CXSortedNegLinker<CConcept*>* initializingConceptLinker) {
					mInitializingConceptLinker = initializingConceptLinker;
					return this;
				}


				bool CIndividualPilingProcessNode::hasAssertionConcepts() {
					return mAssertionConceptLinker != nullptr;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::clearAssertionConcepts() {
					mAssertionConceptLinker = nullptr;
					return this;
				}


				CXNegLinker<CConcept*>* CIndividualPilingProcessNode::getAssertionConceptLinker() {
					return mAssertionConceptLinker;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setAssertionConceptLinker(CXNegLinker<CConcept*>* assertionConceptLinker) {
					mAssertionConceptLinker = assertionConceptLinker;
					return this;
				}


				cint64 CIndividualPilingProcessNode::getIndividualID() {
					return mIndiID;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setIndividualID(cint64 indiID) {
					mIndiID = indiID;
					return this;
				}

				bool CIndividualPilingProcessNode::isInitialized() {
					return mInitialized;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setInitialized(bool initialized) {
					mInitialized = initialized;
					return this;
				}

				CBackwardPilingPropagationLink* CIndividualPilingProcessNode::getInitializingBackwardPropagationLinks() {
					return mInitBackwardPropLinks;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setInitializingBackwardPropagationLinks(CBackwardPilingPropagationLink* backwardPropLinks) {
					mInitBackwardPropLinks = backwardPropLinks;
					return this;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::addInitializingBackwardPropagationLinks(CBackwardPilingPropagationLink* backwardPropLinks) {
					if (backwardPropLinks) {
						mInitBackwardPropLinks = backwardPropLinks->append(mInitBackwardPropLinks);
					}
					return this;
				}

				bool CIndividualPilingProcessNode::hasInitializedConcepts() {
					return mInitializedConceptLinker != nullptr;
				}

				CXSortedNegLinker<CConcept*>* CIndividualPilingProcessNode::getInitializedConceptLinker() {
					return mInitializedConceptLinker;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setInitializedConceptLinker(CXSortedNegLinker<CConcept*>* initializedConceptLinker) {
					mInitializedConceptLinker = initializedConceptLinker;
					return this;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::getReferenceIndividualPilingProcessNode() {
					return mReferenceIndiNode;
				}

				cint64 CIndividualPilingProcessNode::getReferenceMode() {
					return mReferenceMode;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setReferenceIndividualPilingProcessNode(CIndividualPilingProcessNode* refNode) {
					mReferenceIndiNode = refNode;
					return this;
				}

				CIndividualPilingProcessNode* CIndividualPilingProcessNode::setReferenceMode(cint64 refMode) {
					mReferenceMode = refMode;
					return this;
				}

				CIndividualNodeExpressivityViolationFlags* CIndividualPilingProcessNode::getDirectViolationFlags(bool create) {
					if (!mDirectViolationFlags && create) {
						mDirectViolationFlags = CObjectParameterizingAllocator< CIndividualNodeExpressivityViolationFlags,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mDirectViolationFlags->initViolationFlags();
					}
					return mDirectViolationFlags;
				}

				CIndividualNodeExpressivityViolationFlags* CIndividualPilingProcessNode::getIndirectViolationFlags(bool create) {
					if (!mIndirectViolationFlags && create) {
						mIndirectViolationFlags = CObjectParameterizingAllocator< CIndividualNodeExpressivityViolationFlags,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mIndirectViolationFlags->initViolationFlags();
					}
					return mIndirectViolationFlags;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

