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

#include "CBackendRepresentativeMemoryCacheIndividualAssociationContext.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheIndividualAssociationContext::CBackendRepresentativeMemoryCacheIndividualAssociationContext(CBackendRepresentativeMemoryCacheContext* cacheContext) 
					: mCacheContext(cacheContext), 
					mMemoryPoolProvider(cacheContext->getMemoryPoolProvider()),
					mMemMan(&mMemPoolContainer, cacheContext->getMemoryPoolProvider()) {

					mFirstRecomputationReferenceLinker = nullptr;
					mLastRecomputationReferenceLinker = nullptr;

					mIndividualAssociationDataUsageCount = 0;
					mPreviousMemoryManagementCount = 0;
					setData(this);
				}




				CMemoryAllocationManager* CBackendRepresentativeMemoryCacheIndividualAssociationContext::getMemoryAllocationManager() {
					return &mMemMan;
				}

				CMemoryPoolProvider* CBackendRepresentativeMemoryCacheIndividualAssociationContext::getMemoryPoolProvider() {
					return mMemoryPoolProvider;
				}

				CMemoryPoolContainer* CBackendRepresentativeMemoryCacheIndividualAssociationContext::getMemoryPoolContainer() {
					return &mMemPoolContainer;
				}

				CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* CBackendRepresentativeMemoryCacheIndividualAssociationContext::getLastRecomputationReferenceLinker() {
					return mLastRecomputationReferenceLinker;
				}

				CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* CBackendRepresentativeMemoryCacheIndividualAssociationContext::getFirstRecomputationReferenceLinker() {
					return mFirstRecomputationReferenceLinker;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationContext* CBackendRepresentativeMemoryCacheIndividualAssociationContext::setLastRecomputationReferenceLinker(CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* recomputationReferenceLinker) {
					mLastRecomputationReferenceLinker = recomputationReferenceLinker;
					return this;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationContext* CBackendRepresentativeMemoryCacheIndividualAssociationContext::setFirstRecomputationReferenceLinker(CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* recomputationReferenceLinker) {
					mFirstRecomputationReferenceLinker = recomputationReferenceLinker;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheIndividualAssociationContext::getIndividualAssociationDataUsageCount() {
					return mIndividualAssociationDataUsageCount;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationContext* CBackendRepresentativeMemoryCacheIndividualAssociationContext::incIndividualAssociationDataUsageCount(cint64 count) {
					mIndividualAssociationDataUsageCount += count;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheIndividualAssociationContext::getPreviousMemoryManagementCount() {
					return mPreviousMemoryManagementCount;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationContext* CBackendRepresentativeMemoryCacheIndividualAssociationContext::setPreviousMemoryManagementCount(cint64 previousCount) {
					mPreviousMemoryManagementCount = previousCount;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
