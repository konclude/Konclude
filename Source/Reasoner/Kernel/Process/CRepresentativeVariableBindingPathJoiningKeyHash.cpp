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

#include "CRepresentativeVariableBindingPathJoiningKeyHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CRepresentativeVariableBindingPathJoiningKeyHash::CRepresentativeVariableBindingPathJoiningKeyHash(CProcessContext* context) : CPROCESSHASH<CRepresentativeVariableBindingPathJoiningKeyHasher,CRepresentativeVariableBindingPathJoiningKeyHashData>(context) {
					mContext = context;
				}


				CRepresentativeVariableBindingPathJoiningKeyHash* CRepresentativeVariableBindingPathJoiningKeyHash::initRepresentativeVariableBindingPathJoiningKeyHash(CRepresentativeVariableBindingPathJoiningKeyHash* prevHash) {
					if (prevHash) {
						*this = *prevHash;
						mNextRepVarBindPathJoiningKeyTag = prevHash->mNextRepVarBindPathJoiningKeyTag;
					} else {
						CPROCESSHASH<CRepresentativeVariableBindingPathJoiningKeyHasher,CRepresentativeVariableBindingPathJoiningKeyHashData>::clear();
						mNextRepVarBindPathJoiningKeyTag = 1;
					}
					return this;
				}


				CVariableBindingDescriptor* CRepresentativeVariableBindingPathJoiningKeyHash::createVariableBindingHashKeyDescriptor(CVariableBindingPath* varBindPath, CSortedLinker<CVariable*>* keyVars) {
					CMemoryAllocationManager* taskMemMan = mContext->getUsedMemoryAllocationManager();

					CVariableBindingDescriptor* keyVarBindDesLinker = nullptr;
					CVariableBindingDescriptor* lastKeyVarBindDesLinker = nullptr;
					CSortedLinker<CVariable*>* varLinkerIt = keyVars;
					CVariableBindingDescriptor* varBindDesIt = varBindPath->getVariableBindingDescriptorLinker();
					while (varLinkerIt) {
						CVariableBinding* varBind = varBindDesIt->getVariableBinding();
						if (varBind->getBindedVariable() == varLinkerIt->getData()) {
							CVariableBindingDescriptor* nextKeyVarBindDesLinker = CObjectAllocator< CVariableBindingDescriptor >::allocateAndConstruct(taskMemMan);
							nextKeyVarBindDesLinker->initVariableBindingDescriptor(varBind);

							if (lastKeyVarBindDesLinker) {
								lastKeyVarBindDesLinker->setNext(nextKeyVarBindDesLinker);
								lastKeyVarBindDesLinker = nextKeyVarBindDesLinker;
							} else {
								keyVarBindDesLinker = lastKeyVarBindDesLinker = nextKeyVarBindDesLinker;
							}
							varLinkerIt = varLinkerIt->getNext();
							varBindDesIt = varBindDesIt->getNext();
						} else {
							varBindDesIt = varBindDesIt->getNext();
						}
					}
					return keyVarBindDesLinker;
				}


				CRepresentativeVariableBindingPathJoiningKeyData* CRepresentativeVariableBindingPathJoiningKeyHash::getRepresentativeVariableBindingPathJoiningKeyData(CVariableBindingPath* varBindPath, CSortedLinker<CVariable*>* keyVars, bool create) {
					CRepresentativeVariableBindingPathJoiningKeyData* varBindPathJoiningData = nullptr;
					CRepresentativeVariableBindingPathJoiningKeyHasher hasher(varBindPath,keyVars);
					const CRepresentativeVariableBindingPathJoiningKeyHashData& data = CPROCESSHASH<CRepresentativeVariableBindingPathJoiningKeyHasher,CRepresentativeVariableBindingPathJoiningKeyHashData>::value(hasher);
					varBindPathJoiningData = data.mVarBindPathJoiningData;
					if (!varBindPathJoiningData && create) {
						varBindPathJoiningData = CObjectAllocator< CRepresentativeVariableBindingPathJoiningKeyData >::allocateAndConstruct(mContext->getUsedMemoryAllocationManager());
						CVariableBindingDescriptor* keyVarBindDesLinker = createVariableBindingHashKeyDescriptor(varBindPath,keyVars);
						varBindPathJoiningData->initVariableBindingPathJoiningData(keyVarBindDesLinker,mNextRepVarBindPathJoiningKeyTag++);
						CRepresentativeVariableBindingPathJoiningKeyHashData& data = CPROCESSHASH<CRepresentativeVariableBindingPathJoiningKeyHasher,CRepresentativeVariableBindingPathJoiningKeyHashData>::operator[](CRepresentativeVariableBindingPathJoiningKeyHasher(varBindPathJoiningData));
						data.mVarBindPathJoiningData = varBindPathJoiningData;
					}
					return varBindPathJoiningData;
				}


				cint64 CRepresentativeVariableBindingPathJoiningKeyHash::getRepresentativeVariableBindingPathJoiningKey(CVariableBindingPath* varBindPath, CSortedLinker<CVariable*>* keyVars, bool create) {
					CRepresentativeVariableBindingPathJoiningKeyData* varBindPathJoiningData = getRepresentativeVariableBindingPathJoiningKeyData(varBindPath,keyVars,create);
					if (varBindPathJoiningData) {
						return varBindPathJoiningData->getJoiningKey();
					} else {
						return 0;
					}
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
