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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONINDEXCACHEDATA_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONINDEXCACHEDATA_H

// Libraries includes
#include <redland.h>


// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler.h"
#include "CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData.h"
#include "CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Triples {



			/*! 
			 *
			 *		\class		CRedlandStoredTriplesIndividualAssertionIndexCacheData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesIndividualAssertionIndexCacheData {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesIndividualAssertionIndexCacheData(librdf_node* individualNode, cint64 indiviualId, CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler* releaser);

					~CRedlandStoredTriplesIndividualAssertionIndexCacheData();


					librdf_node* getIndividualNode();
					QString getIndividualName();
					cint64 getIndividualId();

					bool isAnonymous();
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* setAnonymous(bool anonymous);

					bool isIndividualNonAnonymityStored();
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* setIndividualNonAnonymityStored(bool stored);


					bool isIndividualIdStored();
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* setIndividualIdStored(bool stored);


					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* getCurrentRepresentativeIndividualAssertionsIndexData();
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* setCurrentRepresentativeIndividualAssertionsIndexData(CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* repData);



					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* getStoredRepresentativeIndividualAssertionsIndexData();
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* setStoredRepresentativeIndividualAssertionsIndexData(CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* repData);


					CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData* getExtensionData(bool create = true);
					CRedlandStoredTriplesIndividualAssertionIndexCacheData* clearExtensionData();


				// protected methods
				protected:

				// protected variables
				protected:
					CRedlandStoredTriplesIndividualAssertionIndexCacheDataUpdateHandler* mReleaser;
					librdf_node* mIndividualNode;
					cint64 mIndividualId = 0;
					bool mIdStored = false;
					bool mAnonymous = true;
					bool mNonAnonymityStored = false;

					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* mCurrentRepresentativeIndividualAssertionsIndexData;
					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* mStoredRepresentativeIndividualAssertionsIndexData;

					CRedlandStoredTriplesIndividualAssertionIndexCacheExtensionData* mExtensionData = nullptr;


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALASSERTIONINDEXCACHEDATA_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
