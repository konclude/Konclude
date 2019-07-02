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

#include "COWLFileOWLlinkIncrementalAssertionRequestGenerator.h"


namespace Konclude {

	namespace Test {



		COWLFileOWLlinkIncrementalAssertionRequestGenerator::COWLFileOWLlinkIncrementalAssertionRequestGenerator(CConfiguration *config) 
					: CLogIdentifier("::Konclude::Test::OWLFileOWLlinkIncrementalAssertionRequestGenerator",this),mConfig(config) {


			mReasonerTestKBName = "http://www.konclude.com/evaluation-testsuite/test-ontology";


		}


		COWLFileOWLlinkIncrementalAssertionRequestGenerator::~COWLFileOWLlinkIncrementalAssertionRequestGenerator() {
		}



		QDomDocument COWLFileOWLlinkIncrementalAssertionRequestGenerator::createRequestDocument() {
			QDomDocument document;

			QString owllinkNS = "http://www.owllink.org/owllink#";
			QString owlNS = "http://www.w3.org/2002/07/owl#";

			QDomElement rootNode;

			rootNode = document.createElementNS(owllinkNS,"RequestMessage");
			//rootNode.setAttributeNS("xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
			rootNode.setAttributeNS("http://www.w3.org/2001/XMLSchema-instance","xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
			rootNode.setAttribute("xmlns:owl",owlNS);
			document.appendChild(rootNode);

			return document;
		}


		QList<QDomElement> COWLFileOWLlinkIncrementalAssertionRequestGenerator::loadOntologyAssertions(const QString& requestFileName) {
			QList<QDomElement> assertionList;
			QFile ontologyFile(requestFileName);
			QDomDocument ontologyDocument;
			if (ontologyDocument.setContent(&ontologyFile,true)) {
				QDomElement rootElement = ontologyDocument.documentElement();
				QDomElement nextElement = rootElement.firstChildElement();
				while (!nextElement.isNull()) {
					QString tagName = nextElement.tagName();
					bool isAssertionNode = false;
					if (tagName == "ClassAssertion" || tagName == "ObjectPropertyAssertion" || tagName == "SameIndividual" || tagName == "DifferentIndividuals") {
						isAssertionNode = true;
					}
					else if (tagName == "DataPropertyAssertion" || tagName == "NegativeDataPropertyAssertion" || tagName == "NegativeObjectPropertyAssertion") {
						isAssertionNode = true;
					}
					if (isAssertionNode) {
						assertionList.append(nextElement);
					}
					nextElement = nextElement.nextSiblingElement();
				}
			}
			return assertionList;
		}






		bool COWLFileOWLlinkIncrementalAssertionRequestGenerator::generateOWLlinkIncrementalAssertionRequests(const QString& requestFileName, const QString& responseFileName, double removeAssertionPercentage, double addAssertionPercentage, cint64 testCount) {
			bool generatedTestfiles = false;
			QList<QDomElement> assertionList = loadOntologyAssertions(requestFileName);
			cint64 assertionCount = assertionList.count();


			cint64 removeAssertionCount = assertionCount*removeAssertionPercentage;
			cint64 addAssertionCount = assertionCount*addAssertionPercentage;

			addAssertionCount = qMax((cint64)1,addAssertionCount);
			removeAssertionCount = qMax((cint64)1,removeAssertionCount);

			if (assertionList.count() > removeAssertionCount + addAssertionCount) {

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

				for (cint64 testNr = 0; testNr < testCount; ++testNr) {

					cint64 removalNrHashCode = 0;
					cint64 additionNrHashCode = 0;

					QList<QDomElement> removalAssertionList;
					QList<QDomElement> additionAssertionList;
					QSet<cint64> assertionNrSet;
					for (cint64 remNr = 0; remNr < removeAssertionCount; ++remNr) {
						cint64 nextAssertionNr = (double)qrand() / (double)RAND_MAX * (double)(assertionCount-1);
						removalNrHashCode += nextAssertionNr;
						if (!assertionNrSet.contains(nextAssertionNr)) {
							assertionNrSet.insert(nextAssertionNr);
							QDomElement nextAssertionNode = assertionList.at(nextAssertionNr);
							removalAssertionList.append(nextAssertionNode);
						}
					}
					for (cint64 addNr = 0; addNr < addAssertionCount; ++addNr) {
						cint64 nextAssertionNr = (double)qrand() / (double)RAND_MAX * (double)(assertionCount-1);
						additionNrHashCode += nextAssertionNr;
						if (!assertionNrSet.contains(nextAssertionNr)) {
							assertionNrSet.insert(nextAssertionNr);
							QDomElement nextAssertionNode = assertionList.at(nextAssertionNr);
							additionAssertionList.append(nextAssertionNode);
						}
					}

					QString addRemTestFileAppendix = QString("%1~#%2-%3~#%4").arg(addAssertionCount).arg(additionNrHashCode).arg(removeAssertionCount).arg(removalNrHashCode);
					QString testResponseFileName = QString(responseFileName).arg(addRemTestFileAppendix);

					generatedTestfiles |= generateOWLlinkIncrementalAssertionConsistencyRequest(requestFileName,testResponseFileName,removalAssertionList,additionAssertionList);
				}

			}
			return generatedTestfiles;
		}




		bool COWLFileOWLlinkIncrementalAssertionRequestGenerator::generateOWLlinkIncrementalAssertionRequests(const QString& requestFileName, const QString& responseFileName, cint64 removeAssertionCount, cint64 addAssertionCount, cint64 testCount) {
			bool generatedTestfiles = false;
			QList<QDomElement> assertionList = loadOntologyAssertions(requestFileName);
			cint64 assertionCount = assertionList.count();



			if (assertionList.count() > removeAssertionCount + addAssertionCount) {

				for (cint64 testNr = 0; testNr < testCount; ++testNr) {

					cint64 removalNrHashCode = 0;
					cint64 additionNrHashCode = 0;

					QList<QDomElement> removalAssertionList;
					QList<QDomElement> additionAssertionList;
					QSet<cint64> assertionNrSet;
					for (cint64 remNr = 0; remNr < removeAssertionCount; ++remNr) {
						cint64 nextAssertionNr = (double)qrand() / (double)RAND_MAX * (double)(assertionCount-1);
						removalNrHashCode += nextAssertionNr;
						if (!assertionNrSet.contains(nextAssertionNr)) {
							assertionNrSet.insert(nextAssertionNr);
							QDomElement nextAssertionNode = assertionList.at(nextAssertionNr);
							removalAssertionList.append(nextAssertionNode);
						}
					}
					for (cint64 addNr = 0; addNr < addAssertionCount; ++addNr) {
						cint64 nextAssertionNr = (double)qrand() / (double)RAND_MAX * (double)(assertionCount-1);
						additionNrHashCode += nextAssertionNr;
						if (!assertionNrSet.contains(nextAssertionNr)) {
							assertionNrSet.insert(nextAssertionNr);
							QDomElement nextAssertionNode = assertionList.at(nextAssertionNr);
							additionAssertionList.append(nextAssertionNode);
						}
					}

					QString addRemTestFileAppendix = QString("%1~#%2-%3~#%4").arg(addAssertionCount).arg(additionNrHashCode).arg(removeAssertionCount).arg(removalNrHashCode);
					QString testResponseFileName = QString(responseFileName).arg(addRemTestFileAppendix);

					generatedTestfiles |= generateOWLlinkIncrementalAssertionConsistencyRequest(requestFileName,testResponseFileName,removalAssertionList,additionAssertionList);
				}

			}
			return generatedTestfiles;
		}


		bool COWLFileOWLlinkIncrementalAssertionRequestGenerator::generateOWLlinkIncrementalAssertionConsistencyRequest(const QString& requestFileName, const QString& responseFileName, QList<QDomElement>& removalAssertionList, QList<QDomElement>& additionAssertionList) {

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


			QDomElement retractAdditionsElement = requestDocument.createElement("Retract");
			retractAdditionsElement.setAttribute("kb",mReasonerTestKBName);
			for (QList<QDomElement>::const_iterator addIt = additionAssertionList.constBegin(), addItEnd = additionAssertionList.constEnd(); addIt != addItEnd; ++addIt) {
				QDomElement assertionElement(*addIt);
				QDomNode importedAssertionElement = requestDocument.importNode(assertionElement,true);
				retractAdditionsElement.appendChild(importedAssertionElement);
			}
			rootElement.appendChild(retractAdditionsElement);

			QDomElement reasonElement = getFirstReasoningRequestNode(requestDocument);
			rootElement.appendChild(reasonElement);




			QDomElement tellAdditionsElement = requestDocument.createElement("Tell");
			tellAdditionsElement.setAttribute("kb",mReasonerTestKBName);
			for (QList<QDomElement>::const_iterator addIt = additionAssertionList.constBegin(), addItEnd = additionAssertionList.constEnd(); addIt != addItEnd; ++addIt) {
				QDomElement assertionElement(*addIt);
				QDomNode importedAssertionElement = requestDocument.importNode(assertionElement,true);
				tellAdditionsElement.appendChild(importedAssertionElement);
			}
			rootElement.appendChild(tellAdditionsElement);

			QDomElement retractRemovalElement = requestDocument.createElement("Retract");
			retractRemovalElement.setAttribute("kb",mReasonerTestKBName);
			for (QList<QDomElement>::const_iterator remIt = removalAssertionList.constBegin(), remItEnd = removalAssertionList.constEnd(); remIt != remItEnd; ++remIt) {
				QDomElement assertionElement(*remIt);
				QDomNode importedAssertionElement = requestDocument.importNode(assertionElement,true);
				retractRemovalElement.appendChild(importedAssertionElement);
			}
			rootElement.appendChild(retractRemovalElement);


			QDomElement incReasonElement = getSecondReasoningRequestNode(requestDocument);
			rootElement.appendChild(incReasonElement);



			QDomElement releaseKBElement = requestDocument.createElement("ReleaseKB");
			releaseKBElement.setAttribute("kb",mReasonerTestKBName);
			rootElement.appendChild(releaseKBElement);

			QFile outFile(responseFileName);
			if (outFile.open(QIODevice::WriteOnly)) {
				outFile.write(requestDocument.toByteArray());
				outFile.close();
			}

			LOG(INFO,getLogDomain(),logTr("Generated OWLlink incremental assertion %3 request '%1' for ontology '%2'.").arg(responseFileName).arg(requestFileName).arg(mRequestTypeName),this);

			return false;
		}


	}; // end namespace Test

}; // end namespace Konclude
