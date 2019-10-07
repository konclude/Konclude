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

#include "COptimizedComplexVariableIndividualMappingsMultiHashPart.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualMappingsMultiHashPart::COptimizedComplexVariableIndividualMappingsMultiHashPart(cint64 bindingSize) : CMemoryPoolNewAllocationIncreasingContext(500, 500000, 2), mMappingCardinalityHash(this) {
				mBindingCount = 0;
				mLastUpdatedBindingCount = 0;
				mFirstUpdateCardinalityLinker = nullptr;
				mLastUpdateCardinalityLinker = nullptr;
				mFirstAddedBindingsCardinalityLinker = nullptr;
				mLastAddedBindingsCardinalityLinker = nullptr;
				mCurrentUpdateId = 0;
				mBindingSize = bindingSize;

				mFreeCardCopy = nullptr;
				mFreeBindCardLinkerCopy = nullptr;
				mFreeBindingsCopy = nullptr;

				mCurrentLinkerBatchingSize = 100;
				mLinkerBatchingSizeIncreasingFactor = 1.25;

				mLastAddedBindingsCardinalityBatchLinker = nullptr;
				mFirstAddedBindingsCardinalityBatchLinker = nullptr;

				mLastAddedBindingsCardinalityBatchLinkerUpdateId = 0;
				mMaximumCardinalitySameIndividualsJointlyConsidered = 0;
				mMaximumCardinalitySameIndividualsSeparatelyConsidered = 0;
			}


			COptimizedComplexVariableIndividualMappingsMultiHashPart::~COptimizedComplexVariableIndividualMappingsMultiHashPart() {
			}



			cint64 COptimizedComplexVariableIndividualMappingsMultiHashPart::getMaximumCardinalitySameIndividualsJointlyConsidered() {
				return mMaximumCardinalitySameIndividualsJointlyConsidered;
			}

			cint64 COptimizedComplexVariableIndividualMappingsMultiHashPart::getMaximumCardinalitySameIndividualsSeparatelyConsidered() {
				return mMaximumCardinalitySameIndividualsSeparatelyConsidered;
			}


			cint64 COptimizedComplexVariableIndividualMappingsMultiHashPart::getLastUpdatedBindingCount(bool update) {
				cint64 count = mLastUpdatedBindingCount;
				if (update) {
					mLastUpdatedBindingCount = mBindingCount;
				}
				return count;
			}


			cint64 COptimizedComplexVariableIndividualMappingsMultiHashPart::getBindingCount() {
				return mBindingCount;
			}

			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualMappingsMultiHashPart::getLastAddedBindingsCardinalityLinker() {
				return mLastAddedBindingsCardinalityLinker;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::addLastAddedBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker) {
				extendAddedBindingsCardinalityBatchLinker(linker);
				if (!mFirstAddedBindingsCardinalityLinker) {
					mFirstAddedBindingsCardinalityLinker = linker;
				}
				mLastAddedBindingsCardinalityLinker = linker->append(mLastAddedBindingsCardinalityLinker);
				return this;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::addLastUpdatedCardinalityLinker(COptimizedComplexVariableIndividualUpdateCardinalityLinker* linker) {
				if (!mFirstUpdateCardinalityLinker) {
					mFirstUpdateCardinalityLinker = linker;
				}
				mLastUpdateCardinalityLinker = linker->append(mLastUpdateCardinalityLinker);
				return this;
			}
			


			COptimizedComplexVariableIndividualUpdateCardinalityLinker* COptimizedComplexVariableIndividualMappingsMultiHashPart::getLastUpdatedCardinalityLinker() {
				return mLastUpdateCardinalityLinker;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::setCurrentUpdateId(cint64 currUpdateId) {
				mCurrentUpdateId = currUpdateId;
				return this;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::incCurrentUpdateId(cint64 incUpdateCount) {
				mCurrentUpdateId += incUpdateCount;
				return this;
			}

			cint64 COptimizedComplexVariableIndividualMappingsMultiHashPart::getCurrentUpdateId() {
				return mCurrentUpdateId;
			}



			bool COptimizedComplexVariableIndividualMappingsMultiHashPart::addInsertingBindingsCardinalityLinkerCopy(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, bool addCardinalitiesIfAlreadyPresent) {
				COptimizedComplexVariableIndividualBindingsHasher hasher(linker);
				return addInsertingBindingsCardinalityLinkerCopy(linker, hasher.getHashValue(), addCardinalitiesIfAlreadyPresent);
			}



			bool COptimizedComplexVariableIndividualMappingsMultiHashPart::addInsertingBindingsCardinalityLinkerCopy(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, cint64 hashValue, bool addCardinalitiesIfAlreadyPresent) {
				++mStatInsertionCount;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linkerCopy = getBindingsCardinalityLinkerCopy(linker);
				COptimizedComplexVariableIndividualBindingsHasher hasher(linkerCopy, hashValue);
				COptimizedComplexVariableIndividualBindingsCardinalityLinker*& hashedLinker = mMappingCardinalityHash[hasher];
				if (!hashedLinker) {
					hashedLinker = linkerCopy;
					if (linkerCopy->getCurrentCardinalities()) {
						linkerCopy->getCurrentCardinalities()->setCardinalityUpdateId(mCurrentUpdateId);
						mMaximumCardinalitySameIndividualsJointlyConsidered = qMax(mMaximumCardinalitySameIndividualsJointlyConsidered, linkerCopy->getCurrentCardinalities()->getSameIndividualsJointlyConsideredCardinality());
						mMaximumCardinalitySameIndividualsSeparatelyConsidered = qMax(mMaximumCardinalitySameIndividualsSeparatelyConsidered, linkerCopy->getCurrentCardinalities()->getSameIndividualsSeparatlyConsideredCardinality());
					}
					addLastAddedBindingsCardinalityLinker(linkerCopy);
					mBindingCount = mMappingCardinalityHash.size();
					return true;
				} else if (addCardinalitiesIfAlreadyPresent) {
					releaseBindingsCardinalityLinkerCopy(linkerCopy);
					COptimizedComplexVariableIndividualBindingsCardinality* cardinalities = linker->getCurrentCardinalities();
					if (cardinalities) {
						if (hashedLinker->getCurrentCardinalities() && hashedLinker->getCurrentCardinalities()->getCardinalityUpdateId() == mCurrentUpdateId) {
							hashedLinker->getCurrentCardinalities()->addCardinalities(cardinalities);
							mMaximumCardinalitySameIndividualsJointlyConsidered = qMax(mMaximumCardinalitySameIndividualsJointlyConsidered, hashedLinker->getCurrentCardinalities()->getSameIndividualsJointlyConsideredCardinality());
							mMaximumCardinalitySameIndividualsSeparatelyConsidered = qMax(mMaximumCardinalitySameIndividualsSeparatelyConsidered, hashedLinker->getCurrentCardinalities()->getSameIndividualsSeparatlyConsideredCardinality());
						} else {
							cardinalities = getBindingsCardinalityCopy(cardinalities);
							COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalities = hashedLinker->getCurrentCardinalities();
							if (prevCardinalities) {
								cardinalities->addCardinalities(prevCardinalities);
							}
							mMaximumCardinalitySameIndividualsJointlyConsidered = qMax(mMaximumCardinalitySameIndividualsJointlyConsidered, cardinalities->getSameIndividualsJointlyConsideredCardinality());
							mMaximumCardinalitySameIndividualsSeparatelyConsidered = qMax(mMaximumCardinalitySameIndividualsSeparatelyConsidered, cardinalities->getSameIndividualsSeparatlyConsideredCardinality());
							hashedLinker->updateCardinality(cardinalities);
							cardinalities->setCardinalityUpdateId(mCurrentUpdateId);
							addCreatingUpdateCardinalityLinker(hashedLinker, prevCardinalities);
						}
					}
				}
				return false;
			}


			bool COptimizedComplexVariableIndividualMappingsMultiHashPart::addBindingsCardinalitiesCopy(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* addedCardinalites) {
				COptimizedComplexVariableIndividualBindingsCardinalityLinker tmpLinker(bindings, addedCardinalites);
				COptimizedComplexVariableIndividualBindingsHasher hasher(&tmpLinker);
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* hashedLinker = mMappingCardinalityHash.value(hasher);
				if (hashedLinker) {
					if (hashedLinker->getCurrentCardinalities() && hashedLinker->getCurrentCardinalities()->getCardinalityUpdateId() == mCurrentUpdateId) {
						hashedLinker->getCurrentCardinalities()->addCardinalities(addedCardinalites);
						mMaximumCardinalitySameIndividualsJointlyConsidered = qMax(mMaximumCardinalitySameIndividualsJointlyConsidered, hashedLinker->getCurrentCardinalities()->getSameIndividualsJointlyConsideredCardinality());
						mMaximumCardinalitySameIndividualsSeparatelyConsidered = qMax(mMaximumCardinalitySameIndividualsSeparatelyConsidered, hashedLinker->getCurrentCardinalities()->getSameIndividualsSeparatlyConsideredCardinality());
					} else {
						COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalities = hashedLinker->getCurrentCardinalities();
						if (prevCardinalities) {
							addedCardinalites->addCardinalities(prevCardinalities);
						}
						mMaximumCardinalitySameIndividualsJointlyConsidered = qMax(mMaximumCardinalitySameIndividualsJointlyConsidered, addedCardinalites->getSameIndividualsJointlyConsideredCardinality());
						mMaximumCardinalitySameIndividualsSeparatelyConsidered = qMax(mMaximumCardinalitySameIndividualsSeparatelyConsidered, addedCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
						addedCardinalites->setCardinalityUpdateId(mCurrentUpdateId);
						hashedLinker->updateCardinality(getBindingsCardinalityCopy(addedCardinalites));
						addCreatingUpdateCardinalityLinker(hashedLinker, prevCardinalities);
						return true;
					}
				}
				return false;
			}



			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualMappingsMultiHashPart::getBindingsCardinalityLinkerCopy(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker) {
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingCardLinkerCopy = mFreeBindCardLinkerCopy;
				if (mFreeBindCardLinkerCopy) {
					mFreeBindCardLinkerCopy = nullptr;
				} else {
					bindingCardLinkerCopy = CObjectAllocator<COptimizedComplexVariableIndividualBindingsCardinalityLinker>::allocateAndConstruct(getMemoryAllocationManager());
				}
				*bindingCardLinkerCopy = *linker;

				COptimizedComplexVariableIndividualBindings* bindingsCopy = getBindingsCopy(linker->getBindings());
				COptimizedComplexVariableIndividualBindingsCardinality* cardCopy = getBindingsCardinalityCopy(linker->getCurrentCardinalities());
				bindingCardLinkerCopy->initBindingsCardinalityLinker(bindingsCopy, cardCopy);
				return bindingCardLinkerCopy;
			}


			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::releaseBindingsCardinalityLinkerCopy(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker) {
				mFreeBindCardLinkerCopy = linker;
				releaseBindingsCopy(mFreeBindCardLinkerCopy->getBindings());
				releaseBindingsCardinalityCopy(mFreeBindCardLinkerCopy->getCurrentCardinalities());
				return this;
			}


			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexVariableIndividualMappingsMultiHashPart::getBindingsCardinalityCopy(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities) {
				if (!cardinalities) {
					return nullptr;
				}
				++mStatCardinalitiesCopyCount;
				COptimizedComplexVariableIndividualBindingsCardinality* cardCopy = mFreeCardCopy;
				if (mFreeCardCopy) {
					mFreeCardCopy = nullptr;
				} else {
					cardCopy = CObjectAllocator<COptimizedComplexVariableIndividualBindingsCardinality>::allocateAndConstruct(getMemoryAllocationManager());
				}
				*cardCopy = *cardinalities;
				return cardCopy;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::releaseBindingsCardinalityCopy(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities) {
				if (cardinalities) {
					mFreeCardCopy = cardinalities;
				}
				return this;
			}





			COptimizedComplexVariableIndividualBindings* COptimizedComplexVariableIndividualMappingsMultiHashPart::getBindingsCopy(COptimizedComplexVariableIndividualBindings* bindings) {
				++mStatBindingsCopyCount;
				COptimizedComplexVariableIndividualBindings* bindingsCopy = mFreeBindingsCopy;
				if (mFreeBindingsCopy) {
					mFreeBindingsCopy = nullptr;
				} else {
					TIndividualInstanceItemDataBinding* bindingArray = CObjectAllocator<TIndividualInstanceItemDataBinding>::allocateAndConstructArray(getMemoryAllocationManager(), mBindingSize);
					bindingsCopy = CObjectAllocator<COptimizedComplexVariableIndividualBindings>::allocateAndConstruct(getMemoryAllocationManager());
					bindingsCopy->initIndividualBindings(mBindingSize, bindingArray);
				}
				for (cint64 i = 0; i < mBindingSize; ++i) {
					bindingsCopy->setBinding(i, bindings->getBinding(i));
				}
				return bindingsCopy;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::releaseBindingsCopy(COptimizedComplexVariableIndividualBindings* bindings) {
				mFreeBindingsCopy = bindings;
				return this;
			}




			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::addCreatingUpdateCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingsLinker, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalities) {
				COptimizedComplexVariableIndividualUpdateCardinalityLinker* updateCardLinker = CObjectAllocator<COptimizedComplexVariableIndividualUpdateCardinalityLinker>::allocateAndConstruct(getMemoryAllocationManager());
				updateCardLinker->initBindingsUpdatedCardinalityLinker(bindingsLinker, prevCardinalities);
				addLastUpdatedCardinalityLinker(updateCardLinker);
				return this;
			}




			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualMappingsMultiHashPart::getFirstBindingsCardinalityLinker() {
				return mFirstAddedBindingsCardinalityLinker;
			}

			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualMappingsMultiHashPart::getLastBindingsCardinalityLinker() {
				return mLastAddedBindingsCardinalityLinker;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::setLastBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker) {
				mLastAddedBindingsCardinalityLinker = linker;
				return this;
			}


			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::clearBindingsCardinalityLinkers() {
				mFirstAddedBindingsCardinalityLinker = nullptr;
				mLastAddedBindingsCardinalityLinker = nullptr;
				return this;
			}



			COptimizedComplexVariableIndividualUpdateCardinalityLinker* COptimizedComplexVariableIndividualMappingsMultiHashPart::getFirstUpdateCardinalityLinker() {
				return mFirstUpdateCardinalityLinker;
			}

			COptimizedComplexVariableIndividualUpdateCardinalityLinker* COptimizedComplexVariableIndividualMappingsMultiHashPart::getLastUpdateCardinalityLinker() {
				return mLastUpdateCardinalityLinker;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::setLastUpdateCardinalityLinker(COptimizedComplexVariableIndividualUpdateCardinalityLinker* linker) {
				mLastUpdateCardinalityLinker = linker;
				return this;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::clearUpdateCardinalityLinkers() {
				mFirstUpdateCardinalityLinker = nullptr;
				mLastUpdateCardinalityLinker = nullptr;
				return this;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::removeBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker) {
				mMappingCardinalityHash.remove(linker);
				return this;
			}


			COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* COptimizedComplexVariableIndividualMappingsMultiHashPart::getLastAddedBindingsCardinalityBatchLinker() {
				return mLastAddedBindingsCardinalityBatchLinker;
			}

			COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* COptimizedComplexVariableIndividualMappingsMultiHashPart::getFirstAddedBindingsCardinalityBatchLinker() {
				return mFirstAddedBindingsCardinalityBatchLinker;
			}

			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::clearBindingsCardinalityBatchLinker() {
				mLastAddedBindingsCardinalityBatchLinker = nullptr;
				mFirstAddedBindingsCardinalityBatchLinker = nullptr;
				return this;
			}



			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHashPart::extendAddedBindingsCardinalityBatchLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker) {
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* stopLinker = nullptr;
				if (mLastAddedBindingsCardinalityBatchLinker) {
					stopLinker = mLastAddedBindingsCardinalityBatchLinker->getStartBindingsCardinalityLinker();
				}
				cint64 count = 0;
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* lastLinker = nullptr;
				for (COptimizedComplexVariableIndividualBindingsCardinalityLinker* linkerIt = linker; linkerIt && linkerIt != stopLinker; linkerIt = linkerIt->getNext()) {
					++count;
					lastLinker = linkerIt;
				}
				if (mLastAddedBindingsCardinalityBatchLinkerUpdateId == mCurrentUpdateId && mLastAddedBindingsCardinalityBatchLinker && mLastAddedBindingsCardinalityBatchLinker->getLinkerCount() < mCurrentLinkerBatchingSize && mLastAddedBindingsCardinalityBatchLinker->getStartBindingsCardinalityLinker() == mLastAddedBindingsCardinalityLinker) {
					mLastAddedBindingsCardinalityBatchLinker->addStartBindingsCardLinker(linker, count);
				} else {
					if (mLastAddedBindingsCardinalityBatchLinker) {
						mCurrentLinkerBatchingSize *= mLinkerBatchingSizeIncreasingFactor;
					}
					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* newLastAddedBindingsCardinalityBatchLinker = new COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker(linker, lastLinker, count);
					if (mLastAddedBindingsCardinalityBatchLinker) {
						mLastAddedBindingsCardinalityBatchLinker->setNext(newLastAddedBindingsCardinalityBatchLinker);
					} else {
						mFirstAddedBindingsCardinalityBatchLinker = newLastAddedBindingsCardinalityBatchLinker;
					}
					//newLastAddedBindingsCardinalityBatchLinker->setNext(mLastAddedBindingsCardinalityBatchLinker);
					mLastAddedBindingsCardinalityBatchLinker = newLastAddedBindingsCardinalityBatchLinker;
					mLastAddedBindingsCardinalityBatchLinkerUpdateId = mCurrentUpdateId;
				}
				return this;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
