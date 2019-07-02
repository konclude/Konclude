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

#include "CQtXMLContentHandlerConverter.h"


namespace Konclude {

	namespace Parser {



		CQtXMLContentHandlerConverter::CQtXMLContentHandlerConverter(CXMLHandlerStackBase* handlerStack, CXMLParsingHandlingContext* parsingHandlingContext) {
			mHandlerStack = handlerStack;
			mParsingHandlingContext = parsingHandlingContext;
		}


		bool CQtXMLContentHandlerConverter::endElement(const QString& namespaceURI, const QString& localName, const QString& qName) {
			return mHandlerStack->endElement(namespaceURI,localName,qName,mParsingHandlingContext);
			return true;
		}

		bool CQtXMLContentHandlerConverter::startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts) {
			return mHandlerStack->startElement(namespaceURI,localName,qName,atts,mParsingHandlingContext);
			return true;
		}

		bool CQtXMLContentHandlerConverter::startDocument() {
			return mHandlerStack->startDocument(mParsingHandlingContext);
		}

		bool CQtXMLContentHandlerConverter::endDocument() {
			return mHandlerStack->endDocument(mParsingHandlingContext);
		}



	}; // end namespace Parser

}; // end namespace Konclude
