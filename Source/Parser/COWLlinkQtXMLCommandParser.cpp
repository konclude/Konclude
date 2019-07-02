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

#include "COWLlinkQtXMLCommandParser.h"


namespace Konclude {

	namespace Parser {



		COWLlinkQtXMLCommandParser::COWLlinkQtXMLCommandParser(CConfiguration* config) : CLogIdentifier("::Konclude::Parser::OWLlinkQtCommandParser",this) {
			LOG(INFO,"::Konclude::Parser::OWLlinkQtCommandParser",logTr("OWLlink Parser initialized."),this);
			command = 0;
			recorder = 0;
			delegater = 0;

			mConfInversePropDirectSupport = true;
			if (config) {
				mConfInversePropDirectSupport = CConfigDataReader::readConfigBoolean(config,"Konclude.OWLlink.DirectInversePropertiesQuerySupport",true);
			}
		}


		COWLlinkQtXMLCommandParser::~COWLlinkQtXMLCommandParser() {
		}




		bool COWLlinkQtXMLCommandParser::realizeCommand(CParseOWLlinkCommandsCommand *parseCommand, CCommandDelegater *commandDelegater) {
			delegater = commandDelegater;
			command = parseCommand;
			if (command) {
				recorder = command->getRecorder();
			}
			CStartProcessCommandRecord::makeRecord(recorder,getLogDomain(),command);
			QByteArray *byteArray = command->getByteArray();
			bool success = parse(*byteArray);
			CStopProcessCommandRecord::makeRecord(recorder,getLogDomain(),command);
			CFinishProcessCommandRecord::makeRecord(recorder,getLogDomain(),command);
			return success;
		}


		bool COWLlinkQtXMLCommandParser::parse(const QByteArray &content) {
			bool successfullParsed = true;


			QDomDocument document;

			document.setContent(content,true);

			QDomElement documentElement(document.documentElement());

			successfullParsed = parse(&documentElement);

			return successfullParsed;
		}


		bool COWLlinkQtXMLCommandParser::parse(QDomElement *node) {
			bool successfullParsed = true;

			QDomElement documentElement(*node);

			QString docElemName = documentElement.tagName();
			if (docElemName == "ox:RequestMessage" || docElemName == "RequestMessage") {
				LOG(INFO,getLogDomain(),logTr("Found OWLlink-Request Node."),this);
				parseOWLlinkRequestNode(&documentElement);
			} else {
				successfullParsed = false;
				CNotSupportedNodeErrorRecord::makeRecord(docElemName,recorder,getLogDomain(),command);
			}

			return successfullParsed;
		}




		void COWLlinkQtXMLCommandParser::parseOWLlinkRequestNode(QDomElement *requestNode) {
			QDomElement node = requestNode->firstChildElement();

			CPreconditionCommand *lastPreComm = 0;

			QLinkedList<CPreconditionCommand *> prevCalcPreCommList;

			while (!node.isNull()) {
				// build for each node an expression

				CPreconditionCommand *preComm;

				QString namespaceString = node.namespaceURI();
				QString docElemName = node.localName();
				bool inversePropertiesDirectQuerySupport = false;

				if (docElemName.isEmpty()) {
					docElemName = node.tagName();
				}

				if (namespaceString.isEmpty() || namespaceString == "http://www.owllink.org/owllink-xml#" || namespaceString == "http://www.owllink.org/owllink#" || namespaceString == "http://www.owllink.org/ext/retraction#") {


					if (docElemName == "ImmediatelyExit") {
						QCoreApplication::exit();
					} else if (docElemName == "ox:GetDescription" || docElemName == "GetDescription") {
						preComm = parseGetDescriptionNode(&node);
					} else if (docElemName == "ox:CreateKB" || docElemName == "CreateKB") {
						preComm = parseCreateKnowledgeBaseNode(&node);
					} else if (docElemName == "ox:InsertResponseComment" || docElemName == "InsertResponseComment") {
						preComm = parseInsertResponseCommentNode(&node);
					} else if (docElemName == "ox:Tell" || docElemName == "Tell") {
						preComm = parseTellKnowledgeBaseNode(&node);
					} else if (docElemName == "rec:Retract" || docElemName == "Retract") {
						preComm = parseRectractKnowledgeBaseNode(&node);
					} else if (docElemName == "ox:LoadKRSSTestConcepts" || docElemName == "LoadKRSSTestConcepts") {
						preComm = parseLoadKnowledgeBaseKRSSTestConceptNode(&node);
					} else if (docElemName == "ox:LoadOntology" || docElemName == "LoadOntology") {
						preComm = parseLoadKnowledgeBaseOntologyNode(&node);
					} else if (docElemName == "ox:LoadOntologies" || docElemName == "LoadOntologies") {
						preComm = parseLoadKnowledgeBaseOntologyNode(&node);
					} else if (docElemName == "ox:SaveExtractedKB" || docElemName == "SaveExtractedKB") {
						preComm = parseSaveExtractedKnowledgeBaseNode(&node);
					} else if (docElemName == "ox:SaveExtractedOWLlinkTestCaseKB" || docElemName == "SaveExtractedOWLlinkTestCaseKB") {
						preComm = parseSaveExtractedOWLlinkTestCaseKnowledgeBaseNode(&node);
					} else if (docElemName == "ox:IsClassSatisfiable" || docElemName == "IsClassSatisfiable") {
						preComm = parseProcessQueryNode(&node);
					} else if (docElemName == "ox:AreClassesEquivalent" || docElemName == "AreClassesEquivalent") {
						preComm = parseProcessQueryNode(&node);
					} else if (docElemName == "ox:AreClassesDisjoint" || docElemName == "AreClassesDisjoint") {
						preComm = parseProcessQueryNode(&node);
					} else if (docElemName == "ox:IsClassSubsumedBy" || docElemName == "IsClassSubsumedBy") {
						preComm = parseProcessQueryNode(&node);
					} else if (docElemName == "ox:IsInstanceOf" || docElemName == "IsInstanceOf") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetFlattenedTypes" || docElemName == "GetFlattenedTypes") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetFlattenedInstances" || docElemName == "GetFlattenedInstances") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetTypes" || docElemName == "GetTypes") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetSameIndividuals" || docElemName == "GetSameIndividuals") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetInstances" || docElemName == "GetInstances") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetObjectPropertyTargets" || docElemName == "GetObjectPropertyTargets") {
						inversePropertiesDirectQuerySupport = mConfInversePropDirectSupport;
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetFlattenedObjectPropertyTargets" || docElemName == "GetFlattenedObjectPropertyTargets") {
						inversePropertiesDirectQuerySupport = mConfInversePropDirectSupport;
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetObjectPropertySources" || docElemName == "GetObjectPropertySources") {
						inversePropertiesDirectQuerySupport = mConfInversePropDirectSupport;
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetFlattenedObjectPropertySources" || docElemName == "GetFlattenedObjectPropertySources") {
						inversePropertiesDirectQuerySupport = mConfInversePropDirectSupport;
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetSubClasses" || docElemName == "GetSubClasses") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetSuperClasses" || docElemName == "GetSuperClasses") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetEquivalentClasses" || docElemName == "GetEquivalentClasses") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:IsEntailed" || docElemName == "IsEntailed") {
						preComm = parseProcessQueryNode(&node);

					}  else if (docElemName == "ox:GetObjectPropertiesOfSource" || docElemName == "GetObjectPropertiesOfSource") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetObjectPropertiesOfTarget" || docElemName == "GetObjectPropertiesOfTarget") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetObjectPropertiesBetween" || docElemName == "GetObjectPropertiesBetween") {
						preComm = parseProcessQueryNode(&node);


					}  else if (docElemName == "ox:GetSubObjectProperties" || docElemName == "GetSubObjectProperties") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetSuperObjectProperties" || docElemName == "GetSuperObjectProperties") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetEquivalentObjectProperties" || docElemName == "GetEquivalentObjectProperties") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetSubObjectPropertyHierarchy" || docElemName == "GetSubObjectPropertyHierarchy") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:IsObjectPropertySatisfiable" || docElemName == "IsObjectPropertySatisfiable") {
						preComm = parseProcessQueryNode(&node);

					}  else if (docElemName == "ox:GetSubDataProperties" || docElemName == "GetSubDataProperties") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetSuperDataProperties" || docElemName == "GetSuperDataProperties") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetEquivalentDataProperties" || docElemName == "GetEquivalentDataProperties") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetSubDataPropertyHierarchy" || docElemName == "GetSubDataPropertyHierarchy") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:IsDataPropertySatisfiable" || docElemName == "IsDataPropertySatisfiable") {
						preComm = parseProcessQueryNode(&node);

					}  else if (docElemName == "ox:GetNondeterministicIndividuals" || docElemName == "GetNondeterministicIndividuals") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetDeterministicIndividuals" || docElemName == "GetDeterministicIndividuals") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetNondeterministicClassAssertions" || docElemName == "GetNondeterministicClassAssertions" || docElemName == "ox:GetPossibleClassAssertions" || docElemName == "GetPossibleClassAssertions") {
						preComm = parseProcessQueryNode(&node);
					}  else if (docElemName == "ox:GetDeterministicClassAssertions" || docElemName == "GetDeterministicClassAssertions" || docElemName == "ox:GetKnownClassAssertions" || docElemName == "GetKnownClassAssertions") {
						preComm = parseProcessQueryNode(&node);

					}  else if (docElemName == "ox:GetSubClassHierarchy" || docElemName == "GetSubClassHierarchy") {
						preComm = parseGetSubClassHierarchyNode(&node);
					}  else if (docElemName == "ox:IsKBSatisfiable" || docElemName == "IsKBSatisfiable") {
						preComm = parseIsConsistentNode(&node);
					} else if (docElemName == "ox:ReleaseKB" || docElemName == "ReleaseKB") {
						preComm = parseReleaseKnowledgeBaseNode(&node);
					} else if (docElemName == "ox:GetKBLanguage" || docElemName == "GetKBLanguage") {
						preComm = parseGetKnowledgeBaseLanguageNode(&node);
					} else if (docElemName == "ox:GetAllClasses" || docElemName == "GetAllClasses") {
						preComm = parseGetKnowledgeBaseClassEntitiesNode(&node);
					} else if (docElemName == "ox:GetAllObjectProperties" || docElemName == "GetAllObjectProperties") {
						preComm = parseGetKnowledgeBaseObjectPropertyEntitiesNode(&node);
					} else if (docElemName == "ox:GetAllIndividuals" || docElemName == "GetAllIndividuals") {
						preComm = parseGetKnowledgeBaseIndividualEntitiesNode(&node);
					} else if (docElemName == "ox:Classify" || docElemName == "Classify") {
						preComm = parseKnowledgeBaseClassifyNode(&node);
					} else if (docElemName == "ox:Realize" || docElemName == "Realize" || docElemName == "ox:Realise" || docElemName == "Realise") {
						preComm = parseKnowledgeBaseRealizeNode(&node);
					} else if (docElemName == "ox:TestTestsuite" || docElemName == "TestTestsuite") {
						preComm = parseTestTestsuiteNode(&node);
					} else if (docElemName == "ox:EvaluateTestsuite" || docElemName == "EvaluateTestsuite") {
						preComm = parseEvaluateTestsuiteNode(&node);
					} else if (docElemName == "ox:GetSetting" || docElemName == "GetSetting") {
						preComm = parseGetSettingNode(&node);
					} else if (docElemName == "ox:GetSettings" || docElemName == "GetSettings") {
						preComm = parseGetSettingsNode(&node);
					} else if (docElemName == "ox:Set" || docElemName == "Set" || docElemName == "ox:SetSetting" || docElemName == "SetSetting") {
						preComm = parseSetSettingNode(&node);
					} else {
						CNotSupportedNodeErrorRecord::makeRecord(docElemName,recorder,getLogDomain(),command);
						preComm = new CSkipUnsupportedCommandCommand(docElemName);
					}

				} else {
					QString namespaceNodeName = docElemName;
					if (!namespaceString.isEmpty()) {
						namespaceNodeName = namespaceString+":"+docElemName;
					}
					CNotSupportedNodeErrorRecord::makeRecord(namespaceNodeName,recorder,getLogDomain(),command);
					preComm = new CSkipUnsupportedCommandCommand(namespaceNodeName);
				}

				if (preComm) {

					CKnowledgeBaseQueryCommand *queryCommand = dynamic_cast<CKnowledgeBaseQueryCommand *>(preComm);
					if (queryCommand) {
						parseQueryExtensions(&node,queryCommand,inversePropertiesDirectQuerySupport);

						// build default structure for query commands
						CBuildQueryCommand *buildCommand = 0;
						CCalculateQueryCommand *calcCommand = 0;

						buildCommand = queryCommand->getBuildQueryCommand();
						if (!buildCommand) {
							// generate a build query subcommand
							buildCommand = new CBuildQueryCommand(0,queryCommand);
							queryCommand->setBuildQueryCommand(buildCommand);
						} 
						calcCommand = queryCommand->getCalculateQueryCommand();
						if (!calcCommand) {
							calcCommand = new CCalculateQueryCommand(buildCommand,queryCommand);
							queryCommand->setCalculateQueryCommand(calcCommand);
						}
						prevCalcPreCommList.append(queryCommand);
					} else {
						foreach (CPreconditionCommand *calcPreComm, prevCalcPreCommList) {
							if (calcPreComm != lastPreComm) {
								preComm->addCommandPrecondition(new CCommandProcessedPrecondition(calcPreComm));
							}
						}
					}


					if (lastPreComm) {
						preComm->addCommandPrecondition(new CCommandProcessedPrecondition(lastPreComm));
					}
					if (command) {
						command->makeToSubCommand(preComm);
					}
					if (delegater) {
						delegater->delegateCommand(preComm);
					}

					lastPreComm = preComm;


					if (queryCommand) {
						if (queryCommand->getBuildQueryCommand()) {
							// because of revision management, the system has not to wait until the calculation of a query has finished to continue
							// with the followed commands
							lastPreComm = queryCommand->getBuildQueryCommand();
						}
					}
				}

				preComm = 0;

				node = node.nextSiblingElement();
			}
		}



		CGetDescriptionCommand *COWLlinkQtXMLCommandParser::parseGetDescriptionNode(QDomElement *descriptionNode) {
			return new CGetDescriptionCommand();
		}


		CInsertResponseCommentCommand *COWLlinkQtXMLCommandParser::parseInsertResponseCommentNode(QDomElement *node) {
			QString commentString;
			commentString = node->attribute(QString("commentText"));
			return new CInsertResponseCommentCommand(commentString);
		}

		CCreateKnowledgeBaseCommand *COWLlinkQtXMLCommandParser::parseCreateKnowledgeBaseNode(QDomElement *node) {
			QString kbName;
			bool hasKBName = true;
			if (!node->hasAttribute(QString("kb"))) {
				hasKBName = false;
			} else {
				kbName = node->attribute(QString("kb"));
			}
			CCreateKnowledgeBaseCommand* cKCommand = new CCreateKnowledgeBaseCommand(kbName);

			QDomElement childNode = node->firstChildElement("Prefix");
			while (!childNode.isNull()) {
				QString name = childNode.attribute("name");
				QString fullIRI = childNode.attribute("fullIRI");
				if (!name.isEmpty() && !fullIRI.isEmpty()) {
					CNamePrefix* namePrefix = new CNamePrefix(name,fullIRI);
					cKCommand->addNamePrefix(namePrefix);
				}
				childNode = childNode.nextSiblingElement("Prefix");
			}

			return cKCommand;
		}

		CReleaseKnowledgeBaseCommand *COWLlinkQtXMLCommandParser::parseReleaseKnowledgeBaseNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CReleaseKnowledgeBaseCommand(kbName);
		}

		CClassifyQueryCommand* COWLlinkQtXMLCommandParser::parseKnowledgeBaseClassifyNode(QDomElement *node) {
			QString kbName = node->attribute(QString("kb"));
			return new CClassifyQueryCommand(kbName);
		}

		CRealizeQueryCommand* COWLlinkQtXMLCommandParser::parseKnowledgeBaseRealizeNode(QDomElement *node) {
			QString kbName = node->attribute(QString("kb"));
			return new CRealizeQueryCommand(kbName);
		}

		CParseTestsuiteXMLNodeCommand *COWLlinkQtXMLCommandParser::parseTestTestsuiteNode(QDomElement *node) {
			CParseTestsuiteXMLNodeCommand *parseTestsuiteComm = new CParseTestsuiteXMLNodeCommand(*node);
			if (command) {
				QString sourcePath = command->getSourcePath();
				parseTestsuiteComm->setSourcePath(sourcePath);
			}
			return parseTestsuiteComm;
		}


		CGetKnowledgeBaseLanguageCommand *COWLlinkQtXMLCommandParser::parseGetKnowledgeBaseLanguageNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CGetKnowledgeBaseLanguageCommand(kbName);
		}


		CGetKnowledgeBaseClassEntitiesCommand *COWLlinkQtXMLCommandParser::parseGetKnowledgeBaseClassEntitiesNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CGetKnowledgeBaseClassEntitiesCommand(kbName);
		}


		CGetKnowledgeBaseObjectPropertyEntitiesCommand *COWLlinkQtXMLCommandParser::parseGetKnowledgeBaseObjectPropertyEntitiesNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CGetKnowledgeBaseObjectPropertyEntitiesCommand(kbName);
		}


		CGetKnowledgeBaseNamedIndividualEntitiesCommand *COWLlinkQtXMLCommandParser::parseGetKnowledgeBaseIndividualEntitiesNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CGetKnowledgeBaseNamedIndividualEntitiesCommand(kbName);
		}



		CEvaluateTestsuiteCommand* COWLlinkQtXMLCommandParser::parseEvaluateTestsuiteNode(QDomElement* node) {
			CReasonerEvaluationProgram* evalProg = new CReasonerEvaluationProgram();
			QString testName = node->attribute("name");
			QString configFile = node->attribute("configFile");
			QString inputDir = node->attribute("inputDirectory");
			QString outputDir = node->attribute("outputDirectory");
			QString analysingDir = node->attribute("analysingDirectory");
			evalProg->setConfigFileString(configFile);
			evalProg->setInputDirectoryString(inputDir);
			evalProg->setOutputDirectoryString(outputDir);
			evalProg->setAnalyserDirectoryString(analysingDir);
			evalProg->setProgramName(testName);

			QDomElement reasonerChildElm = node->firstChildElement("EvaluateReasoner");
			while (!reasonerChildElm.isNull()) {
				QString initReasonerString = reasonerChildElm.attribute("initFile");
				evalProg->appendReasonerString(initReasonerString);
				reasonerChildElm = reasonerChildElm.nextSiblingElement("EvaluateReasoner");
			}

			QDomElement analyseChildElm = node->firstChildElement("Analysing");
			while (!analyseChildElm.isNull()) {
				QString analyseString = analyseChildElm.attribute("analyseName");
				evalProg->appendAnalyserString(analyseString);
				analyseChildElm = analyseChildElm.nextSiblingElement("Analysing");
			}

			QDomElement filteringChildElm = node->firstChildElement("Filtering");
			while (!filteringChildElm.isNull()) {
				QString filterFileString = filteringChildElm.attribute("filterFile");
				evalProg->appendFilterString(filterFileString);
				filteringChildElm = filteringChildElm.nextSiblingElement("Filtering");
			}

			CEvaluateTestsuiteCommand* evaluateTestsuiteComm = new CEvaluateTestsuiteCommand(testName,evalProg);
			return evaluateTestsuiteComm;
		}


		CTellKnowledgeBaseOWL2XMLNodeCommand *COWLlinkQtXMLCommandParser::parseTellKnowledgeBaseNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CTellKnowledgeBaseOWL2XMLNodeCommand(kbName,*node);
		}

		CTellKnowledgeBaseOWL2XMLNodeCommand *COWLlinkQtXMLCommandParser::parseRectractKnowledgeBaseNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CTellKnowledgeBaseOWL2XMLNodeCommand(kbName,*node);
		}

		CLoadKnowledgeBaseKRSSTestConceptCommand *COWLlinkQtXMLCommandParser::parseLoadKnowledgeBaseKRSSTestConceptNode(QDomElement *node) {
			QString kbName = node->attribute(QString("kb"));
			QString defNames = node->attribute(QString("testConceptNames"));
			QString loadIRI = node->attribute(QString("loadIRI"));
			return new CLoadKnowledgeBaseKRSSTestConceptCommand(kbName,loadIRI,defNames);
		}


		CLoadKnowledgeBaseOWLAutoOntologyCommand *COWLlinkQtXMLCommandParser::parseLoadKnowledgeBaseOntologyNode(QDomElement *node) {
			QString kbName = node->attribute(QString("kb"));

			QStringList ontoIRIList;
			QDomElement childElm = node->firstChildElement("ontologyIRI");
			while (!childElm.isNull()) {
				QString ontoIRIString = childElm.attribute("IRI");
				ontoIRIList.append(ontoIRIString);
				childElm = childElm.nextSiblingElement("ontologyIRI");
			}
			childElm = node->firstChildElement("OntologyIRI");
			while (!childElm.isNull()) {
				QString ontoIRIString = childElm.attribute("IRI");
				ontoIRIList.append(ontoIRIString);
				childElm = childElm.nextSiblingElement("OntologyIRI");
			}
			return new CLoadKnowledgeBaseOWLAutoOntologyCommand(kbName,ontoIRIList);
		}


		CSaveOWL2XMLExtractedKnowledgeBaseCommand *COWLlinkQtXMLCommandParser::parseSaveExtractedKnowledgeBaseNode(QDomElement *node) {
			QString kbName = node->attribute(QString("kb"));
			QStringList entityNames;
			QDomElement childNode = node->firstChildElement();
			while (!childNode.isNull()) {
				QString entityName = childNode.attribute("IRI");
				if (!entityName.isEmpty()) {
					entityNames.append(entityName);
				}
				childNode = childNode.nextSiblingElement();
			}
			QString saveIRI = node->attribute(QString("saveIRI"));
			return new CSaveOWL2XMLExtractedKnowledgeBaseCommand(kbName,saveIRI,entityNames);
		}


		CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand* COWLlinkQtXMLCommandParser::parseSaveExtractedOWLlinkTestCaseKnowledgeBaseNode(QDomElement *node) {
			QString kbName = node->attribute(QString("kb"));
			QStringList entityNames;
			QDomElement childNode = node->firstChildElement();
			while (!childNode.isNull()) {
				QString entityName = childNode.attribute("IRI");
				if (!entityName.isEmpty()) {
					entityNames.append(entityName);
				}
				childNode = childNode.nextSiblingElement();
			}
			QString saveIRI = node->attribute(QString("saveIRI"));
			QString testKBName = node->attribute(QString("testKBName"));
			return new CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand(kbName,saveIRI,entityNames,testKBName);
		}


		CProcessQueryOWL2XMLNodeCommand *COWLlinkQtXMLCommandParser::parseProcessQueryNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CProcessQueryOWL2XMLNodeCommand(kbName,*node);
		}


		CIsConsistentQueryOWL2XMLNodeCommand* COWLlinkQtXMLCommandParser::parseIsConsistentNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CIsConsistentQueryOWL2XMLNodeCommand(kbName,*node);
		}

		CGetSubClassHierarchyQueryOWL2XMLNodeCommand *COWLlinkQtXMLCommandParser::parseGetSubClassHierarchyNode(QDomElement *node) {
			QString kbName;
			kbName = node->attribute(QString("kb"));
			return new CGetSubClassHierarchyQueryOWL2XMLNodeCommand(kbName,*node);
		}

		void COWLlinkQtXMLCommandParser::parseQueryExtensions(QDomElement *node, CKnowledgeBaseQueryCommand *queryCommand, bool inversePropDirectSupport) {

			
			CTestExtendedCalculateQueryCommand *testExtCalcQueryComm = dynamic_cast<CTestExtendedCalculateQueryCommand *>(queryCommand->getCalculateQueryCommand()); 
			if (node->hasAttribute("timing")) {
				if (CStringPrimitiveConverter::convertStringToBoolean(node->attribute("result"),true)) {
					if (!testExtCalcQueryComm) {
						CBuildQueryCommand *buildCommand = queryCommand->getBuildQueryCommand();
						if (!buildCommand) {
							// generate a build query subcommand
							buildCommand = new CBuildQueryCommand(0,queryCommand);
							queryCommand->setBuildQueryCommand(buildCommand);
						}
						testExtCalcQueryComm = new CTestExtendedCalculateQueryCommand(buildCommand,queryCommand);					
						queryCommand->setCalculateQueryCommand(testExtCalcQueryComm);
					}
					testExtCalcQueryComm->setTimingReportLevel(1);
				}
			}
			if (requiresComplexQuery(node,inversePropDirectSupport)) {
				queryCommand->setQueryComplexity(true);
			}

			parseQueryExpectedResult(node,queryCommand);
		}

		void COWLlinkQtXMLCommandParser::parseQueryExpectedResult(QDomElement *node, CKnowledgeBaseQueryCommand *queryCommand) {
			CTestExtendedCalculateQueryCommand *testExtCalcQueryComm = dynamic_cast<CTestExtendedCalculateQueryCommand *>(queryCommand->getCalculateQueryCommand()); 
			QDomElement expResNode = node->firstChildElement("ExpectedResult");
			if (!expResNode.isNull()) {
				if (!testExtCalcQueryComm) {
					CBuildQueryCommand *buildCommand = queryCommand->getBuildQueryCommand();
					if (!buildCommand) {
						// generate a build query subcommand
						buildCommand = new CBuildQueryCommand(0,queryCommand);
						queryCommand->setBuildQueryCommand(buildCommand);
					}
					testExtCalcQueryComm = new CTestExtendedCalculateQueryCommand(buildCommand,queryCommand);					
					queryCommand->setCalculateQueryCommand(testExtCalcQueryComm);
				}
				CQueryResult *expectedResult = 0;
				expectedResult = parseQueryResult(&expResNode);
				testExtCalcQueryComm->setExpectedQueryResult(expectedResult);
			}
			
		}



		bool COWLlinkQtXMLCommandParser::requiresComplexQuery(QDomElement *node, bool inversePropDirectSupport) {
			bool reComplex = false;
			QDomElement childEl = node->firstChildElement();
			while (!reComplex && !childEl.isNull()) {
				bool childComplex = true;
				QString elName = childEl.tagName();
				if (elName == "Class" || elName == "owl:Class") {
					childComplex = false;
				} else if (elName == "NamedIndividual" || elName == "owl:NamedIndividual") {
					childComplex = false;
				} else if (elName == "ObjectProperty" || elName == "owl:ObjectProperty") {
					childComplex = false;
				} else if (elName == "DataProperty" || elName == "owl:DataProperty") {
					childComplex = false;
				} else if (elName == "ExpectedResult") {
					childComplex = false;
				} else if (inversePropDirectSupport && elName == "ObjectInverseOf" || elName == "owl:ObjectInverseOf") {
					if (!requiresComplexQuery(&childEl,inversePropDirectSupport)) {
						childComplex = false;
					}
				}

				reComplex |= childComplex;

				childEl = childEl.nextSiblingElement();
			}
			return reComplex;
		}




		CQueryResult *COWLlinkQtXMLCommandParser::parseQueryResult(QDomElement *resultNode) {
			COWLlinkQtXMLResultParser resultParser;
			QDomElement node = resultNode->firstChildElement();
			CQueryResult *queryResult = 0;
			while (!node.isNull() && !queryResult) {
				QString docElemName = node.tagName();
				if (docElemName == "ox:BooleanResponse" || docElemName == "BooleanResponse") {
					queryResult = resultParser.parseBooleanQueryResult(&node);
				} else if (docElemName == "ox:ClassHierarchy" || docElemName == "ClassHierarchy") {
					queryResult = resultParser.parseClassHierarchyResult(&node);
				} else {
					node = node.nextSiblingElement();
				}

			}
			return queryResult;
		}


		CGetConfigCommand *COWLlinkQtXMLCommandParser::parseGetSettingNode(QDomElement *node) {
			QString kbName = node->attribute(QString("kb"),"");
			QString propertyName = node->attribute(QString("key"),"");
			propertyName = getOWLLinkMappedConfigString(propertyName);
			CGetConfigCommand *confComm = 0;
			if (!kbName.isEmpty()) {
				confComm = new CGetKnowledgeBaseConfigCommand(propertyName,kbName);
			} else {
				confComm = new CGetConfigCommand(propertyName);
			}
			return confComm;
		}


		CGetSettingsCommand *COWLlinkQtXMLCommandParser::parseGetSettingsNode(QDomElement *node) {
			QString kbName = node->attribute(QString("kb"),"");
			CGetSettingsCommand *confComm = 0;
			if (!kbName.isEmpty()) {
				confComm = new CGetKnowledgeBaseSettingsCommand(kbName);
			} else {
				confComm = new CGetSettingsCommand();
			}
			return confComm;
		}


		CSetConfigCommand *COWLlinkQtXMLCommandParser::parseSetSettingNode(QDomElement *node) {
			QString kbName = node->attribute(QString("kb"),"");
			QString propertyName = node->attribute(QString("key"),"");
			QString propertyValue = node->firstChildElement("Literal").text();
			propertyName = getOWLLinkMappedConfigString(propertyName);
			CSetConfigCommand *confComm = 0;
			if (!kbName.isEmpty()) {
				confComm = new CSetKnowledgeBaseConfigCommand(propertyName,propertyValue,kbName);
			} else {
				confComm = new CSetConfigCommand(propertyName,propertyValue);
			}
			return confComm;
		}


		QString COWLlinkQtXMLCommandParser::getOWLLinkMappedConfigString(const QString &keyName) {
			if (keyName == "abbreviatesIRIs") {
				return QString("Konclude.OWLlink.AbbreviatedIRIs");
			}
			return keyName;
		}


	}; // end namespace Parser

}; // end namespace Konclude
