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

#include "CRedlandRasqalLiteralHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CRedlandRasqalLiteralHasher::CRedlandRasqalLiteralHasher(rasqal_literal* node) {
				mNode = rasqal_new_literal_from_literal(node);
				rasqal_literal_type litType = rasqal_literal_get_type(mNode);

				if (litType == RASQAL_LITERAL_URI) {					
					mHashValue = hash_c_string((const char*)rasqal_literal_as_string(mNode));
				} else {
					mHashValue = hash_c_string((const char*)rasqal_literal_as_string(mNode));
				}
			}

			CRedlandRasqalLiteralHasher::CRedlandRasqalLiteralHasher(const CRedlandRasqalLiteralHasher& hasher) {
				mNode = rasqal_new_literal_from_literal(hasher.mNode);
				mHashValue = hasher.mHashValue;
			}


			CRedlandRasqalLiteralHasher& CRedlandRasqalLiteralHasher::operator=(const CRedlandRasqalLiteralHasher& hasher) {
				mNode = rasqal_new_literal_from_literal(hasher.mNode);
				mHashValue = hasher.mHashValue;
				return *this;
			}


			CRedlandRasqalLiteralHasher::~CRedlandRasqalLiteralHasher() {
				rasqal_free_literal(mNode);
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
