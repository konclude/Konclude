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

#include "COptimizedComplexVariableIndividualMappingsHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualMappingsHash::COptimizedComplexVariableIndividualMappingsHash(cint64 bindingSize) : COptimizedComplexVariableIndividualMappings(bindingSize) {
			}


			COptimizedComplexVariableIndividualMappingsHash::~COptimizedComplexVariableIndividualMappingsHash() {
			}



			bool COptimizedComplexVariableIndividualMappingsHash::addInsertingBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, bool addCardinalitiesIfAlreadyPresent) {
				COptimizedComplexVariableIndividualBindingsHasher hasher(linker);
				COptimizedComplexVariableIndividualBindingsCardinalityLinker*& hashedLinker = mMappingCardinalityHash[hasher];
				if (!hashedLinker) {
					hashedLinker = linker;
					if (linker->getCurrentCardinalities()) {
						linker->getCurrentCardinalities()->setCardinalityUpdateId(mCurrentUpdateId);
						mMaximumCardinalitySameIndividualsJointlyConsidered = qMax(mMaximumCardinalitySameIndividualsJointlyConsidered, linker->getCurrentCardinalities()->getSameIndividualsJointlyConsideredCardinality());
						mMaximumCardinalitySameIndividualsSeparatelyConsidered = qMax(mMaximumCardinalitySameIndividualsSeparatelyConsidered, linker->getCurrentCardinalities()->getSameIndividualsSeparatlyConsideredCardinality());
					}
					addLastAddedBindingsCardinalityLinker(linker);
					mBindingCount = mMappingCardinalityHash.size();
					return true;
				} else if (addCardinalitiesIfAlreadyPresent) {
					COptimizedComplexVariableIndividualBindingsCardinality* cardinalities = linker->getCurrentCardinalities();
					if (cardinalities) {
						if (hashedLinker->getCurrentCardinalities() && hashedLinker->getCurrentCardinalities()->getCardinalityUpdateId() == mCurrentUpdateId) {
							hashedLinker->getCurrentCardinalities()->addCardinalities(cardinalities);
							mMaximumCardinalitySameIndividualsJointlyConsidered = qMax(mMaximumCardinalitySameIndividualsJointlyConsidered, hashedLinker->getCurrentCardinalities()->getSameIndividualsJointlyConsideredCardinality());
							mMaximumCardinalitySameIndividualsSeparatelyConsidered = qMax(mMaximumCardinalitySameIndividualsSeparatelyConsidered, hashedLinker->getCurrentCardinalities()->getSameIndividualsSeparatlyConsideredCardinality());
						} else {
							linker->clearCardinalities();
							COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalities = hashedLinker->getCurrentCardinalities();
							if (prevCardinalities) {
								cardinalities->addCardinalities(prevCardinalities);
							}
							mMaximumCardinalitySameIndividualsJointlyConsidered = qMax(mMaximumCardinalitySameIndividualsJointlyConsidered, cardinalities->getSameIndividualsJointlyConsideredCardinality());
							mMaximumCardinalitySameIndividualsSeparatelyConsidered = qMax(mMaximumCardinalitySameIndividualsSeparatelyConsidered, cardinalities->getSameIndividualsSeparatlyConsideredCardinality());
							hashedLinker->updateCardinality(cardinalities);
							cardinalities->setCardinalityUpdateId(mCurrentUpdateId);
							COptimizedComplexVariableIndividualUpdateCardinalityLinker* updateCardLinker = new COptimizedComplexVariableIndividualUpdateCardinalityLinker(hashedLinker, prevCardinalities);
							addLastUpdatedCardinalityLinker(updateCardLinker);
						}
					}
				}
				return false;
			}


			bool COptimizedComplexVariableIndividualMappingsHash::addBindingsCardinalities(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* addedCardinalites) {
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
						hashedLinker->updateCardinality(addedCardinalites);
						addedCardinalites->setCardinalityUpdateId(mCurrentUpdateId);
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* updateCardLinker = new COptimizedComplexVariableIndividualUpdateCardinalityLinker(hashedLinker, prevCardinalities);
						addLastUpdatedCardinalityLinker(updateCardLinker);
						return true;
					}
				}
				return false;
			}



			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualMappingsHash::takeBindingsCardinalityLinker() {
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = mLastAddedBindingsCardinalityLinker;
				if (mLastAddedBindingsCardinalityLinker) {
					mLastAddedBindingsCardinalityLinker = mLastAddedBindingsCardinalityLinker->getNext();
				}
				if (linker) {
					mMappingCardinalityHash.remove(linker);
					mBindingCount = mMappingCardinalityHash.size();
					linker->clearNext();
				}
				return linker;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
