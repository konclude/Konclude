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

#include "CRepresentativePropagationSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativePropagationSet::CRepresentativePropagationSet(CProcessContext* processContext) : mProcessContext(processContext),mRepPropMap(processContext) {
				}

				CRepresentativePropagationSet* CRepresentativePropagationSet::initRepresentativePropagationSet(CRepresentativePropagationSet* prevSet) {
					if (prevSet) {
						mRepPropMap.initRepresentativePropagationMap(&prevSet->mRepPropMap);
						mIncomingRepPropSignature.initSignature(&prevSet->mIncomingRepPropSignature);
						mConceptDescriptor = prevSet->mConceptDescriptor;
						mIncomingRepPropDesLinker = prevSet->mIncomingRepPropDesLinker;
						mOutgoingRepPropDesLinker = prevSet->mOutgoingRepPropDesLinker;
						mLastProcessedIncomingRepPropDesLinker = prevSet->mLastProcessedIncomingRepPropDesLinker;
					} else {
						mRepPropMap.initRepresentativePropagationMap(nullptr);
						mIncomingRepPropSignature.initSignature(nullptr);
						mConceptDescriptor = nullptr;
						mIncomingRepPropDesLinker = nullptr;
						mOutgoingRepPropDesLinker = nullptr;
						mLastProcessedIncomingRepPropDesLinker = nullptr;
					}
					return this;
				}

				bool CRepresentativePropagationSet::containsRepresentativePropagation(CRepresentativeVariableBindingPathSetData* repVarBindPathSetData) {
					return mRepPropMap.contains(repVarBindPathSetData->getRepresentativeID());
				}

				bool CRepresentativePropagationSet::containsRepresentativePropagation(cint64 repID) {
					return mRepPropMap.contains(repID);
				}

				CRepresentativePropagationDescriptor* CRepresentativePropagationSet::getRepresentativePropagationDescriptor(CRepresentativeVariableBindingPathSetData* repVarBindPathSetData) {
					return mRepPropMap.value(repVarBindPathSetData->getRepresentativeID()).getRepresentativePropagationDescriptor();
				}


				CRepresentativePropagationMap* CRepresentativePropagationSet::getRepresentativePropagationMap() {
					return &mRepPropMap;
				}

				CRepresentativePropagationSet* CRepresentativePropagationSet::addIncomingRepresentativePropagation(CRepresentativePropagationDescriptor* repPropDes) {
					CRepresentativePropagationMapData& data = mRepPropMap[repPropDes->getRepresentativeVariableBindingPathSetData()->getRepresentativeID()];
					if (!data.hasRepresentativePropagationDescriptor()) {
						data.setRepresentativePropagationDescriptor(repPropDes);
						if (mIncomingRepPropDesLinker) {
							mIncomingRepPropSignature.addKey(repPropDes->getRepresentativeVariableBindingPathSetData()->getRepresentativeKey());
						} else {
							mIncomingRepPropSignature.initSignature(repPropDes->getRepresentativeVariableBindingPathSetData()->getKeySignature());
						}
						mIncomingRepPropDesLinker = repPropDes->append(mIncomingRepPropDesLinker);
					}
					return this;
				}


				CRepresentativePropagationSet* CRepresentativePropagationSet::copyRepresentativePropagations(CRepresentativePropagationMap* repPropMap) {
					if (repPropMap) {
						mRepPropMap = *repPropMap;
					}
					return this;
				}

				CConceptDescriptor* CRepresentativePropagationSet::getConceptDescriptor() {
					return mConceptDescriptor;
				}

				CRepresentativePropagationSet* CRepresentativePropagationSet::setConceptDescriptor(CConceptDescriptor* conDes) {
					mConceptDescriptor = conDes;
					return this;
				}


				CRepresentativePropagationSet* CRepresentativePropagationSet::addIncomingRepresentativePropagationDescriptorLinker(CRepresentativePropagationDescriptor* repPropDesLinker) {
					for (CRepresentativePropagationDescriptor* repPropDesLinkerIt = repPropDesLinker; repPropDesLinkerIt; repPropDesLinkerIt = repPropDesLinkerIt->getNext()) {
						mIncomingRepPropSignature.addKey(repPropDesLinkerIt->getRepresentativeVariableBindingPathSetData()->getRepresentativeKey());
					}
					mIncomingRepPropDesLinker = repPropDesLinker->append(mIncomingRepPropDesLinker);
					return this;
				}


				CRepresentativePropagationDescriptor* CRepresentativePropagationSet::getIncomingRepresentativePropagationDescriptorLinker() {
					return mIncomingRepPropDesLinker;
				}

				CRepresentativePropagationSet* CRepresentativePropagationSet::setOutgoingRepresentativePropagationDescriptorLinker(CRepresentativePropagationDescriptor* repPropDesLinker) {
					mOutgoingRepPropDesLinker = repPropDesLinker;
					return this;
				}

				CRepresentativePropagationSet* CRepresentativePropagationSet::addOutgoingRepresentativePropagationDescriptorLinker(CRepresentativePropagationDescriptor* repPropDesLinker) {
					mOutgoingRepPropDesLinker = repPropDesLinker->append(mOutgoingRepPropDesLinker);
					return this;
				}

				CRepresentativePropagationDescriptor* CRepresentativePropagationSet::getOutgoingRepresentativePropagationDescriptorLinker() {
					return mOutgoingRepPropDesLinker;
				}

				cint64 CRepresentativePropagationSet::getIncomingRepresentativePropagationSignatureKey() {
					return mIncomingRepPropSignature.getSignatureValue();
				}

				CRepresentativePropagationDescriptor* CRepresentativePropagationSet::getLastProcessedIncomingRepresentativePropagationDescriptorLinker() {
					return mLastProcessedIncomingRepPropDesLinker;
				}

				CRepresentativePropagationSet* CRepresentativePropagationSet::setLastProcessedIncomingRepresentativePropagationDescriptorLinker(CRepresentativePropagationDescriptor* descriptor) {
					mLastProcessedIncomingRepPropDesLinker = descriptor;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
