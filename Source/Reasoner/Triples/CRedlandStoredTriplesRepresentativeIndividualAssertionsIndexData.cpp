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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#include "CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData() {
			}


			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData(CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* individualAssIndData) {
				mRepConSet = individualAssIndData->mRepConSet;
				mOutgoingObjectRolesSet = individualAssIndData->mOutgoingObjectRolesSet;
				mIncomingObjectRolesSet = individualAssIndData->mIncomingObjectRolesSet;
				mOutgoingDataRolesSet = individualAssIndData->mOutgoingDataRolesSet;
				mDifferentFromAssertions = individualAssIndData->mDifferentFromAssertions;
				mSameAsAssertions = individualAssIndData->mSameAsAssertions;
			}


			CRedlandStoredTriplesRepresentativeTagTypeConceptsSet* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::getRepresentativeTypeConceptsSet() {
				return mRepConSet;
			}

			CRedlandStoredTriplesRepresentativeTagRolesSet* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::getRepresentativeOutgoingObjectRolesSet() {
				return mOutgoingObjectRolesSet;
			}

			CRedlandStoredTriplesRepresentativeTagRolesSet* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::getRepresentativeIncomingObjectRolesSet() {
				return mIncomingObjectRolesSet;
			}

			CRedlandStoredTriplesRepresentativeTagRolesSet* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::getRepresentativeOutgoingDataRolesSet() {
				return mOutgoingDataRolesSet;
			}

			bool CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::hasDifferentFromAssertions() {
				return mDifferentFromAssertions;
			}

			bool CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::hasSameAsAssertions() {
				return mSameAsAssertions;
			}


			bool CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::hasTypeConceptsAssertions() {
				return mRepConSet != nullptr && mRepConSet->getTagElementCount() > 0;
			}

			bool CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::hasOutgoingObjectRoleAssertions() {
				return mOutgoingObjectRolesSet != nullptr && mOutgoingObjectRolesSet->getTagElementCount() > 0;
			}

			bool CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::hasIncomingObjectRoleAssertions() {
				return mIncomingObjectRolesSet != nullptr && mIncomingObjectRolesSet->getTagElementCount() > 0;
			}

			bool CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::hasOutgoingDataRoleAssertions() {
				return mOutgoingDataRolesSet != nullptr && mOutgoingDataRolesSet->getTagElementCount() > 0;
			}


			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::setRepresentativeTypeConceptsSet(CRedlandStoredTriplesRepresentativeTagTypeConceptsSet* repConSet) {
				mRepConSet = repConSet;
				return this;
			}

			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::setRepresentativeOutgoingObjectRolesSet(CRedlandStoredTriplesRepresentativeTagRolesSet* repRoleSet) {
				mOutgoingObjectRolesSet = repRoleSet;
				return this;
			}

			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::setRepresentativeIncomingObjectRolesSet(CRedlandStoredTriplesRepresentativeTagRolesSet* repRoleSet) {
				mIncomingObjectRolesSet = repRoleSet;
				return this;
			}

			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::setRepresentativeOutgoingDataRolesSet(CRedlandStoredTriplesRepresentativeTagRolesSet* repRoleSet) {
				mOutgoingDataRolesSet = repRoleSet;
				return this;
			}

			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::setHasDifferentFromAssertions(bool hasAssertions) {
				mDifferentFromAssertions = hasAssertions;
				return this;
			}

			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData::setHasSameAsAssertions(bool hasAssertions) {
				mSameAsAssertions = hasAssertions;
				return this;
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
