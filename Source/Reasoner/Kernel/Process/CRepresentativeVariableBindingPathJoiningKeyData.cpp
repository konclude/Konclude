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

#include "CRepresentativeVariableBindingPathJoiningKeyData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CRepresentativeVariableBindingPathJoiningKeyData::CRepresentativeVariableBindingPathJoiningKeyData() {
				}



				CRepresentativeVariableBindingPathJoiningKeyData* CRepresentativeVariableBindingPathJoiningKeyData::initVariableBindingPathJoiningData(CRepresentativeVariableBindingPathJoiningKeyData* prevJoinData) {
					if (prevJoinData) {
						mKeyVarBindDesLinker = prevJoinData->mKeyVarBindDesLinker;
						mHashValueCalculated = prevJoinData->mHashValueCalculated;
						mCalculatedHashValue = prevJoinData->mCalculatedHashValue;
						mJoiningKey = prevJoinData->mJoiningKey;
					} else {
						mKeyVarBindDesLinker = nullptr;
						mHashValueCalculated = false;
						mCalculatedHashValue = 0;
						mJoiningKey = 0;
					}
					return this;
				}


				CRepresentativeVariableBindingPathJoiningKeyData* CRepresentativeVariableBindingPathJoiningKeyData::initVariableBindingPathJoiningData(CVariableBindingDescriptor* keyVarBindDesLinker, cint64 joiningKey) {
					mJoiningKey = joiningKey;
					mKeyVarBindDesLinker = keyVarBindDesLinker;
					mHashValueCalculated = false;
					mCalculatedHashValue = getCalculatedHashValue();
					return this;
				}

				CVariableBindingDescriptor* CRepresentativeVariableBindingPathJoiningKeyData::getKeyVariableBindingDescriptorLinker() const {
					return mKeyVarBindDesLinker;
				}


				cint64 CRepresentativeVariableBindingPathJoiningKeyData::getJoiningKey() const {
					return mJoiningKey;
				}


				cint64 CRepresentativeVariableBindingPathJoiningKeyData::getCalculatedHashValue() const {
					if (!mHashValueCalculated) {
						mHashValueCalculated = true;
						mCalculatedHashValue = 0;
						cint64 multiplier = 13;
						for (CVariableBindingDescriptor* linkerIt = mKeyVarBindDesLinker; linkerIt; linkerIt = linkerIt->getNext()) {
							CVariableBinding* variableBinding = linkerIt->getVariableBinding();
							mCalculatedHashValue += multiplier*(cint64)variableBinding;
							multiplier = multiplier*2+1;
						}
					}
					return mCalculatedHashValue;
				}

				bool CRepresentativeVariableBindingPathJoiningKeyData::isKeyEquivalentTo(const CRepresentativeVariableBindingPathJoiningKeyData& data) const {
					if (mJoiningKey != data.mJoiningKey) {
						return false;
					}
					return true;
				}

				bool CRepresentativeVariableBindingPathJoiningKeyData::isKeyEquivalentTo(CVariableBindingPath* varBindPath) const {
					CVariableBindingDescriptor* linkerIt1 = mKeyVarBindDesLinker;
					CVariableBindingDescriptor* linkerIt2 = varBindPath->getVariableBindingDescriptorLinker();
					while (linkerIt2 && linkerIt1) {
						if (linkerIt2->getVariableBinding()->getBindedVariable() < linkerIt1->getVariableBinding()->getBindedVariable()) {
							linkerIt2 = linkerIt2->getNext();
						} else {
							if (linkerIt1->getVariableBinding() != linkerIt2->getVariableBinding()) {
								return false;
							}
							linkerIt1 = linkerIt1->getNext();
							linkerIt2 = linkerIt2->getNext();
						}
					}
					if (linkerIt1) {
						return false;
					}
					return true;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
