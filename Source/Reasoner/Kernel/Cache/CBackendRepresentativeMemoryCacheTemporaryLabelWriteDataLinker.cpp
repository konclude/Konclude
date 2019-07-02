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

#include "CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker() {
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::initLabelWriteData(cint64 signature) {
					mDetValueCount = 0;
					mSignature = signature;
					mDetValueLinker = nullptr;
					mCompletelyHandled = false;
					mCompletelySaturated = false;
					mTmpData = nullptr;
					return this;
				}



				cint64 CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::getSignature() {
					return mSignature;
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::setSignature(cint64 signature) {
					mSignature = signature;
					return this;
				}


				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::appendDeterministicCacheValueLinker(CBackendRepresentativeMemoryLabelValueLinker* linker) {
					CBackendRepresentativeMemoryLabelValueLinker* linkerIt = linker;
					while (linkerIt) {
						++mDetValueCount;
						linkerIt = linkerIt->getNext();
					}
					mDetValueLinker = linker->append(mDetValueLinker);
					return this;
				}


				CBackendRepresentativeMemoryLabelValueLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::getDeterministicCacheValueLinker() {
					return mDetValueLinker;
				}



				cint64 CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::getDeterministicCacheValueCount() {
					return mDetValueCount;
				}

				bool CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::isCompletelyHandled() {
					return mCompletelyHandled;
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::setCompletelyHandled(bool completelyHandled) {
					mCompletelyHandled = completelyHandled;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::isCompletelySaturated() {
					return mCompletelySaturated;
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker::setCompletelySaturated(bool completelySaturated) {
					mCompletelySaturated = completelySaturated;
					return this;
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


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
