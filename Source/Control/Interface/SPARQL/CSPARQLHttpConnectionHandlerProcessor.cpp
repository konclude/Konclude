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

#include "CSPARQLHttpConnectionHandlerProcessor.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace SPARQL {


				CSPARQLHttpConnectionHandlerProcessor::CSPARQLHttpConnectionHandlerProcessor(CConfiguration* loaderConfig, CQtHttpConnectionHandlerReleaser* releaser) : COWLlinkProcessor(false) {
					mReleaser = releaser;
					mLoaderConfig = loaderConfig;
					reasonerCommander = CConfigManagerReader::readCommanderManagerConfig(mLoaderConfig);
					mSocket = nullptr;
					mQueued = false;
					mForceDisconnect = CConfigDataReader::readConfigBoolean(mLoaderConfig,"Konclude.SPARQL.Server.ForceDisconnectAfterProcessing",false);
					mProcessingCommand = nullptr;
					startThread();
					mThreadStartedSemaphore.acquire(1);
					mOriginPreSynchronizer = preSynchronizer;
					mDataWritten = false;


					mStreamSPARQLHeader = QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<sparql xmlns=\"http://www.w3.org/2005/sparql-results#\" xml:base=\"http://www.w3.org/2005/sparql-results#\" xmlns:owl=\"http://www.w3.org/2002/07/owl#\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\">").toLocal8Bit();
					mStreamSPARQLFooter = QString("\r\n</sparql>").toLocal8Bit();

				}


				CSPARQLHttpConnectionHandlerProcessor::~CSPARQLHttpConnectionHandlerProcessor() {
				}

				void CSPARQLHttpConnectionHandlerProcessor::threadStarted() {
					COWLlinkProcessor::threadStarted();

					mParser = new CQtHttpRequestBodyOnlyParser();
					mSocket = new QTcpSocket();
					mSocket->moveToThread(this);

					mProcessingRequest = false;
					mProcessMoreRead = false;

					connect(mSocket,SIGNAL(readyRead()),this,SLOT(connectionRead()));
					connect(mSocket,SIGNAL(disconnected()),this,SLOT(connectionDisconnect()));

					mThreadStartedSemaphore.release(1);
				}

				void CSPARQLHttpConnectionHandlerProcessor::threadStopped() {
					COWLlinkProcessor::threadStopped();

					mSocket->close();
					delete mSocket;
					delete mParser;
				}


				bool CSPARQLHttpConnectionHandlerProcessor::handleIncomingConnection(int socketDescriptor) {
					postEvent(new CHandleIncomingHttpConnectionEvent(socketDescriptor));
					return true;
				}


				bool CSPARQLHttpConnectionHandlerProcessor::isHandlerQueued() {
					return mQueued;
				}

				CSPARQLHttpConnectionHandlerProcessor* CSPARQLHttpConnectionHandlerProcessor::setHandlerQueued(bool queued) {
					mQueued = queued;
					return this;
				}



				bool CSPARQLHttpConnectionHandlerProcessor::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (COWLlinkProcessor::processCustomsEvents(type,event)) {
						return true;
					} else {
						if (type == CConnectionReadyReadEvent::EVENTTYPE) {
							CConnectionReadyReadEvent* crre = (CConnectionReadyReadEvent*)event;

							if (!mProcessingRequest) {

								if (!mParser->readFromDevice(mSocket)) {
									// error
									QString errorString = QString("HTTP/1.1 400 error parsing request\r\nConnection: close\r\n\r\n400 Parsing error: %1\r\n").arg(mParser->getHttpRequestParsingErrorString());
									LOG(INFO,"::Konclude::Control::Interface::SPARQL::SPARQLHTTPConnectionProcessor",logTr("Error parsing HTTP request, '%1'.").arg(mParser->getHttpRequestParsingErrorString()),this);
                                    mSocket->write(errorString.toLocal8Bit());
									mSocket->disconnectFromHost();
								}

								if (mParser->hasHttpRequestParsingCompleted()) {
									mProcessingRequest = true;
									mProcessingByteArray = mParser->takeBodyData();
									if (mProcessingByteArray != 0) {
										LOG(INFO,"::Konclude::Control::Interface::SPARQL::SPARQLHTTPConnectionProcessor",logTr("HTTP request successfully parsed, %1 Bytes received.").arg(mProcessingByteArray->count()),this);


										mSPARQLInterpreter = new CSPARQLRecordResultStreamingInterpreter(this, mOriginPreSynchronizer, mLoaderConfig);
										mWritingStarted = false;
										mWritingFailed = false;
										mDataWritten = false;
										mChunkPart = 0;
										defaultCommandDelegater = mSPARQLInterpreter;
										preSynchronizer = new CPreconditionSynchronizer(mSPARQLInterpreter);

										QString dataString = QString::fromUtf8(mProcessingByteArray->data());
										CParseProcessSPARQLTextCommand* parseProcSPARQLCommand = new CParseProcessSPARQLTextCommand(dataString);
										CCommandProcessedCallbackEvent *proComm = new CCommandProcessedCallbackEvent(this, parseProcSPARQLCommand);
										parseProcSPARQLCommand->setRecorder(mSPARQLInterpreter);

										mProcessingCommand = parseProcSPARQLCommand;

										defaultCommandDelegater->delegateCommand(parseProcSPARQLCommand);

										parseProcSPARQLCommand->addProcessedCallback(proComm);

										delete mProcessingByteArray;
									} else {
										// invalid request

										QString koncludeString = QString("%1 %2").arg(CKoncludeInfo::getKoncludeName()).arg(CKoncludeInfo::getKoncludeVersionString());
										sendData(QString("<h2>%1 SPARQL Sever</h2><p>This response is generated from Konclude's SPARQL server for invalid SPARQL requests. Please make sure that the SPARQL request is appended as content (with correct Content-Length) of your HTTP request. See <a href=\"http://konclude.com\">konclude.com</a> for help and further information.</p>").arg(koncludeString).toLocal8Bit());

										mProcessingRequest = false;
										if (mForceDisconnect || mParser->hasRequestedCloseConnection()) {
											mSocket->disconnectFromHost();
										} else if (mProcessMoreRead) {
											mProcessMoreRead = false;
											connectionRead();
										}

										mParser->reset();

									}


								}
							}

							return true;
						} else if (type == CConnectionDisconnectedEvent::EVENTTYPE) {
							CConnectionDisconnectedEvent* cde = (CConnectionDisconnectedEvent*)event;
							mParser->reset();
							mProcessMoreRead = false;
							if (!mProcessingRequest) {
								mHandlerBusy = false;
							}
							mReleaser->releaseConnectionHandler(this);
							return true;


						} else if (type == CHandleIncomingHttpConnectionEvent::EVENTTYPE) {
							CHandleIncomingHttpConnectionEvent* hihc = (CHandleIncomingHttpConnectionEvent*)event;
							cint64 socketDescriptor = hihc->getSocketDescriptor();
							LOG(INFO,"::Konclude::Control::Interface::SPARQL::SPARQLHTTPConnectionProcessor",logTr("Start processing new incoming connection."),this);
							mSocket->setSocketDescriptor(socketDescriptor);
							mHandlerBusy = true;
							return true;
						} else if (type == CResultStreamingWriteEvent::EVENTTYPE) {
							CResultStreamingWriteEvent* rswe = (CResultStreamingWriteEvent*)event;
							QByteArray* buffer = rswe->getBuffer();
							bool last = rswe->isLast();
							writeStreamDataToSocket(buffer, last);
							mDataWritten = true;
							return true;
						}

					}
					return false;
				}

				COWLlinkProcessor* CSPARQLHttpConnectionHandlerProcessor::initializeOWLlinkContent() {
					return this;
				}



				CSPARQLStreamingWriter* CSPARQLHttpConnectionHandlerProcessor::writeChunk(const QByteArray& byteArray) {
					QString chunckHeader = QString::number(byteArray.length(), 16) + "\r\n";
					mSocket->write(chunckHeader.toLocal8Bit());
					mSocket->write(byteArray);
					mSocket->write(QString("\r\n").toLocal8Bit());
					return this;
				}


				bool CSPARQLHttpConnectionHandlerProcessor::writeStreamData(QByteArray* buffer, bool last) {
					postEvent(new CResultStreamingWriteEvent(buffer, last));
					//writeStreamDataToSocket(buffer, last);
					//mDataWritten = true;
					return !mWritingFailed;
				}


				CSPARQLStreamingWriter* CSPARQLHttpConnectionHandlerProcessor::writeStreamDataToSocket(QByteArray* buffer, bool last) {
					if (mSocket->state() == QTcpSocket::ConnectedState) {
						if (!mWritingStarted && last) {
							sendData(*buffer);
							delete buffer;
						} else {
							++mChunkPart;
							// write with chunked encoding
							bool firstChunk = false;
							if (!mWritingStarted) {
								mWritingStarted = true;
								QString connectionString = QString("Connection: ");
								if (mParser->hasRequestedCloseConnection()) {
									connectionString += QString("close");
								} else {
									connectionString += QString("keep-alive");
								}
								QString serverString = QString("Server: %1 %2").arg(CKoncludeInfo::getKoncludeName()).arg(CKoncludeInfo::getKoncludeVersionString());
								QString responseHeadString = QString("HTTP/1.1 200 OK\r\n%1\r\n%2\r\nTransfer-Encoding: chunked\r\n\r\n").arg(serverString).arg(connectionString);

								mSocket->write(responseHeadString.toLocal8Bit());

								writeChunk(mStreamSPARQLHeader);
								mSocket->flush();
								firstChunk = true;
							}
							writeChunk(*buffer);
							mSocket->flush();
							if (last) {
								writeChunk(mStreamSPARQLFooter);
								writeChunk(QByteArray());
								mSocket->flush();
								LOG(INFO, "::Konclude::Control::Interface::SPARQL::SPARQLHTTPConnectionProcessor", logTr("HTTP request successfully processed, last part of %1 with %2 bytes sent.").arg(mChunkPart).arg(buffer->size()), this);
							} else {
								LOG(INFO, "::Konclude::Control::Interface::SPARQL::SPARQLHTTPConnectionProcessor", logTr("Sending part %1 with %2 bytes of HTTP response via chunked encoding.").arg(mChunkPart).arg(buffer->size()), this);
							}
							delete buffer;
						}
					} else {
						mWritingFailed = true;
						LOG(INFO, "::Konclude::Control::Interface::SPARQL::SPARQLHTTPConnectionProcessor", logTr("Cannot write HTTP response data since connection already closed."), this);
					}
					return this;
				}



				COWLlinkProcessor* CSPARQLHttpConnectionHandlerProcessor::sendData(const QByteArray& dataArray) {
					if (mSocket->state() == QTcpSocket::ConnectedState) {
						QString connectionString = QString("Connection: ");
						if (mParser->hasRequestedCloseConnection()) {
							connectionString += QString("close");
						} else {
							connectionString += QString("keep-alive");
						}
						cint64 contentLength = dataArray.length();
						if (contentLength > 0) {
							contentLength += mStreamSPARQLHeader.length();
							contentLength += mStreamSPARQLFooter.length();
						}
						QString serverString = QString("Server: %1 %2").arg(CKoncludeInfo::getKoncludeName()).arg(CKoncludeInfo::getKoncludeVersionString());
						QString responseHeadString = QString("HTTP/1.1 200 OK\r\n%1\r\n%2\r\nContent-Length: %3\r\n\r\n").arg(serverString).arg(connectionString).arg(contentLength);

						mSocket->write(responseHeadString.toLocal8Bit());
						if (contentLength > 0) {
							mSocket->write(mStreamSPARQLHeader);
							mSocket->write(dataArray);
							mSocket->write(mStreamSPARQLFooter);
						}
						mSocket->flush();

						LOG(INFO,"::Konclude::Control::Interface::SPARQL::SPARQLHTTPConnectionProcessor",logTr("HTTP request successfully processed, %1 bytes sent.").arg(contentLength),this);
					} else {
						LOG(INFO,"::Konclude::Control::Interface::SPARQL::SPARQLHTTPConnectionProcessor",logTr("HTTP request successfully processed, but connection already closed."),this);
					}
					return this;
				}

				COWLlinkProcessor* CSPARQLHttpConnectionHandlerProcessor::concludeOWLlinkContent() {
					if (!mDataWritten) {
						mDataWritten = true;
						sendData(QByteArray());
					}
					delete mSPARQLInterpreter;
					delete mProcessingCommand;
					mProcessingCommand = nullptr;
					mSPARQLInterpreter = nullptr;
					mProcessingRequest = false;


					if (mForceDisconnect || mParser->hasRequestedCloseConnection()) {
						mSocket->disconnectFromHost();
					} else if (mProcessMoreRead) {
						mProcessMoreRead = false;
						connectionRead();
					}

					mParser->reset();
					return this;
				}

				void CSPARQLHttpConnectionHandlerProcessor::connectionRead() {
					postEvent(new CConnectionReadyReadEvent());
				}

				void CSPARQLHttpConnectionHandlerProcessor::connectionDisconnect() {
					postEvent(new CConnectionDisconnectedEvent());
				}

				CConfiguration* CSPARQLHttpConnectionHandlerProcessor::getConfiguration() {
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


			}; // end namespace SPARQL 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
