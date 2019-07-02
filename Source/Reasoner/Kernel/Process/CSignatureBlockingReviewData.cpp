/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CSignatureBlockingReviewData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSignatureBlockingReviewData::CSignatureBlockingReviewData(CProcessContext* context) : mIndividualSet(context),mDepthIndividualMap(context) {
					mContext = context;
				}


				CSignatureBlockingReviewData* CSignatureBlockingReviewData::initSignatureBlockingReviewData(CSignatureBlockingReviewData* sigBlockRevData) {
					if (sigBlockRevData) {
						mIndividualSet = sigBlockRevData->mIndividualSet;
						mDepthIndividualMap = sigBlockRevData->mDepthIndividualMap;
					} else {
						mIndividualSet.clear();
						mDepthIndividualMap.clear();
					}
					return this;
				}

				CSignatureBlockingReviewData* CSignatureBlockingReviewData::insert(cint64 depth, cint64 indiID) {
					if (!mIndividualSet.contains(indiID)) {
						mIndividualSet.insert(indiID);
						mDepthIndividualMap.insertMulti(depth,indiID);
					}
					return this;
				}


				bool CSignatureBlockingReviewData::contains(cint64 indiID) {
					return mIndividualSet.contains(indiID);
				}

				CSignatureBlockingReviewData* CSignatureBlockingReviewData::remove(cint64 indiID) {
					mIndividualSet.remove(indiID);
					return this;
				}

				bool CSignatureBlockingReviewData::isEmpty() {
					return mIndividualSet.isEmpty();
				}

				CSignatureBlockingReviewDataIterator CSignatureBlockingReviewData::getIterator() {
					return CSignatureBlockingReviewDataIterator(&mIndividualSet,&mDepthIndividualMap);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
