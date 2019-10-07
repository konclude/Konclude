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

#include "CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData::CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData() {
					mIndiIdLinker = nullptr;
					mCount = 0;
				}


				bool CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData::visitNeighbourIndividualIds(function<bool(cint64 id)> visitFunc) {
					bool continueVisiting = true;
					bool visited = false;
					for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinkerIt = mIndiIdLinker; continueVisiting && indiIdLinkerIt; indiIdLinkerIt = indiIdLinkerIt->getNext()) {
						cint64 indiId = indiIdLinkerIt->getIndividualId();
						continueVisiting &= visitFunc(indiId);
						visited = true;
					}
					return visited;
				}

				bool CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData::visitNeighbourIndividualIdsFromCursor(function<bool(cint64 id, cint64 cursor)> visitFunc, cint64 cursor) {
					bool continueVisiting = true;
					bool visited = false;
					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinkerIt = (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker*)cursor;
					if (!indiIdLinkerIt) {
						indiIdLinkerIt = mIndiIdLinker;
					} else {
						indiIdLinkerIt = indiIdLinkerIt->getNext();
					}
					while (continueVisiting && indiIdLinkerIt) {
						cint64 indiId = indiIdLinkerIt->getIndividualId();
						continueVisiting &= visitFunc(indiId, (cint64)indiIdLinkerIt);
						indiIdLinkerIt = indiIdLinkerIt->getNext();
						visited = true;
					}
					return visited;
				}

				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData::getIndividualIdLinker() {
					return mIndiIdLinker;
				}

				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData* CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData::setIndividualIdLinker(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinker) {
					mIndiIdLinker = indiIdLinker;
					mCount = mIndiIdLinker->getCount();
					return this;
				}


				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData* CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData::addIndividualIdLinker(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinker) {
					mCount = indiIdLinker->getCount();
					mIndiIdLinker = indiIdLinker->append(mIndiIdLinker);
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData::getIndividualCount() {
					return mCount;
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
