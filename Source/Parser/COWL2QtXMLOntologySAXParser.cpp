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

#include "COWL2QtXMLOntologySAXParser.h"


namespace Konclude {

	namespace Parser {



		COWL2QtXMLOntologySAXParser::COWL2QtXMLOntologySAXParser(COntologyBuilder* ontologyBuilder) {
			mOntologyBuilder = ontologyBuilder;

			mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider();
			mMemoryPoolAllocationMan = new CMemoryPoolAllocationManager(mMemoryPoolProvider);
			mContextBase = new CContextBase(mMemoryPoolAllocationMan);

			mHandlerStack = new CXMLHandlerStackBase();
			mOWLXMLStackHandler = new CXMLOWL2Handler(mOntologyBuilder);
			mParsingHandlingContext = new CXMLParsingHandlingContextBase(mHandlerStack,mMemoryPoolAllocationMan);
			mHandlerStack->pushStackedHandler(mOWLXMLStackHandler,true,mParsingHandlingContext);
		}


		COWL2QtXMLOntologySAXParser::~COWL2QtXMLOntologySAXParser() {
			delete mMemoryPoolAllocationMan;
			delete mMemoryPoolProvider;
			delete mHandlerStack;
			delete mOWLXMLStackHandler;
			delete mParsingHandlingContext;
			delete mContextBase;
		}


		bool COWL2QtXMLOntologySAXParser::parseOntology(const QString& fileString) {
			QFile file(fileString);
			bool succParsed = false;
			if (file.open(QIODevice::ReadOnly)) {
				succParsed = parseOntology(&file);
				file.close();
			}
			return succParsed;
		}

		bool COWL2QtXMLOntologySAXParser::parseOntology(QIODevice* ioDevice) {
			//QHash<QStringRef,QString> testHash;
			//QString testString("test");
			//testHash.insert(testString.leftRef(-1),"test");

			QXmlSimpleReader xmlReader;
			QXmlInputSource* source = new QXmlInputSource(ioDevice);

			CQtXMLContentHandlerConverter* xmlConHanCon = new CQtXMLContentHandlerConverter(mHandlerStack,mParsingHandlingContext);
			xmlReader.setContentHandler(xmlConHanCon);
			xmlReader.setErrorHandler(xmlConHanCon);
			xmlReader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);

			bool succParsed = xmlReader.parse(source);

			delete xmlConHanCon;
			delete source;
			return succParsed;

			//QStringRef baseNameRef;

			//QXmlStreamReader xmlReader(ioDevice);
			//while (!xmlReader.atEnd()) {
			//	QXmlStreamReader::TokenType token(xmlReader.readNext());
			//	 if (token == QXmlStreamReader::StartElement) {
			//		 QStringRef nameRef(xmlReader.name());
			//		 baseNameRef = nameRef;
			//	 }
			//}
			//return true;
		}


	}; // end namespace Parser

}; // end namespace Konclude
