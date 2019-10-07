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
					mAdditionalConceptDesDepMap = nullptr;
				}


				CReapplyConceptLabelSet::~CReapplyConceptLabelSet() {
				}


				CConceptSetFlags* CReapplyConceptLabelSet::getConceptFlags() {
					return &mConceptFlags;
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
						if (!prevConceptLabelSet->mAdditionalConceptDesDepMap && prevConceptLabelSet->mConceptDesDepMap.size() <= 50 || prevConceptLabelSet->mAdditionalConceptDesDepMap && prevConceptLabelSet->mConceptDesDepMap.size()*10 < prevConceptLabelSet->mAdditionalConceptDesDepMap->size()) {
							mConceptDesDepMap = prevConceptLabelSet->mConceptDesDepMap;
							mAdditionalConceptDesDepMap = prevConceptLabelSet->mAdditionalConceptDesDepMap;
						} else {
							mConceptDesDepMap.clear();
							if (prevConceptLabelSet->mAdditionalConceptDesDepMap) {
								mAdditionalConceptDesDepMap = CObjectParameterizingAllocator< CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getMemoryAllocationManager(),mProcessContext);
								*mAdditionalConceptDesDepMap = *prevConceptLabelSet->mAdditionalConceptDesDepMap;
								for (CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator it = prevConceptLabelSet->mConceptDesDepMap.constBegin(), itEnd = prevConceptLabelSet->mConceptDesDepMap.constEnd(); it != itEnd; ++it) {
									cint64 conTag = it.key();
									const CConceptDescriptorDependencyReapplyData& conData = it.value();
									(*mAdditionalConceptDesDepMap)[conTag] = conData;
								}
							} else {
								mAdditionalConceptDesDepMap = &prevConceptLabelSet->mConceptDesDepMap;
							}
						}
						mConceptFlags = prevConceptLabelSet->mConceptFlags;
						mConceptSignature = prevConceptLabelSet->mConceptSignature;
						mCoreConDesLinker = prevConceptLabelSet->mCoreConDesLinker;
						mConceptStructure = prevConceptLabelSet->mConceptStructure;
					} else {
						mConceptDesDepMap.clear();
						mAdditionalConceptDesDepMap = nullptr;
						mConceptDesLinker = nullptr;
						mPrevConceptDesLinker = nullptr;
						mCoreConDesLinker = nullptr;
						mConceptCount = 0;
						mConceptFlags.reset();
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
					if (!mConceptFlags.containsConceptFlags(concept, negated)) {
						return false;
					}
					cint64 conTag = concept->getConceptTag();
					CConceptDescriptorDependencyReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (!conDesDepData && mAdditionalConceptDesDepMap) {						
						isContained = mAdditionalConceptDesDepMap->tryGetValuePointer(conTag,conDesDepData);
					}
					return isContained && conDesDepData->mConceptDescriptor && conDesDepData->mConceptDescriptor->isNegated() == negated;
				}

				bool CReapplyConceptLabelSet::hasConcept(CConcept* concept, bool* containsNegated) {
					if (!mConceptFlags.containsConceptFlags(concept)) {
						return false;
					}
					cint64 conTag = concept->getConceptTag();
					CConceptDescriptorDependencyReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (!conDesDepData && mAdditionalConceptDesDepMap) {						
						isContained = mAdditionalConceptDesDepMap->tryGetValuePointer(conTag,conDesDepData);
					}
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
					if (!conDesDepData && mAdditionalConceptDesDepMap) {						
						contained = mAdditionalConceptDesDepMap->tryGetValuePointer(conTag,conDesDepData);
					}
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
					if (mAdditionalConceptDesDepMap && !conDesDepData.mConceptDescriptor && conDesDepData.mPosNegReapplyQueue.isEmpty()) {
						const CConceptDescriptorDependencyReapplyData& addConDesDepData = mAdditionalConceptDesDepMap->value(conTag);
						conDesDepData = addConDesDepData;
					}
					conDesDepData.mConceptDescriptor = conceptDescriptor;
					++mConceptCount;
					mConceptSignature.addConceptSignature(conceptDescriptor);
					mConceptFlags.addConceptFlags(conceptDescriptor);
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
					bool containFromAdditionMap = false;
					CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
					mConceptDesDepMap.tryInsert(conTag,CConceptDescriptorDependencyReapplyData(conceptDescriptor),&containsAlready,&containedConDesDepData);
					if (!containsAlready && mAdditionalConceptDesDepMap) {
						const CConceptDescriptorDependencyReapplyData& addConDesDepData = mAdditionalConceptDesDepMap->value(conTag);
						if (addConDesDepData.mConceptDescriptor || !addConDesDepData.mPosNegReapplyQueue.isEmpty()) {
							*containedConDesDepData = addConDesDepData;
							containsAlready = true;
							containFromAdditionMap = true;
						}
					}
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
						mConceptFlags.addConceptFlags(conceptDescriptor);
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
					if (!containsAlready && mAdditionalConceptDesDepMap) {
						const CConceptDescriptorDependencyReapplyData& addConDesDepData = mAdditionalConceptDesDepMap->value(conTag);
						if (addConDesDepData.mConceptDescriptor || !addConDesDepData.mPosNegReapplyQueue.isEmpty()) {
							*containedConDesDepData = addConDesDepData;
							containsAlready = true;
						}
					}
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
						mConceptFlags.addConceptFlags(conceptDescriptor);
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
						if (mAdditionalConceptDesDepMap) {
							return CReapplyConceptLabelSetIterator(mConceptCount,nullptr,mConceptDesDepMap.constBegin(),mConceptDesDepMap.constEnd(),mAdditionalConceptDesDepMap->constBegin(),mAdditionalConceptDesDepMap->constEnd(),!getAllStructure);
						} else {
							return CReapplyConceptLabelSetIterator(mConceptCount,nullptr,mConceptDesDepMap.constBegin(),mConceptDesDepMap.constEnd(),CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator(),CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator(),!getAllStructure);
						}
					} else {
						return CReapplyConceptLabelSetIterator(mConceptCount,mConceptDesLinker,CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator(),CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator(),CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator(),CPROCESSMAP<cint64,CConceptDescriptorDependencyReapplyData>::const_iterator());
					}
				}

				bool CReapplyConceptLabelSet::getConceptDescriptorAndReapplyQueue(CConcept* concept, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue) {
					cint64 conTag = concept->getConceptTag();
					return getConceptDescriptorAndReapplyQueue(conTag,conDes,depTrackPoint,reapplyQueue);
				}


				bool CReapplyConceptLabelSet::getConceptDescriptorAndReapplyQueue(cint64 conTag, CConceptDescriptor*& conDes, CDependencyTrackPoint*& depTrackPoint, CCondensedReapplyQueue*& reapplyQueue) {
					CConceptDescriptorDependencyReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepData);
					if (!conDesDepData && mAdditionalConceptDesDepMap) {						
						contained = mAdditionalConceptDesDepMap->tryGetValuePointer(conTag,conDesDepData);
					}
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
					if (!conDesDepData && mAdditionalConceptDesDepMap) {						
						contained = mAdditionalConceptDesDepMap->tryGetValuePointer(conTag,conDesDepData);
					}
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
						if (!containsAlready && mAdditionalConceptDesDepMap) {
							const CConceptDescriptorDependencyReapplyData& addConDesDepData = mAdditionalConceptDesDepMap->value(conTag);
							if (addConDesDepData.mConceptDescriptor || !addConDesDepData.mPosNegReapplyQueue.isEmpty()) {
								*containedConDesDepData = addConDesDepData;
								containsAlready = true;
							}
						}
						conDes = containedConDesDepData->mConceptDescriptor;
						return &containedConDesDepData->mPosNegReapplyQueue;
					} else {
						CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
						if (mConceptDesDepMap.tryGetValuePointer(conTag,containedConDesDepData)) {
							conDes = containedConDesDepData->mConceptDescriptor;
							return &containedConDesDepData->mPosNegReapplyQueue;
						} else if (mAdditionalConceptDesDepMap && mAdditionalConceptDesDepMap->tryGetValuePointer(conTag,containedConDesDepData)) {
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
						if (!containsAlready && mAdditionalConceptDesDepMap) {
							const CConceptDescriptorDependencyReapplyData& addConDesDepData = mAdditionalConceptDesDepMap->value(conTag);
							if (addConDesDepData.mConceptDescriptor || !addConDesDepData.mPosNegReapplyQueue.isEmpty()) {
								*containedConDesDepData = addConDesDepData;
								containsAlready = true;
							}
						}
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
						} else if (mAdditionalConceptDesDepMap && mAdditionalConceptDesDepMap->tryGetValuePointer(conTag,containedConDesDepData)) {
							return &containedConDesDepData->mPosNegReapplyQueue;
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
					} else if (mAdditionalConceptDesDepMap && mAdditionalConceptDesDepMap->tryGetValuePointer(conTag,containedConDesDepData)) {
						return !containedConDesDepData->mPosNegReapplyQueue.isEmpty();
					}
					return false;
				}

				CCondensedReapplyQueueIterator CReapplyConceptLabelSet::getConceptReapplyIterator(CConcept* concept, bool conceptNegation, bool clearDynamicReapplyQueue) {
					cint64 conTag = concept->getConceptTag();
					CConceptDescriptorDependencyReapplyData* containedConDesDepData = nullptr;
					if (mConceptDesDepMap.tryGetValuePointer(conTag,containedConDesDepData)) {
						if (!clearDynamicReapplyQueue) {
							return containedConDesDepData->mPosNegReapplyQueue.getIterator(conceptNegation, false);
						} else {
							CConceptDescriptorDependencyReapplyData& conDesDepData = mConceptDesDepMap[conTag];
							return conDesDepData.mPosNegReapplyQueue.getIterator(conceptNegation, true);
						}
						//if (conceptNegation) {
						//	return containedConDesDepData->mNegReapplyQueue.getIterator(clearDynamicReapplyQueue);
						//} else {
						//	return containedConDesDepData->mPosReapplyQueue.getIterator(clearDynamicReapplyQueue);
						//}
					} else if (mAdditionalConceptDesDepMap && mAdditionalConceptDesDepMap->tryGetValuePointer(conTag,containedConDesDepData)) {
						if (!clearDynamicReapplyQueue) {
							return containedConDesDepData->mPosNegReapplyQueue.getIterator(conceptNegation, false);
						} else {
							if (!containedConDesDepData->mPosNegReapplyQueue.isEmpty()) {
								CConceptDescriptorDependencyReapplyData& conDesDepData = mConceptDesDepMap[conTag];
								conDesDepData = *containedConDesDepData;
								return conDesDepData.mPosNegReapplyQueue.getIterator(conceptNegation, true);
							} else {
								return containedConDesDepData->mPosNegReapplyQueue.getIterator(conceptNegation, false);
							}
						}
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
					CConceptDescriptorDependencyReapplyData& conData = mConceptDesDepMap[dataTag];
					if (mAdditionalConceptDesDepMap && !conData.mConceptDescriptor && conData.mPosNegReapplyQueue.isEmpty()) {
						const CConceptDescriptorDependencyReapplyData& addConDesDepData = mAdditionalConceptDesDepMap->value(dataTag);
						conData = addConDesDepData;
					}
					return &conData;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
