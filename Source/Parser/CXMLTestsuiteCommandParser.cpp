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

#include "CXMLTestsuiteCommandParser.h"


namespace Konclude {

	namespace Parser {



		CXMLTestsuiteCommandParser::CXMLTestsuiteCommandParser() : CLogIdentifier("::Konclude::Parser::XMLTestsuiteCommandParser",this) {
			LOG(INFO,"::Konclude::Parser::XMLTestsuiteCommandParser",logTr("XML Testsuite Parser initialized."),this);
			command = 0;
			recorder = 0;
			delegater = 0;
		}


		CXMLTestsuiteCommandParser::~CXMLTestsuiteCommandParser() {
		}




		bool CXMLTestsuiteCommandParser::realizeCommand(CParseTestsuiteCommandsCommand *parseCommand, CCommandDelegater *commandDelegater) {
			delegater = commandDelegater;
			command = parseCommand;
			if (command) {
				recorder = command->getRecorder();
			}
			sourcePath = command->getSourcePath();
			CStartProcessCommandRecord::makeRecord(recorder,getLogDomain(),command);
			QByteArray *byteArray = command->getByteArray();
			bool success = parse(*byteArray);
			CStopProcessCommandRecord::makeRecord(recorder,getLogDomain(),command);
			CFinishProcessCommandRecord::makeRecord(recorder,getLogDomain(),command);
			return success;
		}


		bool CXMLTestsuiteCommandParser::parse(const QByteArray &content) {
			bool successfullParsed = true;


			QDomDocument document;

			document.setContent(content,true);

			QDomElement documentElement(document.documentElement());

			successfullParsed = parse(&documentElement);

			return successfullParsed;
		}


		bool CXMLTestsuiteCommandParser::parse(QDomElement *node) {
			bool successfullParsed = true;

			QDomElement documentElement(*node);

			QString docElemName = documentElement.tagName();
			if (docElemName == "ox:RequestMessage" || docElemName == "RequestMessage") {
				LOG(INFO,getLogDomain(),logTr("Found OWLlink-Request Node."),this);
				parseOWLlinkTestsuiteRequestNode(&documentElement);
			} else {
				successfullParsed = false;
				CNotSupportedNodeErrorRecord::makeRecord(docElemName,recorder,getLogDomain(),command);
			}

			return successfullParsed;
		}



		bool CXMLTestsuiteCommandParser::realizeCommand(CParseTestsuiteXMLNodeCommand *testsuiteNodeCommand, CCommandDelegater *commandDelegater) {
			delegater = commandDelegater;
			nodeCommand = testsuiteNodeCommand;
			if (nodeCommand) {
				recorder = nodeCommand->getRecorder();
			}
			CStartProcessCommandRecord::makeRecord(recorder,getLogDomain(),nodeCommand);
			QDomElement testsuiteNode = nodeCommand->getTestsuiteNode();

			sourcePath = nodeCommand->getSourcePath();

			QString docElemName = testsuiteNode.tagName();
			if (docElemName == "ox:TestTestsuite" || docElemName == "TestTestsuite") {
				CTestTestsuiteCommand *testTestsuiteComm = parseTestTestsuiteNode(&testsuiteNode);
				nodeCommand->makeToSubCommand(testTestsuiteComm);
				QDomElement testNode = testsuiteNode.firstChildElement();
				while (!testNode.isNull()) {
					QString testNodeElemName = testNode.tagName();

					if (testNodeElemName == "ox:TestTestfileErrors" || testNodeElemName == "TestTestfileErrors") {
						CTestTestfileCommand *testfileTestComm = parseTestTestfileNode(&testNode,testTestsuiteComm);
					}

					testNode = testNode.nextSiblingElement();
				}
				delegater->delegateCommand(testTestsuiteComm);

			} else {
				CNotSupportedNodeErrorRecord::makeRecord(docElemName,recorder,getLogDomain(),nodeCommand);
				CCommand *skipComm = new CSkipUnsupportedCommandCommand(docElemName);
				delegater->delegateCommand(skipComm);
			}

			CStopProcessCommandRecord::makeRecord(recorder,getLogDomain(),nodeCommand);
			CFinishProcessCommandRecord::makeRecord(recorder,getLogDomain(),nodeCommand);
			return true;
		}



		void CXMLTestsuiteCommandParser::parseOWLlinkTestsuiteRequestNode(QDomElement *requestNode) {
			QDomElement node = requestNode->firstChildElement();

			CPreconditionCommand *lastPreComm = 0;

			while (!node.isNull()) {
				// build for each node an expression

				CPreconditionCommand *preComm;

				QString docElemName = node.tagName();
				if (docElemName == "ox:TestTestsuite" || docElemName == "TestTestsuite") {
					CTestTestsuiteCommand *testTestsuiteComm = parseTestTestsuiteNode(&node);
					QDomElement testNode = node.firstChildElement();
					while (!testNode.isNull()) {
						QString testNodeElemName = testNode.tagName();

						if (testNodeElemName == "ox:TestTestfileErrors" || testNodeElemName == "TestTestfileErrors") {
							CTestTestfileCommand *testfileTestComm = parseTestTestfileNode(&testNode,testTestsuiteComm);
						}

						testNode = testNode.nextSiblingElement();
					}
					preComm = testTestsuiteComm;
				} else {
					CNotSupportedNodeErrorRecord::makeRecord(docElemName,recorder,getLogDomain(),command);
					preComm = new CSkipUnsupportedCommandCommand(docElemName);
				}


				if (preComm) {


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

				}

				preComm = 0;

				node = node.nextSiblingElement();
			}
		}



		CTestTestsuiteCommand *CXMLTestsuiteCommandParser::parseTestTestsuiteNode(QDomElement *node) {
			QString nameString = node->attribute("name","Unamed Testsuite");
			CReasonerTestsuiteTest *testsuiteTest = new CReasonerTestsuiteTest();
			CTestTestsuiteCommand *testTestSuiteCommand = new CTestTestsuiteCommand(nameString);
			testTestSuiteCommand->setTestsuiteTest(testsuiteTest);
			QFileInfo fiSP(sourcePath);

			QString loopCountString = node->attribute("defaultTestLoops","1");
			bool convSucc = false;
			qint64 loopCount = loopCountString.toLongLong(&convSucc);
			if (convSucc) {
				testsuiteTest->setDefaultTestLoopCount(loopCount);
			}

			if (node->hasAttribute("defaultTimeout")) {
				QString timeoutString = node->attribute("defaultTimeout","0");
				convSucc = false;
				qint64 defTimeout = timeoutString.toLongLong(&convSucc);
				if (convSucc) {
					testsuiteTest->setDefaultTimeout(defTimeout);
				}
			}

			if (fiSP.absoluteDir().exists()) {
				testsuiteTest->setDefaultWorkingDirectory(fiSP.absolutePath());
			}

			if (node->hasAttribute("defaultWorkingDirectory")) {
				QString workingDir = node->attribute("defaultWorkingDirectory");
				testsuiteTest->setDefaultWorkingDirectory(workingDir);
			}

			return testTestSuiteCommand;
		}



		CTestTestfileCommand *CXMLTestsuiteCommandParser::parseTestTestfileNode(QDomElement *node, CTestTestsuiteCommand *testTestsuiteComm) {
			CReasonerTestsuiteTest *testsuiteTest = testTestsuiteComm->getTestsuiteTest();
			CReasonerTestfileTest *testfileTest = testsuiteTest->createTestfileTest();
			CTestTestfileCommand *testTestfileComm = new CTestTestfileCommand(testfileTest,testTestsuiteComm);

			if (node->hasAttribute("testLoops")) {
				QString loopCountString = node->attribute("testLoops","1");
				bool convSucc = false;
				qint64 loopCount = loopCountString.toLongLong(&convSucc);
				if (convSucc) {
					testfileTest->setTestLoopCount(loopCount);
				}
			}

			if (node->hasAttribute("timeout")) {
				QString timeoutString = node->attribute("timeout");
				bool convSucc = false;
				qint64 timeout = timeoutString.toLongLong(&convSucc);
				if (convSucc) {
					testfileTest->setTimeout(timeout);
				}
			}

			if (node->hasAttribute("workingDirectory")) {
				QString workingDir = node->attribute("workingDirectory");
				testfileTest->setWorkingDirectory(workingDir);
			}

			if (node->hasAttribute("requestSource")) {
				QString requestSource = node->attribute("requestSource");
				testfileTest->setTestRequestFile(requestSource);
			}

			if (node->hasAttribute("expectedResponseSource")) {
				QString expResponseSource = node->attribute("expectedResponseSource");
				testfileTest->setTestExpectedResponseFile(expResponseSource);
			}
			return testTestfileComm;
		}



	}; // end namespace Parser

}; // end namespace Konclude
