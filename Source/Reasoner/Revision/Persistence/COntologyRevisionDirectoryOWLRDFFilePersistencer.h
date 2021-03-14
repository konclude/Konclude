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

#ifndef KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONDIRECTORYOWLRDFFILEPERSISTENCER_H
#define KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONDIRECTORYOWLRDFFILEPERSISTENCER_H

// Libraries includes
#include <QDir>
#include <QTemporaryFile>
#include <QFile>
#include <QDomDocument>

// Namespace includes
#include "PersistenceSettings.h"
#include "COntologyRevisionDirectoryFilePersistencerBase.h"


// Other includes
#include "Config/CConfigDataReader.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Generator/CConcreteOntologyUpdateCollectorBuilder.h"

#include "Parser/CRDFRedlandRaptorParser.h"

#include "Renderer/COntologyAxiomExpressionsRenderVisitor.h"
#include "Renderer/COntologyRedlandRDFRenderer.h"

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
				 *		\class		COntologyRevisionDirectoryOWLRDFFilePersistencer
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COntologyRevisionDirectoryOWLRDFFilePersistencer : public COntologyRevisionDirectoryFilePersistencerBase {
					// public methods
					public:
						//! Constructor
						COntologyRevisionDirectoryOWLRDFFilePersistencer(CConfiguration* config);

						//! Destructor
						virtual ~COntologyRevisionDirectoryOWLRDFFilePersistencer();


					// protected methods
					protected:


						virtual bool writeOntologyToFile(QFile* file, COntologyRevision* ontoRev);

					// protected variables
					protected:
						QString mSerializingFormat;
					
					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REVISION_PERSISTENCE_CONTOLOGYREVISIONDIRECTORYOWLRDFFILEPERSISTENCER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
