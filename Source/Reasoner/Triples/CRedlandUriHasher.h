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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDURIHASHER_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDURIHASHER_H

// Libraries includes
#include <QHash>
#include <Redland.h>

// Namespace includes
#include "TriplesSettings.h"


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
			 *		\class		CRedlandUriHasher
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandUriHasher {
				// public methods
				public:
					//! Constructor
					CRedlandUriHasher(librdf_uri* uri) {
						mUri = uri;
						mHashValue = hash_c_string((const char*)librdf_uri_as_string(mUri));
					}

					cint64 getUriHashValue() const {
						return mHashValue;
					}

					bool operator==(const CRedlandUriHasher& uriHasher) const {
						if (mHashValue != uriHasher.getUriHashValue()) {
							return false;
						}
						if (!librdf_uri_equals(mUri, uriHasher.mUri)) {
							return false;
						}
						return true;
					}


					cint64 hash_c_string(const char* p) {
						cint64 result = 0;
						const cint64 prime = 31;
						for (cint64 i = 0; p[i] != 0; ++i) {
							result = p[i] + (result * prime);
						}
						return result;
					}

				// protected methods
				protected:

				// protected variables
				protected:
					librdf_uri* mUri;
					cint64 mHashValue;

				// private methods
				private:

				// private variables
				private:

			};


	

			inline uint qHash(const CRedlandUriHasher& uriHasher) {
				qint64 key = (qint64)uriHasher.getUriHashValue();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				} else {
					return uint(key);
				}
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude


#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDURIHASHER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
