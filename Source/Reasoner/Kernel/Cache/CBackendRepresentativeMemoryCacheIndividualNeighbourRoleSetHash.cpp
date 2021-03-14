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

#include "CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash::CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash(CBackendRepresentativeMemoryCacheContext* context) {
					mContext = context;
					mNeighbourRoleSetLabelHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItem*>, CContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(), mContext);
					mPrevNeighbourRoleSetLabelHash = nullptr;
				}

				CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash::initNeighbourRoleSetHash(CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetHash, bool detach) {
					if (neighbourRoleSetHash) {
						if (detach) {

							if (neighbourRoleSetHash->mPrevNeighbourRoleSetLabelHash) {
								*mNeighbourRoleSetLabelHash = *neighbourRoleSetHash->mPrevNeighbourRoleSetLabelHash;
								mNeighbourRoleSetLabelHash->detach();
								for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItem*>::const_iterator it = neighbourRoleSetHash->mNeighbourRoleSetLabelHash->constBegin(), itEnd = neighbourRoleSetHash->mNeighbourRoleSetLabelHash->constEnd(); it != itEnd; ++it) {
									cint64 neighbourId = it.key();
									CBackendRepresentativeMemoryLabelCacheItem* item = it.value();
									mNeighbourRoleSetLabelHash->insert(neighbourId, item);
								}
							} else {
								*mNeighbourRoleSetLabelHash = *neighbourRoleSetHash->mNeighbourRoleSetLabelHash;
								mNeighbourRoleSetLabelHash->detach();
							}
							mPrevNeighbourRoleSetLabelHash = nullptr;

						} else {

							if (!neighbourRoleSetHash->mPrevNeighbourRoleSetLabelHash && neighbourRoleSetHash->mNeighbourRoleSetLabelHash->size() <= 20) {
								*mNeighbourRoleSetLabelHash = *neighbourRoleSetHash->mNeighbourRoleSetLabelHash;
								mPrevNeighbourRoleSetLabelHash = nullptr;
							} else if (!neighbourRoleSetHash->mPrevNeighbourRoleSetLabelHash && neighbourRoleSetHash->mNeighbourRoleSetLabelHash->size() > 20) {
								mPrevNeighbourRoleSetLabelHash = neighbourRoleSetHash->mNeighbourRoleSetLabelHash;
							} else {

								if (neighbourRoleSetHash->mNeighbourRoleSetLabelHash->size() > neighbourRoleSetHash->mPrevNeighbourRoleSetLabelHash->size()) {
									*mNeighbourRoleSetLabelHash = *neighbourRoleSetHash->mPrevNeighbourRoleSetLabelHash;

									for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItem*>::const_iterator it = neighbourRoleSetHash->mNeighbourRoleSetLabelHash->constBegin(), itEnd = neighbourRoleSetHash->mNeighbourRoleSetLabelHash->constEnd(); it != itEnd; ++it) {
										cint64 neighbourId = it.key();
										CBackendRepresentativeMemoryLabelCacheItem* item = it.value();
										mNeighbourRoleSetLabelHash->insert(neighbourId, item);
									}
									mPrevNeighbourRoleSetLabelHash = nullptr;
								} else {
									*mNeighbourRoleSetLabelHash = *neighbourRoleSetHash->mNeighbourRoleSetLabelHash;
									mPrevNeighbourRoleSetLabelHash = neighbourRoleSetHash->mPrevNeighbourRoleSetLabelHash;
								}

							}
						}
					}
					return this;
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash::getNeighbourRoleSetLabel(cint64 neighbourIndiId) {
					CBackendRepresentativeMemoryLabelCacheItem* item =  mNeighbourRoleSetLabelHash->value(neighbourIndiId);
					if (!item && mPrevNeighbourRoleSetLabelHash) {
						item = mPrevNeighbourRoleSetLabelHash->value(neighbourIndiId);
					}
					return item;
				}

				cint64 CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash::getNeighbourCount() {
					cint64 count = mNeighbourRoleSetLabelHash->size();
					if (mPrevNeighbourRoleSetLabelHash) {
						count += mPrevNeighbourRoleSetLabelHash->size();
					}
					return count;
				}


				CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash::setNeighbourRoleSetLabel(cint64 neighbourIndiId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel) {
					mNeighbourRoleSetLabelHash->insert(neighbourIndiId, neighbourRoleSetLabel);
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
