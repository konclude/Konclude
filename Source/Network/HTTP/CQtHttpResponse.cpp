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

#include "CQtHttpResponse.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			CQtHttpResponse::CQtHttpResponse(CQtHttpRequest* request, QNetworkReply* qNetworkReply) : mQNetworkReply(qNetworkReply) {
				mRequest = request;
				mDownloadSizeLimit = -1;
				mAborted = false;
				mAbortReason = 0;
			}

			CQtHttpResponse::~CQtHttpResponse() {
			}

			QNetworkReply* CQtHttpResponse::getQNetworkReply() {
				return mQNetworkReply;
			}


			CQtHttpRequest* CQtHttpResponse::getHttpRequest() {
				return mRequest;
			}

			CQtHttpResponse* CQtHttpResponse::setQNetworkReply(QNetworkReply* qNetworkReply) {
				if (mQNetworkReply) {
					delete mQNetworkReply;
				}
				mQNetworkReply = qNetworkReply;
				return this;
			}

			CCallbackRememberLinkedExecuter* CQtHttpResponse::getFinishedCallbackList() {
				return &finishCallbackList;
			}


			cint64 CQtHttpResponse::getDownloadSizeLimit() {
				return mDownloadSizeLimit;
			}

			CQtHttpResponse* CQtHttpResponse::setDownloadSizeLimit(cint64 limit) {
				mDownloadSizeLimit = limit;
				return this;
			}

			bool CQtHttpResponse::isAborted() {
				return mAborted;
			}

			cint64 CQtHttpResponse::getAbortReason() {
				return mAbortReason;
			}

			CQtHttpResponse* CQtHttpResponse::setAborted(cint64 abortReason) {
				mAborted = true;
				mAbortReason = abortReason;
				return this;
			}

		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

