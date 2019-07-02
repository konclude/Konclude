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

#ifndef KONCLUDE_PARSER_CXMLHANDLERSTACKBASE_H
#define KONCLUDE_PARSER_CXMLHANDLERSTACKBASE_H

// Libraries includes
#include <QDomDocument>
#include <QStack>
#include <QMap>
#include <QXmlDefaultHandler>

// Namespace includes
#include "ParserSettings.h"
#include "CXMLHandlerStack.h"
#include "CXMLHandler.h"

// Other includes
#include "Logger/CLogIdentifier.h"


// Logger includes
#include "Logger/CLogger.h"


using namespace Konclude::Logger;



namespace Konclude {


	namespace Parser {







		/*! 
		 *
		 *		\class		CXMLHandlerStack
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CXMLHandlerStackBase : public CXMLHandlerStack, public CXMLHandler, public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				CXMLHandlerStackBase();



				bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts, CParsingHandlingContext* handlingContext);
				bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName, CParsingHandlingContext* handlingContext);


				bool startDocument(CParsingHandlingContext* handlingContext);
				bool endDocument(CParsingHandlingContext* handlingContext);


				bool pushStackedHandler(CXMLHandler* handler, bool automaticallyPop, CParsingHandlingContext* handlingContext);
				bool popStackedHandler(CXMLHandler* handler, CParsingHandlingContext* handlingContext);

			// protected methods
			protected:


			// protected variables
			protected:
				class CXMLHandlerData {
					public:
						inline CXMLHandlerData(CXMLHandler* handler, bool automatPop, cint64 parseElementLevel) {
							mHandler = handler;
							mAutoPop = automatPop;
							mParseElementLevel = parseElementLevel;
							mStartDocumentCalled = false;
						}
						CXMLHandler* mHandler;
						bool mAutoPop;
						cint64 mParseElementLevel;
						bool mStartDocumentCalled;
				};

				QStack<CXMLHandlerData*> mHandlerDataStack;
				CXMLHandlerData* mCurrentHandlerData;

				cint64 mParseElementLevel;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CXMLHANDLERSTACKBASE_H
