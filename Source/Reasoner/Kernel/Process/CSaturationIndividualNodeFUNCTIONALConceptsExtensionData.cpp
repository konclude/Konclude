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

#include "CSaturationIndividualNodeFUNCTIONALConceptsExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::CSaturationIndividualNodeFUNCTIONALConceptsExtensionData(CProcessContext* processContext) : mLinkedSuccRoleFUNCTIONALConceptExtHash(processContext), mLinkedPredRoleFUNCTIONALConceptExtHash(processContext) {
					mProcessContext = processContext;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::initFUNCTIONALConceptsExtensionData(CIndividualSaturationProcessNode* indiProcessNode) {
					mLinkedSuccRoleFUNCTIONALConceptExtHash.initLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash();
					mLinkedPredRoleFUNCTIONALConceptExtHash.initLinkedPredecessorRoleFUNCTIONALConceptsExtensionHash();
					mIndiProcessNode = indiProcessNode;
					mLinkedSuccessorAddedRoleProcessLinker = nullptr;
					mLinkedPredecessorAddedRoleProcessLinker = nullptr;
					mQualFuncAtmostConProcessLinker = nullptr;
					mFunctionalityAddedRoleProcessLinker = nullptr;
					mCopyingInitializingRoleProcessLinker = nullptr;
					mSuccessorExtensionInitialized = false;
					mExtensionProcessingQueued = false;
					mSuccessorExtensionProcessLinker = nullptr;
					mPredecessorExtensionProcessLinker = nullptr;
					mForwardingPredMergedHash = nullptr;
					mQualifiedFunctionalAtmostConceptProcessSet = nullptr;
					return this;
				}

				CRoleSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getLinkedSuccessorAddedRoleProcessLinker() {
					return mLinkedSuccessorAddedRoleProcessLinker;
				}

				CRoleSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::takeLinkedSuccessorAddedRoleProcessLinker() {
					CRoleSaturationProcessLinker* tmpRoleProcessLinker = mLinkedSuccessorAddedRoleProcessLinker;
					mLinkedSuccessorAddedRoleProcessLinker = nullptr;
					return tmpRoleProcessLinker;
				}

				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::isSuccessorExtensionInitialized() {
					return mSuccessorExtensionInitialized;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::setSuccessorExtensionInitialized(bool initialized) {
					mSuccessorExtensionInitialized = initialized;
					return this;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::addLinkedSuccessorAddedRoleProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker) {
					if (roleProcessLinker) {
						mLinkedSuccessorAddedRoleProcessLinker = roleProcessLinker->append(mLinkedSuccessorAddedRoleProcessLinker);
					}
					return this;
				}

				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasLinkedSuccessorAddedProcessLinkerForRole(CRole* role) {
					cint64 maxTestCount = 10;
					for (CRoleSaturationProcessLinker* roleProcessLinkerIt = mLinkedSuccessorAddedRoleProcessLinker; roleProcessLinkerIt && --maxTestCount > 0; roleProcessLinkerIt = roleProcessLinkerIt->getNext()) {
						if (roleProcessLinkerIt->getRole() == role) {
							return true;
						}
					}
					return false;
				}





				CRoleSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getFunctionalityAddedRoleProcessLinker() {
					return mFunctionalityAddedRoleProcessLinker;
				}

				CRoleSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::takeFunctionalityAddedRoleProcessLinker() {
					CRoleSaturationProcessLinker* tmpRoleProcessLinker = mFunctionalityAddedRoleProcessLinker;
					mFunctionalityAddedRoleProcessLinker = nullptr;
					return tmpRoleProcessLinker;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::addFunctionalityAddedRoleProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker) {
					if (roleProcessLinker) {
						mFunctionalityAddedRoleProcessLinker = roleProcessLinker->append(mFunctionalityAddedRoleProcessLinker);
					}
					return this;
				}

				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasFunctionalityAddedProcessLinkerForRole(CRole* role) {
					cint64 maxTestCount = 10;
					for (CRoleSaturationProcessLinker* roleProcessLinkerIt = mFunctionalityAddedRoleProcessLinker; roleProcessLinkerIt && --maxTestCount > 0; roleProcessLinkerIt = roleProcessLinkerIt->getNext()) {
						if (roleProcessLinkerIt->getRole() == role) {
							return true;
						}
					}
					return false;
				}






				CRoleSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getCopyingInitializingRoleProcessLinker() {
					return mCopyingInitializingRoleProcessLinker;
				}

				CRoleSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::takeCopyingInitializingRoleProcessLinker() {
					CRoleSaturationProcessLinker* tmpRoleProcessLinker = mCopyingInitializingRoleProcessLinker;
					mCopyingInitializingRoleProcessLinker = nullptr;
					return tmpRoleProcessLinker;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::addCopyingInitializingRoleProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker) {
					if (roleProcessLinker) {
						mCopyingInitializingRoleProcessLinker = roleProcessLinker->append(mCopyingInitializingRoleProcessLinker);
					}
					return this;
				}

				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasCopyingInitializingProcessLinkerForRole(CRole* role) {
					cint64 maxTestCount = 10;
					for (CRoleSaturationProcessLinker* roleProcessLinkerIt = mCopyingInitializingRoleProcessLinker; roleProcessLinkerIt && --maxTestCount > 0; roleProcessLinkerIt = roleProcessLinkerIt->getNext()) {
						if (roleProcessLinkerIt->getRole() == role) {
							return true;
						}
					}
					return false;
				}





				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::isExtensionProcessingQueued() {
					return mExtensionProcessingQueued;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::setExtensionProcessingQueued(bool queued) {
					mExtensionProcessingQueued = queued;
					return this;
				}

				CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash() {
					return &mLinkedSuccRoleFUNCTIONALConceptExtHash;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getSuccessorFUNCTIONALConceptsExtensionData(CRole* role, bool create) {
					return mLinkedSuccRoleFUNCTIONALConceptExtHash.getSuccessorFunctionalConceptsExtensionData(role);
				}

				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasSuccessorExtensionProcessData() {
					return mSuccessorExtensionProcessLinker != nullptr;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::takeNextSuccessorExtensionProcessData() {
					CSaturationSuccessorFUNCTIONALConceptExtensionData* tmpExtProcessLinker = mSuccessorExtensionProcessLinker;
					if (mSuccessorExtensionProcessLinker) {
						mSuccessorExtensionProcessLinker = mSuccessorExtensionProcessLinker->getNext();
						tmpExtProcessLinker->clearNext();
					}
					return tmpExtProcessLinker;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getSuccessorExtensionProcessDataLinker() {
					return mSuccessorExtensionProcessLinker;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::addSuccessorExtensionProcessData(CSaturationSuccessorFUNCTIONALConceptExtensionData* processData) {
					mSuccessorExtensionProcessLinker = processData->append(mSuccessorExtensionProcessLinker);
					return this;
				}









				CRoleSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getLinkedPredecessorAddedRoleProcessLinker() {
					return mLinkedPredecessorAddedRoleProcessLinker;
				}

				CRoleSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::takeLinkedPredecessorAddedRoleProcessLinker() {
					CRoleSaturationProcessLinker* tmpRoleProcessLinker = mLinkedPredecessorAddedRoleProcessLinker;
					mLinkedPredecessorAddedRoleProcessLinker = nullptr;
					return tmpRoleProcessLinker;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::addLinkedPredecessorAddedRoleProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker) {
					if (roleProcessLinker) {
						mLinkedPredecessorAddedRoleProcessLinker = roleProcessLinker->append(mLinkedPredecessorAddedRoleProcessLinker);
					}
					return this;
				}

				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasLinkedPredecessorAddedProcessLinkerForRole(CRole* role) {
					cint64 maxTestCount = 10;
					for (CRoleSaturationProcessLinker* roleProcessLinkerIt = mLinkedPredecessorAddedRoleProcessLinker; roleProcessLinkerIt && --maxTestCount > 0; roleProcessLinkerIt = roleProcessLinkerIt->getNext()) {
						if (roleProcessLinkerIt->getRole() == role) {
							return true;
						}
					}
					return false;
				}


				CSaturationPredecessorRoleFUNCTIONALConceptsExtensionHash* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getPredecessorRoleFUNCTIONALConceptsExtensionHash() {
					return &mLinkedPredRoleFUNCTIONALConceptExtHash;
				}

				CSaturationPredecessorFUNCTIONALConceptExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getPredecessorFUNCTIONALConceptsExtensionData(CRole* role, bool create) {
					return mLinkedPredRoleFUNCTIONALConceptExtHash.getPredecessorFunctionalConceptsExtensionData(role);
				}

				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasPredecessorExtensionProcessData() {
					return mPredecessorExtensionProcessLinker != nullptr;
				}

				CSaturationPredecessorFUNCTIONALConceptExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::takeNextPredecessorExtensionProcessData() {
					CSaturationPredecessorFUNCTIONALConceptExtensionData* tmpExtProcessLinker = mPredecessorExtensionProcessLinker;
					if (mPredecessorExtensionProcessLinker) {
						mPredecessorExtensionProcessLinker = mPredecessorExtensionProcessLinker->getNext();
						tmpExtProcessLinker->clearNext();
					}
					return tmpExtProcessLinker;
				}

				CSaturationPredecessorFUNCTIONALConceptExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getPredecessorExtensionProcessDataLinker() {
					return mPredecessorExtensionProcessLinker;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::addPredecessorExtensionProcessData(CSaturationPredecessorFUNCTIONALConceptExtensionData* processData) {
					mPredecessorExtensionProcessLinker = processData->append(mPredecessorExtensionProcessLinker);
					return this;
				}





				CPROCESSHASH<CIndividualSaturationProcessNode*,CRole*>* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getForwardingPredecessorMergedHash(bool create) {
					if (create && !mForwardingPredMergedHash) {
						mForwardingPredMergedHash = CObjectParameterizingAllocator< CPROCESSHASH<CIndividualSaturationProcessNode*,CRole*>,CContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					}
					return mForwardingPredMergedHash;
				}



				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasIndividualNodeForwardingPredecessorMerged(CIndividualSaturationProcessNode* indiNode) {
					if (mForwardingPredMergedHash) {
						return mForwardingPredMergedHash->contains(indiNode);
					}
					return false;
				}


				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasIndividualNodeForwardingPredecessorMerged(CIndividualSaturationProcessNode* indiNode, CRole* role) {
					if (!mForwardingPredMergedHash) {
						return false;
					}
					for (CPROCESSHASH<CIndividualSaturationProcessNode*,CRole*>::const_iterator it = mForwardingPredMergedHash->constFind(indiNode), itEnd = mForwardingPredMergedHash->constEnd(); it != itEnd; ++it) {
						if (it.key() == indiNode) {							
							if (it.value() == role) {
								return true;
							}
						} else {
							break;
						}
					}
					return false;
				}



				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::setIndividualNodeForwardingPredecessorMerged(CIndividualSaturationProcessNode* indiNode, CRole* role) {
					if (!hasIndividualNodeForwardingPredecessorMerged(indiNode,role)) {
						getForwardingPredecessorMergedHash(true)->insertMulti(indiNode,role);
					}
					return this;
				}










				CConceptSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getQualifiedFunctionalAtmostConceptProcessLinker() {
					return mQualFuncAtmostConProcessLinker;
				}

				CConceptSaturationProcessLinker* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::takeQualifiedFunctionalAtmostConceptProcessLinker() {
					CConceptSaturationProcessLinker* tmpConProcessLinker = mQualFuncAtmostConProcessLinker;
					mQualFuncAtmostConProcessLinker = nullptr;
					return tmpConProcessLinker;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::addQualifiedFunctionalAtmostConceptProcessLinker(CConceptSaturationProcessLinker* conceptProcessLinker) {
					if (conceptProcessLinker) {
						if (!mQualifiedFunctionalAtmostConceptProcessSet) {
							mQualifiedFunctionalAtmostConceptProcessSet = CObjectParameterizingAllocator< CPROCESSSET<CConceptSaturationDescriptor*>,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						}
						mQualifiedFunctionalAtmostConceptProcessSet->insert(conceptProcessLinker->getConceptSaturationDescriptor());
						mQualFuncAtmostConProcessLinker = conceptProcessLinker->append(mQualFuncAtmostConProcessLinker);
					}
					return this;
				}

				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasQualifiedFunctionalAtmostConceptProcessLinkerForConcept(CConceptSaturationDescriptor* conDes) {
					if (mQualifiedFunctionalAtmostConceptProcessSet && mQualifiedFunctionalAtmostConceptProcessSet->contains(conDes)) {
						return true;
					}
					return false;
				}




			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
