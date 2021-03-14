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
				if (mWriteLimitSemaphore) {
					delete mWriteLimitSemaphore;
				}
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


				cint64 writeBufferBlockingLimit = CConfigDataReader::readConfigInteger(mLoaderConfig, "Konclude.SPARQL.File.WriteBufferBlockingLimit", 20);
				if (writeBufferBlockingLimit > 0) {
					mWriteLimitSemaphore = new QSemaphore(writeBufferBlockingLimit);
				} else {
					mWriteLimitSemaphore = nullptr;
				}
				mBlockingWarned = false;


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
					QList<CSPARQLResultBufferWriteData>* bufferList = rswe->getBufferList();
					bool last = rswe->isLast();
					writeStreamDataToFile(bufferList, last);
					mWriteLimitSemaphore->release(1);
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

						if (!resFileString.isEmpty()) {
							mSPARQLInterpreter = new CSPARQLRecordResultStreamingInterpreter(this, preSynchronizer, mLoaderConfig);
						} else {
							LOG(WARN, getLogDomain(), logTr("No output file given, requesting only answer counting."), this);
							mSPARQLInterpreter = new CSPARQLRecordResultStreamingInterpreter(nullptr, preSynchronizer, mLoaderConfig);
						}
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
						mWritingAppendingMode = false;


						if (mConfLogProcessingTimes) {
							cint64 readMilliSeconds = mResponseTime.elapsed();
							LOG(INFO,getLogDomain(),logTr("Read XML content in %1 ms.").arg(readMilliSeconds),this);
						}

					} else {
						LOG(ERROR,getLogDomain(),logTr("SPARQL request file '%1' not found.").arg(reqFileString),this);
						// seems to be required to preventing crashes
						// TODO: fix 
						this->usleep(200);
						concludeOWLlinkContent();
					}
				}	
				return this;
			}



			bool CSPARQLBatchFileLoader::writeStreamData(const QList<CSPARQLResultBufferWriteData>& bufferList, bool last) {
				if (mWriteLimitSemaphore) {
					if (!mBlockingWarned && mWriteLimitSemaphore->available() <= 2) {
						mBlockingWarned = true;
						LOG(WARN, getLogDomain(), logTr("Result writing buffer limit almost reached, result generation may be blocked due to slow file system write access."), this);
					}
					mWriteLimitSemaphore->acquire(1);
				}
				postEvent(new CResultStreamingWriteEvent(bufferList, last));
				return !mWritingFailed;
			}

			CSPARQLStreamingWriter* CSPARQLBatchFileLoader::writeStreamDataToFile(QList<CSPARQLResultBufferWriteData>* bufferList, bool last) {
				++mChunkPart;
				// write with chunked encoding
				bool firstChunk = false;
				if (!mWritingStarted) {
					mWritingStarted = true;
					if (!resFileString.isEmpty()) {
						mResponseFile = new QFile(resFileString);
						bool fileOpen = false;
						if (!mWritingAppendingMode) {
							fileOpen = mResponseFile->open(QIODevice::WriteOnly);
						} else {
							fileOpen = mResponseFile->open(QIODevice::Append);
							if (fileOpen) {
								mResponseFile->write(QString("\r\n\r\n").toLocal8Bit());
							}
						}
						if (!fileOpen) {
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
					for (CSPARQLResultBufferWriteData bufferData : *bufferList) {
						for (cint64 i = bufferData.getWriteCount(); i > 0; --i) {							
							mResponseFile->write(*bufferData.getBuffer());
						}
					}
				} else {
					if (!mConfDirectConsoleOutput && !mConfDirectErrorOutput) {
						// answers are not further used, no need to continue computation
						mWritingFailed = true;
					}
				}
				if (mConfDirectConsoleOutput) {
					for (CSPARQLResultBufferWriteData bufferData : *bufferList) {
						for (cint64 i = bufferData.getWriteCount(); i > 0; --i) {
							std::cout << bufferData.getBuffer();
						}
					}
				}
				if (mConfDirectErrorOutput) {
					for (CSPARQLResultBufferWriteData bufferData : *bufferList) {
						for (cint64 i = bufferData.getWriteCount(); i > 0; --i) {
							std::cout << bufferData.getBuffer();
						}
					}
				}
				cint64 bufferSize = 0;
				for (CSPARQLResultBufferWriteData bufferData : *bufferList) {
					bufferSize += bufferData.getBuffer()->size() * bufferData.getWriteCount();
					delete bufferData.getBuffer();
				}
				if (last) {
					if (mResponseFile) {
						mResponseFile->write(mStreamSPARQLFooter);
						LOG(INFO, getLogDomain(), logTr("Last part of %1 with %2 bytes written.").arg(mChunkPart).arg(bufferSize), this);
						mResponseFile->close();
						mWritingAppendingMode = true;
						mWritingStarted = false;
						delete mResponseFile;
					}
				} else {
					if (mResponseFile) {
						LOG(INFO, getLogDomain(), logTr("Writing part %1 with %2 bytes to file.").arg(mChunkPart).arg(bufferSize), this);
						mResponseFile->flush();
					}
				}
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
