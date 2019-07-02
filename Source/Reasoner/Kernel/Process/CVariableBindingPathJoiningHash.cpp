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

#include "CVariableBindingPathJoiningHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CVariableBindingPathJoiningHash::CVariableBindingPathJoiningHash(CProcessContext* context) : CPROCESSHASH<CVariableBindingPathJoiningHasher,CVariableBindingPathJoiningHashData>(context) {
					mContext = context;
				}



				CVariableBindingPathJoiningData* CVariableBindingPathJoiningHash::getVariableBindingPathJoiningData(const CVariableBindingPathJoiningHasher& hasher, bool localize) {
					CVariableBindingPathJoiningData* varBindPathJoiningData = nullptr;
					CVariableBindingPathJoiningHashData data = CPROCESSHASH<CVariableBindingPathJoiningHasher,CVariableBindingPathJoiningHashData>::value(hasher);
					varBindPathJoiningData = data.mUseVarBindPathJoiningData;
					if (CPROCESSHASH<CVariableBindingPathJoiningHasher,CVariableBindingPathJoiningHashData>::contains(hasher)) {
						CVariableBindingPathJoiningHashData& data = CPROCESSHASH<CVariableBindingPathJoiningHasher,CVariableBindingPathJoiningHashData>::operator[](hasher);
						if (!data.mLocVarBindPathJoiningData && data.mUseVarBindPathJoiningData) {
							data.mLocVarBindPathJoiningData = CObjectAllocator< CVariableBindingPathJoiningData >::allocateAndConstruct(mContext->getUsedMemoryAllocationManager());
							data.mLocVarBindPathJoiningData->initVariableBindingPathJoiningData(data.mUseVarBindPathJoiningData);
							data.mUseVarBindPathJoiningData = data.mLocVarBindPathJoiningData;
						}
						varBindPathJoiningData = data.mUseVarBindPathJoiningData;
					}
					return varBindPathJoiningData;
				}


				CVariableBindingPathJoiningHash* CVariableBindingPathJoiningHash::insertVariableBindingPathJoiningData(const CVariableBindingPathJoiningHasher& hasher, CVariableBindingPathJoiningData* joinData) {
					CVariableBindingPathJoiningHashData& data = CPROCESSHASH<CVariableBindingPathJoiningHasher,CVariableBindingPathJoiningHashData>::operator[](hasher);
					data.mUseVarBindPathJoiningData = data.mLocVarBindPathJoiningData = joinData;
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
