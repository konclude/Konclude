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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSET_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSET_H

// Libraries includes
#include <QMap>
#include <QHash>
#include <Redland.h>

// Namespace includes
#include "TriplesSettings.h"


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
			 *		\class		CRedlandStoredTriplesRepresentativeTagSet
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CRedlandStoredTriplesRepresentativeTagSet {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesRepresentativeTagSet(cint64 representativeId);

					CRedlandStoredTriplesRepresentativeTagSet<T>* extendFrom(CRedlandStoredTriplesRepresentativeTagSet<T>* extending, QMap<cint64, T>* extensionMap);

					bool hasTagElement(cint64 tag);
					cint64 getRepresentativeId();


					cint64 getHashValue() const;
					cint64 getHashValueExtendedByMap(QMap<cint64, T>* extensionMap) const;


					QMap<cint64,T>* getTagDataMap();

					bool equals(CRedlandStoredTriplesRepresentativeTagSet<T>* tagSet) const;
					bool equals(CRedlandStoredTriplesRepresentativeTagSet<T>* tagSet, QMap<cint64, T>* tagSetExtensionMap) const;
					bool checkEqualTags(CRedlandStoredTriplesRepresentativeTagSet<T>* tagSet, QMap<cint64, T>* tagSetExtensionMap) const;


					CRedlandStoredTriplesRepresentativeTagSet<T>* addIndividualCacheData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData, bool fromStore = false);
					CRedlandStoredTriplesRepresentativeTagSet<T>* removeIndividualCacheData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData, bool toStore = false);
					cint64 getActiveIndividualCount();
					cint64 getTotalIndividualCount();
					
					cint64 getTagElementCount();

					librdf_node*& getRedlandTagNode();

					
				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mRepresentativeId;
					QMap<cint64, T> mTagDataMap;

					mutable bool mHashValueCalculated = false;
					mutable cint64 mTagHashValue = 0;
					mutable cint64 mTagAdditionHashValue = 0;
					mutable cint64 mTagMultiplicationHashValue = 0;
					mutable bool mTagMultiplicationHashInitialized = false;


					QSet<CRedlandStoredTriplesIndividualAssertionIndexCacheData*> mIndiDataSet;
					cint64 mActiveCount = 0;
					cint64 mTotalCount = 0;

					librdf_node* mTagNode;


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CRedlandStoredTriplesRepresentativeTagSet.cpp"


#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSET_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
