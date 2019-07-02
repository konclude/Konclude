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

#include "COWLlinkHttpConnectionHandlerProcessor.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace OWLlink {


				COWLlinkHttpConnectionHandlerProcessor::COWLlinkHttpConnectionHandlerProcessor(CConfiguration* loaderConfig, CQtHttpConnectionHandlerReleaser* releaser) : COWLlinkProcessor(false) {
					mReleaser = releaser;
					reasonerCommander = CConfigManagerReader::readCommanderManagerConfig(loaderConfig);
					mSocket = nullptr;
					mQueued = false;
					startThread();
					mThreadStartedSemaphore.acquire(1);
				}


				COWLlinkHttpConnectionHandlerProcessor::~COWLlinkHttpConnectionHandlerProcessor() {
				}

				void COWLlinkHttpConnectionHandlerProcessor::threadStarted() {
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

				void COWLlinkHttpConnectionHandlerProcessor::threadStopped() {
					COWLlinkProcessor::threadStopped();

					mSocket->close();
					delete mSocket;
					delete mParser;
				}


				bool COWLlinkHttpConnectionHandlerProcessor::handleIncomingConnection(int socketDescriptor) {
					postEvent(new CHandleIncomingHttpConnectionEvent(socketDescriptor));
					return true;
				}


				bool COWLlinkHttpConnectionHandlerProcessor::isHandlerQueued() {
					return mQueued;
				}

				COWLlinkHttpConnectionHandlerProcessor* COWLlinkHttpConnectionHandlerProcessor::setHandlerQueued(bool queued) {
					mQueued = queued;
					return this;
				}



				bool COWLlinkHttpConnectionHandlerProcessor::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (COWLlinkProcessor::processCustomsEvents(type,event)) {
						return true;
					} else {
						if (type == CConnectionReadyReadEvent::EVENTTYPE) {
							CConnectionReadyReadEvent* crre = (CConnectionReadyReadEvent*)event;

							if (!mProcessingRequest) {

								if (!mParser->readFromDevice(mSocket)) {
									// error
									QString errorString = QString("HTTP/1.1 400 error parsing request\r\nConnection: close\r\n\r\n400 Parsing error: %1\r\n").arg(mParser->getHttpRequestParsingErrorString());
									LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("Error parsing HTTP request, '%1'.").arg(mParser->getHttpRequestParsingErrorString()),this);
                                    mSocket->write(errorString.toLocal8Bit());
									mSocket->disconnectFromHost();
								}

								if (mParser->hasHttpRequestParsingCompleted()) {
									mProcessingRequest = true;
									mProcessingByteArray = mParser->takeBodyData();
									LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("HTTP request successfully parsed, %1 Bytes received.").arg(mProcessingByteArray->count()),this);

									COWLlinkQtXMLCommandParser *owllinkCommandParser = new COWLlinkQtXMLCommandParser();
									mOwllinkInterpreter = new COWLLinkRecordInterpreter(preSynchronizer);
									defaultCommandDelegater = mOwllinkInterpreter;

									CParseOWLlinkCommandsCommand *parseCommand = new CParseOWLlinkCommandsCommand(mProcessingByteArray,mSocket->peerAddress().toString());
									parseCommand->setRecorder(mOwllinkInterpreter);
									parseCommand->setReportErrorFromSubCommands(false);

									CCommandProcessedCallbackEvent *proComm = new CCommandProcessedCallbackEvent(this,parseCommand);
									parseCommand->addProcessedCallback(proComm);


									owllinkCommandParser->realizeCommand(parseCommand,mOwllinkInterpreter);

									delete owllinkCommandParser;
									delete mProcessingByteArray;


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
							LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("Start processing new incoming connection."),this);
							mSocket->setSocketDescriptor(socketDescriptor);
							mHandlerBusy = true;
							return true;
						}

					}
					return false;
				}

				COWLlinkProcessor* COWLlinkHttpConnectionHandlerProcessor::initializeOWLlinkContent() {
					return this;
				}

				COWLlinkProcessor* COWLlinkHttpConnectionHandlerProcessor::concludeOWLlinkContent() {
					if (mSocket->state() == QTcpSocket::ConnectedState) {

						QByteArray bodyData = mOwllinkInterpreter->getByteArray();

						QString connectionString = QString("Connection: ");
						if (mParser->hasRequestedCloseConnection()) {
							connectionString += QString("close");
						} else {
							connectionString += QString("Keep-Alive");
						}
						QString serverString = QString("%1 %2").arg(CKoncludeInfo::getKoncludeName()).arg(CKoncludeInfo::getKoncludeVersionString());
						QString responseHeadString = QString("HTTP/1.1 200 OK\r\n%1\r\n%2\r\nContent-Length: %3\r\n\r\n").arg(serverString).arg(connectionString).arg(bodyData.length());

                        mSocket->write(responseHeadString.toLocal8Bit());
						mSocket->write(bodyData);

						LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("HTTP request successfully processed, %1 Bytes sent.").arg(bodyData.count()),this);
					} else {
						LOG(INFO,"::Konclude::Control::Interface::OWLlink::OWLlinkHTTPConnectionProcessor",logTr("HTTP request successfully processed, but connection already closed."),this);

					}
					delete mOwllinkInterpreter;
					mOwllinkInterpreter = nullptr;
					mProcessingRequest = false;


					if (true ||mParser->hasRequestedCloseConnection()) {
						mSocket->disconnectFromHost();
					} else if (mProcessMoreRead) {
						mProcessMoreRead = false;
						connectionRead();
					}

					mParser->reset();
					return this;
				}

				void COWLlinkHttpConnectionHandlerProcessor::connectionRead() {
					postEvent(new CConnectionReadyReadEvent());
				}

				void COWLlinkHttpConnectionHandlerProcessor::connectionDisconnect() {
					postEvent(new CConnectionDisconnectedEvent());
				}

				CConfiguration* COWLlinkHttpConnectionHandlerProcessor::getConfiguration() {
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


			}; // end namespace OWLlink 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
