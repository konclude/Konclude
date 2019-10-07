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

#include "COptimizedComplexVariableIndividualMappingsMultiHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualMappingsMultiHash::COptimizedComplexVariableIndividualMappingsMultiHash(cint64 bindingSize, cint64 multiHashPartsCount) : COptimizedComplexVariableIndividualMappings(bindingSize) {
				mMultiHashPartsCount = multiHashPartsCount;
				mMultiHashPartArray = new COptimizedComplexVariableIndividualMappingsMultiHashPart*[mMultiHashPartsCount];
				for (cint64 i = 0; i < mMultiHashPartsCount; ++i) {
					mMultiHashPartArray[i] = new COptimizedComplexVariableIndividualMappingsMultiHashPart(bindingSize);
				}
				mLastBindingCardinalityBatchLinkerUpdateId = 0;
			}


			COptimizedComplexVariableIndividualMappingsMultiHash::~COptimizedComplexVariableIndividualMappingsMultiHash() {
				for (cint64 i = 0; i < mMultiHashPartsCount; ++i) {
					delete mMultiHashPartArray[i];
				}
				delete [] mMultiHashPartArray;
			}


			COptimizedComplexVariableIndividualMappingsMultiHashPart* COptimizedComplexVariableIndividualMappingsMultiHash::getMultiHashPart(cint64 index) {
				if (index >= 0 && index <= mMultiHashPartsCount) {
					return mMultiHashPartArray[index];
				} else {
					return nullptr;
				}
			}



			COptimizedComplexVariableIndividualMappingsMultiHash* COptimizedComplexVariableIndividualMappingsMultiHash::updateLinkers(COptimizedComplexVariableIndividualMappingsMultiHashPart* multiPart) {
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* lastAddedBindingsCardinalityLinker = multiPart->getLastBindingsCardinalityLinker();
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* firstAddedBindingsCardinalityLinker = multiPart->getFirstBindingsCardinalityLinker();

				COptimizedComplexVariableIndividualUpdateCardinalityLinker* lastUpdateCardinalityLinker = multiPart->getLastUpdateCardinalityLinker();
				COptimizedComplexVariableIndividualUpdateCardinalityLinker* firstUpdateCardinalityLinker = multiPart->getFirstUpdateCardinalityLinker();


				COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* lastAddedBindingsCardinalityBatchLinker = multiPart->getLastAddedBindingsCardinalityBatchLinker();
				COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* firstAddedBindingsCardinalityBatchLinker = multiPart->getFirstAddedBindingsCardinalityBatchLinker();




				if (firstAddedBindingsCardinalityBatchLinker && lastAddedBindingsCardinalityBatchLinker) {


					if (mLastAddedBindingsCardinalityBatchLinker && mLastAddedBindingsCardinalityBatchLinker->getStartBindingsCardinalityLinker() == mLastAddedBindingsCardinalityLinker
						&& mLastAddedBindingsCardinalityBatchLinker->getLinkerCount() + firstAddedBindingsCardinalityBatchLinker->getLinkerCount() < 100 && mLastBindingCardinalityBatchLinkerUpdateId == mCurrentUpdateId) {

						mLastAddedBindingsCardinalityBatchLinker->addStartBindingsCardLinker(firstAddedBindingsCardinalityBatchLinker->getStartBindingsCardinalityLinker(), firstAddedBindingsCardinalityBatchLinker->getLinkerCount());

						if (firstAddedBindingsCardinalityBatchLinker == lastAddedBindingsCardinalityBatchLinker) {
							firstAddedBindingsCardinalityBatchLinker = nullptr;
						} else {
							firstAddedBindingsCardinalityBatchLinker = firstAddedBindingsCardinalityBatchLinker->getNext();
						}
					}

					
					if (firstAddedBindingsCardinalityBatchLinker) {
						if (!mFirstAddedBindingsCardinalityBatchLinker) {
							mFirstAddedBindingsCardinalityBatchLinker = firstAddedBindingsCardinalityBatchLinker;
						} else {
							mLastAddedBindingsCardinalityBatchLinker->setNext(firstAddedBindingsCardinalityBatchLinker);
						}
						mLastAddedBindingsCardinalityBatchLinker = lastAddedBindingsCardinalityBatchLinker;
						mLastBindingCardinalityBatchLinkerUpdateId = mCurrentUpdateId;
					}
					multiPart->clearBindingsCardinalityBatchLinker();
				}

				if (firstAddedBindingsCardinalityLinker && lastAddedBindingsCardinalityLinker) {
					firstAddedBindingsCardinalityLinker->append(mLastAddedBindingsCardinalityLinker);
					mLastAddedBindingsCardinalityLinker = lastAddedBindingsCardinalityLinker;
					multiPart->clearBindingsCardinalityLinkers();
				}

				if (firstUpdateCardinalityLinker && lastUpdateCardinalityLinker) {
					firstUpdateCardinalityLinker->append(mLastUpdateCardinalityLinker);
					mLastUpdateCardinalityLinker = lastUpdateCardinalityLinker;
					multiPart->clearUpdateCardinalityLinkers();
				}

				return this;
			}

			COptimizedComplexVariableIndividualMappingsMultiHash* COptimizedComplexVariableIndividualMappingsMultiHash::updateBindingCount(COptimizedComplexVariableIndividualMappingsMultiHashPart* multiPart) {
				mBindingCount -= multiPart->getLastUpdatedBindingCount(true);
				mBindingCount += multiPart->getBindingCount();

				mMaximumCardinalitySameIndividualsJointlyConsidered = qMax(mMaximumCardinalitySameIndividualsJointlyConsidered, multiPart->getMaximumCardinalitySameIndividualsJointlyConsidered());
				mMaximumCardinalitySameIndividualsSeparatelyConsidered = qMax(mMaximumCardinalitySameIndividualsSeparatelyConsidered, multiPart->getMaximumCardinalitySameIndividualsSeparatelyConsidered());

				return this;
			}


			COptimizedComplexVariableIndividualMappingsMultiHash* COptimizedComplexVariableIndividualMappingsMultiHash::resetBindingCount() {
				mBindingCount = 0;
				return this;
			}



			bool COptimizedComplexVariableIndividualMappingsMultiHash::addInsertingBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, bool addCardinalitiesIfAlreadyPresent) {
				COptimizedComplexVariableIndividualBindingsHasher hasher(linker);
				cint64 partIndex = hasher.getHashValue() % mMultiHashPartsCount;
				COptimizedComplexVariableIndividualMappingsMultiHashPart* partHasher = mMultiHashPartArray[partIndex];

				bool addedLinker = partHasher->addInsertingBindingsCardinalityLinkerCopy(linker, addCardinalitiesIfAlreadyPresent);
				if (addedLinker) {
					mBindingCount++;
				}

				updateLinkers(partHasher);
				return addedLinker;
			}


			bool COptimizedComplexVariableIndividualMappingsMultiHash::addBindingsCardinalities(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* addedCardinalites) {
				COptimizedComplexVariableIndividualBindingsCardinalityLinker tmpLinker(bindings, addedCardinalites);
				COptimizedComplexVariableIndividualBindingsHasher hasher(&tmpLinker);
				cint64 partIndex = hasher.getHashValue() % mMultiHashPartsCount;
				COptimizedComplexVariableIndividualMappingsMultiHashPart* partHasher = mMultiHashPartArray[partIndex];

				bool addedLinker = partHasher->addBindingsCardinalitiesCopy(bindings, addedCardinalites);
				if (addedLinker) {
					mBindingCount++;
				}

				updateLinkers(partHasher);
				return addedLinker;
			}



			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualMappingsMultiHash::takeBindingsCardinalityLinker() {
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = mLastAddedBindingsCardinalityLinker;
				if (mLastAddedBindingsCardinalityLinker) {
					mLastAddedBindingsCardinalityLinker = mLastAddedBindingsCardinalityLinker->getNext();
				}
				if (linker) {
					COptimizedComplexVariableIndividualBindingsHasher hasher(linker);
					cint64 partIndex = hasher.getHashValue() % mMultiHashPartsCount;
					COptimizedComplexVariableIndividualMappingsMultiHashPart* partHasher = mMultiHashPartArray[partIndex];

					partHasher->removeBindingsCardinalityLinker(linker);
					mBindingCount--;
					linker->clearNext();
				}
				return linker;
			}


			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableIndividualMappingsMultiHash::incCurrentUpdateId(cint64 incUpdateCount) {
				COptimizedComplexVariableIndividualMappings::incCurrentUpdateId(incUpdateCount);
				for (cint64 i = 0; i < mMultiHashPartsCount; ++i) {
					mMultiHashPartArray[i]->setCurrentUpdateId(mCurrentUpdateId);
				}
				return this;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
