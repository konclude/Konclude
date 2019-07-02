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

#include "CSPARQLRecordInterpreter.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace SPARQL {

				CSPARQLRecordInterpreter::CSPARQLRecordInterpreter(CCommandDelegater *commandDelegater, CConfiguration* config) 
						: CCommandRecorder() {
					delegater = commandDelegater;
					nextSeqNumber = 0;
					recNextSeqNumber = 0;

					owllinkNS = "http://www.owllink.org/owllink#";
					owlNS = "http://www.w3.org/2002/07/owl#";


					mConfig = config;

				}


				CSPARQLRecordInterpreter::~CSPARQLRecordInterpreter() {
				}


				CCommandDelegater *CSPARQLRecordInterpreter::delegateCommand(CCommand *command) {
					if (dynamic_cast<CCalculateQueryCommand *>(command)) {
						seqSyncMutex.lock();
						commandSequenceHash.insert(command, nextSeqNumber++);
						seqSyncMutex.unlock();
					}
					if (delegater) {
						delegater->delegateCommand(command);
					}
					return this;
				}

				CCommandRecorder *CSPARQLRecordInterpreter::recordData(CCommandRecordData *recData) {
					CCommandRecorder::recordData(recData);
					if (dynamic_cast<CClosureProcessCommandRecord *>(recData)) {
						if (recData) {
							CCommand *command = recData->getCommand();
							if (command) {

								if (commandSequenceHash.contains(command)) {


									if (dynamic_cast<CCalculateQueryCommand *>(command)) {
										CCalculateQueryCommand *calcQueryCommand = dynamic_cast<CCalculateQueryCommand *>(command);
										if (calcQueryCommand) {
											if (command->getMaxErrorLevel() <= 0) {

												CQuery *query = calcQueryCommand->getQuery();
												if (query) {
													qint64 seqNumber = commandSequenceHash.value(command);
													if (query->hasError()) {
														QString errorString = query->getErrorStringList().join(" ");
														mSerializer.addResultError(errorString, "Error", seqNumber);
													} else {
														CQueryResult* queryResult = query->getQueryResult();
														if (queryResult) {
															CVariableBindingsAnswersResult* varIndBindsResult = dynamic_cast<CVariableBindingsAnswersResult*>(queryResult);
															if (varIndBindsResult) {
																mSerializer.addResultSerialization(varIndBindsResult->getVariableNames(), varIndBindsResult, seqNumber);

															}
															CBooleanQueryResult* booleanResult = dynamic_cast<CBooleanQueryResult*>(queryResult);
															if (booleanResult) {
																mSerializer.addResultSerialization(booleanResult, seqNumber);
															}
														}
														CQueryStatistics* stats = query->getQueryStatistics();
														if (stats) {
															mSerializer.addResultStatistics(stats, seqNumber);
														}
													}


												}



											} else {
												qint64 seqNumber = commandSequenceHash.value(command);
												QString errorString = getErrorString(recData, "Error");
												mSerializer.addResultError(errorString, "Error", seqNumber);
											}
										}
									} else {
										if (command->getMaxErrorLevel() > 0) {
											qint64 seqNumber = commandSequenceHash.value(command);
											QString errorString = getErrorString(recData, "Error");
											mSerializer.addResultError(errorString, "Error", seqNumber);
										}

									}
								}

							}
						}
					}
					return this;
				}




				QByteArray CSPARQLRecordInterpreter::getByteArray() {
					return mSerializer.getCombinedResultSerialization();
				}




				QString CSPARQLRecordInterpreter::getErrorString(CCommandRecordData *recData, const QString &nodeString) {
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
						errorText = QString("An error has occurred while processing, process Interface not available");
					}
					return errorText;
				}




				QString CSPARQLRecordInterpreter::getErrorLevelString(double level) {
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
