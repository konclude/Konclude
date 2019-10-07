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

#include "CIndividualProcessData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CIndividualProcessData::CIndividualProcessData() {
			}

			CIndividualProcessData* CIndividualProcessData::initIndividualProcessExtensionData() {
				mComputedConsequencesCachingData = nullptr;
				mSatRefLinkingData = nullptr;
				mBackendCachingData = nullptr;
				return this;
			}

			CComputedConsequencesCachingData* CIndividualProcessData::getComputedConsequencesCachingData() {
				return mComputedConsequencesCachingData;
			}

			CIndividualProcessData* CIndividualProcessData::setComputedConsequencesCachingData(CComputedConsequencesCachingData* computedConsequencesCachingData) {
				mComputedConsequencesCachingData = computedConsequencesCachingData;
				return this;
			}


			CIndividualSaturationReferenceLinkingData* CIndividualProcessData::getSaturationReferenceLinkingData() {
				return mSatRefLinkingData;
			}

			CIndividualProcessData* CIndividualProcessData::setSaturationReferenceLinkingData(CIndividualSaturationReferenceLinkingData* refLinkingData) {
				mSatRefLinkingData = refLinkingData;
				return this;
			}

			CIndividualBackendCachingData* CIndividualProcessData::getBackendCachingData() {
				return mBackendCachingData;
			}

			CIndividualProcessData* CIndividualProcessData::setBackendCachingData(CIndividualBackendCachingData* backendCachingData) {
				mBackendCachingData = backendCachingData;
				return this;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
