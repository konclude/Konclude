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

#include "CConnectionSuccessorSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConnectionSuccessorSet::CConnectionSuccessorSet(CProcessContext* context) : mContext(context) {
					mAncConnID = CINT64_MIN;
					mConnSet = nullptr;
					mPrevConnSet = nullptr;
				}

				CConnectionSuccessorSet* CConnectionSuccessorSet::initConnectionSuccessorSet(CConnectionSuccessorSet* connSuccSet) {
					if (connSuccSet) {
						mAncConnID = connSuccSet->mAncConnID;
						if (connSuccSet->mConnSet) {

							if (!mConnSet) {
								mConnSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>, CContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(), mContext);
							} else {
								mConnSet->clear();
							}


							if (!connSuccSet->mPrevConnSet && connSuccSet->mConnSet->size() <= 100) {
								*mConnSet = *connSuccSet->mConnSet;
								mPrevConnSet = nullptr;
							} else if (!connSuccSet->mPrevConnSet && connSuccSet->mConnSet->size() > 100) {
								mPrevConnSet = connSuccSet->mConnSet;
							} else {
								if (connSuccSet->mConnSet->size() * 10 > connSuccSet->mPrevConnSet->size()) {
									*mConnSet = *connSuccSet->mPrevConnSet;
									for (CPROCESSSET<cint64>::const_iterator it = connSuccSet->mConnSet->constBegin(), itEnd = connSuccSet->mConnSet->constEnd(); it != itEnd; ++it) {
										cint64 indiId = *it;
										mConnSet->insert(indiId);
									}
									mPrevConnSet = nullptr;
								} else {
									*mConnSet = *connSuccSet->mConnSet;
									mPrevConnSet = connSuccSet->mPrevConnSet;
								}

							}

						}
					} else {
						mAncConnID = CINT64_MIN;
						if (mConnSet) {
							mConnSet->clear();
						}
						mPrevConnSet = nullptr;
					}
					return this;
				}



				bool CConnectionSuccessorSet::hasConnectionSuccessor(cint64 indiID) {
					if (mPrevConnSet && mPrevConnSet->contains(indiID)) {
						return true;
					}
					if (mConnSet) {
						return mConnSet->contains(indiID);
					} 
					return mAncConnID == indiID;
				}


				CConnectionSuccessorSet* CConnectionSuccessorSet::insertConnectionSuccessor(cint64 indiID) {
					if (mAncConnID != indiID) {
						if (mAncConnID != CINT64_MIN) {
							if (!mConnSet) {
								mConnSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>,CContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(),mContext);
								mConnSet->insert(mAncConnID);
							}
							if (!mPrevConnSet || !mPrevConnSet->contains(indiID)) {
								mConnSet->insert(indiID);
							}
						} else {
							mAncConnID = indiID;
						}
					}
					return this;
				}


				CConnectionSuccessorSet* CConnectionSuccessorSet::removeConnection(cint64 indiID) {
					if (mPrevConnSet) {
						if (mPrevConnSet->contains(indiID)) {
							for (CPROCESSSET<cint64>::const_iterator it = mPrevConnSet->constBegin(), itEnd = mPrevConnSet->constEnd(); it != itEnd; ++it) {
								cint64 indiId = *it;
								mConnSet->insert(indiId);
							}
							mPrevConnSet = nullptr;
						}
					}
					if (mConnSet) {
						mConnSet->remove(indiID);
					} else if (mAncConnID == indiID) {
						mAncConnID = CINT64_MIN;
					}
					return this;
				}


				CConnectionSuccessorSetIterator CConnectionSuccessorSet::getConnectionSuccessorIterator() {
					if (mPrevConnSet) {
						return CConnectionSuccessorSetIterator(mConnSet->begin(), mConnSet->end(), mPrevConnSet->begin(), mPrevConnSet->end());
					} else if (mConnSet) {
						return CConnectionSuccessorSetIterator(mConnSet->begin(), mConnSet->end());
					} else {
						return CConnectionSuccessorSetIterator(mAncConnID);
					}
				}


				cint64 CConnectionSuccessorSet::getConnectionSuccessorCount() {
					if (!mConnSet) {
						if(mAncConnID != CINT64_MIN) {
							return 1;
						} else {
							return 0;
						}
					} else if (mPrevConnSet) {
						return mConnSet->count() + mPrevConnSet->count();
					} else {
						return mConnSet->count();
					}
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
