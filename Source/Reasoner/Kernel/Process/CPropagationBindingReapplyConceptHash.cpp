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

#include "CPropagationBindingReapplyConceptHash.h"
#include "CIndividualProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CPropagationBindingReapplyConceptHash::CPropagationBindingReapplyConceptHash(CProcessContext* processContext) : mProcessContext(processContext) {
				}

				CPropagationBindingReapplyConceptHash* CPropagationBindingReapplyConceptHash::initPropagationBindingReapplyConceptHash(CPropagationBindingReapplyConceptHash* prevHash) {
					if (prevHash) {
						*this = *prevHash;
					} else {
						CPROCESSHASH<TIndividualConceptPair,CPropagationBindingReapplyConceptHashData>::clear();
					}
					return this;
				}


				CPropagationBindingReapplyConceptHash* CPropagationBindingReapplyConceptHash::addPropagationBindingReapplyConceptDescriptor(CIndividualProcessNode* indi, CConcept* concept, CPropagationBindingReapplyConceptDescriptor* reapplyConDes) {
					return addPropagationBindingReapplyConceptDescriptor(TIndividualConceptPair(indi->getIndividualID(),concept),reapplyConDes);
				}


				CPropagationBindingReapplyConceptHash* CPropagationBindingReapplyConceptHash::addPropagationBindingReapplyConceptDescriptor(const TIndividualConceptPair& indiConPair, CPropagationBindingReapplyConceptDescriptor* reapplyConDes) {
					CPropagationBindingReapplyConceptHashData& data = CPROCESSHASH<TIndividualConceptPair,CPropagationBindingReapplyConceptHashData>::operator[](indiConPair);
					reapplyConDes->append(data.getPropagationBindingReapplyConceptDescriptor());
					data.setPropagationBindingReapplyConceptDescriptor(reapplyConDes);
					return this;
				}

				CPropagationBindingReapplyConceptDescriptor* CPropagationBindingReapplyConceptHash::takePropagationBindingReapplyConceptDescriptor(const TIndividualConceptPair& indiConPair) {
					CPropagationBindingReapplyConceptHashData data = CPROCESSHASH<TIndividualConceptPair,CPropagationBindingReapplyConceptHashData>::value(indiConPair);
					CPropagationBindingReapplyConceptDescriptor* reapplyConDes = data.getPropagationBindingReapplyConceptDescriptor();
					data.clearPropagationBindingReapplyConceptDescriptor();
					return reapplyConDes;
				}


				bool CPropagationBindingReapplyConceptHash::hasPropagationBindingReapplyConceptDescriptor(const TIndividualConceptPair& indiConPair) {
					CPropagationBindingReapplyConceptHashData data = CPROCESSHASH<TIndividualConceptPair,CPropagationBindingReapplyConceptHashData>::value(indiConPair);
					return data.getPropagationBindingReapplyConceptDescriptor() != nullptr;
				}


				CPropagationBindingReapplyConceptIterator CPropagationBindingReapplyConceptHash::getPropagationBindingReapplyConceptDescriptorIterator() {
					return CPropagationBindingReapplyConceptIterator(CPROCESSHASH<TIndividualConceptPair,CPropagationBindingReapplyConceptHashData>::begin(),CPROCESSHASH<TIndividualConceptPair,CPropagationBindingReapplyConceptHashData>::end());
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
