/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CXMLHandlerStackBase.h"


namespace Konclude {

	namespace Parser {



		CXMLHandlerStackBase::CXMLHandlerStackBase() : CLogIdentifier("::Konclude::Parser::XMLHandlerStackBase",this) {
			mCurrentHandlerData = nullptr;
			mParseElementLevel = 0;
		}

		bool CXMLHandlerStackBase::pushStackedHandler(CXMLHandler* handler, bool automaticallyPop, CParsingHandlingContext* handlingContext) {
			if (handler) {
				CXMLHandlerData* newHandlerData = new CXMLHandlerData(handler,automaticallyPop,mParseElementLevel);
				mHandlerDataStack.push(newHandlerData);
				mCurrentHandlerData = newHandlerData;
				return true;
			}
			return false;
		}

		bool CXMLHandlerStackBase::popStackedHandler(CXMLHandler* handler, CParsingHandlingContext* handlingContext) {
			if (!mCurrentHandlerData || mCurrentHandlerData->mHandler != handler) {
				return false;
			}
			mHandlerDataStack.pop();
			mCurrentHandlerData = nullptr;
			if (mHandlerDataStack.count() > 0) {
				mCurrentHandlerData = mHandlerDataStack.top();
			}
			return true;
		}


		bool CXMLHandlerStackBase::startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts, CParsingHandlingContext* handlingContext) {
			++mParseElementLevel;
			if (mCurrentHandlerData) {
				if (!mCurrentHandlerData->mStartDocumentCalled) {
					mCurrentHandlerData->mStartDocumentCalled = true;
					mCurrentHandlerData->mHandler->startDocument(handlingContext);
				}
				return mCurrentHandlerData->mHandler->startElement(namespaceURI,localName,qName,atts,handlingContext);
			}
			return false;
		}

		bool CXMLHandlerStackBase::endElement(const QString& namespaceURI, const QString& localName, const QString& qName, CParsingHandlingContext* handlingContext) {
			--mParseElementLevel;
			if (mCurrentHandlerData) {
				if (mCurrentHandlerData->mParseElementLevel > mParseElementLevel) {
					bool autoPop = mCurrentHandlerData->mAutoPop;
					mCurrentHandlerData->mHandler->endDocument(handlingContext);
					if (autoPop) {
						popStackedHandler(mCurrentHandlerData->mHandler,handlingContext);
					}
				}
			}

			if (mCurrentHandlerData) {
				return mCurrentHandlerData->mHandler->endElement(namespaceURI,localName,qName,handlingContext);
			}

			return false;
		}


		bool CXMLHandlerStackBase::startDocument(CParsingHandlingContext* handlingContext) {
			return true;
		}

		bool CXMLHandlerStackBase::endDocument(CParsingHandlingContext* handlingContext) {
			while (mCurrentHandlerData) {
				bool autoPop = mCurrentHandlerData->mAutoPop;
				mCurrentHandlerData->mHandler->endDocument(handlingContext);
				if (autoPop) {
					popStackedHandler(mCurrentHandlerData->mHandler,handlingContext);
				}
			}
			return true;
		}



	}; // end namespace Parser

}; // end namespace Konclude
