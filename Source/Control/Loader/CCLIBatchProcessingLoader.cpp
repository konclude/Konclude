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

#include "CCLIBatchProcessingLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CCLIBatchProcessingLoader::CCLIBatchProcessingLoader() : mLogIdentifier("::Konclude::CLIBatchProcessor",this) {
				mOWLLinkInterpreter = nullptr;
				mLoaderConfig = nullptr;
				mConfLogProcessingTimes = false;
				mProcessingCommandData = nullptr;
				mLogTotalProcessingTime = false;

			}



			CCLIBatchProcessingLoader::~CCLIBatchProcessingLoader() {
			}



			CCommandRecorder* CCLIBatchProcessingLoader::recordData(CCommandRecordData *recData) {
				LOGC(recData->getRecordErrorLevel(),recData->getRecordDomain(),recData->getRecordMessage(),recData->getCommand());
				return this;
			}



			CLoader *CCLIBatchProcessingLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				mLoaderConfig = config;
				reasonerCommander = CConfigManagerReader::readCommanderManagerConfig(config);

				mCloseAfterOutput = CConfigDataReader::readConfigBoolean(mLoaderConfig, "Konclude.CLI.CloseAfterProcessedRequest", true);
				mBlockUntilProcessed = CConfigDataReader::readConfigBoolean(mLoaderConfig, "Konclude.CLI.BlockUntilProcessedRequest", true);


				mRequestFileString = CConfigDataReader::readConfigString(config,"Konclude.CLI.RequestFile");
				mResponseFileString = CConfigDataReader::readConfigString(config,"Konclude.CLI.ResponseFile");
				mIRINameString = CConfigDataReader::readConfigString(config,"Konclude.CLI.IRIName");
				mConfLogProcessingTimes = mLogTotalProcessingTime = CConfigDataReader::readConfigBoolean(config,"Konclude.OWLlink.LoggingExtendedProcessingTimes");

				return this;
			}



			CLoader *CCLIBatchProcessingLoader::load() {
				startProcessing();
				if (mBlockUntilProcessed) {
					mBlockingSemaphore.acquire();
				}
				return this;
			}



			CConfiguration *CCLIBatchProcessingLoader::getConfiguration() {
				if (!reasonerCommander) {
					return mLoaderConfig;
				} else {
					CGetDescriptionCommand* getDesComm = new CGetDescriptionCommand();
					reasonerCommander->delegateCommand(getDesComm);
					CCommandExecutedBlocker commExeBlocker;
					commExeBlocker.waitExecutedCommand(getDesComm);
					CConfiguration* config = getDesComm->getConfiguration();
					return config;
				}
			}
			
			
			void CCLIBatchProcessingLoader::createTestingCommands() {
			}



			void CCLIBatchProcessingLoader::addProcessingCommand(CCommand* command) {
				mProcessCommandList.append(new CCommandProcessData(command));
			}

			void CCLIBatchProcessingLoader::addProcessingCommand(CCommand* command, bool measureTime, const QString& measuredOutputString) {
				mProcessCommandList.append(new CCommandProcessData(command,measureTime,measuredOutputString));
			}


			void CCLIBatchProcessingLoader::addProcessingCommand(CCommand* command, bool measureTime, const QString& measuredOutputString, bool writeSatisfiabilityOutput, const QString& outputFileName) {
				mProcessCommandList.append(new CCommandProcessData(command,measureTime,measuredOutputString,writeSatisfiabilityOutput,outputFileName));
			}


			COWLlinkProcessor *CCLIBatchProcessingLoader::initializeOWLlinkContent() {
				if (mLogTotalProcessingTime) {
					mTotalTime.start();
				}
				createTestingCommands();
				return this;
			}

			COWLlinkProcessor *CCLIBatchProcessingLoader::concludeOWLlinkContent() {
				finishCommandProcessing();
				if (hasMoreProcessingCommands()) {
					processNextCommand();
				} else {
					finishProcessing();
					terminateProcessing();
				}
				return this;
			}


			void CCLIBatchProcessingLoader::logOutputError(const QString& errorString) {
				LOG(ERROR,mLogIdentifier.getLogDomain(),errorString,this);
			}
			void CCLIBatchProcessingLoader::logOutputNotice(const QString& outputString) {
				LOG(NOTICE,mLogIdentifier.getLogDomain(),outputString,this);
			}

			void CCLIBatchProcessingLoader::logOutputMessage(const QString& outputString) {				
				LOG(INFO,mLogIdentifier.getLogDomain(),outputString,this);
			}

			void CCLIBatchProcessingLoader::forcedPathCreated(const QString& filePath) {				
				QString path = filePath;
				if (path.contains("/") || path.contains("\\")) {
					int lastSlash = path.lastIndexOf("/");
					int lastBackSlash = path.lastIndexOf("\\");
					int lastSeparator = qMax(lastBackSlash,lastSlash);
					path = path.mid(0,lastSeparator);
					if (!path.isEmpty()) {
						QDir dir;
						dir.mkpath(path);
					}
				}
			}

			void CCLIBatchProcessingLoader::writeCommandOutput(const QString& outputFileName, CCommand* processedCommand) {
			}

			void CCLIBatchProcessingLoader::finishCommandProcessing() {
				if (mProcessingCommandData) {
					if (mProcessingCommandData->mMeasureTime) {
						QString outputString = mProcessingCommandData->mMeasuredOutputString;
						cint64 timeElapsed = mMeasurementTime.elapsed();
						if (mOutputMeasuredTimeInSeconds) {
							logOutputMessage(outputString.arg(timeElapsed/1000.));
						} else {
							logOutputMessage(outputString.arg(timeElapsed));
						}
					}
					if (mProcessingCommandData->mWriteOutput) {
						QString outputFile = mProcessingCommandData->mWriteOutputFile;
						writeCommandOutput(outputFile,mProcessingCommandData->mCommand);
					}
					mProcessingCommandData = nullptr;
				}
			}

			void CCLIBatchProcessingLoader::processNextCommand() {
				mProcessingCommandData = mProcessCommandList.takeFirst();
				CCommand* command = mProcessingCommandData->mCommand;
				logOutputNotice(QString("Starting '%1' command").arg(command->getBriefCommandDescription()));
				command->setRecorder(this);
				command->setReportErrorFromSubCommands(false);
				CCommandProcessedCallbackEvent *proComm = new CCommandProcessedCallbackEvent(this,command);
				command->addProcessedCallback(proComm);
				mMeasurementTime.start();
				preSynchronizer->delegateCommand(command);
			}

			bool CCLIBatchProcessingLoader::hasMoreProcessingCommands() {
				return !mProcessCommandList.isEmpty();
			}

			void CCLIBatchProcessingLoader::finishProcessing() {
			}

			void CCLIBatchProcessingLoader::terminateProcessing() {

				if (mLogTotalProcessingTime) {
					cint64 timeElapsed = mTotalTime.elapsed();
					logOutputMessage(QString("Total processing time: %1 ms.").arg(timeElapsed));
				}

				if (mBlockUntilProcessed) {
					mBlockingSemaphore.release();
				}

				if (mCloseAfterOutput) {
					CLogger::getInstance()->waitSynchronization();
                    QCoreApplication::exit();
				}
			}



			CLoader *CCLIBatchProcessingLoader::exit() {
				stopThread();
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
