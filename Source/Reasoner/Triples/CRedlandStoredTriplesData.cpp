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

#include "CRedlandStoredTriplesData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			CRedlandStoredTriplesData::CRedlandStoredTriplesData() {
				mLastSharingTripleData = nullptr;
				mWorld = nullptr;
				mIndexedStorage = nullptr;
				mIndexedModel = nullptr;
				mStatementLinker = nullptr;
				mUpdateType = CTRIPLES_DATA_UPDATE_TYPE::TRIPLES_DATA_NOTHING;
			}

			CRedlandStoredTriplesData::~CRedlandStoredTriplesData() {
				if (mIndexedModel && (!mLastSharingTripleData || mLastSharingTripleData->mIndexedModel != mIndexedModel)) {
					librdf_free_model(mIndexedModel);
				}
				if (mIndexedStorage && (!mLastSharingTripleData || mLastSharingTripleData->mIndexedStorage != mIndexedStorage)) {
					librdf_free_storage(mIndexedStorage);
				}
				if (mWorld && (!mLastSharingTripleData || mLastSharingTripleData->mWorld != mWorld)) {
					librdf_free_world(mWorld);
				}
			}

			CRedlandStoredTriplesData* CRedlandStoredTriplesData::initTriplesData(CTRIPLES_DATA_UPDATE_TYPE updateType, CRedlandStoredTriplesData* lastSharingTripleData) {
				mLastSharingTripleData = (CRedlandStoredTriplesData*)lastSharingTripleData;
				mUpdateType = updateType;
				return this;
			}

			CTRIPLES_DATA_UPDATE_TYPE CRedlandStoredTriplesData::getUpdateType() {
				return mUpdateType;
			}

			CRedlandStoredTriplesData* CRedlandStoredTriplesData::setRedlandModelStorageWorldData(librdf_world* world, librdf_storage* indexedStorage, librdf_model* indexedModel, CXLinker<librdf_statement*>* linker) {
				mWorld = world;
				mIndexedStorage = indexedStorage;
				mIndexedModel = indexedModel;
				mStatementLinker = linker;
				return this;
			}


			CRedlandStoredTriplesData* CRedlandStoredTriplesData::setRedlandWorldData(librdf_world* world) {
				mWorld = world;
				return this;
			}

			CRedlandStoredTriplesData* CRedlandStoredTriplesData::setRedlandIndexedStorageData(librdf_storage* storage) {
				mIndexedStorage = storage;
				return this;
			}

			CRedlandStoredTriplesData* CRedlandStoredTriplesData::setRedlandIndexedModelData(librdf_model* model) {
				mIndexedModel = model;
				return this;
			}


			librdf_world* CRedlandStoredTriplesData::getRedlandWorld() {
				return mWorld;
			}

			librdf_storage* CRedlandStoredTriplesData::getRedlandIndexedStorage() {
				return mIndexedStorage;
			}

			librdf_model* CRedlandStoredTriplesData::getRedlandIndexedModel() {
				return mIndexedModel;
			}


			CXLinker<librdf_statement*>* CRedlandStoredTriplesData::getRedlandStatementLinker() {
				return mStatementLinker;
			}

			CRedlandStoredTriplesData* CRedlandStoredTriplesData::setRedlandStatementLinker(CXLinker<librdf_statement*>* linker) {
				mStatementLinker = linker;
				return this;
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
