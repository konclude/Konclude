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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESDATA_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESDATA_H

// Libraries includes
#include <Redland.h>


// Namespace includes
#include "CTriplesData.h"
#include "TriplesSettings.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			/*! 
			 *
			 *		\class		CRedlandStoredTriplesData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesData : public CTriplesData {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesData();
					virtual ~CRedlandStoredTriplesData();

					virtual CRedlandStoredTriplesData* initTriplesData(CTRIPLES_DATA_UPDATE_TYPE updateType, CRedlandStoredTriplesData* lastSharingTripleData = nullptr);


					CRedlandStoredTriplesData* setRedlandModelStorageWorldData(librdf_world* world, librdf_storage* streamedStorage, librdf_model* indexedModel, CXLinker<librdf_statement*>* linker);
					CRedlandStoredTriplesData* setRedlandWorldData(librdf_world* world);
					CRedlandStoredTriplesData* setRedlandIndexedStorageData(librdf_storage* storage);
					CRedlandStoredTriplesData* setRedlandIndexedModelData(librdf_model* model);

					librdf_world* getRedlandWorld();
					librdf_storage* getRedlandIndexedStorage();
					librdf_model* getRedlandIndexedModel();

					CXLinker<librdf_statement*>* getRedlandStatementLinker();
					CRedlandStoredTriplesData* setRedlandStatementLinker(CXLinker<librdf_statement*>* linker);

					virtual CTRIPLES_DATA_UPDATE_TYPE getUpdateType() override;


				// protected methods
				protected:

				// protected variables
				protected:
					CTRIPLES_DATA_UPDATE_TYPE mUpdateType;

					librdf_world* mWorld;
					librdf_storage* mIndexedStorage;
					librdf_model* mIndexedModel;

					CXLinker<librdf_statement*>* mStatementLinker;

					CRedlandStoredTriplesData* mLastSharingTripleData;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESDATA_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
