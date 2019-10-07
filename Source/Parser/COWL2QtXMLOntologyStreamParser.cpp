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

#include "COWL2QtXMLOntologyStreamParser.h"


namespace Konclude {

	namespace Parser {



		COWL2QtXMLOntologyStreamParser::COWL2QtXMLOntologyStreamParser(COntologyBuilder* ontologyBuilder) {
			mOntologyBuilder = ontologyBuilder;
			mOWLXMLStreamHandler = new CXMLOWL2StreamHandler(ontologyBuilder);

			mError = false;
		}


		COWL2QtXMLOntologyStreamParser::~COWL2QtXMLOntologyStreamParser() {
			delete mOWLXMLStreamHandler;
		}


		bool COWL2QtXMLOntologyStreamParser::parseOntology(const QString& fileString) {
			QFile file(fileString);
			mError = false;
			if (file.open(QIODevice::ReadOnly)) {
				mError = !parseOntology(&file);
				file.close();
			} else {
				mErrorString = QString("File '%1' does not exist or cannot be opened.").arg(fileString);
				mError = true;
			}
			return !mError;
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
				} else if (token == QXmlStreamReader::Characters) {
					mOWLXMLStreamHandler->readText(xmlReader.text());
				} else if (token == QXmlStreamReader::StartDocument) {
					mOWLXMLStreamHandler->endDocument();
				}
			}
			if (xmlReader.hasError()) {
				mErrorString = QString("XML parsing error at %2:%3: '%1'.").arg(xmlReader.errorString()).arg(xmlReader.lineNumber()).arg(xmlReader.columnNumber());
				mError = true;
				LOG(ERROR,"::Konclude::Parser::OWL2QtXMLStreamOntologyParser",mErrorString,this);
			} else if (!mOWLXMLStreamHandler->hasOntologyNodeFound()) {				
				mErrorString = QString("OWL2/XML Ontology node not found.");
				mError = true;
				LOG(ERROR,"::Konclude::Parser::OWL2QtXMLStreamOntologyParser",mErrorString,this);
			}
			return !mError;
		}

		QString COWL2QtXMLOntologyStreamParser::getErrorString() {
			return mErrorString;
		}

		bool COWL2QtXMLOntologyStreamParser::hasError() {
			return mError;
		}

	}; // end namespace Parser

}; // end namespace Konclude
