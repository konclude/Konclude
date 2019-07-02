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

#include "CRepresentativeVariableBindingPathJoiningKeyHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CRepresentativeVariableBindingPathJoiningKeyHasher::CRepresentativeVariableBindingPathJoiningKeyHasher(CRepresentativeVariableBindingPathJoiningKeyData* data) {
					mVarBindPath = nullptr;
					mKeyVars = nullptr;
					mJoiningData = data;
					mHashValue = mJoiningData->getCalculatedHashValue();
				}


				CRepresentativeVariableBindingPathJoiningKeyHasher::CRepresentativeVariableBindingPathJoiningKeyHasher(CVariableBindingPath* varBindPath, CSortedLinker<CVariable*>* keyVars) {
					mJoiningData = nullptr;
					mVarBindPath = varBindPath;
					mKeyVars = keyVars;
					mHashValue = calculateHashValue(mVarBindPath,mKeyVars);
				}


				cint64 CRepresentativeVariableBindingPathJoiningKeyHasher::calculateHashValue(CVariableBindingPath* varBindPath, CSortedLinker<CVariable*>* keyVars) {
					cint64 hashValue = 0;
					CSortedLinker<CVariable*>* keyVarsIt = keyVars;
					cint64 multiplier = 13;
					for (CVariableBindingDescriptor* linkerIt = varBindPath->getVariableBindingDescriptorLinker(); keyVarsIt && linkerIt; linkerIt = linkerIt->getNext()) {
						CVariableBinding* variableBinding = linkerIt->getVariableBinding();
						CVariable* variable = variableBinding->getBindedVariable();
						CVariable* keyVariable = keyVarsIt->getData();
						if (variable == keyVariable) {
							hashValue += multiplier*(cint64)variableBinding;
							multiplier = multiplier*2+1;
							keyVarsIt = keyVarsIt->getNext();
						}
					}
					return hashValue;
				}


				cint64 CRepresentativeVariableBindingPathJoiningKeyHasher::getHashValue() const {
					return mHashValue;
				}

				bool CRepresentativeVariableBindingPathJoiningKeyHasher::operator==(const CRepresentativeVariableBindingPathJoiningKeyHasher& hasher) const {
					if (mJoiningData && hasher.mJoiningData) {
						return mJoiningData->isKeyEquivalentTo(*hasher.mJoiningData);
					} else if (mJoiningData && hasher.mVarBindPath) {
						return mJoiningData->isKeyEquivalentTo(hasher.mVarBindPath);
					} else if (mVarBindPath && hasher.mJoiningData) {
						return hasher.mJoiningData->isKeyEquivalentTo(mVarBindPath);
					}
					return false;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
