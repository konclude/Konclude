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

#include "COWLlinkProcessor.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace OWLlink {


				COWLlinkProcessor::COWLlinkProcessor(bool immediatelyStartThread) : CThread("OWLlinkProcessor"), CLogIdentifier("::Konclude::Control::Interface::OWLlink::OWLlinkProcessor",this) {
					preSynchronizer = new CPreconditionSynchronizer(this);

					reasonerCommander = 0;

					mConfLoadOntologiesSAXParsing = true;
					mConfLoadOntologiesStreamParsing = true;

					defaultCommandDelegater = preSynchronizer;

					mConfLogProcessingTimes = false;

					mNetworkManager = nullptr;

					if (immediatelyStartThread) {
						startThread();
					}
				}



				COWLlinkProcessor::~COWLlinkProcessor() {
					stopThread();
					delete preSynchronizer;
				}




				COWLlinkProcessor *COWLlinkProcessor::startProcessing() {
					postEvent(new CInitializeEvent());
					return this;
				}



				CCommandDelegater *COWLlinkProcessor::delegateCommand(CCommand *command) {
					postEvent(new CRealizeCommandEvent(command));
					return this;
				}

				QStringList COWLlinkProcessor::getParserOrderFromFileName(const QString& fileName) {
					QStringList parserList;
					bool owl2FunctionalParserAdded = false;
					bool owl2XMLParserAdded = false;
					bool turtleParserAdded = false;
					bool owl2RDFXMLParserAdded = false;
					QString upperFileString = fileName.toUpper();
					if (!owl2RDFXMLParserAdded && (upperFileString.endsWith(".OWX") || upperFileString.endsWith(".OWL"))) {
						owl2RDFXMLParserAdded = true;
						owl2XMLParserAdded = true;
						parserList.append(QString("OWL2XML"));
						parserList.append(QString("OWLRDFXML"));
					}if (!owl2RDFXMLParserAdded && (upperFileString.endsWith(".RDF.XML") || upperFileString.endsWith(".OWL.RDF.XML"))) {
						owl2RDFXMLParserAdded = true;
						parserList.append(QString("OWLRDFXML"));
					}
					if (!turtleParserAdded && (upperFileString.endsWith(".NT"))) {
						turtleParserAdded = true;
						parserList.append(QString("RDFNTRIPLES"));
					}
					if (!turtleParserAdded && (upperFileString.endsWith(".TURTLE") || upperFileString.endsWith(".TTL") || upperFileString.endsWith(".NT"))) {
						turtleParserAdded = true;
						parserList.append(QString("RDFTURTLE"));
					}
					if (!owl2XMLParserAdded && (upperFileString.endsWith(".OWL.XML") || upperFileString.endsWith(".XML.OWL"))) {
						owl2XMLParserAdded = true;
						parserList.append(QString("OWL2XML"));
					}
					if (!owl2FunctionalParserAdded && (upperFileString.endsWith(".OWL.FSS") || upperFileString.endsWith(".FSS.OWL"))) {
						owl2FunctionalParserAdded = true;
						parserList.append(QString("OWL2Functional"));
					}
					if (!owl2XMLParserAdded && (upperFileString.endsWith(".XML"))) {
						owl2XMLParserAdded = true;
						parserList.append(QString("OWL2XML"));
					}
					if (!owl2FunctionalParserAdded && (upperFileString.endsWith(".FSS"))) {
						owl2FunctionalParserAdded = true;
						parserList.append(QString("OWL2Functional"));
					}
					if (!owl2XMLParserAdded && (upperFileString.contains(".XML"))) {
						owl2XMLParserAdded = true;
						parserList.append(QString("OWL2XML"));
					}
					if (!owl2FunctionalParserAdded && (upperFileString.contains(".FSS"))) {
						owl2FunctionalParserAdded = true;
						parserList.append(QString("OWL2Functional"));
					}
					if (!owl2XMLParserAdded && (upperFileString.contains("XML"))) {
						owl2XMLParserAdded = true;
						parserList.append(QString("OWL2XML"));
					}
					if (!owl2FunctionalParserAdded && (upperFileString.contains("FSS"))) {
						owl2FunctionalParserAdded = true;
						parserList.append(QString("OWL2Functional"));
					}
					if (!owl2FunctionalParserAdded) {
						owl2FunctionalParserAdded = true;
						parserList.append(QString("OWL2Functional"));
					}
					if (!owl2XMLParserAdded) {
						owl2XMLParserAdded = true;
						parserList.append(QString("OWL2XML"));
					}
					if (!owl2RDFXMLParserAdded) {
						owl2RDFXMLParserAdded = true;
						parserList.append(QString("OWLRDFXML"));
					}
					if (!turtleParserAdded) {
						turtleParserAdded = true;
						parserList.append(QString("RDFTURTLE"));
					}
					return parserList;
				}


				CQtHttpTransactionManager* COWLlinkProcessor::getNetworkTransactionManager() {
					if (!mNetworkManager) {
						mNetworkManager = new CQtHttpTransactionManager(30*60*1000);
					}
					return mNetworkManager;
				}



#ifdef KONCLUDE_REDLAND_INTEGRATION

				bool COWLlinkProcessor::parseOntologyWithRaptor(QIODevice* device, CConcreteOntologyUpdateCollectorBuilder *builder, const QString& format, const QString& formatName, const QString& resolvedIRI, QString& parsingTryLogString, QStringList& parserErrorList, CConfiguration* configuration, CCommandRecordRouter& commandRecordRouter) {
					bool parsingSucceeded = false;
					CRDFRedlandRaptorParser *owl2Parser = new CRDFRedlandRaptorParser(builder, CTRIPLES_DATA_UPDATE_TYPE::TRIPLES_DATA_ADDITION, format, configuration);
					parsingTryLogString = QString("Trying %1 parsing of '%2' with Redland Raptor.").arg(formatName).arg(resolvedIRI);
					LOG(INFO, getLogDomain(), parsingTryLogString, this);
					if (device->open(QIODevice::ReadOnly)) {
						device->reset();
						if (owl2Parser->parseTriples(device, resolvedIRI)) {
							parsingSucceeded = true;
							LOG(INFO, getLogDomain(), logTr("Finished %1 parsing of '%2' with Redland Raptor.").arg(formatName).arg(resolvedIRI), this);
						} else {
							parserErrorList.append(QString("%1 parsing of '%2' with Redland Raptor failed due to error '%3'.").arg(formatName).arg(resolvedIRI).arg(owl2Parser->getErrorString()));
						}
						device->close();
					} else {
						CUnspecifiedMessageErrorRecord::makeRecord(QString("Data from '%1' cannot be read.").arg(resolvedIRI), &commandRecordRouter);
					}

					delete owl2Parser;
					return parsingSucceeded;
				}

#endif // !KONCLUDE_REDLAND_INTEGRATION



				bool COWLlinkProcessor::parseOntology(QIODevice* device, const QString& ontoIRI, const QString& resolvedIRI, CConcreteOntology *ont, COntologyConfigurationExtension *ontConfig, QList<QString>& importOntologiesList, QStringList& parserErrorList, CCommandRecordRouter& commandRecordRouter) {

					QString iriFileString = resolvedIRI;
					QStringList parserList = getParserOrderFromFileName(iriFileString);

					CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ont);
					builder->initializeBuilding();

					bool parsingSucceeded = false;
					bool triplesParsed = false;
					for (QStringList::const_iterator parserIt = parserList.constBegin(), parserItEnd = parserList.constEnd(); !parsingSucceeded && parserIt != parserItEnd; ++parserIt) {

						QString parserString(*parserIt);
						QString parsingTryLogString;

						if (parserString == "OWLRDFXML") {
#ifdef KONCLUDE_REDLAND_INTEGRATION
							parsingSucceeded = parseOntologyWithRaptor(device, builder, "rdfxml", "RDF/XML", resolvedIRI, parsingTryLogString, parserErrorList, ontConfig, commandRecordRouter);
							if (parsingSucceeded) {
								triplesParsed = true;
							}
#endif // !KONCLUDE_REDLAND_INTEGRATION

						} else if (parserString == "OWL2Functional") {
							COWL2FunctionalJAVACCOntologyStreamParser *owl2Parser = new COWL2FunctionalJAVACCOntologyStreamParser(builder);
							parsingTryLogString = QString("Trying stream-based OWL2/Functional ontology parsing for '%1'.").arg(iriFileString);
							LOG(INFO, getLogDomain(), parsingTryLogString, this);
							if (device->open(QIODevice::ReadOnly)) {
								device->reset();
								if (owl2Parser->parseOntology(device)) {
									parsingSucceeded = true;
									LOG(INFO, getLogDomain(), logTr("Finished stream-based OWL2/Functional ontology parsing for '%1'.").arg(iriFileString), this);
								} else {
									if (owl2Parser->hasError()) {
										parserErrorList.append(QString("Stream-based OWL2/Functional ontology parsing error: %1").arg(owl2Parser->getErrorString()));
									}
									parserErrorList.append(QString("Stream-based OWL2/Functional ontology parsing for '%1' failed.").arg(iriFileString));
								}
								device->close();
							} else {
								CUnspecifiedMessageErrorRecord::makeRecord(QString("Data for '%1' cannot be read.").arg(resolvedIRI), &commandRecordRouter);
							}

							delete owl2Parser;

						} else if (parserString == "OWL2XML") {

							bool enforceUTF8ConvertingParsing = CConfigDataReader::readConfigBoolean(ontConfig, "Konclude.Parser.UTF8CompatibilityEnforcedXMLStreamParsing");
							COWL2QtXMLOntologyStreamParser *owl2Parser = nullptr;
							if (enforceUTF8ConvertingParsing) {
								owl2Parser = new COWL2QtXMLOntologyStableStreamParser(builder);
							} else {
								owl2Parser = new COWL2QtXMLOntologyStreamParser(builder);
							}
							parsingTryLogString = QString("Trying stream-based OWL2/XML ontology parsing for '%1'.").arg(iriFileString);
							LOG(INFO, getLogDomain(), parsingTryLogString, this);
							if (device->open(QIODevice::ReadOnly)) {
								device->reset();
								if (owl2Parser->parseOntology(device)) {
									parsingSucceeded = true;
									LOG(INFO, getLogDomain(), logTr("Finished stream-based OWL2/XML ontology parsing for '%1'.").arg(iriFileString), this);
								} else {
									if (owl2Parser->hasError()) {
										parserErrorList.append(QString("Stream-based OWL2/XML ontology parsing error: %1").arg(owl2Parser->getErrorString()));
									}
									parserErrorList.append(QString("Stream-based OWL2/XML ontology parsing for '%1' failed.").arg(iriFileString));
								}
								device->close();
							} else {
								CUnspecifiedMessageErrorRecord::makeRecord(QString("Data for '%1' cannot be read.").arg(resolvedIRI), &commandRecordRouter);
							}

							delete owl2Parser;

						} else if (parserString == "RDFTURTLE") {

#ifdef KONCLUDE_REDLAND_INTEGRATION
							parsingSucceeded = parseOntologyWithRaptor(device, builder, "turtle", "RDF Turtle", resolvedIRI, parsingTryLogString, parserErrorList, ontConfig, commandRecordRouter);
							if (parsingSucceeded) {
								triplesParsed = true;
							}
#endif // !KONCLUDE_REDLAND_INTEGRATION

						} else if (parserString == "RDFNTRIPLES") {

#ifdef KONCLUDE_REDLAND_INTEGRATION
							parsingSucceeded = parseOntologyWithRaptor(device, builder, "ntriples", "RDF NTriples", resolvedIRI, parsingTryLogString, parserErrorList, ontConfig, commandRecordRouter);
							if (parsingSucceeded) {
								triplesParsed = true;
							}
#endif // !KONCLUDE_REDLAND_INTEGRATION

						}
					}

					if (triplesParsed) {
#ifdef KONCLUDE_REDLAND_INTEGRATION
						CConcreteOntologyRedlandTriplesDataExpressionMapper* triplesMapper = new CConcreteOntologyRedlandTriplesDataExpressionMapper(builder);
						triplesMapper->mapTriples(ont, ont->getOntologyTriplesData());
#endif // !KONCLUDE_REDLAND_INTEGRATION
					}
					if (parsingSucceeded) {
						builder->completeBuilding();
					}

					importOntologiesList.append(builder->takeAddedImportOntologyList());
					delete builder;

					return parsingSucceeded;
				}






				bool COWLlinkProcessor::createDownloadParseCommands(const QString& knowledgeBaseNameString, CCommand* parentCommand, CKnowledgeBaseRevisionCommandProvider* kbProviderCommand, const QMap<QString,QString>& ontoIRIMapping, const QStringList& ontoIRIStringList, CCommandRecordRouter& commandRecordRouter) {
					QList<CLoadKnowledgeBaseData*> loadDataList;
					QList<CCommand*> preconditionsList;

					cint64 downloadRequiredCount = 0;

					foreach (const QString& ontoIRIString, ontoIRIStringList) {

						QString resolvedString = ontoIRIString;
						for (QMap<QString,QString>::const_iterator resolveIRIIt = ontoIRIMapping.constBegin(), resolveIRIItEnd = ontoIRIMapping.constEnd(); resolveIRIIt != resolveIRIItEnd; ++resolveIRIIt) {
							QString identifyIRI(resolveIRIIt.key());
							QString resolvingIRI(resolveIRIIt.value());
							if (resolvedString.startsWith(identifyIRI)) {
								resolvedString = resolvedString.mid(identifyIRI.length());
								resolvedString = resolvingIRI + resolvedString;
							}
						}
						if (resolvedString != ontoIRIString) {
							CUnspecifiedMessageInformationRecord::makeRecord(QString("Redirecting '%1' to '%2'.").arg(ontoIRIString).arg(resolvedString),&commandRecordRouter);
						}

						if (resolvedString.startsWith("http:") || resolvedString.startsWith("https:") || resolvedString.startsWith("ftp:")) {
							QBuffer* tmpBuffer = new QBuffer();

							CLoadKnowledgeBaseFinishDownloadCommand* finishDownloadCommand = new CLoadKnowledgeBaseFinishDownloadCommand(resolvedString,tmpBuffer,preSynchronizer);
							parentCommand->makeToSubCommand(finishDownloadCommand);

							CLoadKnowledgeBaseStartDownloadCommand* startDownloadCommand = new CLoadKnowledgeBaseStartDownloadCommand(resolvedString,finishDownloadCommand);
							parentCommand->makeToSubCommand(startDownloadCommand);
							preSynchronizer->delegateCommand(startDownloadCommand);

							preconditionsList.append(startDownloadCommand);
							preconditionsList.append(finishDownloadCommand);
							++downloadRequiredCount;

							CLoadKnowledgeBaseData* loadData = new CLoadKnowledgeBaseData(tmpBuffer,ontoIRIString,resolvedString);
							loadDataList.append(loadData);

						} else {

							QString iriFileString = resolvedString;
							if (iriFileString.startsWith("file:")) {
								iriFileString.replace("file:","");
							}
							while (!QFile::exists(iriFileString) && iriFileString.startsWith("/")) {
								iriFileString = iriFileString.remove(0,1);
							}

							if (!QFile::exists(iriFileString)) {
								CUnspecifiedMessageErrorRecord::makeRecord(QString("File '%1' not found.").arg(resolvedString),&commandRecordRouter);
							} else {
								QFile* file = new QFile(iriFileString);
								CLoadKnowledgeBaseData* loadData = new CLoadKnowledgeBaseData(file,ontoIRIString,resolvedString);
								loadDataList.append(loadData);
							}

						}
					}

					CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand* chPaInKBOntComm = new CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand(knowledgeBaseNameString,loadDataList,ontoIRIMapping,kbProviderCommand);
					parentCommand->makeToSubCommand(chPaInKBOntComm);
					for (QList<CCommand*>::const_iterator it = preconditionsList.constBegin(), itEnd = preconditionsList.constEnd(); it != itEnd; ++it) {
						CCommand* preconditionCommand = *it;
						chPaInKBOntComm->addCommandPrecondition(new CCommandProcessedPrecondition(preconditionCommand));
					}
					if (kbProviderCommand->getCommand() != parentCommand) {
						chPaInKBOntComm->addCommandPrecondition(new CCommandProcessedPrecondition(kbProviderCommand->getCommand()));
					}
					preSynchronizer->delegateCommand(chPaInKBOntComm);
					return true;
				}



				bool COWLlinkProcessor::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (!CThread::processCustomsEvents(type,event)) {
						if (type == EVENTREALIZECOMMAND) {
							CRealizeCommandEvent *commandEvent = (CRealizeCommandEvent *)event;
							if (commandEvent) {
								CCommand *command = commandEvent->getCommand();
								if (command) {
									if (dynamic_cast<CTellKnowledgeBaseOWL2XMLNodeCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CTellKnowledgeBaseOWL2XMLNodeCommand *tellKBC = (CTellKnowledgeBaseOWL2XMLNodeCommand *)command;

										CCreateKnowledgeBaseRevisionUpdateCommand *crKBRevUpC = new CCreateKnowledgeBaseRevisionUpdateCommand(tellKBC->getKnowledgeBaseName());
										tellKBC->makeToSubCommand(crKBRevUpC);
										preSynchronizer->delegateCommand(crKBRevUpC);

										CParseOWL2XMLOntologyCommand *pOWLOntC = new CParseOWL2XMLOntologyCommand(tellKBC->getOWL2Node(),crKBRevUpC);
										tellKBC->makeToSubCommand(pOWLOntC);
										preSynchronizer->delegateCommand(pOWLOntC);

										CInstallKnowledgeBaseRevisionUpdateCommand *inKBRevUpC = new CInstallKnowledgeBaseRevisionUpdateCommand(tellKBC->getKnowledgeBaseName(),pOWLOntC);
										inKBRevUpC->addCommandPrecondition(new CCommandProcessedPrecondition(pOWLOntC));
										tellKBC->makeToSubCommand(inKBRevUpC);
										preSynchronizer->delegateCommand(inKBRevUpC);

										CPrepareKnowledgeBaseForRevisionCommand* prepareKBC = new CPrepareKnowledgeBaseForRevisionCommand(crKBRevUpC);
										tellKBC->makeToSubCommand(prepareKBC);
										prepareKBC->addCommandPrecondition(new CCommandProcessedPrecondition(inKBRevUpC));
										preSynchronizer->delegateCommand(prepareKBC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CLoadKnowledgeBaseKRSSTestConceptCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CLoadKnowledgeBaseKRSSTestConceptCommand *loadKBC = (CLoadKnowledgeBaseKRSSTestConceptCommand *)command;

										CCreateKnowledgeBaseRevisionUpdateCommand *crKBRevUpC = new CCreateKnowledgeBaseRevisionUpdateCommand(loadKBC->getKnowledgeBaseName());
										loadKBC->makeToSubCommand(crKBRevUpC);
										preSynchronizer->delegateCommand(crKBRevUpC);

										CReadTextCommand *readTextC = new CReadTextCommand(loadKBC->getLoadIRIString());
										loadKBC->makeToSubCommand(readTextC);
										preSynchronizer->delegateCommand(readTextC);

										CParseKRSSTestConceptOntologyCommand *pOWLOntC = new CParseKRSSTestConceptOntologyCommand(readTextC,crKBRevUpC,loadKBC->getTestConceptNameString());
										loadKBC->makeToSubCommand(pOWLOntC);
										preSynchronizer->delegateCommand(pOWLOntC);

										CInstallKnowledgeBaseRevisionUpdateCommand *inKBRevUpC = new CInstallKnowledgeBaseRevisionUpdateCommand(loadKBC->getKnowledgeBaseName(),pOWLOntC);
										inKBRevUpC->addCommandPrecondition(new CCommandProcessedPrecondition(pOWLOntC));
										loadKBC->makeToSubCommand(inKBRevUpC);
										preSynchronizer->delegateCommand(inKBRevUpC);

										CPrepareKnowledgeBaseForRevisionCommand* prepareKBC = new CPrepareKnowledgeBaseForRevisionCommand(crKBRevUpC);
										loadKBC->makeToSubCommand(prepareKBC);
										prepareKBC->addCommandPrecondition(new CCommandProcessedPrecondition(inKBRevUpC));
										preSynchronizer->delegateCommand(prepareKBC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CPrepareKnowledgeBaseForRevisionCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CPrepareKnowledgeBaseForRevisionCommand *prepareKBC = (CPrepareKnowledgeBaseForRevisionCommand *)command;

										COntologyRevision* ontoRev = prepareKBC->getOntologyRevision();
										bool preparingOntology = false;
										if (ontoRev) {
											CConfigurationBase* config = ontoRev->getOntologyConfiguration();

											COntologyProcessingStepVector* ontProStepVec = COntologyProcessingStepVector::getProcessingStepVectorInstance();
											bool build = CConfigDataReader::readConfigBoolean(config,"Konclude.Ontology.Revision.BuildAfterModification");
											if (build) {
												prepareKBC->addOntologyProcessingStepRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSBUILD), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
											}
											bool preprocess = CConfigDataReader::readConfigBoolean(config,"Konclude.Ontology.Revision.PreprocessAfterModification");
											if (preprocess) {
												prepareKBC->addOntologyProcessingStepRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPREPROCESS), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
											}
											bool consistency = CConfigDataReader::readConfigBoolean(config,"Konclude.Ontology.Revision.ConsistencyPrecomputationAfterModification");
											if (consistency) {
												prepareKBC->addOntologyProcessingStepRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSCONSISTENCY), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
											}
											bool cycles = CConfigDataReader::readConfigBoolean(config,"Konclude.Ontology.Revision.CyclesPrecomputationAfterModification");
											if (cycles) {
												prepareKBC->addOntologyProcessingStepRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPRECOMPUTECYCLES), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
											}
											bool classify = CConfigDataReader::readConfigBoolean(config,"Konclude.Ontology.Revision.ClassifyAfterModification");
											if (classify) {
												prepareKBC->addOntologyProcessingStepRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSCLASSCLASSIFY), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
											}

											if (!prepareKBC->getOntologyProcessingRequirementList()->isEmpty() && reasonerCommander) {
												CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
												preparingOntology = true;
												reasonerCommander->realizeCommand(command);
											}
										}

										if (!preparingOntology) {
											CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
											CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
										}



									} else if (dynamic_cast<CLoadKnowledgeBaseOWLFunctionalOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CLoadKnowledgeBaseOWLFunctionalOntologyCommand *loadKBOWLXMLOnto = (CLoadKnowledgeBaseOWLFunctionalOntologyCommand *)command;

										CCreateKnowledgeBaseRevisionUpdateCommand *crKBRevUpC = new CCreateKnowledgeBaseRevisionUpdateCommand(loadKBOWLXMLOnto->getKnowledgeBaseName());
										loadKBOWLXMLOnto->makeToSubCommand(crKBRevUpC);
										preSynchronizer->delegateCommand(crKBRevUpC);

										CCommand* lastWaitCommand = crKBRevUpC;
										CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand = nullptr;

										QStringList ontoIRIs(loadKBOWLXMLOnto->getLoadIRIStrings());
										foreach (QString ontoIRI, ontoIRIs) {

											CStreamParseOWL2FunctionalOntologyCommand *pOWLOntC = new CStreamParseOWL2FunctionalOntologyCommand(ontoIRI,crKBRevUpC);
											if (lastWaitCommand) {
												pOWLOntC->addCommandPrecondition(new CCommandProcessedPrecondition(lastWaitCommand));
											}

											loadKBOWLXMLOnto->makeToSubCommand(pOWLOntC);
											preSynchronizer->delegateCommand(pOWLOntC);

											ontologyRevisionProviderCommand = pOWLOntC;
											lastWaitCommand = pOWLOntC;
										}

										CInstallKnowledgeBaseRevisionUpdateCommand *inKBRevUpC = new CInstallKnowledgeBaseRevisionUpdateCommand(loadKBOWLXMLOnto->getKnowledgeBaseName(),ontologyRevisionProviderCommand);
										inKBRevUpC->addCommandPrecondition(new CCommandProcessedPrecondition(lastWaitCommand));
										loadKBOWLXMLOnto->makeToSubCommand(inKBRevUpC);
										preSynchronizer->delegateCommand(inKBRevUpC);

										CPrepareKnowledgeBaseForRevisionCommand* prepareKBC = new CPrepareKnowledgeBaseForRevisionCommand(ontologyRevisionProviderCommand);
										loadKBOWLXMLOnto->makeToSubCommand(prepareKBC);
										prepareKBC->addCommandPrecondition(new CCommandProcessedPrecondition(inKBRevUpC));
										preSynchronizer->delegateCommand(prepareKBC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);




									} else if (dynamic_cast<CLoadKnowledgeBaseOWLXMLOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CLoadKnowledgeBaseOWLXMLOntologyCommand *loadKBOWLXMLOnto = (CLoadKnowledgeBaseOWLXMLOntologyCommand *)command;

										CCreateKnowledgeBaseRevisionUpdateCommand *crKBRevUpC = new CCreateKnowledgeBaseRevisionUpdateCommand(loadKBOWLXMLOnto->getKnowledgeBaseName());
										loadKBOWLXMLOnto->makeToSubCommand(crKBRevUpC);
										preSynchronizer->delegateCommand(crKBRevUpC);

										QStringList ontoIRIs(loadKBOWLXMLOnto->getLoadIRIStrings());

										CChooseParseInstallKnowledgeBaseOWLXMLOntologyCommand* chPaInKBOntComm = new CChooseParseInstallKnowledgeBaseOWLXMLOntologyCommand(loadKBOWLXMLOnto->getKnowledgeBaseName(),ontoIRIs,crKBRevUpC);
										loadKBOWLXMLOnto->makeToSubCommand(chPaInKBOntComm);
										preSynchronizer->delegateCommand(chPaInKBOntComm);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);




									} else if (dynamic_cast<CChooseParseInstallKnowledgeBaseOWLXMLOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CChooseParseInstallKnowledgeBaseOWLXMLOntologyCommand* chPaInKBOntComm = (CChooseParseInstallKnowledgeBaseOWLXMLOntologyCommand *)command;

										CConfigData* data = nullptr;
										COntologyRevision* ontoRev = chPaInKBOntComm->getOntologyRevision();
										if (ontoRev) {
											COntologyConfigurationExtension* ontoConfExt = chPaInKBOntComm->getOntologyRevision()->getOntologyConfiguration();

											bool confLoadOntologiesStreamParsing = mConfLoadOntologiesStreamParsing;
											bool confLoadOntologiesSAXParsing = mConfLoadOntologiesSAXParsing;
											data = ontoConfExt->getRelatedConfigChange("Konclude.OWLlink.StreamBasedLoadOntologiesParsing");
											if (data) {
												confLoadOntologiesStreamParsing = data->getBoolean();
											}
											data = ontoConfExt->getRelatedConfigChange("Konclude.OWLlink.SAXBasedLoadOntologiesParsing");
											if (data) {
												confLoadOntologiesSAXParsing = data->getBoolean();
											}

											QStringList ontoIRIs(chPaInKBOntComm->getLoadIRIStrings());

											CCommand* lastWaitCommand = nullptr;
											CKnowledgeBaseRevisionCommandProvider *ontologyRevisionProviderCommand = nullptr;

											foreach (QString ontoIRI, ontoIRIs) {
												if (confLoadOntologiesStreamParsing) {
													CStreamParseOWL2XMLOntologyCommand *pOWLOntC = new CStreamParseOWL2XMLOntologyCommand(ontoIRI,chPaInKBOntComm);
													if (lastWaitCommand) {
														pOWLOntC->addCommandPrecondition(new CCommandProcessedPrecondition(lastWaitCommand));
													}

													chPaInKBOntComm->makeToSubCommand(pOWLOntC);
													preSynchronizer->delegateCommand(pOWLOntC);

													ontologyRevisionProviderCommand = pOWLOntC;
													lastWaitCommand = pOWLOntC;
												} else if (confLoadOntologiesSAXParsing) {
													CSAXParseOWL2XMLOntologyCommand *pOWLOntC = new CSAXParseOWL2XMLOntologyCommand(ontoIRI,chPaInKBOntComm);
													if (lastWaitCommand) {
														pOWLOntC->addCommandPrecondition(new CCommandProcessedPrecondition(lastWaitCommand));
													}
													chPaInKBOntComm->makeToSubCommand(pOWLOntC);
													preSynchronizer->delegateCommand(pOWLOntC);

													ontologyRevisionProviderCommand = pOWLOntC;
													lastWaitCommand = pOWLOntC;
												} else {
													CReadXMLCommand *readXMLC = new CReadXMLCommand(ontoIRI);
													chPaInKBOntComm->makeToSubCommand(readXMLC);
													preSynchronizer->delegateCommand(readXMLC);

													CParseOWL2XMLOntologyCommand *pOWLOntC = new CParseOWL2XMLOntologyCommand(readXMLC,chPaInKBOntComm);
													if (lastWaitCommand) {
														pOWLOntC->addCommandPrecondition(new CCommandProcessedPrecondition(lastWaitCommand));
													}
													chPaInKBOntComm->makeToSubCommand(pOWLOntC);
													preSynchronizer->delegateCommand(pOWLOntC);

													ontologyRevisionProviderCommand = pOWLOntC;
													lastWaitCommand = pOWLOntC;
												}

											}

											CInstallKnowledgeBaseRevisionUpdateCommand *inKBRevUpC = new CInstallKnowledgeBaseRevisionUpdateCommand(chPaInKBOntComm->getKnowledgeBaseName(),ontologyRevisionProviderCommand);
											inKBRevUpC->addCommandPrecondition(new CCommandProcessedPrecondition(lastWaitCommand));
											chPaInKBOntComm->makeToSubCommand(inKBRevUpC);
											preSynchronizer->delegateCommand(inKBRevUpC);

											CPrepareKnowledgeBaseForRevisionCommand* prepareKBC = new CPrepareKnowledgeBaseForRevisionCommand(ontologyRevisionProviderCommand);
											chPaInKBOntComm->makeToSubCommand(prepareKBC);
											prepareKBC->addCommandPrecondition(new CCommandProcessedPrecondition(inKBRevUpC));
											preSynchronizer->delegateCommand(prepareKBC);
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Ontology revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);







									} else if (dynamic_cast<CLoadKnowledgeBaseStartDownloadCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CLoadKnowledgeBaseStartDownloadCommand* loadKBStartDownload = (CLoadKnowledgeBaseStartDownloadCommand *)command;

										CLoadKnowledgeBaseFinishDownloadCommand* finishCommand = loadKBStartDownload->getFinishCommand();
										QByteArray* destinationDataArray = new QByteArray();

										CQtHttpTransactionManager* transMan = getNetworkTransactionManager();
										CHttpRequest* request = transMan->createRequest(loadKBStartDownload->getDownloadIRIString());
										CHttpResponse* response = transMan->getResponse(request);
										transMan->callbackResponseData(response,destinationDataArray,finishCommand);

										finishCommand->setTemporaryDataPointer(destinationDataArray);
										finishCommand->setResponse(response);

										LOG(INFO,getLogDomain(),logTr("Downloading '%1'.").arg(loadKBStartDownload->getDownloadIRIString()),this);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);



									} else if (dynamic_cast<CLoadKnowledgeBaseFinishDownloadCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CLoadKnowledgeBaseFinishDownloadCommand* loadKBFinishDownload = (CLoadKnowledgeBaseFinishDownloadCommand *)command;

										CQtHttpTransactionManager* transMan = getNetworkTransactionManager();
										CHttpResponse* response = loadKBFinishDownload->getResponse();
										QString iriString = loadKBFinishDownload->getDownloadString();
										QByteArray* dataArray = loadKBFinishDownload->getTemporaryDataPointer(true);


										if (!transMan->hasFinishedSucecssfully(response)) {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Download of '%1' has failed.").arg(iriString),&commandRecordRouter);
										} else {
											cint64 downloadSize = dataArray->count();
											QString unitString = "Bytes";
											if (downloadSize > 10000) {
												downloadSize /= 1024;
												unitString = "KBytes";
											}
											if (downloadSize > 10000) {
												downloadSize /= 1024;
												unitString = "MBytes";
											}
											LOG(INFO,getLogDomain(),logTr("Successfully downloaded '%1', received %2 %3.").arg(iriString).arg(downloadSize).arg(unitString),this);
											//CUnspecifiedMessageInformationRecord::makeRecord(QString("Successfully downloaded '%1', received %2 MBytes.").arg(iriString).arg(downloadSize),&commandRecordRouter);
											QIODevice* writeDevice = loadKBFinishDownload->getWriteIODevice();
											if (writeDevice) {
												if (writeDevice->open(QIODevice::WriteOnly)) {
													writeDevice->write(*dataArray);
													writeDevice->close();
												}
											}
										}
										delete dataArray;
										transMan->releaseResponse(response);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);






									} else if (dynamic_cast<CLoadKnowledgeBaseOWLAutoOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CLoadKnowledgeBaseOWLAutoOntologyCommand *loadKBOWLAutoOnto = (CLoadKnowledgeBaseOWLAutoOntologyCommand *)command;

										CCreateKnowledgeBaseRevisionUpdateCommand *crKBRevUpC = new CCreateKnowledgeBaseRevisionUpdateCommand(loadKBOWLAutoOnto->getKnowledgeBaseName());
										loadKBOWLAutoOnto->makeToSubCommand(crKBRevUpC);
										preSynchronizer->delegateCommand(crKBRevUpC);


										QMap<QString,QString> ontoIRIMapping = loadKBOWLAutoOnto->getOntologieIRIMappings();

										QStringList ontoIRIStringList(loadKBOWLAutoOnto->getLoadIRIStrings());

										createDownloadParseCommands(loadKBOWLAutoOnto->getKnowledgeBaseName(),loadKBOWLAutoOnto,crKBRevUpC,ontoIRIMapping,ontoIRIStringList,commandRecordRouter);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);






									} else if (dynamic_cast<CImportKnowledgeBaseOWLAutoOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CImportKnowledgeBaseOWLAutoOntologyCommand *importKBOWLAutoOnto = (CImportKnowledgeBaseOWLAutoOntologyCommand *)command;


										QMap<QString,QString> ontoIRIMapping = importKBOWLAutoOnto->getOntologieIRIMappings();

										QStringList ontoIRIStringList(importKBOWLAutoOnto->getLoadIRIStrings());

										createDownloadParseCommands(importKBOWLAutoOnto->getKnowledgeBaseName(),importKBOWLAutoOnto,importKBOWLAutoOnto,ontoIRIMapping,ontoIRIStringList,commandRecordRouter);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);







									} else if (dynamic_cast<CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand* chPaInKBOntComm = (CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand *)command;

										CConfigData* data = nullptr;
										COntologyRevision* ontoRev = chPaInKBOntComm->getOntologyRevision();
										if (ontoRev) {
											COntologyConfigurationExtension* ontoConfExt = chPaInKBOntComm->getOntologyRevision()->getOntologyConfiguration();

											COntologyConfigurationExtension *ontConfig = ontoRev->getOntologyConfiguration();
											CConcreteOntology* ont = ontoRev->getOntology();


											QList<QString> importOntologiesList;

											QList<CLoadKnowledgeBaseData*> loadDataList(chPaInKBOntComm->getLoadDataList());

											bool allParsingSucceeded = true;

											for (QList<CLoadKnowledgeBaseData*>::const_iterator loadDataIt = loadDataList.constBegin(), loadDataItEnd = loadDataList.constEnd(); loadDataIt != loadDataItEnd; ++loadDataIt) {
												CLoadKnowledgeBaseData* loadData = *loadDataIt;

												QIODevice* device = loadData->getLoadDevice();
												QString ontoIRI = loadData->getOriginalDataName();
												QString resolvedIRI = loadData->getResolvedDataName();


												QTime parsingTime;
												if (mConfLogProcessingTimes) {
													parsingTime.start();
												}

												QStringList parserErrorList;


												bool parsingSucceeded = parseOntology(device, ontoIRI, resolvedIRI, ont, ontConfig, importOntologiesList, parserErrorList, commandRecordRouter);


												if (!parsingSucceeded) {
													allParsingSucceeded = false;
													parserErrorList.append(QString("All parsers failed for '%1'.").arg(resolvedIRI));
													for (QStringList::const_iterator errorIt = parserErrorList.constBegin(), errorItEnd = parserErrorList.constEnd(); errorIt != errorItEnd; ++errorIt) {
														CUnspecifiedMessageErrorRecord::makeRecord(*errorIt,&commandRecordRouter);
													}
												} else {
													ont->getLoadData()->addLoadedOntologyIRI(ontoIRI, resolvedIRI);
												}


												if (mConfLogProcessingTimes) {
													cint64 parsingMilliSeconds = parsingTime.elapsed();
													QString ontologieCountString = "Ontology";
													if (loadDataList.count() > 1) {
														ontologieCountString = "Ontologies";
													}
													LOG(INFO,getLogDomain(),logTr("%1 parsed in %2 ms.").arg(ontologieCountString).arg(parsingMilliSeconds),this);
												}

											}

											if (importOntologiesList.isEmpty()) {
												CInstallKnowledgeBaseRevisionUpdateCommand *inKBRevUpC = new CInstallKnowledgeBaseRevisionUpdateCommand(chPaInKBOntComm->getKnowledgeBaseName(),chPaInKBOntComm);
												chPaInKBOntComm->makeToSubCommand(inKBRevUpC);
												preSynchronizer->delegateCommand(inKBRevUpC);

												CPrepareKnowledgeBaseForRevisionCommand* prepareKBC = new CPrepareKnowledgeBaseForRevisionCommand(chPaInKBOntComm);
												chPaInKBOntComm->makeToSubCommand(prepareKBC);
												prepareKBC->addCommandPrecondition(new CCommandProcessedPrecondition(inKBRevUpC));
												preSynchronizer->delegateCommand(prepareKBC);
											} else {
												if (importOntologiesList.count() > 1) {
													LOG(INFO,getLogDomain(),logTr("Scheduling the import of %1 ontologies.").arg(importOntologiesList.count()),this);
												} else {
													LOG(INFO,getLogDomain(),logTr("Scheduling the import of '%1'.").arg(importOntologiesList.first()),this);
												}
												CImportKnowledgeBaseOWLAutoOntologyCommand* importKBsCommand = new CImportKnowledgeBaseOWLAutoOntologyCommand(chPaInKBOntComm->getKnowledgeBaseName(),importOntologiesList,chPaInKBOntComm->getOntologieIRIMappings(),chPaInKBOntComm);
												chPaInKBOntComm->makeToSubCommand(importKBsCommand);
												preSynchronizer->delegateCommand(importKBsCommand);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Ontology revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);






									} else if (dynamic_cast<CSaveOWL2XMLExtractedKnowledgeBaseCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CSaveOWL2XMLExtractedKnowledgeBaseCommand *saveKBC = (CSaveOWL2XMLExtractedKnowledgeBaseCommand *)command;

										CGetCurrentKnowledgeBaseRevisionCommand *gcKBRevUpC = new CGetCurrentKnowledgeBaseRevisionCommand(saveKBC->getKnowledgeBaseName());
										saveKBC->makeToSubCommand(gcKBRevUpC);
										preSynchronizer->delegateCommand(gcKBRevUpC);

										COntologyOWL2QtXMLRenderer* renderer = new COntologyOWL2QtXMLRenderer();

										CExtractToldExpressionsOntologyCommand *extTEOC = new CExtractToldExpressionsOntologyCommand(renderer,gcKBRevUpC,saveKBC->getExtractEntitiyList());
										saveKBC->makeToSubCommand(extTEOC);
										preSynchronizer->delegateCommand(extTEOC);

										CWriteOWL2XMLOntologyCommand *writeOntC = new CWriteOWL2XMLOntologyCommand(renderer,saveKBC->getSaveIRIString());
										writeOntC->addCommandPrecondition(new CCommandProcessedPrecondition(extTEOC));
										saveKBC->makeToSubCommand(writeOntC);
										preSynchronizer->delegateCommand(writeOntC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(command->getRecorder(),getLogDomain(),command);
										CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand *saveKBC = (CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand *)command;

										CGetCurrentKnowledgeBaseRevisionCommand *gcKBRevUpC = new CGetCurrentKnowledgeBaseRevisionCommand(saveKBC->getKnowledgeBaseName());
										saveKBC->makeToSubCommand(gcKBRevUpC);
										preSynchronizer->delegateCommand(gcKBRevUpC);

										COntologyOWL2QtXMLRenderer* renderer = new COntologyOWL2QtXMLRenderer();

										CExtractToldExpressionsOntologyCommand *extTEOC = new CExtractToldExpressionsOntologyCommand(renderer,gcKBRevUpC,saveKBC->getExtractEntitiyList());
										saveKBC->makeToSubCommand(extTEOC);
										preSynchronizer->delegateCommand(extTEOC);

										CWriteOWLlinkOWL2XMLSatisfiableTestCommand *writeOntC = new CWriteOWLlinkOWL2XMLSatisfiableTestCommand(renderer,saveKBC->getSaveIRIString(),saveKBC->getExtractEntitiyList(),saveKBC->getTestKBName());
										writeOntC->addCommandPrecondition(new CCommandProcessedPrecondition(extTEOC));
										saveKBC->makeToSubCommand(writeOntC);
										preSynchronizer->delegateCommand(writeOntC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CExtractOntologyCommand *>(command)) {
										COntologyExtractionManager exMan;
										exMan.realizeCommand(command);

									} else if (dynamic_cast<CWriteOntologyCommand *>(command)) {
										CRenderWriteManager writeMan;
										writeMan.realizeCommand(command);

									} else if (dynamic_cast<CReadTextCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CReadTextCommand* readTextC = (CReadTextCommand *)command;

										QString iriString = readTextC->getIRIString();
										QFile file(iriString);
										if (file.open(QIODevice::ReadOnly)) {
											CUnspecifiedMessageInformationRecord::makeRecord(QString("Read file '%1'.").arg(iriString),&commandRecordRouter);
											readTextC->setReadText(file.readAll());
											file.close();
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("File '%1' not found.").arg(iriString),&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);



									} else if (dynamic_cast<CReadXMLCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CReadXMLCommand* readXMLC = (CReadXMLCommand *)command;

										QString iriString = readXMLC->getIRIString();
										if (iriString.startsWith("file:")) {
											iriString.replace("file:","");
										}
										while (!QFile::exists(iriString) && iriString.startsWith("/")) {
											iriString = iriString.remove(0,1);
										}
										QFile file(iriString);
										if (file.open(QIODevice::ReadOnly)) {
											CUnspecifiedMessageInformationRecord::makeRecord(QString("Read file '%1'.").arg(iriString),&commandRecordRouter);

											QDomDocument document;
											QString errorMessage;
											int errorLine = 0, errorColumn = 0;
											if (document.setContent(file.readAll(),true,&errorMessage,&errorLine,&errorColumn)) {
												readXMLC->setReadXMLNode(document.documentElement());
											}

											file.close();
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("File '%1' not found.").arg(iriString),&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CParseKRSSTestConceptOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CParseKRSSTestConceptOntologyCommand *krssTCOntC = (CParseKRSSTestConceptOntologyCommand *)command;


										COntologyRevision *ontRev = krssTCOntC->getOntologyRevision();
										if (ontRev) {
											CConcreteOntology *ont = ontRev->getOntology();

											QTime parsingTime;
											if (mConfLogProcessingTimes) {
												parsingTime.start();
											}

											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();

											CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ont);
											CKRSSOntologyParser *krssParser = new CKRSSOntologyParser(builder,krssTCOntC->getTestConceptNameString(),&commandRecordRouter);
											QString krssText = krssTCOntC->getOntologyKRSSText();
											builder->initializeBuilding();
											krssParser->parseOntologyText(krssText);
											builder->completeBuilding();
											delete builder;
											delete krssParser;

											//COntologyConfigDependedPreProcesser *ontConDepPreProcessor = new COntologyConfigDependedPreProcesser(commandRecordRouter);
											//ontConDepPreProcessor->preprocess(ont,ontConfig);
											//delete ontConDepPreProcessor;


											if (mConfLogProcessingTimes) {
												cint64 parsingMilliSeconds = parsingTime.elapsed();
												LOG(INFO,getLogDomain(),logTr("Ontology parsed in %1 ms.").arg(parsingMilliSeconds),this);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Knowledge base revision not available."),&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);







									} else if (dynamic_cast<CParseSPARQLQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command, this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CParseSPARQLQueryCommand *pSPARQLQueryC = (CParseSPARQLQueryCommand*)command;


										COntologyRevision *ontRev = pSPARQLQueryC->getOntologyRevision();
										if (ontRev) {
											CConcreteOntology* onto = ontRev->getOntology();
											CConcreteOntology* baseOnt = ontRev->getPreviousOntologyRevision()->getOntology();

											QTime parsingTime;
											if (mConfLogProcessingTimes) {
												parsingTime.start();
											}

											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();

											CConcreteOntologyUpdateSeparateHashingCollectorBuilder *builder = new CConcreteOntologyUpdateSeparateHashingCollectorBuilder(onto);
											CConcreteOntologyQueryExtendedBuilder* queryBuilderGen = new CConcreteOntologyQueryExtendedBuilder(baseOnt, onto, ontConfig, builder);
											CSPARQLSimpleQueryParser* sparqlQueryParser = new CSPARQLSimpleQueryParser(queryBuilderGen, builder, onto);

                                            QStringList queryStringList = pSPARQLQueryC->getSPARQLQueryTextList();
											builder->initializeBuilding();
											sparqlQueryParser->parseQueryTextList(queryStringList);
											builder->completeBuilding();
											QList<CQuery*> queryList = queryBuilderGen->generateQuerys();
											if (queryList.isEmpty()) {
												LOG(WARN, getLogDomain(), logTr("No SPARQL query found in request."), this);
											}
											pSPARQLQueryC->setQueryList(queryList);
											delete builder;
											delete queryBuilderGen;
											delete sparqlQueryParser;

											if (mConfLogProcessingTimes) {
												cint64 parsingMilliSeconds = parsingTime.elapsed();
												LOG(INFO, getLogDomain(), logTr("SPARQL Query parsed in %1 ms.").arg(parsingMilliSeconds), this);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Knowledge base revision not available."), &commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);




									} else if (dynamic_cast<CParseProcessSPARQLUpdateTextCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command, this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CParseProcessSPARQLUpdateTextCommand *pSPARQLUpdateC = (CParseProcessSPARQLUpdateTextCommand*)command;


										COntologyRevision *ontRev = pSPARQLUpdateC->getOntologyRevision();
										if (ontRev) {
											CConcreteOntology* onto = ontRev->getOntology();
											CConcreteOntology* baseOnt = ontRev->getPreviousOntologyRevision()->getOntology();

											QTime parsingTime;
											if (mConfLogProcessingTimes) {
												parsingTime.start();
											}

											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();

											CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(onto);

											CSPARQLSimpleUpdateParser* sparqlUpdateParser = new CSPARQLSimpleUpdateParser(builder, onto);
											builder->initializeBuilding();
											if (pSPARQLUpdateC->isUpdateTextSplitted()) {
												QStringList updateStringList = pSPARQLUpdateC->getSPARQLUpdateTextList();
												sparqlUpdateParser->parseUpdateTextList(updateStringList);
											} else {
												QString updateString = pSPARQLUpdateC->getSPARQLUpdateText();
												sparqlUpdateParser->parseUpdateText(updateString);
											}
											builder->completeBuilding();
											delete builder;
											delete sparqlUpdateParser;

											if (mConfLogProcessingTimes) {
												cint64 parsingMilliSeconds = parsingTime.elapsed();
												LOG(INFO, getLogDomain(), logTr("SPARQL Update parsed in %1 ms.").arg(parsingMilliSeconds), this);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Knowledge base revision not available."), &commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);



									} else if (dynamic_cast<CParseProcessSPARQLManageTextCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command, this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CParseProcessSPARQLManageTextCommand *pSPARQLUpdateC = (CParseProcessSPARQLManageTextCommand*)command;


										CSPARQLSimpleManagementParser managementParser(preSynchronizer, pSPARQLUpdateC->getKnowlegeBaseString(), pSPARQLUpdateC);
										managementParser.parseUpdateTextList(pSPARQLUpdateC->getSPARQLUpdateTextList());

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CCalculateQueriesCommand*>(command)) {
										CCommandRecordRouter commandRecordRouter(command, this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CCalculateQueriesCommand *calcQueriesC = (CCalculateQueriesCommand*)command;

										for (CQuery* query : calcQueriesC->getQueryList()) {
											if (query) {
												CCalculateQueryCommand* calcQueryC = new CCalculateQueryCommand(query, calcQueriesC);
												preSynchronizer->delegateCommand(calcQueryC);
											}
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CParseProcessSPARQLTextCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command, this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CParseProcessSPARQLTextCommand *pSPARQLQueryC = (CParseProcessSPARQLTextCommand *)command;

										CConfiguration* configuration = getConfiguration();
										bool ignoreFromClause = CConfigDataReader::readConfigBoolean(configuration, "Konclude.SPARQL.AlwaysResolveToDefault");


										CSPARQLKnowledgeBaseSplittingOperationParser* sparqlSplittingParser = new CSPARQLKnowledgeBaseSplittingOperationParser();
										const QString& queryString = pSPARQLQueryC->getSPARQLQueryText();
										sparqlSplittingParser->parseQueryText(queryString);
										bool foundSparqlOperation = false;
										for (QString kbString : sparqlSplittingParser->getKnowlegdeBaseList()) {
											QList<CSPARQLKnowledgeBaseSplittingOperationData*> queryStringList = sparqlSplittingParser->getKnowlegdeBaseSPARQLOperationList(kbString);

											if (ignoreFromClause || kbString.trimmed().isEmpty()) {
												kbString = CConfigDataReader::readConfigString(configuration, "Konclude.SPARQL.DefaultResolvingOntology");
											}

											CCreateKnowledgeBaseRevisionUpdateCommand* lastGetCurrKBRevC = nullptr;
											CParseSPARQLQueryCommand* lastSparqlQC = nullptr;

											CCommand* lastNonQueryC = nullptr;

											QList<CCommand*> commandList;

											while (!queryStringList.isEmpty()) {
												CSPARQLKnowledgeBaseSplittingOperationData* splitKBOperationData(queryStringList.takeLast());
												QStringList textList = splitKBOperationData->getOperationPartStringList();
												CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_COMMAND_TYPE operationType = splitKBOperationData->getOperationType();
												if (operationType == CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_QUERY) {
													foundSparqlOperation = true;
													if (!lastGetCurrKBRevC) {
														lastGetCurrKBRevC = new CCreateKnowledgeBaseRevisionUpdateCommand(kbString, pSPARQLQueryC);
														if (lastNonQueryC) {
															lastGetCurrKBRevC->addCommandPrecondition(new CCommandProcessedPrecondition(lastNonQueryC));
														}
														commandList.append(lastGetCurrKBRevC);
													}
													if (!lastSparqlQC) {
														lastSparqlQC = new CParseSPARQLQueryCommand(textList, lastGetCurrKBRevC, pSPARQLQueryC);
														commandList.append(lastSparqlQC);
														CCalculateQueriesCommand* calcQueriesC = new CCalculateQueriesCommand(lastSparqlQC, pSPARQLQueryC);
														commandList.append(calcQueriesC);
													} else {
														lastSparqlQC->addQueryText(textList);
													}
												} else if (operationType == CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MANAGE) {
													foundSparqlOperation = true;

													// some SPARQL update commands may have dependencies to several graphs/knowledge bases, e.g., COPY, i.e., we need a more sophisticated mechanism at some point
													CParseProcessSPARQLManageTextCommand* ppSPARQLUTC = new CParseProcessSPARQLManageTextCommand(textList, kbString, pSPARQLQueryC);
													commandList.append(ppSPARQLUTC);
													if (lastNonQueryC) {
														ppSPARQLUTC->addCommandPrecondition(new CCommandProcessedPrecondition(lastNonQueryC));
													}

													lastNonQueryC = ppSPARQLUTC;

												} else if (operationType == CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MODIFY) {
													foundSparqlOperation = true;
													lastGetCurrKBRevC = nullptr;
													lastSparqlQC = nullptr;

													CCreateKnowledgeBaseRevisionUpdateCommand* getCurrKBRevC = new CCreateKnowledgeBaseRevisionUpdateCommand(kbString, true, pSPARQLQueryC);
													if (lastNonQueryC) {
														getCurrKBRevC->addCommandPrecondition(new CCommandProcessedPrecondition(lastNonQueryC));
													}
													commandList.append(getCurrKBRevC);

													CParseProcessSPARQLUpdateTextCommand* ppSPARQLUTC = new CParseProcessSPARQLUpdateTextCommand(textList, getCurrKBRevC, pSPARQLQueryC);
													commandList.append(ppSPARQLUTC);

													CInstallKnowledgeBaseRevisionUpdateCommand *inKBRevUpC = new CInstallKnowledgeBaseRevisionUpdateCommand(kbString, ppSPARQLUTC, pSPARQLQueryC);
													inKBRevUpC->addCommandPrecondition(new CCommandProcessedPrecondition(ppSPARQLUTC));
													commandList.append(inKBRevUpC);
													lastNonQueryC = inKBRevUpC;
												}
											}

											for (CCommand* command : commandList) {
												preSynchronizer->delegateCommand(command);
											}
										}
										delete sparqlSplittingParser;

										if (!foundSparqlOperation) {
											LOG(WARN, getLogDomain(), logTr("No SPARQL command found in request."), this);
										}


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);




									} else if (dynamic_cast<CParseOWL2XMLOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CParseOWL2XMLOntologyCommand *pOWLOntC = (CParseOWL2XMLOntologyCommand *)command;


										COntologyRevision *ontRev = pOWLOntC->getOntologyRevision();
										if (ontRev) {
											CConcreteOntology *ont = ontRev->getOntology();

											QTime parsingTime;
											if (mConfLogProcessingTimes) {
												parsingTime.start();
											}

											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();

											CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ont);
											COWL2QtXMLOntologyParser *owl2Parser = new COWL2QtXMLOntologyParser(builder,&commandRecordRouter);
											QDomElement domEl = pOWLOntC->getOntologyNode();
											builder->initializeBuilding();
											owl2Parser->parseOntologyNode(&domEl);
											builder->completeBuilding();
											delete builder;
											delete owl2Parser;

											if (mConfLogProcessingTimes) {
												cint64 parsingMilliSeconds = parsingTime.elapsed();
												LOG(INFO,getLogDomain(),logTr("Ontology parsed in %1 ms.").arg(parsingMilliSeconds),this);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Knowledge base revision not available."),&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CSAXParseOWL2XMLOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CSAXParseOWL2XMLOntologyCommand *pOWLOntC = (CSAXParseOWL2XMLOntologyCommand *)command;

										QString iriString = pOWLOntC->getSourceString();
										if (iriString.startsWith("file:")) {
											iriString.replace("file:","");
										}
										while (!QFile::exists(iriString) && iriString.startsWith("/")) {
											iriString = iriString.remove(0,1);
										}


										COntologyRevision *ontRev = pOWLOntC->getOntologyRevision();
										if (ontRev) {
											CConcreteOntology *ont = ontRev->getOntology();

											QTime parsingTime;
											if (mConfLogProcessingTimes) {
												parsingTime.start();
											}

											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();

											CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ont);
											COWL2QtXMLOntologySAXParser *owl2Parser = new COWL2QtXMLOntologySAXParser(builder);
											builder->initializeBuilding();
											LOG(INFO,getLogDomain(),logTr("Starting SAX-based ontology parsing for '%1'.").arg(iriString),this);
											owl2Parser->parseOntology(iriString);
											LOG(INFO,getLogDomain(),logTr("Finished SAX-based ontology parsing for '%1'.").arg(iriString),this);

											builder->completeBuilding();
											delete builder;
											delete owl2Parser;

											if (mConfLogProcessingTimes) {
												cint64 parsingMilliSeconds = parsingTime.elapsed();
												LOG(INFO,getLogDomain(),logTr("Ontology parsed in %1 ms.").arg(parsingMilliSeconds),this);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Knowledge base revision not available."),&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CStreamParseOWL2XMLOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CStreamParseOWL2XMLOntologyCommand *pOWLOntC = (CStreamParseOWL2XMLOntologyCommand *)command;

										QString iriString = pOWLOntC->getSourceString();
										if (iriString.startsWith("file:")) {
											iriString.replace("file:","");
										}
										while (!QFile::exists(iriString) && iriString.startsWith("/")) {
											iriString = iriString.remove(0,1);
										}


										COntologyRevision *ontRev = pOWLOntC->getOntologyRevision();
										if (ontRev) {
											CConcreteOntology *ont = ontRev->getOntology();

											QTime parsingTime;
											if (mConfLogProcessingTimes) {
												parsingTime.start();
											}

											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();

											CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ont);
											bool enforceUTF8ConvertingParsing = CConfigDataReader::readConfigBoolean(ontConfig,"Konclude.Parser.UTF8CompatibilityEnforcedXMLStreamParsing");
											COWL2QtXMLOntologyStreamParser *owl2Parser = nullptr;
											if (enforceUTF8ConvertingParsing) {
												owl2Parser = new COWL2QtXMLOntologyStableStreamParser(builder);
											} else {
												owl2Parser = new COWL2QtXMLOntologyStreamParser(builder);
											}
											builder->initializeBuilding();
											LOG(INFO,getLogDomain(),logTr("Starting stream-based OWL2/XML ontology parsing for '%1'.").arg(iriString),this);
											if (owl2Parser->parseOntology(iriString)) {
												LOG(INFO,getLogDomain(),logTr("Finished stream-based OWL2/XML ontology parsing for '%1'.").arg(iriString),this);
												builder->completeBuilding();
											} else {
												if (owl2Parser->hasError()) {
													CUnspecifiedMessageErrorRecord::makeRecord(QString("Stream-based OWL2/XML ontology parsing error: '%1'.").arg(owl2Parser->getErrorString()),&commandRecordRouter);
												}
												CUnspecifiedMessageErrorRecord::makeRecord(QString("Stream-based OWL2/XML ontology parsing for '%1' failed.").arg(iriString),&commandRecordRouter);
											}

											delete builder;
											delete owl2Parser;

											if (mConfLogProcessingTimes) {
												cint64 parsingMilliSeconds = parsingTime.elapsed();
												LOG(INFO,getLogDomain(),logTr("Ontology parsed in %1 ms.").arg(parsingMilliSeconds),this);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Knowledge base revision not available."),&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);









									} else if (dynamic_cast<CStreamParseOWL2FunctionalOntologyCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CStreamParseOWL2FunctionalOntologyCommand *pOWLOntC = (CStreamParseOWL2FunctionalOntologyCommand *)command;

										QString iriString = pOWLOntC->getSourceString();
										if (iriString.startsWith("file:")) {
											iriString.replace("file:","");
										}
										while (!QFile::exists(iriString) && iriString.startsWith("/")) {
											iriString = iriString.remove(0,1);
										}


										COntologyRevision *ontRev = pOWLOntC->getOntologyRevision();
										if (ontRev) {
											CConcreteOntology *ont = ontRev->getOntology();

											QTime parsingTime;
											if (mConfLogProcessingTimes) {
												parsingTime.start();
											}

											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();

											CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ont);
											COWL2FunctionalJAVACCOntologyStreamParser *owl2Parser = new COWL2FunctionalJAVACCOntologyStreamParser(builder);
											builder->initializeBuilding();
											LOG(INFO,getLogDomain(),logTr("Starting stream-based OWL2/Functional ontology parsing for '%1'.").arg(iriString),this);
											if (owl2Parser->parseOntology(iriString)) {
												LOG(INFO,getLogDomain(),logTr("Finished stream-based OWL2/Functional ontology parsing for '%1'.").arg(iriString),this);
												builder->completeBuilding();
											} else {
												if (owl2Parser->hasError()) {
													CUnspecifiedMessageErrorRecord::makeRecord(QString("Stream-based OWL2/Functional ontology parsing error: '%1'.").arg(owl2Parser->getErrorString()),&commandRecordRouter);
												}
												CUnspecifiedMessageErrorRecord::makeRecord(QString("Stream-based OWL2/Functional ontology parsing for '%1' failed.").arg(iriString),&commandRecordRouter);
											}

											delete builder;
											delete owl2Parser;


											if (mConfLogProcessingTimes) {
												cint64 parsingMilliSeconds = parsingTime.elapsed();
												LOG(INFO,getLogDomain(),logTr("Ontology parsed in %1 ms.").arg(parsingMilliSeconds),this);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord(QString("Knowledge base revision not available."),&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);









									} else if (dynamic_cast<CProcessQueryOWL2XMLNodeCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CProcessQueryOWL2XMLNodeCommand *pqOWLC = (CProcessQueryOWL2XMLNodeCommand *)command;

										CBuildQueryCommand *buildQC = pqOWLC->getBuildQueryCommand();

										CKnowledgeBaseRevisionCommandProvider* cKBRevCP = nullptr;

										bool complexQuery = pqOWLC->isComplexQuery();
										if (complexQuery) {
											CCreateKnowledgeBaseRevisionUpdateCommand *crKBRevUpC = new CCreateKnowledgeBaseRevisionUpdateCommand(pqOWLC->getKnowledgeBaseName());
											buildQC->makeToSubCommand(crKBRevUpC);
											cKBRevCP = crKBRevUpC;
										} else {
											CGetCurrentKnowledgeBaseRevisionCommand *crKBRevGet = new CGetCurrentKnowledgeBaseRevisionCommand(pqOWLC->getKnowledgeBaseName());
											buildQC->makeToSubCommand(crKBRevGet);
											cKBRevCP = crKBRevGet;
										}

										CParseOWL2XMLQueryCommand *pOWLQC = new CParseOWL2XMLQueryCommand(pqOWLC->getQueryNode(),cKBRevCP);
										buildQC->makeToSubCommand(pOWLQC);
										pOWLQC->setQueryComplexity(complexQuery);


										CPrepareKnowledgeBaseForQueryCommand* prepKBForQueryC = new CPrepareKnowledgeBaseForQueryCommand(cKBRevCP,pOWLQC);
										buildQC->makeToSubCommand(prepKBForQueryC);
										prepKBForQueryC->addCommandPrecondition(new CCommandProcessedPrecondition(pOWLQC));

										CGenerateQueryCommand* genQC = new CGenerateQueryCommand(pOWLQC);
										buildQC->makeToSubCommand(genQC);
										buildQC->setQueryCommandProvider(genQC);
										genQC->addCommandPrecondition(new CCommandProcessedPrecondition(prepKBForQueryC));

										CCalculateQueryCommand *calcQC = pqOWLC->getCalculateQueryCommand();

										preSynchronizer->delegateCommand(buildQC);
										preSynchronizer->delegateCommand(calcQC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CPrepareKnowledgeBaseForQueryCommand*>(command)) {
										CCommandRecordRouter commandRecordRouter(command, this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CPrepareKnowledgeBaseForQueryCommand *pKBFQC = (CPrepareKnowledgeBaseForQueryCommand *)command;

										CQueryGenerator* queryGenerator = pKBFQC->getQueryGenerator();
										if (queryGenerator->requiresPreprocessedOntology()) {
											CPreprocessKnowledgeBaseRequirementsForQueryCommand* prepKBForQueryC = new CPreprocessKnowledgeBaseRequirementsForQueryCommand(pKBFQC->getOntologyRevisionProvider());
											pKBFQC->makeToSubCommand(prepKBForQueryC);
											preSynchronizer->delegateCommand(prepKBForQueryC);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CProcessClassNameSatisfiableQueryCommand*>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CProcessClassNameSatisfiableQueryCommand *pqOWLC = (CProcessClassNameSatisfiableQueryCommand *)command;


										CBuildQueryCommand *buildQC = pqOWLC->getBuildQueryCommand();
										if (!buildQC) {
											buildQC = new CBuildQueryCommand(0,pqOWLC);
											pqOWLC->setBuildQueryCommand(buildQC);
										}

										CCalculateQueryCommand *calcQC = pqOWLC->getCalculateQueryCommand();
										if (!calcQC) {
											calcQC = new CCalculateQueryCommand(buildQC,pqOWLC);
											pqOWLC->setCalculateQueryCommand(calcQC);
										}


										CGetCurrentKnowledgeBaseRevisionCommand *crKBRevGet = new CGetCurrentKnowledgeBaseRevisionCommand(pqOWLC->getKnowledgeBaseName());
										buildQC->makeToSubCommand(crKBRevGet);

										CPreprocessKnowledgeBaseRequirementsForQueryCommand* prepKBForQueryC = new CPreprocessKnowledgeBaseRequirementsForQueryCommand(crKBRevGet);
										buildQC->makeToSubCommand(prepKBForQueryC);
										prepKBForQueryC->addCommandPrecondition(new CCommandProcessedPrecondition(crKBRevGet));

										CConstructClassSatisfiableQueryCommand *pOWLQC = new CConstructClassSatisfiableQueryCommand(pqOWLC->getClassName(),crKBRevGet);
										buildQC->makeToSubCommand(pOWLQC);
										buildQC->setQueryCommandProvider(pOWLQC);
										pOWLQC->addCommandPrecondition(new CCommandProcessedPrecondition(prepKBForQueryC));

										preSynchronizer->delegateCommand(buildQC);
										preSynchronizer->delegateCommand(calcQC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CBuildQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CLinker<CCommand *> *subComIt = command->getSubCommandLinker();
										while (subComIt) {
											CCommand *supCommand = subComIt->getData();
											preSynchronizer->delegateCommand(supCommand);
											subComIt = subComIt->getNext();
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CInsertResponseCommentCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CParseTestsuiteXMLNodeCommand *>(command)) {
										CParseTestsuiteXMLNodeCommand *parseTestsuiteComm = (CParseTestsuiteXMLNodeCommand *)command;
										CXMLTestsuiteCommandParser *testsuiteParser = new CXMLTestsuiteCommandParser();
										testsuiteParser->realizeCommand(parseTestsuiteComm,preSynchronizer);
										delete testsuiteParser;


									} else if (dynamic_cast<CTestTestsuiteCommand *>(command)) {
										CTestTestsuiteCommand *testTestsuiteComm = (CTestTestsuiteCommand *)command;
										CConfiguration *config = getConfiguration();
										CReasonerTestsuiteTester *testsuiteTester = new CReasonerTestsuiteTester(config);
										testsuiteTester->realizeTestCommand(testTestsuiteComm);
										delete testsuiteTester;


									} else if (dynamic_cast<CEvaluateTestsuiteCommand *>(command)) {
										CEvaluateTestsuiteCommand* evalTestsuiteComm = (CEvaluateTestsuiteCommand*)command;
										CConfiguration *config = getConfiguration();

										CReasonerEvaluationManagerThread* evalManager = new CReasonerEvaluationManagerThread(config);;
										evalManager->realizeEvaluationCommand(evalTestsuiteComm);
										evalManager->waitSynchronization();
										delete evalManager;



									} else if (dynamic_cast<CWriteFunctionalSubClassHierarchyQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CWriteFunctionalSubClassHierarchyQueryCommand *wFSCHC = (CWriteFunctionalSubClassHierarchyQueryCommand *)command;

										CBuildQueryCommand *buildQC = wFSCHC->getBuildQueryCommand();
										if (!buildQC) {
											buildQC = new CBuildQueryCommand(0,wFSCHC);
											wFSCHC->setBuildQueryCommand(buildQC);
										}

										CCalculateQueryCommand *calcQC = wFSCHC->getCalculateQueryCommand();
										if (!calcQC) {
											calcQC = new CCalculateQueryCommand(buildQC,wFSCHC);
											wFSCHC->setCalculateQueryCommand(calcQC);
										}

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(wFSCHC->getKnowledgeBaseName());
										buildQC->makeToSubCommand(getCKBRC);

										CConstructWriteFunctionalSubClassHirarchyQueryCommand *pOWLQC = new CConstructWriteFunctionalSubClassHirarchyQueryCommand(getCKBRC,wFSCHC->getOutputFileString());
										buildQC->makeToSubCommand(pOWLQC);
										buildQC->setQueryCommandProvider(pOWLQC);


										preSynchronizer->delegateCommand(buildQC);
										preSynchronizer->delegateCommand(calcQC);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);



									} else if (dynamic_cast<CWriteCustomQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CWriteCustomQueryCommand *wCQC = (CWriteCustomQueryCommand *)command;

										CBuildQueryCommand *buildQC = wCQC->getBuildQueryCommand();
										if (!buildQC) {
											buildQC = new CBuildQueryCommand(0,wCQC);
											wCQC->setBuildQueryCommand(buildQC);
										}

										CCalculateQueryCommand *calcQC = wCQC->getCalculateQueryCommand();
										if (!calcQC) {
											calcQC = new CCalculateQueryCommand(buildQC,wCQC);
											wCQC->setCalculateQueryCommand(calcQC);
										}

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(wCQC->getKnowledgeBaseName());
										buildQC->makeToSubCommand(getCKBRC);

										CConstructWriteCustomQueryCommand *pOWLQC = new CConstructWriteCustomQueryCommand(getCKBRC,wCQC->getWriteQueryType(),wCQC->getWriteQuerySerializer(),wCQC->getEntityRestrictionString());
										buildQC->makeToSubCommand(pOWLQC);
										buildQC->setQueryCommandProvider(pOWLQC);


										preSynchronizer->delegateCommand(buildQC);
										preSynchronizer->delegateCommand(calcQC);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);






									} else if (dynamic_cast<CClassifyQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CClassifyQueryCommand* cqC = (CClassifyQueryCommand*)command;

										CBuildQueryCommand *buildQC = cqC->getBuildQueryCommand();
										if (!buildQC) {
											buildQC = new CBuildQueryCommand(0,cqC);
											cqC->setBuildQueryCommand(buildQC);
										}

										CCalculateQueryCommand *calcQC = cqC->getCalculateQueryCommand();
										if (!calcQC) {
											calcQC = new CCalculateQueryCommand(buildQC,cqC);
											cqC->setCalculateQueryCommand(calcQC);
										}

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(cqC->getKnowledgeBaseName());
										buildQC->makeToSubCommand(getCKBRC);

										CConstructClassifyQueryCommand *pOWLQC = new CConstructClassifyQueryCommand(getCKBRC);
										buildQC->makeToSubCommand(pOWLQC);
										buildQC->setQueryCommandProvider(pOWLQC);


										preSynchronizer->delegateCommand(buildQC);
										preSynchronizer->delegateCommand(calcQC);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);



									} else if (dynamic_cast<CRealizeQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CRealizeQueryCommand* rqC = (CRealizeQueryCommand*)command;

										CBuildQueryCommand *buildQC = rqC->getBuildQueryCommand();
										if (!buildQC) {
											buildQC = new CBuildQueryCommand(0,rqC);
											rqC->setBuildQueryCommand(buildQC);
										}

										CCalculateQueryCommand *calcQC = rqC->getCalculateQueryCommand();
										if (!calcQC) {
											calcQC = new CCalculateQueryCommand(buildQC,rqC);
											rqC->setCalculateQueryCommand(calcQC);
										}

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(rqC->getKnowledgeBaseName());
										buildQC->makeToSubCommand(getCKBRC);

										CConstructRealizeQueryCommand *pOWLQC = new CConstructRealizeQueryCommand(getCKBRC);
										buildQC->makeToSubCommand(pOWLQC);
										buildQC->setQueryCommandProvider(pOWLQC);


										preSynchronizer->delegateCommand(buildQC);
										preSynchronizer->delegateCommand(calcQC);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CGetKnowledgeBaseEntitiesCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CGetKnowledgeBaseEntitiesCommand* gKBEC = (CGetKnowledgeBaseEntitiesCommand*)command;

										if (!gKBEC->getExtractedEntityContainer()) {
											gKBEC->setExtractedEntityContainer(new CExtractedKnowledgeBaseEntitiesContainer());
										}

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(gKBEC->getKnowledgeBaseName());
										gKBEC->makeToSubCommand(getCKBRC);

										CPrepareKnowledgeBaseCommand* prepKBC = new CPrepareKnowledgeBaseCommand(getCKBRC);
										prepKBC->addCommandPrecondition(new CCommandProcessedPrecondition(getCKBRC));
										gKBEC->makeToSubCommand(prepKBC);

										COntologyProcessingStepVector* ontProStepVec = COntologyProcessingStepVector::getProcessingStepVectorInstance();
										prepKBC->addRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSACTIVECOUNT), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );

										preSynchronizer->delegateCommand(prepKBC);


										CExtractKnowledgeBaseEntitiesCommand *eKBEC = new CExtractKnowledgeBaseEntitiesCommand(gKBEC->getExtractedEntityContainer(),gKBEC->getEntityExtractionSpecification(),getCKBRC);
										eKBEC->addCommandPrecondition(new CCommandProcessedPrecondition(prepKBC));
										gKBEC->makeToSubCommand(eKBEC);

										preSynchronizer->delegateCommand(getCKBRC);
										preSynchronizer->delegateCommand(eKBEC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CGetKnowledgeBaseLanguageCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CGetKnowledgeBaseLanguageCommand* gKBLC = (CGetKnowledgeBaseLanguageCommand*)command;

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(gKBLC->getKnowledgeBaseName());
										gKBLC->makeToSubCommand(getCKBRC);

										CPrepareKnowledgeBaseCommand* prepKBC = new CPrepareKnowledgeBaseCommand(getCKBRC);
										prepKBC->addCommandPrecondition(new CCommandProcessedPrecondition(getCKBRC));
										gKBLC->makeToSubCommand(prepKBC);

										COntologyProcessingStepVector* ontProStepVec = COntologyProcessingStepVector::getProcessingStepVectorInstance();
										prepKBC->addRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPREPROCESS), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );

										preSynchronizer->delegateCommand(prepKBC);


										CExtractKnowledgeBaseLanguageCommand *eKBEC = new CExtractKnowledgeBaseLanguageCommand(gKBLC->getLanguageStringPointer(),getCKBRC);
										eKBEC->addCommandPrecondition(new CCommandProcessedPrecondition(prepKBC));
										gKBLC->makeToSubCommand(eKBEC);

										preSynchronizer->delegateCommand(getCKBRC);
										preSynchronizer->delegateCommand(eKBEC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CGetSubClassHierarchyQueryOWL2XMLNodeCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CGetSubClassHierarchyQueryOWL2XMLNodeCommand *gSCHOWLC = (CGetSubClassHierarchyQueryOWL2XMLNodeCommand *)command;

										CBuildQueryCommand *buildQC = gSCHOWLC->getBuildQueryCommand();

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(gSCHOWLC->getKnowledgeBaseName());
										buildQC->makeToSubCommand(getCKBRC);

										CConstructSubClassHirarchyQueryCommand *pOWLQC = new CConstructSubClassHirarchyQueryCommand(getCKBRC,gSCHOWLC->getQueryNode());
										buildQC->makeToSubCommand(pOWLQC);
										buildQC->setQueryCommandProvider(pOWLQC);

										CCalculateQueryCommand *calcQC = gSCHOWLC->getCalculateQueryCommand();

										preSynchronizer->delegateCommand(buildQC);
										preSynchronizer->delegateCommand(calcQC);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);



									} else if (dynamic_cast<CIsConsistentQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CIsConsistentQueryCommand* iCQC = (CIsConsistentQueryCommand*)command;

										CBuildQueryCommand *buildQC = iCQC->getBuildQueryCommand();
										if (!buildQC) {
											buildQC = new CBuildQueryCommand(0,iCQC);
											iCQC->setBuildQueryCommand(buildQC);
										}

										CCalculateQueryCommand *calcQC = iCQC->getCalculateQueryCommand();
										if (!calcQC) {
											calcQC = new CCalculateQueryCommand(buildQC,iCQC);
											iCQC->setCalculateQueryCommand(calcQC);
										}

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(iCQC->getKnowledgeBaseName());
										buildQC->makeToSubCommand(getCKBRC);

										CConstructIsConsistentQueryCommand *pOWLQC = new CConstructIsConsistentQueryCommand(getCKBRC);
										buildQC->makeToSubCommand(pOWLQC);
										buildQC->setQueryCommandProvider(pOWLQC);


										preSynchronizer->delegateCommand(buildQC);
										preSynchronizer->delegateCommand(calcQC);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);





									} else if (dynamic_cast<CIsTriviallyConsistentQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CIsTriviallyConsistentQueryCommand* iCQC = (CIsTriviallyConsistentQueryCommand*)command;

										CBuildQueryCommand *buildQC = iCQC->getBuildQueryCommand();
										if (!buildQC) {
											buildQC = new CBuildQueryCommand(0,iCQC);
											iCQC->setBuildQueryCommand(buildQC);
										}

										CCalculateQueryCommand *calcQC = iCQC->getCalculateQueryCommand();
										if (!calcQC) {
											calcQC = new CCalculateQueryCommand(buildQC,iCQC);
											iCQC->setCalculateQueryCommand(calcQC);
										}

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(iCQC->getKnowledgeBaseName());
										buildQC->makeToSubCommand(getCKBRC);

										CConstructIsTriviallyConsistentQueryCommand *pOWLQC = new CConstructIsTriviallyConsistentQueryCommand(getCKBRC);
										buildQC->makeToSubCommand(pOWLQC);
										buildQC->setQueryCommandProvider(pOWLQC);


										preSynchronizer->delegateCommand(buildQC);
										preSynchronizer->delegateCommand(calcQC);


										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);





									} else if (dynamic_cast<CKnowledgeBaseProcessCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CKnowledgeBaseProcessCommand* kbPC = (CKnowledgeBaseProcessCommand *)command;

										CGetCurrentKnowledgeBaseRevisionCommand *getCKBRC = new CGetCurrentKnowledgeBaseRevisionCommand(kbPC->getKnowledgeBaseName());
										kbPC->makeToSubCommand(getCKBRC);

										CPrepareKnowledgeBaseCommand* prepKBC = new CPrepareKnowledgeBaseCommand(getCKBRC);
										prepKBC->addCommandPrecondition(new CCommandProcessedPrecondition(getCKBRC));
										kbPC->makeToSubCommand(prepKBC);
										COntologyProcessingStepVector* ontProStepVec = COntologyProcessingStepVector::getProcessingStepVectorInstance();
										prepKBC->addRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSPREPROCESS), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );

										if (dynamic_cast<CKnowledgeBaseClassifyCommand*>(kbPC) != nullptr) {
											prepKBC->addRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSCLASSCLASSIFY), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
										}
										if (dynamic_cast<CKnowledgeBaseRealizeCommand*>(kbPC) != nullptr) {
											prepKBC->addRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSCONCEPTREALIZE), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
											prepKBC->addRequirement( new COntologyProcessingStepRequirement(ontProStepVec->getProcessingStep(COntologyProcessingStep::OPSSAMEINDIVIDUALSREALIZE), COntologyProcessingStatus::PSCOMPLETELYYPROCESSED, 0, COntologyProcessingStatus::PSSUCESSFULL, 0) );
										}

										preSynchronizer->delegateCommand(getCKBRC);
										preSynchronizer->delegateCommand(prepKBC);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CConstructSubClassHirarchyQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CConstructSubClassHirarchyQueryCommand *cSCHQC = (CConstructSubClassHirarchyQueryCommand *)command;
										COntologyRevision *ontRev = cSCHQC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											COWLlinkQueryExtensionParser quExtParser;
											CSubClassHierarchyQuery *query = new CSubClassHierarchyQuery(ontRev->getOntology(),nullptr,ontConfig);
											if (CConfigDataReader::readConfigBoolean(ontConfig,"Konclude.Query.Statistics.CollectStatistics",false)) {
												query->setQueryStatistics(new CQueryStatisticsCollectionStrings());
											}
											cSCHQC->setQuery(query);
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CExtractKnowledgeBaseEntitiesCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CExtractKnowledgeBaseEntitiesCommand* eKBEC = (CExtractKnowledgeBaseEntitiesCommand*)command;
										COntologyRevision *ontRev = eKBEC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											CConcreteOntology* onto = ontRev->getOntology();

											CKnowledgeBaseEntitiesExtractionSpecification* extractSpec = eKBEC->getEntitiesExtractionSpecification();
											CExtractedKnowledgeBaseEntitiesContainer* extractionContainer = eKBEC->getExtractionContainer();

											CActiveEntityCountVector* activeEntityVector = onto->getDataBoxes()->getExpressionDataBoxMapping()->getActiveEntityVector();
											if (activeEntityVector) {
												CActiveEntityIterator activeEntityIterator(activeEntityVector->getActiveEntityIterator());
												while (activeEntityIterator.hasNextActiveEntity()) {
													CExpressionEntity* entity = activeEntityIterator.getNextActiveEntity();

													if (extractSpec->hasPartialEntityExtractionFlags(CKnowledgeBaseEntitiesExtractionSpecification::EXTRACTCLASSENTITIES)) {
														CClassExpression* classEntity = dynamic_cast<CClassExpression*>(entity);
														if (classEntity) {
															extractionContainer->addExtractedEntity(classEntity->getName());
														}
													}

													if (extractSpec->hasPartialEntityExtractionFlags(CKnowledgeBaseEntitiesExtractionSpecification::EXTRACTOBJECTPROPERTYENTITIES)) {
														CObjectPropertyExpression* propertyEntity = dynamic_cast<CObjectPropertyExpression*>(entity);
														if (propertyEntity) {
															extractionContainer->addExtractedEntity(propertyEntity->getName());
														}
													}

													if (extractSpec->hasPartialEntityExtractionFlags(CKnowledgeBaseEntitiesExtractionSpecification::EXTRACTINDIVIDUALSENTITIES)) {
														CNamedIndividualExpression* individualEntity = dynamic_cast<CNamedIndividualExpression*>(entity);
														if (individualEntity) {
															extractionContainer->addExtractedEntity(individualEntity->getName());
														}
													}
												}
											}


										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CExtractKnowledgeBaseLanguageCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CExtractKnowledgeBaseLanguageCommand* eKBLC = (CExtractKnowledgeBaseLanguageCommand*)command;
										COntologyRevision *ontRev = eKBLC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											CConcreteOntology* onto = ontRev->getOntology();

											if (onto) {
												eKBLC->setKBLanguageString(onto->getStructureSummary()->getExpressivenessString());
											}


										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);



									} else if (dynamic_cast<CConstructIsConsistentQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CConstructIsConsistentQueryCommand *cICQC = (CConstructIsConsistentQueryCommand *)command;
										COntologyRevision *ontRev = cICQC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											CIsConsistentQuery *query = new CIsConsistentQuery(ontRev->getOntology(),ontConfig);
											if (CConfigDataReader::readConfigBoolean(ontConfig,"Konclude.Query.Statistics.CollectStatistics",false)) {
												query->setQueryStatistics(new CQueryStatisticsCollectionStrings());
											}
											cICQC->setQuery(query);
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);




									} else if (dynamic_cast<CConstructIsTriviallyConsistentQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CConstructIsTriviallyConsistentQueryCommand *cITCQC = (CConstructIsTriviallyConsistentQueryCommand *)command;
										COntologyRevision *ontRev = cITCQC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											CIsTriviallyConsistentQuery *query = new CIsTriviallyConsistentQuery(ontRev->getOntology(),ontConfig);
											if (CConfigDataReader::readConfigBoolean(ontConfig,"Konclude.Query.Statistics.CollectStatistics",false)) {
												query->setQueryStatistics(new CQueryStatisticsCollectionStrings());
											}
											cITCQC->setQuery(query);
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CConstructClassSatisfiableQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CConstructClassSatisfiableQueryCommand *cCSQC = (CConstructClassSatisfiableQueryCommand *)command;
										COntologyRevision *ontRev = cCSQC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											CIsClassSatisfiableQuery *query = new CIsClassSatisfiableQuery(cCSQC->getClassName());

											CCalculationConfigurationExtension *calcConfig = new CCalculationConfigurationExtension(ontConfig,0);
											CConcept* concept = ontRev->getOntology()->getStringMapping()->getClassNameConceptMappingHash()->value(cCSQC->getClassName(),nullptr);
											if (concept) {
												CSatisfiableCalculationJobGenerator* satCalcJobGenerator = new CSatisfiableCalculationJobGenerator(ontRev->getOntology());
												CSatisfiableCalculationJob* satCalcJob = satCalcJobGenerator->getSatisfiableCalculationJob(concept,false);

												//CQueryStatisticsCollectionStrings* queryStats = nullptr;
												//if (true) {
												//	queryStats = new CQueryStatisticsCollectionStrings();
												//	satCalcJob->setCalclulationStatisticsCollector(queryStats->createCalculationStatisticsCollection());
												//}
												//query->setQueryStatistics(queryStats);
												satCalcJob->setCalculationConfiguration(calcConfig);
												query->setCalculationConfiguration(calcConfig);
												query->addTestSatisfiableCalculationJob(satCalcJob,true);

												cCSQC->setQuery(query);
											} else {
												CUnspecifiedMessageErrorRecord::makeRecord(QString("Concept '%1' cannot be resolved in ontology '%2'.").arg(cCSQC->getClassName()).arg(ontRev->getOntology()->getOntologyName()),&commandRecordRouter);
											}
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);



									} else if (dynamic_cast<CConstructWriteFunctionalSubClassHirarchyQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CConstructWriteFunctionalSubClassHirarchyQueryCommand *cWFSCHQC = (CConstructWriteFunctionalSubClassHirarchyQueryCommand *)command;
										COntologyRevision *ontRev = cWFSCHQC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											CWriteOREFunctionalClassSubsumptionsHierarchyQuery *query = new CWriteOREFunctionalClassSubsumptionsHierarchyQuery(ontRev->getOntology(),ontConfig,cWFSCHQC->getOutputFileName());
											cWFSCHQC->setQuery(query);
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CConstructWriteCustomQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CConstructWriteCustomQueryCommand *cWCQC = (CConstructWriteCustomQueryCommand *)command;
										COntologyRevision *ontRev = cWCQC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											CWriteQuery::WRITEQUERYTYPE queryType = cWCQC->getWriteQueryType();
											CWriteQuerySerializer* serializer = cWCQC->getWriteQuerySerializer();
											const QString& entityRestrictionString = cWCQC->getEntityRestrictionString();
											if (queryType == CWriteQuery::WRITESUBCLASSHIERARCHY) {
												CWriteSerializerClassSubsumptionsHierarchyQuery *query = new CWriteSerializerClassSubsumptionsHierarchyQuery(ontRev->getOntology(),ontConfig,serializer,entityRestrictionString);
												cWCQC->setQuery(query);
											} else if (queryType == CWriteQuery::WRITESUBOBJECTPROPERTYHIERARCHY) {
												CWriteSerializerPropertySubsumptionsHierarchyQuery *query = new CWriteSerializerPropertySubsumptionsHierarchyQuery(ontRev->getOntology(),ontConfig,serializer,false,entityRestrictionString);
												cWCQC->setQuery(query);
											} else if (queryType == CWriteQuery::WRITESUBDATAPROPERTYHIERARCHY) {
												CWriteSerializerPropertySubsumptionsHierarchyQuery *query = new CWriteSerializerPropertySubsumptionsHierarchyQuery(ontRev->getOntology(),ontConfig,serializer,true,entityRestrictionString);
												cWCQC->setQuery(query);
											} else if (queryType == CWriteQuery::WRITEINDIVIDUALFLATTENEDTYPES) {
												CWriteSerializerIndividualFlattenedTypesQuery *query = new CWriteSerializerIndividualFlattenedTypesQuery(ontRev->getOntology(),ontConfig,serializer,entityRestrictionString);
												cWCQC->setQuery(query);
											}

										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);




									} else if (dynamic_cast<CConstructClassifyQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CConstructClassifyQueryCommand *cCQC = (CConstructClassifyQueryCommand*)command;
										COntologyRevision *ontRev = cCQC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											CClassifyQuery *query = new CClassifyQuery(ontRev->getOntology(),ontConfig);
											if (CConfigDataReader::readConfigBoolean(ontConfig,"Konclude.Query.Statistics.CollectStatistics",false)) {
												query->setQueryStatistics(new CQueryStatisticsCollectionStrings());
											}
											cCQC->setQuery(query);
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CConstructRealizeQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CConstructRealizeQueryCommand *cRQC = (CConstructRealizeQueryCommand*)command;
										COntologyRevision *ontRev = cRQC->getOntologyRevision();
										if (ontRev) {
											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();
											CRealizeQuery *query = new CRealizeQuery(ontRev->getOntology(),ontConfig);
											if (CConfigDataReader::readConfigBoolean(ontConfig,"Konclude.Query.Statistics.CollectStatistics",false)) {
												query->setQueryStatistics(new CQueryStatisticsCollectionStrings());
											}
											cRQC->setQuery(query);
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);


									} else if (dynamic_cast<CGenerateQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

										CGenerateQueryCommand* gQC = (CGenerateQueryCommand *)command;
										CQueryBuilderGenerator* queryGenerator = gQC->getQueryGenerator();
										QList<CQuery*> queryList;
										if (queryGenerator) {
											queryList = queryGenerator->generateQuerys();
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("No query generated.",&commandRecordRouter);
										}

										CQuery *query = 0;
										if (queryList.count() > 0) {
											query = queryList.first();
										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Query couldn't be parsed.",&commandRecordRouter);
										}
										gQC->setQuery(query);

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else if (dynamic_cast<CParseOWL2XMLQueryCommand *>(command)) {
										CCommandRecordRouter commandRecordRouter(command,this);
										CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
										CParseOWL2XMLQueryCommand *pOWLQC = (CParseOWL2XMLQueryCommand *)command;

										QDomElement queryNode = pOWLQC->getQueryNode();
										COntologyRevision *ontRev = pOWLQC->getOntologyRevision();

										if (ontRev) {

											bool complex = pOWLQC->isComplexQuery();

											CConcreteOntology* onto = ontRev->getOntology();

											COntologyConfigurationExtension *ontConfig = ontRev->getOntologyConfiguration();

											CQueryBuilderGenerator* queryBuilderGen = nullptr;

											QList<CQuery *> queryList;
											if (complex) {
												CConcreteOntology* baseOnto = ontRev->getPreviousOntologyRevision()->getOntology();

												COWLlinkQueryParser *queryParser = nullptr;
												CConcreteOntologyUpdateSeparateHashingCollectorBuilder *ontoBuilder = new CConcreteOntologyUpdateSeparateHashingCollectorBuilder(onto);
												queryBuilderGen = new CConcreteOntologyQueryExtendedBuilder(baseOnto,onto,ontConfig,ontoBuilder);
												COWL2QtXMLOntologyParser *owl2Parser = new COWL2QtXMLOntologyParser(ontoBuilder,&commandRecordRouter);
												queryParser = new COWLlinkQtXMLComplexQueryParser(queryBuilderGen,owl2Parser);
												ontoBuilder->initializeBuilding();
												queryParser->parseQueryNode(&queryNode);
												ontoBuilder->completeBuilding();

												//COntologyConfigDependedPreProcesser *ontConDepPreProcessor = new COntologyConfigDependedPreProcesser(commandRecordRouter);
												//ontConDepPreProcessor->preprocess(onto,ontConfig);
												//delete ontConDepPreProcessor;


												delete ontoBuilder;
												delete queryParser;
											} else {
												COWLlinkQueryParser *queryParser = nullptr;
												queryBuilderGen = new CConcreteOntologyQuerySimpleBuilder(onto,ontConfig);
												queryParser = new COWLlinkQtXMLSimpleQueryParser(queryBuilderGen,onto);

												queryParser->parseQueryNode(&queryNode);
												delete queryParser;
											}


											pOWLQC->setQueryGenerator(queryBuilderGen);

										} else {
											CUnspecifiedMessageErrorRecord::makeRecord("Knowledge base revision not available.",&commandRecordRouter);
										}

										CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
										CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);

									} else {
										if (reasonerCommander) {
											reasonerCommander->realizeCommand(command);
										} else {
											CCommandRecordRouter commandRecordRouter(command,this);
											CStartProcessCommandRecord::makeRecord(&commandRecordRouter);
											CUnspecifiedMessageErrorRecord::makeRecord("Reasoner Commander missing.",&commandRecordRouter);
											CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
											CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
											LOG(ERROR,getLogDomain(),logTr("Reasoner Commander missing."),this);
										}
									}
								}
							}
							return true;
						} else if (type == EVENTINITIALIZE) {
							initializeOWLlinkContent();
							return true;
						} else if (type == EVENTCOMMANDPROCESSEDCALLBACK) {
							CCommandProcessedCallbackEvent *commProCallbackEvent = (CCommandProcessedCallbackEvent *)event;	
							concludeOWLlinkContent();
							return true;
						}

					} else {
						return true;
					}
					return false;
				}




			}; // end namespace OWLlink 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
