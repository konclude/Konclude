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

#include "CSaturationLinkedSuccessorIndividualALLConceptsExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationLinkedSuccessorIndividualALLConceptsExtensionData::CSaturationLinkedSuccessorIndividualALLConceptsExtensionData(CProcessContext* processContext) : mProcessContext(processContext) {
				}

				CSaturationLinkedSuccessorIndividualALLConceptsExtensionData* CSaturationLinkedSuccessorIndividualALLConceptsExtensionData::initLinkedSuccessorIndividualALLConceptsExtensionData(CIndividualSaturationProcessNode* indiProcSatNode) {
					mIndiProcSatNode = indiProcSatNode;
					mRoleConceptExtensionHash = nullptr;
					mOnlyRole = nullptr;
					mOnlyAllConceptExtData = nullptr;
					return this;
				}

				CPROCESSHASH<CRole*,CSaturationSuccessorALLConceptExtensionData*>* CSaturationLinkedSuccessorIndividualALLConceptsExtensionData::getRoleConceptExtensionHash() {
					return mRoleConceptExtensionHash;
				}


				CSaturationSuccessorALLConceptExtensionData* CSaturationLinkedSuccessorIndividualALLConceptsExtensionData::getRoleSuccessorALLConceptExtensionData(CRole* role, bool create) {
					if (create) {
						if (mOnlyRole && mOnlyRole != role) {
							if (!mRoleConceptExtensionHash) {
								mRoleConceptExtensionHash = CObjectParameterizingAllocator< CPROCESSHASH<CRole*,CSaturationSuccessorALLConceptExtensionData*>,CContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							}
							CSaturationSuccessorALLConceptExtensionData*& data = (*mRoleConceptExtensionHash)[mOnlyRole];
							data = mOnlyAllConceptExtData;
							mOnlyAllConceptExtData = nullptr;
							mOnlyRole = nullptr;
						}
						if (mRoleConceptExtensionHash) {
							CSaturationSuccessorALLConceptExtensionData*& data = (*mRoleConceptExtensionHash)[role];
							if (!data) {
								data = CObjectParameterizingAllocator<CSaturationSuccessorALLConceptExtensionData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
								data->initSuccessorConceptExtensionData(role,mIndiProcSatNode);
							}
							return data;
						}
						if (!mOnlyRole) {
							mOnlyRole = role;
							mOnlyAllConceptExtData = CObjectParameterizingAllocator<CSaturationSuccessorALLConceptExtensionData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							mOnlyAllConceptExtData->initSuccessorConceptExtensionData(role,mIndiProcSatNode);
						}
						return mOnlyAllConceptExtData;
					} else {
						if (mRoleConceptExtensionHash) {
							return mRoleConceptExtensionHash->value(role,nullptr);
						}
						if (mOnlyRole == role) {
							return mOnlyAllConceptExtData;
						}
						return nullptr;
					}
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
