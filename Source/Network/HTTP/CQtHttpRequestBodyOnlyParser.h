/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_NETWORK_HTTP_CQTHTTPREQUESTBODYONLYPARSER_H
#define KONCLUDE_NETWORK_HTTP_CQTHTTPREQUESTBODYONLYPARSER_H

// Namespace includes
#include "HttpSettings.h"
#include "CHttpRequest.h"
#include "CQtHttpRequest.h"
#include "CQtHttpRequestParser.h"


// Library includes
#include <QIODevice>

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {


			/*! 
			*
			*		\class		CQtHttpRequestBodyOnlyParser
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			class CQtHttpRequestBodyOnlyParser : public CQtHttpRequestParser {

				// public methods
				public:
					//! Constructor
					CQtHttpRequestBodyOnlyParser();

					~CQtHttpRequestBodyOnlyParser();

					virtual CQtHttpRequestParser* reset();
					virtual bool readFromDevice(QIODevice* readDevice);

					virtual CHttpRequest* takeParsedHttpRequest();
					virtual bool hasHttpRequestParsingError();
					virtual bool hasHttpRequestParsingCompleted();
					virtual bool hasRequestedCloseConnection();
					virtual QString getHttpRequestParsingErrorString();

					virtual QByteArray* takeBodyData();


				// protected methods
				protected:
					void parseHttpRequestHeader();
					void parseHttpMethodProtocolURI();
					
					cint64 getNextCharacterPosition(char character, cint64 searchStartPos);


				// protected variables
				protected:
					char* mHeaderLineReadBuffer;
					cint64 mHeaderLineReadBufferSize;
					cint64 mHeaderLineReadBufferPosition;
					QByteArray mHeaderData;
					cint64 mReadHeaderDataSize;
					cint64 mMaxHeaderDataSize;
					QString mRequestURIString;
					QString mRequestMethod;
					QString mRequestProtocol;
					bool mHeaderMethodProtocolURIParsed;
					bool mHeaderCompleted;

					cint64 mExpectedBodyLength;
					QByteArray* mBodyData;
					cint64 mMaxBodyDataSize;
					cint64 mReadBodyDataSize;
					bool mRequestCompleted;
					bool mRequestError;

					QString mErrorString;

					QString mHeaderContentLengthString;
					QString mHeaderConnectionString;
					QString mHeaderConnectionCloseString;

					bool mAfterwardsCloseConnection;


				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude


#endif // KONCLUDE_NETWORK_HTTP_CQTHTTPREQUESTBODYONLYPARSER_H
