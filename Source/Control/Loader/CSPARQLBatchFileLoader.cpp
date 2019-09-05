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

#include "CSPARQLBatchFileLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CSPARQLBatchFileLoader::CSPARQLBatchFileLoader() {
				mSPARQLInterpreter = 0;
				mLoaderConfig = 0;
				mConfLogProcessingTimes = false;
				
				mStreamSPARQLHeader = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<sparql xmlns=\"http://www.w3.org/2005/sparql-results#\" xml:base=\"http://www.w3.org/2005/sparql-results#\" xmlns:owl=\"http://www.w3.org/2002/07/owl#\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\">").toLocal8Bit();
				mStreamSPARQLFooter = QString("\r\n</sparql>").toLocal8Bit();
			}



			CSPARQLBatchFileLoader::~CSPARQLBatchFileLoader() {
			}


			CLoader *CSPARQLBatchFileLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				mLoaderConfig = config;
				CConfigData *data = config->getLastConfigChange("Konclude.SPARQL.RequestFile");
				if (data) {
					reqFileString = data->getString();
				}

				data = config->getLastConfigChange("Konclude.SPARQL.ResponseFile");
				if (data) {
					resFileString = data->getString();
				}


				mConfDirectConsoleOutput = false;
				data = config->getLastConfigChange("Konclude.SPARQL.WriteResponseToStandardOutput");
				if (data) {
					mConfDirectConsoleOutput = data->getBoolean();
				}
				mConfDirectErrorOutput = false;
				data = config->getLastConfigChange("Konclude.SPARQL.WriteResponseToStandardError");
				if (data) {
					mConfDirectErrorOutput = data->getBoolean();
				}
				mConfCloseAfterOutput = false;
				data = config->getLastConfigChange("Konclude.SPARQL.CloseAfterProcessedRequest");
				if (data) {
					mConfCloseAfterOutput = data->getBoolean();
				}
				mBlockUntilProcessed = false;
				data = config->getLastConfigChange("Konclude.SPARQL.BlockUntilProcessedRequest");
				if (data) {
					mBlockUntilProcessed = data->getBoolean();
				}
				mConfLogProcessingTimes = false;
				data = config->getLastConfigChange("Konclude.SPARQL.LoggingExtendedProcessingTimes");
				if (data) {
					mConfLogProcessingTimes = data->getBoolean();
				}

				reasonerCommander = CConfigManagerReader::readCommanderManagerConfig(config);

				return this;
			}



			CLoader *CSPARQLBatchFileLoader::load() {
				startProcessing();
				if (mBlockUntilProcessed) {
					mBlockingSem.acquire();
				}
				return this;
			}



			CConfiguration *CSPARQLBatchFileLoader::getConfiguration() {
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



			bool CSPARQLBatchFileLoader::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (COWLlinkProcessor::processCustomsEvents(type, event)) {
					return true;
				} else if (type == CResultStreamingWriteEvent::EVENTTYPE) {
					CResultStreamingWriteEvent* rswe = (CResultStreamingWriteEvent*)event;
					QByteArray* buffer = rswe->getBuffer();
					bool last = rswe->isLast();
					writeStreamDataToFile(buffer, last);
					return true;
				}
				return false;
			}


			COWLlinkProcessor *CSPARQLBatchFileLoader::initializeOWLlinkContent() {
				mResponseTime.start();
				if (!reqFileString.isEmpty()) {
					QFile file(reqFileString);
					LOG(INFO,getLogDomain(),logTr("Processing SPARQL request from file '%1'.").arg(reqFileString),this);
					if (file.open(QIODevice::ReadOnly)) {
						QString fileContentString = file.readAll();

						mSPARQLInterpreter = new CSPARQLRecordResultStreamingInterpreter(this, preSynchronizer, mLoaderConfig);
						defaultCommandDelegater = mSPARQLInterpreter;
						preSynchronizer = new CPreconditionSynchronizer(mSPARQLInterpreter);

						CParseProcessSPARQLTextCommand* parseProcSPARQLCommand = new CParseProcessSPARQLTextCommand(fileContentString);
						CCommandProcessedCallbackEvent *proComm = new CCommandProcessedCallbackEvent(this, parseProcSPARQLCommand);
						parseProcSPARQLCommand->setRecorder(mSPARQLInterpreter);
						parseProcSPARQLCommand->addProcessedCallback(proComm);
						defaultCommandDelegater->delegateCommand(parseProcSPARQLCommand);

						mWritingStarted = false;
						mChunkPart = 0;
						mResponseFile = nullptr;
						mWritingFailed = false;


						if (mConfLogProcessingTimes) {
							cint64 readMilliSeconds = mResponseTime.elapsed();
							LOG(INFO,getLogDomain(),logTr("Read XML content in %1 ms.").arg(readMilliSeconds),this);
						}

					} else {
						LOG(ERROR,getLogDomain(),logTr("SPARQL request file '%1' not found.").arg(reqFileString),this);
						concludeOWLlinkContent();
					}
				}	
				return this;
			}



			bool CSPARQLBatchFileLoader::writeStreamData(QByteArray* buffer, bool last) {
				postEvent(new CResultStreamingWriteEvent(buffer, last));
				return !mWritingFailed;
			}

			CSPARQLStreamingWriter* CSPARQLBatchFileLoader::writeStreamDataToFile(QByteArray* buffer, bool last) {
				++mChunkPart;
				// write with chunked encoding
				bool firstChunk = false;
				if (!mWritingStarted) {
					mWritingStarted = true;
					if (!resFileString.isEmpty()) {
						mResponseFile = new QFile(resFileString);
						if (!mResponseFile->open(QIODevice::WriteOnly)) {
							LOG(ERROR, getLogDomain(), logTr("Opening file '%1' for writing failed.").arg(resFileString), this);
							delete mResponseFile;
							mResponseFile = nullptr;
						} else {
							mResponseFile->write(mStreamSPARQLHeader);
						}
					}
					firstChunk = true;
				}
				if (mResponseFile) {
					mResponseFile->write(*buffer);
				} else {
					if (!mConfDirectConsoleOutput && !mConfDirectErrorOutput) {
						// answers are not further used, no need to continue computation
						mWritingFailed = true;
					}
				}
				if (mConfDirectConsoleOutput) {
					std::cout << buffer->data();
				}
				if (mConfDirectErrorOutput) {
					std::cout << buffer->data();
				}
				if (last) {
					if (mResponseFile) {
						mResponseFile->write(mStreamSPARQLFooter);
						LOG(INFO, getLogDomain(), logTr("Last part of %1 with %2 bytes written.").arg(mChunkPart).arg(buffer->size()), this);
						mResponseFile->close();
					}
				} else {
					if (mResponseFile) {
						LOG(INFO, getLogDomain(), logTr("Writing part %1 with %2 bytes to file.").arg(mChunkPart).arg(buffer->size()), this);
						mResponseFile->flush();
					}
				}
				delete buffer;
				return this;
			}



			COWLlinkProcessor *CSPARQLBatchFileLoader::concludeOWLlinkContent() {
				if (mConfLogProcessingTimes) {
					cint64 responseMilliSeconds = mResponseTime.elapsed();
					LOG(INFO,getLogDomain(),logTr("SPARQL request processed in %1 ms.").arg(responseMilliSeconds),this);
				}
				if (mBlockUntilProcessed) {
					mBlockingSem.release();
				}
				if (mConfCloseAfterOutput) {
					CLogger::getInstance()->waitSynchronization();
					//qSleep(10);
					//qWait();
					QCoreApplication::exit();
				}
				return this;
			}


			CLoader *CSPARQLBatchFileLoader::exit() {

				stopThread();

				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
