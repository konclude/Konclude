/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "COWLFileOWLlinkClassifyRequestGenerator.h"


namespace Konclude {

	namespace Test {



		COWLFileOWLlinkClassifyRequestGenerator::COWLFileOWLlinkClassifyRequestGenerator(CConfiguration *config) 
					: CLogIdentifier("::Konclude::Test::OWLFileOWLlinkClassifyRequestGenerator",this),mConfig(config) {


			mReasonerTestKBName = "http://www.konclude.com/evaluation-testsuite/test-ontology";
		}


		COWLFileOWLlinkClassifyRequestGenerator::~COWLFileOWLlinkClassifyRequestGenerator() {
		}



		QDomDocument COWLFileOWLlinkClassifyRequestGenerator::createRequestDocument() {
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


		bool COWLFileOWLlinkClassifyRequestGenerator::generateOWLlinkClassifyRequest(const QString& requestFileName, const QString& responseFileName) {

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

			QDomElement getClassifyElement = requestDocument.createElement("Classify");
			getClassifyElement.setAttribute("kb",mReasonerTestKBName);
			rootElement.appendChild(getClassifyElement);

			QDomElement getSubClassHirElement = requestDocument.createElement("GetSubClassHierarchy");
			getSubClassHirElement.setAttribute("kb",mReasonerTestKBName);
			rootElement.appendChild(getSubClassHirElement);

			QDomElement releaseKBElement = requestDocument.createElement("ReleaseKB");
			releaseKBElement.setAttribute("kb",mReasonerTestKBName);
			rootElement.appendChild(releaseKBElement);

			QFile outFile(responseFileName);
			if (outFile.open(QIODevice::WriteOnly)) {
				outFile.write(requestDocument.toByteArray());
				outFile.close();
			}

			LOG(INFO,getLogDomain(),logTr("Generated OWLlink classify request '%1' for ontology '%2'.").arg(responseFileName).arg(requestFileName),this);

			return false;
		}


	}; // end namespace Test

}; // end namespace Konclude
