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

#include "CBackendRepresentativeMemoryCacheOntologyData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheOntologyData::CBackendRepresentativeMemoryCacheOntologyData(CBackendRepresentativeMemoryCacheBaseContext* baseContext) : mTemporaryContext(baseContext) {
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::initOntologyData(cint64 ontologyIdentifer) {
					mOntologyIdentifer = ontologyIdentifer;

					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT; ++i) {
						mSigLabelItemHash[i] = nullptr;
					}
					mNominalIndiIdIndirectConnectionDataHash = nullptr;
					mIndiIdAssoDataHash = nullptr;
					mIndiIdAssoDataVector = nullptr;
					mIndiIdAssoDataVectorSize = 0;
					mSameMergedIndisInCache = false;
					mZeroIncompletelyHandledIndiIdCountDebugWritten = false;
					mLastMinIncompletelyHandledIndiId = CINT64_MAX;
					mMaxIndiAssocDataUpdateCount = 0;
					mIncompletelyHandledIndiIdCount = 0;
					mUsageCount = 0;
					mOntologyContext = nullptr;
					mNextEntryID = 1;

					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyData::getOntologyIdentifer() {
					return mOntologyIdentifer;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getUsageCount() {
					return mUsageCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incUsageCount(cint64 count) {
					mUsageCount += count;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::decUsageCount(cint64 count) {
					mUsageCount -= count;
					return this;
				}


				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setSignatureLabelItemHash(cint64 labelType, CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigItemHash) {
					mSigLabelItemHash[labelType] = sigItemHash;
					return this;
				}



				bool CBackendRepresentativeMemoryCacheOntologyData::hasSameIndividualsMergings() {
					return mSameMergedIndisInCache;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setSameIndividualsMergings(bool sameIndisMergings) {
					mSameMergedIndisInCache = sameIndisMergings;
					return this;
				}


				CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* CBackendRepresentativeMemoryCacheOntologyData::getSignatureLabelItemHash(cint64 labelType) {
					return mSigLabelItemHash[labelType];
				}


				CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* CBackendRepresentativeMemoryCacheOntologyData::getIndividualIdAssociationDataHash() {
					return mIndiIdAssoDataHash;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIndividualIdAssociationDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* indiIdAssoDataHash) {
					mIndiIdAssoDataHash = indiIdAssoDataHash;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyData::getIndividualIdAssoiationDataVectorSize() {
					return mIndiIdAssoDataVectorSize;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIndividualIdAssoiationDataVectorSize(cint64 size) {
					mIndiIdAssoDataVectorSize = size;
					return this;
				}


				CBackendRepresentativeMemoryCacheIndividualAssociationData** CBackendRepresentativeMemoryCacheOntologyData::getIndividualIdAssoiationDataVector() {
					return mIndiIdAssoDataVector;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIndividualIdAssoiationDataVector(cint64 indiIdAssoDataVectorSize, CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector) {
					mIndiIdAssoDataVector = indiIdAssoDataVector;
					mIndiIdAssoDataVectorSize = indiIdAssoDataVectorSize;
					return this;
				}


				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setNominaIIndividualdIndirectConnectionDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* hash) {
					mNominalIndiIdIndirectConnectionDataHash = hash;
					return this;
				}


				CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* CBackendRepresentativeMemoryCacheOntologyData::getNominaIIndividualdIndirectConnectionDataHash() {
					return mNominalIndiIdIndirectConnectionDataHash;
				}







				cint64 CBackendRepresentativeMemoryCacheOntologyData::getNextEntryID(bool moveNext) {
					cint64 nextEntryId = mNextEntryID;
					if (moveNext) {
						mNextEntryID++;
					}
					return nextEntryId;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setNextEntryID(cint64 nextEntryID) {
					mNextEntryID = nextEntryID;
					return this;
				}



				cint64 CBackendRepresentativeMemoryCacheOntologyData::getLastMinIncompletelyHandledIndvidualiId() {
					return mLastMinIncompletelyHandledIndiId;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setLastMinIncompletelyHandledIndvidualiId(cint64 id) {
					mLastMinIncompletelyHandledIndiId = id;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incLastMinIncompletelyHandledIndvidualiId(cint64 count) {
					mLastMinIncompletelyHandledIndiId += count;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getIncompletelyHandledIndividualIdCount() {
					return mIncompletelyHandledIndiIdCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIncompletelyHandledIndividualIdCount(cint64 count) {
					mIncompletelyHandledIndiIdCount = count;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incIncompletelyHandledIndividualIdCount(cint64 count) {
					mIncompletelyHandledIndiIdCount += count;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::decIncompletelyHandledIndividualIdCount(cint64 count) {
					mIncompletelyHandledIndiIdCount -= count;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheOntologyData::getZeroIncompletelyHandledIndividualIdCountDebugWritten() {
					return mZeroIncompletelyHandledIndiIdCountDebugWritten;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setZeroIncompletelyHandledIndividualIdCountDebugWritten(bool written) {
					mZeroIncompletelyHandledIndiIdCountDebugWritten = written;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getMaxIndividualAssociationDataUpdateCount() {
					return mMaxIndiAssocDataUpdateCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setMaxIndividualAssociationDataUpdateCount(cint64 count) {
					mMaxIndiAssocDataUpdateCount = count;
					return this;
				}


				CBackendRepresentativeMemoryCacheOntologyContext* CBackendRepresentativeMemoryCacheOntologyData::getTemporaryContext() {
					return &mTemporaryContext;
				}

				CBackendRepresentativeMemoryCacheOntologyContext* CBackendRepresentativeMemoryCacheOntologyData::getOntologyContext() {
					return mOntologyContext;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setOntologyContext(CBackendRepresentativeMemoryCacheOntologyContext* ontContext) {
					mOntologyContext = ontContext;
					return this;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
