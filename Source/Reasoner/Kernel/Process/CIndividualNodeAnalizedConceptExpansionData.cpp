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

#include "CIndividualNodeAnalizedConceptExpansionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeAnalizedConceptExpansionData::CIndividualNodeAnalizedConceptExpansionData() {
				}


				CIndividualNodeAnalizedConceptExpansionData* CIndividualNodeAnalizedConceptExpansionData::initBlockingExplorationData(CIndividualNodeAnalizedConceptExpansionData* prevData) {
					if (prevData) {
						mLastConceptCount = prevData->mLastConceptCount;
						mLastConDes = prevData->mLastConDes;
						mRevAnaConExpLinker = prevData->mRevAnaConExpLinker;
						mMinValidConceptCountLimit = prevData->mMinValidConceptCountLimit;
						mExpCount = prevData->mExpCount;
						mLastConceptSignature = prevData->mLastConceptSignature;
						mInvalidBlocker = prevData->mInvalidBlocker;
						mNonDetExpansionLinker = prevData->mNonDetExpansionLinker;
					} else {
						mLastConceptCount = nullptr;
						mLastConDes = nullptr;
						mRevAnaConExpLinker = nullptr;
						mExpCount = 0;
						mMinValidConceptCountLimit = 0;
						mLastConceptSignature = 0;
						mInvalidBlocker = false;
						mNonDetExpansionLinker = nullptr;
					}
					return this;
				}


				CIndividualNodeAnalizedConceptExpansionData* CIndividualNodeAnalizedConceptExpansionData::addAnalizedConceptExpansionLinker(CAnalizedConceptExpansionLinker *linker) {
					mExpCount += linker->getCount();
					if (mRevAnaConExpLinker) {
						mRevAnaConExpLinker = linker->append(mRevAnaConExpLinker);
					} else {
						mRevAnaConExpLinker = linker;
					}
					return this;
				}


				CAnalizedConceptExpansionLinker* CIndividualNodeAnalizedConceptExpansionData::getReverseAnalizedConceptExpansionLinker() {
					return mRevAnaConExpLinker;
				}


				CConceptDescriptor* CIndividualNodeAnalizedConceptExpansionData::getLastConceptDescriptor() {
					return mLastConDes;
				}

				cint64 CIndividualNodeAnalizedConceptExpansionData::getLastConceptCount() {
					return mLastConceptCount;
				}

				cint64 CIndividualNodeAnalizedConceptExpansionData::getExpansionConceptCount() {
					return mExpCount;
				}

				cint64 CIndividualNodeAnalizedConceptExpansionData::getMinimalValidConceptCountLimit() {
					return mMinValidConceptCountLimit;
				}

				cint64 CIndividualNodeAnalizedConceptExpansionData::getLastConceptSignature() {
					return mLastConceptSignature;
				}

				bool CIndividualNodeAnalizedConceptExpansionData::isInvalidBlocker() {
					return mInvalidBlocker;
				}

				CIndividualNodeAnalizedConceptExpansionData* CIndividualNodeAnalizedConceptExpansionData::setLastConceptDescriptor(CConceptDescriptor* conDes) {
					mLastConDes = conDes;
					return this;
				}

				CIndividualNodeAnalizedConceptExpansionData* CIndividualNodeAnalizedConceptExpansionData::setLastConceptSignature(cint64 signature) {
					mLastConceptSignature = signature;
					return this;
				}

				CIndividualNodeAnalizedConceptExpansionData* CIndividualNodeAnalizedConceptExpansionData::setLastConceptCount(cint64 conCount) {
					mLastConceptCount = conCount;
					return this;
				}

				CIndividualNodeAnalizedConceptExpansionData* CIndividualNodeAnalizedConceptExpansionData::setMinimalValidConceptCountLimit(cint64 conCount) {
					mMinValidConceptCountLimit = conCount;
					return this;
				}

				CIndividualNodeAnalizedConceptExpansionData* CIndividualNodeAnalizedConceptExpansionData::setInvalidBlocker(bool invalid) {
					mInvalidBlocker = invalid;
					return this;
				}


				CXLinker<CConceptDescriptor*>* CIndividualNodeAnalizedConceptExpansionData::getAnalysedNonDeterministicConceptExpansionLinker() {
					return mNonDetExpansionLinker;
				}

				CIndividualNodeAnalizedConceptExpansionData* CIndividualNodeAnalizedConceptExpansionData::setAnalysedNonDeterministicConceptExpansionLinker(CXLinker<CConceptDescriptor*>* linker) {
					mNonDetExpansionLinker = linker;
					return this;
				}


				CIndividualNodeAnalizedConceptExpansionData* CIndividualNodeAnalizedConceptExpansionData::addAnalysedNonDeterministicConceptExpansionLinker(CXLinker<CConceptDescriptor*>* linker) {
					if (linker) {
						mNonDetExpansionLinker = linker->append(mNonDetExpansionLinker);
					}
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
