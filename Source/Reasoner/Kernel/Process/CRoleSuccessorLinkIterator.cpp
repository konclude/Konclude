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

#include "CRoleSuccessorLinkIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CRoleSuccessorLinkIterator::CRoleSuccessorLinkIterator()
					: mLinkLinker(nullptr),mLastLink(nullptr) {
					mIterator1 = false;
					mIterator2 = false;
				}

				CRoleSuccessorLinkIterator::CRoleSuccessorLinkIterator(const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& beginIt, const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& endIt)
						: mLinkLinker(nullptr),mBeginIt1(beginIt),mEndIt1(endIt),mLastLink(nullptr) {
					mIterator1 = true;
					mIterator2 = false;
				}

				CRoleSuccessorLinkIterator::CRoleSuccessorLinkIterator(const CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& beginIt1, const CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& endIt1, const CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& beginIt2, const CPROCESSHASH<cint64, CIndividualLinkEdge*>::iterator& endIt2) 
					: mLinkLinker(nullptr), mBeginIt1(beginIt1), mEndIt1(endIt1), mBeginIt2(beginIt2), mEndIt2(endIt2), mLastLink(nullptr) {
					mIterator1 = true;
					mIterator2 = true;
				}


				CRoleSuccessorLinkIterator::CRoleSuccessorLinkIterator(CIndividualLinkEdge* linkLinker, CIndividualLinkEdge* lastLink)
						: mLinkLinker(linkLinker),mLastLink(lastLink) {
					mIterator1 = false;
					mIterator2 = false;
				}


				bool CRoleSuccessorLinkIterator::hasNext() {
					return (mLinkLinker && mLinkLinker != mLastLink) || mIterator1 && mBeginIt1 != mEndIt1 || mIterator2 && mBeginIt2 != mEndIt2;
				}

				CIndividualLinkEdge* CRoleSuccessorLinkIterator::next(bool moveNext) {
					CIndividualLinkEdge* link = nullptr;
					if (mLinkLinker) {
						link = mLinkLinker;
						if (moveNext) {
							mLinkLinker = mLinkLinker->getNext();
						}
					} else {
						if (mIterator1 && mBeginIt1 != mEndIt1) {
							link = mBeginIt1.value();
							if (moveNext) {
								++mBeginIt1;
							}
						} else if (mIterator2 && mBeginIt2 != mEndIt2) {
							link = mBeginIt2.value();
							if (moveNext) {
								++mBeginIt2;
							}
						}
					}
					return link;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
