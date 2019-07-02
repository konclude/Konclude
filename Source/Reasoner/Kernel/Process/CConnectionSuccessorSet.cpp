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

#include "CConnectionSuccessorSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConnectionSuccessorSet::CConnectionSuccessorSet(CProcessContext* context) : mContext(context) {
					mAncConnID = -1;
					mConnSet = nullptr;
				}

				CConnectionSuccessorSet::~CConnectionSuccessorSet() {
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
						mAncConnID = -1;
						if (mConnSet) {
							mConnSet->clear();
						}
					}
					return this;
				}


				CConnectionSuccessorSet* CConnectionSuccessorSet::insertConnectionSuccessor(cint64 indiID) {
					if (mAncConnID != indiID) {
						if (mAncConnID >= 0) {
							if (!mConnSet) {
								mConnSet = CObjectParameterizingAllocator< CPROCESSSET<cint64>,CContext* >::allocateAndConstructAndParameterize(mContext->getUsedMemoryAllocationManager(),mContext);
								mConnSet->insert(mAncConnID);
							}
							mConnSet->insert(indiID);
						}
						mAncConnID = indiID;
					}
					return this;
				}


				CConnectionSuccessorSet* CConnectionSuccessorSet::removeConnection(cint64 indiID) {
					if (mConnSet) {
						mConnSet->remove(indiID);
					} else if (mAncConnID == indiID) {
						mAncConnID = -1;
					}
					return this;
				}


				CConnectionSuccessorSetIterator CConnectionSuccessorSet::getConnectionSuccessorIterator() {
					if (mConnSet) {
						return CConnectionSuccessorSetIterator(mConnSet->begin(),mConnSet->end());
					} else {
						return CConnectionSuccessorSetIterator(mAncConnID);
					}
				}


				cint64 CConnectionSuccessorSet::getConnectionSuccessorCount() {
					if (!mConnSet) {
						return 1;
					} else {
						return mConnSet->count();
					}
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
