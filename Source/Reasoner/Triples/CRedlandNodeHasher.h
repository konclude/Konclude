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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDNODEHASHER_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDNODEHASHER_H

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
			 *		\class		CRedlandNodeHasher
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandNodeHasher {
				// public methods
				public:
					//! Constructor
					CRedlandNodeHasher(librdf_node* node);
					CRedlandNodeHasher(const CRedlandNodeHasher& hasher);

					~CRedlandNodeHasher();

					CRedlandNodeHasher& operator=(const CRedlandNodeHasher& hasher);

					librdf_node* getRedlandNode() const;
					cint64 getNodeHashValue() const {
						return mHashValue;
					}

					bool operator==(const CRedlandNodeHasher& nodeHasher) const {
						if (mHashValue != nodeHasher.getNodeHashValue()) {
							return false;
						}
						if (!librdf_node_equals(mNode, nodeHasher.mNode)) {
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
					librdf_node* mNode;
					cint64 mHashValue;

				// private methods
				private:

				// private variables
				private:

			};


	

			inline uint qHash(const CRedlandNodeHasher& nodeHasher) {
				qint64 key = (qint64)nodeHasher.getNodeHashValue();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				} else {
					return uint(key);
				}
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDNODEHASHER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
