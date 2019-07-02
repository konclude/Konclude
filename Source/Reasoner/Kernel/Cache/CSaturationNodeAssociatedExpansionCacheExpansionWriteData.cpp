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

#include "CSaturationNodeAssociatedExpansionCacheExpansionWriteData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSaturationNodeAssociatedExpansionCacheExpansionWriteData::CSaturationNodeAssociatedExpansionCacheExpansionWriteData() {
					mWriteDataType = SNAECWT_EXPAND;
				}


				CSaturationNodeAssociatedExpansionCacheExpansionWriteData* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::initExpansionWriteData(CIndividualSaturationProcessNode* saturationNode, CSaturationNodeAssociatedConceptLinker* conLinker) {
					mSaturationNode = saturationNode;
					mExpansionConceptLinker = conLinker;
					mDependentNominalSet = nullptr;
					mTightAtMostRestriction = false;
					mDeterministicExpansion = true;
					mRequiresNondeterministicExpansion = false;
					mConceptSetSignature = 0;
					mTotalConceptCount = 0;
					return this;
				}

				CIndividualSaturationProcessNode* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::getSaturationIndividualNode() {
					return mSaturationNode;
				}

				CSaturationNodeAssociatedDependentNominalSet* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::getDependentNominalSet() {
					return mDependentNominalSet;
				}


				CSaturationNodeAssociatedExpansionCacheExpansionWriteData* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::setDependentNominalSet(CSaturationNodeAssociatedDependentNominalSet* depNomSet) {
					mDependentNominalSet = depNomSet;
					return this;
				}


				CSaturationNodeAssociatedConceptLinker* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::getExpansionConceptLinker() {
					return mExpansionConceptLinker;
				}

				bool CSaturationNodeAssociatedExpansionCacheExpansionWriteData::hasTightAtMostRestriction() {
					return mTightAtMostRestriction;
				}

				CSaturationNodeAssociatedExpansionCacheExpansionWriteData* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::setTightAtMostRestriction(bool tightAtMostRestriction) {
					mTightAtMostRestriction = tightAtMostRestriction;
					return this;
				}


				bool CSaturationNodeAssociatedExpansionCacheExpansionWriteData::isDeterministicExpansion() {
					return mDeterministicExpansion;
				}

				CSaturationNodeAssociatedExpansionCacheExpansionWriteData* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::setDeterministicExpansion(bool detExpansion) {
					mDeterministicExpansion = detExpansion;
					return this;
				}


				bool CSaturationNodeAssociatedExpansionCacheExpansionWriteData::requiresNondeterministicExpansion() {
					return mRequiresNondeterministicExpansion;
				}

				CSaturationNodeAssociatedExpansionCacheExpansionWriteData* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::setRequiresNondeterministicExpansion(bool requiresnondetExpansion) {
					mRequiresNondeterministicExpansion = requiresnondetExpansion;
					return this;
				}


				cint64 CSaturationNodeAssociatedExpansionCacheExpansionWriteData::getConceptSetSignature() {
					return mConceptSetSignature;
				}

				cint64 CSaturationNodeAssociatedExpansionCacheExpansionWriteData::getTotalConceptCount() {
					return mTotalConceptCount;
				}


				CSaturationNodeAssociatedExpansionCacheExpansionWriteData* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::setConceptSetSignature(cint64 signature) {
					mConceptSetSignature = signature;
					return this;
				}

				CSaturationNodeAssociatedExpansionCacheExpansionWriteData* CSaturationNodeAssociatedExpansionCacheExpansionWriteData::setTotalConceptCount(cint64 conceptCount) {
					mTotalConceptCount = conceptCount;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
