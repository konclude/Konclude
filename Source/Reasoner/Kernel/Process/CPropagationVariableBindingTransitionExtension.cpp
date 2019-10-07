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

#include "CPropagationVariableBindingTransitionExtension.h"
#include "CIndividualProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CPropagationVariableBindingTransitionExtension::CPropagationVariableBindingTransitionExtension(CProcessContext* processContext) {
					mProcessContext = processContext;
				}


				CPropagationVariableBindingTransitionExtension* CPropagationVariableBindingTransitionExtension::initPropagationVariableBindingTransitionExtension(CPropagationVariableBindingTransitionExtension* propVarBindTransExtension) {
					if (propVarBindTransExtension) {
						mLastAnalysedPropBindDes = propVarBindTransExtension->mLastAnalysedPropBindDes;
						mLocVarBindTriggerHash = nullptr;
						mUseVarBindTriggerHash = propVarBindTransExtension->mUseVarBindTriggerHash;
						mLocVarBindPathJoiningHash = nullptr;
						mUseVarBindPathJoiningHash = propVarBindTransExtension->mUseVarBindPathJoiningHash;
						mTriggeredVarIndPair = propVarBindTransExtension->mTriggeredVarIndPair;
						mLeftLastVarBindPathJoiningDes = propVarBindTransExtension->mLeftLastVarBindPathJoiningDes;
						mRightLastVarBindPathJoiningDes = propVarBindTransExtension->mRightLastVarBindPathJoiningDes;
						mLastAnalysedPropagateAllFlag = propVarBindTransExtension->mLastAnalysedPropagateAllFlag;
						mProcessingCompleted = propVarBindTransExtension->mProcessingCompleted;
					} else {
						mLastAnalysedPropBindDes = nullptr;
						mUseVarBindTriggerHash = nullptr;
						mLocVarBindTriggerHash = nullptr;
						mUseVarBindPathJoiningHash = nullptr;
						mLocVarBindPathJoiningHash = nullptr;
						mTriggeredVarIndPair = TVariableIndividualPair(nullptr,0);
						mLeftLastVarBindPathJoiningDes = nullptr;
						mRightLastVarBindPathJoiningDes = nullptr;
						mLastAnalysedPropagateAllFlag = false;
						mProcessingCompleted = false;
					}
					return this;
				}

				bool CPropagationVariableBindingTransitionExtension::getLastAnalysedPropagateAllFlag() {
					return mLastAnalysedPropagateAllFlag;
				}

				CPropagationVariableBindingTransitionExtension* CPropagationVariableBindingTransitionExtension::setLastAnalysedPropagateAllFlag(bool propagateAllFlag) {
					mLastAnalysedPropagateAllFlag = propagateAllFlag;
					return this;
				}


				CPropagationBindingDescriptor* CPropagationVariableBindingTransitionExtension::getLastAnalysedPropagationBindingDescriptor() {
					return mLastAnalysedPropBindDes;
				}

				CPropagationVariableBindingTransitionExtension* CPropagationVariableBindingTransitionExtension::setLastAnalysedPropagationBindingDescriptor(CPropagationBindingDescriptor* lastAnalPropBindDes) {
					mLastAnalysedPropBindDes = lastAnalPropBindDes;
					return this;
				}

				CVariableBindingTriggerHash* CPropagationVariableBindingTransitionExtension::getVariableBindingTriggerHash(bool localize) {
					if (localize && !mLocVarBindTriggerHash) {
						CMemoryAllocationManager* taskMemMan = mProcessContext->getUsedMemoryAllocationManager();
						mLocVarBindTriggerHash = CObjectParameterizingAllocator< CVariableBindingTriggerHash,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,mProcessContext);
						if (mUseVarBindTriggerHash) {
							mLocVarBindTriggerHash->initVariableBindingTriggerHash(mUseVarBindTriggerHash);
						}
						mUseVarBindTriggerHash = mLocVarBindTriggerHash;
					}
					return mUseVarBindTriggerHash;
				}



				CVariableBindingPathJoiningHash* CPropagationVariableBindingTransitionExtension::getVariableBindingPathJoiningHash(bool localize) {
					if (localize && !mLocVarBindPathJoiningHash) {
						CMemoryAllocationManager* taskMemMan = mProcessContext->getUsedMemoryAllocationManager();
						mLocVarBindPathJoiningHash = CObjectParameterizingAllocator< CVariableBindingPathJoiningHash,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,mProcessContext);
						if (mUseVarBindPathJoiningHash) {
							*mLocVarBindPathJoiningHash = *mUseVarBindPathJoiningHash;
						}
						mUseVarBindPathJoiningHash = mLocVarBindPathJoiningHash;
					}
					return mUseVarBindPathJoiningHash;
				}


				CVariableBindingPathDescriptor* CPropagationVariableBindingTransitionExtension::getLeftLastVariableBindingPathJoiningDescriptor() {
					return mLeftLastVarBindPathJoiningDes;
				}

				CVariableBindingPathDescriptor* CPropagationVariableBindingTransitionExtension::getRightLastVariableBindingPathJoiningDescriptor() {
					return mRightLastVarBindPathJoiningDes;
				}


				bool CPropagationVariableBindingTransitionExtension::addAnalysedPropagationBindingDescriptorReturnMatched(CPropagationBindingDescriptor* propBindDes, CVariableBindingTriggerLinker** reapplyTriggerLinker) {
					CVariable* variable = propBindDes->getPropagationBinding()->getBindedVariable();
					CIndividualProcessNode* indiNode = propBindDes->getPropagationBinding()->getBindedIndividual();
					TVariableIndividualPair varIndiPair(variable,indiNode->getIndividualNodeID());

					if (mTriggeredVarIndPair == varIndiPair) {
						return true;
					}
					if (mUseVarBindTriggerHash) {
						CVariableBindingTriggerLinker* triggerLinker = mUseVarBindTriggerHash->setTriggeredReturnTriggerLinker(variable,indiNode);
						if (triggerLinker) {
							if (reapplyTriggerLinker) {
								*reapplyTriggerLinker = triggerLinker;
							}
							return true;
						}
					}
					return false;
				}

				CPropagationVariableBindingTransitionExtension* CPropagationVariableBindingTransitionExtension::setLeftLastVariableBindingPathJoiningDescriptor(CVariableBindingPathDescriptor* varBindPathDes) {
					mLeftLastVarBindPathJoiningDes = varBindPathDes;
					return this;
				}

				CPropagationVariableBindingTransitionExtension* CPropagationVariableBindingTransitionExtension::setRightLastVariableBindingPathJoiningDescriptor(CVariableBindingPathDescriptor* varBindPathDes) {
					mRightLastVarBindPathJoiningDes = varBindPathDes;
					return this;
				}


				CPropagationVariableBindingTransitionExtension* CPropagationVariableBindingTransitionExtension::setTriggeredVariableIndividualPair(const TVariableIndividualPair& triggeredVarIndPair) {
					mTriggeredVarIndPair = triggeredVarIndPair;
					return this;
				}


				CPropagationVariableBindingTransitionExtension* CPropagationVariableBindingTransitionExtension::setTriggeredVariableIndividualPair(CVariable* variable, CIndividualProcessNode* indiNode) {
					mTriggeredVarIndPair = TVariableIndividualPair(variable,indiNode->getIndividualNodeID());
					return this;
				}


				TVariableIndividualPair CPropagationVariableBindingTransitionExtension::getTriggeredVariableIndividualPair() {
					return mTriggeredVarIndPair;
				}

				bool CPropagationVariableBindingTransitionExtension::isProcessingCompleted() {
					return mProcessingCompleted;
				}

				CPropagationVariableBindingTransitionExtension* CPropagationVariableBindingTransitionExtension::setProcessingCompleted(bool completed) {
					mProcessingCompleted = completed;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
