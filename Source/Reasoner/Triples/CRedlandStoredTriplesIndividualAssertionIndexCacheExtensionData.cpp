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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#include "CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData::CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData() {
			}

			CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData::~CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData() {
				if (mTypeConceptExtensionMap) {
					delete mTypeConceptExtensionMap;
				}
				if (mOutgoingObjectRoleExtensionMap) {
					delete mOutgoingObjectRoleExtensionMap;
				}
				if (mIncomingObjectRoleExtensionMap) {
					delete mIncomingObjectRoleExtensionMap;
				}
				if (mOutgoingDataRoleExtensionMap) {
					delete mOutgoingDataRoleExtensionMap;
				}
			}


			QMap<cint64, CConcept*>* CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData::getTypeConceptExtensionMap(bool create) {
				if (!mTypeConceptExtensionMap && create) {
					mTypeConceptExtensionMap = new QMap<cint64, CConcept*>();
				}
				return mTypeConceptExtensionMap;
			}



			QMap<cint64, CRole*>* CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData::getOutgoingObjectRoleExtensionMap(bool create) {
				if (!mOutgoingObjectRoleExtensionMap && create) {
					mOutgoingObjectRoleExtensionMap = new QMap<cint64, CRole*>();
				}
				return mOutgoingObjectRoleExtensionMap;
			}



			QMap<cint64, CRole*>* CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData::getIncomingObjectRoleExtensionMap(bool create) {
				if (!mIncomingObjectRoleExtensionMap && create) {
					mIncomingObjectRoleExtensionMap = new QMap<cint64, CRole*>();
				}
				return mIncomingObjectRoleExtensionMap;
			}


			QMap<cint64, CRole*>* CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData::getOutgoingDataRoleExtensionMap(bool create) {
				if (!mOutgoingDataRoleExtensionMap && create) {
					mOutgoingDataRoleExtensionMap = new QMap<cint64, CRole*>();
				}
				return mOutgoingDataRoleExtensionMap;
			}



		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
