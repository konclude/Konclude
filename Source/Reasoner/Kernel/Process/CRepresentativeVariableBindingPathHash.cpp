/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CRepresentativeVariableBindingPathHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeVariableBindingPathHash::CRepresentativeVariableBindingPathHash(CProcessContext* processContext) : mProcessContext(processContext),CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>(processContext) {
				}

				CRepresentativeVariableBindingPathHash* CRepresentativeVariableBindingPathHash::initRepresentativeVariableBindingPathHash(CRepresentativeVariableBindingPathHash* prevMap) {
					if (prevMap) {
						*this = *prevMap;
					} else {
						CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>::clear();
					}
					return this;
				}


				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathHash::getRepresentativeVariableBindingPathSetData(CVariableBindingPath* varBindPath, bool createOrLocalize) {
					CRepresentativeVariableBindingPathSetData* repData = nullptr;
					if (createOrLocalize) {
						CRepresentativeVariableBindingPathSetHashData& data = CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>::operator[](varBindPath->getPropagationID());
						repData = data.mUseDataLinker;
						if (!data.mLocDataLinker) {
							repData = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathSetData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							repData->initRepresentativeVariableBindingPathData(nullptr);
							data.mLocDataLinker = repData;
							data.mUseDataLinker = repData;
						}
					} else {
						const CRepresentativeVariableBindingPathSetHashData& data = CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>::value(varBindPath->getPropagationID());
						repData = data.mUseDataLinker;
					}
					return repData;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
