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

#include "CBackendRepresentativeMemoryCacheTemporaryLabelReference.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CBackendRepresentativeMemoryCacheTemporaryLabelReference::CBackendRepresentativeMemoryCacheTemporaryLabelReference() : QPair<CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*, CBackendRepresentativeMemoryLabelCacheItem*>(nullptr, nullptr) {
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelReference::CBackendRepresentativeMemoryCacheTemporaryLabelReference(CBackendRepresentativeMemoryLabelCacheItem* referredLabelData) : QPair<CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*, CBackendRepresentativeMemoryLabelCacheItem*>(nullptr, referredLabelData) {
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelReference::CBackendRepresentativeMemoryCacheTemporaryLabelReference(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* referredTmpLabelData) : QPair<CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*, CBackendRepresentativeMemoryLabelCacheItem*>(referredTmpLabelData, nullptr) {
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelReference* CBackendRepresentativeMemoryCacheTemporaryLabelReference::initLabelReferenceData(CBackendRepresentativeMemoryCacheTemporaryLabelReference* labelReferenceData) {
					first = nullptr;
					second = nullptr;
					if (labelReferenceData) {
						*this = *labelReferenceData;
					}
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelReference* CBackendRepresentativeMemoryCacheTemporaryLabelReference::setReferredTemporaryLabelData(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* referredTmpLabelData) {
					first = referredTmpLabelData;
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryLabelReference* CBackendRepresentativeMemoryCacheTemporaryLabelReference::setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem* referredLabelData) {
					second = referredLabelData;
					return this;
				}




				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryLabelReference::getReferredTemporaryLabelData() {
					return first;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheTemporaryLabelReference::getReferredLabelData() {
					return second;
				}




			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
