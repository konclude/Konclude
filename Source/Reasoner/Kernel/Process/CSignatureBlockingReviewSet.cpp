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

#include "CSignatureBlockingReviewSet.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSignatureBlockingReviewSet::CSignatureBlockingReviewSet(CProcessContext* context) : mSubsetReviews(context),mNonSubsetReviews(context) {
					mContext = context;
				}


				CSignatureBlockingReviewSet* CSignatureBlockingReviewSet::initSignatureBlockingReviewSet(CSignatureBlockingReviewSet* sigBlockRevSet) {
					if (sigBlockRevSet) {
						mSubsetReviews.initSignatureBlockingReviewData(&sigBlockRevSet->mSubsetReviews);
						mNonSubsetReviews.initSignatureBlockingReviewData(&sigBlockRevSet->mNonSubsetReviews);
					} else {
						mSubsetReviews.initSignatureBlockingReviewData(nullptr);
						mNonSubsetReviews.initSignatureBlockingReviewData(nullptr);
					}
					return this;
				}

				CSignatureBlockingReviewData* CSignatureBlockingReviewSet::getSubsetReviewData() {
					return &mSubsetReviews;
				}

				CSignatureBlockingReviewData* CSignatureBlockingReviewSet::getReviewData(bool subset) {
					if (subset) {
						return &mSubsetReviews;
					} else {
						return &mNonSubsetReviews;
					}
				}


				CSignatureBlockingReviewData* CSignatureBlockingReviewSet::getNonSubsetReviewData() {
					return &mNonSubsetReviews;
				}

				bool CSignatureBlockingReviewSet::isEmpty() {
					return mSubsetReviews.isEmpty() && mNonSubsetReviews.isEmpty();
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
