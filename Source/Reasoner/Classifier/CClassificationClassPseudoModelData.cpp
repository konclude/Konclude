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

#include "CClassificationClassPseudoModelData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassificationClassPseudoModelData::CClassificationClassPseudoModelData(CContext* context) : mContext(context) {
				mLocConceptMap = nullptr;
				mUseConceptMap = nullptr;
				mLocRoleMap = nullptr;
				mUseRoleMap = nullptr;
				mValidConceptMap = false;
				mValidRoleMap = false;
			}


			CClassificationClassPseudoModelData* CClassificationClassPseudoModelData::initPseudoModelData(CClassificationClassPseudoModelData* data) {
				if (data) {
					mUseConceptMap = data->mUseConceptMap;
					mUseRoleMap = data->mUseRoleMap;
					mValidConceptMap = data->mValidConceptMap;
					mValidRoleMap = data->mValidRoleMap;
				}
				return this;
			}


			CClassificationClassPseudoModelConceptMap* CClassificationClassPseudoModelData::getPseudoModelConceptMap(bool create) {
				if (!mLocConceptMap && create) {
					mLocConceptMap = CObjectParameterizingAllocator< CClassificationClassPseudoModelConceptMap,CContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(),mContext);
					mLocConceptMap->initPseudoModelConceptMap(mUseConceptMap);
					mUseConceptMap = mLocConceptMap;
				}
				return mUseConceptMap;
			}


			CClassificationClassPseudoModelRoleMap* CClassificationClassPseudoModelData::getPseudoModelRoleMap(bool create) {
				if (!mLocRoleMap && create) {
					mLocRoleMap = CObjectParameterizingAllocator< CClassificationClassPseudoModelRoleMap,CContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(),mContext);
					mLocRoleMap->initPseudoModelRoleMap(mUseRoleMap);
					mUseRoleMap = mLocRoleMap;
				}
				return mUseRoleMap;
			}

			bool CClassificationClassPseudoModelData::hasValidRoleMap() {
				return mValidRoleMap;
			}

			bool CClassificationClassPseudoModelData::hasValidConceptMap() {
				return mValidConceptMap;
			}

			CClassificationClassPseudoModelData* CClassificationClassPseudoModelData::setValidRoleMap(bool valid) {
				mValidRoleMap = valid;
				return this;
			}

			CClassificationClassPseudoModelData* CClassificationClassPseudoModelData::setValidConceptMap(bool valid) {
				mValidConceptMap = valid;
				return this;
			}

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
