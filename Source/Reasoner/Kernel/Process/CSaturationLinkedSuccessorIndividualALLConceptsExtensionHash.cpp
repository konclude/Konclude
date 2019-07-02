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

#include "CSaturationLinkedSuccessorIndividualALLConceptsExtensionHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationLinkedSuccessorIndividualALLConceptsExtensionHash::CSaturationLinkedSuccessorIndividualALLConceptsExtensionHash(CProcessContext* processContext) : mProcessContext(processContext),mLinkedSuccIndiALLConceptExtHash(processContext) {
				}

				CSaturationLinkedSuccessorIndividualALLConceptsExtensionHash* CSaturationLinkedSuccessorIndividualALLConceptsExtensionHash::initLinkedSuccessorIndividualALLConceptsExtensionHash() {
					mLinkedSuccIndiALLConceptExtHash.clear();
					return this;
				}

				CPROCESSHASH<CIndividualSaturationProcessNode*,CSaturationLinkedSuccessorIndividualALLConceptsExtensionData*>* CSaturationLinkedSuccessorIndividualALLConceptsExtensionHash::getLinkedSuccessorIndividualALLConceptsExtensionHash() {
					return &mLinkedSuccIndiALLConceptExtHash;
				}

				CSaturationLinkedSuccessorIndividualALLConceptsExtensionData* CSaturationLinkedSuccessorIndividualALLConceptsExtensionHash::getALLConceptsExtensionData(CIndividualSaturationProcessNode* indiNode, bool create) {
					if (create) {
						CSaturationLinkedSuccessorIndividualALLConceptsExtensionData*& data = mLinkedSuccIndiALLConceptExtHash[indiNode];
						if (!data) {
							data = CObjectParameterizingAllocator< CSaturationLinkedSuccessorIndividualALLConceptsExtensionData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							data->initLinkedSuccessorIndividualALLConceptsExtensionData(indiNode);
						}
						return data;
					} else {
						return mLinkedSuccIndiALLConceptExtHash.value(indiNode,nullptr);
					}
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
