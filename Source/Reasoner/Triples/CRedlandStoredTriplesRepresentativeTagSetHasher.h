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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASHER_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASHER_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandStoredTriplesRepresentativeTagSet.h"


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
			 *		\class		CRedlandStoredTriplesRepresentativeTagSetHasher
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			template<class T>
			class CRedlandStoredTriplesRepresentativeTagSetHasher {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesRepresentativeTagSetHasher(CRedlandStoredTriplesRepresentativeTagSet<T>* tagSet, QMap<cint64, T>* tagSetExtensionMap = nullptr);

					cint64 getTagHashValue() const {
						return mHashValue;
					}

					bool operator==(const CRedlandStoredTriplesRepresentativeTagSetHasher& tagSetHasher) const {
						if (mHashValue != tagSetHasher.getTagHashValue()) {
							return false;
						}

						CRedlandStoredTriplesRepresentativeTagSet<T>* baseTagSet = mTagSet;
						CRedlandStoredTriplesRepresentativeTagSet<T>* extendedTagSet = tagSetHasher.mTagSet;
						QMap<cint64, T>* tagSetExtensionMap = tagSetHasher.mTagSetExtensionMap;

						if (!tagSetExtensionMap) {
							baseTagSet = tagSetHasher.mTagSet;
							extendedTagSet = mTagSet;
							tagSetExtensionMap = mTagSetExtensionMap;
						}

						if (tagSetExtensionMap) {
							return baseTagSet->checkEqualTags(extendedTagSet, tagSetExtensionMap);
						} else {
							return baseTagSet->equals(extendedTagSet);
						}
					}



				// protected methods
				protected:

				// protected variables
				protected:
					CRedlandStoredTriplesRepresentativeTagSet<T>* mTagSet;
					QMap<cint64, T>* mTagSetExtensionMap;
					cint64 mHashValue;


				// private methods
				private:

				// private variables
				private:

			};

			template<class T>
			inline uint qHash(const CRedlandStoredTriplesRepresentativeTagSetHasher<T>& tagSetHasher) {
				qint64 key = (qint64)tagSetHasher.getTagHashValue();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				}
				else {
					return uint(key);
				}
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CRedlandStoredTriplesRepresentativeTagSetHasher.cpp"


#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESREPRESENTATIVETAGSETHASHER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
