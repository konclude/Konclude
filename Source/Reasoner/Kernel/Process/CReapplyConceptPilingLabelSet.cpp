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

#include "CReapplyConceptPilingLabelSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptPilingLabelSet::CReapplyConceptPilingLabelSet(CProcessContext* processContext) : mProcessContext(processContext),mConceptDesDepMap(processContext) {
				}


				CReapplyConceptPilingLabelSet* CReapplyConceptPilingLabelSet::initReapplyConceptPilingLabelSet() {
					mConceptDesDepMap.clear();
					mConceptPilDesLinker = nullptr;
					mConceptCount = 0;
					mTotelCount = 0;
					return this;
				}

				CReapplyConceptPilingLabelSet* CReapplyConceptPilingLabelSet::copyReapplyConceptPilingLabelSet(CReapplyConceptPilingLabelSet* copyConceptPilingLabelSet, cint64 maxCopyPilingDepth) {
					mConceptCount = 0;
					mTotelCount = 0;
					cint64 maxCopyPilDepth = maxCopyPilingDepth;
					CMemoryAllocationManager* memMan = mProcessContext->getUsedMemoryAllocationManager();
					for (CPROCESSHASH<cint64,CConceptPilingDescriptorReapplyData>::const_iterator it = copyConceptPilingLabelSet->mConceptDesDepMap.constBegin(), itEnd = copyConceptPilingLabelSet->mConceptDesDepMap.constEnd(); it != itEnd; ++it) {
						const CConceptPilingDescriptorReapplyData& conPilDesReapplyData = it.value();
						bool conceptAvailable = false;
#ifdef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
						if (0 <= maxCopyPilDepth) {
#else
						if (conPilDesReapplyData.mPilingDepth <= maxCopyPilDepth) {
#endif
							if (conPilDesReapplyData.mConPilDes) {
								++mConceptCount;
								conceptAvailable = true;
							} else {
								++mTotelCount;
							}
						}
						CImplicationReapplyConceptPilingDescriptor* impReapplyConPilDesIt = conPilDesReapplyData.mImpReapplyConPilDes;
						while (impReapplyConPilDesIt) {
							CImplicationReapplyConceptPilingDescriptor* copyImpReapplyConPilDes = nullptr;
							if (!conceptAvailable) {
								if (impReapplyConPilDesIt->getPilingDepth() <= maxCopyPilDepth && impReapplyConPilDesIt->getPreviousMaxPilingDepth() <= maxCopyPilDepth) {
									copyImpReapplyConPilDes = CObjectAllocator<CImplicationReapplyConceptPilingDescriptor>::allocateAndConstruct(memMan);
									copyImpReapplyConPilDes->copyImplicationReapllyConceptPilingDescriptor(impReapplyConPilDesIt);
								}
							}
							impReapplyConPilDesIt->setCopyReapplyConceptPilingDescriptor(copyImpReapplyConPilDes);
							impReapplyConPilDesIt = impReapplyConPilDesIt->getNext();
						}
					}
					mConceptDesDepMap = copyConceptPilingLabelSet->mConceptDesDepMap;
					//mConceptDesDepMap.detachRestrictedCopy(&maxCopyPilDepth);
					mConceptDesDepMap.detach();
					mConceptPilDesLinker = copyConceptPilingLabelSet->mConceptPilDesLinker;
					return this;
				}


				bool CReapplyConceptPilingLabelSet::getConceptDescriptorAndReapplyQueue(CConcept* concept, CConceptPilingDescriptor*& conPilDes, CImplicationReapplyConceptPilingDescriptor*& impReapplyConPilDes, cint64& pilingDepth) {
					return getConceptDescriptorAndReapplyQueue(concept->getConceptTag(),conPilDes,impReapplyConPilDes,pilingDepth);
				}

				bool CReapplyConceptPilingLabelSet::getConceptDescriptorAndReapplyQueue(cint64 conTag, CConceptPilingDescriptor*& conPilDes, CImplicationReapplyConceptPilingDescriptor*& impReapplyConPilDes, cint64& pilingDepth) {
					CConceptPilingDescriptorReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (contained)  {
						conPilDes = conDesDepData->mConPilDes;
#ifdef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
						pilingDepth = 0;
#else
						pilingDepth = conDesDepData->mPilingDepth;
#endif
						impReapplyConPilDes = conDesDepData->mImpReapplyConPilDes;
					}
					return contained;
				}


				bool CReapplyConceptPilingLabelSet::getConceptPilingDescriptor(CConcept* concept, CConceptPilingDescriptor*& conPilDes, CImplicationReapplyConceptPilingDescriptor*& impReapplyConPilDes, cint64& pilingDepth) {
					return getConceptPilingDescriptor(concept->getConceptTag(),conPilDes,impReapplyConPilDes,pilingDepth);
				}


				bool CReapplyConceptPilingLabelSet::getConceptPilingDescriptor(cint64 conTag, CConceptPilingDescriptor*& conPilDes, CImplicationReapplyConceptPilingDescriptor*& impReapplyConPilDes, cint64& pilingDepth) {
					CConceptPilingDescriptorReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (contained)  {
						conPilDes = conDesDepData->mConPilDes;
						if (conPilDes) {
#ifdef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
							pilingDepth = 0;
#else
							pilingDepth = conDesDepData->mPilingDepth;
#endif
							impReapplyConPilDes = conDesDepData->mImpReapplyConPilDes;
						} else {
							pilingDepth = nullptr;
							impReapplyConPilDes = nullptr;
						}
						contained &= conPilDes != nullptr;
					}
					return contained;
				}


				bool CReapplyConceptPilingLabelSet::hasConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes) {
					return hasConcept(conPilDes->getConcept(),conPilDes->isNegated());
				}


				bool CReapplyConceptPilingLabelSet::hasConcept(CConcept* concept, bool negated) {
					cint64 conTag = concept->getConceptTag();
					return hasConcept(conTag,negated);
				}

				bool CReapplyConceptPilingLabelSet::hasConcept(cint64 conTag, bool negated) {
					CConceptPilingDescriptorReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					return isContained && conDesDepData->mConPilDes && conDesDepData->mConPilDes->isNegated() == negated;
				}


				bool CReapplyConceptPilingLabelSet::hasConcept(cint64 maxPilingDepth, CConcept* concept, bool negated) {
					cint64 conTag = concept->getConceptTag();
					return hasConcept(maxPilingDepth,conTag,negated);
				}


				bool CReapplyConceptPilingLabelSet::hasConcept(cint64 maxPilingDepth, cint64 conTag, bool negated) {
					CConceptPilingDescriptorReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
#ifdef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					return isContained && conDesDepData->mConPilDes && conDesDepData->mConPilDes->isNegated() == negated;
#else
					return isContained && conDesDepData->mConPilDes && conDesDepData->mConPilDes->isNegated() == negated && conDesDepData->mPilingDepth <= maxPilingDepth;
#endif
				}


				bool CReapplyConceptPilingLabelSet::hasConcept(CConcept* concept, bool* containsNegated) {
					cint64 conTag = concept->getConceptTag();
					return hasConcept(conTag,containsNegated);
				}

				bool CReapplyConceptPilingLabelSet::hasConcept(cint64 conTag, bool* containsNegated) {
					CConceptPilingDescriptorReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (isContained && containsNegated) {
						*containsNegated = conDesDepData->mConPilDes && conDesDepData->mConPilDes->isNegated();
					}
					return isContained && conDesDepData->mConPilDes;
				}

				bool CReapplyConceptPilingLabelSet::containsConcept(CConcept* concept, bool* containsNegated) {
					return hasConcept(concept,containsNegated);
				}

				bool CReapplyConceptPilingLabelSet::containsConcept(CConcept* concept, bool negated) {
					return hasConcept(concept,negated);
				}

				bool CReapplyConceptPilingLabelSet::containsConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes) {
					return hasConceptPilingDescriptor(conPilDes);
				}


				bool CReapplyConceptPilingLabelSet::insertConceptReturnClashed(CConceptPilingDescriptor* conPilDes, cint64 pilingDepth, bool* newInsertion, CImplicationReapplyConceptPilingDescriptor** reapplyImpReapplyConPilDes, bool* insertionUpdate, cint64* prevPilingDepth, CImplicationReapplyConceptPilingDescriptor*** impReapplyConPilDesPtr) {
					cint64 conTag = conPilDes->getConceptTag();
					CConceptPilingDescriptorReapplyData& conDesDepData = mConceptDesDepMap[conTag];
					bool inserted = false;
					if (!conDesDepData.mConPilDes) {
						conDesDepData.mConPilDes = conPilDes;
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
						conDesDepData.mPilingDepth = pilingDepth;
#endif
						inserted = true;
						if (newInsertion) {
							*newInsertion = true;
						}
						mConceptPilDesLinker = conPilDes->append(mConceptPilDesLinker);
						++mConceptCount;
					} else if (conDesDepData.mConPilDes->isNegated() != conPilDes->isNegated()) {
						return true;
					} 
#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
					 else if (conDesDepData.mPilingDepth > pilingDepth) {
						if (prevPilingDepth) {
							*prevPilingDepth = conDesDepData.mPilingDepth;
						}
						conDesDepData.mPilingDepth = pilingDepth;
						if (newInsertion) {
							*newInsertion = false;
						}
						inserted = true;
					}
#endif
					if (inserted) {
						if (insertionUpdate) {
							*insertionUpdate = true;
						}
						if (reapplyImpReapplyConPilDes) {
							*reapplyImpReapplyConPilDes = conDesDepData.mImpReapplyConPilDes;
						}
					} else {
						if (insertionUpdate) {
							*insertionUpdate = false;
						}
					}
					if (impReapplyConPilDesPtr) {
						*impReapplyConPilDesPtr = &conDesDepData.mImpReapplyConPilDes;
					}
					return false;
				}



				bool CReapplyConceptPilingLabelSet::insertConceptReapplicationReturnTriggered(cint64 conTag, CImplicationReapplyConceptPilingDescriptor* reapplyImpReapplyConPilDes, CConceptPilingDescriptor** conPilDes, cint64* pilingDepth) {
					CConceptPilingDescriptorReapplyData& conDesDepData = mConceptDesDepMap[conTag];
					bool triggered = conDesDepData.mConPilDes && !conDesDepData.mConPilDes->isNegated();
					if (triggered) {
						if (conPilDes) {
							*conPilDes = conDesDepData.mConPilDes;
						}
						if (pilingDepth) {
#ifdef KONCLUDE_REASONER_KERNEL_PROCESS_SATURATION_DEACTIVATED_PILING
							*pilingDepth = 0;
#else
							*pilingDepth = conDesDepData.mPilingDepth;
#endif
						}
					}
					conDesDepData.mImpReapplyConPilDes = reapplyImpReapplyConPilDes->append(conDesDepData.mImpReapplyConPilDes);
					return triggered;
				}


				CConceptPilingDescriptor* CReapplyConceptPilingLabelSet::getConceptPilingDescriptionLinker() {
					return mConceptPilDesLinker;
				}


				CReapplyConceptPilingLabelSetIterator CReapplyConceptPilingLabelSet::getIterator(bool iterateConPilDes, bool iterateReapplies, cint64 maxPilingDepthIterate) {
					return CReapplyConceptPilingLabelSetIterator(mConceptDesDepMap.begin(),mConceptDesDepMap.end(),iterateConPilDes,iterateReapplies,maxPilingDepthIterate);
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
