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

#include "COWL2QtXMLOntologyStableStreamParser.h"


namespace Konclude {

	namespace Parser {



		COWL2QtXMLOntologyStableStreamParser::COWL2QtXMLOntologyStableStreamParser(COntologyBuilder* ontologyBuilder) : COWL2QtXMLOntologyStreamParser(ontologyBuilder) {
			mReadBufferSize = 10000;
			mReadBuffer = new char[mReadBufferSize];
		}


		COWL2QtXMLOntologyStableStreamParser::~COWL2QtXMLOntologyStableStreamParser() {
			delete [] mReadBuffer;
		}



		bool COWL2QtXMLOntologyStableStreamParser::addMoreDataToXMLReader(QXmlStreamReader* xmlReader, QIODevice* ioDevice) {
			if (!ioDevice->atEnd()) {

				cint64 remainTmpSize = 10;
				cint64 readDataSize = ioDevice->read(mReadBuffer,mReadBufferSize-remainTmpSize);
				mReadBuffer[readDataSize] = 0;
				bool usesUnicodeChars = false;
				if (readDataSize > 0) {
					while (!ioDevice->atEnd() && mReadBuffer[readDataSize-1] > 127 && remainTmpSize > 0) {
						--remainTmpSize;
						usesUnicodeChars = true;
						cint64 newReadSize = ioDevice->read(&mReadBuffer[readDataSize],1);
						readDataSize = readDataSize+newReadSize;
						mReadBuffer[readDataSize] = 0;
					}
				}

				if (!usesUnicodeChars) {
					for (cint64 i = 0; !usesUnicodeChars && i < readDataSize; ++i) {
						if (mReadBuffer[readDataSize] > 127) {
							usesUnicodeChars = true;
						}
					}
				}

				if (usesUnicodeChars) {
					QString dataString = QString::fromUtf8(mReadBuffer,readDataSize);
					xmlReader->addData(dataString);
				} else {
					QByteArray dataByteArray = QByteArray::fromRawData(mReadBuffer,readDataSize);
					xmlReader->addData(dataByteArray);
				}

				return true;
			}
			return false;
		}


		bool COWL2QtXMLOntologyStableStreamParser::parseOntology(QIODevice* ioDevice) {
			QXmlStreamReader xmlReader;
			addMoreDataToXMLReader(&xmlReader,ioDevice);
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
				} else if (token == QXmlStreamReader::Invalid) {
					if (xmlReader.error() == QXmlStreamReader::PrematureEndOfDocumentError) {
						addMoreDataToXMLReader(&xmlReader,ioDevice);
					}
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


	}; // end namespace Parser

}; // end namespace Konclude
