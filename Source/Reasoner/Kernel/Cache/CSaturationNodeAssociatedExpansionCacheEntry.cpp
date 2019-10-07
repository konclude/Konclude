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

#include "CSaturationNodeAssociatedExpansionCacheEntry.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSaturationNodeAssociatedExpansionCacheEntry::CSaturationNodeAssociatedExpansionCacheEntry(CSaturationNodeAssociatedExpansionCacheContext* context) : mContext(context) {
				}


				CSaturationNodeAssociatedExpansionCacheEntry* CSaturationNodeAssociatedExpansionCacheEntry::initCacheEntry(CIndividualSaturationProcessNode* saturationNode, cint64 remainingAllowedNonDetExpansionCount) {
					mSaturationNode = saturationNode;
					mNondetExpansionLinker = nullptr;
					mDetExpansion = nullptr;
					mRemainingAllowedNonDetExpansionCount = remainingAllowedNonDetExpansionCount;
					return this;
				}


				CIndividualSaturationProcessNode* CSaturationNodeAssociatedExpansionCacheEntry::getSaturationIndividualNode() {
					return mSaturationNode;
				}


				CSaturationNodeAssociatedDeterministicConceptExpansion* CSaturationNodeAssociatedExpansionCacheEntry::getDeterministicConceptExpansion() {
					return mDetExpansion;
				}

				bool CSaturationNodeAssociatedExpansionCacheEntry::hasDeterministicConceptExpansion() {
					return mDetExpansion != nullptr;
				}


				CSaturationNodeAssociatedNondeterministicConceptExpansion* CSaturationNodeAssociatedExpansionCacheEntry::getNondeterministicConceptExpansionLinker() {
					return mNondetExpansionLinker;
				}


				CSaturationNodeAssociatedExpansionCacheEntry* CSaturationNodeAssociatedExpansionCacheEntry::setDeterministicConceptExpansion(CSaturationNodeAssociatedDeterministicConceptExpansion* detConceptExpansion) {
					mDetExpansion = detConceptExpansion;
					return this;
				}

				CSaturationNodeAssociatedExpansionCacheEntry* CSaturationNodeAssociatedExpansionCacheEntry::addNondeterministicConceptExpansion(CSaturationNodeAssociatedNondeterministicConceptExpansion* nondetConceptExpansion) {
					mNondetExpansionLinker = nondetConceptExpansion->append(mNondetExpansionLinker);
					return this;
				}


				cint64 CSaturationNodeAssociatedExpansionCacheEntry::getRemainingAllowedNondeterministicExpansionCount() {
					return mRemainingAllowedNonDetExpansionCount;
				}


				bool CSaturationNodeAssociatedExpansionCacheEntry::areMoreNondeterministicExpansionAllowed() {
					return mRemainingAllowedNonDetExpansionCount > 0;
				}

				CSaturationNodeAssociatedExpansionCacheEntry* CSaturationNodeAssociatedExpansionCacheEntry::decRemainingAllowedNondeterministicExpansionCount(cint64 decCount) {
					if (mRemainingAllowedNonDetExpansionCount > 0) {
						mRemainingAllowedNonDetExpansionCount -= decCount;
					}
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
