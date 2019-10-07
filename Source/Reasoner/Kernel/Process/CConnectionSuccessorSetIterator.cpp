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

#include "CConnectionSuccessorSetIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CConnectionSuccessorSetIterator::CConnectionSuccessorSetIterator(cint64 connID) {
					mConnID = connID;
				}


				CConnectionSuccessorSetIterator::CConnectionSuccessorSetIterator(const CPROCESSSET<cint64>::iterator& beginIt, const CPROCESSSET<cint64>::iterator& endIt)
						: mBeginIt(beginIt),mEndIt(endIt) {
					mConnID = CINT64_MIN;
				}



				bool CConnectionSuccessorSetIterator::hasNext() {
					return mConnID != CINT64_MIN || mBeginIt != mEndIt;
				}

				cint64 CConnectionSuccessorSetIterator::nextSuccessorConnectionID(bool moveNext) {
					cint64 indiID = 0;
					if (mConnID != CINT64_MIN) {
						indiID = mConnID;
						if (moveNext) {
							mConnID = CINT64_MIN;
						}
					}
					if (mBeginIt != mEndIt) {
						indiID = *mBeginIt;
						if (moveNext) {
							++mBeginIt;
						}
					}
					return indiID;
				}


				cint64 CConnectionSuccessorSetIterator::next(bool moveNext) {
					return nextSuccessorConnectionID(moveNext);
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
