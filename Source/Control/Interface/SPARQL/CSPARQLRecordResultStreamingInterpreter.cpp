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

#include "CSPARQLRecordResultStreamingInterpreter.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace SPARQL {

				CSPARQLRecordResultStreamingInterpreter::CSPARQLRecordResultStreamingInterpreter(CSPARQLStreamingWriter* resultStreamingWriter, CCommandDelegater *commandDelegater, CConfiguration* config)
						: CCommandRecorder() {

					mResultStreamingWriter = resultStreamingWriter;


					mConfBufferSize = 104857600;

					mDelegater = commandDelegater;
					mCurrentProcessingSeqNumber = 0;

					mConfig = config;
					mConfBufferSize = CConfigDataReader::readConfigInteger(mConfig, "Konclude.SPARQL.Serialization.ChunkEncodingSize", 104857600);
					mNextSeqNumber = 0;
				}


				CSPARQLRecordResultStreamingInterpreter::~CSPARQLRecordResultStreamingInterpreter() {
				}


				CCommandDelegater *CSPARQLRecordResultStreamingInterpreter::delegateCommand(CCommand *command) {
					CCalculateQueryCommand* calcQueryCommand = dynamic_cast<CCalculateQueryCommand*>(command);
					if (calcQueryCommand) {
						mSeqSyncMutex.lock();
						cint64 sequenceNumber = mNextSeqNumber++;
						CQueryCommandProvider* queryProvider = calcQueryCommand->getQueryCommandProvider();
						CSPARQLResultStreamingData* data = nullptr;
						if (queryProvider) {
							data = new CSPARQLResultStreamingData(queryProvider, sequenceNumber, mConfBufferSize, this);
						} else {
							data = new CSPARQLResultStreamingData(calcQueryCommand->getQuery(), sequenceNumber, mConfBufferSize, this);
						}
						calcQueryCommand->setQueryCommandProvider(data);
						mCommandSequenceDataHash.insert(command, data);
						mSequenceDataHash.insert(sequenceNumber, data);
						mSeqSyncMutex.unlock();
					}
					if (mDelegater) {
						mDelegater->delegateCommand(command);
					}
					return this;
				}


				bool CSPARQLRecordResultStreamingInterpreter::notifyWriteRequest(cint64 sequenceNumber) {
					bool continueWriting = true;
					if (mCurrentProcessingSeqNumber == sequenceNumber) {
						CSPARQLResultStreamingData* seqData = mSequenceDataHash.value(sequenceNumber);
						continueWriting = writeStreamingData(seqData, false);
					}
					return continueWriting;
				}


				bool CSPARQLRecordResultStreamingInterpreter::writeStreamingData(CSPARQLResultStreamingData* seqData, bool lastData) {
					QList<CSPARQLResultBufferWriteData> bufferList = seqData->takeWriteableBuffers();
					bool continueWriting = true;
					if (mResultStreamingWriter) {
						continueWriting &= mResultStreamingWriter->writeStreamData(bufferList, lastData);
					} else {
						continueWriting = false;
					}
					return continueWriting;
				}

				bool CSPARQLRecordResultStreamingInterpreter::canWrite() {
					return mResultStreamingWriter != nullptr;
				}


				CCommandRecorder* CSPARQLRecordResultStreamingInterpreter::recordData(CCommandRecordData *recData) {
					CCommandRecorder::recordData(recData);
					if (dynamic_cast<CClosureProcessCommandRecord *>(recData)) {
						if (recData) {
							CCommand *command = recData->getCommand();
							if (command) {

								if (mCommandSequenceDataHash.contains(command)) {


									CCalculateQueryCommand *calcQueryCommand = dynamic_cast<CCalculateQueryCommand *>(command);
									if (calcQueryCommand) {
										CQuery *query = calcQueryCommand->getQuery();
										CSPARQLResultStreamingData* seqData = mCommandSequenceDataHash.value(command);

										if (command->getMaxErrorLevel() <= 0) {

											if (query) {
												if (query->hasError()) {
													QString errorString = query->getErrorStringList().join(" ");
													seqData->handleQueryError(query, errorString);
												} else {
													CQueryResult* queryResult = query->getQueryResult();
													seqData->handleQueryResult(query, queryResult);
												}
											}

										} else {
											QString errorString = getErrorString(recData, "Error");
											seqData->handleQueryError(query, errorString);
										}
										seqData->finalize();
										writeStreamingData(seqData, true);
										mSeqSyncMutex.lock();
										mSequenceDataHash.remove(seqData->getSequenceNumber());
										mCommandSequenceDataHash.remove(command);
										mCurrentProcessingSeqNumber = seqData->getSequenceNumber() + 1;
										delete seqData;
										mSeqSyncMutex.unlock();


									} else {
										if (command->getMaxErrorLevel() > 0) {
											QString errorString = getErrorString(recData, "Error");
											//TODO: report error to stream writer
										}

									}
								}

							}
						}
					}
					return this;
				}





				QString CSPARQLRecordResultStreamingInterpreter::getErrorString(CCommandRecordData *recData, const QString &nodeString) {
					QString errorText;
					if (recData) {
						errorText = QString("An error has occurred while processing '%1',\n%2 process protocol:\n").arg(recData->getCommand()->getBriefCommandDescription()).arg(nodeString);

						CCommandRecordData *nextRecData = recData;

						while (nextRecData) {
							errorText += QString("{%3}, [%1]:%2\n").arg(nextRecData->getRecordDomain()).arg(nextRecData->getRecordMessage()).arg(getErrorLevelString(nextRecData->getRecordErrorLevel()));
							nextRecData = nextRecData->getNextCommandRecordData();
						}

						CCommand *command = recData->getCommand();
						CLinker<CCommand *> *subCommIt = command->getSubCommandLinker();
						QString subNodeString = QString("Sub-%1").arg(nodeString);
						while (subCommIt) {
							CCommand *subCommand = subCommIt->getData();
							if (subCommand->getMaxErrorLevel() > 0) {
								CCommandRecordData *subCommandRecData = subCommand->getRecordData();
								if (subCommandRecData) {
									QString subErrorString = QString("{%1}, %2").arg(getErrorLevelString(subCommand->getMaxErrorLevel())).arg(getErrorString(subCommandRecData, subNodeString));
									errorText += subErrorString;
								}
							}
							subCommIt = subCommIt->getNext();
						}

					}
					else {
						errorText = QString("An error has occurred while processing, process interface not available");
					}
					return errorText;
				}




				QString CSPARQLRecordResultStreamingInterpreter::getErrorLevelString(double level) {
					QString logLevelString;
					if (level >= 90) {
						logLevelString = "catastrophic error";
					}
					else if (level >= 80) {
						logLevelString = "exception error";
					}
					else if (level >= 70) {
						logLevelString = "error";
					}
					else if (level >= 60) {
						logLevelString = "warning";
					}
					else if (level >= 30) {
						logLevelString = "info";
					}
					else {
						logLevelString = "notice";
					}
					return logLevelString;
				}



			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
