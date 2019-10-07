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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASH_CPP
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASH_CPP


#include "CRedlandStoredTriplesRepresentativeTagSetHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			template<class T, class S>
			CRedlandStoredTriplesRepresentativeTagSetHash<T, S>::CRedlandStoredTriplesRepresentativeTagSetHash() {
				cint64 id = mNextRepresentativeId++;
				mBaseTagSet = new S(id);
				CRedlandStoredTriplesRepresentativeTagSetHasher<T> hasher(mBaseTagSet);
				mTagSetDataHash.insert(hasher, mBaseTagSet);
			}


			template<class T, class S>
			S* CRedlandStoredTriplesRepresentativeTagSetHash<T, S>::getBaseTagSet() {
				return mBaseTagSet;
			}

			template<class T, class S>
			S* CRedlandStoredTriplesRepresentativeTagSetHash<T, S>::getRepresentativeTagSet(cint64 repId) {
				S* repTagSet = mRepIdDataHash.value(repId);
				return repTagSet;
			}

			template<class T, class S>
			S* CRedlandStoredTriplesRepresentativeTagSetHash<T, S>::update(S* tagSet, QMap<cint64, T>* extensionTagMap, CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData) {
				CRedlandStoredTriplesRepresentativeTagSetHasher<T> hasher(tagSet, extensionTagMap);
				S* newTagSet = mTagSetDataHash.value(hasher);
				if (!newTagSet) {
					newTagSet = new S(mNextRepresentativeId++);
					newTagSet->extendFrom(tagSet, extensionTagMap);
					CRedlandStoredTriplesRepresentativeTagSetHasher<T> newHasher(newTagSet);
					mTagSetDataHash.insert(newHasher, newTagSet);
					mRepIdDataHash.insert(newTagSet->getRepresentativeId(), newTagSet);

					for (typename QMap<cint64, T>::const_iterator it = extensionTagMap->constBegin(), itEnd = extensionTagMap->constEnd(); it != itEnd; ++it) {
						T tagElement(it.value());
						mOccurringTagElementSet.insert(tagElement);
					}
				}
				tagSet->removeIndividualCacheData(indiCacheData);
				newTagSet->addIndividualCacheData(indiCacheData);
				return newTagSet;
			}



			template<class T, class S>
			QSet<T>* CRedlandStoredTriplesRepresentativeTagSetHash<T, S>::getOccurringTagElementSet() {
				return &mOccurringTagElementSet;
			}



		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASH_CPP

#endif // !KONCLUDE_REDLAND_INTEGRATION
