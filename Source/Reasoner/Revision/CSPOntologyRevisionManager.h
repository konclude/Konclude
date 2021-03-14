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

#ifndef KONCLUDE_REASONER_REVISION_CSPONTOLOGYREVISIONMANAGER_H
#define KONCLUDE_REASONER_REVISION_CSPONTOLOGYREVISIONMANAGER_H

// Libraries includes
#include <QString>
#include <QHash>
#include <QUuid>

// Namespace includes
#include "COntologyRevisionManager.h"

// Other includes
#include "Reasoner/Revision/Persistence/COntologyRevisionPersistencer.h"

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
#include "Reasoner/Generator/CConcreteOntologyMergingRebuildingBuilder.h"

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

			using namespace Persistence;

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
					CSPOntologyRevisionManager(COntologyRevisionPersistencer* persistencer = nullptr);

					//! Destructor
					virtual ~CSPOntologyRevisionManager();

					virtual COntologyRevisionManager *initializeManager(CConfigurationProvider *configurationProvider);

					virtual qint64 getOntologyCount();

					virtual CCommandDelegater *delegateCommand(CCommand *command);
				

				// protected methods
				protected:
					CConcreteOntology* getBasementOntology(CCommandRecordRouter& commandRecordRouter);

					COntologyRevision* createNewOntologyRevisionFromBasementOntology(const QString& ontologyName, CCommandRecordRouter& commandRecordRouter);

					COntologyRevision* createNewOntologyRevision(const QString& ontologyName, bool forceCreation, bool reportError, CCommandRecordRouter& commandRecordRouter);
					QString createNewOntology(const QString& ontologyName, QList<CNamePrefix*>* prefixList, CCommandRecordRouter& commandRecordRouter);

					bool checkHasOntology(const QString& ontologyName);

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

					QString mLastOntologyRevisionUpdateKBNameOrNewAnonymous;
					QString mLastOntologyRevisionUpdateKBName;
					QString mLastOntologyCreatedKBName;
					QString mFirstOntologyRevisionUpdateKBName;
					QString mFirstOntologyCreatedKBName;
					bool mHasFirstCreatedOntology;
					bool mHasFirstOntologyRevisionUpdate;


					COntologyRevisionPersistencer* mOntoRevPersistencer;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REVISION_CSPONTOLOGYREVISIONMANAGER_H
