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

#include "CSaturationIndividualNodeALLConceptsExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationIndividualNodeALLConceptsExtensionData::CSaturationIndividualNodeALLConceptsExtensionData(CProcessContext* processContext) : mLinkedSuccIdnALLConceptExtHash(processContext) {
					mProcessContext = processContext;
				}

				CSaturationIndividualNodeALLConceptsExtensionData* CSaturationIndividualNodeALLConceptsExtensionData::initALLConceptsExtensionData(CIndividualSaturationProcessNode* indiProcessNode) {
					mLinkedSuccIdnALLConceptExtHash.initLinkedSuccessorIndividualALLConceptsExtensionHash();
					mIndiProcessNode = indiProcessNode;
					mRoleProcessLinker = nullptr;
					mSuccessorExtensionInitialized = false;
					mExtensionProcessingQueued = false;
					mExtensionProcessLinker = nullptr;
					return this;
				}

				CRoleSaturationProcessLinker* CSaturationIndividualNodeALLConceptsExtensionData::getRoleProcessLinker() {
					return mRoleProcessLinker;
				}

				CRoleSaturationProcessLinker* CSaturationIndividualNodeALLConceptsExtensionData::takeRoleProcessLinker() {
					CRoleSaturationProcessLinker* tmpRoleProcessLinker = mRoleProcessLinker;
					mRoleProcessLinker = nullptr;
					return tmpRoleProcessLinker;
				}

				bool CSaturationIndividualNodeALLConceptsExtensionData::isSuccessorExtensionInitialized() {
					return mSuccessorExtensionInitialized;
				}

				CSaturationIndividualNodeALLConceptsExtensionData* CSaturationIndividualNodeALLConceptsExtensionData::setSuccessorExtensionInitialized(bool initialized) {
					mSuccessorExtensionInitialized = initialized;
					return this;
				}

				CSaturationIndividualNodeALLConceptsExtensionData* CSaturationIndividualNodeALLConceptsExtensionData::addRoleProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker) {
					if (roleProcessLinker) {
						mRoleProcessLinker = roleProcessLinker->append(mRoleProcessLinker);
					}
					return this;
				}

				bool CSaturationIndividualNodeALLConceptsExtensionData::hasProcessLinkerForRole(CRole* role) {
					cint64 maxTestCount = 10;
					for (CRoleSaturationProcessLinker* roleProcessLinkerIt = mRoleProcessLinker; roleProcessLinkerIt && --maxTestCount > 0; roleProcessLinkerIt = roleProcessLinkerIt->getNext()) {
						if (roleProcessLinkerIt->getRole() == role) {
							return true;
						}
					}
					return false;
				}

				bool CSaturationIndividualNodeALLConceptsExtensionData::isExtensionProcessingQueued() {
					return mExtensionProcessingQueued;
				}

				CSaturationIndividualNodeALLConceptsExtensionData* CSaturationIndividualNodeALLConceptsExtensionData::setExtensionProcessingQueued(bool queued) {
					mExtensionProcessingQueued = queued;
					return this;
				}

				CSaturationLinkedSuccessorIndividualALLConceptsExtensionHash* CSaturationIndividualNodeALLConceptsExtensionData::getLinkedSuccessorIndividualALLConceptsExtensionHash() {
					return &mLinkedSuccIdnALLConceptExtHash;
				}

				CSaturationLinkedSuccessorIndividualALLConceptsExtensionData* CSaturationIndividualNodeALLConceptsExtensionData::getALLConceptsExtensionData(CIndividualSaturationProcessNode* indiNode, bool create) {
					return mLinkedSuccIdnALLConceptExtHash.getALLConceptsExtensionData(indiNode,create);
				}

				bool CSaturationIndividualNodeALLConceptsExtensionData::hasExtensionProcessData() {
					return mExtensionProcessLinker != nullptr;
				}

				CSaturationSuccessorALLConceptExtensionData* CSaturationIndividualNodeALLConceptsExtensionData::takeNextExtensionProcessData() {
					CSaturationSuccessorALLConceptExtensionData* tmpExtProcessLinker = mExtensionProcessLinker;
					if (mExtensionProcessLinker) {
						mExtensionProcessLinker = mExtensionProcessLinker->getNext();
						tmpExtProcessLinker->clearNext();
					}
					return tmpExtProcessLinker;
				}

				CSaturationSuccessorALLConceptExtensionData* CSaturationIndividualNodeALLConceptsExtensionData::getExtensionProcessDataLinker() {
					return mExtensionProcessLinker;
				}

				CSaturationIndividualNodeALLConceptsExtensionData* CSaturationIndividualNodeALLConceptsExtensionData::addExtensionProcessData(CSaturationSuccessorALLConceptExtensionData* processData) {
					mExtensionProcessLinker = processData->append(mExtensionProcessLinker);
					return this;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
