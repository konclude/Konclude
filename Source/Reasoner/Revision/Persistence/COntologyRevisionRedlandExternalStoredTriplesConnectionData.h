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

#ifndef KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONREDLANDEXTERNALSTOREDTRIPLESCONNECTIONDATA_H
#define KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONREDLANDEXTERNALSTOREDTRIPLESCONNECTIONDATA_H

// Libraries includes
#include <QSet>


// Namespace includes
#include "PersistenceSettings.h"


// Other includes
#include "Reasoner/Triples/CRedlandStoredTriplesData.h"

#include "Parser/Expressions/CAxiomExpression.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Triples;

		namespace Revision {

			namespace Persistence {

				/*! 
				 *
				 *		\class		COntologyRevisionRedlandExternalStoredTriplesConnectionData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COntologyRevisionRedlandExternalStoredTriplesConnectionData : public CRedlandStoredTriplesData {
					// public methods
					public:
						//! Constructor
						COntologyRevisionRedlandExternalStoredTriplesConnectionData();

						//! Destructor
						virtual ~COntologyRevisionRedlandExternalStoredTriplesConnectionData();

						COntologyRevisionRedlandExternalStoredTriplesConnectionData* setInitiallyLoaded(bool loaded);
						bool isInitiallyLoaded();

						cint64 getLastPersistedAxiomChangeNumber();
						COntologyRevisionRedlandExternalStoredTriplesConnectionData* setLastPersistedAxiomChangeNumber(cint64 number);

						QSet<CTriplesData*>* getPersistedTriplesDataSet();

						QSet<CAxiomExpression*>* getPersistedAxiomExpressionSet();

					// protected methods
					protected:

					// protected variables
					protected:
						bool mInitiallyLoaded;
						cint64 mLastPersistedAxiomChangeNumber;
						QSet<CAxiomExpression*> mPersistedAxiomExpSet;
						QSet<CTriplesData*> mPersistedTriplesDataSet;
					
					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONREDLANDEXTERNALSTOREDTRIPLESCONNECTIONDATA_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
