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

#include "CQtHttpResponseHandler.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			CQtHttpResponseHandler::CQtHttpResponseHandler(CQtHttpRequest* request, CQtHttpResponse* response, cint64 downloadSizeLimit, CHttpTransactionManager* manager) {
				mRequest = request;
				mResponse = response;
				mManager = manager;
				mDownloadSizeLimit = downloadSizeLimit;
				mTotalBytesReceived = 0;
				mAborted = false;
			}

			CQtHttpRequest* CQtHttpResponseHandler::getRequest() {
				return mRequest;
			}

			CQtHttpResponse* CQtHttpResponseHandler::getResponse() {
				return mResponse;
			}

			void CQtHttpResponseHandler::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
				mTotalBytesReceived += bytesReceived;
				if (mTotalBytesReceived > mDownloadSizeLimit && !mAborted) {
					mAborted = true;
					mManager->abort(mRequest, mResponse);
				}
			}


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

