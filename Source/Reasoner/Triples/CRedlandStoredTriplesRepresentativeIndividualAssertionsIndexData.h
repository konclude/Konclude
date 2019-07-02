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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVEINDIVIDUALASSERTIONSINDEXDATA_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVEINDIVIDUALASSERTIONSINDEXDATA_H

// Libraries includes


// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandStoredTriplesRepresentativeTagTypeConceptsSet.h"
#include "CRedlandStoredTriplesRepresentativeTagRolesSet.h"


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
			 *		\class		CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData();

					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData(CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* individualAssIndData);


					CRedlandStoredTriplesRepresentativeTagTypeConceptsSet* getRepresentativeTypeConceptsSet();
					CRedlandStoredTriplesRepresentativeTagRolesSet* getRepresentativeOutgoingObjectRolesSet();
					CRedlandStoredTriplesRepresentativeTagRolesSet*  getRepresentativeIncomingObjectRolesSet();
					CRedlandStoredTriplesRepresentativeTagRolesSet*  getRepresentativeOutgoingDataRolesSet();

					bool hasDifferentFromAssertions();
					bool hasSameAsAssertions();

					bool hasTypeConceptsAssertions();
					bool hasOutgoingObjectRoleAssertions();
					bool hasIncomingObjectRoleAssertions();
					bool hasOutgoingDataRoleAssertions();


					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* setRepresentativeTypeConceptsSet(CRedlandStoredTriplesRepresentativeTagTypeConceptsSet* repConSet);
					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* setRepresentativeOutgoingObjectRolesSet(CRedlandStoredTriplesRepresentativeTagRolesSet* repRoleSet);
					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* setRepresentativeIncomingObjectRolesSet(CRedlandStoredTriplesRepresentativeTagRolesSet* repRoleSet);
					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* setRepresentativeOutgoingDataRolesSet(CRedlandStoredTriplesRepresentativeTagRolesSet* repRoleSet);
					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* setHasDifferentFromAssertions(bool hasAssertions);
					CRedlandStoredTriplesRepresentativeIndividualAssertionsIndexData* setHasSameAsAssertions(bool hasAssertions);


				// protected methods
				protected:

				// protected variables
				protected:

					CRedlandStoredTriplesRepresentativeTagTypeConceptsSet* mRepConSet = nullptr;

					CRedlandStoredTriplesRepresentativeTagRolesSet* mOutgoingObjectRolesSet = nullptr;
					CRedlandStoredTriplesRepresentativeTagRolesSet* mIncomingObjectRolesSet = nullptr;
					CRedlandStoredTriplesRepresentativeTagRolesSet* mOutgoingDataRolesSet = nullptr;

					bool mDifferentFromAssertions = false;
					bool mSameAsAssertions = false;



				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVEINDIVIDUALASSERTIONSINDEXDATA_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
