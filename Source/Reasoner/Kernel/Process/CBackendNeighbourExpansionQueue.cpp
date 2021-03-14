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

#include "CBackendNeighbourExpansionQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CBackendNeighbourExpansionQueue::CBackendNeighbourExpansionQueue(CProcessContext* processContext) {
				}



				CBackendNeighbourExpansionQueue* CBackendNeighbourExpansionQueue::initBackendNeighbourExpansionQueue(CBackendNeighbourExpansionQueue* queue) {
					if (queue) {
						mCurrentQueueLinker = queue->mCurrentQueueLinker;
						mNextQueueLinker = queue->mNextQueueLinker;
						mHasPropagationCut = queue->mHasPropagationCut;
					} else {
						mHasPropagationCut = false;
						mCurrentQueueLinker = nullptr;
						mNextQueueLinker = nullptr;
					}
					return this;
				}

				bool CBackendNeighbourExpansionQueue::hasQueuedNeighbourExpansions() {
					return mCurrentQueueLinker || mNextQueueLinker;
				}

				CBackendNeighbourExpansionQueueDataLinker* CBackendNeighbourExpansionQueue::getNextNeighbourExpansionQueueDataLinker() {
					if (!mCurrentQueueLinker) {
						mCurrentQueueLinker = mNextQueueLinker;
						mNextQueueLinker = nullptr;
					}
					return mCurrentQueueLinker;
				}

				CBackendNeighbourExpansionQueueDataLinker* CBackendNeighbourExpansionQueue::takeNextNeighbourExpansionQueueDataLinker() {
					if (!mCurrentQueueLinker) {
						mCurrentQueueLinker = mNextQueueLinker;
						mNextQueueLinker = nullptr;
					}
					CBackendNeighbourExpansionQueueDataLinker* linker = mCurrentQueueLinker;
					if (mCurrentQueueLinker) {
						mCurrentQueueLinker = mCurrentQueueLinker->getNext();
					}
					return linker;
				}

				CBackendNeighbourExpansionQueue* CBackendNeighbourExpansionQueue::addNeighbourExpansionQueueDataLinker(CBackendNeighbourExpansionQueueDataLinker* linker, bool addPrioritzied) {
					if (addPrioritzied) {
						mCurrentQueueLinker = linker->append(mCurrentQueueLinker);
					} else {
						mNextQueueLinker = linker->append(mNextQueueLinker);
					}
					return this;
				}

				CBackendNeighbourExpansionQueueDataLinker* CBackendNeighbourExpansionQueue::findNeighbourExpansionQueueDataLinker(cint64 arrayIndex) {
					for (CBackendNeighbourExpansionQueueDataLinker* linkerIt = mCurrentQueueLinker; linkerIt; linkerIt = linkerIt->getNext()) {
						if (linkerIt->getNeighbourArrayId() == arrayIndex) {
							return linkerIt;
						}
					}
					for (CBackendNeighbourExpansionQueueDataLinker* linkerIt = mNextQueueLinker; linkerIt; linkerIt = linkerIt->getNext()) {
						if (linkerIt->getNeighbourArrayId() == arrayIndex) {
							return linkerIt;
						}
					}
					return nullptr;
				}


				bool CBackendNeighbourExpansionQueue::visitNeighbourExpansionQueueDataLinkers(function<bool(CBackendNeighbourExpansionQueueDataLinker* linker)> visitFunc) {
					bool visisted = false;
					bool continueVisiting = true;
					for (CBackendNeighbourExpansionQueueDataLinker* linkerIt = mCurrentQueueLinker; linkerIt && continueVisiting; linkerIt = linkerIt->getNext()) {
						continueVisiting = visitFunc(linkerIt);
						visisted = true;
					}
					for (CBackendNeighbourExpansionQueueDataLinker* linkerIt = mNextQueueLinker; linkerIt && continueVisiting; linkerIt = linkerIt->getNext()) {
						continueVisiting = visitFunc(linkerIt);
						visisted = true;
					}
					return visisted;
				}

				bool CBackendNeighbourExpansionQueue::hasCuttedPropagationCutPropagation() {
					return mHasPropagationCut;
				}

				CBackendNeighbourExpansionQueue* CBackendNeighbourExpansionQueue::setCuttedPropagationCutPropagation(bool hasPropagationCut) {
					mHasPropagationCut = hasPropagationCut;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
