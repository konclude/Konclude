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

#include "COntologyRevisionRedlandExternalStoredTriplesConnectionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {

			namespace Persistence {

				COntologyRevisionRedlandExternalStoredTriplesConnectionData::COntologyRevisionRedlandExternalStoredTriplesConnectionData() {
					mInitiallyLoaded = false;
					mLastPersistedAxiomChangeNumber = 0;
				}

				COntologyRevisionRedlandExternalStoredTriplesConnectionData::~COntologyRevisionRedlandExternalStoredTriplesConnectionData() {
				}


				COntologyRevisionRedlandExternalStoredTriplesConnectionData* COntologyRevisionRedlandExternalStoredTriplesConnectionData::setInitiallyLoaded(bool loaded) {
					mInitiallyLoaded = loaded;
					return this;
				}

				bool COntologyRevisionRedlandExternalStoredTriplesConnectionData::isInitiallyLoaded() {
					return mInitiallyLoaded;
				}


				cint64 COntologyRevisionRedlandExternalStoredTriplesConnectionData::getLastPersistedAxiomChangeNumber() {
					return mLastPersistedAxiomChangeNumber;
				}

				COntologyRevisionRedlandExternalStoredTriplesConnectionData* COntologyRevisionRedlandExternalStoredTriplesConnectionData::setLastPersistedAxiomChangeNumber(cint64 number) {
					mLastPersistedAxiomChangeNumber = number;
					return this;
				}

				QSet<CTriplesData*>* COntologyRevisionRedlandExternalStoredTriplesConnectionData::getPersistedTriplesDataSet() {
					return &mPersistedTriplesDataSet;
				}

				QSet<CAxiomExpression*>* COntologyRevisionRedlandExternalStoredTriplesConnectionData::getPersistedAxiomExpressionSet() {
					return &mPersistedAxiomExpSet;
				}


			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // !KONCLUDE_REDLAND_INTEGRATION
