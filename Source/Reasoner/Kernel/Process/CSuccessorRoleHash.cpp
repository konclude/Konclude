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

#include "CSuccessorRoleHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSuccessorRoleHash::CSuccessorRoleHash(CProcessContext* context) : mContext(context) {
					mSuccessorLinkHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64, CIndividualLinkEdge*>, CProcessContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(), mContext);
					mPrevSuccessorLinkHash = nullptr;
					mPrevValidatingRequired = false;
				}


				CSuccessorRoleHash* CSuccessorRoleHash::initSuccessorRoleHash(CSuccessorRoleHash* prevRoleSuccHash) {
					if (prevRoleSuccHash) {

						if (!prevRoleSuccHash->mPrevSuccessorLinkHash && prevRoleSuccHash->mSuccessorLinkHash->size() <= 100) {
							*mSuccessorLinkHash = *prevRoleSuccHash->mSuccessorLinkHash;
							mPrevSuccessorLinkHash = nullptr;
							mPrevValidatingRequired = false;
						} else if (!prevRoleSuccHash->mPrevSuccessorLinkHash && prevRoleSuccHash->mSuccessorLinkHash->size() > 100) {
							mSuccessorLinkHash->clear();
							mPrevSuccessorLinkHash = prevRoleSuccHash->mSuccessorLinkHash;
							mPrevValidatingRequired = false;
						} else {

							if (prevRoleSuccHash->mSuccessorLinkHash->size() * 10 > prevRoleSuccHash->mPrevSuccessorLinkHash->size()) {
								*mSuccessorLinkHash = *prevRoleSuccHash->mSuccessorLinkHash;

								for (CPROCESSHASH<cint64, CIndividualLinkEdge*>::const_iterator it = prevRoleSuccHash->mPrevSuccessorLinkHash->constBegin(), itEnd = prevRoleSuccHash->mPrevSuccessorLinkHash->constEnd(); it != itEnd; ++it) {
									cint64 neighbourId = it.key();
									if (!mPrevValidatingRequired || !prevRoleSuccHash->mSuccessorLinkHash->contains(neighbourId)) {
										CIndividualLinkEdge* link = it.value();
										mSuccessorLinkHash->insertMulti(neighbourId, link);
									}
								}
								mPrevSuccessorLinkHash = nullptr;
								mPrevValidatingRequired = false;
							} else {
								*mSuccessorLinkHash = *prevRoleSuccHash->mSuccessorLinkHash;
								mPrevSuccessorLinkHash = prevRoleSuccHash->mPrevSuccessorLinkHash;
								mPrevValidatingRequired = false;
							}

						}

					} else {
						mPrevValidatingRequired = false;
						mSuccessorLinkHash->clear();
						mPrevSuccessorLinkHash = nullptr;
					}
					return this;
				}

				CSuccessorRoleHash* CSuccessorRoleHash::insertSuccessorRoleLink(cint64 indi, CIndividualLinkEdge* link) {

					if (mPrevSuccessorLinkHash && mPrevSuccessorLinkHash->contains(indi) && !mSuccessorLinkHash->contains(indi)) {
						for (CPROCESSHASH<cint64, CIndividualLinkEdge*>::const_iterator it = mPrevSuccessorLinkHash->find(indi), itEnd = mPrevSuccessorLinkHash->constEnd(); it != itEnd; ++it) {
							cint64 indiId = it.key();
							if (indiId != indi) {
								break;
							}
							CIndividualLinkEdge* link = it.value();
							mSuccessorLinkHash->insertMulti(indiId, link);
							mPrevValidatingRequired = true;
						}
					}

					mSuccessorLinkHash->insertMulti(indi,link);
					return this;
				}

				CSuccessorRoleIterator CSuccessorRoleHash::getSuccessorRoleIterator(cint64 indi) {
					if (!mPrevSuccessorLinkHash || mSuccessorLinkHash->contains(indi)) {
						return CSuccessorRoleIterator(indi, mSuccessorLinkHash->find(indi), mSuccessorLinkHash->end());
					} else if (mPrevSuccessorLinkHash && mPrevSuccessorLinkHash->contains(indi)) {
						return CSuccessorRoleIterator(indi, mPrevSuccessorLinkHash->find(indi), mPrevSuccessorLinkHash->end());
					} else {
						return CSuccessorRoleIterator(indi, mSuccessorLinkHash->end(), mSuccessorLinkHash->end());
					}
				}

				bool CSuccessorRoleHash::hasSuccessorIndividualNode(cint64 indi) {
					if (mSuccessorLinkHash->contains(indi)) {
						return true;
					}
					if (mPrevSuccessorLinkHash && mPrevSuccessorLinkHash->contains(indi)) {
						return true;
					}
					return false;
				}

				CSuccessorRoleHash* CSuccessorRoleHash::removeSuccessor(cint64 indi) {

					if (mPrevSuccessorLinkHash && mPrevSuccessorLinkHash->contains(indi)) {
						for (CPROCESSHASH<cint64, CIndividualLinkEdge*>::const_iterator it = mPrevSuccessorLinkHash->constBegin(), itEnd = mPrevSuccessorLinkHash->constEnd(); it != itEnd; ) {
							cint64 indiId = it.key();
							if (!mPrevValidatingRequired || !mSuccessorLinkHash->contains(indiId)) {
								do {
									CIndividualLinkEdge* link = it.value();
									mSuccessorLinkHash->insertMulti(indiId, link);
									++it;
								} while (it != itEnd && it.key() == indiId);
							} else {
								++it;
							}
						}
						mPrevSuccessorLinkHash = nullptr;
					}

					mSuccessorLinkHash->remove(indi);
					return this;
				}

				CSuccessorIterator CSuccessorRoleHash::getSuccessorIterator() {
					if (mPrevSuccessorLinkHash) {
						if (mPrevValidatingRequired) {
							return CSuccessorIterator(mSuccessorLinkHash->begin(), mSuccessorLinkHash->end(), mPrevSuccessorLinkHash->begin(), mPrevSuccessorLinkHash->end(), mSuccessorLinkHash);
						} else {
							return CSuccessorIterator(mSuccessorLinkHash->begin(), mSuccessorLinkHash->end(), mPrevSuccessorLinkHash->begin(), mPrevSuccessorLinkHash->end());
						}
					} else {
						return CSuccessorIterator(mSuccessorLinkHash->begin(), mSuccessorLinkHash->end());
					}
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
