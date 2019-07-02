/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CReapplyConceptSaturationLabelSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptSaturationLabelSet::CReapplyConceptSaturationLabelSet(CProcessContext* processContext) : mProcessContext(processContext),mConceptDesDepMap(processContext) {
				}


				CReapplyConceptSaturationLabelSet* CReapplyConceptSaturationLabelSet::initReapplyConceptSaturationLabelSet() {
					mConceptDesDepMap.clear();
					mConceptSatDesLinker = nullptr;
					mConceptCount = 0;
					mTotelCount = 0;
					mModifiedUpdateLinker = nullptr;
					return this;
				}

				CReapplyConceptSaturationLabelSet* CReapplyConceptSaturationLabelSet::copyReapplyConceptSaturationLabelSet(CReapplyConceptSaturationLabelSet* copyConceptSaturationLabelSet) {
					mConceptCount = copyConceptSaturationLabelSet->mConceptCount;
					mTotelCount = copyConceptSaturationLabelSet->mTotelCount;
					mConceptDesDepMap = copyConceptSaturationLabelSet->mConceptDesDepMap;
					mConceptDesDepMap.detach();
					mConceptSatDesLinker = copyConceptSaturationLabelSet->mConceptSatDesLinker;
					return this;
				}


				bool CReapplyConceptSaturationLabelSet::getConceptDescriptorAndReapplyQueue(CConcept* concept, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes) {
					return getConceptDescriptorAndReapplyQueue(concept->getConceptTag(),conSatDes,impReapplyConSatDes);
				}

				bool CReapplyConceptSaturationLabelSet::getConceptDescriptorAndReapplyQueue(cint64 conTag, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (contained)  {
						conSatDes = conDesDepData->mConSatDes;
						impReapplyConSatDes = conDesDepData->mImpReapplyConSatDes;
					}
					return contained;
				}



				bool CReapplyConceptSaturationLabelSet::containsConceptOrReapllyQueue(cint64 conTag) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (contained)  {
						return conDesDepData->mConSatDes || conDesDepData->mImpReapplyConSatDes;
					}
					return false;
				}

				cint64 CReapplyConceptSaturationLabelSet::getConceptCount() {
					return mConceptCount;
				}


				bool CReapplyConceptSaturationLabelSet::getConceptSaturationDescriptor(CConcept* concept, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes) {
					return getConceptSaturationDescriptor(concept->getConceptTag(),conSatDes,impReapplyConSatDes);
				}


				bool CReapplyConceptSaturationLabelSet::getConceptSaturationDescriptor(cint64 conTag, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (contained)  {
						conSatDes = conDesDepData->mConSatDes;
						if (conSatDes) {
							impReapplyConSatDes = conDesDepData->mImpReapplyConSatDes;
						} else {
							impReapplyConSatDes = nullptr;
						}
						contained &= conSatDes != nullptr;
					}
					return contained;
				}


				bool CReapplyConceptSaturationLabelSet::hasConceptSaturationDescriptor(CConceptSaturationDescriptor* conSatDes) {
					return hasConcept(conSatDes->getConcept(),conSatDes->isNegated());
				}


				bool CReapplyConceptSaturationLabelSet::hasConcept(CConcept* concept, bool negated) {
					cint64 conTag = concept->getConceptTag();
					return hasConcept(conTag,negated);
				}


				bool CReapplyConceptSaturationLabelSet::hasConcept(cint64 conTag, bool negated) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					return isContained && conDesDepData->mConSatDes && conDesDepData->mConSatDes->isNegated() == negated;
				}


				bool CReapplyConceptSaturationLabelSet::hasConcept(CConcept* concept, bool* containsNegated) {
					cint64 conTag = concept->getConceptTag();
					return hasConcept(conTag,containsNegated);
				}

				bool CReapplyConceptSaturationLabelSet::hasConcept(cint64 conTag, bool* containsNegated) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (isContained && containsNegated) {
						*containsNegated = conDesDepData->mConSatDes && conDesDepData->mConSatDes->isNegated();
					}
					return isContained && conDesDepData->mConSatDes;
				}

				bool CReapplyConceptSaturationLabelSet::containsConcept(CConcept* concept, bool* containsNegated) {
					return hasConcept(concept,containsNegated);
				}

				bool CReapplyConceptSaturationLabelSet::containsConcept(CConcept* concept, bool negated) {
					return hasConcept(concept,negated);
				}

				bool CReapplyConceptSaturationLabelSet::containsConceptSaturationDescriptor(CConceptSaturationDescriptor* conSatDes) {
					return hasConceptSaturationDescriptor(conSatDes);
				}


				bool CReapplyConceptSaturationLabelSet::hasModifiedUpdateLinkers() {
					return mModifiedUpdateLinker != nullptr;
				}

				CSaturationModifiedProcessUpdateLinker* CReapplyConceptSaturationLabelSet::getModifiedUpdateLinker() {
					return mModifiedUpdateLinker;
				}

				CReapplyConceptSaturationLabelSet* CReapplyConceptSaturationLabelSet::addModifiedUpdateLinker(CSaturationModifiedProcessUpdateLinker* modUpdateLinker) {
					mModifiedUpdateLinker = modUpdateLinker->append(mModifiedUpdateLinker);
					return this;
				}

				CReapplyConceptSaturationLabelSet* CReapplyConceptSaturationLabelSet::setModifiedUpdateLinker(CSaturationModifiedProcessUpdateLinker* modUpdateLinker) {
					mModifiedUpdateLinker = modUpdateLinker;
					return this;
				}

				bool CReapplyConceptSaturationLabelSet::insertConceptReturnClashed(CConceptSaturationDescriptor* conSatDes, bool* newInsertion, CImplicationReapplyConceptSaturationDescriptor*** impReapplyConSatDesPtr) {
					cint64 conTag = conSatDes->getConceptTag();
					CConceptSaturationDescriptorReapplyData& conDesDepData = mConceptDesDepMap[conTag];
					bool inserted = false;
					if (!conDesDepData.mConSatDes) {
						conDesDepData.mConSatDes = conSatDes;

						inserted = true;
						if (newInsertion) {
							*newInsertion = true;
						}
						mConceptSatDesLinker = conSatDes->append(mConceptSatDesLinker);
						++mConceptCount;
					} else if (conDesDepData.mConSatDes->isNegated() != conSatDes->isNegated()) {
						return true;
					} 

					if (impReapplyConSatDesPtr) {
						*impReapplyConSatDesPtr = &conDesDepData.mImpReapplyConSatDes;
					}
					return false;
				}



				bool CReapplyConceptSaturationLabelSet::insertConceptReapplicationReturnTriggered(cint64 conTag, CImplicationReapplyConceptSaturationDescriptor* reapplyImpReapplyConSatDes, CConceptSaturationDescriptor** conSatDes) {
					CConceptSaturationDescriptorReapplyData& conDesDepData = mConceptDesDepMap[conTag];
					bool triggered = conDesDepData.mConSatDes && !conDesDepData.mConSatDes->isNegated();
					if (triggered) {
						if (conSatDes) {
							*conSatDes = conDesDepData.mConSatDes;
						}
					}
					conDesDepData.mImpReapplyConSatDes = reapplyImpReapplyConSatDes->append(conDesDepData.mImpReapplyConSatDes);
					return triggered;
				}


				CConceptSaturationDescriptor* CReapplyConceptSaturationLabelSet::getConceptSaturationDescriptionLinker() {
					return mConceptSatDesLinker;
				}


				CReapplyConceptSaturationLabelSetIterator CReapplyConceptSaturationLabelSet::getIterator(bool iterateConSatDes, bool iterateReapplies) {
					return CReapplyConceptSaturationLabelSetIterator(mConceptDesDepMap.begin(),mConceptDesDepMap.end(),iterateConSatDes,iterateReapplies);
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
