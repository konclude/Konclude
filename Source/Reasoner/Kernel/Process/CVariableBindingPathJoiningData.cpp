/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CVariableBindingPathJoiningData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CVariableBindingPathJoiningData::CVariableBindingPathJoiningData() {
				}



				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::initVariableBindingPathJoiningData(CVariableBindingPathJoiningData* prevJoinData) {
					if (prevJoinData) {
						mKeyVarBindDesLinker = prevJoinData->mKeyVarBindDesLinker;
						mLeftVarBindPathDesLinker = prevJoinData->mLeftVarBindPathDesLinker;
						mRightVarBindPathDesLinker = prevJoinData->mRightVarBindPathDesLinker;
						mHashValueCalculated = prevJoinData->mHashValueCalculated;
						mCalculatedHashValue = prevJoinData->mCalculatedHashValue;
						mAllKeyTriggersAvailable = prevJoinData->mAllKeyTriggersAvailable;
						mNextKeyTriggerLinker = prevJoinData->mNextKeyTriggerLinker;
					} else {
						mKeyVarBindDesLinker = nullptr;
						mLeftVarBindPathDesLinker = nullptr;
						mRightVarBindPathDesLinker = nullptr;
						mHashValueCalculated = false;
						mCalculatedHashValue = 0;
						mAllKeyTriggersAvailable = false;
						mNextKeyTriggerLinker = nullptr;
					}
					return this;
				}


				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::initVariableBindingPathJoiningData(CVariableBindingDescriptor* keyVarBindDesLinker, CVariableBindingPathDescriptor* leftVarBindPathDesLinker, CVariableBindingPathDescriptor* rightVarBindPathDesLinker) {
					mKeyVarBindDesLinker = keyVarBindDesLinker;
					mLeftVarBindPathDesLinker = leftVarBindPathDesLinker;
					mRightVarBindPathDesLinker = rightVarBindPathDesLinker;
					mHashValueCalculated = false;
					mCalculatedHashValue = 0;
					mAllKeyTriggersAvailable = false;
					mNextKeyTriggerLinker = nullptr;
					return this;
				}

				CVariableBindingDescriptor* CVariableBindingPathJoiningData::getKeyVariableBindingDescriptorLinker() const {
					return mKeyVarBindDesLinker;
				}

				CVariableBindingPathDescriptor* CVariableBindingPathJoiningData::getLeftVariableBindingPathDescriptorLinker() const {
					return mLeftVarBindPathDesLinker;
				}

				CVariableBindingPathDescriptor* CVariableBindingPathJoiningData::getRightVariableBindingPathDescriptorLinker() const {
					return mRightVarBindPathDesLinker;
				}

				CVariableBindingDescriptor* CVariableBindingPathJoiningData::getNextKeyTriggerLinker(bool moveNext) {
					CVariableBindingDescriptor* nextDesLinker = mNextKeyTriggerLinker;
					if (moveNext) {
						mNextKeyTriggerLinker = mNextKeyTriggerLinker->getNext();
					}
					return nextDesLinker;
				}


				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::setKeyVariableBindingDescriptorLinker(CVariableBindingDescriptor* keyVarBindDesLinker) {
					mKeyVarBindDesLinker = keyVarBindDesLinker;
					return this;
				}

				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::setLeftVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* leftVarBindPathDesLinker) {
					mLeftVarBindPathDesLinker = leftVarBindPathDesLinker;
					return this;
				}

				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::setRightVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* rightVarBindPathDesLinker) {
					mRightVarBindPathDesLinker = rightVarBindPathDesLinker;
					return this;
				}

				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::setNextKeyTriggerLinker(CVariableBindingDescriptor* nextKeyTrigger) {
					mNextKeyTriggerLinker = nextKeyTrigger;
					return this;
				}


				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::addKeyVariableBindingDescriptorLinker(CVariableBindingDescriptor* keyVarBindDesLinker) {
					mKeyVarBindDesLinker = keyVarBindDesLinker->append(mKeyVarBindDesLinker);
					return this;
				}

				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::addLeftVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* leftVarBindPathDesLinker) {
					mLeftVarBindPathDesLinker = leftVarBindPathDesLinker->append(mLeftVarBindPathDesLinker);
					return this;
				}

				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::addRightVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* rightVarBindPathDesLinker) {
					mRightVarBindPathDesLinker = rightVarBindPathDesLinker->append(mRightVarBindPathDesLinker);
					return this;
				}


				bool CVariableBindingPathJoiningData::allKeyTriggersAvailable() {
					return mAllKeyTriggersAvailable;
				}

				CVariableBindingPathJoiningData* CVariableBindingPathJoiningData::setAllKeyTriggersAvailable(bool allAvailable) {
					mAllKeyTriggersAvailable = allAvailable;
					return this;
				}


				cint64 CVariableBindingPathJoiningData::getCalculatedHashValue() const {
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

				bool CVariableBindingPathJoiningData::isKeyEquivalentTo(const CVariableBindingPathJoiningData& data) const {
					if (getCalculatedHashValue() != data.getCalculatedHashValue()) {
						return false;
					}
					CVariableBindingDescriptor* linkerIt1 = mKeyVarBindDesLinker;
					CVariableBindingDescriptor* linkerIt2 = data.mKeyVarBindDesLinker;
					while (linkerIt1 && linkerIt2) {
						if (linkerIt1->getVariableBinding() != linkerIt2->getVariableBinding()) {
							return false;
						}
						linkerIt1 = linkerIt1->getNext();
						linkerIt2 = linkerIt2->getNext();
					}
					if (linkerIt1 || linkerIt2) {
						return false;
					}
					return true;
				}

				bool CVariableBindingPathJoiningData::isKeyEquivalentTo(CVariableBindingPath* varBindPath) const {
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
