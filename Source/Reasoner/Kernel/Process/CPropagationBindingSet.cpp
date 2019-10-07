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

#include "CPropagationBindingSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CPropagationBindingSet::CPropagationBindingSet(CProcessContext* processContext) : mProcessContext(processContext),mPropMap(processContext) {
				}

				CPropagationBindingSet* CPropagationBindingSet::initPropagationBindingSet(CPropagationBindingSet* prevSet) {
					if (prevSet) {
						mPropMap.initPropagationBindingMap(&prevSet->mPropMap);
						mConceptDescriptor = prevSet->mConceptDescriptor;
						mSpecialNewPropBindDes = prevSet->mSpecialNewPropBindDes;
						mPropBindDesLinker = prevSet->mPropBindDesLinker;
						mReapplyHash = nullptr;
						if (prevSet->mReapplyHash) {
							getPropagationBindingReapplyConceptHash(true)->initPropagationBindingReapplyConceptHash(prevSet->mReapplyHash);
						}
						mPropVarBindTransExtension = nullptr;
						mPropRepTransExtension = nullptr;
						if (prevSet->mPropVarBindTransExtension) {
							getPropagationVariableBindingTransitionExtension(true)->initPropagationVariableBindingTransitionExtension(prevSet->mPropVarBindTransExtension);
						}
						if (prevSet->mPropRepTransExtension) {
							getPropagationRepresentativeTransitionExtension(true)->initPropagationRepresentativeTransitionExtension(prevSet->mPropRepTransExtension);
						}
						mPropagateAllFlag = prevSet->mPropagateAllFlag;
					} else {
						mPropMap.initPropagationBindingMap(nullptr);
						mConceptDescriptor = nullptr;
						mSpecialNewPropBindDes = nullptr;
						mPropBindDesLinker = nullptr;
						mReapplyHash = nullptr;
						mPropVarBindTransExtension = nullptr;
						mPropRepTransExtension = nullptr;
						mPropagateAllFlag = false;
					}
					return this;
				}

				bool CPropagationBindingSet::containsPropagationBinding(CPropagationBinding* propagationBinding) {
					return mPropMap.contains(propagationBinding->getPropagationID()) && mPropMap.value(propagationBinding->getPropagationID()).hasPropagationBindingDescriptor();
				}

				bool CPropagationBindingSet::containsPropagationBinding(cint64 bindingID) {
					return mPropMap.contains(bindingID) && mPropMap.value(bindingID).hasPropagationBindingDescriptor();
				}

				CPropagationBindingDescriptor* CPropagationBindingSet::getPropagationBindingDescriptor(CPropagationBinding* propagationBinding) {
					return mPropMap.value(propagationBinding->getPropagationID()).getPropagationBindingDescriptor();
				}


				CPropagationBindingMap* CPropagationBindingSet::getPropagationBindingMap() {
					return &mPropMap;
				}

				CPropagationBindingDescriptor* CPropagationBindingSet::getNewSepcialPropagationBindingDescriptor() {
					return mSpecialNewPropBindDes;
				}

				CPropagationBindingSet* CPropagationBindingSet::addPropagationBinding(CPropagationBindingDescriptor* propBindDes, bool newSpecial) {
					CPropagationBindingMapData& data = mPropMap[propBindDes->getPropagationBinding()->getPropagationID()];
					data.setPropagationBindingDescriptor(propBindDes);
					mPropBindDesLinker = propBindDes->append(mPropBindDesLinker);
					if (newSpecial) {
						mSpecialNewPropBindDes = propBindDes;
					}
					return this;
				}


				CPropagationBindingReapplyConceptDescriptor* CPropagationBindingSet::addPropagationBindingReturnReapplyLinker(CPropagationBindingDescriptor* propBindDes, bool newSpecial) {
					CPropagationBindingMapData& data = mPropMap[propBindDes->getPropagationBinding()->getPropagationID()];
					data.setPropagationBindingDescriptor(propBindDes);
					mPropBindDesLinker = propBindDes->append(mPropBindDesLinker);
					if (newSpecial) {
						mSpecialNewPropBindDes = propBindDes;
					}
					return data.getReapplyConceptDescriptor();
				}

				CPropagationBindingSet* CPropagationBindingSet::copyPropagationBindings(CPropagationBindingMap* propBindMap) {
					if (propBindMap) {
						mPropMap = *propBindMap;
					}
					return this;
				}

				CConceptDescriptor* CPropagationBindingSet::getConceptDescriptor() {
					return mConceptDescriptor;
				}

				CPropagationBindingSet* CPropagationBindingSet::setConceptDescriptor(CConceptDescriptor* conDes) {
					mConceptDescriptor = conDes;
					return this;
				}


				CPropagationBindingSet* CPropagationBindingSet::addPropagationBindingDescriptorLinker(CPropagationBindingDescriptor* propBindDesLinker) {
					mPropBindDesLinker = propBindDesLinker->append(mPropBindDesLinker);
					return this;
				}


				CPropagationBindingDescriptor* CPropagationBindingSet::getPropagationBindingDescriptorLinker() {
					return mPropBindDesLinker;
				}


				CPropagationBindingReapplyConceptHash* CPropagationBindingSet::getPropagationBindingReapplyConceptHash(bool create) {
					if (create && !mReapplyHash) {
						mReapplyHash = CObjectParameterizingAllocator< CPropagationBindingReapplyConceptHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					}
					return mReapplyHash;
				}

				CPropagationBindingSet* CPropagationBindingSet::addPropagationBindingReapplyConceptDescriptor(CPropagationBindingReapplyConceptDescriptor* propBindReapplyConDesLinker) {
					getPropagationBindingReapplyConceptHash(true)->addPropagationBindingReapplyConceptDescriptor(propBindReapplyConDesLinker->getReapllyIndividualNode(),propBindReapplyConDesLinker->getConceptDescriptor()->getConcept(),propBindReapplyConDesLinker);
					CPropagationBindingMapData& data = mPropMap[propBindReapplyConDesLinker->getPropagationBinding()->getPropagationID()];
					data.setReapplyConceptDescriptor(propBindReapplyConDesLinker->append(data.getReapplyConceptDescriptor()));
					return this;
				}


				CPropagationVariableBindingTransitionExtension* CPropagationBindingSet::getPropagationVariableBindingTransitionExtension(bool create) {
					if (!mPropVarBindTransExtension && create) {
						mPropVarBindTransExtension = CObjectParameterizingAllocator< CPropagationVariableBindingTransitionExtension,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mPropVarBindTransExtension->initPropagationVariableBindingTransitionExtension(nullptr);
					}
					return mPropVarBindTransExtension;
				}

				CPropagationRepresentativeTransitionExtension* CPropagationBindingSet::getPropagationRepresentativeTransitionExtension(bool create) {
					if (!mPropRepTransExtension && create) {
						mPropRepTransExtension = CObjectParameterizingAllocator< CPropagationRepresentativeTransitionExtension,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mPropRepTransExtension->initPropagationRepresentativeTransitionExtension(nullptr);
					}
					return mPropRepTransExtension;
				}

				bool CPropagationBindingSet::hasPropagateAllFlag() {
					return mPropagateAllFlag;
				}

				bool CPropagationBindingSet::getPropagateAllFlag() {
					return mPropagateAllFlag;
				}

				CPropagationBindingSet* CPropagationBindingSet::setPropagateAllFlag(bool propAllFlag) {
					mPropagateAllFlag = propAllFlag;
					return this;
				}

				bool CPropagationBindingSet::adoptPropagateAllFlag(CPropagationBindingSet* propBindSet) {
					if (propBindSet->mPropagateAllFlag && !mPropagateAllFlag) {
						mPropagateAllFlag = true;
						return true;
					}
					return false;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
