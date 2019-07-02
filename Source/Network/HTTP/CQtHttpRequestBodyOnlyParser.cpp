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

#include "CQtHttpRequestBodyOnlyParser.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			CQtHttpRequestBodyOnlyParser::CQtHttpRequestBodyOnlyParser() {
				mHeaderLineReadBufferSize = 20000;
				// maximum header size 10 KByte
				mMaxHeaderDataSize = 1024*10;
				mHeaderLineReadBuffer = new char[mHeaderLineReadBufferSize+1];
				mHeaderLineReadBuffer[mHeaderLineReadBufferSize] = '\0';
				// maximum body size 2 GByte
				mMaxBodyDataSize = Q_INT64_C(1024*1024*1024*2);
				mBodyData = nullptr;

				mHeaderContentLengthString = QString("Content-Length").toUpper();
				mHeaderConnectionString = QString("Connection").toUpper();
				mHeaderConnectionCloseString = QString("close").toUpper();

				reset();
			}

			CQtHttpRequestBodyOnlyParser::~CQtHttpRequestBodyOnlyParser() {
				delete mBodyData;
			}

			CQtHttpRequestParser* CQtHttpRequestBodyOnlyParser::reset() {
				mHeaderLineReadBufferPosition = 0;
				mReadBodyDataSize = 0;
				mErrorString.clear();
				mHeaderData.clear();
				delete mBodyData;
				mBodyData = nullptr;
				mRequestError = false;
				mRequestCompleted = false;
				mHeaderCompleted = false;
				mHeaderMethodProtocolURIParsed = false;
				mAfterwardsCloseConnection = false;
				mReadHeaderDataSize = 0;
				mExpectedBodyLength = 0;
				return this;
			}


			QByteArray* CQtHttpRequestBodyOnlyParser::takeBodyData() {
				QByteArray* bodyData = mBodyData;
				mBodyData = nullptr;
				return bodyData;
			}


			bool CQtHttpRequestBodyOnlyParser::readFromDevice(QIODevice* readDevice) {
				cint64 availableBytesCount = readDevice->bytesAvailable();
				while (availableBytesCount > 1 && !mRequestError && !mRequestCompleted) {
					if (!mHeaderCompleted) {
						if (mHeaderLineReadBufferSize - mHeaderLineReadBufferPosition <= 0) {
							mRequestError = true;
                            mErrorString = QString("Line in header is longer than allowed, '%1' is longer than %2").arg(QString::fromUtf8(mHeaderLineReadBuffer)).arg(mHeaderLineReadBufferSize);
						} else if (mMaxHeaderDataSize - mReadHeaderDataSize <= 0) {
							mRequestError = true;
							mErrorString = QString("Header is longer than allowed");
						} else {
							cint64 maxToReadLine = mHeaderLineReadBufferSize - mHeaderLineReadBufferPosition + 1;
							bool lineCompleted = false;
							cint64 maxToAvailableRead = qMin(readDevice->bytesAvailable(),maxToReadLine);
							maxToAvailableRead = qMin(maxToAvailableRead,mMaxHeaderDataSize-mReadHeaderDataSize+1);
							cint64 read = readDevice->readLine(&mHeaderLineReadBuffer[mHeaderLineReadBufferPosition],maxToAvailableRead);
							cint64 newPos = mHeaderLineReadBufferPosition+read;
							if (mHeaderLineReadBuffer[newPos-1] == '\n') {
								lineCompleted = true;
							}
							//if (newPos == 1 && mHeaderLineReadBuffer[newPos-1] == 13) {
							//	lineCompleted = true;
							//	if (mExpectedBodyLength == 0) {
							//		mRequestCompleted = true;
							//	}
							//}
							mHeaderLineReadBufferPosition = newPos;
							mReadHeaderDataSize += mHeaderLineReadBufferPosition;
							if (lineCompleted) {
								if (!mHeaderMethodProtocolURIParsed) {
									mHeaderMethodProtocolURIParsed = true;
									parseHttpMethodProtocolURI();
								} else {
									parseHttpRequestHeader();
								}
								mHeaderLineReadBufferPosition = 0;
							}
						}
					} else {
						if (mExpectedBodyLength <= 0) {
							mRequestCompleted = true;
						} else {
							if (mExpectedBodyLength > mMaxBodyDataSize) {
								mRequestError = true;
								mErrorString = QString("Expected body '%1' is longer than allowed '%2'").arg(mExpectedBodyLength).arg(mMaxBodyDataSize);
							} else {
								if (mBodyData == nullptr) {
									mBodyData = new QByteArray();
									mBodyData->resize(mExpectedBodyLength);
								}
								cint64 maxReadBodyData = mExpectedBodyLength - mReadBodyDataSize;
								maxReadBodyData = qMin(maxReadBodyData,mHeaderLineReadBufferSize);
								qint64 read = readDevice->read(mHeaderLineReadBuffer,maxReadBodyData);
								mBodyData->replace(mReadBodyDataSize,read,mHeaderLineReadBuffer, read);
								mReadBodyDataSize += read;

								if (mReadBodyDataSize >= mExpectedBodyLength) {
									mRequestCompleted = true;
								}
							}
						}
					}
					availableBytesCount = readDevice->bytesAvailable();
				}
				return !mRequestError;
			}

			cint64 CQtHttpRequestBodyOnlyParser::getNextCharacterPosition(char character, cint64 searchStartPos) {
				for (cint64 i = searchStartPos; i < mHeaderLineReadBufferSize; ++i) {
					if (mHeaderLineReadBuffer[i] == character) {
						return i;
					}
				}
				return mHeaderLineReadBufferSize;
			}

			void CQtHttpRequestBodyOnlyParser::parseHttpMethodProtocolURI() {
				cint64 currSpacePos = 0;
				cint64 nextSpacePos = getNextCharacterPosition(' ',currSpacePos);

				bool methodProtocolURIParseError = false;

				if (nextSpacePos == mHeaderLineReadBufferSize) {
					methodProtocolURIParseError = true;
				} else {
                    mRequestMethod = QString::fromUtf8(&mHeaderLineReadBuffer[currSpacePos],nextSpacePos-currSpacePos);
					currSpacePos = nextSpacePos+1;

					nextSpacePos = getNextCharacterPosition(' ',currSpacePos);
					if (nextSpacePos == mHeaderLineReadBufferSize) {
						methodProtocolURIParseError = true;
					} else {
                        mRequestURIString = QString::fromUtf8(&mHeaderLineReadBuffer[currSpacePos],nextSpacePos-currSpacePos);
						currSpacePos = nextSpacePos+1;

						nextSpacePos = mHeaderLineReadBufferPosition-2;
                        mRequestProtocol = QString::fromUtf8(&mHeaderLineReadBuffer[currSpacePos],nextSpacePos-currSpacePos);
					}
				}

				if (methodProtocolURIParseError) {
					mRequestError = true;
                    mErrorString = QString("Could not parse method, protocol and uri from request '%1'").arg(QString::fromUtf8(mHeaderLineReadBuffer));
				}
			}

			void CQtHttpRequestBodyOnlyParser::parseHttpRequestHeader() {
				if (mHeaderLineReadBufferPosition == 2) {
					// empty new line, end of header
					mHeaderCompleted = true;
				} else {
					cint64 doublePointPos = getNextCharacterPosition(':',0);
                    QString headerKey = QString::fromUtf8(mHeaderLineReadBuffer,doublePointPos);
					if (QString::compare(headerKey.trimmed(),mHeaderContentLengthString,Qt::CaseInsensitive) == 0) {
                        QString contentLengthValueString = QString::fromUtf8(&mHeaderLineReadBuffer[doublePointPos+1],mHeaderLineReadBufferPosition-3-doublePointPos);
						bool successfulParsedExpectedContentLength = false;
						QString trimmedContentLengthValueString = contentLengthValueString.trimmed();
						mExpectedBodyLength = trimmedContentLengthValueString.toLong(&successfulParsedExpectedContentLength);
						if (!successfulParsedExpectedContentLength) {
							mRequestError = true;
                            mErrorString = QString("Could not parse content length value from '%1'").arg(QString::fromUtf8(mHeaderLineReadBuffer));
						}
					} else {
						if (QString::compare(headerKey.trimmed(),mHeaderConnectionString,Qt::CaseInsensitive) == 0) {							
                            QString connectionValueString = QString::fromUtf8(&mHeaderLineReadBuffer[doublePointPos+1],mHeaderLineReadBufferPosition-3-doublePointPos);
							if (QString::compare(connectionValueString,mHeaderConnectionString,Qt::CaseInsensitive) == 0) {
								mAfterwardsCloseConnection = true;
							}
						}
					}
				}
			}


			CHttpRequest* CQtHttpRequestBodyOnlyParser::takeParsedHttpRequest() {
				CQtHttpRequest* request = new CQtHttpRequest(mRequestURIString,mBodyData);
				return request;
			}

			bool CQtHttpRequestBodyOnlyParser::hasHttpRequestParsingError() {
				return mRequestError;
			}

			bool CQtHttpRequestBodyOnlyParser::hasHttpRequestParsingCompleted() {
				return mRequestCompleted;
			}

			QString CQtHttpRequestBodyOnlyParser::getHttpRequestParsingErrorString() {
				return mErrorString;
			}
			
			bool CQtHttpRequestBodyOnlyParser::hasRequestedCloseConnection() {
				return mAfterwardsCloseConnection;
			}


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

