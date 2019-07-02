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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASH_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASH_H

// Libraries includes
#include <QHash>
#include <QMap>

// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandStoredTriplesRepresentativeTagSetHasher.h"

// Other includes
#include "Utilities/UtilitiesSettings.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Container;

	namespace Reasoner {

		namespace Triples {



			/*! 
			 *
			 *		\class		CRedlandStoredTriplesRepresentativeTagSetHash
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T, class S>
			class CRedlandStoredTriplesRepresentativeTagSetHash {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesRepresentativeTagSetHash();


					S* getRepresentativeTagSet(cint64 repId);
					S* getBaseTagSet();

					S* update(S* tagSet, QMap<cint64,T>* extensionTagMap, CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData);


					QSet<T>* getOccurringTagElementSet();

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mNextRepresentativeId = 0;
					QHash< CRedlandStoredTriplesRepresentativeTagSetHasher<T>, S* > mTagSetDataHash;
					QHash< cint64, S* > mRepIdDataHash;
					S* mBaseTagSet = nullptr;


					QSet<T> mOccurringTagElementSet;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CRedlandStoredTriplesRepresentativeTagSetHash.cpp"


#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASH_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
