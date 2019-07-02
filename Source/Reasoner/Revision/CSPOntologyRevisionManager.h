/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_REVISION_CSPONTOLOGYREVISIONMANAGER_H
#define KONCLUDE_REASONER_REVISION_CSPONTOLOGYREVISIONMANAGER_H

// Libraries includes
#include <QString>

// Namespace includes
#include "COntologyRevisionManager.h"

// Other includes
#include "Reasoner/Classifier/CClassificationManager.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CMBox.h"

#include "Control/Command/Instructions/CCreateKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CCreateKnowledgeBaseRevisionUpdateCommand.h"
#include "Control/Command/Instructions/CInstallKnowledgeBaseRevisionUpdateCommand.h"
#include "Control/Command/Instructions/CGetCurrentKnowledgeBaseRevisionCommand.h"
#include "Control/Command/Instructions/CReleaseKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseConfigCommand.h"
#include "Control/Command/Instructions/CSetKnowledgeBaseConfigCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseSettingsCommand.h"

#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/CConfigManagerReader.h"


#include "Control/Command/Records/CStartProcessCommandRecord.h"
#include "Control/Command/Records/CStopProcessCommandRecord.h"
#include "Control/Command/Records/CFinishProcessCommandRecord.h"
#include "Control/Command/Records/CUnspecifiedMessageErrorRecord.h"

#include "Config/CConfigurationProvider.h"
#include "Config/CConfigDataReader.h"

#include "Reasoner/Generator/CConcreteOntologyBasementBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyMergingBuilder.h"

#include "Reasoner/Preprocess/COntologyConfigDependedPreProcesser.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	using namespace Logger;
	using namespace Config;
	using namespace Utilities;
	using namespace Control::Command::Instructions;
	using namespace Control::Command::Records;
	using namespace Control::Command;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Classifier;
		using namespace Generator;
		using namespace Preprocess;

		namespace Revision {

			/*! 
			 *
			 *		\class		CSPOntologyRevisionManager
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSPOntologyRevisionManager : public COntologyRevisionManager, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					CSPOntologyRevisionManager();

					//! Destructor
					virtual ~CSPOntologyRevisionManager();

					virtual COntologyRevisionManager *initializeManager(CConfigurationProvider *configurationProvider);

					virtual qint64 getOntologyCount();

					virtual CCommandDelegater *delegateCommand(CCommand *command);
				

				// protected methods
				protected:
					CConcreteOntology* getBasementOntology(CCommandRecordRouter& commandRecordRouter);

				// protected variables
				protected:
					qint64 nextOntologyID;
					qint64 ontologyCount;
					qint64 nextRevisionPathID;

					QHash<qint64,COntologyRevision *> revisionHash;
					QHash<QString,qint64> nameIDHash;

					QSet<COntologyRevision *> onRevContainer;

					qint64 nextUnamedOntologyNumber;

					CConfigurationProvider *confProvider;

					CClassificationManager *classificationMan;

					CConcreteOntology* mBaseOnto;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REVISION_CSPONTOLOGYREVISIONMANAGER_H
