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

#include "CReasonerEvaluationProgramLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CReasonerEvaluationProgramLoader::CReasonerEvaluationProgramLoader() {
				mReasonerCommander = nullptr;
				mDeleteReasonerCommander = nullptr;
				mLoaderConfig = nullptr;
			}



			CReasonerEvaluationProgramLoader::~CReasonerEvaluationProgramLoader() {
			}


			CLoader *CReasonerEvaluationProgramLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {

				mLoaderConfig = config;

				CConfigurationGroup* group = mLoaderConfig->getConfigurationGroup();

				CConfigData* data = mLoaderConfig->createConfig("Konclude.Execution.CommanderManager");
				CCommanderManagerConfigType* rCConfig = dynamic_cast<CCommanderManagerConfigType*>(data->getConfigType());
				if (rCConfig) {
					mReasonerCommander = dynamic_cast<CCommanderManagerThread*>(rCConfig->getCommanderManager());
				}

				if (!mReasonerCommander) {
					mDeleteReasonerCommander = mReasonerCommander = new CCommanderManagerThread();
					rCConfig->setCommanderManager(mReasonerCommander);
				}

				group->setConfigDefaultData(group->getConfigIndex("Konclude.Execution.CommanderManager"),data);


				data = config->getLastConfigChange("Konclude.Evaluation.ProgramFile");
				if (data) {
					mProgramFileString = data->getString();
				}

				mCloseAfterOutput = true;


				return this;
			}



			CLoader *CReasonerEvaluationProgramLoader::load() {
				if (mReasonerCommander) {
					mReasonerCommander->realizeCommand(new CInitializeConfigurationCommand(mLoaderConfig));
				}

				startProcessing();
				mBlockingSem.acquire();

				return this;
			}



			COWLlinkProcessor *CReasonerEvaluationProgramLoader::initializeOWLlinkContent() {
				if (!mProgramFileString.isEmpty()) {
					QFile file(mProgramFileString);
					LOG(INFO,getLogDomain(),logTr("Processing evaluation program from file '%1'.").arg(mProgramFileString),this);
					if (file.open(QIODevice::ReadOnly)) {
						fileContent = file.readAll();
						COWLlinkQtXMLCommandParser *owllinkCommandParser = new COWLlinkQtXMLCommandParser();
						defaultCommandDelegater = preSynchronizer;

						CParseOWLlinkCommandsCommand *parseCommand = new CParseOWLlinkCommandsCommand(&fileContent,mProgramFileString);
						parseCommand->setReportErrorFromSubCommands(false);

						CCommandProcessedCallbackEvent *proComm = new CCommandProcessedCallbackEvent(this,parseCommand);
						parseCommand->addProcessedCallback(proComm);


						owllinkCommandParser->realizeCommand(parseCommand,defaultCommandDelegater);

						delete owllinkCommandParser;

					} else {
						LOG(ERROR,getLogDomain(),logTr("Evaluation program file '%1' not found.").arg(mProgramFileString),this);
						concludeOWLlinkContent();
					}
				}	
				return this;
			}



			CConfiguration *CReasonerEvaluationProgramLoader::getConfiguration() {
				if (!mReasonerCommander) {
					return mLoaderConfig;
				} else {
					CGetDescriptionCommand *getDesComm = new CGetDescriptionCommand();
					mReasonerCommander->delegateCommand(getDesComm);
					CCommandExecutedBlocker commExeBlocker;
					commExeBlocker.waitExecutedCommand(getDesComm);
					CConfiguration *config = getDesComm->getConfiguration();
					return config;
				}
			}


			COWLlinkProcessor *CReasonerEvaluationProgramLoader::concludeOWLlinkContent() {
				mBlockingSem.release();
				if (mCloseAfterOutput) {
					CLogger::getInstance()->waitSynchronization();
					//qSleep(10);
					//qWait();
                    QCoreApplication::exit();
				}
				return this;
			}


			CLoader *CReasonerEvaluationProgramLoader::exit() {
				stopThread();
				delete mDeleteReasonerCommander;
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
