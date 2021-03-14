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

#ifndef KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONREDLANDEXTERNALTRIPLESDSNPERSISTENCERBASE_H
#define KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONREDLANDEXTERNALTRIPLESDSNPERSISTENCERBASE_H

// Libraries includes
#include <QUrl>

// Namespace includes
#include "PersistenceSettings.h"
#include "COntologyRevisionPersistencer.h"
#include "COntologyRevisionRedlandExternalStoredTriplesConnectionData.h"


// Other includes
#include "Config/CConfigDataReader.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Renderer/COntologyAxiomExpressionsRenderVisitor.h"
#include "Renderer/COntologyRedlandRDFRenderer.h"

#include "Control/Command/CCommandRecordRouter.h"

#include "Reasoner/Generator/CConcreteOntologyRedlandTriplesDataExpressionMapper.h"
#include "Reasoner/Generator/CConcreteOntologyUpdateCollectorBuilder.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	using namespace Logger;
	using namespace Renderer;
	using namespace Config;
	using namespace Control::Command;

	namespace Reasoner {

		using namespace Generator;

		namespace Revision {

			namespace Persistence {

				/*! 
				 *
				 *		\class		COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer : public COntologyRevisionPersistencer, public CLogIdentifier {
					// public methods
					public:
						//! Constructor
						COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer(CConfiguration* config);

						//! Destructor
						virtual ~COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer();

						virtual bool persistOntologyRevision(COntologyRevision* ontoRev, COntologyRevision* prevOntoRev);

						virtual bool loadLatestOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev);

						virtual bool hasNewOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev);

						virtual bool hasOntologyPersisted(const QString& ontologyName);
						virtual bool deletePersistedOntology(const QString& ontologyName);

					// protected methods
					protected:

						COntologyRevisionRedlandExternalStoredTriplesConnectionData* getOntologyPersistingTriplesData(const QString& ontName);
						COntologyRevisionRedlandExternalTriplesDSNOnlyWritingPersistencer* initTriplesData(CRedlandStoredTriplesData* triplesData, const QString& ontName);

						CRedlandStoredTriplesData* getUpdatingTripleData();
						librdf_node* getLocalizedNode(librdf_node* node, CRedlandStoredTriplesData* newTriplesData);


					// protected variables
					protected:
						CConfiguration* mConfig;

						QHash<COntologyRevision*,QString> mOntologyNameFileHash;

						QString mTriplesDBName;
						QString mDsnConfig;

						bool mConfInitialLoading;
						bool mConfDirectFullLoading;

						QHash<QString, COntologyRevisionRedlandExternalStoredTriplesConnectionData*> mOntologyTriplesDataHash;
					
					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONREDLANDEXTERNALTRIPLESDSNPERSISTENCERBASE_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
