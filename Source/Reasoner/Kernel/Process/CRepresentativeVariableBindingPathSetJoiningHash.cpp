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

#include "CRepresentativeVariableBindingPathSetJoiningHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeVariableBindingPathSetJoiningHash::CRepresentativeVariableBindingPathSetJoiningHash(CProcessContext* processContext) : mProcessContext(processContext),CPROCESSHASH<CConcept*,CRepresentativeVariableBindingPathSetJoiningHashData>(processContext) {
				}

				CRepresentativeVariableBindingPathSetJoiningHash* CRepresentativeVariableBindingPathSetJoiningHash::initRepresentativeVariableBindingPathSetJoiningHash(CRepresentativeVariableBindingPathSetJoiningHash* prevHash) {
					if (prevHash) {
						CPROCESSHASH<CConcept*, CRepresentativeVariableBindingPathSetJoiningHashData>::operator =(*prevHash);
					} else {
						CPROCESSHASH<CConcept*,CRepresentativeVariableBindingPathSetJoiningHashData>::clear();
					}
					return this;
				}


				CRepresentativeVariableBindingPathSetJoiningData* CRepresentativeVariableBindingPathSetJoiningHash::getRepresentativeVariableBindingPathSetJoiningData(CConcept* joinConcept, bool create) {
					CRepresentativeVariableBindingPathSetJoiningData* repData = nullptr;
					if (create) {
						CRepresentativeVariableBindingPathSetJoiningHashData& data = CPROCESSHASH<CConcept*,CRepresentativeVariableBindingPathSetJoiningHashData>::operator[](joinConcept);
						repData = data.mUseJoiningData;
						if (!repData) {
							repData = CObjectParameterizingAllocator< CRepresentativeVariableBindingPathSetJoiningData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
							repData->initRepresentativeVariableBindingPathSetJoiningData(nullptr);
							data.mUseJoiningData = repData;
						}
					} else {
						const CRepresentativeVariableBindingPathSetJoiningHashData& data = CPROCESSHASH<CConcept*,CRepresentativeVariableBindingPathSetJoiningHashData>::value(joinConcept);
						repData = data.mUseJoiningData;
					}
					return repData;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
