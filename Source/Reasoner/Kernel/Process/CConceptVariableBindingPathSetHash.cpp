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

#include "CConceptVariableBindingPathSetHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConceptVariableBindingPathSetHash::CConceptVariableBindingPathSetHash(CProcessContext* processContext) : mProcessContext(processContext) {
				}


				CConceptVariableBindingPathSetHash* CConceptVariableBindingPathSetHash::initConceptVariableBindingPathSetHash(CConceptVariableBindingPathSetHash* prevHash) {
					if (prevHash) {
						*this = *prevHash;
					} else {
						CPROCESSHASH<cint64,CConceptVariableBindingPathSetHashData>::clear();
					}
					return this;
				}


				CVariableBindingPathSet* CConceptVariableBindingPathSetHash::getVariableBindingPathSet(CConcept* concept, bool localize) {
					CVariableBindingPathSet* propBindSet = nullptr;
					cint64 conTag = concept->getConceptTag();
					if (localize) {
						CConceptVariableBindingPathSetHashData& data = CPROCESSHASH<cint64,CConceptVariableBindingPathSetHashData>::operator[](conTag);
						if (!data.mLocVariableBindingPathSet) {
							CMemoryAllocationManager* taskMemMan = mProcessContext->getUsedMemoryAllocationManager();
							propBindSet = CObjectParameterizingAllocator< CVariableBindingPathSet,CProcessContext* >::allocateAndConstructAndParameterize(taskMemMan,mProcessContext);
							propBindSet->initVariableBindingPathSet(data.mUseVariableBindingPathSet);
							data.mUseVariableBindingPathSet = data.mLocVariableBindingPathSet = propBindSet;
						} else {
							propBindSet = data.mUseVariableBindingPathSet;
						}
					} else {
						CConceptVariableBindingPathSetHashData data = CPROCESSHASH<cint64,CConceptVariableBindingPathSetHashData>::value(conTag);
						propBindSet = data.mUseVariableBindingPathSet;
					}
					return propBindSet;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
