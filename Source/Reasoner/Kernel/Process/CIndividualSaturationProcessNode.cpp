/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CIndividualSaturationProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {




				CIndividualSaturationProcessNode::CIndividualSaturationProcessNode(CProcessContext* processContext) : mProcessContext(processContext) {
					mMemAllocMan = mProcessContext->getMemoryAllocationManager();
				}


				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::initIndividualSaturationProcessNode(cint64 individualID, CExtendedConceptReferenceLinkingData* conSatRefLinkData, CIndividualSaturationReferenceLinkingData* indSatRefLinkData) {
					mRoleBackPropHash = nullptr;
					mReapplyConSatLabelSet = nullptr;
					mIndiProcessLinker = nullptr;
					mConceptSaturationProcessLinker = nullptr;
					mSubstituteIndiNode = nullptr;
					mCopyIndiNode = nullptr;
					mRequiredBackProp = false;

					mDependingIndiNodeLinker = nullptr;
					mDepSaturationIndiNode = nullptr;
					mDirectSaturationIndiNode = nullptr;
					mNonInverseConnectedIndiNodeLinker = nullptr;
					mMultipleCardinalityAncestorNodesLinker = nullptr;

					mIndiID = individualID;
					mInitBackwardPropLinks = nullptr;
					mReferenceIndiNode = nullptr;
					mIndiExtensionData = nullptr;
					mClashedConSatDesLinker = nullptr;
					mIndiCompletionLinker = nullptr;
					mReferenceMode = 0;
					mConceptSaturationLinkRefData = conSatRefLinkData;
					mIndividualSaturationLinkRefData = indSatRefLinkData;
					mIntegratedNominalIndi = nullptr;
					mDataValueApplied = false;
					mCacheData = nullptr;
					mNominalIndi = nullptr;
					mSeparatedSaturation = false;
					return this;
				}

				CExtendedConceptReferenceLinkingData* CIndividualSaturationProcessNode::getSaturationConceptReferenceLinking() {
					return mConceptSaturationLinkRefData;
				}

				CIndividualSaturationReferenceLinkingData* CIndividualSaturationProcessNode::getSaturationIndividualReferenceLinking() {
					return mIndividualSaturationLinkRefData;
				}


				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::initRootIndividualSaturationProcessNode() {
					return this;
				}


				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::initCopingIndividualSaturationProcessNode(CIndividualSaturationProcessNode* indiNode, bool tryFlatLabelCopy) {
					if (indiNode->getRoleBackwardPropagationHash(false)) {
						getRoleBackwardPropagationHash(true)->copyRoleBackwardSaturationPropagationHash(indiNode->getRoleBackwardPropagationHash(false),this);
					}
					if (indiNode->getReapplyConceptSaturationLabelSet(false)) {
						getReapplyConceptSaturationLabelSet(true)->copyReapplyConceptSaturationLabelSet(indiNode->getReapplyConceptSaturationLabelSet(false),tryFlatLabelCopy);
					}
					if (indiNode->getSuccessorConnectedNominalSet(false)) {
						getSuccessorConnectedNominalSet(true)->copySuccessorConnectedNominalSet(indiNode->getSuccessorConnectedNominalSet(false));
					}

					mIntegratedNominalIndi = indiNode->mIntegratedNominalIndi;
					if (indiNode->mNominalIndi) {
						mIntegratedNominalIndi = indiNode->mNominalIndi;
					}
					if (mNominalIndi) {
						mIntegratedNominalIndi = mNominalIndi;
					}
					mDataValueApplied = indiNode->mDataValueApplied;

					if (indiNode->getAppliedDatatypeData(false)) {
						getAppliedDatatypeData(true)->setAppliedDataLiteral(indiNode->getAppliedDatatypeData(false)->getAppliedDataLiteral());
						getAppliedDatatypeData(true)->setAppliedDatatype(indiNode->getAppliedDatatypeData(false)->getAppliedDatatype());
					}


					CXNegLinker<CIndividualSaturationProcessNode*>* depCopyLinker = CObjectAllocator< CXNegLinker<CIndividualSaturationProcessNode*> >::allocateAndConstruct(mMemAllocMan);
					depCopyLinker->initNegLinker(this,true);
					indiNode->addCopyDependingIndividualNodeLinker(depCopyLinker);
					return this;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::initSubstituitingIndividualSaturationProcessNode(CIndividualSaturationProcessNode* indiNode) {
					mDirectStatusFlags = indiNode->mDirectStatusFlags;
					mIndirectStatusFlags = indiNode->mIndirectStatusFlags;
					return this;
				}

				CReapplyConceptSaturationLabelSet* CIndividualSaturationProcessNode::getReapplyConceptSaturationLabelSet(bool create) {
					if (create && !mReapplyConSatLabelSet) {
						mReapplyConSatLabelSet = CObjectParameterizingAllocator< CReapplyConceptSaturationLabelSet,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mReapplyConSatLabelSet->initReapplyConceptSaturationLabelSet();
					}
					return mReapplyConSatLabelSet;
				}

				CIndividualSaturationProcessNodeExtensionData* CIndividualSaturationProcessNode::getIndividualExtensionData(bool create) {
					if (create && !mIndiExtensionData) {
						mIndiExtensionData = CObjectParameterizingAllocator< CIndividualSaturationProcessNodeExtensionData,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mIndiExtensionData->initIndividualExtensionData(this);
					}
					return mIndiExtensionData;
				}

				CSaturationDisjunctCommonConceptExtractionData* CIndividualSaturationProcessNode::getDisjunctCommonConceptExtractionData(bool create) {
					if (create) {
						return getIndividualExtensionData(true)->getDisjunctCommonConceptExtractionData(true);
					}
					if (mIndiExtensionData) {
						return mIndiExtensionData->getDisjunctCommonConceptExtractionData(false);
					}
					return nullptr;
				}


				CRoleBackwardSaturationPropagationHash* CIndividualSaturationProcessNode::getRoleBackwardPropagationHash(bool create) {
					if (create && !mRoleBackPropHash) {
						mRoleBackPropHash = CObjectParameterizingAllocator< CRoleBackwardSaturationPropagationHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mRoleBackPropHash->initRoleBackwardSaturationPropagationHash();
					}
					return mRoleBackPropHash;
				}


				CSuccessorConnectedNominalSet* CIndividualSaturationProcessNode::getSuccessorConnectedNominalSet(bool create) {
					CSuccessorConnectedNominalSet* succConnectedNominalSet = nullptr;
					CSaturationIndividualNodeNominalHandlingData* nominalHandlingData = getNominalHandlingData(create);
					if (nominalHandlingData) {
						succConnectedNominalSet = nominalHandlingData->getSuccessorConnectedNominalSet(create);
					}
					return succConnectedNominalSet;
				}



				CCriticalPredecessorRoleCardinalityHash* CIndividualSaturationProcessNode::getCriticalPredecessorRoleCardinalityHash(bool create) {
					if (create) {
						return getIndividualExtensionData(true)->getCriticalPredecessorRoleCardinalityHash(true);
					}
					if (mIndiExtensionData) {
						return mIndiExtensionData->getCriticalPredecessorRoleCardinalityHash(false);
					}
					return nullptr;
				}


				CLinkedRoleSaturationSuccessorHash* CIndividualSaturationProcessNode::getLinkedRoleSuccessorHash(bool create) {
					if (create) {
						return getIndividualExtensionData(true)->getLinkedRoleSuccessorHash(true);
					}
					if (mIndiExtensionData) {
						return mIndiExtensionData->getLinkedRoleSuccessorHash(false);
					}
					return nullptr;
				}


				CCriticalSaturationConceptTypeQueues* CIndividualSaturationProcessNode::getCriticalConceptTypeQueues(bool create) {
					if (create) {
						return getIndividualExtensionData(true)->getCriticalConceptTypeQueues(true);
					}
					if (mIndiExtensionData) {
						return mIndiExtensionData->getCriticalConceptTypeQueues(false);
					}
					return nullptr;
				}


				CSaturationIndividualNodeSuccessorExtensionData* CIndividualSaturationProcessNode::getSuccessorExtensionData(bool create) {
					if (create) {
						return getIndividualExtensionData(true)->getSuccessorExtensionData(true);
					}
					if (mIndiExtensionData) {
						return mIndiExtensionData->getSuccessorExtensionData(false);
					}
					return nullptr;
				}


				CSaturationIndividualNodeNominalHandlingData* CIndividualSaturationProcessNode::getNominalHandlingData(bool create) {
					if (create) {
						return getIndividualExtensionData(true)->getNominalHandlingData(true);
					}
					if (mIndiExtensionData) {
						return mIndiExtensionData->getNominalHandlingData(false);
					}
					return nullptr;
				}


				CSaturationIndividualNodeDatatypeData* CIndividualSaturationProcessNode::getAppliedDatatypeData(bool create) {
					if (create) {
						return getIndividualExtensionData(true)->getAppliedDatatypeData(true);
					}
					if (mIndiExtensionData) {
						return mIndiExtensionData->getAppliedDatatypeData(false);
					}
					return nullptr;
				}

				CIndividualSaturationProcessNodeLinker* CIndividualSaturationProcessNode::getIndividualSaturationProcessNodeLinker() {
					return mIndiProcessLinker;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* processNodeLinker) {
					mIndiProcessLinker = processNodeLinker;
					return this;
				}

				CConceptSaturationProcessLinker* CIndividualSaturationProcessNode::getConceptSaturationProcessLinker() {
					return mConceptSaturationProcessLinker;
				}

				CConceptSaturationProcessLinker* CIndividualSaturationProcessNode::takeConceptSaturationProcessLinker() {
					CConceptSaturationProcessLinker* conProcLinker = mConceptSaturationProcessLinker;
					if (mConceptSaturationProcessLinker) {
						mConceptSaturationProcessLinker = mConceptSaturationProcessLinker->getNext();
					}
					return conProcLinker;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conProcessLinker) {
					mConceptSaturationProcessLinker = conProcessLinker;
					return this;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::addConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conProcessLinker) {
					mConceptSaturationProcessLinker = conProcessLinker->append(mConceptSaturationProcessLinker);
					return this;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::clearConceptSaturationProcessLinker() {
					mConceptSaturationProcessLinker = nullptr;
					return this;
				}



				CSaturationSuccessorRoleAssertionLinker* CIndividualSaturationProcessNode::getRoleAssertionLinker() {
					if (mIndiExtensionData) {
						return mIndiExtensionData->getRoleAssertionLinker();
					}
					return nullptr;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::addRoleAssertionLinker(CSaturationSuccessorRoleAssertionLinker* roleAssertionLinker) {
					getIndividualExtensionData(true)->addRoleAssertionLinker(roleAssertionLinker);
					return this;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::addRoleAssertion(CIndividualSaturationProcessNode* destinationNode, CRole* role, bool roleNegation) {
					getIndividualExtensionData(true)->addRoleAssertion(destinationNode,role,roleNegation);
					return this;
				}




				bool CIndividualSaturationProcessNode::getRequiredBackwardPropagation() {
					return mRequiredBackProp;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setRequiredBackwardPropagation(bool requiredBackProp) {
					mRequiredBackProp = requiredBackProp;
					return this;
				}

				bool CIndividualSaturationProcessNode::hasSubstituteIndividualNode() {
					return mSubstituteIndiNode != nullptr;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::getSubstituteIndividualNode() {
					return mSubstituteIndiNode;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setSubstituteIndividualNode(CIndividualSaturationProcessNode* indiNode) {
					mSubstituteIndiNode = indiNode;
					return this;
				}

				bool CIndividualSaturationProcessNode::hasCopyIndividualNode() {
					return mCopyIndiNode != nullptr;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::getCopyIndividualNode() {
					return mCopyIndiNode;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setCopyIndividualNode(CIndividualSaturationProcessNode* indiNode) {
					mCopyIndiNode = indiNode;
					return this;
				}

				bool CIndividualSaturationProcessNode::hasCopyDependingIndividualNodeLinker() {
					return mDependingIndiNodeLinker != nullptr;
				}

				CXNegLinker<CIndividualSaturationProcessNode*>* CIndividualSaturationProcessNode::getCopyDependingIndividualNodeLinker() {
					return mDependingIndiNodeLinker;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setCopyDependingIndividualNodeLinker(CXNegLinker<CIndividualSaturationProcessNode*>* indiLinker) {
					mDependingIndiNodeLinker = indiLinker;
					return this;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::addCopyDependingIndividualNodeLinker(CXNegLinker<CIndividualSaturationProcessNode*>* indiLinker) {
					if (indiLinker) {
						mDependingIndiNodeLinker = indiLinker->append(mDependingIndiNodeLinker);
					}
					return this;
				}

				bool CIndividualSaturationProcessNode::hasDependingSaturationIndividualNode() {
					return mDepSaturationIndiNode != nullptr;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::getDependingSaturationIndividualNode() {
					return mDepSaturationIndiNode;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setDependingSaturationIndividualNode(CIndividualSaturationProcessNode* indiNode) {
					mDepSaturationIndiNode = indiNode;
					return this;
				}

				bool CIndividualSaturationProcessNode::hasDirectSaturationIndividualNode() {
					return mDirectSaturationIndiNode != nullptr;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::getDirectSaturationIndividualNode() {
					return mDirectSaturationIndiNode;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setDirectSaturationIndividualNode(CIndividualSaturationProcessNode* indiNode) {
					mDirectSaturationIndiNode = indiNode;
					return this;
				}


				cint64 CIndividualSaturationProcessNode::getIndividualID() {
					return mIndiID;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setIndividualID(cint64 indiID) {
					mIndiID = indiID;
					return this;
				}

				bool CIndividualSaturationProcessNode::isInitialized() {
					return mDirectStatusFlags.hasInitializedFlag();
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setInitialized(bool initialized) {
					mDirectStatusFlags.setInitializedFlag(initialized);
					mIndirectStatusFlags.setInitializedFlag(initialized);
					return this;
				}



				bool CIndividualSaturationProcessNode::isCompleted() {
					return mDirectStatusFlags.hasCompletedFlag();
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setCompleted(bool completed) {
					mDirectStatusFlags.setCompletedFlag(completed);
					mIndirectStatusFlags.setCompletedFlag(completed);
					return this;
				}


				CBackwardSaturationPropagationLink* CIndividualSaturationProcessNode::getInitializingBackwardPropagationLinks() {
					return mInitBackwardPropLinks;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setInitializingBackwardPropagationLinks(CBackwardSaturationPropagationLink* backwardPropLinks) {
					mInitBackwardPropLinks = backwardPropLinks;
					return this;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::addInitializingBackwardPropagationLinks(CBackwardSaturationPropagationLink* backwardPropLinks) {
					if (backwardPropLinks) {
						mInitBackwardPropLinks = backwardPropLinks->append(mInitBackwardPropLinks);
					}
					return this;
				}


				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::getReferenceIndividualSaturationProcessNode() {
					return mReferenceIndiNode;
				}

				cint64 CIndividualSaturationProcessNode::getReferenceMode() {
					return mReferenceMode;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setReferenceIndividualSaturationProcessNode(CIndividualSaturationProcessNode* refNode) {
					mReferenceIndiNode = refNode;
					return this;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setReferenceMode(cint64 refMode) {
					mReferenceMode = refMode;
					return this;
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNode::getDirectStatusFlags() {
					return &mDirectStatusFlags;
				}

				CIndividualSaturationProcessNodeStatusFlags* CIndividualSaturationProcessNode::getIndirectStatusFlags() {
					return &mIndirectStatusFlags;
				}



				bool CIndividualSaturationProcessNode::hasClashedConceptSaturationDescriptorLinker() {
					return mClashedConSatDesLinker != nullptr;
				}

				CConceptSaturationDescriptor* CIndividualSaturationProcessNode::getClashedConceptSaturationDescriptorLinker() {
					return mClashedConSatDesLinker;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::addClashedConceptSaturationDescriptorLinker(CConceptSaturationDescriptor* clashConSatDes) {
					mClashedConSatDesLinker = clashConSatDes->append(mClashedConSatDesLinker);
					return this;
				}






				bool CIndividualSaturationProcessNode::hasNonInverseConnectedIndividualNodeLinker() {
					return mNonInverseConnectedIndiNodeLinker != nullptr;
				}

				CXLinker<CIndividualSaturationProcessNode*>* CIndividualSaturationProcessNode::getNonInverseConnectedIndividualNodeLinker() {
					return mNonInverseConnectedIndiNodeLinker;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setNonInverseConnectedIndividualNodeLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker) {
					mNonInverseConnectedIndiNodeLinker = indiLinker;
					return this;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::addNonInverseConnectedIndividualNodeLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker) {
					if (indiLinker) {
						mNonInverseConnectedIndiNodeLinker = indiLinker->append(mNonInverseConnectedIndiNodeLinker);
					}
					return this;
				}




				bool CIndividualSaturationProcessNode::hasNominalIntegrated() {
					return mIntegratedNominalIndi;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setIntegratedNominal(CIndividual* nominalIndi) {
					mIntegratedNominalIndi = nominalIndi;
					return this;
				}

				CIndividual* CIndividualSaturationProcessNode::getIntegratedNominalIndividual() {
					return mIntegratedNominalIndi;
				}




				bool CIndividualSaturationProcessNode::hasDataValueApplied() {
					return mDataValueApplied;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setDataValueApplied(bool dataApplied) {
					mDataValueApplied = dataApplied;
					return this;
				}





				bool CIndividualSaturationProcessNode::hasMultipleCardinalityAncestorNodesLinker() {
					return mMultipleCardinalityAncestorNodesLinker != nullptr;
				}

				CXLinker<CIndividualSaturationProcessNode*>* CIndividualSaturationProcessNode::getMultipleCardinalityAncestorNodesLinker() {
					return mMultipleCardinalityAncestorNodesLinker;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setMultipleCardinalityAncestorNodesLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker) {
					mMultipleCardinalityAncestorNodesLinker = indiLinker;
					return this;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::addMultipleCardinalityAncestorNodesLinker(CXLinker<CIndividualSaturationProcessNode*>* indiLinker) {
					if (indiLinker) {
						mMultipleCardinalityAncestorNodesLinker = indiLinker->append(mMultipleCardinalityAncestorNodesLinker);
					}
					return this;
				}


				CIndividual* CIndividualSaturationProcessNode::getNominalIndividual() {
					return mNominalIndi;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setNominalIndividual(CIndividual* nominalIndi) {
					mNominalIndi = nominalIndi;
					mIntegratedNominalIndi = mNominalIndi;
					return this;
				}




				bool CIndividualSaturationProcessNode::isIndividualSaturationCompletionNodeLinkerQueued() {
					if (mIndiCompletionLinker) {
						return mIndiCompletionLinker->isProcessingQueued();
					}
					return false;
				}

				CIndividualSaturationProcessNodeLinker* CIndividualSaturationProcessNode::getIndividualSaturationCompletionNodeLinker(bool create) {
					if (!mIndiCompletionLinker && create) {
						mIndiCompletionLinker = CObjectAllocator< CIndividualSaturationProcessNodeLinker >::allocateAndConstruct(mMemAllocMan);
						mIndiCompletionLinker->initProcessNodeLinker(this,false);
					}
					return mIndiCompletionLinker;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setIndividualSaturationCompletionNodeLinker(CIndividualSaturationProcessNodeLinker* processNodeLinker) {
					mIndiCompletionLinker = processNodeLinker;
					return this;
				}


				CIndividualSaturationProcessNodeCacheData* CIndividualSaturationProcessNode::getCacheExpansionData() {
					return mCacheData;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setCacheExpansionData(CIndividualSaturationProcessNodeCacheData* cacheData) {
					mCacheData = cacheData;
					return this;
				}


				bool CIndividualSaturationProcessNode::isSeparated() {
					return mSeparatedSaturation;
				}

				CIndividualSaturationProcessNode* CIndividualSaturationProcessNode::setSeparated(bool separated) {
					mSeparatedSaturation = separated;
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

