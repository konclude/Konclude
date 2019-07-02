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

#include "CConceptPropagationBindingSetHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConceptPropagationBindingSetHash::CConceptPropagationBindingSetHash(CProcessContext* processContext) : mProcessContext(processContext) {
					mLastPropagationBindingDescriptor = nullptr;
				}


				CConceptPropagationBindingSetHash* CConceptPropagationBindingSetHash::initConceptPropagationBindingSetHash(CConceptPropagationBindingSetHash* prevHash) {
					if (prevHash) {
						*this = *prevHash;
						mLastPropagationBindingDescriptor = prevHash->mLastPropagationBindingDescriptor;
					} else {
						CPROCESSHASH<cint64,CConceptPropagationBindingSetHashData>::clear();
						mLastPropagationBindingDescriptor = nullptr;
					}
					return this;
				}


				CPropagationBindingSet* CConceptPropagationBindingSetHash::getPropagationBindingSet(CConcept* concept, bool localize) {
					CPropagationBindingSet* propBindSet = nullptr;
					cint64 conTag = concept->getConceptTag();
					if (localize) {
						CConceptPropagationBindingSetHashData& data = CPROCESSHASH<cint64,CConceptPropagationBindingSetHashData>::operator[](conTag);
						if (!data.mLocPropBindingSet) {
							CMemoryAllocationManager* taskMemMan = mProcessContext->getUsedMemoryAllocationManager();
							propBindSet = CObjectParameterizingAllocator< CPropagationBindingSet,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,mProcessContext);
							propBindSet->initPropagationBindingSet(data.mUsePropBindingSet);
							data.mUsePropBindingSet = data.mLocPropBindingSet = propBindSet;
						} else {
							propBindSet = data.mUsePropBindingSet;
						}
					} else {
						CConceptPropagationBindingSetHashData data = CPROCESSHASH<cint64,CConceptPropagationBindingSetHashData>::value(conTag);
						propBindSet = data.mUsePropBindingSet;
					}
					return propBindSet;
				}


				CPropagationBindingDescriptor* CConceptPropagationBindingSetHash::getLastPropagationBindingDescriptor() {
					return mLastPropagationBindingDescriptor;
				}

				CConceptPropagationBindingSetHash* CConceptPropagationBindingSetHash::setLastPropagationBindingDescriptor(CPropagationBindingDescriptor* propBindDes) {
					mLastPropagationBindingDescriptor = propBindDes;
					return this;
				}




			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
