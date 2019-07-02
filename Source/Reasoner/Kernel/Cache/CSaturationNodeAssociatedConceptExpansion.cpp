/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CSaturationNodeAssociatedConceptExpansion.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSaturationNodeAssociatedConceptExpansion::CSaturationNodeAssociatedConceptExpansion(CSaturationNodeAssociatedExpansionCacheContext* context) : mContext(context),mConceptExpansionHash(context) {
				}

				CSaturationNodeAssociatedConceptExpansion* CSaturationNodeAssociatedConceptExpansion::initConceptExpansion() {
					mConceptExpansionLinker = nullptr;
					mHasTightCardinalityRestriction = false;
					mConceptExpansionCount = 0;
					mDependentNominalSet = nullptr;
					mConceptSetSignature = 0;
					mTotalConceptCount = 0;
					return this;
				}

				CCACHINGHASH<CCacheValueHasher,CSaturationNodeAssociatedConceptLinker*>* CSaturationNodeAssociatedConceptExpansion::getConceptExpansionHash() {
					return &mConceptExpansionHash;
				}


				CSaturationNodeAssociatedConceptExpansion* CSaturationNodeAssociatedConceptExpansion::addConceptExpansionLinker(CSaturationNodeAssociatedConceptLinker* conceptLinker) {
					if (conceptLinker) {
						mConceptExpansionCount += conceptLinker->getCount();
						mConceptExpansionLinker = conceptLinker->append(mConceptExpansionLinker);
						mConceptExpansionHash.insert(CCacheValueHasher(conceptLinker->getCacheValue()),conceptLinker);
					}
					return this;
				}


				CSaturationNodeAssociatedConceptLinker* CSaturationNodeAssociatedConceptExpansion::getConceptExpansionLinker(CCacheValue* cacheValue) {
					CSaturationNodeAssociatedConceptLinker* linker = mConceptExpansionHash.value(CCacheValueHasher(cacheValue),nullptr);
					return linker;
				}


				bool CSaturationNodeAssociatedConceptExpansion::hasConceptExpansionLinker(CCacheValue* cacheValue) {
					return mConceptExpansionHash.contains(CCacheValueHasher(cacheValue));
				}


				CSaturationNodeAssociatedConceptLinker* CSaturationNodeAssociatedConceptExpansion::getConceptExpansionLinker() {
					return mConceptExpansionLinker;
				}


				cint64 CSaturationNodeAssociatedConceptExpansion::getConceptExpansionCount() {
					return mConceptExpansionCount;
				}

				CSaturationNodeAssociatedConceptExpansion* CSaturationNodeAssociatedConceptExpansion::setConceptExpansionCount(cint64 count) {
					mConceptExpansionCount = count;
					return this;
				}

				CSaturationNodeAssociatedDependentNominalSet* CSaturationNodeAssociatedConceptExpansion::getDependentNominalSet(bool create) {
					if (!mDependentNominalSet && create) {
						mDependentNominalSet = CObjectParameterizingAllocator< CSaturationNodeAssociatedDependentNominalSet,CSaturationNodeAssociatedExpansionCacheContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(),mContext);
					}
					return mDependentNominalSet;
				}

				bool CSaturationNodeAssociatedConceptExpansion::getHasTightAtMostRestriction() {
					return mHasTightCardinalityRestriction;
				}


				CSaturationNodeAssociatedConceptExpansion* CSaturationNodeAssociatedConceptExpansion::setHasTightCardinalityRestriction(bool tightAtMostRestrictions) {
					mHasTightCardinalityRestriction = tightAtMostRestrictions;
					return this;
				}


				cint64 CSaturationNodeAssociatedConceptExpansion::getConceptSetSignature() {
					return mConceptSetSignature;
				}

				CSaturationNodeAssociatedConceptExpansion* CSaturationNodeAssociatedConceptExpansion::setConceptSetSignature(cint64 signature) {
					mConceptSetSignature = signature;
					return this;
				}


				cint64 CSaturationNodeAssociatedConceptExpansion::getTotalConceptCount() {
					return mTotalConceptCount;
				}

				CSaturationNodeAssociatedConceptExpansion* CSaturationNodeAssociatedConceptExpansion::setTotalConceptCount(cint64 totalConceptCount) {
					mTotalConceptCount = totalConceptCount;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
