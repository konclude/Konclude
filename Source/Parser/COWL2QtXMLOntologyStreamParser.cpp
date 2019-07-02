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

#include "COWL2QtXMLOntologyStreamParser.h"


namespace Konclude {

	namespace Parser {



		COWL2QtXMLOntologyStreamParser::COWL2QtXMLOntologyStreamParser(COntologyBuilder* ontologyBuilder) {
			mOntologyBuilder = ontologyBuilder;
			mOWLXMLStreamHandler = new CXMLOWL2StreamHandler(ontologyBuilder);
		}


		COWL2QtXMLOntologyStreamParser::~COWL2QtXMLOntologyStreamParser() {
			delete mOWLXMLStreamHandler;
		}


		bool COWL2QtXMLOntologyStreamParser::parseOntology(const QString& fileString) {
			QFile file(fileString);
			bool succParsed = false;
			if (file.open(QIODevice::ReadOnly)) {
				succParsed = parseOntology(&file);
				file.close();
			}
			return succParsed;
		}

		bool COWL2QtXMLOntologyStreamParser::parseOntology(QIODevice* ioDevice) {
			QXmlStreamReader xmlReader(ioDevice);
			while (!xmlReader.atEnd()) {
				QXmlStreamReader::TokenType token(xmlReader.readNext());
				if (token == QXmlStreamReader::StartDocument) {
					mOWLXMLStreamHandler->startDocument();
				} else if (token == QXmlStreamReader::StartElement) {
					mOWLXMLStreamHandler->startElement(xmlReader.namespaceUri(),xmlReader.name(),xmlReader.qualifiedName(),xmlReader.attributes());
				} else if (token == QXmlStreamReader::EndElement) {
					mOWLXMLStreamHandler->endElement(xmlReader.namespaceUri(),xmlReader.name(),xmlReader.qualifiedName());
				} else if (token == QXmlStreamReader::StartDocument) {
					mOWLXMLStreamHandler->endDocument();
				}
			}
			if (xmlReader.hasError()) {
				LOG(ERROR,"::Konclude::Parser::OWL2QtXMLStreamOntologyParser",logTr("XML parsing error: %1").arg(xmlReader.errorString()),this);
				return false;
			}
			return true;
		}


	}; // end namespace Parser

}; // end namespace Konclude
