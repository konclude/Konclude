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

#include "COWLFileOWLlinkTypesRequestGenerator.h"


namespace Konclude {

	namespace Test {



		COWLFileOWLlinkTypesRequestGenerator::COWLFileOWLlinkTypesRequestGenerator(CConfiguration *config) 
					: CLogIdentifier("::Konclude::Test::OWLFileOWLlinkIndividualifyRequestGenerator",this),mConfig(config) {


			mReasonerTestKBName = "http://www.konclude.com/evaluation-testsuite/test-ontology";
		}


		COWLFileOWLlinkTypesRequestGenerator::~COWLFileOWLlinkTypesRequestGenerator() {
		}



		QDomDocument COWLFileOWLlinkTypesRequestGenerator::createRequestDocument() {
			QDomDocument document;

			QString owllinkNS = "http://www.owllink.org/owllink#";
			QString owlNS = "http://www.w3.org/2002/07/owl#";

			QDomElement rootNode;

			rootNode = document.createElementNS(owllinkNS,"RequestMessage");
			rootNode.setAttributeNS("http://www.w3.org/2001/XMLSchema-instance","xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
			rootNode.setAttribute("xmlns:owl",owlNS);
			document.appendChild(rootNode);

			return document;
		}


		QSet<QString> COWLFileOWLlinkTypesRequestGenerator::loadIndividualEntities(const QString& requestFileName) {

			CConcreteOntologyEntityCollectorBuilder entityGenBuilder;
			COWL2QtXMLOntologyStreamParser* owl2Parser = new COWL2QtXMLOntologyStreamParser(&entityGenBuilder);
			owl2Parser->parseOntology(requestFileName);
			delete owl2Parser;

			QSet<QString> individualNameSet = *entityGenBuilder.getIndividualNameSet();
			return individualNameSet;
		}



		bool COWLFileOWLlinkTypesRequestGenerator::generateOWLlinkTypesRequests(const QString& requestFileName, const QString& responseFileName, cint64 testCount) {
			bool generatedTestfiles = false;
			QSet<QString> individualNameTestingSet;
			QSet<QString> individualNameSet = loadIndividualEntities(requestFileName);
			cint64 maxTestIndividualCount = qMin(testCount,(cint64)individualNameSet.count());

			cint64 slashIndex = qMax(requestFileName.lastIndexOf("/"),requestFileName.lastIndexOf("\\"));
			QString fileNameString = requestFileName;
			if (slashIndex >= 0) {
				fileNameString = requestFileName.mid(slashIndex+1);
			}
			uint randInitNumber = 0;
			for (QString::const_iterator it = requestFileName.constBegin(), itEnd = requestFileName.constEnd(); it != itEnd; ++it) {
				QChar nameChar(*it);
				randInitNumber += nameChar.unicode();
			}
			qsrand(randInitNumber);

			for (cint64 testNr = 0; testNr < maxTestIndividualCount; ++testNr) {				
				cint64 nextTextIndividualNr = qrand() % individualNameSet.count();
				QSet<QString>::iterator individualNameIt = individualNameSet.begin();
				for (cint64 i = 0; i < nextTextIndividualNr; ++i) {
					++individualNameIt;
				}
				QString nextTestIndividualName = *individualNameIt;
				cint64 nextIndividualSwitchCount = individualNameSet.count();
				while (individualNameTestingSet.contains(nextTestIndividualName) && nextIndividualSwitchCount > 0) {
					++individualNameIt;
					++nextTextIndividualNr;
					if (individualNameIt == individualNameSet.end()) {
						individualNameIt == individualNameSet.begin();
						nextTextIndividualNr = 0;
					}
					nextTestIndividualName = *individualNameIt;
					--nextIndividualSwitchCount;
				}
				if (individualNameTestingSet.contains(nextTestIndividualName)) {
					break;
				}

				QString individualNameTestFileAppendix = getIndividualNameRequestFileAppendix(nextTestIndividualName,nextTextIndividualNr);
				QString testResponseFileName = QString(responseFileName).arg(individualNameTestFileAppendix);

				generatedTestfiles |= generateOWLlinkTypesRequest(requestFileName,testResponseFileName,nextTestIndividualName);
			}
			return generatedTestfiles;
		}

		QString COWLFileOWLlinkTypesRequestGenerator::getIndividualNameRequestFileAppendix(const QString& individualName, cint64 testNumber) {
			QString individualNameFileAppendixString = individualName;
			cint64 lastSlash = individualNameFileAppendixString.lastIndexOf("/");
			if (lastSlash != -1) {
				individualNameFileAppendixString = individualNameFileAppendixString.mid(lastSlash+1);
			}
			cint64 lastGatt = individualNameFileAppendixString.lastIndexOf("#");
			if (lastGatt != -1) {
				individualNameFileAppendixString = individualNameFileAppendixString.mid(lastGatt+1);
			}
			QString nextIndividualNameFileAppendixString;
			cint64 maxIndividualChar = 15;
			for (QString::const_iterator charIt = individualNameFileAppendixString.constBegin(), charItEnd = individualNameFileAppendixString.constEnd(); charIt != charItEnd; ++charIt) {
				QChar character(*charIt);
				if (character.isLetterOrNumber()) {
                    char asciiCharacter = character.toLatin1();
					if (asciiCharacter != 0 && maxIndividualChar > 0) {
						--maxIndividualChar;
						nextIndividualNameFileAppendixString += asciiCharacter;
					}
				}
			}
			return QString("%1~%2").arg(testNumber).arg(nextIndividualNameFileAppendixString);
		}


		bool COWLFileOWLlinkTypesRequestGenerator::generateOWLlinkTypesRequest(const QString& requestFileName, const QString& responseFileName, const QString& individualName) {

			QDomDocument requestDocument = createRequestDocument();
			QDomElement rootElement = requestDocument.documentElement();

			QDomElement cKBElement = requestDocument.createElement("CreateKB");
			cKBElement.setAttribute("kb",mReasonerTestKBName);
			rootElement.appendChild(cKBElement);

			QDomElement loadElement = requestDocument.createElement("LoadOntologies");
			loadElement.setAttribute("kb",mReasonerTestKBName);
			QDomElement ontoIRIElement = requestDocument.createElement("OntologyIRI");
			ontoIRIElement.setAttribute("IRI",QString("file:%1").arg(requestFileName));
			loadElement.appendChild(ontoIRIElement);
			rootElement.appendChild(loadElement);

			QDomElement realizeKBElement = requestDocument.createElement("Realize");
			realizeKBElement.setAttribute("kb",mReasonerTestKBName);
			rootElement.appendChild(realizeKBElement);

			QDomElement typesIndividualElement = requestDocument.createElement("GetFlattenedTypes");
			typesIndividualElement.setAttribute("kb",mReasonerTestKBName);
			QDomElement individualElement = requestDocument.createElement("owl:NamedIndividual");
			individualElement.setAttribute("IRI",individualName);
			typesIndividualElement.appendChild(individualElement);
			rootElement.appendChild(typesIndividualElement);

			QDomElement releaseKBElement = requestDocument.createElement("ReleaseKB");
			releaseKBElement.setAttribute("kb",mReasonerTestKBName);
			rootElement.appendChild(releaseKBElement);

			QFile outFile(responseFileName);
			if (outFile.open(QIODevice::WriteOnly)) {
				outFile.write(requestDocument.toByteArray());
				outFile.close();
			}

			LOG(INFO,getLogDomain(),logTr("Generated OWLlink types request '%1' for ontology '%2'.").arg(responseFileName).arg(requestFileName),this);

			return false;
		}


	}; // end namespace Test

}; // end namespace Konclude
