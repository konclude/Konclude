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

#include "COntologyRevisionDirectoryOWL2XMLFilePersistencer.h"


namespace Konclude {

	namespace Reasoner {

		namespace Revision {

			namespace Persistence {

				COntologyRevisionDirectoryOWL2XMLFilePersistencer::COntologyRevisionDirectoryOWL2XMLFilePersistencer(CConfiguration* config) : CLogDomain("::Konclude::Reasoner::Persistence::DirectoryOWL2XMLFilePersistencer") {
					mConfig = config;
					mOWLNS = "http://www.w3.org/2002/07/owl#";
					mConfBaseDirectory = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.DirectoryOWL2XMLFilePersistencer.BaseDirectory","./Data/Ontologies/Persistence/");
					mConfEnforceUTF8ConvertingParsing = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Parser.UTF8CompatibilityEnforcedXMLStreamParsing");

				}

				COntologyRevisionDirectoryOWL2XMLFilePersistencer::~COntologyRevisionDirectoryOWL2XMLFilePersistencer() {
				}




				QString COntologyRevisionDirectoryOWL2XMLFilePersistencer::getNextPersistingOntologyRevisionString(const QString& ontologyName, const QStringList& pathSepStrings, COntologyRevision* ontoRev) {
					QString lastVersionFileNameString = mOntologyNameLastFileHash.value(ontologyName);

					QString dirName = pathSepStrings.join('/');
					QString fileNameStart = pathSepStrings.join('_') + ".";
					QString fileNameEnd = ".owl.xml";

					lastVersionFileNameString.remove(0,fileNameStart.length());
					lastVersionFileNameString.remove(fileNameEnd);
					lastVersionFileNameString.remove("R");
					cint64 lastVersion = lastVersionFileNameString.toLongLong();

					cint64 newVersion = lastVersion+1;

					QString newVersionString = QString("R%1").arg(newVersion);
					return newVersionString;
				}



				bool COntologyRevisionDirectoryOWL2XMLFilePersistencer::persistOntologyRevision(COntologyRevision* ontoRev, COntologyRevision* prevOntoRev) {


					if (mOntologyNameFileHash.contains(prevOntoRev) && ontoRev->getOntology()->getBuildData()->getChangeAxiomList()->size() == prevOntoRev->getOntology()->getBuildData()->getChangeAxiomList()->size()) {
						mOntologyNameFileHash.insert(ontoRev, mOntologyNameFileHash.value(prevOntoRev));
						return true;
					}

					CConcreteOntology* ontology = ontoRev->getOntology();
					QString ontName = ontology->getOntologyName();
					cint64 ontoVers = ontoRev->getOntologyRevisionDepth();
					
					QStringList pathSepStrings = getOntologyFileNameParts(ontName);

					QString dirName = pathSepStrings.join('/');
					QString revName = getNextPersistingOntologyRevisionString(ontName,pathSepStrings,ontoRev);
					QString fileName = pathSepStrings.join('_') + "." + revName;
					QString owlFileName = fileName + ".owl.xml";
					QString tmpFileName = fileName + "-tmp-XXXXXX";

					if (QFile::exists(QString("%1%2/%3").arg(mConfBaseDirectory).arg(dirName).arg(owlFileName))) {
						return false;
					}

					COntologyAxiomExpressionsRenderVisitor renderer;
					COntologyOWL2QtXMLRenderer owlXMLRenderer("");

					CONTOLOGYAXIOMSET<CAxiomExpression*>* currAxiomSet = ontology->getBuildData()->getTellAxiomSet();
					CQtList<CAxiomExpression*> axiomList;
					for (CAxiomExpression* axiomExp : *currAxiomSet) {
						axiomList.append(axiomExp);
					}

					renderer.visitAndRender(axiomList, ontology, &owlXMLRenderer);


					QDomDocument document;
					QDomProcessingInstruction procInstHeader = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
					document.appendChild(procInstHeader);
					QDomElement rootNode = document.createElementNS(mOWLNS, "Ontology");
					rootNode.setAttribute("xmlns:owl", mOWLNS);
					rootNode.setAttribute("xml:base", mOWLNS);
					rootNode.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema#");
					rootNode.setAttribute("xmlns:rdfs", "http://www.w3.org/2000/01/rdf-schema#");
					rootNode.setAttribute("xmlns:rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
					rootNode.setAttribute("xmlns:xml", "http://www.w3.org/XML/1998/namespace");
					document.appendChild(rootNode);

					//<Ontology xmlns = "http://www.w3.org/2002/07/owl#"
					//	xml:base = "http://LUBM-1"
					//	xmlns : rdfs = "http://www.w3.org/2000/01/rdf-schema#"
					//	xmlns : xsd = "http://www.w3.org/2001/XMLSchema#"
					//	xmlns : rdf = "http://www.w3.org/1999/02/22-rdf-syntax-ns#"
					//	xmlns : xml = "http://www.w3.org/XML/1998/namespace"
					//	ontologyIRI = "http://LUBM-1">
					//	<Prefix name = "rdf" IRI = "http://www.w3.org/1999/02/22-rdf-syntax-ns#" / >
					//	<Prefix name = "rdfs" IRI = "http://www.w3.org/2000/01/rdf-schema#" / >
					//	<Prefix name = "xsd" IRI = "http://www.w3.org/2001/XMLSchema#" / >
					//	<Prefix name = "owl" IRI = "http://www.w3.org/2002/07/owl#" / >
					owlXMLRenderer.addRenderedAxiomsToDomElement(rootNode);

					const QByteArray& byteArray = document.toByteArray();

					bool successfullyWritten = false;
					QDir baseDir(mConfBaseDirectory);
					baseDir.mkpath(dirName);
					QTemporaryFile tmpFile(QString("%1%2/%3").arg(mConfBaseDirectory).arg(dirName).arg(tmpFileName));
					tmpFile.setAutoRemove(false);
					if (tmpFile.open()) {
						cint64 written = tmpFile.write(byteArray);
						tmpFile.close();
						if (written == byteArray.size()) {
							successfullyWritten = true;
						}
					}

					if (successfullyWritten) {
						successfullyWritten = tmpFile.rename(QString("%1%2/%3").arg(mConfBaseDirectory).arg(dirName).arg(owlFileName));
						if (!successfullyWritten) {
							tmpFile.remove();
						} else {
							mOntologyNameFileHash.insert(ontoRev, owlFileName);
							mOntologyNameLastFileHash.insert(ontName, owlFileName);
						}
					}

					return successfullyWritten;
				}




				bool COntologyRevisionDirectoryOWL2XMLFilePersistencer::isOntologyFileNameNewer(const QString& currentOntologyName, const QString& newOntologyName) {
					if (currentOntologyName.length() > newOntologyName.length()) {
						return false;
					}
					if (currentOntologyName.length() < newOntologyName.length()) {
						return true;
					}
					int comp = QString::compare(currentOntologyName, newOntologyName, Qt::CaseInsensitive) < 0;
					return comp;
				}




				bool COntologyRevisionDirectoryOWL2XMLFilePersistencer::hasNewOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {

					QString lastVersionFileNameString = mOntologyNameFileHash.value(ontoRev);
					QString lastFileNameString = mOntologyNameLastFileHash.value(ontologyName);
					if (lastVersionFileNameString != lastFileNameString) {
						return true;
					}

					QStringList pathSepStrings = getOntologyFileNameParts(ontologyName);


					QString dirName = pathSepStrings.join('/');
					QString fileName = pathSepStrings.join('_');
					QString owlFileNameFilter = fileName + "*.owl.xml";


					QStringList nameFilter(owlFileNameFilter);
					QDir directory(mConfBaseDirectory+dirName);
					QStringList ontoFiles = directory.entryList(nameFilter,QDir::Files);


					QString newFileNameString = lastFileNameString;
					for (const QString& fileName : ontoFiles) {
						if (isOntologyFileNameNewer(lastFileNameString,fileName)) {
							newFileNameString = fileName;
						}
					}
					if (newFileNameString != lastFileNameString) {
						mOntologyNameLastFileHash.insert(ontologyName,newFileNameString);
						return true;
					}
					return false;
				}



				bool COntologyRevisionDirectoryOWL2XMLFilePersistencer::loadLatestOntologyRevision(const QString& ontologyName, COntologyRevision* ontoRev) {


					if (hasNewOntologyRevision(ontologyName,ontoRev)) {

						QStringList pathSepStrings = getOntologyFileNameParts(ontologyName);
						QString dirName = pathSepStrings.join('/');

						QString lastVersionFileNameString = mOntologyNameLastFileHash.value(ontologyName);

						CConcreteOntologyUpdateCollectorBuilder *builder = new CConcreteOntologyUpdateCollectorBuilder(ontoRev->getOntology());
						builder->initializeBuilding();

						bool parsingSucceeded = false;

						QFile ontoVersionFile(mConfBaseDirectory + dirName + "/" + lastVersionFileNameString);

						COWL2QtXMLOntologyStreamParser *owl2Parser = nullptr;
						if (mConfEnforceUTF8ConvertingParsing) {
							owl2Parser = new COWL2QtXMLOntologyStableStreamParser(builder);
						} else {
							owl2Parser = new COWL2QtXMLOntologyStreamParser(builder);
						}
						LOG(INFO, getDomain(), logTr("Trying loading last revision of ontology '%1' from file '%2'").arg(ontologyName).arg(lastVersionFileNameString), this);
						if (ontoVersionFile.open(QIODevice::ReadOnly)) {
							if (owl2Parser->parseOntology(&ontoVersionFile)) {
								builder->completeBuilding();
								parsingSucceeded = true;
								mOntologyNameFileHash.insert(ontoRev,lastVersionFileNameString);
								LOG(INFO, getDomain(), logTr("Finished stream-based OWL2/XML ontology parsing for '%1'.").arg(lastVersionFileNameString), this);
							} else {
								if (owl2Parser->hasError()) {
									LOG(ERROR, getDomain(), logTr("Stream-based OWL2/XML ontology parsing error: %1").arg(owl2Parser->getErrorString()), this);
								}
								LOG(ERROR, getDomain(), logTr("Stream-based OWL2/XML ontology parsing for '%1' failed.").arg(lastVersionFileNameString), this);
							}
							ontoVersionFile.close();
						} else {
							LOG(ERROR, getDomain(), logTr("File '%1' cannot be opened.").arg(lastVersionFileNameString), this);
						}

						delete owl2Parser;
						delete builder;


						return parsingSucceeded;
					} else {
						return false;
					}
				}


				QStringList COntologyRevisionDirectoryOWL2XMLFilePersistencer::getOntologyFileNameParts(const QString& ontologyName) {
					QStringList pathSepStrings;
					QString currPartString;
					for (cint64 i = 0; i < ontologyName.length(); ++i) {
						QChar partChar = ontologyName.at(i);
						bool separator = false;
						if (partChar == ':' || partChar == '/' || partChar == '\\' || partChar == '?' || partChar == '*' || partChar == '%' || partChar == '$' || partChar == '<' || partChar == '>' || partChar == '|' || partChar == '\"') {
							separator = true;
						}
						if (separator) {
							if (!currPartString.isEmpty()) {
								pathSepStrings += currPartString;
							}
							currPartString = QString();
						} else {
							currPartString += partChar;
						}
					}
					if (!currPartString.isEmpty()) {
						pathSepStrings += currPartString;
					}
					return pathSepStrings;
				}



			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude
