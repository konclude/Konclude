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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSET_CPP
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSET_CPP



#include "CRedlandStoredTriplesRepresentativeTagSet.h"

namespace Konclude {

	namespace Reasoner {

		namespace Triples {


			inline uint cHash(quint64 key, uint seed = 0) {
				return uint(((key >> (8 * sizeof(uint) - 1)) ^ key) & (~0U)) ^ seed;
			}

			template<class T>
			CRedlandStoredTriplesRepresentativeTagSet<T>::CRedlandStoredTriplesRepresentativeTagSet(cint64 representativeId) {
				mRepresentativeId = representativeId;
				mTagNode = nullptr;
			}


			template<class T>
			CRedlandStoredTriplesRepresentativeTagSet<T>* CRedlandStoredTriplesRepresentativeTagSet<T>::extendFrom(CRedlandStoredTriplesRepresentativeTagSet<T>* extending, QMap<cint64, T>* extensionMap) {
				QMap<cint64, T>* copyMap = extensionMap;
				QMap<cint64, T>* insertMap = &extending->mTagDataMap;
				if (extending->mTagDataMap.size() >= extensionMap->size()) {
					copyMap = &extending->mTagDataMap;
					insertMap = extensionMap;
				}

				mTagDataMap = *copyMap;
				for (typename QMap<cint64, T>::const_iterator it = insertMap->constBegin(), itEnd = insertMap->constEnd(); it != itEnd; ++it) {
					mTagDataMap.insert(it.key(), it.value());
				}
				return this;
			}


			template<class T>
			cint64 CRedlandStoredTriplesRepresentativeTagSet<T>::getRepresentativeId() {
				return mRepresentativeId;
			}


			template<class T>
			bool CRedlandStoredTriplesRepresentativeTagSet<T>::hasTagElement(cint64 tag) {
				return mTagDataMap.contains(tag);
			}

			template<class T>
			cint64 CRedlandStoredTriplesRepresentativeTagSet<T>::getHashValue() const {

				if (!mHashValueCalculated) {

					mTagMultiplicationHashInitialized = false;
					for (typename QMap<cint64, T>::const_iterator it = mTagDataMap.constBegin(), itEnd = mTagDataMap.constEnd(); it != itEnd; ++it) {
						cint64 tag = it.key();
						uint hashedTag = cHash(tag, 0);
						mTagAdditionHashValue += hashedTag;
						if (hashedTag != 0) {
							if (!mTagMultiplicationHashInitialized) {
								mTagMultiplicationHashInitialized = true;
								mTagMultiplicationHashValue = hashedTag;
							} else {
								mTagMultiplicationHashValue *= hashedTag;
							}
						}
					}

					mTagHashValue = mTagMultiplicationHashValue << 5 ^ mTagAdditionHashValue;
					mHashValueCalculated = true;
				}

				return mTagHashValue;
			}


			template<class T>
			bool CRedlandStoredTriplesRepresentativeTagSet<T>::equals(CRedlandStoredTriplesRepresentativeTagSet<T>* tagSet) const {
				if (tagSet->getHashValue() != getHashValue()) {
					return false;
				}
				return mTagDataMap == tagSet->mTagDataMap;
			}


			template<class T>
			bool CRedlandStoredTriplesRepresentativeTagSet<T>::equals(CRedlandStoredTriplesRepresentativeTagSet<T>* tagSet, QMap<cint64, T>* tagSetExtensionMap) const {
				if (tagSet->getHashValueExtendedByMap(tagSetExtensionMap) != getHashValue()) {
					return false;
				}
				return checkEqualTags(tagSet, tagSetExtensionMap);
			}



			template<class T>
			bool CRedlandStoredTriplesRepresentativeTagSet<T>::checkEqualTags(CRedlandStoredTriplesRepresentativeTagSet<T>* tagSet, QMap<cint64, T>* tagSetExtensionMap) const {
				typename QMap<cint64, T>::const_iterator it1 = mTagDataMap.constBegin(), it1End = mTagDataMap.constEnd();

				typename QMap<cint64, T>::const_iterator it2a = tagSet->mTagDataMap.constBegin(), it2aEnd = tagSet->mTagDataMap.constEnd();
				typename QMap<cint64, T>::const_iterator it2b = tagSetExtensionMap->constBegin(), it2bEnd = tagSetExtensionMap->constEnd();

				while (it1 != it1End) {

					cint64 tag1 = it1.key();
					cint64 tag2a = -1;
					if (it2a != it2aEnd) {
						tag2a = it2a.key();
					}
					cint64 tag2b = -1;
					if (it2b != it2bEnd) {
						tag2b = it2b.key();
					}

					if (tag1 != tag2a && tag1 != tag2b) {
						return false;
					}
					++it1;
					if (tag1 == tag2a) {
						++it2a;
					}
					if (tag1 == tag2b) {
						++it2b;
					}

				}

				if (it2a != it2aEnd) {
					return false;
				}
				if (it2b != it2bEnd) {
					return false;
				}
				return true;
			}



			template<class T>
			cint64 CRedlandStoredTriplesRepresentativeTagSet<T>::getHashValueExtendedByMap(QMap<cint64, T>* extensionMap) const {
				getHashValue();

				cint64 tempTagHashValue = 0;
				cint64 tempTagAdditionHashValue = mTagAdditionHashValue;
				cint64 tempTagMultiplicationHashValue = mTagMultiplicationHashValue;
				bool tempTagMultiplicationHashInitialized = mTagMultiplicationHashInitialized;


				for (typename QMap<cint64, T>::const_iterator it = extensionMap->constBegin(), itEnd = extensionMap->constEnd(); it != itEnd;  ++it) {
					cint64 tag = it.key();
					uint hashedTag = cHash(tag);
					tempTagAdditionHashValue += hashedTag;
					if (hashedTag != 0) {
						if (!tempTagMultiplicationHashInitialized) {
							tempTagMultiplicationHashInitialized = true;
							tempTagMultiplicationHashValue = hashedTag;
						} else {
							tempTagMultiplicationHashValue *= hashedTag;
						}
					}
				}
				tempTagHashValue = tempTagMultiplicationHashValue << 5 ^ tempTagAdditionHashValue;
				return tempTagHashValue;
			}



			template<class T>
			QMap<cint64, T>* CRedlandStoredTriplesRepresentativeTagSet<T>::getTagDataMap() {
				return &mTagDataMap;
			}



			template<class T>
			CRedlandStoredTriplesRepresentativeTagSet<T>* CRedlandStoredTriplesRepresentativeTagSet<T>::addIndividualCacheData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData, bool fromStore) {
				mIndiDataSet.insert(indiCacheData);
				if (!fromStore) {
					++mTotalCount;
				}
				++mActiveCount;
				return this;
			}

			template<class T>
			CRedlandStoredTriplesRepresentativeTagSet<T>* CRedlandStoredTriplesRepresentativeTagSet<T>::removeIndividualCacheData(CRedlandStoredTriplesIndividualAssertionIndexCacheData* indiCacheData, bool toStore) {
				mIndiDataSet.remove(indiCacheData);
				if (!toStore) {
					--mTotalCount;
				}
				--mActiveCount;
				return this;
			}


			template<class T>
			cint64 CRedlandStoredTriplesRepresentativeTagSet<T>::getActiveIndividualCount() {
				return mActiveCount;
			}

			template<class T>
			cint64 CRedlandStoredTriplesRepresentativeTagSet<T>::getTotalIndividualCount() {
				return mTotalCount;
			}


			template<class T>
			cint64 CRedlandStoredTriplesRepresentativeTagSet<T>::getTagElementCount() {
				return mTagDataMap.size();
			}


			template<class T>
			librdf_node*& CRedlandStoredTriplesRepresentativeTagSet<T>::getRedlandTagNode() {
				return mTagNode;
			}

		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSET_CPP

#endif // !KONCLUDE_REDLAND_INTEGRATION
