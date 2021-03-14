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

				COntologyRevisionDirectoryOWL2XMLFilePersistencer::COntologyRevisionDirectoryOWL2XMLFilePersistencer(CConfiguration* config) : COntologyRevisionDirectoryFilePersistencerBase(config) {
					mConfEnforceUTF8ConvertingParsing = CConfigDataReader::readConfigBoolean(mConfig, "Konclude.Parser.UTF8CompatibilityEnforcedXMLStreamParsing");
					QString confOWL2XMLBaseDirectory = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.DirectoryOWL2XMLFilePersistencer.BaseDirectory", "./Data/Ontologies/Persistence/");
					if (!confOWL2XMLBaseDirectory.isEmpty()) {
						mConfBaseDirectory = confOWL2XMLBaseDirectory;
					}
					mFileEndingWritingName = CConfigDataReader::readConfigString(mConfig, "Konclude.Persistence.DirectoryOWLRDFFilePersistencer.WritingFileEnding");
			}

				COntologyRevisionDirectoryOWL2XMLFilePersistencer::~COntologyRevisionDirectoryOWL2XMLFilePersistencer() {
				}



				bool COntologyRevisionDirectoryOWL2XMLFilePersistencer::writeOntologyToFile(QFile* file, COntologyRevision* ontoRev) {

					CConcreteOntology* ontology = ontoRev->getOntology();

					COntologyAxiomExpressionsRenderVisitor renderer;
					COntologyOWL2QtXMLRenderer owlXMLRenderer("");

					CONTOLOGYAXIOMSET<CAxiomExpression*>* currAxiomSet = ontology->getBuildData()->getTellAxiomSet();
					CQtList<CAxiomExpression*> axiomList;
					for (CAxiomExpression* axiomExp : *currAxiomSet) {
						axiomList.append(axiomExp);
					}

					renderer.visitAndRender(axiomList, ontology, &owlXMLRenderer);

					QList<CTriplesData*> triplesDataList;
					COntologyTriplesData* ontTriplesData = ontoRev->getOntology()->getOntologyTriplesData();
					if (ontTriplesData) {
						for (CTriplesData* tripleData : *ontTriplesData->getAllTriplesData()) {
							triplesDataList.append(tripleData);
						}
					}

					renderer.visitAndRender(&triplesDataList, ontology, &owlXMLRenderer);



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

					cint64 written = file->write(byteArray);
					if (written == byteArray.size()) {
						return true;
					}

					return false;
				}







			}; // end namespace Persistence

		}; // end namespace Revision

	}; // end namespace Reasoner

}; // end namespace Konclude
