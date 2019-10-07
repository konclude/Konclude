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

#include "CConceptLabelSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CConceptLabelSet::CConceptLabelSet(CProcessContext* processContext) : mProcessContext(processContext),mConceptDesDepMap(processContext) {
					mConceptDesLinker = nullptr;
					mPrevConceptDesLinker = nullptr;
					mConceptCount = 0;
				}


				CConceptLabelSet::~CConceptLabelSet() {
				}


				CConceptSetSignature* CConceptLabelSet::getConceptSignature() {
					return &mConceptSignature;
				}


				cint64 CConceptLabelSet::getConceptSignatureValue() {
					return mConceptSignature.getSignatureValue();
				}


				CConceptLabelSet* CConceptLabelSet::initConceptLabelSet(CConceptLabelSet* prevConceptLabelSet) {
					if (prevConceptLabelSet) {
						mConceptDesLinker = prevConceptLabelSet->mConceptDesLinker;
						mPrevConceptDesLinker = mConceptDesLinker;
						mConceptCount = prevConceptLabelSet->mConceptCount;
						mConceptDesDepMap = prevConceptLabelSet->mConceptDesDepMap;
					} else {
						mConceptDesDepMap.clear();
						mConceptDesLinker = nullptr;
						mPrevConceptDesLinker = nullptr;
						mConceptCount = 0;
					}
					return this;
				}



				bool CConceptLabelSet::hasConceptDescriptor(CConceptDescriptor* conceptDescriptor) {
					return hasConcept(conceptDescriptor->getConcept(),conceptDescriptor->getNegation());
				}


				bool CConceptLabelSet::containsConceptDescriptor(CConceptDescriptor* conceptDescriptor) {
					return hasConceptDescriptor(conceptDescriptor);
				}


				bool CConceptLabelSet::hasConcept(CConcept* concept, bool negated) {
					cint64 conTag = concept->getConceptTag();
					CConceptDescriptorDependencyPair* conDesDepPair = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepPair);
					return isContained && conDesDepPair->first->isNegated() == negated;
				}

				bool CConceptLabelSet::hasConcept(CConcept* concept, bool* containsNegated) {
					cint64 conTag = concept->getConceptTag();
					CConceptDescriptorDependencyPair* conDesDepPair = nullptr;
					bool isContained = mConceptDesDepMap.tryGetValuePointer(conTag,conDesDepPair);
					if (isContained && containsNegated) {
						*containsNegated = conDesDepPair->first->isNegated();
					}
					return isContained;
				}

				bool CConceptLabelSet::containsConcept(CConcept* concept, bool* containsNegated) {
					return hasConcept(concept,containsNegated);
				}

				bool CConceptLabelSet::containsConcept(CConcept* concept, bool negated) {
					return hasConcept(concept,negated);
				}


				CClashedConceptDescriptor* CConceptLabelSet::insertConceptReturnClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint) {
					cint64 conTag = conceptDescriptor->getConceptTag();
					bool alreadyContained = false;
					CPROCESSMAP<cint64,CConceptDescriptorDependencyPair>::iterator it = mConceptDesDepMap.tryInsert(conTag,CConceptDescriptorDependencyPair(conceptDescriptor,depTrackPoint),&alreadyContained);
					if (alreadyContained) {
						cint64 containConceptTag(it.key());
						CConceptDescriptorDependencyPair containsDesDepPair(it.value());
						CConceptDescriptor* containsConDes = containsDesDepPair.first;
						if (containsConDes->isNegated() != conceptDescriptor->isNegated()) {
							CDependencyTrackPoint* containsDepTrackPoint = containsDesDepPair.second;
							// create clash
							CClashedConceptDescriptor* clashDes1 = CObjectAllocator<CClashedConceptDescriptor>::allocateAndConstruct(CContext::getMemoryAllocationManager(mProcessContext));
							clashDes1->initClashedConceptDescriptor(conceptDescriptor,depTrackPoint,0);
							CClashedConceptDescriptor* clashDes2 = CObjectAllocator<CClashedConceptDescriptor>::allocateAndConstruct(CContext::getMemoryAllocationManager(mProcessContext));
							clashDes2->initClashedConceptDescriptor(containsConDes,containsDesDepPair.second,0);
							clashDes1->append(clashDes2);
							return clashDes1;
						}
					} else {
						++mConceptCount;
						mConceptSignature.addConceptSignature(conceptDescriptor);
						mConceptDesLinker = conceptDescriptor->append(mConceptDesLinker);
					}
					return nullptr;
				}


				CConceptLabelSet* CConceptLabelSet::insertConceptThrowClash(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint) {
					CClashedConceptDescriptor* clash = insertConceptReturnClash(conceptDescriptor,depTrackPoint);
					if (clash) {
						throw clash;
					}
					return this;
				}


				CConceptLabelSet* CConceptLabelSet::updateConceptDependencyTrackPoint(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* updateDepTrackPoint) {
					cint64 conTag = conceptDescriptor->getConceptTag();
					mConceptDesDepMap.insert(conTag,CConceptDescriptorDependencyPair(conceptDescriptor,updateDepTrackPoint));
					return this;
				}



				cint64 CConceptLabelSet::getConceptCount() {
					return mConceptCount;
				}

				CConceptLabelSetIterator CConceptLabelSet::getConceptLabelSetIterator(bool getSorted, bool getDependencies) {
					if (getSorted || getDependencies) {
						return CConceptLabelSetIterator(mConceptCount,nullptr,mConceptDesDepMap.begin(),mConceptDesDepMap.end());
					} else {
						return CConceptLabelSetIterator(mConceptCount,mConceptDesLinker,mConceptDesDepMap.end(),mConceptDesDepMap.end());
					}
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
