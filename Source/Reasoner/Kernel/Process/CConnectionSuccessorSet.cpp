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
				}

				CConnectionSuccessorSet* CConnectionSuccessorSet::initConnectionSuccessorSet(CConnectionSuccessorSet* connSuccSet) {
					if (connSuccSet) {
						mAncConnID = connSuccSet->mAncConnID;
						if (connSuccSet->mConnSet) {
							if (!mConnSet) {
								mConnSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>,CContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(),mContext);
							}
							*mConnSet = *connSuccSet->mConnSet;
						}
					} else {
						mAncConnID = CINT64_MIN;
						if (mConnSet) {
							mConnSet->clear();
						}
					}
					return this;
				}



				bool CConnectionSuccessorSet::hasConnectionSuccessor(cint64 indiID) {
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
							mConnSet->insert(indiID);
						} else {
							mAncConnID = indiID;
						}
					}
					return this;
				}


				CConnectionSuccessorSet* CConnectionSuccessorSet::removeConnection(cint64 indiID) {
					if (mConnSet) {
						mConnSet->remove(indiID);
					} else if (mAncConnID == indiID) {
						mAncConnID = CINT64_MIN;
					}
					return this;
				}


				CConnectionSuccessorSetIterator CConnectionSuccessorSet::getConnectionSuccessorIterator() {
					if (mConnSet) {
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
					} else {
						return mConnSet->count();
					}
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
