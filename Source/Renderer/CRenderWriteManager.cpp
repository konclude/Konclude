/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CRenderWriteManager.h"


namespace Konclude {

	namespace Renderer {


		CRenderWriteManager::CRenderWriteManager() : CLogIdentifier("::Konclude::Renderer::RenderWriteManager",this) {
		}

		bool CRenderWriteManager::realizeCommand(CCommand* command) {
			if (dynamic_cast<CWriteOntologyCommand*>(command)) {
				if (dynamic_cast<CWriteOWL2XMLOntologyCommand*>(command)) {
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					CWriteOWL2XMLOntologyCommand* writeOWL2XMLOntC = (CWriteOWL2XMLOntologyCommand*)command;
					COntologyOWL2QtXMLRenderer* renderer = writeOWL2XMLOntC->getOWL2XMLRenderer();
					QString writeFileName = writeOWL2XMLOntC->getWriteFileName();
					QFile file(writeFileName);

					if (file.open(QIODevice::WriteOnly)) {

						QString owlNS = "http://www.w3.org/2002/07/owl#";

						QDomDocument document;
						QDomElement rootNode = document.createElementNS(owlNS,"Ontology");
						rootNode.setAttribute("xmlns:owl",owlNS);
						renderer->addRenderedAxiomsToDomElement(rootNode);
						document.appendChild(rootNode);

						file.write(document.toByteArray());
						file.close();
					}

					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
					return true;
				} else if (dynamic_cast<CWriteOWLlinkOWL2XMLSatisfiableTestCommand*>(command)) {
					CCommandRecordRouter commandRecordRouter(command,this);
					CStartProcessCommandRecord::makeRecord(&commandRecordRouter);

					CWriteOWLlinkOWL2XMLSatisfiableTestCommand* writeOWL2XMLOntC = (CWriteOWLlinkOWL2XMLSatisfiableTestCommand*)command;
					COntologyOWL2QtXMLRenderer* renderer = writeOWL2XMLOntC->getOWL2XMLRenderer();
					QString writeFileName = writeOWL2XMLOntC->getWriteFileName();
					QString destKBName = writeOWL2XMLOntC->getTestKBName();
					QStringList testClassNames = writeOWL2XMLOntC->getSatisfiableTestClassesList();
					QFile file(writeFileName);

					if (file.open(QIODevice::WriteOnly)) {

						QString owlNS = "http://www.w3.org/2002/07/owl#";
						QString owllinkNS = "http://www.owllink.org/owllink#";

						QDomDocument document;
						QDomProcessingInstruction header = document.createProcessingInstruction("xml","version=\"1.0\"");
						document.appendChild(header);
						QDomElement rootNode = document.createElementNS(owllinkNS,"RequestMessage");
						rootNode.setAttributeNS("http://www.w3.org/2001/XMLSchema-instance","xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
						rootNode.setAttribute("xmlns:owl",owlNS);
						document.appendChild(rootNode);


						QDomElement createOntologyNode = document.createElement("CreateKB");
						createOntologyNode.setAttribute(QString("kb"),QString(destKBName));
						rootNode.appendChild(createOntologyNode);

						QDomElement tellOntologyNode = document.createElement("Tell");
						tellOntologyNode.setAttribute(QString("kb"),QString(destKBName));
						renderer->addRenderedAxiomsToDomElement(tellOntologyNode);
						rootNode.appendChild(tellOntologyNode);


						foreach (QString testClass, testClassNames) {
							QDomElement queryOntologyNode = document.createElement("IsClassSatisfiable");
							queryOntologyNode.setAttribute(QString("kb"),QString(destKBName));
							queryOntologyNode.setAttribute(QString("timing"),QString("simple"));
							QDomElement classEl = document.createElement("owl:Class");
							classEl.setAttribute("IRI",testClass);
							queryOntologyNode.appendChild(classEl);
							rootNode.appendChild(queryOntologyNode);
						}

						QDomElement releaseOntologyNode = document.createElement("ReleaseKB");
						releaseOntologyNode.setAttribute(QString("kb"),QString(destKBName));
						rootNode.appendChild(releaseOntologyNode);

						document.appendChild(rootNode);

						file.write(document.toByteArray());
						file.close();
					}

					CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
					return true;
				}
			}
			return false;
		}

	}; // end namespace Renderer

}; // end namespace Konclude
