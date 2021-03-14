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

#include "CRedlandNodeHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Triples {
			

			CRedlandNodeHasher::CRedlandNodeHasher(librdf_node* node) {
				mNode = librdf_new_node_from_node(node);
				if (librdf_node_is_literal(mNode)) {
					mHashValue = hash_c_string((const char*)librdf_node_get_literal_value(mNode));
				} else if (!librdf_node_is_blank(node)) {
					librdf_uri* uri = librdf_node_get_uri(mNode);
					mHashValue = hash_c_string((const char*)librdf_uri_as_string(uri));
				} else {
					mHashValue = hash_c_string((const char*)librdf_node_get_blank_identifier(mNode));
				}
			}

			CRedlandNodeHasher::CRedlandNodeHasher(const CRedlandNodeHasher& hasher) {
				mNode = librdf_new_node_from_node(hasher.mNode);
				mHashValue = hasher.mHashValue;
			}


			CRedlandNodeHasher& CRedlandNodeHasher::operator=(const CRedlandNodeHasher& hasher) {
				mNode = librdf_new_node_from_node(hasher.mNode);
				mHashValue = hasher.mHashValue;
				return *this;
			}

			librdf_node* CRedlandNodeHasher::getRedlandNode() const {
				return mNode;
			}

			CRedlandNodeHasher::~CRedlandNodeHasher() {
				librdf_free_node(mNode);
			}


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
