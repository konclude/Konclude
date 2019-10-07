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

#include "COptimizedComplexVariableIndividualMappings.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualMappings::COptimizedComplexVariableIndividualMappings(cint64 bindingSize) {
				mBindingSize = bindingSize;
				mBindingCount = 0;
				mBindingMapping = new VARIABLE_TYPE[bindingSize];
				mLastAddedBindingsCardinalityLinker = nullptr;
				mLastAddedBindingsCardinalityBatchLinker = nullptr;
				mFirstAddedBindingsCardinalityBatchLinker = nullptr;
				mLastUpdateCardinalityLinker = nullptr;
				mCurrentUpdateId = 0;
				mLastAddedBindingsCardinalityBatchLinkerUpdateId = 0;
				mMaximumCardinalitySameIndividualsJointlyConsidered = 0;
				mMaximumCardinalitySameIndividualsSeparatelyConsidered = 0;

				mLinkerBatchingSizeIncreasingFactor = 1.25;
				mCurrentLinkerBatchingSize = 100;
			}


			COptimizedComplexVariableIndividualMappings::~COptimizedComplexVariableIndividualMappings() {
				delete[] mBindingMapping;
			}


			cint64 COptimizedComplexVariableIndividualMappings::getMaximumCardinalitySameIndividualsJointlyConsidered() {
				return mMaximumCardinalitySameIndividualsJointlyConsidered;
			}

			cint64 COptimizedComplexVariableIndividualMappings::getMaximumCardinalitySameIndividualsSeparatelyConsidered() {
				return mMaximumCardinalitySameIndividualsSeparatelyConsidered;
			}



			COptimizedComplexVariableIndividualMappings::VARIABLE_TYPE COptimizedComplexVariableIndividualMappings::getBindingMapping(cint64 idx) {
				return mBindingMapping[idx];
			}

			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableIndividualMappings::setBindingMapping(cint64 idx, COptimizedComplexVariableIndividualMappings::VARIABLE_TYPE varType) {
				mBindingMapping[idx] = varType;
				return this;
			}

			cint64 COptimizedComplexVariableIndividualMappings::getBindingSize() {
				return mBindingSize;
			}

			cint64 COptimizedComplexVariableIndividualMappings::getBindingCount() {
				return mBindingCount;
			}

			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualMappings::getLastAddedBindingsCardinalityLinker() {
				return mLastAddedBindingsCardinalityLinker;
			}

			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableIndividualMappings::addLastAddedBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker) {
				extendAddedBindingsCardinalityBatchLinker(linker);
				mLastAddedBindingsCardinalityLinker = linker->append(mLastAddedBindingsCardinalityLinker);
				return this;
			}

			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableIndividualMappings::addLastUpdatedCardinalityLinker(COptimizedComplexVariableIndividualUpdateCardinalityLinker* linker) {
				mLastUpdateCardinalityLinker = linker->append(mLastUpdateCardinalityLinker);
				return this;
			}



			COptimizedComplexVariableIndividualUpdateCardinalityLinker* COptimizedComplexVariableIndividualMappings::getLastUpdatedCardinalityLinker() {
				return mLastUpdateCardinalityLinker;
			}

			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableIndividualMappings::setCurrentUpdateId(cint64 currUpdateId) {
				mCurrentUpdateId = currUpdateId;
				return this;
			}

			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableIndividualMappings::incCurrentUpdateId(cint64 incUpdateCount) {
				mCurrentUpdateId += incUpdateCount;
				return this;
			}

			cint64 COptimizedComplexVariableIndividualMappings::getCurrentUpdateId() {
				return mCurrentUpdateId;
			}

			COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* COptimizedComplexVariableIndividualMappings::getLastAddedBindingsCardinalityBatchLinker() {
				return mLastAddedBindingsCardinalityBatchLinker;
			}


			COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* COptimizedComplexVariableIndividualMappings::getFirstAddedBindingsCardinalityBatchLinker() {
				return mFirstAddedBindingsCardinalityBatchLinker;
			}

			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableIndividualMappings::extendAddedBindingsCardinalityBatchLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker) {
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
