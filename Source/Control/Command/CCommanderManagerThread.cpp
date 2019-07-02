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

#include "CCommanderManagerThread.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CCommanderManagerThread::CCommanderManagerThread() : CThread("ReasonderCommander"), CLogIdentifier("::Konclude::Command::ReasonerCommanderThread",this) {
				configurationProvider = 0;
				reasonerManager = 0;
				kbCommandsDelegater = 0;
				classificationMan = 0;
				ontoRevMan = 0;
				startThread();
			}


			CCommanderManagerThread::~CCommanderManagerThread() {
				if (reasonerManager) {
					delete reasonerManager;
				}
				if (configurationProvider) {
					delete configurationProvider;
				}
				if (classificationMan) {
					delete classificationMan;
				}
				if (ontoRevMan) {
					delete ontoRevMan;
				}
			}

			CCommanderManagerThread *CCommanderManagerThread::realizeCommand(CCommand *command) {
				postEvent(new CRealizeCommandEvent(command));
				return this;
			}


			bool CCommanderManagerThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (!CThread::processCustomsEvents(type,event)) {
					if (type == EVENTREALIZECOMMAND) {
						CRealizeCommandEvent *commandEvent = (CRealizeCommandEvent *)event;
						if (commandEvent) {
							CCommand *command = commandEvent->getCommand();
							if (command) {
								if (command->getCommandTag() == INITIALIZECONFIGURATIONCOMMAND) {
									CInitializeConfigurationCommand *initializeConfigCommand = (CInitializeConfigurationCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
									initializeConfiguration(initializeConfigCommand->getInitialConfiguration(),&commandRecordRouter);
									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (command->getCommandTag() == INITIALIZEREASONERCOMMAND) {
									CInitializeReasonerCommand *initializeReasonerCommand = (CInitializeReasonerCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
									CCommanderInitializationFactory *initFact = initializeReasonerCommand->getInitializationFactory();
									initializeCommander(initFact,&commandRecordRouter);
									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (command->getCommandTag() == GETKNOWLEGEBASECONFIGURATIONCOMMAND) {
									CGetKnowledgeBaseConfigCommand *getConfigCommand = (CGetKnowledgeBaseConfigCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

									kbCommandsDelegater->delegateCommand(getConfigCommand);

									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (command->getCommandTag() == SETKNOWLEDGEBASECONFIGURATIONCOMMAND) {
									CSetKnowledgeBaseConfigCommand *setConfigCommand = (CSetKnowledgeBaseConfigCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

									kbCommandsDelegater->delegateCommand(setConfigCommand);

									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (command->getCommandTag() == GETCONFIGURATIONCOMMAND) {
									CGetConfigCommand *getConfigCommand = (CGetConfigCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

									CConfigurationBase *config = configurationProvider->getCurrentConfiguration();
									CConfigData *data = config->getRelatedConfigChange(getConfigCommand->getConfigNameString(),true);
									if (!data) {
										CUnspecifiedMessageErrorRecord::makeRecord(QString("Configuration property '%1' not available.").arg(getConfigCommand->getConfigNameString()),&commandRecordRouter);
									}
									getConfigCommand->setConfigData(data);

									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (command->getCommandTag() == SETCONFIGURATIONCOMMAND) {
									CSetConfigCommand *setConfigCommand = (CSetConfigCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

									CGlobalConfigurationBase *config = configurationProvider->createNextGlobalConfiguration();
									if (!config->isConfigLevelLocked(setConfigCommand->getConfigNameString())) {
										CConfigData *configData = config->createAndSetConfig(setConfigCommand->getConfigNameString());
										if (configData) {
											if (configData->supportsStringConvertion()) {
												if (configData->readFromString(setConfigCommand->getConfigValueString())) {
													CUnspecifiedMessageInformationRecord::makeRecord(QString("Setting configuration for '%1' to value '%2'.").arg(setConfigCommand->getConfigNameString()).arg(setConfigCommand->getConfigValueString()),&commandRecordRouter);
													configurationProvider->installGlobalConfiguration(config);
												} else {
													CUnspecifiedMessageErrorRecord::makeRecord(QString("Value for configuration property '%1' cannot be read from string '%2'.").arg(setConfigCommand->getConfigNameString()).arg(setConfigCommand->getConfigValueString()),&commandRecordRouter);
												}
											} else {
												CUnspecifiedMessageErrorRecord::makeRecord(QString("Value for configuration property '%1' cannot be from string '%2'.").arg(setConfigCommand->getConfigNameString()).arg(setConfigCommand->getConfigValueString()),&commandRecordRouter);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Configuration property '%1' not available.").arg(setConfigCommand->getConfigNameString()),&commandRecordRouter);
										}
									} else {
										CUnspecifiedMessageErrorRecord::makeRecord(QString("Configuration property '%1' is locked.").arg(setConfigCommand->getConfigNameString()),&commandRecordRouter);
									}

									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (command->getCommandTag() == GETDESCRIPTIONCOMMAND) {
									CGetDescriptionCommand *getDesCommand = (CGetDescriptionCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
									getDesCommand->setConfiguration(configurationProvider->getCurrentConfiguration());
									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (command->getCommandTag() == GETSETTINGSCOMMAND) {
									CGetSettingsCommand *getDesCommand = (CGetSettingsCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
									getDesCommand->setConfiguration(configurationProvider->getCurrentConfiguration());
									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (command->getCommandTag() == GETREASONERCOMMAND) {
									CGetReasonerCommand *getReasonerCommand = (CGetReasonerCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
									getReasonerCommand->setReasonerManager(reasonerManager);
									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (command->getCommandTag() == GETCLASSIFICATIONMANAGERCOMMAND) {
									CGetClassificationManagerCommand *getClassificationManCommand = (CGetClassificationManagerCommand *)command;
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
									getClassificationManCommand->setClassificationManager(classificationMan);
									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else if (dynamic_cast<CKnowledgeBaseCommand *>(command)) {
									CCommandRecordRouter commandRecordRouter(command,this);
									if (kbCommandsDelegater) {
										kbCommandsDelegater->delegateCommand(command);
									} else {
										CUnspecifiedMessageErrorRecord::makeRecord("Missing ontology manager.",&commandRecordRouter);
									}

								} else if (dynamic_cast<CCalculateQueryCommand *>(command)) {
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

									CCalculateQueryCommand *calcQueryCommand = (CCalculateQueryCommand *)command;
									CQuery *query = calcQueryCommand->getQuery();
									if (reasonerManager) {
										if (query) {
											CUnspecifiedMessageInformationRecord::makeRecord("Sending query to reasoner manager.",&commandRecordRouter);
											
											CCommandCalculatedQueryCallbackEvent *calculatedQueryCE = new CCommandCalculatedQueryCallbackEvent(this,calcQueryCommand);
											reasonerManager->reasoningQuery(query,calculatedQueryCE);
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Query cannot be be resolved.",&commandRecordRouter);
											CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
											CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
										}
									} else {
										CUnspecifiedMessageErrorRecord::makeRecord("Missing Reasoner Manager.",&commandRecordRouter);
										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
									}

								} else if (dynamic_cast<CPrepareKnowledgeBaseCommand *>(command)) {
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

									CPrepareKnowledgeBaseCommand* prepareKBCommand = (CPrepareKnowledgeBaseCommand*)command;
									if (reasonerManager) {
										CUnspecifiedMessageInformationRecord::makeRecord("Preparing ontology.",&commandRecordRouter);

										CConcreteOntology* ontology = prepareKBCommand->getOntology();
										if (ontology) {
											COntologyPreparedCallbackEvent *ontPrepQueryCE = new COntologyPreparedCallbackEvent(this,prepareKBCommand);
											reasonerManager->prepareOntology(ontology,*prepareKBCommand->getOntologyProcessingRequirementList(),ontPrepQueryCE);
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Ontology not specified.",&commandRecordRouter);
											CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
											CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
										}
									} else {
										CUnspecifiedMessageErrorRecord::makeRecord("Missing reasoner manager.",&commandRecordRouter);
										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
									}

								} else if (dynamic_cast<CSkipUnsupportedCommandCommand *>(command)) {
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

								} else {
									CCommandRecordRouter commandRecordRouter(command,this);
									CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
									CUnspecifiedMessageErrorRecord::makeRecord("Unsupported Command.",&commandRecordRouter);
									CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
									CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
								}
	

							}
						}
						return true;


					} else if (type == EVENTCOMMANDCALCULATEDQUERYCALLBACK) {
						CCommandCalculatedQueryCallbackEvent *commCalQueryCallbackEv = (CCommandCalculatedQueryCallbackEvent *)event;
						CCalculateQueryCommand *command = dynamic_cast<CCalculateQueryCommand *>(commCalQueryCallbackEv->getCommand());
						if (command) {							
							CCommandRecordRouter commandRecordRouter(command,this);
							CUnspecifiedMessageInformationRecord::makeRecord("Query by reasoner manager calculated.",&commandRecordRouter);
							CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
							CFinishProcessCommandRecord::makeRecord(&commandRecordRouter); 
						}
						return true;

					} else if (type == EVENTONTOLOGYPREPAREDCALLBACK) {
						COntologyPreparedCallbackEvent* ontPreparedCallbackEv = (COntologyPreparedCallbackEvent*)event;
						CPrepareKnowledgeBaseCommand* command = dynamic_cast<CPrepareKnowledgeBaseCommand*>(ontPreparedCallbackEv->getCommand());
						if (command) {							
							CCommandRecordRouter commandRecordRouter(command,this);
							CUnspecifiedMessageInformationRecord::makeRecord("Ontology for reasoner manager prepared.",&commandRecordRouter);
							CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
							CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
						}
						return true;

					}
				} else {
					return true;
				}
				return false;
			}



			void CCommanderManagerThread::initializeConfiguration(CConfiguration *initConfig, CCommandRecordRouter *commandRecordRouter) {
				if (configurationProvider) {
					CUnspecifiedMessageErrorRecord::makeRecord("Initial configuration already exists.",commandRecordRouter);
				} else {
					configurationProvider = new CReasonerCommanderGlobalConfiguration(initConfig);
				}
			}


			void CCommanderManagerThread::initializeCommander(CCommanderInitializationFactory *initializationFactory, CCommandRecordRouter *commandRecordRouter) {
				if (!configurationProvider) {
					CUnspecifiedMessageErrorRecord::makeRecord("Missing initial configuration.",commandRecordRouter);
				} else {
					CGlobalConfigurationBase *config = 0;
					config = configurationProvider->createNextGlobalConfiguration();

					if (reasonerManager) {
						CUnspecifiedMessageErrorRecord::makeRecord("Reasoner manager already exists.",commandRecordRouter);
					} else {
						reasonerManager = initializationFactory->createReasonerManager(configurationProvider);
						((CReasonerManagerConfigType *)(config->createAndSetConfig("Konclude.Execution.ReasonerManager"))->getConfigType())->setReasonerManager(reasonerManager);
					}

					if (classificationMan) {
						CUnspecifiedMessageErrorRecord::makeRecord("Classification manager already exists.",commandRecordRouter);
					} else {
						classificationMan = initializationFactory->createClassificationManager(configurationProvider);
						((CClassificationManagerConfigType *)config->createAndSetConfig("Konclude.Execution.ClassificationManager")->getConfigType())->setClassificationManager(classificationMan);
					}

					if (ontoRevMan) {
						CUnspecifiedMessageErrorRecord::makeRecord("Ontology revision manager already exists.",commandRecordRouter);
					} else {
						ontoRevMan = initializationFactory->createOntologyRevisionManager(configurationProvider);
						((COntologyRevisionManagerConfigType *)config->createAndSetConfig("Konclude.Execution.OntologyRevisionManager")->getConfigType())->setOntologyRevisionManager(ontoRevMan);
					}

					configurationProvider->installGlobalConfiguration(config);

					initializationFactory->initializeReasonerManager(reasonerManager,configurationProvider);
					initializationFactory->initializeClassificationManager(classificationMan,configurationProvider);
					initializationFactory->initializeOntologyRevisionManager(ontoRevMan,configurationProvider);

					kbCommandsDelegater = dynamic_cast<CSPOntologyRevisionManager *>(ontoRevMan);
				}
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
