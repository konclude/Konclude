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

#include "CQtHttpRequest.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			CQtHttpRequest::CQtHttpRequest(const QUrl &url) : mQNetworkRequest(url),mByteArrayData(nullptr) {
			}


			CQtHttpRequest::CQtHttpRequest(const QUrl &url, QByteArray* data) : mQNetworkRequest(url),mByteArrayData(data) {
				mQNetworkRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/octet-stream"));
			}

			CQtHttpRequest::~CQtHttpRequest() {
				delete mByteArrayData;
			}

			QNetworkRequest* CQtHttpRequest::getQNetworkRequest() {
				return &mQNetworkRequest;
			}


			QByteArray* CQtHttpRequest::getByteArrayData() {
				return mByteArrayData;
			}


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

