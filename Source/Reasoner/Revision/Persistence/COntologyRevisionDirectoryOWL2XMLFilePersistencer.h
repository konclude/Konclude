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

#ifndef KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONDIRECTORYOWL2XMLFILEPERSISTENCER_H
#define KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONDIRECTORYOWL2XMLFILEPERSISTENCER_H

// Libraries includes
#include <QDir>
#include <QTemporaryFile>
#include <QFile>
#include <QDomDocument>

// Namespace includes
#include "PersistenceSettings.h"
#include "COntologyRevisionPersistencer.h"


// Other includes
#include "Config/CConfigDataReader.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Generator/CConcreteOntologyUpdateCollectorBuilder.h"

#include "Parser/COWL2QtXMLOntologyStableStreamParser.h"
#include "Parser/COWL2QtXMLOntologyStreamParser.h"

#include "Renderer/COntologyAxiomExpressionsRenderVisitor.h"
#include "Renderer/COntologyOWL2QtXMLRenderer.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogDomain.h"


namespace Konclude {

	using namespace Logger;
	using namespace Renderer;
	using namespace Config;
	using namespace Parser;

	namespace Reasoner {

		using namespace Generator;

		namespace Revision {

			namespace Persistence {

				/*! 
				 *
				 *		\class		COntologyRevisionDirectoryOWL2XMLFilePersistencer
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COntologyRevisionDirectoryOWL2XMLFilePersistencer : public COntologyRevisionPersistencer, public CLogDomain {
					// public methods
					public:
						//! Constructor
						COntologyRevisionDirectoryOWL2XMLFilePersistencer(CConfiguration* config);

						//! Destructor
						virtual ~COntologyRevisionDirectoryOWL2XMLFilePersistencer();

						virtual bool persistOntologyRevision(COntologyRevision* ontoRev, COntologyRevision* prevOntoRev);

						virtual bool loadLatestOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev);

						virtual bool hasNewOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev);

					// protected methods
					protected:
						QStringList getOntologyFileNameParts(const QString& ontologyName);
						bool isOntologyFileNameNewer(const QString& currentOntologyName, const QString& newOntologyName);

						QString getNextPersistingOntologyRevisionString(const QString& ontologyName, const QStringList& pathSepStrings, COntologyRevision* ontoRev);


					// protected variables
					protected:
						QString mConfBaseDirectory;
						bool mConfEnforceUTF8ConvertingParsing;

						QString mOWLNS;

						CConfiguration* mConfig;

						QHash<QString,QString> mOntologyNameLastFileHash;
						QHash<COntologyRevision*,QString> mOntologyNameFileHash;
					
					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONDIRECTORYOWL2XMLFILEPERSISTENCER_H