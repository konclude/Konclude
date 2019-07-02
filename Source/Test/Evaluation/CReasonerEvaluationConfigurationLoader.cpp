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

#include "CReasonerEvaluationConfigurationLoader.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {


			CReasonerEvaluationConfigurationLoader::CReasonerEvaluationConfigurationLoader() {
				mLoaderConfig = nullptr;
				mConfLogProcessingTimes = false;
			}



			CReasonerEvaluationConfigurationLoader::~CReasonerEvaluationConfigurationLoader() {
			}


			bool CReasonerEvaluationConfigurationLoader::loadConfiguration(const QString& owllinkConfigFile, CConfiguration *config) {
				mLoaderConfig = config;
				mReqFileString = owllinkConfigFile;

				mBlockUntilProcessed = true;
				mConfLogProcessingTimes = false;
				CConfigData* data = config->getLastConfigChange("Konclude.OWLlink.LoggingExtendedProcessingTimes");
				if (data) {
					mConfLogProcessingTimes = data->getBoolean();
				}

				reasonerCommander = CConfigManagerReader::readCommanderManagerConfig(config);

				mConfigSucessfullyLoaded = false;

				startProcessing();
				if (mBlockUntilProcessed) {
					mBlockingSem.acquire();
				}
				stopThread();
				return mConfigSucessfullyLoaded;
			}




			CConfiguration* CReasonerEvaluationConfigurationLoader::getConfiguration() {
				if (!reasonerCommander) {
					return mLoaderConfig;
				} else {
					CGetDescriptionCommand *getDesComm = new CGetDescriptionCommand();
					reasonerCommander->delegateCommand(getDesComm);
					CCommandExecutedBlocker commExeBlocker;
					commExeBlocker.waitExecutedCommand(getDesComm);
					CConfiguration *config = getDesComm->getConfiguration();
					return config;
				}
			}



			COWLlinkProcessor* CReasonerEvaluationConfigurationLoader::initializeOWLlinkContent() {
				mResponseTime.start();
				if (!mReqFileString.isEmpty()) {
					QFile file(mReqFileString);
					LOG(INFO,getLogDomain(),logTr("Processing OWLlink configuration request from file '%1'.").arg(mReqFileString),this);
					if (file.open(QIODevice::ReadOnly)) {
						fileContent = file.readAll();
						COWLlinkQtXMLCommandParser *owllinkCommandParser = new COWLlinkQtXMLCommandParser(mLoaderConfig);
						defaultCommandDelegater = preSynchronizer;

						CParseOWLlinkCommandsCommand *parseCommand = new CParseOWLlinkCommandsCommand(&fileContent,mReqFileString);
						parseCommand->setReportErrorFromSubCommands(false);

						CCommandProcessedCallbackEvent *proComm = new CCommandProcessedCallbackEvent(this,parseCommand);
						parseCommand->addProcessedCallback(proComm);


						owllinkCommandParser->realizeCommand(parseCommand,preSynchronizer);

						delete owllinkCommandParser;

						if (mConfLogProcessingTimes) {
							cint64 readMilliSeconds = mResponseTime.elapsed();
							LOG(INFO,getLogDomain(),logTr("Read XML content in %1 ms.").arg(readMilliSeconds),this);
						}
						file.close();

						mConfigSucessfullyLoaded = true;

					} else {
						LOG(ERROR,getLogDomain(),logTr("OWLlink configuration request file '%1' not found.").arg(mReqFileString),this);
						concludeOWLlinkContent();
					}
				}	
				return this;
			}



			COWLlinkProcessor* CReasonerEvaluationConfigurationLoader::concludeOWLlinkContent() {
				if (mConfLogProcessingTimes) {
					cint64 responseMilliSeconds = mResponseTime.elapsed();
					LOG(INFO,getLogDomain(),logTr("OWLlink request processed in %1 ms.").arg(responseMilliSeconds),this);
				}
				if (mBlockUntilProcessed) {
					mBlockingSem.release();
				}
				return this;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
