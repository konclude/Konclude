/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "COWLlinkBatchFileLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			COWLlinkBatchFileLoader::COWLlinkBatchFileLoader() {
				owllinkInterpreter = 0;
				loaderConfig = 0;
				mConfLogProcessingTimes = false;
			}



			COWLlinkBatchFileLoader::~COWLlinkBatchFileLoader() {
			}


			CLoader *COWLlinkBatchFileLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				loaderConfig = config;
				CConfigData *data = config->getLastConfigChange("Konclude.OWLlink.RequestFile");
				if (data) {
					reqFileString = data->getString();
				}

				data = config->getLastConfigChange("Konclude.OWLlink.ResponseFile");
				if (data) {
					resFileString = data->getString();
				}


				directConsoleOutput = false;
				data = config->getLastConfigChange("Konclude.OWLlink.WriteResponseToStandardOutput");
				if (data) {
					directConsoleOutput = data->getBoolean();
				}
				directErrorOutput = false;
				data = config->getLastConfigChange("Konclude.OWLlink.WriteResponseToStandardError");
				if (data) {
					directErrorOutput = data->getBoolean();
				}
				closeAfterOutput = false;
				data = config->getLastConfigChange("Konclude.OWLlink.CloseAfterProcessedRequest");
				if (data) {
					closeAfterOutput = data->getBoolean();
				}
				blockUntilProcessed = false;
				data = config->getLastConfigChange("Konclude.OWLlink.BlockUntilProcessedRequest");
				if (data) {
					blockUntilProcessed = data->getBoolean();
				}
				mConfLogProcessingTimes = false;
				data = config->getLastConfigChange("Konclude.OWLlink.LoggingExtendedProcessingTimes");
				if (data) {
					mConfLogProcessingTimes = data->getBoolean();
				}
				data = config->getLastConfigChange("Konclude.OWLlink.SAXBasedLoadOntologiesParsing");
				if (data) {
					mConfLoadOntologiesSAXParsing = data->getBoolean();
				}

				reasonerCommander = CConfigManagerReader::readCommanderManagerConfig(config);

				return this;
			}



			CLoader *COWLlinkBatchFileLoader::load() {
				startProcessing();
				if (blockUntilProcessed) {
					blockingSem.acquire();
				}
				return this;
			}



			CConfiguration *COWLlinkBatchFileLoader::getConfiguration() {
				if (!reasonerCommander) {
					return loaderConfig;
				} else {
					CGetDescriptionCommand *getDesComm = new CGetDescriptionCommand();
					reasonerCommander->delegateCommand(getDesComm);
					CCommandExecutedBlocker commExeBlocker;
					commExeBlocker.waitExecutedCommand(getDesComm);
					CConfiguration *config = getDesComm->getConfiguration();
					return config;
				}
			}



			COWLlinkProcessor *COWLlinkBatchFileLoader::initializeOWLlinkContent() {
				mResponseTime.start();
				if (!reqFileString.isEmpty()) {
					QFile file(reqFileString);
					LOG(INFO,getLogDomain(),logTr("Processing OWLlink request from file '%1'.").arg(reqFileString),this);
					if (file.open(QIODevice::ReadOnly)) {
						fileContent = file.readAll();
						COWLlinkQtXMLCommandParser *owllinkCommandParser = new COWLlinkQtXMLCommandParser();
						owllinkInterpreter = nullptr;
						if (!resFileString.isEmpty()) {
							owllinkInterpreter = new COWLLinkRecordInterpreter(preSynchronizer,loaderConfig);
							defaultCommandDelegater = owllinkInterpreter;
						} else {
							defaultCommandDelegater = preSynchronizer;
						}

						CParseOWLlinkCommandsCommand *parseCommand = new CParseOWLlinkCommandsCommand(&fileContent,reqFileString);
						parseCommand->setRecorder(owllinkInterpreter);
						parseCommand->setReportErrorFromSubCommands(false);

						CCommandProcessedCallbackEvent *proComm = new CCommandProcessedCallbackEvent(this,parseCommand);
						parseCommand->addProcessedCallback(proComm);


						owllinkCommandParser->realizeCommand(parseCommand,defaultCommandDelegater);

						delete owllinkCommandParser;

						if (mConfLogProcessingTimes) {
							cint64 readMilliSeconds = mResponseTime.elapsed();
							LOG(INFO,getLogDomain(),logTr("Read XML content in %1 ms.").arg(readMilliSeconds),this);
						}

					} else {
						LOG(ERROR,getLogDomain(),logTr("OWLlink request file '%1' not found.").arg(reqFileString),this);
						concludeOWLlinkContent();
					}
				}	
				return this;
			}



			COWLlinkProcessor *COWLlinkBatchFileLoader::concludeOWLlinkContent() {
				if (owllinkInterpreter) {
					QByteArray fileContent(owllinkInterpreter->getByteArray());
					if (!resFileString.isEmpty()) {
						QFile file(resFileString);
						if (file.open(QIODevice::WriteOnly)) {
							file.write(fileContent);
							file.close();
						}
					}
					if (directConsoleOutput) {
						std::cout<<fileContent.data();
					}
					if (directErrorOutput) {
						std::cerr<<fileContent.data();
					}
				}
				if (mConfLogProcessingTimes) {
					cint64 responseMilliSeconds = mResponseTime.elapsed();
					LOG(INFO,getLogDomain(),logTr("OWLlink request processed in %1 ms.").arg(responseMilliSeconds),this);
				}
				if (blockUntilProcessed) {
					blockingSem.release();
				}
				if (closeAfterOutput) {
					CLogger::getInstance()->waitSynchronization();
					//qSleep(10);
					//qWait();
                    QCoreApplication::exit();
				}
				return this;
			}


			CLoader *COWLlinkBatchFileLoader::exit() {

				stopThread();

				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
