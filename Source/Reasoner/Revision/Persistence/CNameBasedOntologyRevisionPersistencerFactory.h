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

#ifndef KONCLUDE_REASONER_REVISION_PERSISTENCE_CNAMEBASEDONTOLOGYREVISIONPERSISTENCERFACTORY_H
#define KONCLUDE_REASONER_REVISION_PERSISTENCE_CNAMEBASEDONTOLOGYREVISIONPERSISTENCERFACTORY_H

// Libraries includes


// Namespace includes
#include "PersistenceSettings.h"
#include "COntologyRevisionPersistencerFactory.h"
#include "COntologyRevisionDirectoryOWL2XMLFilePersistencer.h"
#include "COntologyRevisionDirectoryOWLRDFFilePersistencer.h"
#include "COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer.h"
#include "COntologyRevisionRedlandExternalTriplesSPARQLServicePersistencer.h"
#include "COntologyRevisionMultiPersistencer.h"


// Other includes
#include "Config/CConfigDataReader.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		namespace Revision {

			namespace Persistence {

				/*! 
				 *
				 *		\class		CNameBasedOntologyRevisionPersistencerFactory
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CNameBasedOntologyRevisionPersistencerFactory : public COntologyRevisionPersistencerFactory {
					// public methods
					public:
						//! Constructor
						CNameBasedOntologyRevisionPersistencerFactory(const QString& persisterName, CConfiguration* config);

						virtual COntologyRevisionPersistencer* createOntologyRevisionPersistencer();

					// protected methods
					protected:

					// protected variables
					protected:
						CConfiguration* mConfig;
						QString mPersisterName;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REVISION_PERSISTENCE_CNAMEBASEDONTOLOGYREVISIONPERSISTENCERFACTORY_H
