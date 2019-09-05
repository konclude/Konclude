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

#include "CRepresentativeVariableBindingPathHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeVariableBindingPathHash::CRepresentativeVariableBindingPathHash(CProcessContext* processContext) : mProcessContext(processContext),CPROCESSHASH<cint64,CRepresentativeVariableBindingPathSetHashData>(processContext) {
				}

				CRepresentativeVariableBindingPathHash* CRepresentativeVariableBindingPathHash::initRepresentativeVariableBindingPathHash(CRepresentativeVariableBindingPathHash* prevMap) {
					if (prevMap) {
						CPROCESSHASH<cint64, CRepresentativeVariableBindingPathSetHashData>::operator=(*prevMap);
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
