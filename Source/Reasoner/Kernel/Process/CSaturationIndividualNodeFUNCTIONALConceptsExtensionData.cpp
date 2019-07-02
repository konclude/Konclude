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

#include "CSaturationIndividualNodeFUNCTIONALConceptsExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::CSaturationIndividualNodeFUNCTIONALConceptsExtensionData(CProcessContext* processContext) : mLinkedSuccRoleFUNCTIONALConceptExtHash(processContext) {
					mProcessContext = processContext;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::initFUNCTIONALConceptsExtensionData(CIndividualSaturationProcessNode* indiProcessNode) {
					mLinkedSuccRoleFUNCTIONALConceptExtHash.initLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash();
					mIndiProcessNode = indiProcessNode;
					mLinkedSuccessorAddedRoleProcessLinker = nullptr;
					mFunctionalityAddedRoleProcessLinker = nullptr;
					mCopyingInitializingRoleProcessLinker = nullptr;
					mSuccessorExtensionInitialized = false;
					mExtensionProcessingQueued = false;
					mExtensionProcessLinker = nullptr;
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

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getFUNCTIONALConceptsExtensionData(CRole* role, bool create) {
					return mLinkedSuccRoleFUNCTIONALConceptExtHash.getFunctionalConceptsExtensionData(role);
				}

				bool CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::hasExtensionProcessData() {
					return mExtensionProcessLinker != nullptr;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::takeNextExtensionProcessData() {
					CSaturationSuccessorFUNCTIONALConceptExtensionData* tmpExtProcessLinker = mExtensionProcessLinker;
					if (mExtensionProcessLinker) {
						mExtensionProcessLinker = mExtensionProcessLinker->getNext();
						tmpExtProcessLinker->clearNext();
					}
					return tmpExtProcessLinker;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::getExtensionProcessDataLinker() {
					return mExtensionProcessLinker;
				}

				CSaturationIndividualNodeFUNCTIONALConceptsExtensionData* CSaturationIndividualNodeFUNCTIONALConceptsExtensionData::addExtensionProcessData(CSaturationSuccessorFUNCTIONALConceptExtensionData* processData) {
					mExtensionProcessLinker = processData->append(mExtensionProcessLinker);
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
