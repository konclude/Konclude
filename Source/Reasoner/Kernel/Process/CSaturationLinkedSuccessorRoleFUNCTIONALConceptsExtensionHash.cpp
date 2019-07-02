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

#include "CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash::CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash(CProcessContext* processContext) : mProcessContext(processContext),mLinkedSuccRoleFUNCTIONALConceptExtHash(processContext) {
				}

				CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash* CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash::initLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash() {
					mLinkedSuccRoleFUNCTIONALConceptExtHash.clear();
					return this;
				}

				CPROCESSHASH<CRole*,CSaturationSuccessorFUNCTIONALConceptExtensionData*>* CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash::getLinkedSuccessorIndividualFUNCTIONALConceptsExtensionHash() {
					return &mLinkedSuccRoleFUNCTIONALConceptExtHash;
				}

				CSaturationSuccessorFUNCTIONALConceptExtensionData* CSaturationLinkedSuccessorRoleFUNCTIONALConceptsExtensionHash::getFunctionalConceptsExtensionData(CRole* role, bool create) {
					if (create) {
						CSaturationSuccessorFUNCTIONALConceptExtensionData*& data = mLinkedSuccRoleFUNCTIONALConceptExtHash[role];
						if (!data) {
							data = CObjectParameterizingAllocator< CSaturationSuccessorFUNCTIONALConceptExtensionData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							data->initSuccessorConceptExtensionData(role);
						}
						return data;
					} else {
						return mLinkedSuccRoleFUNCTIONALConceptExtHash.value(role,nullptr);
					}
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
