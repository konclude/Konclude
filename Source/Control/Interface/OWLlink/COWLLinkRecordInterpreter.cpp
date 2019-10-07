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

#include "COWLLinkRecordInterpreter.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace OWLlink {

				COWLLinkRecordInterpreter::COWLLinkRecordInterpreter(CCommandDelegater *commandDelegater, CConfiguration* config) 
						: CCommandRecorder(),document("ResponseMessage") {
					delegater = commandDelegater;
					nextSeqNumber = 0;
					recNextSeqNumber = 0;

					owllinkNS = "http://www.owllink.org/owllink#";
					owlNS = "http://www.w3.org/2002/07/owl#";


					QDomProcessingInstruction procInstHeader = document.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"");
					document.appendChild(procInstHeader);
					rootNode = document.createElementNS(owllinkNS,"ResponseMessage");
					//rootNode.setAttributeNS("xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
					//rootNode.setAttributeNS("http://www.w3.org/2001/XMLSchema-instance","xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
					rootNode.setAttribute("xmlns:owl",owlNS);
					rootNode.setAttribute("xml:base",owlNS);
					rootNode.setAttribute("xmlns:xsd","http://www.w3.org/2001/XMLSchema#");
					document.appendChild(rootNode);

					mConfFileAppendSimpleStats = false;
					mStatFileString = "reasoning-statistics.txt";
					mStatStringList.append("calculation-reasoning-time");
					mStatStringList.append("task-process-change-count");
					mStatStringList.append("calculation-memory-consumption");
					mStatStringList.append("individual-node-switch-count");
					mStatStringList.append("unsatisfiable-cache-used-count");
					mStatStringList.append("created-successor-individual-node-count");
					mStatVector.resize(mStatStringList.count());
					mCalculatedWrongResult = false;
					mRespondQueryStatistics = true;
					mConfig = config;

					mConfExtendedErrorReporting = CConfigDataReader::readConfigBoolean(mConfig,"Konclude.OWLlink.ExtendedErrorReporting",false);
				}


				COWLLinkRecordInterpreter::~COWLLinkRecordInterpreter() {
				}


				CCommandDelegater *COWLLinkRecordInterpreter::delegateCommand(CCommand *command) {
					seqSyncMutex.lock();
					commandSequenceHash.insert(command,nextSeqNumber++);
					seqSyncMutex.unlock();
					if (delegater) {
						delegater->delegateCommand(command);
					}
					return this;
				}

				bool COWLLinkRecordInterpreter::hasReasonerCalculatedWrongResult() {
					return mCalculatedWrongResult;
				}

				CCommandRecorder *COWLLinkRecordInterpreter::recordData(CCommandRecordData *recData) {
					CCommandRecorder::recordData(recData);
					if (dynamic_cast<CClosureProcessCommandRecord *>(recData)) {
						if (recData) {
							CCommand *command = recData->getCommand();
							if (command) {

								if (commandSequenceHash.contains(command)) {

									QDomNode domEl;

									if (dynamic_cast<CGetDescriptionCommand *>(command)) {
										CGetDescriptionCommand *desCommand = dynamic_cast<CGetDescriptionCommand *>(command);
										if (desCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												CConfigurationBase *config = desCommand->getConfiguration();
												if (config) {
													domEl = document.createElement("Description");
													domEl.toElement().setAttribute("name",config->getRelatedConfigChange("Konclude.Name")->getString());

													QDomElement protVerNode = document.createElement("ProtocolVersion");
													protVerNode.setAttribute("major",config->getRelatedConfigChange("Konclude.OWLlink.Version.Major")->getString());
													protVerNode.setAttribute("minor",config->getRelatedConfigChange("Konclude.OWLlink.Version.Minor")->getString());
													domEl.appendChild(protVerNode);

													QDomElement reasVerNode = document.createElement("ReasonerVersion");
													reasVerNode.setAttribute("major",config->getRelatedConfigChange("Konclude.Version.Major")->getString());
													reasVerNode.setAttribute("minor",config->getRelatedConfigChange("Konclude.Version.Minor")->getString());
													reasVerNode.setAttribute("build",config->getRelatedConfigChange("Konclude.Version.Build")->getString());
													domEl.appendChild(reasVerNode);


													domEl.appendChild(getOneOfStringConfigNode("selectedProfile","SROIQV(D)","SROIQV(D)","Property"));
													domEl.appendChild(getOneOfStringConfigNode("appliedSemantics","direct","direct","Property"));

													domEl.appendChild(getBooleanConfigNode("abbreviatesIRIs",config->getRelatedConfigChange("Konclude.OWLlink.AbbreviatedIRIs")->getString(),"Setting"));
													domEl.appendChild(getBooleanConfigNode("ignoresAnnotations","true","Property"));
													domEl.appendChild(getBooleanConfigNode("ignoresDeclarations","true","Property"));
													domEl.appendChild(getBooleanConfigNode("uniqueNameAssumption","false","Property"));


													QDomElement supDatatypesNode = document.createElement("Property");
													supDatatypesNode.setAttribute("key","supportedDatatypes");
													QDomElement supDatatypesListNode = document.createElement("List");
													supDatatypesListNode.setAttribute("abbreviatedIRI","xsd:anyURI");
													supDatatypesNode.appendChild(supDatatypesListNode);

													domEl.appendChild(supDatatypesNode);


												}
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CCreateKnowledgeBaseCommand *>(command)) {
										CCreateKnowledgeBaseCommand *createKBCommand = dynamic_cast<CCreateKnowledgeBaseCommand *>(command);
										if (createKBCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												domEl = document.createElement("KB");
												domEl.toElement().setAttribute("kb",createKBCommand->getKnowledgeBaseName());
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CKnowledgeBaseProcessCommand *>(command)) {
										CKnowledgeBaseProcessCommand *kbProcessCommand = dynamic_cast<CKnowledgeBaseProcessCommand *>(command);
										if (kbProcessCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												domEl = document.createElement("OK");
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CLoadKnowledgeBaseKRSSTestConceptCommand *>(command)) {
										CLoadKnowledgeBaseKRSSTestConceptCommand *loadCommand = dynamic_cast<CLoadKnowledgeBaseKRSSTestConceptCommand *>(command);
										if (loadCommand) {
											mStatLoadString = loadCommand->getLoadIRIString();
											if (command->getMaxErrorLevel() <= 0) {
												domEl = document.createElement("OK");
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CSetConfigCommand *>(command)) {
										CSetConfigCommand *setConfigCommand = dynamic_cast<CSetConfigCommand *>(command);
										if (setConfigCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												domEl = document.createElement("OK");
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CGetConfigCommand *>(command)) {
										CGetConfigCommand *getConfigCommand = dynamic_cast<CGetConfigCommand *>(command);
										if (getConfigCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												QString configName = getConfigCommand->getConfigNameString();
												CConfigData *configData = getConfigCommand->getConfigData();
												if (configData->supportsBooleanConvertion()) {
													bool configVal = configData->getBoolean();
													QString configValString("false");
													if (configVal) {
														configValString = "true";
													}
													domEl = getBooleanConfigNode(configName,configValString,"Setting");
												} else if (configData->supportsIntegerConvertion()) {
													qint64 integer = configData->getInteger();
													domEl = getIntegerConfigNode(configName,integer,"Setting");
												} else if (configData->supportsStringConvertion()) {
													domEl = getStringConfigNode(configName,configData->getString(),"Setting");
												} else {
													domEl = document.createElement("Error");
													domEl.toElement().setAttribute("error",QString("Convertion from config value to string not possible."));
												}
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CGetSettingsCommand *>(command)) {
										CGetSettingsCommand *getConfigCommand = dynamic_cast<CGetSettingsCommand*>(command);
										if (getConfigCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												domEl = document.createElement("Settings");
												CConfigurationBase *configuration = getConfigCommand->getConfiguration();
												CConfigurationGroup *configGroup = configuration->getConfigurationGroup();
												qint64 configCount = configGroup->getConfigPropertyCount();
												for (qint64 configIdx = 0; configIdx < configCount; ++configIdx) {
													CConfigDescription *configDes = configGroup->getConfigDescription(configIdx);
													QString configName = configDes->getConfigName();
													QDomElement settEl;
													if (configName.startsWith("Konclude.") || configName.startsWith("Konclude.OWLlink.")) {
														CConfigData *configData = configuration->getRelatedConfigChange(configIdx,true);
														if (configData) {
															if (configData->supportsBooleanConvertion()) {
																bool configVal = configData->getBoolean();
																QString configValString("false");
																if (configVal) {
																	configValString = "true";
																}
																settEl = getBooleanConfigNode(configName,configValString,"Setting");
															} else if (configData->supportsIntegerConvertion()) {
																qint64 integer = configData->getInteger();
																settEl = getIntegerConfigNode(configName,integer,"Setting");
															} else if (configData->supportsStringConvertion()) {
																settEl = getStringConfigNode(configName,configData->getString(),"Setting");
															} 
														}
													}
													if (!settEl.isNull()) {
														domEl.appendChild(settEl);
													}
												}
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CInsertResponseCommentCommand *>(command)) {
										CInsertResponseCommentCommand *commentCommand = dynamic_cast<CInsertResponseCommentCommand *>(command);
										if (commentCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												domEl = document.createComment(commentCommand->getResponseComment());
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CTellKnowledgeBaseCommand *>(command)) {
										CTellKnowledgeBaseCommand *tellKBCommand = dynamic_cast<CTellKnowledgeBaseCommand *>(command);
										if (tellKBCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												domEl = document.createElement("OK");
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CLoadKnowledgeBaseCommand *>(command)) {
										CLoadKnowledgeBaseCommand *loadKBCommand = dynamic_cast<CLoadKnowledgeBaseCommand *>(command);
										if (loadKBCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												domEl = document.createElement("OK");
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CGetKnowledgeBaseLanguageCommand *>(command)) {
										CGetKnowledgeBaseLanguageCommand *getKBLanguageCommand = dynamic_cast<CGetKnowledgeBaseLanguageCommand *>(command);
										if (getKBLanguageCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												QDomElement langEl = document.createElement("StringResponse");
												langEl.setAttribute("result",getKBLanguageCommand->getLanguageString());
												domEl = langEl;
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CGetKnowledgeBaseClassEntitiesCommand *>(command)) {
										CGetKnowledgeBaseClassEntitiesCommand *getClassEntitiesCommand = dynamic_cast<CGetKnowledgeBaseClassEntitiesCommand *>(command);
										if (getClassEntitiesCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												CExtractedKnowledgeBaseEntitiesContainer* entityContainer = getClassEntitiesCommand->getExtractedEntityContainer();
												domEl = getEntitiesSetNode("SetOfClasses","owl:Class",entityContainer);
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CGetKnowledgeBaseObjectPropertyEntitiesCommand *>(command)) {
										CGetKnowledgeBaseObjectPropertyEntitiesCommand *getObjectPropertyEntitiesCommand = dynamic_cast<CGetKnowledgeBaseObjectPropertyEntitiesCommand *>(command);
										if (getObjectPropertyEntitiesCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												CExtractedKnowledgeBaseEntitiesContainer* entityContainer = getObjectPropertyEntitiesCommand->getExtractedEntityContainer();
												domEl = getEntitiesSetNode("SetOfObjectProperties","owl:ObjectProperty",entityContainer);
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CGetKnowledgeBaseNamedIndividualEntitiesCommand *>(command)) {
										CGetKnowledgeBaseNamedIndividualEntitiesCommand *getIndividualEntitiesCommand = dynamic_cast<CGetKnowledgeBaseNamedIndividualEntitiesCommand *>(command);
										if (getIndividualEntitiesCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												CExtractedKnowledgeBaseEntitiesContainer* entityContainer = getIndividualEntitiesCommand->getExtractedEntityContainer();
												domEl = getEntitiesSetNode("SetOfIndividuals","owl:NamedIndividual",entityContainer);
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CKnowledgeBaseQueryCommand *>(command)) {
										CKnowledgeBaseQueryCommand *queryCommand = dynamic_cast<CKnowledgeBaseQueryCommand *>(command);
										if (queryCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												bool wrongResult = false;
												CCalculateQueryCommand *calcQueryCommand = queryCommand->getCalculateQueryCommand();
												CTestExtendedCalculateQueryCommand *testExtCalcQueryCommand = dynamic_cast<CTestExtendedCalculateQueryCommand *>(calcQueryCommand);
												CQuery *query = calcQueryCommand->getQuery();
												CQueryResult *queryResult = 0;
												CQueryResult *expectedQueryResult = 0;
												QDomElement queryErrorNode;
												if (query) {
													QDomElement queryResultNode;
													queryResult = query->getQueryResult();
													if (queryResult) {
														CBooleanQueryResult *boolQueryResult = dynamic_cast<CBooleanQueryResult *>(queryResult);
														if (boolQueryResult) {
															bool result = false;
															result = boolQueryResult->getResult();
															queryResultNode = document.createElement("BooleanResponse");
															if (result) {
																queryResultNode.setAttribute("result","true");
															} else {
																queryResultNode.setAttribute("result","false");
															}
															domEl = queryResultNode;
														}

														CClassHierarchyResult *subClassHierarchyResult = dynamic_cast<CClassHierarchyResult *>(queryResult);
														if (subClassHierarchyResult) {
															queryResultNode = document.createElement("ClassHierarchy");
															CClassSynsetResult *bottomClassSynsetResult = subClassHierarchyResult->getBottomClassSynset();
															QDomElement classSynsetNode = document.createElement("ClassSynset");
															foreach (QString className, bottomClassSynsetResult->getEquivalentClassNameList()) {
																QDomElement classNameNode = document.createElement("owl:Class");
																classNameNode.setAttribute("IRI",className);
																classSynsetNode.appendChild(classNameNode);
															}
															queryResultNode.appendChild(classSynsetNode);
															QList<CClassSynsetResult *> procList;
															QSet<CClassSynsetResult *> procSet;
															CClassSynsetResult *topClassSynsetResult = subClassHierarchyResult->getTopClassSynset();
															procList.append(topClassSynsetResult);
															while (!procList.isEmpty()) {
																CClassSynsetResult *classSynsetResult = procList.takeFirst();
																CClassSubClassesRelationResult *subClassRelationResult = subClassHierarchyResult->getSubClassRelation(classSynsetResult,false);
																if (subClassRelationResult) {
																	QDomElement classSubClassPairNode = document.createElement("ClassSubClassesPair");
																	// super class serialization
																	QDomElement superClassSynsetNode = document.createElement("ClassSynset");
																	foreach (QString className, classSynsetResult->getEquivalentClassNameList()) {
																		QDomElement classNameNode = document.createElement("owl:Class");
																		classNameNode.setAttribute("IRI",className);
																		superClassSynsetNode.appendChild(classNameNode);
																	}
																	classSubClassPairNode.appendChild(superClassSynsetNode);
																	QDomElement subClassSynsetsNode = document.createElement("SubClassSynsets");
																	// sub classes serialization
																	foreach (CClassSynsetResult *subClassSynsetResult, subClassRelationResult->getSubClassesSynsetList()) {
																		QDomElement subClassSynsetNode = document.createElement("ClassSynset");
																		foreach (QString className, subClassSynsetResult->getEquivalentClassNameList()) {
																			QDomElement classNameNode = document.createElement("owl:Class");
																			classNameNode.setAttribute("IRI",className);
																			subClassSynsetNode.appendChild(classNameNode);
																		}
																		subClassSynsetsNode.appendChild(subClassSynsetNode);
																		if (!procSet.contains(subClassSynsetResult)) {
																			procSet.insert(subClassSynsetResult);
																			procList.append(subClassSynsetResult);
																		}
																	}
																	classSubClassPairNode.appendChild(subClassSynsetsNode);
																	queryResultNode.appendChild(classSubClassPairNode);
																}
															}
															domEl = queryResultNode;
														}

														CClassesResult* classesResult = dynamic_cast<CClassesResult*>(queryResult);
														if (classesResult) {
															queryResultNode = document.createElement("Classes");
															QSet<QString>* classesSet = classesResult->getClassesSet();
															for (QSet<QString>::const_iterator it = classesSet->constBegin(), itEnd = classesSet->constEnd(); it != itEnd; ++it) {
																const QString& className(*it);
																QDomElement classNameNode = document.createElement("owl:Class");
																classNameNode.setAttribute("IRI",className);
																queryResultNode.appendChild(classNameNode);
															}
															domEl = queryResultNode;
														}


														CClassSetResult* classSetResult = dynamic_cast<CClassSetResult*>(queryResult);
														if (classSetResult) {
															queryResultNode = document.createElement("SetOfClasses");
															QSet<QString>* classesSet = classSetResult->getClassesSet();
															for (QSet<QString>::const_iterator it = classesSet->constBegin(), itEnd = classesSet->constEnd(); it != itEnd; ++it) {
																const QString& className(*it);
																QDomElement classNameNode = document.createElement("owl:Class");
																classNameNode.setAttribute("IRI",className);
																queryResultNode.appendChild(classNameNode);
															}
															domEl = queryResultNode;
														}

														CClassSynsetsResult* classSynsetsResult = dynamic_cast<CClassSynsetsResult*>(queryResult);
														if (classSynsetsResult) {
															queryResultNode = document.createElement("SetOfClassSynsets");
															QList<CClassSynsetResult*>* classSynsetList = classSynsetsResult->getClassSynsetList();
															for (QList<CClassSynsetResult*>::const_iterator it = classSynsetList->constBegin(), itEnd = classSynsetList->constEnd(); it != itEnd; ++it) {
																CClassSynsetResult* classSynset(*it);
																QDomElement superClassSynsetNode = document.createElement("ClassSynset");
																foreach (QString className, classSynset->getEquivalentClassNameList()) {
																	QDomElement classNameNode = document.createElement("owl:Class");
																	classNameNode.setAttribute("IRI",className);
																	superClassSynsetNode.appendChild(classNameNode);
																}
																queryResultNode.appendChild(superClassSynsetNode);
															}
															domEl = queryResultNode;
														}



								


														CPropertyHierarchyResult *subPropertyHierarchyResult = dynamic_cast<CPropertyHierarchyResult *>(queryResult);
														if (subPropertyHierarchyResult) {

															bool dataPropertyQuery = subPropertyHierarchyResult->hasDataProperties();
															QString dataObjectString = QString("Object");
															if (dataPropertyQuery) {
																dataObjectString = QString("Data");
															}

															queryResultNode = document.createElement(QString("%1PropertyHierarchy").arg(dataObjectString));
															CPropertySynsetResult *bottomPropertySynsetResult = subPropertyHierarchyResult->getBottomPropertySynset();
															QDomElement propertySynsetNode = document.createElement(QString("%1PropertySynset").arg(dataObjectString));
															foreach (QString propertyName, bottomPropertySynsetResult->getEquivalentPropertyNameList()) {
																QDomElement propertyNameNode = document.createElement(QString("owl:%1Property").arg(dataObjectString));
																propertyNameNode.setAttribute("IRI",propertyName);
																propertySynsetNode.appendChild(propertyNameNode);
															}
															queryResultNode.appendChild(propertySynsetNode);
															QList<CPropertySynsetResult *> procList;
															QSet<CPropertySynsetResult *> procSet;
															CPropertySynsetResult *topPropertySynsetResult = subPropertyHierarchyResult->getTopPropertySynset();
															procList.append(topPropertySynsetResult);
															while (!procList.isEmpty()) {
																CPropertySynsetResult *propertySynsetResult = procList.takeFirst();
																CPropertySubPropertiesRelationResult *subPropertyRelationResult = subPropertyHierarchyResult->getSubPropertyRelation(propertySynsetResult,false);
																if (subPropertyRelationResult) {
																	QDomElement propertySubPropertyPairNode = document.createElement(QString("%1PropertySub%2PropertiesPair").arg(dataObjectString).arg(dataObjectString));
																	// super property serialization
																	QDomElement superPropertySynsetNode = document.createElement(QString("%1PropertySynset").arg(dataObjectString));
																	foreach (QString propertyName, propertySynsetResult->getEquivalentPropertyNameList()) {
																		QDomElement propertyNameNode = document.createElement(QString("owl:%1Property").arg(dataObjectString));
																		propertyNameNode.setAttribute("IRI",propertyName);
																		superPropertySynsetNode.appendChild(propertyNameNode);
																	}
																	propertySubPropertyPairNode.appendChild(superPropertySynsetNode);
																	QDomElement subPropertySynsetsNode = document.createElement(QString("Sub%1PropertySynsets").arg(dataObjectString));
																	// sub properties serialization
																	foreach (CPropertySynsetResult *subPropertySynsetResult, subPropertyRelationResult->getSubPropertiesSynsetList()) {
																		QDomElement subPropertySynsetNode = document.createElement(QString("%1PropertySynset").arg(dataObjectString));
																		foreach (QString propertyName, subPropertySynsetResult->getEquivalentPropertyNameList()) {
																			QDomElement propertyNameNode = document.createElement(QString("owl:%1Property").arg(dataObjectString));
																			propertyNameNode.setAttribute("IRI",propertyName);
																			subPropertySynsetNode.appendChild(propertyNameNode);
																		}
																		subPropertySynsetsNode.appendChild(subPropertySynsetNode);
																		if (!procSet.contains(subPropertySynsetResult)) {
																			procSet.insert(subPropertySynsetResult);
																			procList.append(subPropertySynsetResult);
																		}
																	}
																	propertySubPropertyPairNode.appendChild(subPropertySynsetsNode);
																	queryResultNode.appendChild(propertySubPropertyPairNode);
																}
															}
															domEl = queryResultNode;
														}

														//CPropertiesResult* propertiesResult = dynamic_cast<CPropertiesResult*>(queryResult);
														//if (propertiesResult) {
														//	queryResultNode = document.createElement("Properties");
														//	QSet<QString>* propertiesSet = propertiesResult->getPropertiesSet();
														//	for (QSet<QString>::const_iterator it = propertiesSet->constBegin(), itEnd = propertiesSet->constEnd(); it != itEnd; ++it) {
														//		const QString& propertyName(*it);
														//		QDomElement propertyNameNode = document.createElement("owl:Property");
														//		propertyNameNode.setAttribute("IRI",propertyName);
														//		queryResultNode.appendChild(propertyNameNode);
														//	}
														//	domEl = queryResultNode;
														//}


														



														CPropertySetResult* propertySetResult = dynamic_cast<CPropertySetResult*>(queryResult);
														if (propertySetResult) {

															QString dataObjectString = QString("Object");
															bool dataPropertyResult = propertySetResult->hasDataProperties();
															if (dataPropertyResult) {
																dataObjectString = QString("Data");
															}

															if (dataPropertyResult) {
																queryResultNode = document.createElement(QString("DataPropertySynonyms"));
															} else {
																queryResultNode = document.createElement(QString("SetOfObjectProperties"));
															}
															QSet<QString>* propertiesSet = propertySetResult->getPropertiesSet();
															for (QSet<QString>::const_iterator it = propertiesSet->constBegin(), itEnd = propertiesSet->constEnd(); it != itEnd; ++it) {
																const QString& propertyName(*it);
																QDomElement propertyNameNode = document.createElement(QString("owl:%1Property").arg(dataObjectString));
																propertyNameNode.setAttribute("IRI", propertyName);
																queryResultNode.appendChild(propertyNameNode);
															}
															domEl = queryResultNode;
														}

														CPropertySynsetsResult* propertySynsetsResult = dynamic_cast<CPropertySynsetsResult*>(queryResult);
														if (propertySynsetsResult) {
															QString dataObjectString = QString("Object");
															bool dataPropertyResult = propertySynsetsResult->hasDataProperties();
															if (dataPropertyResult) {
																dataObjectString = QString("Data");
															}

															queryResultNode = document.createElement(QString("SetOf%1PropertySynsets").arg(dataObjectString));
															QList<CPropertySynsetResult*>* propertySynsetList = propertySynsetsResult->getPropertySynsetList();
															for (QList<CPropertySynsetResult*>::const_iterator it = propertySynsetList->constBegin(), itEnd = propertySynsetList->constEnd(); it != itEnd; ++it) {
																CPropertySynsetResult* propertySynset(*it);
																QDomElement superPropertySynsetNode = document.createElement(QString("%1PropertySynset").arg(dataObjectString));
																foreach(QString propertyName, propertySynset->getEquivalentPropertyNameList()) {
																	QDomElement propertyNameNode = document.createElement(QString("owl:%1Property").arg(dataObjectString));
																	propertyNameNode.setAttribute("IRI", propertyName);
																	superPropertySynsetNode.appendChild(propertyNameNode);
																}
																queryResultNode.appendChild(superPropertySynsetNode);
															}
															domEl = queryResultNode;
														}











														CIndividualSynonymsResult* individualSynonymsResult = dynamic_cast<CIndividualSynonymsResult*>(queryResult);
														if (individualSynonymsResult) {
															queryResultNode = document.createElement("IndividualSynonyms");
															QSet<QString>* individualsSet = individualSynonymsResult->getIndividualsSet();
															for (QSet<QString>::const_iterator it = individualsSet->constBegin(), itEnd = individualsSet->constEnd(); it != itEnd; ++it) {
																const QString& indiName(*it);
																QDomElement indiNameNode = document.createElement("owl:NamedIndividual");
																indiNameNode.setAttribute("IRI",indiName);
																queryResultNode.appendChild(indiNameNode);
															}
															domEl = queryResultNode;
														} else {
															CIndividualsResult* individualResult = dynamic_cast<CIndividualsResult*>(queryResult);
															if (individualResult) {
																queryResultNode = document.createElement("SetOfIndividuals");
																QSet<QString>* individualsSet = individualResult->getIndividualsSet();
																for (QSet<QString>::const_iterator it = individualsSet->constBegin(), itEnd = individualsSet->constEnd(); it != itEnd; ++it) {
																	const QString& indiName(*it);
																	QDomElement indiNameNode = document.createElement("owl:NamedIndividual");
																	indiNameNode.setAttribute("IRI",indiName);
																	queryResultNode.appendChild(indiNameNode);
																}
																domEl = queryResultNode;
															}
														}


														CIndividualClassAssertionsResult* classAssertionResult = dynamic_cast<CIndividualClassAssertionsResult*>(queryResult);
														if (classAssertionResult) {
															queryResultNode = document.createElement("SetOfClassAssertions");
															QSet< QPair<QString,QString> >* individualClassAssSet = classAssertionResult->getIndividualConceptAssertionSet();
															for (QSet< QPair<QString,QString> >::const_iterator it = individualClassAssSet->constBegin(), itEnd = individualClassAssSet->constEnd(); it != itEnd; ++it) {
																const QPair<QString,QString>& indiClassNamePair(*it);
																QDomElement classAssNode = document.createElement("owl:ClassAssertion");
																QDomElement indiNameNode = document.createElement("owl:NamedIndividual");
																QDomElement classNode = document.createElement("owl:Class");
																indiNameNode.setAttribute("IRI",indiClassNamePair.first);
																classNode.setAttribute("IRI",indiClassNamePair.second);
																classAssNode.appendChild(classNode);
																classAssNode.appendChild(indiNameNode);
																queryResultNode.appendChild(classAssNode);
															}
															domEl = queryResultNode;
														}


														CIndividualSynsetsResult* individualSynsetsResult = dynamic_cast<CIndividualSynsetsResult*>(queryResult);
														if (individualSynsetsResult) {
															queryResultNode = document.createElement("SetOfIndividualSynsets");
															QList<CIndividualSynsetResult*>* individualSynsetList = individualSynsetsResult->getIndividualSynsetList();
															for (QList<CIndividualSynsetResult*>::const_iterator it = individualSynsetList->constBegin(), itEnd = individualSynsetList->constEnd(); it != itEnd; ++it) {
																CIndividualSynsetResult* individualSynset(*it);
																QDomElement superIndividualSynsetNode = document.createElement("IndividualSynset");
																foreach (QString individualName, individualSynset->getEquivalentIndividualNameList()) {
																	QDomElement individualNameNode = document.createElement("owl:NamedIndividual");
																	individualNameNode.setAttribute("IRI",individualName);
																	superIndividualSynsetNode.appendChild(individualNameNode);
																}
																queryResultNode.appendChild(superIndividualSynsetNode);
															}
															domEl = queryResultNode;
														}

														CSucceedQueryResult* succeedResult = dynamic_cast<CSucceedQueryResult*>(queryResult);
														if (succeedResult) {
															queryResultNode = document.createElement("OK");
															domEl = queryResultNode;
														}

														if (testExtCalcQueryCommand) {
															expectedQueryResult = testExtCalcQueryCommand->getExpectedQueryResult();
															if (expectedQueryResult) {
																if (!expectedQueryResult->isResultEquivalentTo(queryResult)) {
																	wrongResult = true;
																}
															}

															if (testExtCalcQueryCommand->getTimingReportLevel() > 0) {
																queryResultNode.setAttribute("response-time",QString::number(testExtCalcQueryCommand->getCreationElapsedMilliSeconds()));
															}

														}
													}
													if (query->hasError()) {
														CQueryInconsitentOntologyError* inOnEr = CQueryInconsitentOntologyError::getInconsistentOntologyError(query->getQueryErrorLinker());
														if (inOnEr) {
															QDomElement erDomEl = getErrorNode(inOnEr->getErrorString(),"UnsatisfiableKBError");
															queryErrorNode = erDomEl;
														} else {
															QString errorString = query->getErrorStringList().join(" ");
															if (errorString.isEmpty()) {
																errorString = QString("Unknown error.");
															}
															QDomElement erDomEl = getErrorNode(errorString);
															queryErrorNode = erDomEl;
														}
													}


#ifndef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
													CQueryStatistics* queryStats = query->getQueryStatistics();
													if (queryStats && mRespondQueryStatistics) {
														QList<QString> statStrings(queryStats->getStatisticsNameStringList());
														if (statStrings.count() >= 1) {
															QVector<QDomElement> statNodeVec(statStrings.count()+1);
															QList<QDomElement> statMoreNodeList;
															QList<QDomElement> statClassNodeList;
															QDomElement statNode = document.createElement("ResponseStatistics");
															foreach (QString statName, statStrings) {
																if (mConfFileAppendSimpleStats) {
																	for (cint64 i = 0; i < mStatStringList.count(); ++i) {
																		if (statName == mStatStringList[i]) {
																			cint64 statValue = queryStats->getStatisticIntegerValue(statName);
																			mStatVector[i] = statValue;
																			break;
																		}
																	}
																}
																cint64 index = CProcessingStatistics::getIndexFromProcessingStatisticDescriptionName(statName);
																if (index >= 0) {
																	QDomElement statValueNode = document.createElement("CountedProcessStatistic");
																	statValueNode.setAttribute("statistic-name",statName);
																	statValueNode.setAttribute("statistic-value",queryStats->getStatisticIntegerValue(statName));
																	statNodeVec[index] = statValueNode;
																} else {
																	if (statName.startsWith("class-classification")) {
																		QDomElement statValueNode = document.createElement("ClassClassificationStatistic");
																		statValueNode.setAttribute("statistic-name",statName);
																		statValueNode.setAttribute("statistic-value",queryStats->getStatisticIntegerValue(statName));
																		statClassNodeList.append(statValueNode);
																	} else {
																		QDomElement statValueNode = document.createElement("CalculationStatistic");
																		statValueNode.setAttribute("statistic-name",statName);
																		statValueNode.setAttribute("statistic-value",queryStats->getStatisticIntegerValue(statName));
																		statMoreNodeList.append(statValueNode);
																	}
																}
															}
															foreach (QDomElement statValueNode, statMoreNodeList) {
																statNode.appendChild(statValueNode);
															}
															foreach (QDomElement statValueNode, statClassNodeList) {
																statNode.appendChild(statValueNode);
															}
															for (cint64 index = 0; index < statNodeVec.count(); ++index) {
																QDomElement statValueNode = statNodeVec[index];
																if (!statValueNode.isNull()) {
																	statNode.appendChild(statValueNode);
																}
															}
															if (!queryResultNode.isNull()) {
																queryResultNode.appendChild(statNode);
															} else if (!queryErrorNode.isNull()) {
																queryErrorNode.appendChild(statNode);
															}
														}
													}
#endif
												} else {
													domEl = getErrorNode(recData);
												}

												if (wrongResult) {
													mCalculatedWrongResult = true;
													LOG(WARNING,"::Konclude::Control::Interface::OWLlink::OWLlinkRecordInterpreter",logTr("Reasoner calculated unexpected result on query %1.").arg(query->getQueryString()),this);
													QDomElement errorMistakeNode = getErrorNode(QString("Reasoner calculated unexpected result"));
													QDomNode prevDomEl = domEl;
													errorMistakeNode.appendChild(prevDomEl);
													QDomElement expQueryResultNode = document.createElement("ExpectedResponse");
													CBooleanQueryResult *boolExpecQueRes = dynamic_cast<CBooleanQueryResult *>(expectedQueryResult);
													if (boolExpecQueRes) {
														QDomElement expBoolQueryResultNode = document.createElement("BooleanResponse");
														bool expecQueRes = boolExpecQueRes->getResult();
														if (expecQueRes) {
															expBoolQueryResultNode.setAttribute("result","true");
														} else {
															expBoolQueryResultNode.setAttribute("result","false");
														}
														expQueryResultNode.appendChild(expBoolQueryResultNode);
													}
													errorMistakeNode.appendChild(expQueryResultNode);
													domEl = errorMistakeNode;
												} else if (mConfFileAppendSimpleStats) {
													CIsClassSatisfiableQuery* satQuery = dynamic_cast<CIsClassSatisfiableQuery*>(query);
													if (satQuery) {
														mStatQueryString = satQuery->getClassStringName();
													}
													//QFile mStatFile(mStatFileString);
													//if (!mStatFile.exists()) {
													//	if (mStatFile.open(QIODevice::WriteOnly)) {
													//		mStatFile.write((QString("test-name ")+mStatStringList.join(" ").append("\r\n")).toLocal8Bit());
													//		mStatFile.close();
													//	}
													//}
													//if (mStatFile.open(QIODevice::Append)) {
													//	mStatKBString = queryCommand->getKnowledgeBaseName();
													//	QString appendString = QString("%1#%2#%3").arg(mStatKBString).arg(mStatLoadString).arg(mStatQueryString);
													//	mStatKBString = "";
													//	mStatLoadString = "";
													//	mStatQueryString = "";
													//	for (cint64 i = 0; i < mStatVector.count(); ++i) {
													//		cint64 statValue = mStatVector[i];
													//		if (!appendString.isEmpty()) {
													//			appendString.append(" ");
													//		}
													//		appendString.append(QString("%1").arg(statValue));
													//	}
													//	appendString.append(QString("\r\n"));
													//	mStatFile.write(appendString.toLocal8Bit());
													//	mStatFile.close();
													//}
												}

												if (!queryErrorNode.isNull()) {
													if (!domEl.isNull()) {
														queryErrorNode.appendChild(domEl);															
													}
													domEl = queryErrorNode;
												}

												
											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CSkipUnsupportedCommandCommand *>(command)) {
										CSkipUnsupportedCommandCommand *skipUnsCommand = dynamic_cast<CSkipUnsupportedCommandCommand *>(command);
										if (skipUnsCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												QDomElement errorNode = getErrorNode(QString("Instruction '%1' is currently not supported.").arg(skipUnsCommand->getSkippedCommandString()),"Error");
												domEl = errorNode;

											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CReleaseKnowledgeBaseCommand *>(command)) {
										CReleaseKnowledgeBaseCommand *relCommand = dynamic_cast<CReleaseKnowledgeBaseCommand *>(command);
										if (relCommand) {
											if (command->getMaxErrorLevel() <= 0) {
												QDomElement releasedNode = document.createElement("OK");
												domEl = releasedNode;

											} else {
												domEl = getErrorNode(recData);
											}
										}
									} else if (dynamic_cast<CParseTestsuiteXMLNodeCommand *>(command)) {
										CParseTestsuiteXMLNodeCommand *parseTestCommand = dynamic_cast<CParseTestsuiteXMLNodeCommand *>(command);
										CTestTestsuiteCommand *testsuiteCommand = 0;
										CLinker<CCommand *> *subCommIt = parseTestCommand->getSubCommandLinker();
										while (subCommIt && !testsuiteCommand) {
											testsuiteCommand = dynamic_cast<CTestTestsuiteCommand *>(subCommIt->getData());
											subCommIt = subCommIt->getNext();
										}
										if (testsuiteCommand && testsuiteCommand->getMaxErrorLevel() <= 0) {
											CReasonerTestsuiteTest *testsuiteTest = testsuiteCommand->getTestsuiteTest();
											QLinkedList<CReasonerTestfileTest *> testfileTestList = testsuiteTest->getTestfileTestList();
											QDomElement resultNode = document.createElement("TestsuiteResult");

											resultNode.setAttribute("totalTestingTime",testsuiteTest->getTestsuiteResult()->getTotalTestingTime());

											foreach (CReasonerTestfileTest *testfileTest, testfileTestList) {
												CReasonerTestfileTestResult *testfileTestResult = testfileTest->getTestResults();
												QDomElement testfileResultNode = document.createElement("TestfileResult");
												testfileResultNode.setAttribute("testSource",testfileTest->getTestRequestFile());
												testfileResultNode.setAttribute("testLoopCount",testfileTestResult->getTotalTestCount());
												if (testfileTestResult->getFailedCount() > 0) {
													testfileResultNode.setAttribute("failedCount",testfileTestResult->getFailedCount());
												}
												if (testfileTestResult->getCrashCount() > 0) {
													testfileResultNode.setAttribute("crashCount",testfileTestResult->getCrashCount());
												}
												if (testfileTestResult->getErrorCount() > 0) {
													testfileResultNode.setAttribute("errorCount",testfileTestResult->getErrorCount());
												}
												if (testfileTestResult->getWrongResponseCount() > 0) {
													testfileResultNode.setAttribute("wrongCount",testfileTestResult->getWrongResponseCount());
												}
												if (testfileTestResult->getTimeoutCount() > 0) {
													testfileResultNode.setAttribute("timeoutCount",testfileTestResult->getTimeoutCount());
												}
												testfileResultNode.setAttribute("correctCount",testfileTestResult->getCorrectResponseCount());
												if (testfileTestResult->getAverageCorrectResponseTime() > 0) {
													testfileResultNode.setAttribute("averageCorrectResponseTime",testfileTestResult->getAverageCorrectResponseTime());													
												}
												resultNode.appendChild(testfileResultNode);
											}
											domEl = resultNode;

										} else {
											domEl = getErrorNode(recData);
										}
									} 

									if (domEl.isNull()) {
										domEl = getErrorNode(QString("No handler for '%1' command ").arg(command->getBriefCommandDescription()));
									}


									if (!domEl.isNull()) {
										seqSyncMutex.lock();
										qint64 seqNumber = commandSequenceHash.value(command);
										seqNodeHash.insert(seqNumber,domEl);
										seqSyncMutex.unlock();

									}

									while (seqNodeHash.contains(recNextSeqNumber)) {
										seqSyncMutex.lock();
										if (seqNodeHash.contains(recNextSeqNumber)) {
											rootNode.appendChild(seqNodeHash.value(recNextSeqNumber));
											recNextSeqNumber++;
										}
										seqSyncMutex.unlock();
									}

								}

							}
						}
					}
					return this;
				}



				QDomElement COWLLinkRecordInterpreter::getEntitiesSetNode(const QString &nodeString, const QString &entityString, CExtractedKnowledgeBaseEntitiesContainer* entityContainer) {
					QDomElement entitySetNode = document.createElement(nodeString);
					QStringList* entitySet = entityContainer->getExtractedEntityList();
					for (QStringList::const_iterator it = entitySet->constBegin(), itEnd = entitySet->constEnd(); it != itEnd; ++it) {
						const QString& entityName(*it);
						QDomElement entityNameNode = document.createElement(entityString);
						entityNameNode.setAttribute("IRI",entityName);
						entitySetNode.appendChild(entityNameNode);
					}
					return entitySetNode;
				}



				QDomElement COWLLinkRecordInterpreter::getErrorNode(CCommandRecordData *recData, const QString &nodeString) {
					QDomElement errorNode = document.createElement(nodeString);

					QDomElement errorTextNodePa = document.createElement(QString("%1Text").arg(nodeString));

					QString errorText = getErrorString(recData,nodeString);

					QDomText errorTextNode = document.createTextNode(errorText);
					errorNode.setAttribute("error",errorText);

					errorTextNodePa.appendChild(errorTextNode);
					errorNode.appendChild(errorTextNodePa);

					return errorNode;
				}


				QDomElement COWLLinkRecordInterpreter::getErrorNode(const QString& errorText, const QString &nodeString) {
					QDomElement errorNode = document.createElement(nodeString);

					QDomElement errorTextNodePa = document.createElement(QString("%1Text").arg(nodeString));

					QDomText errorTextNode = document.createTextNode(errorText);
					errorNode.setAttribute("error",errorText); 

					errorTextNodePa.appendChild(errorTextNode);
					errorNode.appendChild(errorTextNodePa);

					return errorNode;
				}


				QString COWLLinkRecordInterpreter::getErrorLevelString(double level) {
					QString logLevelString;
					if (level >= 90) {
						logLevelString = "catastrophic error";
					} else if (level >= 80) {
						logLevelString = "exception error";
					} else if (level >= 70) {
						logLevelString = "error";
					} else if (level >= 60) {
						logLevelString = "warning";
					} else if (level >= 30) {
						logLevelString = "info";
					} else {
						logLevelString = "notice";
					}
					return logLevelString;
				}


				QString COWLLinkRecordInterpreter::getErrorString(CCommandRecordData *recData, const QString &nodeString) {
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
									QString subErrorString = QString("{%1}, %2").arg(getErrorLevelString(subCommand->getMaxErrorLevel())).arg(getErrorString(subCommandRecData,subNodeString));
									errorText += subErrorString;
								}
							}
							subCommIt = subCommIt->getNext();
						}

					} else {
						errorText = QString("An error has occurred while processing, process Interface not available");
					}
					return errorText;
				}



				QDomElement COWLLinkRecordInterpreter::getOneOfStringConfigNode(const QString &keyString, const QString &oneOfOptions, const QString &defaultOption, const QString &nodeString) {
					QDomElement configNode = document.createElement(nodeString);
					configNode.setAttribute("key",getOWLLinkMappedConfigString(keyString));

					QDomElement oneOfNode = document.createElement("OneOf");
					oneOfNode.setAttribute("abbreviatedIRI","xsd:string");
					configNode.appendChild(oneOfNode);

					QStringList optionsList(oneOfOptions);
					foreach (QString optionString, optionsList) {
						QDomElement litNode = document.createElement("Literal");
						QDomText optTextNode = document.createTextNode(optionString);
						litNode.appendChild(optTextNode);
						oneOfNode.appendChild(litNode);
					}

					QDomElement litNode = document.createElement("Literal");
					QDomText optTextNode = document.createTextNode(defaultOption);
					litNode.appendChild(optTextNode);
					configNode.appendChild(litNode);

					return configNode;
				}


				QDomElement COWLLinkRecordInterpreter::getBooleanConfigNode(const QString &keyString, const QString &defaultValue, const QString &nodeString) {
					QDomElement configNode = document.createElement(nodeString);
					configNode.setAttribute("key",getOWLLinkMappedConfigString(keyString));

					QDomElement datatypeNode = document.createElement("Datatype");
					datatypeNode.setAttribute("abbreviatedIRI","xsd:boolean");
					configNode.appendChild(datatypeNode);

					QDomElement litNode = document.createElement("Literal");
					QDomText optTextNode = document.createTextNode(defaultValue);
					litNode.appendChild(optTextNode);
					configNode.appendChild(litNode);

					return configNode;
				}

				QDomElement COWLLinkRecordInterpreter::getIntegerConfigNode(const QString &keyString, qint64 integer, const QString &nodeString) {
					QDomElement configNode = document.createElement(nodeString);
					configNode.setAttribute("key",getOWLLinkMappedConfigString(keyString));

					QDomElement datatypeNode = document.createElement("Datatype");
					datatypeNode.setAttribute("abbreviatedIRI","xsd:integer");
					configNode.appendChild(datatypeNode);

					QDomElement litNode = document.createElement("Literal");
					QDomText optTextNode = document.createTextNode(QString::number(integer));
					litNode.appendChild(optTextNode);
					configNode.appendChild(litNode);

					return configNode;
				}

				QDomElement COWLLinkRecordInterpreter::getStringConfigNode(const QString &keyString, const QString &defaultValue, const QString &nodeString) {
					QDomElement configNode = document.createElement(nodeString);
					configNode.setAttribute("key",getOWLLinkMappedConfigString(keyString));

					QDomElement datatypeNode = document.createElement("Datatype");
					datatypeNode.setAttribute("abbreviatedIRI","xsd:string");
					configNode.appendChild(datatypeNode);

					QDomElement litNode = document.createElement("Literal");
					QDomText optTextNode = document.createTextNode(defaultValue);
					litNode.appendChild(optTextNode);
					configNode.appendChild(litNode);

					return configNode;
				}

				QByteArray COWLLinkRecordInterpreter::getByteArray() {
					return document.toString().toUtf8();
				}


				QString COWLLinkRecordInterpreter::getOWLLinkMappedConfigString(const QString &keyName) {
					if (keyName == "Konclude.OWLlink.AbbreviatedIRIs") {
						return QString("abbreviatesIRIs");
					}
					return keyName;
				}

			}; // end namespace OWLlink

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
