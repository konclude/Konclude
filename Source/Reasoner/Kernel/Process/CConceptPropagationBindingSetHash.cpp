/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CConceptPropagationBindingSetHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConceptPropagationBindingSetHash::CConceptPropagationBindingSetHash(CProcessContext* processContext) : mProcessContext(processContext) {
				}


				CConceptPropagationBindingSetHash* CConceptPropagationBindingSetHash::initConceptPropagationBindingSetHash(CConceptPropagationBindingSetHash* prevHash) {
					if (prevHash) {
						*this = *prevHash;
					} else {
						CPROCESSHASH<cint64,CConceptPropagationBindingSetHashData>::clear();
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


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
