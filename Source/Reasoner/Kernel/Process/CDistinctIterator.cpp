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

#include "CDistinctIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {



				CDistinctIterator::CDistinctIterator() {
				}


				CDistinctIterator::CDistinctIterator(const CPROCESSHASH<cint64,CDistinctEdge*>::iterator& beginIt, const CPROCESSHASH<cint64,CDistinctEdge*>::iterator& endIt)
						: mBeginIt(beginIt),mEndIt(endIt) {
				}



				bool CDistinctIterator::hasNext() {
					return mBeginIt != mEndIt;
				}

				cint64 CDistinctIterator::nextDistinctIndividualID(bool moveNext) {
					cint64 indi = 0;
					if (mBeginIt != mEndIt) {
						indi = mBeginIt.key();
						if (moveNext) {
							++mBeginIt;
						}
					}
					return indi;
				}


				cint64 CDistinctIterator::nextDistinctIndividualID(CDependencyTrackPoint*& depTrackPoint, bool moveNext) {
					cint64 indi = 0;
					if (mBeginIt != mEndIt) {
						indi = mBeginIt.key();
						depTrackPoint = mBeginIt.value()->getDependencyTrackPoint();
						if (moveNext) {
							++mBeginIt;
						}
					}
					return indi;
				}


				CDistinctEdge* CDistinctIterator::next(bool moveNext) {
					CDistinctEdge* disEdge = nullptr;
					if (mBeginIt != mEndIt) {
						disEdge = mBeginIt.value();
						if (moveNext) {
							++mBeginIt;
						}
					}
					return disEdge;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
