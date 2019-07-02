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

#include "COREBatchProcessingLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			COREBatchProcessingLoader::COREBatchProcessingLoader() : mORELogIdentifier("::Konclude::OREBatchProcessor",this) {
				mOWLLinkInterpreter = nullptr;
				mLoaderConfig = nullptr;
				mConfLogProcessingTimes = false;
				mProcessingCommandData = nullptr;

				mCloseAfterOutput = true;
				mBlockUntilProcessed = true;

				mSatOperationString = QString("sat").toLower();
				mSatisfiabilityOperationString = QString("satisfiability").toLower();
				mQueryOperationString = QString("query").toLower();
				mClassificationOperationString = QString("classification").toLower();
				mConsistencyOperationString = QString("consistency").toLower();
				mRealisationOperationString = QString("realisation").toLower();
				mRealizationOperationString = QString("realization").toLower();


				mOperationTimeOutputString = QString("Operation time: ");

				mInfoConsoleObserver = nullptr;
				mErrorFileObserver = nullptr;

				mConsistencyKBCommand = nullptr;
				mTriviallyConsistencyKBCommand = nullptr;
				mAdditionalCheckingTime = nullptr;

				mReportAllTime = false;
			}



			COREBatchProcessingLoader::~COREBatchProcessingLoader() {
			}



			CCommandRecorder* COREBatchProcessingLoader::recordData(CCommandRecordData *recData) {
				LOGC(recData->getRecordErrorLevel(),recData->getRecordDomain(),recData->getRecordMessage(),recData->getCommand());
				return this;
			}



			CLoader *COREBatchProcessingLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				mLoaderConfig = config;
				reasonerCommander = CConfigManagerReader::readCommanderManagerConfig(config);

				mOperationTaskString = CConfigDataReader::readConfigString(config,"Konclude.ORE.OperationTask");
				mOntologyFileString = CConfigDataReader::readConfigString(config,"Konclude.ORE.OntologyFile");
				mResponseFileString = CConfigDataReader::readConfigString(config,"Konclude.ORE.ResponseFile");
				mIRINameString = CConfigDataReader::readConfigString(config,"Konclude.ORE.IRIName");
				mCloseAfterOutput = CConfigDataReader::readConfigBoolean(config,"Konclude.ORE.CloseAfterProcessed");
				mOutputMeasuredTimeInSeconds = CConfigDataReader::readConfigBoolean(config,"Konclude.ORE.OutputTimeMeasurementInSeconds");

				mReportParsingTimeForConsistency = CConfigDataReader::readConfigBoolean(config,"Konclude.ORE.ReportParsingTimeForConsistency");
				mReportParsingTimeForSatisfiability = CConfigDataReader::readConfigBoolean(config,"Konclude.ORE.ReportParsingTimeForSatisfiability");
				mReportParsingTimeForRealisation = CConfigDataReader::readConfigBoolean(config,"Konclude.ORE.ReportParsingTimeForRealization");
				mReportParsingTimeForClassification = CConfigDataReader::readConfigBoolean(config,"Konclude.ORE.ReportParsingTimeForClassification");

				mInfoConsoleObserver = new COREConsolePrintLogObserver(QStringList()<<mORELogIdentifier.getLogDomain());
				mErrorFileObserver = new COREFilePrintLogObserver(mResponseFileString+"_err");
				CLogger::getInstance()->addLogObserver(mInfoConsoleObserver);
				CLogger::getInstance()->addLogObserver(mErrorFileObserver);

				return this;
			}



			CLoader *COREBatchProcessingLoader::load() {
				startProcessing();
				if (mBlockUntilProcessed) {
					mBlockingSemaphore.acquire();
				}
				return this;
			}



			CConfiguration *COREBatchProcessingLoader::getConfiguration() {
				if (!reasonerCommander) {
					return mLoaderConfig;
				} else {
					CGetDescriptionCommand* getDesComm = new CGetDescriptionCommand();
					reasonerCommander->delegateCommand(getDesComm);
					CCommandExecutedBlocker commExeBlocker;
					commExeBlocker.waitExecutedCommand(getDesComm);
					CConfiguration* config = getDesComm->getConfiguration();
					return config;
				}
			}


			void COREBatchProcessingLoader::createClassificationTestingCommands() {
				logOutputMessage(QString("Started %1 on %2").arg(mProcessingOperationString).arg(mOntologyFileString));
				QString oreTestKB = QString("http://konclude.com/ore/test/kb");
				CCreateKnowledgeBaseCommand* createKBCommand = new CCreateKnowledgeBaseCommand(oreTestKB);
				QStringList ontoIRIList;
				ontoIRIList.append(mOntologyFileString);
				CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLAutoOntologyCommand(oreTestKB,ontoIRIList);
				CKnowledgeBaseClassifyCommand* classifyKBCommand = new CKnowledgeBaseClassifyCommand(oreTestKB);
				CWriteFunctionalSubClassHierarchyQueryCommand* writeHierarchyCommand = new CWriteFunctionalSubClassHierarchyQueryCommand(oreTestKB,mResponseFileString);
				if (mReportParsingTimeForClassification) {
					mReportAllTime = true;
				}
				addProcessingCommand(createKBCommand);
				addProcessingCommand(loadKBCommand);
				addProcessingCommand(classifyKBCommand,true,mOperationTimeOutputString);
				addProcessingCommand(writeHierarchyCommand);
				processNextCommand();
			}

			void COREBatchProcessingLoader::createRealisationTestingCommands() {
				logOutputMessage(QString("Started %1 on %2").arg(mProcessingOperationString).arg(mOntologyFileString));
				QString oreTestKB = QString("http://konclude.com/ore/test/kb");
				CCreateKnowledgeBaseCommand* createKBCommand = new CCreateKnowledgeBaseCommand(oreTestKB);
				QStringList ontoIRIList;
				ontoIRIList.append(mOntologyFileString);
				CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLAutoOntologyCommand(oreTestKB,ontoIRIList);
				CKnowledgeBaseRealizeCommand* realizeKBCommand = new CKnowledgeBaseRealizeCommand(oreTestKB);
				CWriteFunctionalIndividualTypesQueryCommand* writeIndividualTypesCommand = new CWriteFunctionalIndividualTypesQueryCommand(oreTestKB,mResponseFileString);
				if (mReportParsingTimeForRealisation) {
					mReportAllTime = true;
				}
				addProcessingCommand(createKBCommand);
				addProcessingCommand(loadKBCommand);
				addProcessingCommand(realizeKBCommand,true,mOperationTimeOutputString);
				addProcessingCommand(writeIndividualTypesCommand);
				processNextCommand();
			}


			void COREBatchProcessingLoader::createConsistencyTestingCommands() {
				logOutputMessage(QString("Started %1 on %2").arg(mProcessingOperationString).arg(mOntologyFileString));
				QString oreTestKB = QString("http://konclude.com/ore/test/kb");
				CCreateKnowledgeBaseCommand* createKBCommand = new CCreateKnowledgeBaseCommand(oreTestKB);
				QStringList ontoIRIList;
				ontoIRIList.append(mOntologyFileString);
				CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLAutoOntologyCommand(oreTestKB,ontoIRIList);
				mConsistencyKBCommand = new CIsConsistentQueryCommand(oreTestKB);
				mTriviallyConsistencyKBCommand = new CIsTriviallyConsistentQueryCommand(oreTestKB);
				if (mReportParsingTimeForConsistency) {
					mReportAllTime = true;
				}
				addProcessingCommand(createKBCommand);
				addProcessingCommand(loadKBCommand);
				addProcessingCommand(mTriviallyConsistencyKBCommand,true,mOperationTimeOutputString,true,mResponseFileString);
				processNextCommand();
			}


			void COREBatchProcessingLoader::createSatisfiabilityTesingCommands() {
				logOutputMessage(QString("Started %1 on %2").arg(mProcessingOperationString).arg(mOntologyFileString));
				QString oreTestKB = QString("http://konclude.com/ore/test/kb");
				CCreateKnowledgeBaseCommand* createKBCommand = new CCreateKnowledgeBaseCommand(oreTestKB);
				QStringList ontoIRIList;
				ontoIRIList.append(mOntologyFileString);
				mWriteSatisfiabilityPrefixString = mIRINameString+QString(",");
				CLoadKnowledgeBaseOWLAutoOntologyCommand* loadKBCommand = new CLoadKnowledgeBaseOWLAutoOntologyCommand(oreTestKB,ontoIRIList);
				CProcessClassNameSatisfiableQueryCommand* consistencyKBCommand = new CProcessClassNameSatisfiableQueryCommand(oreTestKB,mIRINameString);
				if (mReportParsingTimeForSatisfiability) {
					mReportAllTime = true;
				}
				addProcessingCommand(createKBCommand);
				addProcessingCommand(loadKBCommand);
				addProcessingCommand(consistencyKBCommand,true,mOperationTimeOutputString,true,mResponseFileString);
				processNextCommand();
			}

			void COREBatchProcessingLoader::addProcessingCommand(CCommand* command) {
				mProcessCommandList.append(new CCommandProcessData(command));
			}

			void COREBatchProcessingLoader::addProcessingCommand(CCommand* command, bool measureTime, const QString& measuredOutputString) {
				mProcessCommandList.append(new CCommandProcessData(command,measureTime,measuredOutputString));
			}


			void COREBatchProcessingLoader::addProcessingCommand(CCommand* command, bool measureTime, const QString& measuredOutputString, bool writeSatisfiabilityOutput, const QString& outputFileName) {
				mProcessCommandList.append(new CCommandProcessData(command,measureTime,measuredOutputString,writeSatisfiabilityOutput,outputFileName));
			}


			COWLlinkProcessor *COREBatchProcessingLoader::initializeOWLlinkContent() {


				QString operationTaskString = mOperationTaskString;
				operationTaskString = operationTaskString.toLower();

				if (operationTaskString == mSatOperationString) {
					mProcessingOperationString = mSatOperationString;
					createSatisfiabilityTesingCommands();

				} else if (operationTaskString == mSatisfiabilityOperationString) {
					mProcessingOperationString = mSatisfiabilityOperationString;
					createSatisfiabilityTesingCommands();

				} else if (operationTaskString == mClassificationOperationString) {
					mProcessingOperationString = mClassificationOperationString;
					createClassificationTestingCommands();

				} else if (operationTaskString == mConsistencyOperationString) {
					mProcessingOperationString = mConsistencyOperationString;
					createConsistencyTestingCommands();

				} else if (operationTaskString == mRealisationOperationString) {
					mProcessingOperationString = mRealisationOperationString;
					createRealisationTestingCommands();

				} else if (operationTaskString == mRealizationOperationString) {
					mProcessingOperationString = mRealizationOperationString;
					createRealisationTestingCommands();

				} else {
					logOutputError(QString("Operation %1 not supported").arg(mOperationTaskString));
					terminateProcessing();
				}

				return this;
			}

			COWLlinkProcessor *COREBatchProcessingLoader::concludeOWLlinkContent() {
				finishCommandProcessing();
				if (hasMoreProcessingCommands()) {
					processNextCommand();
				} else {
					logOutputMessage(QString("Completed %1 on %2").arg(mProcessingOperationString).arg(mOntologyFileString));
					terminateProcessing();
				}
				return this;
			}


			void COREBatchProcessingLoader::logOutputError(const QString& errorString) {
				LOG(ERROR,mORELogIdentifier.getLogDomain(),logTr("Error: %1").arg(errorString),this);
			}
			void COREBatchProcessingLoader::logOutputNotice(const QString& outputString) {
				LOG(NOTICE,mORELogIdentifier.getLogDomain(),logTr("%1").arg(outputString),this);
			}

			void COREBatchProcessingLoader::logOutputMessage(const QString& outputString) {				
				LOG(INFO,mORELogIdentifier.getLogDomain(),logTr("%1").arg(outputString),this);
			}

			void COREBatchProcessingLoader::forcedPathCreated(const QString& filePath) {
				QString path = filePath;
				if (path.contains("/") || path.contains("\\")) {
					int lastSlash = path.lastIndexOf("/");
					int lastBackSlash = path.lastIndexOf("\\");
					int lastSeparator = qMax(lastBackSlash,lastSlash);
					path = path.mid(0,lastSeparator);
					if (!path.isEmpty()) {
						QDir dir;
						dir.mkpath(path);
					}
				}
			}

			void COREBatchProcessingLoader::writeSatisfiabilityResult(const QString& outputFileName, CCommand* processedCommand) {
				bool outputWritten = false;
				bool calculationCompleted = false;
				CKnowledgeBaseQueryCommand* kbQueryCommand = dynamic_cast<CKnowledgeBaseQueryCommand*>(processedCommand);
				if (kbQueryCommand) {
					CQuery* query = kbQueryCommand->getCalculateQueryCommand()->getQuery();
					if (query) {
						CQueryResult* queryResult = query->getQueryResult();
						if (queryResult) {
							CBooleanQueryResult* boolQueryResult = dynamic_cast<CBooleanQueryResult*>(queryResult);
							if (boolQueryResult) {
								calculationCompleted = true;
								forcedPathCreated(outputFileName);
								QFile outputFile(outputFileName);
								if (outputFile.open(QIODevice::Append)) {
									QString outputData;
									if (boolQueryResult->getResult() == true) {
										outputData = mWriteSatisfiabilityPrefixString+QString("true\n");
									} else {
										outputData = mWriteSatisfiabilityPrefixString+QString("false\n");
									}
									outputFile.write(outputData.toUtf8());
									outputFile.close();
									outputWritten = true;
								} else {
									logOutputError(QString("Failed writing output to file '%1'").arg(outputFileName));
								}
							}
						}
					}
				}
				if (!calculationCompleted) {
					logOutputError("Calculation failed");
				}
			}

			void COREBatchProcessingLoader::finishCommandProcessing() {
				if (mProcessingCommandData) {
					bool writeResults = true;
					if (mProcessingCommandData->mCommand == mTriviallyConsistencyKBCommand) {
						CKnowledgeBaseQueryCommand* kbQueryCommand = dynamic_cast<CKnowledgeBaseQueryCommand*>(mProcessingCommandData->mCommand);
						if (kbQueryCommand) {
							CQuery* query = kbQueryCommand->getCalculateQueryCommand()->getQuery();
							if (query) {
								CQueryResult* queryResult = query->getQueryResult();
								if (queryResult) {
									CBooleanQueryResult* boolQueryResult = dynamic_cast<CBooleanQueryResult*>(queryResult);
									if (boolQueryResult) {
										if (!boolQueryResult->getResult()) {
											writeResults = false;
											mAdditionalCheckingTime += mMeasurementTime.elapsed();
											addProcessingCommand(mConsistencyKBCommand,true,mOperationTimeOutputString,true,mResponseFileString);
										}
									}
								}
							}
						}
					}

					if (writeResults) {
						if (mProcessingCommandData->mMeasureTime) {
							QString outputString = mProcessingCommandData->mMeasuredOutputString;
							cint64 timeElapsed = mMeasurementTime.elapsed();
							timeElapsed += mAdditionalCheckingTime;
							if (mOutputMeasuredTimeInSeconds) {
								logOutputMessage(outputString+QString("%1").arg(timeElapsed/1000.));
							} else {
								logOutputMessage(outputString+QString("%1").arg(timeElapsed));
							}
						} else {
							if (mReportAllTime) {
								mAdditionalCheckingTime += mMeasurementTime.elapsed();
							}
						}
						if (mProcessingCommandData->mWriteSatisfiableOutput) {
							QString outputFile = mProcessingCommandData->mWriteOutputFile;
							writeSatisfiabilityResult(outputFile,mProcessingCommandData->mCommand);
						}
					}
					mProcessingCommandData = nullptr;
				}
			}

			void COREBatchProcessingLoader::processNextCommand() {
				mProcessingCommandData = mProcessCommandList.takeFirst();
				CCommand* command = mProcessingCommandData->mCommand;
				logOutputNotice(QString("Starting '%1' command").arg(command->getBriefCommandDescription()));
				command->setRecorder(this);
				command->setReportErrorFromSubCommands(false);
				CCommandProcessedCallbackEvent *proComm = new CCommandProcessedCallbackEvent(this,command);
				command->addProcessedCallback(proComm);
				mMeasurementTime.start();
				preSynchronizer->delegateCommand(command);
			}

			bool COREBatchProcessingLoader::hasMoreProcessingCommands() {
				return !mProcessCommandList.isEmpty();
			}

			void COREBatchProcessingLoader::terminateProcessing() {
				mInfoConsoleObserver->flushLogging();
				mErrorFileObserver->flushLogging();

				if (mBlockUntilProcessed) {
					mBlockingSemaphore.release();
				}

				if (mCloseAfterOutput) {
					CLogger::getInstance()->waitSynchronization();
                    QCoreApplication::exit();
				}
			}



			CLoader *COREBatchProcessingLoader::exit() {
				stopThread();
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
