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

#include "CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker() {
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::initLabelWriteData(cint64 signature, cint64 labelType) {
					initCachingStatusFlags();
					mLabelType = labelType;
					mDetValueCount = 0;
					mSignature = signature;
					mDetValueLinker = nullptr;
					mTmpData = nullptr;
					mStatusFlags = 0;
					return this;
				}



				cint64 CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::getSignature() {
					return mSignature;
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::setSignature(cint64 signature) {
					mSignature = signature;
					return this;
				}


				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::appendCacheValueLinker(CBackendRepresentativeMemoryLabelValueLinker* linker) {
					CBackendRepresentativeMemoryLabelValueLinker* linkerIt = linker;
					while (linkerIt) {
						++mDetValueCount;
						linkerIt = linkerIt->getNext();
					}
					mDetValueLinker = linker->append(mDetValueLinker);
					return this;
				}


				CBackendRepresentativeMemoryLabelValueLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::getCacheValueLinker() {
					return mDetValueLinker;
				}


				cint64 CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::getCacheValueCount() {
					return mDetValueCount;
				}


				void* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::getTemporaryData() {
					return mTmpData;
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::setTemporaryData(void* tmpData) {
					mTmpData = tmpData;
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::clearTemporaryData() {
					mTmpData = nullptr;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::getLabelType() {
					return mLabelType;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
