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

#include "CSuccessorRoleIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CSuccessorRoleIterator::CSuccessorRoleIterator() {
				}

				CSuccessorRoleIterator::CSuccessorRoleIterator(cint64 indi, const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& beginIt, const CPROCESSHASH<cint64,CIndividualLinkEdge*>::iterator& endIt)
						: mIndi(indi),mBeginIt(beginIt),mEndIt(endIt) {
				}



				bool CSuccessorRoleIterator::hasNext() {
					return mBeginIt != mEndIt && mBeginIt.key() == mIndi && mBeginIt.value() != nullptr;
				}

				CIndividualLinkEdge* CSuccessorRoleIterator::next(bool moveNext) {
					CIndividualLinkEdge* link = nullptr;
					if (mBeginIt != mEndIt && mBeginIt.key() == mIndi) {
						link = mBeginIt.value();
						if (moveNext) {
							++mBeginIt;
						}
					}
					return link;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
