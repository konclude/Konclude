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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYTRIPLESDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYTRIPLESDATA_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyTriplesDataStatus.h"
#include "COntologyTriplesAssertionsAccessor.h"


// Other includes
#include "Reasoner/Triples/CTriplesData.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Triples;

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyTriplesData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyTriplesData {
				// public methods
				public:
					//! Constructor
					COntologyTriplesData();

					COntologyTriplesData* referenceTriplesData(COntologyTriplesData* tripleData);

					CTriplesData* getLatestTriplesData(bool forceLocal = false);
					QList<CTriplesData*>* getAllTriplesData();
					COntologyTriplesData* addTriplesData(CTriplesData* triplesData);

					COntologyTriplesDataStatus* getTriplesDataStatus();

					COntologyTriplesAssertionsAccessor* getTripleAssertionAccessor();
					COntologyTriplesData* setTripleAssertionAccessor(COntologyTriplesAssertionsAccessor* accessor);


				// protected methods
				protected:

				// protected variables
				protected:
					CTriplesData* mLocLatestTriplesData;
					CTriplesData* mPrevLatestTriplesData;
					COntologyTriplesDataStatus mTriplesDataStatus;

					QList<CTriplesData*> mAllTriplesDataList;


					COntologyTriplesAssertionsAccessor* mTripleAssertionAccessor = nullptr;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYTRIPLESDATA_H
