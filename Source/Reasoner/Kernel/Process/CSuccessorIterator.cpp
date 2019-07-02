/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CSuccessorIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CSuccessorIterator::CSuccessorIterator() {
					mLastSucc = -1;
				}


				CSuccessorIterator::CSuccessorIterator(const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& beginIt, const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& endIt)
						: mBeginIt(beginIt),mEndIt(endIt) {

					mLastSucc = -1;
					while (mBeginIt != mEndIt && mBeginIt.value() == nullptr) {
						++mBeginIt;
					}
				}



				bool CSuccessorIterator::hasNext() {
					return mBeginIt != mEndIt;
				}

				CIndividualLinkEdge* CSuccessorIterator::nextLink(bool moveNext) {
					CIndividualLinkEdge* link = nullptr;
					if (mBeginIt != mEndIt) {
						link = mBeginIt.value();
						mLastSucc = mBeginIt.key();
						if (moveNext) {
							++mBeginIt;
							while (mBeginIt != mEndIt && (mBeginIt.value() == nullptr || mLastSucc == mBeginIt.key())) {
								++mBeginIt;
							}
						}
					}
					return link;
				}


				cint64 CSuccessorIterator::nextIndividualID(bool moveNext) {
					cint64 indiID = 0;
					if (mBeginIt != mEndIt) {
						indiID = mBeginIt.key();
						mLastSucc = mBeginIt.key();
						if (moveNext) {
							++mBeginIt;
							while (mBeginIt != mEndIt && (mBeginIt.value() == nullptr || mLastSucc == mBeginIt.key())) {
								++mBeginIt;
							}
						}
					}
					return indiID;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
