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

#include "CReapplyConceptLabelSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptLabelSet::CReapplyConceptLabelSet(CProcessContext* processContext) : mProcessContext(processContext),mConceptDesDepMap(processContext) {
					mConceptDesLinker = nullptr;
					mPrevConceptDesLinker = nullptr;
					mCoreConDesLinker = nullptr;
					mConceptCount = 0;
				}


				CReapplyConceptLabelSet::~CReapplyConceptLabelSet() {
				}


				CConceptSetSignature* CReapplyConceptLabelSet::getConceptSignature() {
					return &mConceptSignature;
				}


				CConceptSetStructure* CReapplyConceptLabelSet::getConceptStructure() {
					return &mConceptStructure;
				}

				cint64 CReapplyConceptLabelSet::getConceptSignatureValue() {
					return mConceptSignature.getSignatureValue();
				}


				CReapplyConceptLabelSet* CReapplyConceptLabelSet::initConceptLabelSet(CReapplyConceptLabelSet* prevConceptLabelSet) {
					if (prevConceptLabelSet) {
						mConceptDesLinker = prevConceptLabelSet->mConceptDesLinker;
						mPrevConceptDesLinker = mConceptDesLinker;
						mConceptCount = prevConceptLabelSet->mConceptCount;
						mConceptDesDepMap = prevConceptLabelSet->mConceptDesDepMap;
						mConceptSignature = prevConceptLabelSet->mConceptSignature;
						mCoreConDesLinker = prevConceptLabelSet->mCoreConDesLinker;
						mConceptStructure = prevConceptLabelSet->mConceptStructure;
					} else {
						mConceptDesDepMap.clear();
						mConceptDesLinker = nullptr;
						mPrevConceptDesLinker = nullptr;
						mCoreConDesLinker = nullptr;
						mConceptCount = 0;
						mConceptSignature.reset();
						mConceptStructure.reset();
					}
					return this;
				}


				CCoreConceptDescriptor* CReapplyConceptLabelSet::getCoreConceptDescriptorLinker() {
					return mCoreConDesLinker;
				}

				CReapplyConceptLabelSet* CReapplyConceptLabelSet::addCoreConceptDescriptor(CCoreConceptDescriptor* coreConDes) {
					mCoreConDesLinker = coreConDes->append(mCoreConDesLinker);
					return this;
				}

				bool CReapplyConceptLabelSet::hasConceptDescriptor(CConceptDescriptor* conceptDescriptor) {
					return hasConcept(conceptDescriptor->getConcept(),conceptDescriptor->getNegation());
				}


				bool CReapplyConceptLabelSet::containsConceptDescriptor(CConceptDescriptor* conceptDescriptor) {
					return hasConceptDescriptor(conceptDescriptor);
				}


				bool CReapplyConceptLabelSet::hasConcept(CConcept* concept, bool negated) {
					cint64 conTag = concept->getConceptTag();
					CConceptDescriptorDependencyReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					return isContained && conDesDepData->mConceptDescriptor && conDesDepData->mConceptDescriptor->isNegated() == negated;
				}

				bool CReapplyConceptLabelSet::hasConcept(CConcept* concept, bool* containsNegated) {
					cint64 conTag = concept->getConceptTag();
					CConceptDescriptorDependencyReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (isContained && containsNegated) {
						*containsNegated = conDesDepData->mConceptDescriptor && conDesDepData->mConceptDescriptor->isNegated();
					}
					return isContained && conDesDepData->mConceptDescriptor;
				}

				bool CReapplyConceptLabelSet::containsConcept(CConcept* concept, bool* containsNegated) {
					return hasConcept(concept,containsNegated);
				}

				bool CReapplyConceptLabelSet::containsConcept(CConcept* concept, bool negated) {
					return hasConcept(concept,negated);
				}



				bool CReapplyConceptLabelSet::getConceptDescriptor(CConcept* concept, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint) {
					return getConceptDescriptor(concept->getConceptTag(),conDes,depTrackPoint);
				}


				bool CReapplyConceptLabelSet::getConceptDescriptor(cint64 conTag, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint) {
					CConceptDescriptorDependencyReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (contained)  {
						conDes = conDesDepData->mConceptDescriptor;
						if (conDes) {
							depTrackPoint = conDes->getDependencyTrackPoint();
						} else {
							depTrackPoint = nullptr;
						}
						contained &= conDes != nullptr;
					}
					return contained;
				}


				bool CReapplyConceptLabelSet::insertConceptIgnoreClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CCondensedReapplyQueueIterator* reapplyQueueIt) {
					cint64 conTag = conceptDescriptor->getConceptTag();
					CConceptDescriptorDependencyReapplyData& conDesDepData = mConceptDesDepMap[conTag];
					conDesDepData.mConceptDescriptor = conceptDescriptor;
					++mConceptCount;
					mConceptSignature.addConceptSignature(conceptDescriptor);
					mConceptStructure.addedConcept(conceptDescriptor);
					mConceptDesLinker = conceptDescriptor->append(mConceptDesLinker);
					if (reapplyQueueIt) {
						*reapplyQueueIt = conDesDepData.mPosNegReapplyQueue.getIterator(conceptDescriptor->isNegated(),true);
					}
					return true;
				}




				bool CReapplyConceptLabelSet::insertConceptGetClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CCondensedReapplyQueueIterator* reapplyQueueIt, CConceptDescriptor** clashedConDes, CDependencyTrackPoint** clashedDepTrackPoint) {
					cint64 conTag = conceptDescriptor->getConceptTag();
					bool containsAlready = false;
					CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
					mConceptDesDepMap.tryInsert(conTag,CConceptDescriptorDependencyReapplyData(conceptDescriptor),&containsAlready,&containedConDesDepData);
					if (containedConDesDepData->mConceptDescriptor == nullptr) {
						containedConDesDepData->mConceptDescriptor = conceptDescriptor;
						containsAlready = false;
					}
					if (containsAlready) {
						CConceptDescriptor* containsConDes = containedConDesDepData->mConceptDescriptor;
						if (containsConDes->isNegated() != conceptDescriptor->isNegated()) {					
							if (clashedConDes) {
								*clashedConDes = containsConDes;
							}
							if (clashedDepTrackPoint) {
								*clashedDepTrackPoint = containsConDes->getDependencyTrackPoint();
							}
						} 
						return true;
					} else {
						++mConceptCount;
						mConceptSignature.addConceptSignature(conceptDescriptor);
						mConceptStructure.addedConcept(conceptDescriptor);
						mConceptDesLinker = conceptDescriptor->append(mConceptDesLinker);
						if (reapplyQueueIt) {
							*reapplyQueueIt = containedConDesDepData->mPosNegReapplyQueue.getIterator(conceptDescriptor->isNegated(),true);
							//if (conceptDescriptor->isNegated()) {
							//	*reapplyQueueIt = containedConDesDepData->mNegReapplyQueue.getIterator(true);
							//} else {
							//	*reapplyQueueIt = containedConDesDepData->mPosReapplyQueue.getIterator(true);
							//}
						}
						return false;
					}
				}


				CClashedConceptDescriptor* CReapplyConceptLabelSet::insertConceptReturnClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, bool* hasContained, CCondensedReapplyQueueIterator* reapplyQueueIt) {
					cint64 conTag = conceptDescriptor->getConceptTag();
					bool containsAlready = false;
					CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
					mConceptDesDepMap.tryInsert(conTag,CConceptDescriptorDependencyReapplyData(conceptDescriptor),&containsAlready,&containedConDesDepData);
					if (containedConDesDepData->mConceptDescriptor == nullptr) {
						containedConDesDepData->mConceptDescriptor = conceptDescriptor;
						containsAlready = false;
					}
					if (containsAlready) {
						if (hasContained) {
							*hasContained = true;
						}
						CConceptDescriptor* containsConDes = containedConDesDepData->mConceptDescriptor;
						if (containsConDes->isNegated() != conceptDescriptor->isNegated()) {
							CDependencyTrackPoint* containsDepTrackPoint = containsConDes->getDependencyTrackPoint();
							// create clash
							CClashedConceptDescriptor* clashDes1 = CObjectAllocator<CClashedConceptDescriptor>::allocateAndConstruct(CContext::getMemoryAllocationManager(mProcessContext));
							clashDes1->initClashedConceptDescriptor(conceptDescriptor,depTrackPoint,0);
							CClashedConceptDescriptor* clashDes2 = CObjectAllocator<CClashedConceptDescriptor>::allocateAndConstruct(CContext::getMemoryAllocationManager(mProcessContext));
							clashDes2->initClashedConceptDescriptor(containsConDes,containsDepTrackPoint,0);
							clashDes1->append(clashDes2);
							return clashDes1;
						}
					} else {
						if (hasContained) {
							*hasContained = false;
						}
						++mConceptCount;
						mConceptSignature.addConceptSignature(conceptDescriptor);
						mConceptStructure.addedConcept(conceptDescriptor);
						mConceptDesLinker = conceptDescriptor->append(mConceptDesLinker);
						if (reapplyQueueIt) {
							*reapplyQueueIt = containedConDesDepData->mPosNegReapplyQueue.getIterator(conceptDescriptor->isNegated(),true);
							//if (conceptDescriptor->isNegated()) {
							//	*reapplyQueueIt = containedConDesDepData->mNegReapplyQueue.getIterator(true);
							//} else {
							//	*reapplyQueueIt = containedConDesDepData->mPosReapplyQueue.getIterator(true);
							//}
						}
					}
					return nullptr;
				}


				bool CReapplyConceptLabelSet::insertConceptThrowClashReturnContained(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CCondensedReapplyQueueIterator* reapplyQueueIt) {
					bool contained = false;
					CClashedConceptDescriptor* clash = insertConceptReturnClash(conceptDescriptor,depTrackPoint,&contained,reapplyQueueIt);
					if (clash) {
						throw clash;
					}
					return contained;
				}



				cint64 CReapplyConceptLabelSet::getConceptCount() {
					return mConceptCount;
				}



				CReapplyConceptLabelSetIterator CReapplyConceptLabelSet::getConceptLabelSetIterator(bool getSorted, bool getDependencies, bool getAllStructure) {
					if (getSorted || getDependencies || getAllStructure) {
						return CReapplyConceptLabelSetIterator(mConceptCount,nullptr,mConceptDesDepMap.constBegin(),mConceptDesDepMap.constEnd(),!getAllStructure);
					} else {
						return CReapplyConceptLabelSetIterator(mConceptCount,mConceptDesLinker,CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator(),CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator());
					}
				}

				bool CReapplyConceptLabelSet::getConceptDescriptorAndReapplyQueue(CConcept* concept, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue) {
					cint64 conTag = concept->getConceptTag();
					return getConceptDescriptorAndReapplyQueue(conTag,conDes,depTrackPoint,reapplyQueue);
				}


				bool CReapplyConceptLabelSet::getConceptDescriptorAndReapplyQueue(cint64 conTag, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue) {
					CConceptDescriptorDependencyReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (contained)  {
						conDes = conDesDepData->mConceptDescriptor;
						reapplyQueue = &conDesDepData->mPosNegReapplyQueue;
						if (conDes) {
							depTrackPoint = conDes->getDependencyTrackPoint();
						} else {
							depTrackPoint = nullptr;
						}
						contained = conDes != nullptr;
					}
					return contained;
				}


				bool CReapplyConceptLabelSet::getConceptDescriptorOrReapplyQueue(CConcept* concept, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue) {
					cint64 conTag = concept->getConceptTag();
					return getConceptDescriptorOrReapplyQueue(conTag,conDes,depTrackPoint,reapplyQueue);
				}


				bool CReapplyConceptLabelSet::getConceptDescriptorOrReapplyQueue(cint64 conTag, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue) {
					CConceptDescriptorDependencyReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (contained)  {
						conDes = conDesDepData->mConceptDescriptor;
						reapplyQueue = &conDesDepData->mPosNegReapplyQueue;
						if (conDes) {
							depTrackPoint = conDes->getDependencyTrackPoint();
						} else {
							depTrackPoint = nullptr;
						}
						contained = conDes != nullptr || reapplyQueue != nullptr;
					}
					return contained;
				}


				CCondensedReapplyQueue* CReapplyConceptLabelSet::getConceptDescriptorAndReapplyQueue(CConcept*& concept, CConceptDescriptor*& conDes, bool create) {
					cint64 conTag = concept->getConceptTag();
					if (create) {
						bool containsAlready = false;
						CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
						mConceptDesDepMap.tryInsert(conTag,CConceptDescriptorDependencyReapplyData(nullptr),&containsAlready,&containedConDesDepData);
						conDes = containedConDesDepData->mConceptDescriptor;
						return &containedConDesDepData->mPosNegReapplyQueue;
					} else {
						CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
						if (mConceptDesDepMap.tryGetValuePointer(conTag,containedConDesDepData)) {
							conDes = containedConDesDepData->mConceptDescriptor;
							return &containedConDesDepData->mPosNegReapplyQueue;
						}
					}
					return nullptr;
				}



				CCondensedReapplyQueue* CReapplyConceptLabelSet::getConceptReapplyQueue(cint64 conTag, bool create) {
					if (create) {
						bool containsAlready = false;
						CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
						mConceptDesDepMap.tryInsert(conTag,CConceptDescriptorDependencyReapplyData(nullptr),&containsAlready,&containedConDesDepData);
						return &containedConDesDepData->mPosNegReapplyQueue;
						//if (conceptNegation) {
						//	return &containedConDesDepData->mNegReapplyQueue;
						//} else {
						//	return &containedConDesDepData->mPosReapplyQueue;
						//}
					} else {
						CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
						if (mConceptDesDepMap.tryGetValuePointer(conTag,containedConDesDepData)) {
							return &containedConDesDepData->mPosNegReapplyQueue;
							//if (conceptNegation) {
							//	return &containedConDesDepData->mNegReapplyQueue;
							//} else {
							//	return &containedConDesDepData->mPosReapplyQueue;
							//}
						}
					}
					return nullptr;
				}



				CCondensedReapplyQueue* CReapplyConceptLabelSet::getConceptReapplyQueue(CConcept *&concept, bool &conceptNegation, bool create) {
					cint64 conTag = concept->getConceptTag();
					return getConceptReapplyQueue(conTag,create);
				}


				bool CReapplyConceptLabelSet::containsConceptReapplyQueue(CConcept *&concept, bool &conceptNegation) {
					cint64 conTag = concept->getConceptTag();
					CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
					if (mConceptDesDepMap.tryGetValuePointer(conTag,containedConDesDepData)) {
						return !containedConDesDepData->mPosNegReapplyQueue.isEmpty();
						//if (conceptNegation) {
						//	return !containedConDesDepData->mNegReapplyQueue.isEmpty();
						//} else {
						//	return !containedConDesDepData->mPosReapplyQueue.isEmpty();
						//}
					}
					return false;
				}

				CCondensedReapplyQueueIterator CReapplyConceptLabelSet::getConceptReapplyIterator(CConcept* concept, bool conceptNegation, bool clearDynamicReapplyQueue) {
					cint64 conTag = concept->getConceptTag();
					CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
					if (mConceptDesDepMap.tryGetValuePointer(conTag,containedConDesDepData)) {
						return containedConDesDepData->mPosNegReapplyQueue.getIterator(conceptNegation,clearDynamicReapplyQueue);
						//if (conceptNegation) {
						//	return containedConDesDepData->mNegReapplyQueue.getIterator(clearDynamicReapplyQueue);
						//} else {
						//	return containedConDesDepData->mPosReapplyQueue.getIterator(clearDynamicReapplyQueue);
						//}
					}
					return CCondensedReapplyQueueIterator(nullptr,conceptNegation);
				}

				CCondensedReapplyQueueIterator CReapplyConceptLabelSet::getConceptReapplyIterator(CConceptDescriptor* conDes, bool clearDynamicReapplyQueue) {
					return getConceptReapplyIterator(conDes->getConcept(),conDes->isNegated(),clearDynamicReapplyQueue);
				}


				CConceptDescriptor* CReapplyConceptLabelSet::getAddingSortedConceptDescriptionLinker() {
					return mConceptDesLinker;
				}


				CConceptDescriptorDependencyReapplyData* CReapplyConceptLabelSet::getConceptDescriptorDependencyReapplyData(cint64 dataTag) {
					return &mConceptDesDepMap[dataTag];
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
