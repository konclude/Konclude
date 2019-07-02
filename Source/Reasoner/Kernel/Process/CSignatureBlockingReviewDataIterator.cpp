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

#include "CSignatureBlockingReviewDataIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSignatureBlockingReviewDataIterator::CSignatureBlockingReviewDataIterator(CPROCESSSET<cint64>* individualSet, CPROCESSMAP<cint64,cint64>* depthIndividualMap)
						: mIndividualSet(individualSet), mDepthIndividualMap(depthIndividualMap) {

					mDepthIndividualMapIt = mDepthIndividualMap->begin();
					mDepthIndividualMapItEnd = mDepthIndividualMap->end();

					while (mDepthIndividualMapIt != mDepthIndividualMapItEnd) {
						cint64 indiID = mDepthIndividualMapIt.value();
						if (mIndividualSet->contains(indiID)) {
							break;
						} else {
							mDepthIndividualMapIt = mDepthIndividualMap->erase(mDepthIndividualMapIt);
						}
					}
				}


				bool CSignatureBlockingReviewDataIterator::hasNext() {
					return mDepthIndividualMapIt != mDepthIndividualMapItEnd;
				}


				bool CSignatureBlockingReviewDataIterator::removeAndMoveNext() {
					if (mDepthIndividualMapIt != mDepthIndividualMapItEnd) {
						cint64 indiID = mDepthIndividualMapIt.value();
						mIndividualSet->remove(indiID);
						mDepthIndividualMapIt = mDepthIndividualMap->erase(mDepthIndividualMapIt);

						while (mDepthIndividualMapIt != mDepthIndividualMapItEnd) {
							cint64 indiID = mDepthIndividualMapIt.value();
							if (mIndividualSet->contains(indiID)) {
								break;
							} else {
								mDepthIndividualMapIt = mDepthIndividualMap->erase(mDepthIndividualMapIt);
							}
						}
						return true;
					}
					return false;
				}


				cint64 CSignatureBlockingReviewDataIterator::next(bool moveNext) {
					cint64 indiID = -1;
					if (mDepthIndividualMapIt != mDepthIndividualMapItEnd) {
						indiID = mDepthIndividualMapIt.value();
						if (moveNext) {
							while (mDepthIndividualMapIt != mDepthIndividualMapItEnd) {
								cint64 indiID = mDepthIndividualMapIt.value();
								if (mIndividualSet->contains(indiID)) {
									break;
								} else {
									mDepthIndividualMapIt = mDepthIndividualMap->erase(mDepthIndividualMapIt);
								}
							}
						}
					}
					return indiID;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
