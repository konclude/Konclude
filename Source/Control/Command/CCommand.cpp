/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CCommand.h"


#include "Records/CClosureProcessCommandRecord.h"



namespace Konclude {

	namespace Control {

		namespace Command {

			using namespace Records;

			CCommand::CCommand(CCommand *parentSuperCommand) {
				subCommandLinker = 0;
				superCommand = parentSuperCommand;

				processed = false;
				processing = false;

				recordData = 0;
				recorder = 0;
				closured = false;
				errorLevel = 0;

				mUnclosedCommandCount = 1;

				if (parentSuperCommand) {
					parentSuperCommand->makeToSubCommand(this);
				}

				reportErrorFromSubCommands = true;
			}


			CCommand::~CCommand() {
				deleteDependencies();
			}


			bool CCommand::hasReportErrorFromSubCommands() {
				return reportErrorFromSubCommands;
			}
			
			
			CCommand *CCommand::setReportErrorFromSubCommands(bool reportSubCommandErrors) {
				reportErrorFromSubCommands = reportSubCommandErrors;
				return this;
			}


			double CCommand::getMaxErrorLevel() {
				return errorLevel;
			}
			
			CCommand *CCommand::reportError(double errorLevelReport) {
				if (errorLevelReport > errorLevel) {
					errorSyncMutex.lock();
					errorLevel = qMax(errorLevel,errorLevelReport);
					errorSyncMutex.unlock();
					if (superCommand && superCommand->hasReportErrorFromSubCommands()) {
						superCommand->reportError(errorLevelReport);
					}
				}
				return this;
			}


			CLinker<CCommand *> *CCommand::getSubCommandLinker() {
				return subCommandLinker;
			}


			CCommand *CCommand::addSubCommand(CCommand *subCommand) {
				subCommandLinker = new CLinker<CCommand *>(subCommand,subCommandLinker);
				return this;
			}


			CCommand *CCommand::makeToSubCommand(CCommand *subCommand) {
				++mUnclosedCommandCount;
				addSubCommand(subCommand);
				subCommand->setSuperCommand(this);
				subCommand->setRecorder(recorder);
				return this;
			}


			CCommand *CCommand::getSuperCommand() {
				return superCommand;
			}

			CCommand *CCommand::setSuperCommand(CCommand *parentSuperCommand) {
				superCommand = parentSuperCommand;
				return this;
			}

			CCommand *CCommand::setProcessed(bool processedCommand) {
				if (processed) {
					closureSyncMutex.lock();
					mUnclosedCommandCount++;
					processed = processedCommand;
					closureSyncMutex.unlock();
				} else {
					processed = processedCommand;
					reduceUnclosedCheckCallback();
				}
				return this;
			}


			bool CCommand::areAllSubCommandsProcessed() {
				bool allSubCommPro = true;
				CLinker<CCommand *> *subCommIt = subCommandLinker;
				while (subCommIt) {
					CCommand *subCommand = subCommIt->getData();		
					if (!subCommand->isProcessed()) {
						allSubCommPro = false;
						break;
					}
					subCommIt = (CLinker<CCommand *> *)subCommIt->getNext();
				}
				return allSubCommPro;
			}


			bool CCommand::isProcessed() {
				return mUnclosedCommandCount <= 0;
			}

			bool CCommand::isThisCommandProcessed() {
				return processed;
			}


			CCommand *CCommand::getNextProcessableSubCommand() {
				CLinker<CCommand *> *subCommandIt = subCommandLinker;
				while (subCommandIt) {
					CCommand *subCommand = subCommandIt->getData();
					if (subCommand && subCommand->isProcessable() && !subCommand->isProcessing()) {
						return subCommand;
					}
				}
				return 0;
			}


			QString CCommand::getBriefCommandDescription() {
				return QString("Undefined Command");
			}



			CCommand *CCommand::setProcessing(bool processingCommand) {
				processing = processingCommand;
				return this;
			}

			bool CCommand::isProcessing() {
				return processing;
			}



			CCommandRecordData *CCommand::getRecordData() {
				return recordData;
			}

			CCommand *CCommand::setRecordData(CCommandRecordData *takeRecordData) {
				if (recordData) {
					delete recordData;
				}
				recordData = takeRecordData;
				return this;
			}

			CCommand *CCommand::overwriteRecordData(CCommandRecordData *takeRecordData) {
				recordData = takeRecordData;
				return this;
			}


			CCommand *CCommand::appendRecordData(CCommandRecordData *takeRecordData) {
				takeRecordData->setNextRecordData(recordData);
				recordData = takeRecordData;
				return this;
			}



			CCommandRecorder *CCommand::getRecorder() {
				return recorder;
			}

			CCommand *CCommand::setRecorder(CCommandRecorder *useCommandRecorder) {
				recorder = useCommandRecorder;
				return this;
			}


			CCommand *CCommand::deleteDependencies() {
				deleteSubCommands();
				deleteRecordData();
				return this;
			}


			CCommand *CCommand::deleteSubCommands() {
				CLinker<CCommand *> *subCommandIt = subCommandLinker;
				while (subCommandIt) {
					CCommand *subCommand = subCommandIt->getData();
					subCommand->deleteDependencies();
					delete subCommand;
					CLinker<CCommand *> *tmpSubCommIt = subCommandIt;
					subCommandIt = (CLinker<CCommand *> *)subCommandIt->getNext();
					delete tmpSubCommIt;
				}
				return this;
			}


			CCommand *CCommand::deleteRecordData() {
				CCommandRecordData *recordIt = recordData;
				while (recordIt) {
					CCommandRecordData *tmpRecordData = recordIt;
					recordIt = recordIt->getNextCommandRecordData();
					delete tmpRecordData;
				}
				return this;
			}


			CCommand *CCommand::addProcessedCallback(CCallbackData *callback) {
				if (isProcessed()) {
					callback->doCallback();
				} else {
					processedCallbackExecuter.addCallbackData(callback);
				}
				return this;
			}


			CCommand* CCommand::closedSubCommand() {
				reduceUnclosedCheckCallback();
				return this;
			}

			CCommand *CCommand::reduceUnclosedCheckCallback() {
				closureSyncMutex.lock();
				mUnclosedCommandCount--;
				bool closeCommand = false;
				if (mUnclosedCommandCount == 0) {
					closeCommand = true;
				}
				closureSyncMutex.unlock();
				if (!closured && closeCommand) {
					CClosureProcessCommandRecord::makeRecord(recorder,"::Konclude::Command::Command",this);
					closured = true;
					CCommand* tmpSuperCommand = superCommand;
					processedCallbackExecuter.doCallback();
					if (tmpSuperCommand) {
						tmpSuperCommand->closedSubCommand();
					}
				}
				return this;
			}


		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
