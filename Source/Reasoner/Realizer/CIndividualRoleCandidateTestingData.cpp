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

#include "CIndividualRoleCandidateTestingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {

			CIndividualRoleCandidateTestingData::CIndividualRoleCandidateTestingData() {
				mMarkerConcept = nullptr;
				mPropagationConcept = nullptr;
				mIndiComplexData = nullptr;
				mRoleInstancesItem = nullptr;
				mInversed = false;
			}

			CIndividualRoleCandidateTestingData::CIndividualRoleCandidateTestingData(CConcept* propagationConcept, CConcept* markerConcept, COptimizedKPSetIndividualComplexRoleData* indiComplexData, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed) {
				mMarkerConcept = markerConcept;
				mPropagationConcept = propagationConcept;
				mIndiComplexData = indiComplexData;
				mRoleInstancesItem = roleInstancesItem;
				mInversed = inversed;
			}


			CConcept* CIndividualRoleCandidateTestingData::getMarkerConcept() {
				return mMarkerConcept;
			}

			CConcept* CIndividualRoleCandidateTestingData::getPropagationConcept() {
				return mPropagationConcept;
			}

			COptimizedKPSetIndividualComplexRoleData* CIndividualRoleCandidateTestingData::getIndividualComplexData() {
				return mIndiComplexData;
			}

			COptimizedKPSetRoleInstancesItem* CIndividualRoleCandidateTestingData::getRoleInstancesItem() {
				return mRoleInstancesItem;
			}

			bool CIndividualRoleCandidateTestingData::getInversed() {
				return mInversed;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
