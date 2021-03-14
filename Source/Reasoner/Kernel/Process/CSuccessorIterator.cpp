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

#include "CSuccessorIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CSuccessorIterator::CSuccessorIterator() {
					mLastSucc = CINT64_MIN;
					mIterator1 = false;
					mIterator2 = false;
				}


				CSuccessorIterator::CSuccessorIterator(const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& beginIt, const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& endIt)
						: mBeginIt1(beginIt),mEndIt1(endIt) {

					mLastSucc = CINT64_MIN;
					mIterator1 = true;
					mIterator2 = false;
					while (mBeginIt1 != mEndIt1 && mBeginIt1.value() == nullptr) {
						++mBeginIt1;
					}
				}

				
				CSuccessorIterator::CSuccessorIterator(const CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& beginIt1, const CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& endIt1, const CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& beginIt2, const CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& endIt2, const CPROCESSHASH<cint64, CIndividualLinkEdge*>* it2ValHash)
					: mBeginIt1(beginIt1), mEndIt1(endIt1), mBeginIt2(beginIt2), mEndIt2(endIt2) {

					mLastSucc = CINT64_MIN;
					mIterator1 = true;
					mIterator2 = true;
					while (mBeginIt1 != mEndIt1 && mBeginIt1.value() == nullptr) {
						++mBeginIt1;
					}
					mIt2ValHash = it2ValHash;
					while (mBeginIt2 != mEndIt2 && mBeginIt2.value() == nullptr && (!mIt2ValHash || mIt2ValHash->contains(mBeginIt2.key()))) {
						++mBeginIt2;
					}
				}


				bool CSuccessorIterator::hasNext() {
					return mIterator1 && mBeginIt1 != mEndIt1 || mIterator2 && mBeginIt2 != mEndIt2;
				}

				CIndividualLinkEdge* CSuccessorIterator::nextLink(bool moveNext) {
					CIndividualLinkEdge* link = nullptr;
					cint64 indiID = 0;
					if (mIterator1 && mBeginIt1 != mEndIt1) {
						iterate(link, indiID, moveNext, mBeginIt1, mEndIt1, nullptr);
					} else if (mIterator2 && mBeginIt2 != mEndIt2) {
						iterate(link, indiID, moveNext, mBeginIt2, mEndIt2, mIt2ValHash);
					}
					return link;
				}


				cint64 CSuccessorIterator::nextIndividualID(bool moveNext) {
					CIndividualLinkEdge* link = nullptr;
					cint64 indiID = 0;
					if (mIterator1 && mBeginIt1 != mEndIt1) {
						iterate(link, indiID, moveNext, mBeginIt1, mEndIt1, nullptr);
					} else if (mIterator2 && mBeginIt2 != mEndIt2) {
						iterate(link, indiID, moveNext, mBeginIt2, mEndIt2, mIt2ValHash);
					}
					return indiID;
				}

				bool CSuccessorIterator::iterate(CIndividualLinkEdge*& link, cint64& indiId, bool moveNext, CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& beginIt, CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& endIt, const CPROCESSHASH<cint64, CIndividualLinkEdge*>* itValHash) {
					link = beginIt.value();
					indiId = beginIt.key();
					mLastSucc = beginIt.key();
					if (moveNext) {
						++beginIt;
						while (beginIt != endIt && (beginIt.value() == nullptr || mLastSucc == beginIt.key()) && (!itValHash || itValHash->contains(beginIt.key()))) {
							++beginIt;
						}
					}						
					return true;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
