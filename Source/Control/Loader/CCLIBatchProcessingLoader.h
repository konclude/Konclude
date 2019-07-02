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

#ifndef KONCLUDE_CONTROL_LOADER_CCLIBATCHPROCESSINGLOADER_H
#define KONCLUDE_CONTROL_LOADER_CCLIBATCHPROCESSINGLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>
#include <QTime>
#include <QDir>

// Namespace includes
#include "LoaderSettings.h"
#include "CLoader.h"
#include "CLoaderFactory.h"

// Other includes
#include "Control/Interface/OWLlink/COWLlinkProcessor.h"
#include "Control/Interface/OWLlink/COWLLinkRecordInterpreter.h"

#include "Control/Command/CConfigManagerReader.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"
#include "Logger/COREConsolePrintLogObserver.h"
#include "Logger/COREFilePrintLogObserver.h"


#include <stdio.h>
#include <iostream>


namespace Konclude {

	using namespace Logger;

	namespace Control {

		using namespace Interface::OWLlink;
		using namespace Command;

		namespace Loader {

			/*! 
			 *
			 *		\class		CCLIBatchProcessingLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CCLIBatchProcessingLoader : public CLoader, public COWLlinkProcessor, public CCommandRecorder {
				// public methods
				public:
					//! Constructor
					CCLIBatchProcessingLoader();

					//! Destructor
					virtual ~CCLIBatchProcessingLoader();


					virtual CLoader *init(CLoaderFactory *loaderFactory = 0, CConfiguration *config = 0);
					virtual CLoader *load();
					virtual CLoader *exit();

					virtual CCommandRecorder *recordData(CCommandRecordData *recData);


				// protected methods
				protected:

					class CCommandProcessData {
						public:
							CCommandProcessData(CCommand* command){
								mCommand = command;
								mMeasureTime = false;
								mWriteOutput = false;
							}
							CCommandProcessData(CCommand* command, bool measureTime, const QString& measuredOutputString) {
								mCommand = command;
								mMeasureTime = measureTime;
								mMeasuredOutputString = measuredOutputString;
								mWriteOutput = false;
							}
							CCommandProcessData(CCommand* command, bool measureTime, const QString& measuredOutputString, bool writeOutput, const QString& outputFileName) {
								mCommand = command;
								mMeasureTime = measureTime;
								mMeasuredOutputString = measuredOutputString;
								mWriteOutput = writeOutput;
								mWriteOutputFile = outputFileName;
							}
						public:
							CCommand* mCommand;
							bool mMeasureTime;
							QString mMeasuredOutputString;
							bool mWriteOutput;
							QString mWriteOutputFile;
					};

					virtual COWLlinkProcessor* initializeOWLlinkContent();
					virtual COWLlinkProcessor* concludeOWLlinkContent();

					virtual CConfiguration* getConfiguration();

					virtual void createTestingCommands();

					void addProcessingCommand(CCommand* command);
					void addProcessingCommand(CCommand* command, bool measureTime, const QString& measuredOutputString);
					void addProcessingCommand(CCommand* command, bool measureTime, const QString& measuredOutputString, bool writeSatisfiabilityOutput, const QString& outputFileName);

					bool hasMoreProcessingCommands();
					void terminateProcessing();
					void processNextCommand();
					void finishCommandProcessing();
					virtual void finishProcessing();

					void logOutputNotice(const QString& outputString);
					void logOutputMessage(const QString& outputString);
					void logOutputError(const QString& errorString);

					void forcedPathCreated(const QString& filePath);

					virtual void writeCommandOutput(const QString& outputFileName, CCommand* processedCommand);

				// protected variables
				protected:
					QString mIRINameString;
					QString mRequestFileString;
					QString mResponseFileString;

					CLogIdentifier mLogIdentifier;

					QList<CCommandProcessData*> mProcessCommandList;
					CCommandProcessData* mProcessingCommandData;


					QTime mTotalTime;
					bool mLogTotalProcessingTime;
					bool mCloseAfterOutput;
					bool mOutputMeasuredTimeInSeconds;
					bool mBlockUntilProcessed;
					QTime mMeasurementTime;
					QSemaphore mBlockingSemaphore;
					COWLLinkRecordInterpreter* mOWLLinkInterpreter;

					CConfiguration* mLoaderConfig;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_LOADER_CCLIBATCHPROCESSINGLOADER_H
