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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASHER_CPP
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASHER_CPP


#include "CRedlandStoredTriplesRepresentativeTagSetHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			template<class T>
			CRedlandStoredTriplesRepresentativeTagSetHasher<T>::CRedlandStoredTriplesRepresentativeTagSetHasher(CRedlandStoredTriplesRepresentativeTagSet<T>* tagSet, QMap<cint64, T>* tagSetExtensionMap) {
				mTagSetExtensionMap = tagSetExtensionMap;
				mTagSet = tagSet;

				if (!mTagSetExtensionMap) {
					mHashValue = mTagSet->getHashValue();
				} else {
					mHashValue = mTagSet->getHashValueExtendedByMap(mTagSetExtensionMap);
				}
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASHER_CPP


#endif // !KONCLUDE_REDLAND_INTEGRATION
