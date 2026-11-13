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

#include "CReapplyConceptSaturationLabelSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CReapplyConceptSaturationLabelSet::CReapplyConceptSaturationLabelSet(CProcessContext* processContext) : mProcessContext(processContext) {
				}


				CReapplyConceptSaturationLabelSet* CReapplyConceptSaturationLabelSet::initReapplyConceptSaturationLabelSet() {
					mConceptDesDepHash = nullptr;
					mAdditionalConceptDesDepHash = nullptr;
					mConceptSatDesLinker = nullptr;
					mConceptDesDepHash = CObjectParameterizingAllocator< CReapplyConceptSaturationLabelHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					mConceptCount = 0;
					mTotelCount = 0;
					mModifiedUpdateLinker = nullptr;
					mLastNominalIndepConSatDes = nullptr;
					mConceptFlags.reset();
					return this;
				}

				CReapplyConceptSaturationLabelSet* CReapplyConceptSaturationLabelSet::copyReapplyConceptSaturationLabelSet(CReapplyConceptSaturationLabelSet* copyConceptSaturationLabelSet, bool tryFlatLabelCopy) {
					mConceptCount = copyConceptSaturationLabelSet->mConceptCount;
					mTotelCount = copyConceptSaturationLabelSet->mTotelCount;
					mConceptDesDepHash = CObjectParameterizingAllocator< CReapplyConceptSaturationLabelHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					mConceptFlags = copyConceptSaturationLabelSet->mConceptFlags;
					if (copyConceptSaturationLabelSet->mConceptDesDepHash->count() >= ADDITIONALCOPYSIZE || tryFlatLabelCopy && copyConceptSaturationLabelSet->mConceptDesDepHash->count() > 0) {

						CReapplyConceptSaturationLabelHash* tmpAdditionalConDesDepHash = CObjectParameterizingAllocator< CReapplyConceptSaturationLabelHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						if (copyConceptSaturationLabelSet->mAdditionalConceptDesDepHash) {
							if (copyConceptSaturationLabelSet->mAdditionalConceptDesDepHash->count() > copyConceptSaturationLabelSet->mConceptDesDepHash->count()) {
								tmpAdditionalConDesDepHash->copyFrom(*copyConceptSaturationLabelSet->mAdditionalConceptDesDepHash);

								for (CReapplyConceptSaturationLabelHash::const_iterator it = copyConceptSaturationLabelSet->mConceptDesDepHash->constBegin(), itEnd = copyConceptSaturationLabelSet->mConceptDesDepHash->constEnd(); it != itEnd; ++it) {
									cint64 conTag = it.key();
									const CConceptSaturationDescriptorReapplyData& conDesReapplyData(it.value());
									CConceptSaturationDescriptorReapplyData& newConDesReapplyData = (*tmpAdditionalConDesDepHash)[conTag];
									if (conDesReapplyData.mConSatDes) {
										newConDesReapplyData.mConSatDes = conDesReapplyData.mConSatDes;
									}
									if (conDesReapplyData.mImpReapplyConSatDes) {
										newConDesReapplyData.mImpReapplyConSatDes = conDesReapplyData.mImpReapplyConSatDes;
									}
								}
							} else {
								tmpAdditionalConDesDepHash->copyFrom(*copyConceptSaturationLabelSet->mConceptDesDepHash);

								// the entries of the own hash were pulled from the shared hash when first touched and extended since
								// (implications waiting on the trigger are prepended to the reapply list), so they supersede the shared
								// entries; the shared hash only supplies the tags the own hash has not touched (issue #34)
								for (CReapplyConceptSaturationLabelHash::const_iterator it = copyConceptSaturationLabelSet->mAdditionalConceptDesDepHash->constBegin(), itEnd = copyConceptSaturationLabelSet->mAdditionalConceptDesDepHash->constEnd(); it != itEnd; ++it) {
									cint64 conTag = it.key();
									const CConceptSaturationDescriptorReapplyData& conDesReapplyData(it.value());
									CConceptSaturationDescriptorReapplyData& newConDesReapplyData = (*tmpAdditionalConDesDepHash)[conTag];
									if (!newConDesReapplyData.mConSatDes && !newConDesReapplyData.mImpReapplyConSatDes) {
										newConDesReapplyData.mConSatDes = conDesReapplyData.mConSatDes;
										newConDesReapplyData.mImpReapplyConSatDes = conDesReapplyData.mImpReapplyConSatDes;
									}
								}
							}
							copyConceptSaturationLabelSet->mAdditionalConceptDesDepHash = tmpAdditionalConDesDepHash;
						} else {
							copyConceptSaturationLabelSet->mAdditionalConceptDesDepHash = copyConceptSaturationLabelSet->mConceptDesDepHash;
						}
						CReapplyConceptSaturationLabelHash* tmpNewConDesDepHash = CObjectParameterizingAllocator< CReapplyConceptSaturationLabelHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						// the source's new own hash grows from the source's own context: in the parallel saturation (#92) the
						// source's thread fills it later while this thread allocates from its own context
						if (copyConceptSaturationLabelSet->mProcessContext) {
							tmpNewConDesDepHash->setProcessContext(copyConceptSaturationLabelSet->mProcessContext);
						}
						copyConceptSaturationLabelSet->mConceptDesDepHash = tmpNewConDesDepHash;
					}

					mConceptDesDepHash->copyFrom(*copyConceptSaturationLabelSet->mConceptDesDepHash);
					mAdditionalConceptDesDepHash = copyConceptSaturationLabelSet->mAdditionalConceptDesDepHash;
					mConceptSatDesLinker = copyConceptSaturationLabelSet->mConceptSatDesLinker;
					mLastNominalIndepConSatDes = copyConceptSaturationLabelSet->mLastNominalIndepConSatDes;
					return this;
				}


				bool CReapplyConceptSaturationLabelSet::getConceptDescriptorAndReapplyQueue(CConcept* concept, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes) {
					return getConceptDescriptorAndReapplyQueue(concept->getConceptTag(),conSatDes,impReapplyConSatDes);
				}

				bool CReapplyConceptSaturationLabelSet::getConceptDescriptorAndReapplyQueue(cint64 conTag, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					if (!contained && mAdditionalConceptDesDepHash) {
						contained = mAdditionalConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					}
					if (contained)  {
						conSatDes = conDesDepData->mConSatDes;
						impReapplyConSatDes = conDesDepData->mImpReapplyConSatDes;
					}
					return contained;
				}



				bool CReapplyConceptSaturationLabelSet::containsConceptOrReapllyQueue(cint64 conTag) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					if (!contained && mAdditionalConceptDesDepHash) {
						contained = mAdditionalConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					}
					if (contained)  {
						return conDesDepData->mConSatDes || conDesDepData->mImpReapplyConSatDes;
					}
					return false;
				}

				cint64 CReapplyConceptSaturationLabelSet::getConceptCount() {
					return mConceptCount;
				}

				cint64 CReapplyConceptSaturationLabelSet::getTotalCount() {
					return mTotelCount;
				}

				bool CReapplyConceptSaturationLabelSet::getConceptSaturationDescriptor(CConcept* concept, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes) {
					return getConceptSaturationDescriptor(concept->getConceptTag(),conSatDes,impReapplyConSatDes);
				}


				bool CReapplyConceptSaturationLabelSet::getConceptSaturationDescriptor(cint64 conTag, CConceptSaturationDescriptor*& conSatDes, CImplicationReapplyConceptSaturationDescriptor*& impReapplyConSatDes) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool contained = mConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					if (!contained && mAdditionalConceptDesDepHash) {
						contained = mAdditionalConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					}
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
					if (!mConceptFlags.containsConceptFlags(concept, negated)) {
						return false;
					}
					cint64 conTag = concept->getConceptTag();
					return hasConcept(conTag,negated);
				}


				bool CReapplyConceptSaturationLabelSet::hasConcept(cint64 conTag, bool negated) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					if (!isContained && mAdditionalConceptDesDepHash) {
						isContained = mAdditionalConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					}
					return isContained && conDesDepData->mConSatDes && conDesDepData->mConSatDes->isNegated() == negated;
				}


				bool CReapplyConceptSaturationLabelSet::hasConcept(CConcept* concept, bool* containsNegated) {
					if (!mConceptFlags.containsConceptFlags(concept)) {
						return false;
					}
					cint64 conTag = concept->getConceptTag();
					return hasConcept(conTag,containsNegated);
				}

				bool CReapplyConceptSaturationLabelSet::hasConcept(cint64 conTag, bool* containsNegated) {
					CConceptSaturationDescriptorReapplyData* conDesDepData = nullptr;
					bool isContained = mConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					if (!isContained && mAdditionalConceptDesDepHash) {
						isContained = mAdditionalConceptDesDepHash->tryGetValuePointer(conTag,conDesDepData);
					}
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
					CConceptSaturationDescriptorReapplyData& conDesDepData = (*mConceptDesDepHash)[conTag];
					if (!conDesDepData.mConSatDes && !conDesDepData.mImpReapplyConSatDes && mAdditionalConceptDesDepHash) {
						CConceptSaturationDescriptorReapplyData* prevConDesDepData = nullptr;
						bool contained = mAdditionalConceptDesDepHash->tryGetValuePointer(conTag,prevConDesDepData);
						if (contained && prevConDesDepData) {
							if (prevConDesDepData->mConSatDes) {
								conDesDepData.mConSatDes = prevConDesDepData->mConSatDes;
							}
							if (prevConDesDepData->mImpReapplyConSatDes) {
								conDesDepData.mImpReapplyConSatDes = prevConDesDepData->mImpReapplyConSatDes;
							}
						}
					}
					bool inserted = false;
					if (!conDesDepData.mConSatDes) {
						conDesDepData.mConSatDes = conSatDes;

						inserted = true;
						if (newInsertion) {
							*newInsertion = true;
						}
						mConceptSatDesLinker = conSatDes->append(mConceptSatDesLinker);
						mConceptFlags.addConceptFlags(conSatDes->getConcept(), conSatDes->getNegation());
						++mConceptCount;
						++mTotelCount;
					} else if (conDesDepData.mConSatDes->isNegated() != conSatDes->isNegated()) {
						return true;
					} 

					if (impReapplyConSatDesPtr) {
						*impReapplyConSatDesPtr = &conDesDepData.mImpReapplyConSatDes;
					}
					return false;
				}



				bool CReapplyConceptSaturationLabelSet::insertConceptReapplicationReturnTriggered(cint64 conTag, CImplicationReapplyConceptSaturationDescriptor* reapplyImpReapplyConSatDes, CConceptSaturationDescriptor** conSatDes) {
					CConceptSaturationDescriptorReapplyData& conDesDepData = (*mConceptDesDepHash)[conTag];
					if (!conDesDepData.mConSatDes && !conDesDepData.mImpReapplyConSatDes && mAdditionalConceptDesDepHash) {
						CConceptSaturationDescriptorReapplyData* prevConDesDepData = nullptr;
						bool contained = mAdditionalConceptDesDepHash->tryGetValuePointer(conTag,prevConDesDepData);
						if (contained && prevConDesDepData) {
							if (prevConDesDepData->mConSatDes) {
								conDesDepData.mConSatDes = prevConDesDepData->mConSatDes;
							}
							if (prevConDesDepData->mImpReapplyConSatDes) {
								conDesDepData.mImpReapplyConSatDes = prevConDesDepData->mImpReapplyConSatDes;
							}
						}
					}
					bool triggered = conDesDepData.mConSatDes && !conDesDepData.mConSatDes->isNegated();
					if (!conDesDepData.mImpReapplyConSatDes) {
						++mTotelCount;
					}
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
					if (mAdditionalConceptDesDepHash) {
						return CReapplyConceptSaturationLabelSetIterator(mConceptDesDepHash->begin(),mConceptDesDepHash->end(),mAdditionalConceptDesDepHash->begin(),mAdditionalConceptDesDepHash->end(),iterateConSatDes,iterateReapplies);
					} else {
						return CReapplyConceptSaturationLabelSetIterator(mConceptDesDepHash->begin(),mConceptDesDepHash->end(),mConceptDesDepHash->end(),mConceptDesDepHash->end(),iterateConSatDes,iterateReapplies);
					}
				}


				CReapplyConceptSaturationLabelSet* CReapplyConceptSaturationLabelSet::setProcessContext(CProcessContext* processContext) {
					mProcessContext = processContext;
					if (mConceptDesDepHash) {
						mConceptDesDepHash->setProcessContext(processContext);
					}
					return this;
				}

				bool CReapplyConceptSaturationLabelSet::areAllConceptsInAdditionalHash() {
					return mConceptDesDepHash->isEmpty();
				}



				CConceptSetFlags* CReapplyConceptSaturationLabelSet::getConceptFlags() {
					return &mConceptFlags;
				}




				CConceptSaturationDescriptor* CReapplyConceptSaturationLabelSet::getLastNominalIndependentConceptSaturationDescriptorLinker() {
					return mLastNominalIndepConSatDes;
				}

				CReapplyConceptSaturationLabelSet* CReapplyConceptSaturationLabelSet::setLastNominalIndependentConceptSaturationDescriptorLinker(CConceptSaturationDescriptor* conSatDes) {
					mLastNominalIndepConSatDes = conSatDes;
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
