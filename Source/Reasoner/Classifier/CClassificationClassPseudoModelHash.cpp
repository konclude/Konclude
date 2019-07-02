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

#include "CClassificationClassPseudoModelHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassificationClassPseudoModelHash::CClassificationClassPseudoModelHash(CContext* context) : mPMDataHash(context),mContext(context) {
			}

			CClassificationClassPseudoModelHash* CClassificationClassPseudoModelHash::initPseudoModelHash(CClassificationClassPseudoModelHash* hash) {
				if (hash) {
					mPMDataHash = hash->mPMDataHash;
				} else {
					mPMDataHash.clear();
				}
				return this;
			}

			CClassificationClassPseudoModelData* CClassificationClassPseudoModelHash::getPseudoModelData(cint64 node, bool create) {
				CClassificationClassPseudoModelData* pmData = nullptr;
				if (create) {
					CClassificationClassPseudoModelHashData& data = mPMDataHash[node];
					if (!data.mLocData) {
						data.mLocData = CObjectParameterizingAllocator< CClassificationClassPseudoModelData,CContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(),mContext);
						data.mLocData->initPseudoModelData(data.mUseData);
						data.mUseData = data.mLocData;
					}
					pmData = data.mUseData;
				} else {
					const CClassificationClassPseudoModelHashData& data = mPMDataHash.value(node,CClassificationClassPseudoModelHashData());
					pmData = data.mUseData;
				}
				return pmData;
			}

			cint64 CClassificationClassPseudoModelHash::getCount() {
				return mPMDataHash.count();
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
