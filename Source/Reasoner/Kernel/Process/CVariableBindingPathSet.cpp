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

#include "CVariableBindingPathSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CVariableBindingPathSet::CVariableBindingPathSet(CProcessContext* processContext) : mProcessContext(processContext),mVarBindPathMap(processContext) {
				}

				CVariableBindingPathSet* CVariableBindingPathSet::initVariableBindingPathSet(CVariableBindingPathSet* prevSet) {
					if (prevSet) {
						mVarBindPathMap.initVariableBindingPathMap(&prevSet->mVarBindPathMap);
						mConceptDescriptor = prevSet->mConceptDescriptor;
						mVarBindPathDesLinker = prevSet->mVarBindPathDesLinker;
					} else {
						mVarBindPathMap.initVariableBindingPathMap(nullptr);
						mConceptDescriptor = nullptr;
						mVarBindPathDesLinker = nullptr;
					}
					return this;
				}

				bool CVariableBindingPathSet::containsVariableBindingPath(CVariableBindingPath* variableBindingPath) {
					return mVarBindPathMap.contains(variableBindingPath->getPropagationID());
				}

				bool CVariableBindingPathSet::containsVariableBindingPath(cint64 bindingID) {
					return mVarBindPathMap.contains(bindingID);
				}

				CVariableBindingPathDescriptor* CVariableBindingPathSet::getVariableBindingPathDescriptor(CVariableBindingPath* variableBindingPath) {
					return mVarBindPathMap.value(variableBindingPath->getPropagationID()).getVariableBindingPathDescriptor();
				}


				CVariableBindingPathMap* CVariableBindingPathSet::getVariableBindingPathMap() {
					return &mVarBindPathMap;
				}

				CVariableBindingPathSet* CVariableBindingPathSet::addVariableBindingPath(CVariableBindingPathDescriptor* varBindPathDes) {
					CVariableBindingPathMapData& data = mVarBindPathMap[varBindPathDes->getVariableBindingPath()->getPropagationID()];
					if (!data.hasVariableBindingPathDescriptor()) {
						data.setVariableBindingPathDescriptor(varBindPathDes);
						mVarBindPathDesLinker = varBindPathDes->append(mVarBindPathDesLinker);
					}
					return this;
				}


				CVariableBindingPathSet* CVariableBindingPathSet::copyVariableBindingPaths(CVariableBindingPathMap* varBindPathMap) {
					if (varBindPathMap) {
						mVarBindPathMap = *varBindPathMap;
					}
					return this;
				}

				CConceptDescriptor* CVariableBindingPathSet::getConceptDescriptor() {
					return mConceptDescriptor;
				}

				CVariableBindingPathSet* CVariableBindingPathSet::setConceptDescriptor(CConceptDescriptor* conDes) {
					mConceptDescriptor = conDes;
					return this;
				}


				CVariableBindingPathSet* CVariableBindingPathSet::addVariableBindingPathDescriptorLinker(CVariableBindingPathDescriptor* varBindPathDesLinker) {
					mVarBindPathDesLinker = varBindPathDesLinker->append(mVarBindPathDesLinker);
					return this;
				}


				CVariableBindingPathDescriptor* CVariableBindingPathSet::getVariableBindingPathDescriptorLinker() {
					return mVarBindPathDesLinker;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
