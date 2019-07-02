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

#ifndef KONCLUDE_Control_COMMAND_CCOMMANDERMANAGERTHREAD_H
#define KONCLUDE_Control_COMMAND_CCOMMANDERMANAGERTHREAD_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CCommand.h"
#include "CCommandRecordRouter.h"
#include "CCommanderManager.h"
#include "CReasonerCommanderGlobalConfiguration.h"

// Other includes
#include "Control/Command/Records/CStartProcessCommandRecord.h"
#include "Control/Command/Records/CStopProcessCommandRecord.h"
#include "Control/Command/Records/CFinishProcessCommandRecord.h"
#include "Control/Command/Records/CUnspecifiedMessageErrorRecord.h"
#include "Control/Command/Records/CUnspecifiedMessageInformationRecord.h"

#include "Reasoner/Kernel/Manager/CReasonerManager.h"
#include "Reasoner/Kernel/Manager/CAnalyseReasonerManager.h"
#include "Reasoner/Kernel/Manager/CExperimentalReasonerManager.h"

#include "Reasoner/Revision/CSPOntologyRevisionManager.h"

#include "Control/Command/Instructions/CInitializeConfigurationCommand.h"
#include "Control/Command/Instructions/CInitializeReasonerCommand.h"
#include "Control/Command/Instructions/CGetDescriptionCommand.h"
#include "Control/Command/Instructions/CSkipUnsupportedCommandCommand.h"
#include "Control/Command/Instructions/CKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CCalculateQueryCommand.h"
#include "Control/Command/Instructions/CGetReasonerCommand.h"
#include "Control/Command/Instructions/CGetClassificationManagerCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseConfigCommand.h"
#include "Control/Command/Instructions/CSetKnowledgeBaseConfigCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseSettingsCommand.h"
#include "Control/Command/Instructions/CPreprocessKnowledgeBaseRequirementsForQueryCommand.h"

#include "Control/Command/Events/CCommandCalculatedQueryCallbackEvent.h"
#include "Control/Command/Events/COntologyPreparedCallbackEvent.h"

#include "Events/CRealizeCommandEvent.h"

#include "Config/CConfiguration.h"

#include "Concurrent/CThread.h"

#include "Logger/Record/CContextRecorder.h"
#include "Logger/CLogIdentifier.h"

#include "Reasoner/Classifier/CClassificationManager.h"
#include "Reasoner/Classifier/CSubsumptionClassifierFactory.h"
#include "Reasoner/Classifier/CConfigDependedSubsumptionClassifierFactory.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Reasoner;
	using namespace Kernel::Manager;
	using namespace Revision;

	using namespace Concurrent;
	using namespace Logger;
	using namespace Logger::Record;
	using namespace Config;

	namespace Control {

		namespace Command {

			using namespace Events;
			using namespace Records;

			/*! 
			 *
			 *		\class		CCommanderManagerThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCommanderManagerThread : public CCommanderManager, public CThread, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					CCommanderManagerThread();

					//! Destructor
					virtual ~CCommanderManagerThread();


					virtual CCommanderManagerThread *realizeCommand(CCommand *command);

					

				// protected methods
				protected:
					bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

					virtual void initializeConfiguration(CConfiguration *initConfig, CCommandRecordRouter *commandRecordRouter);
					virtual void initializeCommander(CCommanderInitializationFactory *initializationFactory, CCommandRecordRouter *commandRecordRouter);

				// protected variables
				protected:
					CReasonerCommanderGlobalConfiguration *configurationProvider;

					CReasonerManager *reasonerManager;

					CCommandDelegater *kbCommandsDelegater;

					CClassificationManager *classificationMan;

					COntologyRevisionManager *ontoRevMan;

					

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CCOMMANDERMANAGERTHREAD_H
