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

#include "CRedlandStoredTriplesIndividualAssertionIndexCacheData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			CRedlandStoredTriplesIndividualAssertionIndexCacheData::CRedlandStoredTriplesIndividualAssertionIndexCacheData(librdf_node* individualNode, cint64 indiviualId, CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler* releaser) {
				mReleaser = releaser;
				mIndividualId = indiviualId;
				mIndividualNode = individualNode;
				mCurrentRepresentativeIndividualAssertionsIndexData = nullptr;
				mStoredRepresentativeIndividualAssertionsIndexData = nullptr;
			}


			CRedlandStoredTriplesIndividualAssertionIndexCacheData::~CRedlandStoredTriplesIndividualAssertionIndexCacheData() {
				if (mReleaser) {
					mReleaser->releaseCacheData(this);
				}
				if (mExtensionData) {
					delete mExtensionData;
				}
			}


			librdf_node* CRedlandStoredTriplesIndividualAssertionIndexCacheData::getIndividualNode() {
				return mIndividualNode;
			}


			QString CRedlandStoredTriplesIndividualAssertionIndexCacheData::getIndividualName() {
				const char* indiNameCharPointer = nullptr;
				if (!librdf_node_is_blank(mIndividualNode)) {
					librdf_uri* uri = librdf_node_get_uri(mIndividualNode);
					indiNameCharPointer = (const char*)librdf_uri_as_string(uri);
				} else {
					indiNameCharPointer = (const char*)librdf_node_get_blank_identifier(mIndividualNode);
				}
				return QString::fromUtf8(indiNameCharPointer);
			}

			cint64 CRedlandStoredTriplesIndividualAssertionIndexCacheData::getIndividualId() {
				return mIndividualId;
			}


			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* CRedlandStoredTriplesIndividualAssertionIndexCacheData::getCurrentRepresentativeIndividualAssertionsIndexData() {
				return mCurrentRepresentativeIndividualAssertionsIndexData;
			}


			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCacheData::setCurrentRepresentativeIndividualAssertionsIndexData(CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* repData) {
				mCurrentRepresentativeIndividualAssertionsIndexData = repData;
				return this;
			}


			CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* CRedlandStoredTriplesIndividualAssertionIndexCacheData::getStoredRepresentativeIndividualAssertionsIndexData() {
				return mStoredRepresentativeIndividualAssertionsIndexData;
			}


			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCacheData::setStoredRepresentativeIndividualAssertionsIndexData(CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* repData) {
				mStoredRepresentativeIndividualAssertionsIndexData = repData;
				return this;
			}

			CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData* CRedlandStoredTriplesIndividualAssertionIndexCacheData::getExtensionData(bool create) {
				if (!mExtensionData && create) {
					mExtensionData = new CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData();
				}
				return mExtensionData;
			}

			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCacheData::clearExtensionData() {
				if (mExtensionData) {
					delete mExtensionData;
					mExtensionData = nullptr;
				}
				return this;
			}


			bool CRedlandStoredTriplesIndividualAssertionIndexCacheData::isIndividualIdStored() {
				return mIdStored;
			}

			CRedlandStoredTriplesIndividualAssertionIndexCacheData* CRedlandStoredTriplesIndividualAssertionIndexCacheData::setIndividualIdStored(bool stored) {
				mIdStored = stored;
				return this;
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
