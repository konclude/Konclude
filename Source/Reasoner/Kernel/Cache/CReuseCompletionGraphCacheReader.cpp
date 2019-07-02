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

#include "CReuseCompletionGraphCacheReader.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CReuseCompletionGraphCacheReader::CReuseCompletionGraphCacheReader() : CLinkerBase<CReuseCompletionGraphCacheReader*,CReuseCompletionGraphCacheReader>(this) {
					mCurrentSlot = nullptr;
					mFreeLinker = nullptr;
				}


				CReuseCompletionGraphCacheReader* CReuseCompletionGraphCacheReader::updateSlot(CReuseCompletionGraphCacheSlotItem* updatedSlot) {
					CReuseCompletionGraphCacheSlotItem* prevSlot = mUpdatedSlot.fetchAndStoreOrdered(updatedSlot);
					if (prevSlot != nullptr) {
						prevSlot->decReader();
					}
					return this;
				}


				bool CReuseCompletionGraphCacheReader::hasUpdatedSlotItem() {
                    return mUpdatedSlot.fetchAndAddRelaxed(0) != nullptr;
				}


				bool CReuseCompletionGraphCacheReader::switchToUpdatedSlotItem() {
					CReuseCompletionGraphCacheSlotItem* updatedSlot = mUpdatedSlot.fetchAndStoreOrdered(nullptr);
					if (updatedSlot) {
						CReuseCompletionGraphCacheSlotItem* prevSlot = mCurrentSlot;
						mCurrentSlot = updatedSlot;
						if (prevSlot != nullptr) {
							prevSlot->decReader();
						}
						return true;
					}
					return false;
				}


				CXLinker<TConceptNegPair>* CReuseCompletionGraphCacheReader::getConceptTestLinker() {
					CXLinker<TConceptNegPair>* linker = nullptr;
					if (mFreeLinker) {
						linker = mFreeLinker;
						mFreeLinker = mFreeLinker->getNext();
					} else {
						linker = CObjectAllocator< CXLinker<TConceptNegPair> >::allocateAndConstruct(mContext.getMemoryAllocationManager());
					}
					linker->clearNext();
					return linker;
				}


				CReuseCompletionGraphCacheReader* CReuseCompletionGraphCacheReader::releaseConceptTestLinker(CXLinker<TConceptNegPair>* linker) {
					mFreeLinker = linker->append(mFreeLinker);
					return this;
				}


				bool CReuseCompletionGraphCacheReader::addTestingConcepts(CSortedNegLinker<CConcept*>* conLinker, bool negate, cint64 addingLevel, CCACHINGSET<CConcept*>* testedConceptSet) {
					bool oneAdded = false;
					for (CSortedNegLinker<CConcept*>* conLinkerIt = conLinker; conLinkerIt; conLinkerIt = conLinkerIt->getNext()) {
						CConcept* concept = conLinkerIt->getData();
						bool negated = conLinkerIt->isNegated();
						oneAdded |= addTestingConcept(concept,negated^negate,addingLevel,testedConceptSet);
					}
					return oneAdded;
				}



				bool CReuseCompletionGraphCacheReader::addTestingConcept(CConcept* concept, bool negated, cint64 addingLevel, CCACHINGSET<CConcept*>* testedConceptSet) {
					if (!testedConceptSet->contains(concept)) {
						testedConceptSet->insert(concept);
						CXLinker<TConceptNegPair>* newLinker = getConceptTestLinker();
						newLinker->initLinker(TConceptNegPair(concept,negated));
						if (addingLevel < mTestConceptLinkerLevel) {
							addingLevel = mTestConceptLinkerLevel;
						}
						mTestConceptLinkerVec[addingLevel] = newLinker->append(mTestConceptLinkerVec[addingLevel]);
						++mTestConceptLinkerCount;
						return true;
					}
					return false;
				}


				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheReader::getCacheEntry(CConcept* concept, bool* minimalCompletionGraph, bool* deterministicConnection) {
					CReuseCompletionGraphCacheEntry* entry = nullptr;
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CReuseCompletionGraphCompatibilityEntryHash* entryHash = mCurrentSlot->getEntryHash();
						cint64 entryCount = mCurrentSlot->getEntryCount();

						if (entryCount > 0) {

							if (mEntryVotingVec.size() < entryCount) {
								mEntryVotingVec.resize(entryCount);
							}

							for (cint64 i = 0; i < entryCount; ++i) {
								CCacheEntryVotingItem& item = mEntryVotingVec[i];
								item.reset();
							}

							CCACHINGSET<CConcept*> testedConceptSet(&mContext);

							for (cint64 i = 0; i < mTestConceptLinkerVecSize; ++i) {
								mTestConceptLinkerVec[i] = nullptr;
							}
							mTestConceptLinkerCount = 1;
							mTestConceptLinkerLevel = 0;
							CXLinker<TConceptNegPair>* testConceptLinker = getConceptTestLinker();
							testConceptLinker->initLinker(TConceptNegPair(concept,false));
							mTestConceptLinkerVec[0] = testConceptLinker;
							testedConceptSet.insert(concept);

							while (mTestConceptLinkerCount > 0) {
								CXLinker<TConceptNegPair>* tmpTestConLinker = nullptr;
								for (; mTestConceptLinkerLevel < mTestConceptLinkerVecSize; ++mTestConceptLinkerLevel) {
									tmpTestConLinker = mTestConceptLinkerVec[mTestConceptLinkerLevel];
									if (tmpTestConLinker) {
										mTestConceptLinkerVec[mTestConceptLinkerLevel] = tmpTestConLinker->getNext();
										tmpTestConLinker->clearNext();
										break;
									}
								}
								--mTestConceptLinkerCount;
								CConcept* testCon = tmpTestConLinker->getData().first;
								bool testConNeg = tmpTestConLinker->getData().second;

								bool searchOperandConcepts = true;

								if (!testConNeg && testCon->hasClassName()) {
									// test whether concept is available in cache
									CCacheValue cacheValue(testCon->getConceptTag(),(cint64)testCon,CCacheValue::CACHEVALTAGANDCONCEPT);
									const CReuseCompletionGraphCompatibilityEntryHashData& data = entryHash->value(cacheValue);
									CXLinker<CReuseCompletionGraphCacheEntry*>* entailedEntryLinker = data.getEntailedEntryLinker();
									CXLinker<CReuseCompletionGraphCacheEntry*>* incompatibleEntryLinker = data.getIncompatibleEntryLinker();
									if (entailedEntryLinker || incompatibleEntryLinker) {

										for (CXLinker<CReuseCompletionGraphCacheEntry*>* entailedEntryLinkerIt = entailedEntryLinker; entailedEntryLinkerIt; entailedEntryLinkerIt = entailedEntryLinkerIt->getNext()) {
											CReuseCompletionGraphCacheEntry* entry = entailedEntryLinkerIt->getData();
											CCacheEntryVotingItem& item = mEntryVotingVec[entry->getEntryID()];
											item.mEntry = entry;
											item.mEntailedCount++;
											item.mTmpReferenced = true;
											item.mMinConnectionLevel = qMin(item.mMinConnectionLevel,mTestConceptLinkerLevel);
											item.mMinimalFound |= entry->hasMinimalValue(cacheValue);
										}

										for (CXLinker<CReuseCompletionGraphCacheEntry*>* incompatibleEntryLinkerIt = incompatibleEntryLinker; incompatibleEntryLinkerIt; incompatibleEntryLinkerIt = incompatibleEntryLinkerIt->getNext()) {
											CReuseCompletionGraphCacheEntry* entry = incompatibleEntryLinkerIt->getData();
											CCacheEntryVotingItem& item = mEntryVotingVec[entry->getEntryID()];
											item.mEntry = entry;
											item.mIncompatibleCount++;
											item.mTmpReferenced = true;
										}

										for (cint64 i = 0; i < entryCount; ++i) {
											CCacheEntryVotingItem& item = mEntryVotingVec[i];
											if (!item.mTmpReferenced) {
												item.mMissingCount++;
											} else {
												item.mTmpReferenced = false;
											}
										}

										searchOperandConcepts = false;
									}
								}

								if (searchOperandConcepts) {

									CRole* role = testCon->getRole();
									if (role) {
										addTestingConcepts(role->getDomainConceptList(),false,0,&testedConceptSet);
										addTestingConcepts(role->getRangeConceptList(),false,2,&testedConceptSet);
									}
									cint64 opCode = testCon->getOperatorCode();
									CSortedNegLinker<CConcept*>* opLinker = testCon->getOperandList();

									if (!testConNeg && (opCode == CCIMPLAQAND || opCode == CCBRANCHAQAND || opCode == CCAQAND || opCode == CCEQ || opCode == CCAND || opCode == CCSUB || opCode == CCIMPLTRIG) || testConNeg && (opCode == CCOR)) {
										addTestingConcepts(opLinker,testConNeg,0,&testedConceptSet);
									} else if (!testConNeg && (opCode == CCOR) || testConNeg && (opCode == CCEQ || opCode == CCAND)) {
										addTestingConcepts(opLinker,testConNeg,3,&testedConceptSet);
									} else if (!testConNeg && (opCode == CCSOME || opCode == CCAQSOME) || testConNeg && (opCode == CCALL)) {
										addTestingConcepts(opLinker,testConNeg,1,&testedConceptSet);
									} else if (!testConNeg && (opCode == CCATLEAST) || testConNeg && (opCode == CCATMOST)) {
										cint64 cardinality = testCon->getParameter() + 1*testConNeg;
										if (cardinality >= 1) {
											addTestingConcepts(opLinker,testConNeg,1,&testedConceptSet);
										}
									} else if (opCode == CCAQALL || opCode == CCIMPLALL || opCode == CCBRANCHALL) {
										addTestingConcepts(opLinker,false,2,&testedConceptSet);
									} else if (opCode == CCATMOST || opCode == CCATLEAST) {
										addTestingConcepts(opLinker,false,3,&testedConceptSet);
									} else if (opCode == CCIMPL) {
										addTestingConcept(opLinker->getData(),opLinker->isNegated(),0,&testedConceptSet);
									} else if (opCode == CCAQCHOOCE) {
										for (CSortedNegLinker<CConcept*>* opLinkerIt = opLinker; opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
											if (opLinkerIt->isNegated() == testConNeg) {
												addTestingConcept(opLinkerIt->getData(),false,0,&testedConceptSet);
											}
										}
									}

								}

								releaseConceptTestLinker(tmpTestConLinker);
							}

							cint64 minMissingCount = 0;
							CCacheEntryVotingItem* bestEntryItem = nullptr;

							for (cint64 i = 0; i < entryCount; ++i) {
								CCacheEntryVotingItem& item = mEntryVotingVec[i];	
								if (item.mEntailedCount >= 1 && item.mIncompatibleCount <= 0 && (bestEntryItem == nullptr || minMissingCount < item.mMissingCount)) {
									minMissingCount = item.mMissingCount;
									bestEntryItem = &item;
								}
							}

							if (bestEntryItem) {
								entry = bestEntryItem->mEntry;		
								if (deterministicConnection) {
									*deterministicConnection = bestEntryItem->mMinConnectionLevel <= 1;
								}
								if (minimalCompletionGraph) {
									*minimalCompletionGraph = bestEntryItem->mMinimalFound;
								}
							}
						}

					}
					return entry;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
