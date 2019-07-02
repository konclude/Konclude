/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_Control_COMMAND_CCOMMAND_H
#define KONCLUDE_Control_COMMAND_CCOMMAND_H

// Libraries includes
#include <QString>

// Namespace includes
#include "CommandSettings.h"
#include "CCommandRecorder.h"
#include "CCommandRecordData.h"

// Other includes
#include "Concurrent/Callback/CSynchronizedCallbackListExecuter.h"

#include "Utilities/CLinker.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Logger;
	using namespace Concurrent::Callback;
	using namespace Utilities;

	namespace Control {

		namespace Command {

			/*! 
			 *
			 *		\class		CCommand
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCommand {
				// public methods
				public:
					//! Constructor
					CCommand(CCommand *parentSuperCommand = 0);

					//! Destructor
					virtual ~CCommand();
					
					virtual CLinker<CCommand *> *getSubCommandLinker();
					virtual CCommand *addSubCommand(CCommand *subCommand);
					virtual CCommand *makeToSubCommand(CCommand *subCommand);
					virtual CCommand *getSuperCommand();
					virtual CCommand *setSuperCommand(CCommand *parentSuperCommand);

					virtual qint64 getCommandTag() = 0;
					virtual bool isProcessable() = 0;

					virtual QString getBriefCommandDescription();
					virtual CCommand *getNextProcessableSubCommand();

					virtual CCommand *setProcessed(bool processedCommand = true);
					virtual bool isProcessed();
					virtual bool areAllSubCommandsProcessed();
					virtual bool isThisCommandProcessed();

					virtual CCommand *setProcessing(bool processingCommand = true);
					virtual bool isProcessing();

					virtual CCommandRecordData *getRecordData();
					virtual CCommand *setRecordData(CCommandRecordData *takeRecordData);
					virtual CCommand *overwriteRecordData(CCommandRecordData *takeRecordData);
					virtual CCommand *appendRecordData(CCommandRecordData *takeRecordData);

					virtual CCommandRecorder *getRecorder();
					virtual CCommand *setRecorder(CCommandRecorder *useCommandRecorder);

					virtual CCommand *deleteDependencies();
					virtual CCommand *deleteSubCommands();
					virtual CCommand *deleteRecordData();

					virtual CCommand *addProcessedCallback(CCallbackData *callback);
					virtual CCommand *checkProcessedCallback();

					virtual double getMaxErrorLevel();
					virtual CCommand *reportError(double errorLevelReport);

					virtual bool hasReportErrorFromSubCommands();
					virtual CCommand *setReportErrorFromSubCommands(bool reportSubCommandErrors);

				// protected methods
				protected:

				// protected variables
				protected:
					CLinker<CCommand *> *subCommandLinker;
					CCommand *superCommand;
					CCommandRecordData *recordData;
					CCommandRecorder *recorder;

					QMutex closureSyncMutex;
					QMutex errorSyncMutex;

					bool processed;
					bool processing;
					bool closured;
					bool reportErrorFromSubCommands;

					CSynchronizedCallbackListExecuter processedCallbackExecuter;

					double errorLevel;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_CCOMMAND_H
