/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CRepresentativeVariableBindingPathSetData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CRepresentativeVariableBindingPathSetData::CRepresentativeVariableBindingPathSetData(CProcessContext* processContext) : CLinkerBase<cint64,CRepresentativeVariableBindingPathSetData>(0),CLocalizationTag(processContext->getUsedProcessTagger()->getCurrentLocalizationTag()) {
					mProcessContext = processContext;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetData::initRepresentativeVariableBindingPathData(CRepresentativeVariableBindingPathSetData* data) {
					if (data) {
						setData(data->getData());
						mMigratable = data->mMigratable;
						mUseCount = data->mUseCount;
						mShareCount = data->mShareCount;
						mUseMigrateData = data->mUseMigrateData;
						mSigKey.initSignature(&data->mSigKey);
						mUseJoiningHash = data->mUseJoiningHash;
						mLocMigrateData = nullptr;
						mLocJoiningHash = nullptr;
					} else {
						setData(0);
						mMigratable = true;
						mUseMigrateData = nullptr;
						mLocMigrateData = nullptr;
						mUseJoiningHash = nullptr;
						mLocJoiningHash = nullptr;
						mSigKey.initSignature(nullptr);
						mUseCount = 0;
						mShareCount = 0;
					}
					return this;
				}

				cint64 CRepresentativeVariableBindingPathSetData::getRepresentativeKey() {
					return mSigKey.getSignatureValue();
				}

				cint64 CRepresentativeVariableBindingPathSetData::getRepresentativeID() {
					return getData();
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetData::setRepresentativeID(cint64 repID) {
					setData(repID);
					return this;
				}

				cint64 CRepresentativeVariableBindingPathSetData::getUseCount() {
					return mUseCount;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetData::setUseCount(cint64 useCount) {
					mUseCount = useCount;
					return this;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetData::incUseCount(cint64 incCount) {
					mUseCount += incCount;
					return this;
				}

				cint64 CRepresentativeVariableBindingPathSetData::getShareCount() {
					return mShareCount;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetData::setShareCount(cint64 shareCount) {
					mShareCount = shareCount;
					return this;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetData::incShareCount(cint64 incCount) {
					mShareCount += incCount;
					return this;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetData::decShareCount(cint64 decCount) {
					mShareCount -= decCount;
					return this;
				}

				bool CRepresentativeVariableBindingPathSetData::isMigratable() {
					return mMigratable;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetData::setMigratable(bool migratable) {
					mMigratable = migratable;
					return this;
				}


				bool CRepresentativeVariableBindingPathSetData::hasMigrateData() {
					return mUseMigrateData != nullptr;
				}

				CRepresentativeVariableBindingPathSetData* CRepresentativeVariableBindingPathSetData::addKeySignatureValue(cint64 keySignatureValue) {
					mSigKey.addKey(keySignatureValue);
					return this;
				}

				CRepresentativeVariableBindingPathSetDataSignature* CRepresentativeVariableBindingPathSetData::getKeySignature() {
					return &mSigKey;
				}

				CRepresentativeVariableBindingPathSetMigrateData* CRepresentativeVariableBindingPathSetData::getMigrateData(bool localizeOrCreate) {
					CRepresentativeVariableBindingPathSetMigrateData* data = nullptr;
					if (!mLocMigrateData && localizeOrCreate) {
						mLocMigrateData = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathSetMigrateData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getMemoryAllocationManager(),mProcessContext);
						mLocMigrateData->initRepresentativeVariableBindingPathSetMigrateData(mUseMigrateData);
						mUseMigrateData = mLocMigrateData;
					}
					return mUseMigrateData;
				}


				CRepresentativeVariableBindingPathSetMigrateData* CRepresentativeVariableBindingPathSetData::takeMigrateDataFrom(CRepresentativeVariableBindingPathSetData* repData) {
					mLocMigrateData = repData->mUseMigrateData;
					mUseMigrateData = mLocMigrateData;
					mSigKey.initSignature(&repData->mSigKey);
					repData->mUseMigrateData = nullptr;
					repData->mLocMigrateData = nullptr;
					return mUseMigrateData;
				}

				CRepresentativeVariableBindingPathSetMigrateData* CRepresentativeVariableBindingPathSetData::copyMigrateDataFrom(CRepresentativeVariableBindingPathSetData* repData) {
					mLocMigrateData = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathSetMigrateData,CProcessContext*>::allocateAndConstructAndParameterize(mProcessContext->getMemoryAllocationManager(),mProcessContext);
					mLocMigrateData->initRepresentativeVariableBindingPathSetMigrateData(repData->mUseMigrateData);
					mSigKey.initSignature(&repData->mSigKey);
					mUseMigrateData = mLocMigrateData;
					return mUseMigrateData;
				}


				CRepresentativeVariableBindingPathSetJoiningHash* CRepresentativeVariableBindingPathSetData::getJoiningHash(bool create) {
					if (create && !mLocJoiningHash) {
						mLocJoiningHash = CObjectParameterizingAllocator< CRepresentativeVariableBindingPathSetJoiningHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mLocJoiningHash->initRepresentativeVariableBindingPathSetJoiningHash(mUseJoiningHash);
						mUseJoiningHash = mLocJoiningHash;
					}
					return mUseJoiningHash;
				}


				bool CRepresentativeVariableBindingPathSetData::hasJoiningData(CConcept* joinConcept) {
					if (!mUseJoiningHash) {
						return false;
					}
					const CRepresentativeVariableBindingPathSetJoiningHashData& joiningData = mUseJoiningHash->value(joinConcept);
					return joiningData.mUseJoiningData != nullptr;
				}

				cint64 CRepresentativeVariableBindingPathSetData::getRepresentatedVariableCount() {
					cint64 varCount = 0;
					if (mUseMigrateData) {
						varCount = mUseMigrateData->getRepresentativeVariableBindingPathMap()->count();
					}
					return varCount;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
